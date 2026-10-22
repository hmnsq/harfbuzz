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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_DRAW

#अन्तर्भाव "hb-draw.hh"

#अन्तर्भाव "hb-geometry.hh"

#अन्तर्भाव "hb-machinery.hh"

#अन्तर्भाव <cmath>


/**
 * SECTION:hb-draw
 * @title: hb-draw
 * @short_description: Glyph drawing
 * @include: hb.h
 *
 * Functions for drawing (extracting) glyph shapes.
 *
 * The #hb_draw_funcs_t struct can be used with hb_font_draw_glyph().
 **/

स्थिर शून्य
hb_draw_move_to_nil (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data HB_UNUSED,
		     hb_draw_state_t *st HB_UNUSED,
		     दशांश to_x HB_UNUSED, दशांश to_y HB_UNUSED,
		     शून्य *user_data HB_UNUSED) {}

स्थिर शून्य
hb_draw_ओळ_to_nil (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data HB_UNUSED,
		     hb_draw_state_t *st HB_UNUSED,
		     दशांश to_x HB_UNUSED, दशांश to_y HB_UNUSED,
		     शून्य *user_data HB_UNUSED) {}

स्थिर शून्य
hb_draw_quadratic_to_nil (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
			  hb_draw_state_t *st,
			  दशांश control_x, दशांश control_y,
			  दशांश to_x, दशांश to_y,
			  शून्य *user_data HB_UNUSED)
{
  /* Compute in double precision, so that the result does not depend
   * on how the compiler evaluates float expressions. */
#define HB_TWO_THIRD 0.66666666666666666666666667
  द्विगुण current_x = (द्विगुण) st->current_x;
  द्विगुण current_y = (द्विगुण) st->current_y;
  द्विगुण cx = (द्विगुण) control_x;
  द्विगुण cy = (द्विगुण) control_y;
  द्विगुण tx = (द्विगुण) to_x;
  द्विगुण ty = (द्विगुण) to_y;
  dfuncs->emit_cubic_to (draw_data, *st,
			 (दशांश) (current_x + (cx - current_x) * HB_TWO_THIRD),
			 (दशांश) (current_y + (cy - current_y) * HB_TWO_THIRD),
			 (दशांश) (tx + (cx - tx) * HB_TWO_THIRD),
			 (दशांश) (ty + (cy - ty) * HB_TWO_THIRD),
			 to_x, to_y);
#undef HB_TWO_THIRD
}

स्थिर शून्य
hb_draw_cubic_to_nil (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data HB_UNUSED,
		      hb_draw_state_t *st HB_UNUSED,
		      दशांश control1_x HB_UNUSED, दशांश control1_y HB_UNUSED,
		      दशांश control2_x HB_UNUSED, दशांश control2_y HB_UNUSED,
		      दशांश to_x HB_UNUSED, दशांश to_y HB_UNUSED,
		      शून्य *user_data HB_UNUSED) {}

स्थिर शून्य
hb_draw_close_path_nil (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data HB_UNUSED,
			hb_draw_state_t *st HB_UNUSED,
			शून्य *user_data HB_UNUSED) {}

स्थिर hb_bool_t
hb_draw_set_budget_nil (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data HB_UNUSED,
			int64_t budget HB_UNUSED,
			शून्य *user_data HB_UNUSED) { परत असत्य; }

स्थिर int64_t
hb_draw_get_budget_nil (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data HB_UNUSED,
			शून्य *user_data HB_UNUSED) { परत HB_BUDGET_DEFAULT; }

स्थिर int64_t *
hb_draw_get_budget_remaining_nil (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data HB_UNUSED,
				  शून्य *user_data HB_UNUSED) { परत nullptr; }


स्थिर तार्किक
_hb_draw_funcs_set_preamble (hb_draw_funcs_t    *dfuncs,
			     तार्किक                func_is_null,
			     शून्य              **user_data,
			     hb_destroy_func_t  *destroy)
{
  जर (hb_object_is_immutable (dfuncs))
  {
    जर (*destroy)
      (*destroy) (*user_data);
    परत असत्य;
  }

  जर (func_is_null)
  {
    जर (*destroy)
      (*destroy) (*user_data);
    *destroy = nullptr;
    *user_data = nullptr;
  }

  परत सत्य;
}

