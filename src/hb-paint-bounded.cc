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

#ضمن "hb-paint-bounded.hh"

#ضمن "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساكن فراغ
hb_paint_bounded_افزودن_clip_glyph (hb_paint_funcs_t *funcs HB_استفادهنشده,
				  فراغ *paint_داده,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_داده;

  c->افزودن_clip ();
}

ساكن فراغ
hb_paint_bounded_افزودن_clip_rectangle (hb_paint_funcs_t *funcs HB_استفادهنشده,
				      فراغ *paint_داده,
				      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
				      فراغ *user_داده)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_داده;

  c->افزودن_clip ();
}

ساكن فراغ
hb_paint_bounded_برداشت_clip (hb_paint_funcs_t *funcs HB_استفادهنشده,
			   فراغ *paint_داده,
			   فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_داده;

  c->برداشت_clip ();
}

ساكن فراغ
hb_paint_bounded_افزودن_گروه (hb_paint_funcs_t *funcs HB_استفادهنشده,
			     فراغ *paint_داده,
			     فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_داده;

  c->افزودن_گروه ();
}

ساكن فراغ
hb_paint_bounded_برداشت_گروه (hb_paint_funcs_t *funcs HB_استفادهنشده,
			    فراغ *paint_داده,
			    hb_paint_مرکب_نما_t نما,
			    فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_داده;

  c->برداشت_گروه (نما);
}

ساكن hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_استفادهنشده,
			      فراغ *paint_داده,
			      hb_blob_t *blob HB_استفادهنشده,
			      بیعلامت صحيح عرض HB_استفادهنشده,
			      بیعلامت صحيح ارتفاع HB_استفادهنشده,
			      hb_برچسب_t format HB_استفادهنشده,
			      عائم slant HB_استفادهنشده,
			      hb_glyph_extents_t *glyph_extents,
			      فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_داده;

  c->افزودن_clip ();
  c->paint ();
  c->برداشت_clip ();

  أرجع درست;
}

ساكن فراغ
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_استفادهنشده,
			      فراغ *paint_داده,
			      hb_bool_t use_foreground HB_استفادهنشده,
			      hb_color_t color HB_استفادهنشده,
			      فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_داده;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_خطی_gradient (hb_paint_funcs_t *funcs HB_استفادهنشده,
				        فراغ *paint_داده,
				        hb_color_سطر_t *color_سطر HB_استفادهنشده,
				        عائم x0 HB_استفادهنشده, عائم y0 HB_استفادهنشده,
				        عائم x1 HB_استفادهنشده, عائم y1 HB_استفادهنشده,
				        عائم x2 HB_استفادهنشده, عائم y2 HB_استفادهنشده,
				        فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_داده;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_استفادهنشده,
				        فراغ *paint_داده,
				        hb_color_سطر_t *color_سطر HB_استفادهنشده,
				        عائم x0 HB_استفادهنشده, عائم y0 HB_استفادهنشده, عائم r0 HB_استفادهنشده,
				        عائم x1 HB_استفادهنشده, عائم y1 HB_استفادهنشده, عائم r1 HB_استفادهنشده,
				        فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_داده;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_استفادهنشده,
				       فراغ *paint_داده,
				       hb_color_سطر_t *color_سطر HB_استفادهنشده,
				       عائم cx HB_استفادهنشده, عائم cy HB_استفادهنشده,
				       عائم شروع_angle HB_استفادهنشده,
				       عائم انتها_angle HB_استفادهنشده,
				       فراغ *user_داده HB_استفادهنشده)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_داده;

  c->paint ();
}

ساكن مضمن فراغ free_static_paint_bounded_funcs ();

ساكن بنية hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_مجموعه_افزودن_clip_glyph_func (funcs, hb_paint_bounded_افزودن_clip_glyph, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_rectangle_func (funcs, hb_paint_bounded_افزودن_clip_rectangle, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_clip_func (funcs, hb_paint_bounded_برداشت_clip, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_گروه_func (funcs, hb_paint_bounded_افزودن_گروه, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_گروه_func (funcs, hb_paint_bounded_برداشت_گروه, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_color_func (funcs, hb_paint_bounded_paint_color, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_image_func (funcs, hb_paint_bounded_paint_image, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_خطی_gradient_func (funcs, hb_paint_bounded_paint_خطی_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, اشاره_تهی, اشاره_تهی);

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


#پایان_اگر
