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

#अगर_अघोषित HB_FONT_HH
#घोषणा HB_FONT_HH

#समावेश "hb.hh"

#समावेश "hb-face.hh"
#समावेश "hb-atomic.hh"
#समावेश "hb-draw.hh"
#समावेश "hb-paint-extents.hh"
#समावेश "hb-shaper.hh"
#समावेश "hb-outline.hh"


/*
 * hb_font_funcs_t
 */

#घोषणा HB_FONT_FUNCS_IMPLEMENT_CALLBACKS \
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

काष्ठा hb_font_funcs_t
{
  hb_object_header_t header;

  काष्ठा {
#घोषणा HB_FONT_FUNC_IMPLEMENT(get_,name) व्योम *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_FONT_FUNC_IMPLEMENT
  } *user_तथ्य;

  काष्ठा {
#घोषणा HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  जोड़ get_t {
    काष्ठा get_funcs_t {
#घोषणा HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_FONT_FUNC_IMPLEMENT
    } f;
    व्योम (*array[0
#घोषणा HB_FONT_FUNC_IMPLEMENT(get_,name) +1
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_FONT_FUNC_IMPLEMENT
		]) ();
  } लो;
};
DECLARE_NULL_INSTANCE (hb_font_funcs_t);


/*
 * hb_font_t
 */

#घोषणा HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_तथ्य_INSTANTIATE_SHAPERS(shaper, font);
#समावेश "hb-shaper-list.hh"
#अघोषित HB_SHAPER_IMPLEMENT

काष्ठा hb_font_t
{
  hb_object_header_t header;
  hb_atomic_t<अचिन्हित> serial;
  hb_atomic_t<अचिन्हित> serial_coords;

  hb_font_t *parent;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  bool is_synthetic;

  भग्न x_embolden;
  भग्न y_embolden;
  bool embolden_in_place;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  भग्न slant;
  भग्न slant_xy;

  भग्न x_multf;
  भग्न y_multf;
  int64_t x_mult;
  int64_t y_mult;

  अचिन्हित पूर्णांक x_ppem;
  अचिन्हित पूर्णांक y_ppem;

  भग्न ptem;

  /* Font variation coordinates. */
  अचिन्हित पूर्णांक instance_सूचकांक;
  bool has_nonzero_coords;
  अचिन्हित पूर्णांक अंक_coords;
  पूर्णांक *coords;
  भग्न *design_coords;

  hb_font_funcs_t   *klass;
  व्योम              *user_तथ्य;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> तथ्य; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { वापस HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { वापस em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { वापस em_mult (v, y_mult); }
  hb_position_t em_scalef_x (भग्न v) { वापस em_multf (v, x_multf); }
  hb_position_t em_scalef_y (भग्न v) { वापस em_multf (v, y_multf); }
  भग्न em_fscale_x (int32_t v) { वापस em_fmult (v, x_multf); }
  भग्न em_fscale_y (int32_t v) { वापस em_fmult (v, y_multf); }
  भग्न em_fscalef_x (भग्न v) { वापस em_fmultf (v, x_multf); }
  भग्न em_fscalef_y (भग्न v) { वापस em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { वापस em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t parent_scale_x_distance (hb_position_t v)
  {
    अगर (unlikely (parent && parent->x_scale && parent->x_scale != x_scale))
      वापस (hb_position_t) (v * (int64_t) यह->x_scale / यह->parent->x_scale);
    वापस v;
  }
  hb_position_t parent_scale_y_distance (hb_position_t v)
  {
    अगर (unlikely (parent && parent->y_scale && parent->y_scale != y_scale))
      वापस (hb_position_t) (v * (int64_t) यह->y_scale / यह->parent->y_scale);
    वापस v;
  }
  hb_position_t parent_scale_x_position (hb_position_t v)
  { वापस parent_scale_x_distance (v); }
  hb_position_t parent_scale_y_position (hb_position_t v)
  { वापस parent_scale_y_distance (v); }

  व्योम parent_scale_distance (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_distance (*x);
    *y = parent_scale_y_distance (*y);
  }
  व्योम parent_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_position (*x);
    *y = parent_scale_y_position (*y);
  }

  व्योम scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    भग्न x1 = em_fscale_x (extents->x_bearing);
    भग्न y1 = em_fscale_y (extents->y_bearing);
    भग्न x2 = x1 + em_fscale_x (extents->width);
    भग्न y2 = y1 + em_fscale_y (extents->height);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->width = ceilf (x2) - extents->x_bearing;
    extents->height = ceilf (y2) - extents->y_bearing;
  }

  व्योम synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    अगर (slant_xy)
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
    अगर (x_strength || y_strength)
    {
      /* Y */
      पूर्णांक y_shift = y_strength;
      अगर (y_scale < 0) y_shift = -y_shift;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->y_bearing + y_shift);
      extents->height = hb_clamp_to<hb_position_t> ((int64_t) extents->height - y_shift);

      /* X */
      पूर्णांक x_shift = x_strength;
      अगर (x_scale < 0) x_shift = -x_shift;
      अगर (embolden_in_place)
	extents->x_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->x_bearing - x_shift / 2);
      extents->width = hb_clamp_to<hb_position_t> ((int64_t) extents->width + x_shift);
    }
  }


  /* Public getters */

  HB_INTERNAL bool has_func (अचिन्हित पूर्णांक i);
  HB_INTERNAL bool has_func_set (अचिन्हित पूर्णांक i);

  /* has_* ... */