स्थिर तार्किक
hb_draw_funcs_set_मधला (hb_draw_funcs_t   *dfuncs,
			   शून्य              *user_data,
			   hb_destroy_func_t  destroy)
{
  स्वतः destroy_guard = hb_make_scope_guard ([&]() {
    जर (destroy) destroy (user_data);
  });

  जर (user_data && !dfuncs->user_data)
  {
    dfuncs->user_data = (decltype (dfuncs->user_data)) hb_calloc (1, आकार (*dfuncs->user_data));
    जर (unlikely (!dfuncs->user_data))
      परत असत्य;
  }
  जर (destroy && !dfuncs->destroy)
  {
    dfuncs->destroy = (decltype (dfuncs->destroy)) hb_calloc (1, आकार (*dfuncs->destroy));
    जर (unlikely (!dfuncs->destroy))
      परत असत्य;
  }

  destroy_guard.release ();
  परत सत्य;
}

#define HB_DRAW_FUNC_IMPLEMENT(name)						\
										\
शून्य										\
hb_draw_funcs_set_##name##_func (hb_draw_funcs_t	 *dfuncs,		\
				 hb_draw_##name##_func_t  func,			\
				 शून्य			 *user_data,		\
				 hb_destroy_func_t	  destroy)		\
{										\
  जर (!_hb_draw_funcs_set_preamble (dfuncs, !func, &user_data, &destroy))\
      परत;                                                            \
										\
  जर (dfuncs->destroy && dfuncs->destroy->name)					\
    dfuncs->destroy->name (!dfuncs->user_data ? nullptr : dfuncs->user_data->name); \
									 \
  जर (!hb_draw_funcs_set_मधला (dfuncs, user_data, destroy))           \
      परत;                                                            \
									\
  जर (func)								\
    dfuncs->func.name = func;						\
  अन्यथा									\
    dfuncs->func.name = hb_draw_##name##_nil;				\
									\
  जर (dfuncs->user_data)						\
    dfuncs->user_data->name = user_data;				\
  जर (dfuncs->destroy)							\
    dfuncs->destroy->name = destroy;					\
}

HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT

/**
 * hb_draw_funcs_create:
 *
 * Creates a new draw callbacks object.
 *
 * Return value: (transfer full):
 * A newly allocated #hb_draw_funcs_t with a reference count of 1. The initial
 * reference count should be released with hb_draw_funcs_destroy when you are
 * done using the #hb_draw_funcs_t. This function never returns `NULL`. If
 * memory cannot be allocated, a special singleton #hb_draw_funcs_t object will
 * be returned.
 *
 * Since: 4.0.0
 **/
hb_draw_funcs_t *
hb_draw_funcs_create ()
{
  hb_draw_funcs_t *dfuncs;
  जर (unlikely (!(dfuncs = hb_object_create<hb_draw_funcs_t> ())))
    परत const_cast<hb_draw_funcs_t *> (&Null (hb_draw_funcs_t));

  dfuncs->func =  Null (hb_draw_funcs_t).func;

  परत dfuncs;
}

DEFINE_NULL_INSTANCE (hb_draw_funcs_t) =
{
  HB_OBJECT_HEADER_STATIC,

  {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_nil,
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  }
};

/**
 * hb_draw_funcs_get_empty:
 *
 * Fetches the singleton empty draw-functions structure.
 *
 * Return value: (transfer full): The empty draw-functions structure
 *
 * Since: 7.0.0
 **/
hb_draw_funcs_t *
hb_draw_funcs_get_empty ()
{
  परत const_cast<hb_draw_funcs_t *> (&Null (hb_draw_funcs_t));
}

/**
 * hb_draw_funcs_reference: (skip)
 * @dfuncs: draw functions
 *
 * Increases the reference count on @dfuncs by one.
 *
 * This prevents @dfuncs from being destroyed until a matching
 * call to hb_draw_funcs_destroy() is made.
 *
 * Return value: (transfer full):
 * The referenced #hb_draw_funcs_t.
 *
 * Since: 4.0.0
 **/
