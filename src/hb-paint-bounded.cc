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

#ضمن "hb-paint-bounded.hh"

#ضمن "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساكن فراغ
hb_paint_bounded_ڈالو_clip_glyph (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				  فراغ *paint_انکڑے,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_انکڑے;

  c->ڈالو_clip ();
}

ساكن فراغ
hb_paint_bounded_ڈالو_clip_rectangle (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				      فراغ *paint_انکڑے,
				      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
				      فراغ *user_انکڑے)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_انکڑے;

  c->ڈالو_clip ();
}

ساكن فراغ
hb_paint_bounded_نکالو_clip (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			   فراغ *paint_انکڑے,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_انکڑے;

  c->نکالو_clip ();
}

ساكن فراغ
hb_paint_bounded_ڈالو_گروہ (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			     فراغ *paint_انکڑے,
			     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_انکڑے;

  c->ڈالو_گروہ ();
}

ساكن فراغ
hb_paint_bounded_نکالو_گروہ (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			    فراغ *paint_انکڑے,
			    hb_paint_مرکب_منوال_t منوال,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_انکڑے;

  c->نکالو_گروہ (منوال);
}

ساكن hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			      فراغ *paint_انکڑے,
			      hb_blob_t *blob HB_بن_ورتیا,
			      بنانشانی صحيح چوڑائی HB_بن_ورتیا,
			      بنانشانی صحيح اچائی HB_بن_ورتیا,
			      hb_ٹیگ_t format HB_بن_ورتیا,
			      عائم slant HB_بن_ورتیا,
			      hb_glyph_extents_t *glyph_extents,
			      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_انکڑے;

  c->ڈالو_clip ();
  c->paint ();
  c->نکالو_clip ();

  أرجع سچ;
}

ساكن فراغ
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			      فراغ *paint_انکڑے,
			      hb_bool_t use_foreground HB_بن_ورتیا,
			      hb_color_t color HB_بن_ورتیا,
			      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_انکڑے;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_خطی_gradient (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				        فراغ *paint_انکڑے,
				        hb_color_سطر_t *color_سطر HB_بن_ورتیا,
				        عائم x0 HB_بن_ورتیا, عائم y0 HB_بن_ورتیا,
				        عائم x1 HB_بن_ورتیا, عائم y1 HB_بن_ورتیا,
				        عائم x2 HB_بن_ورتیا, عائم y2 HB_بن_ورتیا,
				        فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_انکڑے;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				        فراغ *paint_انکڑے,
				        hb_color_سطر_t *color_سطر HB_بن_ورتیا,
				        عائم x0 HB_بن_ورتیا, عائم y0 HB_بن_ورتیا, عائم r0 HB_بن_ورتیا,
				        عائم x1 HB_بن_ورتیا, عائم y1 HB_بن_ورتیا, عائم r1 HB_بن_ورتیا,
				        فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_انکڑے;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				       فراغ *paint_انکڑے,
				       hb_color_سطر_t *color_سطر HB_بن_ورتیا,
				       عائم cx HB_بن_ورتیا, عائم cy HB_بن_ورتیا,
				       عائم شروع_angle HB_بن_ورتیا,
				       عائم سرا_angle HB_بن_ورتیا,
				       فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_انکڑے;

  c->paint ();
}

ساكن مضمن فراغ free_static_paint_bounded_funcs ();

ساكن بنية hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, hb_paint_bounded_ڈالو_clip_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, hb_paint_bounded_ڈالو_clip_rectangle, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, hb_paint_bounded_نکالو_clip, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, hb_paint_bounded_ڈالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, hb_paint_bounded_نکالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_color_func (funcs, hb_paint_bounded_paint_color, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_image_func (funcs, hb_paint_bounded_paint_image, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, hb_paint_bounded_paint_خطی_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, خالی_سوچک, خالی_سوچک);

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


#جے_انت
