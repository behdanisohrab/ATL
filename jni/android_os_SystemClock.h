/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_os_SystemClock */

#ifndef _Included_android_os_SystemClock
#define _Included_android_os_SystemClock
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     android_os_SystemClock
 * Method:    setCurrentTimeMillis
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_android_os_SystemClock_setCurrentTimeMillis
  (JNIEnv *, jclass, jlong);

/*
 * Class:     android_os_SystemClock
 * Method:    uptimeMillis
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_android_os_SystemClock_uptimeMillis
  (JNIEnv *, jclass);

/*
 * Class:     android_os_SystemClock
 * Method:    elapsedRealtime
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_android_os_SystemClock_elapsedRealtime
  (JNIEnv *, jclass);

/*
 * Class:     android_os_SystemClock
 * Method:    elapsedRealtimeNanos
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_android_os_SystemClock_elapsedRealtimeNanos
  (JNIEnv *, jclass);

/*
 * Class:     android_os_SystemClock
 * Method:    currentThreadTimeMillis
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_android_os_SystemClock_currentThreadTimeMillis
  (JNIEnv *, jclass);

/*
 * Class:     android_os_SystemClock
 * Method:    currentThreadTimeMicro
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_android_os_SystemClock_currentThreadTimeMicro
  (JNIEnv *, jclass);

/*
 * Class:     android_os_SystemClock
 * Method:    currentTimeMicro
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_android_os_SystemClock_currentTimeMicro
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
