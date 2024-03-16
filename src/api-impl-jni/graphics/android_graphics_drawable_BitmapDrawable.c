#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

JNIEXPORT jlong JNICALL Java_android_graphics_drawable_BitmapDrawable_native_1paintable_1from_1pixbuf(JNIEnv *env, jclass this, jlong _pixbuf)
{
	GdkPixbuf *pixbuf = (GdkPixbuf *)_PTR(_pixbuf);

	return _INTPTR(gdk_texture_new_for_pixbuf(pixbuf));
}
