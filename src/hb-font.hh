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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-face.hh"
#அந்தர்பாவ "hb-atomic.hh"
#அந்தர்பாவ "hb-draw.hh"
#அந்தர்பாவ "hb-paint-extents.hh"
#அந்தர்பாவ "hb-shaper.hh"
#அந்தர்பாவ "hb-outline.hh"


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

கட்டமைப்பு hb_font_funcs_t
{
  hb_object_header_t header;

  கட்டமைப்பு {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) வெறுமை *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *user_data;

  கட்டமைப்பு {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  ஒன்றிப்பு get_t {
    கட்டமைப்பு get_funcs_t {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
    } f;
    வெறுமை (*array[0
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
#அந்தர்பாவ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

கட்டமைப்பு hb_font_t
{
  hb_object_header_t header;
  hb_atomic_t<unsigned> serial;
  hb_atomic_t<unsigned> serial_coords;

  hb_font_t *parent;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  தருக்க is_synthetic;

  தசம x_embolden;
  தசம y_embolden;
  தருக்க embolden_in_place;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  தசம slant;
  தசம slant_xy;

  தசம x_multf;
  தசம y_multf;
  int64_t x_mult;
  int64_t y_mult;

  unsigned முழு x_ppem;
  unsigned முழு y_ppem;

  தசம ptem;

  /* Font variation coordinates. */
  unsigned முழு instance_index;
  தருக்க has_nonzero_coords;
  unsigned முழு இலக்கம்_coords;
  முழு *coords;
  தசம *design_coords;

  hb_font_funcs_t   *klass;
  வெறுமை              *user_data;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> data; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { திரும்பு HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { திரும்பு em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { திரும்பு em_mult (v, y_mult); }
  hb_position_t em_scalef_x (தசம v) { திரும்பு em_multf (v, x_multf); }
  hb_position_t em_scalef_y (தசம v) { திரும்பு em_multf (v, y_multf); }
  தசம em_fscale_x (int32_t v) { திரும்பு em_fmult (v, x_multf); }
  தசம em_fscale_y (int32_t v) { திரும்பு em_fmult (v, y_multf); }
  தசம em_fscalef_x (தசம v) { திரும்பு em_fmultf (v, x_multf); }
  தசம em_fscalef_y (தசம v) { திரும்பு em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { திரும்பு em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t parent_scale_x_distance (hb_position_t v)
  {
    எனில் (unlikely (parent && parent->x_scale && parent->x_scale != x_scale))
      திரும்பு (hb_position_t) (v * (int64_t) இது->x_scale / இது->parent->x_scale);
    திரும்பு v;
  }
  hb_position_t parent_scale_y_distance (hb_position_t v)
  {
    எனில் (unlikely (parent && parent->y_scale && parent->y_scale != y_scale))
      திரும்பு (hb_position_t) (v * (int64_t) இது->y_scale / இது->parent->y_scale);
    திரும்பு v;
  }
  hb_position_t parent_scale_x_position (hb_position_t v)
  { திரும்பு parent_scale_x_distance (v); }
  hb_position_t parent_scale_y_position (hb_position_t v)
  { திரும்பு parent_scale_y_distance (v); }

  வெறுமை parent_scale_distance (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_distance (*x);
    *y = parent_scale_y_distance (*y);
  }
  வெறுமை parent_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_position (*x);
    *y = parent_scale_y_position (*y);
  }

  வெறுமை scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    தசம x1 = em_fscale_x (extents->x_bearing);
    தசம y1 = em_fscale_y (extents->y_bearing);
    தசம x2 = x1 + em_fscale_x (extents->width);
    தசம y2 = y1 + em_fscale_y (extents->height);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->width = ceilf (x2) - extents->x_bearing;
    extents->height = ceilf (y2) - extents->y_bearing;
  }

  வெறுமை synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    எனில் (slant_xy)
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
    எனில் (x_strength || y_strength)
    {
      /* Y */
      முழு y_shift = y_strength;
      எனில் (y_scale < 0) y_shift = -y_shift;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->y_bearing + y_shift);
      extents->height = hb_clamp_to<hb_position_t> ((int64_t) extents->height - y_shift);

      /* X */
      முழு x_shift = x_strength;
      எனில் (x_scale < 0) x_shift = -x_shift;
      எனில் (embolden_in_place)
	extents->x_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->x_bearing - x_shift / 2);
      extents->width = hb_clamp_to<hb_position_t> ((int64_t) extents->width + x_shift);
    }
  }


  /* Public getters */

  HB_INTERNAL தருக்க has_func (unsigned முழு i);
  HB_INTERNAL தருக்க has_func_set (unsigned முழு i);

  /* has_* ... */
#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
  தருக்க \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = இது->klass; \
    unsigned முழு i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / அளவு (funcs->get.array[0]); \
    திரும்பு has_func (i); \
  } \
  தருக்க \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = இது->klass; \
    unsigned முழு i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / அளவு (funcs->get.array[0]); \
    திரும்பு has_func_set (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				தருக்க synthetic = உண்மை)
  {
    hb_memset (extents, 0, அளவு (*extents));
    தருக்க ret = klass->get.f.font_h_extents (இது, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_h_extents);

    எனில் (synthetic && ret)
    {
      /* Embolden */
      முழு y_shift = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_shift;
    }

    திரும்பு ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				தருக்க synthetic = உண்மை)
  {
    hb_memset (extents, 0, அளவு (*extents));
    தருக்க ret = klass->get.f.font_v_extents (இது, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_v_extents);

    எனில் (synthetic && ret)
    {
      /* Embolden */
      முழு x_shift = x_scale < 0 ? -x_strength : x_strength;
      எனில் (embolden_in_place)
      {
	extents->ascender += x_shift / 2;
	extents->descender -= x_shift - x_shift / 2;
      }
      இல்லையெனில்
	extents->ascender += x_shift;
    }

    திரும்பு ret;
  }

  தருக்க has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    திரும்பு get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    திரும்பு klass->get.f.nominal_glyph (இது, user_data,
				       unicode, glyph,
				       !klass->user_data ? nullptr : klass->user_data->nominal_glyph);
  }
  unsigned முழு get_nominal_glyphs (unsigned முழு மொத்தம்,
				   மாறிலி hb_codepoint_t *first_unicode,
				   unsigned முழு unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned முழு glyph_stride)
  {
    திரும்பு klass->get.f.nominal_glyphs (இது, user_data,
					மொத்தம்,
					first_unicode, unicode_stride,
					first_glyph, glyph_stride,
					!klass->user_data ? nullptr : klass->user_data->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    திரும்பு klass->get.f.variation_glyph (இது, user_data,
					 unicode, variation_selector, glyph,
					 !klass->user_data ? nullptr : klass->user_data->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     தருக்க synthetic = உண்மை)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (இது, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_h_advance);

    எனில் (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    திரும்பு advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     தருக்க synthetic = உண்மை)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (இது, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_v_advance);

    எனில் (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    திரும்பு advance;
  }

  வெறுமை get_glyph_h_advances (unsigned முழு மொத்தம்,
			     மாறிலி hb_codepoint_t *first_glyph,
			     unsigned முழு glyph_stride,
			     hb_position_t *first_advance,
			     unsigned முழு advance_stride,
			     தருக்க synthetic = உண்மை)
  {
    klass->get.f.glyph_h_advances (இது, user_data,
				   மொத்தம்,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_h_advances);

    எனில் (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  வெறுமை get_glyph_v_advances (unsigned முழு மொத்தம்,
			     மாறிலி hb_codepoint_t *first_glyph,
			     unsigned முழு glyph_stride,
			     hb_position_t *first_advance,
			     unsigned முழு advance_stride,
			     தருக்க synthetic = உண்மை)
  {
    klass->get.f.glyph_v_advances (இது, user_data,
				   மொத்தம்,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_v_advances);

    எனில் (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				தருக்க synthetic = உண்மை)
  {
    *x = *y = 0;
    தருக்க ret = klass->get.f.glyph_h_origin (இது, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_h_origin);

    எனில் (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      எனில் (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    திரும்பு ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				தருக்க synthetic = உண்மை)
  {
    *x = *y = 0;
    தருக்க ret = klass->get.f.glyph_v_origin (இது, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_v_origin);

    எனில் (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      எனில் (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    திரும்பு ret;
  }

  hb_bool_t get_glyph_h_origins (unsigned முழு மொத்தம்,
				 மாறிலி hb_codepoint_t *first_glyph,
				 unsigned முழு glyph_stride,
				 hb_position_t *first_x,
				 unsigned முழு x_stride,
				 hb_position_t *first_y,
				 unsigned முழு y_stride,
				 தருக்க synthetic = உண்மை)

  {
    தருக்க ret = klass->get.f.glyph_h_origins (இது, user_data,
					     மொத்தம்,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_h_origins);

    எனில் (synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	எனில் (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    திரும்பு ret;
  }

  hb_bool_t get_glyph_v_origins (unsigned முழு மொத்தம்,
				 மாறிலி hb_codepoint_t *first_glyph,
				 unsigned முழு glyph_stride,
				 hb_position_t *first_x,
				 unsigned முழு x_stride,
				 hb_position_t *first_y,
				 unsigned முழு y_stride,
				 தருக்க synthetic = உண்மை)

  {
    தருக்க ret = klass->get.f.glyph_v_origins (இது, user_data,
					     மொத்தம்,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_v_origins);

    எனில் (synthetic && is_synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	எனில் (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    திரும்பு ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t இடது_glyph,
				     hb_codepoint_t வலது_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    திரும்பு 0;
#இல்லையெனில்
    திரும்பு klass->get.f.glyph_h_kerning (இது, user_data,
					 இடது_glyph, வலது_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_h_kerning);
#endif
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t உச்சி_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    திரும்பு 0;
#இல்லையெனில்
    திரும்பு klass->get.f.glyph_v_kerning (இது, user_data,
					 உச்சி_glyph, bottom_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_v_kerning);
#endif
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       தருக்க synthetic = உண்மை)
  {
    hb_memset (extents, 0, அளவு (*extents));

    /* This is rather messy, but necessary. */

    எனில் (!synthetic)
    {
      திரும்பு klass->get.f.glyph_extents (இது, user_data,
					 glyph,
					 extents,
					 !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    }
    எனில் (!is_synthetic &&
	klass->get.f.glyph_extents (இது, user_data,
				    glyph,
				    extents,
				    !klass->user_data ? nullptr : klass->user_data->glyph_extents))
      திரும்பு உண்மை;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#ifndef HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    எனில் (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().to_glyph_extents ();
      திரும்பு உண்மை;
    }
#endif

#ifndef HB_NO_DRAW
    hb_extents_t<> draw_extents;
    எனில் (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.to_glyph_extents ();
      திரும்பு உண்மை;
    }
#endif

    தருக்க ret = klass->get.f.glyph_extents (இது, user_data,
					   glyph,
					   extents,
					   !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    எனில் (ret)
      synthetic_glyph_extents (extents);

    திரும்பு ret;
  }

  hb_bool_t get_glyph_contour_point (hb_codepoint_t glyph, unsigned முழு point_index,
				     hb_position_t *x, hb_position_t *y,
				     தருக்க synthetic = உண்மை)
  {
    *x = *y = 0;
    தருக்க ret = klass->get.f.glyph_contour_point (இது, user_data,
						 glyph, point_index,
						 x, y,
						 !klass->user_data ? nullptr : klass->user_data->glyph_contour_point);

    எனில் (synthetic && ret)
    {
      /* Slant */
      எனில் (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      எனில் (!embolden_in_place)
      {
	முழு x_shift = x_scale < 0 ? -x_strength : x_strength;
	*x += x_shift;
      }
    }

    திரும்பு ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    எழுத்து *name, unsigned முழு size)
  {
    எனில் (size) *name = '\0';
    திரும்பு klass->get.f.glyph_name (இது, user_data,
				    glyph,
				    name, size,
				    !klass->user_data ? nullptr : klass->user_data->glyph_name);
  }

  hb_bool_t get_glyph_from_name (மாறிலி எழுத்து *name, முழு len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    எனில் (len == -1) len = strlen (name);
    திரும்பு klass->get.f.glyph_from_name (இது, user_data,
					 name, len,
					 glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_from_name);
  }

  தருக்க draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, வெறுமை *draw_data,
			   தருக்க synthetic = உண்மை)
  {
#ifndef HB_NO_DRAW
#ifndef HB_NO_OUTLINE
    தருக்க embolden = x_strength || y_strength;
    தருக்க slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#இல்லையெனில்
    synthetic = பொய்;
#endif

    எனில் (!synthetic)
    {
      திரும்பு klass->get.f.draw_glyph_or_fail (இது, user_data,
					      glyph,
					      draw_funcs, draw_data,
					      !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail);
    }

#ifndef HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_remaining = draw_funcs->get_budget_remaining_ptr (draw_data);
    எனில் (!klass->get.f.draw_glyph_or_fail (இது, user_data,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail))
      திரும்பு பொய்;

    // Slant before embolden; produces nicer results.

    எனில் (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, பொய்);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    எனில் (embolden)
    {
      தசம x_shift = embolden_in_place ? 0 : (தசம) x_strength / 2;
      தசம y_shift = (தசம) y_strength / 2;
      எனில் (x_scale < 0) x_shift = -x_shift;
      எனில் (y_scale < 0) y_shift = -y_shift;
      outline.embolden (x_strength, y_strength, x_shift, y_shift);
    }

    outline.replay (draw_funcs, draw_data);

    திரும்பு உண்மை;
#endif
#endif
    திரும்பு பொய்;
  }

  தருக்க paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, வெறுமை *paint_data,
			    unsigned முழு palette,
			    hb_color_t foreground,
			    தருக்க synthetic = உண்மை)
  {
#ifndef HB_NO_PAINT
    /* Slant */
    எனில் (synthetic && slant_xy)
      hb_paint_தள்ளு_transform (paint_funcs, paint_data,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    தருக்க ret = klass->get.f.paint_glyph_or_fail (இது, user_data,
						 glyph,
						 paint_funcs, paint_data,
						 palette, foreground,
						 !klass->user_data ? nullptr : klass->user_data->paint_glyph_or_fail);

    எனில் (synthetic && slant_xy)
      hb_paint_எடு_transform (paint_funcs, paint_data);

    திரும்பு ret;
#endif
    திரும்பு பொய்;
  }

  /* A bit higher-level, and with fallback */

  HB_INTERNAL
  வெறுமை paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, வெறுமை *paint_data,
		    unsigned முழு palette,
		    hb_color_t foreground);

  வெறுமை get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    எனில் (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->வரி_gap = 0;
    }
  }
  வெறுமை get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    எனில் (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->வரி_gap = 0;
    }
  }

  வெறுமை get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    எனில் (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    இல்லையெனில்
      get_v_extents_with_fallback (extents);
  }

  வெறுமை get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    எனில் (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    இல்லையெனில்
      *y = get_glyph_v_advance (glyph);
  }
  வெறுமை get_glyph_advances_for_direction (hb_direction_t direction,
					 unsigned முழு மொத்தம்,
					 மாறிலி hb_codepoint_t *first_glyph,
					 unsigned glyph_stride,
					 hb_position_t *first_advance,
					 unsigned advance_stride)
  {
    எனில் (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (மொத்தம், first_glyph, glyph_stride, first_advance, advance_stride);
    இல்லையெனில்
      get_glyph_v_advances (மொத்தம், first_glyph, glyph_stride, first_advance, advance_stride);
  }

  வெறுமை apply_offset (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     signed mult)
  {
    ப்ரதிஜ்ஞா (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  வெறுமை add_offset (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  வெறுமை subtract_offset (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  வெறுமை guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  வெறுமை apply_glyph_h_origins_with_fallback (hb_buffer_t *buf, முழு mult)
  {
    தருக்க has_ascender = பொய்;
    hb_position_t ascender = 0;

    கட்டமைப்பு { hb_position_t x, y; } origins[32];

    unsigned முழு offset = 0;
    unsigned முழு மொத்தம் = buf->len;
    போது (offset < மொத்தம்)
    {
      unsigned n = hb_min (மொத்தம் - offset, ARRAY_LENGTH (origins));
      எனில் (!get_glyph_h_origins (n,
				&buf->info[offset].codepoint, அளவு (hb_glyph_info_t),
				&origins[0].x, அளவு (origins[0]),
				&origins[0].y, அளவு (origins[0])))
      {
        எனில் (get_glyph_v_origins (n,
				  &buf->info[offset].codepoint, அளவு (hb_glyph_info_t),
				  &origins[0].x, அளவு (origins[0]),
				  &origins[0].y, அளவு (origins[0])))
	{
	  எனில் (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = உண்மை;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  க்கு (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	இல்லையெனில்
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      ப்ரதிஜ்ஞா (mult == -1 || mult == +1 || mult == 0);
      எனில் (mult == +1)
        க்கு (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      இல்லையெனில் எனில் (mult == -1)
	க்கு (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }
  வெறுமை apply_glyph_v_origins_with_fallback (hb_buffer_t *buf, முழு mult)
  {
    தருக்க has_ascender = பொய்;
    hb_position_t ascender = 0;

    கட்டமைப்பு { hb_position_t x, y; } origins[32];

    unsigned முழு offset = 0;
    unsigned முழு மொத்தம் = buf->len;
    போது (offset < மொத்தம்)
    {
      unsigned n = hb_min (மொத்தம் - offset, ARRAY_LENGTH (origins));
      எனில் (!get_glyph_v_origins (n,
				&buf->info[offset].codepoint, அளவு (hb_glyph_info_t),
				&origins[0].x, அளவு (origins[0]),
				&origins[0].y, அளவு (origins[0])))
      {
	எனில் (get_glyph_h_origins (n,
				 &buf->info[offset].codepoint, அளவு (hb_glyph_info_t),
				 &origins[0].x, அளவு (origins[0]),
				 &origins[0].y, அளவு (origins[0])))
	{
	  எனில் (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = உண்மை;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  க்கு (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	இல்லையெனில்
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      ப்ரதிஜ்ஞா (mult == -1 || mult == +1 || mult == 0);
      எனில் (mult == +1)
        க்கு (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      இல்லையெனில் எனில் (mult == -1)
	க்கு (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }

  வெறுமை get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    எனில் (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      subtract_offset (x, y, dx, dy);
    }
  }
  வெறுமை get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    எனில் (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      add_offset (x, y, dx, dy);
    }
  }

  வெறுமை get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    எனில் (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    இல்லையெனில்
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  வெறுமை add_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  வெறுமை add_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  வெறுமை add_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    add_offset (x, y, origin_x, origin_y);
  }

  வெறுமை subtract_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  வெறுமை subtract_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  வெறுமை subtract_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    subtract_offset (x, y, origin_x, origin_y);
  }

  வெறுமை get_glyph_kerning_for_direction (hb_codepoint_t first_glyph, hb_codepoint_t second_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    எனில் (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (first_glyph, second_glyph);
    } இல்லையெனில் {
      *x = 0;
      *y = get_glyph_v_kerning (first_glyph, second_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    எனில் (ret)
      subtract_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    திரும்பு ret;
  }

  hb_bool_t get_glyph_contour_point_for_origin (hb_codepoint_t glyph, unsigned முழு point_index,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_point (glyph, point_index, x, y);

    எனில் (ret)
      subtract_glyph_origin_for_direction (glyph, direction, x, y);

    திரும்பு ret;
  }

  /* Generates gidDDD if glyph has no name. */
  வெறுமை
  glyph_to_string (hb_codepoint_t glyph,
		   எழுத்து *s, unsigned முழு size)
  {
    எனில் (get_glyph_name (glyph, s, size)) திரும்பு;

    எனில் (size && snprintf (s, size, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (மாறிலி எழுத்து *s, முழு len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    எனில் (get_glyph_from_name (s, len, glyph)) திரும்பு உண்மை;

    எனில் (len == -1) len = strlen (s);

    /* Straight glyph index. */
    எனில் (hb_codepoint_parse (s, len, 10, glyph))
      திரும்பு உண்மை;

    எனில் (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      எனில் (0 == strncmp (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	திரும்பு உண்மை;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      எனில் (0 == strncmp (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	திரும்பு உண்மை;
    }

    திரும்பு பொய்;
  }

  வெறுமை changed ()
  {
    தசம upem = face->get_upem ();

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
  { திரும்பு (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (தசம v, தசம mult)
  { திரும்பு (hb_position_t) roundf (em_fmultf (v, mult)); }
  தசம em_fmultf (தசம v, தசம mult)
  { திரும்பு v * mult; }
  தசம em_fmult (int32_t v, தசம mult)
  { திரும்பு (தசம) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#endif /* HB_FONT_HH */
