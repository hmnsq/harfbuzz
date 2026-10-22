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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-face.hh"
#अन्तर्भाव "hb-atomic.hh"
#अन्तर्भाव "hb-draw.hh"
#अन्तर्भाव "hb-paint-extents.hh"
#अन्तर्भाव "hb-shaper.hh"
#अन्तर्भाव "hb-outline.hh"


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

संरचना hb_font_funcs_t
{
  hb_object_header_t header;

  संरचना {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) शून्य *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *user_data;

  संरचना {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  सङ्घ get_t {
    संरचना get_funcs_t {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
    } f;
    शून्य (*array[0
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
#अन्तर्भाव "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

संरचना hb_font_t
{
  hb_object_header_t header;
  hb_atomic_t<अचिन्हित> serial;
  hb_atomic_t<अचिन्हित> serial_coords;

  hb_font_t *parent;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  तार्किक is_synthetic;

  भिन्न x_embolden;
  भिन्न y_embolden;
  तार्किक embolden_in_place;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  भिन्न slant;
  भिन्न slant_xy;

  भिन्न x_multf;
  भिन्न y_multf;
  int64_t x_mult;
  int64_t y_mult;

  अचिन्हित पूर्णाङ्क x_ppem;
  अचिन्हित पूर्णाङ्क y_ppem;

  भिन्न ptem;

  /* Font variation coordinates. */
  अचिन्हित पूर्णाङ्क instance_index;
  तार्किक has_nonzero_coords;
  अचिन्हित पूर्णाङ्क अङ्क_coords;
  पूर्णाङ्क *coords;
  भिन्न *design_coords;

  hb_font_funcs_t   *klass;
  शून्य              *user_data;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> data; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { फर्काऊ HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { फर्काऊ em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { फर्काऊ em_mult (v, y_mult); }
  hb_position_t em_scalef_x (भिन्न v) { फर्काऊ em_multf (v, x_multf); }
  hb_position_t em_scalef_y (भिन्न v) { फर्काऊ em_multf (v, y_multf); }
  भिन्न em_fscale_x (int32_t v) { फर्काऊ em_fmult (v, x_multf); }
  भिन्न em_fscale_y (int32_t v) { फर्काऊ em_fmult (v, y_multf); }
  भिन्न em_fscalef_x (भिन्न v) { फर्काऊ em_fmultf (v, x_multf); }
  भिन्न em_fscalef_y (भिन्न v) { फर्काऊ em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { फर्काऊ em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t parent_scale_x_distance (hb_position_t v)
  {
    यदि (unlikely (parent && parent->x_scale && parent->x_scale != x_scale))
      फर्काऊ (hb_position_t) (v * (int64_t) एतत्->x_scale / एतत्->parent->x_scale);
    फर्काऊ v;
  }
  hb_position_t parent_scale_y_distance (hb_position_t v)
  {
    यदि (unlikely (parent && parent->y_scale && parent->y_scale != y_scale))
      फर्काऊ (hb_position_t) (v * (int64_t) एतत्->y_scale / एतत्->parent->y_scale);
    फर्काऊ v;
  }
  hb_position_t parent_scale_x_position (hb_position_t v)
  { फर्काऊ parent_scale_x_distance (v); }
  hb_position_t parent_scale_y_position (hb_position_t v)
  { फर्काऊ parent_scale_y_distance (v); }

  शून्य parent_scale_distance (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_distance (*x);
    *y = parent_scale_y_distance (*y);
  }
  शून्य parent_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_position (*x);
    *y = parent_scale_y_position (*y);
  }

  शून्य scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    भिन्न x1 = em_fscale_x (extents->x_bearing);
    भिन्न y1 = em_fscale_y (extents->y_bearing);
    भिन्न x2 = x1 + em_fscale_x (extents->width);
    भिन्न y2 = y1 + em_fscale_y (extents->height);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->width = ceilf (x2) - extents->x_bearing;
    extents->height = ceilf (y2) - extents->y_bearing;
  }

  शून्य synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    यदि (slant_xy)
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
    यदि (x_strength || y_strength)
    {
      /* Y */
      पूर्णाङ्क y_shift = y_strength;
      यदि (y_scale < 0) y_shift = -y_shift;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->y_bearing + y_shift);
      extents->height = hb_clamp_to<hb_position_t> ((int64_t) extents->height - y_shift);

      /* X */
      पूर्णाङ्क x_shift = x_strength;
      यदि (x_scale < 0) x_shift = -x_shift;
      यदि (embolden_in_place)
	extents->x_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->x_bearing - x_shift / 2);
      extents->width = hb_clamp_to<hb_position_t> ((int64_t) extents->width + x_shift);
    }
  }


  /* Public getters */

  HB_INTERNAL तार्किक has_func (अचिन्हित पूर्णाङ्क i);
  HB_INTERNAL तार्किक has_func_set (अचिन्हित पूर्णाङ्क i);

  /* has_* ... */
#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
  तार्किक \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = एतत्->klass; \
    अचिन्हित पूर्णाङ्क i = दुरत्व (hb_font_funcs_t::get_t::get_funcs_t, name) / परिमाण (funcs->get.array[0]); \
    फर्काऊ has_func (i); \
  } \
  तार्किक \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = एतत्->klass; \
    अचिन्हित पूर्णाङ्क i = दुरत्व (hb_font_funcs_t::get_t::get_funcs_t, name) / परिमाण (funcs->get.array[0]); \
    फर्काऊ has_func_set (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				तार्किक synthetic = सत्य)
  {
    hb_memset (extents, 0, परिमाण (*extents));
    तार्किक ret = klass->get.f.font_h_extents (एतत्, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_h_extents);

    यदि (synthetic && ret)
    {
      /* Embolden */
      पूर्णाङ्क y_shift = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_shift;
    }

    फर्काऊ ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				तार्किक synthetic = सत्य)
  {
    hb_memset (extents, 0, परिमाण (*extents));
    तार्किक ret = klass->get.f.font_v_extents (एतत्, user_data,
					    extents,
					    !klass->user_data ? nullptr : klass->user_data->font_v_extents);

    यदि (synthetic && ret)
    {
      /* Embolden */
      पूर्णाङ्क x_shift = x_scale < 0 ? -x_strength : x_strength;
      यदि (embolden_in_place)
      {
	extents->ascender += x_shift / 2;
	extents->descender -= x_shift - x_shift / 2;
      }
      अन्यथा
	extents->ascender += x_shift;
    }

    फर्काऊ ret;
  }

  तार्किक has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    फर्काऊ get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    फर्काऊ klass->get.f.nominal_glyph (एतत्, user_data,
				       unicode, glyph,
				       !klass->user_data ? nullptr : klass->user_data->nominal_glyph);
  }
  अचिन्हित पूर्णाङ्क get_nominal_glyphs (अचिन्हित पूर्णाङ्क गन्ती,
				   अचर hb_codepoint_t *first_unicode,
				   अचिन्हित पूर्णाङ्क unicode_stride,
				   hb_codepoint_t *first_glyph,
				   अचिन्हित पूर्णाङ्क glyph_stride)
  {
    फर्काऊ klass->get.f.nominal_glyphs (एतत्, user_data,
					गन्ती,
					first_unicode, unicode_stride,
					first_glyph, glyph_stride,
					!klass->user_data ? nullptr : klass->user_data->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    फर्काऊ klass->get.f.variation_glyph (एतत्, user_data,
					 unicode, variation_selector, glyph,
					 !klass->user_data ? nullptr : klass->user_data->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     तार्किक synthetic = सत्य)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (एतत्, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_h_advance);

    यदि (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    फर्काऊ advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     तार्किक synthetic = सत्य)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (एतत्, user_data,
							  glyph,
							  !klass->user_data ? nullptr : klass->user_data->glyph_v_advance);

    यदि (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    फर्काऊ advance;
  }

  शून्य get_glyph_h_advances (अचिन्हित पूर्णाङ्क गन्ती,
			     अचर hb_codepoint_t *first_glyph,
			     अचिन्हित पूर्णाङ्क glyph_stride,
			     hb_position_t *first_advance,
			     अचिन्हित पूर्णाङ्क advance_stride,
			     तार्किक synthetic = सत्य)
  {
    klass->get.f.glyph_h_advances (एतत्, user_data,
				   गन्ती,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_h_advances);

    यदि (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  शून्य get_glyph_v_advances (अचिन्हित पूर्णाङ्क गन्ती,
			     अचर hb_codepoint_t *first_glyph,
			     अचिन्हित पूर्णाङ्क glyph_stride,
			     hb_position_t *first_advance,
			     अचिन्हित पूर्णाङ्क advance_stride,
			     तार्किक synthetic = सत्य)
  {
    klass->get.f.glyph_v_advances (एतत्, user_data,
				   गन्ती,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_data ? nullptr : klass->user_data->glyph_v_advances);

    यदि (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				तार्किक synthetic = सत्य)
  {
    *x = *y = 0;
    तार्किक ret = klass->get.f.glyph_h_origin (एतत्, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_h_origin);

    यदि (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      यदि (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    फर्काऊ ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				तार्किक synthetic = सत्य)
  {
    *x = *y = 0;
    तार्किक ret = klass->get.f.glyph_v_origin (एतत्, user_data,
					    glyph, x, y,
					    !klass->user_data ? nullptr : klass->user_data->glyph_v_origin);

    यदि (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      यदि (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    फर्काऊ ret;
  }

  hb_bool_t get_glyph_h_origins (अचिन्हित पूर्णाङ्क गन्ती,
				 अचर hb_codepoint_t *first_glyph,
				 अचिन्हित पूर्णाङ्क glyph_stride,
				 hb_position_t *first_x,
				 अचिन्हित पूर्णाङ्क x_stride,
				 hb_position_t *first_y,
				 अचिन्हित पूर्णाङ्क y_stride,
				 तार्किक synthetic = सत्य)

  {
    तार्किक ret = klass->get.f.glyph_h_origins (एतत्, user_data,
					     गन्ती,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_h_origins);

    यदि (synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      लागि (अचिन्हित i = 0; i < गन्ती; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	यदि (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    फर्काऊ ret;
  }

  hb_bool_t get_glyph_v_origins (अचिन्हित पूर्णाङ्क गन्ती,
				 अचर hb_codepoint_t *first_glyph,
				 अचिन्हित पूर्णाङ्क glyph_stride,
				 hb_position_t *first_x,
				 अचिन्हित पूर्णाङ्क x_stride,
				 hb_position_t *first_y,
				 अचिन्हित पूर्णाङ्क y_stride,
				 तार्किक synthetic = सत्य)

  {
    तार्किक ret = klass->get.f.glyph_v_origins (एतत्, user_data,
					     गन्ती,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_data ? nullptr : klass->user_data->glyph_v_origins);

    यदि (synthetic && is_synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      लागि (अचिन्हित i = 0; i < गन्ती; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	यदि (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    फर्काऊ ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t बायाँ_glyph,
				     hb_codepoint_t दायाँ_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    फर्काऊ 0;
#अन्यथा
    फर्काऊ klass->get.f.glyph_h_kerning (एतत्, user_data,
					 बायाँ_glyph, दायाँ_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_h_kerning);
#endif
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t शीर्ष_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    फर्काऊ 0;
#अन्यथा
    फर्काऊ klass->get.f.glyph_v_kerning (एतत्, user_data,
					 शीर्ष_glyph, bottom_glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_v_kerning);
#endif
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       तार्किक synthetic = सत्य)
  {
    hb_memset (extents, 0, परिमाण (*extents));

    /* This is rather messy, but necessary. */

    यदि (!synthetic)
    {
      फर्काऊ klass->get.f.glyph_extents (एतत्, user_data,
					 glyph,
					 extents,
					 !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    }
    यदि (!is_synthetic &&
	klass->get.f.glyph_extents (एतत्, user_data,
				    glyph,
				    extents,
				    !klass->user_data ? nullptr : klass->user_data->glyph_extents))
      फर्काऊ सत्य;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#ifndef HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    यदि (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().to_glyph_extents ();
      फर्काऊ सत्य;
    }
#endif

#ifndef HB_NO_DRAW
    hb_extents_t<> draw_extents;
    यदि (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.to_glyph_extents ();
      फर्काऊ सत्य;
    }
#endif

    तार्किक ret = klass->get.f.glyph_extents (एतत्, user_data,
					   glyph,
					   extents,
					   !klass->user_data ? nullptr : klass->user_data->glyph_extents);
    यदि (ret)
      synthetic_glyph_extents (extents);

    फर्काऊ ret;
  }

  hb_bool_t get_glyph_contour_point (hb_codepoint_t glyph, अचिन्हित पूर्णाङ्क point_index,
				     hb_position_t *x, hb_position_t *y,
				     तार्किक synthetic = सत्य)
  {
    *x = *y = 0;
    तार्किक ret = klass->get.f.glyph_contour_point (एतत्, user_data,
						 glyph, point_index,
						 x, y,
						 !klass->user_data ? nullptr : klass->user_data->glyph_contour_point);

    यदि (synthetic && ret)
    {
      /* Slant */
      यदि (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      यदि (!embolden_in_place)
      {
	पूर्णाङ्क x_shift = x_scale < 0 ? -x_strength : x_strength;
	*x += x_shift;
      }
    }

    फर्काऊ ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    वर्ण *name, अचिन्हित पूर्णाङ्क size)
  {
    यदि (size) *name = '\0';
    फर्काऊ klass->get.f.glyph_name (एतत्, user_data,
				    glyph,
				    name, size,
				    !klass->user_data ? nullptr : klass->user_data->glyph_name);
  }

  hb_bool_t get_glyph_from_name (अचर वर्ण *name, पूर्णाङ्क len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    यदि (len == -1) len = म_माप (name);
    फर्काऊ klass->get.f.glyph_from_name (एतत्, user_data,
					 name, len,
					 glyph,
					 !klass->user_data ? nullptr : klass->user_data->glyph_from_name);
  }

  तार्किक draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, शून्य *draw_data,
			   तार्किक synthetic = सत्य)
  {
#ifndef HB_NO_DRAW
#ifndef HB_NO_OUTLINE
    तार्किक embolden = x_strength || y_strength;
    तार्किक slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#अन्यथा
    synthetic = असत्य;
#endif

    यदि (!synthetic)
    {
      फर्काऊ klass->get.f.draw_glyph_or_fail (एतत्, user_data,
					      glyph,
					      draw_funcs, draw_data,
					      !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail);
    }

#ifndef HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_remaining = draw_funcs->get_budget_remaining_ptr (draw_data);
    यदि (!klass->get.f.draw_glyph_or_fail (एतत्, user_data,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_data ? nullptr : klass->user_data->draw_glyph_or_fail))
      फर्काऊ असत्य;

    // Slant before embolden; produces nicer results.

    यदि (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, असत्य);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    यदि (embolden)
    {
      भिन्न x_shift = embolden_in_place ? 0 : (भिन्न) x_strength / 2;
      भिन्न y_shift = (भिन्न) y_strength / 2;
      यदि (x_scale < 0) x_shift = -x_shift;
      यदि (y_scale < 0) y_shift = -y_shift;
      outline.embolden (x_strength, y_strength, x_shift, y_shift);
    }

    outline.replay (draw_funcs, draw_data);

    फर्काऊ सत्य;
#endif
#endif
    फर्काऊ असत्य;
  }

  तार्किक paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, शून्य *paint_data,
			    अचिन्हित पूर्णाङ्क palette,
			    hb_color_t foreground,
			    तार्किक synthetic = सत्य)
  {
#ifndef HB_NO_PAINT
    /* Slant */
    यदि (synthetic && slant_xy)
      hb_paint_थप_transform (paint_funcs, paint_data,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    तार्किक ret = klass->get.f.paint_glyph_or_fail (एतत्, user_data,
						 glyph,
						 paint_funcs, paint_data,
						 palette, foreground,
						 !klass->user_data ? nullptr : klass->user_data->paint_glyph_or_fail);

    यदि (synthetic && slant_xy)
      hb_paint_झिक_transform (paint_funcs, paint_data);

    फर्काऊ ret;
#endif
    फर्काऊ असत्य;
  }

  /* A bit higher-level, and with fallback */

  HB_INTERNAL
  शून्य paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, शून्य *paint_data,
		    अचिन्हित पूर्णाङ्क palette,
		    hb_color_t foreground);

  शून्य get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    यदि (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->पङ्क्ति_gap = 0;
    }
  }
  शून्य get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    यदि (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->पङ्क्ति_gap = 0;
    }
  }

  शून्य get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    यदि (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    अन्यथा
      get_v_extents_with_fallback (extents);
  }

  शून्य get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    यदि (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    अन्यथा
      *y = get_glyph_v_advance (glyph);
  }
  शून्य get_glyph_advances_for_direction (hb_direction_t direction,
					 अचिन्हित पूर्णाङ्क गन्ती,
					 अचर hb_codepoint_t *first_glyph,
					 अचिन्हित glyph_stride,
					 hb_position_t *first_advance,
					 अचिन्हित advance_stride)
  {
    यदि (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (गन्ती, first_glyph, glyph_stride, first_advance, advance_stride);
    अन्यथा
      get_glyph_v_advances (गन्ती, first_glyph, glyph_stride, first_advance, advance_stride);
  }

  शून्य apply_offset (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     चिन्हित mult)
  {
    प्रतिज्ञा (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  शून्य add_offset (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  शून्य subtract_offset (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  शून्य guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  शून्य apply_glyph_h_origins_with_fallback (hb_buffer_t *buf, पूर्णाङ्क mult)
  {
    तार्किक has_ascender = असत्य;
    hb_position_t ascender = 0;

    संरचना { hb_position_t x, y; } origins[32];

    अचिन्हित पूर्णाङ्क offset = 0;
    अचिन्हित पूर्णाङ्क गन्ती = buf->len;
    जबसम्म (offset < गन्ती)
    {
      अचिन्हित n = hb_min (गन्ती - offset, ARRAY_LENGTH (origins));
      यदि (!get_glyph_h_origins (n,
				&buf->info[offset].codepoint, परिमाण (hb_glyph_info_t),
				&origins[0].x, परिमाण (origins[0]),
				&origins[0].y, परिमाण (origins[0])))
      {
        यदि (get_glyph_v_origins (n,
				  &buf->info[offset].codepoint, परिमाण (hb_glyph_info_t),
				  &origins[0].x, परिमाण (origins[0]),
				  &origins[0].y, परिमाण (origins[0])))
	{
	  यदि (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = सत्य;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  लागि (अचिन्हित j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	अन्यथा
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      प्रतिज्ञा (mult == -1 || mult == +1 || mult == 0);
      यदि (mult == +1)
        लागि (अचिन्हित j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      अन्यथा यदि (mult == -1)
	लागि (अचिन्हित j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }
  शून्य apply_glyph_v_origins_with_fallback (hb_buffer_t *buf, पूर्णाङ्क mult)
  {
    तार्किक has_ascender = असत्य;
    hb_position_t ascender = 0;

    संरचना { hb_position_t x, y; } origins[32];

    अचिन्हित पूर्णाङ्क offset = 0;
    अचिन्हित पूर्णाङ्क गन्ती = buf->len;
    जबसम्म (offset < गन्ती)
    {
      अचिन्हित n = hb_min (गन्ती - offset, ARRAY_LENGTH (origins));
      यदि (!get_glyph_v_origins (n,
				&buf->info[offset].codepoint, परिमाण (hb_glyph_info_t),
				&origins[0].x, परिमाण (origins[0]),
				&origins[0].y, परिमाण (origins[0])))
      {
	यदि (get_glyph_h_origins (n,
				 &buf->info[offset].codepoint, परिमाण (hb_glyph_info_t),
				 &origins[0].x, परिमाण (origins[0]),
				 &origins[0].y, परिमाण (origins[0])))
	{
	  यदि (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = सत्य;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  लागि (अचिन्हित j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	अन्यथा
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      प्रतिज्ञा (mult == -1 || mult == +1 || mult == 0);
      यदि (mult == +1)
        लागि (अचिन्हित j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      अन्यथा यदि (mult == -1)
	लागि (अचिन्हित j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }

  शून्य get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    यदि (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      subtract_offset (x, y, dx, dy);
    }
  }
  शून्य get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    यदि (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      add_offset (x, y, dx, dy);
    }
  }

  शून्य get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    यदि (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    अन्यथा
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  शून्य add_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  शून्य add_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  शून्य add_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    add_offset (x, y, origin_x, origin_y);
  }

  शून्य subtract_glyph_h_origins (hb_buffer_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  शून्य subtract_glyph_v_origins (hb_buffer_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  शून्य subtract_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    subtract_offset (x, y, origin_x, origin_y);
  }

  शून्य get_glyph_kerning_for_direction (hb_codepoint_t first_glyph, hb_codepoint_t second_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    यदि (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (first_glyph, second_glyph);
    } अन्यथा {
      *x = 0;
      *y = get_glyph_v_kerning (first_glyph, second_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    यदि (ret)
      subtract_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    फर्काऊ ret;
  }

  hb_bool_t get_glyph_contour_point_for_origin (hb_codepoint_t glyph, अचिन्हित पूर्णाङ्क point_index,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_point (glyph, point_index, x, y);

    यदि (ret)
      subtract_glyph_origin_for_direction (glyph, direction, x, y);

    फर्काऊ ret;
  }

  /* Generates gidDDD if glyph has no name. */
  शून्य
  glyph_to_string (hb_codepoint_t glyph,
		   वर्ण *s, अचिन्हित पूर्णाङ्क size)
  {
    यदि (get_glyph_name (glyph, s, size)) फर्काऊ;

    यदि (size && snprintf (s, size, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (अचर वर्ण *s, पूर्णाङ्क len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    यदि (get_glyph_from_name (s, len, glyph)) फर्काऊ सत्य;

    यदि (len == -1) len = म_माप (s);

    /* Straight glyph index. */
    यदि (hb_codepoint_parse (s, len, 10, glyph))
      फर्काऊ सत्य;

    यदि (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      यदि (0 == म_भेदन (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	फर्काऊ सत्य;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      यदि (0 == म_भेदन (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	फर्काऊ सत्य;
    }

    फर्काऊ असत्य;
  }

  शून्य changed ()
  {
    भिन्न upem = face->get_upem ();

    x_multf = x_scale / upem;
    y_multf = y_scale / upem;
    x_mult = (int64_t) x_scale * 0x10000 / upem;
    y_mult = (int64_t) y_scale * 0x10000 / upem;

    is_synthetic =  x_embolden || y_embolden || slant;

    x_strength = roundf (असल (x_scale) * x_embolden);
    y_strength = roundf (असल (y_scale) * y_embolden);

    slant_xy = y_scale ? slant * x_scale / y_scale : 0.f;

    data.fini ();

    serial++;
  }

  hb_position_t em_mult (int32_t v, int64_t mult)
  { फर्काऊ (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (भिन्न v, भिन्न mult)
  { फर्काऊ (hb_position_t) roundf (em_fmultf (v, mult)); }
  भिन्न em_fmultf (भिन्न v, भिन्न mult)
  { फर्काऊ v * mult; }
  भिन्न em_fmult (int32_t v, भिन्न mult)
  { फर्काऊ (भिन्न) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#endif /* HB_FONT_HH */
