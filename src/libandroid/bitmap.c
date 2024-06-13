#include <gtk/gtk.h>
#include <jni.h>

// FIXME: put the header in a common place
#include "../api-impl-jni/defines.h"

#define ANDROID_BITMAP_RESULT_SUCCESS 0

struct AndroidBitmapInfo {
	uint32_t width;
	uint32_t height;
	uint32_t stride;
	int32_t  format;
	uint32_t flags;
};

int AndroidBitmap_getInfo(JNIEnv* env, jobject bitmap, struct AndroidBitmapInfo *info) {
	GdkPixbuf *pixbuf = _PTR(_GET_LONG_FIELD(bitmap, "pixbuf"));
	info->width = gdk_pixbuf_get_width(pixbuf);
	info->height = gdk_pixbuf_get_height(pixbuf);
	info->stride = gdk_pixbuf_get_rowstride(pixbuf);
	info->format = 1; // ANDROID_BITMAP_FORMAT_RGBA_8888
	return ANDROID_BITMAP_RESULT_SUCCESS;
}
int AndroidBitmap_lockPixels(JNIEnv* env, jobject bitmap, void** pixels) {
	GdkPixbuf *pixbuf = _PTR(_GET_LONG_FIELD(bitmap, "pixbuf"));
	*pixels = gdk_pixbuf_get_pixels(pixbuf);
	return ANDROID_BITMAP_RESULT_SUCCESS;
}
int AndroidBitmap_unlockPixels(JNIEnv* env, jobject bitmap) {
	return ANDROID_BITMAP_RESULT_SUCCESS;
}
