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

#ضمن "hb.hh"

#اگر_تعریف_نشده HB_NO_DRAW

#ضمن "hb-draw.hh"

#ضمن "hb-geometry.hh"

#ضمن "hb-machinery.hh"

#ضمن <cmath>


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

ساكن فراغ
hb_draw_move_مقصد_nil (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *draw_داده HB_استفادهنشده,
		     hb_draw_state_t *st HB_استفادهنشده,
		     عائم مقصد_x HB_استفادهنشده, عائم مقصد_y HB_استفادهنشده,
		     فراغ *user_داده HB_استفادهنشده) {}

ساكن فراغ
hb_draw_سطر_مقصد_nil (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *draw_داده HB_استفادهنشده,
		     hb_draw_state_t *st HB_استفادهنشده,
		     عائم مقصد_x HB_استفادهنشده, عائم مقصد_y HB_استفادهنشده,
		     فراغ *user_داده HB_استفادهنشده) {}

ساكن فراغ
hb_draw_quadratic_مقصد_nil (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
			  hb_draw_state_t *st,
			  عائم control_x, عائم control_y,
			  عائم مقصد_x, عائم مقصد_y,
			  فراغ *user_داده HB_استفادهنشده)
{
  /* Compute in double precision, so that the result does not depend
   * on how the compiler evaluates float expressions. */
#تعریف_کن HB_TWO_سوم 0.66666666666666666666666667
  مزدوج current_x = (مزدوج) st->current_x;
  مزدوج current_y = (مزدوج) st->current_y;
  مزدوج cx = (مزدوج) control_x;
  مزدوج cy = (مزدوج) control_y;
  مزدوج tx = (مزدوج) مقصد_x;
  مزدوج ty = (مزدوج) مقصد_y;
  dfuncs->emit_cubic_مقصد (draw_داده, *st,
			 (عائم) (current_x + (cx - current_x) * HB_TWO_سوم),
			 (عائم) (current_y + (cy - current_y) * HB_TWO_سوم),
			 (عائم) (tx + (cx - tx) * HB_TWO_سوم),
			 (عائم) (ty + (cy - ty) * HB_TWO_سوم),
			 مقصد_x, مقصد_y);
#لغو_تعریف HB_TWO_سوم
}

ساكن فراغ
hb_draw_cubic_مقصد_nil (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *draw_داده HB_استفادهنشده,
		      hb_draw_state_t *st HB_استفادهنشده,
		      عائم control1_x HB_استفادهنشده, عائم control1_y HB_استفادهنشده,
		      عائم control2_x HB_استفادهنشده, عائم control2_y HB_استفادهنشده,
		      عائم مقصد_x HB_استفادهنشده, عائم مقصد_y HB_استفادهنشده,
		      فراغ *user_داده HB_استفادهنشده) {}

ساكن فراغ
hb_draw_close_مسیر_nil (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *draw_داده HB_استفادهنشده,
			hb_draw_state_t *st HB_استفادهنشده,
			فراغ *user_داده HB_استفادهنشده) {}

ساكن hb_bool_t
hb_draw_مجموعه_budget_nil (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *draw_داده HB_استفادهنشده,
			int64_t budget HB_استفادهنشده,
			فراغ *user_داده HB_استفادهنشده) { أرجع خطأ; }

ساكن int64_t
hb_draw_get_budget_nil (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *draw_داده HB_استفادهنشده,
			فراغ *user_داده HB_استفادهنشده) { أرجع HB_BUDGET_DEFAULT; }

ساكن int64_t *
hb_draw_get_budget_مانده_nil (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *draw_داده HB_استفادهنشده,
				  فراغ *user_داده HB_استفادهنشده) { أرجع اشاره_تهی; }


ساكن منطقي
hb_draw_funcs_مجموعه_preamble (hb_draw_funcs_t    *dfuncs,
			     منطقي                func_is_null,
			     فراغ              **user_داده,
			     hb_destroy_func_t  *destroy)
{
  إذا (hb_object_is_immutable (dfuncs))
  {
    إذا (*destroy)
      (*destroy) (*user_داده);
    أرجع خطأ;
  }

  إذا (func_is_null)
  {
    إذا (*destroy)
      (*destroy) (*user_داده);
    *destroy = اشاره_تهی;
    *user_داده = اشاره_تهی;
  }

  أرجع درست;
}

