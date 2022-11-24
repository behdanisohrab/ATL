#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "generated_headers/android_view_Window.h"

JNIEXPORT void JNICALL Java_android_view_Window_set_1widget_1as_1root(JNIEnv *env, jobject this, jlong window, jlong widget)
{
	gtk_window_set_child(GTK_WINDOW(_PTR(window)), gtk_widget_get_parent(GTK_WIDGET(_PTR(widget))));
}

// FIXME put this in a header file
struct input_queue {
	int fd;
	GtkEventController *controller;
};

JNIEXPORT void JNICALL Java_android_view_Window_take_1input_1queue(JNIEnv *env, jobject this, jlong native_window, jobject callback, jobject queue)
{
	GtkWidget *window = _PTR(native_window);
	printf("in Java_android_view_Window_take_1input_1queue\n");

	GtkEventController *controller = GTK_EVENT_CONTROLLER(gtk_event_controller_legacy_new());
	gtk_widget_add_controller(window, controller);

	struct input_queue *input_queue = malloc(sizeof(struct input_queue));
	input_queue->fd = -1;
	input_queue->controller = controller;

	_SET_LONG_FIELD(queue, "native_ptr", _INTPTR(input_queue));

	// we need to keep these for later, so they can be called after OnCreate finishes
	g_object_set_data(G_OBJECT(window), "input_queue_callback", (gpointer)_REF(callback));
	g_object_set_data(G_OBJECT(window), "input_queue", (gpointer)_REF(queue));
}
