#include <stddef.h>
#include <stdint.h>

#include <gtk/gtk.h>

enum {
	AINPUT_EVENT_TYPE_KEY = 1,
	AINPUT_EVENT_TYPE_MOTION = 2,
	AINPUT_EVENT_TYPE_FOCUS = 3,
	AINPUT_EVENT_TYPE_CAPTURE = 4,
	AINPUT_EVENT_TYPE_DRAG = 5,
	AINPUT_EVENT_TYPE_TOUCH_MODE = 6
};

enum {
	AINPUT_SOURCE_CLASS_MASK = 0x000000ff,
	AINPUT_SOURCE_CLASS_NONE = 0x00000000,
	AINPUT_SOURCE_CLASS_BUTTON = 0x00000001,
	AINPUT_SOURCE_CLASS_POINTER = 0x00000002,
	AINPUT_SOURCE_CLASS_NAVIGATION = 0x00000004,
	AINPUT_SOURCE_CLASS_POSITION = 0x00000008,
	AINPUT_SOURCE_CLASS_JOYSTICK = 0x00000010
};

enum {
	AINPUT_SOURCE_UNKNOWN = 0x00000000,
	AINPUT_SOURCE_KEYBOARD = 0x00000100 | AINPUT_SOURCE_CLASS_BUTTON,
	AINPUT_SOURCE_DPAD = 0x00000200 | AINPUT_SOURCE_CLASS_BUTTON,
	AINPUT_SOURCE_GAMEPAD = 0x00000400 | AINPUT_SOURCE_CLASS_BUTTON,
	AINPUT_SOURCE_TOUCHSCREEN = 0x00001000 | AINPUT_SOURCE_CLASS_POINTER,
	AINPUT_SOURCE_MOUSE = 0x00002000 | AINPUT_SOURCE_CLASS_POINTER,
	AINPUT_SOURCE_STYLUS = 0x00004000 | AINPUT_SOURCE_CLASS_POINTER,
	AINPUT_SOURCE_BLUETOOTH_STYLUS = 0x00008000 | AINPUT_SOURCE_STYLUS,
	AINPUT_SOURCE_TRACKBALL = 0x00010000 | AINPUT_SOURCE_CLASS_NAVIGATION,
	AINPUT_SOURCE_MOUSE_RELATIVE = 0x00020000 | AINPUT_SOURCE_CLASS_NAVIGATION,
	AINPUT_SOURCE_TOUCHPAD = 0x00100000 | AINPUT_SOURCE_CLASS_POSITION,
	AINPUT_SOURCE_TOUCH_NAVIGATION = 0x00200000 | AINPUT_SOURCE_CLASS_NONE,
	AINPUT_SOURCE_JOYSTICK = 0x01000000 | AINPUT_SOURCE_CLASS_JOYSTICK,
	AINPUT_SOURCE_HDMI = 0x02000000 | AINPUT_SOURCE_CLASS_BUTTON,
	AINPUT_SOURCE_SENSOR = 0x04000000 | AINPUT_SOURCE_CLASS_NONE,
	AINPUT_SOURCE_ROTARY_ENCODER = 0x00400000 | AINPUT_SOURCE_CLASS_NONE,
	AINPUT_SOURCE_ANY = 0xffffff00
};

enum {
	AMOTION_EVENT_ACTION_MASK = 0xff,
	AMOTION_EVENT_ACTION_POINTER_INDEX_MASK = 0xff00,
	AMOTION_EVENT_ACTION_DOWN = 0,
	AMOTION_EVENT_ACTION_UP = 1,
	AMOTION_EVENT_ACTION_MOVE = 2,
	AMOTION_EVENT_ACTION_CANCEL = 3,
	AMOTION_EVENT_ACTION_OUTSIDE = 4,
	AMOTION_EVENT_ACTION_POINTER_DOWN = 5,
	AMOTION_EVENT_ACTION_POINTER_UP = 6,
	AMOTION_EVENT_ACTION_HOVER_MOVE = 7,
	AMOTION_EVENT_ACTION_SCROLL = 8,
	AMOTION_EVENT_ACTION_HOVER_ENTER = 9,
	AMOTION_EVENT_ACTION_HOVER_EXIT = 10,
	AMOTION_EVENT_ACTION_BUTTON_PRESS = 11,
	AMOTION_EVENT_ACTION_BUTTON_RELEASE = 12
};

struct AInputEvent {
	double x;
	double y;
	int32_t action;
};

typedef void AInputQueue;

struct ALooper;
typedef int (*Looper_callbackFunc)(int fd, int events, void* data);

float AMotionEvent_getAxisValue(const struct AInputEvent* motion_event, int32_t axis, size_t pointer_index)
{
	return -1; // no clue what to do here
}

size_t AMotionEvent_getPointerCount(const struct AInputEvent* motion_event)
{
	return 1; // FIXME
}

int32_t AInputEvent_getType(const struct AInputEvent* event)
{
	if(event) {
		return AINPUT_EVENT_TYPE_MOTION; // FIXME
	} else {
		return -1;
	}
}

