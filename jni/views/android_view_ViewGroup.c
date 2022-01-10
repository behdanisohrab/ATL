#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "android_view_ViewGroup.h"
#include "android_view_View.h"

JNIEXPORT void JNICALL Java_android_view_ViewGroup_addView(JNIEnv *env, jobject this, jobject child, jint index, jobject layout_params)
{
	GtkWidget *_child = gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(child, "widget"))));
	jint child_width = -1;
	jint child_height = -1;

	if(layout_params) {
		(*env)->CallVoidMethod(env, child, handle_cache.view.setLayoutParams, layout_params);
		if((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);

		/*
		jint child_width = _GET_INT_FIELD(layout_params, "width");
		jint child_height = _GET_INT_FIELD(layout_params, "height");

		jint child_gravity = _GET_INT_FIELD(layout_params, "gravity");

		if(child_width > 0)
			g_object_set(G_OBJECT(_child), "width-request", child_width, NULL);
		if(child_height > 0)
			g_object_set(G_OBJECT(_child), "height-request", child_height, NULL);

		if(child_gravity != -1) {
			printf(":::-: setting child gravity: %d", child_gravity);
			Java_android_view_View_setGravity(env, child, child_gravity);
		}*/
	}

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
