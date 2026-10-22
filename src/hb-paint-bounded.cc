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

#ifndef HB_NO_PAINT

#ضمن "hb-paint-bounded.hh"

#ضمن "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساكن فراغ
hb_paint_bounded_push_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  فراغ *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  فراغ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->push_clip ();
}

ساكن فراغ
hb_paint_bounded_push_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      فراغ *paint_data,
				      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
				      فراغ *user_data)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->push_clip ();
}

ساكن فراغ
hb_paint_bounded_pop_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   فراغ *paint_data,
			   فراغ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->pop_clip ();
}

ساكن فراغ
hb_paint_bounded_push_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     فراغ *paint_data,
			     فراغ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->push_group ();
}

ساكن فراغ
hb_paint_bounded_pop_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    فراغ *paint_data,
			    hb_paint_composite_mode_t mode,
			    فراغ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->pop_group (mode);
}

ساكن hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      فراغ *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned صحيح width HB_UNUSED,
			      unsigned صحيح height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      عائم slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      فراغ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->push_clip ();
  c->paint ();
  c->pop_clip ();

  أرجع 𐡒𐡔𐡈;
}

ساكن فراغ
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      فراغ *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      فراغ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        فراغ *paint_data,
				        hb_color_line_t *color_line HB_UNUSED,
				        عائم x0 HB_UNUSED, عائم y0 HB_UNUSED,
				        عائم x1 HB_UNUSED, عائم y1 HB_UNUSED,
				        عائم x2 HB_UNUSED, عائم y2 HB_UNUSED,
				        فراغ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        فراغ *paint_data,
				        hb_color_line_t *color_line HB_UNUSED,
				        عائم x0 HB_UNUSED, عائم y0 HB_UNUSED, عائم r0 HB_UNUSED,
				        عائم x1 HB_UNUSED, عائم y1 HB_UNUSED, عائم r1 HB_UNUSED,
				        فراغ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ساكن فراغ
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       فراغ *paint_data,
				       hb_color_line_t *color_line HB_UNUSED,
				       عائم cx HB_UNUSED, عائم cy HB_UNUSED,
				       عائم start_angle HB_UNUSED,
				       عائم end_angle HB_UNUSED,
				       فراغ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ساكن مضمن فراغ free_static_paint_bounded_funcs ();

ساكن بنية hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_push_clip_glyph_func (funcs, hb_paint_bounded_push_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_push_clip_rectangle_func (funcs, hb_paint_bounded_push_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_pop_clip_func (funcs, hb_paint_bounded_pop_clip, nullptr, nullptr);
    hb_paint_funcs_set_push_group_func (funcs, hb_paint_bounded_push_group, nullptr, nullptr);
    hb_paint_funcs_set_pop_group_func (funcs, hb_paint_bounded_pop_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_paint_bounded_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_paint_bounded_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_paint_bounded_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, nullptr, nullptr);

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


#endif
