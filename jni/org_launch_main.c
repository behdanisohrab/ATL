#include <gtk/gtk.h>

#include "defines.h"
#include "util.h"

#include "org_launch_main.h"
#include "android_app_Activity.h"

GtkWidget *window;

// we will be calling this, so need to declare it
static int main (int argc, char **argv, JNIEnv *env);

// android_app_Activity native methods

JNIEXPORT void JNICALL Java_android_view_Window_set_1widget_1as_1root(JNIEnv *env, jobject this, jlong widget)
{
	gtk_window_set_child(GTK_WINDOW(window), gtk_widget_get_parent(GTK_WIDGET(_PTR(widget))));
}

JNIEXPORT void JNICALL Java_org_launch_main_real_1main(JNIEnv *env, jclass this_class, jobjectArray args)
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
	/* -- run the main activity's onDestroy -- */

	(*env)->CallVoidMethod(env, handle_cache.apk_main_activity.object, handle_cache.apk_main_activity.onDestroy, NULL);

	return false;
}

// FIXME: used by hacks in GLSurfaceView
int FIXME__WIDTH;
int FIXME__HEIGHT;

struct jni_callback_data { JavaVM *jvm; char *apk_main_activity_class; uint32_t window_width; uint32_t window_height;};
static void activate(GtkApplication *app, struct jni_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	if(!d->apk_main_activity_class) {
		printf("error: missing required option --launch-activity <activity>.\nyou can specify --help to see the list of options\n");
		exit(1);
	}

	set_up_handle_cache(env, d->apk_main_activity_class);

	jclass display_class = (*env)->FindClass(env, "android/view/Display");
	_SET_STATIC_INT_FIELD(display_class, "window_width", d->window_width);
	_SET_STATIC_INT_FIELD(display_class, "window_height", d->window_height);

	FIXME__WIDTH = d->window_width;
	FIXME__HEIGHT = d->window_height;

	window = gtk_application_window_new (app);

	gtk_window_set_title(GTK_WINDOW(window), "com.example.demo_application");
	gtk_window_set_default_size(GTK_WINDOW(window), d->window_width, d->window_height);
	g_signal_connect(window, "close-request", G_CALLBACK (app_exit), env);

//	TODO: set icon according to how android gets it for the purposes of displaying it in the launcher
//	gtk_window_set_icon_name(window, "weather-clear");

	gtk_widget_show(window);

	/* -- run the main activity's onCreate -- */

	(*env)->CallVoidMethod(env, handle_cache.apk_main_activity.object, handle_cache.apk_main_activity.onCreate, NULL);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

//	TODO: some apps wait for this to actually do stuff
/*	(*env)->CallVoidMethod(env, handle_cache.apk_main_activity.object, handle_cache.apk_main_activity.onWindowFocusChanged, true);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);*/
}

void init_cmd_parameters(GApplication *app, struct jni_callback_data *d)
{
  const GOptionEntry cmd_params[] =
  {
    {
      .long_name = "launch-activity",
      .short_name = 'l',
      .flags = G_OPTION_FLAG_NONE,
      .arg = G_OPTION_ARG_STRING,
      .arg_data = &d->apk_main_activity_class,
      .description = "the fully quilifed name of the activity you wish to launch (usually the apk's main activity)",
      .arg_description = NULL,
    },
    {
      .long_name = "window-width",
      .short_name = 'w',
      .flags = G_OPTION_FLAG_NONE,
      .arg = G_OPTION_ARG_INT,
      .arg_data = &d->window_width,
      .description = "window width to launch with (some apps react poorly to runtime window size adjustments)",
      .arg_description = NULL,
    },
    {
      .long_name = "window-height",
      .short_name = 'h',
      .flags = G_OPTION_FLAG_NONE,
      .arg = G_OPTION_ARG_INT,
      .arg_data = &d->window_height,
      .description = "window height to launch with (some apps react poorly to runtime window size adjustments)",
      .arg_description = NULL,
    },
    {NULL}
  };

  g_application_add_main_option_entries (G_APPLICATION (app), cmd_params);
}

static int main(int argc, char **argv, JNIEnv *env)
{
 	GtkApplication *app;
 	int status;

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct jni_callback_data *callback_data = malloc(sizeof(struct jni_callback_data));
	callback_data->jvm = jvm;
	callback_data->apk_main_activity_class = NULL;
	callback_data->window_width = 960;
	callback_data->window_height = 540;

 	app = gtk_application_new("com.example.demo_application", G_APPLICATION_NON_UNIQUE);

	// cmdline related setup
	init_cmd_parameters(G_APPLICATION(app), callback_data);
	g_application_set_option_context_summary(G_APPLICATION(app), "actual usage:\nLD_PRELOAD=libpthread_bio.so ./dalvik/dalvik -verbose:jni -cp hax_arsc_parser.dex:hax_xmlpull.dex:hax.dex:main.dex:${1}:com.google.android.gms.apk org/launch/main ${2}\nwhere ${1} is the path to the apk and ${2} is the cmdline");

 	g_signal_connect(app, "activate", G_CALLBACK (activate), callback_data);
 	status = g_application_run(G_APPLICATION(app), argc, argv);
 	g_object_unref(app);

 	return status;
}
