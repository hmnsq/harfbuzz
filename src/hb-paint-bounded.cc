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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint-bounded.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"


/*
 * This file implements boundedness computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ਸਥਿਰ ਖਾਲੀ
hb_paint_bounded_ਧੱਕੋ_clip_glyph (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				  ਖਾਲੀ *paint_ਅੰਕੜੇ,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ਅੰਕੜੇ;

  c->ਧੱਕੋ_clip ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_bounded_ਧੱਕੋ_clip_rectangle (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				      ਖਾਲੀ *paint_ਅੰਕੜੇ,
				      ਦਸ਼ਮਲਵ xmin, ਦਸ਼ਮਲਵ ymin, ਦਸ਼ਮਲਵ xmax, ਦਸ਼ਮਲਵ ymax,
				      ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ਅੰਕੜੇ;

  c->ਧੱਕੋ_clip ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_bounded_ਕੱਢੋ_clip (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			   ਖਾਲੀ *paint_ਅੰਕੜੇ,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ਅੰਕੜੇ;

  c->ਕੱਢੋ_clip ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_bounded_ਧੱਕੋ_ਸਮੂਹ (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *paint_ਅੰਕੜੇ,
			     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ਅੰਕੜੇ;

  c->ਧੱਕੋ_ਸਮੂਹ ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_bounded_ਕੱਢੋ_ਸਮੂਹ (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			    ਖਾਲੀ *paint_ਅੰਕੜੇ,
			    hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ਅੰਕੜੇ;

  c->ਕੱਢੋ_ਸਮੂਹ (ਬਹੁਲਕ);
}

ਸਥਿਰ hb_bool_t
hb_paint_bounded_paint_image (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *paint_ਅੰਕੜੇ,
			      hb_blob_t *blob HB_ਅਣਵਰਤਿਆ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੌੜਾਈ HB_ਅਣਵਰਤਿਆ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉਚਾਈ HB_ਅਣਵਰਤਿਆ,
			      hb_ਟੈਗ_t format HB_ਅਣਵਰਤਿਆ,
			      ਦਸ਼ਮਲਵ slant HB_ਅਣਵਰਤਿਆ,
			      hb_glyph_extents_t *glyph_extents,
			      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ਅੰਕੜੇ;

  c->ਧੱਕੋ_clip ();
  c->paint ();
  c->ਕੱਢੋ_clip ();

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_bounded_paint_color (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *paint_ਅੰਕੜੇ,
			      hb_bool_t use_foreground HB_ਅਣਵਰਤਿਆ,
			      hb_color_t color HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ਅੰਕੜੇ;

  c->paint ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_bounded_paint_ਰੇਖਿਕ_gradient (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				        ਖਾਲੀ *paint_ਅੰਕੜੇ,
				        hb_color_ਸਤਰ_t *color_ਸਤਰ HB_ਅਣਵਰਤਿਆ,
				        ਦਸ਼ਮਲਵ x0 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y0 HB_ਅਣਵਰਤਿਆ,
				        ਦਸ਼ਮਲਵ x1 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y1 HB_ਅਣਵਰਤਿਆ,
				        ਦਸ਼ਮਲਵ x2 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y2 HB_ਅਣਵਰਤਿਆ,
				        ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ਅੰਕੜੇ;

  c->paint ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_bounded_paint_radial_gradient (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				        ਖਾਲੀ *paint_ਅੰਕੜੇ,
				        hb_color_ਸਤਰ_t *color_ਸਤਰ HB_ਅਣਵਰਤਿਆ,
				        ਦਸ਼ਮਲਵ x0 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y0 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ r0 HB_ਅਣਵਰਤਿਆ,
				        ਦਸ਼ਮਲਵ x1 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y1 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ r1 HB_ਅਣਵਰਤਿਆ,
				        ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ਅੰਕੜੇ;

  c->paint ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_bounded_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				       ਖਾਲੀ *paint_ਅੰਕੜੇ,
				       hb_color_ਸਤਰ_t *color_ਸਤਰ HB_ਅਣਵਰਤਿਆ,
				       ਦਸ਼ਮਲਵ cx HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ cy HB_ਅਣਵਰਤਿਆ,
				       ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle HB_ਅਣਵਰਤਿਆ,
				       ਦਸ਼ਮਲਵ ਸਿਰਾ_angle HB_ਅਣਵਰਤਿਆ,
				       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_bounded_context_t *c = (hb_paint_bounded_context_t *) paint_ਅੰਕੜੇ;

  c->paint ();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_paint_bounded_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_paint_bounded_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_bounded_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_glyph_func (funcs, hb_paint_bounded_ਧੱਕੋ_clip_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_rectangle_func (funcs, hb_paint_bounded_ਧੱਕੋ_clip_rectangle, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_clip_func (funcs, hb_paint_bounded_ਕੱਢੋ_clip, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_ਸਮੂਹ_func (funcs, hb_paint_bounded_ਧੱਕੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_ਸਮੂਹ_func (funcs, hb_paint_bounded_ਕੱਢੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_color_func (funcs, hb_paint_bounded_paint_color, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_image_func (funcs, hb_paint_bounded_paint_image, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਰੇਖਿਕ_gradient_func (funcs, hb_paint_bounded_paint_ਰੇਖਿਕ_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_radial_gradient_func (funcs, hb_paint_bounded_paint_radial_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_sweep_gradient_func (funcs, hb_paint_bounded_paint_sweep_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_bounded_funcs);

    ਵਾਪਸ funcs;
  }
} static_paint_bounded_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_paint_bounded_funcs ()
{
  static_paint_bounded_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_bounded_get_funcs ()
{
  ਵਾਪਸ static_paint_bounded_funcs.get_unconst ();
}


#ਜੇ_ਅੰਤ
