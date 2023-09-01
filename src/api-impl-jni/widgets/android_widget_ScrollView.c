#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_ScrollView.h"
#include "../generated_headers/android_view_ViewGroup.h"

// FIXME not a scrollview

JNIEXPORT jlong JNICALL Java_android_widget_ScrollView_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	int orientation = attribute_set_get_int(env, attrs, "orientation", NULL, 0);

	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *box = gtk_box_new(orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "ScrollView");
	return _INTPTR(box);
}
