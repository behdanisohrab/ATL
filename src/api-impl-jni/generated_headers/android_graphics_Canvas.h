/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_graphics_Canvas */

#ifndef _Included_android_graphics_Canvas
#define _Included_android_graphics_Canvas
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     android_graphics_Canvas
 * Method:    native_canvas_from_bitmap
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_android_graphics_Canvas_native_1canvas_1from_1bitmap
  (JNIEnv *, jclass, jlong);

/*
 * Class:     android_graphics_Canvas
 * Method:    native_save
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1save
  (JNIEnv *, jclass, jlong, jlong);

/*
 * Class:     android_graphics_Canvas
 * Method:    native_restore
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1restore
  (JNIEnv *, jclass, jlong, jlong);

/*
 * Class:     android_graphics_Canvas
 * Method:    native_drawText
 * Signature: (JLjava/lang/CharSequence;IIFFJJ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawText
  (JNIEnv *, jclass, jlong, jobject, jint, jint, jfloat, jfloat, jlong, jlong);

/*
 * Class:     android_graphics_Canvas
 * Method:    native_drawRect
 * Signature: (JFFFFJ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawRect
  (JNIEnv *, jclass, jlong, jfloat, jfloat, jfloat, jfloat, jlong);

/*
 * Class:     android_graphics_Canvas
 * Method:    native_drawLine
 * Signature: (JJFFFFJ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawLine
  (JNIEnv *, jclass, jlong, jlong, jfloat, jfloat, jfloat, jfloat, jlong);

/*
 * Class:     android_graphics_Canvas
 * Method:    native_drawBitmap
 * Signature: (JJJFFFFFFFFJ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawBitmap
  (JNIEnv *, jclass, jlong, jlong, jlong, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jlong);

/*
 * Class:     android_graphics_Canvas
 * Method:    native_rotate
 * Signature: (JJF)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1rotate
  (JNIEnv *, jclass, jlong, jlong, jfloat);

/*
 * Class:     android_graphics_Canvas
 * Method:    native_rotate_and_translate
 * Signature: (JJFFF)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1rotate_1and_1translate
  (JNIEnv *, jclass, jlong, jlong, jfloat, jfloat, jfloat);

/*
 * Class:     android_graphics_Canvas
 * Method:    native_drawPath
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawPath
  (JNIEnv *, jclass, jlong, jlong, jlong);

#ifdef __cplusplus
}
#endif
#endif
