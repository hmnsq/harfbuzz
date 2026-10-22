/*
 * Copyright © 2026  Behdad Esfahbod
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
 *
 * Author(s): Behdad Esfahbod
 */

#ضمن "hb.hh"
#ضمن "hb-vector-paint.hh"
#ضمن "hb-vector-path.hh"
#ضمن "hb-font.hh"

/**
 * hb_vector_paint_create_or_fail:
 * @format: output format.
 *
 * Creates a new paint context for vector output.
 *
 * Return value: (nullable): a newly allocated #hb_vector_paint_t, or `NULL` on failure.
 *
 * Since: 13.0.0
 */
hb_ویکټور_paint_t *
hb_ویکټور_paint_create_or_fail (hb_ویکټور_format_t format)
{
  بدل (format)
  {
    حالة HB_ویکټور_FORMAT_SVG:
    حالة HB_ویکټور_FORMAT_PDF:
      اكسر;
    حالة HB_ویکټور_FORMAT_INVALID: افتراضي:
      أرجع تش_اشاره;
  }

  hb_ویکټور_paint_t *paint = hb_object_create<hb_ویکټور_paint_t> ();
  إذا (unlikely (!paint))
    أرجع تش_اشاره;
  paint->format = format;

  paint->فعال_color_glyphs = hb_سېټ_create ();
  paint->defs.alloc (4096);
  paint->لار.alloc (2048);
  paint->captured_scratch.alloc (4096);
  paint->color_stops_scratch.alloc (16);

  أرجع paint;
}

/**
 * hb_vector_paint_reference:
 * @paint: a paint context.
 *
 * Increases the reference count of @paint.
 *
 * Return value: (transfer full): referenced @paint.
 *
 * Since: 13.0.0
 */
hb_ویکټور_paint_t *
hb_ویکټور_paint_reference (hb_ویکټور_paint_t *paint)
{
  أرجع hb_object_reference (paint);
}

/**
 * hb_vector_paint_destroy:
 * @paint: a paint context.
 *
 * Decreases the reference count of @paint and destroys it when it reaches zero.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_destroy (hb_ویکټور_paint_t *paint)
{
  إذا (!hb_object_should_destroy (paint))
    أرجع;

  إذا (paint->format == HB_ویکټور_FORMAT_PDF)
    hb_ویکټور_paint_pdf_free_resources (paint);
  hb_blob_destroy (paint->recycled_blob);
  hb_سېټ_destroy (paint->فعال_color_glyphs);
  hb_free (paint->id_مختاړی);
  hb_object_actually_destroy (paint);
  hb_free (paint);
}

/**
 * hb_vector_paint_set_user_data:
 * @paint: a paint context.
 * @key: user-data key.
 * @data: user-data value.
 * @destroy: (nullable): destroy callback for @data.
 * @replace: whether to replace an existing value for @key.
 *
 * Attaches user data to @paint.
 *
 * Return value: `true` on success, `false` otherwise.
 *
 * Since: 13.0.0
 */
hb_bool_t
hb_ویکټور_paint_سېټ_user_معلومات (hb_ویکټور_paint_t  *paint,
                               hb_user_معلومات_کلی_t *key,
                               فراغ               *معلومات,
                               hb_destroy_func_t   destroy,
                               hb_bool_t           replace)
{
  أرجع hb_object_سېټ_user_معلومات (paint, key, معلومات, destroy, replace);
}

/**
 * hb_vector_paint_get_user_data:
 * @paint: a paint context.
 * @key: user-data key.
 *
 * Gets previously attached user data from @paint.
 *
 * Return value: (nullable): user-data value associated with @key.
 *
 * Since: 13.0.0
 */
فراغ *
hb_ویکټور_paint_get_user_معلومات (ثابت hb_ویکټور_paint_t  *paint,
                               hb_user_معلومات_کلی_t *key)
{
  أرجع hb_object_get_user_معلومات (paint, key);
}

