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
	handle_cache.apk_main_activity.class = (*env)->FindClass(env, "com/example/demo_application/MainActivity");
	handle_cache.apk_main_activity.object = _REF((*env)->NewObject(env, handle_cache.apk_main_activity.class, _METHOD(handle_cache.apk_main_activity.class, "<init>", "()V")));
	handle_cache.apk_main_activity.onCreate = (*env)->GetMethodID(env, handle_cache.apk_main_activity.class, "onCreate", "(Landroid/os/Bundle;)V");

	handle_cache.attribute_set.class = (*env)->FindClass(env, "android/util/AttributeSet");
	handle_cache.attribute_set.getAttributeValue_string = _METHOD(handle_cache.attribute_set.class, "getAttributeValue", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
	handle_cache.attribute_set.getAttributeValue_int = _METHOD(handle_cache.attribute_set.class, "getAttributeIntValue", "(Ljava/lang/String;Ljava/lang/String;I)I");

	handle_cache.array_list.class = (*env)->FindClass(env, "java/util/ArrayList");
	handle_cache.array_list.add = _METHOD(handle_cache.array_list.class, "add", "(Ljava/lang/Object;)Z");
}
