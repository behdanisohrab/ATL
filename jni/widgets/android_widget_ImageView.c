#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "android_widget_ImageView.h"

JNIEXPORT void JNICALL Java_android_widget_ImageView_native_1constructor__Landroid_util_AttributeSet_2(JNIEnv *env, jobject this, jobject attrs)
{
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *image = gtk_image_new_from_icon_name("FIXME"); // will not actually use gtk_image_new_from_icon_name when implementing this, but we want that nice "broken image" icon
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), image);
	_SET_LONG_FIELD(this, "widget", _INTPTR(image));}

JNIEXPORT void JNICALL Java_android_widget_ImageView_native_1constructor__Landroid_content_Context_2(JNIEnv *env, jobject this, jobject context)
{
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *image = gtk_image_new_from_icon_name("FIXME"); // will not actually use gtk_image_new_from_icon_name when implementing this, but we want that nice "broken image" icon
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), image);
	_SET_LONG_FIELD(this, "widget", _INTPTR(image));
}

