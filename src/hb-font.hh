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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-face.hh"
#അന്തര്ഭാവ "hb-atomic.hh"
#അന്തര്ഭാവ "hb-draw.hh"
#അന്തര്ഭാവ "hb-paint-extents.hh"
#അന്തര്ഭാവ "hb-shaper.hh"
#അന്തര്ഭാവ "hb-outline.hh"


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

ഘടന hb_font_funcs_t
{
  hb_object_header_t header;

  ഘടന {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) ശൂന്യം *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *user_data;

  ഘടന {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  സംഘം get_t {
    ഘടന get_funcs_t {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
    } f;
    ശൂന്യം (*array[0
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
#അന്തര്ഭാവ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ഘടന hb_font_t
{
  hb_object_header_t header;
  hb_atomic_t<unsigned> serial;
  hb_atomic_t<unsigned> serial_coords;

  hb_font_t *parent;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  തർക്കം is_synthetic;

  ദശാംശം x_embolden;
  ദശാംശം y_embolden;
  തർക്കം embolden_in_place;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  ദശാംശം slant;
  ദശാംശം slant_xy;

  ദശാംശം x_multf;
  ദശാംശം y_multf;
  int64_t x_mult;
  int64_t y_mult;

  unsigned പൂർണ്ണസംഖ്യ x_ppem;
  unsigned പൂർണ്ണസംഖ്യ y_ppem;

  ദശാംശം ptem;

  /* Font variation coordinates. */
  unsigned പൂർണ്ണസംഖ്യ instance_index;
  തർക്കം has_nonzero_coords;
  unsigned പൂർണ്ണസംഖ്യ അക്കം_coords;
  പൂർണ്ണസംഖ്യ *coords;
  ദശാംശം *design_coords;

  hb_font_funcs_t   *klass;
  ശൂന്യം              *user_data;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> data; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { തിരികെ HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { തിരികെ em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { തിരികെ em_mult (v, y_mult); }
  hb_position_t em_scalef_x (ദശാംശം v) { തിരികെ em_multf (v, x_multf); }
  hb_position_t em_scalef_y (ദശാംശം v) { തിരികെ em_multf (v, y_multf); }
  ദശാംശം em_fscale_x (int32_t v) { തിരികെ em_fmult (v, x_multf); }
  ദശാംശം em_fscale_y (int32_t v) { തിരികെ em_fmult (v, y_multf); }
  ദശാംശം em_fscalef_x (ദശാംശം v) { തിരികെ em_fmultf (v, x_multf); }
  ദശാംശം em_fscalef_y (ദശാംശം v) { തിരികെ em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { തിരികെ em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t parent_scale_x_distance (hb_position_t v)
  {
    എങ്കിൽ (unlikely (parent && parent->x_scale && parent->x_scale != x_scale))
      തിരികെ (hb_position_t) (v * (int64_t) ഇത്->x_scale / ഇത്->parent->x_scale);
    തിരികെ v;
  }
  hb_position_t parent_scale_y_distance (hb_position_t v)
  {
    എങ്കിൽ (unlikely (parent && parent->y_scale && parent->y_scale != y_scale))
      തിരികെ (hb_position_t) (v * (int64_t) ഇത്->y_scale / ഇത്->parent->y_scale);
    തിരികെ v;
  }
  hb_position_t parent_scale_x_position (hb_position_t v)
  { തിരികെ parent_scale_x_distance (v); }
  hb_position_t parent_scale_y_position (hb_position_t v)
  { തിരികെ parent_scale_y_distance (v); }

  ശൂന്യം parent_scale_distance (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_distance (*x);
    *y = parent_scale_y_distance (*y);
  }
  ശൂന്യം parent_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_position (*x);
    *y = parent_scale_y_position (*y);
  }

  ശൂന്യം scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    ദശാംശം x1 = em_fscale_x (extents->x_bearing);
    ദശാംശം y1 = em_fscale_y (extents->y_bearing);
    ദശാംശം x2 = x1 + em_fscale_x (extents->width);
    ദശാംശം y2 = y1 + em_fscale_y (extents->height);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->width = ceilf (x2) - extents->x_bearing;
    extents->height = ceilf (y2) - extents->y_bearing;
  }

  ശൂന്യം synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    എങ്കിൽ (slant_xy)
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
    എങ്കിൽ (x_strength || y_strength)
    {
      /* Y */
      പൂർണ്ണസംഖ്യ y_shift = y_strength;
      എങ്കിൽ (y_scale < 0) y_shift = -y_shift;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->y_bearing + y_shift);
      extents->height = hb_clamp_to<hb_position_t> ((int64_t) extents->height - y_shift);

      /* X */
      പൂർണ്ണസംഖ്യ x_shift = x_strength;
      എങ്കിൽ (x_scale < 0) x_shift = -x_shift;
      എങ്കിൽ (embolden_in_place)
	extents->x_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->x_bearing - x_shift / 2);
      extents->width = hb_clamp_to<hb_position_t> ((int64_t) extents->width + x_shift);
    }
  }


  /* Public getters */

  HB_INTERNAL തർക്കം has_func (unsigned പൂർണ്ണസംഖ്യ i);
  HB_INTERNAL തർക്കം has_func_set (unsigned പൂർണ്ണസംഖ്യ i);

  /* has_* ... */
#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
  തർക്കം \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = ഇത്->klass; \
    unsigned പൂർണ്ണസംഖ്യ i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / വലുപ്പം (funcs->get.array[0]); \
    തിരികെ has_func (i); \
  } \
  തർക്കം \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = ഇത്->klass; \
    unsigned പൂർണ്ണസംഖ്യ i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / വലുപ്പം (funcs->get.array[0]); \
    തിരികെ has_func_set (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				തർക്കം synthetic = സത്യം)
  {
    hb_memset (extents, 0, വലുപ്പം (*extents));
    തർക്കം ret = klass->get.f.font_h_extents (ഇത്, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_h_extents);

    എങ്കിൽ (synthetic && ret)
    {
      /* Embolden */
      പൂർണ്ണസംഖ്യ y_shift = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_shift;
    }

    തിരികെ ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				തർക്കം synthetic = സത്യം)
  {
    hb_memset (extents, 0, വലുപ്പം (*extents));
    തർക്കം ret = klass->get.f.font_v_extents (ഇത്, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_v_extents);

    എങ്കിൽ (synthetic && ret)
    {
      /* Embolden */
      പൂർണ്ണസംഖ്യ x_shift = x_scale < 0 ? -x_strength : x_strength;
      എങ്കിൽ (embolden_in_place)
      {
	extents->ascender += x_shift / 2;
	extents->descender -= x_shift - x_shift / 2;
      }
      അല്ലെങ്കിൽ
	extents->ascender += x_shift;
    }

    തിരികെ ret;
  }

  തർക്കം has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    തിരികെ get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    തിരികെ klass->get.f.nominal_glyph (ഇത്, user_data,
				       unicode, glyph,
				       !klass->user_data ? nullptr : klass->user_data->nominal_glyph);
  }
  unsigned പൂർണ്ണസംഖ്യ get_nominal_glyphs (unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				   സ്ഥിരം hb_codepoint_t *first_unicode,
				   unsigned പൂർണ്ണസംഖ്യ unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned പൂർണ്ണസംഖ്യ glyph_stride)
  {
    തിരികെ klass->get.f.nominal_glyphs (ഇത്, user_data,
					എണ്ണം,
					first_unicode, unicode_stride,
					first_glyph, glyph_stride,
					!klass->user_data ? nullptr : klass->user_data->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    തിരികെ klass->get.f.variation_glyph (ഇത്, user_data,
					 unicode, variation_selector, glyph,
					 !klass->user_data ? nullptr : klass->user_data->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     തർക്കം synthetic = സത്യം)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (ഇത്, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_h_advance);

    എങ്കിൽ (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    തിരികെ advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     തർക്കം synthetic = സത്യം)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (ഇത്, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_v_advance);

    എങ്കിൽ (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    തിരികെ advance;
  }

  ശൂന്യം get_glyph_h_advances (unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
			     സ്ഥിരം hb_codepoint_t *first_glyph,
			     unsigned പൂർണ്ണസംഖ്യ glyph_stride,
			     hb_position_t *first_advance,
			     unsigned പൂർണ്ണസംഖ്യ advance_stride,
			     തർക്കം synthetic = സത്യം)
  {
    klass->get.f.glyph_h_advances (ഇത്, user_data,
				   എണ്ണം,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_h_advances);

    എങ്കിൽ (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  ശൂന്യം get_glyph_v_advances (unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
			     സ്ഥിരം hb_codepoint_t *first_glyph,
			     unsigned പൂർണ്ണസംഖ്യ glyph_stride,
			     hb_position_t *first_advance,
			     unsigned പൂർണ്ണസംഖ്യ advance_stride,
			     തർക്കം synthetic = സത്യം)
  {
    klass->get.f.glyph_v_advances (ഇത്, user_data,
				   എണ്ണം,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_v_advances);

    എങ്കിൽ (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				തർക്കം synthetic = സത്യം)
  {
    *x = *y = 0;
    തർക്കം ret = klass->get.f.glyph_h_origin (ഇത്, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_h_origin);

    എങ്കിൽ (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      എങ്കിൽ (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    തിരികെ ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				തർക്കം synthetic = സത്യം)
  {
    *x = *y = 0;
    തർക്കം ret = klass->get.f.glyph_v_origin (ഇത്, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_v_origin);

    എങ്കിൽ (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      എങ്കിൽ (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    തിരികെ ret;
  }

  hb_bool_t get_glyph_h_origins (unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				 സ്ഥിരം hb_codepoint_t *first_glyph,
				 unsigned പൂർണ്ണസംഖ്യ glyph_stride,
				 hb_position_t *first_x,
				 unsigned പൂർണ്ണസംഖ്യ x_stride,
				 hb_position_t *first_y,
				 unsigned പൂർണ്ണസംഖ്യ y_stride,
				 തർക്കം synthetic = സത്യം)

  {
    തർക്കം ret = klass->get.f.glyph_h_origins (ഇത്, user_data,
					     എണ്ണം,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_h_origins);

    എങ്കിൽ (synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	എങ്കിൽ (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    തിരികെ ret;
  }

  hb_bool_t get_glyph_v_origins (unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				 സ്ഥിരം hb_codepoint_t *first_glyph,
				 unsigned പൂർണ്ണസംഖ്യ glyph_stride,
				 hb_position_t *first_x,
				 unsigned പൂർണ്ണസംഖ്യ x_stride,
				 hb_position_t *first_y,
				 unsigned പൂർണ്ണസംഖ്യ y_stride,
				 തർക്കം synthetic = സത്യം)

  {
    തർക്കം ret = klass->get.f.glyph_v_origins (ഇത്, user_data,
					     എണ്ണം,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_v_origins);

    എങ്കിൽ (synthetic && is_synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	എങ്കിൽ (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    തിരികെ ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t ഇടത്_glyph,
				     hb_codepoint_t വലത്_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    തിരികെ 0;
#അല്ലെങ്കിൽ
    തിരികെ klass->get.f.glyph_h_kerning (ഇത്, user_data,
					 ഇടത്_glyph, വലത്_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_h_kerning);
#endif
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t മുകൾ_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    തിരികെ 0;
#അല്ലെങ്കിൽ
    തിരികെ klass->get.f.glyph_v_kerning (ഇത്, user_data,
					 മുകൾ_glyph, bottom_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_v_kerning);
#endif
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       തർക്കം synthetic = സത്യം)
  {
    hb_memset (extents, 0, വലുപ്പം (*extents));

    /* This is rather messy, but necessary. */

    എങ്കിൽ (!synthetic)
    {
      തിരികെ klass->get.f.glyph_extents (ഇത്, user_data,
					 glyph,
					 extents,
					 !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    }
    എങ്കിൽ (!is_synthetic &&
	klass->get.f.glyph_extents (ഇത്, user_data,
				    glyph,
				    extents,
				    !klass->user_data ? nullptr : klass->user_data->glyph_extents))
      തിരികെ സത്യം;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#ifndef HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    എങ്കിൽ (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().to_glyph_extents ();
      തിരികെ സത്യം;
    }
#endif

#ifndef HB_NO_DRAW
    hb_extents_t<> draw_extents;
    എങ്കിൽ (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.to_glyph_extents ();
      തിരികെ സത്യം;
    }
#endif

    തർക്കം ret = klass->get.f.glyph_extents (ഇത്, user_data,
					   glyph,
					   extents,
					   !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    എങ്കിൽ (ret)
      synthetic_glyph_extents (extents);

    തിരികെ ret;
  }

  hb_bool_t get_glyph_contour_point (hb_codepoint_t glyph, unsigned പൂർണ്ണസംഖ്യ point_index,
				     hb_position_t *x, hb_position_t *y,
				     തർക്കം synthetic = സത്യം)
  {
    *x = *y = 0;
    തർക്കം ret = klass->get.f.glyph_contour_point (ഇത്, user_data,
						 glyph, point_index,
						 x, y,
						 !klass->user_data ? nullptr : klass->user_data->glyph_contour_point);

    എങ്കിൽ (synthetic && ret)
    {
      /* Slant */
      എങ്കിൽ (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      എങ്കിൽ (!embolden_in_place)
      {
	പൂർണ്ണസംഖ്യ x_shift = x_scale < 0 ? -x_strength : x_strength;
	*x += x_shift;
      }
    }

    തിരികെ ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    അക്ഷരം *name, unsigned പൂർണ്ണസംഖ്യ size)
  {
    എങ്കിൽ (size) *name = '\0';
    തിരികെ klass->get.f.glyph_name (ഇത്, user_data,
				    glyph,
				    name, size,
				    !klass->user_data ? nullptr : klass->user_data->glyph_name);
  }

  hb_bool_t get_glyph_from_name (സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    എങ്കിൽ (len == -1) len = strlen (name);
    തിരികെ klass->get.f.glyph_from_name (ഇത്, user_data,
					 name, len,
					 glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_from_name);
  }

  തർക്കം draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, ശൂന്യം *draw_data,
			   തർക്കം synthetic = സത്യം)
  {
#ifndef HB_NO_DRAW
#ifndef HB_NO_OUTLINE
    തർക്കം embolden = x_strength || y_strength;
    തർക്കം slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#അല്ലെങ്കിൽ
    synthetic = അസത്യം;
#endif

    എങ്കിൽ (!synthetic)
    {
      തിരികെ klass->get.f.draw_glyph_or_fail (ഇത്, user_data,
					      glyph,
					      draw_funcs, draw_data,
					      !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail);
    }

#ifndef HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_remaining = draw_funcs->get_budget_remaining_ptr (draw_data);
    എങ്കിൽ (!klass->get.f.draw_glyph_or_fail (ഇത്, user_data,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail))
      തിരികെ അസത്യം;

    // Slant before embolden; produces nicer results.

    എങ്കിൽ (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, അസത്യം);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    എങ്കിൽ (embolden)
    {
      ദശാംശം x_shift = embolden_in_place ? 0 : (ദശാംശം) x_strength / 2;
      ദശാംശം y_shift = (ദശാംശം) y_strength / 2;
      എങ്കിൽ (x_scale < 0) x_shift = -x_shift;
      എങ്കിൽ (y_scale < 0) y_shift = -y_shift;
      outline.embolden (x_strength, y_strength, x_shift, y_shift);
    }

    outline.replay (draw_funcs, draw_data);

    തിരികെ സത്യം;
#endif
#endif
    തിരികെ അസത്യം;
  }

  തർക്കം paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, ശൂന്യം *paint_data,
			    unsigned പൂർണ്ണസംഖ്യ palette,
			    hb_color_t foreground,
			    തർക്കം synthetic = സത്യം)
  {
#ifndef HB_NO_PAINT
    /* Slant */
    എങ്കിൽ (synthetic && slant_xy)
      hb_paint_തള്ളുക_transform (paint_funcs, paint_data,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    തർക്കം ret = klass->get.f.paint_glyph_or_fail (ഇത്, user_data,
						 glyph,
						 paint_funcs, paint_data,
						 palette, foreground,
						 !klass->user_data ? nullptr : klass->user_data->paint_glyph_or_fail);

    എങ്കിൽ (synthetic && slant_xy)
      hb_paint_എടുക്കുക_transform (paint_funcs, paint_data);

    തിരികെ ret;
#endif
    തിരികെ അസത്യം;
  }

  /* A bit higher-level, and with fallback */

  HB_INTERNAL
  ശൂന്യം paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, ശൂന്യം *paint_data,
		    unsigned പൂർണ്ണസംഖ്യ palette,
		    hb_color_t foreground);

  ശൂന്യം get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    എങ്കിൽ (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->വരി_gap = 0;
    }
  }
  ശൂന്യം get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    എങ്കിൽ (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->വരി_gap = 0;
    }
  }

  ശൂന്യം get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    എങ്കിൽ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    അല്ലെങ്കിൽ
      get_v_extents_with_fallback (extents);
  }

  ശൂന്യം get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    എങ്കിൽ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    അല്ലെങ്കിൽ
      *y = get_glyph_v_advance (glyph);
  }
  ശൂന്യം get_glyph_advances_for_direction (hb_direction_t direction,
					 unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
					 സ്ഥിരം hb_codepoint_t *first_glyph,
					 unsigned glyph_stride,
					 hb_position_t *first_advance,
					 unsigned advance_stride)
  {
    എങ്കിൽ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (എണ്ണം, first_glyph, glyph_stride, first_advance, advance_stride);
    അല്ലെങ്കിൽ
      get_glyph_v_advances (എണ്ണം, first_glyph, glyph_stride, first_advance, advance_stride);
  }

  ശൂന്യം apply_offset (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     signed mult)
  {
    പ്രതിജ്ഞാ (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  ശൂന്യം add_offset (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  ശൂന്യം subtract_offset (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  ശൂന്യം guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  ശൂന്യം apply_glyph_h_origins_with_fallback (hb_buffer_t *buf, പൂർണ്ണസംഖ്യ mult)
  {
    തർക്കം has_ascender = അസത്യം;
    hb_position_t ascender = 0;

    ഘടന { hb_position_t x, y; } origins[32];

    unsigned പൂർണ്ണസംഖ്യ offset = 0;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buf->len;
    ആകുമ്പോൾ (offset < എണ്ണം)
    {
      unsigned n = hb_min (എണ്ണം - offset, ARRAY_LENGTH (origins));
      എങ്കിൽ (!get_glyph_h_origins (n,
				&buf->info[offset].codepoint, വലുപ്പം (hb_glyph_info_t),
				&origins[0].x, വലുപ്പം (origins[0]),
				&origins[0].y, വലുപ്പം (origins[0])))
      {
        എങ്കിൽ (get_glyph_v_origins (n,
				  &buf->info[offset].codepoint, വലുപ്പം (hb_glyph_info_t),
				  &origins[0].x, വലുപ്പം (origins[0]),
				  &origins[0].y, വലുപ്പം (origins[0])))
	{
	  എങ്കിൽ (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = സത്യം;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  വേണ്ടി (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	അല്ലെങ്കിൽ
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      പ്രതിജ്ഞാ (mult == -1 || mult == +1 || mult == 0);
      എങ്കിൽ (mult == +1)
        വേണ്ടി (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      അല്ലെങ്കിൽ എങ്കിൽ (mult == -1)
	വേണ്ടി (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }
  ശൂന്യം apply_glyph_v_origins_with_fallback (hb_buffer_t *buf, പൂർണ്ണസംഖ്യ mult)
  {
    തർക്കം has_ascender = അസത്യം;
    hb_position_t ascender = 0;

    ഘടന { hb_position_t x, y; } origins[32];

    unsigned പൂർണ്ണസംഖ്യ offset = 0;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buf->len;
    ആകുമ്പോൾ (offset < എണ്ണം)
    {
      unsigned n = hb_min (എണ്ണം - offset, ARRAY_LENGTH (origins));
      എങ്കിൽ (!get_glyph_v_origins (n,
				&buf->info[offset].codepoint, വലുപ്പം (hb_glyph_info_t),
				&origins[0].x, വലുപ്പം (origins[0]),
				&origins[0].y, വലുപ്പം (origins[0])))
      {
	എങ്കിൽ (get_glyph_h_origins (n,
				 &buf->info[offset].codepoint, വലുപ്പം (hb_glyph_info_t),
				 &origins[0].x, വലുപ്പം (origins[0]),
				 &origins[0].y, വലുപ്പം (origins[0])))
	{
	  എങ്കിൽ (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = സത്യം;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  വേണ്ടി (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	അല്ലെങ്കിൽ
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      പ്രതിജ്ഞാ (mult == -1 || mult == +1 || mult == 0);
      എങ്കിൽ (mult == +1)
        വേണ്ടി (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      അല്ലെങ്കിൽ എങ്കിൽ (mult == -1)
	വേണ്ടി (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }

  ശൂന്യം get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    എങ്കിൽ (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      subtract_offset (x, y, dx, dy);
    }
  }
  ശൂന്യം get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    എങ്കിൽ (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      add_offset (x, y, dx, dy);
    }
  }

  ശൂന്യം get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    എങ്കിൽ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    അല്ലെങ്കിൽ
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  ശൂന്യം add_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  ശൂന്യം add_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  ശൂന്യം add_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    add_offset (x, y, origin_x, origin_y);
  }

  ശൂന്യം subtract_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  ശൂന്യം subtract_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  ശൂന്യം subtract_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    subtract_offset (x, y, origin_x, origin_y);
  }

  ശൂന്യം get_glyph_kerning_for_direction (hb_codepoint_t first_glyph, hb_codepoint_t second_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    എങ്കിൽ (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (first_glyph, second_glyph);
    } അല്ലെങ്കിൽ {
      *x = 0;
      *y = get_glyph_v_kerning (first_glyph, second_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    എങ്കിൽ (ret)
      subtract_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    തിരികെ ret;
  }

  hb_bool_t get_glyph_contour_point_for_origin (hb_codepoint_t glyph, unsigned പൂർണ്ണസംഖ്യ point_index,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_point (glyph, point_index, x, y);

    എങ്കിൽ (ret)
      subtract_glyph_origin_for_direction (glyph, direction, x, y);

    തിരികെ ret;
  }

  /* Generates gidDDD if glyph has no name. */
  ശൂന്യം
  glyph_to_string (hb_codepoint_t glyph,
		   അക്ഷരം *s, unsigned പൂർണ്ണസംഖ്യ size)
  {
    എങ്കിൽ (get_glyph_name (glyph, s, size)) തിരികെ;

    എങ്കിൽ (size && snprintf (s, size, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (സ്ഥിരം അക്ഷരം *s, പൂർണ്ണസംഖ്യ len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    എങ്കിൽ (get_glyph_from_name (s, len, glyph)) തിരികെ സത്യം;

    എങ്കിൽ (len == -1) len = strlen (s);

    /* Straight glyph index. */
    എങ്കിൽ (hb_codepoint_parse (s, len, 10, glyph))
      തിരികെ സത്യം;

    എങ്കിൽ (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      എങ്കിൽ (0 == strncmp (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	തിരികെ സത്യം;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      എങ്കിൽ (0 == strncmp (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	തിരികെ സത്യം;
    }

    തിരികെ അസത്യം;
  }

  ശൂന്യം changed ()
  {
    ദശാംശം upem = face->get_upem ();

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
  { തിരികെ (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (ദശാംശം v, ദശാംശം mult)
  { തിരികെ (hb_position_t) roundf (em_fmultf (v, mult)); }
  ദശാംശം em_fmultf (ദശാംശം v, ദശാംശം mult)
  { തിരികെ v * mult; }
  ദശാംശം em_fmult (int32_t v, ദശാംശം mult)
  { തിരികെ (ദശാംശം) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#endif /* HB_FONT_HH */
