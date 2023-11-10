#ifndef WRAPPER_WIDGET_H
#define WRAPPER_WIDGET_H

#include <jni.h>
#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE (WrapperWidget, wrapper_widget, WRAPPER, WIDGET, GtkWidget)

struct _WrapperWidget
{
	GtkWidget parent_instance;
	GtkWidget *child;
	GtkWidget *sk_area;
	JavaVM *jvm;
	jobject jobj;
	jobject canvas;
	jmethodID draw_method;
	jmethodID measure_method;
	jmethodID computeScroll_method;
	int layout_width;
	int layout_height;
};

struct _WrapperWidgetClass
{
	GtkWidgetClass parent_class;
};

GtkWidget * wrapper_widget_new(void);
void wrapper_widget_set_child(WrapperWidget *parent, GtkWidget *child);
void wrapper_widget_set_jobject(WrapperWidget *wrapper, JNIEnv *env, jobject jobj);
void wrapper_widget_queue_draw(WrapperWidget *wrapper);
void wrapper_widget_set_layout_params(WrapperWidget *wrapper, int width, int height);

void _setOnTouchListener(JNIEnv *env, jobject this, GtkWidget *widget, jobject on_touch_listener);

#endif
