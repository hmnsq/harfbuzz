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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifndef HB_NO_PAINT

#ಅನ್ತರ್ಭಾವ "hb-paint-bounded.hh"

#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ಸ್ಥಿರ ಶೂನ್ಯ
hb_paint_bounded_ತಳ್ಳು_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  ಶೂನ್ಯ *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ತಳ್ಳು_clip ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_paint_bounded_ತಳ್ಳು_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      ಶೂನ್ಯ *paint_data,
				      ಭಿನ್ನ xmin, ಭಿನ್ನ ymin, ಭಿನ್ನ xmax, ಭಿನ್ನ ymax,
				      ಶೂನ್ಯ *user_data)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ತಳ್ಳು_clip ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_paint_bounded_ತೆಗೆ_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   ಶೂನ್ಯ *paint_data,
			   ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ತೆಗೆ_clip ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_paint_bounded_ತಳ್ಳು_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     ಶೂನ್ಯ *paint_data,
			     ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ತಳ್ಳು_group ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_paint_bounded_ತೆಗೆ_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    ಶೂನ್ಯ *paint_data,
			    hb_paint_composite_mode_t mode,
			    ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ತೆಗೆ_group (mode);
}

ಸ್ಥಿರ hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      ಶೂನ್ಯ *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned ಪೂರ್ಣಾಙ್ಕ width HB_UNUSED,
			      unsigned ಪೂರ್ಣಾಙ್ಕ height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      ಭಿನ್ನ slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ತಳ್ಳು_clip ();
  c->paint ();
  c->ತೆಗೆ_clip ();

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      ಶೂನ್ಯ *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_paint_bounded_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        ಶೂನ್ಯ *paint_data,
				        hb_color_ಸಾಲು_t *color_ಸಾಲು HB_UNUSED,
				        ಭಿನ್ನ x0 HB_UNUSED, ಭಿನ್ನ y0 HB_UNUSED,
				        ಭಿನ್ನ x1 HB_UNUSED, ಭಿನ್ನ y1 HB_UNUSED,
				        ಭಿನ್ನ x2 HB_UNUSED, ಭಿನ್ನ y2 HB_UNUSED,
				        ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        ಶೂನ್ಯ *paint_data,
				        hb_color_ಸಾಲು_t *color_ಸಾಲು HB_UNUSED,
				        ಭಿನ್ನ x0 HB_UNUSED, ಭಿನ್ನ y0 HB_UNUSED, ಭಿನ್ನ r0 HB_UNUSED,
				        ಭಿನ್ನ x1 HB_UNUSED, ಭಿನ್ನ y1 HB_UNUSED, ಭಿನ್ನ r1 HB_UNUSED,
				        ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       ಶೂನ್ಯ *paint_data,
				       hb_color_ಸಾಲು_t *color_ಸಾಲು HB_UNUSED,
				       ಭಿನ್ನ cx HB_UNUSED, ಭಿನ್ನ cy HB_UNUSED,
				       ಭಿನ್ನ start_angle HB_UNUSED,
				       ಭಿನ್ನ end_angle HB_UNUSED,
				       ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_paint_bounded_funcs ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  ಸ್ಥಿರ hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ತಳ್ಳು_clip_glyph_func (funcs, hb_paint_bounded_ತಳ್ಳು_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_clip_rectangle_func (funcs, hb_paint_bounded_ತಳ್ಳು_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_ತೆಗೆ_clip_func (funcs, hb_paint_bounded_ತೆಗೆ_clip, nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_group_func (funcs, hb_paint_bounded_ತಳ್ಳು_group, nullptr, nullptr);
    hb_paint_funcs_set_ತೆಗೆ_group_func (funcs, hb_paint_bounded_ತೆಗೆ_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_paint_bounded_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_paint_bounded_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_paint_bounded_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_bounded_funcs);

    ಪ್ರತ್ಯಾಗಮ funcs;
  }
} static_paint_bounded_funcs;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ free_static_paint_bounded_funcs ()
{
  static_paint_bounded_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_bounded_get_funcs ()
{
  ಪ್ರತ್ಯಾಗಮ static_paint_bounded_funcs.get_unconst ();
}


#endif
