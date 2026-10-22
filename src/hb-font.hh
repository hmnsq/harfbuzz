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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-face.hh"
#ଅନ୍ତର୍ଭାଵ "hb-atomic.hh"
#ଅନ୍ତର୍ଭାଵ "hb-draw.hh"
#ଅନ୍ତର୍ଭାଵ "hb-paint-extents.hh"
#ଅନ୍ତର୍ଭାଵ "hb-shaper.hh"
#ଅନ୍ତର୍ଭାଵ "hb-outline.hh"


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

ସଂରଚନା hb_font_funcs_t
{
  hb_object_header_t header;

  ସଂରଚନା {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) ଶୂନ୍ୟ *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *user_data;

  ସଂରଚନା {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  ସଂଘ get_t {
    ସଂରଚନା get_funcs_t {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
    } f;
    ଶୂନ୍ୟ (*array[0
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
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ସଂରଚନା hb_font_t
{
  hb_object_header_t header;
  hb_atomic_t<unsigned> serial;
  hb_atomic_t<unsigned> serial_coords;

  hb_font_t *parent;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  ତାର୍କିକ is_synthetic;

  ଦଶମିକ x_embolden;
  ଦଶମିକ y_embolden;
  ତାର୍କିକ embolden_in_place;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  ଦଶମିକ slant;
  ଦଶମିକ slant_xy;

  ଦଶମିକ x_multf;
  ଦଶମିକ y_multf;
  int64_t x_mult;
  int64_t y_mult;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_ppem;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା y_ppem;

  ଦଶମିକ ptem;

  /* Font variation coordinates. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା instance_index;
  ତାର୍କିକ has_nonzero_coords;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_coords;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords;
  ଦଶମିକ *design_coords;

  hb_font_funcs_t   *klass;
  ଶୂନ୍ୟ              *user_data;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> data; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { ଫେରନ୍ତୁ HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { ଫେରନ୍ତୁ em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { ଫେରନ୍ତୁ em_mult (v, y_mult); }
  hb_position_t em_scalef_x (ଦଶମିକ v) { ଫେରନ୍ତୁ em_multf (v, x_multf); }
  hb_position_t em_scalef_y (ଦଶମିକ v) { ଫେରନ୍ତୁ em_multf (v, y_multf); }
  ଦଶମିକ em_fscale_x (int32_t v) { ଫେରନ୍ତୁ em_fmult (v, x_multf); }
  ଦଶମିକ em_fscale_y (int32_t v) { ଫେରନ୍ତୁ em_fmult (v, y_multf); }
  ଦଶମିକ em_fscalef_x (ଦଶମିକ v) { ଫେରନ୍ତୁ em_fmultf (v, x_multf); }
  ଦଶମିକ em_fscalef_y (ଦଶମିକ v) { ଫେରନ୍ତୁ em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { ଫେରନ୍ତୁ em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t parent_scale_x_distance (hb_position_t v)
  {
    ଯଦି (unlikely (parent && parent->x_scale && parent->x_scale != x_scale))
      ଫେରନ୍ତୁ (hb_position_t) (v * (int64_t) ଏହି->x_scale / ଏହି->parent->x_scale);
    ଫେରନ୍ତୁ v;
  }
  hb_position_t parent_scale_y_distance (hb_position_t v)
  {
    ଯଦି (unlikely (parent && parent->y_scale && parent->y_scale != y_scale))
      ଫେରନ୍ତୁ (hb_position_t) (v * (int64_t) ଏହି->y_scale / ଏହି->parent->y_scale);
    ଫେରନ୍ତୁ v;
  }
  hb_position_t parent_scale_x_position (hb_position_t v)
  { ଫେରନ୍ତୁ parent_scale_x_distance (v); }
  hb_position_t parent_scale_y_position (hb_position_t v)
  { ଫେରନ୍ତୁ parent_scale_y_distance (v); }

  ଶୂନ୍ୟ parent_scale_distance (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_distance (*x);
    *y = parent_scale_y_distance (*y);
  }
  ଶୂନ୍ୟ parent_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_position (*x);
    *y = parent_scale_y_position (*y);
  }

  ଶୂନ୍ୟ scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    ଦଶମିକ x1 = em_fscale_x (extents->x_bearing);
    ଦଶମିକ y1 = em_fscale_y (extents->y_bearing);
    ଦଶମିକ x2 = x1 + em_fscale_x (extents->width);
    ଦଶମିକ y2 = y1 + em_fscale_y (extents->height);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->width = ceilf (x2) - extents->x_bearing;
    extents->height = ceilf (y2) - extents->y_bearing;
  }

  ଶୂନ୍ୟ synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    ଯଦି (slant_xy)
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
    ଯଦି (x_strength || y_strength)
    {
      /* Y */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା y_shift = y_strength;
      ଯଦି (y_scale < 0) y_shift = -y_shift;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->y_bearing + y_shift);
      extents->height = hb_clamp_to<hb_position_t> ((int64_t) extents->height - y_shift);

      /* X */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_shift = x_strength;
      ଯଦି (x_scale < 0) x_shift = -x_shift;
      ଯଦି (embolden_in_place)
	extents->x_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->x_bearing - x_shift / 2);
      extents->width = hb_clamp_to<hb_position_t> ((int64_t) extents->width + x_shift);
    }
  }


  /* Public getters */

  HB_INTERNAL ତାର୍କିକ has_func (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i);
  HB_INTERNAL ତାର୍କିକ has_func_set (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i);

  /* has_* ... */
#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
  ତାର୍କିକ \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = ଏହି->klass; \
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / ଆକାର (funcs->get.array[0]); \
    ଫେରନ୍ତୁ has_func (i); \
  } \
  ତାର୍କିକ \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = ଏହି->klass; \
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / ଆକାର (funcs->get.array[0]); \
    ଫେରନ୍ତୁ has_func_set (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
    hb_memset (extents, 0, ଆକାର (*extents));
    ତାର୍କିକ ret = klass->get.f.font_h_extents (ଏହି, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_h_extents);

    ଯଦି (synthetic && ret)
    {
      /* Embolden */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା y_shift = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_shift;
    }

    ଫେରନ୍ତୁ ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
    hb_memset (extents, 0, ଆକାର (*extents));
    ତାର୍କିକ ret = klass->get.f.font_v_extents (ଏହି, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_v_extents);

    ଯଦି (synthetic && ret)
    {
      /* Embolden */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_shift = x_scale < 0 ? -x_strength : x_strength;
      ଯଦି (embolden_in_place)
      {
	extents->ascender += x_shift / 2;
	extents->descender -= x_shift - x_shift / 2;
      }
      ଅନ୍ୟଥା
	extents->ascender += x_shift;
    }

    ଫେରନ୍ତୁ ret;
  }

  ତାର୍କିକ has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    ଫେରନ୍ତୁ get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    ଫେରନ୍ତୁ klass->get.f.nominal_glyph (ଏହି, user_data,
				       unicode, glyph,
				       !klass->user_data ? nullptr : klass->user_data->nominal_glyph);
  }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_nominal_glyphs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				   ଅଚଳ hb_codepoint_t *first_unicode,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_stride)
  {
    ଫେରନ୍ତୁ klass->get.f.nominal_glyphs (ଏହି, user_data,
					ଗଣନା_2,
					first_unicode, unicode_stride,
					first_glyph, glyph_stride,
					!klass->user_data ? nullptr : klass->user_data->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    ଫେରନ୍ତୁ klass->get.f.variation_glyph (ଏହି, user_data,
					 unicode, variation_selector, glyph,
					 !klass->user_data ? nullptr : klass->user_data->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (ଏହି, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_h_advance);

    ଯଦି (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    ଫେରନ୍ତୁ advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (ଏହି, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_v_advance);

    ଯଦି (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    ଫେରନ୍ତୁ advance;
  }

  ଶୂନ୍ୟ get_glyph_h_advances (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
			     ଅଚଳ hb_codepoint_t *first_glyph,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_stride,
			     hb_position_t *first_advance,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା advance_stride,
			     ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
    klass->get.f.glyph_h_advances (ଏହି, user_data,
				   ଗଣନା_2,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_h_advances);

    ଯଦି (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  ଶୂନ୍ୟ get_glyph_v_advances (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
			     ଅଚଳ hb_codepoint_t *first_glyph,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_stride,
			     hb_position_t *first_advance,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା advance_stride,
			     ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
    klass->get.f.glyph_v_advances (ଏହି, user_data,
				   ଗଣନା_2,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_v_advances);

    ଯଦି (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
    *x = *y = 0;
    ତାର୍କିକ ret = klass->get.f.glyph_h_origin (ଏହି, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_h_origin);

    ଯଦି (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      ଯଦି (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    ଫେରନ୍ତୁ ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
    *x = *y = 0;
    ତାର୍କିକ ret = klass->get.f.glyph_v_origin (ଏହି, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_v_origin);

    ଯଦି (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      ଯଦି (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    ଫେରନ୍ତୁ ret;
  }

  hb_bool_t get_glyph_h_origins (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				 ଅଚଳ hb_codepoint_t *first_glyph,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_stride,
				 hb_position_t *first_x,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_stride,
				 hb_position_t *first_y,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା y_stride,
				 ତାର୍କିକ synthetic = ସତ୍ୟ)

  {
    ତାର୍କିକ ret = klass->get.f.glyph_h_origins (ଏହି, user_data,
					     ଗଣନା_2,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_h_origins);

    ଯଦି (synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	ଯଦି (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    ଫେରନ୍ତୁ ret;
  }

  hb_bool_t get_glyph_v_origins (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				 ଅଚଳ hb_codepoint_t *first_glyph,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_stride,
				 hb_position_t *first_x,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_stride,
				 hb_position_t *first_y,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା y_stride,
				 ତାର୍କିକ synthetic = ସତ୍ୟ)

  {
    ତାର୍କିକ ret = klass->get.f.glyph_v_origins (ଏହି, user_data,
					     ଗଣନା_2,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_v_origins);

    ଯଦି (synthetic && is_synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	ଯଦି (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    ଫେରନ୍ତୁ ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t ବାମ_glyph,
				     hb_codepoint_t ଡାହାଣ_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ klass->get.f.glyph_h_kerning (ଏହି, user_data,
					 ବାମ_glyph, ଡାହାଣ_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_h_kerning);
#endif
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t ଶୀର୍ଷ_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ klass->get.f.glyph_v_kerning (ଏହି, user_data,
					 ଶୀର୍ଷ_glyph, bottom_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_v_kerning);
#endif
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
    hb_memset (extents, 0, ଆକାର (*extents));

    /* This is rather messy, but necessary. */

    ଯଦି (!synthetic)
    {
      ଫେରନ୍ତୁ klass->get.f.glyph_extents (ଏହି, user_data,
					 glyph,
					 extents,
					 !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    }
    ଯଦି (!is_synthetic &&
	klass->get.f.glyph_extents (ଏହି, user_data,
				    glyph,
				    extents,
				    !klass->user_data ? nullptr : klass->user_data->glyph_extents))
      ଫେରନ୍ତୁ ସତ୍ୟ;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#ifndef HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    ଯଦି (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().to_glyph_extents ();
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
#endif

#ifndef HB_NO_DRAW
    hb_extents_t<> draw_extents;
    ଯଦି (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.to_glyph_extents ();
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
#endif

    ତାର୍କିକ ret = klass->get.f.glyph_extents (ଏହି, user_data,
					   glyph,
					   extents,
					   !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    ଯଦି (ret)
      synthetic_glyph_extents (extents);

    ଫେରନ୍ତୁ ret;
  }

  hb_bool_t get_glyph_contour_point (hb_codepoint_t glyph, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା point_index,
				     hb_position_t *x, hb_position_t *y,
				     ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
    *x = *y = 0;
    ତାର୍କିକ ret = klass->get.f.glyph_contour_point (ଏହି, user_data,
						 glyph, point_index,
						 x, y,
						 !klass->user_data ? nullptr : klass->user_data->glyph_contour_point);

    ଯଦି (synthetic && ret)
    {
      /* Slant */
      ଯଦି (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      ଯଦି (!embolden_in_place)
      {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_shift = x_scale < 0 ? -x_strength : x_strength;
	*x += x_shift;
      }
    }

    ଫେରନ୍ତୁ ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    ଅକ୍ଷର *name, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
  {
    ଯଦି (size) *name = '\0';
    ଫେରନ୍ତୁ klass->get.f.glyph_name (ଏହି, user_data,
				    glyph,
				    name, size,
				    !klass->user_data ? nullptr : klass->user_data->glyph_name);
  }

  hb_bool_t get_glyph_from_name (ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    ଯଦି (len == -1) len = strlen (name);
    ଫେରନ୍ତୁ klass->get.f.glyph_from_name (ଏହି, user_data,
					 name, len,
					 glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_from_name);
  }

  ତାର୍କିକ draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, ଶୂନ୍ୟ *draw_data,
			   ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
#ifndef HB_NO_DRAW
#ifndef HB_NO_OUTLINE
    ତାର୍କିକ embolden = x_strength || y_strength;
    ତାର୍କିକ slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#ଅନ୍ୟଥା
    synthetic = ମିଥ୍ୟା;
#endif

    ଯଦି (!synthetic)
    {
      ଫେରନ୍ତୁ klass->get.f.draw_glyph_or_fail (ଏହି, user_data,
					      glyph,
					      draw_funcs, draw_data,
					      !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail);
    }

#ifndef HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_remaining = draw_funcs->get_budget_remaining_ptr (draw_data);
    ଯଦି (!klass->get.f.draw_glyph_or_fail (ଏହି, user_data,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    // Slant before embolden; produces nicer results.

    ଯଦି (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, ମିଥ୍ୟା);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    ଯଦି (embolden)
    {
      ଦଶମିକ x_shift = embolden_in_place ? 0 : (ଦଶମିକ) x_strength / 2;
      ଦଶମିକ y_shift = (ଦଶମିକ) y_strength / 2;
      ଯଦି (x_scale < 0) x_shift = -x_shift;
      ଯଦି (y_scale < 0) y_shift = -y_shift;
      outline.embolden (x_strength, y_strength, x_shift, y_shift);
    }

    outline.replay (draw_funcs, draw_data);

    ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
#endif
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, ଶୂନ୍ୟ *paint_data,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette,
			    hb_color_t foreground,
			    ତାର୍କିକ synthetic = ସତ୍ୟ)
  {
#ifndef HB_NO_PAINT
    /* Slant */
    ଯଦି (synthetic && slant_xy)
      hb_paint_ଠେଲ_transform (paint_funcs, paint_data,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    ତାର୍କିକ ret = klass->get.f.paint_glyph_or_fail (ଏହି, user_data,
						 glyph,
						 paint_funcs, paint_data,
						 palette, foreground,
						 !klass->user_data ? nullptr : klass->user_data->paint_glyph_or_fail);

    ଯଦି (synthetic && slant_xy)
      hb_paint_କାଢ଼_transform (paint_funcs, paint_data);

    ଫେରନ୍ତୁ ret;
#endif
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  /* A bit higher-level, and with fallback */

  HB_INTERNAL
  ଶୂନ୍ୟ paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, ଶୂନ୍ୟ *paint_data,
		    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette,
		    hb_color_t foreground);

  ଶୂନ୍ୟ get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    ଯଦି (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->ଧାଡ଼ି_gap = 0;
    }
  }
  ଶୂନ୍ୟ get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    ଯଦି (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->ଧାଡ଼ି_gap = 0;
    }
  }

  ଶୂନ୍ୟ get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    ଯଦି (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    ଅନ୍ୟଥା
      get_v_extents_with_fallback (extents);
  }

  ଶୂନ୍ୟ get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    ଯଦି (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    ଅନ୍ୟଥା
      *y = get_glyph_v_advance (glyph);
  }
  ଶୂନ୍ୟ get_glyph_advances_for_direction (hb_direction_t direction,
					 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
					 ଅଚଳ hb_codepoint_t *first_glyph,
					 unsigned glyph_stride,
					 hb_position_t *first_advance,
					 unsigned advance_stride)
  {
    ଯଦି (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (ଗଣନା_2, first_glyph, glyph_stride, first_advance, advance_stride);
    ଅନ୍ୟଥା
      get_glyph_v_advances (ଗଣନା_2, first_glyph, glyph_stride, first_advance, advance_stride);
  }

  ଶୂନ୍ୟ apply_offset (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     signed mult)
  {
    ପ୍ରତିଜ୍ଞା (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  ଶୂନ୍ୟ add_offset (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  ଶୂନ୍ୟ subtract_offset (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  ଶୂନ୍ୟ guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  ଶୂନ୍ୟ apply_glyph_h_origins_with_fallback (hb_buffer_t *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mult)
  {
    ତାର୍କିକ has_ascender = ମିଥ୍ୟା;
    hb_position_t ascender = 0;

    ସଂରଚନା { hb_position_t x, y; } origins[32];

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = buf->len;
    ଯେତେବେଳେ (offset < ଗଣନା_2)
    {
      unsigned n = hb_min (ଗଣନା_2 - offset, ARRAY_LENGTH (origins));
      ଯଦି (!get_glyph_h_origins (n,
				&buf->info[offset].codepoint, ଆକାର (hb_glyph_info_t),
				&origins[0].x, ଆକାର (origins[0]),
				&origins[0].y, ଆକାର (origins[0])))
      {
        ଯଦି (get_glyph_v_origins (n,
				  &buf->info[offset].codepoint, ଆକାର (hb_glyph_info_t),
				  &origins[0].x, ଆକାର (origins[0]),
				  &origins[0].y, ଆକାର (origins[0])))
	{
	  ଯଦି (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = ସତ୍ୟ;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  ପାଇଁ (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	ଅନ୍ୟଥା
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      ପ୍ରତିଜ୍ଞା (mult == -1 || mult == +1 || mult == 0);
      ଯଦି (mult == +1)
        ପାଇଁ (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      ଅନ୍ୟଥା ଯଦି (mult == -1)
	ପାଇଁ (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }
  ଶୂନ୍ୟ apply_glyph_v_origins_with_fallback (hb_buffer_t *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mult)
  {
    ତାର୍କିକ has_ascender = ମିଥ୍ୟା;
    hb_position_t ascender = 0;

    ସଂରଚନା { hb_position_t x, y; } origins[32];

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = buf->len;
    ଯେତେବେଳେ (offset < ଗଣନା_2)
    {
      unsigned n = hb_min (ଗଣନା_2 - offset, ARRAY_LENGTH (origins));
      ଯଦି (!get_glyph_v_origins (n,
				&buf->info[offset].codepoint, ଆକାର (hb_glyph_info_t),
				&origins[0].x, ଆକାର (origins[0]),
				&origins[0].y, ଆକାର (origins[0])))
      {
	ଯଦି (get_glyph_h_origins (n,
				 &buf->info[offset].codepoint, ଆକାର (hb_glyph_info_t),
				 &origins[0].x, ଆକାର (origins[0]),
				 &origins[0].y, ଆକାର (origins[0])))
	{
	  ଯଦି (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = ସତ୍ୟ;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  ପାଇଁ (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	ଅନ୍ୟଥା
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      ପ୍ରତିଜ୍ଞା (mult == -1 || mult == +1 || mult == 0);
      ଯଦି (mult == +1)
        ପାଇଁ (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      ଅନ୍ୟଥା ଯଦି (mult == -1)
	ପାଇଁ (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }

  ଶୂନ୍ୟ get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    ଯଦି (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      subtract_offset (x, y, dx, dy);
    }
  }
  ଶୂନ୍ୟ get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    ଯଦି (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      add_offset (x, y, dx, dy);
    }
  }

  ଶୂନ୍ୟ get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    ଯଦି (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    ଅନ୍ୟଥା
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  ଶୂନ୍ୟ add_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  ଶୂନ୍ୟ add_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  ଶୂନ୍ୟ add_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    add_offset (x, y, origin_x, origin_y);
  }

  ଶୂନ୍ୟ subtract_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  ଶୂନ୍ୟ subtract_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  ଶୂନ୍ୟ subtract_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    subtract_offset (x, y, origin_x, origin_y);
  }

  ଶୂନ୍ୟ get_glyph_kerning_for_direction (hb_codepoint_t first_glyph, hb_codepoint_t second_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    ଯଦି (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (first_glyph, second_glyph);
    } ଅନ୍ୟଥା {
      *x = 0;
      *y = get_glyph_v_kerning (first_glyph, second_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    ଯଦି (ret)
      subtract_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    ଫେରନ୍ତୁ ret;
  }

  hb_bool_t get_glyph_contour_point_for_origin (hb_codepoint_t glyph, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା point_index,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_point (glyph, point_index, x, y);

    ଯଦି (ret)
      subtract_glyph_origin_for_direction (glyph, direction, x, y);

    ଫେରନ୍ତୁ ret;
  }

  /* Generates gidDDD if glyph has no name. */
  ଶୂନ୍ୟ
  glyph_to_string (hb_codepoint_t glyph,
		   ଅକ୍ଷର *s, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
  {
    ଯଦି (get_glyph_name (glyph, s, size)) ଫେରନ୍ତୁ;

    ଯଦି (size && snprintf (s, size, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (ଅଚଳ ଅକ୍ଷର *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    ଯଦି (get_glyph_from_name (s, len, glyph)) ଫେରନ୍ତୁ ସତ୍ୟ;

    ଯଦି (len == -1) len = strlen (s);

    /* Straight glyph index. */
    ଯଦି (hb_codepoint_parse (s, len, 10, glyph))
      ଫେରନ୍ତୁ ସତ୍ୟ;

    ଯଦି (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      ଯଦି (0 == strncmp (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	ଫେରନ୍ତୁ ସତ୍ୟ;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      ଯଦି (0 == strncmp (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ changed ()
  {
    ଦଶମିକ upem = face->get_upem ();

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
  { ଫେରନ୍ତୁ (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (ଦଶମିକ v, ଦଶମିକ mult)
  { ଫେରନ୍ତୁ (hb_position_t) roundf (em_fmultf (v, mult)); }
  ଦଶମିକ em_fmultf (ଦଶମିକ v, ଦଶମିକ mult)
  { ଫେରନ୍ତୁ v * mult; }
  ଦଶମିକ em_fmult (int32_t v, ଦଶମିକ mult)
  { ଫେରନ୍ତୁ (ଦଶମିକ) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#endif /* HB_FONT_HH */
