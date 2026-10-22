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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-face.hh"
#અન્તર્ભાવ "hb-atomic.hh"
#અન્તર્ભાવ "hb-draw.hh"
#અન્તર્ભાવ "hb-paint-extents.hh"
#અન્તર્ભાવ "hb-shaper.hh"
#અન્તર્ભાવ "hb-outline.hh"


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

સંરચના hb_font_funcs_t
{
  hb_object_header_t header;

  સંરચના {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) શૂન્ય *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *user_data;

  સંરચના {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  સંઘ get_t {
    સંરચના get_funcs_t {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
    } f;
    શૂન્ય (*array[0
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
#અન્તર્ભાવ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

સંરચના hb_font_t
{
  hb_object_header_t header;
  hb_atomic_t<unsigned> serial;
  hb_atomic_t<unsigned> serial_coords;

  hb_font_t *parent;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  તાર્કિક is_synthetic;

  દશાંશ x_embolden;
  દશાંશ y_embolden;
  તાર્કિક embolden_in_place;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  દશાંશ slant;
  દશાંશ slant_xy;

  દશાંશ x_multf;
  દશાંશ y_multf;
  int64_t x_mult;
  int64_t y_mult;

  unsigned પૂર્ણાંક x_ppem;
  unsigned પૂર્ણાંક y_ppem;

  દશાંશ ptem;

  /* Font variation coordinates. */
  unsigned પૂર્ણાંક instance_index;
  તાર્કિક has_nonzero_coords;
  unsigned પૂર્ણાંક અંક_coords;
  પૂર્ણાંક *coords;
  દશાંશ *design_coords;

  hb_font_funcs_t   *klass;
  શૂન્ય              *user_data;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> data; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { પાછા HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { પાછા em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { પાછા em_mult (v, y_mult); }
  hb_position_t em_scalef_x (દશાંશ v) { પાછા em_multf (v, x_multf); }
  hb_position_t em_scalef_y (દશાંશ v) { પાછા em_multf (v, y_multf); }
  દશાંશ em_fscale_x (int32_t v) { પાછા em_fmult (v, x_multf); }
  દશાંશ em_fscale_y (int32_t v) { પાછા em_fmult (v, y_multf); }
  દશાંશ em_fscalef_x (દશાંશ v) { પાછા em_fmultf (v, x_multf); }
  દશાંશ em_fscalef_y (દશાંશ v) { પાછા em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { પાછા em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t parent_scale_x_distance (hb_position_t v)
  {
    જો (unlikely (parent && parent->x_scale && parent->x_scale != x_scale))
      પાછા (hb_position_t) (v * (int64_t) આ->x_scale / આ->parent->x_scale);
    પાછા v;
  }
  hb_position_t parent_scale_y_distance (hb_position_t v)
  {
    જો (unlikely (parent && parent->y_scale && parent->y_scale != y_scale))
      પાછા (hb_position_t) (v * (int64_t) આ->y_scale / આ->parent->y_scale);
    પાછા v;
  }
  hb_position_t parent_scale_x_position (hb_position_t v)
  { પાછા parent_scale_x_distance (v); }
  hb_position_t parent_scale_y_position (hb_position_t v)
  { પાછા parent_scale_y_distance (v); }

  શૂન્ય parent_scale_distance (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_distance (*x);
    *y = parent_scale_y_distance (*y);
  }
  શૂન્ય parent_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_position (*x);
    *y = parent_scale_y_position (*y);
  }

  શૂન્ય scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    દશાંશ x1 = em_fscale_x (extents->x_bearing);
    દશાંશ y1 = em_fscale_y (extents->y_bearing);
    દશાંશ x2 = x1 + em_fscale_x (extents->width);
    દશાંશ y2 = y1 + em_fscale_y (extents->height);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->width = ceilf (x2) - extents->x_bearing;
    extents->height = ceilf (y2) - extents->y_bearing;
  }

  શૂન્ય synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    જો (slant_xy)
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
    જો (x_strength || y_strength)
    {
      /* Y */
      પૂર્ણાંક y_shift = y_strength;
      જો (y_scale < 0) y_shift = -y_shift;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->y_bearing + y_shift);
      extents->height = hb_clamp_to<hb_position_t> ((int64_t) extents->height - y_shift);

      /* X */
      પૂર્ણાંક x_shift = x_strength;
      જો (x_scale < 0) x_shift = -x_shift;
      જો (embolden_in_place)
	extents->x_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->x_bearing - x_shift / 2);
      extents->width = hb_clamp_to<hb_position_t> ((int64_t) extents->width + x_shift);
    }
  }


  /* Public getters */

  HB_INTERNAL તાર્કિક has_func (unsigned પૂર્ણાંક i);
  HB_INTERNAL તાર્કિક has_func_set (unsigned પૂર્ણાંક i);

  /* has_* ... */
#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
  તાર્કિક \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = આ->klass; \
    unsigned પૂર્ણાંક i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / કદ (funcs->get.array[0]); \
    પાછા has_func (i); \
  } \
  તાર્કિક \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = આ->klass; \
    unsigned પૂર્ણાંક i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / કદ (funcs->get.array[0]); \
    પાછા has_func_set (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				તાર્કિક synthetic = સાચું)
  {
    hb_memset (extents, 0, કદ (*extents));
    તાર્કિક ret = klass->get.f.font_h_extents (આ, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_h_extents);

    જો (synthetic && ret)
    {
      /* Embolden */
      પૂર્ણાંક y_shift = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_shift;
    }

    પાછા ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				તાર્કિક synthetic = સાચું)
  {
    hb_memset (extents, 0, કદ (*extents));
    તાર્કિક ret = klass->get.f.font_v_extents (આ, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_v_extents);

    જો (synthetic && ret)
    {
      /* Embolden */
      પૂર્ણાંક x_shift = x_scale < 0 ? -x_strength : x_strength;
      જો (embolden_in_place)
      {
	extents->ascender += x_shift / 2;
	extents->descender -= x_shift - x_shift / 2;
      }
      નહીંતર
	extents->ascender += x_shift;
    }

    પાછા ret;
  }

  તાર્કિક has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    પાછા get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    પાછા klass->get.f.nominal_glyph (આ, user_data,
				       unicode, glyph,
				       !klass->user_data ? nullptr : klass->user_data->nominal_glyph);
  }
  unsigned પૂર્ણાંક get_nominal_glyphs (unsigned પૂર્ણાંક ગણતરી,
				   અચલ hb_codepoint_t *first_unicode,
				   unsigned પૂર્ણાંક unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned પૂર્ણાંક glyph_stride)
  {
    પાછા klass->get.f.nominal_glyphs (આ, user_data,
					ગણતરી,
					first_unicode, unicode_stride,
					first_glyph, glyph_stride,
					!klass->user_data ? nullptr : klass->user_data->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    પાછા klass->get.f.variation_glyph (આ, user_data,
					 unicode, variation_selector, glyph,
					 !klass->user_data ? nullptr : klass->user_data->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     તાર્કિક synthetic = સાચું)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (આ, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_h_advance);

    જો (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    પાછા advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     તાર્કિક synthetic = સાચું)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (આ, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_v_advance);

    જો (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    પાછા advance;
  }

  શૂન્ય get_glyph_h_advances (unsigned પૂર્ણાંક ગણતરી,
			     અચલ hb_codepoint_t *first_glyph,
			     unsigned પૂર્ણાંક glyph_stride,
			     hb_position_t *first_advance,
			     unsigned પૂર્ણાંક advance_stride,
			     તાર્કિક synthetic = સાચું)
  {
    klass->get.f.glyph_h_advances (આ, user_data,
				   ગણતરી,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_h_advances);

    જો (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  શૂન્ય get_glyph_v_advances (unsigned પૂર્ણાંક ગણતરી,
			     અચલ hb_codepoint_t *first_glyph,
			     unsigned પૂર્ણાંક glyph_stride,
			     hb_position_t *first_advance,
			     unsigned પૂર્ણાંક advance_stride,
			     તાર્કિક synthetic = સાચું)
  {
    klass->get.f.glyph_v_advances (આ, user_data,
				   ગણતરી,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_v_advances);

    જો (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				તાર્કિક synthetic = સાચું)
  {
    *x = *y = 0;
    તાર્કિક ret = klass->get.f.glyph_h_origin (આ, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_h_origin);

    જો (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      જો (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    પાછા ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				તાર્કિક synthetic = સાચું)
  {
    *x = *y = 0;
    તાર્કિક ret = klass->get.f.glyph_v_origin (આ, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_v_origin);

    જો (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      જો (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    પાછા ret;
  }

  hb_bool_t get_glyph_h_origins (unsigned પૂર્ણાંક ગણતરી,
				 અચલ hb_codepoint_t *first_glyph,
				 unsigned પૂર્ણાંક glyph_stride,
				 hb_position_t *first_x,
				 unsigned પૂર્ણાંક x_stride,
				 hb_position_t *first_y,
				 unsigned પૂર્ણાંક y_stride,
				 તાર્કિક synthetic = સાચું)

  {
    તાર્કિક ret = klass->get.f.glyph_h_origins (આ, user_data,
					     ગણતરી,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_h_origins);

    જો (synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      માટે (unsigned i = 0; i < ગણતરી; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	જો (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    પાછા ret;
  }

  hb_bool_t get_glyph_v_origins (unsigned પૂર્ણાંક ગણતરી,
				 અચલ hb_codepoint_t *first_glyph,
				 unsigned પૂર્ણાંક glyph_stride,
				 hb_position_t *first_x,
				 unsigned પૂર્ણાંક x_stride,
				 hb_position_t *first_y,
				 unsigned પૂર્ણાંક y_stride,
				 તાર્કિક synthetic = સાચું)

  {
    તાર્કિક ret = klass->get.f.glyph_v_origins (આ, user_data,
					     ગણતરી,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_v_origins);

    જો (synthetic && is_synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      માટે (unsigned i = 0; i < ગણતરી; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	જો (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    પાછા ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t ડાબું_glyph,
				     hb_codepoint_t જમણું_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    પાછા 0;
#નહીંતર
    પાછા klass->get.f.glyph_h_kerning (આ, user_data,
					 ડાબું_glyph, જમણું_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_h_kerning);
#endif
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t ટોચ_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    પાછા 0;
#નહીંતર
    પાછા klass->get.f.glyph_v_kerning (આ, user_data,
					 ટોચ_glyph, bottom_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_v_kerning);
#endif
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       તાર્કિક synthetic = સાચું)
  {
    hb_memset (extents, 0, કદ (*extents));

    /* This is rather messy, but necessary. */

    જો (!synthetic)
    {
      પાછા klass->get.f.glyph_extents (આ, user_data,
					 glyph,
					 extents,
					 !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    }
    જો (!is_synthetic &&
	klass->get.f.glyph_extents (આ, user_data,
				    glyph,
				    extents,
				    !klass->user_data ? nullptr : klass->user_data->glyph_extents))
      પાછા સાચું;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#ifndef HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    જો (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().to_glyph_extents ();
      પાછા સાચું;
    }
#endif

#ifndef HB_NO_DRAW
    hb_extents_t<> draw_extents;
    જો (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.to_glyph_extents ();
      પાછા સાચું;
    }
#endif

    તાર્કિક ret = klass->get.f.glyph_extents (આ, user_data,
					   glyph,
					   extents,
					   !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    જો (ret)
      synthetic_glyph_extents (extents);

    પાછા ret;
  }

  hb_bool_t get_glyph_contour_point (hb_codepoint_t glyph, unsigned પૂર્ણાંક point_index,
				     hb_position_t *x, hb_position_t *y,
				     તાર્કિક synthetic = સાચું)
  {
    *x = *y = 0;
    તાર્કિક ret = klass->get.f.glyph_contour_point (આ, user_data,
						 glyph, point_index,
						 x, y,
						 !klass->user_data ? nullptr : klass->user_data->glyph_contour_point);

    જો (synthetic && ret)
    {
      /* Slant */
      જો (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      જો (!embolden_in_place)
      {
	પૂર્ણાંક x_shift = x_scale < 0 ? -x_strength : x_strength;
	*x += x_shift;
      }
    }

    પાછા ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    અક્ષર *name, unsigned પૂર્ણાંક size)
  {
    જો (size) *name = '\0';
    પાછા klass->get.f.glyph_name (આ, user_data,
				    glyph,
				    name, size,
				    !klass->user_data ? nullptr : klass->user_data->glyph_name);
  }

  hb_bool_t get_glyph_from_name (અચલ અક્ષર *name, પૂર્ણાંક len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    જો (len == -1) len = strlen (name);
    પાછા klass->get.f.glyph_from_name (આ, user_data,
					 name, len,
					 glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_from_name);
  }

  તાર્કિક draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, શૂન્ય *draw_data,
			   તાર્કિક synthetic = સાચું)
  {
#ifndef HB_NO_DRAW
#ifndef HB_NO_OUTLINE
    તાર્કિક embolden = x_strength || y_strength;
    તાર્કિક slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#નહીંતર
    synthetic = ખોટું;
#endif

    જો (!synthetic)
    {
      પાછા klass->get.f.draw_glyph_or_fail (આ, user_data,
					      glyph,
					      draw_funcs, draw_data,
					      !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail);
    }

#ifndef HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_remaining = draw_funcs->get_budget_remaining_ptr (draw_data);
    જો (!klass->get.f.draw_glyph_or_fail (આ, user_data,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail))
      પાછા ખોટું;

    // Slant before embolden; produces nicer results.

    જો (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, ખોટું);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    જો (embolden)
    {
      દશાંશ x_shift = embolden_in_place ? 0 : (દશાંશ) x_strength / 2;
      દશાંશ y_shift = (દશાંશ) y_strength / 2;
      જો (x_scale < 0) x_shift = -x_shift;
      જો (y_scale < 0) y_shift = -y_shift;
      outline.embolden (x_strength, y_strength, x_shift, y_shift);
    }

    outline.replay (draw_funcs, draw_data);

    પાછા સાચું;
#endif
#endif
    પાછા ખોટું;
  }

  તાર્કિક paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, શૂન્ય *paint_data,
			    unsigned પૂર્ણાંક palette,
			    hb_color_t foreground,
			    તાર્કિક synthetic = સાચું)
  {
#ifndef HB_NO_PAINT
    /* Slant */
    જો (synthetic && slant_xy)
      hb_paint_ધકેલો_transform (paint_funcs, paint_data,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    તાર્કિક ret = klass->get.f.paint_glyph_or_fail (આ, user_data,
						 glyph,
						 paint_funcs, paint_data,
						 palette, foreground,
						 !klass->user_data ? nullptr : klass->user_data->paint_glyph_or_fail);

    જો (synthetic && slant_xy)
      hb_paint_કાઢો_transform (paint_funcs, paint_data);

    પાછા ret;
#endif
    પાછા ખોટું;
  }

  /* A bit higher-level, and with fallback */

  HB_INTERNAL
  શૂન્ય paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, શૂન્ય *paint_data,
		    unsigned પૂર્ણાંક palette,
		    hb_color_t foreground);

  શૂન્ય get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    જો (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->લીટી_gap = 0;
    }
  }
  શૂન્ય get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    જો (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->લીટી_gap = 0;
    }
  }

  શૂન્ય get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    જો (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    નહીંતર
      get_v_extents_with_fallback (extents);
  }

  શૂન્ય get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    જો (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    નહીંતર
      *y = get_glyph_v_advance (glyph);
  }
  શૂન્ય get_glyph_advances_for_direction (hb_direction_t direction,
					 unsigned પૂર્ણાંક ગણતરી,
					 અચલ hb_codepoint_t *first_glyph,
					 unsigned glyph_stride,
					 hb_position_t *first_advance,
					 unsigned advance_stride)
  {
    જો (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (ગણતરી, first_glyph, glyph_stride, first_advance, advance_stride);
    નહીંતર
      get_glyph_v_advances (ગણતરી, first_glyph, glyph_stride, first_advance, advance_stride);
  }

  શૂન્ય apply_offset (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     signed mult)
  {
    પ્રતિજ્ઞા (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  શૂન્ય add_offset (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  શૂન્ય subtract_offset (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  શૂન્ય guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  શૂન્ય apply_glyph_h_origins_with_fallback (hb_buffer_t *buf, પૂર્ણાંક mult)
  {
    તાર્કિક has_ascender = ખોટું;
    hb_position_t ascender = 0;

    સંરચના { hb_position_t x, y; } origins[32];

    unsigned પૂર્ણાંક offset = 0;
    unsigned પૂર્ણાંક ગણતરી = buf->len;
    જ્યારે (offset < ગણતરી)
    {
      unsigned n = hb_min (ગણતરી - offset, ARRAY_LENGTH (origins));
      જો (!get_glyph_h_origins (n,
				&buf->info[offset].codepoint, કદ (hb_glyph_info_t),
				&origins[0].x, કદ (origins[0]),
				&origins[0].y, કદ (origins[0])))
      {
        જો (get_glyph_v_origins (n,
				  &buf->info[offset].codepoint, કદ (hb_glyph_info_t),
				  &origins[0].x, કદ (origins[0]),
				  &origins[0].y, કદ (origins[0])))
	{
	  જો (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = સાચું;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  માટે (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	નહીંતર
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      પ્રતિજ્ઞા (mult == -1 || mult == +1 || mult == 0);
      જો (mult == +1)
        માટે (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      નહીંતર જો (mult == -1)
	માટે (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }
  શૂન્ય apply_glyph_v_origins_with_fallback (hb_buffer_t *buf, પૂર્ણાંક mult)
  {
    તાર્કિક has_ascender = ખોટું;
    hb_position_t ascender = 0;

    સંરચના { hb_position_t x, y; } origins[32];

    unsigned પૂર્ણાંક offset = 0;
    unsigned પૂર્ણાંક ગણતરી = buf->len;
    જ્યારે (offset < ગણતરી)
    {
      unsigned n = hb_min (ગણતરી - offset, ARRAY_LENGTH (origins));
      જો (!get_glyph_v_origins (n,
				&buf->info[offset].codepoint, કદ (hb_glyph_info_t),
				&origins[0].x, કદ (origins[0]),
				&origins[0].y, કદ (origins[0])))
      {
	જો (get_glyph_h_origins (n,
				 &buf->info[offset].codepoint, કદ (hb_glyph_info_t),
				 &origins[0].x, કદ (origins[0]),
				 &origins[0].y, કદ (origins[0])))
	{
	  જો (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = સાચું;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  માટે (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	નહીંતર
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      પ્રતિજ્ઞા (mult == -1 || mult == +1 || mult == 0);
      જો (mult == +1)
        માટે (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      નહીંતર જો (mult == -1)
	માટે (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }

  શૂન્ય get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    જો (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      subtract_offset (x, y, dx, dy);
    }
  }
  શૂન્ય get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    જો (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      add_offset (x, y, dx, dy);
    }
  }

  શૂન્ય get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    જો (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    નહીંતર
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  શૂન્ય add_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  શૂન્ય add_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  શૂન્ય add_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    add_offset (x, y, origin_x, origin_y);
  }

  શૂન્ય subtract_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  શૂન્ય subtract_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  શૂન્ય subtract_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    subtract_offset (x, y, origin_x, origin_y);
  }

  શૂન્ય get_glyph_kerning_for_direction (hb_codepoint_t first_glyph, hb_codepoint_t second_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    જો (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (first_glyph, second_glyph);
    } નહીંતર {
      *x = 0;
      *y = get_glyph_v_kerning (first_glyph, second_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    જો (ret)
      subtract_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    પાછા ret;
  }

  hb_bool_t get_glyph_contour_point_for_origin (hb_codepoint_t glyph, unsigned પૂર્ણાંક point_index,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_point (glyph, point_index, x, y);

    જો (ret)
      subtract_glyph_origin_for_direction (glyph, direction, x, y);

    પાછા ret;
  }

  /* Generates gidDDD if glyph has no name. */
  શૂન્ય
  glyph_to_string (hb_codepoint_t glyph,
		   અક્ષર *s, unsigned પૂર્ણાંક size)
  {
    જો (get_glyph_name (glyph, s, size)) પાછા;

    જો (size && snprintf (s, size, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (અચલ અક્ષર *s, પૂર્ણાંક len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    જો (get_glyph_from_name (s, len, glyph)) પાછા સાચું;

    જો (len == -1) len = strlen (s);

    /* Straight glyph index. */
    જો (hb_codepoint_parse (s, len, 10, glyph))
      પાછા સાચું;

    જો (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      જો (0 == strncmp (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	પાછા સાચું;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      જો (0 == strncmp (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	પાછા સાચું;
    }

    પાછા ખોટું;
  }

  શૂન્ય changed ()
  {
    દશાંશ upem = face->get_upem ();

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
  { પાછા (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (દશાંશ v, દશાંશ mult)
  { પાછા (hb_position_t) roundf (em_fmultf (v, mult)); }
  દશાંશ em_fmultf (દશાંશ v, દશાંશ mult)
  { પાછા v * mult; }
  દશાંશ em_fmult (int32_t v, દશાંશ mult)
  { પાછા (દશાંશ) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#endif /* HB_FONT_HH */
