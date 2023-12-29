#include <gdk/gdk.h>

#include "../defines.h"
#include "../generated_headers/android_graphics_drawable_Drawable.h"

JNIEXPORT jlong JNICALL Java_android_graphics_drawable_Drawable_native_1paintable_1from_1path(JNIEnv *env, jclass class, jstring pathStr) {
	const char *path = (*env)->GetStringUTFChars(env, pathStr, NULL);

	GdkPaintable *paintable = GDK_PAINTABLE(gdk_texture_new_from_filename(path, NULL));

	(*env)->ReleaseStringUTFChars(env, pathStr, path);
	return _INTPTR(paintable);
}
