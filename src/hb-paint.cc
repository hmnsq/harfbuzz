/*
 * Copyright © 2022 Matthias Clasen
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.hh"

/**
 * SECTION: hb-paint
 * @title: hb-paint
 * @short_description: Glyph painting
 * @include: hb.h
 *
 * Functions for painting glyphs.
 *
 * The main purpose of these functions is to paint (extract) color glyph layers
 * from the COLRv1 table, but the API works for drawing ordinary outlines and
 * images as well.
 *
 * The #hb_paint_funcs_t struct can be used with hb_font_paint_glyph().
 **/

ਸਥਿਰ ਖਾਲੀ
hb_paint_ਧੱਕੋ_transform_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                             ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
                             ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
                             ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy,
                             ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ ਖਾਲੀ
hb_paint_ਕੱਢੋ_transform_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                            ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ hb_bool_t
hb_paint_color_glyph_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                          hb_codepoint_t glyph,
                          hb_font_t *font,
                          ਖਾਲੀ *user_ਅੰਕੜੇ) { ਵਾਪਸ ਝੂਠ; }

ਸਥਿਰ ਖਾਲੀ
hb_paint_fill_glyph_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                         hb_codepoint_t glyph,
                         hb_font_t *font,
                         hb_bool_t is_foreground,
                         hb_color_t color,
                         ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  /* Decompose into the equivalent general sequence. */
  funcs->ਧੱਕੋ_clip_glyph (paint_ਅੰਕੜੇ, glyph, font);
  funcs->color (paint_ਅੰਕੜੇ, is_foreground, color);
  funcs->ਕੱਢੋ_clip (paint_ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_ਧੱਕੋ_clip_glyph_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                              hb_codepoint_t glyph,
                              hb_font_t *font,
                              ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ ਖਾਲੀ
hb_paint_ਧੱਕੋ_clip_rectangle_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                  ਦਸ਼ਮਲਵ xmin, ਦਸ਼ਮਲਵ ymin, ਦਸ਼ਮਲਵ xmax, ਦਸ਼ਮਲਵ ymax,
                                  ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ hb_draw_funcs_t *
hb_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                   ਖਾਲੀ **draw_ਅੰਕੜੇ,
                                   ਖਾਲੀ *user_ਅੰਕੜੇ) { ਜੇ (draw_ਅੰਕੜੇ) *draw_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ; ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; }

ਸਥਿਰ ਖਾਲੀ
hb_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                 ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ ਖਾਲੀ
hb_paint_ਕੱਢੋ_clip_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                       ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ ਖਾਲੀ
hb_paint_color_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                    hb_bool_t is_foreground,
                    hb_color_t color,
                    ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ hb_bool_t
hb_paint_image_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                    hb_blob_t *image,
                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੌੜਾਈ,
                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉਚਾਈ,
                    hb_ਟੈਗ_t format,
                    ਦਸ਼ਮਲਵ slant_xy_deprecated,
                    hb_glyph_extents_t *extents,
                    ਖਾਲੀ *user_ਅੰਕੜੇ) { ਵਾਪਸ ਝੂਠ; }

ਸਥਿਰ ਖਾਲੀ
hb_paint_ਰੇਖਿਕ_gradient_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                              hb_color_ਸਤਰ_t *color_ਸਤਰ,
                              ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
                              ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
                              ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
                              ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ ਖਾਲੀ
hb_paint_radial_gradient_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                              hb_color_ਸਤਰ_t *color_ਸਤਰ,
                              ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
                              ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1,
                              ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ ਖਾਲੀ
hb_paint_sweep_gradient_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                             hb_color_ਸਤਰ_t *color_ਸਤਰ,
                             ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
                             ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle,
                             ਦਸ਼ਮਲਵ ਸਿਰਾ_angle,
                             ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ ਖਾਲੀ
hb_paint_ਧੱਕੋ_ਸਮੂਹ_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                         ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ ਖਾਲੀ
hb_paint_ਧੱਕੋ_ਸਮੂਹ_for_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                             hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ,
                             ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_paint_ਧੱਕੋ_ਸਮੂਹ (funcs, paint_ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_ਕੱਢੋ_ਸਮੂਹ_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                        hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ,
                        ਖਾਲੀ *user_ਅੰਕੜੇ) {}

ਸਥਿਰ hb_bool_t
hb_paint_custom_palette_color_nil (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ color_ਸੂਚਕਾਂਕ,
                                   hb_color_t *color,
                                   ਖਾਲੀ *user_ਅੰਕੜੇ) { ਵਾਪਸ ਝੂਠ; }

ਸਥਿਰ hb_bool_t
hb_paint_ਸੈੱਟ_budget_nil (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *paint_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			 int64_t budget HB_ਅਣਵਰਤਿਆ,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ) { ਵਾਪਸ ਝੂਠ; }

ਸਥਿਰ int64_t
hb_paint_get_budget_nil (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *paint_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ) { ਵਾਪਸ HB_BUDGET_DEFAULT; }

ਸਥਿਰ int64_t *
hb_paint_get_budget_ਬਚਿਆ_nil (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *paint_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ) { ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; }

