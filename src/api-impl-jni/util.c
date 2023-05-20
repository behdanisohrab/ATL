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

JavaVM *jvm;

// TODO: use this everywhere, not just for gdb helper functions
JNIEnv * get_jni_env(void)
{
	JNIEnv *env;
	(*jvm)->GetEnv(jvm, (void**)&env, JNI_VERSION_1_6);
	return env;
}

JNIEnv * _gdb_get_jni_env(void)
{
	return get_jni_env();
}

void _gdb_get_java_stack_trace(void)
{
	JNIEnv *env = get_jni_env();
	(*env)->ExceptionDescribe(env);
}

void set_up_handle_cache(JNIEnv *env, char *apk_main_activity_class)
{
	(*env)->GetJavaVM(env, &jvm);

	handle_cache.apk_main_activity.class = _REF((*env)->FindClass(env, "android/app/Activity"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	jmethodID createMainActivity = _STATIC_METHOD(handle_cache.apk_main_activity.class, "createMainActivity", "(Ljava/lang/String;)Landroid/app/Activity;");
	handle_cache.apk_main_activity.object = _REF((*env)->CallStaticObjectMethod(env, handle_cache.apk_main_activity.class, createMainActivity, _JSTRING(apk_main_activity_class)));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	handle_cache.apk_main_activity.onCreate = _METHOD(handle_cache.apk_main_activity.class, "onCreate", "(Landroid/os/Bundle;)V");
	handle_cache.apk_main_activity.onStart = _METHOD(handle_cache.apk_main_activity.class, "onStart", "()V");
	handle_cache.apk_main_activity.onWindowFocusChanged = _METHOD(handle_cache.apk_main_activity.class, "onWindowFocusChanged", "(Z)V");
	handle_cache.apk_main_activity.onResume = _METHOD(handle_cache.apk_main_activity.class, "onResume", "()V");
	handle_cache.apk_main_activity.onDestroy = _METHOD(handle_cache.apk_main_activity.class, "onDestroy", "()V");
	handle_cache.apk_main_activity.set_window = _METHOD((*env)->FindClass(env, "android/app/Activity"), "set_window", "(J)V");

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
	handle_cache.gl_surface_view.wrap_EGLContextFactory_createContext = _METHOD(handle_cache.gl_surface_view.class, "wrap_EGLContextFactory_createContext", "(JJ)J");
	handle_cache.gl_surface_view.wrap_EGLConfigChooser_chooseConfig = _METHOD(handle_cache.gl_surface_view.class, "wrap_EGLConfigChooser_chooseConfig", "(J)J");

	handle_cache.audio_track_periodic_listener.class = _REF((*env)->FindClass(env, "android/media/AudioTrack$OnPlaybackPositionUpdateListener"));
	handle_cache.audio_track_periodic_listener.onPeriodicNotification = _METHOD(handle_cache.audio_track_periodic_listener.class, "onPeriodicNotification", "(Landroid/media/AudioTrack;)V");

	handle_cache.input_queue_callback.class = _REF((*env)->FindClass(env, "android/view/InputQueue$Callback"));
	handle_cache.input_queue_callback.onInputQueueCreated = _METHOD(handle_cache.input_queue_callback.class, "onInputQueueCreated", "(Landroid/view/InputQueue;)V");

	handle_cache.surface_holder_callback.class = _REF((*env)->FindClass(env, "android/view/SurfaceHolder$Callback"));
	handle_cache.surface_holder_callback.surfaceCreated = _METHOD(handle_cache.surface_holder_callback.class, "surfaceCreated", "(Landroid/view/SurfaceHolder;)V");

	handle_cache.view.class = _REF((*env)->FindClass(env, "android/view/View"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	handle_cache.view.setLayoutParams = _METHOD(handle_cache.view.class, "setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V");
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	handle_cache.asset_manager.class = _REF((*env)->FindClass(env, "android/content/res/AssetManager"));
	handle_cache.asset_manager.extractFromAPK = _STATIC_METHOD(handle_cache.asset_manager.class, "extractFromAPK", "(Ljava/lang/String;Ljava/lang/String;)V");
}

void extract_from_apk(const char *path, const char *target) {
	JNIEnv *env = get_jni_env();
	(*env)->CallStaticObjectMethod(env, handle_cache.asset_manager.class, handle_cache.asset_manager.extractFromAPK, _JSTRING(path), _JSTRING(target));
}
