#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_ImageView.h"

JNIEXPORT jlong JNICALL Java_android_widget_ImageView_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *image = gtk_picture_new_for_resource("/org/gtk/libgtk/icons/16x16/status/image-missing.png"); // show "broken image" icon
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), image);
	wrapper_widget_set_jobject(WRAPPER_WIDGET(wrapper), env, this);
	return _INTPTR(image);
}

JNIEXPORT void JNICALL Java_android_widget_ImageView_native_1setPixbuf(JNIEnv *env, jobject this, jlong pixbuf_ptr)
{
	GtkWidget *image = _PTR(_GET_LONG_FIELD(this, "widget"));
	GdkPixbuf *pixbuf = _PTR(pixbuf_ptr);
	gtk_picture_set_pixbuf(GTK_PICTURE(image), pixbuf);
}

JNIEXPORT void JNICALL Java_android_widget_ImageView_native_1setDrawable(JNIEnv *env, jobject this, jlong widget_ptr, jlong paintable_ptr)
{
	GtkPicture *picture = _PTR(widget_ptr);
	GdkPaintable *paintable = _PTR(paintable_ptr);
	gtk_picture_set_paintable(picture, paintable);
}
