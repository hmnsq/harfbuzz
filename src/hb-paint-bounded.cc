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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_PAINT

#அந்தர்பாவ "hb-paint-bounded.hh"

#அந்தர்பாவ "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

நிலையான வெறுமை
hb_paint_bounded_தள்ளு_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  வெறுமை *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  வெறுமை *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->தள்ளு_clip ();
}

நிலையான வெறுமை
hb_paint_bounded_தள்ளு_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      வெறுமை *paint_data,
				      தசம xmin, தசம ymin, தசம xmax, தசம ymax,
				      வெறுமை *user_data)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->தள்ளு_clip ();
}

நிலையான வெறுமை
hb_paint_bounded_எடு_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   வெறுமை *paint_data,
			   வெறுமை *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->எடு_clip ();
}

நிலையான வெறுமை
hb_paint_bounded_தள்ளு_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     வெறுமை *paint_data,
			     வெறுமை *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->தள்ளு_group ();
}

நிலையான வெறுமை
hb_paint_bounded_எடு_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    வெறுமை *paint_data,
			    hb_paint_composite_mode_t mode,
			    வெறுமை *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->எடு_group (mode);
}

நிலையான hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      வெறுமை *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned முழு width HB_UNUSED,
			      unsigned முழு height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      தசம slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      வெறுமை *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->தள்ளு_clip ();
  c->paint ();
  c->எடு_clip ();

  திரும்பு உண்மை;
}

நிலையான வெறுமை
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      வெறுமை *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      வெறுமை *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

நிலையான வெறுமை
hb_paint_bounded_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        வெறுமை *paint_data,
				        hb_color_வரி_t *color_வரி HB_UNUSED,
				        தசம x0 HB_UNUSED, தசம y0 HB_UNUSED,
				        தசம x1 HB_UNUSED, தசம y1 HB_UNUSED,
				        தசம x2 HB_UNUSED, தசம y2 HB_UNUSED,
				        வெறுமை *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

நிலையான வெறுமை
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        வெறுமை *paint_data,
				        hb_color_வரி_t *color_வரி HB_UNUSED,
				        தசம x0 HB_UNUSED, தசம y0 HB_UNUSED, தசம r0 HB_UNUSED,
				        தசம x1 HB_UNUSED, தசம y1 HB_UNUSED, தசம r1 HB_UNUSED,
				        வெறுமை *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

நிலையான வெறுமை
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       வெறுமை *paint_data,
				       hb_color_வரி_t *color_வரி HB_UNUSED,
				       தசம cx HB_UNUSED, தசம cy HB_UNUSED,
				       தசம start_angle HB_UNUSED,
				       தசம end_angle HB_UNUSED,
				       வெறுமை *user_data HB_UNUSED)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_data;

  c->paint ();
}

நிலையான உள்வரி வெறுமை free_static_paint_bounded_funcs ();

நிலையான கட்டமைப்பு hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  நிலையான hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_தள்ளு_clip_glyph_func (funcs, hb_paint_bounded_தள்ளு_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_rectangle_func (funcs, hb_paint_bounded_தள்ளு_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_எடு_clip_func (funcs, hb_paint_bounded_எடு_clip, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_group_func (funcs, hb_paint_bounded_தள்ளு_group, nullptr, nullptr);
    hb_paint_funcs_set_எடு_group_func (funcs, hb_paint_bounded_எடு_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_paint_bounded_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_paint_bounded_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_paint_bounded_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_bounded_funcs);

    திரும்பு funcs;
  }
} static_paint_bounded_funcs;

நிலையான உள்வரி
வெறுமை free_static_paint_bounded_funcs ()
{
  static_paint_bounded_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_bounded_get_funcs ()
{
  திரும்பு static_paint_bounded_funcs.get_unconst ();
}


#endif
