#include <stddef.h>
#include <stdint.h>

struct AInputEvent;
struct AInputQueue;

struct ALooper;
typedef void * ALooper_callbackFunc;

int32_t AInputEvent_getType(const struct AInputEvent* event)
{
	return -1;
}

float AMotionEvent_getX(const struct AInputEvent* motion_event, size_t pointer_index)
{
	return 0;
}

float AMotionEvent_getY(const struct AInputEvent* motion_event, size_t pointer_index)
{
	return 0;
}

void AInputQueue_detachLooper(struct AInputQueue* queue)
{
	return;
}

void AInputQueue_attachLooper(struct AInputQueue* queue, struct ALooper* looper, int ident, ALooper_callbackFunc callback, void* data)
{
	return;
}

int32_t AInputQueue_getEvent(struct AInputQueue* queue, struct AInputEvent** outEvent)
{
	return -1; // no events or error
}

int32_t AInputQueue_preDispatchEvent(struct AInputQueue* queue, struct AInputEvent* event)
{
	return -1; // not sure what is best suited for a stub
}

void AInputQueue_finishEvent(struct AInputQueue* queue, struct AInputEvent* event, int handled)
{
	return;
}
