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

#جيڪڏهن_وصف_نه_ٿيل HB_FONT_HH
#وصف_ڪريو HB_FONT_HH

#ضمن "hb.hh"

#ضمن "hb-face.hh"
#ضمن "hb-atomic.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint-extents.hh"
#ضمن "hb-shaper.hh"
#ضمن "hb-outline.hh"


/*
 * hb_font_funcs_t
 */

#وصف_ڪريو HB_FONT_FUNCS_IMPLEMENT_CALLBACKS \
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
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_contour_نقطو) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_name) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_from_name) \
  HB_FONT_FUNC_IMPLEMENT (,draw_glyph_or_fail) \
  HB_FONT_FUNC_IMPLEMENT (,paint_glyph_or_fail) \
  /* ^--- Add new callbacks here */

بنية hb_font_funcs_t
{
  hb_object_سرو_t سرو;

  بنية {
#وصف_ڪريو HB_FONT_FUNC_IMPLEMENT(get_,name) فراغ *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#وصف_رد HB_FONT_FUNC_IMPLEMENT
  } *user_معلومات;

  بنية {
#وصف_ڪريو HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#وصف_رد HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  اتحاد get_t {
    بنية get_funcs_t {
#وصف_ڪريو HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#وصف_رد HB_FONT_FUNC_IMPLEMENT
    } f;
    فراغ (*array[0
#وصف_ڪريو HB_FONT_FUNC_IMPLEMENT(get_,name) +1
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#وصف_رد HB_FONT_FUNC_IMPLEMENT
		]) ();
  } get;
};
DECLARE_NULL_INSTANCE (hb_font_funcs_t);


/*
 * hb_font_t
 */

#وصف_ڪريو HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_معلومات_INSTANTIATE_SHAPERS(shaper, font);
#ضمن "hb-shaper-list.hh"
#وصف_رد HB_SHAPER_IMPLEMENT

