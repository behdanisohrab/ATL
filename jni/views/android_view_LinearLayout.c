#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "android_view_LinearLayout.h"
#include "android_view_ViewGroup.h"

JNIEXPORT void JNICALL Java_android_view_LinearLayout_native_1constructor(JNIEnv *env, jobject this, jobject attrs)
{
	int orientation = attribute_set_get_int(env, attrs, "orientation", NULL, 0);

	GtkWidget *box = gtk_box_new(orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	_SET_LONG_FIELD(this, "widget", (long)box);
	g_object_ref(box);
}

JNIEXPORT void JNICALL Java_android_view_LinearLayout_addView(JNIEnv *env, jobject this, jobject child, jint index, jobject layout_params)
{
	Java_android_view_ViewGroup_addView(env, this, child, index, layout_params);

	gtk_box_append(GTK_BOX(_PTR(_GET_LONG_FIELD(this, "widget"))), GTK_WIDGET(_PTR(_GET_LONG_FIELD(child, "widget")))); // FIXME - ignores index argument
}
