/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2011  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_FONT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-face.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-atomic.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint-extents.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-outline.hh"


/*
 * hb_font_funcs_t
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_FONT_FUNC_IMPLEMENT (get_,font_h_extents) \
  HB_FONT_FUNC_IMPLEMENT (get_,font_v_extents) \
  HB_FONT_FUNC_IMPLEMENT (get_,nominal_glyph) \
  HB_FONT_FUNC_IMPLEMENT (get_,nominal_glyphs) \
  HB_FONT_FUNC_IMPLEMENT (get_,variation_glyph) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_h_advance) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_v_advance) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_h_advances) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_v_advances) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_h_origin) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_v_origin) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_h_origins) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_v_origins) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_h_kerning) \
  HB_IF_NOT_DEPRECATED (HB_FONT_FUNC_IMPLEMENT (get_,glyph_v_kerning)) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_extents) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_contour_ਬਿੰਦੂ) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_name) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_from_name) \
  HB_FONT_FUNC_IMPLEMENT (,draw_glyph_or_fail) \
  HB_FONT_FUNC_IMPLEMENT (,paint_glyph_or_fail) \
  /* ^--- Add new callbacks here */

ਬਣਤਰ hb_font_funcs_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_FUNC_IMPLEMENT(get_,name) ਖਾਲੀ *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_FONT_FUNC_IMPLEMENT
  } *user_ਅੰਕੜੇ;

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  ਸੰਘ get_t {
    ਬਣਤਰ get_funcs_t {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_FONT_FUNC_IMPLEMENT
    } f;
    ਖਾਲੀ (*array[0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_FUNC_IMPLEMENT(get_,name) +1
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_FONT_FUNC_IMPLEMENT
		]) ();
  } get;
};
DECLARE_NULL_INSTANCE (hb_font_funcs_t);


