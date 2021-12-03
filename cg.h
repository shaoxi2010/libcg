#ifndef __CG_H__
#define __CG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <swft.h>

struct cg_point_t {
	double x;
	double y;
};

struct cg_rect_t {
	double x;
	double y;
	double w;
	double h;
};

struct cg_matrix_t {
	double m00; double m10;
	double m01; double m11;
	double m02; double m12;
};

struct cg_color_t {
	double r;
	double g;
	double b;
	double a;
};

struct cg_gradient_stop_t {
	double offset;
	struct cg_color_t color;
};

enum cg_path_element_t {
	XVG_PATH_ELEMENT_MOVE_TO,
	XVG_PATH_ELEMENT_LINE_TO,
	XVG_PATH_ELEMENT_CURVE_TO,
	XVG_PATH_ELEMENT_CLOSE,
};

enum cg_spread_method_t {
	XVG_SPREAD_METHOD_PAD,
	XVG_SPREAD_METHOD_REFLECT,
	XVG_SPREAD_METHOD_REPEAT,
};

enum cg_gradient_type_t {
	XVG_GRADIENT_TYPE_LINEAR,
	XVG_GRADIENT_TYPE_RADIAL,
};

enum cg_texture_type_t {
	XVG_TEXTURE_TYPE_PLAIN,
	XVG_TEXTURE_TYPE_TILED,
};

enum cg_line_cap_t {
	XVG_LINE_CAP_BUTT,
	XVG_LINE_CAP_ROUND,
	XVG_LINE_CAP_SQUARE,
};

enum cg_line_join_t {
	XVG_LINE_JOIN_MITER,
	XVG_LINE_JOIN_ROUND,
	XVG_LINE_JOIN_BEVEL,
};

enum cg_fill_rule_t {
	XVG_FILL_RULE_WINDING,
	XVG_FILL_RULE_EVEN_ODD,
};

enum cg_paint_type_t {
	XVG_PAINT_TYPE_COLOR,
	XVG_PAINT_TYPE_GRADIENT,
	XVG_PAINT_TYPE_TEXTURE,
};

enum cg_operator_t {
	XVG_OPERATOR_SRC,
	XVG_OPERATOR_SRC_OVER,
	XVG_OPERATOR_DST_IN,
	XVG_OPERATOR_DST_OUT,
};

struct cg_surface_t {
	int ref;
	int width;
	int height;
	int stride;
	int owndata;
	void * pixels;
};

struct cg_path_t {
	int ref;
	int contours;
	struct cg_point_t start;
	struct {
		enum cg_path_element_t * data;
		int size;
		int capacity;
	} elements;
	struct {
		struct cg_point_t * data;
		int size;
		int capacity;
	} points;
};

struct cg_gradient_t {
	int ref;
	enum cg_gradient_type_t type;
	enum cg_spread_method_t spread;
	struct cg_matrix_t matrix;
	double values[6];
	double opacity;
	struct {
		struct cg_gradient_stop_t * data;
		int size;
		int capacity;
	} stops;
};

struct cg_texture_t {
	int ref;
	enum cg_texture_type_t type;
	struct cg_surface_t * surface;
	struct cg_matrix_t matrix;
	double opacity;
};

struct cg_paint_t {
	int ref;
	enum cg_paint_type_t type;
	union {
		struct cg_color_t * color;
		struct cg_gradient_t * gradient;
		struct cg_texture_t * texture;
	};
};

struct cg_span_t {
	short x;
	short y;
	unsigned short len;
	unsigned char coverage;
};

struct cg_rle_t {
	struct {
		struct cg_span_t * data;
		int size;
		int capacity;
	} spans;
	int x;
	int y;
	int w;
	int h;
};

struct cg_dash_t {
	double offset;
	double * data;
	int size;
};

struct cg_stroke_data_t {
	double width;
	double miterlimit;
	enum cg_line_cap_t cap;
	enum cg_line_join_t join;
	struct cg_dash_t * dash;
};

struct cg_state_t {
	struct cg_rle_t * clippath;
	struct cg_paint_t * source;
	struct cg_matrix_t matrix;
	enum cg_fill_rule_t winding;
	struct cg_stroke_data_t stroke;
	enum cg_operator_t op;
	double opacity;
	struct cg_state_t * next;
};

struct cg_ctx_t {
	struct cg_surface_t * surface;
	struct cg_state_t * state;
	struct cg_path_t * path;
	struct cg_rle_t * rle;
	struct cg_rle_t * clippath;
	struct cg_rect_t clip;
};

