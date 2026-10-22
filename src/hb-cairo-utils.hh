/*
 * Copyright © 2022  Red Hat, Inc
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
 * Google Author(s): Matthias Clasen
 */

#నిర్వచితం_కాకపోతే HB_CAIRO_UTILS_H
#నిర్వచించు HB_CAIRO_UTILS_H

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-cairo.h"


నిర్మాణం hb_cairo_budget_t
{
  /* Cairo serializes glyph-cache population for a scaled font.  Keeping the
   * owner there lets recursive color-glyph callbacks share one session. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_మిగిలిన = HB_BUDGET_GLYPH;
  గుర్తులేని పూర్ణాంకం చురుకైన = 0;

  శూన్య enter ()
  {
    ఒకవేళ (!చురుకైన++)
      recharge ();
  }

  శూన్య leave ()
  {
    ప్రతిజ్ఞా (చురుకైన);
    చురుకైన--;
  }

  శూన్య recharge ()
  {
    budget_మిగిలిన = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }
};

నిర్మాణం hb_cairo_context_t
{
  hb_cairo_context_t (cairo_scaled_font_t *scaled_font_,
		      cairo_t *cr_,
		      hb_map_t *color_cache_,
		      hb_cairo_budget_t *budget_) :
    scaled_font (scaled_font_), cr (cr_), color_cache (color_cache_),
    budget (budget_)
  {
    budget->enter ();
  }

  ~hb_cairo_context_t ()
  {
    budget->leave ();
  }

  తార్కిక సమితి_budget (int64_t value)
  {
    budget->budget = value;
    budget->recharge ();
    తిరిగి నిజం;
  }

  int64_t get_budget () స్థిరరాశి { తిరిగి budget->budget; }
  int64_t *get_budget_మిగిలిన () { తిరిగి &budget->budget_మిగిలిన; }

  తార్కిక spend (గుర్తులేని పూర్ణాంకం ఖర్చు, గుర్తులేని పూర్ణాంకం mult = 1)
  { తిరిగి hb_budget_spend (budget->budget_మిగిలిన, ఖర్చు, mult); }

  cairo_scaled_font_t *scaled_font;
  cairo_t *cr;
  hb_map_t *color_cache;
  hb_cairo_budget_t *budget;
};

స్థిర అంతఃవరుస cairo_operator_t
hb_paint_సంయుక్త_బహుళకం_గమ్యం_cairo (hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
{
  ఎంపిక (బహుళకం)
    {
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_CLEAR: తిరిగి CAIRO_OPERATOR_CLEAR;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC: తిరిగి CAIRO_OPERATOR_ఆధారం;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST: తిరిగి CAIRO_OPERATOR_DEST;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_OVER: తిరిగి CAIRO_OPERATOR_OVER;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_OVER: తిరిగి CAIRO_OPERATOR_DEST_OVER;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_లోపల: తిరిగి CAIRO_OPERATOR_లోపల;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_లోపల: తిరిగి CAIRO_OPERATOR_DEST_లోపల;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_బహిర్గతం: తిరిగి CAIRO_OPERATOR_బహిర్గతం;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_బహిర్గతం: తిరిగి CAIRO_OPERATOR_DEST_బహిర్గతం;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_ATOP: తిరిగి CAIRO_OPERATOR_ATOP;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_ATOP: తిరిగి CAIRO_OPERATOR_DEST_ATOP;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_XOR: తిరిగి CAIRO_OPERATOR_XOR;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_PLUS: తిరిగి CAIRO_OPERATOR_కూడు;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SCREEN: తిరిగి CAIRO_OPERATOR_SCREEN;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_OVERLAY: తిరిగి CAIRO_OPERATOR_OVERLAY;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DARKEN: తిరిగి CAIRO_OPERATOR_DARKEN;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_LIGHTEN: తిరిగి CAIRO_OPERATOR_LIGHTEN;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_COLOR_DODGE: తిరిగి CAIRO_OPERATOR_COLOR_DODGE;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_COLOR_BURN: తిరిగి CAIRO_OPERATOR_COLOR_BURN;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_HARD_LIGHT: తిరిగి CAIRO_OPERATOR_HARD_LIGHT;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SOFT_LIGHT: తిరిగి CAIRO_OPERATOR_SOFT_LIGHT;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DIFFERENCE: తిరిగి CAIRO_OPERATOR_DIFFERENCE;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_EXCLUSION: తిరిగి CAIRO_OPERATOR_EXCLUSION;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_MULTIPLY: తిరిగి CAIRO_OPERATOR_MULTIPLY;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_HUE: తిరిగి CAIRO_OPERATOR_HSL_HUE;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_SATURATION: తిరిగి CAIRO_OPERATOR_HSL_SATURATION;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_COLOR: తిరిగి CAIRO_OPERATOR_HSL_COLOR;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_LUMINOSITY: తిరిగి CAIRO_OPERATOR_HSL_LUMINOSITY;
    అప్రమేయ: తిరిగి CAIRO_OPERATOR_CLEAR;
    }
}

HB_అంతర్గత శూన్య
hb_cairo_సమితి_ఆధారం_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color);

HB_అంతర్గత hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     గుర్తులేని వెడల్పు,
			     గుర్తులేని ఎత్తు,
			     hb_ట్యాగ్_t format,
			     దశాంశం slant,
			     hb_glyph_extents_t *extents);

HB_అంతర్గత శూన్య
hb_cairo_paint_రేఖీయ_gradient (hb_cairo_context_t *c,
				 hb_color_పంక్తి_t *color_పంక్తి,
				 దశాంశం x0, దశాంశం y0,
				 దశాంశం x1, దశాంశం y1,
				 దశాంశం x2, దశాంశం y2);

HB_అంతర్గత శూన్య
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_పంక్తి_t *color_పంక్తి,
				 దశాంశం x0, దశాంశం y0, దశాంశం r0,
				 దశాంశం x1, దశాంశం y1, దశాంశం r1);

HB_అంతర్గత శూన్య
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_పంక్తి_t *color_పంక్తి,
				దశాంశం x0, దశాంశం y0,
				దశాంశం ప్రారంభం_angle, దశాంశం చివర_angle);


#అయితే_ముగింపు /* HB_CAIRO_UTILS_H */
