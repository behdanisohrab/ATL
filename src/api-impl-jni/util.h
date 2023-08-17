#ifndef _UTILS_H_
#define _UTILS_H_

#include <jni.h>

#include "defines.h"

struct handle_cache {
	struct {
		jclass class;
		jmethodID onCreate;
		jmethodID onStart;
		jmethodID onResume;
		jmethodID onWindowFocusChanged;
		jmethodID onDestroy;
		jmethodID onStop;
		jmethodID onPause;
	} apk_main_activity;
	struct {
		jclass class;
		jmethodID getAttributeValue_string;
		jmethodID getAttributeValue_int;
	} attribute_set;
	struct {
		jclass class;
		jmethodID add;
		jmethodID remove;
		jmethodID get;
		jmethodID size;
		jmethodID clear;
	} array_list;
	struct {
		jclass class;
		jmethodID getColor;
	} paint;
	struct {
		jclass class;
		jmethodID constructor;
	} motion_event;
	struct {
		jclass class;
		jmethodID constructor;
	} canvas;
	struct {
		jclass class;
		jmethodID onSurfaceCreated;
		jmethodID onSurfaceChanged;
		jmethodID onDrawFrame;
	} renderer;
	struct {
		jclass class;
		jmethodID onTouchEvent;
		jmethodID wrap_EGLContextFactory_createContext;
		jmethodID wrap_EGLConfigChooser_chooseConfig;
	} gl_surface_view;
	struct {
		jclass class;
		jmethodID onPeriodicNotification;
	} audio_track_periodic_listener;
	struct {
		jclass class;
		jmethodID onInputQueueCreated;
	} input_queue_callback;
	struct {
		jclass class;
		jmethodID surfaceCreated;
	} surface_holder_callback;
	struct {
		jclass class;
		jmethodID setLayoutParams;
		jmethodID onDraw;
		jmethodID onMeasure;
	} view;
	struct {
		jclass class;
		jmethodID extractFromAPK;
	} asset_manager;
	struct {
		jclass class;
		jmethodID get_package_name;
	} context;
	struct {
		jclass class;
		jmethodID get_app_icon_path;
	} application;
};

extern struct handle_cache handle_cache;

JNIEnv * get_jni_env(void);

const char * attribute_set_get_string(JNIEnv *env, jobject attrs, char *attribute, char *schema);
int attribute_set_get_int(JNIEnv *env, jobject attrs, char *attribute, char *schema, int default_value);
void set_up_handle_cache(JNIEnv *env);
void extract_from_apk(const char *path, const char *target);

void prepare_main_looper(JNIEnv* env);

#endif
