#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "defines.h"
#include "util.h"
#include "android_content_res_AssetManager.h"

#define ASSET_DIR "/home/Mis012/Github_and_other_sources/android_translation_layer_PoC/data/assets/"

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_openAsset(JNIEnv *env, jobject this, jstring _file_name, jint mode)
{
	const char *file_name = _CSTRING(_file_name);
	char *path = malloc(strlen(file_name) + strlen(ASSET_DIR) + 1);
	int fd;

	strcpy(path, ASSET_DIR);
	strcat(path, file_name);

	printf("openning asset with filename: %s\n", _CSTRING(_file_name));

	printf("openning asset at path: %s\n", path);

	fd = open(path, O_CLOEXEC | O_RDWR);

	free(path);

	return fd;
}

JNIEXPORT jlong JNICALL Java_android_content_res_AssetManager_getAssetLength(JNIEnv *env, jobject this, jint fd)
{
	int ret;
	struct stat statbuf;

	ret = fstat(fd, &statbuf);
	if(ret)
		printf("oopsie, fstat failed on fd: %d with errno: %d\n", fd, errno);

	return statbuf.st_size;
}

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_readAsset(JNIEnv *env, jobject this, jint fd, jbyteArray b, jint off, jint len)
{
	int ret;
	int err;

	jbyte *array = _GET_BYTE_ARRAY_ELEMENTS(b);
	ret = read(fd, &array[off], len);
	_RELEASE_BYTE_ARRAY_ELEMENTS(b, array);

	if(ret < 0) {
		err = errno;
		printf("oopsie, read failed on fd: %d with errno: %d\n", fd, err);
		exit(err);
	} else if (ret == 0) { //EOF
		return -1;
	} else {
		return ret;
	}
}

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_readAssetChar(JNIEnv *env, jobject this, jint fd)
{
	int ret;
	int err;
	unsigned char byte;

	ret = read(fd, &byte, 1);
	if(ret == 1)
		return byte;
	else if(ret == 0)
		return -1;
	else {
		err = errno;
		printf("oopsie, read failed on fd: %d with errno: %d\n", fd, err);
		exit(err);
	}
}

JNIEXPORT jlong JNICALL Java_android_content_res_AssetManager_seekAsset(JNIEnv *env, jobject this, jint fd, jlong off, jint whence)
{
	return lseek(fd, off, (whence > 0) ? SEEK_END : (whence < 0 ? SEEK_SET : SEEK_CUR));
}

JNIEXPORT void JNICALL Java_android_content_res_AssetManager_destroyAsset(JNIEnv *env, jobject this, jint fd)
{
	printf("closing asset with fd: %d\n", fd);
	close(fd);
}
