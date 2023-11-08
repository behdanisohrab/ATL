#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "../widgets/WrapperWidget.h"
#include "../views/AndroidLayout.h"

#include "../generated_headers/android_view_View.h"

#define SOURCE_TOUCHSCREEN 0x1002

struct touch_callback_data { JavaVM *jvm; jobject this; jobject on_touch_listener; jclass on_touch_listener_class;};

static bool call_ontouch_callback(int action, double x, double y, struct touch_callback_data *d)
{
	bool ret;
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	jobject motion_event = (*env)->NewObject(env, handle_cache.motion_event.class, handle_cache.motion_event.constructor, SOURCE_TOUCHSCREEN, action, (float)x, (float)y);

	/* NULL listener means the callback was registered for onTouchEvent */
	if(d->on_touch_listener)
		ret = (*env)->CallBooleanMethod(env, d->on_touch_listener, _METHOD(d->on_touch_listener_class, "onTouch", "(Landroid/view/View;Landroid/view/MotionEvent;)Z"), d->this, motion_event);
	else
		ret = (*env)->CallBooleanMethod(env, d->this, _METHOD(d->on_touch_listener_class, "onTouchEvent", "(Landroid/view/MotionEvent;)Z"), motion_event);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	(*env)->DeleteLocalRef(env, motion_event);
	return ret;
}
static void gdk_event_get_widget_relative_position(GdkEvent *event, GtkWidget *widget, double *x, double *y)
{
	double off_x;
	double off_y;
	gdk_event_get_position(event, x, y);
	GtkWidget *window = GTK_WIDGET(gtk_widget_get_native(widget));
	gtk_native_get_surface_transform(GTK_NATIVE(window), &off_x, &off_y);
	gtk_widget_translate_coordinates(window, widget, *x - off_x, *y - off_y, x, y);
}

// TODO: find a way to reconcile this with libandroid/input.c?
static gboolean on_event(GtkEventControllerLegacy *event_controller, GdkEvent *event, struct touch_callback_data *d)
{
	double x;
	double y;

	GtkWidget *widget = gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(event_controller));

	// TODO: this doesn't work for multitouch
	switch(gdk_event_get_event_type(event)) {
		case GDK_BUTTON_PRESS:
		case GDK_TOUCH_BEGIN:
			gdk_event_get_widget_relative_position(event, widget, &x, &y);
			return call_ontouch_callback(MOTION_EVENT_ACTION_DOWN, x, y, d);
			break;
		case GDK_BUTTON_RELEASE:
		case GDK_TOUCH_END:
			gdk_event_get_widget_relative_position(event, widget, &x, &y);
			return call_ontouch_callback(MOTION_EVENT_ACTION_UP, x, y, d);
			break;
		case GDK_MOTION_NOTIFY:
			if (!(gdk_event_get_modifier_state(event) & GDK_BUTTON1_MASK))
				break;
		case GDK_TOUCH_UPDATE:
			gdk_event_get_widget_relative_position(event, widget, &x, &y);
			return call_ontouch_callback(MOTION_EVENT_ACTION_MOVE, x, y, d);
			break;
		default:
			break;
	}

	return false;
}

static void on_click(GtkGestureClick *gesture, int n_press, double x, double y, struct touch_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	(*env)->CallBooleanMethod(env, d->on_touch_listener, _METHOD(d->on_touch_listener_class, "onClick", "(Landroid/view/View;)V"), d->this);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

void _setOnTouchListener(JNIEnv *env, jobject this, GtkWidget *widget, jobject on_touch_listener)
{
	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct touch_callback_data *callback_data = malloc(sizeof(struct touch_callback_data));
	callback_data->jvm = jvm;
	callback_data->this = _REF(this);
 	callback_data->on_touch_listener = on_touch_listener ? _REF(on_touch_listener) : NULL;
	callback_data->on_touch_listener_class = on_touch_listener ? _REF(_CLASS(callback_data->on_touch_listener)) : _REF(_CLASS(callback_data->this));

	GtkEventController *old_controller = g_object_get_data(G_OBJECT(widget), "on_touch_listener");
	if(old_controller)
		gtk_widget_remove_controller(widget, old_controller);

	GtkEventController *controller = GTK_EVENT_CONTROLLER(gtk_event_controller_legacy_new());
	gtk_event_controller_set_propagation_phase(controller, GTK_PHASE_BUBBLE);

	g_signal_connect(controller, "event", G_CALLBACK(on_event), callback_data);
	gtk_widget_add_controller(widget, controller);
	g_object_set_data(G_OBJECT(widget), "on_touch_listener", controller);
}

JNIEXPORT void JNICALL Java_android_view_View_setOnTouchListener(JNIEnv *env, jobject this, jobject on_touch_listener)
{
	GtkWidget *widget = GTK_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget")));

	_setOnTouchListener(env, this, widget, on_touch_listener);
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

	GtkEventController *old_controller = g_object_get_data(G_OBJECT(widget), "on_click_listener");
	if(old_controller)
		gtk_widget_remove_controller(widget, old_controller);

	GtkEventController *controller = GTK_EVENT_CONTROLLER(gtk_gesture_click_new());

	g_signal_connect(controller, "released", G_CALLBACK(on_click), callback_data); // the release completes the click, I guess?
	gtk_widget_add_controller(widget, controller);
	g_object_set_data(G_OBJECT(widget), "on_click_listener", controller);
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

JNIEXPORT void JNICALL Java_android_view_View_native_1setLayoutParams(JNIEnv *env, jobject this, jlong widget_ptr, jint width, jint height, jint gravity, jfloat weight,
		jint leftMargin, jint topMargin, jint rightMargin, jint bottomMargin)
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

	GtkWidget *parent = gtk_widget_get_parent(widget);
	// if parent is Java widget, it will handle the margins by itself
	if (parent && !ATL_IS_ANDROID_LAYOUT(gtk_widget_get_layout_manager(parent))) {
		gtk_widget_set_margin_start(widget, leftMargin);
		gtk_widget_set_margin_top(widget, topMargin);
		gtk_widget_set_margin_end(widget, rightMargin);
		gtk_widget_set_margin_bottom(widget, bottomMargin);
	}

	GtkLayoutManager *layout_manager = gtk_widget_get_layout_manager(WRAPPER_WIDGET(widget)->child);
	if (ATL_IS_ANDROID_LAYOUT(layout_manager))
		android_layout_set_params(ATL_ANDROID_LAYOUT(layout_manager), width, height);
}

JNIEXPORT void JNICALL Java_android_view_View_native_1setVisibility(JNIEnv *env, jobject this, jlong widget_ptr, jint visibility) {
	GtkWidget *widget = gtk_widget_get_parent(GTK_WIDGET(_PTR(widget_ptr)));

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

JNIEXPORT jlong JNICALL Java_android_view_View_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	// FIXME: we don't really care all that much what this is, since drawing into empty widgets is done by WrapperWidget, but a drawing area is just confusing
	GtkWidget *widget = gtk_drawing_area_new();
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), widget);
	wrapper_widget_set_jobject(WRAPPER_WIDGET(wrapper), env, this);

	jclass class = _CLASS(this);
	jmethodID measure_method = _METHOD(class, "onMeasure", "(II)V");
	jmethodID layout_method = _METHOD(class, "onLayout", "(ZIIII)V");
	if (measure_method != handle_cache.view.onMeasure || layout_method != handle_cache.view.onLayout) {
		gtk_widget_set_layout_manager(widget, android_layout_new(_REF(this)));
		(*env)->SetBooleanField(env, this, _FIELD_ID(class, "haveCustomMeasure", "Z"), true);
	}

	return _INTPTR(widget);
}

