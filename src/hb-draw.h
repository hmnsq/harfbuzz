/*
 * Copyright © 2019-2020  Ebrahim Byagowi
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#अगर !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#त्रुटि "Include <hb.h> instead."
#पूर्ण_अगर

#अगर_अघोषित HB_DRAW_H
#घोषणा HB_DRAW_H

#समावेश "hb.h"

HB_BEGIN_DECLS


/**
 * hb_draw_state_t
 * @path_open: Whether there is an open path
 * @path_start_x: X component of the start of current path
 * @path_start_y: Y component of the start of current path
 * @current_x: X component of current point
 * @current_y: Y component of current point
 *
 * Current drawing state.
 *
 * Since: 4.0.0
 **/
प्रकार काष्ठा hb_draw_state_t {
  hb_bool_t path_open;

  भग्न path_start_x;
  भग्न path_start_y;

  भग्न current_x;
  भग्न current_y;

  /*< private >*/
  hb_var_अंक_t_2   reserved1;
  hb_var_अंक_t_2   reserved2;
  hb_var_अंक_t_2   reserved3;
  hb_var_अंक_t_2   reserved4;
  hb_var_अंक_t_2   reserved5;
  hb_var_अंक_t_2   reserved6;
  hb_var_अंक_t_2   reserved7;
} hb_draw_state_t;

/**
 * HB_DRAW_STATE_DEFAULT:
 *
 * The default #hb_draw_state_t at the start of glyph drawing.
 */
#घोषणा HB_DRAW_STATE_DEFAULT {0, 0.f, 0.f, 0.f, 0.f, {0}, {0}, {0}, {0}, {0}, {0}, {0}}


/**
 * hb_draw_funcs_t:
 *
 * Glyph draw callbacks.
 *
 * #hb_draw_move_to_func_t, #hb_draw_line_to_func_t and
 * #hb_draw_cubic_to_func_t calls are necessary to be defined but we translate
 * #hb_draw_quadratic_to_func_t calls to #hb_draw_cubic_to_func_t if the
 * callback isn't defined.
 *
 * Since: 4.0.0
 **/

प्रकार काष्ठा hb_draw_funcs_t hb_draw_funcs_t;


/**
 * hb_draw_move_to_func_t:
 * @dfuncs: draw functions object
 * @draw_data: The data accompanying the draw functions in hb_font_draw_glyph()
 * @st: current draw state
 * @to_x: X component of target point
 * @to_y: Y component of target point
 * @user_data: User data pointer passed to hb_draw_funcs_set_move_to_func()
 *
 * A virtual method for the #hb_draw_funcs_t to perform a "move-to" draw
 * operation.
 *
 * Since: 4.0.0
 *
 **/
प्रकार व्योम (*hb_draw_move_to_func_t) (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
					hb_draw_state_t *st,
					भग्न to_x, भग्न to_y,
					व्योम *user_तथ्य);

/**
 * hb_draw_line_to_func_t:
 * @dfuncs: draw functions object
 * @draw_data: The data accompanying the draw functions in hb_font_draw_glyph()
 * @st: current draw state
 * @to_x: X component of target point
 * @to_y: Y component of target point
 * @user_data: User data pointer passed to hb_draw_funcs_set_line_to_func()
 *
 * A virtual method for the #hb_draw_funcs_t to perform a "line-to" draw
 * operation.
 *
 * Since: 4.0.0
 *
 **/
प्रकार व्योम (*hb_draw_पंक्ति_to_func_t) (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
					hb_draw_state_t *st,
					भग्न to_x, भग्न to_y,
					व्योम *user_तथ्य);

/**
 * hb_draw_quadratic_to_func_t:
 * @dfuncs: draw functions object
 * @draw_data: The data accompanying the draw functions in hb_font_draw_glyph()
 * @st: current draw state
 * @control_x: X component of control point
 * @control_y: Y component of control point
 * @to_x: X component of target point
 * @to_y: Y component of target point
 * @user_data: User data pointer passed to hb_draw_funcs_set_quadratic_to_func()
 *
 * A virtual method for the #hb_draw_funcs_t to perform a "quadratic-to" draw
 * operation.
 *
 * Since: 4.0.0
 *
 **/
