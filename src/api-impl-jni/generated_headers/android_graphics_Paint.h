/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_graphics_Paint */

#ifndef _Included_android_graphics_Paint
#define _Included_android_graphics_Paint
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     android_graphics_Paint
 * Method:    native_constructor
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_android_graphics_Paint_native_1constructor
  (JNIEnv *, jobject);

/*
 * Class:     android_graphics_Paint
 * Method:    native_set_color
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Paint_native_1set_1color
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     android_graphics_Paint
 * Method:    native_get_color
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_android_graphics_Paint_native_1get_1color
  (JNIEnv *, jobject, jlong);

/*
 * Class:     android_graphics_Paint
 * Method:    native_create_font
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_android_graphics_Paint_native_1create_1font
  (JNIEnv *, jclass);

/*
 * Class:     android_graphics_Paint
 * Method:    native_ascent
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_android_graphics_Paint_native_1ascent
  (JNIEnv *, jclass, jlong);

/*
 * Class:     android_graphics_Paint
 * Method:    native_set_typeface
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Paint_native_1set_1typeface
  (JNIEnv *, jclass, jlong, jlong);

/*
 * Class:     android_graphics_Paint
 * Method:    native_set_text_size
 * Signature: (JF)V
 */
JNIEXPORT void JNICALL Java_android_graphics_Paint_native_1set_1text_1size
  (JNIEnv *, jclass, jlong, jfloat);

/*
 * Class:     android_graphics_Paint
 * Method:    native_measure_text
 * Signature: (JLjava/lang/CharSequence;IIJ)F
 */
JNIEXPORT jfloat JNICALL Java_android_graphics_Paint_native_1measure_1text
  (JNIEnv *, jclass, jlong, jobject, jint, jint, jlong);

#ifdef __cplusplus
}
#endif
#endif