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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_PAINT

#ଅନ୍ତର୍ଭାଵ "hb-paint-bounded.hh"

#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_bounded_ଠେଲ_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  ଶୂନ୍ୟ *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ଠେଲ_clip ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_bounded_ଠେଲ_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      ଶୂନ୍ୟ *paint_data,
				      ଦଶମିକ xmin, ଦଶମିକ ymin, ଦଶମିକ xmax, ଦଶମିକ ymax,
				      ଶୂନ୍ୟ *user_data)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ଠେଲ_clip ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_bounded_କାଢ଼_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   ଶୂନ୍ୟ *paint_data,
			   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->କାଢ଼_clip ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_bounded_ଠେଲ_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     ଶୂନ୍ୟ *paint_data,
			     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ଠେଲ_group ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_bounded_କାଢ଼_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    ଶୂନ୍ୟ *paint_data,
			    hb_paint_composite_mode_t mode,
			    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->କାଢ଼_group (mode);
}

ସ୍ଥିର hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      ଶୂନ୍ୟ *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା width HB_UNUSED,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      ଦଶମିକ slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ଠେଲ_clip ();
  c->paint ();
  c->କାଢ଼_clip ();

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      ଶୂନ୍ୟ *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_bounded_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        ଶୂନ୍ୟ *paint_data,
				        hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି HB_UNUSED,
				        ଦଶମିକ x0 HB_UNUSED, ଦଶମିକ y0 HB_UNUSED,
				        ଦଶମିକ x1 HB_UNUSED, ଦଶମିକ y1 HB_UNUSED,
				        ଦଶମିକ x2 HB_UNUSED, ଦଶମିକ y2 HB_UNUSED,
				        ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        ଶୂନ୍ୟ *paint_data,
				        hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି HB_UNUSED,
				        ଦଶମିକ x0 HB_UNUSED, ଦଶମିକ y0 HB_UNUSED, ଦଶମିକ r0 HB_UNUSED,
				        ଦଶମିକ x1 HB_UNUSED, ଦଶମିକ y1 HB_UNUSED, ଦଶମିକ r1 HB_UNUSED,
				        ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       ଶୂନ୍ୟ *paint_data,
				       hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି HB_UNUSED,
				       ଦଶମିକ cx HB_UNUSED, ଦଶମିକ cy HB_UNUSED,
				       ଦଶମିକ start_angle HB_UNUSED,
				       ଦଶମିକ end_angle HB_UNUSED,
				       ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_paint_bounded_funcs ();

ସ୍ଥିର ସଂରଚନା hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ଠେଲ_clip_glyph_func (funcs, hb_paint_bounded_ଠେଲ_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_rectangle_func (funcs, hb_paint_bounded_ଠେଲ_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_clip_func (funcs, hb_paint_bounded_କାଢ଼_clip, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_group_func (funcs, hb_paint_bounded_ଠେଲ_group, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_group_func (funcs, hb_paint_bounded_କାଢ଼_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_paint_bounded_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_paint_bounded_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_paint_bounded_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_bounded_funcs);

    ଫେରନ୍ତୁ funcs;
  }
} static_paint_bounded_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_paint_bounded_funcs ()
{
  static_paint_bounded_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_bounded_get_funcs ()
{
  ଫେରନ୍ତୁ static_paint_bounded_funcs.get_unconst ();
}


#endif
