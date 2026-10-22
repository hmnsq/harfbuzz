/*
 * Copyright © 2022  Red Hat, Inc.
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
 * Red Hat Author(s): Matthias Clasen
 */

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_CAIRO

#అన్తర్భావ "hb-cairo.h"

#అన్తర్భావ "hb-cairo-utils.hh"

#అన్తర్భావ "hb-machinery.hh"
#అన్తర్భావ "hb-utf.hh"


/**
 * SECTION:hb-cairo
 * @title: hb-cairo
 * @short_description: Cairo integration
 * @include: hb-cairo.h
 *
 * Functions for using HarfBuzz with the cairo library.
 *
 * HarfBuzz supports using cairo for rendering.
 **/

స్థిర శూన్య
hb_cairo_move_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
		  శూన్య *draw_దత్తాంశం,
		  hb_draw_state_t *st HB_వాడని,
		  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
		  శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!c->spend (HB_BUDGET_1)))
    తిరిగి;

  cairo_move_గమ్యం (c->cr, (ద్విగుణ) గమ్యం_x, (ద్విగుణ) గమ్యం_y);
}

స్థిర శూన్య
hb_cairo_పంక్తి_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
		  శూన్య *draw_దత్తాంశం,
		  hb_draw_state_t *st HB_వాడని,
		  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
		  శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!c->spend (HB_BUDGET_1)))
    తిరిగి;

  cairo_పంక్తి_గమ్యం (c->cr, (ద్విగుణ) గమ్యం_x, (ద్విగుణ) గమ్యం_y);
}

స్థిర శూన్య
hb_cairo_cubic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
		   శూన్య *draw_దత్తాంశం,
		   hb_draw_state_t *st HB_వాడని,
		   దశాంశం control1_x, దశాంశం control1_y,
		   దశాంశం control2_x, దశాంశం control2_y,
		   దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
		   శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!c->spend (HB_BUDGET_1)))
    తిరిగి;

  cairo_వక్రం_గమ్యం (c->cr,
                  (ద్విగుణ) control1_x, (ద్విగుణ) control1_y,
                  (ద్విగుణ) control2_x, (ద్విగుణ) control2_y,
                  (ద్విగుణ) గమ్యం_x, (ద్విగుణ) గమ్యం_y);
}

స్థిర శూన్య
hb_cairo_close_మార్గం (hb_draw_funcs_t *dfuncs HB_వాడని,
		     శూన్య *draw_దత్తాంశం,
		     hb_draw_state_t *st HB_వాడని,
		     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!c->spend (HB_BUDGET_1)))
    తిరిగి;

  cairo_close_మార్గం (c->cr);
}

స్థిర hb_bool_t
hb_cairo_draw_సమితి_budget (hb_draw_funcs_t *dfuncs HB_వాడని,
			  శూన్య *draw_దత్తాంశం,
			  int64_t budget,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి ((hb_cairo_context_t *) draw_దత్తాంశం)->సమితి_budget (budget);
}

స్థిర int64_t
hb_cairo_draw_get_budget (hb_draw_funcs_t *dfuncs HB_వాడని,
			  శూన్య *draw_దత్తాంశం,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి ((hb_cairo_context_t *) draw_దత్తాంశం)->get_budget ();
}

స్థిర int64_t *
hb_cairo_draw_get_budget_మిగిలిన (hb_draw_funcs_t *dfuncs HB_వాడని,
				    శూన్య *draw_దత్తాంశం,
				    శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి ((hb_cairo_context_t *) draw_దత్తాంశం)->get_budget_మిగిలిన ();
}

స్థిర అంతఃవరుస శూన్య free_static_cairo_draw_funcs ();

స్థిర నిర్మాణం hb_cairo_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_cairo_draw_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_సమితి_move_గమ్యం_func (funcs, hb_cairo_move_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func (funcs, hb_cairo_పంక్తి_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_cubic_గమ్యం_func (funcs, hb_cairo_cubic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_close_మార్గం_func (funcs, hb_cairo_close_మార్గం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_సమితి_budget_func (funcs, hb_cairo_draw_సమితి_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_func (funcs, hb_cairo_draw_get_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_cairo_draw_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_draw_funcs);

    తిరిగి funcs;
  }
} static_cairo_draw_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_cairo_draw_funcs ()
{
  static_cairo_draw_funcs.free_instance ();
}

స్థిర hb_draw_funcs_t *
hb_cairo_draw_get_funcs ()
{
  తిరిగి static_cairo_draw_funcs.get_unconst ();
}


#నిర్వచితమైతే HAVE_CAIRO_USER_FONT_FACE_సమితి_RENDER_COLOR_GLYPH_FUNC

స్థిర శూన్య
hb_cairo_నెట్టు_transform (hb_paint_funcs_t *pfuncs HB_వాడని,
			 శూన్య *paint_దత్తాంశం,
			 దశాంశం xx, దశాంశం yx,
			 దశాంశం xy, దశాంశం yy,
			 దశాంశం dx, దశాంశం dy,
			 శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  cairo_మాతృక_t m;

  cairo_save (cr);
  cairo_మాతృక_init (&m, (ద్విగుణ) xx, (ద్విగుణ) yx,
                         (ద్విగుణ) xy, (ద్విగుణ) yy,
                         (ద్విగుణ) dx, (ద్విగుణ) dy);
  cairo_transform (cr, &m);
}

స్థిర శూన్య
hb_cairo_తీయి_transform (hb_paint_funcs_t *pfuncs HB_వాడని,
		        శూన్య *paint_దత్తాంశం,
		        శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

స్థిర శూన్య
hb_cairo_fill_glyph (hb_paint_funcs_t *pfuncs HB_వాడని,
		     శూన్య *paint_దత్తాంశం,
		     hb_codepoint_t glyph,
		     hb_font_t *font,
		     hb_bool_t use_foreground,
		     hb_color_t color,
		     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  cairo_save (cr);

  cairo_new_మార్గం (cr);
  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);
  cairo_close_మార్గం (cr);
  hb_cairo_సమితి_ఆధారం_color (c, use_foreground, color);
  cairo_fill (cr);

  cairo_restore (cr);
}

స్థిర hb_bool_t
hb_cairo_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_వాడని,
			    శూన్య *paint_దత్తాంశం,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  ఒకవేళ (unlikely (!c->spend (HB_BUDGET_1)))
    తిరిగి నిజం;

  cairo_save (cr);

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr, x_scale, -y_scale);

  cairo_glyph_t cairo_glyph = { glyph, 0, 0 };
  cairo_సమితి_scaled_font (cr, c->scaled_font);
  cairo_సమితి_font_కొలత (cr, 1);
  cairo_show_glyphs (cr, &cairo_glyph, 1);

  cairo_restore (cr);

  తిరిగి నిజం;
}

స్థిర శూన్య
hb_cairo_నెట్టు_clip_glyph (hb_paint_funcs_t *pfuncs HB_వాడని,
			  శూన్య *paint_దత్తాంశం,
			  hb_codepoint_t glyph,
			  hb_font_t *font,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_మార్గం (cr);

  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);

  cairo_close_మార్గం (cr);
  cairo_clip (cr);
}

