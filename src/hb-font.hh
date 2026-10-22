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

#ifndef HB_FONT_HH
#define HB_FONT_HH

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

#define HB_FONT_FUNCS_IMPLEMENT_CALLBACKS \
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
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_contour_point) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_name) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_from_name) \
  HB_FONT_FUNC_IMPLEMENT (,draw_glyph_or_fail) \
  HB_FONT_FUNC_IMPLEMENT (,paint_glyph_or_fail) \
  /* ^--- Add new callbacks here */

ساختار hb_font_funcs_t
{
  hb_object_header_t header;

  ساختار {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) تهی *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *user_data;

  ساختار {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  اجتماع get_t {
    ساختار get_funcs_t {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
    } f;
    تهی (*array[0
#define HB_FONT_FUNC_IMPLEMENT(get_,name) +1
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
		]) ();
  } get;
};
DECLARE_NULL_INSTANCE (hb_font_funcs_t);


/*
 * hb_font_t
 */

#define HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, font);
#شامل "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ساختار hb_font_t
{
  hb_object_header_t header;
  hb_atomic_t<unsigned> serial;
  hb_atomic_t<unsigned> serial_coords;

  hb_font_t *parent;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  منطقی is_synthetic;

  اعشاری x_embolden;
  اعشاری y_embolden;
  منطقی embolden_in_place;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  اعشاری slant;
  اعشاری slant_xy;

  اعشاری x_multf;
  اعشاری y_multf;
  int64_t x_mult;
  int64_t y_mult;

  unsigned صحیح x_ppem;
  unsigned صحیح y_ppem;

  اعشاری ptem;

  /* Font variation coordinates. */
  unsigned صحیح instance_index;
  منطقی has_nonzero_coords;
  unsigned صحیح رقم_coords;
  صحیح *coords;
  اعشاری *design_coords;

  hb_font_funcs_t   *klass;
  تهی              *user_data;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> data; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { بازگشت HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { بازگشت em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { بازگشت em_mult (v, y_mult); }
  hb_position_t em_scalef_x (اعشاری v) { بازگشت em_multf (v, x_multf); }
  hb_position_t em_scalef_y (اعشاری v) { بازگشت em_multf (v, y_multf); }
  اعشاری em_fscale_x (int32_t v) { بازگشت em_fmult (v, x_multf); }
  اعشاری em_fscale_y (int32_t v) { بازگشت em_fmult (v, y_multf); }
  اعشاری em_fscalef_x (اعشاری v) { بازگشت em_fmultf (v, x_multf); }
  اعشاری em_fscalef_y (اعشاری v) { بازگشت em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { بازگشت em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t parent_scale_x_distance (hb_position_t v)
  {
    اگر (unlikely (parent && parent->x_scale && parent->x_scale != x_scale))
      بازگشت (hb_position_t) (v * (int64_t) این->x_scale / این->parent->x_scale);
    بازگشت v;
  }
  hb_position_t parent_scale_y_distance (hb_position_t v)
  {
    اگر (unlikely (parent && parent->y_scale && parent->y_scale != y_scale))
      بازگشت (hb_position_t) (v * (int64_t) این->y_scale / این->parent->y_scale);
    بازگشت v;
  }
  hb_position_t parent_scale_x_position (hb_position_t v)
  { بازگشت parent_scale_x_distance (v); }
  hb_position_t parent_scale_y_position (hb_position_t v)
  { بازگشت parent_scale_y_distance (v); }

  تهی parent_scale_distance (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_distance (*x);
    *y = parent_scale_y_distance (*y);
  }
  تهی parent_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_position (*x);
    *y = parent_scale_y_position (*y);
  }

  تهی scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    اعشاری x1 = em_fscale_x (extents->x_bearing);
    اعشاری y1 = em_fscale_y (extents->y_bearing);
    اعشاری x2 = x1 + em_fscale_x (extents->width);
    اعشاری y2 = y1 + em_fscale_y (extents->height);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->width = ceilf (x2) - extents->x_bearing;
    extents->height = ceilf (y2) - extents->y_bearing;
  }

  تهی synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    اگر (slant_xy)
    {
      /* x_bearing/width and y_bearing/height are each clamped to the full
       * hb_position_t range independently by the extent producers, so their
       * sums and differences can overflow int32.  Accumulate in int64. */
      int64_t y1 = extents->y_bearing;
      int64_t y2 = (int64_t) extents->y_bearing + extents->height;

      int64_t x1 = (int64_t) extents->x_bearing + (int64_t) floorf (hb_min (y1 * slant_xy, y2 * slant_xy));
      int64_t x2 = (int64_t) extents->x_bearing + extents->width + (int64_t) ceilf (hb_max (y1 * slant_xy, y2 * slant_xy));

      extents->x_bearing = hb_clamp_to<hb_position_t> (x1);
      extents->width = hb_clamp_to<hb_position_t> (x2 - x1);
    }

    /* Embolden. */
    اگر (x_strength || y_strength)
    {
      /* Y */
      صحیح y_shift = y_strength;
      اگر (y_scale < 0) y_shift = -y_shift;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->y_bearing + y_shift);
      extents->height = hb_clamp_to<hb_position_t> ((int64_t) extents->height - y_shift);

      /* X */
      صحیح x_shift = x_strength;
      اگر (x_scale < 0) x_shift = -x_shift;
      اگر (embolden_in_place)
	extents->x_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->x_bearing - x_shift / 2);
      extents->width = hb_clamp_to<hb_position_t> ((int64_t) extents->width + x_shift);
    }
  }


  /* Public getters */

  HB_INTERNAL منطقی has_func (unsigned صحیح i);
  HB_INTERNAL منطقی has_func_set (unsigned صحیح i);

  /* has_* ... */
#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
  منطقی \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = این->klass; \
    unsigned صحیح i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / اندازه (funcs->get.array[0]); \
    بازگشت has_func (i); \
  } \
  منطقی \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = این->klass; \
    unsigned صحیح i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / اندازه (funcs->get.array[0]); \
    بازگشت has_func_set (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				منطقی synthetic = درست)
  {
    hb_memset (extents, 0, اندازه (*extents));
    منطقی ret = klass->get.f.font_h_extents (این, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_h_extents);

    اگر (synthetic && ret)
    {
      /* Embolden */
      صحیح y_shift = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_shift;
    }

    بازگشت ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				منطقی synthetic = درست)
  {
    hb_memset (extents, 0, اندازه (*extents));
    منطقی ret = klass->get.f.font_v_extents (این, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_v_extents);

    اگر (synthetic && ret)
    {
      /* Embolden */
      صحیح x_shift = x_scale < 0 ? -x_strength : x_strength;
      اگر (embolden_in_place)
      {
	extents->ascender += x_shift / 2;
	extents->descender -= x_shift - x_shift / 2;
      }
      وگرنه
	extents->ascender += x_shift;
    }

    بازگشت ret;
  }

  منطقی has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    بازگشت get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    بازگشت klass->get.f.nominal_glyph (این, user_data,
				       unicode, glyph,
				       !klass->user_data ? nullptr : klass->user_data->nominal_glyph);
  }
  unsigned صحیح get_nominal_glyphs (unsigned صحیح شمار,
				   ثابت hb_codepoint_t *first_unicode,
				   unsigned صحیح unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned صحیح glyph_stride)
  {
    بازگشت klass->get.f.nominal_glyphs (این, user_data,
					شمار,
					first_unicode, unicode_stride,
					first_glyph, glyph_stride,
					!klass->user_data ? nullptr : klass->user_data->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    بازگشت klass->get.f.variation_glyph (این, user_data,
					 unicode, variation_selector, glyph,
					 !klass->user_data ? nullptr : klass->user_data->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     منطقی synthetic = درست)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (این, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_h_advance);

    اگر (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    بازگشت advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     منطقی synthetic = درست)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (این, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_v_advance);

    اگر (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    بازگشت advance;
  }

  تهی get_glyph_h_advances (unsigned صحیح شمار,
			     ثابت hb_codepoint_t *first_glyph,
			     unsigned صحیح glyph_stride,
			     hb_position_t *first_advance,
			     unsigned صحیح advance_stride,
			     منطقی synthetic = درست)
  {
    klass->get.f.glyph_h_advances (این, user_data,
				   شمار,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_h_advances);

    اگر (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      برای (unsigned صحیح i = 0; i < شمار; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  تهی get_glyph_v_advances (unsigned صحیح شمار,
			     ثابت hb_codepoint_t *first_glyph,
			     unsigned صحیح glyph_stride,
			     hb_position_t *first_advance,
			     unsigned صحیح advance_stride,
			     منطقی synthetic = درست)
  {
    klass->get.f.glyph_v_advances (این, user_data,
				   شمار,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_v_advances);

    اگر (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      برای (unsigned صحیح i = 0; i < شمار; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				منطقی synthetic = درست)
  {
    *x = *y = 0;
    منطقی ret = klass->get.f.glyph_h_origin (این, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_h_origin);

    اگر (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      اگر (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    بازگشت ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				منطقی synthetic = درست)
  {
    *x = *y = 0;
    منطقی ret = klass->get.f.glyph_v_origin (این, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_v_origin);

    اگر (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      اگر (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    بازگشت ret;
  }

  hb_bool_t get_glyph_h_origins (unsigned صحیح شمار,
				 ثابت hb_codepoint_t *first_glyph,
				 unsigned صحیح glyph_stride,
				 hb_position_t *first_x,
				 unsigned صحیح x_stride,
				 hb_position_t *first_y,
				 unsigned صحیح y_stride,
				 منطقی synthetic = درست)

  {
    منطقی ret = klass->get.f.glyph_h_origins (این, user_data,
					     شمار,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_h_origins);

    اگر (synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      برای (unsigned i = 0; i < شمار; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	اگر (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    بازگشت ret;
  }

  hb_bool_t get_glyph_v_origins (unsigned صحیح شمار,
				 ثابت hb_codepoint_t *first_glyph,
				 unsigned صحیح glyph_stride,
				 hb_position_t *first_x,
				 unsigned صحیح x_stride,
				 hb_position_t *first_y,
				 unsigned صحیح y_stride,
				 منطقی synthetic = درست)

  {
    منطقی ret = klass->get.f.glyph_v_origins (این, user_data,
					     شمار,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_v_origins);

    اگر (synthetic && is_synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      برای (unsigned i = 0; i < شمار; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	اگر (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    بازگشت ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t چپ_glyph,
				     hb_codepoint_t راست_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    بازگشت 0;
#وگرنه
    بازگشت klass->get.f.glyph_h_kerning (این, user_data,
					 چپ_glyph, راست_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_h_kerning);
#endif
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t بالاترین_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    بازگشت 0;
#وگرنه
    بازگشت klass->get.f.glyph_v_kerning (این, user_data,
					 بالاترین_glyph, bottom_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_v_kerning);
#endif
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       منطقی synthetic = درست)
  {
    hb_memset (extents, 0, اندازه (*extents));

    /* This is rather messy, but necessary. */

    اگر (!synthetic)
    {
      بازگشت klass->get.f.glyph_extents (این, user_data,
					 glyph,
					 extents,
					 !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    }
    اگر (!is_synthetic &&
	klass->get.f.glyph_extents (این, user_data,
				    glyph,
				    extents,
				    !klass->user_data ? nullptr : klass->user_data->glyph_extents))
      بازگشت درست;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#ifndef HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    اگر (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().to_glyph_extents ();
      بازگشت درست;
    }
#endif

#ifndef HB_NO_DRAW
    hb_extents_t<> draw_extents;
    اگر (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.to_glyph_extents ();
      بازگشت درست;
    }
#endif

    منطقی ret = klass->get.f.glyph_extents (این, user_data,
					   glyph,
					   extents,
					   !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    اگر (ret)
      synthetic_glyph_extents (extents);

    بازگشت ret;
  }

  hb_bool_t get_glyph_contour_point (hb_codepoint_t glyph, unsigned صحیح point_index,
				     hb_position_t *x, hb_position_t *y,
				     منطقی synthetic = درست)
  {
    *x = *y = 0;
    منطقی ret = klass->get.f.glyph_contour_point (این, user_data,
						 glyph, point_index,
						 x, y,
						 !klass->user_data ? nullptr : klass->user_data->glyph_contour_point);

    اگر (synthetic && ret)
    {
      /* Slant */
      اگر (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      اگر (!embolden_in_place)
      {
	صحیح x_shift = x_scale < 0 ? -x_strength : x_strength;
	*x += x_shift;
      }
    }

    بازگشت ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    نویسه *name, unsigned صحیح size)
  {
    اگر (size) *name = '\0';
    بازگشت klass->get.f.glyph_name (این, user_data,
				    glyph,
				    name, size,
				    !klass->user_data ? nullptr : klass->user_data->glyph_name);
  }

  hb_bool_t get_glyph_from_name (ثابت نویسه *name, صحیح len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    اگر (len == -1) len = strlen (name);
    بازگشت klass->get.f.glyph_from_name (این, user_data,
					 name, len,
					 glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_from_name);
  }

  منطقی draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, تهی *draw_data,
			   منطقی synthetic = درست)
  {
#ifndef HB_NO_DRAW
#ifndef HB_NO_OUTLINE
    منطقی embolden = x_strength || y_strength;
    منطقی slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#وگرنه
    synthetic = نادرست;
#endif

    اگر (!synthetic)
    {
      بازگشت klass->get.f.draw_glyph_or_fail (این, user_data,
					      glyph,
					      draw_funcs, draw_data,
					      !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail);
    }

#ifndef HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_remaining = draw_funcs->get_budget_remaining_ptr (draw_data);
    اگر (!klass->get.f.draw_glyph_or_fail (این, user_data,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail))
      بازگشت نادرست;

    // Slant before embolden; produces nicer results.

    اگر (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, نادرست);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    اگر (embolden)
    {
      اعشاری x_shift = embolden_in_place ? 0 : (اعشاری) x_strength / 2;
      اعشاری y_shift = (اعشاری) y_strength / 2;
      اگر (x_scale < 0) x_shift = -x_shift;
      اگر (y_scale < 0) y_shift = -y_shift;
      outline.embolden (x_strength, y_strength, x_shift, y_shift);
    }

    outline.replay (draw_funcs, draw_data);

    بازگشت درست;
#endif
#endif
    بازگشت نادرست;
  }

  منطقی paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, تهی *paint_data,
			    unsigned صحیح palette,
			    hb_color_t foreground,
			    منطقی synthetic = درست)
  {
#ifndef HB_NO_PAINT
    /* Slant */
    اگر (synthetic && slant_xy)
      hb_paint_افزودن_transform (paint_funcs, paint_data,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    منطقی ret = klass->get.f.paint_glyph_or_fail (این, user_data,
						 glyph,
						 paint_funcs, paint_data,
						 palette, foreground,
						 !klass->user_data ? nullptr : klass->user_data->paint_glyph_or_fail);

    اگر (synthetic && slant_xy)
      hb_paint_برداشت_transform (paint_funcs, paint_data);

    بازگشت ret;
#endif
    بازگشت نادرست;
  }

  /* A bit higher-level, and with fallback */

  HB_INTERNAL
  تهی paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, تهی *paint_data,
		    unsigned صحیح palette,
		    hb_color_t foreground);

  تهی get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    اگر (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->سطر_gap = 0;
    }
  }
  تهی get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    اگر (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->سطر_gap = 0;
    }
  }

  تهی get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    وگرنه
      get_v_extents_with_fallback (extents);
  }

  تهی get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    وگرنه
      *y = get_glyph_v_advance (glyph);
  }
  تهی get_glyph_advances_for_direction (hb_direction_t direction,
					 unsigned صحیح شمار,
					 ثابت hb_codepoint_t *first_glyph,
					 unsigned glyph_stride,
					 hb_position_t *first_advance,
					 unsigned advance_stride)
  {
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (شمار, first_glyph, glyph_stride, first_advance, advance_stride);
    وگرنه
      get_glyph_v_advances (شمار, first_glyph, glyph_stride, first_advance, advance_stride);
  }

  تهی apply_offset (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     signed mult)
  {
    أكد (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  تهی add_offset (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  تهی subtract_offset (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  تهی guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  تهی apply_glyph_h_origins_with_fallback (hb_buffer_t *buf, صحیح mult)
  {
    منطقی has_ascender = نادرست;
    hb_position_t ascender = 0;

    ساختار { hb_position_t x, y; } origins[32];

    unsigned صحیح offset = 0;
    unsigned صحیح شمار = buf->len;
    وقتی (offset < شمار)
    {
      unsigned n = hb_min (شمار - offset, ARRAY_LENGTH (origins));
      اگر (!get_glyph_h_origins (n,
				&buf->info[offset].codepoint, اندازه (hb_glyph_info_t),
				&origins[0].x, اندازه (origins[0]),
				&origins[0].y, اندازه (origins[0])))
      {
        اگر (get_glyph_v_origins (n,
				  &buf->info[offset].codepoint, اندازه (hb_glyph_info_t),
				  &origins[0].x, اندازه (origins[0]),
				  &origins[0].y, اندازه (origins[0])))
	{
	  اگر (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = درست;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  برای (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	وگرنه
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      أكد (mult == -1 || mult == +1 || mult == 0);
      اگر (mult == +1)
        برای (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      وگرنه اگر (mult == -1)
	برای (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }
  تهی apply_glyph_v_origins_with_fallback (hb_buffer_t *buf, صحیح mult)
  {
    منطقی has_ascender = نادرست;
    hb_position_t ascender = 0;

    ساختار { hb_position_t x, y; } origins[32];

    unsigned صحیح offset = 0;
    unsigned صحیح شمار = buf->len;
    وقتی (offset < شمار)
    {
      unsigned n = hb_min (شمار - offset, ARRAY_LENGTH (origins));
      اگر (!get_glyph_v_origins (n,
				&buf->info[offset].codepoint, اندازه (hb_glyph_info_t),
				&origins[0].x, اندازه (origins[0]),
				&origins[0].y, اندازه (origins[0])))
      {
	اگر (get_glyph_h_origins (n,
				 &buf->info[offset].codepoint, اندازه (hb_glyph_info_t),
				 &origins[0].x, اندازه (origins[0]),
				 &origins[0].y, اندازه (origins[0])))
	{
	  اگر (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = درست;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  برای (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	وگرنه
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      أكد (mult == -1 || mult == +1 || mult == 0);
      اگر (mult == +1)
        برای (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      وگرنه اگر (mult == -1)
	برای (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }

  تهی get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    اگر (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      subtract_offset (x, y, dx, dy);
    }
  }
  تهی get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    اگر (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      add_offset (x, y, dx, dy);
    }
  }

  تهی get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    وگرنه
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  تهی add_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  تهی add_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  تهی add_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    add_offset (x, y, origin_x, origin_y);
  }

  تهی subtract_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  تهی subtract_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  تهی subtract_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    subtract_offset (x, y, origin_x, origin_y);
  }

  تهی get_glyph_kerning_for_direction (hb_codepoint_t first_glyph, hb_codepoint_t second_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (first_glyph, second_glyph);
    } وگرنه {
      *x = 0;
      *y = get_glyph_v_kerning (first_glyph, second_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    اگر (ret)
      subtract_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    بازگشت ret;
  }

  hb_bool_t get_glyph_contour_point_for_origin (hb_codepoint_t glyph, unsigned صحیح point_index,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_point (glyph, point_index, x, y);

    اگر (ret)
      subtract_glyph_origin_for_direction (glyph, direction, x, y);

    بازگشت ret;
  }

  /* Generates gidDDD if glyph has no name. */
  تهی
  glyph_to_string (hb_codepoint_t glyph,
		   نویسه *s, unsigned صحیح size)
  {
    اگر (get_glyph_name (glyph, s, size)) بازگشت;

    اگر (size && snprintf (s, size, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (ثابت نویسه *s, صحیح len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    اگر (get_glyph_from_name (s, len, glyph)) بازگشت درست;

    اگر (len == -1) len = strlen (s);

    /* Straight glyph index. */
    اگر (hb_codepoint_parse (s, len, 10, glyph))
      بازگشت درست;

    اگر (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      اگر (0 == strncmp (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	بازگشت درست;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      اگر (0 == strncmp (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	بازگشت درست;
    }

    بازگشت نادرست;
  }

  تهی changed ()
  {
    اعشاری upem = face->get_upem ();

    x_multf = x_scale / upem;
    y_multf = y_scale / upem;
    x_mult = (int64_t) x_scale * 0x10000 / upem;
    y_mult = (int64_t) y_scale * 0x10000 / upem;

    is_synthetic =  x_embolden || y_embolden || slant;

    x_strength = roundf (abs (x_scale) * x_embolden);
    y_strength = roundf (abs (y_scale) * y_embolden);

    slant_xy = y_scale ? slant * x_scale / y_scale : 0.f;

    data.fini ();

    serial++;
  }

  hb_position_t em_mult (int32_t v, int64_t mult)
  { بازگشت (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (اعشاری v, اعشاری mult)
  { بازگشت (hb_position_t) roundf (em_fmultf (v, mult)); }
  اعشاری em_fmultf (اعشاری v, اعشاری mult)
  { بازگشت v * mult; }
  اعشاری em_fmult (int32_t v, اعشاری mult)
  { بازگشت (اعشاری) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#endif /* HB_FONT_HH */
