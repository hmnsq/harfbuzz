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

#که_نه_تعریف_شوی HB_NO_PAINT

#ضمن "hb-paint-extents.hh"

#ضمن "hb-draw.hh"

#ضمن "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساكن فراغ
hb_paint_extents_ټېل_transform (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				 فراغ *paint_معلومات,
				 عائم xx, عائم yx,
				 عائم xy, عائم yy,
				 عائم dx, عائم dy,
				 فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->ټېل_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

ساكن فراغ
hb_paint_extents_راوباسه_transform (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			        فراغ *paint_معلومات,
				فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->راوباسه_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
بنية hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_پاتې_شوی;

  منطقي consume_segment ()
  {
    أرجع hb_budget_spend (*budget_پاتې_شوی, HB_BUDGET_1);
  }
};

ساكن فراغ
hb_paint_extents_شمېر_move_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی, فراغ *معلومات,
				hb_draw_state_t *st HB_ناکارول_شوی,
				عائم منزل_x, عائم منزل_y,
				فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) معلومات;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جمع_کړه_ټکی (منزل_x, منزل_y);
}

ساكن فراغ
hb_paint_extents_شمېر_کرښه_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی, فراغ *معلومات,
				hb_draw_state_t *st HB_ناکارول_شوی,
				عائم منزل_x, عائم منزل_y,
				فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) معلومات;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جمع_کړه_ټکی (منزل_x, منزل_y);
}

ساكن فراغ
hb_paint_extents_شمېر_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی, فراغ *معلومات,
				     hb_draw_state_t *st HB_ناکارول_شوی,
				     عائم control_x, عائم control_y,
				     عائم منزل_x, عائم منزل_y,
				     فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) معلومات;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جمع_کړه_ټکی (control_x, control_y);
  sink->extents.جمع_کړه_ټکی (منزل_x, منزل_y);
}

ساكن فراغ
hb_paint_extents_شمېر_cubic_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی, فراغ *معلومات,
				 hb_draw_state_t *st HB_ناکارول_شوی,
				 عائم control1_x, عائم control1_y,
				 عائم control2_x, عائم control2_y,
				 عائم منزل_x, عائم منزل_y,
				 فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) معلومات;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جمع_کړه_ټکی (control1_x, control1_y);
  sink->extents.جمع_کړه_ټکی (control2_x, control2_y);
  sink->extents.جمع_کړه_ټکی (منزل_x, منزل_y);
}

ساكن int64_t *
hb_paint_extents_get_budget_پاتې_شوی (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع ((hb_paint_extents_sink_t *) draw_معلومات)->budget_پاتې_شوی;
}

ساكن مضمن فراغ free_static_paint_extents_draw_funcs ();