ਸਥਿਰ ਤਾਰਕਿਕ
hb_paint_funcs_ਸੈੱਟ_preamble (hb_paint_funcs_t  *funcs,
                             ਤਾਰਕਿਕ                func_is_null,
                             ਖਾਲੀ              **user_ਅੰਕੜੇ,
                             hb_destroy_func_t  *destroy)
{
  ਜੇ (hb_object_is_immutable (funcs))
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
hb_paint_funcs_ਸੈੱਟ_ਵਿਚਕਾਰ (hb_paint_funcs_t  *funcs,
                            ਖਾਲੀ              *user_ਅੰਕੜੇ,
                            hb_destroy_func_t  destroy)
{
  ਸਵੈਚਾਲਿਤ destroy_guard = hb_make_scope_guard ([&]() {
    ਜੇ (destroy) destroy (user_ਅੰਕੜੇ);
  });

  ਜੇ (user_ਅੰਕੜੇ && !funcs->user_ਅੰਕੜੇ)
  {
    funcs->user_ਅੰਕੜੇ = (decltype (funcs->user_ਅੰਕੜੇ)) hb_calloc (1, ਆਕਾਰ (*funcs->user_ਅੰਕੜੇ));
    ਜੇ (unlikely (!funcs->user_ਅੰਕੜੇ))
      ਵਾਪਸ ਝੂਠ;
  }
  ਜੇ (destroy && !funcs->destroy)
  {
    funcs->destroy = (decltype (funcs->destroy)) hb_calloc (1, ਆਕਾਰ (*funcs->destroy));
    ਜੇ (unlikely (!funcs->destroy))
      ਵਾਪਸ ਝੂਠ;
  }

  destroy_guard.release ();
  ਵਾਪਸ ਸੱਚ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_FUNC_IMPLEMENT(name)                                           \
                                                                                \
ਖਾਲੀ                                                                            \
hb_paint_funcs_ਸੈੱਟ##name##_func (hb_paint_funcs_t         *funcs,              \
                                  hb_paint_##name##_func_t  func,               \
                                  ਖਾਲੀ                     *user_ਅੰਕੜੇ,          \
                                  hb_destroy_func_t         destroy)            \
{                                                                               \
  ਜੇ (!hb_paint_funcs_ਸੈੱਟ_preamble (funcs, !func, &user_ਅੰਕੜੇ, &destroy))       \
      ਵਾਪਸ;                                                                   \
                                                                                \
  ਜੇ (funcs->destroy && funcs->destroy->name)                                   \
    funcs->destroy->name (!funcs->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : funcs->user_ਅੰਕੜੇ->name);\
                                                                                \
  ਜੇ (!hb_paint_funcs_ਸੈੱਟ_ਵਿਚਕਾਰ (funcs, user_ਅੰਕੜੇ, destroy))                  \
      ਵਾਪਸ;                                                                   \
                                                                                \
  ਜੇ (func)                                                                     \
    funcs->func.name = func;                                                    \
  ਨਹੀਂਤਾਂ                                                                          \
    funcs->func.name = hb_paint_##name##_nil;                                   \
                                                                                \
  ਜੇ (funcs->user_ਅੰਕੜੇ)                                                         \
    funcs->user_ਅੰਕੜੇ->name = user_ਅੰਕੜੇ;                                         \
  ਜੇ (funcs->destroy)                                                           \
    funcs->destroy->name = destroy;                                             \
}

HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_PAINT_FUNC_IMPLEMENT

/**
 * hb_paint_funcs_create:
 *
 * Creates a new #hb_paint_funcs_t structure of paint functions.
 *
 * The initial reference count of 1 should be released with hb_paint_funcs_destroy()
 * when you are done using the #hb_paint_funcs_t. This function never returns
 * `NULL`. If memory cannot be allocated, a special singleton #hb_paint_funcs_t
 * object will be returned.
 *
 * Returns value: (transfer full): the paint-functions structure
 *
 * Since: 7.0.0
 */
hb_paint_funcs_t *
hb_paint_funcs_create ()
{
  hb_paint_funcs_t *funcs;
  ਜੇ (unlikely (!(funcs = hb_object_create<hb_paint_funcs_t> ())))
    ਵਾਪਸ const_cast<hb_paint_funcs_t *> (&Null (hb_paint_funcs_t));

  funcs->func =  Null (hb_paint_funcs_t).func;

  ਵਾਪਸ funcs;
}

DEFINE_NULL_INSTANCE (hb_paint_funcs_t) =
{
  HB_OBJECT_ਸਿਰਲੇਖ_STATIC,

  {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_nil,
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_PAINT_FUNC_IMPLEMENT
  }
};

/**
 * hb_paint_funcs_get_empty:
 *
 * Fetches the singleton empty paint-functions structure.
 *
 * Return value: (transfer full): The empty paint-functions structure
 *
 * Since: 7.0.0
 **/
hb_paint_funcs_t *
hb_paint_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ ()
{
  ਵਾਪਸ const_cast<hb_paint_funcs_t *> (&Null (hb_paint_funcs_t));
}

/**
 * hb_paint_funcs_reference: (skip)
 * @funcs: The paint-functions structure
 *
 * Increases the reference count on a paint-functions structure.
 *
 * This prevents @funcs from being destroyed until a matching
 * call to hb_paint_funcs_destroy() is made.
 *
 * Return value: The paint-functions structure
 *
 * Since: 7.0.0
 */
hb_paint_funcs_t *
hb_paint_funcs_reference (hb_paint_funcs_t *funcs)
{
  ਵਾਪਸ hb_object_reference (funcs);
}

/**
 * hb_paint_funcs_destroy: (skip)
 * @funcs: The paint-functions structure
 *
 * Decreases the reference count on a paint-functions structure.
 *
 * When the reference count reaches zero, the structure
 * is destroyed, freeing all memory.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_funcs_destroy (hb_paint_funcs_t *funcs)
{
  ਜੇ (!hb_object_destroy (funcs)) ਵਾਪਸ;

  ਜੇ (funcs->destroy)
  {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_FUNC_IMPLEMENT(name) \
    ਜੇ (funcs->destroy->name) funcs->destroy->name (!funcs->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : funcs->user_ਅੰਕੜੇ->name);
      HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_PAINT_FUNC_IMPLEMENT
  }

  hb_free (funcs->destroy);
  hb_free (funcs->user_ਅੰਕੜੇ);
  hb_free (funcs);
}

/**
 * hb_paint_funcs_set_user_data: (skip)
 * @funcs: The paint-functions structure
 * @key: The user-data key
 * @data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified paint-functions structure.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 7.0.0
 **/
hb_bool_t
hb_paint_funcs_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_paint_funcs_t *funcs,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			     ਖਾਲੀ *              ਅੰਕੜੇ,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (funcs, key, ਅੰਕੜੇ, destroy, replace);
}