hb_draw_funcs_t *
hb_draw_funcs_reference (hb_draw_funcs_t *dfuncs)
{
  परत hb_object_reference (dfuncs);
}

/**
 * hb_draw_funcs_destroy: (skip)
 * @dfuncs: draw functions
 *
 * Deallocate the @dfuncs.
 * Decreases the reference count on @dfuncs by one. If the result is zero, then
 * @dfuncs and all associated resources are freed. See hb_draw_funcs_reference().
 *
 * Since: 4.0.0
 **/
शून्य
hb_draw_funcs_destroy (hb_draw_funcs_t *dfuncs)
{
  जर (!hb_object_destroy (dfuncs)) परत;

  जर (dfuncs->destroy)
  {
#define HB_DRAW_FUNC_IMPLEMENT(name) \
    जर (dfuncs->destroy->name) dfuncs->destroy->name (!dfuncs->user_data ? nullptr : dfuncs->user_data->name);
      HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  }

  hb_free (dfuncs->destroy);
  hb_free (dfuncs->user_data);

  hb_free (dfuncs);
}

/**
 * hb_draw_funcs_set_user_data: (skip)
 * @dfuncs: The draw-functions structure
 * @key: The user-data key
 * @data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified draw-functions structure.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 7.0.0
 **/
hb_bool_t
hb_draw_funcs_set_user_data (hb_draw_funcs_t *dfuncs,
			     hb_user_data_किल्ली_t *key,
			     शून्य *              data,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  परत hb_object_set_user_data (dfuncs, key, data, destroy, replace);
}

/**
 * hb_draw_funcs_get_user_data: (skip)
 * @dfuncs: The draw-functions structure
 * @key: The user-data key to query
 *
 * Fetches the user-data associated with the specified key,
 * attached to the specified draw-functions structure.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 7.0.0
 **/
शून्य *
hb_draw_funcs_get_user_data (स्थिरांक hb_draw_funcs_t *dfuncs,
			     hb_user_data_किल्ली_t       *key)
{
  परत hb_object_get_user_data (dfuncs, key);
}

/**
 * hb_draw_funcs_make_immutable:
 * @dfuncs: draw functions
 *
 * Makes @dfuncs object immutable.
 *
 * Since: 4.0.0
 **/
शून्य
hb_draw_funcs_make_immutable (hb_draw_funcs_t *dfuncs)
{
  जर (hb_object_is_immutable (dfuncs))
    परत;

  hb_object_make_immutable (dfuncs);
}

/**
 * hb_draw_funcs_is_immutable:
 * @dfuncs: draw functions
 *
 * Checks whether @dfuncs is immutable.
 *
 * Return value: `true` if @dfuncs is immutable, `false` otherwise
 *
 * Since: 4.0.0
 **/
hb_bool_t
hb_draw_funcs_is_immutable (hb_draw_funcs_t *dfuncs)
{
  परत hb_object_is_immutable (dfuncs);
}

/**
 * hb_draw_set_budget:
 * @dfuncs: draw functions object
 * @draw_data: associated draw data
 * @budget: the new work-budget policy
 *
 * Sets the work-budget policy and recharges the live work budget. Negative
 * values other than #HB_BUDGET_DEFAULT are clamped to zero.
 *
 * Return value: `true` if the draw functions support work budgets
 *
 * Since: 14.5.0
 **/
hb_bool_t
hb_draw_set_budget (hb_draw_funcs_t *dfuncs, शून्य *draw_data, int64_t budget)
{
  परत dfuncs->set_budget (draw_data, budget);
}

/**
 * hb_draw_get_budget:
 * @dfuncs: draw functions object
 * @draw_data: associated draw data
 *
 * Fetches the configured work-budget policy.
 *
 * Return value: the configured policy, or #HB_BUDGET_DEFAULT if unsupported
 *
 * Since: 14.5.0
 **/
int64_t
hb_draw_get_budget (hb_draw_funcs_t *dfuncs, शून्य *draw_data)
{
  परत dfuncs->get_budget (draw_data);
}