प्रकार व्योम (*hb_draw_quadratic_to_func_t) (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
					     hb_draw_state_t *st,
					     भग्न control_x, भग्न control_y,
					     भग्न to_x, भग्न to_y,
					     व्योम *user_तथ्य);

/**
 * hb_draw_cubic_to_func_t:
 * @dfuncs: draw functions object
 * @draw_data: The data accompanying the draw functions in hb_font_draw_glyph()
 * @st: current draw state
 * @control1_x: X component of first control point
 * @control1_y: Y component of first control point
 * @control2_x: X component of second control point
 * @control2_y: Y component of second control point
 * @to_x: X component of target point
 * @to_y: Y component of target point
 * @user_data: User data pointer passed to hb_draw_funcs_set_cubic_to_func()
 *
 * A virtual method for the #hb_draw_funcs_t to perform a "cubic-to" draw
 * operation.
 *
 * Since: 4.0.0
 *
 **/
प्रकार व्योम (*hb_draw_cubic_to_func_t) (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
					 hb_draw_state_t *st,
					 भग्न control1_x, भग्न control1_y,
					 भग्न control2_x, भग्न control2_y,
					 भग्न to_x, भग्न to_y,
					 व्योम *user_तथ्य);

/**
 * hb_draw_close_path_func_t:
 * @dfuncs: draw functions object
 * @draw_data: The data accompanying the draw functions in hb_font_draw_glyph()
 * @st: current draw state
 * @user_data: User data pointer passed to hb_draw_funcs_set_close_path_func()
 *
 * A virtual method for the #hb_draw_funcs_t to perform a "close-path" draw
 * operation.
 *
 * Since: 4.0.0
 *
 **/
प्रकार व्योम (*hb_draw_close_path_func_t) (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
					   hb_draw_state_t *st,
					   व्योम *user_तथ्य);

/**
 * hb_draw_set_budget_func_t:
 * @dfuncs: draw functions object
 * @draw_data: The data accompanying the draw functions
 * @budget: the new work-budget policy
 * @user_data: User data pointer passed to hb_draw_funcs_set_set_budget_func()
 *
 * Sets the work-budget policy and recharges the live work budget. @budget is
 * #HB_BUDGET_DEFAULT, #HB_BUDGET_UNLIMITED, or a non-negative concrete value.
 *
 * Return value: `true` if the budget was set, `false` if unsupported
 *
 * Since: 14.5.0
 **/
प्रकार hb_bool_t (*hb_draw_set_budget_func_t) (hb_draw_funcs_t *dfuncs,
						 व्योम *draw_तथ्य,
						 int64_t budget,
						 व्योम *user_तथ्य);

/**
 * hb_draw_get_budget_func_t:
 * @dfuncs: draw functions object
 * @draw_data: The data accompanying the draw functions
 * @user_data: User data pointer passed to hb_draw_funcs_set_get_budget_func()
 *
 * Fetches the configured work-budget policy.
 *
 * Return value: the configured work-budget policy
 *
 * Since: 14.5.0
 **/
प्रकार int64_t (*hb_draw_get_budget_func_t) (hb_draw_funcs_t *dfuncs,
					      व्योम *draw_तथ्य,
					      व्योम *user_तथ्य);

/**
 * hb_draw_get_budget_remaining_func_t:
 * @dfuncs: draw functions object
 * @draw_data: The data accompanying the draw functions
 * @user_data: User data pointer passed to
 *   hb_draw_funcs_set_get_budget_remaining_func()
 *
 * Fetches the address of the live work budget. The returned address must stay
 * valid while @draw_data is used with @dfuncs. The live value must be concrete;
 * it must not contain #HB_BUDGET_DEFAULT.
 *
 * Return value: (nullable) (transfer none): the live work budget, or `NULL` if
 *   live budget accounting is unsupported
 *
 * Since: 14.5.0
 **/
प्रकार int64_t *(*hb_draw_get_budget_remaining_func_t) (hb_draw_funcs_t *dfuncs,
							 व्योम *draw_तथ्य,
							 व्योम *user_तथ्य);

/**
 * hb_draw_funcs_set_move_to_func:
 * @dfuncs: draw functions object
 * @func: (closure user_data) (destroy destroy) (scope notified): move-to callback
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets move-to callback to the draw functions object.
 *
 * Since: 4.0.0
 **/
