// for dladdr
#define _GNU_SOURCE

#include <gtk/gtk.h>
#include <gdk/wayland/gdkwayland.h>
#include <libportal/portal.h>

#include "../api-impl-jni/defines.h"
#include "../api-impl-jni/util.h"
#include "../api-impl-jni/app/android_app_Activity.h"

#include <dlfcn.h>
#include <errno.h>
#include <libgen.h>
#include <locale.h>
#include <stdio.h>
#include <sys/stat.h>

#ifndef DEFFILEMODE
#define DEFFILEMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)/* 0666*/
#endif

#ifdef __x86_64__
#define NATIVE_ARCH "x86_64"
#elifdef __i386__
#define NATIVE_ARCH "x86"
#elifdef __aarch64__
#define NATIVE_ARCH "arm64-v8a"
#elifdef __arm__
#define NATIVE_ARCH "armeabi-v7a"
#else
#error unknown native architecture
#endif

GtkWidget *window;

// standard Gtk Application stuff, more or less

gboolean app_exit(GtkWindow* self, JNIEnv *env) // TODO: do more cleanup?
{
	activity_close_all();
	return false;
}

// this is the equivalent of /data/data/com.example.app/
char *app_data_dir = NULL;
char *get_app_data_dir()
{
	return app_data_dir;
}

char *construct_classpath(char *prefix, char **cp_array, size_t len)
{
	size_t result_len = strlen(prefix);
	for(int i = 0; i < len; i++) {
		if(cp_array[i])
			result_len += strlen(cp_array[i]) + 1; // the 1 is for either : or the final \0
	}

	char *result = malloc(result_len);
	strcpy(result, prefix);
	for(int i = 0; i < len; i++) {
		if(cp_array[i]) {
			if (i > 0)
				strcat(result, ":");
			strcat(result, cp_array[i]);
		}
	}

	printf("construct_classpath: returning >%s<\n", result);

	return result;
}

#define JDWP_ARG "-XjdwpOptions:transport=dt_socket,server=y,suspend=y,address="

JNIEnv* create_vm(char *api_impl_jar, char *apk_classpath, char *microg_apk, char *framework_res_apk, char *api_impl_natives_dir, char *app_lib_dir, char **extra_jvm_options) {
	JavaVM* jvm;
	JNIEnv* env;
	JavaVMInitArgs args = {
		.version = JNI_VERSION_1_6,
		.nOptions = 3,
	};
	JavaVMOption *options;

	int option_counter = args.nOptions;

	char jdwp_option_string[sizeof(JDWP_ARG) + 5] = JDWP_ARG; // 5 chars for port number, NULL byte is counted by sizeof

	const char* jdwp_port = getenv("JDWP_LISTEN");

	if(jdwp_port)
		args.nOptions += 2;
	if(extra_jvm_options)
		args.nOptions += g_strv_length(extra_jvm_options);
	options = malloc(sizeof(JavaVMOption) * args.nOptions);


	if(getenv("RUN_FROM_BUILDDIR")) {
		options[0].optionString = construct_classpath("-Djava.library.path=", (char *[]){"./", app_lib_dir}, 2);
	} else {
		options[0].optionString = construct_classpath("-Djava.library.path=", (char *[]){api_impl_natives_dir, app_lib_dir}, 2);
	}

	options[1].optionString = construct_classpath("-Djava.class.path=", (char *[]){api_impl_jar, apk_classpath, microg_apk, framework_res_apk}, 4);
	options[2].optionString = "-Xcheck:jni";
	if(jdwp_port) {
		strncat(jdwp_option_string, jdwp_port, 5); // 5 chars is enough for a port number, and won't overflow our array
		options[option_counter++].optionString = "-XjdwpProvider:internal";
		options[option_counter++].optionString = jdwp_option_string;
	}

	while(extra_jvm_options && *extra_jvm_options) {
		options[option_counter++].optionString = *(extra_jvm_options++);
	}

	args.options = options;
	args.ignoreUnrecognized = JNI_FALSE;

	int ret = JNI_CreateJavaVM(&jvm, (void **)&env, &args);
	if(ret < 0) {
		printf("Unable to Launch JVM\n");
	} else {
		printf("JVM launched successfully\n");
	}

	free(options);
	return env;
}

