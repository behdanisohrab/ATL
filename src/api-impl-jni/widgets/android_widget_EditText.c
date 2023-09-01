#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_EditText.h"

JNIEXPORT jlong JNICALL Java_android_widget_EditText_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *entry = gtk_entry_new();
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), entry);
	return _INTPTR(entry);
}

JNIEXPORT jstring JNICALL Java_android_widget_EditText_native_1getText(JNIEnv *env, jobject this, jlong widget_ptr)
{
	GtkEntry *entry = GTK_ENTRY(_PTR(widget_ptr));
	const char *text = gtk_entry_buffer_get_text(gtk_entry_get_buffer(entry));
	return _JSTRING(text);
}

struct changed_callback_data {
	jobject this;
	jobject listener;
	jmethodID listener_method;
	jmethodID getText;
};

static void changed_cb(GtkEditable* self, struct changed_callback_data *d) {
	JNIEnv *env = get_jni_env();

	jobject text = (*env)->CallObjectMethod(env, d->this, d->getText);
	(*env)->CallVoidMethod(env, d->listener, d->listener_method, text);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

JNIEXPORT void JNICALL Java_android_widget_EditText_native_1addTextChangedListener(JNIEnv *env, jobject this, jlong widget_ptr, jobject listener) {
	GtkEntry *entry = GTK_ENTRY(_PTR(widget_ptr));

	struct changed_callback_data *callback_data = malloc(sizeof(struct changed_callback_data));
	callback_data->this = _REF(this);
	callback_data->listener = _REF(listener);
	callback_data->listener_method = _METHOD(_CLASS(listener), "afterTextChanged", "(Landroid/text/Editable;)V");
	callback_data->getText = _METHOD(_CLASS(this), "getText", "()Landroid/text/Editable;");

	g_signal_connect(GTK_EDITABLE(entry), "changed", G_CALLBACK(changed_cb), callback_data);
}