void cg_matrix_init_identity(struct cg_matrix_t * matrix);
void cg_matrix_init_translate(struct cg_matrix_t * matrix, double x, double y);
void cg_matrix_init_scale(struct cg_matrix_t * matrix, double x, double y);
void cg_matrix_init_shear(struct cg_matrix_t * matrix, double x, double y);
void cg_matrix_init_rotate(struct cg_matrix_t * matrix, double radians);
void cg_matrix_init_rotate_translate(struct cg_matrix_t * matrix, double radians, double x, double y);
void cg_matrix_translate(struct cg_matrix_t * matrix, double x, double y);
void cg_matrix_scale(struct cg_matrix_t * matrix, double x, double y);
void cg_matrix_shear(struct cg_matrix_t * matrix, double x, double y);
void cg_matrix_rotate(struct cg_matrix_t * matrix, double radians);
void cg_matrix_rotate_translate(struct cg_matrix_t * matrix, double radians, double x, double y);
void cg_matrix_multiply(struct cg_matrix_t * matrix, struct cg_matrix_t * a, struct cg_matrix_t * b);
int cg_matrix_invert(struct cg_matrix_t * matrix);
void cg_matrix_map_point(struct cg_matrix_t * matrix, struct cg_point_t * src, struct cg_point_t * dst);
void cg_matrix_map_rect(struct cg_matrix_t * matrix, struct cg_rect_t * src, struct cg_rect_t * dst);

struct cg_surface_t * cg_surface_create(int width, int height);
struct cg_surface_t * cg_surface_create_for_data(int width, int height, void * pixels);
void cg_surface_destroy(struct cg_surface_t * surface);
struct cg_surface_t * cg_surface_reference(struct cg_surface_t * surface);

struct cg_path_t * cg_path_create(void);
void cg_path_destroy(struct cg_path_t * path);
struct cg_path_t * cg_path_reference(struct cg_path_t * path);
void cg_path_move_to(struct cg_path_t * path, double x, double y);
void cg_path_line_to(struct cg_path_t * path, double x, double y);
void cg_path_curve_to(struct cg_path_t * path, double x1, double y1, double x2, double y2, double x3, double y3);
void cg_path_quad_to(struct cg_path_t * path, double x1, double y1, double x2, double y2);
void cg_path_arc_to(struct cg_path_t * path, double x1, double y1, double x2, double y2, double radius);
void cg_path_close(struct cg_path_t * path);
void cg_path_rel_move_to(struct cg_path_t * path, double dx, double dy);
void cg_path_rel_line_to(struct cg_path_t * path, double dx, double dy);
void cg_path_rel_curve_to(struct cg_path_t * path, double dx1, double dy1, double dx2, double dy2, double dx3, double dy3);
void cg_path_rel_quad_to(struct cg_path_t * path, double dx1, double dy1, double dx2, double dy2);
void cg_path_rel_arc_to(struct cg_path_t * path, double dx1, double dy1, double dx2, double dy2, double radius);
void cg_path_add_rectangle(struct cg_path_t * path, double x, double y, double w, double h);
void cg_path_add_round_rectangle(struct cg_path_t * path, double x, double y, double w, double h, double rx, double ry);
void cg_path_add_ellipse(struct cg_path_t * path, double cx, double cy, double rx, double ry);
void cg_path_add_circle(struct cg_path_t * path, double cx, double cy, double r);
void cg_path_add_arc(struct cg_path_t * path, double cx, double cy, double r, double a0, double a1, int ccw);
void cg_path_add_path(struct cg_path_t * path, struct cg_path_t * source, struct cg_matrix_t * matrix);
void cg_path_transform(struct cg_path_t * path, struct cg_matrix_t * matrix);
void cg_path_get_current_point(struct cg_path_t * path, double * x, double * y);
int cg_path_get_element_count(struct cg_path_t * path);
enum cg_path_element_t * cg_path_get_elements(struct cg_path_t * path);
int cg_path_get_point_count(struct cg_path_t * path);
struct cg_point_t* cg_path_get_points(struct cg_path_t * path);
void cg_path_clear(struct cg_path_t * path);
int cg_path_empty(struct cg_path_t * path);
struct cg_path_t * cg_path_clone(struct cg_path_t * path);
struct cg_path_t * cg_path_clone_flat(struct cg_path_t * path);

struct cg_gradient_t * cg_gradient_create_linear(double x1, double y1, double x2, double y2);
struct cg_gradient_t * cg_gradient_create_radial(double cx, double cy, double cr, double fx, double fy, double fr);
void cg_gradient_destroy(struct cg_gradient_t * gradient);
struct cg_gradient_t * cg_gradient_reference(struct cg_gradient_t * gradient);
void cg_gradient_set_spread(struct cg_gradient_t * gradient, enum cg_spread_method_t spread);
enum cg_spread_method_t cg_gradient_get_spread(struct cg_gradient_t * gradient);
void cg_gradient_set_matrix(struct cg_gradient_t * gradient, struct cg_matrix_t * matrix);
void cg_gradient_get_matrix(struct cg_gradient_t * gradient, struct cg_matrix_t * matrix);
void cg_gradient_add_stop_rgb(struct cg_gradient_t * gradient, double offset, double r, double g, double b);
void cg_gradient_add_stop_rgba(struct cg_gradient_t * gradient, double offset, double r, double g, double b, double a);
void cg_gradient_add_stop_color(struct cg_gradient_t * gradient, double offset, struct cg_color_t * color);
void cg_gradient_add_stop(struct cg_gradient_t * gradient, struct cg_gradient_stop_t *stop);
void cg_gradient_clear_stops(struct cg_gradient_t * gradient);
int cg_gradient_get_stop_count(struct cg_gradient_t * gradient);
struct cg_gradient_stop_t* cg_gradient_get_stops(struct cg_gradient_t * gradient);
enum cg_gradient_type_t cg_gradient_get_type(struct cg_gradient_t * gradient);
void cg_gradient_get_values_linear(struct cg_gradient_t * gradient, double *x1, double *y1, double *x2, double *y2);
void cg_gradient_get_values_radial(struct cg_gradient_t * gradient, double *cx, double *cy, double *cr, double *fx, double *fy, double *fr);
void cg_gradient_set_values_linear(struct cg_gradient_t * gradient, double x1, double y1, double x2, double y2);
void cg_gradient_set_values_radial(struct cg_gradient_t * gradient, double cx, double cy, double cr, double fx, double fy, double fr);
void cg_gradient_set_opacity(struct cg_gradient_t * paint, double opacity);
double cg_gradient_get_opacity(struct cg_gradient_t * paint);

struct cg_texture_t * cg_texture_create(struct cg_surface_t * surface);
void cg_texture_destroy(struct cg_texture_t * texture);
struct cg_texture_t * cg_texture_reference(struct cg_texture_t * texture);
void cg_texture_set_type(struct cg_texture_t * texture, enum cg_texture_type_t type);
enum cg_texture_type_t cg_texture_get_type(struct cg_texture_t * texture);
void cg_texture_set_matrix(struct cg_texture_t * texture, struct cg_matrix_t * matrix);
void cg_texture_get_matrix(struct cg_texture_t * texture, struct cg_matrix_t * matrix);
void cg_texture_set_surface(struct cg_texture_t * texture, struct cg_surface_t * surface);
struct cg_surface_t * cg_texture_get_surface(struct cg_texture_t * texture);
void cg_texture_set_opacity(struct cg_texture_t * texture, double opacity);
double cg_texture_get_opacity(struct cg_texture_t * texture);

struct cg_paint_t * cg_paint_create_rgb(double r, double g, double b);
struct cg_paint_t * cg_paint_create_rgba(double r, double g, double b, double a);
struct cg_paint_t * cg_paint_create_linear(double x1, double y1, double x2, double y2);
struct cg_paint_t * cg_paint_create_radial(double cx, double cy, double cr, double fx, double fy, double fr);
struct cg_paint_t * cg_paint_create_for_surface(struct cg_surface_t * surface);
struct cg_paint_t * cg_paint_create_color(struct cg_color_t * color);
struct cg_paint_t * cg_paint_create_gradient(struct cg_gradient_t * gradient);
struct cg_paint_t * cg_paint_create_texture(struct cg_texture_t * texture);
void cg_paint_destroy(struct cg_paint_t * paint);
struct cg_paint_t * cg_paint_reference(struct cg_paint_t * paint);
enum cg_paint_type_t cg_paint_get_type(struct cg_paint_t * paint);
struct cg_color_t * cg_paint_get_color(struct cg_paint_t * paint);
struct cg_gradient_t * cg_paint_get_gradient(struct cg_paint_t * paint);
struct cg_texture_t * cg_paint_get_texture(struct cg_paint_t * paint);

struct cg_ctx_t * cg_create(struct cg_surface_t * surface);
void cg_destroy(struct cg_ctx_t * ctx);
void cg_save(struct cg_ctx_t * ctx);
void cg_restore(struct cg_ctx_t * ctx);
void cg_set_source_rgb(struct cg_ctx_t * ctx, double r, double g, double b);
void cg_set_source_rgba(struct cg_ctx_t * ctx, double r, double g, double b, double a);
void cg_set_source_surface(struct cg_ctx_t * ctx, struct cg_surface_t * surface, double x, double y);
void cg_set_source_color(struct cg_ctx_t * ctx, struct cg_color_t * color);
void cg_set_source_gradient(struct cg_ctx_t * ctx, struct cg_gradient_t * gradient);
void cg_set_source_texture(struct cg_ctx_t * ctx, struct cg_texture_t * texture);
void cg_set_source(struct cg_ctx_t * ctx, struct cg_paint_t * source);
void cg_set_operator(struct cg_ctx_t * ctx, enum cg_operator_t op);
void cg_set_opacity(struct cg_ctx_t * ctx, double opacity);
void cg_set_fill_rule(struct cg_ctx_t * ctx, enum cg_fill_rule_t winding);
void cg_set_line_width(struct cg_ctx_t * ctx, double width);
void cg_set_line_cap(struct cg_ctx_t * ctx, enum cg_line_cap_t cap);
void cg_set_line_join(struct cg_ctx_t * ctx, enum cg_line_join_t join);
void cg_set_miter_limit(struct cg_ctx_t * ctx, double limit);
void cg_set_dash(struct cg_ctx_t * ctx, double * dashes, int ndash, double offset);
void cg_translate(struct cg_ctx_t * ctx, double x, double y);
void cg_scale(struct cg_ctx_t * ctx, double x, double y);
void cg_rotate(struct cg_ctx_t * ctx, double radians);
void cg_transform(struct cg_ctx_t * ctx, struct cg_matrix_t * matrix);
void cg_set_matrix(struct cg_ctx_t * ctx, struct cg_matrix_t * matrix);
void cg_set_matrix_identity(struct cg_ctx_t * ctx);
void cg_move_to(struct cg_ctx_t * ctx, double x, double y);
void cg_line_to(struct cg_ctx_t * ctx, double x, double y);
void cg_curve_to(struct cg_ctx_t * ctx, double x1, double y1, double x2, double y2, double x3, double y3);
void cg_quad_to(struct cg_ctx_t * ctx, double x1, double y1, double x2, double y2);
void cg_rel_move_to(struct cg_ctx_t * ctx, double dx, double dy);
void cg_rel_line_to(struct cg_ctx_t * ctx, double dx, double dy);
void cg_rel_curve_to(struct cg_ctx_t * ctx, double dx1, double dy1, double dx2, double dy2, double dx3, double dy3);
void cg_rel_quad_to(struct cg_ctx_t * ctx, double dx1, double dy1, double dx2, double dy2);
void cg_rectangle(struct cg_ctx_t * ctx, double x, double y, double w, double h);
void cg_round_rectangle(struct cg_ctx_t * ctx, double x, double y, double w, double h, double rx, double ry);
void cg_ellipse(struct cg_ctx_t * ctx, double cx, double cy, double rx, double ry);
void cg_circle(struct cg_ctx_t * ctx, double cx, double cy, double r);
void cg_arc(struct cg_ctx_t * ctx, double cx, double cy, double r, double a0, double a1);
void cg_arc_negative(struct cg_ctx_t * ctx, double cx, double cy, double r, double a0, double a1);
void cg_add_path(struct cg_ctx_t * ctx, struct cg_path_t * path);
void cg_new_path(struct cg_ctx_t * ctx);
void cg_close_path(struct cg_ctx_t * ctx);
void cg_fill(struct cg_ctx_t * ctx);
void cg_stroke(struct cg_ctx_t * ctx);
void cg_clip(struct cg_ctx_t * ctx);
void cg_paint(struct cg_ctx_t * ctx);
void cg_fill_preserve(struct cg_ctx_t * ctx);
void cg_stroke_preserve(struct cg_ctx_t * ctx);
void cg_clip_preserve(struct cg_ctx_t * ctx);
void cg_reset_clip(struct cg_ctx_t * ctx);

#ifdef __cplusplus
}
#endif

#endif /* __CG_H__ */