ساكن بنية hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سېټ_move_منزل_func (funcs, hb_paint_extents_شمېر_move_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_کرښه_منزل_func (funcs, hb_paint_extents_شمېر_کرښه_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_quadratic_منزل_func (funcs, hb_paint_extents_شمېر_quadratic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_cubic_منزل_func (funcs, hb_paint_extents_شمېر_cubic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_get_budget_پاتې_شوی_func (funcs, hb_paint_extents_get_budget_پاتې_شوی, تش_اشاره, تش_اشاره);

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
hb_paint_extents_ټېل_clip_glyph (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				  فراغ *paint_معلومات,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  hb_paint_extents_sink_t sink {{}, &c->budget_پاتې_شوی};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  إذا (likely (c->budget_پاتې_شوی >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->ټېل_clip (sink.extents);
}

ساكن فراغ
hb_paint_extents_ټېل_clip_rectangle (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				      فراغ *paint_معلومات,
				      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
				      فراغ *user_معلومات)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->ټېل_clip (extents);
}

ساكن فراغ
hb_paint_extents_راوباسه_clip (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			   فراغ *paint_معلومات,
			   فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->راوباسه_clip ();
}

ساكن فراغ
hb_paint_extents_ټېل_ډله (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			     فراغ *paint_معلومات,
			     فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->ټېل_ډله ();
}

ساكن فراغ
hb_paint_extents_راوباسه_ډله (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			    فراغ *paint_معلومات,
			    hb_paint_مرکب_ډېرپېښ_t ډېرپېښ,
			    فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->راوباسه_ډله (ډېرپېښ);
}

ساكن hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			      فراغ *paint_معلومات,
			      hb_blob_t *blob HB_ناکارول_شوی,
			      بینښه صحيح پلنوالی HB_ناکارول_شوی,
			      بینښه صحيح لوړوالی HB_ناکارول_شوی,
			      hb_نښان_t format HB_ناکارول_شوی,
			      عائم slant HB_ناکارول_شوی,
			      hb_glyph_extents_t *glyph_extents,
			      فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  إذا (!glyph_extents)
    أرجع خطأ; // Happens with SVG images.

  hb_extents_t<> extents = {(عائم) glyph_extents->x_bearing,
			    (عائم) glyph_extents->y_bearing + glyph_extents->لوړوالی,
			    (عائم) glyph_extents->x_bearing + glyph_extents->پلنوالی,
			    (عائم) glyph_extents->y_bearing};
  c->ټېل_clip (extents);
  c->paint ();
  c->راوباسه_clip ();

  أرجع ریښتیا;
}

ساكن فراغ
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			      فراغ *paint_معلومات,
			      hb_bool_t use_foreground HB_ناکارول_شوی,
			      hb_color_t color HB_ناکارول_شوی,
			      فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_خطي_gradient (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				        فراغ *paint_معلومات,
				        hb_color_کرښه_t *color_کرښه HB_ناکارول_شوی,
				        عائم x0 HB_ناکارول_شوی, عائم y0 HB_ناکارول_شوی,
				        عائم x1 HB_ناکارول_شوی, عائم y1 HB_ناکارول_شوی,
				        عائم x2 HB_ناکارول_شوی, عائم y2 HB_ناکارول_شوی,
				        فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				        فراغ *paint_معلومات,
				        hb_color_کرښه_t *color_کرښه HB_ناکارول_شوی,
				        عائم x0 HB_ناکارول_شوی, عائم y0 HB_ناکارول_شوی, عائم r0 HB_ناکارول_شوی,
				        عائم x1 HB_ناکارول_شوی, عائم y1 HB_ناکارول_شوی, عائم r1 HB_ناکارول_شوی,
				        فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				       فراغ *paint_معلومات,
				       hb_color_کرښه_t *color_کرښه HB_ناکارول_شوی,
				       عائم cx HB_ناکارول_شوی, عائم cy HB_ناکارول_شوی,
				       عائم پیل_angle HB_ناکارول_شوی,
				       عائم پای_ټکی_angle HB_ناکارول_شوی,
				       فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->paint ();
}

ساكن hb_bool_t
hb_paint_extents_سېټ_budget (hb_paint_funcs_t *, فراغ *paint_معلومات,
			     int64_t budget, فراغ *)
{
  خپلکاری *context = (hb_paint_extents_context_t *) paint_معلومات;
  context->budget = budget;
  context->budget_initialized = ریښتیا;
  context->recharge_budget ();
  أرجع ریښتیا;
}

ساكن int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, فراغ *paint_معلومات, فراغ *)
{
  أرجع ((hb_paint_extents_context_t *) paint_معلومات)->budget;
}

ساكن int64_t *
hb_paint_extents_get_budget_پاتې_شوی (hb_paint_funcs_t *, فراغ *paint_معلومات, فراغ *)
{
  أرجع &((hb_paint_extents_context_t *) paint_معلومات)->budget_پاتې_شوی;
}

ساكن مضمن فراغ free_static_paint_extents_funcs ();

ساكن بنية hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سېټ_ټېل_transform_func (funcs, hb_paint_extents_ټېل_transform, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_transform_func (funcs, hb_paint_extents_راوباسه_transform, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_clip_glyph_func (funcs, hb_paint_extents_ټېل_clip_glyph, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_clip_rectangle_func (funcs, hb_paint_extents_ټېل_clip_rectangle, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_clip_func (funcs, hb_paint_extents_راوباسه_clip, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_ډله_func (funcs, hb_paint_extents_ټېل_ډله, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_ډله_func (funcs, hb_paint_extents_راوباسه_ډله, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_color_func (funcs, hb_paint_extents_paint_color, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_image_func (funcs, hb_paint_extents_paint_image, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_خطي_gradient_func (funcs, hb_paint_extents_paint_خطي_gradient, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_radial_gradient_func (funcs, hb_paint_extents_paint_radial_gradient, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_sweep_gradient_func (funcs, hb_paint_extents_paint_sweep_gradient, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_سېټ_budget_func (funcs, hb_paint_extents_سېټ_budget, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_get_budget_func (funcs, hb_paint_extents_get_budget, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_get_budget_پاتې_شوی_func (funcs, hb_paint_extents_get_budget_پاتې_شوی, تش_اشاره, تش_اشاره);

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


#د_که_پای
