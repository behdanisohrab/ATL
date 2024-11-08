/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_app_Dialog */

#ifndef _Included_android_app_Dialog
#define _Included_android_app_Dialog
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     android_app_Dialog
 * Method:    nativeInit
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_android_app_Dialog_nativeInit
  (JNIEnv *, jobject);

/*
 * Class:     android_app_Dialog
 * Method:    nativeSetTitle
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_android_app_Dialog_nativeSetTitle
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     android_app_Dialog
 * Method:    nativeSetContentView
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_android_app_Dialog_nativeSetContentView
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     android_app_Dialog
 * Method:    nativeShow
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_android_app_Dialog_nativeShow
  (JNIEnv *, jobject, jlong);

/*
 * Class:     android_app_Dialog
 * Method:    nativeClose
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_android_app_Dialog_nativeClose
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
