/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_media_MediaCodec */

#ifndef _Included_android_media_MediaCodec
#define _Included_android_media_MediaCodec
#ifdef __cplusplus
extern "C" {
#endif
#undef android_media_MediaCodec_BUFFER_FLAG_END_OF_STREAM
#define android_media_MediaCodec_BUFFER_FLAG_END_OF_STREAM 4L
/*
 * Class:     android_media_MediaCodec
 * Method:    native_constructor
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_android_media_MediaCodec_native_1constructor
  (JNIEnv *, jobject, jstring);

/*
 * Class:     android_media_MediaCodec
 * Method:    native_configure_audio
 * Signature: (JLjava/nio/ByteBuffer;II)V
 */
JNIEXPORT void JNICALL Java_android_media_MediaCodec_native_1configure_1audio
  (JNIEnv *, jobject, jlong, jobject, jint, jint);

/*
 * Class:     android_media_MediaCodec
 * Method:    native_configure_video
 * Signature: (JLjava/nio/ByteBuffer;Ljava/nio/ByteBuffer;Landroid/view/Surface;)V
 */
JNIEXPORT void JNICALL Java_android_media_MediaCodec_native_1configure_1video
  (JNIEnv *, jobject, jlong, jobject, jobject, jobject);

/*
 * Class:     android_media_MediaCodec
 * Method:    native_start
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_android_media_MediaCodec_native_1start
  (JNIEnv *, jobject, jlong);

/*
 * Class:     android_media_MediaCodec
 * Method:    native_queueInputBuffer
 * Signature: (JLjava/nio/ByteBuffer;J)I
 */
JNIEXPORT jint JNICALL Java_android_media_MediaCodec_native_1queueInputBuffer
  (JNIEnv *, jobject, jlong, jobject, jlong);

/*
 * Class:     android_media_MediaCodec
 * Method:    native_dequeueOutputBuffer
 * Signature: (JLjava/nio/ByteBuffer;Landroid/media/MediaCodec/BufferInfo;)I
 */
JNIEXPORT jint JNICALL Java_android_media_MediaCodec_native_1dequeueOutputBuffer
  (JNIEnv *, jobject, jlong, jobject, jobject);

/*
 * Class:     android_media_MediaCodec
 * Method:    native_releaseOutputBuffer
 * Signature: (JLjava/nio/ByteBuffer;Z)V
 */
JNIEXPORT void JNICALL Java_android_media_MediaCodec_native_1releaseOutputBuffer
  (JNIEnv *, jobject, jlong, jobject, jboolean);

/*
 * Class:     android_media_MediaCodec
 * Method:    native_release
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_android_media_MediaCodec_native_1release
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
