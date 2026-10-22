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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_PAINT

#அந்தர்பாவ "hb-paint.hh"

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

நிலையான வெறுமை
hb_paint_தள்ளு_transform_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                             தசம xx, தசம yx,
                             தசம xy, தசம yy,
                             தசம dx, தசம dy,
                             வெறுமை *user_data) {}

நிலையான வெறுமை
hb_paint_எடு_transform_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                            வெறுமை *user_data) {}

நிலையான hb_bool_t
hb_paint_color_glyph_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                          hb_codepoint_t glyph,
                          hb_font_t *font,
                          வெறுமை *user_data) { திரும்பு பொய்; }

நிலையான வெறுமை
hb_paint_fill_glyph_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                         hb_codepoint_t glyph,
                         hb_font_t *font,
                         hb_bool_t is_foreground,
                         hb_color_t color,
                         வெறுமை *user_data)
{
  /* Decompose into the equivalent general sequence. */
  funcs->தள்ளு_clip_glyph (paint_data, glyph, font);
  funcs->color (paint_data, is_foreground, color);
  funcs->எடு_clip (paint_data);
}

நிலையான வெறுமை
hb_paint_தள்ளு_clip_glyph_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                              hb_codepoint_t glyph,
                              hb_font_t *font,
                              வெறுமை *user_data) {}

நிலையான வெறுமை
hb_paint_தள்ளு_clip_rectangle_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                                  தசம xmin, தசம ymin, தசம xmax, தசம ymax,
                                  வெறுமை *user_data) {}

நிலையான hb_draw_funcs_t *
hb_paint_தள்ளு_clip_path_start_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                                   வெறுமை **draw_data,
                                   வெறுமை *user_data) { எனில் (draw_data) *draw_data = nullptr; திரும்பு nullptr; }

நிலையான வெறுமை
hb_paint_தள்ளு_clip_path_end_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                                 வெறுமை *user_data) {}

நிலையான வெறுமை
hb_paint_எடு_clip_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                       வெறுமை *user_data) {}

நிலையான வெறுமை
hb_paint_color_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                    hb_bool_t is_foreground,
                    hb_color_t color,
                    வெறுமை *user_data) {}

நிலையான hb_bool_t
hb_paint_image_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                    hb_blob_t *image,
                    unsigned முழு width,
                    unsigned முழு height,
                    hb_tag_t format,
                    தசம slant_xy_deprecated,
                    hb_glyph_extents_t *extents,
                    வெறுமை *user_data) { திரும்பு பொய்; }

நிலையான வெறுமை
hb_paint_linear_gradient_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                              hb_color_வரி_t *color_வரி,
                              தசம x0, தசம y0,
                              தசம x1, தசம y1,
                              தசம x2, தசம y2,
                              வெறுமை *user_data) {}

நிலையான வெறுமை
hb_paint_radial_gradient_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                              hb_color_வரி_t *color_வரி,
                              தசம x0, தசம y0, தசம r0,
                              தசம x1, தசம y1, தசம r1,
                              வெறுமை *user_data) {}

நிலையான வெறுமை
hb_paint_sweep_gradient_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                             hb_color_வரி_t *color_வரி,
                             தசம x0, தசம y0,
                             தசம start_angle,
                             தசம end_angle,
                             வெறுமை *user_data) {}

நிலையான வெறுமை
hb_paint_தள்ளு_group_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                         வெறுமை *user_data) {}

நிலையான வெறுமை
hb_paint_தள்ளு_group_for_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                             hb_paint_composite_mode_t mode,
                             வெறுமை *user_data)
{
  hb_paint_தள்ளு_group (funcs, paint_data);
}

நிலையான வெறுமை
hb_paint_எடு_group_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                        hb_paint_composite_mode_t mode,
                        வெறுமை *user_data) {}

நிலையான hb_bool_t
hb_paint_custom_palette_color_nil (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                                   unsigned முழு color_index,
                                   hb_color_t *color,
                                   வெறுமை *user_data) { திரும்பு பொய்; }

நிலையான hb_bool_t
hb_paint_set_budget_nil (hb_paint_funcs_t *funcs HB_UNUSED, வெறுமை *paint_data HB_UNUSED,
			 int64_t budget HB_UNUSED,
			 வெறுமை *user_data HB_UNUSED) { திரும்பு பொய்; }

