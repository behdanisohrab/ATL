#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "../widgets/WrapperWidget.h"

#include "../generated_headers/android_view_ViewGroup.h"
#include "../generated_headers/android_view_View.h"

#define SOURCE_CLASS_POINTER 0x2

struct _AndroidLayout {
	GtkLayoutManager parent_instance;
	jobject view;
};
G_DECLARE_FINAL_TYPE(AndroidLayout, android_layout, ATL, ANDROID_LAYOUT, GtkLayoutManager);

static void android_layout_measure(GtkLayoutManager *layout_manager, GtkWidget *widget, GtkOrientation orientation, int for_size, int *minimum, int *natural, int *minimum_baseline, int *natural_baseline) {
	AndroidLayout *layout = ATL_ANDROID_LAYOUT(layout_manager);
	JNIEnv *env = get_jni_env();

	if (orientation == GTK_ORIENTATION_HORIZONTAL) {
		*minimum = (*env)->CallIntMethod(env, layout->view, handle_cache.view.getSuggestedMinimumWidth);
		*natural = (*env)->CallIntMethod(env, layout->view, handle_cache.view.getMeasuredWidth);
	}
	if (orientation == GTK_ORIENTATION_VERTICAL) {
		*minimum = (*env)->CallIntMethod(env, layout->view, handle_cache.view.getSuggestedMinimumHeight);
		*natural = (*env)->CallIntMethod(env, layout->view, handle_cache.view.getMeasuredHeight);
	}
	if (*natural < *minimum)
		*natural = *minimum;

	*minimum_baseline = -1;
	*natural_baseline = -1;
}

static void android_layout_allocate(GtkLayoutManager *layout_manager, GtkWidget *widget, int width, int height, int baseline) {
	AndroidLayout *layout = ATL_ANDROID_LAYOUT(layout_manager);
	JNIEnv *env = get_jni_env();

	(*env)->CallVoidMethod(env, layout->view, handle_cache.view.layoutInternal, width, height);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

static void android_layout_class_init(AndroidLayoutClass *klass) {
	klass->parent_class.measure = android_layout_measure;
	klass->parent_class.allocate = android_layout_allocate;
}

static void android_layout_init(AndroidLayout *self) {
}

G_DEFINE_TYPE(AndroidLayout, android_layout, GTK_TYPE_LAYOUT_MANAGER)

static GtkLayoutManager *android_layout_new(jobject view) {
	AndroidLayout *layout = g_object_new(android_layout_get_type(), NULL);
	layout->view = view;
	return &layout->parent_instance;
}

#define MAGIC_SCROLL_FACTOR 32

static gboolean scroll_cb(GtkEventControllerScroll* self, gdouble dx, gdouble dy, jobject this)
{
	JNIEnv *env = get_jni_env();
	GdkScrollUnit scroll_unit = gtk_event_controller_scroll_get_unit (self);

	if (scroll_unit == GDK_SCROLL_UNIT_SURFACE) {
		dx /= MAGIC_SCROLL_FACTOR;
		dy /= MAGIC_SCROLL_FACTOR;
	}
	jobject motion_event = (*env)->NewObject(env, handle_cache.motion_event.class, handle_cache.motion_event.constructor, SOURCE_CLASS_POINTER, MOTION_EVENT_ACTION_SCROLL, dx, -dy);

	gboolean ret = (*env)->CallBooleanMethod(env, this, handle_cache.view.onGenericMotionEvent, motion_event);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	return ret;
}

/**
 * Should be overwritten by ViewGroup subclasses.
 * Fall back to vertical GtkBox if subclass is not implemented yet
 */
JNIEXPORT jlong JNICALL Java_android_view_ViewGroup_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);

	const char *name = _CSTRING((*env)->CallObjectMethod(env, _CLASS(this),
			_METHOD((*env)->FindClass(env, "java/lang/Class"), "getName", "()Ljava/lang/String;")));
	gtk_widget_set_name(box, name);

	jmethodID measure_method = _METHOD(_CLASS(this), "onMeasure", "(II)V");
	jmethodID layout_method = _METHOD(_CLASS(this), "onLayout", "(ZIIII)V");
	if (measure_method != handle_cache.view.onMeasure || layout_method != handle_cache.view.onLayout) {
		gtk_widget_set_layout_manager(box, android_layout_new(_REF(this)));
	}
	if (_METHOD(_CLASS(this), "onGenericMotionEvent", "(Landroid/view/MotionEvent;)Z") != handle_cache.view.onGenericMotionEvent) {
		GtkEventController *controller = gtk_event_controller_scroll_new(GTK_EVENT_CONTROLLER_SCROLL_VERTICAL);

		g_signal_connect(controller, "scroll", G_CALLBACK(scroll_cb), _REF(this));
		gtk_widget_add_controller(box, controller);
	}

	return _INTPTR(box);
}

JNIEXPORT void JNICALL Java_android_view_ViewGroup_native_1addView(JNIEnv *env, jobject this, jlong widget, jlong child, jint index, jobject layout_params)
{
	if(layout_params) {
		/*
		GtkWidget *_child = gtk_widget_get_parent(GTK_WIDGET(_PTR(child)));
		jint child_width = -1;
		jint child_height = -1;

		jint child_width = _GET_INT_FIELD(layout_params, "width");
		jint child_height = _GET_INT_FIELD(layout_params, "height");

		jint child_gravity = _GET_INT_FIELD(layout_params, "gravity");

		if(child_width > 0)
			g_object_set(G_OBJECT(_child), "width-request", child_width, NULL);
		if(child_height > 0)
			g_object_set(G_OBJECT(_child), "height-request", child_height, NULL);

		if(child_gravity != -1) {
			printf(":::-: setting child gravity: %d", child_gravity);
			Java_android_view_View_setGravity(env, child, child_gravity);
		}*/
	}
	GtkWidget *parent = _PTR(widget);
	GtkWidget *iter = gtk_widget_get_first_child(parent);
	for(int i = 0; i < index; i++) {
		iter = gtk_widget_get_next_sibling(iter);
		if(iter == NULL)
			break;
	}

	gtk_widget_insert_before(gtk_widget_get_parent(GTK_WIDGET(_PTR(child))), parent, iter);
}

JNIEXPORT void JNICALL Java_android_view_ViewGroup_native_1removeView(JNIEnv *env, jobject this, jlong widget, jlong child)
{
	gtk_widget_unparent(gtk_widget_get_parent(GTK_WIDGET(_PTR(child))));
}
