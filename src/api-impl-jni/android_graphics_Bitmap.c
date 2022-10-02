#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "generated_headers/android_graphics_Bitmap.h"

JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1bitmap_1from_1path(JNIEnv *env, jobject this, jobject path)
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(_CSTRING(path), NULL);
	printf(">>> made pixbuf from path: >%s<, >%p<\n", _CSTRING(path), pixbuf);
	g_object_ref(pixbuf);
	return _INTPTR(pixbuf);
}

JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_getWidth(JNIEnv *env, jobject this)
{
	GdkPixbuf *pixbuf = _PTR(_GET_LONG_FIELD(this, "pixbuf"));

	return gdk_pixbuf_get_width(pixbuf);
}

JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_getHeight(JNIEnv *env, jobject this)
{
	GdkPixbuf *pixbuf = _PTR(_GET_LONG_FIELD(this, "pixbuf"));

	return gdk_pixbuf_get_height(pixbuf);
}