HB_EXTERN व्योम
hb_draw_funcs_set_move_to_func (hb_draw_funcs_t        *dfuncs,
				hb_draw_move_to_func_t  func,
				व्योम *user_तथ्य, hb_destroy_func_t destroy);

/**
 * hb_draw_funcs_set_line_to_func:
 * @dfuncs: draw functions object
 * @func: (closure user_data) (destroy destroy) (scope notified): line-to callback
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets line-to callback to the draw functions object.
 *
 * Since: 4.0.0
 **/
HB_EXTERN व्योम
hb_draw_funcs_set_पंक्ति_to_func (hb_draw_funcs_t        *dfuncs,
				hb_draw_पंक्ति_to_func_t  func,
				व्योम *user_तथ्य, hb_destroy_func_t destroy);

/**
 * hb_draw_funcs_set_quadratic_to_func:
 * @dfuncs: draw functions object
 * @func: (closure user_data) (destroy destroy) (scope notified): quadratic-to callback
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets quadratic-to callback to the draw functions object.
 *
 * Since: 4.0.0
 **/
HB_EXTERN व्योम
hb_draw_funcs_set_quadratic_to_func (hb_draw_funcs_t             *dfuncs,
				     hb_draw_quadratic_to_func_t  func,
				     व्योम *user_तथ्य, hb_destroy_func_t destroy);

/**
 * hb_draw_funcs_set_cubic_to_func:
 * @dfuncs: draw functions
 * @func: (closure user_data) (destroy destroy) (scope notified): cubic-to callback
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets cubic-to callback to the draw functions object.
 *
 * Since: 4.0.0
 **/
HB_EXTERN व्योम
hb_draw_funcs_set_cubic_to_func (hb_draw_funcs_t         *dfuncs,
				 hb_draw_cubic_to_func_t  func,
				 व्योम *user_तथ्य, hb_destroy_func_t destroy);

/**
 * hb_draw_funcs_set_close_path_func:
 * @dfuncs: draw functions object
 * @func: (closure user_data) (destroy destroy) (scope notified): close-path callback
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets close-path callback to the draw functions object.
 *
 * Since: 4.0.0
 **/
HB_EXTERN व्योम
hb_draw_funcs_set_close_path_func (hb_draw_funcs_t           *dfuncs,
				   hb_draw_close_path_func_t  func,
				   व्योम *user_तथ्य, hb_destroy_func_t destroy);

/**
 * hb_draw_funcs_set_set_budget_func:
 * @dfuncs: draw functions object
 * @func: (closure user_data) (destroy destroy) (scope notified): budget setter
 * @user_data: Data to pass to @func
 * @destroy: (nullable): callback to destroy @user_data
 *
 * Sets the budget setter callback.
 *
 * Since: 14.5.0
 **/
HB_EXTERN व्योम
hb_draw_funcs_set_set_budget_func (hb_draw_funcs_t         *dfuncs,
				   hb_draw_set_budget_func_t  func,
				   व्योम *user_तथ्य, hb_destroy_func_t destroy);

/**
 * hb_draw_funcs_set_get_budget_func:
 * @dfuncs: draw functions object
 * @func: (closure user_data) (destroy destroy) (scope notified): budget getter
 * @user_data: Data to pass to @func
 * @destroy: (nullable): callback to destroy @user_data
 *
 * Sets the budget-policy getter callback.
 *
 * Since: 14.5.0
 **/
HB_EXTERN व्योम
hb_draw_funcs_set_get_budget_func (hb_draw_funcs_t         *dfuncs,
				   hb_draw_get_budget_func_t  func,
				   व्योम *user_तथ्य, hb_destroy_func_t destroy);

/**
 * hb_draw_funcs_set_get_budget_remaining_func:
 * @dfuncs: draw functions object
 * @func: (closure user_data) (destroy destroy) (scope notified): live-budget getter
 * @user_data: Data to pass to @func
 * @destroy: (nullable): callback to destroy @user_data
 *
 * Sets the live-budget getter callback.
 *
 * Since: 14.5.0
 **/
HB_EXTERN व्योम
hb_draw_funcs_set_get_budget_remaining_func (hb_draw_funcs_t                   *dfuncs,
					     hb_draw_get_budget_remaining_func_t  func,
					     व्योम *user_तथ्य, hb_destroy_func_t destroy);


