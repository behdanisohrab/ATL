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

GtkSizeRequestMode wrapper_widget_get_request_mode(GtkWidget *widget)
{
	WrapperWidget *wrapper = WRAPPER_WIDGET(widget);
	return gtk_widget_get_request_mode(wrapper->child);
}

void wrapper_widget_measure(GtkWidget *widget, GtkOrientation orientation, int for_size, int *minimum, int *natural, int *minimum_baseline, int *natural_baseline)
{
	WrapperWidget *wrapper = WRAPPER_WIDGET(widget);
	gtk_widget_measure(wrapper->child, orientation, for_size, minimum, natural, minimum_baseline, natural_baseline);
}

void wrapper_widget_allocate(GtkWidget *widget, int width, int height, int baseline)
{
	WrapperWidget *wrapper = WRAPPER_WIDGET(widget);
	GtkAllocation allocation = {
		.x = 0,
		.y = 0,
		.width = width,
		.height = height,
	};

	if (wrapper->computeScroll_method) {
		// The child needs to know its size before calling computeScroll, so we allocate it twice.
		// second allocate will not trigger onLayout, because of unchanged size
		gtk_widget_size_allocate(wrapper->child, &allocation, baseline);

		JNIEnv *env;
		(*wrapper->jvm)->GetEnv(wrapper->jvm, (void**)&env, JNI_VERSION_1_6);
		(*env)->CallVoidMethod(env, wrapper->jobj, wrapper->computeScroll_method);
		if((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);
		allocation.x = -(*env)->CallIntMethod(env, wrapper->jobj, handle_cache.view.getScrollX);
		allocation.y = -(*env)->CallIntMethod(env, wrapper->jobj, handle_cache.view.getScrollY);
	}

	gtk_widget_size_allocate(wrapper->child, &allocation, baseline);
	if (wrapper->sk_area)
		gtk_widget_size_allocate(wrapper->sk_area, &allocation, baseline);
}

static void wrapper_widget_class_init(WrapperWidgetClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

	object_class->dispose = wrapper_widget_dispose;

	widget_class->get_request_mode = wrapper_widget_get_request_mode;
	widget_class->measure = wrapper_widget_measure;
	widget_class->size_allocate = wrapper_widget_allocate;
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
	if (wrapper->computeScroll_method)
		gtk_widget_queue_allocate(GTK_WIDGET(wrapper));
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

	jmethodID ontouchevent_method = _METHOD(_CLASS(jobj), "onTouchEvent", "(Landroid/view/MotionEvent;)Z");
	if (ontouchevent_method != handle_cache.view.onTouchEvent) {
		/* use wrapper->child since the jobject may not have the "widget" variable set yet */
		_setOnTouchListener(env, jobj, wrapper->child, NULL);
	}

	jmethodID computeScroll_method = _METHOD(_CLASS(jobj), "computeScroll", "()V");
	if (computeScroll_method != handle_cache.view.computeScroll) {
		wrapper->computeScroll_method = computeScroll_method;
	}
}