/*
 * hb_font_t
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_ਅੰਕੜੇ_INSTANTIATE_SHAPERS(shaper, font);
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT

ਬਣਤਰ hb_font_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;
  hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> serial;
  hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> serial_coords;

  hb_font_t *ਮਾਪਾ;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  ਤਾਰਕਿਕ is_synthetic;

  ਦਸ਼ਮਲਵ x_embolden;
  ਦਸ਼ਮਲਵ y_embolden;
  ਤਾਰਕਿਕ embolden_ਵਿੱਚ_ਰੱਖੋ;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  ਦਸ਼ਮਲਵ slant;
  ਦਸ਼ਮਲਵ slant_xy;

  ਦਸ਼ਮਲਵ x_multf;
  ਦਸ਼ਮਲਵ y_multf;
  int64_t x_mult;
  int64_t y_mult;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ x_ppem;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ y_ppem;

  ਦਸ਼ਮਲਵ ptem;

  /* Font variation coordinates. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instance_ਸੂਚਕਾਂਕ;
  ਤਾਰਕਿਕ has_nonzero_coords;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_coords;
  ਪੂਰਨ *coords;
  ਦਸ਼ਮਲਵ *design_coords;

  hb_font_funcs_t   *klass;
  ਖਾਲੀ              *user_ਅੰਕੜੇ;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> ਅੰਕੜੇ; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { ਵਾਪਸ HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { ਵਾਪਸ em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { ਵਾਪਸ em_mult (v, y_mult); }
  hb_position_t em_scalef_x (ਦਸ਼ਮਲਵ v) { ਵਾਪਸ em_multf (v, x_multf); }
  hb_position_t em_scalef_y (ਦਸ਼ਮਲਵ v) { ਵਾਪਸ em_multf (v, y_multf); }
  ਦਸ਼ਮਲਵ em_fscale_x (int32_t v) { ਵਾਪਸ em_fmult (v, x_multf); }
  ਦਸ਼ਮਲਵ em_fscale_y (int32_t v) { ਵਾਪਸ em_fmult (v, y_multf); }
  ਦਸ਼ਮਲਵ em_fscalef_x (ਦਸ਼ਮਲਵ v) { ਵਾਪਸ em_fmultf (v, x_multf); }
  ਦਸ਼ਮਲਵ em_fscalef_y (ਦਸ਼ਮਲਵ v) { ਵਾਪਸ em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { ਵਾਪਸ em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t ਮਾਪਾ_scale_x_ਦੂਰੀ (hb_position_t v)
  {
    ਜੇ (unlikely (ਮਾਪਾ && ਮਾਪਾ->x_scale && ਮਾਪਾ->x_scale != x_scale))
      ਵਾਪਸ (hb_position_t) (v * (int64_t) ਇਹ->x_scale / ਇਹ->ਮਾਪਾ->x_scale);
    ਵਾਪਸ v;
  }
  hb_position_t ਮਾਪਾ_scale_y_ਦੂਰੀ (hb_position_t v)
  {
    ਜੇ (unlikely (ਮਾਪਾ && ਮਾਪਾ->y_scale && ਮਾਪਾ->y_scale != y_scale))
      ਵਾਪਸ (hb_position_t) (v * (int64_t) ਇਹ->y_scale / ਇਹ->ਮਾਪਾ->y_scale);
    ਵਾਪਸ v;
  }
  hb_position_t ਮਾਪਾ_scale_x_position (hb_position_t v)
  { ਵਾਪਸ ਮਾਪਾ_scale_x_ਦੂਰੀ (v); }
  hb_position_t ਮਾਪਾ_scale_y_position (hb_position_t v)
  { ਵਾਪਸ ਮਾਪਾ_scale_y_ਦੂਰੀ (v); }

  ਖਾਲੀ ਮਾਪਾ_scale_ਦੂਰੀ (hb_position_t *x, hb_position_t *y)
  {
    *x = ਮਾਪਾ_scale_x_ਦੂਰੀ (*x);
    *y = ਮਾਪਾ_scale_y_ਦੂਰੀ (*y);
  }
  ਖਾਲੀ ਮਾਪਾ_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = ਮਾਪਾ_scale_x_position (*x);
    *y = ਮਾਪਾ_scale_y_position (*y);
  }

  ਖਾਲੀ scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    ਦਸ਼ਮਲਵ x1 = em_fscale_x (extents->x_bearing);
    ਦਸ਼ਮਲਵ y1 = em_fscale_y (extents->y_bearing);
    ਦਸ਼ਮਲਵ x2 = x1 + em_fscale_x (extents->ਚੌੜਾਈ);
    ਦਸ਼ਮਲਵ y2 = y1 + em_fscale_y (extents->ਉਚਾਈ);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->ਚੌੜਾਈ = ceilf (x2) - extents->x_bearing;
    extents->ਉਚਾਈ = ceilf (y2) - extents->y_bearing;
  }

  ਖਾਲੀ synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    ਜੇ (slant_xy)
    {
      /* x_bearing/width and y_bearing/height are each clamped to the full
       * hb_position_t range independently by the extent producers, so their
       * sums and differences can overflow int32.  Accumulate in int64. */
      int64_t y1 = extents->y_bearing;
      int64_t y2 = (int64_t) extents->y_bearing + extents->ਉਚਾਈ;

      int64_t x1 = (int64_t) extents->x_bearing + (int64_t) floorf (hb_min (y1 * slant_xy, y2 * slant_xy));
      int64_t x2 = (int64_t) extents->x_bearing + extents->ਚੌੜਾਈ + (int64_t) ceilf (hb_max (y1 * slant_xy, y2 * slant_xy));

      extents->x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (x1);
      extents->ਚੌੜਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (x2 - x1);
    }

    /* Embolden. */
    ਜੇ (x_strength || y_strength)
    {
      /* Y */
      ਪੂਰਨ y_ਖਿਸਕਾਓ = y_strength;
      ਜੇ (y_scale < 0) y_ਖਿਸਕਾਓ = -y_ਖਿਸਕਾਓ;
      extents->y_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) extents->y_bearing + y_ਖਿਸਕਾਓ);
      extents->ਉਚਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) extents->ਉਚਾਈ - y_ਖਿਸਕਾਓ);

      /* X */
      ਪੂਰਨ x_ਖਿਸਕਾਓ = x_strength;
      ਜੇ (x_scale < 0) x_ਖਿਸਕਾਓ = -x_ਖਿਸਕਾਓ;
      ਜੇ (embolden_ਵਿੱਚ_ਰੱਖੋ)
	extents->x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) extents->x_bearing - x_ਖਿਸਕਾਓ / 2);
      extents->ਚੌੜਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) extents->ਚੌੜਾਈ + x_ਖਿਸਕਾਓ);
    }
  }


  /* Public getters */

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ has_func (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i);
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ has_func_ਸੈੱਟ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i);

  /* has_* ... */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_FUNC_IMPLEMENT(get_,name) \
  ਤਾਰਕਿਕ \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = ਇਹ->klass; \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / ਆਕਾਰ (funcs->get.array[0]); \
    ਵਾਪਸ has_func (i); \
  } \
  ਤਾਰਕਿਕ \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = ਇਹ->klass; \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / ਆਕਾਰ (funcs->get.array[0]); \
    ਵਾਪਸ has_func_ਸੈੱਟ (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
    hb_memset (extents, 0, ਆਕਾਰ (*extents));
    ਤਾਰਕਿਕ ret = klass->get.f.font_h_extents (ਇਹ, user_ਅੰਕੜੇ,
					    extents,
					    !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->font_h_extents);

    ਜੇ (synthetic && ret)
    {
      /* Embolden */
      ਪੂਰਨ y_ਖਿਸਕਾਓ = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_ਖਿਸਕਾਓ;
    }

    ਵਾਪਸ ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
    hb_memset (extents, 0, ਆਕਾਰ (*extents));
    ਤਾਰਕਿਕ ret = klass->get.f.font_v_extents (ਇਹ, user_ਅੰਕੜੇ,
					    extents,
					    !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->font_v_extents);

    ਜੇ (synthetic && ret)
    {
      /* Embolden */
      ਪੂਰਨ x_ਖਿਸਕਾਓ = x_scale < 0 ? -x_strength : x_strength;
      ਜੇ (embolden_ਵਿੱਚ_ਰੱਖੋ)
      {
	extents->ascender += x_ਖਿਸਕਾਓ / 2;
	extents->descender -= x_ਖਿਸਕਾਓ - x_ਖਿਸਕਾਓ / 2;
      }
      ਨਹੀਂਤਾਂ
	extents->ascender += x_ਖਿਸਕਾਓ;
    }

    ਵਾਪਸ ret;
  }

  ਤਾਰਕਿਕ has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    ਵਾਪਸ get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_ਮਿਲਿਆ = 0)
  {
    *glyph = not_ਮਿਲਿਆ;
    ਵਾਪਸ klass->get.f.nominal_glyph (ਇਹ, user_ਅੰਕੜੇ,
				       unicode, glyph,
				       !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->nominal_glyph);
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_nominal_glyphs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				   ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_unicode,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ unicode_stride,
				   hb_codepoint_t *ਪਹਿਲਾ_glyph,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride)
  {
    ਵਾਪਸ klass->get.f.nominal_glyphs (ਇਹ, user_ਅੰਕੜੇ,
					ਗਿਣਤੀ_2,
					ਪਹਿਲਾ_unicode, unicode_stride,
					ਪਹਿਲਾ_glyph, glyph_stride,
					!klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_ਮਿਲਿਆ = 0)
  {
    *glyph = not_ਮਿਲਿਆ;
    ਵਾਪਸ klass->get.f.variation_glyph (ਇਹ, user_ਅੰਕੜੇ,
					 unicode, variation_selector, glyph,
					 !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (ਇਹ, user_ਅੰਕੜੇ,
							  glyph,
							  !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_h_advance);

    ਜੇ (synthetic && x_strength && !embolden_ਵਿੱਚ_ਰੱਖੋ)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    ਵਾਪਸ advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (ਇਹ, user_ਅੰਕੜੇ,
							  glyph,
							  !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_v_advance);

    ਜੇ (synthetic && y_strength && !embolden_ਵਿੱਚ_ਰੱਖੋ)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    ਵਾਪਸ advance;
  }

  ਖਾਲੀ get_glyph_h_advances (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
			     ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride,
			     hb_position_t *ਪਹਿਲਾ_advance,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ advance_stride,
			     ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
    klass->get.f.glyph_h_advances (ਇਹ, user_ਅੰਕੜੇ,
				   ਗਿਣਤੀ_2,
				   ਪਹਿਲਾ_glyph, glyph_stride,
				   ਪਹਿਲਾ_advance, advance_stride,
				   !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_h_advances);

    ਜੇ (synthetic && x_strength && !embolden_ਵਿੱਚ_ਰੱਖੋ)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	*ਪਹਿਲਾ_advance += *ਪਹਿਲਾ_advance ? strength : 0;
	ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
      }
    }
  }

  ਖਾਲੀ get_glyph_v_advances (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
			     ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride,
			     hb_position_t *ਪਹਿਲਾ_advance,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ advance_stride,
			     ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
    klass->get.f.glyph_v_advances (ਇਹ, user_ਅੰਕੜੇ,
				   ਗਿਣਤੀ_2,
				   ਪਹਿਲਾ_glyph, glyph_stride,
				   ਪਹਿਲਾ_advance, advance_stride,
				   !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_v_advances);

    ਜੇ (synthetic && y_strength && !embolden_ਵਿੱਚ_ਰੱਖੋ)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	*ਪਹਿਲਾ_advance += *ਪਹਿਲਾ_advance ? strength : 0;
	ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
    *x = *y = 0;
    ਤਾਰਕਿਕ ret = klass->get.f.glyph_h_origin (ਇਹ, user_ਅੰਕੜੇ,
					    glyph, x, y,
					    !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_h_origin);

    ਜੇ (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      ਜੇ (!embolden_ਵਿੱਚ_ਰੱਖੋ)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    ਵਾਪਸ ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
    *x = *y = 0;
    ਤਾਰਕਿਕ ret = klass->get.f.glyph_v_origin (ਇਹ, user_ਅੰਕੜੇ,
					    glyph, x, y,
					    !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_v_origin);

    ਜੇ (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      ਜੇ (!embolden_ਵਿੱਚ_ਰੱਖੋ)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    ਵਾਪਸ ret;
  }

  hb_bool_t get_glyph_h_origins (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				 ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride,
				 hb_position_t *ਪਹਿਲਾ_x,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ x_stride,
				 hb_position_t *ਪਹਿਲਾ_y,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ y_stride,
				 ਤਾਰਕਿਕ synthetic = ਸੱਚ)

  {
    ਤਾਰਕਿਕ ret = klass->get.f.glyph_h_origins (ਇਹ, user_ਅੰਕੜੇ,
					     ਗਿਣਤੀ_2,
					     ਪਹਿਲਾ_glyph, glyph_stride,
					     ਪਹਿਲਾ_x, x_stride, ਪਹਿਲਾ_y, y_stride,
					     !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_h_origins);

    ਜੇ (synthetic && ret)
    {
      hb_position_t x_ਖਿਸਕਾਓ = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_ਖਿਸਕਾਓ = y_scale < 0 ? -y_strength : y_strength;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	ਜੇ (!embolden_ਵਿੱਚ_ਰੱਖੋ)
	{
	  *ਪਹਿਲਾ_x += x_ਖਿਸਕਾਓ;
	  *ਪਹਿਲਾ_y += y_ਖਿਸਕਾਓ;
	}
	ਪਹਿਲਾ_x = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_x, x_stride);
	ਪਹਿਲਾ_y = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_y, y_stride);
      }
    }

    ਵਾਪਸ ret;
  }

  hb_bool_t get_glyph_v_origins (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				 ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride,
				 hb_position_t *ਪਹਿਲਾ_x,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ x_stride,
				 hb_position_t *ਪਹਿਲਾ_y,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ y_stride,
				 ਤਾਰਕਿਕ synthetic = ਸੱਚ)

  {
    ਤਾਰਕਿਕ ret = klass->get.f.glyph_v_origins (ਇਹ, user_ਅੰਕੜੇ,
					     ਗਿਣਤੀ_2,
					     ਪਹਿਲਾ_glyph, glyph_stride,
					     ਪਹਿਲਾ_x, x_stride, ਪਹਿਲਾ_y, y_stride,
					     !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_v_origins);

    ਜੇ (synthetic && is_synthetic && ret)
    {
      hb_position_t x_ਖਿਸਕਾਓ = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_ਖਿਸਕਾਓ = y_scale < 0 ? -y_strength : y_strength;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	ਜੇ (!embolden_ਵਿੱਚ_ਰੱਖੋ)
	{
	  *ਪਹਿਲਾ_x += x_ਖਿਸਕਾਓ;
	  *ਪਹਿਲਾ_y += y_ਖਿਸਕਾਓ;
	}
	ਪਹਿਲਾ_x = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_x, x_stride);
	ਪਹਿਲਾ_y = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_y, y_stride);
      }
    }

    ਵਾਪਸ ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t ਖੱਬਾ_glyph,
				     hb_codepoint_t ਸੱਜਾ_glyph)
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
    ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ klass->get.f.glyph_h_kerning (ਇਹ, user_ਅੰਕੜੇ,
					 ਖੱਬਾ_glyph, ਸੱਜਾ_glyph,
					 !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_h_kerning);
