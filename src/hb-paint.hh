/*
 * Copyright © 2022 Matthias Clasen
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
 */

#నిర్వచితం_కాకపోతే HB_PAINT_HH
#నిర్వచించు HB_PAINT_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-face.hh"
#అన్తర్భావ "hb-font.hh"
#అన్తర్భావ "hb-geometry.hh"

#నిర్వచించు HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS \
  HB_PAINT_FUNC_IMPLEMENT (నెట్టు_transform) \
  HB_PAINT_FUNC_IMPLEMENT (తీయి_transform) \
  HB_PAINT_FUNC_IMPLEMENT (color_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (నెట్టు_clip_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (నెట్టు_clip_rectangle) \
  HB_PAINT_FUNC_IMPLEMENT (నెట్టు_clip_మార్గం_ప్రారంభం) \
  HB_PAINT_FUNC_IMPLEMENT (నెట్టు_clip_మార్గం_చివర) \
  HB_PAINT_FUNC_IMPLEMENT (తీయి_clip) \
  HB_PAINT_FUNC_IMPLEMENT (color) \
  HB_PAINT_FUNC_IMPLEMENT (image) \
  HB_PAINT_FUNC_IMPLEMENT (రేఖీయ_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (radial_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (sweep_gradient) \
  HB_PAINT_FUNC_IMPLEMENT (నెట్టు_సమూహం) \
  HB_PAINT_FUNC_IMPLEMENT (నెట్టు_సమూహం_for) \
  HB_PAINT_FUNC_IMPLEMENT (తీయి_సమూహం) \
  HB_PAINT_FUNC_IMPLEMENT (custom_palette_color) \
  HB_PAINT_FUNC_IMPLEMENT (fill_glyph) \
  HB_PAINT_FUNC_IMPLEMENT (సమితి_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget) \
  HB_PAINT_FUNC_IMPLEMENT (get_budget_మిగిలిన) \
  /* ^--- Add new callbacks here */

నిర్మాణం hb_paint_funcs_t
{
  hb_object_శీర్షిక_t శీర్షిక;

  నిర్మాణం {
#నిర్వచించు HB_PAINT_FUNC_IMPLEMENT(name) hb_paint_##name##_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_PAINT_FUNC_IMPLEMENT
  } func;

  నిర్మాణం {
#నిర్వచించు HB_PAINT_FUNC_IMPLEMENT(name) శూన్య *name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_PAINT_FUNC_IMPLEMENT
  } *user_దత్తాంశం;

  నిర్మాణం {
#నిర్వచించు HB_PAINT_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_PAINT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_PAINT_FUNC_IMPLEMENT
  } *destroy;

  శూన్య నెట్టు_transform (శూన్య *paint_దత్తాంశం,
                       దశాంశం xx, దశాంశం yx,
                       దశాంశం xy, దశాంశం yy,
                       దశాంశం dx, దశాంశం dy)
  {
    // Handle -0.f to avoid -0.f == 0.f in the transform matrix.
    ఒకవేళ (dx == -0.f) dx = 0.f;
    ఒకవేళ (dy == -0.f) dy = 0.f;
    func.నెట్టు_transform (ఇది, paint_దత్తాంశం,
                         xx, yx, xy, yy, dx, dy,
                         !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->నెట్టు_transform); }
  శూన్య తీయి_transform (శూన్య *paint_దత్తాంశం)
  { func.తీయి_transform (ఇది, paint_దత్తాంశం,
                        !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->తీయి_transform); }
  తార్కిక color_glyph (శూన్య *paint_దత్తాంశం,
                    hb_codepoint_t glyph,
                    hb_font_t *font)
  { తిరిగి func.color_glyph (ఇది, paint_దత్తాంశం,
                             glyph,
                             font,
                             !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->color_glyph); }
  శూన్య నెట్టు_clip_glyph (శూన్య *paint_దత్తాంశం,
                        hb_codepoint_t glyph,
                        hb_font_t *font)
  { func.నెట్టు_clip_glyph (ఇది, paint_దత్తాంశం,
                          glyph,
                          font,
                          !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->నెట్టు_clip_glyph); }
  శూన్య నెట్టు_clip_rectangle (శూన్య *paint_దత్తాంశం,
                           దశాంశం xmin, దశాంశం ymin, దశాంశం xmax, దశాంశం ymax)
  { func.నెట్టు_clip_rectangle (ఇది, paint_దత్తాంశం,
                              xmin, ymin, xmax, ymax,
                              !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->నెట్టు_clip_rectangle); }
  hb_draw_funcs_t *నెట్టు_clip_మార్గం_ప్రారంభం (శూన్య *paint_దత్తాంశం,
                                         శూన్య **draw_దత్తాంశం)
  { తిరిగి func.నెట్టు_clip_మార్గం_ప్రారంభం (ఇది, paint_దత్తాంశం, draw_దత్తాంశం,
                                      !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->నెట్టు_clip_మార్గం_ప్రారంభం); }
  శూన్య నెట్టు_clip_మార్గం_చివర (శూన్య *paint_దత్తాంశం)
  { func.నెట్టు_clip_మార్గం_చివర (ఇది, paint_దత్తాంశం,
                             !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->నెట్టు_clip_మార్గం_చివర); }
  శూన్య తీయి_clip (శూన్య *paint_దత్తాంశం)
  { func.తీయి_clip (ఇది, paint_దత్తాంశం,
                   !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->తీయి_clip); }
  శూన్య color (శూన్య *paint_దత్తాంశం,
              hb_bool_t is_foreground,
              hb_color_t color)
  { func.color (ఇది, paint_దత్తాంశం,
                is_foreground, color,
                !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->color); }
  శూన్య fill_glyph (శూన్య *paint_దత్తాంశం,
                   hb_codepoint_t glyph,
                   hb_font_t *font,
                   hb_bool_t is_foreground,
                   hb_color_t color)
  { func.fill_glyph (ఇది, paint_దత్తాంశం,
                     glyph, font,
                     is_foreground, color,
                     !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->fill_glyph); }
  తార్కిక image (శూన్య *paint_దత్తాంశం,
              hb_blob_t *image,
              గుర్తులేని వెడల్పు, గుర్తులేని ఎత్తు,
              hb_ట్యాగ్_t format,
              దశాంశం slant,
              hb_glyph_extents_t *extents)
  { తిరిగి func.image (ఇది, paint_దత్తాంశం,
                       image, వెడల్పు, ఎత్తు, format, slant, extents,
                       !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->image); }
  శూన్య రేఖీయ_gradient (శూన్య *paint_దత్తాంశం,
                        hb_color_పంక్తి_t *color_పంక్తి,
                        దశాంశం x0, దశాంశం y0,
                        దశాంశం x1, దశాంశం y1,
                        దశాంశం x2, దశాంశం y2)
  { func.రేఖీయ_gradient (ఇది, paint_దత్తాంశం,
                          color_పంక్తి, x0, y0, x1, y1, x2, y2,
                          !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->రేఖీయ_gradient); }
  శూన్య radial_gradient (శూన్య *paint_దత్తాంశం,
                        hb_color_పంక్తి_t *color_పంక్తి,
                        దశాంశం x0, దశాంశం y0, దశాంశం r0,
                        దశాంశం x1, దశాంశం y1, దశాంశం r1)
  { func.radial_gradient (ఇది, paint_దత్తాంశం,
                          color_పంక్తి, x0, y0, r0, x1, y1, r1,
                          !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->radial_gradient); }
  శూన్య sweep_gradient (శూన్య *paint_దత్తాంశం,
                       hb_color_పంక్తి_t *color_పంక్తి,
                       దశాంశం x0, దశాంశం y0,
                       దశాంశం ప్రారంభం_angle,
                       దశాంశం చివర_angle)
  { func.sweep_gradient (ఇది, paint_దత్తాంశం,
                         color_పంక్తి, x0, y0, ప్రారంభం_angle, చివర_angle,
                         !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->sweep_gradient); }
  శూన్య నెట్టు_సమూహం (శూన్య *paint_దత్తాంశం)
  { func.నెట్టు_సమూహం (ఇది, paint_దత్తాంశం,
                     !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->నెట్టు_సమూహం); }
  శూన్య నెట్టు_సమూహం_for (శూన్య *paint_దత్తాంశం,
                       hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
  { func.నెట్టు_సమూహం_for (ఇది, paint_దత్తాంశం,
                         బహుళకం,
                         !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->నెట్టు_సమూహం_for); }
  శూన్య తీయి_సమూహం (శూన్య *paint_దత్తాంశం,
                  hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
  { func.తీయి_సమూహం (ఇది, paint_దత్తాంశం,
                    బహుళకం,
                    !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->తీయి_సమూహం); }
  తార్కిక custom_palette_color (శూన్య *paint_దత్తాంశం,
                             గుర్తులేని పూర్ణాంకం color_సూచీ,
                             hb_color_t *color)
  { తిరిగి func.custom_palette_color (ఇది, paint_దత్తాంశం,
                                      color_సూచీ,
                                      color,
                                      !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->custom_palette_color); }
  తార్కిక సమితి_budget (శూన్య *paint_దత్తాంశం, int64_t budget)
  {
    ఒకవేళ (budget < 0 && budget != HB_BUDGET_DEFAULT)
      budget = 0;
    తిరిగి func.సమితి_budget (ఇది, paint_దత్తాంశం, budget,
			    !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->సమితి_budget);
  }
  int64_t get_budget (శూన్య *paint_దత్తాంశం)
  { తిరిగి func.get_budget (ఇది, paint_దత్తాంశం,
			    !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->get_budget); }
  int64_t *get_budget_మిగిలిన_ptr (శూన్య *paint_దత్తాంశం)
  { తిరిగి func.get_budget_మిగిలిన (ఇది, paint_దత్తాంశం,
				      !user_దత్తాంశం ? శూన్యసూచిక : user_దత్తాంశం->get_budget_మిగిలిన); }


  /* Internal specializations. */

  శూన్య నెట్టు_font_transform (శూన్య *paint_దత్తాంశం,
                            స్థిరరాశి hb_font_t *font)
  {
    దశాంశం upem = font->face->get_upem ();
    పూర్ణాంకం xscale = font->x_scale, yscale = font->y_scale;

    నెట్టు_transform (paint_దత్తాంశం,
		    xscale/upem, 0,
		    0, yscale/upem,
		    0, 0);
  }

  శూన్య నెట్టు_inverse_font_transform (శూన్య *paint_దత్తాంశం,
                                    స్థిరరాశి hb_font_t *font)
  {
    దశాంశం upem = font->face->get_upem ();
    పూర్ణాంకం xscale = font->x_scale ? font->x_scale : upem;
    పూర్ణాంకం yscale = font->y_scale ? font->y_scale : upem;

    నెట్టు_transform (paint_దత్తాంశం,
		    upem/xscale, 0,
		    0, upem/yscale,
		    0, 0);
  }

  శూన్య నెట్టు_transform (శూన్య *paint_దత్తాంశం, hb_transform_t<దశాంశం> t)
  {
    నెట్టు_transform (paint_దత్తాంశం, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  }

  శూన్య నెట్టు_translate (శూన్య *paint_దత్తాంశం,
                       దశాంశం dx, దశాంశం dy)
  {
    నెట్టు_transform (paint_దత్తాంశం,
		    hb_transform_t<దశాంశం>::translation (dx, dy));
  }

  శూన్య నెట్టు_scale (శూన్య *paint_దత్తాంశం,
                   దశాంశం sx, దశాంశం sy)
  {
    నెట్టు_transform (paint_దత్తాంశం,
		    hb_transform_t<దశాంశం>::scaling (sx, sy));
  }
  శూన్య నెట్టు_scale_around_center (శూన్య *paint_దత్తాంశం,
				 దశాంశం sx, దశాంశం sy,
				 దశాంశం cx, దశాంశం cy)
  {
    నెట్టు_transform (paint_దత్తాంశం,
		    hb_transform_t<దశాంశం>::scaling_around_center (sx, sy, cx, cy));
  }

  శూన్య నెట్టు_rotate (శూన్య *paint_దత్తాంశం,
                    దశాంశం a)
  {
    నెట్టు_transform (paint_దత్తాంశం,
		    hb_transform_t<దశాంశం>::rotation (a * HB_PI));
  }

  శూన్య నెట్టు_rotate_around_center (శూన్య *paint_దత్తాంశం,
				  దశాంశం a,
				  దశాంశం cx, దశాంశం cy)
  {
    నెట్టు_transform (paint_దత్తాంశం,
		    hb_transform_t<దశాంశం>::rotation_around_center (a * HB_PI, cx, cy));
  }

  శూన్య నెట్టు_skew (శూన్య *paint_దత్తాంశం,
                  దశాంశం sx, దశాంశం sy)
  {
    నెట్టు_transform (paint_దత్తాంశం,
		    hb_transform_t<దశాంశం>::skewing (-sx * HB_PI, sy * HB_PI));
  }
  శూన్య నెట్టు_skew_around_center (శూన్య *paint_దత్తాంశం,
				దశాంశం sx, దశాంశం sy,
				దశాంశం cx, దశాంశం cy)
  {
    నెట్టు_transform (paint_దత్తాంశం,
		    hb_transform_t<దశాంశం>::skewing_around_center (-sx * HB_PI, sy * HB_PI, cx, cy));
  }
};
DECLARE_NULL_INSTANCE (hb_paint_funcs_t);


/* Linearly interpolate between two hb_color_t values, component-wise,
 * in byte-channel space with rounding. */
స్థిర అంతఃవరుస hb_color_t
hb_color_lerp (hb_color_t c0, hb_color_t c1, దశాంశం t)
{
  స్వయంచాలక lerp = [&] (గుర్తులేని జరుపు) -> గుర్తులేని {
    గుర్తులేని v0 = (c0 >> జరుపు) & 0xFF;
    గుర్తులేని v1 = (c1 >> జరుపు) & 0xFF;
    దశాంశం v = v0 + t * ((దశాంశం) v1 - (దశాంశం) v0) + 0.5f;
    /* t may be outside [0,1] (or NaN) for malformed gradients;
     * keep the float-to-unsigned conversion defined. */
    ఒకవేళ (!(v > 0.f)) తిరిగి 0;
    ఒకవేళ (v > 255.f) తిరిగి 255;
    తిరిగి (గుర్తులేని) v;
  };
  తిరిగి HB_COLOR (lerp (0), lerp (8), lerp (16), lerp (24));
}

#అయితే_ముగింపు /* HB_PAINT_HH */