ساكن منطقي
hb_draw_funcs_مجموعه_میانه (hb_draw_funcs_t   *dfuncs,
			   فراغ              *user_داده,
			   hb_destroy_func_t  destroy)
{
  خودکار destroy_guard = hb_make_scope_guard ([&]() {
    إذا (destroy) destroy (user_داده);
  });

  إذا (user_داده && !dfuncs->user_داده)
  {
    dfuncs->user_داده = (decltype (dfuncs->user_داده)) hb_calloc (1, حجم (*dfuncs->user_داده));
    إذا (unlikely (!dfuncs->user_داده))
      أرجع خطأ;
  }
  إذا (destroy && !dfuncs->destroy)
  {
    dfuncs->destroy = (decltype (dfuncs->destroy)) hb_calloc (1, حجم (*dfuncs->destroy));
    إذا (unlikely (!dfuncs->destroy))
      أرجع خطأ;
  }

  destroy_guard.release ();
  أرجع درست;
}

#تعریف_کن HB_DRAW_FUNC_IMPLEMENT(name)						\
										\
فراغ										\
hb_draw_funcs_مجموعه##name##_func (hb_draw_funcs_t	 *dfuncs,		\
				 hb_draw_##name##_func_t  func,			\
				 فراغ			 *user_داده,		\
				 hb_destroy_func_t	  destroy)		\
{										\
  إذا (!hb_draw_funcs_مجموعه_preamble (dfuncs, !func, &user_داده, &destroy))\
      أرجع;                                                            \
										\
  إذا (dfuncs->destroy && dfuncs->destroy->name)					\
    dfuncs->destroy->name (!dfuncs->user_داده ? اشاره_تهی : dfuncs->user_داده->name); \
									 \
  إذا (!hb_draw_funcs_مجموعه_میانه (dfuncs, user_داده, destroy))           \
      أرجع;                                                            \
									\
  إذا (func)								\
    dfuncs->func.name = func;						\
  وإلا									\
    dfuncs->func.name = hb_draw_##name##_nil;				\
									\
  إذا (dfuncs->user_داده)						\
    dfuncs->user_داده->name = user_داده;				\
  إذا (dfuncs->destroy)							\
    dfuncs->destroy->name = destroy;					\
}

HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_DRAW_FUNC_IMPLEMENT

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
  إذا (unlikely (!(dfuncs = hb_object_create<hb_draw_funcs_t> ())))
    أرجع const_cast<hb_draw_funcs_t *> (&Null (hb_draw_funcs_t));

  dfuncs->func =  Null (hb_draw_funcs_t).func;

  أرجع dfuncs;
}

