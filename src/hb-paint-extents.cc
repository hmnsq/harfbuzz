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

#ضمن "hb.hh"

#اگر_تعریف_نشده HB_NO_PAINT

#ضمن "hb-paint-extents.hh"

#ضمن "hb-draw.hh"

#ضمن "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساكن فراغ
hb_paint_extents_افزودن_transform (hb_paint_funcs_t *funcs HB_استفادهنشده,
				 فراغ *paint_داده,
				 عائم xx, عائم yx,
				 عائم xy, عائم yy,
				 عائم dx, عائم dy,
				 فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  c->افزودن_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

ساكن فراغ
hb_paint_extents_برداشت_transform (hb_paint_funcs_t *funcs HB_استفادهنشده,
			        فراغ *paint_داده,
				فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  c->برداشت_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
بنية hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_مانده;

  منطقي consume_segment ()
  {
    أرجع hb_budget_spend (*budget_مانده, HB_BUDGET_1);
  }
};

ساكن فراغ
hb_paint_extents_شمار_move_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *داده,
				hb_draw_state_t *st HB_استفادهنشده,
				عائم مقصد_x, عائم مقصد_y,
				فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) داده;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جمع_کن_نقطه (مقصد_x, مقصد_y);
}

ساكن فراغ
hb_paint_extents_شمار_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *داده,
				hb_draw_state_t *st HB_استفادهنشده,
				عائم مقصد_x, عائم مقصد_y,
				فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) داده;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جمع_کن_نقطه (مقصد_x, مقصد_y);
}

ساكن فراغ
hb_paint_extents_شمار_quadratic_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *داده,
				     hb_draw_state_t *st HB_استفادهنشده,
				     عائم control_x, عائم control_y,
				     عائم مقصد_x, عائم مقصد_y,
				     فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) داده;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جمع_کن_نقطه (control_x, control_y);
  sink->extents.جمع_کن_نقطه (مقصد_x, مقصد_y);
}

ساكن فراغ
hb_paint_extents_شمار_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده, فراغ *داده,
				 hb_draw_state_t *st HB_استفادهنشده,
				 عائم control1_x, عائم control1_y,
				 عائم control2_x, عائم control2_y,
				 عائم مقصد_x, عائم مقصد_y,
				 فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) داده;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جمع_کن_نقطه (control1_x, control1_y);
  sink->extents.جمع_کن_نقطه (control2_x, control2_y);
  sink->extents.جمع_کن_نقطه (مقصد_x, مقصد_y);
}

ساكن int64_t *
hb_paint_extents_get_budget_مانده (hb_draw_funcs_t *, فراغ *draw_داده, فراغ *)
{
  أرجع ((hb_paint_extents_sink_t *) draw_داده)->budget_مانده;
}

ساكن مضمن فراغ free_static_paint_extents_draw_funcs ();

