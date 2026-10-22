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

#جے_بنا_تعریف HB_FONT_HH
#تعریف_کرو HB_FONT_HH

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

بنية hb_font_funcs_t
{
  hb_object_سرخی_t سرخی;

  بنية {
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) فراغ *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_FONT_FUNC_IMPLEMENT
  } *user_انکڑے;

  بنية {
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  اتحاد get_t {
    بنية get_funcs_t {
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_FONT_FUNC_IMPLEMENT
    } f;
    فراغ (*array[0
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) +1
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_FONT_FUNC_IMPLEMENT
		]) ();
  } get;
};
DECLARE_NULL_INSTANCE (hb_font_funcs_t);


/*
 * hb_font_t
 */

#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_انکڑے_INSTANTIATE_SHAPERS(shaper, font);
#ضمن "hb-shaper-list.hh"
#تعریف_رد HB_SHAPER_IMPLEMENT

بنية hb_font_t
{
  hb_object_سرخی_t سرخی;
  hb_atomic_t<بنانشانی> serial;
  hb_atomic_t<بنانشانی> serial_coords;

  hb_font_t *ماپا;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  منطقي is_synthetic;

  عائم x_embolden;
  عائم y_embolden;
  منطقي embolden_وچ_رکھو;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  عائم slant;
  عائم slant_xy;

  عائم x_multf;
  عائم y_multf;
  int64_t x_mult;
  int64_t y_mult;

  بنانشانی صحيح x_ppem;
  بنانشانی صحيح y_ppem;

  عائم ptem;

  /* Font variation coordinates. */
  بنانشانی صحيح instance_اشاریہ;
  منطقي has_nonzero_coords;
  بنانشانی صحيح عدد_coords;
  صحيح *coords;
  عائم *design_coords;

  hb_font_funcs_t   *klass;
  فراغ              *user_انکڑے;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> انکڑے; /* Various shaper data. */


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
  hb_position_t ماپا_scale_x_فاصلہ (hb_position_t v)
  {
    إذا (unlikely (ماپا && ماپا->x_scale && ماپا->x_scale != x_scale))
      أرجع (hb_position_t) (v * (int64_t) هذا->x_scale / هذا->ماپا->x_scale);
    أرجع v;
  }
  hb_position_t ماپا_scale_y_فاصلہ (hb_position_t v)
  {
    إذا (unlikely (ماپا && ماپا->y_scale && ماپا->y_scale != y_scale))
      أرجع (hb_position_t) (v * (int64_t) هذا->y_scale / هذا->ماپا->y_scale);
    أرجع v;
  }
  hb_position_t ماپا_scale_x_position (hb_position_t v)
  { أرجع ماپا_scale_x_فاصلہ (v); }
  hb_position_t ماپا_scale_y_position (hb_position_t v)
  { أرجع ماپا_scale_y_فاصلہ (v); }

  فراغ ماپا_scale_فاصلہ (hb_position_t *x, hb_position_t *y)
  {
    *x = ماپا_scale_x_فاصلہ (*x);
    *y = ماپا_scale_y_فاصلہ (*y);
  }
  فراغ ماپا_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = ماپا_scale_x_position (*x);
    *y = ماپا_scale_y_position (*y);
  }

  فراغ scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    عائم x1 = em_fscale_x (extents->x_bearing);
    عائم y1 = em_fscale_y (extents->y_bearing);
    عائم x2 = x1 + em_fscale_x (extents->چوڑائی);
    عائم y2 = y1 + em_fscale_y (extents->اچائی);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->چوڑائی = ceilf (x2) - extents->x_bearing;
    extents->اچائی = ceilf (y2) - extents->y_bearing;
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
      int64_t y2 = (int64_t) extents->y_bearing + extents->اچائی;

      int64_t x1 = (int64_t) extents->x_bearing + (int64_t) floorf (hb_min (y1 * slant_xy, y2 * slant_xy));
      int64_t x2 = (int64_t) extents->x_bearing + extents->چوڑائی + (int64_t) ceilf (hb_max (y1 * slant_xy, y2 * slant_xy));

      extents->x_bearing = hb_clamp_منزل<hb_position_t> (x1);
      extents->چوڑائی = hb_clamp_منزل<hb_position_t> (x2 - x1);
    }

    /* Embolden. */
    إذا (x_strength || y_strength)
    {
      /* Y */
      صحيح y_کھسکاؤ = y_strength;
      إذا (y_scale < 0) y_کھسکاؤ = -y_کھسکاؤ;
      extents->y_bearing = hb_clamp_منزل<hb_position_t> ((int64_t) extents->y_bearing + y_کھسکاؤ);
      extents->اچائی = hb_clamp_منزل<hb_position_t> ((int64_t) extents->اچائی - y_کھسکاؤ);

      /* X */
      صحيح x_کھسکاؤ = x_strength;
      إذا (x_scale < 0) x_کھسکاؤ = -x_کھسکاؤ;
      إذا (embolden_وچ_رکھو)
	extents->x_bearing = hb_clamp_منزل<hb_position_t> ((int64_t) extents->x_bearing - x_کھسکاؤ / 2);
      extents->چوڑائی = hb_clamp_منزل<hb_position_t> ((int64_t) extents->چوڑائی + x_کھسکاؤ);
    }
  }


  /* Public getters */

  HB_اندرونی منطقي has_func (بنانشانی صحيح i);
  HB_اندرونی منطقي has_func_سیٹ (بنانشانی صحيح i);

  /* has_* ... */
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) \
  منطقي \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = هذا->klass; \
    بنانشانی صحيح i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / حجم (funcs->get.array[0]); \
    أرجع has_func (i); \
  } \
  منطقي \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = هذا->klass; \
    بنانشانی صحيح i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / حجم (funcs->get.array[0]); \
    أرجع has_func_سیٹ (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				منطقي synthetic = سچ)
  {
    hb_memset (extents, 0, حجم (*extents));
    منطقي ret = klass->get.f.font_h_extents (هذا, user_انکڑے,
					    extents,
					    !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->font_h_extents);

    إذا (synthetic && ret)
    {
      /* Embolden */
      صحيح y_کھسکاؤ = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_کھسکاؤ;
    }

    أرجع ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				منطقي synthetic = سچ)
  {
    hb_memset (extents, 0, حجم (*extents));
    منطقي ret = klass->get.f.font_v_extents (هذا, user_انکڑے,
					    extents,
					    !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->font_v_extents);

    إذا (synthetic && ret)
    {
      /* Embolden */
      صحيح x_کھسکاؤ = x_scale < 0 ? -x_strength : x_strength;
      إذا (embolden_وچ_رکھو)
      {
	extents->ascender += x_کھسکاؤ / 2;
	extents->descender -= x_کھسکاؤ - x_کھسکاؤ / 2;
      }
      وإلا
	extents->ascender += x_کھسکاؤ;
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
			       hb_codepoint_t not_لبھیا = 0)
  {
    *glyph = not_لبھیا;
    أرجع klass->get.f.nominal_glyph (هذا, user_انکڑے,
				       unicode, glyph,
				       !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->nominal_glyph);
  }
  بنانشانی صحيح get_nominal_glyphs (بنانشانی صحيح گنتی,
				   ثابت hb_codepoint_t *پہلا_unicode,
				   بنانشانی صحيح unicode_stride,
				   hb_codepoint_t *پہلا_glyph,
				   بنانشانی صحيح glyph_stride)
  {
    أرجع klass->get.f.nominal_glyphs (هذا, user_انکڑے,
					گنتی,
					پہلا_unicode, unicode_stride,
					پہلا_glyph, glyph_stride,
					!klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_لبھیا = 0)
  {
    *glyph = not_لبھیا;
    أرجع klass->get.f.variation_glyph (هذا, user_انکڑے,
					 unicode, variation_selector, glyph,
					 !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     منطقي synthetic = سچ)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (هذا, user_انکڑے,
							  glyph,
							  !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_h_advance);

    إذا (synthetic && x_strength && !embolden_وچ_رکھو)
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
    hb_position_t advance = klass->get.f.glyph_v_advance (هذا, user_انکڑے,
							  glyph,
							  !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_v_advance);

    إذا (synthetic && y_strength && !embolden_وچ_رکھو)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    أرجع advance;
  }

  فراغ get_glyph_h_advances (بنانشانی صحيح گنتی,
			     ثابت hb_codepoint_t *پہلا_glyph,
			     بنانشانی صحيح glyph_stride,
			     hb_position_t *پہلا_advance,
			     بنانشانی صحيح advance_stride,
			     منطقي synthetic = سچ)
  {
    klass->get.f.glyph_h_advances (هذا, user_انکڑے,
				   گنتی,
				   پہلا_glyph, glyph_stride,
				   پہلا_advance, advance_stride,
				   !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_h_advances);

    إذا (synthetic && x_strength && !embolden_وچ_رکھو)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      {
	*پہلا_advance += *پہلا_advance ? strength : 0;
	پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
      }
    }
  }

  فراغ get_glyph_v_advances (بنانشانی صحيح گنتی,
			     ثابت hb_codepoint_t *پہلا_glyph,
			     بنانشانی صحيح glyph_stride,
			     hb_position_t *پہلا_advance,
			     بنانشانی صحيح advance_stride,
			     منطقي synthetic = سچ)
  {
    klass->get.f.glyph_v_advances (هذا, user_انکڑے,
				   گنتی,
				   پہلا_glyph, glyph_stride,
				   پہلا_advance, advance_stride,
				   !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_v_advances);

    إذا (synthetic && y_strength && !embolden_وچ_رکھو)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      {
	*پہلا_advance += *پہلا_advance ? strength : 0;
	پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				منطقي synthetic = سچ)
  {
    *x = *y = 0;
    منطقي ret = klass->get.f.glyph_h_origin (هذا, user_انکڑے,
					    glyph, x, y,
					    !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_h_origin);

    إذا (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      إذا (!embolden_وچ_رکھو)
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
    منطقي ret = klass->get.f.glyph_v_origin (هذا, user_انکڑے,
					    glyph, x, y,
					    !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_v_origin);

    إذا (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      إذا (!embolden_وچ_رکھو)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_h_origins (بنانشانی صحيح گنتی,
				 ثابت hb_codepoint_t *پہلا_glyph,
				 بنانشانی صحيح glyph_stride,
				 hb_position_t *پہلا_x,
				 بنانشانی صحيح x_stride,
				 hb_position_t *پہلا_y,
				 بنانشانی صحيح y_stride,
				 منطقي synthetic = سچ)

  {
    منطقي ret = klass->get.f.glyph_h_origins (هذا, user_انکڑے,
					     گنتی,
					     پہلا_glyph, glyph_stride,
					     پہلا_x, x_stride, پہلا_y, y_stride,
					     !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_h_origins);

    إذا (synthetic && ret)
    {
      hb_position_t x_کھسکاؤ = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_کھسکاؤ = y_scale < 0 ? -y_strength : y_strength;
      لكل (بنانشانی i = 0; i < گنتی; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	إذا (!embolden_وچ_رکھو)
	{
	  *پہلا_x += x_کھسکاؤ;
	  *پہلا_y += y_کھسکاؤ;
	}
	پہلا_x = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_x, x_stride);
	پہلا_y = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_y, y_stride);
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_v_origins (بنانشانی صحيح گنتی,
				 ثابت hb_codepoint_t *پہلا_glyph,
				 بنانشانی صحيح glyph_stride,
				 hb_position_t *پہلا_x,
				 بنانشانی صحيح x_stride,
				 hb_position_t *پہلا_y,
				 بنانشانی صحيح y_stride,
				 منطقي synthetic = سچ)

  {
    منطقي ret = klass->get.f.glyph_v_origins (هذا, user_انکڑے,
					     گنتی,
					     پہلا_glyph, glyph_stride,
					     پہلا_x, x_stride, پہلا_y, y_stride,
					     !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_v_origins);

    إذا (synthetic && is_synthetic && ret)
    {
      hb_position_t x_کھسکاؤ = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_کھسکاؤ = y_scale < 0 ? -y_strength : y_strength;
      لكل (بنانشانی i = 0; i < گنتی; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	إذا (!embolden_وچ_رکھو)
	{
	  *پہلا_x += x_کھسکاؤ;
	  *پہلا_y += y_کھسکاؤ;
	}
	پہلا_x = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_x, x_stride);
	پہلا_y = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_y, y_stride);
      }
    }

    أرجع ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t بایاں_glyph,
				     hb_codepoint_t دایاں_glyph)
  {
#جے_تعریف_شدہ HB_DISABLE_DEPRECATED
    أرجع 0;
#وإلا
    أرجع klass->get.f.glyph_h_kerning (هذا, user_انکڑے,
					 بایاں_glyph, دایاں_glyph,
					 !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_h_kerning);
#جے_انت
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t چوٹی_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#جے_تعریف_شدہ HB_DISABLE_DEPRECATED
    أرجع 0;
#وإلا
    أرجع klass->get.f.glyph_v_kerning (هذا, user_انکڑے,
					 چوٹی_glyph, bottom_glyph,
					 !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_v_kerning);
#جے_انت
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       منطقي synthetic = سچ)
  {
    hb_memset (extents, 0, حجم (*extents));

    /* This is rather messy, but necessary. */

    إذا (!synthetic)
    {
      أرجع klass->get.f.glyph_extents (هذا, user_انکڑے,
					 glyph,
					 extents,
					 !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_extents);
    }
    إذا (!is_synthetic &&
	klass->get.f.glyph_extents (هذا, user_انکڑے,
				    glyph,
				    extents,
				    !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_extents))
      أرجع سچ;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#جے_بنا_تعریف HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    إذا (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().منزل_glyph_extents ();
      أرجع سچ;
    }
#جے_انت

#جے_بنا_تعریف HB_NO_DRAW
    hb_extents_t<> draw_extents;
    إذا (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.منزل_glyph_extents ();
      أرجع سچ;
    }
#جے_انت

    منطقي ret = klass->get.f.glyph_extents (هذا, user_انکڑے,
					   glyph,
					   extents,
					   !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_extents);
    إذا (ret)
      synthetic_glyph_extents (extents);

    أرجع ret;
  }

  hb_bool_t get_glyph_contour_نقطہ (hb_codepoint_t glyph, بنانشانی صحيح نقطہ_اشاریہ,
				     hb_position_t *x, hb_position_t *y,
				     منطقي synthetic = سچ)
  {
    *x = *y = 0;
    منطقي ret = klass->get.f.glyph_contour_نقطہ (هذا, user_انکڑے,
						 glyph, نقطہ_اشاریہ,
						 x, y,
						 !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_contour_نقطہ);

    إذا (synthetic && ret)
    {
      /* Slant */
      إذا (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      إذا (!embolden_وچ_رکھو)
      {
	صحيح x_کھسکاؤ = x_scale < 0 ? -x_strength : x_strength;
	*x += x_کھسکاؤ;
      }
    }

    أرجع ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    حرف *name, بنانشانی صحيح ناپ)
  {
    إذا (ناپ) *name = '\0';
    أرجع klass->get.f.glyph_name (هذا, user_انکڑے,
				    glyph,
				    name, ناپ,
				    !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_name);
  }

  hb_bool_t get_glyph_from_name (ثابت حرف *name, صحيح len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    إذا (len == -1) len = لڑی_لمبائی (name);
    أرجع klass->get.f.glyph_from_name (هذا, user_انکڑے,
					 name, len,
					 glyph,
					 !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->glyph_from_name);
  }

  منطقي draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, فراغ *draw_انکڑے,
			   منطقي synthetic = سچ)
  {
#جے_بنا_تعریف HB_NO_DRAW
#جے_بنا_تعریف HB_NO_OUTLINE
    منطقي embolden = x_strength || y_strength;
    منطقي slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#وإلا
    synthetic = خطأ;
#جے_انت

    إذا (!synthetic)
    {
      أرجع klass->get.f.draw_glyph_or_fail (هذا, user_انکڑے,
					      glyph,
					      draw_funcs, draw_انکڑے,
					      !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->draw_glyph_or_fail);
    }

#جے_بنا_تعریف HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_بچیا = draw_funcs->get_budget_بچیا_ptr (draw_انکڑے);
    إذا (!klass->get.f.draw_glyph_or_fail (هذا, user_انکڑے,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->draw_glyph_or_fail))
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
      عائم x_کھسکاؤ = embolden_وچ_رکھو ? 0 : (عائم) x_strength / 2;
      عائم y_کھسکاؤ = (عائم) y_strength / 2;
      إذا (x_scale < 0) x_کھسکاؤ = -x_کھسکاؤ;
      إذا (y_scale < 0) y_کھسکاؤ = -y_کھسکاؤ;
      outline.embolden (x_strength, y_strength, x_کھسکاؤ, y_کھسکاؤ);
    }

    outline.replay (draw_funcs, draw_انکڑے);

    أرجع سچ;
#جے_انت
#جے_انت
    أرجع خطأ;
  }

  منطقي paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, فراغ *paint_انکڑے,
			    بنانشانی صحيح palette,
			    hb_color_t foreground,
			    منطقي synthetic = سچ)
  {
#جے_بنا_تعریف HB_NO_PAINT
    /* Slant */
    إذا (synthetic && slant_xy)
      hb_paint_ڈالو_transform (paint_funcs, paint_انکڑے,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    منطقي ret = klass->get.f.paint_glyph_or_fail (هذا, user_انکڑے,
						 glyph,
						 paint_funcs, paint_انکڑے,
						 palette, foreground,
						 !klass->user_انکڑے ? خالی_سوچک : klass->user_انکڑے->paint_glyph_or_fail);

    إذا (synthetic && slant_xy)
      hb_paint_نکالو_transform (paint_funcs, paint_انکڑے);

    أرجع ret;
#جے_انت
    أرجع خطأ;
  }

  /* A bit higher-level, and with fallback */

  HB_اندرونی
  فراغ paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, فراغ *paint_انکڑے,
		    بنانشانی صحيح palette,
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
					 بنانشانی صحيح گنتی,
					 ثابت hb_codepoint_t *پہلا_glyph,
					 بنانشانی glyph_stride,
					 hb_position_t *پہلا_advance,
					 بنانشانی advance_stride)
  {
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (گنتی, پہلا_glyph, glyph_stride, پہلا_advance, advance_stride);
    وإلا
      get_glyph_v_advances (گنتی, پہلا_glyph, glyph_stride, پہلا_advance, advance_stride);
  }

  فراغ apply_وتھ (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     نشانیوالا mult)
  {
    أكد (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  فراغ جوڑو_وتھ (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  فراغ گھٹاؤ_وتھ (hb_position_t *x, hb_position_t *y,
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

    بنانشانی صحيح وتھ = 0;
    بنانشانی صحيح گنتی = buf->len;
    طالما (وتھ < گنتی)
    {
      بنانشانی n = hb_min (گنتی - وتھ, ARRAY_لمائی (origins));
      إذا (!get_glyph_h_origins (n,
				&buf->info[وتھ].codepoint, حجم (hb_glyph_info_t),
				&origins[0].x, حجم (origins[0]),
				&origins[0].y, حجم (origins[0])))
      {
        إذا (get_glyph_v_origins (n,
				  &buf->info[وتھ].codepoint, حجم (hb_glyph_info_t),
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
	  لكل (بنانشانی j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[وتھ + j].codepoint;
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
        لكل (بنانشانی j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[وتھ + j];
	  جوڑو_وتھ (&pos->x_وتھ_2, &pos->y_وتھ_2,
		      origins[j].x, origins[j].y);
	}
      وإلا إذا (mult == -1)
	لكل (بنانشانی j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[وتھ + j];
	  گھٹاؤ_وتھ (&pos->x_وتھ_2, &pos->y_وتھ_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      وتھ += n;
    }
  }
  فراغ apply_glyph_v_origins_with_fallback (hb_بفر_t *buf, صحيح mult)
  {
    منطقي has_ascender = خطأ;
    hb_position_t ascender = 0;

    بنية { hb_position_t x, y; } origins[32];

    بنانشانی صحيح وتھ = 0;
    بنانشانی صحيح گنتی = buf->len;
    طالما (وتھ < گنتی)
    {
      بنانشانی n = hb_min (گنتی - وتھ, ARRAY_لمائی (origins));
      إذا (!get_glyph_v_origins (n,
				&buf->info[وتھ].codepoint, حجم (hb_glyph_info_t),
				&origins[0].x, حجم (origins[0]),
				&origins[0].y, حجم (origins[0])))
      {
	إذا (get_glyph_h_origins (n,
				 &buf->info[وتھ].codepoint, حجم (hb_glyph_info_t),
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
	  لكل (بنانشانی j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[وتھ + j].codepoint;
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
        لكل (بنانشانی j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[وتھ + j];
	  جوڑو_وتھ (&pos->x_وتھ_2, &pos->y_وتھ_2,
		      origins[j].x, origins[j].y);
	}
      وإلا إذا (mult == -1)
	لكل (بنانشانی j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[وتھ + j];
	  گھٹاؤ_وتھ (&pos->x_وتھ_2, &pos->y_وتھ_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      وتھ += n;
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
      گھٹاؤ_وتھ (x, y, dx, dy);
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
      جوڑو_وتھ (x, y, dx, dy);
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

  فراغ جوڑو_glyph_h_origins (hb_بفر_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  فراغ جوڑو_glyph_v_origins (hb_بفر_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  فراغ جوڑو_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    جوڑو_وتھ (x, y, origin_x, origin_y);
  }

  فراغ گھٹاؤ_glyph_h_origins (hb_بفر_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  فراغ گھٹاؤ_glyph_v_origins (hb_بفر_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  فراغ گھٹاؤ_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    گھٹاؤ_وتھ (x, y, origin_x, origin_y);
  }

  فراغ get_glyph_kerning_for_direction (hb_codepoint_t پہلا_glyph, hb_codepoint_t دوجا_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (پہلا_glyph, دوجا_glyph);
    } وإلا {
      *x = 0;
      *y = get_glyph_v_kerning (پہلا_glyph, دوجا_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    إذا (ret)
      گھٹاؤ_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    أرجع ret;
  }

  hb_bool_t get_glyph_contour_نقطہ_for_origin (hb_codepoint_t glyph, بنانشانی صحيح نقطہ_اشاریہ,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_نقطہ (glyph, نقطہ_اشاریہ, x, y);

    إذا (ret)
      گھٹاؤ_glyph_origin_for_direction (glyph, direction, x, y);

    أرجع ret;
  }

  /* Generates gidDDD if glyph has no name. */
  فراغ
  glyph_منزل_string (hb_codepoint_t glyph,
		   حرف *s, بنانشانی صحيح ناپ)
  {
    إذا (get_glyph_name (glyph, s, ناپ)) أرجع;

    إذا (ناپ && snprintf (s, ناپ, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (ثابت حرف *s, صحيح len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    إذا (get_glyph_from_name (s, len, glyph)) أرجع سچ;

    إذا (len == -1) len = لڑی_لمبائی (s);

    /* Straight glyph index. */
    إذا (hb_codepoint_parse (s, len, 10, glyph))
      أرجع سچ;

    إذا (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      إذا (0 == لڑی_مقابلہ (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	أرجع سچ;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      إذا (0 == لڑی_مقابلہ (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	أرجع سچ;
    }

    أرجع خطأ;
  }

  فراغ بدلیا ()
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

    انکڑے.fini ();

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


#جے_انت /* HB_FONT_HH */
