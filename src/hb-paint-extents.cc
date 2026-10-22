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

#جے_بنا_تعریف HB_NO_PAINT

#ضمن "hb-paint-extents.hh"

#ضمن "hb-draw.hh"

#ضمن "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساكن فراغ
hb_paint_extents_ڈالو_transform (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				 فراغ *paint_انکڑے,
				 عائم xx, عائم yx,
				 عائم xy, عائم yy,
				 عائم dx, عائم dy,
				 فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  c->ڈالو_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

ساكن فراغ
hb_paint_extents_نکالو_transform (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			        فراغ *paint_انکڑے,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  c->نکالو_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
بنية hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_بچیا;

  منطقي consume_segment ()
  {
    أرجع hb_budget_spend (*budget_بچیا, HB_BUDGET_1);
  }
};

ساكن فراغ
hb_paint_extents_گنتی_move_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *انکڑے,
				hb_draw_state_t *st HB_بن_ورتیا,
				عائم منزل_x, عائم منزل_y,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) انکڑے;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جوڑو_نقطہ (منزل_x, منزل_y);
}

ساكن فراغ
hb_paint_extents_گنتی_سطر_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *انکڑے,
				hb_draw_state_t *st HB_بن_ورتیا,
				عائم منزل_x, عائم منزل_y,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) انکڑے;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جوڑو_نقطہ (منزل_x, منزل_y);
}

ساكن فراغ
hb_paint_extents_گنتی_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *انکڑے,
				     hb_draw_state_t *st HB_بن_ورتیا,
				     عائم control_x, عائم control_y,
				     عائم منزل_x, عائم منزل_y,
				     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) انکڑے;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جوڑو_نقطہ (control_x, control_y);
  sink->extents.جوڑو_نقطہ (منزل_x, منزل_y);
}

ساكن فراغ
hb_paint_extents_گنتی_cubic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *انکڑے,
				 hb_draw_state_t *st HB_بن_ورتیا,
				 عائم control1_x, عائم control1_y,
				 عائم control2_x, عائم control2_y,
				 عائم منزل_x, عائم منزل_y,
				 فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) انکڑے;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.جوڑو_نقطہ (control1_x, control1_y);
  sink->extents.جوڑو_نقطہ (control2_x, control2_y);
  sink->extents.جوڑو_نقطہ (منزل_x, منزل_y);
}

ساكن int64_t *
hb_paint_extents_get_budget_بچیا (hb_draw_funcs_t *, فراغ *draw_انکڑے, فراغ *)
{
  أرجع ((hb_paint_extents_sink_t *) draw_انکڑے)->budget_بچیا;
}

ساكن مضمن فراغ free_static_paint_extents_draw_funcs ();