#घोषणा HB_FONT_FUNC_IMPLEMENT(get_,name) \
  bool \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = यह->klass; \
    अचिन्हित पूर्णांक i = दुरत्व (hb_font_funcs_t::get_t::get_funcs_t, name) / माप (funcs->लो.array[0]); \
    वापस has_func (i); \
  } \
  bool \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = यह->klass; \
    अचिन्हित पूर्णांक i = दुरत्व (hb_font_funcs_t::get_t::get_funcs_t, name) / माप (funcs->लो.array[0]); \
    वापस has_func_set (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				bool synthetic = true)
  {
    hb_memset (extents, 0, माप (*extents));
    bool ret = klass->लो.f.font_h_extents (यह, user_तथ्य,
					    extents,
					    !klass->user_तथ्य ? nullptr : klass->user_तथ्य->font_h_extents);

    अगर (synthetic && ret)
    {
      /* Embolden */
      पूर्णांक y_shift = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_shift;
    }

    वापस ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				bool synthetic = true)
  {
    hb_memset (extents, 0, माप (*extents));
    bool ret = klass->लो.f.font_v_extents (यह, user_तथ्य,
					    extents,
					    !klass->user_तथ्य ? nullptr : klass->user_तथ्य->font_v_extents);

    अगर (synthetic && ret)
    {
      /* Embolden */
      पूर्णांक x_shift = x_scale < 0 ? -x_strength : x_strength;
      अगर (embolden_in_place)
      {
	extents->ascender += x_shift / 2;
	extents->descender -= x_shift - x_shift / 2;
      }
      अन्यथा
	extents->ascender += x_shift;
    }

    वापस ret;
  }

  bool has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    वापस get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    वापस klass->लो.f.nominal_glyph (यह, user_तथ्य,
				       unicode, glyph,
				       !klass->user_तथ्य ? nullptr : klass->user_तथ्य->nominal_glyph);
  }
  अचिन्हित पूर्णांक get_nominal_glyphs (अचिन्हित पूर्णांक गिनती,
				   स्थिर hb_codepoint_t *first_unicode,
				   अचिन्हित पूर्णांक unicode_stride,
				   hb_codepoint_t *first_glyph,
				   अचिन्हित पूर्णांक glyph_stride)
  {
    वापस klass->लो.f.nominal_glyphs (यह, user_तथ्य,
					गिनती,
					first_unicode, unicode_stride,
					first_glyph, glyph_stride,
					!klass->user_तथ्य ? nullptr : klass->user_तथ्य->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    वापस klass->लो.f.variation_glyph (यह, user_तथ्य,
					 unicode, variation_selector, glyph,
					 !klass->user_तथ्य ? nullptr : klass->user_तथ्य->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     bool synthetic = true)
  {
    hb_position_t advance = klass->लो.f.glyph_h_advance (यह, user_तथ्य,
							  glyph,
							  !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_h_advance);

    अगर (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    वापस advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     bool synthetic = true)
  {
    hb_position_t advance = klass->लो.f.glyph_v_advance (यह, user_तथ्य,
							  glyph,
							  !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_v_advance);

    अगर (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    वापस advance;
  }

  व्योम get_glyph_h_advances (अचिन्हित पूर्णांक गिनती,
			     स्थिर hb_codepoint_t *first_glyph,
			     अचिन्हित पूर्णांक glyph_stride,
			     hb_position_t *first_advance,
			     अचिन्हित पूर्णांक advance_stride,
			     bool synthetic = true)
  {
    klass->लो.f.glyph_h_advances (यह, user_तथ्य,
				   गिनती,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_h_advances);

    अगर (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  व्योम get_glyph_v_advances (अचिन्हित पूर्णांक गिनती,
			     स्थिर hb_codepoint_t *first_glyph,
			     अचिन्हित पूर्णांक glyph_stride,
			     hb_position_t *first_advance,
			     अचिन्हित पूर्णांक advance_stride,
			     bool synthetic = true)
  {
    klass->लो.f.glyph_v_advances (यह, user_तथ्य,
				   गिनती,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_v_advances);

    अगर (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				bool synthetic = true)
  {
    *x = *y = 0;
    bool ret = klass->लो.f.glyph_h_origin (यह, user_तथ्य,
					    glyph, x, y,
					    !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_h_origin);

    अगर (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      अगर (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    वापस ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				bool synthetic = true)
  {
    *x = *y = 0;
    bool ret = klass->लो.f.glyph_v_origin (यह, user_तथ्य,
					    glyph, x, y,
					    !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_v_origin);

    अगर (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      अगर (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    वापस ret;
  }

  hb_bool_t get_glyph_h_origins (अचिन्हित पूर्णांक गिनती,
				 स्थिर hb_codepoint_t *first_glyph,
				 अचिन्हित पूर्णांक glyph_stride,
				 hb_position_t *first_x,
				 अचिन्हित पूर्णांक x_stride,
				 hb_position_t *first_y,
				 अचिन्हित पूर्णांक y_stride,
				 bool synthetic = true)

  {
    bool ret = klass->लो.f.glyph_h_origins (यह, user_तथ्य,
					     गिनती,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_h_origins);

    अगर (synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	अगर (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    वापस ret;
  }

  hb_bool_t get_glyph_v_origins (अचिन्हित पूर्णांक गिनती,
				 स्थिर hb_codepoint_t *first_glyph,
				 अचिन्हित पूर्णांक glyph_stride,
				 hb_position_t *first_x,
				 अचिन्हित पूर्णांक x_stride,
				 hb_position_t *first_y,
				 अचिन्हित पूर्णांक y_stride,
				 bool synthetic = true)

  {
    bool ret = klass->लो.f.glyph_v_origins (यह, user_तथ्य,
					     गिनती,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_v_origins);

    अगर (synthetic && is_synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	अगर (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    वापस ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t बायाँ_glyph,
				     hb_codepoint_t दायाँ_glyph)
  {
#अगर_घोषित HB_DISABLE_DEPRECATED
    वापस 0;
#अन्यथा
    वापस klass->लो.f.glyph_h_kerning (यह, user_तथ्य,
					 बायाँ_glyph, दायाँ_glyph,
					 !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_h_kerning);
#पूर्ण_अगर
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t शीर्ष_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#अगर_घोषित HB_DISABLE_DEPRECATED
    वापस 0;
#अन्यथा
    वापस klass->लो.f.glyph_v_kerning (यह, user_तथ्य,
					 शीर्ष_glyph, bottom_glyph,
					 !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_v_kerning);
#पूर्ण_अगर
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       bool synthetic = true)
  {
    hb_memset (extents, 0, माप (*extents));

    /* This is rather messy, but necessary. */

    अगर (!synthetic)
    {
      वापस klass->लो.f.glyph_extents (यह, user_तथ्य,
					 glyph,
					 extents,
					 !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_extents);
    }
    अगर (!is_synthetic &&
	klass->लो.f.glyph_extents (यह, user_तथ्य,
				    glyph,
				    extents,
				    !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_extents))
      वापस true;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#अगर_अघोषित HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    अगर (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().to_glyph_extents ();
      वापस true;
    }
#पूर्ण_अगर

#अगर_अघोषित HB_NO_DRAW
    hb_extents_t<> draw_extents;
    अगर (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.to_glyph_extents ();
      वापस true;
    }
#पूर्ण_अगर

    bool ret = klass->लो.f.glyph_extents (यह, user_तथ्य,
					   glyph,
					   extents,
					   !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_extents);
    अगर (ret)
      synthetic_glyph_extents (extents);

    वापस ret;
  }

  hb_bool_t get_glyph_contour_point (hb_codepoint_t glyph, अचिन्हित पूर्णांक point_सूचकांक,
				     hb_position_t *x, hb_position_t *y,
				     bool synthetic = true)
  {
    *x = *y = 0;
    bool ret = klass->लो.f.glyph_contour_point (यह, user_तथ्य,
						 glyph, point_सूचकांक,
						 x, y,
						 !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_contour_point);

    अगर (synthetic && ret)
    {
      /* Slant */
      अगर (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      अगर (!embolden_in_place)
      {
	पूर्णांक x_shift = x_scale < 0 ? -x_strength : x_strength;
	*x += x_shift;
      }
    }

    वापस ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    अक्षर *name, अचिन्हित पूर्णांक आकार)
  {
    अगर (आकार) *name = '\0';
    वापस klass->लो.f.glyph_name (यह, user_तथ्य,
				    glyph,
				    name, आकार,
				    !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_name);
  }

  hb_bool_t get_glyph_from_name (स्थिर अक्षर *name, पूर्णांक len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    अगर (len == -1) len = म_माप (name);
    वापस klass->लो.f.glyph_from_name (यह, user_तथ्य,
					 name, len,
					 glyph,
					 !klass->user_तथ्य ? nullptr : klass->user_तथ्य->glyph_from_name);
  }

  bool draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, व्योम *draw_तथ्य,
			   bool synthetic = true)
  {
#अगर_अघोषित HB_NO_DRAW
#अगर_अघोषित HB_NO_OUTLINE
    bool embolden = x_strength || y_strength;
    bool slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#अन्यथा
    synthetic = false;
#पूर्ण_अगर

    अगर (!synthetic)
    {
      वापस klass->लो.f.draw_glyph_or_fail (यह, user_तथ्य,
					      glyph,
					      draw_funcs, draw_तथ्य,
					      !klass->user_तथ्य ? nullptr : klass->user_तथ्य->draw_glyph_or_fail);
    }

#अगर_अघोषित HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_remaining = draw_funcs->get_budget_remaining_ptr (draw_तथ्य);
    अगर (!klass->लो.f.draw_glyph_or_fail (यह, user_तथ्य,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_तथ्य ? nullptr : klass->user_तथ्य->draw_glyph_or_fail))
      वापस false;

    // Slant before embolden; produces nicer results.

    अगर (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, false);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    अगर (embolden)
    {
      भग्न x_shift = embolden_in_place ? 0 : (भग्न) x_strength / 2;
      भग्न y_shift = (भग्न) y_strength / 2;
      अगर (x_scale < 0) x_shift = -x_shift;
      अगर (y_scale < 0) y_shift = -y_shift;
      outline.embolden (x_strength, y_strength, x_shift, y_shift);
    }

    outline.replay (draw_funcs, draw_तथ्य);

    वापस true;
#पूर्ण_अगर
#पूर्ण_अगर
    वापस false;
  }

  bool paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, व्योम *paint_तथ्य,
			    अचिन्हित पूर्णांक palette,
			    hb_color_t foreground,
			    bool synthetic = true)
  {
#अगर_अघोषित HB_NO_PAINT
    /* Slant */
    अगर (synthetic && slant_xy)
      hb_paint_धकेलो_transform (paint_funcs, paint_तथ्य,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    bool ret = klass->लो.f.paint_glyph_or_fail (यह, user_तथ्य,
						 glyph,
						 paint_funcs, paint_तथ्य,
						 palette, foreground,
						 !klass->user_तथ्य ? nullptr : klass->user_तथ्य->paint_glyph_or_fail);

    अगर (synthetic && slant_xy)
      hb_paint_निकालो_transform (paint_funcs, paint_तथ्य);

    वापस ret;
#पूर्ण_अगर
    वापस false;
  }

  /* A bit higher-level, and with fallback */

  HB_INTERNAL
  व्योम paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, व्योम *paint_तथ्य,
		    अचिन्हित पूर्णांक palette,
		    hb_color_t foreground);

  व्योम get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    अगर (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->पंक्ति_gap = 0;
    }
  }
  व्योम get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    अगर (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->पंक्ति_gap = 0;
    }
  }

  व्योम get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    अगर (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    अन्यथा
      get_v_extents_with_fallback (extents);
  }

  व्योम get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    अगर (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    अन्यथा
      *y = get_glyph_v_advance (glyph);
  }
  व्योम get_glyph_advances_for_direction (hb_direction_t direction,
					 अचिन्हित पूर्णांक गिनती,
					 स्थिर hb_codepoint_t *first_glyph,
					 अचिन्हित glyph_stride,
					 hb_position_t *first_advance,
					 अचिन्हित advance_stride)
  {
    अगर (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (गिनती, first_glyph, glyph_stride, first_advance, advance_stride);
    अन्यथा
      get_glyph_v_advances (गिनती, first_glyph, glyph_stride, first_advance, advance_stride);
  }

  व्योम apply_offset (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     चिन्हित mult)
  {
    निश्चित (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  व्योम add_offset (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  व्योम subtract_offset (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  व्योम guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  व्योम apply_glyph_h_origins_with_fallback (hb_बफर_t *buf, पूर्णांक mult)
  {
    bool has_ascender = false;
    hb_position_t ascender = 0;

    काष्ठा { hb_position_t x, y; } origins[32];

    अचिन्हित पूर्णांक offset = 0;
    अचिन्हित पूर्णांक गिनती = buf->len;
    जबतक (offset < गिनती)
    {
      अचिन्हित n = hb_min (गिनती - offset, ARRAY_लंबाई (origins));
      अगर (!get_glyph_h_origins (n,
				&buf->info[offset].codepoint, माप (hb_glyph_info_t),
				&origins[0].x, माप (origins[0]),
				&origins[0].y, माप (origins[0])))
      {
        अगर (get_glyph_v_origins (n,
				  &buf->info[offset].codepoint, माप (hb_glyph_info_t),
				  &origins[0].x, माप (origins[0]),
				  &origins[0].y, माप (origins[0])))
	{
	  अगर (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = true;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  क्रम (अचिन्हित j = 0; j < n; j++)
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

      निश्चित (mult == -1 || mult == +1 || mult == 0);
      अगर (mult == +1)
        क्रम (अचिन्हित j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      अन्यथा अगर (mult == -1)
	क्रम (अचिन्हित j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }
  व्योम apply_glyph_v_origins_with_fallback (hb_बफर_t *buf, पूर्णांक mult)
  {
    bool has_ascender = false;
    hb_position_t ascender = 0;

    काष्ठा { hb_position_t x, y; } origins[32];

    अचिन्हित पूर्णांक offset = 0;
    अचिन्हित पूर्णांक गिनती = buf->len;
    जबतक (offset < गिनती)
    {
      अचिन्हित n = hb_min (गिनती - offset, ARRAY_लंबाई (origins));
      अगर (!get_glyph_v_origins (n,
				&buf->info[offset].codepoint, माप (hb_glyph_info_t),
				&origins[0].x, माप (origins[0]),
				&origins[0].y, माप (origins[0])))
      {
	अगर (get_glyph_h_origins (n,
				 &buf->info[offset].codepoint, माप (hb_glyph_info_t),
				 &origins[0].x, माप (origins[0]),
				 &origins[0].y, माप (origins[0])))
	{
	  अगर (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = true;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  क्रम (अचिन्हित j = 0; j < n; j++)
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

      निश्चित (mult == -1 || mult == +1 || mult == 0);
      अगर (mult == +1)
        क्रम (अचिन्हित j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      अन्यथा अगर (mult == -1)
	क्रम (अचिन्हित j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }

  व्योम get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    अगर (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      subtract_offset (x, y, dx, dy);
    }
  }
  व्योम get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    अगर (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      add_offset (x, y, dx, dy);
    }
  }

  व्योम get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    अगर (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    अन्यथा
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  व्योम add_glyph_h_origins (hb_बफर_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  व्योम add_glyph_v_origins (hb_बफर_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  व्योम add_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    add_offset (x, y, origin_x, origin_y);
  }

  व्योम subtract_glyph_h_origins (hb_बफर_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  व्योम subtract_glyph_v_origins (hb_बफर_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  व्योम subtract_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    subtract_offset (x, y, origin_x, origin_y);
  }

  व्योम get_glyph_kerning_for_direction (hb_codepoint_t first_glyph, hb_codepoint_t second_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    अगर (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
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

    अगर (ret)
      subtract_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    वापस ret;
  }

  hb_bool_t get_glyph_contour_point_for_origin (hb_codepoint_t glyph, अचिन्हित पूर्णांक point_सूचकांक,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_point (glyph, point_सूचकांक, x, y);

    अगर (ret)
      subtract_glyph_origin_for_direction (glyph, direction, x, y);

    वापस ret;
  }

  /* Generates gidDDD if glyph has no name. */
  व्योम
  glyph_to_string (hb_codepoint_t glyph,
		   अक्षर *s, अचिन्हित पूर्णांक आकार)
  {
    अगर (get_glyph_name (glyph, s, आकार)) वापस;

    अगर (आकार && snprintf (s, आकार, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (स्थिर अक्षर *s, पूर्णांक len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    अगर (get_glyph_from_name (s, len, glyph)) वापस true;

    अगर (len == -1) len = म_माप (s);

    /* Straight glyph index. */
    अगर (hb_codepoint_parse (s, len, 10, glyph))
      वापस true;

    अगर (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      अगर (0 == म_भेदन (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	वापस true;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      अगर (0 == म_भेदन (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	वापस true;
    }

    वापस false;
  }

  व्योम changed ()
  {
    भग्न upem = face->get_upem ();

    x_multf = x_scale / upem;
    y_multf = y_scale / upem;
    x_mult = (int64_t) x_scale * 0x10000 / upem;
    y_mult = (int64_t) y_scale * 0x10000 / upem;

    is_synthetic =  x_embolden || y_embolden || slant;

    x_strength = roundf (असल (x_scale) * x_embolden);
    y_strength = roundf (असल (y_scale) * y_embolden);

    slant_xy = y_scale ? slant * x_scale / y_scale : 0.f;

    तथ्य.fini ();

    serial++;
  }

  hb_position_t em_mult (int32_t v, int64_t mult)
  { वापस (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (भग्न v, भग्न mult)
  { वापस (hb_position_t) roundf (em_fmultf (v, mult)); }
  भग्न em_fmultf (भग्न v, भग्न mult)
  { वापस v * mult; }
  भग्न em_fmult (int32_t v, भग्न mult)
  { वापस (भग्न) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#पूर्ण_अगर /* HB_FONT_HH */
