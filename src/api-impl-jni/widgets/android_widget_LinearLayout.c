#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../drawables/ninepatch.h"

#include "../generated_headers/android_widget_LinearLayout.h"
#include "../generated_headers/android_view_ViewGroup.h"

JNIEXPORT void JNICALL Java_android_widget_LinearLayout_native_1constructor__Landroid_util_AttributeSet_2(JNIEnv *env, jobject this, jobject attrs)
{
	int orientation = attribute_set_get_int(env, attrs, "orientation", NULL, 0);

	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *box = gtk_box_new(orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "LinearLayout");
	_SET_LONG_FIELD(this, "widget", _INTPTR(box));
}

JNIEXPORT void JNICALL Java_android_widget_LinearLayout_native_1constructor__Landroid_content_Context_2(JNIEnv *env, jobject this, jobject context) {
	GtkWidget *wrapper = wrapper_widget_new();

	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // spacing of 0
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "LinearLayout");
	gtk_widget_set_hexpand_set(box, true); // FIXME: to counteract expand on drawing areas
	gtk_widget_set_vexpand_set(box, true); // XXX
	_SET_LONG_FIELD(this, "widget", _INTPTR(box));

//	struct ninepatch_t *ninepatch = ninepatch_new("/home/Mis012/Github_and_other_sources/org.happysanta.gd_29_src.tar.gz/res/drawable-mdpi/btn_br_down.9.png");
//	g_object_set_data(G_OBJECT(wrapper), "background_ninepatch", ninepatch);
}

JNIEXPORT void JNICALL Java_android_widget_LinearLayout_addView(JNIEnv *env, jobject this, jobject child, jint index, jobject layout_params)
{
	Java_android_view_ViewGroup_addView(env, this, child, index, layout_params);

	gtk_box_append(GTK_BOX(_PTR(_GET_LONG_FIELD(this, "widget"))), gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(child, "widget"))))); // FIXME - ignores index argument
}

JNIEXPORT void JNICALL Java_android_widget_LinearLayout_removeView(JNIEnv *env, jobject this, jobject child)
{
	Java_android_view_ViewGroup_removeView(env, this, child);

	gtk_box_remove(GTK_BOX(_PTR(_GET_LONG_FIELD(this, "widget"))), gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(child, "widget")))));
}

JNIEXPORT void JNICALL Java_android_widget_LinearLayout_removeAllViews(JNIEnv *env, jobject this)
{
	Java_android_view_ViewGroup_removeAllViews(env, this);

	GtkBox *box = GTK_BOX(_PTR(_GET_LONG_FIELD(this, "widget")));

	GtkWidget *child;
	while((child = gtk_widget_get_first_child(GTK_WIDGET(box))) != NULL) {
		gtk_box_remove(box, child);
	}
}

JNIEXPORT void JNICALL Java_android_widget_LinearLayout_setOrientation(JNIEnv *env, jobject this, jint orientation)
{
	gtk_orientable_set_orientation(GTK_ORIENTABLE(_PTR(_GET_LONG_FIELD(this, "widget"))), orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL);
}
