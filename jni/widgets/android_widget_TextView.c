#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "android_widget_TextView.h"

JNIEXPORT void JNICALL Java_android_widget_TextView_native_1constructor(JNIEnv *env, jobject this, jobject attrs)
{
	const char *text = attribute_set_get_string(env, attrs, "text", NULL);

//	_SET_OBJ_FIELD(this, "text", "Ljava/lang/String;", _JSTRING(text)); //TODO: sadly this might be needed, but it's not atm

	GtkWidget *label = gtk_label_new(text);
	_SET_LONG_FIELD(this, "widget", (long)label);
	g_object_ref(label);
}

JNIEXPORT void JNICALL Java_android_widget_TextView_setText(JNIEnv *env, jobject this, jobject charseq)
{
//		_SET_OBJ_FIELD(this, "text", "Ljava/lang/String;", charseq); //TODO: sadly this might be needed, but it's not atm

		gtk_label_set_text(GTK_LABEL(_PTR(_GET_LONG_FIELD(this, "widget"))), _CSTRING(charseq));
}