JNIEXPORT void JNICALL Java_android_view_View_nativeInvalidate(JNIEnv *env, jclass class, jlong widget_ptr) {
	GtkWidget *widget = GTK_WIDGET(_PTR(widget_ptr));

	wrapper_widget_queue_draw(WRAPPER_WIDGET(gtk_widget_get_parent(widget)));
}

JNIEXPORT void JNICALL Java_android_view_View_native_1destructor(JNIEnv *env, jobject this, jlong widget_ptr)
{
	g_object_unref(gtk_widget_get_parent(_PTR(widget_ptr)));
}

#define MEASURE_SPEC_UNSPECIFIED (0 << 30)
#define MEASURE_SPEC_EXACTLY (1 << 30)
#define MEASURE_SPEC_AT_MOST (2 << 30)
#define MEASURE_SPEC_MASK (0x3 << 30)

JNIEXPORT void JNICALL Java_android_view_View_native_1measure(JNIEnv *env, jobject this, jlong widget_ptr, jint width_spec, jint height_spec, jboolean is_complex) {
	int width;
	int height;
	int for_size;
	GtkWidget *widget = gtk_widget_get_parent(GTK_WIDGET(_PTR(widget_ptr)));
	int width_spec_size = width_spec & ~MEASURE_SPEC_MASK;
	int height_spec_size = height_spec & ~MEASURE_SPEC_MASK;
	int width_spec_mode = width_spec & MEASURE_SPEC_MASK;
	int height_spec_mode = height_spec & MEASURE_SPEC_MASK;

	if (width_spec_mode == MEASURE_SPEC_EXACTLY || (!is_complex && width_spec_mode == MEASURE_SPEC_AT_MOST)) {
		width = width_spec_size;
	} else {
		for_size = (height_spec_mode == MEASURE_SPEC_EXACTLY) ? height_spec_size : -1;
		gtk_widget_measure(widget, GTK_ORIENTATION_HORIZONTAL, for_size, NULL, &width, NULL, NULL);
	}
	if (width_spec_mode == MEASURE_SPEC_AT_MOST && width > width_spec_size) {
		width = width_spec_size;
	}

	if (height_spec_mode == MEASURE_SPEC_EXACTLY || (!is_complex && height_spec_mode == MEASURE_SPEC_AT_MOST)) {
		height = height_spec_size;
	} else {
		for_size = (width_spec_mode == MEASURE_SPEC_EXACTLY) ? width_spec_size : -1;
		gtk_widget_measure(widget, GTK_ORIENTATION_VERTICAL, for_size, NULL, &height, NULL, NULL);
	}
	if (height_spec_mode == MEASURE_SPEC_AT_MOST && height > height_spec_size) {
		height = height_spec_size;
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

// FIXME: this will probably behave unfortunately if called multiple times
JNIEXPORT void JNICALL Java_android_view_View_setBackgroundColor(JNIEnv *env, jobject this, jint color)
{
	GtkWidget *widget = GTK_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget")));

	GtkCssProvider *css_provider = gtk_css_provider_new();

	char *css_string = g_markup_printf_escaped("* { background-color: #%06x%02x; }", color & 0xFFFFFF, (color >> 24) & 0xFF);
	gtk_css_provider_load_from_string(css_provider, css_string);
	g_free(css_string);

	gtk_style_context_add_provider(gtk_widget_get_style_context(gtk_widget_get_parent(widget)), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}
