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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-geometry.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>


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

ਸਥਿਰ ਖਾਲੀ
hb_draw_move_ਮੰਜ਼ਿਲ_nil (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
		     hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y HB_ਅਣਵਰਤਿਆ,
		     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ) {}

ਸਥਿਰ ਖਾਲੀ
hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ_nil (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
		     hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y HB_ਅਣਵਰਤਿਆ,
		     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ) {}

ਸਥਿਰ ਖਾਲੀ
hb_draw_quadratic_ਮੰਜ਼ਿਲ_nil (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			  hb_draw_state_t *st,
			  ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
			  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  /* Compute in double precision, so that the result does not depend
   * on how the compiler evaluates float expressions. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_TWO_ਤੀਜਾ 0.66666666666666666666666667
  ਦੁੱਗਣਾ current_x = (ਦੁੱਗਣਾ) st->current_x;
  ਦੁੱਗਣਾ current_y = (ਦੁੱਗਣਾ) st->current_y;
  ਦੁੱਗਣਾ cx = (ਦੁੱਗਣਾ) control_x;
  ਦੁੱਗਣਾ cy = (ਦੁੱਗਣਾ) control_y;
  ਦੁੱਗਣਾ tx = (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_x;
  ਦੁੱਗਣਾ ty = (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_y;
  dfuncs->emit_cubic_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, *st,
			 (ਦਸ਼ਮਲਵ) (current_x + (cx - current_x) * HB_TWO_ਤੀਜਾ),
			 (ਦਸ਼ਮਲਵ) (current_y + (cy - current_y) * HB_TWO_ਤੀਜਾ),
			 (ਦਸ਼ਮਲਵ) (tx + (cx - tx) * HB_TWO_ਤੀਜਾ),
			 (ਦਸ਼ਮਲਵ) (ty + (cy - ty) * HB_TWO_ਤੀਜਾ),
			 ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_TWO_ਤੀਜਾ
}

ਸਥਿਰ ਖਾਲੀ
hb_draw_cubic_ਮੰਜ਼ਿਲ_nil (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
		      hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		      ਦਸ਼ਮਲਵ control1_x HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ control1_y HB_ਅਣਵਰਤਿਆ,
		      ਦਸ਼ਮਲਵ control2_x HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ control2_y HB_ਅਣਵਰਤਿਆ,
		      ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y HB_ਅਣਵਰਤਿਆ,
		      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ) {}

ਸਥਿਰ ਖਾਲੀ
hb_draw_close_ਰਸਤਾ_nil (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
			ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ) {}

ਸਥਿਰ hb_bool_t
hb_draw_ਸੈੱਟ_budget_nil (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			int64_t budget HB_ਅਣਵਰਤਿਆ,
			ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ) { ਵਾਪਸ ਝੂਠ; }

ਸਥਿਰ int64_t
hb_draw_get_budget_nil (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ) { ਵਾਪਸ HB_BUDGET_DEFAULT; }

ਸਥਿਰ int64_t *
hb_draw_get_budget_ਬਚਿਆ_nil (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ) { ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; }


ਸਥਿਰ ਤਾਰਕਿਕ
hb_draw_funcs_ਸੈੱਟ_preamble (hb_draw_funcs_t    *dfuncs,
			     ਤਾਰਕਿਕ                func_is_null,
			     ਖਾਲੀ              **user_ਅੰਕੜੇ,
			     hb_destroy_func_t  *destroy)
{
  ਜੇ (hb_object_is_immutable (dfuncs))
  {
    ਜੇ (*destroy)
      (*destroy) (*user_ਅੰਕੜੇ);
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (func_is_null)
  {
    ਜੇ (*destroy)
      (*destroy) (*user_ਅੰਕੜੇ);
    *destroy = ਖਾਲੀ_ਸੂਚਕ;
    *user_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
hb_draw_funcs_ਸੈੱਟ_ਵਿਚਕਾਰ (hb_draw_funcs_t   *dfuncs,
			   ਖਾਲੀ              *user_ਅੰਕੜੇ,
			   hb_destroy_func_t  destroy)
{
  ਸਵੈਚਾਲਿਤ destroy_guard = hb_make_scope_guard ([&]() {
    ਜੇ (destroy) destroy (user_ਅੰਕੜੇ);
  });

  ਜੇ (user_ਅੰਕੜੇ && !dfuncs->user_ਅੰਕੜੇ)
  {
    dfuncs->user_ਅੰਕੜੇ = (decltype (dfuncs->user_ਅੰਕੜੇ)) hb_calloc (1, ਆਕਾਰ (*dfuncs->user_ਅੰਕੜੇ));
    ਜੇ (unlikely (!dfuncs->user_ਅੰਕੜੇ))
      ਵਾਪਸ ਝੂਠ;
  }
  ਜੇ (destroy && !dfuncs->destroy)
  {
    dfuncs->destroy = (decltype (dfuncs->destroy)) hb_calloc (1, ਆਕਾਰ (*dfuncs->destroy));
    ਜੇ (unlikely (!dfuncs->destroy))
      ਵਾਪਸ ਝੂਠ;
  }

  destroy_guard.release ();
  ਵਾਪਸ ਸੱਚ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DRAW_FUNC_IMPLEMENT(name)						\
										\
ਖਾਲੀ										\
hb_draw_funcs_ਸੈੱਟ##name##_func (hb_draw_funcs_t	 *dfuncs,		\
				 hb_draw_##name##_func_t  func,			\
				 ਖਾਲੀ			 *user_ਅੰਕੜੇ,		\
				 hb_destroy_func_t	  destroy)		\
{										\
  ਜੇ (!hb_draw_funcs_ਸੈੱਟ_preamble (dfuncs, !func, &user_ਅੰਕੜੇ, &destroy))\
      ਵਾਪਸ;                                                            \
										\
  ਜੇ (dfuncs->destroy && dfuncs->destroy->name)					\
    dfuncs->destroy->name (!dfuncs->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : dfuncs->user_ਅੰਕੜੇ->name); \
									 \
  ਜੇ (!hb_draw_funcs_ਸੈੱਟ_ਵਿਚਕਾਰ (dfuncs, user_ਅੰਕੜੇ, destroy))           \
      ਵਾਪਸ;                                                            \
									\
  ਜੇ (func)								\
    dfuncs->func.name = func;						\
  ਨਹੀਂਤਾਂ									\
    dfuncs->func.name = hb_draw_##name##_nil;				\
									\
  ਜੇ (dfuncs->user_ਅੰਕੜੇ)						\
    dfuncs->user_ਅੰਕੜੇ->name = user_ਅੰਕੜੇ;				\
  ਜੇ (dfuncs->destroy)							\
    dfuncs->destroy->name = destroy;					\
}

HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_DRAW_FUNC_IMPLEMENT

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
  ਜੇ (unlikely (!(dfuncs = hb_object_create<hb_draw_funcs_t> ())))
    ਵਾਪਸ const_cast<hb_draw_funcs_t *> (&Null (hb_draw_funcs_t));

  dfuncs->func =  Null (hb_draw_funcs_t).func;

  ਵਾਪਸ dfuncs;
}

DEFINE_NULL_INSTANCE (hb_draw_funcs_t) =
{
  HB_OBJECT_ਸਿਰਲੇਖ_STATIC,

  {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_nil,
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_DRAW_FUNC_IMPLEMENT
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
hb_draw_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ ()
{
  ਵਾਪਸ const_cast<hb_draw_funcs_t *> (&Null (hb_draw_funcs_t));
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
  ਵਾਪਸ hb_object_reference (dfuncs);
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
ਖਾਲੀ
hb_draw_funcs_destroy (hb_draw_funcs_t *dfuncs)
{
  ਜੇ (!hb_object_destroy (dfuncs)) ਵਾਪਸ;

  ਜੇ (dfuncs->destroy)
  {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DRAW_FUNC_IMPLEMENT(name) \
    ਜੇ (dfuncs->destroy->name) dfuncs->destroy->name (!dfuncs->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : dfuncs->user_ਅੰਕੜੇ->name);
      HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_DRAW_FUNC_IMPLEMENT
  }

  hb_free (dfuncs->destroy);
  hb_free (dfuncs->user_ਅੰਕੜੇ);

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
hb_draw_funcs_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_draw_funcs_t *dfuncs,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			     ਖਾਲੀ *              ਅੰਕੜੇ,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (dfuncs, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_draw_funcs_get_user_ਅੰਕੜੇ (ਅਚਲ hb_draw_funcs_t *dfuncs,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t       *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (dfuncs, key);
}

/**
 * hb_draw_funcs_make_immutable:
 * @dfuncs: draw functions
 *
 * Makes @dfuncs object immutable.
 *
 * Since: 4.0.0
 **/
ਖਾਲੀ
hb_draw_funcs_make_immutable (hb_draw_funcs_t *dfuncs)
{
  ਜੇ (hb_object_is_immutable (dfuncs))
    ਵਾਪਸ;

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
  ਵਾਪਸ hb_object_is_immutable (dfuncs);
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
hb_draw_ਸੈੱਟ_budget (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ, int64_t budget)
{
  ਵਾਪਸ dfuncs->ਸੈੱਟ_budget (draw_ਅੰਕੜੇ, budget);
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
hb_draw_get_budget (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ)
{
  ਵਾਪਸ dfuncs->get_budget (draw_ਅੰਕੜੇ);
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
hb_draw_get_budget_ਬਚਿਆ (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ)
{
  int64_t *ਬਚਿਆ = dfuncs->get_budget_ਬਚਿਆ_ptr (draw_ਅੰਕੜੇ);
  ਜੇ (ਬਚਿਆ)
    ਵਾਪਸ *ਬਚਿਆ;

  int64_t budget = dfuncs->get_budget (draw_ਅੰਕੜੇ);
  ਵਾਪਸ budget == HB_BUDGET_DEFAULT ? HB_BUDGET_UNLIMITED : budget;
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
ਖਾਲੀ
hb_draw_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
		 hb_draw_state_t *st,
		 ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
{
  dfuncs->move_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, *st,
		   ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
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
ਖਾਲੀ
hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
		 hb_draw_state_t *st,
		 ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
{
  dfuncs->ਸਤਰ_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, *st,
		   ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
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
ਖਾਲੀ
hb_draw_quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
		      hb_draw_state_t *st,
		      ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
		      ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
{
  dfuncs->quadratic_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, *st,
			control_x, control_y,
			ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
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
ਖਾਲੀ
hb_draw_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
		  hb_draw_state_t *st,
		  ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
		  ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
		  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y)
{
  dfuncs->cubic_ਮੰਜ਼ਿਲ (draw_ਅੰਕੜੇ, *st,
		    control1_x, control1_y,
		    control2_x, control2_y,
		    ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
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
ਖਾਲੀ
hb_draw_close_ਰਸਤਾ (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
		    hb_draw_state_t *st)
{
  dfuncs->close_ਰਸਤਾ (draw_ਅੰਕੜੇ, *st);
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
ਖਾਲੀ
hb_draw_ਸਤਰ (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
	      hb_draw_state_t *st,
	      ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ w0,
	      ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ w1,
	      hb_draw_ਸਤਰ_cap_t cap)
{
  ਜੇ (std::isnan (w1)) w1 = w0;
  ਦਸ਼ਮਲਵ dx = x1 - x0, dy = y1 - y0;
  ਦਸ਼ਮਲਵ len = sqrtf (dx * dx + dy * dy);
  ਜੇ (len <= 0.f)
    ਵਾਪਸ;
  /* Unit tangent and normal to the line direction. */
  ਦਸ਼ਮਲਵ tx = dx / len;
  ਦਸ਼ਮਲਵ ty = dy / len;
  ਦਸ਼ਮਲਵ nx = -ty;
  ਦਸ਼ਮਲਵ ny =  tx;
  ਦਸ਼ਮਲਵ h0 = 0.5f * w0;
  ਦਸ਼ਮਲਵ h1 = 0.5f * w1;
  /* Square caps: extend each endpoint outward along the line
   * tangent by half its local stroke width. */
  ਜੇ (cap == HB_DRAW_ਸਤਰ_CAP_ਵਰਗਫਲ)
  {
    x0 -= tx * h0; y0 -= ty * h0;
    x1 += tx * h1; y1 += ty * h1;
  }
  /* Trapezoid corners (counter-clockwise). */
  ਦਸ਼ਮਲਵ ax = x0 + nx * h0, ay = y0 + ny * h0;
  ਦਸ਼ਮਲਵ bx = x1 + nx * h1, by = y1 + ny * h1;
  ਦਸ਼ਮਲਵ cx = x1 - nx * h1, cy = y1 - ny * h1;
  ਦਸ਼ਮਲਵ dx_ = x0 - nx * h0, dy_ = y0 - ny * h0;

  hb_draw_move_ਮੰਜ਼ਿਲ   (dfuncs, draw_ਅੰਕੜੇ, st, ax, ay);
  hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ   (dfuncs, draw_ਅੰਕੜੇ, st, bx, by);
  hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ   (dfuncs, draw_ਅੰਕੜੇ, st, cx, cy);
  hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ   (dfuncs, draw_ਅੰਕੜੇ, st, dx_, dy_);
  hb_draw_close_ਰਸਤਾ (dfuncs, draw_ਅੰਕੜੇ, st);
}

/* Emit an axis-aligned rectangle as a single closed contour.
 * @ccw picks the winding direction (useful for cutting a hole
 * out of another rectangle in a stroked rect). */
ਸਥਿਰ ਖਾਲੀ
_hb_draw_rect_contour (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
		       hb_draw_state_t *st,
		       ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਦਸ਼ਮਲਵ w, ਦਸ਼ਮਲਵ h,
		       ਤਾਰਕਿਕ ccw)
{
  hb_draw_move_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st, x, y);
  ਜੇ (ccw)
  {
    hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st, x + w, y);
    hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st, x + w, y + h);
    hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st, x,     y + h);
  }
  ਨਹੀਂਤਾਂ
  {
    hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st, x,     y + h);
    hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st, x + w, y + h);
    hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st, x + w, y);
  }
  hb_draw_close_ਰਸਤਾ (dfuncs, draw_ਅੰਕੜੇ, st);
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
ਖਾਲੀ
hb_draw_rectangle (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
		   hb_draw_state_t *st,
		   ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y,
		   ਦਸ਼ਮਲਵ w, ਦਸ਼ਮਲਵ h,
		   ਦਸ਼ਮਲਵ stroke_ਚੌੜਾਈ)
{
  ਜੇ (std::isnan (stroke_ਚੌੜਾਈ))
  {
    /* Filled rectangle with zero area is nothing to draw. */
    ਜੇ (w == 0.f || h == 0.f)
      ਵਾਪਸ;
    _hb_draw_rect_contour (dfuncs, draw_ਅੰਕੜੇ, st, x, y, w, h, /*ccw*/ ਸੱਚ);
    ਵਾਪਸ;
  }

  ਜੇ (stroke_ਚੌੜਾਈ <= 0.f || !std::isfinite (stroke_ਚੌੜਾਈ))
    ਵਾਪਸ;

  /* Normalize to non-negative width/height so the stroke math
   * below (outer grows by sw, inner shrinks by sw) produces the
   * expected outer-contains-inner ring regardless of w/h signs. */
  ਜੇ (w < 0.f) { x += w; w = -w; }
  ਜੇ (h < 0.f) { y += h; h = -h; }
  /* w or h == 0 is still meaningful when stroking: a stroked
   * zero-height rect is a horizontal line of length w; zero
   * width is a vertical line.  Both degenerate to a single
   * outer contour because the inner hole collapses. */

  /* Stroke is centered on the edge: outer contour grows by
   * stroke_width/2, inner contour shrinks by the same. */
  ਦਸ਼ਮਲਵ s = 0.5f * stroke_ਚੌੜਾਈ;
  /* Outer rectangle (CCW = adds coverage). */
  _hb_draw_rect_contour (dfuncs, draw_ਅੰਕੜੇ, st,
			 x - s, y - s,
			 w + stroke_ਚੌੜਾਈ, h + stroke_ਚੌੜਾਈ,
			 /*ccw*/ ਸੱਚ);
  /* Inner rectangle (CW = removes coverage for the hole). */
  ਦਸ਼ਮਲਵ iw = w - stroke_ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ ih = h - stroke_ਚੌੜਾਈ;
  ਜੇ (iw > 0.f && ih > 0.f)
    _hb_draw_rect_contour (dfuncs, draw_ਅੰਕੜੇ, st,
			   x + s, y + s, iw, ih,
			   /*ccw*/ ਝੂਠ);
}

/* Circle approximated by 4 cubic Beziers, one per quadrant.
 * The magic constant 0.5522847498307936 is
 *   (4/3) * (sqrt(2) - 1)
 * and minimizes the max radial error to ~2.7e-4 of r. */
ਸਥਿਰ ਖਾਲੀ
_hb_draw_circle_contour (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			 hb_draw_state_t *st,
			 ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy, ਦਸ਼ਮਲਵ r,
			 ਤਾਰਕਿਕ ccw)
{
  ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ k = 0.5522847498307936f;
  ਦਸ਼ਮਲਵ ck = r * k;

  hb_draw_move_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st, cx + r, cy);
  ਜੇ (ccw)
  {
    hb_draw_cubic_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st,
		      cx + r, cy + ck,
		      cx + ck, cy + r,
		      cx,      cy + r);
    hb_draw_cubic_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st,
		      cx - ck, cy + r,
		      cx - r,  cy + ck,
		      cx - r,  cy);
    hb_draw_cubic_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st,
		      cx - r,  cy - ck,
		      cx - ck, cy - r,
		      cx,      cy - r);
    hb_draw_cubic_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st,
		      cx + ck, cy - r,
		      cx + r,  cy - ck,
		      cx + r,  cy);
  }
  ਨਹੀਂਤਾਂ
  {
    hb_draw_cubic_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st,
		      cx + r, cy - ck,
		      cx + ck, cy - r,
		      cx,      cy - r);
    hb_draw_cubic_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st,
		      cx - ck, cy - r,
		      cx - r,  cy - ck,
		      cx - r,  cy);
    hb_draw_cubic_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st,
		      cx - r,  cy + ck,
		      cx - ck, cy + r,
		      cx,      cy + r);
    hb_draw_cubic_ਮੰਜ਼ਿਲ (dfuncs, draw_ਅੰਕੜੇ, st,
		      cx + ck, cy + r,
		      cx + r,  cy + ck,
		      cx + r,  cy);
  }
  hb_draw_close_ਰਸਤਾ (dfuncs, draw_ਅੰਕੜੇ, st);
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
ਖਾਲੀ
hb_draw_circle (hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
		hb_draw_state_t *st,
		ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy,
		ਦਸ਼ਮਲਵ r,
		ਦਸ਼ਮਲਵ stroke_ਚੌੜਾਈ)
{
  ਜੇ (r <= 0.f)
    ਵਾਪਸ;

  ਜੇ (std::isnan (stroke_ਚੌੜਾਈ))
  {
    _hb_draw_circle_contour (dfuncs, draw_ਅੰਕੜੇ, st, cx, cy, r, /*ccw*/ ਸੱਚ);
    ਵਾਪਸ;
  }

  ਜੇ (stroke_ਚੌੜਾਈ <= 0.f || !std::isfinite (stroke_ਚੌੜਾਈ))
    ਵਾਪਸ;

  ਦਸ਼ਮਲਵ s = 0.5f * stroke_ਚੌੜਾਈ;
  _hb_draw_circle_contour (dfuncs, draw_ਅੰਕੜੇ, st, cx, cy, r + s, /*ccw*/ ਸੱਚ);
  ਦਸ਼ਮਲਵ ir = r - s;
  ਜੇ (ir > 0.f)
    _hb_draw_circle_contour (dfuncs, draw_ਅੰਕੜੇ, st, cx, cy, ir, /*ccw*/ ਝੂਠ);
}