/**
 * hb_paint_funcs_get_user_data: (skip)
 * @funcs: The paint-functions structure
 * @key: The user-data key to query
 *
 * Fetches the user-data associated with the specified key,
 * attached to the specified paint-functions structure.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 7.0.0
 **/
ਖਾਲੀ *
hb_paint_funcs_get_user_ਅੰਕੜੇ (ਅਚਲ hb_paint_funcs_t *funcs,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t       *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (funcs, key);
}

/**
 * hb_paint_funcs_make_immutable:
 * @funcs: The paint-functions structure
 *
 * Makes a paint-functions structure immutable.
 *
 * After this call, all attempts to set one of the callbacks
 * on @funcs will fail.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_funcs_make_immutable (hb_paint_funcs_t *funcs)
{
  ਜੇ (hb_object_is_immutable (funcs))
    ਵਾਪਸ;

  hb_object_make_immutable (funcs);
}

/**
 * hb_paint_funcs_is_immutable:
 * @funcs: The paint-functions structure
 *
 * Tests whether a paint-functions structure is immutable.
 *
 * Return value: `true` if @funcs is immutable, `false` otherwise
 *
 * Since: 7.0.0
 */
hb_bool_t
hb_paint_funcs_is_immutable (hb_paint_funcs_t *funcs)
{
  ਵਾਪਸ hb_object_is_immutable (funcs);
}

/**
 * hb_paint_set_budget:
 * @funcs: paint functions object
 * @paint_data: associated paint data
 * @budget: the new work-budget policy
 *
 * Sets the work-budget policy and recharges the live work budget. Negative
 * values other than #HB_BUDGET_DEFAULT are clamped to zero.
 *
 * Return value: `true` if the paint functions support work budgets
 *
 * Since: 14.5.0
 **/
hb_bool_t
hb_paint_ਸੈੱਟ_budget (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ, int64_t budget)
{
  ਵਾਪਸ funcs->ਸੈੱਟ_budget (paint_ਅੰਕੜੇ, budget);
}

/**
 * hb_paint_get_budget:
 * @funcs: paint functions object
 * @paint_data: associated paint data
 *
 * Fetches the configured work-budget policy.
 *
 * Return value: the configured policy, or #HB_BUDGET_DEFAULT if unsupported
 *
 * Since: 14.5.0
 **/
int64_t
hb_paint_get_budget (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ)
{
  ਵਾਪਸ funcs->get_budget (paint_ਅੰਕੜੇ);
}

/**
 * hb_paint_get_budget_remaining:
 * @funcs: paint functions object
 * @paint_data: associated paint data
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
hb_paint_get_budget_ਬਚਿਆ (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ)
{
  int64_t *ਬਚਿਆ = funcs->get_budget_ਬਚਿਆ_ptr (paint_ਅੰਕੜੇ);
  ਜੇ (ਬਚਿਆ)
    ਵਾਪਸ *ਬਚਿਆ;

  int64_t budget = funcs->get_budget (paint_ਅੰਕੜੇ);
  ਵਾਪਸ budget == HB_BUDGET_DEFAULT ? HB_BUDGET_UNLIMITED : budget;
}


/**
 * hb_color_line_get_color_stops:
 * @color_line: a #hb_color_line_t object
 * @start: the index of the first color stop to return
 * @count: (inout) (optional): Input = the maximum number of feature tags to return;
 *     Output = the actual number of feature tags returned (may be zero)
 * @color_stops: (out) (array length=count) (optional): Array of #hb_color_stop_t to populate
 *
 * Fetches a list of color stops from the given color line object.
 *
 * Note that due to variations being applied, the returned color stops
 * may be out of order. It is the callers responsibility to ensure that
 * color stops are sorted by their offset before they are used.
 *
 * Return value: the total number of color stops in @color_line
 *
 * Since: 7.0.0
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_color_ਸਤਰ_get_color_stops (hb_color_ਸਤਰ_t *color_ਸਤਰ,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਗਿਣਤੀ_2,
                               hb_color_stop_t *color_stops)
{
  ਵਾਪਸ color_ਸਤਰ->get_color_stops (color_ਸਤਰ,
				      color_ਸਤਰ->ਅੰਕੜੇ,
				      ਸ਼ੁਰੂ, ਗਿਣਤੀ_2,
				      color_stops,
				      color_ਸਤਰ->get_color_stops_user_ਅੰਕੜੇ);
}

/**
 * hb_color_line_get_extend:
 * @color_line: a #hb_color_line_t object
 *
 * Fetches the extend mode of the color line object.
 *
 * Return value: the extend mode of @color_line
 *
 * Since: 7.0.0
 */
