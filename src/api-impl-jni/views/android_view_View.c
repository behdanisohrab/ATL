#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "../widgets/WrapperWidget.h"

#include "../generated_headers/android_view_View.h"

#define SOURCE_TOUCHSCREEN 4098

struct touch_callback_data { JavaVM *jvm; jobject this; jobject on_touch_listener; jclass on_touch_listener_class; };

static void call_ontouch_callback(int action, float x, float y, struct touch_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	jobject motion_event = (*env)->NewObject(env, handle_cache.motion_event.class, handle_cache.motion_event.constructor, SOURCE_TOUCHSCREEN, action, x, y);

	(*env)->CallBooleanMethod(env, d->on_touch_listener, _METHOD(d->on_touch_listener_class, "onTouch", "(Landroid/view/View;Landroid/view/MotionEvent;)Z"), d->this, motion_event);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	(*env)->DeleteLocalRef(env, motion_event);
}

static void on_press(GtkGestureClick *gesture, int n_press, double x, double y, struct touch_callback_data *d)
{
	call_ontouch_callback(MOTION_EVENT_ACTION_DOWN, (float)x, (float)y, d);
}

static void on_release(GtkGestureClick *gesture, int n_press, double x, double y, struct touch_callback_data *d)
{
	call_ontouch_callback(MOTION_EVENT_ACTION_UP, (float)x, (float)y, d);
}

static void on_click(GtkGestureClick *gesture, int n_press, double x, double y, struct touch_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	(*env)->CallBooleanMethod(env, d->on_touch_listener, _METHOD(d->on_touch_listener_class, "onClick", "(Landroid/view/View;)V"), d->this);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

JNIEXPORT void JNICALL Java_android_view_View_setOnTouchListener(JNIEnv *env, jobject this, jobject on_touch_listener)
{
	GtkWidget *widget = GTK_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget")));

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct touch_callback_data *callback_data = malloc(sizeof(struct touch_callback_data));
	callback_data->jvm = jvm;
	callback_data->this = _REF(this);
 	callback_data->on_touch_listener = _REF(on_touch_listener);
	callback_data->on_touch_listener_class = _REF(_CLASS(callback_data->on_touch_listener));

	GtkEventController *controller = GTK_EVENT_CONTROLLER(gtk_gesture_click_new());

	g_signal_connect(controller, "pressed", G_CALLBACK(on_press), callback_data);
	g_signal_connect(controller, "released", G_CALLBACK(on_release), callback_data);
	gtk_widget_add_controller(widget, controller);
}

JNIEXPORT void JNICALL Java_android_view_View_setOnClickListener(JNIEnv *env, jobject this, jobject on_click_listener)
{
	GtkWidget *widget = GTK_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget")));
	if (!on_click_listener)
		return;

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct touch_callback_data *callback_data = malloc(sizeof(struct touch_callback_data));
	callback_data->jvm = jvm;
	callback_data->this = _REF(this);
 	callback_data->on_touch_listener = _REF(on_click_listener);
	callback_data->on_touch_listener_class = _REF(_CLASS(callback_data->on_touch_listener));

	GtkEventController *controller = GTK_EVENT_CONTROLLER(gtk_gesture_click_new());

	g_signal_connect(controller, "released", G_CALLBACK(on_click), callback_data); // the release completes the click, I guess?
	gtk_widget_add_controller(widget, controller);
}

JNIEXPORT jint JNICALL Java_android_view_View_getWidth(JNIEnv *env, jobject this)
{
	GtkWidget *widget = GTK_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget")));

/* FIXME: is this needed in Gtk4?
	GtkAllocation alloc;
	gtk_widget_get_allocation(widget, &alloc);
	printf("widget size is currently %dx%d\n", alloc.width, alloc.height);
*/
	return gtk_widget_get_width(widget);
}

JNIEXPORT jint JNICALL Java_android_view_View_getHeight(JNIEnv *env, jobject this)
{
	GtkWidget *widget = GTK_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget")));

	return gtk_widget_get_height(widget);
}

#define GRAVITY_TOP (1<<5)//0x30
#define GRAVITY_BOTTOM (1<<6)//0x50
#define GRAVITY_LEFT (1<<1)//0x3
#define GRAVITY_RIGHT (1<<2)//0x5

#define GRAVITY_CENTER_VERTICAL 0x10
#define GRAVITY_CENTER_HORIZONTAL 0x01

#define GRAVITY_CENTER (GRAVITY_CENTER_VERTICAL | GRAVITY_CENTER_HORIZONTAL)

#define MATCH_PARENT (-1)

JNIEXPORT void JNICALL Java_android_view_View_native_1setLayoutParams(JNIEnv *env, jobject this, jlong widget_ptr, jint width, jint height, jint gravity, jfloat weight)
{
	GtkWidget *widget = gtk_widget_get_parent(GTK_WIDGET(_PTR(widget_ptr)));

	GtkAlign halign = GTK_ALIGN_FILL;
	GtkAlign valign = GTK_ALIGN_FILL;
	gboolean hexpand = FALSE;
	gboolean vexpand = FALSE;

	if (gravity != -1) {
		printf(":::-: setting gravity: %d\n", gravity);

		if(gravity & GRAVITY_BOTTOM)
			valign = GTK_ALIGN_END;
		else if(gravity & GRAVITY_TOP)
			valign = GTK_ALIGN_START;
		else
			valign = GTK_ALIGN_FILL;

		if(gravity & GRAVITY_RIGHT)
			halign = GTK_ALIGN_END;
		else if(gravity & GRAVITY_LEFT)
			halign = GTK_ALIGN_START;
		else
			halign = GTK_ALIGN_FILL;

		if(gravity == GRAVITY_CENTER) {
			valign = GTK_ALIGN_CENTER; // GTK_ALIGN_CENTER doesn't seem to be the right one?
			halign = GTK_ALIGN_CENTER; // ditto (GTK_ALIGN_CENTER)
			hexpand = TRUE; // haxx or not?
			vexpand = TRUE; // seems to be the deciding factor for whether to expand, guess I should try on android
		}
	}

	if (weight > 0.f) {
		printf(":::-: setting weight: %f\n", weight);

		hexpand = TRUE;
		vexpand = TRUE;
	}

	if (width == MATCH_PARENT) {
		hexpand = true;
		halign = GTK_ALIGN_FILL;
	}
	if (height == MATCH_PARENT) {
		vexpand = true;
		valign = GTK_ALIGN_FILL;
	}

	gtk_widget_set_hexpand(widget, hexpand);
	gtk_widget_set_vexpand(widget, vexpand);
	gtk_widget_set_halign(widget, halign);
	gtk_widget_set_valign(widget, valign);

	if(width > 0)
		g_object_set(G_OBJECT(widget), "width-request", width, NULL);
	if(height > 0)
		g_object_set(G_OBJECT(widget), "height-request", height, NULL);
}

JNIEXPORT void JNICALL Java_android_view_View_setVisibility(JNIEnv *env, jobject this, jint visibility) {
	GtkWidget *widget = gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget"))));

	switch (visibility) {
		case android_view_View_VISIBLE:
			gtk_widget_set_visible(widget, true);
			break;
		// TODO: View.INVISIBLE should still reserve layout space for the hidden view
		case android_view_View_INVISIBLE:
		case android_view_View_GONE:
			gtk_widget_set_visible(widget, false);
			break;
	}
}

// FIXME: this is used in one other place as well, should probably go in util.c or gtk_util.c?
gboolean tick_callback(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer user_data)
{
	gtk_widget_queue_draw(widget);
	gtk_widget_queue_draw(gtk_widget_get_parent(widget));
	return G_SOURCE_CONTINUE;
}

JNIEXPORT jlong JNICALL Java_android_view_View_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *area = gtk_drawing_area_new();
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), area);
	wrapper_widget_set_jobject(WRAPPER_WIDGET(wrapper), env, this);

	gtk_widget_add_tick_callback(area, tick_callback, NULL, NULL);

	return _INTPTR(area);
}

