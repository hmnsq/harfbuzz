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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_PAINT

#ضمن "hb-paint-extents.hh"

#ضمن "hb-draw.hh"

#ضمن "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساكن فراغ
hb_paint_extents_ڌڪو_transform (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				 فراغ *paint_معلومات,
				 عائم xx, عائم yx,
				 عائم xy, عائم yy,
				 عائم dx, عائم dy,
				 فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->ڌڪو_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

ساكن فراغ
hb_paint_extents_ڪڍو_transform (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			        فراغ *paint_معلومات,
				فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->ڪڍو_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
بنية hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_بچيل;

  منطقي consume_segment ()
  {
    أرجع hb_budget_spend (*budget_بچيل, HB_BUDGET_1);
  }
};

ساكن فراغ
hb_paint_extents_ڳڻپ_move_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل, فراغ *معلومات,
				hb_draw_state_t *st HB_اڻ_استعمال_ٿيل,
				عائم منزل_x, عائم منزل_y,
				فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) معلومات;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جوڙيو_نقطو (منزل_x, منزل_y);
}

ساكن فراغ
hb_paint_extents_ڳڻپ_سٽ_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل, فراغ *معلومات,
				hb_draw_state_t *st HB_اڻ_استعمال_ٿيل,
				عائم منزل_x, عائم منزل_y,
				فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) معلومات;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جوڙيو_نقطو (منزل_x, منزل_y);
}

ساكن فراغ
hb_paint_extents_ڳڻپ_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل, فراغ *معلومات,
				     hb_draw_state_t *st HB_اڻ_استعمال_ٿيل,
				     عائم control_x, عائم control_y,
				     عائم منزل_x, عائم منزل_y,
				     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) معلومات;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جوڙيو_نقطو (control_x, control_y);
  sink->extents.جوڙيو_نقطو (منزل_x, منزل_y);
}

ساكن فراغ
hb_paint_extents_ڳڻپ_cubic_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل, فراغ *معلومات,
				 hb_draw_state_t *st HB_اڻ_استعمال_ٿيل,
				 عائم control1_x, عائم control1_y,
				 عائم control2_x, عائم control2_y,
				 عائم منزل_x, عائم منزل_y,
				 فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) معلومات;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جوڙيو_نقطو (control1_x, control1_y);
  sink->extents.جوڙيو_نقطو (control2_x, control2_y);
  sink->extents.جوڙيو_نقطو (منزل_x, منزل_y);
}

ساكن int64_t *
hb_paint_extents_get_budget_بچيل (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع ((hb_paint_extents_sink_t *) draw_معلومات)->budget_بچيل;
}

ساكن مضمن فراغ free_static_paint_extents_draw_funcs ();