hb_paint_extend_t
hb_color_ਸਤਰ_get_extend (hb_color_ਸਤਰ_t *color_ਸਤਰ)
{
  ਵਾਪਸ color_ਸਤਰ->get_extend (color_ਸਤਰ,
				 color_ਸਤਰ->ਅੰਕੜੇ,
				 color_ਸਤਰ->get_extend_user_ਅੰਕੜੇ);
}


/**
 * hb_paint_push_transform:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @xx: xx component of the transform matrix
 * @yx: yx component of the transform matrix
 * @xy: xy component of the transform matrix
 * @yy: yy component of the transform matrix
 * @dx: dx component of the transform matrix
 * @dy: dy component of the transform matrix
 *
 * Perform a "push-transform" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_ਧੱਕੋ_transform (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                         ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
                         ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
                         ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy)
{
  funcs->ਧੱਕੋ_transform (paint_ਅੰਕੜੇ, xx, yx, xy, yy, dx, dy);
}

/**
 * hb_paint_push_font_transform:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @font: a font
 *
 * Push the transform reflecting the font's scale and slant
 * settings onto the paint functions.
 *
 * Since: 11.0.0
 */
ਖਾਲੀ
hb_paint_ਧੱਕੋ_font_transform (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                              ਅਚਲ hb_font_t *font)
{
  funcs->ਧੱਕੋ_font_transform (paint_ਅੰਕੜੇ, font);
}

/**
 * hb_paint_push_inverse_font_transform:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @font: a font
 *
 * Push the inverse of the transform reflecting the font's
 * scale and slant settings onto the paint functions.
 *
 * Since: 11.0.0
 */
ਖਾਲੀ
hb_paint_ਧੱਕੋ_inverse_font_transform (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                      ਅਚਲ hb_font_t *font)
{
  funcs->ਧੱਕੋ_inverse_font_transform (paint_ਅੰਕੜੇ, font);
}

/**
 * hb_paint_pop_transform:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 *
 * Perform a "pop-transform" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_ਕੱਢੋ_transform (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ)
{
  funcs->ਕੱਢੋ_transform (paint_ਅੰਕੜੇ);
}

/**
 * hb_paint_color_glyph:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @glyph: the glyph ID
 * @font: the font
 *
 * Perform a "color-glyph" paint operation.
 *
 * Since: 8.2.0
 */
hb_bool_t
hb_paint_color_glyph (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                      hb_codepoint_t glyph,
                      hb_font_t *font)
{
  ਵਾਪਸ funcs->color_glyph (paint_ਅੰਕੜੇ, glyph, font);
}

/**
 * hb_paint_fill_glyph:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @glyph: the glyph ID
 * @font: the font
 * @is_foreground: whether the color is the foreground
 * @color: The color to use
 *
 * Perform a "fill-glyph" paint operation: fill the glyph's shape
 * with a solid color.
 *
 * Since: 14.3.0
 */
ਖਾਲੀ
hb_paint_fill_glyph (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                     hb_codepoint_t glyph,
                     hb_font_t *font,
                     hb_bool_t is_foreground,
                     hb_color_t color)
{
  funcs->fill_glyph (paint_ਅੰਕੜੇ, glyph, font, is_foreground, color);
}

/**
 * hb_paint_push_clip_glyph:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @glyph: the glyph ID
 * @font: the font
 *
 * Perform a "push-clip-glyph" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_ਧੱਕੋ_clip_glyph (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                          hb_codepoint_t glyph,
                          hb_font_t *font)
{
  funcs->ਧੱਕੋ_clip_glyph (paint_ਅੰਕੜੇ, glyph, font);
}

/**
 * hb_paint_push_clip_rectangle:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @xmin: min X for the rectangle
 * @ymin: min Y for the rectangle
 * @xmax: max X for the rectangle
 * @ymax: max Y for the rectangle
 *
 * Perform a "push-clip-rect" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_ਧੱਕੋ_clip_rectangle (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                              ਦਸ਼ਮਲਵ xmin, ਦਸ਼ਮਲਵ ymin, ਦਸ਼ਮਲਵ xmax, ਦਸ਼ਮਲਵ ymax)
{
  funcs->ਧੱਕੋ_clip_rectangle (paint_ਅੰਕੜੇ, xmin, ymin, xmax, ymax);
}

/**
 * hb_paint_push_clip_path_start:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @draw_data: (out) (nullable): location to receive the draw data
 *   the caller should pass alongside the returned draw funcs.
 *
 * Begin clipping to an arbitrary path.  Returns an
 * #hb_draw_funcs_t owned by the backend (the caller must not
 * free it) that the caller uses to emit the clip outline via
 * hb_draw_*() calls, using the returned @draw_data as the
 * draw data.  The returned draw funcs and draw data are only
 * valid until the matching hb_paint_push_clip_path_end() call;
 * no other paint calls should be made between start and end
 * except hb_draw_*() on the returned funcs.  Finish the path
 * with hb_paint_push_clip_path_end(); pop the clip later
 * with hb_paint_pop_clip().
 *
 * Usage:
 *
 * |[<!-- language="plain" -->
 * hb_draw_funcs_t *df = hb_paint_push_clip_path_start (pf, pd, &dd);
 * hb_draw_move_to (df, dd, NULL, ...);
 * hb_draw_line_to (df, dd, NULL, ...);
 * ...
 * hb_draw_close_path (df, dd, NULL);
 * hb_paint_push_clip_path_end (pf, pd);
 * /&ast; paint ops here are clipped to the emitted path &ast;/
 * hb_paint_pop_clip (pf, pd);
 * ]|
 *
 * Return value: (transfer none): draw funcs that accumulate
 *   the clip path, or `NULL` if the backend does not implement
 *   arbitrary-path clipping.
 *
 * Since: 14.2.0
 */
