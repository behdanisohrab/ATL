#define _LARGEFILE64_SOURCE
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

struct AAssetManager {
	char dummy;
};

struct AAssetManager dummy_asset_manager;

struct AAsset{
	int fd;
	off64_t read;
};

struct AAssetDir {
	DIR *dp;
	const char *dirname;
};

typedef int64_t off64_t;
typedef void JNIEnv;
typedef void * jobject;

#define ASSET_DIR "assets/"
char *get_app_data_dir();

int AAsset_openFileDescriptor(struct AAsset *asset, off_t *out_start, off_t *out_length)
{
	int ret;
	int fd = asset->fd;

	printf("openning asset's file descriptor: : %d\n", fd);

	struct stat statbuf;

	ret = fstat(fd, &statbuf);
	if(ret)
		printf("oopsie, fstat failed on fd: %d with errno: %d\n", fd, errno);

	*out_start = 0; // on android, we would be returning the fd of the app's apk, and this would be the offset to a non-compressed archive member
	*out_length = statbuf.st_size; // similarly, this would be the size of the section of memory containing the non-compressed archive member

	return fd;
}

struct AAsset* AAssetManager_open(struct AAssetManager *amgr, const char *file_name, int mode)
{
	char *app_data_dir = get_app_data_dir();
	char *path = malloc(strlen(app_data_dir) + strlen(ASSET_DIR) + strlen(file_name) + 1);
	int fd;
	strcpy(path, app_data_dir);
	strcat(path, ASSET_DIR);
	strcat(path, file_name);

	printf("openning asset with filename: %s\n", file_name);

	printf("openning asset at path: %s\n", path);

	fd = open(path, O_CLOEXEC | O_RDWR);
	if(fd < 0) {
		printf("oopsie, falied to open file: %s (errno: %d)\n", file_name, errno);
		return NULL;
	}

	free(path);

	struct AAsset* asset = malloc(sizeof(struct AAsset));
	asset->fd = fd;
	asset->read = 0;

	return asset;
}

struct AAssetDir * AAssetManager_openDir(struct AAssetManager *amgr, const char *dirname)
{
	char* app_data_dir = get_app_data_dir();
	char* dirpath = malloc(strlen(app_data_dir) + strlen(ASSET_DIR) + strlen(dirname) + 1);
	struct AAssetDir* dir = malloc(sizeof(struct AAssetDir));
	dir->dirname = dirname;

	strcpy(dirpath, app_data_dir);
	strcat(dirpath, ASSET_DIR);
	strcat(dirpath, dirname);
	printf("open directory path %s\n", dirpath);

	dir->dp = opendir(dirpath);
	if (dir->dp < 0)
	{
		printf("failed to open directory %s: %d\n", dirpath, errno);
		return NULL;
	}

	return dir;
}

const char * AAssetDir_getNextFileName(struct AAssetDir *dir)
{
	struct dirent *ep = readdir(dir->dp);
	if (ep == NULL)
		return NULL;

	if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, "..")) {
		return AAssetDir_getNextFileName(dir);
	}

	// google claims to return full path, but it really doesn't seem so
	return ep->d_name;
/*	char *asset_root_path = malloc(strlen(dir->dirname) + 1 + strlen(ep->d_name) + 1);

	sprintf(asset_root_path, "%s/%s", dir->dirname, ep->d_name);
	printf("AAssetDir_getNextFileName: returning >%s<\n", asset_root_path);

	return asset_root_path;*/

}


void AAssetDir_close(struct AAssetDir *dir)
{
	closedir(dir->dp);
	free(dir);
}

const void * AAsset_getBuffer(struct AAsset *asset)
{
	int ret;
	int err;
	int fd = asset->fd;

	struct stat statbuf;

	ret = fstat(fd, &statbuf);
	if(ret)
		printf("oopsie, fstat failed on fd: %d with errno: %d\n", fd, errno);

	uint8_t *buffer = malloc(statbuf.st_size);
	ret = pread(fd, buffer, statbuf.st_size, 0);
	if(ret < 0) {
		err = errno;
		printf("oopsie, read failed on fd: %d with errno: %d\n", fd, err);
		exit(err);
	}

	return buffer;
}

off64_t AAsset_getLength64(struct AAsset *asset)
{
	int ret;
	int fd = asset->fd;

	struct stat statbuf;

	ret = fstat(fd, &statbuf);
	if(ret)
		printf("oopsie, fstat failed on fd: %d with errno: %d\n", fd, errno);

	return statbuf.st_size;
}

off_t AAsset_getLength(struct AAsset *asset)
{
	return AAsset_getLength64(asset);
}
struct AAssetManager * AAssetManager_fromJava(JNIEnv *env, jobject assetManager)
{
	// some apps don't like if we return NULL here
	return &dummy_asset_manager;
}

int AAsset_read(struct AAsset *asset, void *buf, size_t count) {
	off64_t tmp = read(asset->fd, buf, count);
	asset->read += tmp;
	return tmp;
}

off_t AAsset_seek(struct AAsset *asset, off_t offset, int whence) {
	off64_t tmp =  lseek(asset->fd, offset, whence);
	asset->read += tmp;
	return tmp;
}

off64_t AAsset_seek64(struct AAsset *asset, off64_t offset, int whence) {
	off64_t tmp =  lseek64(asset->fd, offset, whence);
	asset->read += tmp;
	return tmp;
}

off_t AAsset_getRemainingLength(struct AAsset* asset)
{
	return AAsset_getLength(asset) - asset->read;
}
off64_t AAsset_getRemainingLength64(struct AAsset* asset)
{
	return AAsset_getLength64(asset) - asset->read;
}

void AAsset_close(struct AAsset *asset)
{
	int fd = asset->fd;

	printf("closing asset with fd: %d\n", fd);
	close(fd);

	free(asset);
}