بنية hb_font_t
{
  hb_object_سرو_t سرو;
  hb_atomic_t<بنانشان> serial;
  hb_atomic_t<بنانشان> serial_coords;

  hb_font_t *والد;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  منطقي is_synthetic;

  عائم x_embolden;
  عائم y_embolden;
  منطقي embolden_اندريون_رکو;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  عائم slant;
  عائم slant_xy;

  عائم x_multf;
  عائم y_multf;
  int64_t x_mult;
  int64_t y_mult;

  بنانشان صحيح x_ppem;
  بنانشان صحيح y_ppem;

  عائم ptem;

  /* Font variation coordinates. */
  بنانشان صحيح instance_اشاريو;
  منطقي has_nonzero_coords;
  بنانشان صحيح انگ_coords;
  صحيح *coords;
  عائم *design_coords;

  hb_font_funcs_t   *klass;
  فراغ              *user_معلومات;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> معلومات; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { أرجع HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { أرجع em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { أرجع em_mult (v, y_mult); }
  hb_position_t em_scalef_x (عائم v) { أرجع em_multf (v, x_multf); }
  hb_position_t em_scalef_y (عائم v) { أرجع em_multf (v, y_multf); }
  عائم em_fscale_x (int32_t v) { أرجع em_fmult (v, x_multf); }
  عائم em_fscale_y (int32_t v) { أرجع em_fmult (v, y_multf); }
  عائم em_fscalef_x (عائم v) { أرجع em_fmultf (v, x_multf); }
  عائم em_fscalef_y (عائم v) { أرجع em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { أرجع em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t والد_scale_x_مفاصلو (hb_position_t v)
  {
    إذا (unlikely (والد && والد->x_scale && والد->x_scale != x_scale))
      أرجع (hb_position_t) (v * (int64_t) هذا->x_scale / هذا->والد->x_scale);
    أرجع v;
  }
  hb_position_t والد_scale_y_مفاصلو (hb_position_t v)
  {
    إذا (unlikely (والد && والد->y_scale && والد->y_scale != y_scale))
      أرجع (hb_position_t) (v * (int64_t) هذا->y_scale / هذا->والد->y_scale);
    أرجع v;
  }
  hb_position_t والد_scale_x_position (hb_position_t v)
  { أرجع والد_scale_x_مفاصلو (v); }
  hb_position_t والد_scale_y_position (hb_position_t v)
  { أرجع والد_scale_y_مفاصلو (v); }

  فراغ والد_scale_مفاصلو (hb_position_t *x, hb_position_t *y)
  {
    *x = والد_scale_x_مفاصلو (*x);
    *y = والد_scale_y_مفاصلو (*y);
  }
  فراغ والد_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = والد_scale_x_position (*x);
    *y = والد_scale_y_position (*y);
  }

  فراغ scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    عائم x1 = em_fscale_x (extents->x_bearing);
    عائم y1 = em_fscale_y (extents->y_bearing);
    عائم x2 = x1 + em_fscale_x (extents->ويڪر);
    عائم y2 = y1 + em_fscale_y (extents->اوچائي);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->ويڪر = ceilf (x2) - extents->x_bearing;
    extents->اوچائي = ceilf (y2) - extents->y_bearing;
  }

  فراغ synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    إذا (slant_xy)
    {
      /* x_bearing/width and y_bearing/height are each clamped to the full
       * hb_position_t range independently by the extent producers, so their
       * sums and differences can overflow int32.  Accumulate in int64. */
      int64_t y1 = extents->y_bearing;
      int64_t y2 = (int64_t) extents->y_bearing + extents->اوچائي;

      int64_t x1 = (int64_t) extents->x_bearing + (int64_t) floorf (hb_min (y1 * slant_xy, y2 * slant_xy));
      int64_t x2 = (int64_t) extents->x_bearing + extents->ويڪر + (int64_t) ceilf (hb_max (y1 * slant_xy, y2 * slant_xy));

      extents->x_bearing = hb_clamp_منزل<hb_position_t> (x1);
      extents->ويڪر = hb_clamp_منزل<hb_position_t> (x2 - x1);
    }

    /* Embolden. */
    إذا (x_strength || y_strength)
    {
      /* Y */
      صحيح y_سرڪايو = y_strength;
      إذا (y_scale < 0) y_سرڪايو = -y_سرڪايو;
      extents->y_bearing = hb_clamp_منزل<hb_position_t> ((int64_t) extents->y_bearing + y_سرڪايو);
      extents->اوچائي = hb_clamp_منزل<hb_position_t> ((int64_t) extents->اوچائي - y_سرڪايو);

      /* X */
      صحيح x_سرڪايو = x_strength;
      إذا (x_scale < 0) x_سرڪايو = -x_سرڪايو;
      إذا (embolden_اندريون_رکو)
	extents->x_bearing = hb_clamp_منزل<hb_position_t> ((int64_t) extents->x_bearing - x_سرڪايو / 2);
      extents->ويڪر = hb_clamp_منزل<hb_position_t> ((int64_t) extents->ويڪر + x_سرڪايو);
    }
  }


  /* Public getters */

  HB_اندروني منطقي has_func (بنانشان صحيح i);
  HB_اندروني منطقي has_func_سيٽ (بنانشان صحيح i);

  /* has_* ... */
#وصف_ڪريو HB_FONT_FUNC_IMPLEMENT(get_,name) \
  منطقي \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = هذا->klass; \
    بنانشان صحيح i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / حجم (funcs->get.array[0]); \
    أرجع has_func (i); \
  } \
  منطقي \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = هذا->klass; \
    بنانشان صحيح i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / حجم (funcs->get.array[0]); \
    أرجع has_func_سيٽ (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#وصف_رد HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				منطقي synthetic = سچ)
  {
    hb_memset (extents, 0, حجم (*extents));
    منطقي ret = klass->get.f.font_h_extents (هذا, user_معلومات,
					    extents,
					    !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->font_h_extents);

    إذا (synthetic && ret)
    {
      /* Embolden */
      صحيح y_سرڪايو = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_سرڪايو;
    }

    أرجع ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				منطقي synthetic = سچ)
  {
    hb_memset (extents, 0, حجم (*extents));
    منطقي ret = klass->get.f.font_v_extents (هذا, user_معلومات,
					    extents,
					    !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->font_v_extents);

    إذا (synthetic && ret)
    {
      /* Embolden */
      صحيح x_سرڪايو = x_scale < 0 ? -x_strength : x_strength;
      إذا (embolden_اندريون_رکو)
      {
	extents->ascender += x_سرڪايو / 2;
	extents->descender -= x_سرڪايو - x_سرڪايو / 2;
      }
      وإلا
	extents->ascender += x_سرڪايو;
    }

    أرجع ret;
  }

  منطقي has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    أرجع get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_مليو = 0)
  {
    *glyph = not_مليو;
    أرجع klass->get.f.nominal_glyph (هذا, user_معلومات,
				       unicode, glyph,
				       !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->nominal_glyph);
  }
  بنانشان صحيح get_nominal_glyphs (بنانشان صحيح ڳڻپ,
				   ثابت hb_codepoint_t *پهريون_unicode,
				   بنانشان صحيح unicode_stride,
				   hb_codepoint_t *پهريون_glyph,
				   بنانشان صحيح glyph_stride)
  {
    أرجع klass->get.f.nominal_glyphs (هذا, user_معلومات,
					ڳڻپ,
					پهريون_unicode, unicode_stride,
					پهريون_glyph, glyph_stride,
					!klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_مليو = 0)
  {
    *glyph = not_مليو;
    أرجع klass->get.f.variation_glyph (هذا, user_معلومات,
					 unicode, variation_selector, glyph,
					 !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     منطقي synthetic = سچ)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (هذا, user_معلومات,
							  glyph,
							  !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_h_advance);

    إذا (synthetic && x_strength && !embolden_اندريون_رکو)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    أرجع advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     منطقي synthetic = سچ)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (هذا, user_معلومات,
							  glyph,
							  !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_v_advance);

    إذا (synthetic && y_strength && !embolden_اندريون_رکو)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    أرجع advance;
  }

  فراغ get_glyph_h_advances (بنانشان صحيح ڳڻپ,
			     ثابت hb_codepoint_t *پهريون_glyph,
			     بنانشان صحيح glyph_stride,
			     hb_position_t *پهريون_advance,
			     بنانشان صحيح advance_stride,
			     منطقي synthetic = سچ)
  {
    klass->get.f.glyph_h_advances (هذا, user_معلومات,
				   ڳڻپ,
				   پهريون_glyph, glyph_stride,
				   پهريون_advance, advance_stride,
				   !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_h_advances);

    إذا (synthetic && x_strength && !embolden_اندريون_رکو)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      {
	*پهريون_advance += *پهريون_advance ? strength : 0;
	پهريون_advance = &Struct_At_وٿي_Unaligned<hb_position_t> (پهريون_advance, advance_stride);
      }
    }
  }

  فراغ get_glyph_v_advances (بنانشان صحيح ڳڻپ,
			     ثابت hb_codepoint_t *پهريون_glyph,
			     بنانشان صحيح glyph_stride,
			     hb_position_t *پهريون_advance,
			     بنانشان صحيح advance_stride,
			     منطقي synthetic = سچ)
  {
    klass->get.f.glyph_v_advances (هذا, user_معلومات,
				   ڳڻپ,
				   پهريون_glyph, glyph_stride,
				   پهريون_advance, advance_stride,
				   !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_v_advances);

    إذا (synthetic && y_strength && !embolden_اندريون_رکو)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      {
	*پهريون_advance += *پهريون_advance ? strength : 0;
	پهريون_advance = &Struct_At_وٿي_Unaligned<hb_position_t> (پهريون_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				منطقي synthetic = سچ)
  {
    *x = *y = 0;
    منطقي ret = klass->get.f.glyph_h_origin (هذا, user_معلومات,
					    glyph, x, y,
					    !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_h_origin);

    إذا (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      إذا (!embolden_اندريون_رکو)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				منطقي synthetic = سچ)
  {
    *x = *y = 0;
    منطقي ret = klass->get.f.glyph_v_origin (هذا, user_معلومات,
					    glyph, x, y,
					    !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_v_origin);

    إذا (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      إذا (!embolden_اندريون_رکو)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_h_origins (بنانشان صحيح ڳڻپ,
				 ثابت hb_codepoint_t *پهريون_glyph,
				 بنانشان صحيح glyph_stride,
				 hb_position_t *پهريون_x,
				 بنانشان صحيح x_stride,
				 hb_position_t *پهريون_y,
				 بنانشان صحيح y_stride,
				 منطقي synthetic = سچ)

  {
    منطقي ret = klass->get.f.glyph_h_origins (هذا, user_معلومات,
					     ڳڻپ,
					     پهريون_glyph, glyph_stride,
					     پهريون_x, x_stride, پهريون_y, y_stride,
					     !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_h_origins);

    إذا (synthetic && ret)
    {
      hb_position_t x_سرڪايو = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_سرڪايو = y_scale < 0 ? -y_strength : y_strength;
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	إذا (!embolden_اندريون_رکو)
	{
	  *پهريون_x += x_سرڪايو;
	  *پهريون_y += y_سرڪايو;
	}
	پهريون_x = &Struct_At_وٿي_Unaligned<hb_position_t> (پهريون_x, x_stride);
	پهريون_y = &Struct_At_وٿي_Unaligned<hb_position_t> (پهريون_y, y_stride);
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_v_origins (بنانشان صحيح ڳڻپ,
				 ثابت hb_codepoint_t *پهريون_glyph,
				 بنانشان صحيح glyph_stride,
				 hb_position_t *پهريون_x,
				 بنانشان صحيح x_stride,
				 hb_position_t *پهريون_y,
				 بنانشان صحيح y_stride,
				 منطقي synthetic = سچ)

  {
    منطقي ret = klass->get.f.glyph_v_origins (هذا, user_معلومات,
					     ڳڻپ,
					     پهريون_glyph, glyph_stride,
					     پهريون_x, x_stride, پهريون_y, y_stride,
					     !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_v_origins);

    إذا (synthetic && is_synthetic && ret)
    {
      hb_position_t x_سرڪايو = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_سرڪايو = y_scale < 0 ? -y_strength : y_strength;
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	إذا (!embolden_اندريون_رکو)
	{
	  *پهريون_x += x_سرڪايو;
	  *پهريون_y += y_سرڪايو;
	}
	پهريون_x = &Struct_At_وٿي_Unaligned<hb_position_t> (پهريون_x, x_stride);
	پهريون_y = &Struct_At_وٿي_Unaligned<hb_position_t> (پهريون_y, y_stride);
      }
    }

    أرجع ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t کاٻو_glyph,
				     hb_codepoint_t ساڄو_glyph)
  {
#جيڪڏهن_وصف_ٿيل HB_DISABLE_DEPRECATED
    أرجع 0;
#وإلا
    أرجع klass->get.f.glyph_h_kerning (هذا, user_معلومات,
					 کاٻو_glyph, ساڄو_glyph,
					 !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_h_kerning);
#جيڪڏهن_پورو
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t چوٽي_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#جيڪڏهن_وصف_ٿيل HB_DISABLE_DEPRECATED
    أرجع 0;
#وإلا
    أرجع klass->get.f.glyph_v_kerning (هذا, user_معلومات,
					 چوٽي_glyph, bottom_glyph,
					 !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_v_kerning);
#جيڪڏهن_پورو
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       منطقي synthetic = سچ)
  {
    hb_memset (extents, 0, حجم (*extents));

    /* This is rather messy, but necessary. */

    إذا (!synthetic)
    {
      أرجع klass->get.f.glyph_extents (هذا, user_معلومات,
					 glyph,
					 extents,
					 !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_extents);
    }
    إذا (!is_synthetic &&
	klass->get.f.glyph_extents (هذا, user_معلومات,
				    glyph,
				    extents,
				    !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_extents))
      أرجع سچ;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#جيڪڏهن_وصف_نه_ٿيل HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    إذا (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().منزل_glyph_extents ();
      أرجع سچ;
    }
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_DRAW
    hb_extents_t<> draw_extents;
    إذا (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.منزل_glyph_extents ();
      أرجع سچ;
    }
#جيڪڏهن_پورو

    منطقي ret = klass->get.f.glyph_extents (هذا, user_معلومات,
					   glyph,
					   extents,
					   !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_extents);
    إذا (ret)
      synthetic_glyph_extents (extents);

    أرجع ret;
  }

  hb_bool_t get_glyph_contour_نقطو (hb_codepoint_t glyph, بنانشان صحيح نقطو_اشاريو,
				     hb_position_t *x, hb_position_t *y,
				     منطقي synthetic = سچ)
  {
    *x = *y = 0;
    منطقي ret = klass->get.f.glyph_contour_نقطو (هذا, user_معلومات,
						 glyph, نقطو_اشاريو,
						 x, y,
						 !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_contour_نقطو);

    إذا (synthetic && ret)
    {
      /* Slant */
      إذا (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      إذا (!embolden_اندريون_رکو)
      {
	صحيح x_سرڪايو = x_scale < 0 ? -x_strength : x_strength;
	*x += x_سرڪايو;
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    حرف *name, بنانشان صحيح سائيز)
  {
    إذا (سائيز) *name = '\0';
    أرجع klass->get.f.glyph_name (هذا, user_معلومات,
				    glyph,
				    name, سائيز,
				    !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_name);
  }

  hb_bool_t get_glyph_from_name (ثابت حرف *name, صحيح len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    إذا (len == -1) len = لڙي_ڊيگهه (name);
    أرجع klass->get.f.glyph_from_name (هذا, user_معلومات,
					 name, len,
					 glyph,
					 !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->glyph_from_name);
  }

  منطقي draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, فراغ *draw_معلومات,
			   منطقي synthetic = سچ)
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_DRAW
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OUTLINE
    منطقي embolden = x_strength || y_strength;
    منطقي slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#وإلا
    synthetic = خطأ;
#جيڪڏهن_پورو

    إذا (!synthetic)
    {
      أرجع klass->get.f.draw_glyph_or_fail (هذا, user_معلومات,
					      glyph,
					      draw_funcs, draw_معلومات,
					      !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->draw_glyph_or_fail);
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_بچيل = draw_funcs->get_budget_بچيل_ptr (draw_معلومات);
    إذا (!klass->get.f.draw_glyph_or_fail (هذا, user_معلومات,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->draw_glyph_or_fail))
      أرجع خطأ;

    // Slant before embolden; produces nicer results.

    إذا (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, خطأ);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    إذا (embolden)
    {
      عائم x_سرڪايو = embolden_اندريون_رکو ? 0 : (عائم) x_strength / 2;
      عائم y_سرڪايو = (عائم) y_strength / 2;
      إذا (x_scale < 0) x_سرڪايو = -x_سرڪايو;
      إذا (y_scale < 0) y_سرڪايو = -y_سرڪايو;
      outline.embolden (x_strength, y_strength, x_سرڪايو, y_سرڪايو);
    }

    outline.replay (draw_funcs, draw_معلومات);

    أرجع سچ;
#جيڪڏهن_پورو
#جيڪڏهن_پورو
    أرجع خطأ;
  }

  منطقي paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, فراغ *paint_معلومات,
			    بنانشان صحيح palette,
			    hb_color_t foreground,
			    منطقي synthetic = سچ)
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_PAINT
    /* Slant */
    إذا (synthetic && slant_xy)
      hb_paint_ڌڪو_transform (paint_funcs, paint_معلومات,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    منطقي ret = klass->get.f.paint_glyph_or_fail (هذا, user_معلومات,
						 glyph,
						 paint_funcs, paint_معلومات,
						 palette, foreground,
						 !klass->user_معلومات ? خالي_اشارو : klass->user_معلومات->paint_glyph_or_fail);

    إذا (synthetic && slant_xy)
      hb_paint_ڪڍو_transform (paint_funcs, paint_معلومات);

    أرجع ret;
#جيڪڏهن_پورو
    أرجع خطأ;
  }

  /* A bit higher-level, and with fallback */

  HB_اندروني
  فراغ paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, فراغ *paint_معلومات,
		    بنانشان صحيح palette,
		    hb_color_t foreground);

  فراغ get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    إذا (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->سٽ_gap = 0;
    }
  }
  فراغ get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    إذا (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->سٽ_gap = 0;
    }
  }

  فراغ get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    وإلا
      get_v_extents_with_fallback (extents);
  }

  فراغ get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    وإلا
      *y = get_glyph_v_advance (glyph);
  }
  فراغ get_glyph_advances_for_direction (hb_direction_t direction,
					 بنانشان صحيح ڳڻپ,
					 ثابت hb_codepoint_t *پهريون_glyph,
					 بنانشان glyph_stride,
					 hb_position_t *پهريون_advance,
					 بنانشان advance_stride)
  {
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (ڳڻپ, پهريون_glyph, glyph_stride, پهريون_advance, advance_stride);
    وإلا
      get_glyph_v_advances (ڳڻپ, پهريون_glyph, glyph_stride, پهريون_advance, advance_stride);
  }

  فراغ apply_وٿي (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     نشانوارو mult)
  {
    أكد (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  فراغ جوڙيو_وٿي (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  فراغ ڪٽيو_وٿي (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  فراغ guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  فراغ apply_glyph_h_origins_with_fallback (hb_بفر_t *buf, صحيح mult)
  {
    منطقي has_ascender = خطأ;
    hb_position_t ascender = 0;

    بنية { hb_position_t x, y; } origins[32];

    بنانشان صحيح وٿي = 0;
    بنانشان صحيح ڳڻپ = buf->len;
    طالما (وٿي < ڳڻپ)
    {
      بنانشان n = hb_min (ڳڻپ - وٿي, ARRAY_ڊيگهائي (origins));
      إذا (!get_glyph_h_origins (n,
				&buf->info[وٿي].codepoint, حجم (hb_glyph_info_t),
				&origins[0].x, حجم (origins[0]),
				&origins[0].y, حجم (origins[0])))
      {
        إذا (get_glyph_v_origins (n,
				  &buf->info[وٿي].codepoint, حجم (hb_glyph_info_t),
				  &origins[0].x, حجم (origins[0]),
				  &origins[0].y, حجم (origins[0])))
	{
	  إذا (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = سچ;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  لكل (بنانشان j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[وٿي + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	وإلا
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      أكد (mult == -1 || mult == +1 || mult == 0);
      إذا (mult == +1)
        لكل (بنانشان j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[وٿي + j];
	  جوڙيو_وٿي (&pos->x_وٿي_2, &pos->y_وٿي_2,
		      origins[j].x, origins[j].y);
	}
      وإلا إذا (mult == -1)
	لكل (بنانشان j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[وٿي + j];
	  ڪٽيو_وٿي (&pos->x_وٿي_2, &pos->y_وٿي_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      وٿي += n;
    }
  }
  فراغ apply_glyph_v_origins_with_fallback (hb_بفر_t *buf, صحيح mult)
  {
    منطقي has_ascender = خطأ;
    hb_position_t ascender = 0;

    بنية { hb_position_t x, y; } origins[32];

    بنانشان صحيح وٿي = 0;
    بنانشان صحيح ڳڻپ = buf->len;
    طالما (وٿي < ڳڻپ)
    {
      بنانشان n = hb_min (ڳڻپ - وٿي, ARRAY_ڊيگهائي (origins));
      إذا (!get_glyph_v_origins (n,
				&buf->info[وٿي].codepoint, حجم (hb_glyph_info_t),
				&origins[0].x, حجم (origins[0]),
				&origins[0].y, حجم (origins[0])))
      {
	إذا (get_glyph_h_origins (n,
				 &buf->info[وٿي].codepoint, حجم (hb_glyph_info_t),
				 &origins[0].x, حجم (origins[0]),
				 &origins[0].y, حجم (origins[0])))
	{
	  إذا (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = سچ;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  لكل (بنانشان j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[وٿي + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	وإلا
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      أكد (mult == -1 || mult == +1 || mult == 0);
      إذا (mult == +1)
        لكل (بنانشان j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[وٿي + j];
	  جوڙيو_وٿي (&pos->x_وٿي_2, &pos->y_وٿي_2,
		      origins[j].x, origins[j].y);
	}
      وإلا إذا (mult == -1)
	لكل (بنانشان j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[وٿي + j];
	  ڪٽيو_وٿي (&pos->x_وٿي_2, &pos->y_وٿي_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      وٿي += n;
    }
  }

  فراغ get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    إذا (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      ڪٽيو_وٿي (x, y, dx, dy);
    }
  }
  فراغ get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    إذا (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      جوڙيو_وٿي (x, y, dx, dy);
    }
  }

  فراغ get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    وإلا
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  فراغ جوڙيو_glyph_h_origins (hb_بفر_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  فراغ جوڙيو_glyph_v_origins (hb_بفر_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  فراغ جوڙيو_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    جوڙيو_وٿي (x, y, origin_x, origin_y);
  }

  فراغ ڪٽيو_glyph_h_origins (hb_بفر_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  فراغ ڪٽيو_glyph_v_origins (hb_بفر_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  فراغ ڪٽيو_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    ڪٽيو_وٿي (x, y, origin_x, origin_y);
  }

  فراغ get_glyph_kerning_for_direction (hb_codepoint_t پهريون_glyph, hb_codepoint_t ٻيو_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (پهريون_glyph, ٻيو_glyph);
    } وإلا {
      *x = 0;
      *y = get_glyph_v_kerning (پهريون_glyph, ٻيو_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    إذا (ret)
      ڪٽيو_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    أرجع ret;
  }

  hb_bool_t get_glyph_contour_نقطو_for_origin (hb_codepoint_t glyph, بنانشان صحيح نقطو_اشاريو,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_نقطو (glyph, نقطو_اشاريو, x, y);

    إذا (ret)
      ڪٽيو_glyph_origin_for_direction (glyph, direction, x, y);

    أرجع ret;
  }

  /* Generates gidDDD if glyph has no name. */
  فراغ
  glyph_منزل_string (hb_codepoint_t glyph,
		   حرف *s, بنانشان صحيح سائيز)
  {
    إذا (get_glyph_name (glyph, s, سائيز)) أرجع;

    إذا (سائيز && snprintf (s, سائيز, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (ثابت حرف *s, صحيح len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    إذا (get_glyph_from_name (s, len, glyph)) أرجع سچ;

    إذا (len == -1) len = لڙي_ڊيگهه (s);

    /* Straight glyph index. */
    إذا (hb_codepoint_parse (s, len, 10, glyph))
      أرجع سچ;

    إذا (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      إذا (0 == لڙي_ڀيٽ (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	أرجع سچ;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      إذا (0 == لڙي_ڀيٽ (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	أرجع سچ;
    }

    أرجع خطأ;
  }

  فراغ بدليو ()
  {
    عائم upem = face->get_upem ();

    x_multf = x_scale / upem;
    y_multf = y_scale / upem;
    x_mult = (int64_t) x_scale * 0x10000 / upem;
    y_mult = (int64_t) y_scale * 0x10000 / upem;

    is_synthetic =  x_embolden || y_embolden || slant;

    x_strength = roundf (مطلق_قدر (x_scale) * x_embolden);
    y_strength = roundf (مطلق_قدر (y_scale) * y_embolden);

    slant_xy = y_scale ? slant * x_scale / y_scale : 0.f;

    معلومات.fini ();

    serial++;
  }

  hb_position_t em_mult (int32_t v, int64_t mult)
  { أرجع (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (عائم v, عائم mult)
  { أرجع (hb_position_t) roundf (em_fmultf (v, mult)); }
  عائم em_fmultf (عائم v, عائم mult)
  { أرجع v * mult; }
  عائم em_fmult (int32_t v, عائم mult)
  { أرجع (عائم) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#جيڪڏهن_پورو /* HB_FONT_HH */