/**
 * hb_vector_paint_set_transform:
 * @paint: a paint context.
 * @xx: transform xx component.
 * @yx: transform yx component.
 * @xy: transform xy component.
 * @yy: transform yy component.
 * @dx: transform x translation.
 * @dy: transform y translation.
 *
 * Sets the affine transform used when painting glyphs.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_سېټ_transform (hb_ویکټور_paint_t *paint,
                               عائم xx, عائم yx,
                               عائم xy, عائم yy,
                               عائم dx, عائم dy)
{
  paint->transform = {xx, yx, xy, yy, dx, dy};
}

/**
 * hb_vector_paint_get_transform:
 * @paint: a paint context.
 * @xx: (out) (nullable): transform xx component.
 * @yx: (out) (nullable): transform yx component.
 * @xy: (out) (nullable): transform xy component.
 * @yy: (out) (nullable): transform yy component.
 * @dx: (out) (nullable): transform x translation.
 * @dy: (out) (nullable): transform y translation.
 *
 * Gets the affine transform used when painting glyphs.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_get_transform (ثابت hb_ویکټور_paint_t *paint,
                               عائم *xx, عائم *yx,
                               عائم *xy, عائم *yy,
                               عائم *dx, عائم *dy)
{
  إذا (xx) *xx = paint->transform.xx;
  إذا (yx) *yx = paint->transform.yx;
  إذا (xy) *xy = paint->transform.xy;
  إذا (yy) *yy = paint->transform.yy;
  إذا (dx) *dx = paint->transform.x0;
  إذا (dy) *dy = paint->transform.y0;
}

/**
 * hb_vector_paint_set_scale_factor:
 * @paint: a paint context.
 * @x_scale_factor: x scale factor.
 * @y_scale_factor: y scale factor.
 *
 * Sets additional output scaling factors.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_سېټ_scale_ضریب (hb_ویکټور_paint_t *paint,
                                  عائم x_scale_ضریب,
                                  عائم y_scale_ضریب)
{
  paint->x_scale_ضریب = x_scale_ضریب > 0.f ? x_scale_ضریب : 1.f;
  paint->y_scale_ضریب = y_scale_ضریب > 0.f ? y_scale_ضریب : 1.f;
}

/**
 * hb_vector_paint_get_scale_factor:
 * @paint: a paint context.
 * @x_scale_factor: (out) (nullable): x scale factor.
 * @y_scale_factor: (out) (nullable): y scale factor.
 *
 * Gets additional output scaling factors.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_get_scale_ضریب (ثابت hb_ویکټور_paint_t *paint,
                                  عائم *x_scale_ضریب,
                                  عائم *y_scale_ضریب)
{
  إذا (x_scale_ضریب) *x_scale_ضریب = paint->x_scale_ضریب;
  إذا (y_scale_ضریب) *y_scale_ضریب = paint->y_scale_ضریب;
}

/**
 * hb_vector_paint_set_extents:
 * @paint: a paint context.
 * @extents: (nullable): output extents to set or expand.
 *
 * Sets or expands output extents on @paint. Passing `NULL` clears extents.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_سېټ_extents (hb_ویکټور_paint_t *paint,
                             ثابت hb_ویکټور_extents_t *extents)
{
  إذا (!extents)
  {
    paint->extents = {0, 0, 0, 0};
    paint->has_extents = خطأ;
    أرجع;
  }

  إذا (extents->پلنوالی == 0.f || extents->لوړوالی == 0.f)
    أرجع;

  /* Caller-supplied extents are in input-space; divide by
   * scale_factor so they end up in output-space, matching
   * the per-glyph extents accumulated via
   * hb_vector_set_glyph_extents_common (which applies the
   * same divide).  Normalize so origin is the min corner and
   * width/height are positive — callers may pass a
   * glyph-extents-style box with negative height. */
  عائم x0 = extents->x / paint->x_scale_ضریب;
  عائم y0 = extents->y / paint->y_scale_ضریب;
  عائم x1 = x0 + extents->پلنوالی  / paint->x_scale_ضریب;
  عائم y1 = y0 + extents->لوړوالی / paint->y_scale_ضریب;
  hb_ویکټور_extents_t e = {
    hb_min (x0, x1), hb_min (y0, y1),
    fabsf (x1 - x0), fabsf (y1 - y0),
  };

  إذا (paint->has_extents)
  {
    عائم x0 = hb_min (paint->extents.x, e.x);
    عائم y0 = hb_min (paint->extents.y, e.y);
    عائم x1 = hb_max (paint->extents.x + paint->extents.پلنوالی,
		       e.x + e.پلنوالی);
    عائم y1 = hb_max (paint->extents.y + paint->extents.لوړوالی,
		       e.y + e.لوړوالی);
    paint->extents = {x0, y0, x1 - x0, y1 - y0};
  }
  وإلا
  {
    paint->extents = e;
    paint->has_extents = ریښتیا;
  }
}

