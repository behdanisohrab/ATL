#define _GNU_SOURCE

#include <gtk/gtk.h>
#include <string.h>

#include "../defines.h"
#include "../util.h"

#include "../generated_headers/android_content_Context.h"

JNIEXPORT void JNICALL Java_android_content_Context_native_1updateConfig(JNIEnv *env, jclass class, jobject config)
{
	GtkSettings *settings = gtk_settings_get_default();
	gchar *theme_name;
	g_object_get(settings, "gtk-theme-name", &theme_name, NULL);
	bool night_mode = strcasestr(theme_name, "dark") || strcasestr(theme_name, "black");
	if (night_mode) {
		_SET_INT_FIELD(config, "uiMode", /*UI_MODE_NIGHT_YES*/ 0x20);
	}
}