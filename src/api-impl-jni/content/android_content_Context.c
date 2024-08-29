#define _GNU_SOURCE

#include <gtk/gtk.h>
#include <string.h>

#include "portal-openuri.h"

#include "../defines.h"
#include "../util.h"

#include "../generated_headers/android_content_Context.h"

extern char *apk_path;

JNIEXPORT jstring JNICALL Java_android_content_Context_native_1get_1apk_1path(JNIEnv *env, jclass this) {
	return _JSTRING(apk_path);
}

JNIEXPORT void JNICALL Java_android_content_Context_native_1updateConfig(JNIEnv *env, jclass this, jobject config)
{
	GtkSettings *settings = gtk_settings_get_default();
	char *theme_name;
	gboolean prefer_dark = false;
	gboolean theme_name_from_env = false;

	theme_name = getenv("GTK_THEME");
	theme_name_from_env = theme_name != NULL;
	if (!theme_name_from_env)
		g_object_get(settings, "gtk-theme-name", &theme_name, "gtk-application-prefer-dark-theme", &prefer_dark, NULL);
	bool night_mode = prefer_dark || strcasestr(theme_name, "dark") || strcasestr(theme_name, "black");
	if (night_mode) {
		_SET_INT_FIELD(config, "uiMode", /*UI_MODE_NIGHT_YES*/ 0x20);
	}
	if (!theme_name_from_env)
		g_free(theme_name);
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
