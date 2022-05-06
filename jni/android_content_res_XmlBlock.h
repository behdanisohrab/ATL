/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class android_content_res_XmlBlock */

#ifndef _Included_android_content_res_XmlBlock
#define _Included_android_content_res_XmlBlock
#ifdef __cplusplus
extern "C" {
#endif
#undef android_content_res_XmlBlock_DEBUG
#define android_content_res_XmlBlock_DEBUG 0L
/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeCreate
 * Signature: ([BII)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeCreate
  (JNIEnv *, jclass, jbyteArray, jint, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetStringBlock
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetStringBlock
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeCreateParseState
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeCreateParseState
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeNext
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeNext
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetNamespace
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetNamespace
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetName
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetName
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetText
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetText
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetLineNumber
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetLineNumber
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetAttributeCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetAttributeCount
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetAttributeNamespace
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetAttributeNamespace
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetAttributeName
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetAttributeName
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetAttributeResource
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetAttributeResource
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetAttributeDataType
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetAttributeDataType
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetAttributeData
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetAttributeData
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetAttributeStringValue
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetAttributeStringValue
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetIdAttribute
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetIdAttribute
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetClassAttribute
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetClassAttribute
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetStyleAttribute
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetStyleAttribute
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeGetAttributeIndex
 * Signature: (ILjava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_android_content_res_XmlBlock_nativeGetAttributeIndex
  (JNIEnv *, jclass, jint, jstring, jstring);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeDestroyParseState
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_android_content_res_XmlBlock_nativeDestroyParseState
  (JNIEnv *, jclass, jint);

/*
 * Class:     android_content_res_XmlBlock
 * Method:    nativeDestroy
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_android_content_res_XmlBlock_nativeDestroy
  (JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif