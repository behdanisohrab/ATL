#include <gtk/gtk.h>

#include "ninepatch.h"

// ----- following yeeted from https://github.com/tongjinlv/my_xboot/blob/3d6a255ef4118486c13953cb07a805b0baab4bc2/src/framework/display/l-ninepatch.c ----- 
// [ FIXME: 1) this doesn't operate on binary 9patch files, but on source ones 2) this only works for basic cases where you have at most nine patches ]

static inline int detect_black_pixel(unsigned char * p)
{
	return (((p[0] == 0) && (p[1] == 0) && (p[2] == 0) && (p[3] != 0)) ? 1 : 0);
}

void ninepatch_stretch(struct ninepatch_t * ninepatch, double width, double height)
{
	int lr = ninepatch->left + ninepatch->right;
	int tb = ninepatch->top + ninepatch->bottom;

	if(width < ninepatch->width)
		width = ninepatch->width;
	if(height < ninepatch->height)
		height = ninepatch->height;
	ninepatch->__w = width;
	ninepatch->__h = height;
	ninepatch->__sx = (ninepatch->__w - lr) / (ninepatch->width - lr);
	ninepatch->__sy = (ninepatch->__h - tb) / (ninepatch->height - tb);
}

static bool surface_to_ninepatch(cairo_surface_t * surface, struct ninepatch_t * patch)
{
	cairo_surface_t * cs;
	cairo_t * cr;
	unsigned char * data;
	int width, height;
	int stride;
	int w, h;
	int i;

	if(!surface || !patch)
		return FALSE;

	width = cairo_image_surface_get_width(surface);
	height = cairo_image_surface_get_height(surface);
	if(width < 3 || height < 3)
		return FALSE;

	/* Nine patch chunk */
	cs = cairo_surface_create_similar_image(surface, CAIRO_FORMAT_ARGB32, width, height);
	cr = cairo_create(cs);
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);
	cairo_destroy(cr);
	data = cairo_image_surface_get_data(cs);
	stride = cairo_image_surface_get_stride(cs);

	/* Nine patch default size */
	width = width - 2;
	height = height - 2;
	patch->width = width;
	patch->height = height;

	/* Stretch information */
	patch->left = 0;
	patch->right = 0;
	patch->top = 0;
	patch->right = 0;

	for(i = 0; i < width; i++)
	{
		if(detect_black_pixel(&data[(i + 1) * 4]))
		{
			patch->left = i;
			break;
		}
	}
	for(i = width - 1; i >= 0; i--)
	{
		if(detect_black_pixel(&data[(i + 1) * 4]))
		{
			patch->right = width - 1 - i;
			break;
		}
	}
	for(i = 0; i < height; i++)
	{
		if(detect_black_pixel(&data[stride * (i + 1)]))
		{
			patch->top = i;
			break;
		}
	}
	for(i = height - 1; i >= 0; i--)
	{
		if(detect_black_pixel(&data[stride * (i + 1)]))
		{
			patch->bottom = height - 1 - i;
			break;
		}
	}
	cairo_surface_destroy(cs);

	/* Left top */
	w = patch->left;
	h = patch->top;
	if(w > 0 && h > 0)
	{
		cs = cairo_surface_create_similar(surface, cairo_surface_get_content(surface), patch->left, patch->top);
		cr = cairo_create(cs);
		cairo_set_source_surface(cr, surface, -1, -1);
		cairo_paint(cr);
		cairo_destroy(cr);
		patch->lt = cs;
	}
	else
	{
		patch->lt = NULL;
	}

	/* Middle top */
	w = width - patch->left - patch->right;
	h = patch->top;
	if(w > 0 && h > 0)
	{
		cs = cairo_surface_create_similar(surface, cairo_surface_get_content(surface), w, h);
		cr = cairo_create(cs);
		cairo_set_source_surface(cr, surface, -patch->left - 1, -1);
		cairo_paint(cr);
		cairo_destroy(cr);
		patch->mt = cs;
	}
	else
	{
		patch->mt = NULL;
	}

	/* Right top */
	w = patch->right;
	h = patch->top;
	if(w > 0 && h > 0)
	{
		cs = cairo_surface_create_similar(surface, cairo_surface_get_content(surface), w, h);
		cr = cairo_create(cs);
		cairo_set_source_surface(cr, surface, -(width - patch->right) - 1, -1);
		cairo_paint(cr);
		cairo_destroy(cr);
		patch->rt = cs;
	}
	else
	{
		patch->rt = NULL;
	}

	/* Left Middle */
	w = patch->left;
	h = height - patch->top - patch->bottom;
	if(w > 0 && h > 0)
	{
		cs = cairo_surface_create_similar(surface, cairo_surface_get_content(surface), w, h);
		cr = cairo_create(cs);
		cairo_set_source_surface(cr, surface, -1, -patch->top - 1);
		cairo_paint(cr);
		cairo_destroy(cr);
		patch->lm = cs;
	}
	else
	{
		patch->lm = NULL;
	}

	/* Middle Middle */
	w = width - patch->left - patch->right;
	h = height - patch->top - patch->bottom;
	if(w > 0 && h > 0)
	{
		cs = cairo_surface_create_similar(surface, cairo_surface_get_content(surface), w, h);
		cr = cairo_create(cs);
		cairo_set_source_surface(cr, surface, -patch->left - 1, -patch->top - 1);
		cairo_paint(cr);
		cairo_destroy(cr);
		patch->mm = cs;
	}
	else
	{
		patch->mm = NULL;
	}

	/* Right middle */
	w = patch->right;
	h = height - patch->top - patch->bottom;
	if(w > 0 && h > 0)
	{
		cs = cairo_surface_create_similar(surface, cairo_surface_get_content(surface), w, h);
		cr = cairo_create(cs);
		cairo_set_source_surface(cr, surface, -(width - patch->right) - 1, -patch->top - 1);
		cairo_paint(cr);
		cairo_destroy(cr);
		patch->rm = cs;
	}
	else
	{
		patch->rm = NULL;
	}

	/* Left bottom */
	w = patch->left;
	h = patch->bottom;
	if(w > 0 && h > 0)
	{
		cs = cairo_surface_create_similar(surface, cairo_surface_get_content(surface), w, h);
		cr = cairo_create(cs);
		cairo_set_source_surface(cr, surface, -1, -(height - patch->bottom) - 1);
		cairo_paint(cr);
		cairo_destroy(cr);
		patch->lb = cs;
	}
	else
	{
		patch->lb = NULL;
	}

	/* Middle bottom */
	w = width - patch->left - patch->right;
	h = patch->bottom;
	if(w > 0 && h > 0)
	{
		cs = cairo_surface_create_similar(surface, cairo_surface_get_content(surface), w, h);
		cr = cairo_create(cs);
		cairo_set_source_surface(cr, surface, -patch->left - 1, -(height - patch->bottom) - 1);
		cairo_paint(cr);
		cairo_destroy(cr);
		patch->mb = cs;
	}
	else
	{
		patch->mb = NULL;
	}

	/* Right bottom */
	w = patch->right;
	h = patch->bottom;
	if(w > 0 && h > 0)
	{
		cs = cairo_surface_create_similar(surface, cairo_surface_get_content(surface), w, h);
		cr = cairo_create(cs);
		cairo_set_source_surface(cr, surface, -(width - patch->right) - 1, -(height - patch->bottom) - 1);
		cairo_paint(cr);
		cairo_destroy(cr);
		patch->rb = cs;
	}
	else
	{
		patch->rb = NULL;
	}

	ninepatch_stretch(patch, width, height);
	return TRUE;
}

