#include "defines.h"
#include "util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* TODO put these in a header */
typedef void ALooper;
ALooper * ALooper_prepare(void);
void ALooper_wake(ALooper *looper);
bool ALooper_isPolling(ALooper *looper);
int ALooper_pollOnce(int timeoutMillis, int* outFd, int* outEvents, void** outData);

struct native_message_queue {
	ALooper *looper;
	bool in_callback;
};

JNIEXPORT jlong JNICALL Java_android_os_MessageQueue_nativeInit(JNIEnv *env, jclass this)
{
	struct native_message_queue *message_queue = malloc(sizeof(struct native_message_queue));

	message_queue->in_callback = false;
	message_queue->looper = ALooper_prepare();

	return _INTPTR(message_queue);
}

JNIEXPORT void JNICALL Java_android_os_MessageQueue_nativeDestroy(JNIEnv *env, jclass this, jlong ptr)
{
	struct native_message_queue *message_queue = _PTR(ptr);

	free(message_queue);
}

JNIEXPORT void JNICALL Java_android_os_MessageQueue_nativePollOnce(JNIEnv *env, jclass this, jlong ptr, jint timeout_millis)
{
	struct native_message_queue *message_queue = _PTR(ptr);

//	printf("Java_android_os_MessageQueue_nativePollOnce: entry (timeout: %d)\n", timeout_millis);
	message_queue->in_callback = true;
	ALooper_pollOnce(timeout_millis, NULL, NULL, NULL);
	message_queue->in_callback = false;
//	printf("Java_android_os_MessageQueue_nativePollOnce: exit\n");

	/* TODO: what's with the exception stuff */
}

JNIEXPORT void JNICALL Java_android_os_MessageQueue_nativeWake(JNIEnv *env, jclass this, jlong ptr)
{
	struct native_message_queue *message_queue = _PTR(ptr);

	ALooper_wake(message_queue->looper);
}

JNIEXPORT jboolean JNICALL Java_android_os_MessageQueue_nativeIsIdling(JNIEnv *env, jclass this, jlong ptr)
{
	struct native_message_queue *message_queue = _PTR(ptr);

	return ALooper_isPolling(message_queue->looper);
}
