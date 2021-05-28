#include "org_launch_main.h"
#include "android_app_Activity.h"
#include "android_view_LinearLayout.h"
#include "android_view_ViewGroup.h"
#include "android_widget_TextView.h"

#include <gtk/gtk.h>

// these macros are a bit hacky, since they deliberately assume that env exists and refers to the JNI env

#define _PTR(ptr)((void*)(int)ptr)
#define _REF(obj)((*env)->NewGlobalRef(env, obj))
#define _CLASS(object) ((*env)->GetObjectClass(env, object))
#define _SUPER(object) ((*env)->GetSuperclass(env, object))
#define _METHOD(class, method, attrs) ((*env)->GetMethodID(env, class, method, attrs))
#define _JSTRING(cstring) ((*env)->NewStringUTF(env, cstring))
#define _CSTRING(jstring) ((*env)->GetStringUTFChars(env, jstring, NULL))
#define _FIELD_ID(class, field, type) ((*env)->GetFieldID(env, class , field, type))
#define _SET_OBJ_FIELD(object, field, type, value) ((*env)->SetObjectField(env, object, _FIELD_ID(_CLASS(object), field, type), value))
#define _GET_OBJ_FIELD(object, field, type) ((*env)->GetObjectField(env, object, _FIELD_ID(_CLASS(object), field, type)))
#define _SET_LONG_FIELD(object, field, value) ((*env)->SetLongField(env, object, _FIELD_ID(_CLASS(object), field, "J"), value))
#define _GET_LONG_FIELD(object, field) ((*env)->GetLongField(env, object, _FIELD_ID(_CLASS(object), field, "J")))

GtkWidget *window;

struct handle_cache {
	struct {
		jclass class;
		jobject object;
		jmethodID onCreate;
	} apk_main_activity;
	struct {
		jclass class;
		jmethodID getAttributeValue_string;
		jmethodID getAttributeValue_int;
	} attribute_set;
	struct {
		jclass class;
		jmethodID add;
	} array_list;
};

static struct handle_cache handle_cache = {0};

const char * attribute_set_get_string(JNIEnv *env, jobject attrs, char *attribute, char *schema)
{
	if(!schema)
		schema = "http://schemas.android.com/apk/res/android";

	return _CSTRING( (jstring)(*env)->CallObjectMethod(env, attrs, handle_cache.attribute_set.getAttributeValue_string, _JSTRING(schema), _JSTRING(attribute)) );
}

int attribute_set_get_int(JNIEnv *env, jobject attrs, char *attribute, char *schema, int default_value)
{
	if(!schema)
		schema = "http://schemas.android.com/apk/res/android";

	return (*env)->CallIntMethod(env, attrs, handle_cache.attribute_set.getAttributeValue_int, _JSTRING(schema), _JSTRING(attribute), default_value);
}

void set_up_handle_cache(JNIEnv *env, char *apk_main_activity_class)
{
	handle_cache.apk_main_activity.class = (*env)->FindClass(env, "com/example/demo_application/MainActivity");
	handle_cache.apk_main_activity.object = _REF((*env)->NewObject(env, handle_cache.apk_main_activity.class, _METHOD(handle_cache.apk_main_activity.class, "<init>", "()V")));
	handle_cache.apk_main_activity.onCreate = (*env)->GetMethodID(env, handle_cache.apk_main_activity.class, "onCreate", "(Landroid/os/Bundle;)V");

	handle_cache.attribute_set.class = (*env)->FindClass(env, "android/util/AttributeSet");
	handle_cache.attribute_set.getAttributeValue_string = _METHOD(handle_cache.attribute_set.class, "getAttributeValue", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
	handle_cache.attribute_set.getAttributeValue_int = _METHOD(handle_cache.attribute_set.class, "getAttributeIntValue", "(Ljava/lang/String;Ljava/lang/String;I)I");

	handle_cache.array_list.class = (*env)->FindClass(env, "java/util/ArrayList");
	handle_cache.array_list.add = _METHOD(handle_cache.array_list.class, "add", "(Ljava/lang/Object;)Z");
}


/* --- */

JNIEXPORT void JNICALL Java_android_app_Activity_set_1widget_1as_1root(JNIEnv *env, jobject this, jlong widget)
{
	gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(_PTR(widget)));
}

/* --- */

JNIEXPORT void JNICALL Java_android_view_LinearLayout_native_1constructor(JNIEnv *env, jobject this, jobject attrs)
{
	int orientation = attribute_set_get_int(env, attrs, "orientation", NULL, 0);

	GtkWidget *box = gtk_box_new(orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	_SET_LONG_FIELD(this, "widget", (long)box);
	g_object_ref(box);
}
/* --- */

JNIEXPORT void JNICALL Java_android_view_LinearLayout_addView(JNIEnv *env, jobject this, jobject child, jint index, jobject layout_params)
{
	Java_android_view_ViewGroup_addView(env, this, child, index, layout_params);

	gtk_box_append(GTK_BOX(_PTR(_GET_LONG_FIELD(this, "widget"))), GTK_WIDGET(_PTR(_GET_LONG_FIELD(child, "widget")))); // FIXME - ignores index argument
}

/* --- */

JNIEXPORT void JNICALL Java_android_view_ViewGroup_addView(JNIEnv *env, jobject this, jobject child, jint index, jobject layout_params)
{
	_SET_OBJ_FIELD(child, "parent", "Landroid/view/ViewGroup;", this);

	jobject children = _GET_OBJ_FIELD(this, "children", "Ljava/util/ArrayList;");

	(*env)->CallVoidMethod(env, children, handle_cache.array_list.add, child);
}

/* --- */

JNIEXPORT void JNICALL Java_android_widget_TextView_native_1constructor(JNIEnv *env, jobject this, jobject attrs)
{
	const char *text = attribute_set_get_string(env, attrs, "text", NULL);

	_SET_OBJ_FIELD(this, "text", "Ljava/lang/String;", _JSTRING(text));

	GtkWidget *label = gtk_label_new(text);
	_SET_LONG_FIELD(this, "widget", (long)label);
	g_object_ref(label);
}

JNIEXPORT void JNICALL Java_android_widget_TextView_setText(JNIEnv *env, jobject this, jobject charseq)
{
		_SET_OBJ_FIELD(this, "text", "Ljava/lang/String;", charseq);

		gtk_label_set_text(GTK_LABEL(_PTR(_GET_LONG_FIELD(this, "widget"))), _CSTRING(charseq));
}

/* --- */

int main (int argc, char **argv, JNIEnv *env);

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

static void activate (GtkApplication *app, JNIEnv *env)
{
 	window = gtk_application_window_new (app);
 	gtk_window_set_title(GTK_WINDOW(window), "com.example.demo_application");
 	gtk_window_set_default_size(GTK_WINDOW(window), 540, 960);

 	gtk_widget_show(window);

	/* -- run the main activity's onCreate -- */

	(*env)->CallVoidMethod(env, handle_cache.apk_main_activity.object, handle_cache.apk_main_activity.onCreate, NULL);
}

int main (int argc, char **argv, JNIEnv *env)
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
