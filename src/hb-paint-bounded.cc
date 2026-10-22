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

#ضمن "hb-paint-bounded.hh"

#ضمن "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساكن فراغ
hb_paint_bounded_ڌڪو_clip_glyph (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				  فراغ *paint_معلومات,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->ڌڪو_clip ();
}

ساكن فراغ
hb_paint_bounded_ڌڪو_clip_rectangle (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				      فراغ *paint_معلومات,
				      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
				      فراغ *user_معلومات)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->ڌڪو_clip ();
}

ساكن فراغ
hb_paint_bounded_ڪڍو_clip (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			   فراغ *paint_معلومات,
			   فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->ڪڍو_clip ();
}

ساكن فراغ
hb_paint_bounded_ڌڪو_گروهه (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			     فراغ *paint_معلومات,
			     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->ڌڪو_گروهه ();
}

ساكن فراغ
hb_paint_bounded_ڪڍو_گروهه (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			    فراغ *paint_معلومات,
			    hb_paint_مرکب_گهڻائي_t گهڻائي,
			    فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->ڪڍو_گروهه (گهڻائي);
}

ساكن hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			      فراغ *paint_معلومات,
			      hb_blob_t *blob HB_اڻ_استعمال_ٿيل,
			      بنانشان صحيح ويڪر HB_اڻ_استعمال_ٿيل,
			      بنانشان صحيح اوچائي HB_اڻ_استعمال_ٿيل,
			      hb_ٽيگ_t format HB_اڻ_استعمال_ٿيل,
			      عائم slant HB_اڻ_استعمال_ٿيل,
			      hb_glyph_extents_t *glyph_extents,
			      فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->ڌڪو_clip ();
  c->paint ();
  c->ڪڍو_clip ();

  أرجع سچ;
}

ساكن فراغ
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
			      فراغ *paint_معلومات,
			      hb_bool_t use_foreground HB_اڻ_استعمال_ٿيل,
			      hb_color_t color HB_اڻ_استعمال_ٿيل,
			      فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_لڪيري_gradient (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				        فراغ *paint_معلومات,
				        hb_color_سٽ_t *color_سٽ HB_اڻ_استعمال_ٿيل,
				        عائم x0 HB_اڻ_استعمال_ٿيل, عائم y0 HB_اڻ_استعمال_ٿيل,
				        عائم x1 HB_اڻ_استعمال_ٿيل, عائم y1 HB_اڻ_استعمال_ٿيل,
				        عائم x2 HB_اڻ_استعمال_ٿيل, عائم y2 HB_اڻ_استعمال_ٿيل,
				        فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				        فراغ *paint_معلومات,
				        hb_color_سٽ_t *color_سٽ HB_اڻ_استعمال_ٿيل,
				        عائم x0 HB_اڻ_استعمال_ٿيل, عائم y0 HB_اڻ_استعمال_ٿيل, عائم r0 HB_اڻ_استعمال_ٿيل,
				        عائم x1 HB_اڻ_استعمال_ٿيل, عائم y1 HB_اڻ_استعمال_ٿيل, عائم r1 HB_اڻ_استعمال_ٿيل,
				        فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_معلومات;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_اڻ_استعمال_ٿيل,
				       فراغ *paint_معلومات,
				       hb_color_سٽ_t *color_سٽ HB_اڻ_استعمال_ٿيل,
				       عائم cx HB_اڻ_استعمال_ٿيل, عائم cy HB_اڻ_استعمال_ٿيل,
				       عائم شروعات_angle HB_اڻ_استعمال_ٿيل,
				       عائم پڇاڙي_angle HB_اڻ_استعمال_ٿيل,
				       فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
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

    hb_paint_funcs_سيٽ_ڌڪو_clip_glyph_func (funcs, hb_paint_bounded_ڌڪو_clip_glyph, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_rectangle_func (funcs, hb_paint_bounded_ڌڪو_clip_rectangle, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_clip_func (funcs, hb_paint_bounded_ڪڍو_clip, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_گروهه_func (funcs, hb_paint_bounded_ڌڪو_گروهه, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_گروهه_func (funcs, hb_paint_bounded_ڪڍو_گروهه, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_color_func (funcs, hb_paint_bounded_paint_color, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_image_func (funcs, hb_paint_bounded_paint_image, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_لڪيري_gradient_func (funcs, hb_paint_bounded_paint_لڪيري_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, خالي_اشارو, خالي_اشارو);

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


#جيڪڏهن_پورو