#ਜੇ_ਅੰਤ
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t ਸਿਖਰ_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
    ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ klass->get.f.glyph_v_kerning (ਇਹ, user_ਅੰਕੜੇ,
					 ਸਿਖਰ_glyph, bottom_glyph,
					 !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_v_kerning);
#ਜੇ_ਅੰਤ
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
    hb_memset (extents, 0, ਆਕਾਰ (*extents));

    /* This is rather messy, but necessary. */

    ਜੇ (!synthetic)
    {
      ਵਾਪਸ klass->get.f.glyph_extents (ਇਹ, user_ਅੰਕੜੇ,
					 glyph,
					 extents,
					 !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_extents);
    }
    ਜੇ (!is_synthetic &&
	klass->get.f.glyph_extents (ਇਹ, user_ਅੰਕੜੇ,
				    glyph,
				    extents,
				    !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_extents))
      ਵਾਪਸ ਸੱਚ;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    ਜੇ (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().ਮੰਜ਼ਿਲ_glyph_extents ();
      ਵਾਪਸ ਸੱਚ;
    }
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW
    hb_extents_t<> draw_extents;
    ਜੇ (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.ਮੰਜ਼ਿਲ_glyph_extents ();
      ਵਾਪਸ ਸੱਚ;
    }
#ਜੇ_ਅੰਤ

    ਤਾਰਕਿਕ ret = klass->get.f.glyph_extents (ਇਹ, user_ਅੰਕੜੇ,
					   glyph,
					   extents,
					   !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_extents);
    ਜੇ (ret)
      synthetic_glyph_extents (extents);

    ਵਾਪਸ ret;
  }

  hb_bool_t get_glyph_contour_ਬਿੰਦੂ (hb_codepoint_t glyph, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੰਦੂ_ਸੂਚਕਾਂਕ,
				     hb_position_t *x, hb_position_t *y,
				     ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
    *x = *y = 0;
    ਤਾਰਕਿਕ ret = klass->get.f.glyph_contour_ਬਿੰਦੂ (ਇਹ, user_ਅੰਕੜੇ,
						 glyph, ਬਿੰਦੂ_ਸੂਚਕਾਂਕ,
						 x, y,
						 !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_contour_ਬਿੰਦੂ);

    ਜੇ (synthetic && ret)
    {
      /* Slant */
      ਜੇ (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      ਜੇ (!embolden_ਵਿੱਚ_ਰੱਖੋ)
      {
	ਪੂਰਨ x_ਖਿਸਕਾਓ = x_scale < 0 ? -x_strength : x_strength;
	*x += x_ਖਿਸਕਾਓ;
      }
    }

    ਵਾਪਸ ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    ਅੱਖਰ *name, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
  {
    ਜੇ (ਮਾਪ) *name = '\0';
    ਵਾਪਸ klass->get.f.glyph_name (ਇਹ, user_ਅੰਕੜੇ,
				    glyph,
				    name, ਮਾਪ,
				    !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_name);
  }

  hb_bool_t get_glyph_from_name (ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    ਜੇ (len == -1) len = ਲੜੀ_ਲੰਬਾਈ (name);
    ਵਾਪਸ klass->get.f.glyph_from_name (ਇਹ, user_ਅੰਕੜੇ,
					 name, len,
					 glyph,
					 !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->glyph_from_name);
  }

  ਤਾਰਕਿਕ draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			   ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OUTLINE
    ਤਾਰਕਿਕ embolden = x_strength || y_strength;
    ਤਾਰਕਿਕ slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#ਨਹੀਂਤਾਂ
    synthetic = ਝੂਠ;
#ਜੇ_ਅੰਤ

    ਜੇ (!synthetic)
    {
      ਵਾਪਸ klass->get.f.draw_glyph_or_fail (ਇਹ, user_ਅੰਕੜੇ,
					      glyph,
					      draw_funcs, draw_ਅੰਕੜੇ,
					      !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->draw_glyph_or_fail);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_ਬਚਿਆ = draw_funcs->get_budget_ਬਚਿਆ_ptr (draw_ਅੰਕੜੇ);
    ਜੇ (!klass->get.f.draw_glyph_or_fail (ਇਹ, user_ਅੰਕੜੇ,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->draw_glyph_or_fail))
      ਵਾਪਸ ਝੂਠ;

    // Slant before embolden; produces nicer results.

    ਜੇ (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, ਝੂਠ);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    ਜੇ (embolden)
    {
      ਦਸ਼ਮਲਵ x_ਖਿਸਕਾਓ = embolden_ਵਿੱਚ_ਰੱਖੋ ? 0 : (ਦਸ਼ਮਲਵ) x_strength / 2;
      ਦਸ਼ਮਲਵ y_ਖਿਸਕਾਓ = (ਦਸ਼ਮਲਵ) y_strength / 2;
      ਜੇ (x_scale < 0) x_ਖਿਸਕਾਓ = -x_ਖਿਸਕਾਓ;
      ਜੇ (y_scale < 0) y_ਖਿਸਕਾਓ = -y_ਖਿਸਕਾਓ;
      outline.embolden (x_strength, y_strength, x_ਖਿਸਕਾਓ, y_ਖਿਸਕਾਓ);
    }

    outline.replay (draw_funcs, draw_ਅੰਕੜੇ);

    ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette,
			    hb_color_t foreground,
			    ਤਾਰਕਿਕ synthetic = ਸੱਚ)
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT
    /* Slant */
    ਜੇ (synthetic && slant_xy)
      hb_paint_ਧੱਕੋ_transform (paint_funcs, paint_ਅੰਕੜੇ,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    ਤਾਰਕਿਕ ret = klass->get.f.paint_glyph_or_fail (ਇਹ, user_ਅੰਕੜੇ,
						 glyph,
						 paint_funcs, paint_ਅੰਕੜੇ,
						 palette, foreground,
						 !klass->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : klass->user_ਅੰਕੜੇ->paint_glyph_or_fail);

    ਜੇ (synthetic && slant_xy)
      hb_paint_ਕੱਢੋ_transform (paint_funcs, paint_ਅੰਕੜੇ);

    ਵਾਪਸ ret;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਝੂਠ;
  }

  /* A bit higher-level, and with fallback */

  HB_ਅੰਦਰੂਨੀ
  ਖਾਲੀ paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette,
		    hb_color_t foreground);

  ਖਾਲੀ get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    ਜੇ (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->ਸਤਰ_gap = 0;
    }
  }
  ਖਾਲੀ get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    ਜੇ (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->ਸਤਰ_gap = 0;
    }
  }

  ਖਾਲੀ get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    ਜੇ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    ਨਹੀਂਤਾਂ
      get_v_extents_with_fallback (extents);
  }

  ਖਾਲੀ get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    ਜੇ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    ਨਹੀਂਤਾਂ
      *y = get_glyph_v_advance (glyph);
  }
  ਖਾਲੀ get_glyph_advances_for_direction (hb_direction_t direction,
					 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
					 ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
					 ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride,
					 hb_position_t *ਪਹਿਲਾ_advance,
					 ਬਿਨਾਨਿਸ਼ਾਨੀ advance_stride)
  {
    ਜੇ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (ਗਿਣਤੀ_2, ਪਹਿਲਾ_glyph, glyph_stride, ਪਹਿਲਾ_advance, advance_stride);
    ਨਹੀਂਤਾਂ
      get_glyph_v_advances (ਗਿਣਤੀ_2, ਪਹਿਲਾ_glyph, glyph_stride, ਪਹਿਲਾ_advance, advance_stride);
  }

  ਖਾਲੀ apply_ਵਿੱਥ (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     ਨਿਸ਼ਾਨੀਵਾਲਾ mult)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  ਖਾਲੀ ਜੋੜੋ_ਵਿੱਥ (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  ਖਾਲੀ ਘਟਾਓ_ਵਿੱਥ (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  ਖਾਲੀ guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  ਖਾਲੀ apply_glyph_h_origins_with_fallback (hb_ਬਫਰ_t *buf, ਪੂਰਨ mult)
  {
    ਤਾਰਕਿਕ has_ascender = ਝੂਠ;
    hb_position_t ascender = 0;

    ਬਣਤਰ { hb_position_t x, y; } origins[32];

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = buf->len;
    ਜਦੋਂ (ਵਿੱਥ < ਗਿਣਤੀ_2)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ n = hb_min (ਗਿਣਤੀ_2 - ਵਿੱਥ, ARRAY_ਲੰਮਾਈ (origins));
      ਜੇ (!get_glyph_h_origins (n,
				&buf->info[ਵਿੱਥ].codepoint, ਆਕਾਰ (hb_glyph_info_t),
				&origins[0].x, ਆਕਾਰ (origins[0]),
				&origins[0].y, ਆਕਾਰ (origins[0])))
      {
        ਜੇ (get_glyph_v_origins (n,
				  &buf->info[ਵਿੱਥ].codepoint, ਆਕਾਰ (hb_glyph_info_t),
				  &origins[0].x, ਆਕਾਰ (origins[0]),
				  &origins[0].y, ਆਕਾਰ (origins[0])))
	{
	  ਜੇ (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = ਸੱਚ;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[ਵਿੱਥ + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      ਪ੍ਰਤਿਜ੍ਞਾ (mult == -1 || mult == +1 || mult == 0);
      ਜੇ (mult == +1)
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[ਵਿੱਥ + j];
	  ਜੋੜੋ_ਵਿੱਥ (&pos->x_ਵਿੱਥ_2, &pos->y_ਵਿੱਥ_2,
		      origins[j].x, origins[j].y);
	}
      ਨਹੀਂਤਾਂ ਜੇ (mult == -1)
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[ਵਿੱਥ + j];
	  ਘਟਾਓ_ਵਿੱਥ (&pos->x_ਵਿੱਥ_2, &pos->y_ਵਿੱਥ_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      ਵਿੱਥ += n;
    }
  }
  ਖਾਲੀ apply_glyph_v_origins_with_fallback (hb_ਬਫਰ_t *buf, ਪੂਰਨ mult)
  {
    ਤਾਰਕਿਕ has_ascender = ਝੂਠ;
    hb_position_t ascender = 0;

    ਬਣਤਰ { hb_position_t x, y; } origins[32];

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = buf->len;
    ਜਦੋਂ (ਵਿੱਥ < ਗਿਣਤੀ_2)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ n = hb_min (ਗਿਣਤੀ_2 - ਵਿੱਥ, ARRAY_ਲੰਮਾਈ (origins));
      ਜੇ (!get_glyph_v_origins (n,
				&buf->info[ਵਿੱਥ].codepoint, ਆਕਾਰ (hb_glyph_info_t),
				&origins[0].x, ਆਕਾਰ (origins[0]),
				&origins[0].y, ਆਕਾਰ (origins[0])))
      {
	ਜੇ (get_glyph_h_origins (n,
				 &buf->info[ਵਿੱਥ].codepoint, ਆਕਾਰ (hb_glyph_info_t),
				 &origins[0].x, ਆਕਾਰ (origins[0]),
				 &origins[0].y, ਆਕਾਰ (origins[0])))
	{
	  ਜੇ (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = ਸੱਚ;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[ਵਿੱਥ + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      ਪ੍ਰਤਿਜ੍ਞਾ (mult == -1 || mult == +1 || mult == 0);
      ਜੇ (mult == +1)
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[ਵਿੱਥ + j];
	  ਜੋੜੋ_ਵਿੱਥ (&pos->x_ਵਿੱਥ_2, &pos->y_ਵਿੱਥ_2,
		      origins[j].x, origins[j].y);
	}
      ਨਹੀਂਤਾਂ ਜੇ (mult == -1)
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[ਵਿੱਥ + j];
	  ਘਟਾਓ_ਵਿੱਥ (&pos->x_ਵਿੱਥ_2, &pos->y_ਵਿੱਥ_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      ਵਿੱਥ += n;
    }
  }

  ਖਾਲੀ get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    ਜੇ (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      ਘਟਾਓ_ਵਿੱਥ (x, y, dx, dy);
    }
  }
  ਖਾਲੀ get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    ਜੇ (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      ਜੋੜੋ_ਵਿੱਥ (x, y, dx, dy);
    }
  }

  ਖਾਲੀ get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    ਜੇ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    ਨਹੀਂਤਾਂ
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  ਖਾਲੀ ਜੋੜੋ_glyph_h_origins (hb_ਬਫਰ_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  ਖਾਲੀ ਜੋੜੋ_glyph_v_origins (hb_ਬਫਰ_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  ਖਾਲੀ ਜੋੜੋ_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    ਜੋੜੋ_ਵਿੱਥ (x, y, origin_x, origin_y);
  }

  ਖਾਲੀ ਘਟਾਓ_glyph_h_origins (hb_ਬਫਰ_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  ਖਾਲੀ ਘਟਾਓ_glyph_v_origins (hb_ਬਫਰ_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  ਖਾਲੀ ਘਟਾਓ_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    ਘਟਾਓ_ਵਿੱਥ (x, y, origin_x, origin_y);
  }

  ਖਾਲੀ get_glyph_kerning_for_direction (hb_codepoint_t ਪਹਿਲਾ_glyph, hb_codepoint_t ਦੂਜਾ_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    ਜੇ (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (ਪਹਿਲਾ_glyph, ਦੂਜਾ_glyph);
    } ਨਹੀਂਤਾਂ {
      *x = 0;
      *y = get_glyph_v_kerning (ਪਹਿਲਾ_glyph, ਦੂਜਾ_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    ਜੇ (ret)
      ਘਟਾਓ_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    ਵਾਪਸ ret;
  }

  hb_bool_t get_glyph_contour_ਬਿੰਦੂ_for_origin (hb_codepoint_t glyph, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੰਦੂ_ਸੂਚਕਾਂਕ,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_ਬਿੰਦੂ (glyph, ਬਿੰਦੂ_ਸੂਚਕਾਂਕ, x, y);

    ਜੇ (ret)
      ਘਟਾਓ_glyph_origin_for_direction (glyph, direction, x, y);

    ਵਾਪਸ ret;
  }

  /* Generates gidDDD if glyph has no name. */
  ਖਾਲੀ
  glyph_ਮੰਜ਼ਿਲ_string (hb_codepoint_t glyph,
		   ਅੱਖਰ *s, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
  {
    ਜੇ (get_glyph_name (glyph, s, ਮਾਪ)) ਵਾਪਸ;

    ਜੇ (ਮਾਪ && snprintf (s, ਮਾਪ, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (ਅਚਲ ਅੱਖਰ *s, ਪੂਰਨ len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    ਜੇ (get_glyph_from_name (s, len, glyph)) ਵਾਪਸ ਸੱਚ;

    ਜੇ (len == -1) len = ਲੜੀ_ਲੰਬਾਈ (s);

    /* Straight glyph index. */
    ਜੇ (hb_codepoint_parse (s, len, 10, glyph))
      ਵਾਪਸ ਸੱਚ;

    ਜੇ (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      ਜੇ (0 == ਲੜੀ_ਤੁਲਨਾ (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	ਵਾਪਸ ਸੱਚ;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      ਜੇ (0 == ਲੜੀ_ਤੁਲਨਾ (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	ਵਾਪਸ ਸੱਚ;
    }

    ਵਾਪਸ ਝੂਠ;
  }

  ਖਾਲੀ ਬਦਲਿਆ ()
  {
    ਦਸ਼ਮਲਵ upem = face->get_upem ();

    x_multf = x_scale / upem;
    y_multf = y_scale / upem;
    x_mult = (int64_t) x_scale * 0x10000 / upem;
    y_mult = (int64_t) y_scale * 0x10000 / upem;

    is_synthetic =  x_embolden || y_embolden || slant;

    x_strength = roundf (ਨਿਰਪੇਖ (x_scale) * x_embolden);
    y_strength = roundf (ਨਿਰਪੇਖ (y_scale) * y_embolden);

    slant_xy = y_scale ? slant * x_scale / y_scale : 0.f;

    ਅੰਕੜੇ.fini ();

    serial++;
  }

  hb_position_t em_mult (int32_t v, int64_t mult)
  { ਵਾਪਸ (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (ਦਸ਼ਮਲਵ v, ਦਸ਼ਮਲਵ mult)
  { ਵਾਪਸ (hb_position_t) roundf (em_fmultf (v, mult)); }
  ਦਸ਼ਮਲਵ em_fmultf (ਦਸ਼ਮਲਵ v, ਦਸ਼ਮਲਵ mult)
  { ਵਾਪਸ v * mult; }
  ਦਸ਼ਮਲਵ em_fmult (int32_t v, ਦਸ਼ਮਲਵ mult)
  { ਵਾਪਸ (ਦਸ਼ਮਲਵ) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#ਜੇ_ਅੰਤ /* HB_FONT_HH */