HB_EXTERN hb_draw_funcs_t *
hb_draw_funcs_create (व्योम);

HB_EXTERN hb_draw_funcs_t *
hb_draw_funcs_get_empty (व्योम);

HB_EXTERN hb_draw_funcs_t *
hb_draw_funcs_reference (hb_draw_funcs_t *dfuncs);

HB_EXTERN व्योम
hb_draw_funcs_destroy (hb_draw_funcs_t *dfuncs);

HB_EXTERN hb_bool_t
hb_draw_funcs_set_user_तथ्य (hb_draw_funcs_t *dfuncs,
			     hb_user_तथ्य_कुंजी_t *कुंजी_2,
			     व्योम *              तथ्य,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace);


HB_EXTERN व्योम *
hb_draw_funcs_get_user_तथ्य (स्थिर hb_draw_funcs_t *dfuncs,
			     hb_user_तथ्य_कुंजी_t       *कुंजी_2);

HB_EXTERN व्योम
hb_draw_funcs_make_immutable (hb_draw_funcs_t *dfuncs);

HB_EXTERN hb_bool_t
hb_draw_funcs_is_immutable (hb_draw_funcs_t *dfuncs);

HB_EXTERN hb_bool_t
hb_draw_set_budget (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
		    int64_t budget);

HB_EXTERN int64_t
hb_draw_get_budget (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य);

HB_EXTERN int64_t
hb_draw_get_budget_remaining (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य);


HB_EXTERN व्योम
hb_draw_move_to (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
		 hb_draw_state_t *st,
		 भग्न to_x, भग्न to_y);

HB_EXTERN व्योम
hb_draw_पंक्ति_to (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
		 hb_draw_state_t *st,
		 भग्न to_x, भग्न to_y);

HB_EXTERN व्योम
hb_draw_quadratic_to (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
		      hb_draw_state_t *st,
		      भग्न control_x, भग्न control_y,
		      भग्न to_x, भग्न to_y);

HB_EXTERN व्योम
hb_draw_cubic_to (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
		  hb_draw_state_t *st,
		  भग्न control1_x, भग्न control1_y,
		  भग्न control2_x, भग्न control2_y,
		  भग्न to_x, भग्न to_y);

HB_EXTERN व्योम
hb_draw_close_path (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
		    hb_draw_state_t *st);


/* Shape helpers.
 *
 * Emit common primitives (tapered line, rectangle, circle) into
 * any pen.  The helpers are thin wrappers over the individual
 * move_to / line_to / cubic_to / close_path calls: callers can
 * always hand-roll the same shapes if they need a variation.
 *
 * For rect / circle the @stroke_width parameter selects between
 * filled and stroked: a positive finite value is the stroke
 * width of the outline; NaN means "filled" (no stroke).
 */

/**
 * hb_draw_line_cap_t:
 * @HB_DRAW_LINE_CAP_BUTT:   No cap; the line ends exactly at
 *   its endpoint.
 * @HB_DRAW_LINE_CAP_SQUARE: Square cap; the line is extended
 *   past its endpoint by half the local stroke width.  Useful
 *   for composing closed shapes from line segments (e.g. a
 *   rectangle made from four lines).
 *
 * End-cap shape for hb_draw_line().
 *
 * Since: 14.2.0
 **/
प्रकार क्रमागत {
  HB_DRAW_पंक्ति_CAP_BUTT   = 0,
  HB_DRAW_पंक्ति_CAP_SQUARE = 1,
} hb_draw_पंक्ति_cap_t;

HB_EXTERN व्योम
hb_draw_पंक्ति (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
	      hb_draw_state_t *st,
	      भग्न x0, भग्न y0, भग्न w0,
	      भग्न x1, भग्न y1, भग्न w1,
	      hb_draw_पंक्ति_cap_t cap);

HB_EXTERN व्योम
hb_draw_rectangle (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
		   hb_draw_state_t *st,
		   भग्न x, भग्न y,
		   भग्न w, भग्न h,
	      भग्न stroke_width);

HB_EXTERN व्योम
hb_draw_circle (hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य,
		hb_draw_state_t *st,
		भग्न cx, भग्न cy,
		भग्न r,
		भग्न stroke_width);


HB_END_DECLS

#पूर्ण_अगर /* HB_DRAW_H */
