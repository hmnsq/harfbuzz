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

#నిర్వచితం_కాకపోతే HB_FONT_HH
#నిర్వచించు HB_FONT_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-face.hh"
#అన్తర్భావ "hb-atomic.hh"
#అన్తర్భావ "hb-draw.hh"
#అన్తర్భావ "hb-paint-extents.hh"
#అన్తర్భావ "hb-shaper.hh"
#అన్తర్భావ "hb-outline.hh"


/*
 * hb_font_funcs_t
 */

#నిర్వచించు HB_FONT_FUNCS_IMPLEMENT_CALLBACKS \
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
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_contour_బిందువు) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_name) \
  HB_FONT_FUNC_IMPLEMENT (get_,glyph_from_name) \
  HB_FONT_FUNC_IMPLEMENT (,draw_glyph_or_fail) \
  HB_FONT_FUNC_IMPLEMENT (,paint_glyph_or_fail) \
  /* ^--- Add new callbacks here */

నిర్మాణం hb_font_funcs_t
{
  hb_object_శీర్షిక_t శీర్షిక;

  నిర్మాణం {
#నిర్వచించు HB_FONT_FUNC_IMPLEMENT(get_,name) శూన్య *name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_FONT_FUNC_IMPLEMENT
  } *user_దత్తాంశం;

  నిర్మాణం {
#నిర్వచించు HB_FONT_FUNC_IMPLEMENT(get_,name) hb_destroy_func_t name;
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_FONT_FUNC_IMPLEMENT
  } *destroy;

  /* Don't access these directly.  Call font->get_*() instead. */
  సంఘం get_t {
    నిర్మాణం get_funcs_t {
#నిర్వచించు HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_func_t name;
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_FONT_FUNC_IMPLEMENT
    } f;
    శూన్య (*array[0
#నిర్వచించు HB_FONT_FUNC_IMPLEMENT(get_,name) +1
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_FONT_FUNC_IMPLEMENT
		]) ();
  } get;
};
DECLARE_NULL_INSTANCE (hb_font_funcs_t);


/*
 * hb_font_t
 */

#నిర్వచించు HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_దత్తాంశం_INSTANTIATE_SHAPERS(shaper, font);
#అన్తర్భావ "hb-shaper-list.hh"
#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT

