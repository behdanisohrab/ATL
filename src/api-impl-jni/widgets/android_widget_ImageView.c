#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_ImageView.h"

JNIEXPORT void JNICALL Java_android_widget_ImageView_native_1constructor__Landroid_util_AttributeSet_2(JNIEnv *env, jobject this, jobject attrs)
{
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *image = gtk_picture_new_for_resource("/org/gtk/libgtk/icons/16x16/status/image-missing.png"); // show "broken image" icon
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), image);
	_SET_LONG_FIELD(this, "widget", _INTPTR(image));}

JNIEXPORT void JNICALL Java_android_widget_ImageView_native_1constructor__Landroid_content_Context_2(JNIEnv *env, jobject this, jobject context)
{
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *image = gtk_picture_new_for_resource("/org/gtk/libgtk/icons/16x16/status/image-missing.png"); // show "broken image" icon
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), image);
	_SET_LONG_FIELD(this, "widget", _INTPTR(image));
}

JNIEXPORT void JNICALL Java_android_widget_ImageView_native_1setPixbuf(JNIEnv *env, jobject this, jlong pixbuf_ptr)
{
	GtkWidget *image = _PTR(_GET_LONG_FIELD(this, "widget"));
	GdkPixbuf *pixbuf = _PTR(pixbuf_ptr);
	gtk_picture_set_pixbuf(GTK_PICTURE(image), pixbuf);
}
