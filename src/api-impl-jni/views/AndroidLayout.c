#include <gtk/gtk.h>

#include "../util.h"
#include "../defines.h"
#include "AndroidLayout.h"

static int make_measure_spec(int layout_size, int for_size)
{
	if (layout_size >= 0)
		return layout_size | MEASURE_SPEC_EXACTLY;
	else if (for_size >= 0 && layout_size == MATCH_PARENT)
		return for_size | MEASURE_SPEC_EXACTLY;
	else if (for_size >= 0 && layout_size == WRAP_CONTENT)
		return for_size | MEASURE_SPEC_AT_MOST;
	else if (layout_size == WRAP_CONTENT)
		return MEASURE_SPEC_UNSPECIFIED;
	else
		return -1;
}

static void android_layout_measure(GtkLayoutManager *layout_manager, GtkWidget *widget, GtkOrientation orientation, int for_size, int *minimum, int *natural, int *minimum_baseline, int *natural_baseline)
{
	AndroidLayout *layout = ATL_ANDROID_LAYOUT(layout_manager);
	JNIEnv *env = get_jni_env();

	if (layout->width || layout->height) {
		int widthMeasureSpec = make_measure_spec(layout->width, orientation == GTK_ORIENTATION_VERTICAL ? for_size : -1);
		int heightMeasureSpec = make_measure_spec(layout->height, orientation == GTK_ORIENTATION_HORIZONTAL ? for_size : -1);

		// if layout params say match_parent, but GTK doesnt specify the dimension, fallback to old specification if available
		if (widthMeasureSpec == -1)
			widthMeasureSpec = _GET_INT_FIELD(layout->view, "oldWidthMeasureSpec");
		if (heightMeasureSpec == -1)
			heightMeasureSpec = _GET_INT_FIELD(layout->view, "oldHeightMeasureSpec");
		if (widthMeasureSpec != -1 && heightMeasureSpec != -1) {
			(*env)->CallVoidMethod(env, layout->view, handle_cache.view.measure, widthMeasureSpec, heightMeasureSpec);
			if((*env)->ExceptionCheck(env)) {
				(*env)->ExceptionDescribe(env);
				(*env)->ExceptionClear(env);
			}
		}
	}

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

static void android_layout_allocate(GtkLayoutManager *layout_manager, GtkWidget *widget, int width, int height, int baseline)
{
	AndroidLayout *layout = ATL_ANDROID_LAYOUT(layout_manager);
	JNIEnv *env = get_jni_env();

	(*env)->CallVoidMethod(env, layout->view, handle_cache.view.layoutInternal, width, height);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

static GtkSizeRequestMode android_layout_get_request_mode(GtkLayoutManager *layout_manager, GtkWidget *widget)
{
	AndroidLayout *layout = ATL_ANDROID_LAYOUT(layout_manager);

	if (layout->height == WRAP_CONTENT) {
		return GTK_SIZE_REQUEST_HEIGHT_FOR_WIDTH;
	} else if (layout->width == WRAP_CONTENT) {
		return GTK_SIZE_REQUEST_WIDTH_FOR_HEIGHT;
	} else {
		return GTK_SIZE_REQUEST_CONSTANT_SIZE;
	}
}

static void android_layout_class_init(AndroidLayoutClass *klass)
{
	klass->parent_class.measure = android_layout_measure;
	klass->parent_class.allocate = android_layout_allocate;
	klass->parent_class.get_request_mode = android_layout_get_request_mode;
}

static void android_layout_init(AndroidLayout *self) {}

G_DEFINE_TYPE(AndroidLayout, android_layout, GTK_TYPE_LAYOUT_MANAGER)

GtkLayoutManager *android_layout_new(jobject view)
{
	AndroidLayout *layout = g_object_new(android_layout_get_type(), NULL);
	layout->view = view;
	return &layout->parent_instance;
}

void android_layout_set_params(AndroidLayout *layout, int width, int height)
{
	layout->width = width;
	layout->height = height;
}