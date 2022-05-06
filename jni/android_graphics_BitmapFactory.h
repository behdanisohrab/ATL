/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_graphics_BitmapFactory */

#ifndef _Included_android_graphics_BitmapFactory
#define _Included_android_graphics_BitmapFactory
#ifdef __cplusplus
extern "C" {
#endif
#undef android_graphics_BitmapFactory_DECODE_BUFFER_SIZE
#define android_graphics_BitmapFactory_DECODE_BUFFER_SIZE 16384L
/*
 * Class:     android_graphics_BitmapFactory
 * Method:    nativeDecodeStream
 * Signature: (Ljava/io/InputStream;[BLandroid/graphics/Rect;Landroid/graphics/BitmapFactory/Options;)Landroid/graphics/Bitmap;
 */
JNIEXPORT jobject JNICALL Java_android_graphics_BitmapFactory_nativeDecodeStream
  (JNIEnv *, jclass, jobject, jbyteArray, jobject, jobject);

/*
 * Class:     android_graphics_BitmapFactory
 * Method:    nativeDecodeFileDescriptor
 * Signature: (Ljava/io/FileDescriptor;Landroid/graphics/Rect;Landroid/graphics/BitmapFactory/Options;)Landroid/graphics/Bitmap;
 */
JNIEXPORT jobject JNICALL Java_android_graphics_BitmapFactory_nativeDecodeFileDescriptor
  (JNIEnv *, jclass, jobject, jobject, jobject);

/*
 * Class:     android_graphics_BitmapFactory
 * Method:    nativeDecodeAsset
 * Signature: (ILandroid/graphics/Rect;Landroid/graphics/BitmapFactory/Options;)Landroid/graphics/Bitmap;
 */
JNIEXPORT jobject JNICALL Java_android_graphics_BitmapFactory_nativeDecodeAsset
  (JNIEnv *, jclass, jint, jobject, jobject);

/*
 * Class:     android_graphics_BitmapFactory
 * Method:    nativeDecodeByteArray
 * Signature: ([BIILandroid/graphics/BitmapFactory/Options;)Landroid/graphics/Bitmap;
 */
JNIEXPORT jobject JNICALL Java_android_graphics_BitmapFactory_nativeDecodeByteArray
  (JNIEnv *, jclass, jbyteArray, jint, jint, jobject);

/*
 * Class:     android_graphics_BitmapFactory
 * Method:    nativeIsSeekable
 * Signature: (Ljava/io/FileDescriptor;)Z
 */
JNIEXPORT jboolean JNICALL Java_android_graphics_BitmapFactory_nativeIsSeekable
  (JNIEnv *, jclass, jobject);

#ifdef __cplusplus
}
#endif
#endif