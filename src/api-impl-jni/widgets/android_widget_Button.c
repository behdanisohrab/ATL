#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_Button.h"

JNIEXPORT jlong JNICALL Java_android_widget_Button_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	const char *text = attribute_set_get_string(env, attrs, "text", NULL);

	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *label = gtk_button_new_with_label(text);
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), label);
	wrapper_widget_consume_touch_events(WRAPPER_WIDGET(wrapper));  // Android button consumes touch events
	return _INTPTR(label);
}

JNIEXPORT void JNICALL Java_android_widget_Button_native_1setText(JNIEnv *env, jobject this, jlong widget_ptr, jobject text)
{
	GtkButton *button = GTK_BUTTON(_PTR(widget_ptr));

	const char *nativeText = ((*env)->GetStringUTFChars(env, text, NULL));
	gtk_button_set_label(button, nativeText);
	((*env)->ReleaseStringUTFChars(env, text, nativeText));
}

struct touch_callback_data {
	JavaVM *jvm;
	jobject this;
	jobject listener;
	jmethodID listener_method;
};

static void clicked_cb(GtkWidget *button, struct touch_callback_data *d) {
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	(*env)->CallVoidMethod(env, d->listener, d->listener_method, d->this);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

JNIEXPORT void JNICALL Java_android_widget_Button_native_1setOnClickListener(JNIEnv *env, jobject this, jlong widget_ptr, jobject on_click_listener)
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
