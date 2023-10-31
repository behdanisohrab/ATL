#include <gtk/gtk.h>

#include "../util.h"
#include "AndroidLayout.h"

static void android_layout_measure(GtkLayoutManager *layout_manager, GtkWidget *widget, GtkOrientation orientation, int for_size, int *minimum, int *natural, int *minimum_baseline, int *natural_baseline)
{
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

static void android_layout_allocate(GtkLayoutManager *layout_manager, GtkWidget *widget, int width, int height, int baseline)
{
	AndroidLayout *layout = ATL_ANDROID_LAYOUT(layout_manager);
	JNIEnv *env = get_jni_env();

	(*env)->CallVoidMethod(env, layout->view, handle_cache.view.layoutInternal, width, height);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

static void android_layout_class_init(AndroidLayoutClass *klass)
{
	klass->parent_class.measure = android_layout_measure;
	klass->parent_class.allocate = android_layout_allocate;
}

static void android_layout_init(AndroidLayout *self) {}

G_DEFINE_TYPE(AndroidLayout, android_layout, GTK_TYPE_LAYOUT_MANAGER)

GtkLayoutManager *android_layout_new(jobject view)
{
	AndroidLayout *layout = g_object_new(android_layout_get_type(), NULL);
	layout->view = view;
	return &layout->parent_instance;
}
