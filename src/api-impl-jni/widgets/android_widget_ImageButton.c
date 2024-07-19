#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_ImageButton.h"

JNIEXPORT jlong JNICALL Java_android_widget_ImageButton_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *button = gtk_button_new();
	GtkWidget *image = gtk_picture_new_for_resource("/org/gtk/libgtk/icons/16x16/status/image-missing.png"); // show "broken image" icon
	gtk_button_set_child(GTK_BUTTON(button), image);
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), button);
	wrapper_widget_set_jobject(WRAPPER_WIDGET(wrapper), env, this);

	return _INTPTR(button);
}

JNIEXPORT void JNICALL Java_android_widget_ImageButton_native_1setPixbuf(JNIEnv *env, jobject this, jlong widget_ptr, jlong pixbuf_ptr)
{
	GdkPaintable *paintable = GDK_PAINTABLE(gdk_texture_new_for_pixbuf(_PTR(pixbuf_ptr)));
	Java_android_widget_ImageButton_native_1setDrawable(env, this, widget_ptr, _INTPTR(paintable));
}

struct touch_callback_data {
	JavaVM *jvm;
	jobject this;
	jobject listener;
	jmethodID listener_method;
};

static void clicked_cb(GtkWidget *button, struct touch_callback_data *d) {
	printf("clicked_cb\n");
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	(*env)->CallVoidMethod(env, d->listener, d->listener_method, d->this);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

JNIEXPORT void JNICALL Java_android_widget_ImageButton_native_1setOnClickListener(JNIEnv *env, jobject this, jlong widget_ptr, jobject on_click_listener)
{
	GtkWidget *button = GTK_WIDGET(_PTR(widget_ptr));
	g_signal_handlers_disconnect_matched(button, G_SIGNAL_MATCH_FUNC, 0, 0, NULL, clicked_cb, NULL);
	if (!on_click_listener)
		return;

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct touch_callback_data *callback_data = malloc(sizeof(struct touch_callback_data));
	callback_data->jvm = jvm;
	callback_data->this = _REF(this);
 	callback_data->listener = _REF(on_click_listener);
	callback_data->listener_method = _METHOD(_CLASS(on_click_listener), "onClick", "(Landroid/view/View;)V");

	g_signal_connect(button, "clicked", G_CALLBACK(clicked_cb), callback_data);
}

JNIEXPORT void JNICALL Java_android_widget_ImageButton_native_1setDrawable(JNIEnv *env, jobject this, jlong widget_ptr, jlong paintable_ptr)
{
	GtkButton *button = _PTR(widget_ptr);
	GtkPicture *picture = GTK_PICTURE(gtk_button_get_child(GTK_BUTTON(button)));
	GdkPaintable *paintable = _PTR(paintable_ptr);
	gtk_picture_set_paintable(picture, paintable);
}
