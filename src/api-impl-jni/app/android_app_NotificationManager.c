#include <gtk/gtk.h>
#include <libportal/portal.h>

#include "../defines.h"
#include "../util.h"

#include "mpris-dbus.h"

#include "../generated_headers/android_app_NotificationManager.h"

#define MPRIS_BUS_NAME_PREFIX "org.mpris.MediaPlayer2."
#define MPRIS_OBJECT_NAME "/org/mpris/MediaPlayer2"

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

JNIEXPORT void JNICALL Java_android_app_NotificationManager_nativeCancel(JNIEnv *env, jobject this, jint id)
{
	char *id_string = g_strdup_printf("%d", id);
	if (portal)
		xdp_portal_remove_notification(portal, id_string);
	g_free(id_string);
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

static MediaPlayer2 *mpris = NULL;
static int dbus_name_id = 0;
extern MediaPlayer2Player *mpris_player;
extern GtkWindow *window;

static gboolean on_media_player_handle_raise(MediaPlayer2 *mpris, GDBusMethodInvocation *invocation, gpointer user_data)
{
	gtk_window_present(window);
	media_player2_complete_raise(mpris, invocation);
	return TRUE;
}

static void on_bus_acquired(GDBusConnection *connection, const char *name, gpointer user_data)
{
	g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(mpris),
	                                 connection, MPRIS_OBJECT_NAME, NULL);

	g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(mpris_player),
	                                 connection, MPRIS_OBJECT_NAME, NULL);
}

JNIEXPORT void JNICALL Java_android_app_NotificationManager_nativeShowMPRIS(JNIEnv *env, jobject this, jstring package_name_jstr, jstring identity_jstr)
{
	if (!mpris) {
		mpris = media_player2_skeleton_new();
		g_signal_connect(mpris, "handle-raise", G_CALLBACK(on_media_player_handle_raise), NULL);
	}
	const char *package_name = NULL;
	const char *app_id = g_application_get_application_id(G_APPLICATION(gtk_window_get_application(window)));
	if ((app_id == NULL || strcmp(app_id, "com.example.demo_application") == 0) && package_name_jstr) {
		// fall back to package name
		app_id = package_name = (*env)->GetStringUTFChars(env, package_name_jstr, NULL);
	}
	if (!dbus_name_id) {
		gchar *bus_name = g_strdup_printf("%s%s", MPRIS_BUS_NAME_PREFIX, app_id);
		dbus_name_id = g_bus_own_name(G_BUS_TYPE_SESSION, bus_name, G_BUS_NAME_OWNER_FLAGS_NONE,
		               on_bus_acquired, NULL, NULL, mpris, NULL);
		g_free(bus_name);
	}
	media_player2_set_can_raise(mpris, TRUE);
	media_player2_set_desktop_entry(mpris, app_id);
	if (package_name) {
		(*env)->ReleaseStringUTFChars(env, package_name_jstr, package_name);
	}
	if (identity_jstr) {
		const char *identity = (*env)->GetStringUTFChars(env, identity_jstr, NULL);
		media_player2_set_identity(mpris, identity);
		(*env)->ReleaseStringUTFChars(env, identity_jstr, identity);
	}
}

JNIEXPORT void JNICALL Java_android_app_NotificationManager_nativeCancelMPRIS(JNIEnv *env, jobject this)
{
	if (dbus_name_id) {
		g_dbus_interface_skeleton_unexport(G_DBUS_INTERFACE_SKELETON(mpris));
		g_dbus_interface_skeleton_unexport(G_DBUS_INTERFACE_SKELETON(mpris_player));
		g_clear_handle_id (&dbus_name_id, g_bus_unown_name);
	}
}
