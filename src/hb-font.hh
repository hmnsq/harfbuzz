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

#اگر_تعریف_نشده HB_FONT_HH
#تعریف_کن HB_FONT_HH

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

#تعریف_کن HB_FONT_FUNCS_IMPLEMENT_CALLBACKS \
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
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_contour_نقطه) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_name) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_from_name) \
  HB_FONT_FUNC_IMPLEMENT (,draw_glyph_or_fail) \
  HB_FONT_FUNC_IMPLEMENT (,paint_glyph_or_fail) \
  /* ^--- Add new callbacks here */

بنية hb_font_funcs_t
{
  hb_object_سرآیند_t سرآیند;

  بنية {
#تعریف_کن HB_FONT_FUNC_IMPLEMENT(get_,name) فراغ *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_FONT_FUNC_IMPLEMENT
  } *user_داده;

  بنية {
#تعریف_کن HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  اتحاد get_t {
    بنية get_funcs_t {
#تعریف_کن HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_FONT_FUNC_IMPLEMENT
    } f;
    فراغ (*array[0
#تعریف_کن HB_FONT_FUNC_IMPLEMENT(get_,name) +1
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_FONT_FUNC_IMPLEMENT
		]) ();
  } get;
};
DECLARE_NULL_INSTANCE (hb_font_funcs_t);


/*
 * hb_font_t
 */

#تعریف_کن HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_داده_INSTANTIATE_SHAPERS(shaper, font);
#ضمن "hb-shaper-list.hh"
#لغو_تعریف HB_SHAPER_IMPLEMENT

بنية hb_font_t
{
  hb_object_سرآیند_t سرآیند;
  hb_atomic_t<بیعلامت> serial;
  hb_atomic_t<بیعلامت> serial_coords;

  hb_font_t *والد;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  منطقي is_synthetic;

  عائم x_embolden;
  عائم y_embolden;
  منطقي embolden_درون_قراربده;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  عائم slant;
  عائم slant_xy;

  عائم x_multf;
  عائم y_multf;
  int64_t x_mult;
  int64_t y_mult;

  بیعلامت صحيح x_ppem;
  بیعلامت صحيح y_ppem;

  عائم ptem;

  /* Font variation coordinates. */
  بیعلامت صحيح instance_شاخص;
  منطقي has_nonzero_coords;
  بیعلامت صحيح رقم_coords;
  صحيح *coords;
  عائم *design_coords;

  hb_font_funcs_t   *klass;
  فراغ              *user_داده;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> داده; /* Various shaper data. */


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
  hb_position_t والد_scale_x_فاصله (hb_position_t v)
  {
    إذا (unlikely (والد && والد->x_scale && والد->x_scale != x_scale))
      أرجع (hb_position_t) (v * (int64_t) هذا->x_scale / هذا->والد->x_scale);
    أرجع v;
  }
  hb_position_t والد_scale_y_فاصله (hb_position_t v)
  {
    إذا (unlikely (والد && والد->y_scale && والد->y_scale != y_scale))
      أرجع (hb_position_t) (v * (int64_t) هذا->y_scale / هذا->والد->y_scale);
    أرجع v;
  }
  hb_position_t والد_scale_x_position (hb_position_t v)
  { أرجع والد_scale_x_فاصله (v); }
  hb_position_t والد_scale_y_position (hb_position_t v)
  { أرجع والد_scale_y_فاصله (v); }

  فراغ والد_scale_فاصله (hb_position_t *x, hb_position_t *y)
  {
    *x = والد_scale_x_فاصله (*x);
    *y = والد_scale_y_فاصله (*y);
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
    عائم x2 = x1 + em_fscale_x (extents->عرض);
    عائم y2 = y1 + em_fscale_y (extents->ارتفاع);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->عرض = ceilf (x2) - extents->x_bearing;
    extents->ارتفاع = ceilf (y2) - extents->y_bearing;
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
      int64_t y2 = (int64_t) extents->y_bearing + extents->ارتفاع;

      int64_t x1 = (int64_t) extents->x_bearing + (int64_t) floorf (hb_min (y1 * slant_xy, y2 * slant_xy));
      int64_t x2 = (int64_t) extents->x_bearing + extents->عرض + (int64_t) ceilf (hb_max (y1 * slant_xy, y2 * slant_xy));

      extents->x_bearing = hb_clamp_مقصد<hb_position_t> (x1);
      extents->عرض = hb_clamp_مقصد<hb_position_t> (x2 - x1);
    }

    /* Embolden. */
    إذا (x_strength || y_strength)
    {
      /* Y */
      صحيح y_لغزش = y_strength;
      إذا (y_scale < 0) y_لغزش = -y_لغزش;
      extents->y_bearing = hb_clamp_مقصد<hb_position_t> ((int64_t) extents->y_bearing + y_لغزش);
      extents->ارتفاع = hb_clamp_مقصد<hb_position_t> ((int64_t) extents->ارتفاع - y_لغزش);

      /* X */
      صحيح x_لغزش = x_strength;
      إذا (x_scale < 0) x_لغزش = -x_لغزش;
      إذا (embolden_درون_قراربده)
	extents->x_bearing = hb_clamp_مقصد<hb_position_t> ((int64_t) extents->x_bearing - x_لغزش / 2);
      extents->عرض = hb_clamp_مقصد<hb_position_t> ((int64_t) extents->عرض + x_لغزش);
    }
  }


  /* Public getters */

  HB_داخلی منطقي has_func (بیعلامت صحيح i);
  HB_داخلی منطقي has_func_مجموعه (بیعلامت صحيح i);

  /* has_* ... */
