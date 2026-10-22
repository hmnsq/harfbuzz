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

#ضمن "hb.hh"

#اگر_تعریف_نشده HB_NO_PAINT

#ضمن "hb-paint.hh"

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

ساكن فراغ
hb_paint_افزودن_transform_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                             عائم xx, عائم yx,
                             عائم xy, عائم yy,
                             عائم dx, عائم dy,
                             فراغ *user_داده) {}

ساكن فراغ
hb_paint_برداشت_transform_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                            فراغ *user_داده) {}

ساكن hb_bool_t
hb_paint_color_glyph_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                          hb_codepoint_t glyph,
                          hb_font_t *font,
                          فراغ *user_داده) { أرجع خطأ; }

ساكن فراغ
hb_paint_fill_glyph_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                         hb_codepoint_t glyph,
                         hb_font_t *font,
                         hb_bool_t is_foreground,
                         hb_color_t color,
                         فراغ *user_داده)
{
  /* Decompose into the equivalent general sequence. */
  funcs->افزودن_clip_glyph (paint_داده, glyph, font);
  funcs->color (paint_داده, is_foreground, color);
  funcs->برداشت_clip (paint_داده);
}

ساكن فراغ
hb_paint_افزودن_clip_glyph_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                              hb_codepoint_t glyph,
                              hb_font_t *font,
                              فراغ *user_داده) {}

ساكن فراغ
hb_paint_افزودن_clip_rectangle_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                                  عائم xmin, عائم ymin, عائم xmax, عائم ymax,
                                  فراغ *user_داده) {}

ساكن hb_draw_funcs_t *
hb_paint_افزودن_clip_مسیر_شروع_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                                   فراغ **draw_داده,
                                   فراغ *user_داده) { إذا (draw_داده) *draw_داده = اشاره_تهی; أرجع اشاره_تهی; }

ساكن فراغ
hb_paint_افزودن_clip_مسیر_انتها_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                                 فراغ *user_داده) {}

ساكن فراغ
hb_paint_برداشت_clip_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                       فراغ *user_داده) {}

ساكن فراغ
hb_paint_color_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                    hb_bool_t is_foreground,
                    hb_color_t color,
                    فراغ *user_داده) {}

ساكن hb_bool_t
hb_paint_image_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                    hb_blob_t *image,
                    بیعلامت صحيح عرض,
                    بیعلامت صحيح ارتفاع,
                    hb_برچسب_t format,
                    عائم slant_xy_deprecated,
                    hb_glyph_extents_t *extents,
                    فراغ *user_داده) { أرجع خطأ; }

ساكن فراغ
hb_paint_خطی_gradient_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                              hb_color_سطر_t *color_سطر,
                              عائم x0, عائم y0,
                              عائم x1, عائم y1,
                              عائم x2, عائم y2,
                              فراغ *user_داده) {}

ساكن فراغ
hb_paint_radial_gradient_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                              hb_color_سطر_t *color_سطر,
                              عائم x0, عائم y0, عائم r0,
                              عائم x1, عائم y1, عائم r1,
                              فراغ *user_داده) {}

ساكن فراغ
hb_paint_sweep_gradient_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                             hb_color_سطر_t *color_سطر,
                             عائم x0, عائم y0,
                             عائم شروع_angle,
                             عائم انتها_angle,
                             فراغ *user_داده) {}

ساكن فراغ
hb_paint_افزودن_گروه_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                         فراغ *user_داده) {}

ساكن فراغ
hb_paint_افزودن_گروه_for_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                             hb_paint_مرکب_نما_t نما,
                             فراغ *user_داده)
{
  hb_paint_افزودن_گروه (funcs, paint_داده);
}

ساكن فراغ
hb_paint_برداشت_گروه_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                        hb_paint_مرکب_نما_t نما,
                        فراغ *user_داده) {}

ساكن hb_bool_t
hb_paint_custom_palette_color_nil (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                                   بیعلامت صحيح color_شاخص,
                                   hb_color_t *color,
                                   فراغ *user_داده) { أرجع خطأ; }

ساكن hb_bool_t
hb_paint_مجموعه_budget_nil (hb_paint_funcs_t *funcs HB_استفادهنشده, فراغ *paint_داده HB_استفادهنشده,
			 int64_t budget HB_استفادهنشده,
			 فراغ *user_داده HB_استفادهنشده) { أرجع خطأ; }