நிலையான int64_t
hb_paint_get_budget_nil (hb_paint_funcs_t *funcs HB_UNUSED, வெறுமை *paint_data HB_UNUSED,
			 வெறுமை *user_data HB_UNUSED) { திரும்பு HB_BUDGET_DEFAULT; }

நிலையான int64_t *
hb_paint_get_budget_remaining_nil (hb_paint_funcs_t *funcs HB_UNUSED, வெறுமை *paint_data HB_UNUSED,
				   வெறுமை *user_data HB_UNUSED) { திரும்பு nullptr; }

நிலையான தருக்க
_hb_paint_funcs_set_preamble (hb_paint_funcs_t  *funcs,
                             தருக்க                func_is_null,
                             வெறுமை              **user_data,
                             hb_destroy_func_t  *destroy)
{
  எனில் (hb_object_is_immutable (funcs))
  {
    எனில் (*destroy)
      (*destroy) (*user_data);
    திரும்பு பொய்;
  }

  எனில் (func_is_null)
  {
    எனில் (*destroy)
      (*destroy) (*user_data);
    *destroy = nullptr;
    *user_data = nullptr;
  }

  திரும்பு உண்மை;
}

நிலையான தருக்க
hb_paint_funcs_set_நடு (hb_paint_funcs_t  *funcs,
                            வெறுமை              *user_data,
                            hb_destroy_func_t  destroy)
{
  auto destroy_guard = hb_make_scope_guard ([&]() {
    எனில் (destroy) destroy (user_data);
  });

  எனில் (user_data && !funcs->user_data)
  {
    funcs->user_data = (decltype (funcs->user_data)) hb_calloc (1, அளவு (*funcs->user_data));
    எனில் (unlikely (!funcs->user_data))
      திரும்பு பொய்;
  }
  எனில் (destroy && !funcs->destroy)
  {
    funcs->destroy = (decltype (funcs->destroy)) hb_calloc (1, அளவு (*funcs->destroy));
    எனில் (unlikely (!funcs->destroy))
      திரும்பு பொய்;
  }

  destroy_guard.release ();
  திரும்பு உண்மை;
}

#define HB_PAINT_FUNC_IMPLEMENT(name)                                           \
                                                                                \
வெறுமை                                                                            \
hb_paint_funcs_set_##name##_func (hb_paint_funcs_t         *funcs,              \
                                  hb_paint_##name##_func_t  func,               \
                                  வெறுமை                     *user_data,          \
                                  hb_destroy_func_t         destroy)            \
{                                                                               \
  எனில் (!_hb_paint_funcs_set_preamble (funcs, !func, &user_data, &destroy))       \
      திரும்பு;                                                                   \
                                                                                \
  எனில் (funcs->destroy && funcs->destroy->name)                                   \
    funcs->destroy->name (!funcs->user_data ? nullptr : funcs->user_data->name);\
                                                                                \
  எனில் (!hb_paint_funcs_set_நடு (funcs, user_data, destroy))                  \
      திரும்பு;                                                                   \
                                                                                \
  எனில் (func)                                                                     \
    funcs->func.name = func;                                                    \
  இல்லையெனில்                                                                          \
    funcs->func.name = hb_paint_##name##_nil;                                   \
                                                                                \
  எனில் (funcs->user_data)                                                         \
    funcs->user_data->name = user_data;                                         \
  எனில் (funcs->destroy)                                                           \
    funcs->destroy->name = destroy;                                             \
}

HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT

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
  எனில் (unlikely (!(funcs = hb_object_create<hb_paint_funcs_t> ())))
    திரும்பு const_cast<hb_paint_funcs_t *> (&Null (hb_paint_funcs_t));

  funcs->func =  Null (hb_paint_funcs_t).func;

  திரும்பு funcs;
}

