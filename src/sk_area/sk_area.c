#include <gtk/gtk.h>

#include <epoxy/egl.h>
#include <epoxy/gl.h>

#include "include/c/sk_canvas.h"
#include "include/c/sk_data.h"
#include "include/c/sk_image.h"
#include "include/c/sk_paint.h"
#include "include/c/sk_path.h"
#include "include/c/sk_surface.h"
#include "include/c/gr_context.h"

#include "sk_area.h"

G_DEFINE_TYPE(SKArea, sk_area, GTK_TYPE_GL_AREA)

static void sk_area_init(SKArea *sk_area)
{

}

static void sk_area_dispose(GObject *sk_area)
{
	G_OBJECT_CLASS(sk_area_parent_class)->dispose(sk_area);
}

static void sk_area_unroot(GtkWidget *widget)
{
	SKArea *sk_area = SK_AREA_WIDGET(widget);
	if(sk_area->surface) {
		sk_surface_unref(sk_area->surface);
		sk_area->surface = NULL;
		sk_area->canvas = NULL;
	}

	if(sk_area->render_target) {
		gr_backendrendertarget_delete(sk_area->render_target);
		sk_area->render_target = NULL;
	}

	if(sk_area->gr_context) {
		gr_direct_context_abandon_context(sk_area->gr_context);
		sk_area->gr_context = NULL;
	}

	if(sk_area->gl_interface) {
		gr_glinterface_unref(sk_area->gl_interface);
		sk_area->gl_interface = NULL;
	}

	GTK_WIDGET_CLASS(sk_area_parent_class)->unroot(widget);
}

typedef gboolean (*render_func)(GtkGLArea *, GdkGLContext *);
static gboolean render(SKArea *sk_area, GdkGLContext *context);
static void sk_area_class_init(SKAreaClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS(class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);
	object_class->dispose = sk_area_dispose;
	widget_class->unroot = sk_area_unroot;
	class->parent_class.render = (render_func)render;
}

GtkWidget * sk_area_new(void)
{
	return g_object_new(sk_area_get_type(), NULL);
}

static gr_gl_func_ptr egl_get_gl_proc(void* unused, const char name[]) {
	return eglGetProcAddress(name);
}

static gboolean render(SKArea *sk_area, GdkGLContext *context)
{
	bool reiniting = false;

	if(!sk_area->draw_func)
		return true;

	int width = gtk_widget_get_width(GTK_WIDGET(sk_area));
	int height = gtk_widget_get_height(GTK_WIDGET(sk_area));

	if(!sk_area->gr_context) {
		if(!sk_area->gl_interface)
			sk_area->gl_interface = gr_glinterface_assemble_interface(NULL, egl_get_gl_proc);

		sk_area->gr_context = gr_direct_context_make_gl(sk_area->gl_interface);
		reiniting = true;
	}

	gr_direct_context_reset_context(sk_area->gr_context, 0xffffffff);

	if((width != sk_area->prev_width || height != sk_area->prev_height) || reiniting) {
		GLint framebuffer = 0;
		GLint stencil_bits = 0;
		GLint samples = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
		glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil_bits);
		glGetIntegerv(GL_SAMPLES, &samples);

		if(sk_area->surface) {
			sk_surface_unref(sk_area->surface);
			sk_area->surface = NULL;
			sk_area->canvas = NULL;
		}

		if(sk_area->render_target)
			gr_backendrendertarget_delete(sk_area->render_target);

		gr_gl_framebufferinfo_t gl_info = {.fFBOID = framebuffer, .fFormat = GL_RGBA8};
		sk_area->render_target = gr_backendrendertarget_new_gl(width, height, samples, stencil_bits, &gl_info);
		sk_area->surface = sk_surface_new_backend_render_target((gr_recording_context_t *)sk_area->gr_context, sk_area->render_target, BOTTOM_LEFT_GR_SURFACE_ORIGIN, RGBA_8888_SK_COLORTYPE, NULL, NULL);
		sk_area->canvas = sk_surface_get_canvas(sk_area->surface);
	}

	// TODO - allegedly android doesn't clear the canvas between frames, but clearly at least gravity defied seems to expect us to do so
	sk_canvas_clear(sk_area->canvas, sk_color_set_argb(0x0, 0x00, 0x00, 0x00));
	sk_area->draw_func(sk_area, sk_area->canvas, sk_area->draw_func_data);
	sk_canvas_flush(sk_area->canvas);
	gr_direct_context_flush(sk_area->gr_context);

	sk_area->prev_width = width;
	sk_area->prev_height = height;

	return true;
}

void sk_area_set_draw_func(SKArea *sk_area, void (* draw_func)(SKArea *sk_area, sk_canvas_t *canvas, gpointer data), gpointer data)
{
	sk_area->draw_func = draw_func;
	sk_area->draw_func_data = data;
}