ساكن بنية hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سيٽ_move_منزل_func (funcs, hb_paint_extents_ڳڻپ_move_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سٽ_منزل_func (funcs, hb_paint_extents_ڳڻپ_سٽ_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_quadratic_منزل_func (funcs, hb_paint_extents_ڳڻپ_quadratic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_cubic_منزل_func (funcs, hb_paint_extents_ڳڻپ_cubic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_paint_extents_get_budget_بچيل, خالي_اشارو, خالي_اشارو);

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
hb_paint_extents_ڌڪو_clip_glyph (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				  فراغ *paint_معلومات,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  hb_paint_extents_sink_t sink {{}, &c->budget_بچيل};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  إذا (likely (c->budget_بچيل >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->ڌڪو_clip (sink.extents);
}

ساكن فراغ
hb_paint_extents_ڌڪو_clip_rectangle (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				      فراغ *paint_معلومات,
				      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
				      فراغ *user_معلومات)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->ڌڪو_clip (extents);
}

ساكن فراغ
hb_paint_extents_ڪڍو_clip (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			   فراغ *paint_معلومات,
			   فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->ڪڍو_clip ();
}

ساكن فراغ
hb_paint_extents_ڌڪو_گروهه (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			     فراغ *paint_معلومات,
			     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->ڌڪو_گروهه ();
}

ساكن فراغ
hb_paint_extents_ڪڍو_گروهه (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			    فراغ *paint_معلومات,
			    hb_paint_مرکب_گهڻائي_t گهڻائي,
			    فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->ڪڍو_گروهه (گهڻائي);
}

ساكن hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			      فراغ *paint_معلومات,
			      hb_blob_t *blob HB_اڻ_استعمال_ٿيل,
			      بنانشان صحيح ويڪر HB_اڻ_استعمال_ٿيل,
			      بنانشان صحيح اوچائي HB_اڻ_استعمال_ٿيل,
			      hb_ٽيگ_t format HB_اڻ_استعمال_ٿيل,
			      عائم slant HB_اڻ_استعمال_ٿيل,
			      hb_glyph_extents_t *glyph_extents,
			      فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  إذا (!glyph_extents)
    أرجع خطأ; // Happens with SVG images.

  hb_extents_t<> extents = {(عائم) glyph_extents->x_bearing,
			    (عائم) glyph_extents->y_bearing + glyph_extents->اوچائي,
			    (عائم) glyph_extents->x_bearing + glyph_extents->ويڪر,
			    (عائم) glyph_extents->y_bearing};
  c->ڌڪو_clip (extents);
  c->paint ();
  c->ڪڍو_clip ();

  أرجع سچ;
}

ساكن فراغ
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			      فراغ *paint_معلومات,
			      hb_bool_t use_foreground HB_اڻ_استعمال_ٿيل,
			      hb_color_t color HB_اڻ_استعمال_ٿيل,
			      فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_لڪيري_gradient (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				        فراغ *paint_معلومات,
				        hb_color_سٽ_t *color_سٽ HB_اڻ_استعمال_ٿيل,
				        عائم x0 HB_اڻ_استعمال_ٿيل, عائم y0 HB_اڻ_استعمال_ٿيل,
				        عائم x1 HB_اڻ_استعمال_ٿيل, عائم y1 HB_اڻ_استعمال_ٿيل,
				        عائم x2 HB_اڻ_استعمال_ٿيل, عائم y2 HB_اڻ_استعمال_ٿيل,
				        فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				        فراغ *paint_معلومات,
				        hb_color_سٽ_t *color_سٽ HB_اڻ_استعمال_ٿيل,
				        عائم x0 HB_اڻ_استعمال_ٿيل, عائم y0 HB_اڻ_استعمال_ٿيل, عائم r0 HB_اڻ_استعمال_ٿيل,
				        عائم x1 HB_اڻ_استعمال_ٿيل, عائم y1 HB_اڻ_استعمال_ٿيل, عائم r1 HB_اڻ_استعمال_ٿيل,
				        فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				       فراغ *paint_معلومات,
				       hb_color_سٽ_t *color_سٽ HB_اڻ_استعمال_ٿيل,
				       عائم cx HB_اڻ_استعمال_ٿيل, عائم cy HB_اڻ_استعمال_ٿيل,
				       عائم شروعات_angle HB_اڻ_استعمال_ٿيل,
				       عائم پڇاڙي_angle HB_اڻ_استعمال_ٿيل,
				       فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_معلومات;

  c->paint ();
}

ساكن hb_bool_t
hb_paint_extents_سيٽ_budget (hb_paint_funcs_t *, فراغ *paint_معلومات,
			     int64_t budget, فراغ *)
{
  خودڪار *context = (hb_paint_extents_context_t *) paint_معلومات;
  context->budget = budget;
  context->budget_initialized = سچ;
  context->recharge_budget ();
  أرجع سچ;
}

ساكن int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, فراغ *paint_معلومات, فراغ *)
{
  أرجع ((hb_paint_extents_context_t *) paint_معلومات)->budget;
}

ساكن int64_t *
hb_paint_extents_get_budget_بچيل (hb_paint_funcs_t *, فراغ *paint_معلومات, فراغ *)
{
  أرجع &((hb_paint_extents_context_t *) paint_معلومات)->budget_بچيل;
}

ساكن مضمن فراغ free_static_paint_extents_funcs ();

ساكن بنية hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سيٽ_ڌڪو_transform_func (funcs, hb_paint_extents_ڌڪو_transform, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_transform_func (funcs, hb_paint_extents_ڪڍو_transform, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_glyph_func (funcs, hb_paint_extents_ڌڪو_clip_glyph, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_rectangle_func (funcs, hb_paint_extents_ڌڪو_clip_rectangle, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_clip_func (funcs, hb_paint_extents_ڪڍو_clip, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_گروهه_func (funcs, hb_paint_extents_ڌڪو_گروهه, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_گروهه_func (funcs, hb_paint_extents_ڪڍو_گروهه, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_color_func (funcs, hb_paint_extents_paint_color, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_image_func (funcs, hb_paint_extents_paint_image, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_لڪيري_gradient_func (funcs, hb_paint_extents_paint_لڪيري_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_radial_gradient_func (funcs, hb_paint_extents_paint_radial_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_sweep_gradient_func (funcs, hb_paint_extents_paint_sweep_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_سيٽ_budget_func (funcs, hb_paint_extents_سيٽ_budget, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_get_budget_func (funcs, hb_paint_extents_get_budget, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_paint_extents_get_budget_بچيل, خالي_اشارو, خالي_اشارو);

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


#جيڪڏهن_پورو