#تعریف_کن HB_FONT_FUNC_IMPLEMENT(get_,name) \
  منطقي \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = هذا->klass; \
    بیعلامت صحيح i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / حجم (funcs->get.array[0]); \
    أرجع has_func (i); \
  } \
  منطقي \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = هذا->klass; \
    بیعلامت صحيح i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / حجم (funcs->get.array[0]); \
    أرجع has_func_مجموعه (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#لغو_تعریف HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				منطقي synthetic = درست)
  {
    hb_memset (extents, 0, حجم (*extents));
    منطقي ret = klass->get.f.font_h_extents (هذا, user_داده,
					    extents,
					    !klass->user_داده ? اشاره_تهی : klass->user_داده->font_h_extents);

    إذا (synthetic && ret)
    {
      /* Embolden */
      صحيح y_لغزش = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_لغزش;
    }

    أرجع ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				منطقي synthetic = درست)
  {
    hb_memset (extents, 0, حجم (*extents));
    منطقي ret = klass->get.f.font_v_extents (هذا, user_داده,
					    extents,
					    !klass->user_داده ? اشاره_تهی : klass->user_داده->font_v_extents);

    إذا (synthetic && ret)
    {
      /* Embolden */
      صحيح x_لغزش = x_scale < 0 ? -x_strength : x_strength;
      إذا (embolden_درون_قراربده)
      {
	extents->ascender += x_لغزش / 2;
	extents->descender -= x_لغزش - x_لغزش / 2;
      }
      وإلا
	extents->ascender += x_لغزش;
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
			       hb_codepoint_t not_یافت = 0)
  {
    *glyph = not_یافت;
    أرجع klass->get.f.nominal_glyph (هذا, user_داده,
				       unicode, glyph,
				       !klass->user_داده ? اشاره_تهی : klass->user_داده->nominal_glyph);
  }
  بیعلامت صحيح get_nominal_glyphs (بیعلامت صحيح شمار,
				   ثابت hb_codepoint_t *اولی_unicode,
				   بیعلامت صحيح unicode_stride,
				   hb_codepoint_t *اولی_glyph,
				   بیعلامت صحيح glyph_stride)
  {
    أرجع klass->get.f.nominal_glyphs (هذا, user_داده,
					شمار,
					اولی_unicode, unicode_stride,
					اولی_glyph, glyph_stride,
					!klass->user_داده ? اشاره_تهی : klass->user_داده->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_یافت = 0)
  {
    *glyph = not_یافت;
    أرجع klass->get.f.variation_glyph (هذا, user_داده,
					 unicode, variation_selector, glyph,
					 !klass->user_داده ? اشاره_تهی : klass->user_داده->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     منطقي synthetic = درست)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (هذا, user_داده,
							  glyph,
							  !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_h_advance);

    إذا (synthetic && x_strength && !embolden_درون_قراربده)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    أرجع advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     منطقي synthetic = درست)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (هذا, user_داده,
							  glyph,
							  !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_v_advance);

    إذا (synthetic && y_strength && !embolden_درون_قراربده)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    أرجع advance;
  }

  فراغ get_glyph_h_advances (بیعلامت صحيح شمار,
			     ثابت hb_codepoint_t *اولی_glyph,
			     بیعلامت صحيح glyph_stride,
			     hb_position_t *اولی_advance,
			     بیعلامت صحيح advance_stride,
			     منطقي synthetic = درست)
  {
    klass->get.f.glyph_h_advances (هذا, user_داده,
				   شمار,
				   اولی_glyph, glyph_stride,
				   اولی_advance, advance_stride,
				   !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_h_advances);

    إذا (synthetic && x_strength && !embolden_درون_قراربده)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      {
	*اولی_advance += *اولی_advance ? strength : 0;
	اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
      }
    }
  }

  فراغ get_glyph_v_advances (بیعلامت صحيح شمار,
			     ثابت hb_codepoint_t *اولی_glyph,
			     بیعلامت صحيح glyph_stride,
			     hb_position_t *اولی_advance,
			     بیعلامت صحيح advance_stride,
			     منطقي synthetic = درست)
  {
    klass->get.f.glyph_v_advances (هذا, user_داده,
				   شمار,
				   اولی_glyph, glyph_stride,
				   اولی_advance, advance_stride,
				   !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_v_advances);

    إذا (synthetic && y_strength && !embolden_درون_قراربده)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      {
	*اولی_advance += *اولی_advance ? strength : 0;
	اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				منطقي synthetic = درست)
  {
    *x = *y = 0;
    منطقي ret = klass->get.f.glyph_h_origin (هذا, user_داده,
					    glyph, x, y,
					    !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_h_origin);

    إذا (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      إذا (!embolden_درون_قراربده)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				منطقي synthetic = درست)
  {
    *x = *y = 0;
    منطقي ret = klass->get.f.glyph_v_origin (هذا, user_داده,
					    glyph, x, y,
					    !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_v_origin);

    إذا (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      إذا (!embolden_درون_قراربده)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_h_origins (بیعلامت صحيح شمار,
				 ثابت hb_codepoint_t *اولی_glyph,
				 بیعلامت صحيح glyph_stride,
				 hb_position_t *اولی_x,
				 بیعلامت صحيح x_stride,
				 hb_position_t *اولی_y,
				 بیعلامت صحيح y_stride,
				 منطقي synthetic = درست)

  {
    منطقي ret = klass->get.f.glyph_h_origins (هذا, user_داده,
					     شمار,
					     اولی_glyph, glyph_stride,
					     اولی_x, x_stride, اولی_y, y_stride,
					     !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_h_origins);

    إذا (synthetic && ret)
    {
      hb_position_t x_لغزش = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_لغزش = y_scale < 0 ? -y_strength : y_strength;
      لكل (بیعلامت i = 0; i < شمار; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	إذا (!embolden_درون_قراربده)
	{
	  *اولی_x += x_لغزش;
	  *اولی_y += y_لغزش;
	}
	اولی_x = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_x, x_stride);
	اولی_y = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_y, y_stride);
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_v_origins (بیعلامت صحيح شمار,
				 ثابت hb_codepoint_t *اولی_glyph,
				 بیعلامت صحيح glyph_stride,
				 hb_position_t *اولی_x,
				 بیعلامت صحيح x_stride,
				 hb_position_t *اولی_y,
				 بیعلامت صحيح y_stride,
				 منطقي synthetic = درست)

  {
    منطقي ret = klass->get.f.glyph_v_origins (هذا, user_داده,
					     شمار,
					     اولی_glyph, glyph_stride,
					     اولی_x, x_stride, اولی_y, y_stride,
					     !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_v_origins);

    إذا (synthetic && is_synthetic && ret)
    {
      hb_position_t x_لغزش = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_لغزش = y_scale < 0 ? -y_strength : y_strength;
      لكل (بیعلامت i = 0; i < شمار; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	إذا (!embolden_درون_قراربده)
	{
	  *اولی_x += x_لغزش;
	  *اولی_y += y_لغزش;
	}
	اولی_x = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_x, x_stride);
	اولی_y = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_y, y_stride);
      }
    }

    أرجع ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t چپ_glyph,
				     hb_codepoint_t راست_glyph)
  {
#اگر_تعریف_شده HB_DISABLE_DEPRECATED
    أرجع 0;
#وإلا
    أرجع klass->get.f.glyph_h_kerning (هذا, user_داده,
					 چپ_glyph, راست_glyph,
					 !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_h_kerning);
#پایان_اگر
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t بالاترین_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#اگر_تعریف_شده HB_DISABLE_DEPRECATED
    أرجع 0;
#وإلا
    أرجع klass->get.f.glyph_v_kerning (هذا, user_داده,
					 بالاترین_glyph, bottom_glyph,
					 !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_v_kerning);
#پایان_اگر
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       منطقي synthetic = درست)
  {
    hb_memset (extents, 0, حجم (*extents));

    /* This is rather messy, but necessary. */

    إذا (!synthetic)
    {
      أرجع klass->get.f.glyph_extents (هذا, user_داده,
					 glyph,
					 extents,
					 !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_extents);
    }
    إذا (!is_synthetic &&
	klass->get.f.glyph_extents (هذا, user_داده,
				    glyph,
				    extents,
				    !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_extents))
      أرجع درست;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#اگر_تعریف_نشده HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    إذا (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().مقصد_glyph_extents ();
      أرجع درست;
    }
#پایان_اگر

#اگر_تعریف_نشده HB_NO_DRAW
    hb_extents_t<> draw_extents;
    إذا (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.مقصد_glyph_extents ();
      أرجع درست;
    }
#پایان_اگر

    منطقي ret = klass->get.f.glyph_extents (هذا, user_داده,
					   glyph,
					   extents,
					   !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_extents);
    إذا (ret)
      synthetic_glyph_extents (extents);

    أرجع ret;
  }

  hb_bool_t get_glyph_contour_نقطه (hb_codepoint_t glyph, بیعلامت صحيح نقطه_شاخص,
				     hb_position_t *x, hb_position_t *y,
				     منطقي synthetic = درست)
  {
    *x = *y = 0;
    منطقي ret = klass->get.f.glyph_contour_نقطه (هذا, user_داده,
						 glyph, نقطه_شاخص,
						 x, y,
						 !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_contour_نقطه);

    إذا (synthetic && ret)
    {
      /* Slant */
      إذا (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      إذا (!embolden_درون_قراربده)
      {
	صحيح x_لغزش = x_scale < 0 ? -x_strength : x_strength;
	*x += x_لغزش;
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    حرف *name, بیعلامت صحيح اندازه)
  {
    إذا (اندازه) *name = '\0';
    أرجع klass->get.f.glyph_name (هذا, user_داده,
				    glyph,
				    name, اندازه,
				    !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_name);
  }

  hb_bool_t get_glyph_from_name (ثابت حرف *name, صحيح len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    إذا (len == -1) len = طول_رشته (name);
    أرجع klass->get.f.glyph_from_name (هذا, user_داده,
					 name, len,
					 glyph,
					 !klass->user_داده ? اشاره_تهی : klass->user_داده->glyph_from_name);
  }

  منطقي draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, فراغ *draw_داده,
			   منطقي synthetic = درست)
  {
#اگر_تعریف_نشده HB_NO_DRAW
#اگر_تعریف_نشده HB_NO_OUTLINE
    منطقي embolden = x_strength || y_strength;
    منطقي slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#وإلا
    synthetic = خطأ;
#پایان_اگر

    إذا (!synthetic)
    {
      أرجع klass->get.f.draw_glyph_or_fail (هذا, user_داده,
					      glyph,
					      draw_funcs, draw_داده,
					      !klass->user_داده ? اشاره_تهی : klass->user_داده->draw_glyph_or_fail);
    }

#اگر_تعریف_نشده HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_مانده = draw_funcs->get_budget_مانده_ptr (draw_داده);
    إذا (!klass->get.f.draw_glyph_or_fail (هذا, user_داده,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_داده ? اشاره_تهی : klass->user_داده->draw_glyph_or_fail))
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
      عائم x_لغزش = embolden_درون_قراربده ? 0 : (عائم) x_strength / 2;
      عائم y_لغزش = (عائم) y_strength / 2;
      إذا (x_scale < 0) x_لغزش = -x_لغزش;
      إذا (y_scale < 0) y_لغزش = -y_لغزش;
      outline.embolden (x_strength, y_strength, x_لغزش, y_لغزش);
    }

    outline.replay (draw_funcs, draw_داده);

    أرجع درست;
#پایان_اگر
#پایان_اگر
    أرجع خطأ;
  }

  منطقي paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, فراغ *paint_داده,
			    بیعلامت صحيح palette,
			    hb_color_t foreground,
			    منطقي synthetic = درست)
  {
#اگر_تعریف_نشده HB_NO_PAINT
    /* Slant */
    إذا (synthetic && slant_xy)
      hb_paint_افزودن_transform (paint_funcs, paint_داده,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    منطقي ret = klass->get.f.paint_glyph_or_fail (هذا, user_داده,
						 glyph,
						 paint_funcs, paint_داده,
						 palette, foreground,
						 !klass->user_داده ? اشاره_تهی : klass->user_داده->paint_glyph_or_fail);

    إذا (synthetic && slant_xy)
      hb_paint_برداشت_transform (paint_funcs, paint_داده);

    أرجع ret;
#پایان_اگر
    أرجع خطأ;
  }

  /* A bit higher-level, and with fallback */

  HB_داخلی
  فراغ paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, فراغ *paint_داده,
		    بیعلامت صحيح palette,
		    hb_color_t foreground);

  فراغ get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    إذا (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->سطر_gap = 0;
    }
  }
  فراغ get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    إذا (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->سطر_gap = 0;
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
					 بیعلامت صحيح شمار,
					 ثابت hb_codepoint_t *اولی_glyph,
					 بیعلامت glyph_stride,
					 hb_position_t *اولی_advance,
					 بیعلامت advance_stride)
  {
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (شمار, اولی_glyph, glyph_stride, اولی_advance, advance_stride);
    وإلا
      get_glyph_v_advances (شمار, اولی_glyph, glyph_stride, اولی_advance, advance_stride);
  }

  فراغ apply_جابه_جایی (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     علامتدار mult)
  {
    أكد (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  فراغ جمع_کن_جابه_جایی (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  فراغ تفریق_جابه_جایی (hb_position_t *x, hb_position_t *y,
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

  فراغ apply_glyph_h_origins_with_fallback (hb_میانگیر_t *buf, صحيح mult)
  {
    منطقي has_ascender = خطأ;
    hb_position_t ascender = 0;

    بنية { hb_position_t x, y; } origins[32];

    بیعلامت صحيح جابه_جایی = 0;
    بیعلامت صحيح شمار = buf->len;
    طالما (جابه_جایی < شمار)
    {
      بیعلامت n = hb_min (شمار - جابه_جایی, ARRAY_درازی (origins));
      إذا (!get_glyph_h_origins (n,
				&buf->info[جابه_جایی].codepoint, حجم (hb_glyph_info_t),
				&origins[0].x, حجم (origins[0]),
				&origins[0].y, حجم (origins[0])))
      {
        إذا (get_glyph_v_origins (n,
				  &buf->info[جابه_جایی].codepoint, حجم (hb_glyph_info_t),
				  &origins[0].x, حجم (origins[0]),
				  &origins[0].y, حجم (origins[0])))
	{
	  إذا (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = درست;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  لكل (بیعلامت j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[جابه_جایی + j].codepoint;
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
        لكل (بیعلامت j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[جابه_جایی + j];
	  جمع_کن_جابه_جایی (&pos->x_جابه_جایی_2, &pos->y_جابه_جایی_2,
		      origins[j].x, origins[j].y);
	}
      وإلا إذا (mult == -1)
	لكل (بیعلامت j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[جابه_جایی + j];
	  تفریق_جابه_جایی (&pos->x_جابه_جایی_2, &pos->y_جابه_جایی_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      جابه_جایی += n;
    }
  }
  فراغ apply_glyph_v_origins_with_fallback (hb_میانگیر_t *buf, صحيح mult)
  {
    منطقي has_ascender = خطأ;
    hb_position_t ascender = 0;

    بنية { hb_position_t x, y; } origins[32];

    بیعلامت صحيح جابه_جایی = 0;
    بیعلامت صحيح شمار = buf->len;
    طالما (جابه_جایی < شمار)
    {
      بیعلامت n = hb_min (شمار - جابه_جایی, ARRAY_درازی (origins));
      إذا (!get_glyph_v_origins (n,
				&buf->info[جابه_جایی].codepoint, حجم (hb_glyph_info_t),
				&origins[0].x, حجم (origins[0]),
				&origins[0].y, حجم (origins[0])))
      {
	إذا (get_glyph_h_origins (n,
				 &buf->info[جابه_جایی].codepoint, حجم (hb_glyph_info_t),
				 &origins[0].x, حجم (origins[0]),
				 &origins[0].y, حجم (origins[0])))
	{
	  إذا (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = درست;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  لكل (بیعلامت j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[جابه_جایی + j].codepoint;
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
        لكل (بیعلامت j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[جابه_جایی + j];
	  جمع_کن_جابه_جایی (&pos->x_جابه_جایی_2, &pos->y_جابه_جایی_2,
		      origins[j].x, origins[j].y);
	}
      وإلا إذا (mult == -1)
	لكل (بیعلامت j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[جابه_جایی + j];
	  تفریق_جابه_جایی (&pos->x_جابه_جایی_2, &pos->y_جابه_جایی_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      جابه_جایی += n;
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
      تفریق_جابه_جایی (x, y, dx, dy);
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
      جمع_کن_جابه_جایی (x, y, dx, dy);
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

  فراغ جمع_کن_glyph_h_origins (hb_میانگیر_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  فراغ جمع_کن_glyph_v_origins (hb_میانگیر_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  فراغ جمع_کن_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    جمع_کن_جابه_جایی (x, y, origin_x, origin_y);
  }

  فراغ تفریق_glyph_h_origins (hb_میانگیر_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  فراغ تفریق_glyph_v_origins (hb_میانگیر_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  فراغ تفریق_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    تفریق_جابه_جایی (x, y, origin_x, origin_y);
  }

  فراغ get_glyph_kerning_for_direction (hb_codepoint_t اولی_glyph, hb_codepoint_t دومی_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (اولی_glyph, دومی_glyph);
    } وإلا {
      *x = 0;
      *y = get_glyph_v_kerning (اولی_glyph, دومی_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    إذا (ret)
      تفریق_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    أرجع ret;
  }

  hb_bool_t get_glyph_contour_نقطه_for_origin (hb_codepoint_t glyph, بیعلامت صحيح نقطه_شاخص,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_نقطه (glyph, نقطه_شاخص, x, y);

    إذا (ret)
      تفریق_glyph_origin_for_direction (glyph, direction, x, y);

    أرجع ret;
  }

  /* Generates gidDDD if glyph has no name. */
  فراغ
  glyph_مقصد_string (hb_codepoint_t glyph,
		   حرف *s, بیعلامت صحيح اندازه)
  {
    إذا (get_glyph_name (glyph, s, اندازه)) أرجع;

    إذا (اندازه && snprintf (s, اندازه, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (ثابت حرف *s, صحيح len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    إذا (get_glyph_from_name (s, len, glyph)) أرجع درست;

    إذا (len == -1) len = طول_رشته (s);

    /* Straight glyph index. */
    إذا (hb_codepoint_parse (s, len, 10, glyph))
      أرجع درست;

    إذا (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      إذا (0 == مقایسه_رشته (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	أرجع درست;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      إذا (0 == مقایسه_رشته (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	أرجع درست;
    }

    أرجع خطأ;
  }

  فراغ تغییریافته ()
  {
    عائم upem = face->get_upem ();

    x_multf = x_scale / upem;
    y_multf = y_scale / upem;
    x_mult = (int64_t) x_scale * 0x10000 / upem;
    y_mult = (int64_t) y_scale * 0x10000 / upem;

    is_synthetic =  x_embolden || y_embolden || slant;

    x_strength = roundf (قدر_مطلق (x_scale) * x_embolden);
    y_strength = roundf (قدر_مطلق (y_scale) * y_embolden);

    slant_xy = y_scale ? slant * x_scale / y_scale : 0.f;

    داده.fini ();

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


#پایان_اگر /* HB_FONT_HH */