/**
 * hb_draw_get_budget_remaining:
 * @dfuncs: draw functions object
 * @draw_data: associated draw data
 *
 * Fetches the live work budget. A negative value means the budget was
 * exhausted. Zero means the preceding work fit exactly, but no positive-cost
 * work can start.
 *
 * Return value: the live work budget; if it is not tracked, the concrete
 *   configured policy or #HB_BUDGET_UNLIMITED
 *
 * Since: 14.5.0
 **/
int64_t
hb_draw_get_budget_remaining (hb_draw_funcs_t *dfuncs, शून्य *draw_data)
{
  int64_t *remaining = dfuncs->get_budget_remaining_ptr (draw_data);
  जर (remaining)
    परत *remaining;

  int64_t budget = dfuncs->get_budget (draw_data);
  परत budget == HB_BUDGET_DEFAULT ? HB_BUDGET_UNLIMITED : budget;
}


/**
 * hb_draw_move_to:
 * @dfuncs: draw functions
 * @draw_data: associated draw data passed by the caller
 * @st: current draw state
 * @to_x: X component of target point
 * @to_y: Y component of target point
 *
 * Perform a "move-to" draw operation.
 *
 * Since: 4.0.0
 **/
शून्य
hb_draw_move_to (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
		 hb_draw_state_t *st,
		 दशांश to_x, दशांश to_y)
{
  dfuncs->move_to (draw_data, *st,
		   to_x, to_y);
}

/**
 * hb_draw_line_to:
 * @dfuncs: draw functions
 * @draw_data: associated draw data passed by the caller
 * @st: current draw state
 * @to_x: X component of target point
 * @to_y: Y component of target point
 *
 * Perform a "line-to" draw operation.
 *
 * Since: 4.0.0
 **/
शून्य
hb_draw_ओळ_to (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
		 hb_draw_state_t *st,
		 दशांश to_x, दशांश to_y)
{
  dfuncs->ओळ_to (draw_data, *st,
		   to_x, to_y);
}

/**
 * hb_draw_quadratic_to:
 * @dfuncs: draw functions
 * @draw_data: associated draw data passed by the caller
 * @st: current draw state
 * @control_x: X component of control point
 * @control_y: Y component of control point
 * @to_x: X component of target point
 * @to_y: Y component of target point
 *
 * Perform a "quadratic-to" draw operation.
 *
 * Since: 4.0.0
 **/
शून्य
hb_draw_quadratic_to (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
		      hb_draw_state_t *st,
		      दशांश control_x, दशांश control_y,
		      दशांश to_x, दशांश to_y)
{
  dfuncs->quadratic_to (draw_data, *st,
			control_x, control_y,
			to_x, to_y);
}

/**
 * hb_draw_cubic_to:
 * @dfuncs: draw functions
 * @draw_data: associated draw data passed by the caller
 * @st: current draw state
 * @control1_x: X component of first control point
 * @control1_y: Y component of first control point
 * @control2_x: X component of second control point
 * @control2_y: Y component of second control point
 * @to_x: X component of target point
 * @to_y: Y component of target point
 *
 * Perform a "cubic-to" draw operation.
 *
 * Since: 4.0.0
 **/
शून्य
hb_draw_cubic_to (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
		  hb_draw_state_t *st,
		  दशांश control1_x, दशांश control1_y,
		  दशांश control2_x, दशांश control2_y,
		  दशांश to_x, दशांश to_y)
{
  dfuncs->cubic_to (draw_data, *st,
		    control1_x, control1_y,
		    control2_x, control2_y,
		    to_x, to_y);
}

/**
 * hb_draw_close_path:
 * @dfuncs: draw functions
 * @draw_data: associated draw data passed by the caller
 * @st: current draw state
 *
 * Perform a "close-path" draw operation.
 *
 * Since: 4.0.0
 **/
शून्य
hb_draw_close_path (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
		    hb_draw_state_t *st)
{
  dfuncs->close_path (draw_data, *st);
}


