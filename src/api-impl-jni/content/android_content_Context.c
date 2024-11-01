#define _GNU_SOURCE

#include <gtk/gtk.h>
#include <string.h>
#include <libportal/portal.h>
#include <libportal/settings.h>

#include "portal-openuri.h"

#include "../defines.h"
#include "../util.h"

#include "../generated_headers/android_content_Context.h"

extern char *apk_path;

JNIEXPORT jstring JNICALL Java_android_content_Context_native_1get_1apk_1path(JNIEnv *env, jclass this) {
	return _JSTRING(apk_path);
}

static void settings_changed_cb(XdpSettings *xdp_settings, gchar *namestpace, gchar *key, GVariant* value, jobject configuration)
{
	JNIEnv *env;
	if (!strcmp(namestpace, "org.freedesktop.appearance") && !strcmp(key, "color-scheme")) {
		int color_sheme = g_variant_get_uint32(value);
		g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", color_sheme == 1, NULL);
		env = get_jni_env();
		if (!configuration) {
			jobject resources = _GET_STATIC_OBJ_FIELD(handle_cache.context.class, "r", "Landroid/content/res/Resources;");
			configuration = _GET_OBJ_FIELD(resources, "mConfiguration", "Landroid/content/res/Configuration;");
		}
		if (color_sheme == 1)  // Prefer dark appearance
			_SET_INT_FIELD(configuration, "uiMode", /*UI_MODE_NIGHT_YES*/ 0x20);
		else if (color_sheme == 2)  // Prefer light appearance
			_SET_INT_FIELD(configuration, "uiMode", /*UI_MODE_NIGHT_NO*/ 0x10);
		else  // No preference
			_SET_INT_FIELD(configuration, "uiMode", /*UI_MODE_NIGHT_UNDEFINED*/ 0x00);
	}
}

static XdpSettings *xdp_settings = NULL;

JNIEXPORT void JNICALL Java_android_content_Context_native_1updateConfig(JNIEnv *env, jclass this, jobject config)
{
	if (!xdp_settings) {
		XdpPortal *portal = xdp_portal_new();
		xdp_settings = xdp_portal_get_settings(portal);
		g_object_unref(portal);
		g_signal_connect(xdp_settings, "changed", G_CALLBACK(settings_changed_cb), NULL);
	}
	GVariant *color_sheme = xdp_settings_read_value(xdp_settings, "org.freedesktop.appearance", "color-scheme", NULL, NULL);
	if (color_sheme) {
		settings_changed_cb(xdp_settings, "org.freedesktop.appearance", "color-scheme", color_sheme, config);
		g_variant_unref(color_sheme);
	}
}

JNIEXPORT void JNICALL Java_android_content_Context_nativeOpenFile(JNIEnv *env, jclass class, jint fd)
{
	GDBusConnection *connection = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, NULL);
	OpenURI *openuri = open_uri_proxy_new_sync(connection, 0, "org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop", NULL, NULL);
	GVariantBuilder opt_builder;
	g_variant_builder_init(&opt_builder, G_VARIANT_TYPE_VARDICT);
	GUnixFDList *fd_list = g_unix_fd_list_new_from_array(&fd, 1);
	open_uri_call_open_file_sync(openuri, "", g_variant_new("h", 0), g_variant_builder_end(&opt_builder), fd_list, NULL, NULL, NULL, NULL);
	g_object_unref(fd_list);
	g_object_unref(openuri);
	g_object_unref(connection);
}
