/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_graphics_Bitmap */

#ifndef _Included_android_graphics_Bitmap
#define _Included_android_graphics_Bitmap
#ifdef __cplusplus
extern "C" {
#endif
#undef android_graphics_Bitmap_DENSITY_NONE
#define android_graphics_Bitmap_DENSITY_NONE 0L
#undef android_graphics_Bitmap_WORKING_COMPRESS_STORAGE
#define android_graphics_Bitmap_WORKING_COMPRESS_STORAGE 4096L
/*
 * Class:     android_graphics_Bitmap
 * Method:    getWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_getWidth
  (JNIEnv *, jobject);

/*
 * Class:     android_graphics_Bitmap
 * Method:    getHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_getHeight
  (JNIEnv *, jobject);

/*
 * Class:     android_graphics_Bitmap
 * Method:    native_bitmap_from_path
 * Signature: (Ljava/lang/CharSequence;)J
 */
JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1bitmap_1from_1path
  (JNIEnv *, jobject, jobject);

/*
 * Class:     android_graphics_Bitmap
 * Method:    native_copy
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1copy
  (JNIEnv *, jclass, jlong);

/*
 * Class:     android_graphics_Bitmap
 * Method:    native_subpixbuf
 * Signature: (JIIII)J
 */
JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1subpixbuf
  (JNIEnv *, jclass, jlong, jint, jint, jint, jint);

/*
 * Class:     android_graphics_Bitmap
 * Method:    native_create
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1create
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeCopy
 * Signature: (IIZ)Landroid/graphics/Bitmap;
 */
JNIEXPORT jobject JNICALL Java_android_graphics_Bitmap_nativeCopy
  (JNIEnv *, jclass, jint, jint, jboolean);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeDestructor
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeDestructor
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeRecycle
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_android_graphics_Bitmap_nativeRecycle
  (JNIEnv *, jclass, jlong);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeReconfigure
 * Signature: (IIIII)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeReconfigure
  (JNIEnv *, jclass, jint, jint, jint, jint, jint);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeCompress
 * Signature: (IIILjava/io/OutputStream;[B)Z
 */
JNIEXPORT jboolean JNICALL Java_android_graphics_Bitmap_nativeCompress
  (JNIEnv *, jclass, jint, jint, jint, jobject, jbyteArray);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeErase
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeErase
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeRowBytes
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_nativeRowBytes
  (JNIEnv *, jclass, jlong);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeConfig
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_nativeConfig
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeGetPixel
 * Signature: (IIIZ)I
 */
JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_nativeGetPixel
  (JNIEnv *, jclass, jint, jint, jint, jboolean);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeGetPixels
 * Signature: (J[IIIIIIIZ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeGetPixels
  (JNIEnv *, jclass, jlong, jintArray, jint, jint, jint, jint, jint, jint, jboolean);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeSetPixel
 * Signature: (IIIIZ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeSetPixel
  (JNIEnv *, jclass, jint, jint, jint, jint, jboolean);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeSetPixels
 * Signature: (I[IIIIIIIZ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeSetPixels
  (JNIEnv *, jclass, jint, jintArray, jint, jint, jint, jint, jint, jint, jboolean);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeCopyPixelsToBuffer
 * Signature: (ILjava/nio/Buffer;)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeCopyPixelsToBuffer
  (JNIEnv *, jclass, jint, jobject);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeCopyPixelsFromBuffer
 * Signature: (ILjava/nio/Buffer;)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeCopyPixelsFromBuffer
  (JNIEnv *, jclass, jint, jobject);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeGenerationId
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_nativeGenerationId
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeExtractAlpha
 * Signature: (II[I)Landroid/graphics/Bitmap;
 */
JNIEXPORT jobject JNICALL Java_android_graphics_Bitmap_nativeExtractAlpha
  (JNIEnv *, jclass, jint, jint, jintArray);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativePrepareToDraw
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativePrepareToDraw
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeSetAlphaAndPremultiplied
 * Signature: (IZZ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeSetAlphaAndPremultiplied
  (JNIEnv *, jclass, jint, jboolean, jboolean);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeHasMipMap
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_android_graphics_Bitmap_nativeHasMipMap
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeSetHasMipMap
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeSetHasMipMap
  (JNIEnv *, jclass, jint, jboolean);

/*
 * Class:     android_graphics_Bitmap
 * Method:    nativeSameAs
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_android_graphics_Bitmap_nativeSameAs
  (JNIEnv *, jclass, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