/**
 * hb_draw_line:
 * @dfuncs: draw functions
 * @draw_data: associated draw data passed by the caller
 * @st: current draw state
 * @x0: start X coordinate
 * @y0: start Y coordinate
 * @w0: stroke width at the start
 * @x1: end X coordinate
 * @y1: end Y coordinate
 * @w1: stroke width at the end
 * @cap: end-cap shape (butt or square)
 *
 * Emits a tapered line segment as a filled trapezoid.  @w0 and
 * @w1 are the full stroke widths at the start and end points
 * respectively; they may differ for a tapered stroke or match
 * for a uniform one.  Pass `NaN` for @w1 to use @w0 (uniform
 * stroke) without repeating the value.
 *
 * With #HB_DRAW_LINE_CAP_SQUARE each endpoint is extended along
 * the line direction by half its local stroke width, so four
 * `hb_draw_line()` calls form a closed rectangle without gaps
 * at the corners.
 *
 * Since: 14.2.0
 **/
शून्य
hb_draw_ओळ (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
	      hb_draw_state_t *st,
	      दशांश x0, दशांश y0, दशांश w0,
	      दशांश x1, दशांश y1, दशांश w1,
	      hb_draw_ओळ_cap_t cap)
{
  जर (std::isnan (w1)) w1 = w0;
  दशांश dx = x1 - x0, dy = y1 - y0;
  दशांश len = sqrtf (dx * dx + dy * dy);
  जर (len <= 0.f)
    परत;
  /* Unit tangent and normal to the line direction. */
  दशांश tx = dx / len;
  दशांश ty = dy / len;
  दशांश nx = -ty;
  दशांश ny =  tx;
  दशांश h0 = 0.5f * w0;
  दशांश h1 = 0.5f * w1;
  /* Square caps: extend each endpoint outward along the line
   * tangent by half its local stroke width. */
  जर (cap == HB_DRAW_ओळ_CAP_SQUARE)
  {
    x0 -= tx * h0; y0 -= ty * h0;
    x1 += tx * h1; y1 += ty * h1;
  }
  /* Trapezoid corners (counter-clockwise). */
  दशांश ax = x0 + nx * h0, ay = y0 + ny * h0;
  दशांश bx = x1 + nx * h1, by = y1 + ny * h1;
  दशांश cx = x1 - nx * h1, cy = y1 - ny * h1;
  दशांश dx_ = x0 - nx * h0, dy_ = y0 - ny * h0;

  hb_draw_move_to   (dfuncs, draw_data, st, ax, ay);
  hb_draw_ओळ_to   (dfuncs, draw_data, st, bx, by);
  hb_draw_ओळ_to   (dfuncs, draw_data, st, cx, cy);
  hb_draw_ओळ_to   (dfuncs, draw_data, st, dx_, dy_);
  hb_draw_close_path (dfuncs, draw_data, st);
}

/* Emit an axis-aligned rectangle as a single closed contour.
 * @ccw picks the winding direction (useful for cutting a hole
 * out of another rectangle in a stroked rect). */
स्थिर शून्य
_hb_draw_rect_contour (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
		       hb_draw_state_t *st,
		       दशांश x, दशांश y, दशांश w, दशांश h,
		       तार्किक ccw)
{
  hb_draw_move_to (dfuncs, draw_data, st, x, y);
  जर (ccw)
  {
    hb_draw_ओळ_to (dfuncs, draw_data, st, x + w, y);
    hb_draw_ओळ_to (dfuncs, draw_data, st, x + w, y + h);
    hb_draw_ओळ_to (dfuncs, draw_data, st, x,     y + h);
  }
  अन्यथा
  {
    hb_draw_ओळ_to (dfuncs, draw_data, st, x,     y + h);
    hb_draw_ओळ_to (dfuncs, draw_data, st, x + w, y + h);
    hb_draw_ओळ_to (dfuncs, draw_data, st, x + w, y);
  }
  hb_draw_close_path (dfuncs, draw_data, st);
}

