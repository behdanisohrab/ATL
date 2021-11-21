#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "android_widget_LinearLayout.h"
#include "../views/android_view_ViewGroup.h"

G_DECLARE_FINAL_TYPE (FrameLayoutWidget, frame_layout_widget, FRAME_LAYOUT, WIDGET, GtkWidget)

struct _FrameLayoutWidget
{
	GtkWidget parent_instance;
};

struct _FrameLayoutWidgetClass
{
	GtkWidgetClass parent_class;
};

G_DEFINE_TYPE(FrameLayoutWidget, frame_layout_widget, GTK_TYPE_WIDGET)

static void frame_layout_widget_init (FrameLayoutWidget *frame_layout)
{

}

static void frame_layout_widget_dispose(GObject *object)
{
	G_OBJECT_CLASS (frame_layout_widget_parent_class)->dispose (object);
}

static void frame_layout_widget_class_init(FrameLayoutWidgetClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

	object_class->dispose = frame_layout_widget_dispose;

	gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

GtkWidget * frame_layout_widget_new (void)
{
	return g_object_new (frame_layout_widget_get_type(), NULL);
}

void frame_layout_widget_insert_child(FrameLayoutWidget *parent, GtkWidget *child)
{
	printf("::::::::::: FrameLayoutWidget: inserting something at the end of the widget list\n");
	gtk_widget_insert_before(child, GTK_WIDGET(parent), NULL);
}

void frame_layout_widget_insert_child_at_index(FrameLayoutWidget *parent, GtkWidget *child, int index)
{
	printf("::::::::::: FrameLayoutWidget: inserting something at index %d\n", index);
	GtkWidget *iter = gtk_widget_get_first_child(GTK_WIDGET(parent));
	GtkWidget *next = NULL;
	for(int i = 0; i < index; i++) {
		next = gtk_widget_get_next_sibling(iter);
		if(next == NULL)
			break;
		iter = next;
	}

	gtk_widget_insert_before(child, GTK_WIDGET(parent), iter);
}

// ---

JNIEXPORT void JNICALL Java_android_widget_FrameLayout_native_1constructor__Landroid_util_AttributeSet_2(JNIEnv *env, jobject this, jobject attrs)
{
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *frame_layout = frame_layout_widget_new();
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), frame_layout);
	gtk_widget_set_name(GTK_WIDGET(frame_layout), "FrameLayout");
	_SET_LONG_FIELD(this, "widget", (long)frame_layout);
	g_object_ref(wrapper);
}

JNIEXPORT void JNICALL Java_android_widget_FrameLayout_native_1constructor__Landroid_content_Context_2(JNIEnv *env, jobject this, jobject context) {
	GtkWidget *wrapper = wrapper_widget_new();
	GtkWidget *frame_layout = frame_layout_widget_new();
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), frame_layout);
	gtk_widget_set_name(GTK_WIDGET(frame_layout), "FrameLayout");
	_SET_LONG_FIELD(this, "widget", (long)frame_layout);
	g_object_ref(wrapper);
}

JNIEXPORT void JNICALL Java_android_widget_FrameLayout_addView(JNIEnv *env, jobject this, jobject child, jint index, jobject layout_params)
{
	Java_android_view_ViewGroup_addView(env, this, child, index, layout_params);

	if(index >= 0)
		frame_layout_widget_insert_child_at_index(FRAME_LAYOUT_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget"))), gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(child, "widget")))), index);
	else
		frame_layout_widget_insert_child(FRAME_LAYOUT_WIDGET(_PTR(_GET_LONG_FIELD(this, "widget"))), gtk_widget_get_parent(GTK_WIDGET(_PTR(_GET_LONG_FIELD(child, "widget")))));
}
