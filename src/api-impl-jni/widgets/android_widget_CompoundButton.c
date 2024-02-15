#include <gtk/gtk.h>

#include "WrapperWidget.h"
#include "../util.h"

#include "../generated_headers/android_widget_CompoundButton.h"
#include "jni.h"

JNIEXPORT jlong JNICALL Java_android_widget_CompoundButton_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
    GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
    GtkWidget *switcher = gtk_switch_new();
    wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), switcher);
    wrapper_widget_set_jobject(WRAPPER_WIDGET(wrapper), env, this);
    return _INTPTR(switcher);
}

JNIEXPORT void JNICALL Java_android_widget_CompoundButton_setChecked(JNIEnv *env, jobject this, jboolean checked)
{
    gtk_switch_set_active(GTK_SWITCH(_PTR(_GET_LONG_FIELD(this, "widget"))), checked);
}

JNIEXPORT jboolean JNICALL Java_android_widget_CompoundButton_isChecked(JNIEnv *env, jobject this)
{
    return gtk_switch_get_active(GTK_SWITCH(_PTR(_GET_LONG_FIELD(this, "widget"))));
}

static gboolean on_state_set(GtkSwitch* self, gboolean state, jobject listener)
{
    JNIEnv *env = get_jni_env();
    WrapperWidget *wrapper = WRAPPER_WIDGET(gtk_widget_get_parent(GTK_WIDGET(self)));
    jmethodID on_check_changed = _METHOD(_CLASS(listener), "onCheckedChanged", "(Landroid/widget/CompoundButton;Z)V");
    (*env)->CallVoidMethod(env, listener, on_check_changed, wrapper->jobj, state);
    return FALSE;
}

JNIEXPORT void JNICALL Java_android_widget_CompoundButton_setOnCheckedChangeListener(JNIEnv *env, jobject this, jobject listener)
{
    GtkSwitch *switcher = GTK_SWITCH(_PTR(_GET_LONG_FIELD(this, "widget")));

    g_signal_handlers_block_matched(switcher, G_SIGNAL_MATCH_FUNC, 0, 0, NULL, on_state_set, NULL);

    if (listener) {
        g_signal_connect(switcher, "state-set", G_CALLBACK(on_state_set), _REF(listener));
    }
}