DEFINE_NULL_INSTANCE (hb_draw_funcs_t) =
{
  HB_OBJECT_سرآیند_STATIC,

  {
#تعریف_کن HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_nil,
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_DRAW_FUNC_IMPLEMENT
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
hb_draw_funcs_get_خالی ()
{
  أرجع const_cast<hb_draw_funcs_t *> (&Null (hb_draw_funcs_t));
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
  أرجع hb_object_reference (dfuncs);
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
فراغ
hb_draw_funcs_destroy (hb_draw_funcs_t *dfuncs)
{
  إذا (!hb_object_destroy (dfuncs)) أرجع;

  إذا (dfuncs->destroy)
  {
#تعریف_کن HB_DRAW_FUNC_IMPLEMENT(name) \
    إذا (dfuncs->destroy->name) dfuncs->destroy->name (!dfuncs->user_داده ? اشاره_تهی : dfuncs->user_داده->name);
      HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_DRAW_FUNC_IMPLEMENT
  }

  hb_free (dfuncs->destroy);
  hb_free (dfuncs->user_داده);

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
hb_draw_funcs_مجموعه_user_داده (hb_draw_funcs_t *dfuncs,
			     hb_user_داده_کلید_t *key,
			     فراغ *              داده,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  أرجع hb_object_مجموعه_user_داده (dfuncs, key, داده, destroy, replace);
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
فراغ *
hb_draw_funcs_get_user_داده (ثابت hb_draw_funcs_t *dfuncs,
			     hb_user_داده_کلید_t       *key)
{
  أرجع hb_object_get_user_داده (dfuncs, key);
}

/**
 * hb_draw_funcs_make_immutable:
 * @dfuncs: draw functions
 *
 * Makes @dfuncs object immutable.
 *
 * Since: 4.0.0
 **/
فراغ
hb_draw_funcs_make_immutable (hb_draw_funcs_t *dfuncs)
{
  إذا (hb_object_is_immutable (dfuncs))
    أرجع;

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
  أرجع hb_object_is_immutable (dfuncs);
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
hb_draw_مجموعه_budget (hb_draw_funcs_t *dfuncs, فراغ *draw_داده, int64_t budget)
{
  أرجع dfuncs->مجموعه_budget (draw_داده, budget);
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
hb_draw_get_budget (hb_draw_funcs_t *dfuncs, فراغ *draw_داده)
{
  أرجع dfuncs->get_budget (draw_داده);
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
hb_draw_get_budget_مانده (hb_draw_funcs_t *dfuncs, فراغ *draw_داده)
{
  int64_t *مانده = dfuncs->get_budget_مانده_ptr (draw_داده);
  إذا (مانده)
    أرجع *مانده;

  int64_t budget = dfuncs->get_budget (draw_داده);
  أرجع budget == HB_BUDGET_DEFAULT ? HB_BUDGET_UNLIMITED : budget;
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
فراغ
hb_draw_move_مقصد (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
		 hb_draw_state_t *st,
		 عائم مقصد_x, عائم مقصد_y)
{
  dfuncs->move_مقصد (draw_داده, *st,
		   مقصد_x, مقصد_y);
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
فراغ
hb_draw_سطر_مقصد (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
		 hb_draw_state_t *st,
		 عائم مقصد_x, عائم مقصد_y)
{
  dfuncs->سطر_مقصد (draw_داده, *st,
		   مقصد_x, مقصد_y);
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
فراغ
hb_draw_quadratic_مقصد (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
		      hb_draw_state_t *st,
		      عائم control_x, عائم control_y,
		      عائم مقصد_x, عائم مقصد_y)
{
  dfuncs->quadratic_مقصد (draw_داده, *st,
			control_x, control_y,
			مقصد_x, مقصد_y);
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
فراغ
hb_draw_cubic_مقصد (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
		  hb_draw_state_t *st,
		  عائم control1_x, عائم control1_y,
		  عائم control2_x, عائم control2_y,
		  عائم مقصد_x, عائم مقصد_y)
{
  dfuncs->cubic_مقصد (draw_داده, *st,
		    control1_x, control1_y,
		    control2_x, control2_y,
		    مقصد_x, مقصد_y);
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
فراغ
hb_draw_close_مسیر (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
		    hb_draw_state_t *st)
{
  dfuncs->close_مسیر (draw_داده, *st);
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
فراغ
hb_draw_سطر (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
	      hb_draw_state_t *st,
	      عائم x0, عائم y0, عائم w0,
	      عائم x1, عائم y1, عائم w1,
	      hb_draw_سطر_cap_t cap)
{
  إذا (std::isnan (w1)) w1 = w0;
  عائم dx = x1 - x0, dy = y1 - y0;
  عائم len = sqrtf (dx * dx + dy * dy);
  إذا (len <= 0.f)
    أرجع;
  /* Unit tangent and normal to the line direction. */
  عائم tx = dx / len;
  عائم ty = dy / len;
  عائم nx = -ty;
  عائم ny =  tx;
  عائم h0 = 0.5f * w0;
  عائم h1 = 0.5f * w1;
  /* Square caps: extend each endpoint outward along the line
   * tangent by half its local stroke width. */
  إذا (cap == HB_DRAW_سطر_CAP_مربع)
  {
    x0 -= tx * h0; y0 -= ty * h0;
    x1 += tx * h1; y1 += ty * h1;
  }
  /* Trapezoid corners (counter-clockwise). */
  عائم ax = x0 + nx * h0, ay = y0 + ny * h0;
  عائم bx = x1 + nx * h1, by = y1 + ny * h1;
  عائم cx = x1 - nx * h1, cy = y1 - ny * h1;
  عائم dx_ = x0 - nx * h0, dy_ = y0 - ny * h0;

  hb_draw_move_مقصد   (dfuncs, draw_داده, st, ax, ay);
  hb_draw_سطر_مقصد   (dfuncs, draw_داده, st, bx, by);
  hb_draw_سطر_مقصد   (dfuncs, draw_داده, st, cx, cy);
  hb_draw_سطر_مقصد   (dfuncs, draw_داده, st, dx_, dy_);
  hb_draw_close_مسیر (dfuncs, draw_داده, st);
}

/* Emit an axis-aligned rectangle as a single closed contour.
 * @ccw picks the winding direction (useful for cutting a hole
 * out of another rectangle in a stroked rect). */
ساكن فراغ
_hb_draw_rect_contour (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
		       hb_draw_state_t *st,
		       عائم x, عائم y, عائم w, عائم h,
		       منطقي ccw)
{
  hb_draw_move_مقصد (dfuncs, draw_داده, st, x, y);
  إذا (ccw)
  {
    hb_draw_سطر_مقصد (dfuncs, draw_داده, st, x + w, y);
    hb_draw_سطر_مقصد (dfuncs, draw_داده, st, x + w, y + h);
    hb_draw_سطر_مقصد (dfuncs, draw_داده, st, x,     y + h);
  }
  وإلا
  {
    hb_draw_سطر_مقصد (dfuncs, draw_داده, st, x,     y + h);
    hb_draw_سطر_مقصد (dfuncs, draw_داده, st, x + w, y + h);
    hb_draw_سطر_مقصد (dfuncs, draw_داده, st, x + w, y);
  }
  hb_draw_close_مسیر (dfuncs, draw_داده, st);
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
فراغ
hb_draw_rectangle (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
		   hb_draw_state_t *st,
		   عائم x, عائم y,
		   عائم w, عائم h,
		   عائم stroke_عرض)
{
  إذا (std::isnan (stroke_عرض))
  {
    /* Filled rectangle with zero area is nothing to draw. */
    إذا (w == 0.f || h == 0.f)
      أرجع;
    _hb_draw_rect_contour (dfuncs, draw_داده, st, x, y, w, h, /*ccw*/ درست);
    أرجع;
  }

  إذا (stroke_عرض <= 0.f || !std::isfinite (stroke_عرض))
    أرجع;

  /* Normalize to non-negative width/height so the stroke math
   * below (outer grows by sw, inner shrinks by sw) produces the
   * expected outer-contains-inner ring regardless of w/h signs. */
  إذا (w < 0.f) { x += w; w = -w; }
  إذا (h < 0.f) { y += h; h = -h; }
  /* w or h == 0 is still meaningful when stroking: a stroked
   * zero-height rect is a horizontal line of length w; zero
   * width is a vertical line.  Both degenerate to a single
   * outer contour because the inner hole collapses. */

  /* Stroke is centered on the edge: outer contour grows by
   * stroke_width/2, inner contour shrinks by the same. */
  عائم s = 0.5f * stroke_عرض;
  /* Outer rectangle (CCW = adds coverage). */
  _hb_draw_rect_contour (dfuncs, draw_داده, st,
			 x - s, y - s,
			 w + stroke_عرض, h + stroke_عرض,
			 /*ccw*/ درست);
  /* Inner rectangle (CW = removes coverage for the hole). */
  عائم iw = w - stroke_عرض;
  عائم ih = h - stroke_عرض;
  إذا (iw > 0.f && ih > 0.f)
    _hb_draw_rect_contour (dfuncs, draw_داده, st,
			   x + s, y + s, iw, ih,
			   /*ccw*/ خطأ);
}

/* Circle approximated by 4 cubic Beziers, one per quadrant.
 * The magic constant 0.5522847498307936 is
 *   (4/3) * (sqrt(2) - 1)
 * and minimizes the max radial error to ~2.7e-4 of r. */
ساكن فراغ
_hb_draw_circle_contour (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
			 hb_draw_state_t *st,
			 عائم cx, عائم cy, عائم r,
			 منطقي ccw)
{
  ساكن ثابت عائم k = 0.5522847498307936f;
  عائم ck = r * k;

  hb_draw_move_مقصد (dfuncs, draw_داده, st, cx + r, cy);
  إذا (ccw)
  {
    hb_draw_cubic_مقصد (dfuncs, draw_داده, st,
		      cx + r, cy + ck,
		      cx + ck, cy + r,
		      cx,      cy + r);
    hb_draw_cubic_مقصد (dfuncs, draw_داده, st,
		      cx - ck, cy + r,
		      cx - r,  cy + ck,
		      cx - r,  cy);
    hb_draw_cubic_مقصد (dfuncs, draw_داده, st,
		      cx - r,  cy - ck,
		      cx - ck, cy - r,
		      cx,      cy - r);
    hb_draw_cubic_مقصد (dfuncs, draw_داده, st,
		      cx + ck, cy - r,
		      cx + r,  cy - ck,
		      cx + r,  cy);
  }
  وإلا
  {
    hb_draw_cubic_مقصد (dfuncs, draw_داده, st,
		      cx + r, cy - ck,
		      cx + ck, cy - r,
		      cx,      cy - r);
    hb_draw_cubic_مقصد (dfuncs, draw_داده, st,
		      cx - ck, cy - r,
		      cx - r,  cy - ck,
		      cx - r,  cy);
    hb_draw_cubic_مقصد (dfuncs, draw_داده, st,
		      cx - r,  cy + ck,
		      cx - ck, cy + r,
		      cx,      cy + r);
    hb_draw_cubic_مقصد (dfuncs, draw_داده, st,
		      cx + ck, cy + r,
		      cx + r,  cy + ck,
		      cx + r,  cy);
  }
  hb_draw_close_مسیر (dfuncs, draw_داده, st);
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
فراغ
hb_draw_circle (hb_draw_funcs_t *dfuncs, فراغ *draw_داده,
		hb_draw_state_t *st,
		عائم cx, عائم cy,
		عائم r,
		عائم stroke_عرض)
{
  إذا (r <= 0.f)
    أرجع;

  إذا (std::isnan (stroke_عرض))
  {
    _hb_draw_circle_contour (dfuncs, draw_داده, st, cx, cy, r, /*ccw*/ درست);
    أرجع;
  }

  إذا (stroke_عرض <= 0.f || !std::isfinite (stroke_عرض))
    أرجع;

  عائم s = 0.5f * stroke_عرض;
  _hb_draw_circle_contour (dfuncs, draw_داده, st, cx, cy, r + s, /*ccw*/ درست);
  عائم ir = r - s;
  إذا (ir > 0.f)
    _hb_draw_circle_contour (dfuncs, draw_داده, st, cx, cy, ir, /*ccw*/ خطأ);
}


ساكن فراغ
hb_draw_extents_move_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
			 فراغ *داده,
			 hb_draw_state_t *st,
			 عائم مقصد_x, عائم مقصد_y,
			 فراغ *user_داده HB_استفادهنشده)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) داده;

  extents->جمع_کن_نقطه (مقصد_x, مقصد_y);
}

ساكن فراغ
hb_draw_extents_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
			 فراغ *داده,
			 hb_draw_state_t *st,
			 عائم مقصد_x, عائم مقصد_y,
			 فراغ *user_داده HB_استفادهنشده)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) داده;

  extents->جمع_کن_نقطه (مقصد_x, مقصد_y);
}

ساكن فراغ
hb_draw_extents_quadratic_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
			      فراغ *داده,
			      hb_draw_state_t *st,
			      عائم control_x, عائم control_y,
			      عائم مقصد_x, عائم مقصد_y,
			      فراغ *user_داده HB_استفادهنشده)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) داده;

  extents->جمع_کن_نقطه (control_x, control_y);
  extents->جمع_کن_نقطه (مقصد_x, مقصد_y);
}

ساكن فراغ
hb_draw_extents_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
			  فراغ *داده,
			  hb_draw_state_t *st,
			  عائم control1_x, عائم control1_y,
			  عائم control2_x, عائم control2_y,
			  عائم مقصد_x, عائم مقصد_y,
			  فراغ *user_داده HB_استفادهنشده)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) داده;

  extents->جمع_کن_نقطه (control1_x, control1_y);
  extents->جمع_کن_نقطه (control2_x, control2_y);
  extents->جمع_کن_نقطه (مقصد_x, مقصد_y);
}

ساكن مضمن فراغ free_static_draw_extents_funcs ();

ساكن بنية hb_draw_extents_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_draw_extents_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_مجموعه_move_مقصد_func (funcs, hb_draw_extents_move_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_سطر_مقصد_func (funcs, hb_draw_extents_سطر_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_quadratic_مقصد_func (funcs, hb_draw_extents_quadratic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_cubic_مقصد_func (funcs, hb_draw_extents_cubic_مقصد, اشاره_تهی, اشاره_تهی);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_draw_extents_funcs);

    أرجع funcs;
  }
} static_draw_extents_funcs;

ساكن مضمن
فراغ free_static_draw_extents_funcs ()
{
  static_draw_extents_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_draw_extents_get_funcs ()
{
  أرجع static_draw_extents_funcs.get_unconst ();
}


#پایان_اگر
