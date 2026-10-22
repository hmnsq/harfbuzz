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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_PAINT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-face.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-geometry.hh"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (ਧੱਕੋ_transform) \
  HB_PAINT_FUNC_IMPLEMENT (ਕੱਢੋ_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ਧੱਕੋ_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ਧੱਕੋ_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ) \
  HB_PAINT_FUNC_IMPLEMENT (ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ) \
  HB_PAINT_FUNC_IMPLEMENT (ਕੱਢੋ_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (ਰੇਖਿਕ_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (ਧੱਕੋ_ਸਮੂਹ) \
  HB_PAINT_FUNC_IMPLEMENT (ਧੱਕੋ_ਸਮੂਹ_for) \
  HB_PAINT_FUNC_IMPLEMENT (ਕੱਢੋ_ਸਮੂਹ) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (ਸੈੱਟ_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_ਬਚਿਆ) \
  /* ^--- Add new callbacks here */

ਬਣਤਰ hb_paint_funcs_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_PAINT_FUNC_IMPLEMENT
  } func;

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_FUNC_IMPLEMENT(name) ਖਾਲੀ *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_PAINT_FUNC_IMPLEMENT
  } *user_ਅੰਕੜੇ;

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  ਖਾਲੀ ਧੱਕੋ_transform (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                       ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
                       ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
                       ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    ਜੇ (dx == -0.f) dx = 0.f;
    ਜੇ (dy == -0.f) dy = 0.f;
    func.ਧੱਕੋ_transform (ਇਹ, paint_ਅੰਕੜੇ,
                         xx, yx, xy, yy, dx, dy,
                         !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਧੱਕੋ_transform); }
  ਖਾਲੀ ਕੱਢੋ_transform (ਖਾਲੀ *paint_ਅੰਕੜੇ)
  { func.ਕੱਢੋ_transform (ਇਹ, paint_ਅੰਕੜੇ,
                        !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਕੱਢੋ_transform); }
  ਤਾਰਕਿਕ color_glyph (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { ਵਾਪਸ func.color_glyph (ਇਹ, paint_ਅੰਕੜੇ,
                             glyph,
                             font,
                             !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->color_glyph); }
  ਖਾਲੀ ਧੱਕੋ_clip_glyph (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.ਧੱਕੋ_clip_glyph (ਇਹ, paint_ਅੰਕੜੇ,
                          glyph,
                          font,
                          !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਧੱਕੋ_clip_glyph); }
  ਖਾਲੀ ਧੱਕੋ_clip_rectangle (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                           ਦਸ਼ਮਲਵ xmin, ਦਸ਼ਮਲਵ ymin, ਦਸ਼ਮਲਵ xmax, ਦਸ਼ਮਲਵ ymax)
  { func.ਧੱਕੋ_clip_rectangle (ਇਹ, paint_ਅੰਕੜੇ,
                              xmin, ymin, xmax, ymax,
                              !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਧੱਕੋ_clip_rectangle); }
  hb_draw_funcs_t *ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                         ਖਾਲੀ **draw_ਅੰਕੜੇ)
  { ਵਾਪਸ func.ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ (ਇਹ, paint_ਅੰਕੜੇ, draw_ਅੰਕੜੇ,
                                      !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ); }
  ਖਾਲੀ ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ (ਖਾਲੀ *paint_ਅੰਕੜੇ)
  { func.ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ (ਇਹ, paint_ਅੰਕੜੇ,
                             !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ); }
  ਖਾਲੀ ਕੱਢੋ_clip (ਖਾਲੀ *paint_ਅੰਕੜੇ)
  { func.ਕੱਢੋ_clip (ਇਹ, paint_ਅੰਕੜੇ,
                   !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਕੱਢੋ_clip); }
  ਖਾਲੀ color (ਖਾਲੀ *paint_ਅੰਕੜੇ,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (ਇਹ, paint_ਅੰਕੜੇ,
                is_foreground, color,
                !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->color); }
  ਖਾਲੀ fill_glyph (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (ਇਹ, paint_ਅੰਕੜੇ,
                     glyph, font,
                     is_foreground, color,
                     !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->fill_glyph); }
  ਤਾਰਕਿਕ image (ਖਾਲੀ *paint_ਅੰਕੜੇ,
              hb_blob_t *image,
              ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
              hb_ਟੈਗ_t format,
              ਦਸ਼ਮਲਵ slant,
              hb_glyph_extents_t *extents)
  { ਵਾਪਸ func.image (ਇਹ, paint_ਅੰਕੜੇ,
                       image, ਚੌੜਾਈ, ਉਚਾਈ, format, slant, extents,
                       !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->image); }
  ਖਾਲੀ ਰੇਖਿਕ_gradient (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                        hb_color_ਸਤਰ_t *color_ਸਤਰ,
                        ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
                        ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
                        ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2)
  { func.ਰੇਖਿਕ_gradient (ਇਹ, paint_ਅੰਕੜੇ,
                          color_ਸਤਰ, x0, y0, x1, y1, x2, y2,
                          !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਰੇਖਿਕ_gradient); }
  ਖਾਲੀ radial_gradient (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                        hb_color_ਸਤਰ_t *color_ਸਤਰ,
                        ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
                        ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1)
  { func.radial_gradient (ਇਹ, paint_ਅੰਕੜੇ,
                          color_ਸਤਰ, x0, y0, r0, x1, y1, r1,
                          !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->radial_gradient); }
  ਖਾਲੀ sweep_gradient (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                       hb_color_ਸਤਰ_t *color_ਸਤਰ,
                       ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
                       ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle,
                       ਦਸ਼ਮਲਵ ਸਿਰਾ_angle)
  { func.sweep_gradient (ਇਹ, paint_ਅੰਕੜੇ,
                         color_ਸਤਰ, x0, y0, ਸ਼ੁਰੂ_angle, ਸਿਰਾ_angle,
                         !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->sweep_gradient); }
  ਖਾਲੀ ਧੱਕੋ_ਸਮੂਹ (ਖਾਲੀ *paint_ਅੰਕੜੇ)
  { func.ਧੱਕੋ_ਸਮੂਹ (ਇਹ, paint_ਅੰਕੜੇ,
                     !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਧੱਕੋ_ਸਮੂਹ); }
  ਖਾਲੀ ਧੱਕੋ_ਸਮੂਹ_for (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                       hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
  { func.ਧੱਕੋ_ਸਮੂਹ_for (ਇਹ, paint_ਅੰਕੜੇ,
                         ਬਹੁਲਕ,
                         !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਧੱਕੋ_ਸਮੂਹ_for); }
  ਖਾਲੀ ਕੱਢੋ_ਸਮੂਹ (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                  hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
  { func.ਕੱਢੋ_ਸਮੂਹ (ਇਹ, paint_ਅੰਕੜੇ,
                    ਬਹੁਲਕ,
                    !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਕੱਢੋ_ਸਮੂਹ); }
  ਤਾਰਕਿਕ custom_palette_color (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ color_ਸੂਚਕਾਂਕ,
                             hb_color_t *color)
  { ਵਾਪਸ func.custom_palette_color (ਇਹ, paint_ਅੰਕੜੇ,
                                      color_ਸੂਚਕਾਂਕ,
                                      color,
                                      !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->custom_palette_color); }
  ਤਾਰਕਿਕ ਸੈੱਟ_budget (ਖਾਲੀ *paint_ਅੰਕੜੇ, int64_t budget)
  {
    ਜੇ (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    ਵਾਪਸ func.ਸੈੱਟ_budget (ਇਹ, paint_ਅੰਕੜੇ, budget,
			    !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->ਸੈੱਟ_budget);
  }
  int64_t get_budget (ਖਾਲੀ *paint_ਅੰਕੜੇ)
  { ਵਾਪਸ func.get_budget (ਇਹ, paint_ਅੰਕੜੇ,
			    !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->get_budget); }
  int64_t *get_budget_ਬਚਿਆ_ptr (ਖਾਲੀ *paint_ਅੰਕੜੇ)
  { ਵਾਪਸ func.get_budget_ਬਚਿਆ (ਇਹ, paint_ਅੰਕੜੇ,
				      !user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : user_ਅੰਕੜੇ->get_budget_ਬਚਿਆ); }


  /* Internal specializations. */

  ਖਾਲੀ ਧੱਕੋ_font_transform (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                            ਅਚਲ hb_font_t *font)
  {
    ਦਸ਼ਮਲਵ upem = font->face->get_upem ();
    ਪੂਰਨ xscale = font->x_scale, yscale = font->y_scale;

    ਧੱਕੋ_transform (paint_ਅੰਕੜੇ,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  ਖਾਲੀ ਧੱਕੋ_inverse_font_transform (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                    ਅਚਲ hb_font_t *font)
  {
    ਦਸ਼ਮਲਵ upem = font->face->get_upem ();
    ਪੂਰਨ xscale = font->x_scale ? font->x_scale : upem;
    ਪੂਰਨ yscale = font->y_scale ? font->y_scale : upem;

    ਧੱਕੋ_transform (paint_ਅੰਕੜੇ,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  ਖਾਲੀ ਧੱਕੋ_transform (ਖਾਲੀ *paint_ਅੰਕੜੇ, hb_transform_t<ਦਸ਼ਮਲਵ> t)
  {
    ਧੱਕੋ_transform (paint_ਅੰਕੜੇ, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  ਖਾਲੀ ਧੱਕੋ_translate (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                       ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy)
  {
    ਧੱਕੋ_transform (paint_ਅੰਕੜੇ,
		    hb_transform_t<ਦਸ਼ਮਲਵ>::translation (dx, dy));
  }

  ਖਾਲੀ ਧੱਕੋ_scale (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                   ਦਸ਼ਮਲਵ sx, ਦਸ਼ਮਲਵ sy)
  {
    ਧੱਕੋ_transform (paint_ਅੰਕੜੇ,
		    hb_transform_t<ਦਸ਼ਮਲਵ>::scaling (sx, sy));
  }
  ਖਾਲੀ ਧੱਕੋ_scale_around_center (ਖਾਲੀ *paint_ਅੰਕੜੇ,
				 ਦਸ਼ਮਲਵ sx, ਦਸ਼ਮਲਵ sy,
				 ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy)
  {
    ਧੱਕੋ_transform (paint_ਅੰਕੜੇ,
		    hb_transform_t<ਦਸ਼ਮਲਵ>::scaling_around_center (sx, sy, cx, cy));
  }

  ਖਾਲੀ ਧੱਕੋ_rotate (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                    ਦਸ਼ਮਲਵ a)
  {
    ਧੱਕੋ_transform (paint_ਅੰਕੜੇ,
		    hb_transform_t<ਦਸ਼ਮਲਵ>::rotation (a * HB_PI));
  }

  ਖਾਲੀ ਧੱਕੋ_rotate_around_center (ਖਾਲੀ *paint_ਅੰਕੜੇ,
				  ਦਸ਼ਮਲਵ a,
				  ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy)
  {
    ਧੱਕੋ_transform (paint_ਅੰਕੜੇ,
		    hb_transform_t<ਦਸ਼ਮਲਵ>::rotation_around_center (a * HB_PI, cx, cy));
  }

  ਖਾਲੀ ਧੱਕੋ_skew (ਖਾਲੀ *paint_ਅੰਕੜੇ,
                  ਦਸ਼ਮਲਵ sx, ਦਸ਼ਮਲਵ sy)
  {
    ਧੱਕੋ_transform (paint_ਅੰਕੜੇ,
		    hb_transform_t<ਦਸ਼ਮਲਵ>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  ਖਾਲੀ ਧੱਕੋ_skew_around_center (ਖਾਲੀ *paint_ਅੰਕੜੇ,
				ਦਸ਼ਮਲਵ sx, ਦਸ਼ਮਲਵ sy,
				ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy)
  {
    ਧੱਕੋ_transform (paint_ਅੰਕੜੇ,
		    hb_transform_t<ਦਸ਼ਮਲਵ>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
ਸਥਿਰ ਸਤਰਅੰਦਰ hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, ਦਸ਼ਮਲਵ t)
{
  ਸਵੈਚਾਲਿਤ lerp = [&] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਿਸਕਾਓ) -> ਬਿਨਾਨਿਸ਼ਾਨੀ {
    ਬਿਨਾਨਿਸ਼ਾਨੀ v0 = (c0 >> ਖਿਸਕਾਓ) & 0xFF;
    ਬਿਨਾਨਿਸ਼ਾਨੀ v1 = (c1 >> ਖਿਸਕਾਓ) & 0xFF;
    ਦਸ਼ਮਲਵ v = v0 + t * ((ਦਸ਼ਮਲਵ) v1 - (ਦਸ਼ਮਲਵ) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    ਜੇ (!(v > 0.f)) ਵਾਪਸ 0;
    ਜੇ (v > 255.f) ਵਾਪਸ 255;
    ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ) v;
  };
  ਵਾਪਸ HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#ਜੇ_ਅੰਤ /* HB_PAINT_HH */
