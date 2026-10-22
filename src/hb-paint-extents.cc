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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_PAINT

#شامل "hb-paint-extents.hh"

#شامل "hb-draw.hh"

#شامل "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساکن خالی
hb_paint_extents_ڈالو_transform (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				 خالی *paint_ڈیٹا,
				 اعشاری xx, اعشاری yx,
				 اعشاری xy, اعشاری yy,
				 اعشاری dx, اعشاری dy,
				 خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  c->ڈالو_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

ساکن خالی
hb_paint_extents_نکالو_transform (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			        خالی *paint_ڈیٹا,
				خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  c->نکالو_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
ساخت hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_بقایا;

  منطقی consume_segment ()
  {
    واپس hb_budget_spend (*budget_بقایا, HB_BUDGET_1);
  }
};

ساکن خالی
hb_paint_extents_گنتی_move_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل, خالی *ڈیٹا,
				hb_draw_state_t *st HB_غیر_مستعمل,
				اعشاری مقصد_x, اعشاری مقصد_y,
				خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) ڈیٹا;
  اگر (unlikely (!sink->consume_segment ())) واپس;
  sink->extents.جوڑو_نقطہ (مقصد_x, مقصد_y);
}

ساکن خالی
hb_paint_extents_گنتی_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل, خالی *ڈیٹا,
				hb_draw_state_t *st HB_غیر_مستعمل,
				اعشاری مقصد_x, اعشاری مقصد_y,
				خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) ڈیٹا;
  اگر (unlikely (!sink->consume_segment ())) واپس;
  sink->extents.جوڑو_نقطہ (مقصد_x, مقصد_y);
}

ساکن خالی
hb_paint_extents_گنتی_quadratic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل, خالی *ڈیٹا,
				     hb_draw_state_t *st HB_غیر_مستعمل,
				     اعشاری control_x, اعشاری control_y,
				     اعشاری مقصد_x, اعشاری مقصد_y,
				     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) ڈیٹا;
  اگر (unlikely (!sink->consume_segment ())) واپس;
  sink->extents.جوڑو_نقطہ (control_x, control_y);
  sink->extents.جوڑو_نقطہ (مقصد_x, مقصد_y);
}

ساکن خالی
hb_paint_extents_گنتی_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل, خالی *ڈیٹا,
				 hb_draw_state_t *st HB_غیر_مستعمل,
				 اعشاری control1_x, اعشاری control1_y,
				 اعشاری control2_x, اعشاری control2_y,
				 اعشاری مقصد_x, اعشاری مقصد_y,
				 خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) ڈیٹا;
  اگر (unlikely (!sink->consume_segment ())) واپس;
  sink->extents.جوڑو_نقطہ (control1_x, control1_y);
  sink->extents.جوڑو_نقطہ (control2_x, control2_y);
  sink->extents.جوڑو_نقطہ (مقصد_x, مقصد_y);
}

ساکن int64_t *
hb_paint_extents_get_budget_بقایا (hb_draw_funcs_t *, خالی *draw_ڈیٹا, خالی *)
{
  واپس ((hb_paint_extents_sink_t *) draw_ڈیٹا)->budget_بقایا;
}

ساکن اندرونی خالی free_static_paint_extents_draw_funcs ();

