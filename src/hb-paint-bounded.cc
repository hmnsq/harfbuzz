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

#ضمن "hb-paint-bounded.hh"

#ضمن "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساكن فراغ
hb_paint_bounded_ټېل_clip_glyph (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				  فراغ *paint_معلومات,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->ټېل_clip ();
}

ساكن فراغ
hb_paint_bounded_ټېل_clip_rectangle (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				      فراغ *paint_معلومات,
				      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
				      فراغ *user_معلومات)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->ټېل_clip ();
}

ساكن فراغ
hb_paint_bounded_راوباسه_clip (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			   فراغ *paint_معلومات,
			   فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->راوباسه_clip ();
}

ساكن فراغ
hb_paint_bounded_ټېل_ډله (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			     فراغ *paint_معلومات,
			     فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->ټېل_ډله ();
}

ساكن فراغ
hb_paint_bounded_راوباسه_ډله (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			    فراغ *paint_معلومات,
			    hb_paint_مرکب_ډېرپېښ_t ډېرپېښ,
			    فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->راوباسه_ډله (ډېرپېښ);
}

ساكن hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			      فراغ *paint_معلومات,
			      hb_blob_t *blob HB_ناکارول_شوی,
			      بینښه صحيح پلنوالی HB_ناکارول_شوی,
			      بینښه صحيح لوړوالی HB_ناکارول_شوی,
			      hb_نښان_t format HB_ناکارول_شوی,
			      عائم slant HB_ناکارول_شوی,
			      hb_glyph_extents_t *glyph_extents,
			      فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->ټېل_clip ();
  c->paint ();
  c->راوباسه_clip ();

  أرجع ریښتیا;
}

ساكن فراغ
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			      فراغ *paint_معلومات,
			      hb_bool_t use_foreground HB_ناکارول_شوی,
			      hb_color_t color HB_ناکارول_شوی,
			      فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_خطي_gradient (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				        فراغ *paint_معلومات,
				        hb_color_کرښه_t *color_کرښه HB_ناکارول_شوی,
				        عائم x0 HB_ناکارول_شوی, عائم y0 HB_ناکارول_شوی,
				        عائم x1 HB_ناکارول_شوی, عائم y1 HB_ناکارول_شوی,
				        عائم x2 HB_ناکارول_شوی, عائم y2 HB_ناکارول_شوی,
				        فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				        فراغ *paint_معلومات,
				        hb_color_کرښه_t *color_کرښه HB_ناکارول_شوی,
				        عائم x0 HB_ناکارول_شوی, عائم y0 HB_ناکارول_شوی, عائم r0 HB_ناکارول_شوی,
				        عائم x1 HB_ناکارول_شوی, عائم y1 HB_ناکارول_شوی, عائم r1 HB_ناکارول_شوی,
				        فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				       فراغ *paint_معلومات,
				       hb_color_کرښه_t *color_کرښه HB_ناکارول_شوی,
				       عائم cx HB_ناکارول_شوی, عائم cy HB_ناکارول_شوی,
				       عائم پیل_angle HB_ناکارول_شوی,
				       عائم پای_ټکی_angle HB_ناکارول_شوی,
				       فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->paint ();
}

ساكن مضمن فراغ free_static_paint_bounded_funcs ();

ساكن بنية hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سېټ_ټېل_clip_glyph_func (funcs, hb_paint_bounded_ټېل_clip_glyph, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_clip_rectangle_func (funcs, hb_paint_bounded_ټېل_clip_rectangle, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_clip_func (funcs, hb_paint_bounded_راوباسه_clip, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_ډله_func (funcs, hb_paint_bounded_ټېل_ډله, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_ډله_func (funcs, hb_paint_bounded_راوباسه_ډله, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_color_func (funcs, hb_paint_bounded_paint_color, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_image_func (funcs, hb_paint_bounded_paint_image, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_خطي_gradient_func (funcs, hb_paint_bounded_paint_خطي_gradient, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, تش_اشاره, تش_اشاره);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_bounded_funcs);

    أرجع funcs;
  }
} static_paint_bounded_funcs;

ساكن مضمن
فراغ free_static_paint_bounded_funcs ()
{
  static_paint_bounded_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_bounded_get_funcs ()
{
  أرجع static_paint_bounded_funcs.get_unconst ();
}


#د_که_پای
