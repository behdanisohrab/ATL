#include <libportal/portal.h>

#include "../defines.h"
#include "../util.h"
#include "../generated_headers/android_hardware_SensorManager.h"

/* FIXME */
float x = 0;
float y = 2;
float z = 0;

struct accel_callback_data { JavaVM *jvm; jobject listener; jclass listener_class; jobject sensor; };
gboolean on_accel_data(struct accel_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	jfloatArray values = (*env)->NewFloatArray(env, 3);
	(*env)->SetFloatArrayRegion(env, values, 0, 3, (jfloat[]){x, y, z});

	jobject sensor_event = (*env)->NewObject(env, handle_cache.sensor_event.class, handle_cache.sensor_event.constructor, values, d->sensor);

	(*env)->CallVoidMethod(env, d->listener, _METHOD(d->listener_class, "onSensorChanged", "(Landroid/hardware/SensorEvent;)V"), sensor_event);
	(*env)->DeleteLocalRef(env, values);
	(*env)->DeleteLocalRef(env, sensor_event);

	return G_SOURCE_CONTINUE;
}

JNIEXPORT void JNICALL Java_android_hardware_SensorManager_register_1accelerometer_1listener_1native(JNIEnv *env , jobject this, jobject listener, jobject sensor)
{
	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct accel_callback_data *callback_data = malloc(sizeof(struct accel_callback_data));
	callback_data->jvm = jvm;
 	callback_data->listener = _REF(listener);
	callback_data->listener_class = _REF(_CLASS(callback_data->listener));
	callback_data->sensor = _REF(sensor);

	g_timeout_add(10, G_SOURCE_FUNC(on_accel_data), callback_data);
}
