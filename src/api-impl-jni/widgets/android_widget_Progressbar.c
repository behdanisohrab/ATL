#include <gtk/gtk.h>
#include <stdio.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_ProgressBar.h"

JNIEXPORT jlong JNICALL Java_android_widget_ProgressBar_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *progress_bar = gtk_progress_bar_new();
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), progress_bar);
	gtk_widget_set_name(progress_bar, "ProgressBar");
	return _INTPTR(progress_bar);
}

JNIEXPORT void JNICALL Java_android_widget_ProgressBar_native_1setProgress(JNIEnv *env, jobject this, jlong widget_ptr, jfloat progress)
{
	GtkProgressBar *progress_bar = GTK_PROGRESS_BAR(_PTR(widget_ptr));
	gtk_progress_bar_set_fraction(progress_bar, progress);
}
