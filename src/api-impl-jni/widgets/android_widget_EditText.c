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