ساكن int64_t
hb_paint_get_budget_nil (hb_paint_funcs_t *funcs HB_استفادهنشده, فراغ *paint_داده HB_استفادهنشده,
			 فراغ *user_داده HB_استفادهنشده) { أرجع HB_BUDGET_DEFAULT; }

ساكن int64_t *
hb_paint_get_budget_مانده_nil (hb_paint_funcs_t *funcs HB_استفادهنشده, فراغ *paint_داده HB_استفادهنشده,
				   فراغ *user_داده HB_استفادهنشده) { أرجع اشاره_تهی; }

ساكن منطقي
hb_paint_funcs_مجموعه_preamble (hb_paint_funcs_t  *funcs,
                             منطقي                func_is_null,
                             فراغ              **user_داده,
                             hb_destroy_func_t  *destroy)
{
  إذا (hb_object_is_immutable (funcs))
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
hb_paint_funcs_مجموعه_میانه (hb_paint_funcs_t  *funcs,
                            فراغ              *user_داده,
                            hb_destroy_func_t  destroy)
{
  خودکار destroy_guard = hb_make_scope_guard ([&]() {
    إذا (destroy) destroy (user_داده);
  });

  إذا (user_داده && !funcs->user_داده)
  {
    funcs->user_داده = (decltype (funcs->user_داده)) hb_calloc (1, حجم (*funcs->user_داده));
    إذا (unlikely (!funcs->user_داده))
      أرجع خطأ;
  }
  إذا (destroy && !funcs->destroy)
  {
    funcs->destroy = (decltype (funcs->destroy)) hb_calloc (1, حجم (*funcs->destroy));
    إذا (unlikely (!funcs->destroy))
      أرجع خطأ;
  }

  destroy_guard.release ();
  أرجع درست;
}

#تعریف_کن HB_PAINT_FUNC_IMPLEMENT(name)                                           \
                                                                                \
فراغ                                                                            \
hb_paint_funcs_مجموعه##name##_func (hb_paint_funcs_t         *funcs,              \
                                  hb_paint_##name##_func_t  func,               \
                                  فراغ                     *user_داده,          \
                                  hb_destroy_func_t         destroy)            \
{                                                                               \
  إذا (!hb_paint_funcs_مجموعه_preamble (funcs, !func, &user_داده, &destroy))       \
      أرجع;                                                                   \
                                                                                \
  إذا (funcs->destroy && funcs->destroy->name)                                   \
    funcs->destroy->name (!funcs->user_داده ? اشاره_تهی : funcs->user_داده->name);\
                                                                                \
  إذا (!hb_paint_funcs_مجموعه_میانه (funcs, user_داده, destroy))                  \
      أرجع;                                                                   \
                                                                                \
  إذا (func)                                                                     \
    funcs->func.name = func;                                                    \
  وإلا                                                                          \
    funcs->func.name = hb_paint_##name##_nil;                                   \
                                                                                \
  إذا (funcs->user_داده)                                                         \
    funcs->user_داده->name = user_داده;                                         \
  إذا (funcs->destroy)                                                           \
    funcs->destroy->name = destroy;                                             \
}

HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_PAINT_FUNC_IMPLEMENT

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
  إذا (unlikely (!(funcs = hb_object_create<hb_paint_funcs_t> ())))
    أرجع const_cast<hb_paint_funcs_t *> (&Null (hb_paint_funcs_t));

  funcs->func =  Null (hb_paint_funcs_t).func;

  أرجع funcs;
}

