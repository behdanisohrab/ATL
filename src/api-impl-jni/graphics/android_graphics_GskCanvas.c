#include <gtk/gtk.h>
#include <graphene.h>

#include "include/c/sk_paint.h"
#include "include/c/sk_path.h"

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

JNIEXPORT void JNICALL Java_android_graphics_GskCanvas_native_1drawRect(JNIEnv *env, jclass this_class, jlong snapshot_ptr, jfloat left, jfloat top, jfloat right, jfloat bottom, jint color)
{
	GdkSnapshot *snapshot = (GdkSnapshot *)_PTR(snapshot_ptr);
	GdkRGBA gdk_color = {
		(float)((color >> 16) & 0xff) / 0xff,
		(float)((color >> 8)  & 0xff) / 0xff,
		(float)((color >> 0)  & 0xff) / 0xff,
		(float)((color >> 24) & 0xff) / 0xff,
	};
	graphene_rect_t bounds = GRAPHENE_RECT_INIT(left, top, right - left, bottom - top);
	gtk_snapshot_append_color(snapshot, &gdk_color, &bounds);
}

JNIEXPORT void JNICALL Java_android_graphics_GskCanvas_native_1drawPath(JNIEnv *env, jclass this_class, jlong snapshot_ptr, jlong path_ptr, jlong paint_ptr)
{
	GtkSnapshot *snapshot = GTK_SNAPSHOT(_PTR(snapshot_ptr));
	sk_paint_t *paint = (sk_paint_t *)_PTR(paint_ptr);
	sk_path_t *path = (sk_path_t *)_PTR(path_ptr);
	GdkRGBA gdk_color;
	sk_path_iterator_t *iterator = sk_path_create_iter(path, 0);
	sk_path_verb_t verb;
	sk_point_t line[4];
	sk_paint_get_color4f(paint, (sk_color4f_t *)&gdk_color);
	float width = sk_paint_get_stroke_width(paint);
	while ((verb = sk_path_iter_next(iterator, line)) != DONE_SK_PATH_VERB) {
		// TODO: use GskPath to support other verbs
		if (verb == LINE_SK_PATH_VERB) {
			gtk_snapshot_save(snapshot);
			gtk_snapshot_translate(snapshot, &GRAPHENE_POINT_INIT(line[0].x, line[0].y));
			float rotation = atan2(line[1].y - line[0].y, line[1].x - line[0].x);
			gtk_snapshot_rotate(snapshot, rotation * 180 / M_PI);
			float length = sqrt((line[1].x - line[0].x) * (line[1].x - line[0].x) + (line[1].y - line[0].y) * (line[1].y - line[0].y));
			gtk_snapshot_append_color(snapshot, &gdk_color, &GRAPHENE_RECT_INIT(0, -width / 2, length, width));
			gtk_snapshot_restore(snapshot);
		}
	}
	sk_path_iter_destroy(iterator);
}

JNIEXPORT void JNICALL Java_android_graphics_GskCanvas_native_1translate(JNIEnv *env, jclass this_class, jlong snapshot_ptr, jfloat dx, jfloat dy)
{
	GtkSnapshot *snapshot = GTK_SNAPSHOT(_PTR(snapshot_ptr));
	gtk_snapshot_translate(snapshot, &GRAPHENE_POINT_INIT(dx, dy));
}

JNIEXPORT void JNICALL Java_android_graphics_GskCanvas_native_1rotate(JNIEnv *env, jclass this_class, jlong snapshot_ptr, jfloat angle)
{
	GtkSnapshot *snapshot = GTK_SNAPSHOT(_PTR(snapshot_ptr));
	gtk_snapshot_rotate(snapshot, angle);
}

JNIEXPORT void JNICALL Java_android_graphics_GskCanvas_native_1save(JNIEnv *env, jclass this_class, jlong snapshot_ptr)
{
	GtkSnapshot *snapshot = GTK_SNAPSHOT(_PTR(snapshot_ptr));
	gtk_snapshot_save(snapshot);
}

JNIEXPORT void JNICALL Java_android_graphics_GskCanvas_native_1restore(JNIEnv *env, jclass this_class, jlong snapshot_ptr)
{
	GtkSnapshot *snapshot = GTK_SNAPSHOT(_PTR(snapshot_ptr));
	gtk_snapshot_restore(snapshot);
}
