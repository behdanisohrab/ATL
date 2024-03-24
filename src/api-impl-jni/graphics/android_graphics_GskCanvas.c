#include <gtk/gtk.h>
#include <graphene.h>

#include "../defines.h"
#include "../util.h"

#include "../generated_headers/android_graphics_GskCanvas.h"

JNIEXPORT void JNICALL Java_android_graphics_GskCanvas_native_1drawBitmap(JNIEnv *env, jclass this_class, jlong snapshot_ptr, jlong pixbuf_ptr, jint x, jint y, jint width, jint height)
{
	GdkSnapshot *snapshot = (GdkSnapshot *)_PTR(snapshot_ptr);
	GdkPixbuf *pixbuf = (GdkPixbuf *)_PTR(pixbuf_ptr);
	GdkTexture *texture = gdk_texture_new_for_pixbuf(pixbuf);
	gtk_snapshot_append_texture(snapshot, texture, &GRAPHENE_RECT_INIT(x, y, width, height));
	g_object_unref(texture);
}
