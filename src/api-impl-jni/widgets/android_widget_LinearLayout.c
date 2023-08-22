#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../drawables/ninepatch.h"

#include "../generated_headers/android_widget_LinearLayout.h"
#include "../generated_headers/android_view_ViewGroup.h"

JNIEXPORT jlong JNICALL Java_android_widget_LinearLayout_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	int orientation = attribute_set_get_int(env, attrs, "orientation", NULL, 1);

	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *box = gtk_box_new(orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "LinearLayout");
	if (!attrs) {
		gtk_widget_set_hexpand_set(box, true); // FIXME: to counteract expand on drawing areas
		gtk_widget_set_vexpand_set(box, true); // XXX
	}
	return _INTPTR(box);

//	struct ninepatch_t *ninepatch = ninepatch_new("/home/Mis012/Github_and_other_sources/org.happysanta.gd_29_src.tar.gz/res/drawable-mdpi/btn_br_down.9.png");
//	g_object_set_data(G_OBJECT(wrapper), "background_ninepatch", ninepatch);
}

JNIEXPORT void JNICALL Java_android_widget_LinearLayout_native_1addView(JNIEnv *env, jobject this, jlong widget, jlong child, jint index, jobject layout_params)
{
	gtk_box_append(GTK_BOX(_PTR(widget)), gtk_widget_get_parent(GTK_WIDGET(_PTR(child)))); // FIXME - ignores index argument
}

JNIEXPORT void JNICALL Java_android_widget_LinearLayout_native_1removeView(JNIEnv *env, jobject this, jlong widget, jlong child)
{
	gtk_box_remove(GTK_BOX(_PTR(widget)), gtk_widget_get_parent(GTK_WIDGET(_PTR(child))));
}

JNIEXPORT void JNICALL Java_android_widget_LinearLayout_setOrientation(JNIEnv *env, jobject this, jint orientation)
{
	gtk_orientable_set_orientation(GTK_ORIENTABLE(_PTR(_GET_LONG_FIELD(this, "widget"))), orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL);
}
