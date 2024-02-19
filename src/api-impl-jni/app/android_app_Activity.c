#include <gtk/gtk.h>
#include <libportal/portal.h>

#include <jni.h>

#include "../defines.h"
#include "../util.h"
#include "android_app_Activity.h"
#include "../generated_headers/android_app_Activity.h"

static GList *activity_backlog = NULL;
static jobject activity_current = NULL;
static jobject activity_toplevel_current = NULL;

static void activity_close(JNIEnv *env, jobject activity)
{
	// in case some exception was left unhandled in native code, print it here so we don't confuse it with an exception thrown by onDestroy
	if((*env)->ExceptionCheck(env)) {
		fprintf(stderr, "activity.onDestroy: seems there was a pending exception... :");
		(*env)->ExceptionDescribe(env);
	}

	/* -- run the activity's onDestroy -- */
	(*env)->CallVoidMethod(env, activity, handle_cache.activity.onDestroy);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

static void activity_unfocus(JNIEnv *env, jobject activity)
{
	(*env)->CallVoidMethod(env, activity, handle_cache.activity.onPause);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	(*env)->CallVoidMethod(env, activity, handle_cache.activity.onStop);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	(*env)->CallVoidMethod(env, activity, handle_cache.activity.onWindowFocusChanged, false);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

static void activity_focus(JNIEnv *env, jobject activity)
{
	(*env)->CallVoidMethod(env, activity, handle_cache.activity.onStart);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	(*env)->CallVoidMethod(env, activity, handle_cache.activity.onResume);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	(*env)->CallVoidMethod(env, activity, handle_cache.activity.onWindowFocusChanged, true);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

static void activity_update_current(JNIEnv *env)
{
	jobject activity_new = activity_backlog ? g_list_first(activity_backlog)->data : NULL;
	jobject activity_toplevel_new = activity_backlog ? g_list_last(activity_backlog)->data : NULL;

	if (activity_current != activity_new) {
		if (activity_current)
			activity_unfocus(env, activity_current);

		if (activity_new)
			activity_focus(env, activity_new);

		activity_current = activity_new;
	}

	/* for Activity.recreate */
	if(activity_toplevel_current != activity_toplevel_new) {
		if (activity_toplevel_current)
			activity_unfocus(env, activity_toplevel_current);

		if (activity_toplevel_new)
			activity_focus(env, activity_toplevel_new);

		activity_toplevel_current = activity_toplevel_new;
	}
}

void activity_window_ready(void)
{
	JNIEnv *env = get_jni_env();

	for (GList *l = activity_backlog; l != NULL; l = l->next) {
		(*env)->CallVoidMethod(env, l->data, handle_cache.activity.onWindowFocusChanged, true);
		if((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);
	}
}

void activity_close_all(void)
{
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

void _activity_start(JNIEnv *env, jobject activity_object, bool recreate)
{
	/* -- run the activity's onCreate -- */
	(*env)->CallVoidMethod(env, activity_object, handle_cache.activity.onCreate, NULL);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	if(recreate) // only allowed for toplevel, so we know for sure where in the stack it belongs
		activity_backlog = g_list_append(activity_backlog, _REF(activity_object));
	else
		activity_backlog = g_list_prepend(activity_backlog, _REF(activity_object));

	activity_update_current(env);

}

void activity_start(JNIEnv *env, jobject activity_object)
{
	_activity_start(env, activity_object, false);
}

JNIEXPORT void JNICALL Java_android_app_Activity_nativeFinish(JNIEnv *env, jobject this, jlong window)
{
	GList *l;
	jobject removed_activity = NULL;
	for (l = activity_backlog; l != NULL; l = l->next) {
		if ((*env)->IsSameObject(env, this, l->data)) {
			removed_activity = l->data;
			activity_backlog = g_list_delete_link(activity_backlog, l);
			break;
		}
	}
	activity_update_current(env);
	activity_close(env, this);
	if (removed_activity)
		_UNREF(removed_activity);
	if (activity_backlog == NULL && window)
		gtk_window_close(GTK_WINDOW(_PTR(window)));
}

JNIEXPORT void JNICALL Java_android_app_Activity_nativeRecreateActivity(JNIEnv *env, jclass class, jobject activity)
{
	_activity_start(env, activity, true);
}


JNIEXPORT void JNICALL Java_android_app_Activity_nativeStartActivity(JNIEnv *env, jclass class, jobject activity)
{
	activity_start(env, activity);
}

JNIEXPORT void JNICALL Java_android_app_Activity_nativeOpenURI(JNIEnv *env, jclass class, jstring uriString)
{
	static XdpPortal *portal = NULL;
	if (!portal) {
		portal = xdp_portal_new();
	}

	const char *uri = (*env)->GetStringUTFChars(env, uriString, NULL);
	xdp_portal_open_uri(portal, NULL, uri, XDP_OPEN_URI_FLAG_NONE, NULL, NULL, NULL);
	(*env)->ReleaseStringUTFChars(env, uriString, uri);
}

extern GtkWindow *window; // TODO: get this in a better way

JNIEXPORT void JNICALL Java_android_app_Activity_nativeShare(JNIEnv *env, jclass class, jstring text_jstring)
{
	const char *text = (*env)->GetStringUTFChars(env, text_jstring, NULL);
	GdkClipboard *clipboard = gdk_display_get_clipboard(gtk_root_get_display(GTK_ROOT(window)));
	gdk_clipboard_set_text(clipboard, text);
	(*env)->ReleaseStringUTFChars(env, text_jstring, text);
}
