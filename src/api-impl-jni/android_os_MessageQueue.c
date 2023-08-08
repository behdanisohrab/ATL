#include "defines.h"
#include "util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <glib.h>

#include "generated_headers/android_os_MessageQueue.h"

/* TODO put these in a header */
typedef void ALooper;
ALooper * ALooper_prepare(void);
void ALooper_wake(ALooper *looper);
bool ALooper_isPolling(ALooper *looper);
int ALooper_pollOnce(int timeoutMillis, int* outFd, int* outEvents, void** outData);

struct native_message_queue {
	ALooper *looper;
	bool in_callback;
	bool is_main_thread;
};

static GThread *main_thread_id;

void prepare_main_looper(JNIEnv* env) {
	main_thread_id = g_thread_self();

	jclass class = (*env)->FindClass(env, "android/os/Looper");
	(*env)->CallStaticVoidMethod(env, class, _STATIC_METHOD(class, "prepareMainLooper", "()V"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	(*env)->CallStaticVoidMethod(env, class, _STATIC_METHOD(class, "loop", "()V"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

JNIEXPORT jlong JNICALL Java_android_os_MessageQueue_nativeInit(JNIEnv *env, jclass this)
{
	struct native_message_queue *message_queue = malloc(sizeof(struct native_message_queue));

	message_queue->in_callback = false;
	message_queue->looper = ALooper_prepare();
	message_queue->is_main_thread = g_thread_self() == main_thread_id;

	return _INTPTR(message_queue);
}

JNIEXPORT void JNICALL Java_android_os_MessageQueue_nativeDestroy(JNIEnv *env, jclass this, jlong ptr)
{
	struct native_message_queue *message_queue = _PTR(ptr);

	free(message_queue);
}

/**
 * callback to execute java handlers on glib managed thread loops
 */
static gboolean glib_context_callback(gpointer user_data) {
	JavaVM *jvm = user_data;
	JNIEnv *env;
	(*jvm)->GetEnv(jvm, (void**)&env, JNI_VERSION_1_6);

	jclass class = (*env)->FindClass(env, "android/os/Looper");
	(*env)->CallStaticVoidMethod(env, class, _STATIC_METHOD(class, "loop", "()V"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	return FALSE;  // cancel timer
}

JNIEXPORT jboolean JNICALL Java_android_os_MessageQueue_nativePollOnce(JNIEnv *env, jclass this, jlong ptr, jint timeout_millis)
{
	struct native_message_queue *message_queue = _PTR(ptr);

	if (message_queue->is_main_thread) {  // thread loop is managed by glib
		JavaVM *jvm;
		(*env)->GetJavaVM(env, &jvm);
		if (timeout_millis) {
			g_timeout_add_full(0, timeout_millis, glib_context_callback, jvm, NULL);
			return true;  // indicate that java side should return to block in glib managed loop
		} else {
			return false;
		}
	}

//	printf("Java_android_os_MessageQueue_nativePollOnce: entry (timeout: %d)\n", timeout_millis);
	message_queue->in_callback = true;
	ALooper_pollOnce(timeout_millis, NULL, NULL, NULL);
	message_queue->in_callback = false;
//	printf("Java_android_os_MessageQueue_nativePollOnce: exit\n");

	/* TODO: what's with the exception stuff */
	return false;
}

JNIEXPORT void JNICALL Java_android_os_MessageQueue_nativeWake(JNIEnv *env, jclass this, jlong ptr)
{
	struct native_message_queue *message_queue = _PTR(ptr);

	if (message_queue->is_main_thread) {  // thread loop is managed by glib
		JavaVM *jvm;
		(*env)->GetJavaVM(env, &jvm);
		g_idle_add_full(0, glib_context_callback, jvm, NULL);
		return;
	}

	ALooper_wake(message_queue->looper);
}

JNIEXPORT jboolean JNICALL Java_android_os_MessageQueue_nativeIsIdling(JNIEnv *env, jclass this, jlong ptr)
{
	struct native_message_queue *message_queue = _PTR(ptr);

	return ALooper_isPolling(message_queue->looper);
}
