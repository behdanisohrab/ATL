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

JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeGetPixels(JNIEnv *env, jclass, jlong bitmapHandle, jintArray pixelArray, jint offset, jint stride, jint x, jint y, jint width, jint height, jboolean premultiplied) {
	int i,j;
	GdkPixbuf *pixbuf = _PTR(bitmapHandle);
	g_assert(gdk_pixbuf_get_n_channels(pixbuf) == 4);
	g_assert(gdk_pixbuf_get_colorspace(pixbuf) == GDK_COLORSPACE_RGB);
	jint *dst = (*env)->GetIntArrayElements(env, pixelArray, NULL);
	jint *d = dst + offset;
	const guint8 *src = gdk_pixbuf_read_pixels(pixbuf);
	int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
	src += y * rowstride + x;
	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			d[j] = src[4*j+3] << 24 | src[4*j+0] << 16 | src[4*j+1] << 8 | src[4*j+2];
		}
		d += stride;
		src += rowstride;
	}
	(*env)->ReleaseIntArrayElements(env, pixelArray, dst, 0);
}

JNIEXPORT jboolean JNICALL Java_android_graphics_Bitmap_nativeRecycle(JNIEnv *env, jclass, jlong bitmapHandle) {
	GdkPixbuf *pixbuf = _PTR(bitmapHandle);
	g_object_unref(pixbuf);
	return true;
}

JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1copy(JNIEnv *env, jclass, jlong src_ptr) {
	GdkPixbuf *src = _PTR(src_ptr);
	GdkPixbuf *copy = gdk_pixbuf_copy(src);
	return _INTPTR(copy);
}

JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_nativeRowBytes(JNIEnv *env, jclass, jlong pixbuf_ptr) {
	GdkPixbuf *pixbuf = _PTR(pixbuf_ptr);

	return gdk_pixbuf_get_rowstride(pixbuf);
}
