#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_TextView.h"

JNIEXPORT jlong JNICALL Java_android_widget_TextView_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	const char *text = attribute_set_get_string(env, attrs, "text", NULL) ?: "FIXME";

//	_SET_OBJ_FIELD(this, "text", "Ljava/lang/String;", _JSTRING(text)); //TODO: sadly this might be needed, but it's not atm

	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *label = gtk_label_new(text);
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), label);
	return _INTPTR(label);
}

JNIEXPORT void JNICALL Java_android_widget_TextView_native_1setText(JNIEnv *env, jobject this, jobject charseq)
{
//	_SET_OBJ_FIELD(this, "text", "Ljava/lang/String;", charseq); //TODO: sadly this might be needed, but it's not atm

	gtk_label_set_text(GTK_LABEL(_PTR(_GET_LONG_FIELD(this, "widget"))), _CSTRING(charseq));
}

JNIEXPORT void JNICALL Java_android_widget_TextView_setTextSize(JNIEnv *env, jobject this, jfloat size)
{
	GtkLabel *label = GTK_LABEL(_PTR(_GET_LONG_FIELD(this, "widget")));
	PangoAttrList *attrs;

	PangoAttrList *old_attrs = gtk_label_get_attributes(label);
	if(old_attrs)
		attrs = pango_attr_list_copy(old_attrs);
	else
		attrs = pango_attr_list_new();

	PangoAttribute *size_attr = pango_attr_size_new(size * PANGO_SCALE);
	pango_attr_list_change(attrs, size_attr);
	gtk_label_set_attributes(label, attrs);

	pango_attr_list_unref(attrs);
}

JNIEXPORT void JNICALL Java_android_widget_TextView_native_1set_1markup(JNIEnv *env, jobject this, jint value)
{
	GtkLabel *label = GTK_LABEL(_PTR(_GET_LONG_FIELD(this, "widget")));

	printf("weeeheee!\n");

	gtk_label_set_use_markup(label, value);
	printf("gtk_label_get_use_markup: %d, >%s<\n", gtk_label_get_use_markup(label), gtk_label_get_text(label));
}
