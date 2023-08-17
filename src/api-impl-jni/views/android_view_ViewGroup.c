#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "../widgets/WrapperWidget.h"

#include "../generated_headers/android_view_ViewGroup.h"
#include "../generated_headers/android_view_View.h"

/**
 * Should be overwritten by ViewGroup subclasses.
 * Fall back to vertical GtkBox if subclass is not implemented yet
 */
JNIEXPORT jlong JNICALL Java_android_view_ViewGroup_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "ViewGroup");
	return _INTPTR(box);
}

JNIEXPORT void JNICALL Java_android_view_ViewGroup_native_1addView(JNIEnv *env, jobject this, jlong widget, jlong child, jint index, jobject layout_params)
{
	if(layout_params) {
		/*
		GtkWidget *_child = gtk_widget_get_parent(GTK_WIDGET(_PTR(child)));
		jint child_width = -1;
		jint child_height = -1;

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
	gtk_box_append(GTK_BOX(_PTR(widget)), gtk_widget_get_parent(GTK_WIDGET(_PTR(child)))); // FIXME - ignores index argument
}

JNIEXPORT void JNICALL Java_android_view_ViewGroup_native_1removeView(JNIEnv *env, jobject this, jlong widget, jlong child)
{
	gtk_box_remove(GTK_BOX(_PTR(widget)), gtk_widget_get_parent(GTK_WIDGET(_PTR(child))));
}