struct ninepatch_t * ninepatch_new(char *filename)
{
	struct ninepatch_t *ninepatch = malloc(sizeof(struct ninepatch_t));
	cairo_surface_t *surface = cairo_image_surface_create_from_png(filename);
	if(cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS)
		exit(-1);
	bool result = surface_to_ninepatch(surface, ninepatch);
	cairo_surface_destroy(surface);
	if(!result)
		exit(-2);

	return ninepatch;
}

// ----- end of borrowed code -----

cairo_surface_t * ninepatch_to_surface(struct ninepatch_t *ninepatch)
{
	static const cairo_matrix_t identity = {1, 0,
	                                        0, 1,
	                                        0, 0};

	cairo_surface_t *cs = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (int)ninepatch->__w, (int)ninepatch->__h);
	cairo_t *cr = cairo_create(cs);

	cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

	// relevant measurements
	const int left_w = ninepatch->left;
	const int top_h = ninepatch->top;
	const int middle_h = ninepatch->height - ninepatch->top - ninepatch->bottom;
	const int middle_w = ninepatch->width - ninepatch->left - ninepatch->right;

	// offset for left/top is zero, and for middle it's width/height of left/top respectively
	double offset_right_x = left_w + middle_w * ninepatch->__sx;
	double offset_bottom_y = top_h + middle_h * ninepatch->__sy;

	// --- left top ---
	if(ninepatch->lt) {
		cairo_set_source_surface(cr, ninepatch->lt, 0, 0);
		cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
		cairo_paint(cr);
	}

	// --- left middle ---
	if(ninepatch->lm) {
		cairo_translate(cr, 0, top_h);
		cairo_scale(cr, 1, ninepatch->__sy);
		cairo_set_source_surface(cr, ninepatch->lm, 0, 0);
		cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
		cairo_paint(cr);
		cairo_set_matrix(cr, &identity);
	}

	// --- left bottom ---
	if(ninepatch->lb) {
		cairo_translate(cr, 0, offset_bottom_y);
		cairo_set_source_surface(cr, ninepatch->lb, 0, 0);
		cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
		cairo_paint(cr);
		cairo_set_matrix(cr, &identity);
	}