/**
 * hb_vector_paint_get_extents:
 * @paint: a paint context.
 * @extents: (out) (nullable): where to store current output extents.
 *
 * Gets current output extents from @paint.
 *
 * Return value: `true` if extents are set, `false` otherwise.
 *
 * Since: 13.0.0
 */
hb_bool_t
hb_ویکټور_paint_get_extents (ثابت hb_ویکټور_paint_t *paint,
                             hb_ویکټور_extents_t *extents)
{
  إذا (!paint->has_extents)
    أرجع خطأ;

  إذا (extents)
    *extents = paint->extents;
  أرجع ریښتیا;
}

/**
 * hb_vector_paint_set_glyph_extents:
 * @paint: a paint context.
 * @glyph_extents: glyph extents in font units.
 *
 * Expands @paint extents using @glyph_extents under the current transform.
 *
 * Return value: `true` on success, `false` otherwise.
 *
 * Since: 13.0.0
 */
hb_bool_t
hb_ویکټور_paint_سېټ_glyph_extents (hb_ویکټور_paint_t *paint,
                                   ثابت hb_glyph_extents_t *glyph_extents)
{
  hb_bool_t has_extents = paint->has_extents;
  hb_bool_t ret = hb_ویکټور_سېټ_glyph_extents_common (paint->transform,
						   paint->x_scale_ضریب,
						   paint->y_scale_ضریب,
						   glyph_extents,
						   &paint->extents,
						   &has_extents);
  paint->has_extents = has_extents;
  أرجع ret;
}

/**
 * hb_vector_paint_set_foreground:
 * @paint: a paint context.
 * @foreground: foreground color used for COLR foreground paints.
 *
 * Sets fallback foreground color used by paint operations.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_سېټ_foreground (hb_ویکټور_paint_t *paint,
                                hb_color_t foreground)
{
  paint->foreground = foreground;
}

/**
 * hb_vector_paint_get_foreground:
 * @paint: a paint context.
 *
 * Returns the foreground color previously set on @paint, or the
 * default opaque black if none was set.
 *
 * Return value: the foreground color.
 *
 * Since: 14.2.0
 */
hb_color_t
hb_ویکټور_paint_get_foreground (ثابت hb_ویکټور_paint_t *paint)
{
  أرجع paint->foreground;
}

/**
 * hb_vector_paint_set_background:
 * @paint: a paint context.
 * @background: background color.
 *
 * Sets the background color for @paint.  If set to a non-transparent
 * value, the renderer emits a filled rectangle covering the extents
 * behind all glyph content.  Default is transparent (no background).
 *
 * Since: 14.2.0
 */
فراغ
hb_ویکټور_paint_سېټ_background (hb_ویکټور_paint_t *paint,
                                hb_color_t background)
{
  paint->background = background;
}

/**
 * hb_vector_paint_get_background:
 * @paint: a paint context.
 *
 * Returns the background color previously set on @paint, or
 * transparent if none was set.
 *
 * Return value: the background color.
 *
 * Since: 14.2.0
 */
hb_color_t
hb_ویکټور_paint_get_background (ثابت hb_ویکټور_paint_t *paint)
{
  أرجع paint->background;
}

/**
 * hb_vector_paint_set_palette:
 * @paint: a paint context.
 * @palette: palette index for color glyph painting.
 *
 * Sets the color palette index used by paint operations.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_سېټ_palette (hb_ویکټور_paint_t *paint,
                             صحيح palette)
{
  paint->palette = palette;
}

/**
 * hb_vector_paint_get_palette:
 * @paint: a paint context.
 *
 * Returns the palette index previously set on @paint, or 0 if none
 * was set.
 *
 * Return value: the palette index.
 *
 * Since: 14.2.0
 */
صحيح
hb_ویکټور_paint_get_palette (ثابت hb_ویکټور_paint_t *paint)
{
  أرجع paint->palette;
}

