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

#اگر_غیر_تعریف_شدہ HB_FONT_HH
#تعریف_کرو HB_FONT_HH

#شامل "hb.hh"

#شامل "hb-face.hh"
#شامل "hb-atomic.hh"
#شامل "hb-draw.hh"
#شامل "hb-paint-extents.hh"
#شامل "hb-shaper.hh"
#شامل "hb-outline.hh"


/*
 * hb_font_funcs_t
 */

#تعریف_کرو HB_FONT_FUNCS_IMPLEMENT_CALLBACKS \
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
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_contour_نقطہ) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_name) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_from_name) \
  HB_FONT_FUNC_IMPLEMENT (,draw_glyph_or_fail) \
  HB_FONT_FUNC_IMPLEMENT (,paint_glyph_or_fail) \
  /* ^--- Add new callbacks here */

ساخت hb_font_funcs_t
{
  hb_object_سرخی_t سرخی;

  ساخت {
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) خالی *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_FONT_FUNC_IMPLEMENT
  } *user_ڈیٹا;

  ساخت {
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  اتحاد get_t {
    ساخت get_funcs_t {
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_FONT_FUNC_IMPLEMENT
    } f;
    خالی (*array[0
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) +1
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_FONT_FUNC_IMPLEMENT
		]) ();
  } get;
};
DECLARE_NULL_INSTANCE (hb_font_funcs_t);


/*
 * hb_font_t
 */

#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_ڈیٹا_INSTANTIATE_SHAPERS(shaper, font);
#شامل "hb-shaper-list.hh"
#تعریف_منسوخ HB_SHAPER_IMPLEMENT

