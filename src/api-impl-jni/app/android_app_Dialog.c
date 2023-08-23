#include <gtk/gtk.h>
#include <jni.h>

#include "../defines.h"
#include "../generated_headers/android_app_Dialog.h"

JNIEXPORT jlong JNICALL Java_android_app_Dialog_nativeInit(JNIEnv *env, jobject this)
{
	GtkWidget *dialog = gtk_dialog_new();
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_window_destroy), dialog);
	return _INTPTR(dialog);
}

JNIEXPORT void JNICALL Java_android_app_Dialog_nativeSetTitle(JNIEnv *env, jobject this, jlong ptr, jstring title)
{
	GtkWindow *dialog = GTK_WINDOW(_PTR(ptr));
	const char* nativeTitle = (*env)->GetStringUTFChars(env, title, NULL);
	gtk_window_set_title(dialog, nativeTitle);
	(*env)->ReleaseStringUTFChars(env, title, nativeTitle);
}

JNIEXPORT void JNICALL Java_android_app_Dialog_nativeSetContentView(JNIEnv *env, jobject this, jlong ptr, jlong widget_ptr) {
	GtkDialog *dialog = GTK_DIALOG(_PTR(ptr));
	GtkWidget *widget = GTK_WIDGET(_PTR(widget_ptr));

	GtkWidget *content_area = gtk_dialog_get_content_area(dialog);
	gtk_box_append(GTK_BOX(content_area), gtk_widget_get_parent(widget));
}

JNIEXPORT void JNICALL Java_android_app_Dialog_nativeShow(JNIEnv *env, jobject this, jlong ptr)
{
	GtkWindow *dialog = GTK_WINDOW(_PTR(ptr));
	gtk_window_present(dialog);
}
