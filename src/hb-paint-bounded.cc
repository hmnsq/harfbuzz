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

#ifndef HB_NO_PAINT

#شامل "hb-paint-bounded.hh"

#شامل "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ایستا تهی
hb_paint_bounded_افزودن_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  تهی *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  تهی *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->افزودن_clip ();
}

ایستا تهی
hb_paint_bounded_افزودن_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      تهی *paint_data,
				      اعشاری xmin, اعشاری ymin, اعشاری xmax, اعشاری ymax,
				      تهی *user_data)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->افزودن_clip ();
}

ایستا تهی
hb_paint_bounded_برداشت_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   تهی *paint_data,
			   تهی *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->برداشت_clip ();
}

ایستا تهی
hb_paint_bounded_افزودن_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     تهی *paint_data,
			     تهی *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->افزودن_group ();
}

ایستا تهی
hb_paint_bounded_برداشت_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    تهی *paint_data,
			    hb_paint_composite_mode_t mode,
			    تهی *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->برداشت_group (mode);
}

ایستا hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      تهی *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned صحیح width HB_UNUSED,
			      unsigned صحیح height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      اعشاری slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      تهی *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->افزودن_clip ();
  c->paint ();
  c->برداشت_clip ();

  بازگشت درست;
}

ایستا تهی
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      تهی *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      تهی *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ایستا تهی
hb_paint_bounded_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        تهی *paint_data,
				        hb_color_سطر_t *color_سطر HB_UNUSED,
				        اعشاری x0 HB_UNUSED, اعشاری y0 HB_UNUSED,
				        اعشاری x1 HB_UNUSED, اعشاری y1 HB_UNUSED,
				        اعشاری x2 HB_UNUSED, اعشاری y2 HB_UNUSED,
				        تهی *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ایستا تهی
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        تهی *paint_data,
				        hb_color_سطر_t *color_سطر HB_UNUSED,
				        اعشاری x0 HB_UNUSED, اعشاری y0 HB_UNUSED, اعشاری r0 HB_UNUSED,
				        اعشاری x1 HB_UNUSED, اعشاری y1 HB_UNUSED, اعشاری r1 HB_UNUSED,
				        تهی *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ایستا تهی
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       تهی *paint_data,
				       hb_color_سطر_t *color_سطر HB_UNUSED,
				       اعشاری cx HB_UNUSED, اعشاری cy HB_UNUSED,
				       اعشاری start_angle HB_UNUSED,
				       اعشاری end_angle HB_UNUSED,
				       تهی *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ایستا درونخطی تهی free_static_paint_bounded_funcs ();

ایستا ساختار hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  ایستا hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_افزودن_clip_glyph_func (funcs, hb_paint_bounded_افزودن_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_افزودن_clip_rectangle_func (funcs, hb_paint_bounded_افزودن_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_برداشت_clip_func (funcs, hb_paint_bounded_برداشت_clip, nullptr, nullptr);
    hb_paint_funcs_set_افزودن_group_func (funcs, hb_paint_bounded_افزودن_group, nullptr, nullptr);
    hb_paint_funcs_set_برداشت_group_func (funcs, hb_paint_bounded_برداشت_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_paint_bounded_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_paint_bounded_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_paint_bounded_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_bounded_funcs);

    بازگشت funcs;
  }
} static_paint_bounded_funcs;

ایستا درونخطی
تهی free_static_paint_bounded_funcs ()
{
  static_paint_bounded_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_bounded_get_funcs ()
{
  بازگشت static_paint_bounded_funcs.get_unconst ();
}


#endif
