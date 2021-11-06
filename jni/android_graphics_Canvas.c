#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "android_graphics_Canvas.h"

static jclass paint_class;
static jmethodID paint_get_color;

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1contructor(JNIEnv *env, jobject this)
{
	paint_class = _REF((*env)->FindClass(env, "android/graphics/Paint"));
	paint_get_color = _METHOD(paint_class, "getColor", "()I");
}

JNIEXPORT void JNICALL Java_android_graphics_Canvas_native_1drawLine(JNIEnv *env, jclass this, jlong cairo_context, jfloat start_x, jfloat start_y, jfloat stop_x, jfloat stop_y, jobject paint)
{
	cairo_t *cr = (cairo_t *)cairo_context;

	jint paint_color = (*env)->CallIntMethod(env, paint, paint_get_color);

	char buf[10]; //#rrggbbaa\0
	snprintf(buf, 10, "#%06x%02x", paint_color & 0x00FFFFFF, paint_color>>24);
	GdkRGBA color;
	gdk_rgba_parse(&color, buf);

	gdk_cairo_set_source_rgba(cr, &color);
	cairo_move_to(cr, start_x, start_y);
	cairo_line_to(cr, stop_x, stop_y);
	cairo_stroke(cr);
}
