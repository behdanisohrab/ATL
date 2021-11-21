#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "android_widget_RelativeLayout.h"
#include "../views/android_view_ViewGroup.h"

// FIXME not a relative layout

JNIEXPORT void JNICALL Java_android_widget_RelativeLayout_native_1constructor__Landroid_util_AttributeSet_2(JNIEnv *env, jobject this, jobject attrs)
{
	int orientation = attribute_set_get_int(env, attrs, "orientation", NULL, 0);

	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *box = gtk_box_new(orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "RelativeLayout");
	_SET_LONG_FIELD(this, "widget", (long)box);
	g_object_ref(wrapper);
}

JNIEXPORT void JNICALL Java_android_widget_RelativeLayout_native_1constructor__Landroid_content_Context_2(JNIEnv *env, jobject this, jobject context) {
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "RelativeLayout");
	_SET_LONG_FIELD(this, "widget", (long)box);
	g_object_ref(wrapper);
}

JNIEXPORT void JNICALL Java_android_widget_RelativeLayout_addView(JNIEnv *env, jobject this, jobject child, jint index, jobject layout_params)
{
	Java_android_view_ViewGroup_addView(env, this, child, index, layout_params);

	gtk_box_append(GTK_BOX(_PTR(_GET_LONG_FIELD(this, "widget"))), gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(child, "widget")))));
}