ساخت hb_font_t
{
  hb_object_سرخی_t سرخی;
  hb_atomic_t<غیرعلامتی> serial;
  hb_atomic_t<غیرعلامتی> serial_coords;

  hb_font_t *والد;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  منطقی is_synthetic;

  اعشاری x_embolden;
  اعشاری y_embolden;
  منطقی embolden_اندر_رکھو;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  اعشاری slant;
  اعشاری slant_xy;

  اعشاری x_multf;
  اعشاری y_multf;
  int64_t x_mult;
  int64_t y_mult;

  غیرعلامتی صحیح x_ppem;
  غیرعلامتی صحیح y_ppem;

  اعشاری ptem;

  /* Font variation coordinates. */
  غیرعلامتی صحیح instance_اشاریہ;
  منطقی has_nonzero_coords;
  غیرعلامتی صحیح عدد_coords;
  صحیح *coords;
  اعشاری *design_coords;

  hb_font_funcs_t   *klass;
  خالی              *user_ڈیٹا;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> ڈیٹا; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { واپس HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { واپس em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { واپس em_mult (v, y_mult); }
  hb_position_t em_scalef_x (اعشاری v) { واپس em_multf (v, x_multf); }
  hb_position_t em_scalef_y (اعشاری v) { واپس em_multf (v, y_multf); }
  اعشاری em_fscale_x (int32_t v) { واپس em_fmult (v, x_multf); }
  اعشاری em_fscale_y (int32_t v) { واپس em_fmult (v, y_multf); }
  اعشاری em_fscalef_x (اعشاری v) { واپس em_fmultf (v, x_multf); }
  اعشاری em_fscalef_y (اعشاری v) { واپس em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { واپس em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t والد_scale_x_فاصلہ (hb_position_t v)
  {
    اگر (unlikely (والد && والد->x_scale && والد->x_scale != x_scale))
      واپس (hb_position_t) (v * (int64_t) یہ->x_scale / یہ->والد->x_scale);
    واپس v;
  }
  hb_position_t والد_scale_y_فاصلہ (hb_position_t v)
  {
    اگر (unlikely (والد && والد->y_scale && والد->y_scale != y_scale))
      واپس (hb_position_t) (v * (int64_t) یہ->y_scale / یہ->والد->y_scale);
    واپس v;
  }
  hb_position_t والد_scale_x_position (hb_position_t v)
  { واپس والد_scale_x_فاصلہ (v); }
  hb_position_t والد_scale_y_position (hb_position_t v)
  { واپس والد_scale_y_فاصلہ (v); }

  خالی والد_scale_فاصلہ (hb_position_t *x, hb_position_t *y)
  {
    *x = والد_scale_x_فاصلہ (*x);
    *y = والد_scale_y_فاصلہ (*y);
  }
  خالی والد_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = والد_scale_x_position (*x);
    *y = والد_scale_y_position (*y);
  }

  خالی scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    اعشاری x1 = em_fscale_x (extents->x_bearing);
    اعشاری y1 = em_fscale_y (extents->y_bearing);
    اعشاری x2 = x1 + em_fscale_x (extents->چوڑائی);
    اعشاری y2 = y1 + em_fscale_y (extents->اونچائی);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->چوڑائی = ceilf (x2) - extents->x_bearing;
    extents->اونچائی = ceilf (y2) - extents->y_bearing;
  }

  خالی synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    اگر (slant_xy)
    {
      /* x_bearing/width and y_bearing/height are each clamped to the full
       * hb_position_t range independently by the extent producers, so their
       * sums and differences can overflow int32.  Accumulate in int64. */
      int64_t y1 = extents->y_bearing;
      int64_t y2 = (int64_t) extents->y_bearing + extents->اونچائی;

      int64_t x1 = (int64_t) extents->x_bearing + (int64_t) floorf (hb_min (y1 * slant_xy, y2 * slant_xy));
      int64_t x2 = (int64_t) extents->x_bearing + extents->چوڑائی + (int64_t) ceilf (hb_max (y1 * slant_xy, y2 * slant_xy));

      extents->x_bearing = hb_clamp_مقصد<hb_position_t> (x1);
      extents->چوڑائی = hb_clamp_مقصد<hb_position_t> (x2 - x1);
    }

    /* Embolden. */
    اگر (x_strength || y_strength)
    {
      /* Y */
      صحیح y_سرکاؤ = y_strength;
      اگر (y_scale < 0) y_سرکاؤ = -y_سرکاؤ;
      extents->y_bearing = hb_clamp_مقصد<hb_position_t> ((int64_t) extents->y_bearing + y_سرکاؤ);
      extents->اونچائی = hb_clamp_مقصد<hb_position_t> ((int64_t) extents->اونچائی - y_سرکاؤ);

      /* X */
      صحیح x_سرکاؤ = x_strength;
      اگر (x_scale < 0) x_سرکاؤ = -x_سرکاؤ;
      اگر (embolden_اندر_رکھو)
	extents->x_bearing = hb_clamp_مقصد<hb_position_t> ((int64_t) extents->x_bearing - x_سرکاؤ / 2);
      extents->چوڑائی = hb_clamp_مقصد<hb_position_t> ((int64_t) extents->چوڑائی + x_سرکاؤ);
    }
  }


  /* Public getters */

  HB_اندرونی منطقی has_func (غیرعلامتی صحیح i);
  HB_اندرونی منطقی has_func_سیٹ (غیرعلامتی صحیح i);

  /* has_* ... */
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) \
  منطقی \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = یہ->klass; \
    غیرعلامتی صحیح i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / حجم (funcs->get.array[0]); \
    واپس has_func (i); \
  } \
  منطقی \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = یہ->klass; \
    غیرعلامتی صحیح i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / حجم (funcs->get.array[0]); \
    واپس has_func_سیٹ (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_منسوخ HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				منطقی synthetic = سچ)
  {
    hb_memset (extents, 0, حجم (*extents));
    منطقی ret = klass->get.f.font_h_extents (یہ, user_ڈیٹا,
					    extents,
					    !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->font_h_extents);

    اگر (synthetic && ret)
    {
      /* Embolden */
      صحیح y_سرکاؤ = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_سرکاؤ;
    }

    واپس ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				منطقی synthetic = سچ)
  {
    hb_memset (extents, 0, حجم (*extents));
    منطقی ret = klass->get.f.font_v_extents (یہ, user_ڈیٹا,
					    extents,
					    !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->font_v_extents);

    اگر (synthetic && ret)
    {
      /* Embolden */
      صحیح x_سرکاؤ = x_scale < 0 ? -x_strength : x_strength;
      اگر (embolden_اندر_رکھو)
      {
	extents->ascender += x_سرکاؤ / 2;
	extents->descender -= x_سرکاؤ - x_سرکاؤ / 2;
      }
      ورنہ
	extents->ascender += x_سرکاؤ;
    }

    واپس ret;
  }

  منطقی has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    واپس get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_ملا = 0)
  {
    *glyph = not_ملا;
    واپس klass->get.f.nominal_glyph (یہ, user_ڈیٹا,
				       unicode, glyph,
				       !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->nominal_glyph);
  }
  غیرعلامتی صحیح get_nominal_glyphs (غیرعلامتی صحیح گنتی,
				   ثابت hb_codepoint_t *پہلا_unicode,
				   غیرعلامتی صحیح unicode_stride,
				   hb_codepoint_t *پہلا_glyph,
				   غیرعلامتی صحیح glyph_stride)
  {
    واپس klass->get.f.nominal_glyphs (یہ, user_ڈیٹا,
					گنتی,
					پہلا_unicode, unicode_stride,
					پہلا_glyph, glyph_stride,
					!klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_ملا = 0)
  {
    *glyph = not_ملا;
    واپس klass->get.f.variation_glyph (یہ, user_ڈیٹا,
					 unicode, variation_selector, glyph,
					 !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     منطقی synthetic = سچ)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (یہ, user_ڈیٹا,
							  glyph,
							  !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_h_advance);

    اگر (synthetic && x_strength && !embolden_اندر_رکھو)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    واپس advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     منطقی synthetic = سچ)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (یہ, user_ڈیٹا,
							  glyph,
							  !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_v_advance);

    اگر (synthetic && y_strength && !embolden_اندر_رکھو)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    واپس advance;
  }

  خالی get_glyph_h_advances (غیرعلامتی صحیح گنتی,
			     ثابت hb_codepoint_t *پہلا_glyph,
			     غیرعلامتی صحیح glyph_stride,
			     hb_position_t *پہلا_advance,
			     غیرعلامتی صحیح advance_stride,
			     منطقی synthetic = سچ)
  {
    klass->get.f.glyph_h_advances (یہ, user_ڈیٹا,
				   گنتی,
				   پہلا_glyph, glyph_stride,
				   پہلا_advance, advance_stride,
				   !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_h_advances);

    اگر (synthetic && x_strength && !embolden_اندر_رکھو)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      {
	*پہلا_advance += *پہلا_advance ? strength : 0;
	پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
      }
    }
  }

  خالی get_glyph_v_advances (غیرعلامتی صحیح گنتی,
			     ثابت hb_codepoint_t *پہلا_glyph,
			     غیرعلامتی صحیح glyph_stride,
			     hb_position_t *پہلا_advance,
			     غیرعلامتی صحیح advance_stride,
			     منطقی synthetic = سچ)
  {
    klass->get.f.glyph_v_advances (یہ, user_ڈیٹا,
				   گنتی,
				   پہلا_glyph, glyph_stride,
				   پہلا_advance, advance_stride,
				   !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_v_advances);

    اگر (synthetic && y_strength && !embolden_اندر_رکھو)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      {
	*پہلا_advance += *پہلا_advance ? strength : 0;
	پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				منطقی synthetic = سچ)
  {
    *x = *y = 0;
    منطقی ret = klass->get.f.glyph_h_origin (یہ, user_ڈیٹا,
					    glyph, x, y,
					    !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_h_origin);

    اگر (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      اگر (!embolden_اندر_رکھو)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    واپس ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				منطقی synthetic = سچ)
  {
    *x = *y = 0;
    منطقی ret = klass->get.f.glyph_v_origin (یہ, user_ڈیٹا,
					    glyph, x, y,
					    !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_v_origin);

    اگر (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      اگر (!embolden_اندر_رکھو)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    واپس ret;
  }

  hb_bool_t get_glyph_h_origins (غیرعلامتی صحیح گنتی,
				 ثابت hb_codepoint_t *پہلا_glyph,
				 غیرعلامتی صحیح glyph_stride,
				 hb_position_t *پہلا_x,
				 غیرعلامتی صحیح x_stride,
				 hb_position_t *پہلا_y,
				 غیرعلامتی صحیح y_stride,
				 منطقی synthetic = سچ)

  {
    منطقی ret = klass->get.f.glyph_h_origins (یہ, user_ڈیٹا,
					     گنتی,
					     پہلا_glyph, glyph_stride,
					     پہلا_x, x_stride, پہلا_y, y_stride,
					     !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_h_origins);

    اگر (synthetic && ret)
    {
      hb_position_t x_سرکاؤ = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_سرکاؤ = y_scale < 0 ? -y_strength : y_strength;
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	اگر (!embolden_اندر_رکھو)
	{
	  *پہلا_x += x_سرکاؤ;
	  *پہلا_y += y_سرکاؤ;
	}
	پہلا_x = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_x, x_stride);
	پہلا_y = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_y, y_stride);
      }
    }

    واپس ret;
  }

  hb_bool_t get_glyph_v_origins (غیرعلامتی صحیح گنتی,
				 ثابت hb_codepoint_t *پہلا_glyph,
				 غیرعلامتی صحیح glyph_stride,
				 hb_position_t *پہلا_x,
				 غیرعلامتی صحیح x_stride,
				 hb_position_t *پہلا_y,
				 غیرعلامتی صحیح y_stride,
				 منطقی synthetic = سچ)

  {
    منطقی ret = klass->get.f.glyph_v_origins (یہ, user_ڈیٹا,
					     گنتی,
					     پہلا_glyph, glyph_stride,
					     پہلا_x, x_stride, پہلا_y, y_stride,
					     !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_v_origins);

    اگر (synthetic && is_synthetic && ret)
    {
      hb_position_t x_سرکاؤ = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_سرکاؤ = y_scale < 0 ? -y_strength : y_strength;
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	اگر (!embolden_اندر_رکھو)
	{
	  *پہلا_x += x_سرکاؤ;
	  *پہلا_y += y_سرکاؤ;
	}
	پہلا_x = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_x, x_stride);
	پہلا_y = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_y, y_stride);
      }
    }

    واپس ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t بایاں_glyph,
				     hb_codepoint_t دایاں_glyph)
  {
#اگر_تعریف_شدہ HB_DISABLE_DEPRECATED
    واپس 0;
#ورنہ
    واپس klass->get.f.glyph_h_kerning (یہ, user_ڈیٹا,
					 بایاں_glyph, دایاں_glyph,
					 !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_h_kerning);
#اگر_ختم
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t چوٹی_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#اگر_تعریف_شدہ HB_DISABLE_DEPRECATED
    واپس 0;
#ورنہ
    واپس klass->get.f.glyph_v_kerning (یہ, user_ڈیٹا,
					 چوٹی_glyph, bottom_glyph,
					 !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_v_kerning);
#اگر_ختم
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       منطقی synthetic = سچ)
  {
    hb_memset (extents, 0, حجم (*extents));

    /* This is rather messy, but necessary. */

    اگر (!synthetic)
    {
      واپس klass->get.f.glyph_extents (یہ, user_ڈیٹا,
					 glyph,
					 extents,
					 !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_extents);
    }
    اگر (!is_synthetic &&
	klass->get.f.glyph_extents (یہ, user_ڈیٹا,
				    glyph,
				    extents,
				    !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_extents))
      واپس سچ;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#اگر_غیر_تعریف_شدہ HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    اگر (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().مقصد_glyph_extents ();
      واپس سچ;
    }
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_DRAW
    hb_extents_t<> draw_extents;
    اگر (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.مقصد_glyph_extents ();
      واپس سچ;
    }