స్థిర శూన్య
hb_cairo_నెట్టు_clip_rectangle (hb_paint_funcs_t *pfuncs HB_వాడని,
			      శూన్య *paint_దత్తాంశం,
			      దశాంశం xmin, దశాంశం ymin, దశాంశం xmax, దశాంశం ymax,
			      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_rectangle (cr,
                   (ద్విగుణ) xmin, (ద్విగుణ) ymin,
                   (ద్విగుణ) (xmax - xmin), (ద్విగుణ) (ymax - ymin));
  cairo_clip (cr);
}

స్థిర hb_draw_funcs_t *
hb_cairo_నెట్టు_clip_మార్గం_ప్రారంభం (hb_paint_funcs_t *pfuncs HB_వాడని,
			       శూన్య *paint_దత్తాంశం,
			       శూన్య **draw_దత్తాంశం,
			       శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_మార్గం (cr);
  *draw_దత్తాంశం = c;
  తిరిగి hb_cairo_draw_get_funcs ();
}

స్థిర శూన్య
hb_cairo_నెట్టు_clip_మార్గం_చివర (hb_paint_funcs_t *pfuncs HB_వాడని,
			     శూన్య *paint_దత్తాంశం,
			     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  cairo_close_మార్గం (cr);
  cairo_clip (cr);
}

స్థిర శూన్య
hb_cairo_తీయి_clip (hb_paint_funcs_t *pfuncs HB_వాడని,
		   శూన్య *paint_దత్తాంశం,
		   శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

స్థిర శూన్య
hb_cairo_నెట్టు_సమూహం (hb_paint_funcs_t *pfuncs HB_వాడని,
		     శూన్య *paint_దత్తాంశం,
		     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_నెట్టు_సమూహం (cr);
}

స్థిర శూన్య
hb_cairo_తీయి_సమూహం (hb_paint_funcs_t *pfuncs HB_వాడని,
		    శూన్య *paint_దత్తాంశం,
		    hb_paint_సంయుక్త_బహుళకం_t బహుళకం,
		    శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  cairo_తీయి_సమూహం_గమ్యం_ఆధారం (cr);
  cairo_సమితి_operator (cr, hb_paint_సంయుక్త_బహుళకం_గమ్యం_cairo (బహుళకం));
  cairo_paint (cr);

  cairo_restore (cr);
}

స్థిర శూన్య
hb_cairo_paint_color (hb_paint_funcs_t *pfuncs HB_వాడని,
		      శూన్య *paint_దత్తాంశం,
		      hb_bool_t use_foreground,
		      hb_color_t color,
		      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

  hb_cairo_సమితి_ఆధారం_color (c, use_foreground, color);
  cairo_paint (cr);
}

స్థిర hb_bool_t
hb_cairo_paint_image (hb_paint_funcs_t *pfuncs HB_వాడని,
		      శూన్య *paint_దత్తాంశం,
		      hb_blob_t *blob,
		      గుర్తులేని వెడల్పు,
		      గుర్తులేని ఎత్తు,
		      hb_ట్యాగ్_t format,
		      దశాంశం slant,
		      hb_glyph_extents_t *extents,
		      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;

  తిరిగి _hb_cairo_paint_glyph_image (c, blob, వెడల్పు, ఎత్తు, format, slant, extents);
}

స్థిర శూన్య
hb_cairo_paint_రేఖీయ_gradient_2 (hb_paint_funcs_t *pfuncs HB_వాడని,
				శూన్య *paint_దత్తాంశం,
				hb_color_పంక్తి_t *color_పంక్తి,
				దశాంశం x0, దశాంశం y0,
				దశాంశం x1, దశాంశం y1,
				దశాంశం x2, దశాంశం y2,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;

  hb_cairo_paint_రేఖీయ_gradient (c, color_పంక్తి, x0, y0, x1, y1, x2, y2);
}

స్థిర శూన్య
hb_cairo_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_వాడని,
				శూన్య *paint_దత్తాంశం,
				hb_color_పంక్తి_t *color_పంక్తి,
				దశాంశం x0, దశాంశం y0, దశాంశం r0,
				దశాంశం x1, దశాంశం y1, దశాంశం r1,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;

  _hb_cairo_paint_radial_gradient (c, color_పంక్తి, x0, y0, r0, x1, y1, r1);
}

స్థిర శూన్య
hb_cairo_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_వాడని,
			       శూన్య *paint_దత్తాంశం,
			       hb_color_పంక్తి_t *color_పంక్తి,
			       దశాంశం x0, దశాంశం y0,
			       దశాంశం ప్రారంభం_angle, దశాంశం చివర_angle,
			       శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;

  _hb_cairo_paint_sweep_gradient (c, color_పంక్తి, x0, y0, ప్రారంభం_angle, చివర_angle);
}