ਸਥਿਰ ਖਾਲੀ
hb_draw_extents_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			 ਖਾਲੀ *ਅੰਕੜੇ,
			 hb_draw_state_t *st,
			 ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) ਅੰਕੜੇ;

  extents->ਜੋੜੋ_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_draw_extents_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			 ਖਾਲੀ *ਅੰਕੜੇ,
			 hb_draw_state_t *st,
			 ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) ਅੰਕੜੇ;

  extents->ਜੋੜੋ_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_draw_extents_quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *ਅੰਕੜੇ,
			      hb_draw_state_t *st,
			      ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
			      ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) ਅੰਕੜੇ;

  extents->ਜੋੜੋ_ਬਿੰਦੂ (control_x, control_y);
  extents->ਜੋੜੋ_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_draw_extents_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *ਅੰਕੜੇ,
			  hb_draw_state_t *st,
			  ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
			  ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
			  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_extents_t<> *extents = (hb_extents_t<> *) ਅੰਕੜੇ;

  extents->ਜੋੜੋ_ਬਿੰਦੂ (control1_x, control1_y);
  extents->ਜੋੜੋ_ਬਿੰਦੂ (control2_x, control2_y);
  extents->ਜੋੜੋ_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_draw_extents_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_draw_extents_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_draw_extents_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func (funcs, hb_draw_extents_move_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func (funcs, hb_draw_extents_ਸਤਰ_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_quadratic_ਮੰਜ਼ਿਲ_func (funcs, hb_draw_extents_quadratic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func (funcs, hb_draw_extents_cubic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_draw_extents_funcs);

    ਵਾਪਸ funcs;
  }
} static_draw_extents_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_draw_extents_funcs ()
{
  static_draw_extents_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_draw_extents_get_funcs ()
{
  ਵਾਪਸ static_draw_extents_funcs.get_unconst ();
}


#ਜੇ_ਅੰਤ
