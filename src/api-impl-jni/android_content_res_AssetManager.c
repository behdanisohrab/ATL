#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <androidfw/androidfw_c_api.h>

#include "defines.h"
#include "util.h"
#include "generated_headers/android_content_res_AssetManager.h"

#include <glib.h>
#include <dirent.h>

#define ASSET_DIR "assets/"
char *get_app_data_dir();

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_openAsset(JNIEnv *env, jobject this, jstring _file_name, jint mode)
{
	const char *file_name = _CSTRING(_file_name);

	/* handle absolute paths */
	if(file_name[0] == '/')
		return open(file_name, O_CLOEXEC | O_RDWR);

	char *app_data_dir = get_app_data_dir();
	char *path = malloc(strlen(app_data_dir) + strlen(ASSET_DIR) + strlen(file_name) + 1);
	int fd;

	strcpy(path, app_data_dir);
	strcat(path, ASSET_DIR);
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

JNIEXPORT jlong JNICALL Java_android_content_res_AssetManager_getAssetRemainingLength(JNIEnv *env, jobject this, jint fd)
{
	jlong file_size = Java_android_content_res_AssetManager_getAssetLength(env, this, fd);
	off_t offset = lseek(fd, 0, SEEK_CUR);

	return file_size - offset;
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

JNIEXPORT void JNICALL Java_android_content_res_AssetManager_init(JNIEnv *env, jobject this)
{
	struct AssetManager *asset_manager = AssetManager_new();
	const struct ResTable_config config = {
		.density = /*ACONFIGURATION_DENSITY_MEDIUM*/ 160,
		.sdkVersion = 24,
	};
	AssetManager_setConfiguration(asset_manager, &config, NULL);
	_SET_LONG_FIELD(this, "mObject", _INTPTR(asset_manager));
}

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_addAssetPathNative(JNIEnv *env, jobject this, jstring path)
{
	int32_t cookie;
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	AssetManager_addAssetPath(asset_manager, _CSTRING(path), &cookie, false, false);
	return cookie;
}

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_loadResourceValue(JNIEnv *env, jobject this, jint ident, jshort density, jobject outValue, jboolean resolve)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	uint32_t resId = ident;
	struct Res_value value;
	uint32_t outSpecFlags;
	struct ResTable_config outConfig;
	ssize_t block = ResTable_getResource(res_table, resId, &value, false, density, &outSpecFlags, &outConfig);
	if (resolve) {
		block = ResTable_resolveReference(res_table, &value, block, &resId, &outSpecFlags, &outConfig);
	}
	if (block >= 0) {
		_SET_INT_FIELD(outValue, "type", value.dataType);
		_SET_INT_FIELD(outValue, "data", value.data);
		_SET_INT_FIELD(outValue, "resourceId", resId);
		_SET_INT_FIELD(outValue, "assetCookie", block);
		if (value.dataType == TYPE_STRING) {
			const struct ResStringPool *string_pool = ResTable_getTableStringBlock(res_table, block);
			size_t len;
			const char16_t *string = ResStringPool_stringAt(string_pool, value.data, &len);
			_SET_OBJ_FIELD(outValue, "string", "Ljava/lang/CharSequence;", (*env)->NewString(env, string, len));
		} else {
			_SET_OBJ_FIELD(outValue, "string", "Ljava/lang/CharSequence;", NULL);
		}
	}
	return block;
}

JNIEXPORT jobjectArray JNICALL Java_android_content_res_AssetManager_getArrayStringResource(JNIEnv *env, jobject this, jint ident)
{
	int i;
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	const struct bag_entry *bag;
	int bag_count = ResTable_lockBag(res_table, ident, &bag);
	jobjectArray array = (*env)->NewObjectArray(env, bag_count, (*env)->FindClass(env, "java/lang/String"), NULL);
	for (i = 0; i < bag_count; i++) {
		struct Res_value value = bag[i].map.value;
		ssize_t block = ResTable_resolveReference(res_table, &value, bag[i].stringBlock, NULL, NULL, NULL);
		if (value.dataType == TYPE_STRING) {
			const struct ResStringPool *string_pool = ResTable_getTableStringBlock(res_table, block);
			if (string_pool == NULL)
				continue;
			size_t len;
			const char16_t *string = ResStringPool_stringAt(string_pool, value.data, &len);
			(*env)->SetObjectArrayElement(env, array, i, (*env)->NewString(env, string, len));
		}
	}

	ResTable_unlockBag(res_table, bag);
	return array;
}

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_getResourceIdentifier(JNIEnv *env, jobject this, jstring name, jstring defType, jstring defPackage)
{
	const char16_t *name16 = NULL;
	const char16_t *defType16 = NULL;
	const char16_t *defPackage16 = NULL;
	int name_len = 0;
	int defType_len = 0;
	int defPackage_len = 0;
	int ret;

	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	if (name) {
		name16 = (*env)->GetStringChars(env, name, NULL);
		name_len = (*env)->GetStringLength(env, name);
	}
	if (defType) {
		defType16 = (*env)->GetStringChars(env, defType, NULL);
		defType_len = (*env)->GetStringLength(env, defType);
	}
	if (defPackage) {
		defPackage16 = (*env)->GetStringChars(env, defPackage, NULL);
		defPackage_len = (*env)->GetStringLength(env, defPackage);
	}
	ret = ResTable_identifierForName(res_table, name16, name_len, defType16, defType_len, defPackage16, defPackage_len, NULL);
	if (name)
		(*env)->ReleaseStringChars(env, name, name16);
	if (defType)
		(*env)->ReleaseStringChars(env, defType, defType16);
	if (defPackage)
		(*env)->ReleaseStringChars(env, defPackage, defPackage16);
	return ret;
}

JNIEXPORT jobject JNICALL Java_android_content_res_AssetManager_getPooledString(JNIEnv *env, jobject this, jint block, jint index)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	const struct ResStringPool *string_pool = ResTable_getTableStringBlock(res_table, block);
	size_t len;
	const char16_t *string = ResStringPool_stringAt(string_pool, index, &len);
	return (*env)->NewString(env, string, len);
}

