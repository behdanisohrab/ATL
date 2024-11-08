#ifndef WRAPPER_WIDGET_H
#define WRAPPER_WIDGET_H

#include <jni.h>
#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE (WrapperWidget, wrapper_widget, WRAPPER, WIDGET, GtkWidget)

struct _WrapperWidget
{
	GtkWidget parent_instance;
	GtkWidget *child;
	GtkWidget *background;
	JavaVM *jvm;
	jobject jobj;
	jobject canvas;
	jmethodID draw_method;
	jmethodID measure_method;
	jmethodID computeScroll_method;
	int layout_width;
	int layout_height;
	int real_width;
	int real_height;
	gboolean needs_allocation;
	gboolean intercepting_touch;
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
void wrapper_widget_set_background(WrapperWidget *wrapper, GdkPaintable *paintable);
void wrapper_widget_consume_touch_events(WrapperWidget *wrapper);

void _setOnTouchListener(JNIEnv *env, jobject this, GtkWidget *widget);

#endif
