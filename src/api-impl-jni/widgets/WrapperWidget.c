#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "../generated_headers/android_view_View.h"

#include "WrapperWidget.h"
#include "src/api-impl-jni/views/AndroidLayout.h"

G_DEFINE_TYPE(WrapperWidget, wrapper_widget, GTK_TYPE_WIDGET)

static void wrapper_widget_init (WrapperWidget *wrapper_widget)
{

}

static void wrapper_widget_dispose(GObject *wrapper_widget)
{
	GtkWidget *widget = GTK_WIDGET(wrapper_widget);
	GtkWidget *child = gtk_widget_get_first_child(widget);
	while (child) {
		GtkWidget *_child = gtk_widget_get_next_sibling(child);
		gtk_widget_unparent(child);
		child = _child;
	}
	WrapperWidget *wrapper = WRAPPER_WIDGET(wrapper_widget);
	if (wrapper->jvm) {
		JNIEnv *env;
		(*wrapper->jvm)->GetEnv(wrapper->jvm, (void**)&env, JNI_VERSION_1_6);
		if (wrapper->jobj)
			_WEAK_UNREF(wrapper->jobj);
		if (wrapper->canvas)
			_UNREF(wrapper->canvas);
	}
	G_OBJECT_CLASS (wrapper_widget_parent_class)->dispose (wrapper_widget);
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
	if (orientation == GTK_ORIENTATION_HORIZONTAL && (wrapper->layout_width > 0)) {
		*minimum = *natural = wrapper->layout_width;
	} else if (orientation == GTK_ORIENTATION_VERTICAL && (wrapper->layout_height > 0)) {
		*minimum = *natural = wrapper->layout_height;
	}
}

void wrapper_widget_allocate(GtkWidget *widget, int width, int height, int baseline)
{
	WrapperWidget *wrapper = WRAPPER_WIDGET(widget);
	if (!width && !height) {
		width = wrapper->real_width;
		height = wrapper->real_height;
	}
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

	if (ATL_IS_ANDROID_LAYOUT(gtk_widget_get_layout_manager(wrapper->child))) {
		AndroidLayout *layout = ATL_ANDROID_LAYOUT(gtk_widget_get_layout_manager(wrapper->child));
		if (layout->real_width != width || layout->real_height != height) {
			layout->real_width = width;
			layout->real_height = height;
			if (!layout->needs_allocation)
				gtk_widget_queue_allocate(wrapper->child);
		}
		if (layout->needs_allocation)
			gtk_widget_size_allocate(wrapper->child, &allocation, baseline);
		else
			gtk_widget_size_allocate(wrapper->child, &(GtkAllocation){.x = allocation.x, .y = allocation.y}, baseline);
	} else {
		gtk_widget_size_allocate(wrapper->child, &allocation, baseline);
	}
	if (wrapper->background)
		gtk_widget_size_allocate(wrapper->background, &allocation, baseline);
}

