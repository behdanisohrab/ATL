#include <gtk/gtk.h>

#include <jni.h>

#include "../defines.h"
#include "../util.h"
#include "android_app_Activity.h"
#include "../generated_headers/android_app_Activity.h"

static GList *activity_backlog = NULL;
static jobject activity_current = NULL;

static void activity_close(JNIEnv *env, jobject activity) {
	// in case some exception was left unhandled in native code, print it here so we don't confuse it with an exception thrown by onDestroy
	if((*env)->ExceptionCheck(env)) {
		fprintf(stderr, "app_exit: seems there was a pending exception... :");
		(*env)->ExceptionDescribe(env);
	}

	/* -- run the main activity's onDestroy -- */
	(*env)->CallVoidMethod(env, activity, handle_cache.apk_main_activity.onDestroy, NULL);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

static void activity_update_current(JNIEnv *env) {
	jobject activity_new = activity_backlog ? g_list_first(activity_backlog)->data : NULL;

	if (activity_current != activity_new) {
		if (activity_current) {
			(*env)->CallVoidMethod(env, activity_current, handle_cache.apk_main_activity.onPause);
			if((*env)->ExceptionCheck(env))
				(*env)->ExceptionDescribe(env);

			(*env)->CallVoidMethod(env, activity_current, handle_cache.apk_main_activity.onStop);
			if((*env)->ExceptionCheck(env))
				(*env)->ExceptionDescribe(env);
		}
		if (activity_new) {
			(*env)->CallVoidMethod(env, activity_new, handle_cache.apk_main_activity.onStart);
			if((*env)->ExceptionCheck(env))
				(*env)->ExceptionDescribe(env);

			(*env)->CallVoidMethod(env, activity_new, handle_cache.apk_main_activity.onResume);
			if((*env)->ExceptionCheck(env))
				(*env)->ExceptionDescribe(env);
		}
		activity_current = activity_new;
	}
}

void activity_window_ready(void) {
	JNIEnv *env = get_jni_env();

	for (GList *l = activity_backlog; l != NULL; l = l->next) {
		(*env)->CallVoidMethod(env, l->data, handle_cache.apk_main_activity.onWindowFocusChanged, true);
		if((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);
	}
}

void activity_close_all(void) {
	GList *activities, *l;
	JNIEnv *env = get_jni_env();
	// local backup of the backlog
	activities = activity_backlog;
	// deactivate all activities
	activity_backlog = NULL;
	activity_update_current(env);
	// destroy all activities
	for (l = activities; l != NULL; l = l->next) {
		activity_close(env, l->data);
		_UNREF(l->data);
	}
	g_list_free(activities);
}

void activity_start(JNIEnv *env, jobject activity_object) {
	/* -- run the activity's onCreate -- */
	(*env)->CallVoidMethod(env, activity_object, handle_cache.apk_main_activity.onCreate, NULL);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	activity_backlog = g_list_prepend(activity_backlog, _REF(activity_object));
	activity_update_current(env);
}

JNIEXPORT void JNICALL Java_android_app_Activity_nativeFinish(JNIEnv *env, jobject this, jlong window) {
	activity_backlog = g_list_remove(activity_backlog, this);
	activity_update_current(env);
	activity_close(env, this);
	_UNREF(this);
	if (activity_backlog == NULL)
		gtk_window_close(GTK_WINDOW(_PTR(window)));
}

JNIEXPORT void JNICALL Java_android_app_Activity_nativeStartActivity(JNIEnv *env, jclass class, jobject activity) {
	activity_start(env, activity);
}