/**
 * hb_draw_rectangle:
 * @dfuncs: draw functions
 * @draw_data: associated draw data passed by the caller
 * @st: current draw state
 * @x: top-left X coordinate
 * @y: top-left Y coordinate
 * @w: width (may be negative)
 * @h: height (may be negative)
 * @stroke_width: stroke width, or `NaN` for a filled rectangle
 *
 * Emits an axis-aligned rectangle.  If @stroke_width is a finite
 * positive value, the rectangle is rendered as an outlined ring
 * of that thickness centered on the edges; if @stroke_width is
 * `NaN`, the rectangle is rendered filled.
 *
 * Note: stroked rectangles produce a bounding box covering the
 * full outer rectangle, so if the pen is a GPU fragment-shader
 * backend, the shader runs for every interior pixel even though
 * only the outline contributes coverage.  For very thin
 * outlines where the interior is much larger than the stroke,
 * emitting four hb_draw_line() segments (one per edge) is
 * considerably cheaper per frame.
 *
 * Since: 14.2.0
 **/
शून्य
hb_draw_rectangle (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
		   hb_draw_state_t *st,
		   दशांश x, दशांश y,
		   दशांश w, दशांश h,
		   दशांश stroke_width)
{
  जर (std::isnan (stroke_width))
  {
    /* Filled rectangle with zero area is nothing to draw. */
    जर (w == 0.f || h == 0.f)
      परत;
    _hb_draw_rect_contour (dfuncs, draw_data, st, x, y, w, h, /*ccw*/ सत्य);
    परत;
  }

  जर (stroke_width <= 0.f || !std::है_सीमित (stroke_width))
    परत;

  /* Normalize to non-negative width/height so the stroke math
   * below (outer grows by sw, inner shrinks by sw) produces the
   * expected outer-contains-inner ring regardless of w/h signs. */
  जर (w < 0.f) { x += w; w = -w; }
  जर (h < 0.f) { y += h; h = -h; }
  /* w or h == 0 is still meaningful when stroking: a stroked
   * zero-height rect is a horizontal line of length w; zero
   * width is a vertical line.  Both degenerate to a single
   * outer contour because the inner hole collapses. */

  /* Stroke is centered on the edge: outer contour grows by
   * stroke_width/2, inner contour shrinks by the same. */
  दशांश s = 0.5f * stroke_width;
  /* Outer rectangle (CCW = adds coverage). */
  _hb_draw_rect_contour (dfuncs, draw_data, st,
			 x - s, y - s,
			 w + stroke_width, h + stroke_width,
			 /*ccw*/ सत्य);
  /* Inner rectangle (CW = removes coverage for the hole). */
  दशांश iw = w - stroke_width;
  दशांश ih = h - stroke_width;
  जर (iw > 0.f && ih > 0.f)
    _hb_draw_rect_contour (dfuncs, draw_data, st,
			   x + s, y + s, iw, ih,
			   /*ccw*/ असत्य);
}

/* Circle approximated by 4 cubic Beziers, one per quadrant.
 * The magic constant 0.5522847498307936 is
 *   (4/3) * (sqrt(2) - 1)
 * and minimizes the max radial error to ~2.7e-4 of r. */
स्थिर शून्य
_hb_draw_circle_contour (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
			 hb_draw_state_t *st,
			 दशांश cx, दशांश cy, दशांश r,
			 तार्किक ccw)
{
  स्थिर स्थिरांक दशांश k = 0.5522847498307936f;
  दशांश ck = r * k;

  hb_draw_move_to (dfuncs, draw_data, st, cx + r, cy);
  जर (ccw)
  {
    hb_draw_cubic_to (dfuncs, draw_data, st,
		      cx + r, cy + ck,
		      cx + ck, cy + r,
		      cx,      cy + r);
    hb_draw_cubic_to (dfuncs, draw_data, st,
		      cx - ck, cy + r,
		      cx - r,  cy + ck,
		      cx - r,  cy);
    hb_draw_cubic_to (dfuncs, draw_data, st,
		      cx - r,  cy - ck,
		      cx - ck, cy - r,
		      cx,      cy - r);
    hb_draw_cubic_to (dfuncs, draw_data, st,
		      cx + ck, cy - r,
		      cx + r,  cy - ck,
		      cx + r,  cy);
  }
  अन्यथा
  {
    hb_draw_cubic_to (dfuncs, draw_data, st,
		      cx + r, cy - ck,
		      cx + ck, cy - r,
		      cx,      cy - r);
    hb_draw_cubic_to (dfuncs, draw_data, st,
		      cx - ck, cy - r,
		      cx - r,  cy - ck,
		      cx - r,  cy);
    hb_draw_cubic_to (dfuncs, draw_data, st,
		      cx - r,  cy + ck,
		      cx - ck, cy + r,
		      cx,      cy + r);
    hb_draw_cubic_to (dfuncs, draw_data, st,
		      cx + ck, cy + r,
		      cx + r,  cy + ck,
		      cx + r,  cy);
  }
  hb_draw_close_path (dfuncs, draw_data, st);
}

