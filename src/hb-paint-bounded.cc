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

#شامل "hb-paint-bounded.hh"

#شامل "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساکن خالی
hb_paint_bounded_ڈالو_clip_glyph (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				  خالی *paint_ڈیٹا,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ڈیٹا;

  c->ڈالو_clip ();
}

ساکن خالی
hb_paint_bounded_ڈالو_clip_rectangle (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				      خالی *paint_ڈیٹا,
				      اعشاری xmin, اعشاری ymin, اعشاری xmax, اعشاری ymax,
				      خالی *user_ڈیٹا)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ڈیٹا;

  c->ڈالو_clip ();
}

ساکن خالی
hb_paint_bounded_نکالو_clip (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			   خالی *paint_ڈیٹا,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ڈیٹا;

  c->نکالو_clip ();
}

ساکن خالی
hb_paint_bounded_ڈالو_گروہ (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			     خالی *paint_ڈیٹا,
			     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ڈیٹا;

  c->ڈالو_گروہ ();
}

ساکن خالی
hb_paint_bounded_نکالو_گروہ (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			    خالی *paint_ڈیٹا,
			    hb_paint_مرکب_منوال_t منوال,
			    خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ڈیٹا;

  c->نکالو_گروہ (منوال);
}

ساکن hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			      خالی *paint_ڈیٹا,
			      hb_blob_t *blob HB_غیر_مستعمل,
			      غیرعلامتی صحیح چوڑائی HB_غیر_مستعمل,
			      غیرعلامتی صحیح اونچائی HB_غیر_مستعمل,
			      hb_لیبل_t format HB_غیر_مستعمل,
			      اعشاری slant HB_غیر_مستعمل,
			      hb_glyph_extents_t *glyph_extents,
			      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ڈیٹا;

  c->ڈالو_clip ();
  c->paint ();
  c->نکالو_clip ();

  واپس سچ;
}

ساکن خالی
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			      خالی *paint_ڈیٹا,
			      hb_bool_t use_foreground HB_غیر_مستعمل,
			      hb_color_t color HB_غیر_مستعمل,
			      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ڈیٹا;

  c->paint ();
}

ساکن خالی
hb_paint_bounded_paint_خطی_gradient (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				        خالی *paint_ڈیٹا,
				        hb_color_سطر_t *color_سطر HB_غیر_مستعمل,
				        اعشاری x0 HB_غیر_مستعمل, اعشاری y0 HB_غیر_مستعمل,
				        اعشاری x1 HB_غیر_مستعمل, اعشاری y1 HB_غیر_مستعمل,
				        اعشاری x2 HB_غیر_مستعمل, اعشاری y2 HB_غیر_مستعمل,
				        خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ڈیٹا;

  c->paint ();
}

ساکن خالی
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				        خالی *paint_ڈیٹا,
				        hb_color_سطر_t *color_سطر HB_غیر_مستعمل,
				        اعشاری x0 HB_غیر_مستعمل, اعشاری y0 HB_غیر_مستعمل, اعشاری r0 HB_غیر_مستعمل,
				        اعشاری x1 HB_غیر_مستعمل, اعشاری y1 HB_غیر_مستعمل, اعشاری r1 HB_غیر_مستعمل,
				        خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ڈیٹا;

  c->paint ();
}

ساکن خالی
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				       خالی *paint_ڈیٹا,
				       hb_color_سطر_t *color_سطر HB_غیر_مستعمل,
				       اعشاری cx HB_غیر_مستعمل, اعشاری cy HB_غیر_مستعمل,
				       اعشاری آغاز_angle HB_غیر_مستعمل,
				       اعشاری انتہا_angle HB_غیر_مستعمل,
				       خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ڈیٹا;

  c->paint ();
}

ساکن اندرونی خالی free_static_paint_bounded_funcs ();

ساکن ساخت hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  ساکن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, hb_paint_bounded_ڈالو_clip_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, hb_paint_bounded_ڈالو_clip_rectangle, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, hb_paint_bounded_نکالو_clip, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, hb_paint_bounded_ڈالو_گروہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, hb_paint_bounded_نکالو_گروہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_color_func (funcs, hb_paint_bounded_paint_color, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_image_func (funcs, hb_paint_bounded_paint_image, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, hb_paint_bounded_paint_خطی_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, خالی_اشاریہ, خالی_اشاریہ);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_bounded_funcs);

    واپس funcs;
  }
} static_paint_bounded_funcs;

ساکن اندرونی
خالی free_static_paint_bounded_funcs ()
{
  static_paint_bounded_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_bounded_get_funcs ()
{
  واپس static_paint_bounded_funcs.get_unconst ();
}


#اگر_ختم