DEFINE_NULL_INSTANCE (hb_paint_funcs_t) =
{
  HB_OBJECT_سرآیند_STATIC,

  {
#تعریف_کن HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_nil,
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_PAINT_FUNC_IMPLEMENT
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
hb_paint_funcs_get_خالی ()
{
  أرجع const_cast<hb_paint_funcs_t *> (&Null (hb_paint_funcs_t));
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
  أرجع hb_object_reference (funcs);
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
فراغ
hb_paint_funcs_destroy (hb_paint_funcs_t *funcs)
{
  إذا (!hb_object_destroy (funcs)) أرجع;

  إذا (funcs->destroy)
  {
#تعریف_کن HB_PAINT_FUNC_IMPLEMENT(name) \
    إذا (funcs->destroy->name) funcs->destroy->name (!funcs->user_داده ? اشاره_تهی : funcs->user_داده->name);
      HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_PAINT_FUNC_IMPLEMENT
  }

  hb_free (funcs->destroy);
  hb_free (funcs->user_داده);
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
hb_paint_funcs_مجموعه_user_داده (hb_paint_funcs_t *funcs,
			     hb_user_داده_کلید_t *key,
			     فراغ *              داده,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  أرجع hb_object_مجموعه_user_داده (funcs, key, داده, destroy, replace);
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
فراغ *
hb_paint_funcs_get_user_داده (ثابت hb_paint_funcs_t *funcs,
			     hb_user_داده_کلید_t       *key)
{
  أرجع hb_object_get_user_داده (funcs, key);
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
فراغ
hb_paint_funcs_make_immutable (hb_paint_funcs_t *funcs)
{
  إذا (hb_object_is_immutable (funcs))
    أرجع;

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
  أرجع hb_object_is_immutable (funcs);
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
hb_paint_مجموعه_budget (hb_paint_funcs_t *funcs, فراغ *paint_داده, int64_t budget)
{
  أرجع funcs->مجموعه_budget (paint_داده, budget);
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
hb_paint_get_budget (hb_paint_funcs_t *funcs, فراغ *paint_داده)
{
  أرجع funcs->get_budget (paint_داده);
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
hb_paint_get_budget_مانده (hb_paint_funcs_t *funcs, فراغ *paint_داده)
{
  int64_t *مانده = funcs->get_budget_مانده_ptr (paint_داده);
  إذا (مانده)
    أرجع *مانده;

  int64_t budget = funcs->get_budget (paint_داده);
  أرجع budget == HB_BUDGET_DEFAULT ? HB_BUDGET_UNLIMITED : budget;
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
بیعلامت صحيح
hb_color_سطر_get_color_stops (hb_color_سطر_t *color_سطر,
                               بیعلامت صحيح شروع,
                               بیعلامت صحيح *شمار,
                               hb_color_stop_t *color_stops)
{
  أرجع color_سطر->get_color_stops (color_سطر,
				      color_سطر->داده,
				      شروع, شمار,
				      color_stops,
				      color_سطر->get_color_stops_user_داده);
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
hb_color_سطر_get_extend (hb_color_سطر_t *color_سطر)
{
  أرجع color_سطر->get_extend (color_سطر,
				 color_سطر->داده,
				 color_سطر->get_extend_user_داده);
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
فراغ
hb_paint_افزودن_transform (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                         عائم xx, عائم yx,
                         عائم xy, عائم yy,
                         عائم dx, عائم dy)
{
  funcs->افزودن_transform (paint_داده, xx, yx, xy, yy, dx, dy);
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
فراغ
hb_paint_افزودن_font_transform (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                              ثابت hb_font_t *font)
{
  funcs->افزودن_font_transform (paint_داده, font);
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
فراغ
hb_paint_افزودن_inverse_font_transform (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                                      ثابت hb_font_t *font)
{
  funcs->افزودن_inverse_font_transform (paint_داده, font);
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
فراغ
hb_paint_برداشت_transform (hb_paint_funcs_t *funcs, فراغ *paint_داده)
{
  funcs->برداشت_transform (paint_داده);
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
hb_paint_color_glyph (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                      hb_codepoint_t glyph,
                      hb_font_t *font)
{
  أرجع funcs->color_glyph (paint_داده, glyph, font);
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
فراغ
hb_paint_fill_glyph (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                     hb_codepoint_t glyph,
                     hb_font_t *font,
                     hb_bool_t is_foreground,
                     hb_color_t color)
{
  funcs->fill_glyph (paint_داده, glyph, font, is_foreground, color);
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
فراغ
hb_paint_افزودن_clip_glyph (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                          hb_codepoint_t glyph,
                          hb_font_t *font)
{
  funcs->افزودن_clip_glyph (paint_داده, glyph, font);
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
فراغ
hb_paint_افزودن_clip_rectangle (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                              عائم xmin, عائم ymin, عائم xmax, عائم ymax)
{
  funcs->افزودن_clip_rectangle (paint_داده, xmin, ymin, xmax, ymax);
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
hb_paint_افزودن_clip_مسیر_شروع (hb_paint_funcs_t  *funcs,
                               فراغ              *paint_داده,
                               فراغ             **draw_داده)
{
  فراغ *scratch = اشاره_تهی;
  إذا (!draw_داده) draw_داده = &scratch;
  أرجع funcs->افزودن_clip_مسیر_شروع (paint_داده, draw_داده);
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
فراغ
hb_paint_افزودن_clip_مسیر_انتها (hb_paint_funcs_t *funcs, فراغ *paint_داده)
{
  funcs->افزودن_clip_مسیر_انتها (paint_داده);
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
فراغ
hb_paint_برداشت_clip (hb_paint_funcs_t *funcs, فراغ *paint_داده)
{
  funcs->برداشت_clip (paint_داده);
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
فراغ
hb_paint_color (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                hb_bool_t is_foreground,
                hb_color_t color)
{
  funcs->color (paint_داده, is_foreground, color);
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
فراغ
hb_paint_image (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                hb_blob_t *image,
                بیعلامت صحيح عرض,
                بیعلامت صحيح ارتفاع,
                hb_برچسب_t format,
                HB_استفادهنشده عائم slant,
                hb_glyph_extents_t *extents)
{
  funcs->image (paint_داده, image, عرض, ارتفاع, format, 0.f, extents);
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
فراغ
hb_paint_خطی_gradient (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                          hb_color_سطر_t *color_سطر,
                          عائم x0, عائم y0,
                          عائم x1, عائم y1,
                          عائم x2, عائم y2)
{
  funcs->خطی_gradient (paint_داده, color_سطر, x0, y0, x1, y1, x2, y2);
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
فراغ
hb_paint_radial_gradient (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                          hb_color_سطر_t *color_سطر,
                          عائم x0, عائم y0, عائم r0,
                          عائم x1, عائم y1, عائم r1)
{
  funcs->radial_gradient (paint_داده, color_سطر, x0, y0, r0, x1, y1, r1);
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
فراغ
hb_paint_sweep_gradient (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                         hb_color_سطر_t *color_سطر,
                         عائم x0, عائم y0,
                         عائم شروع_angle, عائم انتها_angle)
{
  funcs->sweep_gradient (paint_داده, color_سطر, x0, y0, شروع_angle, انتها_angle);
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
فراغ
hb_paint_افزودن_گروه (hb_paint_funcs_t *funcs, فراغ *paint_داده)
{
  funcs->افزودن_گروه (paint_داده);
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
فراغ
hb_paint_افزودن_گروه_for (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                         hb_paint_مرکب_نما_t نما)
{
  funcs->افزودن_گروه_for (paint_داده, نما);
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
فراغ
hb_paint_برداشت_گروه (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                    hb_paint_مرکب_نما_t نما)
{
  funcs->برداشت_گروه (paint_داده, نما);
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
hb_paint_custom_palette_color (hb_paint_funcs_t *funcs, فراغ *paint_داده,
                               بیعلامت صحيح color_شاخص,
                               hb_color_t *color)
{
  أرجع funcs->custom_palette_color (paint_داده, color_شاخص, color);
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
فراغ
hb_paint_reduce_خطی_anchors (عائم x0, عائم y0,
				عائم x1, عائم y1,
				عائم x2, عائم y2,
				عائم *xx0, عائم *yy0,
				عائم *xx1, عائم *yy1)
{
  عائم q2x = x2 - x0, q2y = y2 - y0;
  عائم s = q2x * q2x + q2y * q2y;
  إذا (s < 1e-6f)
  {
    *xx0 = x0; *yy0 = y0;
    *xx1 = x1; *yy1 = y1;
    أرجع;
  }
  عائم q1x = x1 - x0, q1y = y1 - y0;
  عائم k = (q2x * q1x + q2y * q1y) / s;
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
فراغ
hb_paint_normalize_color_سطر (hb_color_stop_t *stops,
			       بیعلامت صحيح     len,
			       عائم           *min,
			       عائم           *max)
{
  إذا (unlikely (!len))
  {
    *min = *max = 0.f;
    أرجع;
  }

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b) {
      أرجع (a.جابه_جایی > b.جابه_جایی) - (a.جابه_جایی < b.جابه_جایی);
    });

  عائم mn = stops[0].جابه_جایی, mx = stops[0].جابه_جایی;
  لكل (بیعلامت i = 1; i < len; i++)
  {
    mn = hb_min (mn, stops[i].جابه_جایی);
    mx = hb_max (mx, stops[i].جابه_جایی);
  }
  إذا (mn != mx)
    لكل (بیعلامت i = 0; i < len; i++)
      stops[i].جابه_جایی = (stops[i].جابه_جایی - mn) / (mx - mn);

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
فراغ
hb_paint_sweep_gradient_tiles (hb_color_stop_t                     *stops,
			       بیعلامت صحيح                         n_stops,
			       hb_paint_extend_t                    extend,
			       عائم                                شروع_angle,
			       عائم                                انتها_angle,
			       hb_paint_sweep_gradient_tile_func_t  emit_patch,
			       فراغ                                *user_داده)
{
  إذا (!n_stops) أرجع;

  إذا (شروع_angle == انتها_angle)
  {
    إذا (extend == HB_PAINT_EXTEND_PAD)
    {
      إذا (شروع_angle > 0.f)
	emit_patch (0.f, stops[0].color, شروع_angle, stops[0].color, user_داده);
      إذا (انتها_angle < HB_2_PI)
	emit_patch (انتها_angle, stops[n_stops - 1].color, HB_2_PI, stops[n_stops - 1].color, user_داده);
    }
    أرجع;
  }

  إذا (انتها_angle < شروع_angle)
  {
    عائم tmp = شروع_angle; شروع_angle = انتها_angle; انتها_angle = tmp;
    لكل (بیعلامت i = 0; i < n_stops - 1 - i; i++)
    {
      hb_color_stop_t t = stops[i];
      stops[i] = stops[n_stops - 1 - i];
      stops[n_stops - 1 - i] = t;
    }
    لكل (بیعلامت i = 0; i < n_stops; i++)
      stops[i].جابه_جایی = 1.f - stops[i].جابه_جایی;
  }

  /* Map stop offsets to angles. */
  عائم angles_buf[16];
  hb_color_t colors_buf[16];
  عائم *angles = angles_buf;
  hb_color_t *colors = colors_buf;
  منطقي dynamic = خطأ;

  إذا (n_stops > 16)
  {
    angles = (عائم *) hb_malloc (حجم (عائم) * n_stops);
    colors = (hb_color_t *) hb_malloc (حجم (hb_color_t) * n_stops);
    إذا (!angles || !colors)
    {
      hb_free (angles);
      hb_free (colors);
      أرجع;
    }
    dynamic = درست;
  }

  لكل (بیعلامت i = 0; i < n_stops; i++)
  {
    angles[i] = شروع_angle + stops[i].جابه_جایی * (انتها_angle - شروع_angle);
    colors[i] = stops[i].color;
  }

  إذا (extend == HB_PAINT_EXTEND_PAD)
  {
    بیعلامت pos;
    hb_color_t color0 = colors[0];
    لكل (pos = 0; pos < n_stops; pos++)
    {
      إذا (angles[pos] >= 0)
      {
	إذا (pos > 0)
	{
	  عائم f = (0.f - angles[pos - 1]) / (angles[pos] - angles[pos - 1]);
	  color0 = hb_color_lerp (colors[pos - 1], colors[pos], f);
	}
	اكسر;
      }
    }
    إذا (pos == n_stops)
    {
      color0 = colors[n_stops - 1];
      emit_patch (0.f, color0, HB_2_PI, color0, user_داده);
      اذهب انجامشده;
    }
    emit_patch (0.f, color0, angles[pos], colors[pos], user_داده);
    لكل (pos++; pos < n_stops; pos++)
    {
      إذا (angles[pos] <= HB_2_PI)
	emit_patch (angles[pos - 1], colors[pos - 1], angles[pos], colors[pos], user_داده);
      وإلا
      {
	عائم f = (HB_2_PI - angles[pos - 1]) / (angles[pos] - angles[pos - 1]);
	hb_color_t color1 = hb_color_lerp (colors[pos - 1], colors[pos], f);
	emit_patch (angles[pos - 1], colors[pos - 1], HB_2_PI, color1, user_داده);
	اكسر;
      }
    }
    إذا (pos == n_stops)
    {
      color0 = colors[n_stops - 1];
      emit_patch (angles[n_stops - 1], color0, HB_2_PI, color0, user_داده);
      اذهب انجامشده;
    }
  }
  وإلا
  {
    عائم بازه = angles[n_stops - 1] - angles[0];
    إذا (!(fabsf (بازه) >= 1e-6f)) /* Reversed to catch NaN. */
      اذهب انجامشده;

    بازه = fabsf (بازه);

    إذا (بازه < HB_2_PI / HB_PAINT_MAX_SWEEP_TILES)
    {
      /* The pattern repeats faster than the tile resolution; tiling
       * it would cost unbounded work for sub-visual detail.  Fill
       * the whole circle with the period-average color instead. */
      عائم r = 0.f, g = 0.f, b = 0.f, a = 0.f;
      لكل (بیعلامت i = 1; i < n_stops; i++)
      {
	عائم w = (angles[i] - angles[i - 1]) / بازه * 0.5f;
	r += w * (hb_color_get_red (colors[i - 1]) + hb_color_get_red (colors[i]));
	g += w * (hb_color_get_green (colors[i - 1]) + hb_color_get_green (colors[i]));
	b += w * (hb_color_get_blue (colors[i - 1]) + hb_color_get_blue (colors[i]));
	a += w * (hb_color_get_alpha (colors[i - 1]) + hb_color_get_alpha (colors[i]));
      }
      hb_color_t avg = HB_COLOR (hb_clamp_مقصد<uint8_t> ((مزدوج) b + 0.5),
				 hb_clamp_مقصد<uint8_t> ((مزدوج) g + 0.5),
				 hb_clamp_مقصد<uint8_t> ((مزدوج) r + 0.5),
				 hb_clamp_مقصد<uint8_t> ((مزدوج) a + 0.5));
      emit_patch (0.f, avg, HB_2_PI, avg, user_داده);
      اذهب انجامشده;
    }

    /* First repeat that can reach angle 0, computed by division;
     * counting there one span at a time can take effectively forever,
     * or fail to terminate outright once the span drops below the
     * float ulp of the angles.  One repeat of slack absorbs rounding
     * differences; earlier repeats are fully clipped anyway. */
    مزدوج dk = floor (-(مزدوج) hb_max (angles[0], angles[n_stops - 1]) / (مزدوج) بازه) - 1.;
    إذا (!(dk >= -1.e18 && dk <= 1.e18)) /* Non-finite angles. */
      اذهب انجامشده;

    بیعلامت iterations = 0, tiles = 0;
    لكل (int64_t l = (int64_t) dk; ; l++)
    {
      لكل (بیعلامت i = 1; i < n_stops; i++)
      {
	/* Bounds clipped-away segments too; those stay negligible for
	 * sorted stops but are unbounded if the sort contract is
	 * violated. */
	إذا (unlikely (iterations++ >= 256 * HB_PAINT_MAX_SWEEP_TILES))
	  اذهب انجامشده;

	عائم a0_l, a1_l;
	hb_color_t col0, col1;
	إذا ((l % 2 != 0) && (extend == HB_PAINT_EXTEND_REFLECT))
	{
	  a0_l = angles[0] + angles[n_stops - 1] - angles[n_stops - i] + l * بازه;
	  a1_l = angles[0] + angles[n_stops - 1] - angles[n_stops - 1 - i] + l * بازه;
	  col0 = colors[n_stops - i];
	  col1 = colors[n_stops - 1 - i];
	}
	وإلا
	{
	  a0_l = angles[i - 1] + l * بازه;
	  a1_l = angles[i] + l * بازه;
	  col0 = colors[i - 1];
	  col1 = colors[i];
	}

	إذا (a1_l < 0.f) استمر;
	إذا (unlikely (tiles++ >= HB_PAINT_MAX_SWEEP_TILES))
	  اذهب انجامشده;
	إذا (a0_l < 0.f)
	{
	  عائم f = (0.f - a0_l) / (a1_l - a0_l);
	  hb_color_t c = hb_color_lerp (col0, col1, f);
	  emit_patch (0.f, c, a1_l, col1, user_داده);
	}
	وإلا إذا (a1_l >= HB_2_PI)
	{
	  عائم f = (HB_2_PI - a0_l) / (a1_l - a0_l);
	  hb_color_t c = hb_color_lerp (col0, col1, f);
	  emit_patch (a0_l, col0, HB_2_PI, c, user_داده);
	  اذهب انجامشده;
	}
	وإلا
	  emit_patch (a0_l, col0, a1_l, col1, user_داده);
      }
    }
  }

انجامشده:
  إذا (dynamic)
  {
    hb_free (angles);
    hb_free (colors);
  }
}

#پایان_اگر
