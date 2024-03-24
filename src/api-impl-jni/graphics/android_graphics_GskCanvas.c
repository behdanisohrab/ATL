#include <gtk/gtk.h>
#include <graphene.h>

#include "../defines.h"
#include "../util.h"

#include "../generated_headers/android_graphics_GskCanvas.h"

JNIEXPORT void JNICALL Java_android_graphics_GskCanvas_native_1drawBitmap(JNIEnv *env, jclass this_class, jlong snapshot_ptr, jlong pixbuf_ptr, jint x, jint y, jint width, jint height, jint color)
{
	GdkSnapshot *snapshot = (GdkSnapshot *)_PTR(snapshot_ptr);
	GdkPixbuf *pixbuf = (GdkPixbuf *)_PTR(pixbuf_ptr);
	GdkTexture *texture = gdk_texture_new_for_pixbuf(pixbuf);
	if (color) {  // use only alpha from pixbuf, color is fixed
		graphene_matrix_t color_matrix;
		graphene_vec4_t color_offset;
		graphene_matrix_init_from_float(&color_matrix, (float[]){
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 1,
		});
		graphene_vec4_init(&color_offset, ((color >> 16) & 0xFF) / 255.f, ((color >> 8) & 0xFF) / 255.f, ((color >> 0) & 0xFF) / 255.f, 0);
		gtk_snapshot_push_color_matrix(snapshot, &color_matrix, &color_offset);
	}
	gtk_snapshot_append_texture(snapshot, texture, &GRAPHENE_RECT_INIT(x, y, width, height));
	if (color)
		gtk_snapshot_pop(snapshot);
	g_object_unref(texture);
}