నిర్మాణం hb_font_t
{
  hb_object_శీర్షిక_t శీర్షిక;
  hb_atomic_t<గుర్తులేని> serial;
  hb_atomic_t<గుర్తులేని> serial_coords;

  hb_font_t *మాతృ;
  hb_face_t *face;

  int32_t x_scale;
  int32_t y_scale;

  తార్కిక is_synthetic;

  దశాంశం x_embolden;
  దశాంశం y_embolden;
  తార్కిక embolden_లోపల_ఉంచు;
  int32_t x_strength; /* x_embolden, in scaled units. */
  int32_t y_strength; /* y_embolden, in scaled units. */

  దశాంశం slant;
  దశాంశం slant_xy;

  దశాంశం x_multf;
  దశాంశం y_multf;
  int64_t x_mult;
  int64_t y_mult;

  గుర్తులేని పూర్ణాంకం x_ppem;
  గుర్తులేని పూర్ణాంకం y_ppem;

  దశాంశం ptem;

  /* Font variation coordinates. */
  గుర్తులేని పూర్ణాంకం instance_సూచీ;
  తార్కిక has_nonzero_coords;
  గుర్తులేని పూర్ణాంకం అంకె_coords;
  పూర్ణాంకం *coords;
  దశాంశం *design_coords;

  hb_font_funcs_t   *klass;
  శూన్య              *user_దత్తాంశం;
  hb_destroy_func_t  destroy;

  hb_shaper_object_dataset_t<hb_font_t> దత్తాంశం; /* Various shaper data. */


  /* Convert from font-space to user-space */
  int64_t dir_mult (hb_direction_t direction)
  { తిరిగి HB_DIRECTION_IS_VERTICAL(direction) ? y_mult : x_mult; }
  hb_position_t em_scale_x (int32_t v) { తిరిగి em_mult (v, x_mult); }
  hb_position_t em_scale_y (int32_t v) { తిరిగి em_mult (v, y_mult); }
  hb_position_t em_scalef_x (దశాంశం v) { తిరిగి em_multf (v, x_multf); }
  hb_position_t em_scalef_y (దశాంశం v) { తిరిగి em_multf (v, y_multf); }
  దశాంశం em_fscale_x (int32_t v) { తిరిగి em_fmult (v, x_multf); }
  దశాంశం em_fscale_y (int32_t v) { తిరిగి em_fmult (v, y_multf); }
  దశాంశం em_fscalef_x (దశాంశం v) { తిరిగి em_fmultf (v, x_multf); }
  దశాంశం em_fscalef_y (దశాంశం v) { తిరిగి em_fmultf (v, y_multf); }
  hb_position_t em_scale_dir (int16_t v, hb_direction_t direction)
  { తిరిగి em_mult (v, dir_mult (direction)); }

  /* Convert from parent-font user-space to our user-space */
  hb_position_t మాతృ_scale_x_దూరం (hb_position_t v)
  {
    ఒకవేళ (unlikely (మాతృ && మాతృ->x_scale && మాతృ->x_scale != x_scale))
      తిరిగి (hb_position_t) (v * (int64_t) ఇది->x_scale / ఇది->మాతృ->x_scale);
    తిరిగి v;
  }
  hb_position_t మాతృ_scale_y_దూరం (hb_position_t v)
  {
    ఒకవేళ (unlikely (మాతృ && మాతృ->y_scale && మాతృ->y_scale != y_scale))
      తిరిగి (hb_position_t) (v * (int64_t) ఇది->y_scale / ఇది->మాతృ->y_scale);
    తిరిగి v;
  }
  hb_position_t మాతృ_scale_x_position (hb_position_t v)
  { తిరిగి మాతృ_scale_x_దూరం (v); }
  hb_position_t మాతృ_scale_y_position (hb_position_t v)
  { తిరిగి మాతృ_scale_y_దూరం (v); }

  శూన్య మాతృ_scale_దూరం (hb_position_t *x, hb_position_t *y)
  {
    *x = మాతృ_scale_x_దూరం (*x);
    *y = మాతృ_scale_y_దూరం (*y);
  }
  శూన్య మాతృ_scale_position (hb_position_t *x, hb_position_t *y)
  {
    *x = మాతృ_scale_x_position (*x);
    *y = మాతృ_scale_y_position (*y);
  }

  శూన్య scale_glyph_extents (hb_glyph_extents_t *extents)
  {
    దశాంశం x1 = em_fscale_x (extents->x_bearing);
    దశాంశం y1 = em_fscale_y (extents->y_bearing);
    దశాంశం x2 = x1 + em_fscale_x (extents->వెడల్పు);
    దశాంశం y2 = y1 + em_fscale_y (extents->ఎత్తు);

    extents->x_bearing = floorf (x1);
    extents->y_bearing = floorf (y1);
    extents->వెడల్పు = ceilf (x2) - extents->x_bearing;
    extents->ఎత్తు = ceilf (y2) - extents->y_bearing;
  }

  శూన్య synthetic_glyph_extents (hb_glyph_extents_t *extents)
  {
    /* Slant. */
    ఒకవేళ (slant_xy)
    {
      /* x_bearing/width and y_bearing/height are each clamped to the full
       * hb_position_t range independently by the extent producers, so their
       * sums and differences can overflow int32.  Accumulate in int64. */
      int64_t y1 = extents->y_bearing;
      int64_t y2 = (int64_t) extents->y_bearing + extents->ఎత్తు;

      int64_t x1 = (int64_t) extents->x_bearing + (int64_t) floorf (hb_min (y1 * slant_xy, y2 * slant_xy));
      int64_t x2 = (int64_t) extents->x_bearing + extents->వెడల్పు + (int64_t) ceilf (hb_max (y1 * slant_xy, y2 * slant_xy));

      extents->x_bearing = hb_clamp_గమ్యం<hb_position_t> (x1);
      extents->వెడల్పు = hb_clamp_గమ్యం<hb_position_t> (x2 - x1);
    }

    /* Embolden. */
    ఒకవేళ (x_strength || y_strength)
    {
      /* Y */
      పూర్ణాంకం y_జరుపు = y_strength;
      ఒకవేళ (y_scale < 0) y_జరుపు = -y_జరుపు;
      extents->y_bearing = hb_clamp_గమ్యం<hb_position_t> ((int64_t) extents->y_bearing + y_జరుపు);
      extents->ఎత్తు = hb_clamp_గమ్యం<hb_position_t> ((int64_t) extents->ఎత్తు - y_జరుపు);

      /* X */
      పూర్ణాంకం x_జరుపు = x_strength;
      ఒకవేళ (x_scale < 0) x_జరుపు = -x_జరుపు;
      ఒకవేళ (embolden_లోపల_ఉంచు)
	extents->x_bearing = hb_clamp_గమ్యం<hb_position_t> ((int64_t) extents->x_bearing - x_జరుపు / 2);
      extents->వెడల్పు = hb_clamp_గమ్యం<hb_position_t> ((int64_t) extents->వెడల్పు + x_జరుపు);
    }
  }


  /* Public getters */

  HB_అంతర్గత తార్కిక has_func (గుర్తులేని పూర్ణాంకం i);
  HB_అంతర్గత తార్కిక has_func_సమితి (గుర్తులేని పూర్ణాంకం i);

  /* has_* ... */
#నిర్వచించు HB_FONT_FUNC_IMPLEMENT(get_,name) \
  తార్కిక \
  has_##name##_func () \
  { \
    hb_font_funcs_t *funcs = ఇది->klass; \
    గుర్తులేని పూర్ణాంకం i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / పరిమాణం (funcs->get.array[0]); \
    తిరిగి has_func (i); \
  } \
  తార్కిక \
  has_##name##_func_set () \
  { \
    hb_font_funcs_t *funcs = ఇది->klass; \
    గుర్తులేని పూర్ణాంకం i = offsetof (hb_font_funcs_t::get_t::get_funcs_t, name) / పరిమాణం (funcs->get.array[0]); \
    తిరిగి has_func_సమితి (i); \
  }
  HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_FONT_FUNC_IMPLEMENT

  hb_bool_t get_font_h_extents (hb_font_extents_t *extents,
				తార్కిక synthetic = నిజం)
  {
    hb_memset (extents, 0, పరిమాణం (*extents));
    తార్కిక ret = klass->get.f.font_h_extents (ఇది, user_దత్తాంశం,
					    extents,
					    !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->font_h_extents);

    ఒకవేళ (synthetic && ret)
    {
      /* Embolden */
      పూర్ణాంకం y_జరుపు = y_scale < 0 ? -y_strength : y_strength;
      extents->ascender += y_జరుపు;
    }

    తిరిగి ret;
  }
  hb_bool_t get_font_v_extents (hb_font_extents_t *extents,
				తార్కిక synthetic = నిజం)
  {
    hb_memset (extents, 0, పరిమాణం (*extents));
    తార్కిక ret = klass->get.f.font_v_extents (ఇది, user_దత్తాంశం,
					    extents,
					    !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->font_v_extents);

    ఒకవేళ (synthetic && ret)
    {
      /* Embolden */
      పూర్ణాంకం x_జరుపు = x_scale < 0 ? -x_strength : x_strength;
      ఒకవేళ (embolden_లోపల_ఉంచు)
      {
	extents->ascender += x_జరుపు / 2;
	extents->descender -= x_జరుపు - x_జరుపు / 2;
      }
      లేకపోతే
	extents->ascender += x_జరుపు;
    }

    తిరిగి ret;
  }

  తార్కిక has_glyph (hb_codepoint_t unicode)
  {
    hb_codepoint_t glyph;
    తిరిగి get_nominal_glyph (unicode, &glyph);
  }

  hb_bool_t get_nominal_glyph (hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       hb_codepoint_t not_దొరికింది = 0)
  {
    *glyph = not_దొరికింది;
    తిరిగి klass->get.f.nominal_glyph (ఇది, user_దత్తాంశం,
				       unicode, glyph,
				       !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->nominal_glyph);
  }
  గుర్తులేని పూర్ణాంకం get_nominal_glyphs (గుర్తులేని పూర్ణాంకం లెక్క,
				   స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
				   గుర్తులేని పూర్ణాంకం unicode_stride,
				   hb_codepoint_t *మొదటి_glyph,
				   గుర్తులేని పూర్ణాంకం glyph_stride)
  {
    తిరిగి klass->get.f.nominal_glyphs (ఇది, user_దత్తాంశం,
					లెక్క,
					మొదటి_unicode, unicode_stride,
					మొదటి_glyph, glyph_stride,
					!klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->nominal_glyphs);
  }

  hb_bool_t get_variation_glyph (hb_codepoint_t unicode, hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 hb_codepoint_t not_దొరికింది = 0)
  {
    *glyph = not_దొరికింది;
    తిరిగి klass->get.f.variation_glyph (ఇది, user_దత్తాంశం,
					 unicode, variation_selector, glyph,
					 !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->variation_glyph);
  }

  hb_position_t get_glyph_h_advance (hb_codepoint_t glyph,
				     తార్కిక synthetic = నిజం)
  {
    hb_position_t advance = klass->get.f.glyph_h_advance (ఇది, user_దత్తాంశం,
							  glyph,
							  !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_h_advance);

    ఒకవేళ (synthetic && x_strength && !embolden_లోపల_ఉంచు)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      advance += advance ? strength : 0;
    }

    తిరిగి advance;
  }

  hb_position_t get_glyph_v_advance (hb_codepoint_t glyph,
				     తార్కిక synthetic = నిజం)
  {
    hb_position_t advance = klass->get.f.glyph_v_advance (ఇది, user_దత్తాంశం,
							  glyph,
							  !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_v_advance);

    ఒకవేళ (synthetic && y_strength && !embolden_లోపల_ఉంచు)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      advance += advance ? strength : 0;
    }

    తిరిగి advance;
  }

  శూన్య get_glyph_h_advances (గుర్తులేని పూర్ణాంకం లెక్క,
			     స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			     గుర్తులేని పూర్ణాంకం glyph_stride,
			     hb_position_t *మొదటి_advance,
			     గుర్తులేని పూర్ణాంకం advance_stride,
			     తార్కిక synthetic = నిజం)
  {
    klass->get.f.glyph_h_advances (ఇది, user_దత్తాంశం,
				   లెక్క,
				   మొదటి_glyph, glyph_stride,
				   మొదటి_advance, advance_stride,
				   !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_h_advances);

    ఒకవేళ (synthetic && x_strength && !embolden_లోపల_ఉంచు)
    {
      /* Embolden */
      hb_position_t strength = x_scale >= 0 ? x_strength : -x_strength;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      {
	*మొదటి_advance += *మొదటి_advance ? strength : 0;
	మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
      }
    }
  }

  శూన్య get_glyph_v_advances (గుర్తులేని పూర్ణాంకం లెక్క,
			     స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			     గుర్తులేని పూర్ణాంకం glyph_stride,
			     hb_position_t *మొదటి_advance,
			     గుర్తులేని పూర్ణాంకం advance_stride,
			     తార్కిక synthetic = నిజం)
  {
    klass->get.f.glyph_v_advances (ఇది, user_దత్తాంశం,
				   లెక్క,
				   మొదటి_glyph, glyph_stride,
				   మొదటి_advance, advance_stride,
				   !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_v_advances);

    ఒకవేళ (synthetic && y_strength && !embolden_లోపల_ఉంచు)
    {
      /* Embolden */
      hb_position_t strength = y_scale >= 0 ? y_strength : -y_strength;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      {
	*మొదటి_advance += *మొదటి_advance ? strength : 0;
	మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
      }
    }
  }

  hb_bool_t get_glyph_h_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				తార్కిక synthetic = నిజం)
  {
    *x = *y = 0;
    తార్కిక ret = klass->get.f.glyph_h_origin (ఇది, user_దత్తాంశం,
					    glyph, x, y,
					    !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_h_origin);

    ఒకవేళ (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      ఒకవేళ (!embolden_లోపల_ఉంచు)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    తిరిగి ret;
  }

  hb_bool_t get_glyph_v_origin (hb_codepoint_t glyph,
				hb_position_t *x, hb_position_t *y,
				తార్కిక synthetic = నిజం)
  {
    *x = *y = 0;
    తార్కిక ret = klass->get.f.glyph_v_origin (ఇది, user_దత్తాంశం,
					    glyph, x, y,
					    !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_v_origin);

    ఒకవేళ (synthetic && ret)
    {
      /* Slant is ignored as it does not affect glyph origin */

      /* Embolden */
      ఒకవేళ (!embolden_లోపల_ఉంచు)
      {
        *x += x_scale < 0 ? -x_strength : x_strength;
	*y += y_scale < 0 ? -y_strength : y_strength;
      }
    }

    తిరిగి ret;
  }

  hb_bool_t get_glyph_h_origins (గుర్తులేని పూర్ణాంకం లెక్క,
				 స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
				 గుర్తులేని పూర్ణాంకం glyph_stride,
				 hb_position_t *మొదటి_x,
				 గుర్తులేని పూర్ణాంకం x_stride,
				 hb_position_t *మొదటి_y,
				 గుర్తులేని పూర్ణాంకం y_stride,
				 తార్కిక synthetic = నిజం)

  {
    తార్కిక ret = klass->get.f.glyph_h_origins (ఇది, user_దత్తాంశం,
					     లెక్క,
					     మొదటి_glyph, glyph_stride,
					     మొదటి_x, x_stride, మొదటి_y, y_stride,
					     !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_h_origins);

    ఒకవేళ (synthetic && ret)
    {
      hb_position_t x_జరుపు = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_జరుపు = y_scale < 0 ? -y_strength : y_strength;
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	ఒకవేళ (!embolden_లోపల_ఉంచు)
	{
	  *మొదటి_x += x_జరుపు;
	  *మొదటి_y += y_జరుపు;
	}
	మొదటి_x = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_x, x_stride);
	మొదటి_y = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_y, y_stride);
      }
    }

    తిరిగి ret;
  }

  hb_bool_t get_glyph_v_origins (గుర్తులేని పూర్ణాంకం లెక్క,
				 స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
				 గుర్తులేని పూర్ణాంకం glyph_stride,
				 hb_position_t *మొదటి_x,
				 గుర్తులేని పూర్ణాంకం x_stride,
				 hb_position_t *మొదటి_y,
				 గుర్తులేని పూర్ణాంకం y_stride,
				 తార్కిక synthetic = నిజం)

  {
    తార్కిక ret = klass->get.f.glyph_v_origins (ఇది, user_దత్తాంశం,
					     లెక్క,
					     మొదటి_glyph, glyph_stride,
					     మొదటి_x, x_stride, మొదటి_y, y_stride,
					     !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_v_origins);

    ఒకవేళ (synthetic && is_synthetic && ret)
    {
      hb_position_t x_జరుపు = x_scale < 0 ? -x_strength : x_strength;
      hb_position_t y_జరుపు = y_scale < 0 ? -y_strength : y_strength;
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      {
	/* Slant is ignored as it does not affect glyph origin */

	/* Embolden */
	ఒకవేళ (!embolden_లోపల_ఉంచు)
	{
	  *మొదటి_x += x_జరుపు;
	  *మొదటి_y += y_జరుపు;
	}
	మొదటి_x = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_x, x_stride);
	మొదటి_y = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_y, y_stride);
      }
    }

    తిరిగి ret;
  }

  hb_position_t get_glyph_h_kerning (hb_codepoint_t ఎడమ_glyph,
				     hb_codepoint_t కుడి_glyph)
  {
#నిర్వచితమైతే HB_DISABLE_DEPRECATED
    తిరిగి 0;
#లేకపోతే
    తిరిగి klass->get.f.glyph_h_kerning (ఇది, user_దత్తాంశం,
					 ఎడమ_glyph, కుడి_glyph,
					 !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_h_kerning);
#అయితే_ముగింపు
  }

  hb_position_t get_glyph_v_kerning (hb_codepoint_t శిఖరం_glyph,
				     hb_codepoint_t bottom_glyph)
  {
#నిర్వచితమైతే HB_DISABLE_DEPRECATED
    తిరిగి 0;
#లేకపోతే
    తిరిగి klass->get.f.glyph_v_kerning (ఇది, user_దత్తాంశం,
					 శిఖరం_glyph, bottom_glyph,
					 !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_v_kerning);
#అయితే_ముగింపు
  }

  hb_bool_t get_glyph_extents (hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       తార్కిక synthetic = నిజం)
  {
    hb_memset (extents, 0, పరిమాణం (*extents));

    /* This is rather messy, but necessary. */

    ఒకవేళ (!synthetic)
    {
      తిరిగి klass->get.f.glyph_extents (ఇది, user_దత్తాంశం,
					 glyph,
					 extents,
					 !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_extents);
    }
    ఒకవేళ (!is_synthetic &&
	klass->get.f.glyph_extents (ఇది, user_దత్తాంశం,
				    glyph,
				    extents,
				    !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_extents))
      తిరిగి నిజం;

    /* Try getting extents from paint(), then draw(), *then* get_extents()
     * and apply synthetic settings in the last case. */

#నిర్వచితం_కాకపోతే HB_NO_PAINT
    hb_paint_extents_context_t paint_extents;
    ఒకవేళ (paint_glyph_or_fail (glyph,
			     hb_paint_extents_get_funcs (), &paint_extents,
			     0, 0))
    {
      *extents = paint_extents.get_extents ().గమ్యం_glyph_extents ();
      తిరిగి నిజం;
    }
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_DRAW
    hb_extents_t<> draw_extents;
    ఒకవేళ (draw_glyph_or_fail (glyph,
			    hb_draw_extents_get_funcs (), &draw_extents))
    {
      *extents = draw_extents.గమ్యం_glyph_extents ();
      తిరిగి నిజం;
    }
#అయితే_ముగింపు

    తార్కిక ret = klass->get.f.glyph_extents (ఇది, user_దత్తాంశం,
					   glyph,
					   extents,
					   !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_extents);
    ఒకవేళ (ret)
      synthetic_glyph_extents (extents);

    తిరిగి ret;
  }

  hb_bool_t get_glyph_contour_బిందువు (hb_codepoint_t glyph, గుర్తులేని పూర్ణాంకం బిందువు_సూచీ,
				     hb_position_t *x, hb_position_t *y,
				     తార్కిక synthetic = నిజం)
  {
    *x = *y = 0;
    తార్కిక ret = klass->get.f.glyph_contour_బిందువు (ఇది, user_దత్తాంశం,
						 glyph, బిందువు_సూచీ,
						 x, y,
						 !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_contour_బిందువు);

    ఒకవేళ (synthetic && ret)
    {
      /* Slant */
      ఒకవేళ (slant_xy)
        *x += roundf (*y * slant_xy);

      /* Embolden */
      ఒకవేళ (!embolden_లోపల_ఉంచు)
      {
	పూర్ణాంకం x_జరుపు = x_scale < 0 ? -x_strength : x_strength;
	*x += x_జరుపు;
      }
    }

    తిరిగి ret;
  }

  hb_bool_t get_glyph_name (hb_codepoint_t glyph,
			    అక్షరం *name, గుర్తులేని పూర్ణాంకం కొలత)
  {
    ఒకవేళ (కొలత) *name = '\0';
    తిరిగి klass->get.f.glyph_name (ఇది, user_దత్తాంశం,
				    glyph,
				    name, కొలత,
				    !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_name);
  }

  hb_bool_t get_glyph_from_name (స్థిరరాశి అక్షరం *name, పూర్ణాంకం len, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph)
  {
    *glyph = 0;
    ఒకవేళ (len == -1) len = అక్షరశ్రేణి_పొడవు (name);
    తిరిగి klass->get.f.glyph_from_name (ఇది, user_దత్తాంశం,
					 name, len,
					 glyph,
					 !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->glyph_from_name);
  }

  తార్కిక draw_glyph_or_fail (hb_codepoint_t glyph,
			   hb_draw_funcs_t *draw_funcs, శూన్య *draw_దత్తాంశం,
			   తార్కిక synthetic = నిజం)
  {
#నిర్వచితం_కాకపోతే HB_NO_DRAW
#నిర్వచితం_కాకపోతే HB_NO_OUTLINE
    తార్కిక embolden = x_strength || y_strength;
    తార్కిక slanted = slant_xy;
    synthetic = synthetic && (embolden || slanted);
#లేకపోతే
    synthetic = అబద్ధం;
#అయితే_ముగింపు

    ఒకవేళ (!synthetic)
    {
      తిరిగి klass->get.f.draw_glyph_or_fail (ఇది, user_దత్తాంశం,
					      glyph,
					      draw_funcs, draw_దత్తాంశం,
					      !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->draw_glyph_or_fail);
    }

#నిర్వచితం_కాకపోతే HB_NO_OUTLINE

    hb_outline_t outline;
    outline.budget_మిగిలిన = draw_funcs->get_budget_మిగిలిన_ptr (draw_దత్తాంశం);
    ఒకవేళ (!klass->get.f.draw_glyph_or_fail (ఇది, user_దత్తాంశం,
					  glyph,
					  hb_outline_recording_pen_get_funcs (), &outline,
					  !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->draw_glyph_or_fail))
      తిరిగి అబద్ధం;

    // Slant before embolden; produces nicer results.

    ఒకవేళ (slanted)
    {
      hb_position_t xo = 0, yo = 0;
      get_glyph_h_origin (glyph, &xo, &yo, అబద్ధం);
      outline.translate (-xo, -yo);
      outline.slant (slant_xy);
      outline.translate (xo, yo);
    }

    ఒకవేళ (embolden)
    {
      దశాంశం x_జరుపు = embolden_లోపల_ఉంచు ? 0 : (దశాంశం) x_strength / 2;
      దశాంశం y_జరుపు = (దశాంశం) y_strength / 2;
      ఒకవేళ (x_scale < 0) x_జరుపు = -x_జరుపు;
      ఒకవేళ (y_scale < 0) y_జరుపు = -y_జరుపు;
      outline.embolden (x_strength, y_strength, x_జరుపు, y_జరుపు);
    }

    outline.replay (draw_funcs, draw_దత్తాంశం);

    తిరిగి నిజం;
#అయితే_ముగింపు
#అయితే_ముగింపు
    తిరిగి అబద్ధం;
  }

  తార్కిక paint_glyph_or_fail (hb_codepoint_t glyph,
			    hb_paint_funcs_t *paint_funcs, శూన్య *paint_దత్తాంశం,
			    గుర్తులేని పూర్ణాంకం palette,
			    hb_color_t foreground,
			    తార్కిక synthetic = నిజం)
  {
#నిర్వచితం_కాకపోతే HB_NO_PAINT
    /* Slant */
    ఒకవేళ (synthetic && slant_xy)
      hb_paint_నెట్టు_transform (paint_funcs, paint_దత్తాంశం,
			       1.f, 0.f,
			       slant_xy, 1.f,
			       0.f, 0.f);

    తార్కిక ret = klass->get.f.paint_glyph_or_fail (ఇది, user_దత్తాంశం,
						 glyph,
						 paint_funcs, paint_దత్తాంశం,
						 palette, foreground,
						 !klass->user_దత్తాంశం ? శూన్యసూచిక : klass->user_దత్తాంశం->paint_glyph_or_fail);

    ఒకవేళ (synthetic && slant_xy)
      hb_paint_తీయి_transform (paint_funcs, paint_దత్తాంశం);

    తిరిగి ret;
#అయితే_ముగింపు
    తిరిగి అబద్ధం;
  }

  /* A bit higher-level, and with fallback */

  HB_అంతర్గత
  శూన్య paint_glyph (hb_codepoint_t glyph,
		    hb_paint_funcs_t *paint_funcs, శూన్య *paint_దత్తాంశం,
		    గుర్తులేని పూర్ణాంకం palette,
		    hb_color_t foreground);

  శూన్య get_h_extents_with_fallback (hb_font_extents_t *extents)
  {
    ఒకవేళ (!get_font_h_extents (extents))
    {
      extents->ascender = y_scale * .8;
      extents->descender = extents->ascender - y_scale;
      extents->పంక్తి_gap = 0;
    }
  }
  శూన్య get_v_extents_with_fallback (hb_font_extents_t *extents)
  {
    ఒకవేళ (!get_font_v_extents (extents))
    {
      extents->ascender = x_scale / 2;
      extents->descender = extents->ascender - x_scale;
      extents->పంక్తి_gap = 0;
    }
  }

  శూన్య get_extents_for_direction (hb_direction_t direction,
				  hb_font_extents_t *extents)
  {
    ఒకవేళ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_h_extents_with_fallback (extents);
    లేకపోతే
      get_v_extents_with_fallback (extents);
  }

  శూన్య get_glyph_advance_for_direction (hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    *x = *y = 0;
    ఒకవేళ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      *x = get_glyph_h_advance (glyph);
    లేకపోతే
      *y = get_glyph_v_advance (glyph);
  }
  శూన్య get_glyph_advances_for_direction (hb_direction_t direction,
					 గుర్తులేని పూర్ణాంకం లెక్క,
					 స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
					 గుర్తులేని glyph_stride,
					 hb_position_t *మొదటి_advance,
					 గుర్తులేని advance_stride)
  {
    ఒకవేళ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_advances (లెక్క, మొదటి_glyph, glyph_stride, మొదటి_advance, advance_stride);
    లేకపోతే
      get_glyph_v_advances (లెక్క, మొదటి_glyph, glyph_stride, మొదటి_advance, advance_stride);
  }

  శూన్య apply_స్థానభ్రంశం (hb_position_t *x, hb_position_t *y,
		     hb_position_t dx, hb_position_t dy,
		     గుర్తుగల mult)
  {
    ప్రతిజ్ఞా (mult == -1 || mult == +1);

    *x += dx * mult;
    *y += dy * mult;
  }
  శూన్య కూడు_స్థానభ్రంశం (hb_position_t *x, hb_position_t *y,
		   hb_position_t dx, hb_position_t dy)
  {
    *x += dx;
    *y += dy;
  }
  శూన్య వ్యవకలనం_స్థానభ్రంశం (hb_position_t *x, hb_position_t *y,
			hb_position_t dx, hb_position_t dy)
  {
    *x -= dx;
    *y -= dy;
  }

  శూన్య guess_v_origin_minus_h_origin (hb_codepoint_t glyph,
				      hb_position_t *x, hb_position_t *y)
  {
    *x = get_glyph_h_advance (glyph) / 2;

    hb_font_extents_t extents;
    get_h_extents_with_fallback (&extents);
    *y = extents.ascender;
  }

  శూన్య apply_glyph_h_origins_with_fallback (hb_బఫరు_t *buf, పూర్ణాంకం mult)
  {
    తార్కిక has_ascender = అబద్ధం;
    hb_position_t ascender = 0;

    నిర్మాణం { hb_position_t x, y; } origins[32];

    గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = 0;
    గుర్తులేని పూర్ణాంకం లెక్క = buf->len;
    అయితే (స్థానభ్రంశం < లెక్క)
    {
      గుర్తులేని n = hb_min (లెక్క - స్థానభ్రంశం, ARRAY_నిడివి (origins));
      ఒకవేళ (!get_glyph_h_origins (n,
				&buf->info[స్థానభ్రంశం].codepoint, పరిమాణం (hb_glyph_info_t),
				&origins[0].x, పరిమాణం (origins[0]),
				&origins[0].y, పరిమాణం (origins[0])))
      {
        ఒకవేళ (get_glyph_v_origins (n,
				  &buf->info[స్థానభ్రంశం].codepoint, పరిమాణం (hb_glyph_info_t),
				  &origins[0].x, పరిమాణం (origins[0]),
				  &origins[0].y, పరిమాణం (origins[0])))
	{
	  ఒకవేళ (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = నిజం;
	  }

	  /* We got the v_origins, adjust them to h_origins. */
	  కొరకు (గుర్తులేని j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[స్థానభ్రంశం + j].codepoint;
	    origins[j].x -= get_glyph_h_advance (glyph) / 2;
	    origins[j].y -= ascender;
	  }
	}
	లేకపోతే
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      ప్రతిజ్ఞా (mult == -1 || mult == +1 || mult == 0);
      ఒకవేళ (mult == +1)
        కొరకు (గుర్తులేని j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[స్థానభ్రంశం + j];
	  కూడు_స్థానభ్రంశం (&pos->x_స్థానభ్రంశం_2, &pos->y_స్థానభ్రంశం_2,
		      origins[j].x, origins[j].y);
	}
      లేకపోతే ఒకవేళ (mult == -1)
	కొరకు (గుర్తులేని j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[స్థానభ్రంశం + j];
	  వ్యవకలనం_స్థానభ్రంశం (&pos->x_స్థానభ్రంశం_2, &pos->y_స్థానభ్రంశం_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      స్థానభ్రంశం += n;
    }
  }
  శూన్య apply_glyph_v_origins_with_fallback (hb_బఫరు_t *buf, పూర్ణాంకం mult)
  {
    తార్కిక has_ascender = అబద్ధం;
    hb_position_t ascender = 0;

    నిర్మాణం { hb_position_t x, y; } origins[32];

    గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = 0;
    గుర్తులేని పూర్ణాంకం లెక్క = buf->len;
    అయితే (స్థానభ్రంశం < లెక్క)
    {
      గుర్తులేని n = hb_min (లెక్క - స్థానభ్రంశం, ARRAY_నిడివి (origins));
      ఒకవేళ (!get_glyph_v_origins (n,
				&buf->info[స్థానభ్రంశం].codepoint, పరిమాణం (hb_glyph_info_t),
				&origins[0].x, పరిమాణం (origins[0]),
				&origins[0].y, పరిమాణం (origins[0])))
      {
	ఒకవేళ (get_glyph_h_origins (n,
				 &buf->info[స్థానభ్రంశం].codepoint, పరిమాణం (hb_glyph_info_t),
				 &origins[0].x, పరిమాణం (origins[0]),
				 &origins[0].y, పరిమాణం (origins[0])))
	{
	  ఒకవేళ (!has_ascender)
	  {
	    hb_font_extents_t extents;
	    get_h_extents_with_fallback (&extents);
	    ascender = extents.ascender;
	    has_ascender = నిజం;
	  }

	  /* We got the h_origins, adjust them to v_origins. */
	  కొరకు (గుర్తులేని j = 0; j < n; j++)
	  {
	    hb_codepoint_t glyph = buf->info[స్థానభ్రంశం + j].codepoint;
	    origins[j].x += get_glyph_h_advance (glyph) / 2;
	    origins[j].y += ascender;
	  }
	}
	లేకపోతే
	{
	  mult = 0; /* Indicates all origins[].x and origins[].y values are 0, therefore we can skip adjusting offsets below */
	}
      }

      ప్రతిజ్ఞా (mult == -1 || mult == +1 || mult == 0);
      ఒకవేళ (mult == +1)
        కొరకు (గుర్తులేని j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[స్థానభ్రంశం + j];
	  కూడు_స్థానభ్రంశం (&pos->x_స్థానభ్రంశం_2, &pos->y_స్థానభ్రంశం_2,
		      origins[j].x, origins[j].y);
	}
      లేకపోతే ఒకవేళ (mult == -1)
	కొరకు (గుర్తులేని j = 0; j < n; j++)
	{
	  hb_glyph_position_t *pos = &buf->pos[స్థానభ్రంశం + j];
	  వ్యవకలనం_స్థానభ్రంశం (&pos->x_స్థానభ్రంశం_2, &pos->y_స్థానభ్రంశం_2,
			   origins[j].x, origins[j].y);
	}
      /* else if (mult == 0) --> Do nothing */

      స్థానభ్రంశం += n;
    }
  }

  శూన్య get_glyph_h_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    ఒకవేళ (!get_glyph_h_origin (glyph, x, y) &&
	 get_glyph_v_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      వ్యవకలనం_స్థానభ్రంశం (x, y, dx, dy);
    }
  }
  శూన్య get_glyph_v_origin_with_fallback (hb_codepoint_t glyph,
					 hb_position_t *x, hb_position_t *y)
  {
    ఒకవేళ (!get_glyph_v_origin (glyph, x, y) &&
	 get_glyph_h_origin (glyph, x, y))
    {
      hb_position_t dx, dy;
      guess_v_origin_minus_h_origin (glyph, &dx, &dy);
      కూడు_స్థానభ్రంశం (x, y, dx, dy);
    }
  }

  శూన్య get_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    ఒకవేళ (likely (HB_DIRECTION_IS_HORIZONTAL (direction)))
      get_glyph_h_origin_with_fallback (glyph, x, y);
    లేకపోతే
      get_glyph_v_origin_with_fallback (glyph, x, y);
  }

  శూన్య కూడు_glyph_h_origins (hb_బఫరు_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, +1);
  }
  శూన్య కూడు_glyph_v_origins (hb_బఫరు_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, +1);
  }
  శూన్య కూడు_glyph_origin_for_direction (hb_codepoint_t glyph,
				       hb_direction_t direction,
				       hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    కూడు_స్థానభ్రంశం (x, y, origin_x, origin_y);
  }

  శూన్య వ్యవకలనం_glyph_h_origins (hb_బఫరు_t *buf)
  {
    apply_glyph_h_origins_with_fallback (buf, -1);
  }
  శూన్య వ్యవకలనం_glyph_v_origins (hb_బఫరు_t *buf)
  {
    apply_glyph_v_origins_with_fallback (buf, -1);
  }
  శూన్య వ్యవకలనం_glyph_origin_for_direction (hb_codepoint_t glyph,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y)
  {
    hb_position_t origin_x, origin_y;
    get_glyph_origin_for_direction (glyph, direction, &origin_x, &origin_y);
    వ్యవకలనం_స్థానభ్రంశం (x, y, origin_x, origin_y);
  }

  శూన్య get_glyph_kerning_for_direction (hb_codepoint_t మొదటి_glyph, hb_codepoint_t రెండవ_glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y)
  {
    ఒకవేళ (likely (HB_DIRECTION_IS_HORIZONTAL (direction))) {
      *y = 0;
      *x = get_glyph_h_kerning (మొదటి_glyph, రెండవ_glyph);
    } లేకపోతే {
      *x = 0;
      *y = get_glyph_v_kerning (మొదటి_glyph, రెండవ_glyph);
    }
  }

  hb_bool_t get_glyph_extents_for_origin (hb_codepoint_t glyph,
					  hb_direction_t direction,
					  hb_glyph_extents_t *extents)
  {
    hb_bool_t ret = get_glyph_extents (glyph, extents);

    ఒకవేళ (ret)
      వ్యవకలనం_glyph_origin_for_direction (glyph, direction, &extents->x_bearing, &extents->y_bearing);

    తిరిగి ret;
  }

  hb_bool_t get_glyph_contour_బిందువు_for_origin (hb_codepoint_t glyph, గుర్తులేని పూర్ణాంకం బిందువు_సూచీ,
						hb_direction_t direction,
						hb_position_t *x, hb_position_t *y)
  {
    hb_bool_t ret = get_glyph_contour_బిందువు (glyph, బిందువు_సూచీ, x, y);

    ఒకవేళ (ret)
      వ్యవకలనం_glyph_origin_for_direction (glyph, direction, x, y);

    తిరిగి ret;
  }

  /* Generates gidDDD if glyph has no name. */
  శూన్య
  glyph_గమ్యం_string (hb_codepoint_t glyph,
		   అక్షరం *s, గుర్తులేని పూర్ణాంకం కొలత)
  {
    ఒకవేళ (get_glyph_name (glyph, s, కొలత)) తిరిగి;

    ఒకవేళ (కొలత && snprintf (s, కొలత, "gid%" PRIu32, glyph) < 0)
      *s = '\0';
  }

  /* Parses gidDDD and uniUUUU strings automatically. */
  hb_bool_t
  glyph_from_string (స్థిరరాశి అక్షరం *s, పూర్ణాంకం len, /* -1 means nul-terminated */
		     hb_codepoint_t *glyph)
  {
    ఒకవేళ (get_glyph_from_name (s, len, glyph)) తిరిగి నిజం;

    ఒకవేళ (len == -1) len = అక్షరశ్రేణి_పొడవు (s);

    /* Straight glyph index. */
    ఒకవేళ (hb_codepoint_parse (s, len, 10, glyph))
      తిరిగి నిజం;

    ఒకవేళ (len > 3)
    {
      /* gidDDD syntax for glyph indices. */
      ఒకవేళ (0 == అక్షరశ్రేణి_పోలిక (s, "gid", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 10, glyph))
	తిరిగి నిజం;

      /* uniUUUU and other Unicode character indices. */
      hb_codepoint_t unichar;
      ఒకవేళ (0 == అక్షరశ్రేణి_పోలిక (s, "uni", 3) &&
	  hb_codepoint_parse (s + 3, len - 3, 16, &unichar) &&
	  get_nominal_glyph (unichar, glyph))
	తిరిగి నిజం;
    }

    తిరిగి అబద్ధం;
  }

  శూన్య మారింది ()
  {
    దశాంశం upem = face->get_upem ();

    x_multf = x_scale / upem;
    y_multf = y_scale / upem;
    x_mult = (int64_t) x_scale * 0x10000 / upem;
    y_mult = (int64_t) y_scale * 0x10000 / upem;

    is_synthetic =  x_embolden || y_embolden || slant;

    x_strength = roundf (నిరపేక్ష (x_scale) * x_embolden);
    y_strength = roundf (నిరపేక్ష (y_scale) * y_embolden);

    slant_xy = y_scale ? slant * x_scale / y_scale : 0.f;

    దత్తాంశం.fini ();

    serial++;
  }

  hb_position_t em_mult (int32_t v, int64_t mult)
  { తిరిగి (hb_position_t) ((v * mult + 32768) >> 16); }
  hb_position_t em_multf (దశాంశం v, దశాంశం mult)
  { తిరిగి (hb_position_t) roundf (em_fmultf (v, mult)); }
  దశాంశం em_fmultf (దశాంశం v, దశాంశం mult)
  { తిరిగి v * mult; }
  దశాంశం em_fmult (int32_t v, దశాంశం mult)
  { తిరిగి (దశాంశం) v * mult; }
};
DECLARE_NULL_INSTANCE (hb_font_t);


#అయితే_ముగింపు /* HB_FONT_HH */
