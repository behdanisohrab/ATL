#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_ScrollView.h"
#include "../generated_headers/android_view_ViewGroup.h"

// FIXME not a scrollview

JNIEXPORT void JNICALL Java_android_widget_ScrollView_native_1constructor__Landroid_util_AttributeSet_2(JNIEnv *env, jobject this, jobject attrs)
{
	int orientation = attribute_set_get_int(env, attrs, "orientation", NULL, 0);

	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *box = gtk_box_new(orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "ScrollView");
	_SET_LONG_FIELD(this, "widget", _INTPTR(box));
}

JNIEXPORT void JNICALL Java_android_widget_ScrollView_native_1constructor__Landroid_content_Context_2(JNIEnv *env, jobject this, jobject context) {
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "ScrollView");
	_SET_LONG_FIELD(this, "widget", _INTPTR(box));
}

JNIEXPORT void JNICALL Java_android_widget_ScrollView_removeView(JNIEnv *env, jobject this, jobject child)
{
	Java_android_view_ViewGroup_removeView(env, this, child);

	GtkWidget *_child = gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(child, "widget"))));

	gtk_box_remove(GTK_BOX(_PTR(_GET_LONG_FIELD(this, "widget"))), g_object_ref(_child));
	g_object_force_floating(G_OBJECT(_child));
}

JNIEXPORT void JNICALL Java_android_widget_ScrollView_removeAllViews(JNIEnv *env, jobject this)
{
	Java_android_view_ViewGroup_removeAllViews(env, this);

	GtkBox *box = GTK_BOX(_PTR(_GET_LONG_FIELD(this, "widget")));

	GtkWidget *child;
	while((child = gtk_widget_get_first_child(GTK_WIDGET(box))) != NULL) {
		gtk_box_remove(box, g_object_ref(child));
		g_object_force_floating(G_OBJECT(child));
	}
}

JNIEXPORT void JNICALL Java_android_widget_ScrollView_addView(JNIEnv *env, jobject this, jobject child, jint index, jobject layout_params)
{
	Java_android_view_ViewGroup_addView(env, this, child, index, layout_params);

	gtk_box_append(GTK_BOX(_PTR(_GET_LONG_FIELD(this, "widget"))), gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(child, "widget")))));
}