hb_draw_funcs_t *
hb_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ (hb_paint_funcs_t  *funcs,
                               ਖਾਲੀ              *paint_ਅੰਕੜੇ,
                               ਖਾਲੀ             **draw_ਅੰਕੜੇ)
{
  ਖਾਲੀ *scratch = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (!draw_ਅੰਕੜੇ) draw_ਅੰਕੜੇ = &scratch;
  ਵਾਪਸ funcs->ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ (paint_ਅੰਕੜੇ, draw_ਅੰਕੜੇ);
}

/**
 * hb_paint_push_clip_path_end:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 *
 * Signal that the arbitrary-clip path started by
 * hb_paint_push_clip_path_start() is fully drawn.  The
 * accumulated path now acts as a clip on the paint context
 * until a matching hb_paint_pop_clip() call.
 *
 * Since: 14.2.0
 */
ਖਾਲੀ
hb_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ)
{
  funcs->ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ (paint_ਅੰਕੜੇ);
}

/**
 * hb_paint_pop_clip:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 *
 * Perform a "pop-clip" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_ਕੱਢੋ_clip (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ)
{
  funcs->ਕੱਢੋ_clip (paint_ਅੰਕੜੇ);
}

/**
 * hb_paint_color:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @is_foreground: whether the color is the foreground
 * @color: The color to use
 *
 * Perform a "color" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_color (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                hb_bool_t is_foreground,
                hb_color_t color)
{
  funcs->color (paint_ਅੰਕੜੇ, is_foreground, color);
}

/**
 * hb_paint_image:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @image: image data
 * @width: width of the raster image in pixels, or 0
 * @height: height of the raster image in pixels, or 0
 * @format: the image format as a tag
 * @slant: Deprecated. set to 0.0
 * @extents: (nullable): the extents of the glyph
 *
 * Perform a "image" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_image (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                hb_blob_t *image,
                ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੌੜਾਈ,
                ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉਚਾਈ,
                hb_ਟੈਗ_t format,
                HB_ਅਣਵਰਤਿਆ ਦਸ਼ਮਲਵ slant,
                hb_glyph_extents_t *extents)
{
  funcs->image (paint_ਅੰਕੜੇ, image, ਚੌੜਾਈ, ਉਚਾਈ, format, 0.f, extents);
}

/**
 * hb_paint_linear_gradient:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @color_line: Color information for the gradient
 * @x0: X coordinate of the first point
 * @y0: Y coordinate of the first point
 * @x1: X coordinate of the second point
 * @y1: Y coordinate of the second point
 * @x2: X coordinate of the third point
 * @y2: Y coordinate of the third point
 *
 * Perform a "linear-gradient" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_ਰੇਖਿਕ_gradient (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                          hb_color_ਸਤਰ_t *color_ਸਤਰ,
                          ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
                          ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
                          ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2)
{
  funcs->ਰੇਖਿਕ_gradient (paint_ਅੰਕੜੇ, color_ਸਤਰ, x0, y0, x1, y1, x2, y2);
}

/**
 * hb_paint_radial_gradient:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @color_line: Color information for the gradient
 * @x0: X coordinate of the first circle's center
 * @y0: Y coordinate of the first circle's center
 * @r0: radius of the first circle
 * @x1: X coordinate of the second circle's center
 * @y1: Y coordinate of the second circle's center
 * @r1: radius of the second circle
 *
 * Perform a "radial-gradient" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_radial_gradient (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                          hb_color_ਸਤਰ_t *color_ਸਤਰ,
                          ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
                          ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1)
{
  funcs->radial_gradient (paint_ਅੰਕੜੇ, color_ਸਤਰ, x0, y0, r0, x1, y1, r1);
}

/**
 * hb_paint_sweep_gradient:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @color_line: Color information for the gradient
 * @x0: X coordinate of the circle's center
 * @y0: Y coordinate of the circle's center
 * @start_angle: the start angle
 * @end_angle: the end angle
 *
 * Perform a "sweep-gradient" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_sweep_gradient (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                         hb_color_ਸਤਰ_t *color_ਸਤਰ,
                         ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
                         ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle, ਦਸ਼ਮਲਵ ਸਿਰਾ_angle)
{
  funcs->sweep_gradient (paint_ਅੰਕੜੇ, color_ਸਤਰ, x0, y0, ਸ਼ੁਰੂ_angle, ਸਿਰਾ_angle);
}

/**
 * hb_paint_push_group:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 *
 * Perform a "push-group" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_ਧੱਕੋ_ਸਮੂਹ (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ)
{
  funcs->ਧੱਕੋ_ਸਮੂਹ (paint_ਅੰਕੜੇ);
}

/**
 * hb_paint_push_group_for:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @mode: the compositing mode that will be used when the group is popped
 *
 * Perform a "push-group" paint operation, with the compositing
 * mode known in advance.  By default, this calls
 * hb_paint_push_group().
 *
 * Since: 14.2.0
 */
ਖਾਲੀ
hb_paint_ਧੱਕੋ_ਸਮੂਹ_for (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                         hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
  funcs->ਧੱਕੋ_ਸਮੂਹ_for (paint_ਅੰਕੜੇ, ਬਹੁਲਕ);
}

