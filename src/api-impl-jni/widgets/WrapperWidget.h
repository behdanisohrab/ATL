#ifndef WRAPPER_WIDGET_H
#define WRAPPER_WIDGET_H

#include <jni.h>
#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE (WrapperWidget, wrapper_widget, WRAPPER, WIDGET, GtkWidget)

struct _WrapperWidget
{
	GtkWidget parent_instance;
	JavaVM *jvm;
	jobject jobj;
	jobject canvas;
	jmethodID draw_method;
	jmethodID measure_method;
};

struct _WrapperWidgetClass
{
	GtkWidgetClass parent_class;
};

GtkWidget * wrapper_widget_new(void);
void wrapper_widget_set_child(WrapperWidget *parent, GtkWidget *child);
void wrapper_widget_set_jobject(WrapperWidget *wrapper, JNIEnv *env, jobject jobj);

void _setOnTouchListener(JNIEnv *env, jobject this, GtkWidget *widget, jobject on_touch_listener);

#endif