ساكن بنية hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_منزل_func (funcs, hb_paint_extents_گنتی_move_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_سطر_منزل_func (funcs, hb_paint_extents_گنتی_سطر_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_quadratic_منزل_func (funcs, hb_paint_extents_گنتی_quadratic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_cubic_منزل_func (funcs, hb_paint_extents_گنتی_cubic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_paint_extents_get_budget_بچیا, خالی_سوچک, خالی_سوچک);

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
hb_paint_extents_ڈالو_clip_glyph (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				  فراغ *paint_انکڑے,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  hb_paint_extents_sink_t sink {{}, &c->budget_بچیا};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  إذا (likely (c->budget_بچیا >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->ڈالو_clip (sink.extents);
}

ساكن فراغ
hb_paint_extents_ڈالو_clip_rectangle (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				      فراغ *paint_انکڑے,
				      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
				      فراغ *user_انکڑے)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->ڈالو_clip (extents);
}

ساكن فراغ
hb_paint_extents_نکالو_clip (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			   فراغ *paint_انکڑے,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  c->نکالو_clip ();
}

ساكن فراغ
hb_paint_extents_ڈالو_گروہ (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			     فراغ *paint_انکڑے,
			     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  c->ڈالو_گروہ ();
}

ساكن فراغ
hb_paint_extents_نکالو_گروہ (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			    فراغ *paint_انکڑے,
			    hb_paint_مرکب_منوال_t منوال,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  c->نکالو_گروہ (منوال);
}

ساكن hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			      فراغ *paint_انکڑے,
			      hb_blob_t *blob HB_بن_ورتیا,
			      بنانشانی صحيح چوڑائی HB_بن_ورتیا,
			      بنانشانی صحيح اچائی HB_بن_ورتیا,
			      hb_ٹیگ_t format HB_بن_ورتیا,
			      عائم slant HB_بن_ورتیا,
			      hb_glyph_extents_t *glyph_extents,
			      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  إذا (!glyph_extents)
    أرجع خطأ; // Happens with SVG images.

  hb_extents_t<> extents = {(عائم) glyph_extents->x_bearing,
			    (عائم) glyph_extents->y_bearing + glyph_extents->اچائی,
			    (عائم) glyph_extents->x_bearing + glyph_extents->چوڑائی,
			    (عائم) glyph_extents->y_bearing};
  c->ڈالو_clip (extents);
  c->paint ();
  c->نکالو_clip ();

  أرجع سچ;
}

ساكن فراغ
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			      فراغ *paint_انکڑے,
			      hb_bool_t use_foreground HB_بن_ورتیا,
			      hb_color_t color HB_بن_ورتیا,
			      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_خطی_gradient (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				        فراغ *paint_انکڑے,
				        hb_color_سطر_t *color_سطر HB_بن_ورتیا,
				        عائم x0 HB_بن_ورتیا, عائم y0 HB_بن_ورتیا,
				        عائم x1 HB_بن_ورتیا, عائم y1 HB_بن_ورتیا,
				        عائم x2 HB_بن_ورتیا, عائم y2 HB_بن_ورتیا,
				        فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				        فراغ *paint_انکڑے,
				        hb_color_سطر_t *color_سطر HB_بن_ورتیا,
				        عائم x0 HB_بن_ورتیا, عائم y0 HB_بن_ورتیا, عائم r0 HB_بن_ورتیا,
				        عائم x1 HB_بن_ورتیا, عائم y1 HB_بن_ورتیا, عائم r1 HB_بن_ورتیا,
				        فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				       فراغ *paint_انکڑے,
				       hb_color_سطر_t *color_سطر HB_بن_ورتیا,
				       عائم cx HB_بن_ورتیا, عائم cy HB_بن_ورتیا,
				       عائم شروع_angle HB_بن_ورتیا,
				       عائم سرا_angle HB_بن_ورتیا,
				       فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_انکڑے;

  c->paint ();
}

ساكن hb_bool_t
hb_paint_extents_سیٹ_budget (hb_paint_funcs_t *, فراغ *paint_انکڑے,
			     int64_t budget, فراغ *)
{
  خودکار *context = (hb_paint_extents_context_t *) paint_انکڑے;
  context->budget = budget;
  context->budget_initialized = سچ;
  context->recharge_budget ();
  أرجع سچ;
}

ساكن int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, فراغ *paint_انکڑے, فراغ *)
{
  أرجع ((hb_paint_extents_context_t *) paint_انکڑے)->budget;
}

ساكن int64_t *
hb_paint_extents_get_budget_بچیا (hb_paint_funcs_t *, فراغ *paint_انکڑے, فراغ *)
{
  أرجع &((hb_paint_extents_context_t *) paint_انکڑے)->budget_بچیا;
}

ساكن مضمن فراغ free_static_paint_extents_funcs ();

ساكن بنية hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سیٹ_ڈالو_transform_func (funcs, hb_paint_extents_ڈالو_transform, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_transform_func (funcs, hb_paint_extents_نکالو_transform, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, hb_paint_extents_ڈالو_clip_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, hb_paint_extents_ڈالو_clip_rectangle, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, hb_paint_extents_نکالو_clip, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, hb_paint_extents_ڈالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, hb_paint_extents_نکالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_color_func (funcs, hb_paint_extents_paint_color, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_image_func (funcs, hb_paint_extents_paint_image, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, hb_paint_extents_paint_خطی_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, hb_paint_extents_paint_radial_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, hb_paint_extents_paint_sweep_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_paint_extents_سیٹ_budget, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_paint_extents_get_budget, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_paint_extents_get_budget_بچیا, خالی_سوچک, خالی_سوچک);

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


#جے_انت
