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
