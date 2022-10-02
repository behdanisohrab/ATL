#ifndef NINEPATCH_H
#define NINEPATCH_H

struct ninepatch_t {
	int width, height;
	int left, top, right, bottom;
	cairo_surface_t * lt;
	cairo_surface_t * mt;
	cairo_surface_t * rt;
	cairo_surface_t * lm;
	cairo_surface_t * mm;
	cairo_surface_t * rm;
	cairo_surface_t * lb;
	cairo_surface_t * mb;
	cairo_surface_t * rb;
	double __w, __h;
	double __sx, __sy;
};

void ninepatch_stretch(struct ninepatch_t * ninepatch, double width, double height);
struct ninepatch_t * ninepatch_new(char *filename);
cairo_surface_t * ninepatch_to_surface(struct ninepatch_t *ninepatch);

#endif