JNIEXPORT jlong JNICALL Java_android_content_res_AssetManager_newTheme(JNIEnv *env, jobject this)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	struct Theme *theme = Theme_new(AssetManager_getResources(asset_manager, true));
	return _INTPTR(theme);
}

JNIEXPORT void JNICALL Java_android_content_res_AssetManager_deleteTheme(JNIEnv *env, jobject this, jlong theme_ptr)
{
	Theme_delete(_PTR(theme_ptr));
}

JNIEXPORT void JNICALL Java_android_content_res_AssetManager_applyThemeStyle(JNIEnv *env, jobject this, jlong theme_ptr, jint styleRes, jboolean force)
{
	Theme_applyStyle(_PTR(theme_ptr), styleRes, force);
}

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_loadThemeAttributeValue(JNIEnv *env, jobject this, jlong theme_ptr, jint ident, jobject outValue, jboolean resolve)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	struct Theme *theme = _PTR(theme_ptr);
	uint32_t resId = ident;
	struct Res_value value;
	uint32_t outSpecFlags;
	struct ResTable_config outConfig;
	int block = Theme_getAttribute(theme, resId, &value, &outSpecFlags);
	if (resolve) {
		block = Theme_resolveAttributeReference(theme, &value, block, &resId, &outSpecFlags, &outConfig);
	}
	if (block >= 0) {
		_SET_INT_FIELD(outValue, "type", value.dataType);
		_SET_INT_FIELD(outValue, "data", value.data);
		_SET_INT_FIELD(outValue, "resourceId", resId);
		_SET_INT_FIELD(outValue, "assetCookie", block);
		if (value.dataType == TYPE_STRING) {
			const struct ResStringPool *string_pool = ResTable_getTableStringBlock(res_table, block);
			size_t len;
			const char16_t *string = ResStringPool_stringAt(string_pool, value.data, &len);
			_SET_OBJ_FIELD(outValue, "string", "Ljava/lang/CharSequence;", (*env)->NewString(env, string, len));
		} else {
			_SET_OBJ_FIELD(outValue, "string", "Ljava/lang/CharSequence;", NULL);
		}
	}
	return block;
}

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_getArraySize(JNIEnv *env, jobject this, jint ident)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	const struct bag_entry *bag;
	int bag_count = ResTable_lockBag(res_table, ident, &bag);
	ResTable_unlockBag(res_table, bag);
	return bag_count;
}

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_retrieveArray(JNIEnv *env, jobject this, jint ident, jintArray outArray)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	const struct bag_entry *bag;
	int i;

	jint *array = (*env)->GetIntArrayElements(env, outArray, NULL);
	int bag_count = ResTable_lockBag(res_table, ident, &bag);
	for (i = 0; i < bag_count; i++) {
		struct Res_value value = bag[i].map.value;
		uint32_t resId = 0;
		ssize_t block = ResTable_resolveReference(res_table, &value, bag[i].stringBlock, &resId, NULL, NULL);

		array[i*android_content_res_AssetManager_STYLE_NUM_ENTRIES + android_content_res_AssetManager_STYLE_TYPE] = value.dataType;
		array[i*android_content_res_AssetManager_STYLE_NUM_ENTRIES + android_content_res_AssetManager_STYLE_DATA] = value.data;
		array[i*android_content_res_AssetManager_STYLE_NUM_ENTRIES + android_content_res_AssetManager_STYLE_ASSET_COOKIE] = block;
		array[i*android_content_res_AssetManager_STYLE_NUM_ENTRIES + android_content_res_AssetManager_STYLE_RESOURCE_ID] = resId;

	}
	ResTable_unlockBag(res_table, bag);
	(*env)->ReleaseIntArrayElements(env, outArray, array, 0);
	return bag_count;
}

