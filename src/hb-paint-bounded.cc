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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_PAINT

#അന്തര്ഭാവ "hb-paint-bounded.hh"

#അന്തര്ഭാവ "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

സ്ഥിതം ശൂന്യം
hb_paint_bounded_തള്ളുക_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  ശൂന്യം *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->തള്ളുക_clip ();
}

സ്ഥിതം ശൂന്യം
hb_paint_bounded_തള്ളുക_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      ശൂന്യം *paint_data,
				      ദശാംശം xmin, ദശാംശം ymin, ദശാംശം xmax, ദശാംശം ymax,
				      ശൂന്യം *user_data)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->തള്ളുക_clip ();
}

സ്ഥിതം ശൂന്യം
hb_paint_bounded_എടുക്കുക_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   ശൂന്യം *paint_data,
			   ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->എടുക്കുക_clip ();
}

സ്ഥിതം ശൂന്യം
hb_paint_bounded_തള്ളുക_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     ശൂന്യം *paint_data,
			     ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->തള്ളുക_group ();
}

സ്ഥിതം ശൂന്യം
hb_paint_bounded_എടുക്കുക_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    ശൂന്യം *paint_data,
			    hb_paint_composite_mode_t mode,
			    ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->എടുക്കുക_group (mode);
}

സ്ഥിതം hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      ശൂന്യം *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned പൂർണ്ണസംഖ്യ width HB_UNUSED,
			      unsigned പൂർണ്ണസംഖ്യ height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      ദശാംശം slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->തള്ളുക_clip ();
  c->paint ();
  c->എടുക്കുക_clip ();

  തിരികെ സത്യം;
}

സ്ഥിതം ശൂന്യം
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      ശൂന്യം *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

സ്ഥിതം ശൂന്യം
hb_paint_bounded_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        ശൂന്യം *paint_data,
				        hb_color_വരി_t *color_വരി HB_UNUSED,
				        ദശാംശം x0 HB_UNUSED, ദശാംശം y0 HB_UNUSED,
				        ദശാംശം x1 HB_UNUSED, ദശാംശം y1 HB_UNUSED,
				        ദശാംശം x2 HB_UNUSED, ദശാംശം y2 HB_UNUSED,
				        ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

സ്ഥിതം ശൂന്യം
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        ശൂന്യം *paint_data,
				        hb_color_വരി_t *color_വരി HB_UNUSED,
				        ദശാംശം x0 HB_UNUSED, ദശാംശം y0 HB_UNUSED, ദശാംശം r0 HB_UNUSED,
				        ദശാംശം x1 HB_UNUSED, ദശാംശം y1 HB_UNUSED, ദശാംശം r1 HB_UNUSED,
				        ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

സ്ഥിതം ശൂന്യം
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       ശൂന്യം *paint_data,
				       hb_color_വരി_t *color_വരി HB_UNUSED,
				       ദശാംശം cx HB_UNUSED, ദശാംശം cy HB_UNUSED,
				       ദശാംശം start_angle HB_UNUSED,
				       ദശാംശം end_angle HB_UNUSED,
				       ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_paint_bounded_funcs ();

സ്ഥിതം ഘടന hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_തള്ളുക_clip_glyph_func (funcs, hb_paint_bounded_തള്ളുക_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_rectangle_func (funcs, hb_paint_bounded_തള്ളുക_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_clip_func (funcs, hb_paint_bounded_എടുക്കുക_clip, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_group_func (funcs, hb_paint_bounded_തള്ളുക_group, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_group_func (funcs, hb_paint_bounded_എടുക്കുക_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_paint_bounded_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_paint_bounded_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_paint_bounded_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_bounded_funcs);

    തിരികെ funcs;
  }
} static_paint_bounded_funcs;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_paint_bounded_funcs ()
{
  static_paint_bounded_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_bounded_get_funcs ()
{
  തിരികെ static_paint_bounded_funcs.get_unconst ();
}


#endif
