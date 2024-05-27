#include <gdk/gdk.h>
#include <string.h>

#include "../defines.h"
#include "../util.h"
#include "NinePatchPaintable.h"
#include "../generated_headers/android_graphics_drawable_Drawable.h"

JNIEXPORT jlong JNICALL Java_android_graphics_drawable_Drawable_native_1paintable_1from_1path(JNIEnv *env, jclass class, jstring pathStr) {
	const char *path = (*env)->GetStringUTFChars(env, pathStr, NULL);
	GdkPaintable *paintable = NULL;

	// check if path ends with .9.png
	int len = strlen(path);
	if (len >= 6 && !strcmp(path + len - 6, ".9.png")) {
		paintable = ninepatch_paintable_new(path);
	}
	if (!paintable)
		paintable = GDK_PAINTABLE(gdk_texture_new_from_filename(path, NULL));

	(*env)->ReleaseStringUTFChars(env, pathStr, path);
	return _INTPTR(paintable);
}

struct _JavaPaintable {
	GObject parent_instance;
	jobject drawable;
};
G_DECLARE_FINAL_TYPE(JavaPaintable, java_paintable, JAVA, PAINTABLE, GObject)

static void java_paintable_snapshot(GdkPaintable *gdk_paintable, GdkSnapshot *snapshot, double width, double height)
{
	JNIEnv *env = get_jni_env();
	JavaPaintable *paintable = JAVA_PAINTABLE(gdk_paintable);
	jclass canvas_class = (*env)->FindClass(env, "android/graphics/GskCanvas");
	jmethodID canvas_constructor = _METHOD(canvas_class, "<init>", "(J)V");
	jobject canvas = (*env)->NewObject(env, canvas_class, canvas_constructor, _INTPTR(snapshot));
	(*env)->CallVoidMethod(env, paintable->drawable, handle_cache.drawable.setBounds, 0, 0, (int)width, (int)height);
	(*env)->CallVoidMethod(env, paintable->drawable, handle_cache.drawable.draw, canvas);
	if ((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	(*env)->DeleteLocalRef(env, canvas);
	(*env)->DeleteLocalRef(env, canvas_class);
}

static int java_paintable_get_intrinsic_width(GdkPaintable *gdk_paintable)
{
	JNIEnv *env = get_jni_env();
	JavaPaintable *paintable = JAVA_PAINTABLE(gdk_paintable);
	jmethodID getIntrinsicWidth = _METHOD(handle_cache.drawable.class, "getIntrinsicWidth", "()I");
	return (*env)->CallIntMethod(env, paintable->drawable, getIntrinsicWidth);
}

static int java_paintable_get_intrinsic_height(GdkPaintable *gdk_paintable)
{
	JNIEnv *env = get_jni_env();
	JavaPaintable *paintable = JAVA_PAINTABLE(gdk_paintable);
	jmethodID getIntrinsicHeight = _METHOD(handle_cache.drawable.class, "getIntrinsicHeight", "()I");
	return (*env)->CallIntMethod(env, paintable->drawable, getIntrinsicHeight);
}

static void java_paintable_init(JavaPaintable *java_paintable)
{
}

static void java_paintable_paintable_init(GdkPaintableInterface *iface)
{
	iface->snapshot = java_paintable_snapshot;
	iface->get_intrinsic_height = java_paintable_get_intrinsic_height;
	iface->get_intrinsic_width = java_paintable_get_intrinsic_width;
}

static void java_paintable_class_init(JavaPaintableClass *class)
{
}

G_DEFINE_TYPE_WITH_CODE(JavaPaintable, java_paintable, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(GDK_TYPE_PAINTABLE, java_paintable_paintable_init))

JNIEXPORT jlong JNICALL Java_android_graphics_drawable_Drawable_native_1constructor(JNIEnv *env, jobject this) {
	JavaPaintable *paintable = NULL;
	if (handle_cache.drawable.draw != _METHOD(_CLASS(this), "draw", "(Landroid/graphics/Canvas;)V")) {
		paintable = g_object_new(java_paintable_get_type(), NULL);
		paintable->drawable = _REF(this);
	}
	return _INTPTR(paintable);
}

JNIEXPORT void JNICALL Java_android_graphics_drawable_Drawable_native_1invalidate(JNIEnv *env, jobject this, jlong paintable_ptr) {
	gdk_paintable_invalidate_contents(GDK_PAINTABLE(_PTR(paintable_ptr)));
}

JNIEXPORT void JNICALL Java_android_graphics_drawable_Drawable_native_1draw(JNIEnv *env, jobject this, jlong paintable_ptr, jlong snapshot_ptr, jint width, jint height) {
	GdkSnapshot *snapshot = (GdkSnapshot *)_PTR(snapshot_ptr);
	GdkPaintable *paintable = GDK_PAINTABLE(_PTR(paintable_ptr));
	gdk_paintable_snapshot(paintable, snapshot, width, height);
}