void icon_override(GtkWidget *window, GList *icon_list) {
	GdkSurface *window_surface = gtk_native_get_surface(GTK_NATIVE(window));
	// set app icon as window icon; this is a noop on Wayland because there is currently no way to set a window icon on Wayland
	gdk_toplevel_set_icon_list(GDK_TOPLEVEL(window_surface), icon_list);
}

/*
 * There is no way to get a nice clean callback for when the window is ready to be used for stuff
 * that requires non-zero dimensions, so we just check periodically
 */
gboolean hacky_on_window_focus_changed_callback(JNIEnv *env)
{
	if(gtk_widget_get_width(window) != 0) {
		activity_window_ready();
		return G_SOURCE_REMOVE;
	}

	return G_SOURCE_CONTINUE;
}

struct dynamic_launcher_callback_data {char *desktop_file_id; char *desktop_entry;};
static void dynamic_launcher_ready_callback(GObject *portal, GAsyncResult *res, gpointer user_data) {
	struct dynamic_launcher_callback_data *data = user_data;
	GVariant *result = xdp_portal_dynamic_launcher_prepare_install_finish(XDP_PORTAL(portal), res, NULL);
	if (!result) {
		printf("cancelled\n");
		exit(0);
	}
	const char *token;
	g_variant_lookup(result, "token", "s", &token);
	GError *err = NULL;
	xdp_portal_dynamic_launcher_install(XDP_PORTAL(portal), token, data->desktop_file_id, data->desktop_entry, &err);
	g_free(data->desktop_file_id);
	g_free(data->desktop_entry);
	g_free(data);
	if (err) {
		printf("failed to install dynamic launcher: %s\n", err->message);
		exit(1);
	}
	exit(0);
}

// this is exported by the shim bionic linker
void dl_parse_library_path(const char *path, char *delim);

#define REL_DEX_INSTALL_PATH "/../java/dex"

#define REL_API_IMPL_JAR_INSTALL_PATH "/android_translation_layer/api-impl.jar"
#define REL_API_IMPL_NATIVES_INSTALL_PATH "/android_translation_layer/natives"
#define REL_MICROG_APK_INSTALL_PATH "/microg/com.google.android.gms.apk"
#define REL_FRAMEWORK_RES_INSTALL_PATH "/android_translation_layer/framework-res.apk"

#define API_IMPL_JAR_PATH_LOCAL "./api-impl.jar"
#define MICROG_APK_PATH_LOCAL "./com.google.android.gms.apk"
#define FRAMEWORK_RES_PATH_LOCAL "./res/framework-res.apk"

struct jni_callback_data {
	char *apk_main_activity_class;
	uint32_t window_width;
	uint32_t window_height;
	gboolean install;
	char *prgname;
	char **extra_jvm_options;
	char **extra_string_keys;
};

