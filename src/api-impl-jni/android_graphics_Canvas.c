#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "../sk_area/include/c/sk_canvas.h"
#include "../sk_area/include/c/sk_image.h"
#include "generated_headers/android_graphics_Canvas.h"

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1save(JNIEnv *env, jclass this, jlong skia_canvas, jlong widget)
{
	sk_canvas_t *canvas = (sk_canvas_t *)_PTR(skia_canvas);

	sk_canvas_save(canvas);
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1restore(JNIEnv *env, jclass this, jlong skia_canvas, jlong widget)
{
	sk_canvas_t *canvas = (sk_canvas_t *)_PTR(skia_canvas);

	sk_canvas_restore(canvas);
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawLine(JNIEnv *env, jclass this_class, jlong skia_canvas, jlong widget, jfloat start_x, jfloat start_y, jfloat stop_x, jfloat stop_y, jlong skia_paint)
{
	sk_canvas_t *canvas = (sk_canvas_t *)_PTR(skia_canvas);
	sk_paint_t *paint = (sk_paint_t *)_PTR(skia_paint);

	sk_canvas_draw_line(canvas, start_x, start_y, stop_x, stop_y, paint);
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawBitmap(JNIEnv *env , jclass this_class, jlong skia_canvas, jlong widget, jlong _pixbuf, jfloat src_x, jfloat src_y, jfloat dest_x , jfloat dest_y, jfloat dest_w , jfloat dest_h, jlong skia_paint)
{
	sk_canvas_t *canvas = (sk_canvas_t *)_PTR(skia_canvas);
	GdkPixbuf *pixbuf = (GdkPixbuf *)_PTR(_pixbuf);
	sk_paint_t *paint = (sk_paint_t *)_PTR(skia_paint);

	sk_image_t *image = g_object_get_data(G_OBJECT(pixbuf), "sk_image");
	if(!image) {
		fprintf(stderr, "pixbuf doesn't have a skia image associated: %p\n", pixbuf);
		return;
	}
	sk_canvas_draw_image(canvas, image, dest_x, dest_y, paint);
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawRect(JNIEnv *env, jclass this, jlong skia_canvas, jfloat left, jfloat top, jfloat right, jfloat bottom, jlong skia_paint)
{
	sk_canvas_t *canvas = (sk_canvas_t *)_PTR(skia_canvas);
	sk_paint_t *paint = (sk_paint_t *)_PTR(skia_paint);

	// FIXME: this doesn't work great with dark mode, since the text stays light even if the game draws white background
//	sk_canvas_draw_rect(canvas, &(sk_rect_t){left, top, right, bottom}, paint);
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1rotate(JNIEnv *env, jclass this, jlong skia_canvas, jlong widget, jfloat angle)
{
	sk_canvas_t *canvas = (sk_canvas_t *)_PTR(skia_canvas);

	sk_canvas_rotate_degrees(canvas, angle);
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1rotate_1and_1translate(JNIEnv *env, jclass this, jlong skia_canvas, jlong widget, jfloat angle, jfloat tx, jfloat ty)
{
	sk_canvas_t *canvas = (sk_canvas_t *)_PTR(skia_canvas);

	sk_canvas_translate(canvas, tx, ty);
	sk_canvas_rotate_degrees(canvas, angle);
	sk_canvas_translate(canvas, -tx, -ty);

}
