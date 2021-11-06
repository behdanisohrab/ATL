/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_content_res_AssetManager */

#ifndef _Included_android_content_res_AssetManager
#define _Included_android_content_res_AssetManager
#ifdef __cplusplus
extern "C" {
#endif
#undef android_content_res_AssetManager_ACCESS_UNKNOWN
#define android_content_res_AssetManager_ACCESS_UNKNOWN 0L
#undef android_content_res_AssetManager_ACCESS_RANDOM
#define android_content_res_AssetManager_ACCESS_RANDOM 1L
#undef android_content_res_AssetManager_ACCESS_STREAMING
#define android_content_res_AssetManager_ACCESS_STREAMING 2L
#undef android_content_res_AssetManager_ACCESS_BUFFER
#define android_content_res_AssetManager_ACCESS_BUFFER 3L
#undef android_content_res_AssetManager_localLOGV
#define android_content_res_AssetManager_localLOGV 0L
#undef android_content_res_AssetManager_DEBUG_REFS
#define android_content_res_AssetManager_DEBUG_REFS 0L
#undef android_content_res_AssetManager_STYLE_NUM_ENTRIES
#define android_content_res_AssetManager_STYLE_NUM_ENTRIES 6L
#undef android_content_res_AssetManager_STYLE_TYPE
#define android_content_res_AssetManager_STYLE_TYPE 0L
#undef android_content_res_AssetManager_STYLE_DATA
#define android_content_res_AssetManager_STYLE_DATA 1L
#undef android_content_res_AssetManager_STYLE_ASSET_COOKIE
#define android_content_res_AssetManager_STYLE_ASSET_COOKIE 2L
#undef android_content_res_AssetManager_STYLE_RESOURCE_ID
#define android_content_res_AssetManager_STYLE_RESOURCE_ID 3L
#undef android_content_res_AssetManager_STYLE_CHANGING_CONFIGURATIONS
#define android_content_res_AssetManager_STYLE_CHANGING_CONFIGURATIONS 4L
#undef android_content_res_AssetManager_STYLE_DENSITY
#define android_content_res_AssetManager_STYLE_DENSITY 5L
/*
 * Class:     android_content_res_AssetManager
 * Method:    list
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_android_content_res_AssetManager_list
  (JNIEnv *, jobject, jstring);

/*
 * Class:     android_content_res_AssetManager
 * Method:    addAssetPathNative
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_addAssetPathNative
  (JNIEnv *, jobject, jstring);

/*
 * Class:     android_content_res_AssetManager
 * Method:    isUpToDate
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_android_content_res_AssetManager_isUpToDate
  (JNIEnv *, jobject);

/*
 * Class:     android_content_res_AssetManager
 * Method:    setLocale
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_android_content_res_AssetManager_setLocale
  (JNIEnv *, jobject, jstring);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getLocales
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_android_content_res_AssetManager_getLocales
  (JNIEnv *, jobject);

/*
 * Class:     android_content_res_AssetManager
 * Method:    setConfiguration
 * Signature: (IILjava/lang/String;IIIIIIIIIIIIII)V
 */
JNIEXPORT void JNICALL Java_android_content_res_AssetManager_setConfiguration
  (JNIEnv *, jobject, jint, jint, jstring, jint, jint, jint, jint, jint, jint, jint, jint, jint, jint, jint, jint, jint, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getResourceName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_android_content_res_AssetManager_getResourceName
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getResourcePackageName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_android_content_res_AssetManager_getResourcePackageName
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getResourceTypeName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_android_content_res_AssetManager_getResourceTypeName
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getResourceEntryName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_android_content_res_AssetManager_getResourceEntryName
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    openAsset
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_openAsset
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    openAssetFd
 * Signature: (Ljava/lang/String;[J)Landroid/os/ParcelFileDescriptor;
 */
JNIEXPORT jobject JNICALL Java_android_content_res_AssetManager_openAssetFd
  (JNIEnv *, jobject, jstring, jlongArray);

/*
 * Class:     android_content_res_AssetManager
 * Method:    openNonAssetNative
 * Signature: (ILjava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_openNonAssetNative
  (JNIEnv *, jobject, jint, jstring, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    openNonAssetFdNative
 * Signature: (ILjava/lang/String;[J)Landroid/os/ParcelFileDescriptor;
 */
JNIEXPORT jobject JNICALL Java_android_content_res_AssetManager_openNonAssetFdNative
  (JNIEnv *, jobject, jint, jstring, jlongArray);

/*
 * Class:     android_content_res_AssetManager
 * Method:    destroyAsset
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_android_content_res_AssetManager_destroyAsset
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    readAssetChar
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_readAssetChar
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    readAsset
 * Signature: (I[BII)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_readAsset
  (JNIEnv *, jobject, jint, jbyteArray, jint, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    seekAsset
 * Signature: (IJI)J
 */
JNIEXPORT jlong JNICALL Java_android_content_res_AssetManager_seekAsset
  (JNIEnv *, jobject, jint, jlong, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getAssetLength
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_android_content_res_AssetManager_getAssetLength
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getAssetRemainingLength
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_android_content_res_AssetManager_getAssetRemainingLength
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    loadResourceValue
 * Signature: (ISLandroid/util/TypedValue;Z)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_loadResourceValue
  (JNIEnv *, jobject, jint, jshort, jobject, jboolean);

/*
 * Class:     android_content_res_AssetManager
 * Method:    loadResourceBagValue
 * Signature: (IILandroid/util/TypedValue;Z)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_loadResourceBagValue
  (JNIEnv *, jobject, jint, jint, jobject, jboolean);

/*
 * Class:     android_content_res_AssetManager
 * Method:    applyStyle
 * Signature: (IIII[I[I[I)Z
 */
JNIEXPORT jboolean JNICALL Java_android_content_res_AssetManager_applyStyle
  (JNIEnv *, jclass, jint, jint, jint, jint, jintArray, jintArray, jintArray);

/*
 * Class:     android_content_res_AssetManager
 * Method:    retrieveAttributes
 * Signature: (I[I[I[I)Z
 */
JNIEXPORT jboolean JNICALL Java_android_content_res_AssetManager_retrieveAttributes
  (JNIEnv *, jobject, jint, jintArray, jintArray, jintArray);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getArraySize
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_getArraySize
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    retrieveArray
 * Signature: (I[I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_retrieveArray
  (JNIEnv *, jobject, jint, jintArray);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getStringBlockCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_getStringBlockCount
  (JNIEnv *, jobject);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getNativeStringBlock
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_getNativeStringBlock
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getCookieName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_android_content_res_AssetManager_getCookieName
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getGlobalAssetCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_getGlobalAssetCount
  (JNIEnv *, jclass);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getAssetAllocations
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_android_content_res_AssetManager_getAssetAllocations
  (JNIEnv *, jclass);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getGlobalAssetManagerCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_getGlobalAssetManagerCount
  (JNIEnv *, jclass);

/*
 * Class:     android_content_res_AssetManager
 * Method:    newTheme
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_newTheme
  (JNIEnv *, jobject);

/*
 * Class:     android_content_res_AssetManager
 * Method:    deleteTheme
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_android_content_res_AssetManager_deleteTheme
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    applyThemeStyle
 * Signature: (IIZ)V
 */
JNIEXPORT void JNICALL Java_android_content_res_AssetManager_applyThemeStyle
  (JNIEnv *, jclass, jint, jint, jboolean);

/*
 * Class:     android_content_res_AssetManager
 * Method:    copyTheme
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_android_content_res_AssetManager_copyTheme
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    loadThemeAttributeValue
 * Signature: (IILandroid/util/TypedValue;Z)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_loadThemeAttributeValue
  (JNIEnv *, jclass, jint, jint, jobject, jboolean);

/*
 * Class:     android_content_res_AssetManager
 * Method:    dumpTheme
 * Signature: (IILjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_android_content_res_AssetManager_dumpTheme
  (JNIEnv *, jclass, jint, jint, jstring, jstring);

/*
 * Class:     android_content_res_AssetManager
 * Method:    openXmlAssetNative
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_AssetManager_openXmlAssetNative
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getArrayStringResource
 * Signature: (I)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_android_content_res_AssetManager_getArrayStringResource
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getArrayStringInfo
 * Signature: (I)[I
 */
JNIEXPORT jintArray JNICALL Java_android_content_res_AssetManager_getArrayStringInfo
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    getArrayIntResource
 * Signature: (I)[I
 */
JNIEXPORT jintArray JNICALL Java_android_content_res_AssetManager_getArrayIntResource
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_content_res_AssetManager
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_android_content_res_AssetManager_destroy
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