/**
 * hb_paint_pop_group:
 * @funcs: paint functions
 * @paint_data: associated data passed by the caller
 * @mode: the compositing mode to use
 *
 * Perform a "pop-group" paint operation.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_paint_ਕੱਢੋ_ਸਮੂਹ (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                    hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
  funcs->ਕੱਢੋ_ਸਮੂਹ (paint_ਅੰਕੜੇ, ਬਹੁਲਕ);
}

/**
 * hb_paint_custom_palette_color:
 * @funcs: paint functions.
 * @paint_data: associated data passed by the caller.
 * @color_index: color index to fetch.
 * @color: (out): fetched color.
 *
 * Gets the custom palette override color for @color_index.
 *
 * Return value: `true` if a custom color is provided, `false` otherwise.
 *
 * Since: 7.0.0
 */
hb_bool_t
hb_paint_custom_palette_color (hb_paint_funcs_t *funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ color_ਸੂਚਕਾਂਕ,
                               hb_color_t *color)
{
  ਵਾਪਸ funcs->custom_palette_color (paint_ਅੰਕੜੇ, color_ਸੂਚਕਾਂਕ, color);
}


/**
 * hb_paint_reduce_linear_anchors:
 * @x0: x coordinate of P0 (color stop 0).
 * @y0: y coordinate of P0 (color stop 0).
 * @x1: x coordinate of P1 (color stop 1).
 * @y1: y coordinate of P1 (color stop 1).
 * @x2: x coordinate of P2 (rotation reference).
 * @y2: y coordinate of P2 (rotation reference).
 * @xx0: (out): x coordinate of the resulting axis start.
 * @yy0: (out): y coordinate of the resulting axis start.
 * @xx1: (out): x coordinate of the resulting axis end.
 * @yy1: (out): y coordinate of the resulting axis end.
 *
 * Reduces a COLRv1 linear gradient's 3-anchor spec (P0=color
 * stop 0, P1=color stop 1, P2=rotation reference) to the
 * 2-point axis (P0, P1') used by SVG / cairo / most software
 * renderers.  P1' is the foot of P1 on the line through P0
 * perpendicular to (P2 - P0); the resulting axis is the
 * gradient's actual direction (perpendicular to the rotation
 * line).  Degenerate (P0 == P2) passes through unchanged.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_paint_reduce_ਰੇਖਿਕ_anchors (ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
				ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
				ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
				ਦਸ਼ਮਲਵ *xx0, ਦਸ਼ਮਲਵ *yy0,
				ਦਸ਼ਮਲਵ *xx1, ਦਸ਼ਮਲਵ *yy1)
{
  ਦਸ਼ਮਲਵ q2x = x2 - x0, q2y = y2 - y0;
  ਦਸ਼ਮਲਵ s = q2x * q2x + q2y * q2y;
  ਜੇ (s < 1e-6f)
  {
    *xx0 = x0; *yy0 = y0;
    *xx1 = x1; *yy1 = y1;
    ਵਾਪਸ;
  }
  ਦਸ਼ਮਲਵ q1x = x1 - x0, q1y = y1 - y0;
  ਦਸ਼ਮਲਵ k = (q2x * q1x + q2y * q1y) / s;
  *xx0 = x0;
  *yy0 = y0;
  *xx1 = x1 - k * q2x;
  *yy1 = y1 - k * q2y;
}

/**
 * hb_paint_normalize_color_line:
 * @stops: (array length=len) (inout): color stops.
 * @len: number of stops.
 * @min: (out): original minimum offset.
 * @max: (out): original maximum offset.
 *
 * Sorts @stops by offset and rescales offsets into [0, 1] in
 * place.  Writes the original (min, max) to @min / @max so the
 * caller can shift the gradient geometry (axis endpoints for
 * linear, centers+radii for radial, start+end angles for sweep)
 * to keep the rendered gradient visually unchanged after the
 * rescale.  Empty input is safe: both out-parameters set to 0.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_paint_normalize_color_ਸਤਰ (hb_color_stop_t *stops,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     len,
			       ਦਸ਼ਮਲਵ           *min,
			       ਦਸ਼ਮਲਵ           *max)
{
  ਜੇ (unlikely (!len))
  {
    *min = *max = 0.f;
    ਵਾਪਸ;
  }

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (ਅਚਲ hb_color_stop_t &a, ਅਚਲ hb_color_stop_t &b) {
      ਵਾਪਸ (a.ਵਿੱਥ > b.ਵਿੱਥ) - (a.ਵਿੱਥ < b.ਵਿੱਥ);
    });

  ਦਸ਼ਮਲਵ mn = stops[0].ਵਿੱਥ, mx = stops[0].ਵਿੱਥ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < len; i++)
  {
    mn = hb_min (mn, stops[i].ਵਿੱਥ);
    mx = hb_max (mx, stops[i].ਵਿੱਥ);
  }
  ਜੇ (mn != mx)
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len; i++)
      stops[i].ਵਿੱਥ = (stops[i].ਵਿੱਥ - mn) / (mx - mn);

  *min = mn;
  *max = mx;
}

/**
 * hb_paint_sweep_gradient_tiles:
 * @stops: (array length=n_stops) (inout): color stops (sorted, offsets in [0,1]).
 * @n_stops: number of stops.
 * @extend: extend mode.
 * @start_angle: sweep start angle, in radians.
 * @end_angle: sweep end angle, in radians.
 * @emit_patch: (scope call): callback invoked once per tile.
 * @user_data: data passed to @emit_patch.
 *
 * Iterates the full 0..2π sweep produced by a color-stop list,
 * invoking @emit_patch once per (start, end) angular segment.
 * Handles #HB_PAINT_EXTEND_PAD, #HB_PAINT_EXTEND_REPEAT, and
 * #HB_PAINT_EXTEND_REFLECT.  Stops must be pre-sorted by
 * offset; use hb_paint_normalize_color_line() first if they
 * aren't.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_paint_sweep_gradient_tiles (hb_color_stop_t                     *stops,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                         n_stops,
			       hb_paint_extend_t                    extend,
			       ਦਸ਼ਮਲਵ                                ਸ਼ੁਰੂ_angle,
			       ਦਸ਼ਮਲਵ                                ਸਿਰਾ_angle,
			       hb_paint_sweep_gradient_tile_func_t  emit_patch,
			       ਖਾਲੀ                                *user_ਅੰਕੜੇ)
{
  ਜੇ (!n_stops) ਵਾਪਸ;

  ਜੇ (ਸ਼ੁਰੂ_angle == ਸਿਰਾ_angle)
  {
    ਜੇ (extend == HB_PAINT_EXTEND_PAD)
    {
      ਜੇ (ਸ਼ੁਰੂ_angle > 0.f)
	emit_patch (0.f, stops[0].color, ਸ਼ੁਰੂ_angle, stops[0].color, user_ਅੰਕੜੇ);
      ਜੇ (ਸਿਰਾ_angle < HB_2_PI)
	emit_patch (ਸਿਰਾ_angle, stops[n_stops - 1].color, HB_2_PI, stops[n_stops - 1].color, user_ਅੰਕੜੇ);
    }
    ਵਾਪਸ;
  }

  ਜੇ (ਸਿਰਾ_angle < ਸ਼ੁਰੂ_angle)
  {
    ਦਸ਼ਮਲਵ tmp = ਸ਼ੁਰੂ_angle; ਸ਼ੁਰੂ_angle = ਸਿਰਾ_angle; ਸਿਰਾ_angle = tmp;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n_stops - 1 - i; i++)
    {
      hb_color_stop_t t = stops[i];
      stops[i] = stops[n_stops - 1 - i];
      stops[n_stops - 1 - i] = t;
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n_stops; i++)
      stops[i].ਵਿੱਥ = 1.f - stops[i].ਵਿੱਥ;
  }

  /* Map stop offsets to angles. */
  ਦਸ਼ਮਲਵ angles_buf[16];
  hb_color_t colors_buf[16];
  ਦਸ਼ਮਲਵ *angles = angles_buf;
  hb_color_t *colors = colors_buf;
  ਤਾਰਕਿਕ dynamic = ਝੂਠ;

  ਜੇ (n_stops > 16)
  {
    angles = (ਦਸ਼ਮਲਵ *) hb_malloc (ਆਕਾਰ (ਦਸ਼ਮਲਵ) * n_stops);
    colors = (hb_color_t *) hb_malloc (ਆਕਾਰ (hb_color_t) * n_stops);
    ਜੇ (!angles || !colors)
    {
      hb_free (angles);
      hb_free (colors);
      ਵਾਪਸ;
    }
    dynamic = ਸੱਚ;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n_stops; i++)
  {
    angles[i] = ਸ਼ੁਰੂ_angle + stops[i].ਵਿੱਥ * (ਸਿਰਾ_angle - ਸ਼ੁਰੂ_angle);
    colors[i] = stops[i].color;
  }

  ਜੇ (extend == HB_PAINT_EXTEND_PAD)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ pos;
    hb_color_t color0 = colors[0];
    ਲਈ (pos = 0; pos < n_stops; pos++)
    {
      ਜੇ (angles[pos] >= 0)
      {
	ਜੇ (pos > 0)
	{
	  ਦਸ਼ਮਲਵ f = (0.f - angles[pos - 1]) / (angles[pos] - angles[pos - 1]);
	  color0 = hb_color_lerp (colors[pos - 1], colors[pos], f);
	}
	ਰੋਕੋ;
      }
    }
    ਜੇ (pos == n_stops)
    {
      color0 = colors[n_stops - 1];
      emit_patch (0.f, color0, HB_2_PI, color0, user_ਅੰਕੜੇ);
      ਜਾਓ ਪੂਰਾ;
    }
    emit_patch (0.f, color0, angles[pos], colors[pos], user_ਅੰਕੜੇ);
    ਲਈ (pos++; pos < n_stops; pos++)
    {
      ਜੇ (angles[pos] <= HB_2_PI)
	emit_patch (angles[pos - 1], colors[pos - 1], angles[pos], colors[pos], user_ਅੰਕੜੇ);
      ਨਹੀਂਤਾਂ
      {
	ਦਸ਼ਮਲਵ f = (HB_2_PI - angles[pos - 1]) / (angles[pos] - angles[pos - 1]);
	hb_color_t color1 = hb_color_lerp (colors[pos - 1], colors[pos], f);
	emit_patch (angles[pos - 1], colors[pos - 1], HB_2_PI, color1, user_ਅੰਕੜੇ);
	ਰੋਕੋ;
      }
    }
    ਜੇ (pos == n_stops)
    {
      color0 = colors[n_stops - 1];
      emit_patch (angles[n_stops - 1], color0, HB_2_PI, color0, user_ਅੰਕੜੇ);
      ਜਾਓ ਪੂਰਾ;
    }
  }
  ਨਹੀਂਤਾਂ
  {
    ਦਸ਼ਮਲਵ ਵਿਸਤਾਰ = angles[n_stops - 1] - angles[0];
    ਜੇ (!(fabsf (ਵਿਸਤਾਰ) >= 1e-6f)) /* Reversed to catch NaN. */
      ਜਾਓ ਪੂਰਾ;

    ਵਿਸਤਾਰ = fabsf (ਵਿਸਤਾਰ);

    ਜੇ (ਵਿਸਤਾਰ < HB_2_PI / HB_PAINT_MAX_SWEEP_TILES)
    {
      /* The pattern repeats faster than the tile resolution; tiling
       * it would cost unbounded work for sub-visual detail.  Fill
       * the whole circle with the period-average color instead. */
      ਦਸ਼ਮਲਵ r = 0.f, g = 0.f, b = 0.f, a = 0.f;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < n_stops; i++)
      {
	ਦਸ਼ਮਲਵ w = (angles[i] - angles[i - 1]) / ਵਿਸਤਾਰ * 0.5f;
	r += w * (hb_color_get_red (colors[i - 1]) + hb_color_get_red (colors[i]));
	g += w * (hb_color_get_green (colors[i - 1]) + hb_color_get_green (colors[i]));
	b += w * (hb_color_get_blue (colors[i - 1]) + hb_color_get_blue (colors[i]));
	a += w * (hb_color_get_alpha (colors[i - 1]) + hb_color_get_alpha (colors[i]));
      }
      hb_color_t avg = HB_COLOR (hb_clamp_ਮੰਜ਼ਿਲ<uint8_t> ((ਦੁੱਗਣਾ) b + 0.5),
				 hb_clamp_ਮੰਜ਼ਿਲ<uint8_t> ((ਦੁੱਗਣਾ) g + 0.5),
				 hb_clamp_ਮੰਜ਼ਿਲ<uint8_t> ((ਦੁੱਗਣਾ) r + 0.5),
				 hb_clamp_ਮੰਜ਼ਿਲ<uint8_t> ((ਦੁੱਗਣਾ) a + 0.5));
      emit_patch (0.f, avg, HB_2_PI, avg, user_ਅੰਕੜੇ);
      ਜਾਓ ਪੂਰਾ;
    }

    /* First repeat that can reach angle 0, computed by division;
     * counting there one span at a time can take effectively forever,
     * or fail to terminate outright once the span drops below the
     * float ulp of the angles.  One repeat of slack absorbs rounding
     * differences; earlier repeats are fully clipped anyway. */
    ਦੁੱਗਣਾ dk = floor (-(ਦੁੱਗਣਾ) hb_max (angles[0], angles[n_stops - 1]) / (ਦੁੱਗਣਾ) ਵਿਸਤਾਰ) - 1.;
    ਜੇ (!(dk >= -1.e18 && dk <= 1.e18)) /* Non-finite angles. */
      ਜਾਓ ਪੂਰਾ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ iterations = 0, tiles = 0;
    ਲਈ (int64_t l = (int64_t) dk; ; l++)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < n_stops; i++)
      {
	/* Bounds clipped-away segments too; those stay negligible for
	 * sorted stops but are unbounded if the sort contract is
	 * violated. */
	ਜੇ (unlikely (iterations++ >= 256 * HB_PAINT_MAX_SWEEP_TILES))
	  ਜਾਓ ਪੂਰਾ;

	ਦਸ਼ਮਲਵ a0_l, a1_l;
	hb_color_t col0, col1;
	ਜੇ ((l % 2 != 0) && (extend == HB_PAINT_EXTEND_REFLECT))
	{
	  a0_l = angles[0] + angles[n_stops - 1] - angles[n_stops - i] + l * ਵਿਸਤਾਰ;
	  a1_l = angles[0] + angles[n_stops - 1] - angles[n_stops - 1 - i] + l * ਵਿਸਤਾਰ;
	  col0 = colors[n_stops - i];
	  col1 = colors[n_stops - 1 - i];
	}
	ਨਹੀਂਤਾਂ
	{
	  a0_l = angles[i - 1] + l * ਵਿਸਤਾਰ;
	  a1_l = angles[i] + l * ਵਿਸਤਾਰ;
	  col0 = colors[i - 1];
	  col1 = colors[i];
	}

	ਜੇ (a1_l < 0.f) ਜਾਰੀ;
	ਜੇ (unlikely (tiles++ >= HB_PAINT_MAX_SWEEP_TILES))
	  ਜਾਓ ਪੂਰਾ;
	ਜੇ (a0_l < 0.f)
	{
	  ਦਸ਼ਮਲਵ f = (0.f - a0_l) / (a1_l - a0_l);
	  hb_color_t c = hb_color_lerp (col0, col1, f);
	  emit_patch (0.f, c, a1_l, col1, user_ਅੰਕੜੇ);
	}
	ਨਹੀਂਤਾਂ ਜੇ (a1_l >= HB_2_PI)
	{
	  ਦਸ਼ਮਲਵ f = (HB_2_PI - a0_l) / (a1_l - a0_l);
	  hb_color_t c = hb_color_lerp (col0, col1, f);
	  emit_patch (a0_l, col0, HB_2_PI, c, user_ਅੰਕੜੇ);
	  ਜਾਓ ਪੂਰਾ;
	}
	ਨਹੀਂਤਾਂ
	  emit_patch (a0_l, col0, a1_l, col1, user_ਅੰਕੜੇ);
      }
    }
  }

ਪੂਰਾ:
  ਜੇ (dynamic)
  {
    hb_free (angles);
    hb_free (colors);
  }
}

#ਜੇ_ਅੰਤ
