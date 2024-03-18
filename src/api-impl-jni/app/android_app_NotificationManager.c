#include <libportal/portal.h>

#include "../defines.h"
#include "../util.h"

#include "../generated_headers/android_app_NotificationManager.h"

static XdpPortal *portal = NULL;
static GHashTable *ongoing_notifications = NULL;

JNIEXPORT jlong JNICALL Java_android_app_NotificationManager_nativeInitBuilder(JNIEnv *env, jobject this)
{
	return _INTPTR(g_variant_builder_new(G_VARIANT_TYPE("aa{sv}")));
}

static GVariant *serialize_intent(JNIEnv *env, jint type, jstring action_jstr, jstring className_jstr)
{
	const char *action = action_jstr ? (*env)->GetStringUTFChars(env, action_jstr, NULL) : NULL;
	const char *className = className_jstr ? (*env)->GetStringUTFChars(env, className_jstr, NULL) : NULL;
	GVariant *intent = g_variant_new("(iss)", type, action ?: "", className ?: "");
	if (action_jstr) (*env)->ReleaseStringUTFChars(env, action_jstr, action);
	if (className_jstr) (*env)->ReleaseStringUTFChars(env, className_jstr, className);
	return intent;
}

JNIEXPORT void JNICALL Java_android_app_NotificationManager_nativeAddAction(JNIEnv *env, jobject this, jlong builder_ptr, jstring name_jstr, jint type, jstring action, jstring className)
{
	GVariantBuilder *builder = _PTR(builder_ptr);
	g_variant_builder_open(builder, G_VARIANT_TYPE("a{sv}"));
	if (name_jstr) {
		const char *name = (*env)->GetStringUTFChars(env, name_jstr, NULL);
		g_variant_builder_add(builder, "{sv}", "label", g_variant_new_string(name));
		(*env)->ReleaseStringUTFChars(env, name_jstr, name);
	}
	g_variant_builder_add(builder, "{sv}", "action", g_variant_new_string("button-action"));
	g_variant_builder_add(builder, "{sv}", "target", serialize_intent(env, type, action, className));
	g_variant_builder_close(builder);
}

static void notification_action_invoked(XdpPortal *portal, gchar *id_str, gchar *action, GVariant *parameter, gpointer user_data)
{
	int id = atoi(id_str);
	int type;
	const char *actionName;
	const char *className;
	GVariant *target;
	JNIEnv *env = get_jni_env();

	GVariantIter *iter = g_variant_iter_new(parameter);
	g_variant_iter_next(iter, "v", &target);
	g_variant_get(target, "(iss)", &type, &actionName, &className);
	jmethodID notificationActionCallback = _STATIC_METHOD((*env)->FindClass(env, "android/app/NotificationManager"), "notificationActionCallback", "(IILjava/lang/String;Ljava/lang/String;)V");
	(*env)->CallStaticVoidMethod(env, (*env)->FindClass(env, "android/app/NotificationManager"), notificationActionCallback, id, type, _JSTRING(actionName), _JSTRING(className));
	g_variant_iter_free(iter);
	g_variant_unref(target);
}

// gnome session locks up when we send notification update before last update was processed
static int callback_pending = 0;
static void natification_callback(GObject* source_object, GAsyncResult* res, gpointer data)
{
	callback_pending = 0;
}

JNIEXPORT void JNICALL Java_android_app_NotificationManager_nativeShowNotification(JNIEnv *env, jobject this, jlong builder_ptr, jint id, jstring title_jstr, jstring text_jstr, jstring icon_jstr, jboolean ongoing, jint type, jstring action, jstring className)
{
	if (callback_pending) {
		return;
	}
	if (!portal) {
		portal = xdp_portal_new();
		g_signal_connect(portal, "notification-action-invoked", G_CALLBACK(notification_action_invoked), NULL);
		ongoing_notifications = g_hash_table_new(NULL, NULL);
	}

	GVariantBuilder *builder = _PTR(builder_ptr);
	GVariant *buttons = g_variant_builder_end(builder);

	g_variant_builder_init(builder, G_VARIANT_TYPE("a{sv}"));
	if (title_jstr) {
		const char *title = (*env)->GetStringUTFChars(env, title_jstr, NULL);
		g_variant_builder_add(builder, "{sv}", "title", g_variant_new_string(title));
		(*env)->ReleaseStringUTFChars(env, title_jstr, title);
	}
	if (text_jstr) {
		const char *text = (*env)->GetStringUTFChars(env, text_jstr, NULL);
		g_variant_builder_add(builder, "{sv}", "body", g_variant_new_string(text));
		(*env)->ReleaseStringUTFChars(env, text_jstr, text);
	}
	if (icon_jstr) {
		const char *icon_path = (*env)->GetStringUTFChars(env, icon_jstr, NULL);
		extract_from_apk(icon_path, icon_path);
		char *icon_path_full = g_strdup_printf("%s/%s", get_app_data_dir(), icon_path);
		GMappedFile *icon_file = g_mapped_file_new(icon_path_full, FALSE, NULL);
		GBytes *icon_bytes = g_mapped_file_get_bytes(icon_file);
		GIcon *icon = g_bytes_icon_new(icon_bytes);
		GVariant *icon_serialized = g_icon_serialize(icon);
		g_variant_builder_add(builder, "{sv}", "icon", icon_serialized);
		g_variant_unref(icon_serialized);
		g_object_unref(icon);
		g_bytes_unref(icon_bytes);
		g_mapped_file_unref(icon_file);
		g_free(icon_path_full);
		(*env)->ReleaseStringUTFChars(env, icon_jstr, icon_path);
	}
	g_variant_builder_add(builder, "{sv}", "default-action", g_variant_new_string("default-action"));
	g_variant_builder_add(builder, "{sv}", "default-action-target", serialize_intent(env, type, action, className));
	g_variant_builder_add(builder, "{sv}", "buttons", buttons);
	GVariant *variant = g_variant_builder_end(builder);
	g_variant_builder_unref(builder);
	char *id_string = g_strdup_printf("%d", id);
	xdp_portal_remove_notification(portal, id_string);
	callback_pending = 1;
	xdp_portal_add_notification(portal, id_string, variant, XDP_NOTIFICATION_FLAG_NONE, NULL, natification_callback, NULL);
	g_free(id_string);
	if (ongoing)
		g_hash_table_add(ongoing_notifications, GINT_TO_POINTER(id));
}

static void remove_ongoing_notification(gpointer key, gpointer value, gpointer user_data)
{
	char *id_string = g_strdup_printf("%d", GPOINTER_TO_INT(key));
	xdp_portal_remove_notification(portal, id_string);
	g_free(id_string);
}

void remove_ongoing_notifications()
{
	if (ongoing_notifications)
		g_hash_table_foreach(ongoing_notifications, remove_ongoing_notification, NULL);
}
