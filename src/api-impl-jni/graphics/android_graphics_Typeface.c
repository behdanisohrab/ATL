#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "../../sk_area/include/c/sk_typeface.h"

#include "../generated_headers/android_graphics_Typeface.h"

JNIEXPORT jlong JNICALL Java_android_graphics_Typeface_native_1create(JNIEnv *env, jclass this, jobject _family_name, jint style)
{
	/* TODO: use the family name */
	return _INTPTR(sk_typeface_create_default()); /* TODO: recycle this */
}