// -------------------------------------------------------------------------

	// --- middle top ---
	if(ninepatch->mt) {
		cairo_translate(cr, left_w, 0);
		cairo_scale(cr, ninepatch->__sx, 1);
		cairo_set_source_surface(cr, ninepatch->mt, 0, 0);
		cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
		cairo_paint(cr);
		cairo_set_matrix(cr, &identity);
	}

	// --- middle middle ---
	if(ninepatch->mm) {
		cairo_translate(cr, left_w, top_h);
		cairo_scale(cr, ninepatch->__sx, ninepatch->__sy);
		cairo_set_source_surface(cr, ninepatch->mm, 0, 0);
		cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
		cairo_paint(cr);
		cairo_set_matrix(cr, &identity);
	}

	// --- middle bottom ---
	if(ninepatch->mb) {
		cairo_translate(cr, left_w, offset_bottom_y);
		cairo_scale(cr, ninepatch->__sx, 1);
		cairo_set_source_surface(cr, ninepatch->mb, 0, 0);
		cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
		cairo_paint(cr);
		cairo_set_matrix(cr, &identity);
	}

// -------------------------------------------------------------------------

	// --- right top ---
	if(ninepatch->rt) {
		cairo_translate(cr, offset_right_x, 0);
		cairo_set_source_surface(cr, ninepatch->rt, 0, 0);
		cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
		cairo_paint(cr);
		cairo_set_matrix(cr, &identity);
	}

	// --- right middle ---
	if(ninepatch->rm) {
		cairo_translate(cr, offset_right_x, top_h);
		cairo_scale(cr, 1, ninepatch->__sy);
		cairo_set_source_surface(cr, ninepatch->rm, 0, 0);
		cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
		cairo_paint(cr);
		cairo_set_matrix(cr, &identity);
	}

	// --- right bottom ---
	if(ninepatch->rb) {
		cairo_translate(cr, offset_right_x, offset_bottom_y);
		cairo_set_source_surface(cr, ninepatch->rb, 0, 0);
		cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
		cairo_paint(cr);
		cairo_set_matrix(cr, &identity);
	}

	printf(";;;;;; %lf %lf | %d %d\n", ninepatch->__w, ninepatch->__h, cairo_image_surface_get_width(cs), cairo_image_surface_get_height(cs));

	return cs;
}
