#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"
#include "marshal.h"

#include "../generated_headers/android_view_SurfaceView.h"

// TODO: currently this widget class doesn't do anything special, will we ever need it to?
G_DECLARE_FINAL_TYPE (SurfaceViewWidget, surface_view_widget, SURFACE_VIEW, WIDGET, GtkWidget)

struct _SurfaceViewWidget
{
	GtkWidget parent_instance;
};

struct _SurfaceViewWidgetClass
{
	GtkWidgetClass parent_class;
};


G_DEFINE_TYPE(SurfaceViewWidget, surface_view_widget, GTK_TYPE_WIDGET)

static void surface_view_widget_init (SurfaceViewWidget *surface_view_widget)
{

}

// resize signal copied from GtkDrawingArea
enum {
  RESIZE,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0, };

static void surface_view_widget_size_allocate(GtkWidget *widget, int width, int height, int baseline)
{
	g_signal_emit(widget, signals[RESIZE], 0, width, height);
}

static void surface_view_widget_class_init(SurfaceViewWidgetClass *class)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

	// resize signal copied from GtkDrawingArea
	widget_class->size_allocate = surface_view_widget_size_allocate;

	signals[RESIZE] =
		g_signal_new("resize",
		             G_TYPE_FROM_CLASS (class),
		             G_SIGNAL_RUN_LAST,
		             G_STRUCT_OFFSET (GtkDrawingAreaClass, resize),
		             NULL, NULL,
		             g_cclosure_user_marshal_VOID__INT_INT,
		             G_TYPE_NONE, 2, G_TYPE_INT, G_TYPE_INT);
	g_signal_set_va_marshaller(signals[RESIZE],
	                           G_TYPE_FROM_CLASS (class),
	                           g_cclosure_user_marshal_VOID__INT_INTv);
}

GtkWidget * surface_view_widget_new(void)
{
	return g_object_new (surface_view_widget_get_type(), NULL);
}

// ---

struct jni_callback_data { JavaVM *jvm; jobject this; jclass this_class;};

static void on_resize(GtkWidget* self, gint width, gint height, struct jni_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	// TODO: are there cases where returning RGBA_8888 is a bad idea?
	// NOTE: we want to call the private method of android.view.SurfaceView, not the related method with this name in the API
	(*env)->CallVoidMethod(env, d->this, _METHOD(d->this_class, "surfaceChanged", "(Landroid/view/SurfaceHolder;III)V"),
	                       _GET_OBJ_FIELD(d->this, "mSurfaceHolder", "Landroid/view/SurfaceHolder;"), 1 /*RGBA_8888*/,
	                       width, height);
}

JNIEXPORT void JNICALL Java_android_view_SurfaceView_native_1constructor(JNIEnv *env, jobject this, jobject context)
{
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *dummy = surface_view_widget_new();
	gtk_widget_set_name(dummy, "dummy widget for SurfaceView");
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), dummy);
	// TODO: is this correct for all usecases? how do we know when it's not?
	gtk_widget_set_hexpand(wrapper, true);
	gtk_widget_set_vexpand(wrapper, true);

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct jni_callback_data *callback_data = malloc(sizeof(struct jni_callback_data));
	callback_data->jvm = jvm;
	callback_data->this = _REF(this);
	callback_data->this_class = _REF((*env)->FindClass(env, "android/view/SurfaceView"));

	g_signal_connect(dummy, "resize", G_CALLBACK(on_resize), callback_data);

	_SET_LONG_FIELD(this, "widget", _INTPTR(dummy));
}