int32_t AInputEvent_getSource(const struct AInputEvent* event)
{
	if(event) {
		return AINPUT_SOURCE_TOUCHSCREEN; // FIXME
	} else {
		return -1;
	}
}

int32_t AMotionEvent_getAction(const struct AInputEvent* motion_event)
{
	if(motion_event) {
		return motion_event->action;
	} else {
		return -1;
	}
}

int32_t AMotionEvent_getPointerId(const struct AInputEvent* motion_event, size_t pointer_index)
{
	if(motion_event) {
		return 1; // FIXME
	} else {
		return -1; // 0?
	}
}

float AMotionEvent_getX(const struct AInputEvent* motion_event, size_t pointer_index)
{
	if(motion_event) {
		return motion_event->x;
	} else {
		return -1;
	}
}

float AMotionEvent_getY(const struct AInputEvent* motion_event, size_t pointer_index)
{
	if(motion_event) {
		return motion_event->y;
	} else {
		return -1;
	}
}

void AInputQueue_detachLooper(AInputQueue* queue)
{
	return;
}

struct android_poll_source {
    // The identifier of this source.  May be LOOPER_ID_MAIN or
    // LOOPER_ID_INPUT.
    int32_t id;

    // The android_app this ident is associated with.
    struct android_app* app;

    // Function to call to perform the standard processing of data from
    // this source.
    void (*process)(struct android_app* app, struct android_poll_source* source);
};

// ugly; if this is < 0, there are no events
// we return this from AInputQueue_getEvent,
// and we don't want to have an actual queue
static int32_t fixme_ugly_are_there_events = -1;

struct AInputEvent fixme_ugly_current_event;

static gboolean on_event(GtkEventControllerLegacy* self, GdkEvent* event, struct android_poll_source *poll_source)
{
	double x;
	double y;

	// TODO: this doesn't work for multitouch
	switch(gdk_event_get_event_type(event)) {
		case GDK_BUTTON_PRESS:
			gdk_event_get_position(event, &x, &y);
			fixme_ugly_current_event.x = x;
			fixme_ugly_current_event.y = y;
			fixme_ugly_current_event.action = AMOTION_EVENT_ACTION_DOWN;
			fixme_ugly_are_there_events = 0; // not < 0, so there are events
			poll_source->process(poll_source->app, poll_source);
			if(fixme_ugly_are_there_events != -1)
				fprintf(stderr, "sus: poll_source->callback finished, but 'fixme_ugly_are_there_events' is not -1 (it's %d)\n"
						       "this should *probably* not happen?\n", fixme_ugly_are_there_events);
			break;
		case GDK_BUTTON_RELEASE:
			gdk_event_get_position(event, &x, &y);
			fixme_ugly_current_event.x = x;
			fixme_ugly_current_event.y = y;
			fixme_ugly_current_event.action = AMOTION_EVENT_ACTION_UP;
			fixme_ugly_are_there_events = 0; // not < 0, so there are events
			poll_source->process(poll_source->app, poll_source);
			if(fixme_ugly_are_there_events != -1)
				fprintf(stderr, "sus: poll_source->callback finished, but 'fixme_ugly_are_there_events' is not -1 (it's %d)\n"
						       "this should *probably* not happen?\n", fixme_ugly_are_there_events);
			break;
		case GDK_MOTION_NOTIFY:
			gdk_event_get_position(event, &x, &y);
			fixme_ugly_current_event.x = x;
			fixme_ugly_current_event.y = y;
			fixme_ugly_current_event.action = AMOTION_EVENT_ACTION_MOVE;
			fixme_ugly_are_there_events = 0; // not < 0, so there are events
			poll_source->process(poll_source->app, poll_source);
			if(fixme_ugly_are_there_events != -1)
				fprintf(stderr, "sus: poll_source->callback finished, but 'fixme_ugly_are_there_events' is not -1 (it's %d)\n"
						       "this should *probably* not happen?\n", fixme_ugly_are_there_events);
			break;
	}
}

void AInputQueue_attachLooper(AInputQueue* queue, struct ALooper* looper, int ident, Looper_callbackFunc callback, void* data)
{
	struct android_poll_source *poll_source = (struct android_poll_source *)data;
	printf("AInputQueue_attachLooper called: queue: %p, looper: %p, ident: %d, callback %p, data: %p, process_func: %p\n", queue, looper, ident, callback, poll_source, poll_source->process);

	GtkEventController *controller = (GtkEventController *)queue; // TODO: is there a saner thing to pass here?

	g_signal_connect(controller, "event", G_CALLBACK(on_event), (gpointer)poll_source);
}

int32_t AInputQueue_getEvent(AInputQueue* queue, struct AInputEvent** outEvent)
{
	if(fixme_ugly_are_there_events == 0) {
		*outEvent = &fixme_ugly_current_event;
		return fixme_ugly_are_there_events;
	} else {
		return -1; // no events or error
	}
}

int32_t AInputQueue_preDispatchEvent(AInputQueue* queue, struct AInputEvent* event)
{
	return 0; // we don't want to claim the event for ourselves, let the app process it
}

void AInputQueue_finishEvent(AInputQueue* queue, struct AInputEvent* event, int handled)
{
	fixme_ugly_are_there_events = -1;
}