static void open(GtkApplication *app, GFile** files, gint nfiles, const gchar* hint, struct jni_callback_data *d)
{
//TODO: pass all files to classpath
/*
	printf("nfiles: %d\n", nfiles);
	for(int i = 0; i < nfiles; i++) {
		printf(">- [%s]\n", g_file_get_path(files[i]));
	}
*/
	char *dex_install_dir;
	char *api_impl_jar;
	char *microg_apk = NULL;
	char *framework_res_apk = NULL;
	const char *package_name;
	int errno_libdir;
	int errno_localdir;
	int ret;
	jobject activity_object;
	jobject application_object;

	char *apk_classpath =  g_file_get_path(files[0]);
	char *apk_name = g_file_get_basename(files[0]);

	if(apk_classpath == NULL) {
		printf("error: the specified file path doesn't seem to be valid\n");
		exit(1);
	}

	if(access(apk_classpath, F_OK) < 0) {
		printf("error: the specified file path doesn't seem to exist (%m)\n");
		exit(1);
	}

	Dl_info libart_so_dl_info;
	// JNI_CreateJavaVM chosen arbitrarily, what matters is that it's a symbol exported by by libart.so
	// TODO: we shouldn't necessarily count on art being installed in the same prefix as we are
	dladdr(JNI_CreateJavaVM, &libart_so_dl_info);
	// make sure we didn't get NULL
	if(libart_so_dl_info.dli_fname) {
		// it's simpler if we can modify the string, so strdup it
		char *libart_so_full_path = strdup(libart_so_dl_info.dli_fname);
		*strrchr(libart_so_full_path, '/') = '\0'; // now we should have something like /usr/lib64/art
		dex_install_dir = malloc(strlen(libart_so_full_path) + strlen(REL_DEX_INSTALL_PATH) + 1); // +1 for NULL
		strcpy(dex_install_dir, libart_so_full_path);
		strcat(dex_install_dir, REL_DEX_INSTALL_PATH);
		free(libart_so_full_path);
	} else {
		dex_install_dir = "DIDN'T_GET_SO_PATH_WITH_dladdr_SUS"; // in case we print this as part of some other error, it should be clear what the real cause is
	}

	char* app_data_dir_base = getenv("ANDROID_APP_DATA_DIR");
	if(!app_data_dir_base) {
		const char* user_data_dir = g_get_user_data_dir();
		if(user_data_dir) {
			app_data_dir_base = g_strdup_printf("%s/android_translation_layer", user_data_dir);
			ret = mkdir(app_data_dir_base, DEFFILEMODE | S_IXUSR | S_IXGRP | S_IXOTH);
			if (ret) {
				if(errno != EEXIST) {
					fprintf(stderr, "error: ANDROID_APP_DATA_DIR not set, and the default directory (%s) coudldn't be created (error: %s)\n", app_data_dir_base, strerror(errno));
					exit(1);
				}
			}
		} else {
			fprintf(stderr, "error: ANDROID_APP_DATA_DIR not set, and HOME is not set either so we can't construct the default path\n");
			exit(1);
		}
	}
	app_data_dir = malloc(strlen(app_data_dir_base) + 1 + strlen(apk_name) + 1 + 1 + 1); // +1 for middle '/', + 1 for _, +1 for end '/', and +1 for NULL
	strcpy(app_data_dir, app_data_dir_base);
	strcat(app_data_dir, "/");
	// TODO: we should possibly use the app id instead, but we don't currently have a way to get that soon enough
	// arguably both the app id and the apk name might have an issue with duplicates, but if two apks use the same app id, chances are it's less of an issue than when two apks have the same name
	strcat(app_data_dir, apk_name);
	strcat(app_data_dir, "_"); // !IMPORTANT! Unity can't comprehend that a directory name could end in .apk, so we have to avoid that here
	strcat(app_data_dir, "/");

	ret = mkdir(app_data_dir, DEFFILEMODE | S_IXUSR | S_IXGRP | S_IXOTH);
	if(ret && errno != EEXIST) {
		fprintf(stderr, "can't create app data dir %s (%s)\n", app_data_dir, strerror(errno));
		exit(1);
	}

	// check for api-impl.jar and com.google.android.gms.apk in './' first (for running from builddir), and in system install path second
	struct stat dont_care;
	ret = stat(API_IMPL_JAR_PATH_LOCAL, &dont_care);
	errno_localdir = errno;
	if(!ret) {
		api_impl_jar = strdup(API_IMPL_JAR_PATH_LOCAL); // for running out of builddir; using strdup so we can always safely call free on this
	} else {
		char *api_impl_install_dir = malloc(strlen(dex_install_dir) + strlen(REL_API_IMPL_JAR_INSTALL_PATH) + 1); // +1 for NULL
		strcpy(api_impl_install_dir, dex_install_dir);
		strcat(api_impl_install_dir, REL_API_IMPL_JAR_INSTALL_PATH);

		ret = stat(api_impl_install_dir, &dont_care);
		errno_libdir = errno;
		if(!ret) {
			api_impl_jar = api_impl_install_dir;
		} else {
			printf("error: can't stat api-impl.jar; tried:\n"
				   "\t\"" API_IMPL_JAR_PATH_LOCAL "\", got - %s\n"
				   "\t\"%s\", got - %s\n",
			       strerror(errno_localdir), 
			       api_impl_install_dir, strerror(errno_libdir));
			exit(1);
		}
	}

	ret = stat(MICROG_APK_PATH_LOCAL, &dont_care);
	errno_localdir = errno;
	if(!ret) {
		microg_apk = strdup(MICROG_APK_PATH_LOCAL); // for running out of builddir; using strdup so we can always safely call free on this
	} else {
		char *microg_install_dir = malloc(strlen(dex_install_dir) + strlen(REL_MICROG_APK_INSTALL_PATH) + 1); // +1 for NULL
		strcpy(microg_install_dir, dex_install_dir);
		strcat(microg_install_dir, REL_MICROG_APK_INSTALL_PATH);

		ret = stat(microg_install_dir, &dont_care);
		errno_libdir = errno;
		if(!ret) {
			microg_apk = microg_install_dir;
		} else {
			printf("warning: can't stat com.google.android.gms.apk; tried:\n"
				   "\t\"" MICROG_APK_PATH_LOCAL "\", got - %s\n"
				   "\t\"%s\", got - %s\n",
			       strerror(errno_localdir),
			       microg_install_dir, strerror(errno_libdir));
		}
	}

	ret = stat(FRAMEWORK_RES_PATH_LOCAL, &dont_care);
	errno_localdir = errno;
	if(!ret) {
		framework_res_apk = strdup(FRAMEWORK_RES_PATH_LOCAL); // for running out of builddir; using strdup so we can always safely call free on this
	} else {
		char *framework_res_install_dir = malloc(strlen(dex_install_dir) + strlen(REL_FRAMEWORK_RES_INSTALL_PATH) + 1); // +1 for NULL
		strcpy(framework_res_install_dir, dex_install_dir);
		strcat(framework_res_install_dir, REL_FRAMEWORK_RES_INSTALL_PATH);

		ret = stat(framework_res_install_dir, &dont_care);
		errno_libdir = errno;
		if(!ret) {
			framework_res_apk = framework_res_install_dir;
		} else {
			printf("warning: can't stat framework-res.apk; tried:\n"
				   "\t\"" FRAMEWORK_RES_PATH_LOCAL "\", got - %s\n"
				   "\t\"%s\", got - %s\n",
			       strerror(errno_localdir),
			       framework_res_install_dir, strerror(errno_libdir));
		}
	}

	char *api_impl_natives_dir = malloc(strlen(dex_install_dir) + strlen(REL_API_IMPL_NATIVES_INSTALL_PATH) + 1); // +1 for NULL
	strcpy(api_impl_natives_dir, dex_install_dir);
	strcat(api_impl_natives_dir, REL_API_IMPL_NATIVES_INSTALL_PATH);

	char *app_lib_dir = malloc(strlen(app_data_dir) + strlen("/lib") + 1); // +1 for NULL
	strcpy(app_lib_dir, app_data_dir);
	strcat(app_lib_dir, "/lib");
	// create lib dir
	mkdir(app_lib_dir, DEFFILEMODE | S_IXUSR | S_IXGRP | S_IXOTH);

	// calling directly into the shim bionic linker to whitelist the app's lib dir as containing bionic-linked libraries
	dl_parse_library_path(app_lib_dir, ":");

	JNIEnv* env = create_vm(api_impl_jar, apk_classpath, microg_apk, framework_res_apk, api_impl_natives_dir, app_lib_dir, d->extra_jvm_options);

	free(app_lib_dir);

	jclass display_class = (*env)->FindClass(env, "android/view/Display");
	_SET_STATIC_INT_FIELD(display_class, "window_width", d->window_width);
	_SET_STATIC_INT_FIELD(display_class, "window_height", d->window_height);

	/* -- register our JNI library under the appropriate classloader -- */

	/* 'android/view/View' is part of the "hax.dex" package, any other function from that package would serve just as well */
	jmethodID getClassLoader = _METHOD((*env)->FindClass(env, "java/lang/Class"), "getClassLoader", "()Ljava/lang/ClassLoader;");
	jobject class_loader = (*env)->CallObjectMethod(env, (*env)->FindClass(env, "android/view/View"), getClassLoader);

	jclass java_runtime_class = (*env)->FindClass(env, "java/lang/Runtime");

	jmethodID getRuntime = _STATIC_METHOD(java_runtime_class, "getRuntime", "()Ljava/lang/Runtime;");
	jobject java_runtime = (*env)->CallStaticObjectMethod(env, java_runtime_class, getRuntime);

	/* this method is private, but it seems we get away with calling it from C */
	jmethodID loadLibrary_with_classloader = _METHOD(java_runtime_class, "loadLibrary", "(Ljava/lang/String;Ljava/lang/ClassLoader;)V");
	(*env)->CallVoidMethod(env, java_runtime, loadLibrary_with_classloader, _JSTRING("translation_layer_main"), class_loader);

	set_up_handle_cache(env);

	/* -- misc -- */

	// some apps need the apk path since they directly read their apk
	jclass context_class = (*env)->FindClass(env, "android/content/Context");
	_SET_STATIC_OBJ_FIELD(context_class, "apk_path", "Ljava/lang/String;", _JSTRING(apk_classpath));

	window = gtk_application_window_new(app);

	if(getenv("ATL_DISABLE_WINDOW_DECORATIONS"))
		gtk_window_set_decorated(GTK_WINDOW(window), 0);

        if(getenv("ATL_FORCE_FULLSCREEN"))
                gtk_window_fullscreen(GTK_WINDOW(window));

	// construct Application
	application_object = (*env)->CallStaticObjectMethod(env, handle_cache.context.class,
		_STATIC_METHOD(handle_cache.context.class, "createApplication", "(J)Landroid/app/Application;"), window);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	/* extract native libraries from apk*/
	if(!getenv("ATL_SKIP_NATIVES_EXTRACTION"))
		extract_from_apk("lib/" NATIVE_ARCH "/", "lib/");

	prepare_main_looper(env);

	jclass content_provider = (*env)->FindClass(env, "android/content/ContentProvider");
	(*env)->CallStaticVoidMethod(env, content_provider, _STATIC_METHOD(content_provider, "createContentProviders", "()V"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	(*env)->CallVoidMethod(env, application_object, _METHOD(handle_cache.application.class, "onCreate", "()V"));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	// construct main Activity
	activity_object = (*env)->CallStaticObjectMethod(env, handle_cache.activity.class,
		_STATIC_METHOD(handle_cache.activity.class, "createMainActivity", "(Ljava/lang/String;J)Landroid/app/Activity;"),
		_JSTRING(d->apk_main_activity_class), _INTPTR(window));
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	if(d->extra_string_keys) {
		GError *error = NULL;
		GRegex *regex = g_regex_new("(?<!\\\\)=", 0, 0, &error);
		if (!regex) {
			fprintf(stderr, "g_regex_new error: '%s'\n", error->message);
			exit(1);
		}

		jobject intent = _GET_OBJ_FIELD(activity_object, "intent", "Landroid/content/Intent;");

		for(char **arg = d->extra_string_keys; *arg; arg++) {
			gchar **keyval = g_regex_split_full(regex, *arg, -1, 0, 0, 2, NULL);
			if(!keyval || !keyval[0] || !keyval[1]) {
				fprintf(stderr, "extra string arg not in 'key=value' format: '%s'\n", *arg);
				exit(1);
			}
			(*env)->CallObjectMethod(env, intent, handle_cache.intent.putExtraCharSequence, _JSTRING(keyval[0]), _JSTRING(keyval[1]));
			g_strfreev(keyval);
		}
		g_regex_unref(regex);
		g_strfreev(d->extra_string_keys);
	}

	/* -- set the window title and app icon -- */

	jstring package_name_jstr = (*env)->CallObjectMethod(env, activity_object, handle_cache.context.get_package_name);
	package_name = package_name_jstr ? _CSTRING(package_name_jstr) : NULL;
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	jstring app_icon_path_jstr = (*env)->CallObjectMethod(env, application_object, handle_cache.application.get_app_icon_path);
	const char *app_icon_path = app_icon_path_jstr ? _CSTRING(app_icon_path_jstr) : NULL;
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	if (d->install) {
		XdpPortal *portal = xdp_portal_new();

		const char *app_label = _CSTRING((*env)->CallObjectMethod(env, application_object, _METHOD(handle_cache.application.class, "get_app_label", "()Ljava/lang/String;")));
		if((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);

		GVariant *icon_serialized = NULL;
		if (app_icon_path) {
			extract_from_apk(app_icon_path, app_icon_path);
			char *app_icon_path_full = g_strdup_printf("%s/%s", app_data_dir, app_icon_path);
			GMappedFile *icon_file = g_mapped_file_new(app_icon_path_full, FALSE, NULL);
			GBytes *icon_bytes = g_mapped_file_get_bytes(icon_file);
			GIcon *icon = g_bytes_icon_new(icon_bytes);
			icon_serialized = g_icon_serialize(icon);
			g_object_unref(icon);
			g_bytes_unref(icon_bytes);
			g_mapped_file_unref(icon_file);
			g_free(app_icon_path_full);
		}
		GFile *dest = g_file_new_build_filename(app_data_dir_base, "_installed_apks_", apk_name, NULL);
		printf("installing %s to %s\n", apk_name, g_file_get_path(dest));
		g_file_make_directory(g_file_get_parent(dest), NULL, NULL);
		g_file_copy(files[0], dest, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL);

		GString *desktop_entry = g_string_new(
				"[Desktop Entry]\n"
				"Type=Application\n"
				"Exec=env ");
		if (getenv("RUN_FROM_BUILDDIR")) {
			printf("WARNING: RUN_FROM_BUILDDIR set and --install given: using current directory in desktop entry\n");
			g_string_append_printf(desktop_entry, "-C %s ", g_get_current_dir());
		}
		char *envs[] = {"RUN_FROM_BUILDDIR", "LD_LIBRARY_PATH", "ANDROID_APP_DATA_DIR", "ATL_DISABLE_WINDOW_DECORATIONS", "UGLY_HACK_FOR_VR", "ATL_FORCE_FULLSCREEN"};
		for (int i = 0; i < sizeof(envs)/sizeof(envs[0]); i++) {
			if (getenv(envs[i])) {
				g_string_append_printf(desktop_entry, "%s=%s ", envs[i], getenv(envs[i]));
			}
		}
		g_string_append_printf(desktop_entry, "%s ", d->prgname);
		g_string_append_printf(desktop_entry, "--gapplication-app-id %s ", package_name);
		if (d->apk_main_activity_class)
			g_string_append_printf(desktop_entry, "-l %s ", d->apk_main_activity_class);
		if (d->window_width)
			g_string_append_printf(desktop_entry, "-w %d ", d->window_width);
		if (d->window_height)
			g_string_append_printf(desktop_entry, "-h %d ", d->window_height);
		g_string_append_printf(desktop_entry, "%s\n", g_file_get_path(dest));
		struct dynamic_launcher_callback_data *cb_data = g_new(struct dynamic_launcher_callback_data, 1);
		cb_data->desktop_file_id = g_strdup_printf("%s.desktop", package_name);
		cb_data->desktop_entry = g_string_free(desktop_entry, FALSE);
		printf("installing %s\n\n%s\n", cb_data->desktop_file_id, cb_data->desktop_entry);
		xdp_portal_dynamic_launcher_prepare_install(portal, NULL, app_label, icon_serialized, XDP_LAUNCHER_APPLICATION, NULL, TRUE, TRUE, NULL, dynamic_launcher_ready_callback, cb_data);
		return;
	}

	gtk_window_set_title(GTK_WINDOW(window), package_name);
	gtk_window_set_default_size(GTK_WINDOW(window), d->window_width, d->window_height);
	g_signal_connect(window, "close-request", G_CALLBACK (app_exit), env);

	gtk_window_present(GTK_WINDOW(window));

	// set package name as application id for window icon on Wayland. Needs a {package_name}.desktop file defining the icon
	GdkToplevel *toplevel = GDK_TOPLEVEL(gtk_native_get_surface(GTK_NATIVE(window)));
	if (GDK_IS_WAYLAND_TOPLEVEL(toplevel)) {
		gdk_wayland_toplevel_set_application_id(GDK_WAYLAND_TOPLEVEL(toplevel), package_name);
	}

	if(app_icon_path) {
		char *app_icon_path_full = malloc(strlen(app_data_dir) + 1 + strlen(app_icon_path) + 1);  // +1 for /, +1 for NULL
		sprintf(app_icon_path_full, "%s/%s", app_data_dir, app_icon_path);

		extract_from_apk(app_icon_path, app_icon_path);

		GError *error = NULL;
		GList *icon_list = g_list_append(NULL, gdk_texture_new_from_filename(app_icon_path_full, &error));
		if(error) {
			printf("gdk_texture_new_from_filename: %s\n", error->message);
			g_clear_error(&error);
		}
		icon_override(window, icon_list);
		/* if Gtk sets the icon list to NULL, override it again */
		g_signal_connect_after(window, "realize", G_CALLBACK (icon_override), icon_list);
	}

	activity_start(env, activity_object);

	g_timeout_add(10, G_SOURCE_FUNC(hacky_on_window_focus_changed_callback), env);


	jobject input_queue_callback = g_object_get_data(G_OBJECT(window), "input_queue_callback");
	if(input_queue_callback) {
		jobject input_queue = g_object_get_data(G_OBJECT(window), "input_queue");

		(*env)->CallVoidMethod(env, input_queue_callback, handle_cache.input_queue_callback.onInputQueueCreated, input_queue);
		if((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);
	}
}

static void activate(GtkApplication *app, struct jni_callback_data *d)
{
	printf("error: usage: ./android-translation-layer [app.apk] -l [path/to/activity]\n"
	       "you can specify --help to see the list of options\n");
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
    {
      .long_name = "install",
      .short_name = 'i',
      .flags = 0,
      .arg = G_OPTION_ARG_NONE,
      .arg_data = &d->install,
      .description = "install .desktop file for the given apk",
    },
    {
      .long_name = "extra-jvm-option",
      .short_name = 'X',
      .flags = 0,
      .arg = G_OPTION_ARG_STRING_ARRAY,
      .arg_data = &d->extra_jvm_options,
      .description = "pass an additional option directly to art",
    },
    {
      .long_name = "extra-string-key",
      .short_name = 'e',
      .flags = 0,
      .arg = G_OPTION_ARG_STRING_ARRAY,
      .arg_data = &d->extra_string_keys,
      .description = "pass a string extra (-e key=value)",
    },
    {NULL}
  };

  g_application_add_main_option_entries (G_APPLICATION (app), cmd_params);
}

void init__r_debug();
void remove_ongoing_notifications();

int main(int argc, char **argv)
{
	GtkApplication *app;
	int status;

	/* this has to be done in the main executable, so might as well do it here*/
	init__r_debug();

	// locale on android always behaves as en_US, and some apps might unbeknownst to them depend on that
	// for correct functionality
	setenv("LC_ALL", "en_US", 1);

	struct jni_callback_data *callback_data = malloc(sizeof(struct jni_callback_data));
	callback_data->apk_main_activity_class = NULL;
	callback_data->window_width = 960;
	callback_data->window_height = 540;
	callback_data->install = FALSE;
	callback_data->prgname = argv[0];
	callback_data->extra_jvm_options = NULL;
	callback_data->extra_string_keys = NULL;

	app = gtk_application_new("com.example.demo_application", G_APPLICATION_NON_UNIQUE | G_APPLICATION_HANDLES_OPEN | G_APPLICATION_CAN_OVERRIDE_APP_ID);

	// cmdline related setup
	init_cmd_parameters(G_APPLICATION(app), callback_data);
	g_application_set_option_context_summary(G_APPLICATION(app), "a translation layer for running android applications natively on Linux");

	g_signal_connect(app, "activate", G_CALLBACK (activate), callback_data);
	g_signal_connect(app, "open", G_CALLBACK (open), callback_data);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	remove_ongoing_notifications();

	return status;
}

/* TODO: recall what this is doing here */
const char dl_loader[] __attribute__((section(".interp"))) = "/lib/ld-linux.so.2";
