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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_PAINT

#અન્તર્ભાવ "hb-paint-bounded.hh"

#અન્તર્ભાવ "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

સ્થિર શૂન્ય
hb_paint_bounded_ધકેલો_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  શૂન્ય *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ધકેલો_clip ();
}

સ્થિર શૂન્ય
hb_paint_bounded_ધકેલો_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      શૂન્ય *paint_data,
				      દશાંશ xmin, દશાંશ ymin, દશાંશ xmax, દશાંશ ymax,
				      શૂન્ય *user_data)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ધકેલો_clip ();
}

સ્થિર શૂન્ય
hb_paint_bounded_કાઢો_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   શૂન્ય *paint_data,
			   શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->કાઢો_clip ();
}

સ્થિર શૂન્ય
hb_paint_bounded_ધકેલો_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     શૂન્ય *paint_data,
			     શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ધકેલો_group ();
}

સ્થિર શૂન્ય
hb_paint_bounded_કાઢો_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    શૂન્ય *paint_data,
			    hb_paint_composite_mode_t mode,
			    શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->કાઢો_group (mode);
}

સ્થિર hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      શૂન્ય *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned પૂર્ણાંક width HB_UNUSED,
			      unsigned પૂર્ણાંક height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      દશાંશ slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->ધકેલો_clip ();
  c->paint ();
  c->કાઢો_clip ();

  પાછા સાચું;
}

સ્થિર શૂન્ય
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      શૂન્ય *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

સ્થિર શૂન્ય
hb_paint_bounded_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        શૂન્ય *paint_data,
				        hb_color_લીટી_t *color_લીટી HB_UNUSED,
				        દશાંશ x0 HB_UNUSED, દશાંશ y0 HB_UNUSED,
				        દશાંશ x1 HB_UNUSED, દશાંશ y1 HB_UNUSED,
				        દશાંશ x2 HB_UNUSED, દશાંશ y2 HB_UNUSED,
				        શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

સ્થિર શૂન્ય
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        શૂન્ય *paint_data,
				        hb_color_લીટી_t *color_લીટી HB_UNUSED,
				        દશાંશ x0 HB_UNUSED, દશાંશ y0 HB_UNUSED, દશાંશ r0 HB_UNUSED,
				        દશાંશ x1 HB_UNUSED, દશાંશ y1 HB_UNUSED, દશાંશ r1 HB_UNUSED,
				        શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

સ્થિર શૂન્ય
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       શૂન્ય *paint_data,
				       hb_color_લીટી_t *color_લીટી HB_UNUSED,
				       દશાંશ cx HB_UNUSED, દશાંશ cy HB_UNUSED,
				       દશાંશ start_angle HB_UNUSED,
				       દશાંશ end_angle HB_UNUSED,
				       શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

સ્થિર અંતઃરેખ શૂન્ય free_static_paint_bounded_funcs ();

સ્થિર સંરચના hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  સ્થિર hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ધકેલો_clip_glyph_func (funcs, hb_paint_bounded_ધકેલો_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ધકેલો_clip_rectangle_func (funcs, hb_paint_bounded_ધકેલો_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_કાઢો_clip_func (funcs, hb_paint_bounded_કાઢો_clip, nullptr, nullptr);
    hb_paint_funcs_set_ધકેલો_group_func (funcs, hb_paint_bounded_ધકેલો_group, nullptr, nullptr);
    hb_paint_funcs_set_કાઢો_group_func (funcs, hb_paint_bounded_કાઢો_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_paint_bounded_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_paint_bounded_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_paint_bounded_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_bounded_funcs);

    પાછા funcs;
  }
} static_paint_bounded_funcs;

સ્થિર અંતઃરેખ
શૂન્ય free_static_paint_bounded_funcs ()
{
  static_paint_bounded_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_bounded_get_funcs ()
{
  પાછા static_paint_bounded_funcs.get_unconst ();
}


#endif
