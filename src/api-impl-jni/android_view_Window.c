#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "generated_headers/android_view_Window.h"

JNIEXPORT void JNICALL Java_android_view_Window_set_1widget_1as_1root(JNIEnv *env, jobject this, jlong window, jlong widget)
{
	gtk_window_set_child(GTK_WINDOW(_PTR(window)), gtk_widget_get_parent(GTK_WIDGET(_PTR(widget))));
}
