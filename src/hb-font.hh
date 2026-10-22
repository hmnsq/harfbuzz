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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-face.hh"
#অন্তর্ভাব "hb-atomic.hh"
#অন্তর্ভাব "hb-draw.hh"
#অন্তর্ভাব "hb-paint-extents.hh"
#অন্তর্ভাব "hb-shaper.hh"
#অন্তর্ভাব "hb-outline.hh"


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

কাঠামো hb_font_funcs_t
{
  hb_object_header_t header;

  কাঠামো {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) শূন্য *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *user_তথ্য;

  কাঠামো {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  সংঘ get_t {
    কাঠামো get_funcs_t {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
    } f;
    শূন্য (*array[0
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

#define HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_তথ্য_INSTANTIATE_SHAPERS(shaper, font);
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

কাঠামো hb_font_t
{
  hb_object_header_t header;
  hb_atomic_t<unsigned> serial;
  hb_atomic_t<unsigned> serial_coords;

  hb_font_t *parent;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  যৌক্তিক is_synthetic;

  দশমিক x_embolden;
  দশমিক y_embolden;
  যৌক্তিক embolden_in_place;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  দশমিক slant;
  দশমিক slant_xy;

  দশমিক x_multf;
  দশমিক y_multf;
  int64_t x_mult;
  int64_t y_mult;

  unsigned পূর্ণসংখ্যা x_ppem;
  unsigned পূর্ণসংখ্যা y_ppem;

  দশমিক ptem;

  /* Font variation coordinates. */
  unsigned পূর্ণসংখ্যা instance_সূচক;
  যৌক্তিক has_nonzero_coords;
  unsigned পূর্ণসংখ্যা অঙ্ক_coords;
  পূর্ণসংখ্যা *coords;
  দশমিক *design_coords;

  hb_font_funcs_t   *klass;
  শূন্য              *user_তথ্য;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> তথ্য; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { ফেরত HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { ফেরত em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { ফেরত em_mult (v, y_mult); }
  hb_position_t em_scalef_x (দশমিক v) { ফেরত em_multf (v, x_multf); }
  hb_position_t em_scalef_y (দশমিক v) { ফেরত em_multf (v, y_multf); }
  দশমিক em_fscale_x (int32_t v) { ফেরত em_fmult (v, x_multf); }
  দশমিক em_fscale_y (int32_t v) { ফেরত em_fmult (v, y_multf); }
  দশমিক em_fscalef_x (দশমিক v) { ফেরত em_fmultf (v, x_multf); }
  দশমিক em_fscalef_y (দশমিক v) { ফেরত em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { ফেরত em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t parent_scale_x_distance (hb_position_t v)
  {
    যদি (unlikely (parent && parent->x_scale && parent->x_scale != x_scale))
      ফেরত (hb_position_t) (v * (int64_t) এই->x_scale / এই->parent->x_scale);
    ফেরত v;
  }
  hb_position_t parent_scale_y_distance (hb_position_t v)
  {
    যদি (unlikely (parent && parent->y_scale && parent->y_scale != y_scale))
      ফেরত (hb_position_t) (v * (int64_t) এই->y_scale / এই->parent->y_scale);
    ফেরত v;
  }
  hb_position_t parent_scale_x_position (hb_position_t v)
  { ফেরত parent_scale_x_distance (v); }
  hb_position_t parent_scale_y_position (hb_position_t v)
  { ফেরত parent_scale_y_distance (v); }

  শূন্য parent_scale_distance (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_distance (*x);
    *y = parent_scale_y_distance (*y);
  }
  শূন্য parent_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = parent_scale_x_position (*x);
    *y = parent_scale_y_position (*y);
  }

  শূন্য scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    দশমিক x1 = em_fscale_x (extents->x_bearing);
    দশমিক y1 = em_fscale_y (extents->y_bearing);
    দশমিক x2 = x1 + em_fscale_x (extents->width);
    দশমিক y2 = y1 + em_fscale_y (extents->height);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->width = ceilf (x2) - extents->x_bearing;
    extents->height = ceilf (y2) - extents->y_bearing;
  }

  শূন্য synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    যদি (slant_xy)
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
    যদি (x_strength || y_strength)
    {
      /* Y */
      পূর্ণসংখ্যা y_shift = y_strength;
      যদি (y_scale < 0) y_shift = -y_shift;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->y_bearing + y_shift);
      extents->height = hb_clamp_to<hb_position_t> ((int64_t) extents->height - y_shift);

      /* X */
      পূর্ণসংখ্যা x_shift = x_strength;
      যদি (x_scale < 0) x_shift = -x_shift;
      যদি (embolden_in_place)
	extents->x_bearing = hb_clamp_to<hb_position_t> ((int64_t) extents->x_bearing - x_shift / 2);
      extents->width = hb_clamp_to<hb_position_t> ((int64_t) extents->width + x_shift);
    }
  }


  /* Public getters */

  HB_INTERNAL যৌক্তিক has_func (unsigned পূর্ণসংখ্যা i);
  HB_INTERNAL যৌক্তিক has_func_set (unsigned পূর্ণসংখ্যা i);

  /* has_* ... */
#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
  যৌক্তিক \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = এই->klass; \
    unsigned পূর্ণসংখ্যা i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / আকার (funcs->get.array[0]); \
    ফেরত has_func (i); \
  } \
  যৌক্তিক \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = এই->klass; \
    unsigned পূর্ণসংখ্যা i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / আকার (funcs->get.array[0]); \
    ফেরত has_func_set (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				যৌক্তিক synthetic = সত্য)
  {
    hb_memset (extents, 0, আকার (*extents));
    যৌক্তিক ret = klass->get.f.font_h_extents (এই, user_তথ্য,
					    extents,
					    !klass->user_তথ্য ? nullptr : klass->user_তথ্য->font_h_extents);

    যদি (synthetic && ret)
    {
      /* Embolden */
      পূর্ণসংখ্যা y_shift = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_shift;
    }

    ফেরত ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				যৌক্তিক synthetic = সত্য)
  {
    hb_memset (extents, 0, আকার (*extents));
    যৌক্তিক ret = klass->get.f.font_v_extents (এই, user_তথ্য,
					    extents,
					    !klass->user_তথ্য ? nullptr : klass->user_তথ্য->font_v_extents);

    যদি (synthetic && ret)
    {
      /* Embolden */
      পূর্ণসংখ্যা x_shift = x_scale < 0 ? -x_strength : x_strength;
      যদি (embolden_in_place)
      {
	extents->ascender += x_shift / 2;
	extents->descender -= x_shift - x_shift / 2;
      }
      অন্যথা
	extents->ascender += x_shift;
    }

    ফেরত ret;
  }

  যৌক্তিক has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    ফেরত get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    ফেরত klass->get.f.nominal_glyph (এই, user_তথ্য,
				       unicode, glyph,
				       !klass->user_তথ্য ? nullptr : klass->user_তথ্য->nominal_glyph);
  }
  unsigned পূর্ণসংখ্যা get_nominal_glyphs (unsigned পূর্ণসংখ্যা গুনতি,
				   ধ্রুবক hb_codepoint_t *first_unicode,
				   unsigned পূর্ণসংখ্যা unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned পূর্ণসংখ্যা glyph_stride)
  {
    ফেরত klass->get.f.nominal_glyphs (এই, user_তথ্য,
					গুনতি,
					first_unicode, unicode_stride,
					first_glyph, glyph_stride,
					!klass->user_তথ্য ? nullptr : klass->user_তথ্য->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_found = 0)
  {
    *glyph = not_found;
    ফেরত klass->get.f.variation_glyph (এই, user_তথ্য,
					 unicode, variation_selector, glyph,
					 !klass->user_তথ্য ? nullptr : klass->user_তথ্য->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     যৌক্তিক synthetic = সত্য)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (এই, user_তথ্য,
							  glyph,
							  !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_h_advance);

    যদি (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    ফেরত advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     যৌক্তিক synthetic = সত্য)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (এই, user_তথ্য,
							  glyph,
							  !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_v_advance);

    যদি (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    ফেরত advance;
  }

  শূন্য get_glyph_h_advances (unsigned পূর্ণসংখ্যা গুনতি,
			     ধ্রুবক hb_codepoint_t *first_glyph,
			     unsigned পূর্ণসংখ্যা glyph_stride,
			     hb_position_t *first_advance,
			     unsigned পূর্ণসংখ্যা advance_stride,
			     যৌক্তিক synthetic = সত্য)
  {
    klass->get.f.glyph_h_advances (এই, user_তথ্য,
				   গুনতি,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_h_advances);

    যদি (synthetic && x_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  শূন্য get_glyph_v_advances (unsigned পূর্ণসংখ্যা গুনতি,
			     ধ্রুবক hb_codepoint_t *first_glyph,
			     unsigned পূর্ণসংখ্যা glyph_stride,
			     hb_position_t *first_advance,
			     unsigned পূর্ণসংখ্যা advance_stride,
			     যৌক্তিক synthetic = সত্য)
  {
    klass->get.f.glyph_v_advances (এই, user_তথ্য,
				   গুনতি,
				   first_glyph, glyph_stride,
				   first_advance, advance_stride,
				   !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_v_advances);

    যদি (synthetic && y_strength && !embolden_in_place)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      {
	*first_advance += *first_advance ? strength : 0;
	first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				যৌক্তিক synthetic = সত্য)
  {
    *x = *y = 0;
    যৌক্তিক ret = klass->get.f.glyph_h_origin (এই, user_তথ্য,
					    glyph, x, y,
					    !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_h_origin);

    যদি (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      যদি (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    ফেরত ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				যৌক্তিক synthetic = সত্য)
  {
    *x = *y = 0;
    যৌক্তিক ret = klass->get.f.glyph_v_origin (এই, user_তথ্য,
					    glyph, x, y,
					    !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_v_origin);

    যদি (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      যদি (!embolden_in_place)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    ফেরত ret;
  }

  hb_bool_t get_glyph_h_origins (unsigned পূর্ণসংখ্যা গুনতি,
				 ধ্রুবক hb_codepoint_t *first_glyph,
				 unsigned পূর্ণসংখ্যা glyph_stride,
				 hb_position_t *first_x,
				 unsigned পূর্ণসংখ্যা x_stride,
				 hb_position_t *first_y,
				 unsigned পূর্ণসংখ্যা y_stride,
				 যৌক্তিক synthetic = সত্য)

  {
    যৌক্তিক ret = klass->get.f.glyph_h_origins (এই, user_তথ্য,
					     গুনতি,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_h_origins);

    যদি (synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      জন্য (unsigned i = 0; i < গুনতি; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	যদি (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    ফেরত ret;
  }

  hb_bool_t get_glyph_v_origins (unsigned পূর্ণসংখ্যা গুনতি,
				 ধ্রুবক hb_codepoint_t *first_glyph,
				 unsigned পূর্ণসংখ্যা glyph_stride,
				 hb_position_t *first_x,
				 unsigned পূর্ণসংখ্যা x_stride,
				 hb_position_t *first_y,
				 unsigned পূর্ণসংখ্যা y_stride,
				 যৌক্তিক synthetic = সত্য)

  {
    যৌক্তিক ret = klass->get.f.glyph_v_origins (এই, user_তথ্য,
					     গুনতি,
					     first_glyph, glyph_stride,
					     first_x, x_stride, first_y, y_stride,
					     !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_v_origins);

    যদি (synthetic && is_synthetic && ret)
    {
      hb_position_t x_shift = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_shift = y_scale < 0 ? -y_strength : y_strength;
      জন্য (unsigned i = 0; i < গুনতি; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	যদি (!embolden_in_place)
	{
	  *first_x += x_shift;
	  *first_y += y_shift;
	}
	first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }

    ফেরত ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t বাম_glyph,
				     hb_codepoint_t ডান_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    ফেরত 0;
#অন্যথা
    ফেরত klass->get.f.glyph_h_kerning (এই, user_তথ্য,
					 বাম_glyph, ডান_glyph,
					 !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_h_kerning);
#endif
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t শীর্ষ_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#ifdef HB_DISABLE_DEPRECATED
    ফেরত 0;
#অন্যথা
    ফেরত klass->get.f.glyph_v_kerning (এই, user_তথ্য,
					 শীর্ষ_glyph, bottom_glyph,
					 !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_v_kerning);
#endif
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       যৌক্তিক synthetic = সত্য)
  {
    hb_memset (extents, 0, আকার (*extents));

    /* This is rather messy, but necessary. */

    যদি (!synthetic)
    {
      ফেরত klass->get.f.glyph_extents (এই, user_তথ্য,
					 glyph,
					 extents,
					 !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_extents);
    }
    যদি (!is_synthetic &&
	klass->get.f.glyph_extents (এই, user_তথ্য,
				    glyph,
				    extents,
				    !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_extents))
      ফেরত সত্য;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#ifndef HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    যদি (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().to_glyph_extents ();
      ফেরত সত্য;
    }
#endif

#ifndef HB_NO_DRAW
    hb_extents_t<> draw_extents;
    যদি (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.to_glyph_extents ();
      ফেরত সত্য;
    }
#endif

    যৌক্তিক ret = klass->get.f.glyph_extents (এই, user_তথ্য,
					   glyph,
					   extents,
					   !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_extents);
    যদি (ret)
      synthetic_glyph_extents (extents);

    ফেরত ret;
  }

  hb_bool_t get_glyph_contour_point (hb_codepoint_t glyph, unsigned পূর্ণসংখ্যা point_সূচক,
				     hb_position_t *x, hb_position_t *y,
				     যৌক্তিক synthetic = সত্য)
  {
    *x = *y = 0;
    যৌক্তিক ret = klass->get.f.glyph_contour_point (এই, user_তথ্য,
						 glyph, point_সূচক,
						 x, y,
						 !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_contour_point);

    যদি (synthetic && ret)
    {
      /* Slant */
      যদি (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      যদি (!embolden_in_place)
      {
	পূর্ণসংখ্যা x_shift = x_scale < 0 ? -x_strength : x_strength;
	*x += x_shift;
      }
    }

    ফেরত ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    অক্ষর *name, unsigned পূর্ণসংখ্যা আকার_2)
  {
    যদি (আকার_2) *name = '\0';
    ফেরত klass->get.f.glyph_name (এই, user_তথ্য,
				    glyph,
				    name, আকার_2,
				    !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_name);
  }

  hb_bool_t get_glyph_from_name (ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    যদি (len == -1) len = strlen (name);
    ফেরত klass->get.f.glyph_from_name (এই, user_তথ্য,
					 name, len,
					 glyph,
					 !klass->user_তথ্য ? nullptr : klass->user_তথ্য->glyph_from_name);
  }

  যৌক্তিক draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, শূন্য *draw_তথ্য,
			   যৌক্তিক synthetic = সত্য)
  {
#ifndef HB_NO_DRAW
#ifndef HB_NO_OUTLINE
    যৌক্তিক embolden = x_strength || y_strength;
    যৌক্তিক slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#অন্যথা
    synthetic = মিথ্যা;
#endif

    যদি (!synthetic)
    {
      ফেরত klass->get.f.draw_glyph_or_fail (এই, user_তথ্য,
					      glyph,
					      draw_funcs, draw_তথ্য,
					      !klass->user_তথ্য ? nullptr : klass->user_তথ্য->draw_glyph_or_fail);
    }

#ifndef HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_remaining = draw_funcs->get_budget_remaining_ptr (draw_তথ্য);
    যদি (!klass->get.f.draw_glyph_or_fail (এই, user_তথ্য,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_তথ্য ? nullptr : klass->user_তথ্য->draw_glyph_or_fail))
      ফেরত মিথ্যা;

    // Slant before embolden; produces nicer results.

    যদি (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, মিথ্যা);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    যদি (embolden)
    {
      দশমিক x_shift = embolden_in_place ? 0 : (দশমিক) x_strength / 2;
      দশমিক y_shift = (দশমিক) y_strength / 2;
      যদি (x_scale < 0) x_shift = -x_shift;
      যদি (y_scale < 0) y_shift = -y_shift;
      outline.embolden (x_strength, y_strength, x_shift, y_shift);
    }

    outline.replay (draw_funcs, draw_তথ্য);

    ফেরত সত্য;
#endif
#endif
    ফেরত মিথ্যা;
  }

  যৌক্তিক paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, শূন্য *paint_তথ্য,
			    unsigned পূর্ণসংখ্যা palette,
			    hb_color_t foreground,
			    যৌক্তিক synthetic = সত্য)
  {
#ifndef HB_NO_PAINT
    /* Slant */
    যদি (synthetic && slant_xy)
      hb_paint_ঠেলো_transform (paint_funcs, paint_তথ্য,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    যৌক্তিক ret = klass->get.f.paint_glyph_or_fail (এই, user_তথ্য,
						 glyph,
						 paint_funcs, paint_তথ্য,
						 palette, foreground,
						 !klass->user_তথ্য ? nullptr : klass->user_তথ্য->paint_glyph_or_fail);

    যদি (synthetic && slant_xy)
      hb_paint_বের_করো_transform (paint_funcs, paint_তথ্য);

    ফেরত ret;
#endif
    ফেরত মিথ্যা;
  }

  /* A bit higher-level, and with fallback */

  HB_INTERNAL
  শূন্য paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, শূন্য *paint_তথ্য,
		    unsigned পূর্ণসংখ্যা palette,
		    hb_color_t foreground);

  শূন্য get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    যদি (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->সারি_gap = 0;
    }
  }
  শূন্য get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    যদি (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->সারি_gap = 0;
    }
  }

  শূন্য get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    যদি (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    অন্যথা
      get_v_extents_with_fallback (extents);
  }

  শূন্য get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    যদি (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    অন্যথা
      *y = get_glyph_v_advance (glyph);
  }
  শূন্য get_glyph_advances_for_direction (hb_direction_t direction,
					 unsigned পূর্ণসংখ্যা গুনতি,
					 ধ্রুবক hb_codepoint_t *first_glyph,
					 unsigned glyph_stride,
					 hb_position_t *first_advance,
					 unsigned advance_stride)
  {
    যদি (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (গুনতি, first_glyph, glyph_stride, first_advance, advance_stride);
    অন্যথা
      get_glyph_v_advances (গুনতি, first_glyph, glyph_stride, first_advance, advance_stride);
  }

  শূন্য apply_offset (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     signed mult)
  {
    প্রতিজ্ঞা (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  শূন্য add_offset (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  শূন্য subtract_offset (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  শূন্য guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  শূন্য apply_glyph_h_origins_with_fallback (hb_বাফার_t *buf, পূর্ণসংখ্যা mult)
  {
    যৌক্তিক has_ascender = মিথ্যা;
    hb_position_t ascender = 0;

    কাঠামো { hb_position_t x, y; } origins[32];

    unsigned পূর্ণসংখ্যা offset = 0;
    unsigned পূর্ণসংখ্যা গুনতি = buf->len;
    যখন (offset < গুনতি)
    {
      unsigned n = hb_min (গুনতি - offset, ARRAY_দৈর্ঘ্য (origins));
      যদি (!get_glyph_h_origins (n,
				&buf->info[offset].codepoint, আকার (hb_glyph_info_t),
				&origins[0].x, আকার (origins[0]),
				&origins[0].y, আকার (origins[0])))
      {
        যদি (get_glyph_v_origins (n,
				  &buf->info[offset].codepoint, আকার (hb_glyph_info_t),
				  &origins[0].x, আকার (origins[0]),
				  &origins[0].y, আকার (origins[0])))
	{
	  যদি (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = সত্য;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  জন্য (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	অন্যথা
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      প্রতিজ্ঞা (mult == -1 || mult == +1 || mult == 0);
      যদি (mult == +1)
        জন্য (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      অন্যথা যদি (mult == -1)
	জন্য (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }
  শূন্য apply_glyph_v_origins_with_fallback (hb_বাফার_t *buf, পূর্ণসংখ্যা mult)
  {
    যৌক্তিক has_ascender = মিথ্যা;
    hb_position_t ascender = 0;

    কাঠামো { hb_position_t x, y; } origins[32];

    unsigned পূর্ণসংখ্যা offset = 0;
    unsigned পূর্ণসংখ্যা গুনতি = buf->len;
    যখন (offset < গুনতি)
    {
      unsigned n = hb_min (গুনতি - offset, ARRAY_দৈর্ঘ্য (origins));
      যদি (!get_glyph_v_origins (n,
				&buf->info[offset].codepoint, আকার (hb_glyph_info_t),
				&origins[0].x, আকার (origins[0]),
				&origins[0].y, আকার (origins[0])))
      {
	যদি (get_glyph_h_origins (n,
				 &buf->info[offset].codepoint, আকার (hb_glyph_info_t),
				 &origins[0].x, আকার (origins[0]),
				 &origins[0].y, আকার (origins[0])))
	{
	  যদি (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = সত্য;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  জন্য (unsigned j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[offset + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	অন্যথা
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      প্রতিজ্ঞা (mult == -1 || mult == +1 || mult == 0);
      যদি (mult == +1)
        জন্য (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  add_offset (&pos->x_offset, &pos->y_offset,
		      origins[j].x, origins[j].y);
	}
      অন্যথা যদি (mult == -1)
	জন্য (unsigned j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[offset + j];
	  subtract_offset (&pos->x_offset, &pos->y_offset,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      offset += n;
    }
  }

  শূন্য get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    যদি (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      subtract_offset (x, y, dx, dy);
    }
  }
  শূন্য get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    যদি (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      add_offset (x, y, dx, dy);
    }
  }

  শূন্য get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    যদি (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    অন্যথা
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  শূন্য add_glyph_h_origins (hb_বাফার_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  শূন্য add_glyph_v_origins (hb_বাফার_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  শূন্য add_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    add_offset (x, y, origin_x, origin_y);
  }

  শূন্য subtract_glyph_h_origins (hb_বাফার_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  শূন্য subtract_glyph_v_origins (hb_বাফার_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  শূন্য subtract_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    subtract_offset (x, y, origin_x, origin_y);
  }

  শূন্য get_glyph_kerning_for_direction (hb_codepoint_t first_glyph, hb_codepoint_t second_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    যদি (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (first_glyph, second_glyph);
    } অন্যথা {
      *x = 0;
      *y = get_glyph_v_kerning (first_glyph, second_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    যদি (ret)
      subtract_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    ফেরত ret;
  }

  hb_bool_t get_glyph_contour_point_for_origin (hb_codepoint_t glyph, unsigned পূর্ণসংখ্যা point_সূচক,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_point (glyph, point_সূচক, x, y);

    যদি (ret)
      subtract_glyph_origin_for_direction (glyph, direction, x, y);

    ফেরত ret;
  }

  /* Generates gidDDD if glyph has no name. */
  শূন্য
  glyph_to_string (hb_codepoint_t glyph,
		   অক্ষর *s, unsigned পূর্ণসংখ্যা আকার_2)
  {
    যদি (get_glyph_name (glyph, s, আকার_2)) ফেরত;

    যদি (আকার_2 && snprintf (s, আকার_2, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (ধ্রুবক অক্ষর *s, পূর্ণসংখ্যা len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    যদি (get_glyph_from_name (s, len, glyph)) ফেরত সত্য;

    যদি (len == -1) len = strlen (s);

    /* Straight glyph index. */
    যদি (hb_codepoint_parse (s, len, 10, glyph))
      ফেরত সত্য;

    যদি (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      যদি (0 == strncmp (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	ফেরত সত্য;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      যদি (0 == strncmp (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	ফেরত সত্য;
    }

    ফেরত মিথ্যা;
  }

  শূন্য changed ()
  {
    দশমিক upem = face->get_upem ();

    x_multf = x_scale / upem;
    y_multf = y_scale / upem;
    x_mult = (int64_t) x_scale * 0x10000 / upem;
    y_mult = (int64_t) y_scale * 0x10000 / upem;

    is_synthetic =  x_embolden || y_embolden || slant;

    x_strength = roundf (abs (x_scale) * x_embolden);
    y_strength = roundf (abs (y_scale) * y_embolden);

    slant_xy = y_scale ? slant * x_scale / y_scale : 0.f;

    তথ্য.fini ();

    serial++;
  }

  hb_position_t em_mult (int32_t v, int64_t mult)
  { ফেরত (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (দশমিক v, দশমিক mult)
  { ফেরত (hb_position_t) roundf (em_fmultf (v, mult)); }
  দশমিক em_fmultf (দশমিক v, দশমিক mult)
  { ফেরত v * mult; }
  দশমিক em_fmult (int32_t v, দশমিক mult)
  { ফেরত (দশমিক) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#endif /* HB_FONT_HH */
