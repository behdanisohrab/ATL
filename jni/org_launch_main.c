#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "org_launch_main.h"
#include "android_app_Activity.h"

GtkWidget *window;

// we will be calling this, so need to declare it
static int main (int argc, char **argv, JNIEnv *env);

// android_app_Activity native methods

JNIEXPORT void JNICALL Java_android_app_Activity_set_1widget_1as_1root(JNIEnv *env, jobject this, jlong widget)
{
	gtk_window_set_child(GTK_WINDOW(window), gtk_widget_get_parent(GTK_WIDGET(_PTR(widget))));
}

JNIEXPORT void JNICALL Java_org_launch_main_real_1main(JNIEnv *env, jclass this, jobjectArray args)
{
	int argc = (*env)->GetArrayLength(env, args);
	char **argv = malloc(argc * sizeof(char *) + 1);

	argv[0] = "FIXME-meaningful-program-name";

	for (int i = 0; i < argc; i++) {
		argv[i+1] = (char *)_CSTRING((*env)->GetObjectArrayElement(env, args, i));
	}

	argc += 1; // the program name in argv[0]

	main(argc, argv, env);
}

// standard Gtk Application stuff, more or less

gboolean app_exit(GtkWindow* self, JNIEnv *env) // TODO: do more cleanup?
{
	/* -- run the main activity's onCreate -- */

	(*env)->CallVoidMethod(env, handle_cache.apk_main_activity.object, handle_cache.apk_main_activity.onDestroy, NULL);

	exit(0);
}


static void activate(GtkApplication *app, JNIEnv *env)
{
 	window = gtk_application_window_new (app);
 	gtk_window_set_title(GTK_WINDOW(window), "com.example.demo_application");
 	gtk_window_set_default_size(GTK_WINDOW(window), 540, 960);
	// Gtk won't see it fit to close the window since we hold references for all the widgets created via JNI, se we do it ourselves for now
 	g_signal_connect(window, "close-request", G_CALLBACK (app_exit), env);

 	gtk_widget_show(window);

	/* -- run the main activity's onCreate -- */

	(*env)->CallVoidMethod(env, handle_cache.apk_main_activity.object, handle_cache.apk_main_activity.onCreate, NULL);
}

static int main(int argc, char **argv, JNIEnv *env)
{
 	GtkApplication *app;
 	int status;

	if(argc < 2) {
		printf("usage: ~TBD~ <fully qualified class name of the entry activity>\n"); // TODO: if launching a different activity works just fine, we should probably note that here
		exit(1);
	}

	set_up_handle_cache(env, argv[1]);

 	app = gtk_application_new("com.example.demo_application", G_APPLICATION_NON_UNIQUE);
 	g_signal_connect(app, "activate", G_CALLBACK (activate), env);
 	status = g_application_run(G_APPLICATION(app), 0, NULL); // we can't wait for gio handle the commandline, since we need to use the argument(s) in JNI context
 	g_object_unref(app);

 	return status;
}
