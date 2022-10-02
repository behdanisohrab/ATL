#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "generated_headers/android_graphics_Canvas.h"

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1save(JNIEnv *env, jclass this, jlong cairo_context, jlong widget)
{
	cairo_t *cr = (cairo_t *)_PTR(cairo_context);

	cairo_save(cr);
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1restore(JNIEnv *env, jclass this, jlong cairo_context, jlong widget)
{
	cairo_t *cr = (cairo_t *)_PTR(cairo_context);

	cairo_restore(cr);
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawLine(JNIEnv *env, jclass this_class, jlong cairo_context, jlong widget, jfloat start_x, jfloat start_y, jfloat stop_x, jfloat stop_y, jint paint_color)
{
	cairo_t *cr = (cairo_t *)_PTR(cairo_context);

	// TODO: cairo is not stateless, so we should probably check that the state is not already what we want it to be before we tell cairo to change it
	// NOTE: we should make sure that cairo doesn't do this microoptimization internally before we implement it here

	char buf[10]; //#rrggbbaa\0
	snprintf(buf, 10, "#%06x%02x", paint_color & 0x00FFFFFF, paint_color>>24);
	GdkRGBA color;
	gdk_rgba_parse(&color, buf);

	gdk_cairo_set_source_rgba(cr, &color);

	cairo_move_to(cr, start_x, start_y);
	cairo_line_to(cr, stop_x, stop_y);
	cairo_stroke(cr);
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawBitmap(JNIEnv *env , jclass this_class, jlong cairo_context, jlong widget, jlong _pixbuf, jfloat src_x, jfloat src_y , jfloat dest_x , jfloat dest_y, jobject paint)
{
	cairo_t *cr = (cairo_t *)_PTR(cairo_context);
	GdkPixbuf *pixbuf = (GdkPixbuf *)_PTR(_pixbuf);

	cairo_translate(cr, dest_x, dest_y);
	gdk_cairo_set_source_pixbuf(cr, pixbuf, src_x, src_y);
	cairo_paint(cr);
	cairo_translate(cr, -dest_x, -dest_y);
}

// TODO: if we switched to using the snapshot mechanic directly instead of having a DrawingArea, these two could possibly (maybe it clips or something?) be replaced with hw-accelerated Gsk functions
// NOTE: it's unclear whether using the snapshot mechanic would still give us the same cairo context each time, and getting the same cairo context each time sure is convenient
JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1rotate(JNIEnv *env, jclass this, jlong cairo_context, jlong widget, jfloat angle)
{
	cairo_t *cr = (cairo_t *)_PTR(cairo_context);

	cairo_rotate(cr, DEG2RAD(angle));
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1rotate_1and_1translate(JNIEnv *env, jclass this, jlong cairo_context, jlong widget, jfloat angle, jfloat tx, jfloat ty)
{
	cairo_t *cr = (cairo_t *)_PTR(cairo_context);

	cairo_translate(cr, tx, ty);
	cairo_rotate(cr, DEG2RAD(angle));
	cairo_translate(cr, -tx, -ty);
}
