#include "util.h"

struct handle_cache handle_cache = {0};

const char * attribute_set_get_string(JNIEnv *env, jobject attrs, char *attribute, char *schema)
{
	if(!schema)
		schema = "http://schemas.android.com/apk/res/android";

	return _CSTRING( (jstring)(*env)->CallObjectMethod(env, attrs, handle_cache.attribute_set.getAttributeValue_string, _JSTRING(schema), _JSTRING(attribute)) );
}

int attribute_set_get_int(JNIEnv *env, jobject attrs, char *attribute, char *schema, int default_value)
{
	if(!schema)
		schema = "http://schemas.android.com/apk/res/android";

	return (*env)->CallIntMethod(env, attrs, handle_cache.attribute_set.getAttributeValue_int, _JSTRING(schema), _JSTRING(attribute), default_value);
}

void set_up_handle_cache(JNIEnv *env, char *apk_main_activity_class)
{
	handle_cache.apk_main_activity.class = _REF((*env)->FindClass(env, apk_main_activity_class));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	handle_cache.apk_main_activity.object = _REF((*env)->NewObject(env, handle_cache.apk_main_activity.class, _METHOD(handle_cache.apk_main_activity.class, "<init>", "()V")));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	handle_cache.apk_main_activity.onCreate = _METHOD(handle_cache.apk_main_activity.class, "onCreate", "(Landroid/os/Bundle;)V");
//	handle_cache.apk_main_activity.onWindowFocusChanged = _METHOD(handle_cache.apk_main_activity.class, "onWindowFocusChanged", "(B)V");
	handle_cache.apk_main_activity.onDestroy = _METHOD(handle_cache.apk_main_activity.class, "onDestroy", "()V");

	handle_cache.attribute_set.class = _REF((*env)->FindClass(env, "android/util/AttributeSet"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	handle_cache.attribute_set.getAttributeValue_string = _METHOD(handle_cache.attribute_set.class, "getAttributeValue", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
	handle_cache.attribute_set.getAttributeValue_int = _METHOD(handle_cache.attribute_set.class, "getAttributeIntValue", "(Ljava/lang/String;Ljava/lang/String;I)I");

	handle_cache.array_list.class = _REF((*env)->FindClass(env, "java/util/ArrayList"));
	handle_cache.array_list.add = _METHOD(handle_cache.array_list.class, "add", "(Ljava/lang/Object;)Z");
	handle_cache.array_list.remove = _METHOD(handle_cache.array_list.class, "remove", "(Ljava/lang/Object;)Z");
	handle_cache.array_list.get = _METHOD(handle_cache.array_list.class, "get", "(I)Ljava/lang/Object;");
	handle_cache.array_list.size = _METHOD(handle_cache.array_list.class, "size", "()I");
	handle_cache.array_list.clear = _METHOD(handle_cache.array_list.class, "clear", "()V");

	handle_cache.paint.class = _REF((*env)->FindClass(env, "android/graphics/Paint"));
	handle_cache.paint.getColor = _METHOD(handle_cache.paint.class, "getColor", "()I");

	handle_cache.motion_event.class = _REF((*env)->FindClass(env, "android/view/MotionEvent"));
	handle_cache.motion_event.constructor = _METHOD(handle_cache.motion_event.class, "<init>", "(IFF)V");

	handle_cache.canvas.class = _REF((*env)->FindClass(env, "android/graphics/Canvas"));
	handle_cache.canvas.constructor = _METHOD(handle_cache.canvas.class, "<init>", "(JJ)V");

	handle_cache.renderer.class = _REF((*env)->FindClass(env, "android/opengl/GLSurfaceView$Renderer"));
	handle_cache.renderer.onSurfaceCreated = _METHOD(handle_cache.renderer.class, "onSurfaceCreated", "(Ljavax/microedition/khronos/opengles/GL10;Ljavax/microedition/khronos/egl/EGLConfig;)V");
	handle_cache.renderer.onSurfaceChanged = _METHOD(handle_cache.renderer.class, "onSurfaceChanged", "(Ljavax/microedition/khronos/opengles/GL10;II)V");
	handle_cache.renderer.onDrawFrame = _METHOD(handle_cache.renderer.class, "onDrawFrame", "(Ljavax/microedition/khronos/opengles/GL10;)V");

	handle_cache.gl_surface_view.class = _REF((*env)->FindClass(env, "android/opengl/GLSurfaceView"));
	handle_cache.gl_surface_view.onTouchEvent = _METHOD(handle_cache.gl_surface_view.class, "onTouchEvent", "(Landroid/view/MotionEvent;)Z");

	handle_cache.view.class = _REF((*env)->FindClass(env, "android/view/View"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	handle_cache.view.setLayoutParams = _METHOD(handle_cache.view.class, "setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V");
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}
