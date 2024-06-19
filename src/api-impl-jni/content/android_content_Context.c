#define _GNU_SOURCE

#include <gtk/gtk.h>
#include <string.h>

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
