#include <gtk/gtk.h>

#include "../defines.h"
#include "../util.h"
#include "../drawables/ninepatch.h"

#include "WrapperWidget.h"

/**
 * gdk_texture_new_for_surface:
 * @surface: a cairo image surface
 *
 * Creates a new texture object representing the surface.
 *
 * @surface must be an image surface with format CAIRO_FORMAT_ARGB32.
 *
 * Returns: a new `GdkTexture`
 */
GdkTexture * gdk_texture_new_for_surface (cairo_surface_t *surface)
{
  GdkTexture *texture;
  GBytes *bytes;

  g_return_val_if_fail (cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_IMAGE, NULL);
  g_return_val_if_fail (cairo_image_surface_get_width (surface) > 0, NULL);
  g_return_val_if_fail (cairo_image_surface_get_height (surface) > 0, NULL);

  bytes = g_bytes_new_with_free_func (cairo_image_surface_get_data (surface),
                                      cairo_image_surface_get_height (surface)
                                      * cairo_image_surface_get_stride (surface),
                                      (GDestroyNotify) cairo_surface_destroy,
                                      cairo_surface_reference (surface));
  
  texture = gdk_memory_texture_new (cairo_image_surface_get_width (surface),
                                    cairo_image_surface_get_height (surface),
                                    GDK_MEMORY_DEFAULT,
                                    bytes,
                                    cairo_image_surface_get_stride (surface));

  g_bytes_unref (bytes);

  return texture;
}

// ---

G_DEFINE_TYPE(WrapperWidget, wrapper_widget, GTK_TYPE_WIDGET)

static void wrapper_widget_init (WrapperWidget *wrapper_widget)
{

}

static void wrapper_widget_dispose(GObject *wrapper_widget)
{
	gtk_widget_unparent(gtk_widget_get_first_child(GTK_WIDGET(wrapper_widget)));
	WrapperWidget *wrapper = WRAPPER_WIDGET(wrapper_widget);
	if (wrapper->jvm) {
		JNIEnv *env;
		(*wrapper->jvm)->GetEnv(wrapper->jvm, (void**)&env, JNI_VERSION_1_6);
		if (wrapper->jobj)
			_UNREF(wrapper->jobj);
		if (wrapper->canvas)
			_UNREF(wrapper->canvas);
	}
	G_OBJECT_CLASS (wrapper_widget_parent_class)->dispose (wrapper_widget);
}

void wrapper_snapshot(GtkWidget* widget, GtkSnapshot* snapshot)
{
	struct ninepatch_t *ninepatch = g_object_get_data(G_OBJECT(widget), "background_ninepatch");

	if(ninepatch && 0) {
		GtkAllocation alloc;
		gtk_widget_get_allocation(widget, &alloc);

		ninepatch_stretch(ninepatch, alloc.width, alloc.height);

		cairo_surface_t *surface = ninepatch_to_surface(ninepatch);

//		GdkPixbuf *pixbuf = gdk_pixbuf_get_from_surface(surface, 0, 0, ninepatch->__w, ninepatch->__h);
//		GdkTexture *texture = gdk_texture_new_for_pixbuf(pixbuf);
		GdkTexture *texture = gdk_texture_new_for_surface(surface);

		gtk_snapshot_append_texture(snapshot, texture, &GRAPHENE_RECT_INIT(0, 0, ninepatch->__w, ninepatch->__h));

		cairo_surface_destroy(surface);
//		g_object_unref(pixbuf);
		g_object_unref(texture);
	}
	gtk_widget_snapshot_child(widget, gtk_widget_get_first_child(widget), snapshot);

	// if onDraw method is overwritten call it now
	WrapperWidget *wrapper_widget = WRAPPER_WIDGET(widget);
	GtkAllocation alloc;
	gtk_widget_get_allocation(widget, &alloc);
	if (wrapper_widget->draw_method) {
		cairo_t *cr = gtk_snapshot_append_cairo (snapshot, &GRAPHENE_RECT_INIT(0, 0, alloc.width, alloc.height));

		JNIEnv *env;
		(*wrapper_widget->jvm)->GetEnv(wrapper_widget->jvm, (void**)&env, JNI_VERSION_1_6);
		if(wrapper_widget->canvas == NULL) {
			wrapper_widget->canvas = _REF((*env)->NewObject(env, handle_cache.canvas.class, handle_cache.canvas.constructor, _INTPTR(cr), 0));
		} else {
			_SET_LONG_FIELD(wrapper_widget->canvas, "cairo_context", _INTPTR(cr));
		}

		(*env)->CallVoidMethod(env, wrapper_widget->jobj, wrapper_widget->draw_method, wrapper_widget->canvas);

		if((*env)->ExceptionCheck(env))
			(*env)->ExceptionDescribe(env);

		cairo_destroy (cr);
	}
}


static void wrapper_widget_class_init(WrapperWidgetClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

	object_class->dispose = wrapper_widget_dispose;

	widget_class->snapshot = wrapper_snapshot;

	gtk_widget_class_set_layout_manager_type(widget_class, GTK_TYPE_BIN_LAYOUT);
}

GtkWidget * wrapper_widget_new(void)
{
	return g_object_new (wrapper_widget_get_type(), NULL);
}

void wrapper_widget_set_child(WrapperWidget *parent, GtkWidget *child) // TODO: make sure there can only be one child
{
	gtk_widget_insert_before(child, GTK_WIDGET(parent), NULL);
}

#define MEASURE_SPEC_EXACTLY (1 << 30)

static void on_mapped(GtkWidget* self, gpointer data)
{
	WrapperWidget *wrapper = WRAPPER_WIDGET(self);
	if (wrapper->jvm) {
		JNIEnv *env;
		(*wrapper->jvm)->GetEnv(wrapper->jvm, (void**)&env, JNI_VERSION_1_6);

		(*env)->CallVoidMethod(env, wrapper->jobj, wrapper->measure_method, MEASURE_SPEC_EXACTLY, MEASURE_SPEC_EXACTLY);
	}
}

void wrapper_widget_set_jobject(WrapperWidget *wrapper, JNIEnv *env, jobject jobj)
{
	JavaVM *jvm;
	(*env)->GetJavaVM(env, &jvm);
	wrapper->jvm = jvm;
	wrapper->jobj = _REF(jobj);
	jmethodID draw_method = _METHOD(_CLASS(jobj), "onDraw", "(Landroid/graphics/Canvas;)V");
	if (draw_method != handle_cache.view.onDraw)
		wrapper->draw_method = draw_method;

	jmethodID measure_method = _METHOD(_CLASS(jobj), "onMeasure", "(II)V");
	if (measure_method != handle_cache.view.onMeasure) {
		wrapper->measure_method = measure_method;
		// add a callback for when the widget is mapped, which will call onMeasure to figure out what size the widget wants to be
		g_signal_connect(wrapper, "map", G_CALLBACK(on_mapped), NULL);
	}
}
