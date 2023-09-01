#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../drawables/ninepatch.h"

#include "../generated_headers/android_widget_LinearLayout.h"
#include "../generated_headers/android_view_ViewGroup.h"

static void on_click(GtkGestureClick *gesture, int n_press, double x, double y, jobject this)
{
	JNIEnv *env = get_jni_env();

	(*env)->CallBooleanMethod(env, this, handle_cache.view.performClick);
	if((*env)->ExceptionCheck(env))
		(*env)->ExceptionDescribe(env);
}

JNIEXPORT jlong JNICALL Java_android_widget_LinearLayout_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	int orientation = attribute_set_get_int(env, attrs, "orientation", NULL, 0);

	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *box = gtk_box_new(orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 1); // spacing of 1
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), box);
	gtk_widget_set_name(GTK_WIDGET(box), "LinearLayout");
	if (!attrs) {
		gtk_widget_set_hexpand_set(box, true); // FIXME: to counteract expand on drawing areas
		gtk_widget_set_vexpand_set(box, true); // XXX
	}
	if (_METHOD(_CLASS(this), "performClick", "()Z") != handle_cache.view.performClick) {
		GtkEventController *controller = GTK_EVENT_CONTROLLER(gtk_gesture_click_new());

		g_signal_connect(controller, "released", G_CALLBACK(on_click), _REF(this));
		gtk_widget_add_controller(box, controller);
	}
	return _INTPTR(box);

//	struct ninepatch_t *ninepatch = ninepatch_new("/home/Mis012/Github_and_other_sources/org.happysanta.gd_29_src.tar.gz/res/drawable-mdpi/btn_br_down.9.png");
//	g_object_set_data(G_OBJECT(wrapper), "background_ninepatch", ninepatch);
}

JNIEXPORT void JNICALL Java_android_widget_LinearLayout_setOrientation(JNIEnv *env, jobject this, jint orientation)
{
	gtk_orientable_set_orientation(GTK_ORIENTABLE(_PTR(_GET_LONG_FIELD(this, "widget"))), orientation ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL);
}
