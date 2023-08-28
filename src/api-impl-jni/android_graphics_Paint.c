#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "../sk_area/include/c/sk_paint.h"
#include "generated_headers/android_graphics_Paint.h"

JNIEXPORT jlong JNICALL Java_android_graphics_Paint_native_1constructor(JNIEnv *env, jobject this)
{
	return _INTPTR(sk_paint_new());
}

/* NOTE: sk_color_t seems to have the same internal representation as android uses for color, so we just pass that directly */
JNIEXPORT void JNICALL Java_android_graphics_Paint_native_1set_1color(JNIEnv *env, jobject this, jlong skia_paint, jint color)
{
	sk_paint_t *paint = (sk_paint_t *)_PTR(skia_paint);

	sk_paint_set_color(paint, color);
}

JNIEXPORT jint JNICALL Java_android_graphics_Paint_native_1get_1color(JNIEnv *env, jobject this, jlong skia_paint)
{
	sk_paint_t *paint = (sk_paint_t *)_PTR(skia_paint);

	return sk_paint_get_color(paint);
}