/**
 * hb_vector_paint_set_custom_palette_color:
 * @paint: a paint context.
 * @color_index: color index to override.
 * @color: replacement color.
 *
 * Overrides one font palette color entry for subsequent paint operations.
 * Overrides are keyed by @color_index and persist on @paint until cleared
 * (or replaced for the same index).
 *
 * These overrides are consulted by paint operations that resolve CPAL
 * entries, including SVG glyph content using `var(--colorN)`.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_سېټ_custom_palette_color (hb_ویکټور_paint_t *paint,
                                          بینښه color_شاخص,
                                          hb_color_t color)
{
  paint->custom_palette_colors.سېټ (color_شاخص, color);
}

/**
 * hb_vector_paint_clear_custom_palette_colors:
 * @paint: a paint context.
 *
 * Clears all custom palette color overrides previously set on @paint.
 *
 * After this call, palette lookups use the selected font palette without
 * custom override entries.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_clear_custom_palette_colors (hb_ویکټور_paint_t *paint)
{
  paint->custom_palette_colors.clear ();
}

/**
 * hb_vector_paint_get_format:
 * @paint: a vector paint context.
 *
 * Gets the output format @paint was created with.
 *
 * Return value: the output format.
 *
 * Since: 14.2.0
 */
hb_ویکټور_format_t
hb_ویکټور_paint_get_format (ثابت hb_ویکټور_paint_t *paint)
{
  أرجع paint->format;
}

/**
 * hb_vector_paint_get_funcs:
 * @paint: a vector paint context.
 *
 * Gets paint callbacks for emitting paint operations into @paint.
 * Pass @paint as the @paint_data argument when calling them.
 *
 * Return value: (transfer none): immutable #hb_paint_funcs_t.
 *
 * Since: 14.2.0
 */
hb_paint_funcs_t *
hb_ویکټور_paint_get_funcs (ثابت hb_ویکټور_paint_t *paint)
{
  بدل (paint ? paint->format : HB_ویکټور_FORMAT_INVALID)
  {
    حالة HB_ویکټور_FORMAT_PDF:
      أرجع hb_ویکټور_paint_pdf_funcs_get ();
    حالة HB_ویکټور_FORMAT_SVG:
      أرجع hb_ویکټور_paint_svg_funcs_get ();
    حالة HB_ویکټور_FORMAT_INVALID:
    افتراضي:
      أرجع تش_اشاره;
  }
}


ساكن hb_bool_t
hb_ویکټور_paint_glyph_impl (hb_ویکټور_paint_t *paint,
			    hb_font_t         *font,
			    hb_codepoint_t     glyph,
			    hb_ویکټور_extents_ډېرپېښ_t extents_ډېرپېښ,
			    hb_bool_t          fallible)
{
  إذا (extents_ډېرپېښ == HB_ویکټور_EXTENTS_ډېرپېښ_EXPAND)
  {
    hb_glyph_extents_t ge;
    إذا (hb_font_get_glyph_extents (font, glyph, &ge))
    {
      hb_bool_t has_extents = paint->has_extents;
      hb_ویکټور_سېټ_glyph_extents_common (paint->transform,
					  paint->x_scale_ضریب,
					  paint->y_scale_ضریب,
					  &ge,
					  &paint->extents,
					  &has_extents);
      paint->has_extents = has_extents;
    }
  }

  hb_paint_funcs_t *funcs = hb_ویکټور_paint_get_funcs (paint);
  hb_paint_ټېل_transform (funcs, paint,
			   paint->transform.xx, paint->transform.yx,
			   paint->transform.xy, paint->transform.yy,
			   paint->transform.x0, paint->transform.y0);

  hb_bool_t ret = ریښتیا;
  إذا (fallible)
    ret = hb_font_paint_glyph_or_fail (font, glyph,
				       funcs, paint,
				       (بینښه) paint->palette,
				       paint->foreground);
  وإلا
    hb_font_paint_glyph (font, glyph,
			 funcs, paint,
			 (بینښه) paint->palette,
			 paint->foreground);

  hb_paint_راوباسه_transform (funcs, paint);
  أرجع ret;
}

/**
 * hb_vector_paint_glyph_or_fail:
 * @paint: a paint context.
 * @font: font object.
 * @glyph: glyph ID.
 * @extents_mode: extents update mode.
 *
 * Convenience to paint one color glyph into @paint.
 * Equivalent to:
 *
 * |[<!-- language="plain" -->
 * // extend extents if requested
 * hb_paint_funcs_t *funcs = hb_vector_paint_get_funcs (paint);
 * hb_paint_push_transform (funcs, paint, ...transform...);
 * hb_font_paint_glyph_or_fail (font, glyph, funcs, paint,
 *   palette, foreground);
 * hb_paint_pop_transform (funcs, paint);
 * ]|
 *
 * Return value: `true` if glyph paint data was emitted, `false` otherwise.
 *
 * Since: 14.2.0
 */
