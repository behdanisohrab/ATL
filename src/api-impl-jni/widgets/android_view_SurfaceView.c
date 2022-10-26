#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_view_SurfaceView.h"

struct jni_callback_data { JavaVM *jvm; jobject this; jclass this_class;};

static void on_mapped(GtkWidget* self, struct jni_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	(*env)->CallVoidMethod(env, d->this, _METHOD(d->this_class, "surfaceChanged", "(Landroid/view/SurfaceHolder;III)V"), _GET_OBJ_FIELD(d->this, "mSurfaceHolder", "Landroid/view/SurfaceHolder;"), 1 /*RGBA_8888*/, /*FIXME*/700, /*FIXME*/700);
}

JNIEXPORT void JNICALL Java_android_view_SurfaceView_native_1constructor(JNIEnv *env, jobject this, jobject context)
{
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *dummy = gtk_fixed_new();
	gtk_widget_set_name(dummy, "dummy widget for SurfaceView");
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), dummy);

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct jni_callback_data *callback_data = malloc(sizeof(struct jni_callback_data));
	callback_data->jvm = jvm;
	callback_data->this = _REF(this);
	callback_data->this_class = _REF(_CLASS(this));

	g_signal_connect(dummy, "map", G_CALLBACK(on_mapped), callback_data);

	_SET_LONG_FIELD(this, "widget", _INTPTR(dummy));
}
