#include <gtk/gtk.h>

#include "../jni/defines.h"
#include "../jni/util.h"

#include "../jni/org_launch_main.h"
#include "../jni/android_app_Activity.h"

#include <dlfcn.h>

GtkWidget *window;

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

char *construct_classpath(char *prefix, char *path_to_prepend, char **cp_array, size_t len)
{
	size_t result_len = strlen(prefix);
	for(int i = 0; i < len; i++) {
		result_len += strlen(path_to_prepend) + strlen(cp_array[i]) + 1; // the 1 is for either : or the final \0
	}

	char *result = malloc(result_len);
	strcpy(result, prefix);
	for(int i = 0; i < len; i++) {
		strcat(result, path_to_prepend);
		strcat(result, cp_array[i]);
		if (i < (len - 1))
			strcat(result, ":");
	}

	return result;
}

char *construct_boot_classpath(char *prefix, char **cp_array, size_t len)
{
	char *android_root_path = getenv("ANDROID_ROOT");
	char *framework_dir = "/framework/";
	char *framework_dir_path = malloc(strlen(android_root_path) + strlen(framework_dir) + 1);
	strcpy(framework_dir_path, android_root_path);
	strcat(framework_dir_path, framework_dir);

	char *result = construct_classpath(prefix, framework_dir_path, cp_array, len);
	free(framework_dir_path);

	return result;
}

JNIEnv* create_vm(char *apk_classpath) {
	JavaVM* jvm;
	JNIEnv* env;
	JavaVMInitArgs args;
	JavaVMOption options[3];
	args.version = JNI_VERSION_1_6;
	args.nOptions = 3;

	// TODO: should probably not hardcode this
	char *boot_cp_arr[] = {
		"core-hostdex.jar",
		"core-junit-hostdex.jar",
		"conscrypt-hostdex.jar",
		"okhttp-hostdex.jar",
		"bouncycastle-hostdex.jar",
		"apachehttp-hostdex.jar",
		"apache-xml-hostdex.jar",
	};
	options[0].optionString = construct_boot_classpath("-Xbootclasspath:", boot_cp_arr, ARRAY_SIZE(boot_cp_arr));

	// or this
	char *cp_array[] = {
		".",
		"hax_arsc_parser.dex",
		"hax.dex",
		"%s",
		"com.google.android.gms.apk",
	};
	char *cp_format_str = construct_classpath("-Djava.class.path=", "", cp_array, ARRAY_SIZE(cp_array));
	size_t cp_len = strlen(cp_format_str) - strlen("%s") + strlen(apk_classpath) + 1;
	options[1].optionString = malloc(cp_len);
	snprintf(options[1].optionString, cp_len, cp_format_str, apk_classpath);

	options[2].optionString = "-verbose:jni";

	args.options = options;
	args.ignoreUnrecognized = JNI_FALSE;

	int ret = JNI_CreateJavaVM(&jvm, (void **)&env, &args);
	if (ret<0){
		printf("Unable to Launch JVM\n");
    } else {
		printf("JVM launched successfully\n");
	}
	return env;
}

bool _Z17dvmLoadNativeCodePKcP6ObjectPPc(const char* pathName, void* classLoader, char** detail);
void * _Z20dvmDecodeIndirectRefP6ThreadP8_jobject(void* self, jobject jobj);
void * _Z13dvmThreadSelfv(void);

struct jni_callback_data { char *apk_main_activity_class; uint32_t window_width; uint32_t window_height;};
static void open(GtkApplication *app, GFile** files, gint nfiles, const gchar* hint, struct jni_callback_data *d)
{
//TODO: pass all files to classpath
/*
	printf("nfiles: %d\n", nfiles);
	for(int i = 0; i < nfiles; i++) {
		printf(">- [%s]\n", g_file_get_path(files[i]));
	}
*/
	char *apk_classpath =  g_file_get_path(files[0]);

	if(apk_classpath == NULL) {
		printf("error: the specified file path doesn't seem to be valid\n");
		exit(1);
	}

	JNIEnv* env = create_vm(apk_classpath);

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
	(*env)->CallVoidMethod(env, handle_cache.apk_main_activity.object, handle_cache.apk_main_activity.set_window, _INTPTR(window));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	gtk_window_set_title(GTK_WINDOW(window), "com.example.demo_application");
	gtk_window_set_default_size(GTK_WINDOW(window), d->window_width, d->window_height);
	g_signal_connect(window, "close-request", G_CALLBACK (app_exit), env);

//	TODO: set icon according to how android gets it for the purposes of displaying it in the launcher
//	gtk_window_set_icon_name(window, "weather-clear");

	gtk_widget_show(window);

	/* -- register our JNI library under the appropriate classloader -- */

	/* 'android/view/View' is part of the "hax.dex" package, any other function from that package would serve just as well */
	jmethodID getClassLoader = _METHOD((*env)->FindClass(env, "java/lang/Class"), "getClassLoader", "()Ljava/lang/ClassLoader;");
	jobject class_loader = (*env)->CallObjectMethod(env, handle_cache.view.class, getClassLoader);

	char* reason = NULL;
	if (!_Z17dvmLoadNativeCodePKcP6ObjectPPc("libtranslation_layer_main.so", _Z20dvmDecodeIndirectRefP6ThreadP8_jobject(_Z13dvmThreadSelfv(), class_loader), &reason)) {
		printf("[main] dvmLoadNativeCode failed for libtranslation_layer_main.so: %s", reason);
		exit(1);
	}

	/* -- run the main activity's onCreate -- */

	(*env)->CallVoidMethod(env, handle_cache.apk_main_activity.object, handle_cache.apk_main_activity.onCreate, NULL);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

//	TODO: some apps wait for this to actually do stuff
/*	(*env)->CallVoidMethod(env, handle_cache.apk_main_activity.object, handle_cache.apk_main_activity.onWindowFocusChanged, true);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);*/
}

static void activate(GtkApplication *app, struct jni_callback_data *d)
{
	printf("error: usage: ./main [app.apk] [path/to/activity]\nyou can specify --help to see the list of options\n");
	exit(1);
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

int main(int argc, char **argv/*, JNIEnv *env*/)
{
 	GtkApplication *app;
 	int status;

	struct jni_callback_data *callback_data = malloc(sizeof(struct jni_callback_data));
	callback_data->apk_main_activity_class = NULL;
	callback_data->window_width = 960;
	callback_data->window_height = 540;

 	app = gtk_application_new("com.example.demo_application", G_APPLICATION_NON_UNIQUE | G_APPLICATION_HANDLES_OPEN);

	// cmdline related setup
	init_cmd_parameters(G_APPLICATION(app), callback_data);
	g_application_set_option_context_summary(G_APPLICATION(app), "actual usage:\nLD_PRELOAD=libpthread_bio.so ./dalvik/dalvik -verbose:jni -cp hax_arsc_parser.dex:hax_xmlpull.dex:hax.dex:main.dex:${1}:com.google.android.gms.apk org/launch/main ${2}\nwhere ${1} is the path to the apk and ${2} is the cmdline");

 	g_signal_connect(app, "activate", G_CALLBACK (activate), callback_data);
 	g_signal_connect(app, "open", G_CALLBACK (open), callback_data);
 	status = g_application_run(G_APPLICATION(app), argc, argv);
 	g_object_unref(app);

 	return status;
}

const char dl_loader[] __attribute__((section(".interp"))) =
    "/lib/ld-linux.so.2";
