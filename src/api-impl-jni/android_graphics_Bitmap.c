#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "../sk_area/include/c/sk_data.h"
#include "../sk_area/include/c/sk_image.h"

#include "generated_headers/android_graphics_Bitmap.h"

/*
 * We use a GdkPixbuf as the backing for a bitmap.
 * We additionally create a view into it as a skia image,
 * so we can pass it to skia functions.
 */
JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1bitmap_1from_1path(JNIEnv *env, jobject this, jobject path)
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(_CSTRING(path), NULL);
	android_log_printf(ANDROID_LOG_VERBOSE, "["__FILE__"]", ">>> made pixbuf from path: >%s<, >%p<\n", _CSTRING(path), pixbuf);

	sk_imageinfo_t info = {
		.width = gdk_pixbuf_get_width(pixbuf),
		.height = gdk_pixbuf_get_height(pixbuf),
		.colorType = RGBA_8888_SK_COLORTYPE, // is this correct?
		.alphaType = PREMUL_SK_ALPHATYPE,
	};

	void *pixbuf_pixels = gdk_pixbuf_get_pixels(pixbuf);
	int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
	size_t pixbuf_size = rowstride * (info.height - 1)
	                     + /* last row: */ info.width * ((gdk_pixbuf_get_n_channels(pixbuf) * gdk_pixbuf_get_bits_per_sample(pixbuf) + 7) / 8);
	/* use the data as-is, and don't ever free it because the pixbuf owns it */
	sk_data_t *pixels = sk_data_new_with_proc(pixbuf_pixels, pixbuf_size, NULL, NULL);

	sk_image_t *image = sk_image_new_raster_data(&info, pixels, rowstride);
	g_object_set_data(G_OBJECT(pixbuf), "sk_image", image);

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
	sk_image_t *image = g_object_get_data(G_OBJECT(pixbuf), "sk_image");
	if(!image) {
		fprintf(stderr, "pixbuf doesn't have a skia image associated: %p\n", pixbuf);
		g_object_unref(pixbuf);
		return true;
	}
	sk_image_unref(image);
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
