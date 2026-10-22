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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-face.hh"
#ಅನ್ತರ್ಭಾವ "hb-atomic.hh"
#ಅನ್ತರ್ಭಾವ "hb-draw.hh"
#ಅನ್ತರ್ಭಾವ "hb-paint-extents.hh"
#ಅನ್ತರ್ಭಾವ "hb-shaper.hh"
#ಅನ್ತರ್ಭಾವ "hb-outline.hh"


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

ಸಂರಚನಾ hb_font_funcs_t
{
  hb_object_header_t header;

  ಸಂರಚನಾ {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) ಶೂನ್ಯ *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *user_data;

  ಸಂರಚನಾ {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  ಸಙ್ಘ get_t {
    ಸಂರಚನಾ get_funcs_t {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
    } f;
    ಶೂನ್ಯ (*array[0
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
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ಸಂರಚನಾ hb_font_t
{
  hb_object_header_t header;
  hb_atomic_t<unsigned> serial;
  hb_atomic_t<unsigned> serial_coords;

  hb_font_t *parent;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  ತಾರ್ಕಿಕ is_synthetic;

  ಭಿನ್ನ x_embolden;
  ಭಿನ್ನ y_embolden;
  ತಾರ್ಕಿಕ embolden_in_place;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  ಭಿನ್ನ slant;
  ಭಿನ್ನ slant_xy;

  ಭಿನ್ನ x_multf;
  ಭಿನ್ನ y_multf;
  int64_t x_mult;
  int64_t y_mult;

  unsigned ಪೂರ್ಣಾಙ್ಕ x_ppem;
  unsigned ಪೂರ್ಣಾಙ್ಕ y_ppem;

  ಭಿನ್ನ ptem;

  /* Font variation coordinates. */
  unsigned ಪೂರ್ಣಾಙ್ಕ instance_index;
  ತಾರ್ಕಿಕ has_nonzero_coords;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_coords;
  ಪೂರ್ಣಾಙ್ಕ *coords;
  ಭಿನ್ನ *design_coords;

  hb_font_funcs_t   *klass;
  ಶೂನ್ಯ              *user_data;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> data; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { ಪ್ರತ್ಯಾಗಮ HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { ಪ್ರತ್ಯಾಗಮ em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { ಪ್ರತ್ಯಾಗಮ em_mult (v, y_mult); }
  hb_position_t em_scalef_x (ಭಿನ್ನ v) { ಪ್ರತ್ಯಾಗಮ em_multf (v, x_multf); }
  hb_position_t em_scalef_y (ಭಿನ್ನ v) { ಪ್ರತ್ಯಾಗಮ em_multf (v, y_multf); }
  ಭಿನ್ನ em_fscale_x (int32_t v) { ಪ್ರತ್ಯಾಗಮ em_fmult (v, x_multf); }
  ಭಿನ್ನ em_fscale_y (int32_t v) { ಪ್ರತ್ಯಾಗಮ em_fmult (v, y_multf); }
  ಭಿನ್ನ em_fscalef_x (ಭಿನ್ನ v) { ಪ್ರತ್ಯಾಗಮ em_fmultf (v, x_multf); }
  ಭಿನ್ನ em_fscalef_y (ಭಿನ್ನ v) { ಪ್ರತ್ಯಾಗಮ em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { ಪ್ರತ್ಯಾಗಮ em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t parent_scale_x_distance (hb_position_t v)
  {
    ಯದಿ (unlikely (parent && parent->x_scale && parent->x_scale != x_scale))
      ಪ್ರತ್ಯಾಗಮ (hb_position_t) (v * (int64_t) ಏತತ್->x_scale / ಏತತ್->parent->x_scale);
    ಪ್ರತ್ಯಾಗಮ v;
  }
  hb_position_t parent_scale_y_distance (hb_position_t v)
  {
    ಯದಿ (unlikely (parent && parent->y_scale && parent->y_scale != y_scale))
      ಪ್ರತ್ಯಾಗಮ (hb_position_t) (v * (int64_t) ಏತತ್->y_scale / ಏತತ್->parent->y_scale);
    ಪ್ರತ್ಯಾಗಮ v;
  }
  hb_position_t parent_scale_x_position (hb_position_t v)
  { ಪ್ರತ್ಯಾಗಮ parent_scale_x_distance (v); }
  hb_position_t parent_scale_y_position (hb_position_t v)
  { ಪ್ರತ್ಯಾಗಮ parent_scale_y_distance (v); }

  ಶೂನ್ಯ parent_scale_distance (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_distance (*x);
    *y = parent_scale_y_distance (*y);
  }
  ಶೂನ್ಯ parent_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_position (*x);
    *y = parent_scale_y_position (*y);
  }

  ಶೂನ್ಯ scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    ಭಿನ್ನ x1 = em_fscale_x (extents->x_bearing);
    ಭಿನ್ನ y1 = em_fscale_y (extents->y_bearing);
    ಭಿನ್ನ x2 = x1 + em_fscale_x (extents->width);
    ಭಿನ್ನ y2 = y1 + em_fscale_y (extents->height);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->width = ceilf (x2) - extents->x_bearing;
    extents->height = ceilf (y2) - extents->y_bearing;
  }

  ಶೂನ್ಯ synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    ಯದಿ (slant_xy)
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
    ಯದಿ (x_strength || y_strength)
    {
      /* Y */
      ಪೂರ್ಣಾಙ್ಕ y_shift = y_strength;
      ಯದಿ (y_scale < 0) y_shift = -y_shift;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->y_bearing + y_shift);
      extents->height = hb_clamp_to<hb_position_t> ((int64_t) extents->height - y_shift);

      /* X */
      ಪೂರ್ಣಾಙ್ಕ x_shift = x_strength;
      ಯದಿ (x_scale < 0) x_shift = -x_shift;
      ಯದಿ (embolden_in_place)
	extents->x_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->x_bearing - x_shift / 2);
      extents->width = hb_clamp_to<hb_position_t> ((int64_t) extents->width + x_shift);
    }
  }


  /* Public getters */

  HB_INTERNAL ತಾರ್ಕಿಕ has_func (unsigned ಪೂರ್ಣಾಙ್ಕ i);
  HB_INTERNAL ತಾರ್ಕಿಕ has_func_set (unsigned ಪೂರ್ಣಾಙ್ಕ i);

  /* has_* ... */
#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
  ತಾರ್ಕಿಕ \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = ಏತತ್->klass; \
    unsigned ಪೂರ್ಣಾಙ್ಕ i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / ಪರಿಮಾಣ (funcs->get.array[0]); \
    ಪ್ರತ್ಯಾಗಮ has_func (i); \
  } \
  ತಾರ್ಕಿಕ \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = ಏತತ್->klass; \
    unsigned ಪೂರ್ಣಾಙ್ಕ i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / ಪರಿಮಾಣ (funcs->get.array[0]); \
    ಪ್ರತ್ಯಾಗಮ has_func_set (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
    hb_memset (extents, 0, ಪರಿಮಾಣ (*extents));
    ತಾರ್ಕಿಕ ret = klass->get.f.font_h_extents (ಏತತ್, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_h_extents);

    ಯದಿ (synthetic && ret)
    {
      /* Embolden */
      ಪೂರ್ಣಾಙ್ಕ y_shift = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_shift;
    }

    ಪ್ರತ್ಯಾಗಮ ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
    hb_memset (extents, 0, ಪರಿಮಾಣ (*extents));
    ತಾರ್ಕಿಕ ret = klass->get.f.font_v_extents (ಏತತ್, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_v_extents);

    ಯದಿ (synthetic && ret)
    {
      /* Embolden */
      ಪೂರ್ಣಾಙ್ಕ x_shift = x_scale < 0 ? -x_strength : x_strength;
      ಯದಿ (embolden_in_place)
      {
	extents->ascender += x_shift / 2;
	extents->descender -= x_shift - x_shift / 2;
      }
      ಅನ್ಯಥಾ
	extents->ascender += x_shift;
    }

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ತಾರ್ಕಿಕ has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    ಪ್ರತ್ಯಾಗಮ get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    ಪ್ರತ್ಯಾಗಮ klass->get.f.nominal_glyph (ಏತತ್, user_data,
				       unicode, glyph,
				       !klass->user_data ? nullptr : klass->user_data->nominal_glyph);
  }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_nominal_glyphs (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				   ಧ್ರುವ hb_codepoint_t *first_unicode,
				   unsigned ಪೂರ್ಣಾಙ್ಕ unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned ಪೂರ್ಣಾಙ್ಕ glyph_stride)
  {
    ಪ್ರತ್ಯಾಗಮ klass->get.f.nominal_glyphs (ಏತತ್, user_data,
					ಎಣಿಕೆ,
					first_unicode, unicode_stride,
					first_glyph, glyph_stride,
					!klass->user_data ? nullptr : klass->user_data->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    ಪ್ರತ್ಯಾಗಮ klass->get.f.variation_glyph (ಏತತ್, user_data,
					 unicode, variation_selector, glyph,
					 !klass->user_data ? nullptr : klass->user_data->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (ಏತತ್, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_h_advance);

    ಯದಿ (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    ಪ್ರತ್ಯಾಗಮ advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (ಏತತ್, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_v_advance);

    ಯದಿ (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    ಪ್ರತ್ಯಾಗಮ advance;
  }

  ಶೂನ್ಯ get_glyph_h_advances (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
			     ಧ್ರುವ hb_codepoint_t *first_glyph,
			     unsigned ಪೂರ್ಣಾಙ್ಕ glyph_stride,
			     hb_position_t *first_advance,
			     unsigned ಪೂರ್ಣಾಙ್ಕ advance_stride,
			     ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
    klass->get.f.glyph_h_advances (ಏತತ್, user_data,
				   ಎಣಿಕೆ,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_h_advances);

    ಯದಿ (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  ಶೂನ್ಯ get_glyph_v_advances (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
			     ಧ್ರುವ hb_codepoint_t *first_glyph,
			     unsigned ಪೂರ್ಣಾಙ್ಕ glyph_stride,
			     hb_position_t *first_advance,
			     unsigned ಪೂರ್ಣಾಙ್ಕ advance_stride,
			     ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
    klass->get.f.glyph_v_advances (ಏತತ್, user_data,
				   ಎಣಿಕೆ,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_v_advances);

    ಯದಿ (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
    *x = *y = 0;
    ತಾರ್ಕಿಕ ret = klass->get.f.glyph_h_origin (ಏತತ್, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_h_origin);

    ಯದಿ (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      ಯದಿ (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
    *x = *y = 0;
    ತಾರ್ಕಿಕ ret = klass->get.f.glyph_v_origin (ಏತತ್, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_v_origin);

    ಯದಿ (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      ಯದಿ (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  hb_bool_t get_glyph_h_origins (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				 ಧ್ರುವ hb_codepoint_t *first_glyph,
				 unsigned ಪೂರ್ಣಾಙ್ಕ glyph_stride,
				 hb_position_t *first_x,
				 unsigned ಪೂರ್ಣಾಙ್ಕ x_stride,
				 hb_position_t *first_y,
				 unsigned ಪೂರ್ಣಾಙ್ಕ y_stride,
				 ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)

  {
    ತಾರ್ಕಿಕ ret = klass->get.f.glyph_h_origins (ಏತತ್, user_data,
					     ಎಣಿಕೆ,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_h_origins);

    ಯದಿ (synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	ಯದಿ (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  hb_bool_t get_glyph_v_origins (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				 ಧ್ರುವ hb_codepoint_t *first_glyph,
				 unsigned ಪೂರ್ಣಾಙ್ಕ glyph_stride,
				 hb_position_t *first_x,
				 unsigned ಪೂರ್ಣಾಙ್ಕ x_stride,
				 hb_position_t *first_y,
				 unsigned ಪೂರ್ಣಾಙ್ಕ y_stride,
				 ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)

  {
    ತಾರ್ಕಿಕ ret = klass->get.f.glyph_v_origins (ಏತತ್, user_data,
					     ಎಣಿಕೆ,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_v_origins);

    ಯದಿ (synthetic && is_synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	ಯದಿ (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t ಎಡ_glyph,
				     hb_codepoint_t ಬಲ_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ klass->get.f.glyph_h_kerning (ಏತತ್, user_data,
					 ಎಡ_glyph, ಬಲ_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_h_kerning);
#endif
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t ತುದಿ_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ klass->get.f.glyph_v_kerning (ಏತತ್, user_data,
					 ತುದಿ_glyph, bottom_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_v_kerning);
#endif
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
    hb_memset (extents, 0, ಪರಿಮಾಣ (*extents));

    /* This is rather messy, but necessary. */

    ಯದಿ (!synthetic)
    {
      ಪ್ರತ್ಯಾಗಮ klass->get.f.glyph_extents (ಏತತ್, user_data,
					 glyph,
					 extents,
					 !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    }
    ಯದಿ (!is_synthetic &&
	klass->get.f.glyph_extents (ಏತತ್, user_data,
				    glyph,
				    extents,
				    !klass->user_data ? nullptr : klass->user_data->glyph_extents))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#ifndef HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    ಯದಿ (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().to_glyph_extents ();
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
#endif

#ifndef HB_NO_DRAW
    hb_extents_t<> draw_extents;
    ಯದಿ (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.to_glyph_extents ();
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
#endif

    ತಾರ್ಕಿಕ ret = klass->get.f.glyph_extents (ಏತತ್, user_data,
					   glyph,
					   extents,
					   !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    ಯದಿ (ret)
      synthetic_glyph_extents (extents);

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  hb_bool_t get_glyph_contour_point (hb_codepoint_t glyph, unsigned ಪೂರ್ಣಾಙ್ಕ point_index,
				     hb_position_t *x, hb_position_t *y,
				     ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
    *x = *y = 0;
    ತಾರ್ಕಿಕ ret = klass->get.f.glyph_contour_point (ಏತತ್, user_data,
						 glyph, point_index,
						 x, y,
						 !klass->user_data ? nullptr : klass->user_data->glyph_contour_point);

    ಯದಿ (synthetic && ret)
    {
      /* Slant */
      ಯದಿ (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      ಯದಿ (!embolden_in_place)
      {
	ಪೂರ್ಣಾಙ್ಕ x_shift = x_scale < 0 ? -x_strength : x_strength;
	*x += x_shift;
      }
    }

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    ವರ್ಣ *name, unsigned ಪೂರ್ಣಾಙ್ಕ size)
  {
    ಯದಿ (size) *name = '\0';
    ಪ್ರತ್ಯಾಗಮ klass->get.f.glyph_name (ಏತತ್, user_data,
				    glyph,
				    name, size,
				    !klass->user_data ? nullptr : klass->user_data->glyph_name);
  }

  hb_bool_t get_glyph_from_name (ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    ಯದಿ (len == -1) len = strlen (name);
    ಪ್ರತ್ಯಾಗಮ klass->get.f.glyph_from_name (ಏತತ್, user_data,
					 name, len,
					 glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_from_name);
  }

  ತಾರ್ಕಿಕ draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, ಶೂನ್ಯ *draw_data,
			   ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
#ifndef HB_NO_DRAW
#ifndef HB_NO_OUTLINE
    ತಾರ್ಕಿಕ embolden = x_strength || y_strength;
    ತಾರ್ಕಿಕ slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#ಅನ್ಯಥಾ
    synthetic = ಅಸತ್ಯ;
#endif

    ಯದಿ (!synthetic)
    {
      ಪ್ರತ್ಯಾಗಮ klass->get.f.draw_glyph_or_fail (ಏತತ್, user_data,
					      glyph,
					      draw_funcs, draw_data,
					      !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail);
    }

#ifndef HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_remaining = draw_funcs->get_budget_remaining_ptr (draw_data);
    ಯದಿ (!klass->get.f.draw_glyph_or_fail (ಏತತ್, user_data,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    // Slant before embolden; produces nicer results.

    ಯದಿ (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, ಅಸತ್ಯ);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    ಯದಿ (embolden)
    {
      ಭಿನ್ನ x_shift = embolden_in_place ? 0 : (ಭಿನ್ನ) x_strength / 2;
      ಭಿನ್ನ y_shift = (ಭಿನ್ನ) y_strength / 2;
      ಯದಿ (x_scale < 0) x_shift = -x_shift;
      ಯದಿ (y_scale < 0) y_shift = -y_shift;
      outline.embolden (x_strength, y_strength, x_shift, y_shift);
    }

    outline.replay (draw_funcs, draw_data);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
#endif
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, ಶೂನ್ಯ *paint_data,
			    unsigned ಪೂರ್ಣಾಙ್ಕ palette,
			    hb_color_t foreground,
			    ತಾರ್ಕಿಕ synthetic = ಸತ್ಯ)
  {
#ifndef HB_NO_PAINT
    /* Slant */
    ಯದಿ (synthetic && slant_xy)
      hb_paint_ತಳ್ಳು_transform (paint_funcs, paint_data,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    ತಾರ್ಕಿಕ ret = klass->get.f.paint_glyph_or_fail (ಏತತ್, user_data,
						 glyph,
						 paint_funcs, paint_data,
						 palette, foreground,
						 !klass->user_data ? nullptr : klass->user_data->paint_glyph_or_fail);

    ಯದಿ (synthetic && slant_xy)
      hb_paint_ತೆಗೆ_transform (paint_funcs, paint_data);

    ಪ್ರತ್ಯಾಗಮ ret;
#endif
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  /* A bit higher-level, and with fallback */

  HB_INTERNAL
  ಶೂನ್ಯ paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, ಶೂನ್ಯ *paint_data,
		    unsigned ಪೂರ್ಣಾಙ್ಕ palette,
		    hb_color_t foreground);

  ಶೂನ್ಯ get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    ಯದಿ (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->ಸಾಲು_gap = 0;
    }
  }
  ಶೂನ್ಯ get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    ಯದಿ (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->ಸಾಲು_gap = 0;
    }
  }

  ಶೂನ್ಯ get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    ಯದಿ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    ಅನ್ಯಥಾ
      get_v_extents_with_fallback (extents);
  }

  ಶೂನ್ಯ get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    ಯದಿ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    ಅನ್ಯಥಾ
      *y = get_glyph_v_advance (glyph);
  }
  ಶೂನ್ಯ get_glyph_advances_for_direction (hb_direction_t direction,
					 unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
					 ಧ್ರುವ hb_codepoint_t *first_glyph,
					 unsigned glyph_stride,
					 hb_position_t *first_advance,
					 unsigned advance_stride)
  {
    ಯದಿ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (ಎಣಿಕೆ, first_glyph, glyph_stride, first_advance, advance_stride);
    ಅನ್ಯಥಾ
      get_glyph_v_advances (ಎಣಿಕೆ, first_glyph, glyph_stride, first_advance, advance_stride);
  }

  ಶೂನ್ಯ apply_offset (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     signed mult)
  {
    ಪ್ರತಿಜ್ಞಾ (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  ಶೂನ್ಯ add_offset (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  ಶೂನ್ಯ subtract_offset (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  ಶೂನ್ಯ guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  ಶೂನ್ಯ apply_glyph_h_origins_with_fallback (hb_buffer_t *buf, ಪೂರ್ಣಾಙ್ಕ mult)
  {
    ತಾರ್ಕಿಕ has_ascender = ಅಸತ್ಯ;
    hb_position_t ascender = 0;

    ಸಂರಚನಾ { hb_position_t x, y; } origins[32];

    unsigned ಪೂರ್ಣಾಙ್ಕ offset = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = buf->len;
    ಯಾವತ್ (offset < ಎಣಿಕೆ)
    {
      unsigned n = hb_min (ಎಣಿಕೆ - offset, ARRAY_LENGTH (origins));
      ಯದಿ (!get_glyph_h_origins (n,
				&buf->info[offset].codepoint, ಪರಿಮಾಣ (hb_glyph_info_t),
				&origins[0].x, ಪರಿಮಾಣ (origins[0]),
				&origins[0].y, ಪರಿಮಾಣ (origins[0])))
      {
        ಯದಿ (get_glyph_v_origins (n,
				  &buf->info[offset].codepoint, ಪರಿಮಾಣ (hb_glyph_info_t),
				  &origins[0].x, ಪರಿಮಾಣ (origins[0]),
				  &origins[0].y, ಪರಿಮಾಣ (origins[0])))
	{
	  ಯದಿ (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = ಸತ್ಯ;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  ಪ್ರತಿ (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	ಅನ್ಯಥಾ
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      ಪ್ರತಿಜ್ಞಾ (mult == -1 || mult == +1 || mult == 0);
      ಯದಿ (mult == +1)
        ಪ್ರತಿ (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      ಅನ್ಯಥಾ ಯದಿ (mult == -1)
	ಪ್ರತಿ (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }
  ಶೂನ್ಯ apply_glyph_v_origins_with_fallback (hb_buffer_t *buf, ಪೂರ್ಣಾಙ್ಕ mult)
  {
    ತಾರ್ಕಿಕ has_ascender = ಅಸತ್ಯ;
    hb_position_t ascender = 0;

    ಸಂರಚನಾ { hb_position_t x, y; } origins[32];

    unsigned ಪೂರ್ಣಾಙ್ಕ offset = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = buf->len;
    ಯಾವತ್ (offset < ಎಣಿಕೆ)
    {
      unsigned n = hb_min (ಎಣಿಕೆ - offset, ARRAY_LENGTH (origins));
      ಯದಿ (!get_glyph_v_origins (n,
				&buf->info[offset].codepoint, ಪರಿಮಾಣ (hb_glyph_info_t),
				&origins[0].x, ಪರಿಮಾಣ (origins[0]),
				&origins[0].y, ಪರಿಮಾಣ (origins[0])))
      {
	ಯದಿ (get_glyph_h_origins (n,
				 &buf->info[offset].codepoint, ಪರಿಮಾಣ (hb_glyph_info_t),
				 &origins[0].x, ಪರಿಮಾಣ (origins[0]),
				 &origins[0].y, ಪರಿಮಾಣ (origins[0])))
	{
	  ಯದಿ (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = ಸತ್ಯ;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  ಪ್ರತಿ (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	ಅನ್ಯಥಾ
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      ಪ್ರತಿಜ್ಞಾ (mult == -1 || mult == +1 || mult == 0);
      ಯದಿ (mult == +1)
        ಪ್ರತಿ (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      ಅನ್ಯಥಾ ಯದಿ (mult == -1)
	ಪ್ರತಿ (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }

  ಶೂನ್ಯ get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    ಯದಿ (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      subtract_offset (x, y, dx, dy);
    }
  }
  ಶೂನ್ಯ get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    ಯದಿ (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      add_offset (x, y, dx, dy);
    }
  }

  ಶೂನ್ಯ get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    ಯದಿ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    ಅನ್ಯಥಾ
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  ಶೂನ್ಯ add_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  ಶೂನ್ಯ add_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  ಶೂನ್ಯ add_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    add_offset (x, y, origin_x, origin_y);
  }

  ಶೂನ್ಯ subtract_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  ಶೂನ್ಯ subtract_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  ಶೂನ್ಯ subtract_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    subtract_offset (x, y, origin_x, origin_y);
  }

  ಶೂನ್ಯ get_glyph_kerning_for_direction (hb_codepoint_t first_glyph, hb_codepoint_t second_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    ಯದಿ (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (first_glyph, second_glyph);
    } ಅನ್ಯಥಾ {
      *x = 0;
      *y = get_glyph_v_kerning (first_glyph, second_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    ಯದಿ (ret)
      subtract_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  hb_bool_t get_glyph_contour_point_for_origin (hb_codepoint_t glyph, unsigned ಪೂರ್ಣಾಙ್ಕ point_index,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_point (glyph, point_index, x, y);

    ಯದಿ (ret)
      subtract_glyph_origin_for_direction (glyph, direction, x, y);

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  /* Generates gidDDD if glyph has no name. */
  ಶೂನ್ಯ
  glyph_to_string (hb_codepoint_t glyph,
		   ವರ್ಣ *s, unsigned ಪೂರ್ಣಾಙ್ಕ size)
  {
    ಯದಿ (get_glyph_name (glyph, s, size)) ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (size && snprintf (s, size, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (ಧ್ರುವ ವರ್ಣ *s, ಪೂರ್ಣಾಙ್ಕ len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    ಯದಿ (get_glyph_from_name (s, len, glyph)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಯದಿ (len == -1) len = strlen (s);

    /* Straight glyph index. */
    ಯದಿ (hb_codepoint_parse (s, len, 10, glyph))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಯದಿ (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      ಯದಿ (0 == strncmp (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      ಯದಿ (0 == strncmp (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ changed ()
  {
    ಭಿನ್ನ upem = face->get_upem ();

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
  { ಪ್ರತ್ಯಾಗಮ (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (ಭಿನ್ನ v, ಭಿನ್ನ mult)
  { ಪ್ರತ್ಯಾಗಮ (hb_position_t) roundf (em_fmultf (v, mult)); }
  ಭಿನ್ನ em_fmultf (ಭಿನ್ನ v, ಭಿನ್ನ mult)
  { ಪ್ರತ್ಯಾಗಮ v * mult; }
  ಭಿನ್ನ em_fmult (int32_t v, ಭಿನ್ನ mult)
  { ಪ್ರತ್ಯಾಗಮ (ಭಿನ್ನ) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#endif /* HB_FONT_HH */
