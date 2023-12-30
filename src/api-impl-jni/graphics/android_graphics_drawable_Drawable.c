#include <gdk/gdk.h>
#include <string.h>

#include "../defines.h"
#include "NinePatchPaintable.h"
#include "../generated_headers/android_graphics_drawable_Drawable.h"

JNIEXPORT jlong JNICALL Java_android_graphics_drawable_Drawable_native_1paintable_1from_1path(JNIEnv *env, jclass class, jstring pathStr) {
	const char *path = (*env)->GetStringUTFChars(env, pathStr, NULL);
	GdkPaintable *paintable = NULL;

	// check if path ends with .9.png
	int len = strlen(path);
	if (len >= 6 && !strcmp(path + len - 6, ".9.png")) {
		paintable = ninepatch_paintable_new(path);
	}
	if (!paintable)
		paintable = GDK_PAINTABLE(gdk_texture_new_from_filename(path, NULL));

	(*env)->ReleaseStringUTFChars(env, pathStr, path);
	return _INTPTR(paintable);
}