JNIEXPORT jstring JNICALL Java_android_content_res_AssetManager_getResourcePackageName(JNIEnv *env, jobject this, jint ident)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	struct resource_name res_name;
	bool ret = ResTable_getResourceName(res_table, ident, false, &res_name);
	return (ret && res_name.package) ? (*env)->NewString(env, res_name.package, res_name.packageLen) : NULL;
}

JNIEXPORT jstring JNICALL Java_android_content_res_AssetManager_getResourceTypeName(JNIEnv *env, jobject this, jint ident)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	struct resource_name res_name;
	bool ret = ResTable_getResourceName(res_table, ident, false, &res_name);
	return (ret && res_name.type) ? (*env)->NewString(env, res_name.type, res_name.typeLen) : NULL;
}

JNIEXPORT jstring JNICALL Java_android_content_res_AssetManager_getResourceEntryName(JNIEnv *env, jobject this, jint ident)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	struct resource_name res_name;
	bool ret = ResTable_getResourceName(res_table, ident, false, &res_name);
	return (ret && res_name.name) ? (*env)->NewString(env, res_name.name, res_name.nameLen) : NULL;
}

JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_loadResourceBagValue(JNIEnv *env, jobject this, jint ident, jint bagEntryId, jobject outValue, jboolean resolve)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable *res_table = AssetManager_getResources(asset_manager, true);
	const struct bag_entry *bag;
	int i;
	ssize_t block = -1;

	int bag_count = ResTable_lockBag(res_table, ident, &bag);
	for (i = 0; i < bag_count; i++) {
		if (bag[i].map.name.ident == bagEntryId) {
			struct Res_value value = bag[i].map.value;
			uint32_t resId = 0;
			block = ResTable_resolveReference(res_table, &value, bag[i].stringBlock, &resId, NULL, NULL);

			_SET_INT_FIELD(outValue, "type", value.dataType);
			_SET_INT_FIELD(outValue, "data", value.data);
			_SET_INT_FIELD(outValue, "resourceId", resId);
			_SET_INT_FIELD(outValue, "assetCookie", block);
			if (value.dataType == TYPE_STRING) {
				const struct ResStringPool *string_pool = ResTable_getTableStringBlock(res_table, block);
				size_t len;
				const char16_t *string = ResStringPool_stringAt(string_pool, value.data, &len);
				_SET_OBJ_FIELD(outValue, "string", "Ljava/lang/CharSequence;", (*env)->NewString(env, string, len));
			} else {
				_SET_OBJ_FIELD(outValue, "string", "Ljava/lang/CharSequence;", NULL);
			}
			break;
		}
	}

	ResTable_unlockBag(res_table, bag);
	return block;
}

