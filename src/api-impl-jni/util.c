#include <stdint.h>
#include <dlfcn.h>
#include <pthread.h>

#include "util.h"
#include "src/api-impl-jni/defines.h"

struct handle_cache handle_cache = {0};

const char * attribute_set_get_string(JNIEnv *env, jobject attrs, char *attribute, char *schema)
{
	if (!attrs)
		return NULL;

	if(!schema)
		schema = "http://schemas.android.com/apk/res/android";

	jstring string = (jstring)(*env)->CallObjectMethod(env, attrs, handle_cache.attribute_set.getAttributeValue_string, _JSTRING(schema), _JSTRING(attribute));
	return string ? _CSTRING(string) : NULL;
}

int attribute_set_get_int(JNIEnv *env, jobject attrs, char *attribute, char *schema, int default_value)
{
	if (!attrs)
		return default_value;

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

void set_up_handle_cache(JNIEnv *env)
{
	(*env)->GetJavaVM(env, &jvm);

	handle_cache.activity.class = _REF((*env)->FindClass(env, "android/app/Activity"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	handle_cache.activity.onCreate = _METHOD(handle_cache.activity.class, "onCreate", "(Landroid/os/Bundle;)V");
	handle_cache.activity.onStart = _METHOD(handle_cache.activity.class, "onStart", "()V");
	handle_cache.activity.onWindowFocusChanged = _METHOD(handle_cache.activity.class, "onWindowFocusChanged", "(Z)V");
	handle_cache.activity.onResume = _METHOD(handle_cache.activity.class, "onResume", "()V");
	handle_cache.activity.onDestroy = _METHOD(handle_cache.activity.class, "onDestroy", "()V");
	handle_cache.activity.onStop = _METHOD(handle_cache.activity.class, "onStop", "()V");
	handle_cache.activity.onPause = _METHOD(handle_cache.activity.class, "onPause", "()V");

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
	handle_cache.motion_event.constructor = _METHOD(handle_cache.motion_event.class, "<init>", "(IIFF)V");

	handle_cache.canvas.class = _REF((*env)->FindClass(env, "android/graphics/Canvas"));
	handle_cache.canvas.constructor = _METHOD(handle_cache.canvas.class, "<init>", "(JJ)V");

	handle_cache.audio_track_periodic_listener.class = _REF((*env)->FindClass(env, "android/media/AudioTrack$OnPlaybackPositionUpdateListener"));
	handle_cache.audio_track_periodic_listener.onPeriodicNotification = _METHOD(handle_cache.audio_track_periodic_listener.class, "onPeriodicNotification", "(Landroid/media/AudioTrack;)V");

	handle_cache.input_queue_callback.class = _REF((*env)->FindClass(env, "android/view/InputQueue$Callback"));
	handle_cache.input_queue_callback.onInputQueueCreated = _METHOD(handle_cache.input_queue_callback.class, "onInputQueueCreated", "(Landroid/view/InputQueue;)V");

	handle_cache.surface_view.class = _REF((*env)->FindClass(env, "android/view/SurfaceView"));
	handle_cache.surface_view.surfaceCreated = _METHOD(handle_cache.surface_view.class, "surfaceCreated", "()V");
	handle_cache.surface_view.surfaceChanged = _METHOD(handle_cache.surface_view.class, "surfaceChanged", "(III)V");

	handle_cache.view.class = _REF((*env)->FindClass(env, "android/view/View"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	handle_cache.view.setLayoutParams = _METHOD(handle_cache.view.class, "setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V");
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	handle_cache.view.onDraw = _METHOD(handle_cache.view.class, "onDraw", "(Landroid/graphics/Canvas;)V");
	handle_cache.view.onMeasure = _METHOD(handle_cache.view.class, "onMeasure", "(II)V");
	handle_cache.view.onLayout = _METHOD(handle_cache.view.class, "onLayout", "(ZIIII)V");
	handle_cache.view.getMeasuredWidth = _METHOD(handle_cache.view.class, "getMeasuredWidth", "()I");
	handle_cache.view.getMeasuredHeight = _METHOD(handle_cache.view.class, "getMeasuredHeight", "()I");
	handle_cache.view.getSuggestedMinimumWidth = _METHOD(handle_cache.view.class, "getSuggestedMinimumWidth", "()I");
	handle_cache.view.getSuggestedMinimumHeight = _METHOD(handle_cache.view.class, "getSuggestedMinimumHeight", "()I");
	handle_cache.view.setMeasuredDimension = _METHOD(handle_cache.view.class, "setMeasuredDimension", "(II)V");
	handle_cache.view.onGenericMotionEvent = _METHOD(handle_cache.view.class, "onGenericMotionEvent", "(Landroid/view/MotionEvent;)Z");
	handle_cache.view.computeScroll = _METHOD(handle_cache.view.class, "computeScroll", "()V");
	handle_cache.view.getScrollX = _METHOD(handle_cache.view.class, "getScrollX", "()I");
	handle_cache.view.getScrollY = _METHOD(handle_cache.view.class, "getScrollY", "()I");
	handle_cache.view.performClick = _METHOD(handle_cache.view.class, "performClick", "()Z");
	handle_cache.view.onTouchEvent = _METHOD(handle_cache.view.class, "onTouchEvent", "(Landroid/view/MotionEvent;)Z");
	handle_cache.view.layoutInternal = _METHOD(handle_cache.view.class, "layoutInternal", "(II)V");
	handle_cache.view.measure = _METHOD(handle_cache.view.class, "measure", "(II)V");

	handle_cache.asset_manager.class = _REF((*env)->FindClass(env, "android/content/res/AssetManager"));
	handle_cache.asset_manager.extractFromAPK = _STATIC_METHOD(handle_cache.asset_manager.class, "extractFromAPK", "(Ljava/lang/String;Ljava/lang/String;)V");

	handle_cache.context.class = _REF((*env)->FindClass(env, "android/content/Context"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
	handle_cache.context.get_package_name = _METHOD(handle_cache.context.class, "getPackageName", "()Ljava/lang/String;");
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	handle_cache.application.class = _REF((*env)->FindClass(env, "android/app/Application"));
	handle_cache.application.get_app_icon_path = _METHOD(handle_cache.application.class, "get_app_icon_path", "()Ljava/lang/String;");

	handle_cache.looper.class = _REF((*env)->FindClass(env, "android/os/Looper"));
	handle_cache.looper.loop = _STATIC_METHOD(handle_cache.looper.class, "loop", "()V");
	handle_cache.looper.prepareMainLooper = _STATIC_METHOD(handle_cache.looper.class, "prepareMainLooper", "()V");

	handle_cache.key_event.class = _REF((*env)->FindClass(env, "android/view/KeyEvent"));
	handle_cache.key_event.constructor = _METHOD(handle_cache.key_event.class, "<init>", "(II)V");
}

void extract_from_apk(const char *path, const char *target) {
	JNIEnv *env = get_jni_env();
	(*env)->CallStaticVoidMethod(env, handle_cache.asset_manager.class, handle_cache.asset_manager.extractFromAPK, _JSTRING(path), _JSTRING(target));
}

/* logging with fallback to stderr */

typedef int __android_log_vprint_type(int prio, const char *tag, const char *fmt, va_list ap);

static int fallback_verbose_log(int prio, const char *tag, const char *fmt, va_list ap)
{
	int ret;

	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&mutex);
	static char buf[1024];
	ret = vsnprintf(buf, sizeof(buf), fmt, ap);
	fprintf(stderr, "%lu: %s\n", pthread_self(), buf);
	pthread_mutex_unlock(&mutex);

	return ret;
}

static int android_log_vprintf(int prio, const char *tag, const char *fmt, va_list ap)
{

	static __android_log_vprint_type *_android_log_vprintf = NULL;
	if(!_android_log_vprintf) {
		_android_log_vprintf = dlsym(RTLD_DEFAULT, "__android_log_vprint");

		if(!_android_log_vprintf) {
			_android_log_vprintf = &fallback_verbose_log;
		}
	}

	return _android_log_vprintf(prio, tag, fmt, ap);
}

int android_log_printf(android_LogPriority prio, const char *tag, const char *fmt, ...)
{
	int ret;

	va_list ap;
	va_start(ap, fmt);

	ret = android_log_vprintf(prio, tag, fmt, ap);

	va_end(ap);

	return ret;
}

void *get_nio_buffer(JNIEnv *env, jobject buffer, jarray *array_ref, jbyte **array)
{
	jclass class;
	void *pointer;
	int elementSizeShift, position;

	if (!buffer) {
		*array_ref = NULL;
		return NULL;
	}
	class = _CLASS(buffer);
	pointer = _PTR((*env)->GetLongField(env, buffer, _FIELD_ID(class, "address", "J")));
	elementSizeShift = (*env)->GetIntField(env, buffer, _FIELD_ID(class, "_elementSizeShift", "I"));
	position = (*env)->GetIntField(env, buffer, _FIELD_ID(class, "position", "I"));
	if (pointer) {   // buffer is direct
		*array_ref = NULL;
		pointer += position << elementSizeShift;
	} else {   // buffer is indirect
		*array_ref = (*env)->CallObjectMethod(env, buffer, _METHOD(class, "array", "()Ljava/lang/Object;"));
		pointer = *array = (*env)->GetPrimitiveArrayCritical(env, *array_ref, NULL);
		jint offset = (*env)->CallIntMethod(env, buffer, _METHOD(class, "arrayOffset", "()I"));
		pointer += (offset + position) << elementSizeShift;
	}
	return pointer;
}

void release_nio_buffer(JNIEnv *env, jarray array_ref, jbyte *array)
{
	if (array_ref)
		(*env)->ReleasePrimitiveArrayCritical(env, array_ref, array, 0);
}

int get_nio_buffer_size(JNIEnv *env, jobject buffer)
{
	jclass class = _CLASS(buffer);;
	int limit = (*env)->GetIntField(env, buffer, _FIELD_ID(class, "limit", "I"));
	int position = (*env)->GetIntField(env, buffer, _FIELD_ID(class, "position", "I"));

	return limit - position;
}