#اگر_ختم

    منطقی ret = klass->get.f.glyph_extents (یہ, user_ڈیٹا,
					   glyph,
					   extents,
					   !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_extents);
    اگر (ret)
      synthetic_glyph_extents (extents);

    واپس ret;
  }

  hb_bool_t get_glyph_contour_نقطہ (hb_codepoint_t glyph, غیرعلامتی صحیح نقطہ_اشاریہ,
				     hb_position_t *x, hb_position_t *y,
				     منطقی synthetic = سچ)
  {
    *x = *y = 0;
    منطقی ret = klass->get.f.glyph_contour_نقطہ (یہ, user_ڈیٹا,
						 glyph, نقطہ_اشاریہ,
						 x, y,
						 !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_contour_نقطہ);

    اگر (synthetic && ret)
    {
      /* Slant */
      اگر (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      اگر (!embolden_اندر_رکھو)
      {
	صحیح x_سرکاؤ = x_scale < 0 ? -x_strength : x_strength;
	*x += x_سرکاؤ;
      }
    }

    واپس ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    حرف *name, غیرعلامتی صحیح سائز)
  {
    اگر (سائز) *name = '\0';
    واپس klass->get.f.glyph_name (یہ, user_ڈیٹا,
				    glyph,
				    name, سائز,
				    !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_name);
  }

  hb_bool_t get_glyph_from_name (ثابت حرف *name, صحیح len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    اگر (len == -1) len = لڑی_لمبائی (name);
    واپس klass->get.f.glyph_from_name (یہ, user_ڈیٹا,
					 name, len,
					 glyph,
					 !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->glyph_from_name);
  }

  منطقی draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, خالی *draw_ڈیٹا,
			   منطقی synthetic = سچ)
  {
#اگر_غیر_تعریف_شدہ HB_NO_DRAW
#اگر_غیر_تعریف_شدہ HB_NO_OUTLINE
    منطقی embolden = x_strength || y_strength;
    منطقی slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#ورنہ
    synthetic = جھوٹ;
#اگر_ختم

    اگر (!synthetic)
    {
      واپس klass->get.f.draw_glyph_or_fail (یہ, user_ڈیٹا,
					      glyph,
					      draw_funcs, draw_ڈیٹا,
					      !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->draw_glyph_or_fail);
    }

#اگر_غیر_تعریف_شدہ HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_بقایا = draw_funcs->get_budget_بقایا_ptr (draw_ڈیٹا);
    اگر (!klass->get.f.draw_glyph_or_fail (یہ, user_ڈیٹا,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->draw_glyph_or_fail))
      واپس جھوٹ;

    // Slant before embolden; produces nicer results.

    اگر (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, جھوٹ);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    اگر (embolden)
    {
      اعشاری x_سرکاؤ = embolden_اندر_رکھو ? 0 : (اعشاری) x_strength / 2;
      اعشاری y_سرکاؤ = (اعشاری) y_strength / 2;
      اگر (x_scale < 0) x_سرکاؤ = -x_سرکاؤ;
      اگر (y_scale < 0) y_سرکاؤ = -y_سرکاؤ;
      outline.embolden (x_strength, y_strength, x_سرکاؤ, y_سرکاؤ);
    }

    outline.replay (draw_funcs, draw_ڈیٹا);

    واپس سچ;
#اگر_ختم
#اگر_ختم
    واپس جھوٹ;
  }

  منطقی paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, خالی *paint_ڈیٹا,
			    غیرعلامتی صحیح palette,
			    hb_color_t foreground,
			    منطقی synthetic = سچ)
  {
#اگر_غیر_تعریف_شدہ HB_NO_PAINT
    /* Slant */
    اگر (synthetic && slant_xy)
      hb_paint_ڈالو_transform (paint_funcs, paint_ڈیٹا,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    منطقی ret = klass->get.f.paint_glyph_or_fail (یہ, user_ڈیٹا,
						 glyph,
						 paint_funcs, paint_ڈیٹا,
						 palette, foreground,
						 !klass->user_ڈیٹا ? خالی_اشاریہ : klass->user_ڈیٹا->paint_glyph_or_fail);

    اگر (synthetic && slant_xy)
      hb_paint_نکالو_transform (paint_funcs, paint_ڈیٹا);

    واپس ret;
#اگر_ختم
    واپس جھوٹ;
  }

  /* A bit higher-level, and with fallback */

  HB_اندرونی
  خالی paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, خالی *paint_ڈیٹا,
		    غیرعلامتی صحیح palette,
		    hb_color_t foreground);

  خالی get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    اگر (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->سطر_gap = 0;
    }
  }
  خالی get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    اگر (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->سطر_gap = 0;
    }
  }

  خالی get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    ورنہ
      get_v_extents_with_fallback (extents);
  }

  خالی get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    ورنہ
      *y = get_glyph_v_advance (glyph);
  }
  خالی get_glyph_advances_for_direction (hb_direction_t direction,
					 غیرعلامتی صحیح گنتی,
					 ثابت hb_codepoint_t *پہلا_glyph,
					 غیرعلامتی glyph_stride,
					 hb_position_t *پہلا_advance,
					 غیرعلامتی advance_stride)
  {
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (گنتی, پہلا_glyph, glyph_stride, پہلا_advance, advance_stride);
    ورنہ
      get_glyph_v_advances (گنتی, پہلا_glyph, glyph_stride, پہلا_advance, advance_stride);
  }

  خالی apply_انحراف (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     علامتی mult)
  {
    أكد (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  خالی جوڑو_انحراف (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  خالی تفریق_انحراف (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  خالی guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  خالی apply_glyph_h_origins_with_fallback (hb_بفر_t *buf, صحیح mult)
  {
    منطقی has_ascender = جھوٹ;
    hb_position_t ascender = 0;

    ساخت { hb_position_t x, y; } origins[32];

    غیرعلامتی صحیح انحراف = 0;
    غیرعلامتی صحیح گنتی = buf->len;
    جب (انحراف < گنتی)
    {
      غیرعلامتی n = hb_min (گنتی - انحراف, ARRAY_لمبائی (origins));
      اگر (!get_glyph_h_origins (n,
				&buf->info[انحراف].codepoint, حجم (hb_glyph_info_t),
				&origins[0].x, حجم (origins[0]),
				&origins[0].y, حجم (origins[0])))
      {
        اگر (get_glyph_v_origins (n,
				  &buf->info[انحراف].codepoint, حجم (hb_glyph_info_t),
				  &origins[0].x, حجم (origins[0]),
				  &origins[0].y, حجم (origins[0])))
	{
	  اگر (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = سچ;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  برائے (غیرعلامتی j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[انحراف + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	ورنہ
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      أكد (mult == -1 || mult == +1 || mult == 0);
      اگر (mult == +1)
        برائے (غیرعلامتی j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[انحراف + j];
	  جوڑو_انحراف (&pos->x_انحراف_2, &pos->y_انحراف_2,
		      origins[j].x, origins[j].y);
	}
      ورنہ اگر (mult == -1)
	برائے (غیرعلامتی j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[انحراف + j];
	  تفریق_انحراف (&pos->x_انحراف_2, &pos->y_انحراف_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      انحراف += n;
    }
  }
  خالی apply_glyph_v_origins_with_fallback (hb_بفر_t *buf, صحیح mult)
  {
    منطقی has_ascender = جھوٹ;
    hb_position_t ascender = 0;

    ساخت { hb_position_t x, y; } origins[32];

    غیرعلامتی صحیح انحراف = 0;
    غیرعلامتی صحیح گنتی = buf->len;
    جب (انحراف < گنتی)
    {
      غیرعلامتی n = hb_min (گنتی - انحراف, ARRAY_لمبائی (origins));
      اگر (!get_glyph_v_origins (n,
				&buf->info[انحراف].codepoint, حجم (hb_glyph_info_t),
				&origins[0].x, حجم (origins[0]),
				&origins[0].y, حجم (origins[0])))
      {
	اگر (get_glyph_h_origins (n,
				 &buf->info[انحراف].codepoint, حجم (hb_glyph_info_t),
				 &origins[0].x, حجم (origins[0]),
				 &origins[0].y, حجم (origins[0])))
	{
	  اگر (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = سچ;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  برائے (غیرعلامتی j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[انحراف + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	ورنہ
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      أكد (mult == -1 || mult == +1 || mult == 0);
      اگر (mult == +1)
        برائے (غیرعلامتی j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[انحراف + j];
	  جوڑو_انحراف (&pos->x_انحراف_2, &pos->y_انحراف_2,
		      origins[j].x, origins[j].y);
	}
      ورنہ اگر (mult == -1)
	برائے (غیرعلامتی j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[انحراف + j];
	  تفریق_انحراف (&pos->x_انحراف_2, &pos->y_انحراف_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      انحراف += n;
    }
  }

  خالی get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    اگر (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      تفریق_انحراف (x, y, dx, dy);
    }
  }
  خالی get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    اگر (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      جوڑو_انحراف (x, y, dx, dy);
    }
  }

  خالی get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    ورنہ
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  خالی جوڑو_glyph_h_origins (hb_بفر_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  خالی جوڑو_glyph_v_origins (hb_بفر_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  خالی جوڑو_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    جوڑو_انحراف (x, y, origin_x, origin_y);
  }

  خالی تفریق_glyph_h_origins (hb_بفر_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  خالی تفریق_glyph_v_origins (hb_بفر_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  خالی تفریق_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    تفریق_انحراف (x, y, origin_x, origin_y);
  }

  خالی get_glyph_kerning_for_direction (hb_codepoint_t پہلا_glyph, hb_codepoint_t دوسرا_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (پہلا_glyph, دوسرا_glyph);
    } ورنہ {
      *x = 0;
      *y = get_glyph_v_kerning (پہلا_glyph, دوسرا_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    اگر (ret)
      تفریق_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    واپس ret;
  }

  hb_bool_t get_glyph_contour_نقطہ_for_origin (hb_codepoint_t glyph, غیرعلامتی صحیح نقطہ_اشاریہ,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_نقطہ (glyph, نقطہ_اشاریہ, x, y);

    اگر (ret)
      تفریق_glyph_origin_for_direction (glyph, direction, x, y);

    واپس ret;
  }

  /* Generates gidDDD if glyph has no name. */
  خالی
  glyph_مقصد_string (hb_codepoint_t glyph,
		   حرف *s, غیرعلامتی صحیح سائز)
  {
    اگر (get_glyph_name (glyph, s, سائز)) واپس;

    اگر (سائز && snprintf (s, سائز, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (ثابت حرف *s, صحیح len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    اگر (get_glyph_from_name (s, len, glyph)) واپس سچ;

    اگر (len == -1) len = لڑی_لمبائی (s);

    /* Straight glyph index. */
    اگر (hb_codepoint_parse (s, len, 10, glyph))
      واپس سچ;

    اگر (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      اگر (0 == لڑی_موازنہ (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	واپس سچ;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      اگر (0 == لڑی_موازنہ (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	واپس سچ;
    }

    واپس جھوٹ;
  }

  خالی بدلا ()
  {
    اعشاری upem = face->get_upem ();

    x_multf = x_scale / upem;
    y_multf = y_scale / upem;
    x_mult = (int64_t) x_scale * 0x10000 / upem;
    y_mult = (int64_t) y_scale * 0x10000 / upem;

    is_synthetic =  x_embolden || y_embolden || slant;

    x_strength = roundf (مطلق_قدر (x_scale) * x_embolden);
    y_strength = roundf (مطلق_قدر (y_scale) * y_embolden);

    slant_xy = y_scale ? slant * x_scale / y_scale : 0.f;

    ڈیٹا.fini ();

    serial++;
  }

  hb_position_t em_mult (int32_t v, int64_t mult)
  { واپس (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (اعشاری v, اعشاری mult)
  { واپس (hb_position_t) roundf (em_fmultf (v, mult)); }
  اعشاری em_fmultf (اعشاری v, اعشاری mult)
  { واپس v * mult; }
  اعشاری em_fmult (int32_t v, اعشاری mult)
  { واپس (اعشاری) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#اگر_ختم /* HB_FONT_HH */