hb_bool_t
hb_ویکټور_paint_glyph_or_fail (hb_ویکټور_paint_t *paint,
			       hb_font_t         *font,
			       hb_codepoint_t     glyph,
			       hb_ویکټور_extents_ډېرپېښ_t extents_ډېرپېښ)
{
  أرجع hb_ویکټور_paint_glyph_impl (paint, font, glyph,
				     extents_ډېرپېښ, ریښتیا);
}

/**
 * hb_vector_paint_glyph:
 * @paint: a paint context.
 * @font: font object.
 * @glyph: glyph ID.
 * @extents_mode: extents update mode.
 *
 * Paints one glyph into @paint.  Unlike
 * hb_vector_paint_glyph_or_fail(), glyphs with no color paint
 * data fall back to a synthesized foreground-colored outline,
 * so any glyph with an outline or bitmap image produces output.
 *
 * Since: 14.2.0
 */
فراغ
hb_ویکټور_paint_glyph (hb_ویکټور_paint_t *paint,
		       hb_font_t         *font,
		       hb_codepoint_t     glyph,
		       hb_ویکټور_extents_ډېرپېښ_t extents_ډېرپېښ)
{
  hb_ویکټور_paint_glyph_impl (paint, font, glyph,
			      extents_ډېرپېښ, خطأ);
}

/**
 * hb_vector_paint_set_svg_prefix:
 * @paint: a paint context.
 * @prefix: a null-terminated ASCII string to prepend to every emitted
 *          SVG `id` and `url(#...)` reference, or `NULL` for none.
 *
 * Namespaces the paint's SVG output.  Callers that inject multiple
 * hb-vector SVGs into the same document (e.g. several glyph previews
 * on one page) must set a distinct prefix per context so that the
 * short IDs hb-vector uses for clipPaths, gradients, and use-refs
 * don't collide in the DOM.
 *
 * No effect on PDF output.
 *
 * Since: 14.2.0
 */
فراغ
hb_ویکټور_paint_سېټ_svg_مختاړی (hb_ویکټور_paint_t *paint,
                                ثابت حرف *مختاړی)
{
  حرف *old = paint->id_مختاړی;
  paint->id_مختاړی = تش_اشاره;
  paint->id_مختاړی_اوږدتیا = 0;
  إذا (مختاړی && *مختاړی)
  {
    بینښه len = مزي_اوږدوالی (مختاړی);
    حرف *copy = (حرف *) hb_malloc (len + 1);
    إذا (likely (copy))
    {
      hb_memcpy (copy, مختاړی, len);
      copy[len] = '\0';
      paint->id_مختاړی = copy;
      paint->id_مختاړی_اوږدتیا = len;
    }
  }
  hb_free (old);
}

/**
 * hb_vector_paint_get_svg_prefix:
 * @paint: a paint context.
 *
 * Returns the SVG id prefix previously set on @paint, or `""` if
 * none was set.  The pointer remains valid until the next call to
 * hb_vector_paint_set_svg_prefix() or hb_vector_paint_reset() on the
 * same context.
 *
 * Return value: the SVG id prefix.
 *
 * Since: 14.2.0
 */
ثابت حرف *
hb_ویکټور_paint_get_svg_مختاړی (ثابت hb_ویکټور_paint_t *paint)
{
  أرجع paint->id_مختاړی ? paint->id_مختاړی : "";
}

/**
 * hb_vector_paint_set_precision:
 * @paint: a paint context.
 * @precision: decimal precision.
 *
 * Sets numeric output precision for paint output.
 *
 * Since: 14.2.0
 */
فراغ
hb_ویکټور_paint_سېټ_precision (hb_ویکټور_paint_t *paint,
                                   بینښه precision)
{
  paint->سېټ_precision (precision);
}

/**
 * hb_vector_paint_get_precision:
 * @paint: a paint context.
 *
 * Returns the numeric output precision previously set on @paint,
 * or the default if none was set.
 *
 * Return value: the precision.
 *
 * Since: 14.2.0
 */
