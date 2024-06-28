#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "../sk_area/include/c/sk_data.h"
#include "../sk_area/include/c/sk_image.h"

#include "generated_headers/android_graphics_Bitmap.h"

void attach_sk_image(GdkPixbuf *pixbuf)
{
	sk_imageinfo_t info = {
		.width = gdk_pixbuf_get_width(pixbuf),
		.height = gdk_pixbuf_get_height(pixbuf),
		.colorType = RGBA_8888_SK_COLORTYPE,
		.alphaType = UNPREMUL_SK_ALPHATYPE,
	};

	void *pixbuf_pixels = gdk_pixbuf_get_pixels(pixbuf);
	int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
	size_t pixbuf_size = rowstride * (info.height - 1)
	                     + /* last row: */ info.width * ((gdk_pixbuf_get_n_channels(pixbuf) * gdk_pixbuf_get_bits_per_sample(pixbuf) + 7) / 8);
	/* use the data as-is, and don't ever free it because the pixbuf owns it */
	sk_data_t *pixels = sk_data_new_with_proc(pixbuf_pixels, pixbuf_size, NULL, NULL);

	sk_image_t *image = sk_image_new_raster_data(&info, pixels, rowstride);
	g_object_set_data(G_OBJECT(pixbuf), "sk_image", image);
}

/*
 * We use a GdkPixbuf as the backing for a bitmap.
 * We additionally create a view into it as a skia image,
 * so we can pass it to skia functions.
 */
JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1bitmap_1from_1path(JNIEnv *env, jobject this, jobject path)
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(_CSTRING(path), NULL);
	android_log_printf(ANDROID_LOG_VERBOSE, "["__FILE__"]", ">>> made pixbuf from path: >%s<, >%p<\n", _CSTRING(path), pixbuf);
	if(gdk_pixbuf_get_n_channels(pixbuf) == 3) { // no alpha, add it (skia doesn't support 24bpp)
		GdkPixbuf *old_pixbuf = pixbuf;
		pixbuf = gdk_pixbuf_add_alpha(pixbuf, false, 0, 0, 0);
		g_object_unref(old_pixbuf);
	}

	attach_sk_image(pixbuf);

	g_object_ref(pixbuf);
	return _INTPTR(pixbuf);
}
/* new empty bitmap */
JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1create(JNIEnv *env, jclass this, jint width, jint height)
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, width, height);

	attach_sk_image(pixbuf);

	return _INTPTR(pixbuf);
}

JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_getWidth(JNIEnv *env, jobject this)
{
	GdkPixbuf *pixbuf = _PTR(_GET_LONG_FIELD(this, "pixbuf"));
	if (!pixbuf) {
		return 10;
	}

	return gdk_pixbuf_get_width(pixbuf);
}

JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_getHeight(JNIEnv *env, jobject this)
{
	GdkPixbuf *pixbuf = _PTR(_GET_LONG_FIELD(this, "pixbuf"));
	if (!pixbuf) {
		return 10;
	}

	return gdk_pixbuf_get_height(pixbuf);
}

JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeGetPixels(JNIEnv *env, jclass, jlong bitmapHandle, jintArray pixelArray, jint offset, jint stride, jint x, jint y, jint width, jint height, jboolean premultiplied)
{
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

JNIEXPORT jboolean JNICALL Java_android_graphics_Bitmap_nativeRecycle(JNIEnv *env, jclass, jlong bitmapHandle, jlong textureHandle)
{
	GdkPixbuf *pixbuf = _PTR(bitmapHandle);
	GdkTexture *texture = _PTR(textureHandle);
	sk_image_t *image = g_object_get_data(G_OBJECT(pixbuf), "sk_image");
	if(image)
		sk_image_unref(image);
	else
		fprintf(stderr, "nativeRecycle: pixbuf doesn't have a skia image associated: %p\n", pixbuf);
	g_object_unref(pixbuf);
	if (texture)
		g_object_unref(texture);
	return true;
}

JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1copy(JNIEnv *env, jclass, jlong src_ptr)
{
	GdkPixbuf *src = _PTR(src_ptr);
	GdkPixbuf *copy = gdk_pixbuf_copy(src);
	sk_image_t *image = g_object_get_data(G_OBJECT(src), "sk_image");
	if(image)
		g_object_set_data(G_OBJECT(copy), "sk_image", sk_image_make_raster_image(image)); // probably?
	else
		fprintf(stderr, "native_copy: pixbuf doesn't have a skia image associated: %p\n", src);
	return _INTPTR(copy);
}

JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1subpixbuf(JNIEnv *env, jclass, jlong _pixbuf, jint x, jint y, jint width, jint height)
{
	GdkPixbuf *pixbuf = _PTR(_pixbuf);
	GdkPixbuf *subpixbuf = gdk_pixbuf_new_subpixbuf(pixbuf, x, y, width, height);
	attach_sk_image(subpixbuf);
	return _INTPTR(subpixbuf);
}

JNIEXPORT jint JNICALL Java_android_graphics_Bitmap_nativeRowBytes(JNIEnv *env, jclass, jlong pixbuf_ptr)
{
	GdkPixbuf *pixbuf = _PTR(pixbuf_ptr);

	return gdk_pixbuf_get_rowstride(pixbuf);
}

JNIEXPORT jlong JNICALL Java_android_graphics_Bitmap_native_1paintable_1from_1pixbuf(JNIEnv *env, jclass, jlong pixbuf_ptr)
{
	GdkPixbuf *pixbuf = _PTR(pixbuf_ptr);
	return _INTPTR(gdk_texture_new_for_pixbuf(pixbuf));
}

JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeCopyPixelsToBuffer(JNIEnv *env, jclass this, jlong _pixbuf, jobject buffer)
{
	GdkPixbuf *pixbuf = _PTR(_pixbuf);
	size_t pixbuf_size = gdk_pixbuf_get_rowstride(pixbuf) * (gdk_pixbuf_get_height(pixbuf) - 1)
	                     + /* last row: */ gdk_pixbuf_get_width(pixbuf) * ((gdk_pixbuf_get_n_channels(pixbuf) * gdk_pixbuf_get_bits_per_sample(pixbuf) + 7) / 8);
	jarray array_ref;
	jbyte *array;
	uint8_t *pixels = get_nio_buffer(env, buffer, &array_ref, &array);
	memcpy(pixels, gdk_pixbuf_get_pixels(pixbuf), pixbuf_size);
	release_nio_buffer(env, array_ref, array);
}

JNIEXPORT void JNICALL Java_android_graphics_Bitmap_nativeCopyPixelsFromBuffer(JNIEnv *env, jclass this, jlong _pixbuf, jobject buffer)
{
	GdkPixbuf *pixbuf = _PTR(_pixbuf);
	size_t pixbuf_size = gdk_pixbuf_get_rowstride(pixbuf) * (gdk_pixbuf_get_height(pixbuf) - 1)
	                     + /* last row: */ gdk_pixbuf_get_width(pixbuf) * ((gdk_pixbuf_get_n_channels(pixbuf) * gdk_pixbuf_get_bits_per_sample(pixbuf) + 7) / 8);
	jarray array_ref;
	jbyte *array;
	uint8_t *pixels = get_nio_buffer(env, buffer, &array_ref, &array);
	memcpy(gdk_pixbuf_get_pixels(pixbuf), pixels, pixbuf_size);
	release_nio_buffer(env, array_ref, array);
}

JNIEXPORT void JNICALL Java_android_graphics_Bitmap_native_1unref_1texture(JNIEnv *env, jclass class, jlong texture_ptr)
{
	GdkTexture *texture = GDK_TEXTURE(_PTR(texture_ptr));
	g_object_unref(texture);
}
