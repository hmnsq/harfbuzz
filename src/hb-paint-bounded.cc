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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_PAINT

#अन्तर्भाव "hb-paint-bounded.hh"

#अन्तर्भाव "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

स्थिर शून्य
hb_paint_bounded_प्रक्षिप_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  शून्य *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  शून्य *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->प्रक्षिप_clip ();
}

स्थिर शून्य
hb_paint_bounded_प्रक्षिप_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      शून्य *paint_data,
				      भिन्न xmin, भिन्न ymin, भिन्न xmax, भिन्न ymax,
				      शून्य *user_data)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->प्रक्षिप_clip ();
}

स्थिर शून्य
hb_paint_bounded_निष्कासय_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   शून्य *paint_data,
			   शून्य *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->निष्कासय_clip ();
}

स्थिर शून्य
hb_paint_bounded_प्रक्षिप_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     शून्य *paint_data,
			     शून्य *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->प्रक्षिप_group ();
}

स्थिर शून्य
hb_paint_bounded_निष्कासय_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    शून्य *paint_data,
			    hb_paint_composite_mode_t mode,
			    शून्य *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->निष्कासय_group (mode);
}

स्थिर hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      शून्य *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned पूर्णाङ्क width HB_UNUSED,
			      unsigned पूर्णाङ्क height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      भिन्न slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      शून्य *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->प्रक्षिप_clip ();
  c->paint ();
  c->निष्कासय_clip ();

  प्रत्यागम सच्च;
}

स्थिर शून्य
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      शून्य *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      शून्य *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

स्थिर शून्य
hb_paint_bounded_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        शून्य *paint_data,
				        hb_color_पङ्क्तिः_t *color_पङ्क्तिः HB_UNUSED,
				        भिन्न x0 HB_UNUSED, भिन्न y0 HB_UNUSED,
				        भिन्न x1 HB_UNUSED, भिन्न y1 HB_UNUSED,
				        भिन्न x2 HB_UNUSED, भिन्न y2 HB_UNUSED,
				        शून्य *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

स्थिर शून्य
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        शून्य *paint_data,
				        hb_color_पङ्क्तिः_t *color_पङ्क्तिः HB_UNUSED,
				        भिन्न x0 HB_UNUSED, भिन्न y0 HB_UNUSED, भिन्न r0 HB_UNUSED,
				        भिन्न x1 HB_UNUSED, भिन्न y1 HB_UNUSED, भिन्न r1 HB_UNUSED,
				        शून्य *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

स्थिर शून्य
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       शून्य *paint_data,
				       hb_color_पङ्क्तिः_t *color_पङ्क्तिः HB_UNUSED,
				       भिन्न cx HB_UNUSED, भिन्न cy HB_UNUSED,
				       भिन्न start_angle HB_UNUSED,
				       भिन्न end_angle HB_UNUSED,
				       शून्य *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

स्थिर अन्तःस्थ शून्य free_static_paint_bounded_funcs ();

स्थिर संरचना hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  स्थिर hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_प्रक्षिप_clip_glyph_func (funcs, hb_paint_bounded_प्रक्षिप_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_प्रक्षिप_clip_rectangle_func (funcs, hb_paint_bounded_प्रक्षिप_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_निष्कासय_clip_func (funcs, hb_paint_bounded_निष्कासय_clip, nullptr, nullptr);
    hb_paint_funcs_set_प्रक्षिप_group_func (funcs, hb_paint_bounded_प्रक्षिप_group, nullptr, nullptr);
    hb_paint_funcs_set_निष्कासय_group_func (funcs, hb_paint_bounded_निष्कासय_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_paint_bounded_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_paint_bounded_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_paint_bounded_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_bounded_funcs);

    प्रत्यागम funcs;
  }
} static_paint_bounded_funcs;

स्थिर अन्तःस्थ
शून्य free_static_paint_bounded_funcs ()
{
  static_paint_bounded_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_bounded_get_funcs ()
{
  प्रत्यागम static_paint_bounded_funcs.get_unconst ();
}


#endif
