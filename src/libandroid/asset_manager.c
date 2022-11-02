
#define _LARGEFILE64_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

struct AAssetManager {
};

struct AAsset{
	int fd;
};

typedef int64_t off64_t;
typedef void JNIEnv;
typedef void * jobject;

#define ASSET_DIR "assets/"
char *get_app_data_dir();

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

	return asset;
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

struct AAssetManager * AAssetManager_fromJava(JNIEnv *env, jobject assetManager)
{
	return NULL;
}

int AAsset_read(struct AAsset *asset, void *buf, size_t count) {
	return read(asset->fd, buf, count);
}

off64_t AAsset_seek64(struct AAsset *asset, off64_t offset, int whence) {
	return lseek64(asset->fd, offset, whence);
}

void AAsset_close(struct AAsset *asset)
{
	int fd = asset->fd;

	printf("closing asset with fd: %d\n", fd);
	close(fd);

	free(asset);
}