static void wrapper_widget_snapshot(GtkWidget *widget, GdkSnapshot *snapshot)
{
	WrapperWidget *wrapper = WRAPPER_WIDGET(widget);
	if (wrapper->real_height > 0 && wrapper->real_width > 0) {
		gtk_snapshot_push_clip(snapshot, &GRAPHENE_RECT_INIT(0, 0, wrapper->real_width, wrapper->real_height));
	}
	if (wrapper->draw_method) {
		JNIEnv *env = get_jni_env();
		_SET_LONG_FIELD(wrapper->canvas, "snapshot", _INTPTR(snapshot));
		(*env)->CallVoidMethod(env, wrapper->jobj, wrapper->draw_method, wrapper->canvas);
		if ((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);
	} else {
		GtkWidget *widget = &wrapper->parent_instance;
		GtkWidget *child = gtk_widget_get_first_child(widget);
		while (child) {
			gtk_widget_snapshot_child(widget, child, snapshot);
			child = gtk_widget_get_next_sibling(child);
		}
	}
	if (wrapper->real_height > 0 && wrapper->real_width > 0) {
		gtk_snapshot_pop(snapshot);
	}
}

static void wrapper_widget_class_init(WrapperWidgetClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS(class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

	object_class->dispose = wrapper_widget_dispose;

	widget_class->get_request_mode = wrapper_widget_get_request_mode;
	widget_class->measure = wrapper_widget_measure;
	widget_class->size_allocate = wrapper_widget_allocate;
	widget_class->snapshot = wrapper_widget_snapshot;
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

static guint queue_queue_redraw(GtkWidget *widget)
{
	gtk_widget_queue_draw(widget);
	return G_SOURCE_REMOVE;
}

void wrapper_widget_queue_draw(WrapperWidget *wrapper)
{
	if (wrapper->draw_method) {
		/* schedule the call to gtk_widget_queue_draw for a future event loop pass in case we're currently inside the snapshot */
		/* GTK+ uses G_PRIORITY_HIGH_IDLE + 10 for resizing operations, and G_PRIORITY_HIGH_IDLE + 20 for redrawing operations. */
		g_idle_add_full(G_PRIORITY_HIGH_IDLE + 20, G_SOURCE_FUNC(queue_queue_redraw), &wrapper->parent_instance, NULL);
	}

	if(wrapper->child)
		gtk_widget_queue_draw(wrapper->child);
	if (wrapper->computeScroll_method)
		gtk_widget_queue_allocate(GTK_WIDGET(wrapper));
}

static bool on_click(GtkGestureClick *gesture, int n_press, double x, double y, jobject this)
{
	JNIEnv *env = get_jni_env();

	bool ret = (*env)->CallBooleanMethod(env, this, handle_cache.view.performClick);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	return ret;
}

void wrapper_widget_set_jobject(WrapperWidget *wrapper, JNIEnv *env, jobject jobj)
{
	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);
	wrapper->jvm = jvm;
	wrapper->jobj = _WEAK_REF(jobj);
	jmethodID on_draw_method = _METHOD(_CLASS(jobj), "onDraw", "(Landroid/graphics/Canvas;)V");
	jmethodID draw_method = _METHOD(_CLASS(jobj), "draw", "(Landroid/graphics/Canvas;)V");
	if (on_draw_method != handle_cache.view.onDraw || draw_method != handle_cache.view.draw) {
		wrapper->draw_method = draw_method;
		jclass canvas_class = (*env)->FindClass(env, "android/graphics/GskCanvas");
		jmethodID canvas_constructor = _METHOD(canvas_class, "<init>", "(J)V");
		wrapper->canvas = _REF((*env)->NewObject(env, canvas_class, canvas_constructor, 0));
		(*env)->DeleteLocalRef(env, canvas_class);
	}

	jmethodID ontouchevent_method = _METHOD(_CLASS(jobj), "onTouchEvent", "(Landroid/view/MotionEvent;)Z");
	if (ontouchevent_method != handle_cache.view.onTouchEvent) {
		_setOnTouchListener(env, jobj, GTK_WIDGET(wrapper));
	}

	jmethodID computeScroll_method = _METHOD(_CLASS(jobj), "computeScroll", "()V");
	if (computeScroll_method != handle_cache.view.computeScroll) {
		wrapper->computeScroll_method = computeScroll_method;
	}

	jmethodID performClick_method = _METHOD(_CLASS(jobj), "performClick", "()Z");
	if (performClick_method != handle_cache.view.performClick) {
		GtkEventController *controller = GTK_EVENT_CONTROLLER(gtk_gesture_click_new());

		g_signal_connect(controller, "released", G_CALLBACK(on_click), wrapper->jobj);
		gtk_widget_add_controller(wrapper->child, controller);
		widget_set_needs_allocation(wrapper->child);
	}
}

void wrapper_widget_set_layout_params(WrapperWidget *wrapper, int width, int height)
{
	wrapper->layout_width = width;
	wrapper->layout_height = height;
}

void wrapper_widget_set_background(WrapperWidget *wrapper, GdkPaintable *paintable)
{
	if (!wrapper->background) {
		wrapper->background = gtk_picture_new();
		gtk_widget_insert_after(wrapper->background, GTK_WIDGET(wrapper), NULL);
	}
	gtk_picture_set_paintable(GTK_PICTURE(wrapper->background), paintable);
}

static gboolean on_touch_event_consume(GtkEventControllerLegacy *controller, GdkEvent *event) {
	switch(gdk_event_get_event_type(event)) {
		case GDK_BUTTON_PRESS:
		case GDK_TOUCH_BEGIN:
		case GDK_BUTTON_RELEASE:
		case GDK_TOUCH_END:
		case GDK_MOTION_NOTIFY:
		case GDK_TOUCH_UPDATE:
			return TRUE;
		default:
			return FALSE;
	}
}

// Add default on touch listener, which just consumes all events to prevent bubbling to the parent
void wrapper_widget_consume_touch_events(WrapperWidget *wrapper)
{
	GtkEventController *controller = GTK_EVENT_CONTROLLER(gtk_event_controller_legacy_new());
	g_signal_connect(controller, "event", G_CALLBACK(on_touch_event_consume), NULL);
	gtk_widget_add_controller(GTK_WIDGET(wrapper), controller);
	g_object_set_data(G_OBJECT(wrapper), "on_touch_listener", controller);

}
