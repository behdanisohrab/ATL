#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "../sk_area/sk_area.h"

#include "../generated_headers/android_view_View.h"

#include "WrapperWidget.h"

G_DEFINE_TYPE(WrapperWidget, wrapper_widget, GTK_TYPE_WIDGET)

static void wrapper_widget_init (WrapperWidget *wrapper_widget)
{

}

static void wrapper_widget_dispose(GObject *wrapper_widget)
{
	gtk_widget_unparent(gtk_widget_get_first_child(GTK_WIDGET(wrapper_widget)));
	WrapperWidget *wrapper = WRAPPER_WIDGET(wrapper_widget);
	if (wrapper->jvm) {
		JNIEnv *env;
		(*wrapper->jvm)->GetEnv(wrapper->jvm, (void**)&env, JNI_VERSION_1_6);
		if (wrapper->jobj)
			_UNREF(wrapper->jobj);
		if (wrapper->canvas)
			_UNREF(wrapper->canvas);
	}
	G_OBJECT_CLASS (wrapper_widget_parent_class)->dispose (wrapper_widget);
}

void skia_draw_func(SKArea *sk_area, sk_canvas_t *canvas, void *user_data)
{
	WrapperWidget *wrapper_widget = WRAPPER_WIDGET(user_data);
	JNIEnv *env;
	(*wrapper_widget->jvm)->GetEnv(wrapper_widget->jvm, (void**)&env, JNI_VERSION_1_6);
	if(wrapper_widget->canvas == NULL) {
		wrapper_widget->canvas = _REF((*env)->NewObject(env, handle_cache.canvas.class, handle_cache.canvas.constructor, _INTPTR(canvas), 0));
	} else {
		_SET_LONG_FIELD(wrapper_widget->canvas, "skia_canvas", _INTPTR(canvas));
	}

	(*env)->CallVoidMethod(env, wrapper_widget->jobj, wrapper_widget->draw_method, wrapper_widget->canvas);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

/*void wrapper_snapshot(GtkWidget* widget, GtkSnapshot* snapshot)
{
	gtk_widget_snapshot_child(widget, gtk_widget_get_first_child(widget), snapshot);
}*/


static void wrapper_widget_class_init(WrapperWidgetClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

	object_class->dispose = wrapper_widget_dispose;

//	widget_class->snapshot = wrapper_snapshot;

	gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

GtkWidget * wrapper_widget_new(void)
{
	return g_object_new (wrapper_widget_get_type(), NULL);
}

void wrapper_widget_set_child(WrapperWidget *parent, GtkWidget *child) // TODO: make sure there can only be one child
{
	gtk_widget_insert_before(child, GTK_WIDGET(parent), NULL);
	parent->child = child;
}

#define MEASURE_SPEC_EXACTLY (1 << 30)

static void on_mapped(GtkWidget* self, gpointer data)
{
	WrapperWidget *wrapper = WRAPPER_WIDGET(self);
	if (wrapper->jvm) {
		JNIEnv *env;
		(*wrapper->jvm)->GetEnv(wrapper->jvm, (void**)&env, JNI_VERSION_1_6);

		(*env)->CallVoidMethod(env, wrapper->jobj, wrapper->measure_method, MEASURE_SPEC_EXACTLY | gtk_widget_get_width(self), MEASURE_SPEC_EXACTLY | gtk_widget_get_height(self));
		int width = (*env)->CallIntMethod(env, wrapper->jobj, handle_cache.view.getMeasuredWidth);
		if (width > 0)
			g_object_set(G_OBJECT(self), "width-request", width, NULL);
		int height = (*env)->CallIntMethod(env, wrapper->jobj, handle_cache.view.getMeasuredHeight);
		if (height > 0)
			g_object_set(G_OBJECT(self), "height-request", height, NULL);
	}
}

static guint sk_area_queue_queue_redraw(GtkWidget *sk_area)
{
	gtk_widget_queue_draw(sk_area);
	return G_SOURCE_REMOVE;
}

void wrapper_widget_queue_draw(WrapperWidget *wrapper)
{
	if(wrapper->sk_area) {
		/* schedule the call to gtk_widget_queue_draw for a future event loop pass in case we're currently inside the sk_area's snapshot */
		/* GTK+ uses G_PRIORITY_HIGH_IDLE + 10 for resizing operations, and G_PRIORITY_HIGH_IDLE + 20 for redrawing operations. */
		g_idle_add_full(G_PRIORITY_HIGH_IDLE + 20, G_SOURCE_FUNC(sk_area_queue_queue_redraw), wrapper->sk_area, NULL);
	}

	if(wrapper->child)
		gtk_widget_queue_draw(wrapper->child);
	gtk_widget_queue_draw(GTK_WIDGET(wrapper));
}

void wrapper_widget_set_jobject(WrapperWidget *wrapper, JNIEnv *env, jobject jobj)
{
	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);
	wrapper->jvm = jvm;
	wrapper->jobj = _REF(jobj);
	jmethodID draw_method = _METHOD(_CLASS(jobj), "onDraw", "(Landroid/graphics/Canvas;)V");
	if (draw_method != handle_cache.view.onDraw) {
		wrapper->draw_method = draw_method;

		GtkWidget *sk_area = sk_area_new();
		gtk_widget_set_sensitive(sk_area, false);
		sk_area_set_draw_func(SK_AREA_WIDGET(sk_area), skia_draw_func, wrapper);
		gtk_widget_insert_before(sk_area, GTK_WIDGET(wrapper), NULL);
		wrapper->sk_area = sk_area;
//		gtk_widget_add_tick_callback(sk_area, tick_callback, NULL, NULL);
	}

	jmethodID measure_method = _METHOD(_CLASS(jobj), "onMeasure", "(II)V");
	if (measure_method != handle_cache.view.onMeasure) {
		wrapper->measure_method = measure_method;
		// add a callback for when the widget is mapped, which will call onMeasure to figure out what size the widget wants to be
		g_signal_connect(wrapper, "map", G_CALLBACK(on_mapped), NULL);
	}

	jmethodID ontouchevent_method = _METHOD(_CLASS(jobj), "onTouchEvent", "(Landroid/view/MotionEvent;)Z");
	if (ontouchevent_method != handle_cache.view.onTouchEvent) {
		/* use wrapper->child since the jobject may not have the "widget" variable set yet */
		_setOnTouchListener(env, jobj, wrapper->child, NULL);
	}
}
