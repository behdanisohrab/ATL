#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "android_view_ViewGroup.h"

JNIEXPORT void JNICALL Java_android_view_ViewGroup_addView(JNIEnv *env, jobject this, jobject child, jint index, jobject layout_params)
{
	_SET_OBJ_FIELD(child, "parent", "Landroid/view/ViewGroup;", this);

	jobject children = _GET_OBJ_FIELD(this, "children", "Ljava/util/ArrayList;");

	(*env)->CallVoidMethod(env, children, handle_cache.array_list.add, child);
}


JNIEXPORT void JNICALL Java_android_view_ViewGroup_removeView(JNIEnv *env, jobject this, jobject child)
{
	_SET_OBJ_FIELD(child, "parent", "Landroid/view/ViewGroup;", NULL);

	jobject children = _GET_OBJ_FIELD(this, "children", "Ljava/util/ArrayList;");

	(*env)->CallVoidMethod(env, children, handle_cache.array_list.remove, child);
}


JNIEXPORT void JNICALL Java_android_view_ViewGroup_removeAllViews(JNIEnv *env, jobject this)
{
	jobject children = _GET_OBJ_FIELD(this, "children", "Ljava/util/ArrayList;");
	jint size = (*env)->CallIntMethod(env, children, handle_cache.array_list.size);

	for(int i = 0; i < size; i++) {
		jobject child = (*env)->CallObjectMethod(env, children, handle_cache.array_list.get, i);
		_SET_OBJ_FIELD(child, "parent", "Landroid/view/ViewGroup;", NULL);
	}

	(*env)->CallVoidMethod(env, children, handle_cache.array_list.clear);
}
