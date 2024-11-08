/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_app_NotificationManager */

#ifndef _Included_android_app_NotificationManager
#define _Included_android_app_NotificationManager
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     android_app_NotificationManager
 * Method:    nativeInitBuilder
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_android_app_NotificationManager_nativeInitBuilder
  (JNIEnv *, jobject);

/*
 * Class:     android_app_NotificationManager
 * Method:    nativeAddAction
 * Signature: (JLjava/lang/String;ILjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_android_app_NotificationManager_nativeAddAction
  (JNIEnv *, jobject, jlong, jstring, jint, jstring, jstring);

/*
 * Class:     android_app_NotificationManager
 * Method:    nativeShowNotification
 * Signature: (JILjava/lang/String;Ljava/lang/String;Ljava/lang/String;ZILjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_android_app_NotificationManager_nativeShowNotification
  (JNIEnv *, jobject, jlong, jint, jstring, jstring, jstring, jboolean, jint, jstring, jstring);

/*
 * Class:     android_app_NotificationManager
 * Method:    nativeShowMPRIS
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_android_app_NotificationManager_nativeShowMPRIS
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     android_app_NotificationManager
 * Method:    nativeCancel
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_android_app_NotificationManager_nativeCancel
  (JNIEnv *, jobject, jint);

/*
 * Class:     android_app_NotificationManager
 * Method:    nativeCancelMPRIS
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_android_app_NotificationManager_nativeCancelMPRIS
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