/**
 * hb_draw_circle:
 * @dfuncs: draw functions
 * @draw_data: associated draw data passed by the caller
 * @st: current draw state
 * @cx: center X coordinate
 * @cy: center Y coordinate
 * @r: radius
 * @stroke_width: stroke width, or `NaN` for a filled disc
 *
 * Emits a circle approximated by four cubic Bezier curves.  If
 * @stroke_width is a finite positive value, the circle is
 * rendered as an outlined ring of that thickness centered on
 * the nominal radius; if @stroke_width is `NaN`, the circle is
 * rendered as a filled disc.
 *
 * Since: 14.2.0
 **/
शून्य
hb_draw_circle (hb_draw_funcs_t *dfuncs, शून्य *draw_data,
		hb_draw_state_t *st,
		दशांश cx, दशांश cy,
		दशांश r,
		दशांश stroke_width)
{
  जर (r <= 0.f)
    परत;

  जर (std::isnan (stroke_width))
  {
    _hb_draw_circle_contour (dfuncs, draw_data, st, cx, cy, r, /*ccw*/ सत्य);
    परत;
  }

  जर (stroke_width <= 0.f || !std::है_सीमित (stroke_width))
    परत;

  दशांश s = 0.5f * stroke_width;
  _hb_draw_circle_contour (dfuncs, draw_data, st, cx, cy, r + s, /*ccw*/ सत्य);
  दशांश ir = r - s;
  जर (ir > 0.f)
    _hb_draw_circle_contour (dfuncs, draw_data, st, cx, cy, ir, /*ccw*/ असत्य);
}


स्थिर शून्य
hb_draw_extents_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			 शून्य *data,
			 hb_draw_state_t *st,
			 दशांश to_x, दशांश to_y,
			 शून्य *user_data HB_UNUSED)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) data;

  extents->add_point (to_x, to_y);
}

स्थिर शून्य
hb_draw_extents_ओळ_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			 शून्य *data,
			 hb_draw_state_t *st,
			 दशांश to_x, दशांश to_y,
			 शून्य *user_data HB_UNUSED)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) data;

  extents->add_point (to_x, to_y);
}

स्थिर शून्य
hb_draw_extents_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			      शून्य *data,
			      hb_draw_state_t *st,
			      दशांश control_x, दशांश control_y,
			      दशांश to_x, दशांश to_y,
			      शून्य *user_data HB_UNUSED)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) data;

  extents->add_point (control_x, control_y);
  extents->add_point (to_x, to_y);
}

स्थिर शून्य
hb_draw_extents_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  शून्य *data,
			  hb_draw_state_t *st,
			  दशांश control1_x, दशांश control1_y,
			  दशांश control2_x, दशांश control2_y,
			  दशांश to_x, दशांश to_y,
			  शून्य *user_data HB_UNUSED)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) data;

  extents->add_point (control1_x, control1_y);
  extents->add_point (control2_x, control2_y);
  extents->add_point (to_x, to_y);
}

स्थिर ओळीत शून्य free_static_draw_extents_funcs ();

स्थिर संरचना hb_draw_extents_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_draw_extents_funcs_lazy_loader_t>
{
  स्थिर hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_draw_extents_move_to, nullptr, nullptr);
    hb_draw_funcs_set_ओळ_to_func (funcs, hb_draw_extents_ओळ_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_draw_extents_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_draw_extents_cubic_to, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_draw_extents_funcs);

    परत funcs;
  }
} static_draw_extents_funcs;

स्थिर ओळीत
शून्य free_static_draw_extents_funcs ()
{
  static_draw_extents_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_draw_extents_get_funcs ()
{
  परत static_draw_extents_funcs.get_unconst ();
}


#endif