ساکن ساخت hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_مقصد_func (funcs, hb_paint_extents_گنتی_move_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func (funcs, hb_paint_extents_گنتی_سطر_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_quadratic_مقصد_func (funcs, hb_paint_extents_گنتی_quadratic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_cubic_مقصد_func (funcs, hb_paint_extents_گنتی_cubic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_paint_extents_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    واپس funcs;
  }
} static_paint_extents_draw_funcs;

ساکن اندرونی
خالی free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

ساکن خالی
hb_paint_extents_ڈالو_clip_glyph (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				  خالی *paint_ڈیٹا,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  hb_paint_extents_sink_t sink {{}, &c->budget_بقایا};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  اگر (likely (c->budget_بقایا >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->ڈالو_clip (sink.extents);
}

ساکن خالی
hb_paint_extents_ڈالو_clip_rectangle (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				      خالی *paint_ڈیٹا,
				      اعشاری xmin, اعشاری ymin, اعشاری xmax, اعشاری ymax,
				      خالی *user_ڈیٹا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->ڈالو_clip (extents);
}

ساکن خالی
hb_paint_extents_نکالو_clip (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			   خالی *paint_ڈیٹا,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  c->نکالو_clip ();
}

ساکن خالی
hb_paint_extents_ڈالو_گروہ (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			     خالی *paint_ڈیٹا,
			     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  c->ڈالو_گروہ ();
}

ساکن خالی
hb_paint_extents_نکالو_گروہ (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			    خالی *paint_ڈیٹا,
			    hb_paint_مرکب_منوال_t منوال,
			    خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  c->نکالو_گروہ (منوال);
}

ساکن hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			      خالی *paint_ڈیٹا,
			      hb_blob_t *blob HB_غیر_مستعمل,
			      غیرعلامتی صحیح چوڑائی HB_غیر_مستعمل,
			      غیرعلامتی صحیح اونچائی HB_غیر_مستعمل,
			      hb_لیبل_t format HB_غیر_مستعمل,
			      اعشاری slant HB_غیر_مستعمل,
			      hb_glyph_extents_t *glyph_extents,
			      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  اگر (!glyph_extents)
    واپس جھوٹ; // Happens with SVG images.

  hb_extents_t<> extents = {(اعشاری) glyph_extents->x_bearing,
			    (اعشاری) glyph_extents->y_bearing + glyph_extents->اونچائی,
			    (اعشاری) glyph_extents->x_bearing + glyph_extents->چوڑائی,
			    (اعشاری) glyph_extents->y_bearing};
  c->ڈالو_clip (extents);
  c->paint ();
  c->نکالو_clip ();

  واپس سچ;
}

ساکن خالی
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			      خالی *paint_ڈیٹا,
			      hb_bool_t use_foreground HB_غیر_مستعمل,
			      hb_color_t color HB_غیر_مستعمل,
			      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  c->paint ();
}

ساکن خالی
hb_paint_extents_paint_خطی_gradient (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				        خالی *paint_ڈیٹا,
				        hb_color_سطر_t *color_سطر HB_غیر_مستعمل,
				        اعشاری x0 HB_غیر_مستعمل, اعشاری y0 HB_غیر_مستعمل,
				        اعشاری x1 HB_غیر_مستعمل, اعشاری y1 HB_غیر_مستعمل,
				        اعشاری x2 HB_غیر_مستعمل, اعشاری y2 HB_غیر_مستعمل,
				        خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  c->paint ();
}

ساکن خالی
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				        خالی *paint_ڈیٹا,
				        hb_color_سطر_t *color_سطر HB_غیر_مستعمل,
				        اعشاری x0 HB_غیر_مستعمل, اعشاری y0 HB_غیر_مستعمل, اعشاری r0 HB_غیر_مستعمل,
				        اعشاری x1 HB_غیر_مستعمل, اعشاری y1 HB_غیر_مستعمل, اعشاری r1 HB_غیر_مستعمل,
				        خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  c->paint ();
}

ساکن خالی
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				       خالی *paint_ڈیٹا,
				       hb_color_سطر_t *color_سطر HB_غیر_مستعمل,
				       اعشاری cx HB_غیر_مستعمل, اعشاری cy HB_غیر_مستعمل,
				       اعشاری آغاز_angle HB_غیر_مستعمل,
				       اعشاری انتہا_angle HB_غیر_مستعمل,
				       خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ڈیٹا;

  c->paint ();
}

ساکن hb_bool_t
hb_paint_extents_سیٹ_budget (hb_paint_funcs_t *, خالی *paint_ڈیٹا,
			     int64_t budget, خالی *)
{
  خودکار *context = (hb_paint_extents_context_t *) paint_ڈیٹا;
  context->budget = budget;
  context->budget_initialized = سچ;
  context->recharge_budget ();
  واپس سچ;
}

ساکن int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, خالی *paint_ڈیٹا, خالی *)
{
  واپس ((hb_paint_extents_context_t *) paint_ڈیٹا)->budget;
}

ساکن int64_t *
hb_paint_extents_get_budget_بقایا (hb_paint_funcs_t *, خالی *paint_ڈیٹا, خالی *)
{
  واپس &((hb_paint_extents_context_t *) paint_ڈیٹا)->budget_بقایا;
}

ساکن اندرونی خالی free_static_paint_extents_funcs ();

ساکن ساخت hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  ساکن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سیٹ_ڈالو_transform_func (funcs, hb_paint_extents_ڈالو_transform, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_transform_func (funcs, hb_paint_extents_نکالو_transform, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, hb_paint_extents_ڈالو_clip_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, hb_paint_extents_ڈالو_clip_rectangle, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, hb_paint_extents_نکالو_clip, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, hb_paint_extents_ڈالو_گروہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, hb_paint_extents_نکالو_گروہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_color_func (funcs, hb_paint_extents_paint_color, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_image_func (funcs, hb_paint_extents_paint_image, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, hb_paint_extents_paint_خطی_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, hb_paint_extents_paint_radial_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, hb_paint_extents_paint_sweep_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_paint_extents_سیٹ_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_paint_extents_get_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_paint_extents_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_funcs);

    واپس funcs;
  }
} static_paint_extents_funcs;

ساکن اندرونی
خالی free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  واپس static_paint_extents_funcs.get_unconst ();
}


#اگر_ختم
