#include <EGL/egl.h>

#include "../defines.h"
#include "../util.h"

#include "com_google_android_gles_jni_EGLImpl.h"

// helpers from android source (TODO: either use GetIntArrayElements, or figure out if GetPrimitiveArrayCritical is superior and use it everywhere if so)
static jint* get_int_array_crit(JNIEnv *env, jintArray array) {
    if (array != NULL) {
        return (jint *)(*env)->GetPrimitiveArrayCritical(env, array, (jboolean *)0);
    } else {
        return(jint*) NULL; // FIXME - do apps expect us to use some default?
    }
}

static void release_int_array_crit(JNIEnv *env, jintArray array, jint* base) {
    if (array != NULL) {
        (*env)->ReleasePrimitiveArrayCritical(env, array, base, JNI_ABORT);
    }
}

// ---

static jlong* get_long_array_crit(JNIEnv *env, jlongArray array) {
    if (array != NULL) {
        return (jlong *)(*env)->GetPrimitiveArrayCritical(env, array, (jboolean *)0);
    } else {
        return(jlong*) NULL; // FIXME - do apps expect us to use some default?
    }
}

static void release_long_array_crit(JNIEnv *env, jlongArray array, jlong* base) {
    if (array != NULL) {
        (*env)->ReleasePrimitiveArrayCritical(env, array, base, JNI_ABORT);
    }
}

JNIEXPORT jlong JNICALL Java_com_google_android_gles_1jni_EGLImpl_native_1eglCreateContext(JNIEnv *env, jobject this, jlong egl_display, jlong egl_config, jobject share_context, jintArray attrib_list)
{
	printf("env: %p, this: %p, egl_display: %p, egl_config: %p, share_context: %p, attrib_list: %p\n", env, this, _PTR(egl_display), _PTR(egl_config), share_context, attrib_list);

    jint* attrib_base = get_int_array_crit(env, attrib_list);

    EGLContext egl_context = eglCreateContext(_PTR(egl_display), _PTR(egl_config), NULL, attrib_base);
	printf("egl_context: %d\n", egl_context);

    release_int_array_crit(env, attrib_list, attrib_base);

	return _INTPTR(egl_context);
}

JNIEXPORT jboolean JNICALL Java_com_google_android_gles_1jni_EGLImpl_native_1eglChooseConfig(JNIEnv *env, jobject this, jlong egl_display, jintArray attrib_list, jlongArray egl_configs, jint config_size, jintArray num_config)
{
	int ret;

	jint* attrib_base = get_int_array_crit(env, attrib_list);
	jlong* configs_base = get_long_array_crit(env, egl_configs);
	jint* num_config_base = get_int_array_crit(env, num_config);

	ret = eglChooseConfig(_PTR(egl_display), attrib_base, egl_configs ? _PTR(configs_base) : NULL, config_size, num_config_base);
	printf(".. eglChooseConfig: egl_display: %p, egl_configs: %d, _PTR(configs_base): %p, config_size: %d, num_config_base[0]: %d\n", egl_display, egl_configs, _PTR(configs_base), config_size, num_config_base[0]);

	release_int_array_crit(env, attrib_list, attrib_base);
	release_long_array_crit(env, egl_configs, configs_base);
	release_int_array_crit(env, num_config, num_config_base);

	return ret;
}
