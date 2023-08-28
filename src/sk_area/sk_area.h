#ifndef SK_AREA_H
#define SK_AREA_H

#include "include/c/sk_canvas.h"

G_DECLARE_FINAL_TYPE(SKArea, sk_area, SK_AREA, WIDGET, GtkGLArea)

struct _SKArea
{
	GtkGLArea parent_instance;
	void (* draw_func)(SKArea *sk_area, sk_canvas_t *canvas, gpointer data);
	gpointer draw_func_data;

	const gr_glinterface_t *gl_interface;
	gr_direct_context_t *gr_context;
	sk_surface_t* surface;
	sk_canvas_t* canvas;
	gr_backendrendertarget_t *render_target;
	int prev_width;
	int prev_height;
};

struct _SKAreaClass
{
	GtkGLAreaClass parent_class;
};

GtkWidget * sk_area_new(void);
void sk_area_set_draw_func(SKArea *sk_area, void (*draw_func)(SKArea *sk_area, sk_canvas_t *canvas, gpointer data), gpointer data);

#endif