స్థిర స్థిరరాశి cairo_user_దత్తాంశం_కీలకం_t color_cache_కీలకం = {0};

స్థిర శూన్య
_hb_cairo_destroy_map (శూన్య *p)
{
  hb_map_destroy ((hb_map_t *) p);
}

స్థిర hb_bool_t
hb_cairo_paint_custom_palette_color (hb_paint_funcs_t *funcs,
                                     శూన్య *paint_దత్తాంశం,
                                     గుర్తులేని పూర్ణాంకం color_సూచీ,
                                     hb_color_t *color,
                                     శూన్య *user_దత్తాంశం HB_వాడని)
{
#నిర్వచితమైతే HAVE_CAIRO_FONT_OPTIONS_GET_CUSTOM_PALETTE_COLOR
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_దత్తాంశం;
  cairo_t *cr = c->cr;

#నిర్వచించు HB_DEADBEEF HB_ట్యాగ్(0xDE,0xAD,0xBE,0xEF)

  hb_map_t *color_cache = c->color_cache;
  hb_codepoint_t *v;
  ఒకవేళ (likely (color_cache && color_cache->has (color_సూచీ, &v)))
  {
    ఒకవేళ (*v == HB_DEADBEEF)
      తిరిగి అబద్ధం;
    *color = *v;
    తిరిగి నిజం;
  }

  cairo_font_options_t *options;
  ద్విగుణ red, green, blue, alpha;

  options = cairo_font_options_create ();
  cairo_get_font_options (cr, options);
  ఒకవేళ (CAIRO_స్థితి_SUCCESS ==
      cairo_font_options_get_custom_palette_color (options, color_సూచీ,
                                                   &red, &green, &blue, &alpha))
  {
    cairo_font_options_destroy (options);
    *color = HB_COLOR (round (255 * blue),
		       round (255 * green),
		       round (255 * red),
		       round (255 * alpha));

    ఒకవేళ (likely (color_cache && *color != HB_DEADBEEF))
      color_cache->సమితి (color_సూచీ, *color);

    తిరిగి నిజం;
  }
  cairo_font_options_destroy (options);

  ఒకవేళ (likely (color_cache))
    color_cache->సమితి (color_సూచీ, HB_DEADBEEF);

#నిర్వచనం_రద్దు HB_DEADBEEF

#అయితే_ముగింపు

  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_cairo_paint_సమితి_budget (hb_paint_funcs_t *pfuncs HB_వాడని,
			   శూన్య *paint_దత్తాంశం,
			   int64_t budget,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి ((hb_cairo_context_t *) paint_దత్తాంశం)->సమితి_budget (budget);
}

స్థిర int64_t
hb_cairo_paint_get_budget (hb_paint_funcs_t *pfuncs HB_వాడని,
			   శూన్య *paint_దత్తాంశం,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి ((hb_cairo_context_t *) paint_దత్తాంశం)->get_budget ();
}

స్థిర int64_t *
hb_cairo_paint_get_budget_మిగిలిన (hb_paint_funcs_t *pfuncs HB_వాడని,
				     శూన్య *paint_దత్తాంశం,
				     శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి ((hb_cairo_context_t *) paint_దత్తాంశం)->get_budget_మిగిలిన ();
}

స్థిర అంతఃవరుస శూన్య free_static_cairo_paint_funcs ();

స్థిర నిర్మాణం hb_cairo_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_cairo_paint_funcs_lazy_loader_t>
{
  స్థిర hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_సమితి_నెట్టు_transform_func (funcs, hb_cairo_నెట్టు_transform, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_transform_func (funcs, hb_cairo_తీయి_transform, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_fill_glyph_func (funcs, hb_cairo_fill_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_color_glyph_func (funcs, hb_cairo_paint_color_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_glyph_func (funcs, hb_cairo_నెట్టు_clip_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_rectangle_func (funcs, hb_cairo_నెట్టు_clip_rectangle, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_మార్గం_ప్రారంభం_func (funcs, hb_cairo_నెట్టు_clip_మార్గం_ప్రారంభం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_మార్గం_చివర_func (funcs, hb_cairo_నెట్టు_clip_మార్గం_చివర, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_clip_func (funcs, hb_cairo_తీయి_clip, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_సమూహం_func (funcs, hb_cairo_నెట్టు_సమూహం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_సమూహం_func (funcs, hb_cairo_తీయి_సమూహం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_color_func (funcs, hb_cairo_paint_color, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_image_func (funcs, hb_cairo_paint_image, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_రేఖీయ_gradient_func (funcs, hb_cairo_paint_రేఖీయ_gradient_2, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_radial_gradient_func (funcs, hb_cairo_paint_radial_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_sweep_gradient_func (funcs, hb_cairo_paint_sweep_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_custom_palette_color_func (funcs, hb_cairo_paint_custom_palette_color, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_సమితి_budget_func (funcs, hb_cairo_paint_సమితి_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_func (funcs, hb_cairo_paint_get_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_cairo_paint_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_paint_funcs);

    తిరిగి funcs;
  }
} static_cairo_paint_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_cairo_paint_funcs ()
{
  static_cairo_paint_funcs.free_instance ();
}

స్థిర hb_paint_funcs_t *
hb_cairo_paint_get_funcs ()
{
  తిరిగి static_cairo_paint_funcs.get_unconst ();
}
#అయితే_ముగింపు

స్థిర స్థిరరాశి cairo_user_దత్తాంశం_కీలకం_t hb_cairo_face_user_దత్తాంశం_కీలకం = {0};
స్థిర స్థిరరాశి cairo_user_దత్తాంశం_కీలకం_t hb_cairo_font_user_దత్తాంశం_కీలకం = {0};
స్థిర స్థిరరాశి cairo_user_దత్తాంశం_కీలకం_t hb_cairo_font_init_func_user_దత్తాంశం_కీలకం = {0};
స్థిర స్థిరరాశి cairo_user_దత్తాంశం_కీలకం_t hb_cairo_font_init_user_దత్తాంశం_user_దత్తాంశం_కీలకం = {0};
స్థిర స్థిరరాశి cairo_user_దత్తాంశం_కీలకం_t hb_cairo_scale_గుణకం_user_దత్తాంశం_కీలకం = {0};
స్థిర స్థిరరాశి cairo_user_దత్తాంశం_కీలకం_t hb_cairo_budget_user_దత్తాంశం_కీలకం = {0};

స్థిర శూన్య hb_cairo_face_destroy (శూన్య *p) { hb_face_destroy ((hb_face_t *) p); }
స్థిర శూన్య hb_cairo_font_destroy (శూన్య *p) { hb_font_destroy ((hb_font_t *) p); }
స్థిర శూన్య
hb_cairo_budget_destroy (శూన్య *p)
{
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) p;
  budget->~hb_cairo_budget_t ();
  hb_free (budget);
}

స్థిర cairo_స్థితి_t
hb_cairo_init_scaled_font (cairo_scaled_font_t  *scaled_font,
			   cairo_t              *cr HB_వాడని,
			   cairo_font_extents_t *extents)
{
  cairo_font_face_t *font_face = cairo_scaled_font_get_font_face (scaled_font);

  hb_font_t *font = (hb_font_t *) cairo_font_face_get_user_దత్తాంశం (font_face,
								 &hb_cairo_font_user_దత్తాంశం_కీలకం);

  ఒకవేళ (!font)
  {
    hb_face_t *face = (hb_face_t *) cairo_font_face_get_user_దత్తాంశం (font_face,
								   &hb_cairo_face_user_దత్తాంశం_కీలకం);
    font = hb_font_create (face);

#ఒకవేళ !defined(HB_NO_VAR) && CAIRO_సంస్కరణ >= CAIRO_సంస్కరణ_ENCODE(1,16,0)
    cairo_font_options_t *font_options = cairo_font_options_create ();

    // Set variations
    cairo_scaled_font_get_font_options (scaled_font, font_options);
    స్థిరరాశి అక్షరం *variations = cairo_font_options_get_variations (font_options);
    hb_సదిశ_t<hb_variation_t> vars;
    స్థిరరాశి అక్షరం *p = variations;
    అయితే (p && *p)
    {
      స్థిరరాశి అక్షరం *చివర = strpbrk ((అక్షరం *) p, ", ");
      hb_variation_t var;
      ఒకవేళ (hb_variation_from_string (p, చివర ? చివర - p : -1, &var))
	vars.push (var);
      p = చివర ? చివర + 1 : శూన్యసూచిక;
    }
    hb_font_సమితి_variations (font, &vars[0], vars.నిడివి);

    cairo_font_options_destroy (font_options);
#అయితే_ముగింపు

    // Set scale; Note: should NOT set slant, or we'll double-slant.
    గుర్తులేని scale_గుణకం = hb_cairo_font_face_get_scale_గుణకం (font_face);
    ఒకవేళ (scale_గుణకం)
    {
      cairo_మాతృక_t font_మాతృక;
      cairo_scaled_font_get_scale_మాతృక (scaled_font, &font_మాతృక);
      hb_font_సమితి_scale (font,
			 hb_clamp_గమ్యం<int32_t> (round (font_మాతృక.xx * scale_గుణకం)),
			 hb_clamp_గమ్యం<int32_t> (round (font_మాతృక.yy * scale_గుణకం)));
    }

    స్వయంచాలక *init_func = (hb_cairo_font_init_func_t)
		      cairo_font_face_get_user_దత్తాంశం (font_face,
						     &hb_cairo_font_init_func_user_దత్తాంశం_కీలకం);
    ఒకవేళ (init_func)
    {
      శూన్య *user_దత్తాంశం = cairo_font_face_get_user_దత్తాంశం (font_face,
						       &hb_cairo_font_init_user_దత్తాంశం_user_దత్తాంశం_కీలకం);
      font = init_func (font, scaled_font, user_దత్తాంశం);
    }

    hb_font_make_immutable (font);
  }

  cairo_scaled_font_సమితి_user_దత్తాంశం (scaled_font,
				   &hb_cairo_font_user_దత్తాంశం_కీలకం,
				   (శూన్య *) hb_font_reference (font),
				   hb_cairo_font_destroy);

  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) hb_malloc (పరిమాణం (hb_cairo_budget_t));
  ఒకవేళ (unlikely (!budget))
    తిరిగి CAIRO_స్థితి_NO_జ్ఞాపకం;
  కొత్త (budget) hb_cairo_budget_t ();
  cairo_స్థితి_t స్థితి_2 = cairo_scaled_font_సమితి_user_దత్తాంశం (scaled_font,
						   &hb_cairo_budget_user_దత్తాంశం_కీలకం,
						   budget,
						   hb_cairo_budget_destroy);
  ఒకవేళ (unlikely (స్థితి_2 != CAIRO_స్థితి_SUCCESS))
  {
    hb_cairo_budget_destroy (budget);
    తిరిగి స్థితి_2;
  }

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_font_extents_t hb_extents;
  hb_font_get_h_extents (font, &hb_extents);

  extents->ascent  = (ద్విగుణ)  hb_extents.ascender  / y_scale;
  extents->descent = -(ద్విగుణ) hb_extents.descender / y_scale;
  extents->ఎత్తు  = extents->ascent + extents->descent;

#నిర్వచితమైతే HAVE_CAIRO_USER_FONT_FACE_సమితి_RENDER_COLOR_GLYPH_FUNC
  hb_map_t *color_cache = hb_map_create ();
  ఒకవేళ (unlikely (CAIRO_స్థితి_SUCCESS != cairo_scaled_font_సమితి_user_దత్తాంశం (scaled_font,
									 &color_cache_కీలకం,
									 color_cache,
									 _hb_cairo_destroy_map)))
    hb_map_destroy (color_cache);
#అయితే_ముగింపు

  తిరిగి CAIRO_స్థితి_SUCCESS;
}

స్థిర cairo_స్థితి_t
hb_cairo_పాఠ్యం_గమ్యం_glyphs (cairo_scaled_font_t        *scaled_font,
			 స్థిరరాశి అక్షరం	            *utf8,
			 పూర్ణాంకం		             utf8_len,
			 cairo_glyph_t	           **glyphs,
			 పూర్ణాంకం		            *అంకె_glyphs,
			 cairo_పాఠ్యం_cluster_t      **clusters,
			 పూర్ణాంకం		            *అంకె_clusters,
			 cairo_పాఠ్యం_cluster_flags_t *cluster_flags)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_దత్తాంశం (scaled_font,
								   &hb_cairo_font_user_దత్తాంశం_కీలకం);

  hb_బఫరు_t *బఫరు = hb_బఫరు_create ();
  hb_బఫరు_కూడు_utf8 (బఫరు, utf8, utf8_len, 0, utf8_len);
  hb_బఫరు_guess_segment_properties (బఫరు);
  hb_shape (font, బఫరు, శూన్యసూచిక, 0);

  పూర్ణాంకం x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_cairo_glyphs_from_బఫరు (బఫరు,
			       నిజం,
			       x_scale, y_scale,
			       0., 0.,
			       utf8, utf8_len,
			       glyphs, (గుర్తులేని *) అంకె_glyphs,
			       clusters, (గుర్తులేని *) అంకె_clusters,
			       cluster_flags);

  hb_బఫరు_destroy (బఫరు);

  తిరిగి CAIRO_స్థితి_SUCCESS;
}

స్థిర cairo_స్థితి_t
hb_cairo_render_glyph (cairo_scaled_font_t  *scaled_font,
		       గుర్తులేని దీర్ఘ         glyph,
		       cairo_t              *cr,
		       cairo_పాఠ్యం_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_దత్తాంశం (scaled_font,
								   &hb_cairo_font_user_దత్తాంశం_కీలకం);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_దత్తాంశం (scaled_font,
									     &hb_cairo_budget_user_దత్తాంశం_కీలకం);
  ఒకవేళ (unlikely (!budget))
    తిరిగి CAIRO_స్థితి_NO_జ్ఞాపకం;
  hb_cairo_context_t c {scaled_font, cr, శూన్యసూచిక, budget};

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));
  ఒకవేళ (hb_font_draw_glyph_or_fail (font, glyph, hb_cairo_draw_get_funcs (), &c))
    cairo_fill (cr);

  // If draw fails, we still return SUCCESS, as we want empty drawing, not
  // setting the cairo object into error.
  తిరిగి CAIRO_స్థితి_SUCCESS;
}

#నిర్వచితమైతే HAVE_CAIRO_USER_FONT_FACE_సమితి_RENDER_COLOR_GLYPH_FUNC

స్థిర cairo_స్థితి_t
hb_cairo_render_color_glyph (cairo_scaled_font_t  *scaled_font,
			     గుర్తులేని దీర్ఘ         glyph,
			     cairo_t              *cr,
			     cairo_పాఠ్యం_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_దత్తాంశం (scaled_font,
								   &hb_cairo_font_user_దత్తాంశం_కీలకం);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_దత్తాంశం (scaled_font,
									     &hb_cairo_budget_user_దత్తాంశం_కీలకం);
  ఒకవేళ (unlikely (!budget))
    తిరిగి CAIRO_స్థితి_NO_జ్ఞాపకం;

  గుర్తులేని పూర్ణాంకం palette = 0;
#నిర్వచితమైతే CAIRO_COLOR_PALETTE_DEFAULT
  cairo_font_options_t *options = cairo_font_options_create ();
  cairo_scaled_font_get_font_options (scaled_font, options);
  palette = cairo_font_options_get_color_palette (options);
  cairo_font_options_destroy (options);
#అయితే_ముగింపు

  hb_color_t color = HB_COLOR (0, 0, 0, 255);
  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));

  hb_cairo_context_t c {scaled_font,
			cr,
			(hb_map_t *) cairo_scaled_font_get_user_దత్తాంశం (scaled_font, &color_cache_కీలకం),
			budget};

  /* Synthesizing variant: mono glyphs render here too via the
   * fill_glyph foreground fallback inside hb_font_paint_glyph.
   * Callers that want the cheaper outline path for mono fonts set
   * CAIRO_COLOR_MODE_NO_COLOR on the font options instead. */
  hb_font_paint_glyph (font, glyph, hb_cairo_paint_get_funcs (), &c, palette, color);

  తిరిగి CAIRO_స్థితి_SUCCESS;
}

#అయితే_ముగింపు

స్థిర cairo_font_face_t *
user_font_face_create (hb_face_t *face)
{
  cairo_font_face_t *cairo_face;

  cairo_face = cairo_user_font_face_create ();
  cairo_user_font_face_సమితి_init_func (cairo_face, hb_cairo_init_scaled_font);
  cairo_user_font_face_సమితి_పాఠ్యం_గమ్యం_glyphs_func (cairo_face, hb_cairo_పాఠ్యం_గమ్యం_glyphs);
  cairo_user_font_face_సమితి_render_glyph_func (cairo_face, hb_cairo_render_glyph);
#నిర్వచితమైతే HAVE_CAIRO_USER_FONT_FACE_సమితి_RENDER_COLOR_GLYPH_FUNC
  cairo_user_font_face_సమితి_render_color_glyph_func (cairo_face, hb_cairo_render_color_glyph);
#అయితే_ముగింపు

  ఒకవేళ (unlikely (CAIRO_స్థితి_SUCCESS != cairo_font_face_సమితి_user_దత్తాంశం (cairo_face,
								       &hb_cairo_face_user_దత్తాంశం_కీలకం,
								       (శూన్య *) hb_face_reference (face),
								       hb_cairo_face_destroy)))
    hb_face_destroy (face);

  తిరిగి cairo_face;
}

/**
 * hb_cairo_font_face_create_for_font:
 * @font: a #hb_font_t
 *
 * Creates a #cairo_font_face_t for rendering text according
 * to @font.
 *
 * Note that the scale of @font does not affect the rendering,
 * but the variations and slant that are set on @font do.
 *
 * Returns: (transfer full): a newly created #cairo_font_face_t
 *
 * Since: 7.0.0
 */
cairo_font_face_t *
hb_cairo_font_face_create_for_font (hb_font_t *font)
{
  hb_font_make_immutable (font);

  స్వయంచాలక *hb_face = hb_font_get_face (font);
  స్వయంచాలక *cairo_face =  user_font_face_create (hb_face);

  ఒకవేళ (unlikely (CAIRO_స్థితి_SUCCESS != cairo_font_face_సమితి_user_దత్తాంశం (cairo_face,
								       &hb_cairo_font_user_దత్తాంశం_కీలకం,
								       (శూన్య *) hb_font_reference (font),
								       hb_cairo_font_destroy)))
    hb_font_destroy (font);

  తిరిగి cairo_face;
}

/**
 * hb_cairo_font_face_get_font:
 * @font_face: a #cairo_font_face_t
 *
 * Gets the #hb_font_t that @font_face was created from.
 *
 * Returns: (nullable) (transfer none): the #hb_font_t that @font_face was created from
 *
 * Since: 7.0.0
 */
hb_font_t *
hb_cairo_font_face_get_font (cairo_font_face_t *font_face)
{
  తిరిగి (hb_font_t *) cairo_font_face_get_user_దత్తాంశం (font_face,
						      &hb_cairo_font_user_దత్తాంశం_కీలకం);
}

/**
 * hb_cairo_font_face_create_for_face:
 * @face: a #hb_face_t
 *
 * Creates a #cairo_font_face_t for rendering text according
 * to @face.
 *
 * Returns: (transfer full): a newly created #cairo_font_face_t
 *
 * Since: 7.0.0
 */
cairo_font_face_t *
hb_cairo_font_face_create_for_face (hb_face_t *face)
{
  hb_face_make_immutable (face);

  తిరిగి user_font_face_create (face);
}

/**
 * hb_cairo_font_face_get_face:
 * @font_face: a #cairo_font_face_t
 *
 * Gets the #hb_face_t associated with @font_face.
 *
 * Returns: (nullable) (transfer none): the #hb_face_t associated with @font_face
 *
 * Since: 7.0.0
 */
hb_face_t *
hb_cairo_font_face_get_face (cairo_font_face_t *font_face)
{
  తిరిగి (hb_face_t *) cairo_font_face_get_user_దత్తాంశం (font_face,
						      &hb_cairo_face_user_దత్తాంశం_కీలకం);
}

/**
 * hb_cairo_font_face_set_font_init_func:
 * @font_face: a #cairo_font_face_t
 * @func: The virtual method to use
 * @user_data: user data accompanying the method
 * @destroy: function to call when @user_data is not needed anymore
 *
 * Set the virtual method to be called when a cairo
 * face created using hb_cairo_font_face_create_for_face()
 * creates an #hb_font_t for a #cairo_scaled_font_t.
 *
 * Since: 7.0.0
 */
శూన్య
hb_cairo_font_face_సమితి_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       శూన్య *user_దత్తాంశం,
				       hb_destroy_func_t destroy)
{
  cairo_font_face_సమితి_user_దత్తాంశం (font_face,
				 &hb_cairo_font_init_func_user_దత్తాంశం_కీలకం,
				 (శూన్య *) func,
				 శూన్యసూచిక);
  ఒకవేళ (unlikely (CAIRO_స్థితి_SUCCESS != cairo_font_face_సమితి_user_దత్తాంశం (font_face,
								       &hb_cairo_font_init_user_దత్తాంశం_user_దత్తాంశం_కీలకం,
								       (శూన్య *) user_దత్తాంశం,
								       destroy)) && destroy)
  {
    destroy (user_దత్తాంశం);
    cairo_font_face_సమితి_user_దత్తాంశం (font_face,
				   &hb_cairo_font_init_func_user_దత్తాంశం_కీలకం,
				   శూన్యసూచిక,
				   శూన్యసూచిక);
  }
}

/**
 * hb_cairo_scaled_font_get_font:
 * @scaled_font: a #cairo_scaled_font_t
 *
 * Gets the #hb_font_t associated with @scaled_font.
 *
 * Returns: (nullable) (transfer none): the #hb_font_t associated with @scaled_font
 *
 * Since: 7.0.0
 */
hb_font_t *
hb_cairo_scaled_font_get_font (cairo_scaled_font_t *scaled_font)
{
  తిరిగి (hb_font_t *) cairo_scaled_font_get_user_దత్తాంశం (scaled_font, &hb_cairo_font_user_దత్తాంశం_కీలకం);
}


/**
 * hb_cairo_font_face_set_scale_factor:
 * @scale_factor: The scale factor to use. See below
 * @font_face: a #cairo_font_face_t
 *
 * Sets the scale factor of the @font_face. Default scale
 * factor is zero.
 *
 * When a #cairo_font_face_t is created from a #hb_face_t using
 * hb_cairo_font_face_create_for_face(), such face will create
 * #hb_font_t objects during scaled-font creation.  The scale
 * factor defines how the scale set on such #hb_font_t objects
 * relates to the font-matrix (as such font size) of the cairo
 * scaled-font.
 *
 * If the scale-factor is zero (default), then the scale of the
 * #hb_font_t object will be left at default, which is the UPEM
 * value of the respective #hb_face_t.
 *
 * If the scale-factor is set to non-zero, then the X and Y scale
 * of the #hb_font_t object will be respectively set to the
 * @scale_factor times the xx and yy elements of the scale-matrix
 * of the cairo scaled-font being created.
 *
 * When using the hb_cairo_glyphs_from_buffer() API to convert the
 * HarfBuzz glyph buffer that resulted from shaping with such a #hb_font_t,
 * if the scale-factor was non-zero, you can pass it directly to
 * that API as both X and Y scale factors.
 *
 * If the scale-factor was zero however, or the cairo face was
 * created using the alternative constructor
 * hb_cairo_font_face_create_for_font(), you need to calculate the
 * correct X/Y scale-factors to pass to hb_cairo_glyphs_from_buffer()
 * by dividing the #hb_font_t X/Y scale-factors by the
 * cairo scaled-font's scale-matrix XX/YY components respectively
 * and use those values.  Or if you know that relationship offhand
 * (because you set the scale of the #hb_font_t yourself), use
 * the conversion rate involved.
 *
 * Since: 7.0.0
 */
శూన్య
hb_cairo_font_face_సమితి_scale_గుణకం (cairo_font_face_t *font_face,
				     గుర్తులేని పూర్ణాంకం scale_గుణకం)
{
  cairo_font_face_సమితి_user_దత్తాంశం (font_face,
				 &hb_cairo_scale_గుణకం_user_దత్తాంశం_కీలకం,
				 (శూన్య *) (uintptr_t) scale_గుణకం,
				 శూన్యసూచిక);
}

/**
 * hb_cairo_font_face_get_scale_factor:
 * @font_face: a #cairo_font_face_t
 *
 * Gets the scale factor set on the @font_face. Defaults to zero.
 * See hb_cairo_font_face_set_scale_factor() for details.
 *
 * Returns: the scale factor of @font_face
 *
 * Since: 7.0.0
 */
గుర్తులేని పూర్ణాంకం
hb_cairo_font_face_get_scale_గుణకం (cairo_font_face_t *font_face)
{
  తిరిగి (గుర్తులేని పూర్ణాంకం) (uintptr_t)
	 cairo_font_face_get_user_దత్తాంశం (font_face,
					&hb_cairo_scale_గుణకం_user_దత్తాంశం_కీలకం);
}

/**
 * hb_cairo_glyphs_from_buffer:
 * @buffer: a #hb_buffer_t containing glyphs
 * @utf8_clusters: `true` if @buffer clusters are in bytes, instead of characters
 * @x_scale_factor: scale factor to divide #hb_position_t Y values by
 * @y_scale_factor: scale factor to divide #hb_position_t X values by
 * @x: X position to place first glyph
 * @y: Y position to place first glyph
 * @utf8: (nullable): the text that was shaped in @buffer
 * @utf8_len: the length of @utf8 in bytes
 * @glyphs: (out): return location for an array of #cairo_glyph_t
 * @num_glyphs: (inout): return location for the length of @glyphs
 * @clusters: (out) (nullable): return location for an array of cluster positions
 * @num_clusters: (inout) (nullable): return location for the length of @clusters
 * @cluster_flags: (out) (nullable): return location for cluster flags
 *
 * Extracts information from @buffer in a form that can be
 * passed to cairo_show_text_glyphs() or cairo_show_glyphs().
 * This API is modeled after cairo_scaled_font_text_to_glyphs() and
 * cairo_user_scaled_font_text_to_glyphs_func_t.
 *
 * The @num_glyphs argument should be preset to the number of glyph entries available
 * in the @glyphs buffer. If the @glyphs buffer is `NULL`, the value of
 * @num_glyphs must be zero.  If the provided glyph array is too short for
 * the conversion (or for convenience), a new glyph array may be allocated
 * using cairo_glyph_allocate() and placed in @glyphs.  Upon return,
 * @num_glyphs should contain the number of generated glyphs.  If the value
 * @glyphs points at has changed after the call, the caller will free the
 * allocated glyph array using cairo_glyph_free().  The caller will also free
 * the original value of @glyphs, so this function shouldn't do so.
 *
 * If @clusters is not `NULL`, then @num_clusters and @cluster_flags
 * should not be either, and @utf8 must be provided, and cluster
 * mapping will be computed. The semantics of how
 * cluster array allocation works is similar to the glyph array.  That is,
 * if @clusters initially points to a non-`NULL` value, that array may be used
 * as a cluster buffer, and @num_clusters points to the number of cluster
 * entries available there.  If the provided cluster array is too short for
 * the conversion (or for convenience), a new cluster array may be allocated
 * using cairo_text_cluster_allocate() and placed in @clusters.  In this case,
 * the original value of @clusters will still be freed by the caller.  Upon
 * return, @num_clusters will contain the number of generated clusters.
 * If the value @clusters points at has changed after the call, the caller
 * will free the allocated cluster array using cairo_text_cluster_free().
 *
 * See hb_cairo_font_face_set_scale_factor() for the details of
 * the @scale_factor argument.
 *
 * The returned @glyphs vector actually has `@num_glyphs + 1` entries in
 * it and the x,y values of the extra entry at the end add up the advance
 * x,y of all the glyphs in the @buffer.
 *
 * Since: 7.0.0
 */
శూన్య
hb_cairo_glyphs_from_బఫరు (hb_బఫరు_t *బఫరు,
			     hb_bool_t utf8_clusters,
			     ద్విగుణ x_scale_గుణకం,
			     ద్విగుణ y_scale_గుణకం,
			     ద్విగుణ x,
			     ద్విగుణ y,
			     స్థిరరాశి అక్షరం *utf8,
			     పూర్ణాంకం utf8_len,
			     cairo_glyph_t **glyphs,
			     గుర్తులేని పూర్ణాంకం *అంకె_glyphs,
			     cairo_పాఠ్యం_cluster_t **clusters,
			     గుర్తులేని పూర్ణాంకం *అంకె_clusters,
			     cairo_పాఠ్యం_cluster_flags_t *cluster_flags)
{
  ఒకవేళ (utf8 && utf8_len < 0)
    utf8_len = అక్షరశ్రేణి_పొడవు (utf8);

  గుర్తులేని orig_అంకె_glyphs = *అంకె_glyphs;
  *అంకె_glyphs = hb_బఫరు_get_నిడివి (బఫరు);
  hb_glyph_info_t *hb_glyph = hb_బఫరు_get_glyph_infos (బఫరు, శూన్యసూచిక);
  hb_glyph_position_t *hb_position = hb_బఫరు_get_glyph_positions (బఫరు, శూన్యసూచిక);
  ఒకవేళ (orig_అంకె_glyphs < *అంకె_glyphs + 1)
    *glyphs = cairo_glyph_allocate (*అంకె_glyphs + 1);

  ఒకవేళ (clusters && utf8)
  {
    గుర్తులేని orig_అంకె_clusters = *అంకె_clusters;
    *అంకె_clusters = *అంకె_glyphs ? 1 : 0;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < *అంకె_glyphs; i++)
      ఒకవేళ (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	(*అంకె_clusters)++;
    ఒకవేళ (orig_అంకె_clusters < *అంకె_clusters)
      *clusters = cairo_పాఠ్యం_cluster_allocate (*అంకె_clusters);
  }

  ద్విగుణ x_scale = x_scale_గుణకం ? 1. / x_scale_గుణకం : 0.;
  ద్విగుణ y_scale = y_scale_గుణకం ? 1. / y_scale_గుణకం : 0.;
  ద్విగుణ hx = 0, hy = 0;
  పూర్ణాంకం i;
  కొరకు (i = 0; i < (పూర్ణాంకం) *అంకె_glyphs; i++)
  {
    (*glyphs)[i].సూచీ_2 = hb_glyph[i].codepoint;
    (*glyphs)[i].x = x + ((ద్విగుణ) hb_position->x_స్థానభ్రంశం_2 + hx) * x_scale;
    (*glyphs)[i].y = y + (-(ద్విగుణ) hb_position->y_స్థానభ్రంశం_2 + hy) * y_scale;
    hx +=  hb_position->x_advance;
    hy -= hb_position->y_advance;

    hb_position++;
  }
  (*glyphs)[i].సూచీ_2 = -1;
  (*glyphs)[i].x = round (hx * x_scale);
  (*glyphs)[i].y = round (hy * y_scale);

  ఒకవేళ (clusters && *అంకె_clusters && utf8)
  {
    hb_memset ((శూన్య *) *clusters, 0, *అంకె_clusters * పరిమాణం ((*clusters)[0]));
    hb_bool_t backward = HB_DIRECTION_IS_BACKWARD (hb_బఫరు_get_direction (బఫరు));
    *cluster_flags = backward ? CAIRO_పాఠ్యం_CLUSTER_FLAG_BACKWARD : (cairo_పాఠ్యం_cluster_flags_t) 0;
    గుర్తులేని పూర్ణాంకం cluster = 0;
    స్థిరరాశి అక్షరం *ప్రారంభం = utf8, *చివర;
    (*clusters)[cluster].అంకె_glyphs++;
    ఒకవేళ (backward)
    {
      కొరకు (i = *అంకె_glyphs - 2; i >= 0; i--)
      {
	ఒకవేళ (hb_glyph[i].cluster != hb_glyph[i+1].cluster)
	{
	  ప్రతిజ్ఞా (hb_glyph[i].cluster > hb_glyph[i+1].cluster);
	  ఒకవేళ (utf8_clusters)
	    చివర = ప్రారంభం + hb_glyph[i].cluster - hb_glyph[i+1].cluster;
	  లేకపోతే
	    చివర = (స్థిరరాశి అక్షరం *) hb_utf_స్థానభ్రంశం_గమ్యం_సూచిక<hb_utf8_t> ((స్థిరరాశి uint8_t *) ప్రారంభం,
								      (స్థిరరాశి uint8_t *) utf8, utf8_len,
								      (గుర్తుగల) (hb_glyph[i].cluster - hb_glyph[i+1].cluster));
	  (*clusters)[cluster].అంకె_బైట్లు = చివర - ప్రారంభం;
	  ప్రారంభం = చివర;
	  cluster++;
	}
	(*clusters)[cluster].అంకె_glyphs++;
      }
      (*clusters)[cluster].అంకె_బైట్లు = utf8 + utf8_len - ప్రారంభం;
    }
    లేకపోతే
    {
      కొరకు (i = 1; i < (పూర్ణాంకం) *అంకె_glyphs; i++)
      {
	ఒకవేళ (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	{
	  ప్రతిజ్ఞా (hb_glyph[i].cluster > hb_glyph[i-1].cluster);
	  ఒకవేళ (utf8_clusters)
	    చివర = ప్రారంభం + hb_glyph[i].cluster - hb_glyph[i-1].cluster;
	  లేకపోతే
	    చివర = (స్థిరరాశి అక్షరం *) hb_utf_స్థానభ్రంశం_గమ్యం_సూచిక<hb_utf8_t> ((స్థిరరాశి uint8_t *) ప్రారంభం,
								      (స్థిరరాశి uint8_t *) utf8, utf8_len,
								      (గుర్తుగల) (hb_glyph[i].cluster - hb_glyph[i-1].cluster));
	  (*clusters)[cluster].అంకె_బైట్లు = చివర - ప్రారంభం;
	  ప్రారంభం = చివర;
	  cluster++;
	}
	(*clusters)[cluster].అంకె_glyphs++;
      }
      (*clusters)[cluster].అంకె_బైట్లు = utf8 + utf8_len - ప్రారంభం;
    }
  }
  లేకపోతే ఒకవేళ (అంకె_clusters)
    *అంకె_clusters = 0;
}

#అయితే_ముగింపు