JNIEXPORT void JNICALL Java_android_view_View_nativeInvalidate(JNIEnv *env, jclass, jlong widget_ptr) {
	GtkWidget *widget = GTK_WIDGET(_PTR(widget_ptr));

	gtk_widget_queue_draw(gtk_widget_get_parent(widget));
}

JNIEXPORT void JNICALL Java_android_view_View_native_1destructor(JNIEnv *env, jobject this, jlong widget_ptr)
{
	g_object_unref(gtk_widget_get_parent(_PTR(widget_ptr)));
}

#define MEASURE_SPEC_UNSPECIFIED (0 << 30)
#define MEASURE_SPEC_EXACTLY (1 << 30)
#define MEASURE_SPEC_MASK (0x3 << 30)

JNIEXPORT void JNICALL Java_android_view_View_native_1measure(JNIEnv *env, jobject this, jlong widget_ptr, jint width_spec, jint height_spec) {
	int width;
	int height;
	int for_size;
	GtkWidget *widget = gtk_widget_get_parent(GTK_WIDGET(_PTR(widget_ptr)));

	if ((width_spec & MEASURE_SPEC_MASK) == MEASURE_SPEC_EXACTLY) {
		width = width_spec & ~MEASURE_SPEC_MASK;
	} else {
		for_size = ((height_spec & MEASURE_SPEC_MASK) == MEASURE_SPEC_EXACTLY) ? (height_spec & ~MEASURE_SPEC_MASK) : -1;
		gtk_widget_measure(widget, GTK_ORIENTATION_HORIZONTAL, for_size, NULL, &width, NULL, NULL);
	}

	if ((height_spec & MEASURE_SPEC_MASK) == MEASURE_SPEC_EXACTLY) {
		height = height_spec & ~MEASURE_SPEC_MASK;
	} else {
		for_size = ((width_spec & MEASURE_SPEC_MASK) == MEASURE_SPEC_EXACTLY) ? (width_spec & ~MEASURE_SPEC_MASK) : -1;
		gtk_widget_measure(widget, GTK_ORIENTATION_VERTICAL, for_size, NULL, &height, NULL, NULL);
	}

	(*env)->CallVoidMethod(env, this, handle_cache.view.setMeasuredDimension, width, height);
}

JNIEXPORT void JNICALL Java_android_view_View_native_1layout(JNIEnv *env, jobject this, jlong widget_ptr, jint l, jint t, jint r, jint b) {
	GtkWidget *widget = gtk_widget_get_parent(GTK_WIDGET(_PTR(widget_ptr)));

	GtkAllocation allocation = {
		.x=l,
		.y=t,
		.width=r-l,
		.height=b-t,
	};
	gtk_widget_size_allocate(widget, &allocation, -1);
}

JNIEXPORT void JNICALL Java_android_view_View_native_1requestLayout(JNIEnv *env, jobject this, jlong widget_ptr) {
	GtkWidget *widget = GTK_WIDGET(_PTR(widget_ptr));

	gtk_widget_queue_resize(widget);
}