ساكن بنية hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_مجموعه_move_مقصد_func (funcs, hb_paint_extents_شمار_move_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_سطر_مقصد_func (funcs, hb_paint_extents_شمار_سطر_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_quadratic_مقصد_func (funcs, hb_paint_extents_شمار_quadratic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_cubic_مقصد_func (funcs, hb_paint_extents_شمار_cubic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_get_budget_مانده_func (funcs, hb_paint_extents_get_budget_مانده, اشاره_تهی, اشاره_تهی);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    أرجع funcs;
  }
} static_paint_extents_draw_funcs;

ساكن مضمن
فراغ free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

ساكن فراغ
hb_paint_extents_افزودن_clip_glyph (hb_paint_funcs_t *funcs HB_استفادهنشده,
				  فراغ *paint_داده,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  hb_paint_extents_sink_t sink {{}, &c->budget_مانده};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  إذا (likely (c->budget_مانده >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->افزودن_clip (sink.extents);
}

ساكن فراغ
hb_paint_extents_افزودن_clip_rectangle (hb_paint_funcs_t *funcs HB_استفادهنشده,
				      فراغ *paint_داده,
				      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
				      فراغ *user_داده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->افزودن_clip (extents);
}

ساكن فراغ
hb_paint_extents_برداشت_clip (hb_paint_funcs_t *funcs HB_استفادهنشده,
			   فراغ *paint_داده,
			   فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  c->برداشت_clip ();
}

ساكن فراغ
hb_paint_extents_افزودن_گروه (hb_paint_funcs_t *funcs HB_استفادهنشده,
			     فراغ *paint_داده,
			     فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  c->افزودن_گروه ();
}

ساكن فراغ
hb_paint_extents_برداشت_گروه (hb_paint_funcs_t *funcs HB_استفادهنشده,
			    فراغ *paint_داده,
			    hb_paint_مرکب_نما_t نما,
			    فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  c->برداشت_گروه (نما);
}

ساكن hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_استفادهنشده,
			      فراغ *paint_داده,
			      hb_blob_t *blob HB_استفادهنشده,
			      بیعلامت صحيح عرض HB_استفادهنشده,
			      بیعلامت صحيح ارتفاع HB_استفادهنشده,
			      hb_برچسب_t format HB_استفادهنشده,
			      عائم slant HB_استفادهنشده,
			      hb_glyph_extents_t *glyph_extents,
			      فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  إذا (!glyph_extents)
    أرجع خطأ; // Happens with SVG images.

  hb_extents_t<> extents = {(عائم) glyph_extents->x_bearing,
			    (عائم) glyph_extents->y_bearing + glyph_extents->ارتفاع,
			    (عائم) glyph_extents->x_bearing + glyph_extents->عرض,
			    (عائم) glyph_extents->y_bearing};
  c->افزودن_clip (extents);
  c->paint ();
  c->برداشت_clip ();

  أرجع درست;
}

ساكن فراغ
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_استفادهنشده,
			      فراغ *paint_داده,
			      hb_bool_t use_foreground HB_استفادهنشده,
			      hb_color_t color HB_استفادهنشده,
			      فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_خطی_gradient (hb_paint_funcs_t *funcs HB_استفادهنشده,
				        فراغ *paint_داده,
				        hb_color_سطر_t *color_سطر HB_استفادهنشده,
				        عائم x0 HB_استفادهنشده, عائم y0 HB_استفادهنشده,
				        عائم x1 HB_استفادهنشده, عائم y1 HB_استفادهنشده,
				        عائم x2 HB_استفادهنشده, عائم y2 HB_استفادهنشده,
				        فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_استفادهنشده,
				        فراغ *paint_داده,
				        hb_color_سطر_t *color_سطر HB_استفادهنشده,
				        عائم x0 HB_استفادهنشده, عائم y0 HB_استفادهنشده, عائم r0 HB_استفادهنشده,
				        عائم x1 HB_استفادهنشده, عائم y1 HB_استفادهنشده, عائم r1 HB_استفادهنشده,
				        فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_استفادهنشده,
				       فراغ *paint_داده,
				       hb_color_سطر_t *color_سطر HB_استفادهنشده,
				       عائم cx HB_استفادهنشده, عائم cy HB_استفادهنشده,
				       عائم شروع_angle HB_استفادهنشده,
				       عائم انتها_angle HB_استفادهنشده,
				       فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_داده;

  c->paint ();
}

ساكن hb_bool_t
hb_paint_extents_مجموعه_budget (hb_paint_funcs_t *, فراغ *paint_داده,
			     int64_t budget, فراغ *)
{
  خودکار *context = (hb_paint_extents_context_t *) paint_داده;
  context->budget = budget;
  context->budget_initialized = درست;
  context->recharge_budget ();
  أرجع درست;
}

ساكن int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, فراغ *paint_داده, فراغ *)
{
  أرجع ((hb_paint_extents_context_t *) paint_داده)->budget;
}

ساكن int64_t *
hb_paint_extents_get_budget_مانده (hb_paint_funcs_t *, فراغ *paint_داده, فراغ *)
{
  أرجع &((hb_paint_extents_context_t *) paint_داده)->budget_مانده;
}

ساكن مضمن فراغ free_static_paint_extents_funcs ();

ساكن بنية hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_مجموعه_افزودن_transform_func (funcs, hb_paint_extents_افزودن_transform, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_transform_func (funcs, hb_paint_extents_برداشت_transform, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_glyph_func (funcs, hb_paint_extents_افزودن_clip_glyph, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_rectangle_func (funcs, hb_paint_extents_افزودن_clip_rectangle, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_clip_func (funcs, hb_paint_extents_برداشت_clip, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_گروه_func (funcs, hb_paint_extents_افزودن_گروه, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_گروه_func (funcs, hb_paint_extents_برداشت_گروه, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_color_func (funcs, hb_paint_extents_paint_color, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_image_func (funcs, hb_paint_extents_paint_image, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_خطی_gradient_func (funcs, hb_paint_extents_paint_خطی_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_radial_gradient_func (funcs, hb_paint_extents_paint_radial_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_sweep_gradient_func (funcs, hb_paint_extents_paint_sweep_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_مجموعه_budget_func (funcs, hb_paint_extents_مجموعه_budget, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_get_budget_func (funcs, hb_paint_extents_get_budget, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_get_budget_مانده_func (funcs, hb_paint_extents_get_budget_مانده, اشاره_تهی, اشاره_تهی);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_funcs);

    أرجع funcs;
  }
} static_paint_extents_funcs;

ساكن مضمن
فراغ free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  أرجع static_paint_extents_funcs.get_unconst ();
}


#پایان_اگر