JNIEXPORT void JNICALL Java_android_content_res_AssetManager_copyTheme(JNIEnv *env, jclass class, jlong dest, jlong src)
{
	Theme_setTo(_PTR(dest), _PTR(src));
}

JNIEXPORT void JNICALL Java_android_content_res_AssetManager_setConfiguration(
    JNIEnv *env, jobject this, jint mcc, jint mnc, jstring locale,
    jint orientation, jint touchscreen, jint density, jint keyboard,
    jint keyboardHidden, jint navigation, jint screenWidth, jint screenHeight,
    jint smallestScreenWidthDp, jint screenWidthDp, jint screenHeightDp,
    jint screenLayout, jint uiMode, jint majorVersion)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const struct ResTable_config config = {
		.mcc = mcc,
		.mnc = mnc,
		.orientation = orientation,
		.touchscreen = touchscreen,
		.density = density,
		.keyboard = keyboard,
		.navigation = navigation,
		.screenWidth = screenWidth,
		.screenHeight = screenHeight,
		.smallestScreenWidthDp = smallestScreenWidthDp,
		.screenWidthDp = screenWidthDp,
		.screenHeightDp = screenHeightDp,
		.screenLayout = screenLayout,
		.uiMode = uiMode,
		.sdkVersion = majorVersion
	};
	AssetManager_setConfiguration(asset_manager, &config, NULL);
}

JNIEXPORT jobjectArray JNICALL Java_android_content_res_AssetManager_list(JNIEnv *env, jobject this, jstring _path)
{
	DIR *d;
	struct dirent *dir;

	const char* path_rel = _CSTRING(_path);
	char *app_data_dir = get_app_data_dir();
	char *path_abs = malloc(strlen(app_data_dir) + strlen(ASSET_DIR) + strlen(path_rel) + 1);

	strcpy(path_abs, app_data_dir);
	strcat(path_abs, ASSET_DIR);
	strcat(path_abs, path_rel);

	d = opendir(path_abs);

	GArray *assets = g_array_new(false, false, sizeof(const char *));
	int i = 0;
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			char *asset_path = malloc (strlen(dir->d_name) + 1);
			strcpy(asset_path, dir->d_name);
			g_array_append_val (assets, asset_path);
		}
		closedir(d);
	}

	jobjectArray array = (*env)->NewObjectArray(env, assets->len, (*env)->FindClass(env, "java/lang/String"), NULL);
	for (i = 0; i < assets->len; i++)
	{
		const char *asset = g_array_index(assets, const char *, i);
		(*env)->SetObjectArrayElement(env, array, i, (*env)->NewStringUTF(env, asset));
	}

	g_array_free(assets, TRUE);

	return array;
}

JNIEXPORT jlong JNICALL Java_android_content_res_AssetManager_openXmlAssetNative(JNIEnv *env, jobject this, jint cookie, jstring _file_name)
{
	struct AssetManager *asset_manager = _PTR(_GET_LONG_FIELD(this, "mObject"));
	const char *file_name = (*env)->GetStringUTFChars(env, _file_name, NULL);
	struct Asset *asset = AssetManager_openNonAsset(asset_manager, file_name, ACCESS_BUFFER);
	(*env)->ReleaseStringUTFChars(env, _file_name, file_name);

	struct ResXMLTree *res_xml = ResXMLTree_new();
	ResXMLTree_setTo(res_xml, Asset_getBuffer(asset, true), Asset_getLength(asset), true);
	Asset_delete(asset);
	return _INTPTR(res_xml);
}
