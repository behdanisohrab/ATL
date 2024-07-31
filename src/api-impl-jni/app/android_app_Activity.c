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
	if(!_GET_BOOL_FIELD(activity, "paused")) {
		(*env)->CallVoidMethod(env, activity, handle_cache.activity.onPause);
		if((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);
	}

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

static jobject activity_not_created = NULL;

void _activity_start(JNIEnv *env, jobject activity_object, bool recreate)
{
	/* -- run the activity's onCreate -- */
	(*env)->CallVoidMethod(env, activity_object, handle_cache.activity.onCreate, NULL);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	if ((*env)->IsSameObject(env, activity_object, activity_not_created)) { // finish() was called before the activity was created
		_UNREF(activity_not_created);
		activity_not_created = NULL;
		return;
	}

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
	if (removed_activity) {
		activity_close(env, removed_activity);
		_UNREF(removed_activity);
	} else {
		activity_not_created = _REF(this);
	}
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

struct filechooser_callback_data { jobject activity; jint request_code; jint action; };

#define RESULT_OK -1
#define RESULT_CANCELED 0

#if GTK_CHECK_VERSION(4, 10, 0)
static void file_dialog_callback(GObject* source_object, GAsyncResult* res, gpointer data) {
	struct filechooser_callback_data *d = data;
	GtkFileDialog *dialog = GTK_FILE_DIALOG(source_object);
	GFile *(*const finish_functions[])(GtkFileDialog *, GAsyncResult *, GError **) = {
		gtk_file_dialog_open_finish,
		gtk_file_dialog_save_finish,
		gtk_file_dialog_select_folder_finish,
	};

	GFile *file = finish_functions[d->action](dialog, res, NULL);
	JNIEnv *env = get_jni_env();
	jmethodID fileChooserResultCallback = _METHOD(handle_cache.activity.class, "fileChooserResultCallback", "(IIILjava/lang/String;)V");

	if (file) {
		char *uri = g_file_get_uri(file);
		
		(*env)->CallVoidMethod(env, d->activity, fileChooserResultCallback, d->request_code, RESULT_OK, d->action, _JSTRING(uri));
		if ((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);

		g_free(uri);
		g_object_unref(file);
	} else {
		(*env)->CallVoidMethod(env, d->activity, fileChooserResultCallback, d->request_code, RESULT_CANCELED, d->action, NULL);
	}
	free(d);
}
#else
static void on_filechooser_response(GtkNativeDialog *native, int response, struct filechooser_callback_data *data)
{
	JNIEnv *env = get_jni_env();
	jmethodID fileChooserResultCallback = _METHOD(handle_cache.activity.class, "fileChooserResultCallback", "(IIILjava/lang/String;)V");

	GtkFileChooser *chooser = GTK_FILE_CHOOSER(native);
	GtkFileChooserAction action = gtk_file_chooser_get_action(chooser);
	if (response == GTK_RESPONSE_ACCEPT) {
		GFile *file = gtk_file_chooser_get_file(chooser);
		char *uri = g_file_get_uri(file);
		
		(*env)->CallVoidMethod(env, data->activity, fileChooserResultCallback, data->request_code, RESULT_OK, action, _JSTRING(uri));
		if ((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);

		g_free(uri);
		g_object_unref(file);
	} else {
		(*env)->CallVoidMethod(env, data->activity, fileChooserResultCallback, data->request_code, RESULT_CANCELED, action, NULL);
	}

	g_object_unref(native);
	_UNREF(data->activity);
	free(data);
}
#endif

JNIEXPORT void JNICALL Java_android_app_Activity_nativeFileChooser(JNIEnv *env, jobject this, jint action, jstring type_jstring, jstring filename_jstring, jint request_code)
{
	const char *chooser_title = ((char *[]){"Open File", "Save File", "Select Folder"})[action];
#if GTK_CHECK_VERSION(4, 10, 0)
	GtkFileDialog *dialog = gtk_file_dialog_new();
	gtk_file_dialog_set_title(GTK_FILE_DIALOG(dialog), chooser_title);
#else
	GtkFileChooserNative *native = gtk_file_chooser_native_new(chooser_title, window, action, NULL, NULL);
#endif

	const char *type = type_jstring ? (*env)->GetStringUTFChars(env, type_jstring, NULL) : NULL;
	if (type) {
		GtkFileFilter *filter = gtk_file_filter_new();
		gtk_file_filter_add_mime_type(filter, type);
		gtk_file_filter_set_name(filter, type);
#if GTK_CHECK_VERSION(4, 10, 0)
		gtk_file_dialog_set_default_filter(GTK_FILE_DIALOG(dialog), filter);
#else
		gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(native), filter);
#endif
		(*env)->ReleaseStringUTFChars(env, type_jstring, type);
	}
	const char *filename = filename_jstring ? (*env)->GetStringUTFChars(env, filename_jstring, NULL) : NULL;
	if (filename) {
#if GTK_CHECK_VERSION(4, 10, 0)
		gtk_file_dialog_set_initial_name(GTK_FILE_DIALOG(dialog), filename);
#else
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(native), filename);
#endif
		(*env)->ReleaseStringUTFChars(env, filename_jstring, filename);
	}

	struct filechooser_callback_data *callback_data = malloc(sizeof(struct filechooser_callback_data));
	callback_data->activity = _REF(this);
	callback_data->request_code = request_code;
	callback_data->action = action;
#if GTK_CHECK_VERSION(4, 10, 0)
	void (* const file_dialog_functions[])(GtkFileDialog *, GtkWindow *, GCancellable *, GAsyncReadyCallback, gpointer) = {
		gtk_file_dialog_open,
		gtk_file_dialog_save,
		gtk_file_dialog_select_folder,
	};
	file_dialog_functions[action](dialog, window, NULL, file_dialog_callback, callback_data);
#else
	g_signal_connect (native, "response", G_CALLBACK(on_filechooser_response), callback_data);
	gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
#endif
}
