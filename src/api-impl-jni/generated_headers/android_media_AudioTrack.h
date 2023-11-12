/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_media_AudioTrack */

#ifndef _Included_android_media_AudioTrack
#define _Included_android_media_AudioTrack
#ifdef __cplusplus
extern "C" {
#endif
#undef android_media_AudioTrack_PLAYSTATE_STOPPED
#define android_media_AudioTrack_PLAYSTATE_STOPPED 1L
#undef android_media_AudioTrack_PLAYSTATE_PAUSED
#define android_media_AudioTrack_PLAYSTATE_PAUSED 2L
#undef android_media_AudioTrack_PLAYSTATE_PLAYING
#define android_media_AudioTrack_PLAYSTATE_PLAYING 3L
/*
 * Class:     android_media_AudioTrack
 * Method:    native_constructor
 * Signature: (IIIIII)V
 */
JNIEXPORT void JNICALL Java_android_media_AudioTrack_native_1constructor
  (JNIEnv *, jobject, jint, jint, jint, jint, jint, jint);

/*
 * Class:     android_media_AudioTrack
 * Method:    getMinBufferSize
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_android_media_AudioTrack_getMinBufferSize
  (JNIEnv *, jclass, jint, jint, jint);

/*
 * Class:     android_media_AudioTrack
 * Method:    native_play
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_android_media_AudioTrack_native_1play
  (JNIEnv *, jobject);

/*
 * Class:     android_media_AudioTrack
 * Method:    native_pause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_android_media_AudioTrack_native_1pause
  (JNIEnv *, jobject);

/*
 * Class:     android_media_AudioTrack
 * Method:    native_write
 * Signature: ([BII)I
 */
JNIEXPORT jint JNICALL Java_android_media_AudioTrack_native_1write
  (JNIEnv *, jobject, jbyteArray, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
