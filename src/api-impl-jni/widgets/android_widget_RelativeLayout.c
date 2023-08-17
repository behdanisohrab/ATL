#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_RelativeLayout.h"
#include "../generated_headers/android_view_ViewGroup.h"

// FIXME not a relative layout

JNIEXPORT jlong JNICALL Java_android_widget_RelativeLayout_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	int orientation = attribute_set_get_int(env, attrs, "orientation", NULL, 1);

	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *box = gtk_box_new(orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "RelativeLayout");
	return _INTPTR(box);
}

JNIEXPORT void JNICALL Java_android_widget_RelativeLayout_native_1addView(JNIEnv *env, jobject this, jlong widget, jlong child, jint index, jobject layout_params)
{
	gtk_box_append(GTK_BOX(_PTR(widget)), gtk_widget_get_parent(GTK_WIDGET(_PTR(child)))); // FIXME - ignores index argument
}

JNIEXPORT void JNICALL Java_android_widget_RelativeLayout_native_1removeView(JNIEnv *env, jobject this, jlong widget, jlong child)
{
	gtk_box_remove(GTK_BOX(_PTR(widget)), gtk_widget_get_parent(GTK_WIDGET(_PTR(child))));
}
