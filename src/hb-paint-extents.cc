/*
 * Copyright © 2022 Behdad Esfahbod
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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_PAINT

#అన్తర్భావ "hb-paint-extents.hh"

#అన్తర్భావ "hb-draw.hh"

#అన్తర్భావ "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

స్థిర శూన్య
hb_paint_extents_నెట్టు_transform (hb_paint_funcs_t *funcs HB_వాడని,
				 శూన్య *paint_దత్తాంశం,
				 దశాంశం xx, దశాంశం yx,
				 దశాంశం xy, దశాంశం yy,
				 దశాంశం dx, దశాంశం dy,
				 శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  c->నెట్టు_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

స్థిర శూన్య
hb_paint_extents_తీయి_transform (hb_paint_funcs_t *funcs HB_వాడని,
			        శూన్య *paint_దత్తాంశం,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  c->తీయి_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
నిర్మాణం hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_మిగిలిన;

  తార్కిక consume_segment ()
  {
    తిరిగి hb_budget_spend (*budget_మిగిలిన, HB_BUDGET_1);
  }
};

స్థిర శూన్య
hb_paint_extents_లెక్క_move_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *దత్తాంశం,
				hb_draw_state_t *st HB_వాడని,
				దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) దత్తాంశం;
  ఒకవేళ (unlikely (!sink->consume_segment ())) తిరిగి;
  sink->extents.కూడు_బిందువు (గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_paint_extents_లెక్క_పంక్తి_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *దత్తాంశం,
				hb_draw_state_t *st HB_వాడని,
				దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) దత్తాంశం;
  ఒకవేళ (unlikely (!sink->consume_segment ())) తిరిగి;
  sink->extents.కూడు_బిందువు (గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_paint_extents_లెక్క_quadratic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *దత్తాంశం,
				     hb_draw_state_t *st HB_వాడని,
				     దశాంశం control_x, దశాంశం control_y,
				     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
				     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) దత్తాంశం;
  ఒకవేళ (unlikely (!sink->consume_segment ())) తిరిగి;
  sink->extents.కూడు_బిందువు (control_x, control_y);
  sink->extents.కూడు_బిందువు (గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_paint_extents_లెక్క_cubic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *దత్తాంశం,
				 hb_draw_state_t *st HB_వాడని,
				 దశాంశం control1_x, దశాంశం control1_y,
				 దశాంశం control2_x, దశాంశం control2_y,
				 దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
				 శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) దత్తాంశం;
  ఒకవేళ (unlikely (!sink->consume_segment ())) తిరిగి;
  sink->extents.కూడు_బిందువు (control1_x, control1_y);
  sink->extents.కూడు_బిందువు (control2_x, control2_y);
  sink->extents.కూడు_బిందువు (గమ్యం_x, గమ్యం_y);
}

స్థిర int64_t *
hb_paint_extents_get_budget_మిగిలిన (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_paint_extents_sink_t *) draw_దత్తాంశం)->budget_మిగిలిన;
}

స్థిర అంతఃవరుస శూన్య free_static_paint_extents_draw_funcs ();

స్థిర నిర్మాణం hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_సమితి_move_గమ్యం_func (funcs, hb_paint_extents_లెక్క_move_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func (funcs, hb_paint_extents_లెక్క_పంక్తి_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_quadratic_గమ్యం_func (funcs, hb_paint_extents_లెక్క_quadratic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_cubic_గమ్యం_func (funcs, hb_paint_extents_లెక్క_cubic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_paint_extents_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    తిరిగి funcs;
  }
} static_paint_extents_draw_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

స్థిర శూన్య
hb_paint_extents_నెట్టు_clip_glyph (hb_paint_funcs_t *funcs HB_వాడని,
				  శూన్య *paint_దత్తాంశం,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  hb_paint_extents_sink_t sink {{}, &c->budget_మిగిలిన};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  ఒకవేళ (likely (c->budget_మిగిలిన >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->నెట్టు_clip (sink.extents);
}

స్థిర శూన్య
hb_paint_extents_నెట్టు_clip_rectangle (hb_paint_funcs_t *funcs HB_వాడని,
				      శూన్య *paint_దత్తాంశం,
				      దశాంశం xmin, దశాంశం ymin, దశాంశం xmax, దశాంశం ymax,
				      శూన్య *user_దత్తాంశం)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->నెట్టు_clip (extents);
}

స్థిర శూన్య
hb_paint_extents_తీయి_clip (hb_paint_funcs_t *funcs HB_వాడని,
			   శూన్య *paint_దత్తాంశం,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  c->తీయి_clip ();
}

స్థిర శూన్య
hb_paint_extents_నెట్టు_సమూహం (hb_paint_funcs_t *funcs HB_వాడని,
			     శూన్య *paint_దత్తాంశం,
			     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  c->నెట్టు_సమూహం ();
}

స్థిర శూన్య
hb_paint_extents_తీయి_సమూహం (hb_paint_funcs_t *funcs HB_వాడని,
			    శూన్య *paint_దత్తాంశం,
			    hb_paint_సంయుక్త_బహుళకం_t బహుళకం,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  c->తీయి_సమూహం (బహుళకం);
}

స్థిర hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_వాడని,
			      శూన్య *paint_దత్తాంశం,
			      hb_blob_t *blob HB_వాడని,
			      గుర్తులేని పూర్ణాంకం వెడల్పు HB_వాడని,
			      గుర్తులేని పూర్ణాంకం ఎత్తు HB_వాడని,
			      hb_ట్యాగ్_t format HB_వాడని,
			      దశాంశం slant HB_వాడని,
			      hb_glyph_extents_t *glyph_extents,
			      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  ఒకవేళ (!glyph_extents)
    తిరిగి అబద్ధం; // Happens with SVG images.

  hb_extents_t<> extents = {(దశాంశం) glyph_extents->x_bearing,
			    (దశాంశం) glyph_extents->y_bearing + glyph_extents->ఎత్తు,
			    (దశాంశం) glyph_extents->x_bearing + glyph_extents->వెడల్పు,
			    (దశాంశం) glyph_extents->y_bearing};
  c->నెట్టు_clip (extents);
  c->paint ();
  c->తీయి_clip ();

  తిరిగి నిజం;
}

స్థిర శూన్య
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_వాడని,
			      శూన్య *paint_దత్తాంశం,
			      hb_bool_t use_foreground HB_వాడని,
			      hb_color_t color HB_వాడని,
			      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  c->paint ();
}

స్థిర శూన్య
hb_paint_extents_paint_రేఖీయ_gradient (hb_paint_funcs_t *funcs HB_వాడని,
				        శూన్య *paint_దత్తాంశం,
				        hb_color_పంక్తి_t *color_పంక్తి HB_వాడని,
				        దశాంశం x0 HB_వాడని, దశాంశం y0 HB_వాడని,
				        దశాంశం x1 HB_వాడని, దశాంశం y1 HB_వాడని,
				        దశాంశం x2 HB_వాడని, దశాంశం y2 HB_వాడని,
				        శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  c->paint ();
}

స్థిర శూన్య
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_వాడని,
				        శూన్య *paint_దత్తాంశం,
				        hb_color_పంక్తి_t *color_పంక్తి HB_వాడని,
				        దశాంశం x0 HB_వాడని, దశాంశం y0 HB_వాడని, దశాంశం r0 HB_వాడని,
				        దశాంశం x1 HB_వాడని, దశాంశం y1 HB_వాడని, దశాంశం r1 HB_వాడని,
				        శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  c->paint ();
}

స్థిర శూన్య
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_వాడని,
				       శూన్య *paint_దత్తాంశం,
				       hb_color_పంక్తి_t *color_పంక్తి HB_వాడని,
				       దశాంశం cx HB_వాడని, దశాంశం cy HB_వాడని,
				       దశాంశం ప్రారంభం_angle HB_వాడని,
				       దశాంశం చివర_angle HB_వాడని,
				       శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_దత్తాంశం;

  c->paint ();
}

స్థిర hb_bool_t
hb_paint_extents_సమితి_budget (hb_paint_funcs_t *, శూన్య *paint_దత్తాంశం,
			     int64_t budget, శూన్య *)
{
  స్వయంచాలక *context = (hb_paint_extents_context_t *) paint_దత్తాంశం;
  context->budget = budget;
  context->budget_initialized = నిజం;
  context->recharge_budget ();
  తిరిగి నిజం;
}

స్థిర int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, శూన్య *paint_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_paint_extents_context_t *) paint_దత్తాంశం)->budget;
}

స్థిర int64_t *
hb_paint_extents_get_budget_మిగిలిన (hb_paint_funcs_t *, శూన్య *paint_దత్తాంశం, శూన్య *)
{
  తిరిగి &((hb_paint_extents_context_t *) paint_దత్తాంశం)->budget_మిగిలిన;
}

స్థిర అంతఃవరుస శూన్య free_static_paint_extents_funcs ();

స్థిర నిర్మాణం hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  స్థిర hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_సమితి_నెట్టు_transform_func (funcs, hb_paint_extents_నెట్టు_transform, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_transform_func (funcs, hb_paint_extents_తీయి_transform, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_glyph_func (funcs, hb_paint_extents_నెట్టు_clip_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_rectangle_func (funcs, hb_paint_extents_నెట్టు_clip_rectangle, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_clip_func (funcs, hb_paint_extents_తీయి_clip, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_సమూహం_func (funcs, hb_paint_extents_నెట్టు_సమూహం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_సమూహం_func (funcs, hb_paint_extents_తీయి_సమూహం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_color_func (funcs, hb_paint_extents_paint_color, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_image_func (funcs, hb_paint_extents_paint_image, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_రేఖీయ_gradient_func (funcs, hb_paint_extents_paint_రేఖీయ_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_radial_gradient_func (funcs, hb_paint_extents_paint_radial_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_sweep_gradient_func (funcs, hb_paint_extents_paint_sweep_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_సమితి_budget_func (funcs, hb_paint_extents_సమితి_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_func (funcs, hb_paint_extents_get_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_paint_extents_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_funcs);

    తిరిగి funcs;
  }
} static_paint_extents_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  తిరిగి static_paint_extents_funcs.get_unconst ();
}


#అయితే_ముగింపు
