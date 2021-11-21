#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "../widgets/WrapperWidget.h"

#define MOTION_EVENT_ACTION_DOWN 0
#define MOTION_EVENT_ACTION_UP 1

struct touch_callback_data { JavaVM *jvm; jobject this; jobject on_touch_listener; jclass on_touch_listener_class; };

static void call_ontouch_callback(int action, float x, float y, struct touch_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	jobject motion_event = (*env)->NewObject(env, handle_cache.motion_event.class, handle_cache.motion_event.constructor, action, x, y);

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

#define GRAVITY_TOP 0x30
#define GRAVITY_BOTTOM 0x50
#define GRAVITY_LEFT 0x3
#define GRAVITY_RIGHT 0x5

#define GRAVITY_CENTER_VERTICAL 0x10
#define GRAVITY_CENTER_HORIZONTAL 0x01

#define GRAVITY_CENTER (GRAVITY_CENTER_VERTICAL | GRAVITY_CENTER_HORIZONTAL)

JNIEXPORT void JNICALL Java_android_view_View_setGravity(JNIEnv *env, jobject this, jint gravity)
{
	GtkWidget *widget = gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget"))));

	switch (gravity) {
		case GRAVITY_TOP:
			gtk_widget_set_valign(widget, GTK_ALIGN_START);
			gtk_widget_set_halign(widget, GTK_ALIGN_FILL);
			break;
		case GRAVITY_BOTTOM:
			gtk_widget_set_valign(widget, GTK_ALIGN_END);
			gtk_widget_set_halign(widget, GTK_ALIGN_FILL);
			break;
		case GRAVITY_LEFT:
			gtk_widget_set_valign(widget, GTK_ALIGN_FILL);
			gtk_widget_set_halign(widget, GTK_ALIGN_START);
			break;
		case GRAVITY_RIGHT:
			gtk_widget_set_valign(widget, GTK_ALIGN_FILL);
			gtk_widget_set_halign(widget, GTK_ALIGN_END);
			break;
		case GRAVITY_CENTER:
			gtk_widget_set_valign(widget, GTK_ALIGN_FILL); // GTK_ALIGN_CENTER doesn't seem to be the right one?
			gtk_widget_set_halign(widget, GTK_ALIGN_FILL); // ditto (GTK_ALIGN_CENTER)
			gtk_widget_set_hexpand(widget, true); // haxx or not?
			gtk_widget_set_vexpand(widget, true); // seems to be the deciding factor expand, guess I should try on android
			break;
	}
}

JNIEXPORT void JNICALL Java_android_view_View_native_1set_1size_1request(JNIEnv *env, jobject this, jint width, jint height)
{
	gtk_widget_set_size_request(gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget")))), width, height);
}

// --- the stuff below only applies to widgets that override the OnDraw() method; other widgets are created by class-specific constructors.
// FIXME: how do we handle someone subclassing something other then View and then overriding the onDraw/onMeasure method(s)?

struct jni_callback_data { JavaVM *jvm; jobject this; jclass this_class; };

static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, struct jni_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	jobject canvas = (*env)->NewObject(env, handle_cache.canvas.class, handle_cache.canvas.constructor, (jlong)cr, (jlong)area);

	(*env)->CallVoidMethod(env, d->this, _METHOD(d->this_class, "onDraw", "(Landroid/graphics/Canvas;)V"), canvas);

	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);

	(*env)->DeleteLocalRef(env, canvas);
}

void on_mapped(GtkWidget* self, struct jni_callback_data *d)
{
	JNIEnv *env;
	(*d->jvm)->GetEnv(d->jvm, (void**)&env, JNI_VERSION_1_6);

	(*env)->CallVoidMethod(env, d->this, _METHOD(d->this_class, "onMeasure", "(II)V"), gtk_widget_get_width(self), gtk_widget_get_height(self));
}

gboolean tick_callback(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer user_data)
{
	gtk_widget_queue_draw(widget);
	return G_SOURCE_CONTINUE;
}

JNIEXPORT void JNICALL Java_android_view_View_native_1constructor(JNIEnv *env, jobject this, jobject Context)
{
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *area = gtk_drawing_area_new();
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), area);
	gtk_widget_set_hexpand(area, true); // this doesn't seem to be done by the app,
	gtk_widget_set_vexpand(area, true); // so presumably it's the default?

	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);

	struct jni_callback_data *callback_data = malloc(sizeof(struct jni_callback_data));
	callback_data->jvm = jvm;
	callback_data->this = _REF(this);
	callback_data->this_class = _REF(_CLASS(this));

	gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), ( void(*)(GtkDrawingArea*,cairo_t*,int,int,gpointer) )draw_function, callback_data, NULL);

	gtk_widget_add_tick_callback(area, tick_callback, NULL, NULL);

	// add a callback for when the widget is mapped, which will call onMeasure to figure out what size the widget wants to be
	g_signal_connect(area, "map", G_CALLBACK(on_mapped), callback_data);

	_SET_LONG_FIELD(this, "widget", (long)area);
	g_object_ref(wrapper);
}