DEFINE_NULL_INSTANCE (hb_paint_funcs_t) =
{
  HB_OBJECT_HEADER_STATIC,

  {
#define HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_nil,
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
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
hb_paint_funcs_get_empty ()
{
  திரும்பு const_cast<hb_paint_funcs_t *> (&Null (hb_paint_funcs_t));
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
  திரும்பு hb_object_reference (funcs);
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
வெறுமை
hb_paint_funcs_destroy (hb_paint_funcs_t *funcs)
{
  எனில் (!hb_object_destroy (funcs)) திரும்பு;

  எனில் (funcs->destroy)
  {
#define HB_PAINT_FUNC_IMPLEMENT(name) \
    எனில் (funcs->destroy->name) funcs->destroy->name (!funcs->user_data ? nullptr : funcs->user_data->name);
      HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_PAINT_FUNC_IMPLEMENT
  }

  hb_free (funcs->destroy);
  hb_free (funcs->user_data);
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
hb_paint_funcs_set_user_data (hb_paint_funcs_t *funcs,
			     hb_user_data_சாவி_t *key,
			     வெறுமை *              data,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  திரும்பு hb_object_set_user_data (funcs, key, data, destroy, replace);
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
வெறுமை *
hb_paint_funcs_get_user_data (மாறிலி hb_paint_funcs_t *funcs,
			     hb_user_data_சாவி_t       *key)
{
  திரும்பு hb_object_get_user_data (funcs, key);
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
வெறுமை
hb_paint_funcs_make_immutable (hb_paint_funcs_t *funcs)
{
  எனில் (hb_object_is_immutable (funcs))
    திரும்பு;

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
  திரும்பு hb_object_is_immutable (funcs);
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
hb_paint_set_budget (hb_paint_funcs_t *funcs, வெறுமை *paint_data, int64_t budget)
{
  திரும்பு funcs->set_budget (paint_data, budget);
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
hb_paint_get_budget (hb_paint_funcs_t *funcs, வெறுமை *paint_data)
{
  திரும்பு funcs->get_budget (paint_data);
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
hb_paint_get_budget_remaining (hb_paint_funcs_t *funcs, வெறுமை *paint_data)
{
  int64_t *remaining = funcs->get_budget_remaining_ptr (paint_data);
  எனில் (remaining)
    திரும்பு *remaining;

  int64_t budget = funcs->get_budget (paint_data);
  திரும்பு budget == HB_BUDGET_DEFAULT ? HB_BUDGET_UNLIMITED : budget;
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
unsigned முழு
hb_color_வரி_get_color_stops (hb_color_வரி_t *color_வரி,
                               unsigned முழு start,
                               unsigned முழு *மொத்தம்,
                               hb_color_stop_t *color_stops)
{
  திரும்பு color_வரி->get_color_stops (color_வரி,
				      color_வரி->data,
				      start, மொத்தம்,
				      color_stops,
				      color_வரி->get_color_stops_user_data);
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
hb_color_வரி_get_extend (hb_color_வரி_t *color_வரி)
{
  திரும்பு color_வரி->get_extend (color_வரி,
				 color_வரி->data,
				 color_வரி->get_extend_user_data);
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
வெறுமை
hb_paint_தள்ளு_transform (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                         தசம xx, தசம yx,
                         தசம xy, தசம yy,
                         தசம dx, தசம dy)
{
  funcs->தள்ளு_transform (paint_data, xx, yx, xy, yy, dx, dy);
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
வெறுமை
hb_paint_தள்ளு_font_transform (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                              மாறிலி hb_font_t *font)
{
  funcs->தள்ளு_font_transform (paint_data, font);
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
வெறுமை
hb_paint_தள்ளு_inverse_font_transform (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                                      மாறிலி hb_font_t *font)
{
  funcs->தள்ளு_inverse_font_transform (paint_data, font);
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
வெறுமை
hb_paint_எடு_transform (hb_paint_funcs_t *funcs, வெறுமை *paint_data)
{
  funcs->எடு_transform (paint_data);
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
hb_paint_color_glyph (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                      hb_codepoint_t glyph,
                      hb_font_t *font)
{
  திரும்பு funcs->color_glyph (paint_data, glyph, font);
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
வெறுமை
hb_paint_fill_glyph (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                     hb_codepoint_t glyph,
                     hb_font_t *font,
                     hb_bool_t is_foreground,
                     hb_color_t color)
{
  funcs->fill_glyph (paint_data, glyph, font, is_foreground, color);
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
வெறுமை
hb_paint_தள்ளு_clip_glyph (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                          hb_codepoint_t glyph,
                          hb_font_t *font)
{
  funcs->தள்ளு_clip_glyph (paint_data, glyph, font);
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
வெறுமை
hb_paint_தள்ளு_clip_rectangle (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                              தசம xmin, தசம ymin, தசம xmax, தசம ymax)
{
  funcs->தள்ளு_clip_rectangle (paint_data, xmin, ymin, xmax, ymax);
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
hb_paint_தள்ளு_clip_path_start (hb_paint_funcs_t  *funcs,
                               வெறுமை              *paint_data,
                               வெறுமை             **draw_data)
{
  வெறுமை *scratch = nullptr;
  எனில் (!draw_data) draw_data = &scratch;
  திரும்பு funcs->தள்ளு_clip_path_start (paint_data, draw_data);
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
வெறுமை
hb_paint_தள்ளு_clip_path_end (hb_paint_funcs_t *funcs, வெறுமை *paint_data)
{
  funcs->தள்ளு_clip_path_end (paint_data);
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
வெறுமை
hb_paint_எடு_clip (hb_paint_funcs_t *funcs, வெறுமை *paint_data)
{
  funcs->எடு_clip (paint_data);
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
வெறுமை
hb_paint_color (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                hb_bool_t is_foreground,
                hb_color_t color)
{
  funcs->color (paint_data, is_foreground, color);
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
வெறுமை
hb_paint_image (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                hb_blob_t *image,
                unsigned முழு width,
                unsigned முழு height,
                hb_tag_t format,
                HB_UNUSED தசம slant,
                hb_glyph_extents_t *extents)
{
  funcs->image (paint_data, image, width, height, format, 0.f, extents);
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
வெறுமை
hb_paint_linear_gradient (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                          hb_color_வரி_t *color_வரி,
                          தசம x0, தசம y0,
                          தசம x1, தசம y1,
                          தசம x2, தசம y2)
{
  funcs->linear_gradient (paint_data, color_வரி, x0, y0, x1, y1, x2, y2);
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
வெறுமை
hb_paint_radial_gradient (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                          hb_color_வரி_t *color_வரி,
                          தசம x0, தசம y0, தசம r0,
                          தசம x1, தசம y1, தசம r1)
{
  funcs->radial_gradient (paint_data, color_வரி, x0, y0, r0, x1, y1, r1);
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
வெறுமை
hb_paint_sweep_gradient (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                         hb_color_வரி_t *color_வரி,
                         தசம x0, தசம y0,
                         தசம start_angle, தசம end_angle)
{
  funcs->sweep_gradient (paint_data, color_வரி, x0, y0, start_angle, end_angle);
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
வெறுமை
hb_paint_தள்ளு_group (hb_paint_funcs_t *funcs, வெறுமை *paint_data)
{
  funcs->தள்ளு_group (paint_data);
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
வெறுமை
hb_paint_தள்ளு_group_for (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                         hb_paint_composite_mode_t mode)
{
  funcs->தள்ளு_group_for (paint_data, mode);
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
வெறுமை
hb_paint_எடு_group (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                    hb_paint_composite_mode_t mode)
{
  funcs->எடு_group (paint_data, mode);
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
hb_paint_custom_palette_color (hb_paint_funcs_t *funcs, வெறுமை *paint_data,
                               unsigned முழு color_index,
                               hb_color_t *color)
{
  திரும்பு funcs->custom_palette_color (paint_data, color_index, color);
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
வெறுமை
hb_paint_reduce_linear_anchors (தசம x0, தசம y0,
				தசம x1, தசம y1,
				தசம x2, தசம y2,
				தசம *xx0, தசம *yy0,
				தசம *xx1, தசம *yy1)
{
  தசம q2x = x2 - x0, q2y = y2 - y0;
  தசம s = q2x * q2x + q2y * q2y;
  எனில் (s < 1e-6f)
  {
    *xx0 = x0; *yy0 = y0;
    *xx1 = x1; *yy1 = y1;
    திரும்பு;
  }
  தசம q1x = x1 - x0, q1y = y1 - y0;
  தசம k = (q2x * q1x + q2y * q1y) / s;
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
வெறுமை
hb_paint_normalize_color_வரி (hb_color_stop_t *stops,
			       unsigned முழு     len,
			       தசம           *min,
			       தசம           *max)
{
  எனில் (unlikely (!len))
  {
    *min = *max = 0.f;
    திரும்பு;
  }

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (மாறிலி hb_color_stop_t &a, மாறிலி hb_color_stop_t &b) {
      திரும்பு (a.offset > b.offset) - (a.offset < b.offset);
    });

  தசம mn = stops[0].offset, mx = stops[0].offset;
  க்கு (unsigned i = 1; i < len; i++)
  {
    mn = hb_min (mn, stops[i].offset);
    mx = hb_max (mx, stops[i].offset);
  }
  எனில் (mn != mx)
    க்கு (unsigned i = 0; i < len; i++)
      stops[i].offset = (stops[i].offset - mn) / (mx - mn);

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
வெறுமை
hb_paint_sweep_gradient_tiles (hb_color_stop_t                     *stops,
			       unsigned முழு                         n_stops,
			       hb_paint_extend_t                    extend,
			       தசம                                start_angle,
			       தசம                                end_angle,
			       hb_paint_sweep_gradient_tile_func_t  emit_patch,
			       வெறுமை                                *user_data)
{
  எனில் (!n_stops) திரும்பு;

  எனில் (start_angle == end_angle)
  {
    எனில் (extend == HB_PAINT_EXTEND_PAD)
    {
      எனில் (start_angle > 0.f)
	emit_patch (0.f, stops[0].color, start_angle, stops[0].color, user_data);
      எனில் (end_angle < HB_2_PI)
	emit_patch (end_angle, stops[n_stops - 1].color, HB_2_PI, stops[n_stops - 1].color, user_data);
    }
    திரும்பு;
  }

  எனில் (end_angle < start_angle)
  {
    தசம tmp = start_angle; start_angle = end_angle; end_angle = tmp;
    க்கு (unsigned i = 0; i < n_stops - 1 - i; i++)
    {
      hb_color_stop_t t = stops[i];
      stops[i] = stops[n_stops - 1 - i];
      stops[n_stops - 1 - i] = t;
    }
    க்கு (unsigned i = 0; i < n_stops; i++)
      stops[i].offset = 1.f - stops[i].offset;
  }

  /* Map stop offsets to angles. */
  தசம angles_buf[16];
  hb_color_t colors_buf[16];
  தசம *angles = angles_buf;
  hb_color_t *colors = colors_buf;
  தருக்க dynamic = பொய்;

  எனில் (n_stops > 16)
  {
    angles = (தசம *) hb_malloc (அளவு (தசம) * n_stops);
    colors = (hb_color_t *) hb_malloc (அளவு (hb_color_t) * n_stops);
    எனில் (!angles || !colors)
    {
      hb_free (angles);
      hb_free (colors);
      திரும்பு;
    }
    dynamic = உண்மை;
  }

  க்கு (unsigned i = 0; i < n_stops; i++)
  {
    angles[i] = start_angle + stops[i].offset * (end_angle - start_angle);
    colors[i] = stops[i].color;
  }

  எனில் (extend == HB_PAINT_EXTEND_PAD)
  {
    unsigned pos;
    hb_color_t color0 = colors[0];
    க்கு (pos = 0; pos < n_stops; pos++)
    {
      எனில் (angles[pos] >= 0)
      {
	எனில் (pos > 0)
	{
	  தசம f = (0.f - angles[pos - 1]) / (angles[pos] - angles[pos - 1]);
	  color0 = hb_color_lerp (colors[pos - 1], colors[pos], f);
	}
	நிறுத்து;
      }
    }
    எனில் (pos == n_stops)
    {
      color0 = colors[n_stops - 1];
      emit_patch (0.f, color0, HB_2_PI, color0, user_data);
      செல் done;
    }
    emit_patch (0.f, color0, angles[pos], colors[pos], user_data);
    க்கு (pos++; pos < n_stops; pos++)
    {
      எனில் (angles[pos] <= HB_2_PI)
	emit_patch (angles[pos - 1], colors[pos - 1], angles[pos], colors[pos], user_data);
      இல்லையெனில்
      {
	தசம f = (HB_2_PI - angles[pos - 1]) / (angles[pos] - angles[pos - 1]);
	hb_color_t color1 = hb_color_lerp (colors[pos - 1], colors[pos], f);
	emit_patch (angles[pos - 1], colors[pos - 1], HB_2_PI, color1, user_data);
	நிறுத்து;
      }
    }
    எனில் (pos == n_stops)
    {
      color0 = colors[n_stops - 1];
      emit_patch (angles[n_stops - 1], color0, HB_2_PI, color0, user_data);
      செல் done;
    }
  }
  இல்லையெனில்
  {
    தசம span = angles[n_stops - 1] - angles[0];
    எனில் (!(fabsf (span) >= 1e-6f)) /* Reversed to catch NaN. */
      செல் done;

    span = fabsf (span);

    எனில் (span < HB_2_PI / HB_PAINT_MAX_SWEEP_TILES)
    {
      /* The pattern repeats faster than the tile resolution; tiling
       * it would cost unbounded work for sub-visual detail.  Fill
       * the whole circle with the period-average color instead. */
      தசம r = 0.f, g = 0.f, b = 0.f, a = 0.f;
      க்கு (unsigned i = 1; i < n_stops; i++)
      {
	தசம w = (angles[i] - angles[i - 1]) / span * 0.5f;
	r += w * (hb_color_get_red (colors[i - 1]) + hb_color_get_red (colors[i]));
	g += w * (hb_color_get_green (colors[i - 1]) + hb_color_get_green (colors[i]));
	b += w * (hb_color_get_blue (colors[i - 1]) + hb_color_get_blue (colors[i]));
	a += w * (hb_color_get_alpha (colors[i - 1]) + hb_color_get_alpha (colors[i]));
      }
      hb_color_t avg = HB_COLOR (hb_clamp_to<uint8_t> ((இரட்டை) b + 0.5),
				 hb_clamp_to<uint8_t> ((இரட்டை) g + 0.5),
				 hb_clamp_to<uint8_t> ((இரட்டை) r + 0.5),
				 hb_clamp_to<uint8_t> ((இரட்டை) a + 0.5));
      emit_patch (0.f, avg, HB_2_PI, avg, user_data);
      செல் done;
    }

    /* First repeat that can reach angle 0, computed by division;
     * counting there one span at a time can take effectively forever,
     * or fail to terminate outright once the span drops below the
     * float ulp of the angles.  One repeat of slack absorbs rounding
     * differences; earlier repeats are fully clipped anyway. */
    இரட்டை dk = floor (-(இரட்டை) hb_max (angles[0], angles[n_stops - 1]) / (இரட்டை) span) - 1.;
    எனில் (!(dk >= -1.e18 && dk <= 1.e18)) /* Non-finite angles. */
      செல் done;

    unsigned iterations = 0, tiles = 0;
    க்கு (int64_t l = (int64_t) dk; ; l++)
    {
      க்கு (unsigned i = 1; i < n_stops; i++)
      {
	/* Bounds clipped-away segments too; those stay negligible for
	 * sorted stops but are unbounded if the sort contract is
	 * violated. */
	எனில் (unlikely (iterations++ >= 256 * HB_PAINT_MAX_SWEEP_TILES))
	  செல் done;

	தசம a0_l, a1_l;
	hb_color_t col0, col1;
	எனில் ((l % 2 != 0) && (extend == HB_PAINT_EXTEND_REFLECT))
	{
	  a0_l = angles[0] + angles[n_stops - 1] - angles[n_stops - i] + l * span;
	  a1_l = angles[0] + angles[n_stops - 1] - angles[n_stops - 1 - i] + l * span;
	  col0 = colors[n_stops - i];
	  col1 = colors[n_stops - 1 - i];
	}
	இல்லையெனில்
	{
	  a0_l = angles[i - 1] + l * span;
	  a1_l = angles[i] + l * span;
	  col0 = colors[i - 1];
	  col1 = colors[i];
	}

	எனில் (a1_l < 0.f) தொடர்;
	எனில் (unlikely (tiles++ >= HB_PAINT_MAX_SWEEP_TILES))
	  செல் done;
	எனில் (a0_l < 0.f)
	{
	  தசம f = (0.f - a0_l) / (a1_l - a0_l);
	  hb_color_t c = hb_color_lerp (col0, col1, f);
	  emit_patch (0.f, c, a1_l, col1, user_data);
	}
	இல்லையெனில் எனில் (a1_l >= HB_2_PI)
	{
	  தசம f = (HB_2_PI - a0_l) / (a1_l - a0_l);
	  hb_color_t c = hb_color_lerp (col0, col1, f);
	  emit_patch (a0_l, col0, HB_2_PI, c, user_data);
	  செல் done;
	}
	இல்லையெனில்
	  emit_patch (a0_l, col0, a1_l, col1, user_data);
      }
    }
  }

done:
  எனில் (dynamic)
  {
    hb_free (angles);
    hb_free (colors);
  }
}

#endif
