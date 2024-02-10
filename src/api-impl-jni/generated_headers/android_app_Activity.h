/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_app_Activity */

#ifndef _Included_android_app_Activity
#define _Included_android_app_Activity
#ifdef __cplusplus
extern "C" {
#endif
#undef android_app_Activity_MODE_PRIVATE
#define android_app_Activity_MODE_PRIVATE 0L
/*
 * Class:     android_app_Activity
 * Method:    nativeFinish
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_android_app_Activity_nativeFinish
  (JNIEnv *, jobject, jlong);

/*
 * Class:     android_app_Activity
 * Method:    nativeStartActivity
 * Signature: (Landroid/app/Activity;)V
 */
JNIEXPORT void JNICALL Java_android_app_Activity_nativeStartActivity
  (JNIEnv *, jclass, jobject);

/*
 * Class:     android_app_Activity
 * Method:    nativeOpenURI
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_android_app_Activity_nativeOpenURI
  (JNIEnv *, jclass, jstring);

/*
 * Class:     android_app_Activity
 * Method:    nativeShare
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_android_app_Activity_nativeShare
  (JNIEnv *, jclass, jstring);

#ifdef __cplusplus
}
#endif
#endif
