#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"

#include "WrapperWidget.h"

#include "../generated_headers/android_widget_LinearLayout.h"
#include "../generated_headers/android_view_ViewGroup.h"

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

static void frame_layout_widget_dispose(GObject *frame_layout)
{
	GtkWidget *child;
	while((child = gtk_widget_get_first_child(GTK_WIDGET(frame_layout))) != NULL) {
		gtk_widget_unparent(child);
	}

	G_OBJECT_CLASS (frame_layout_widget_parent_class)->dispose (frame_layout);
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

JNIEXPORT jlong JNICALL Java_android_widget_FrameLayout_native_1constructor(JNIEnv *env, jobject this, jobject context, jobject attrs)
{
	GtkWidget *wrapper = g_object_ref(wrapper_widget_new());
	GtkWidget *frame_layout = frame_layout_widget_new();
	wrapper_widget_set_child(WRAPPER_WIDGET(wrapper), frame_layout);
	gtk_widget_set_name(GTK_WIDGET(frame_layout), "FrameLayout");
	return _INTPTR(frame_layout);
}

JNIEXPORT void JNICALL Java_android_widget_FrameLayout_native_1addView(JNIEnv *env, jobject this, jlong widget, jlong child, jint index, jobject layout_params)
{
	if(index >= 0)
		frame_layout_widget_insert_child_at_index(FRAME_LAYOUT_WIDGET(_PTR(widget)), gtk_widget_get_parent(GTK_WIDGET(_PTR(child))), index);
	else
		frame_layout_widget_insert_child(FRAME_LAYOUT_WIDGET(_PTR(widget)), gtk_widget_get_parent(GTK_WIDGET(_PTR(child))));
}