بینښه
hb_ویکټور_paint_get_precision (ثابت hb_ویکټور_paint_t *paint)
{
  أرجع paint->get_precision ();
}

/**
 * hb_vector_paint_clear:
 * @paint: a paint context.
 *
 * Discards accumulated paint output so @paint can be reused for
 * another render.  User configuration (transform, scale factors,
 * precision, foreground, background, palette, custom palette colors,
 * SVG id prefix, and budget) is preserved.  Call hb_vector_paint_reset()
 * to also reset user configuration to defaults.
 *
 * Since: 14.2.0
 */
فراغ
hb_ویکټور_paint_clear (hb_ویکټور_paint_t *paint)
{
  paint->extents = {0, 0, 0, 0};
  paint->has_extents = خطأ;

  إذا (paint->format == HB_ویکټور_FORMAT_PDF)
    hb_ویکټور_paint_pdf_free_resources (paint);
  paint->defs.clear ();
  paint->لار.clear ();
  paint->ډله_ډېری.clear ();
  paint->transform_ډله_open_ماسک = 0;
  paint->transform_ډله_ژوروالی = 0;
  paint->transform_ډله_ډکېدنه_ژوروالی = 0;
  paint->clip_rect_counter = 0;
  paint->clip_لار_counter = 0;
  paint->recharge_budget ();
  paint->gradient_counter = 0;
  paint->color_glyph_ژوروالی = 0;
  paint->لار_def_شمېر = 0;
  hb_سېټ_clear (paint->فعال_color_glyphs);
  paint->color_stops_scratch.clear ();
  paint->captured_scratch.clear ();
}


/**
 * hb_vector_paint_render:
 * @paint: a paint context.
 *
 * Renders accumulated paint content to an output blob.
 * Accumulated content and extents are cleared on return, including
 * when rendering fails. User configuration is preserved.
 *
 * Return value: (transfer full) (nullable): output blob, or `NULL` if rendering cannot proceed.
 *
 * Since: 13.0.0
 */
hb_blob_t *
hb_ویکټور_paint_render (hb_ویکټور_paint_t *paint)
{
  HB_SCOPE_GUARD (hb_ویکټور_paint_clear (paint));
  بدل (paint->format)
  {
    حالة HB_ویکټور_FORMAT_SVG:
      أرجع hb_ویکټور_paint_render_svg (paint);

    حالة HB_ویکټور_FORMAT_PDF:
      أرجع hb_ویکټور_paint_render_pdf (paint);

    حالة HB_ویکټور_FORMAT_INVALID: افتراضي:
      أرجع تش_اشاره;
  }
}

/**
 * hb_vector_paint_reset:
 * @paint: a paint context.
 *
 * Restores user configuration to defaults and clears accumulated content.
 * The output format and attached user data are preserved.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_reset (hb_ویکټور_paint_t *paint)
{
  paint->transform = {1, 0, 0, 1, 0, 0};
  paint->x_scale_ضریب = 1.f;
  paint->y_scale_ضریب = 1.f;
  paint->foreground = HB_COLOR (0, 0, 0, 255);
  paint->background = HB_COLOR (0, 0, 0, 0);
  paint->palette = 0;
  hb_ویکټور_paint_سېټ_svg_مختاړی (paint, تش_اشاره);
  hb_ویکټور_paint_clear_custom_palette_colors (paint);
  paint->سېټ_precision (2);
  paint->budget = HB_BUDGET_DEFAULT;
  hb_ویکټور_paint_clear (paint);
}

/**
 * hb_vector_paint_recycle_blob:
 * @paint: a paint context.
 * @blob: (transfer full) (nullable): previously rendered blob to recycle.
 *
 * Provides a blob for internal buffer reuse by later render calls.
 * The caller transfers ownership of one reference to @paint and must
 * not destroy that reference after this call. Finish accessing @blob
 * and its data before recycling: later renders may reuse the blob
 * and overwrite its data.
 *
 * Since: 13.0.0
 */
فراغ
hb_ویکټور_paint_recycle_blob (hb_ویکټور_paint_t *paint,
                              hb_blob_t *blob)
{
  hb_blob_destroy (paint->recycled_blob);
  paint->recycled_blob = تش_اشاره;
  إذا (!blob || blob == hb_blob_get_تشه ())
    أرجع;
  paint->recycled_blob = blob;
}
