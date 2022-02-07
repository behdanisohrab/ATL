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

#define ASSET_DIR "data/assets/"

struct AAsset* AAssetManager_open(struct AAssetManager *amgr, const char *file_name, int mode)
{
	char *path = malloc(strlen(file_name) + strlen(ASSET_DIR) + 1);
	int fd;

	strcpy(path, ASSET_DIR);
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

void AAsset_close(struct AAsset *asset)
{
	int fd = asset->fd;

	printf("closing asset with fd: %d\n", fd);
	close(fd);

	free(asset);
}
