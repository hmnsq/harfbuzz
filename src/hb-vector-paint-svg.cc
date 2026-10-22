/*
 * Copyright © 2026  Behdad Esfahbod
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
 *
 * Author(s): Behdad Esfahbod
 */

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-vector-paint.hh"
#അന്തര്ഭാവ "hb-paint.hh"
#അന്തര്ഭാവ "hb-vector-path.hh"

#അന്തര്ഭാവ <math.h>

സ്ഥിതം സ്ഥിരം അക്ഷരം *
hb_vector_svg_extend_mode_str (hb_paint_extend_t ext)
{
  തിരഞ്ഞെടുപ്പ് (ext)
  {
    അവസ്ഥ HB_PAINT_EXTEND_PAD: തിരികെ "pad";
    അവസ്ഥ HB_PAINT_EXTEND_REPEAT: തിരികെ "repeat";
    അവസ്ഥ HB_PAINT_EXTEND_REFLECT: തിരികെ "reflect";
    സ്ഥിരസ്ഥിതി: തിരികെ "pad";
  }
}

സ്ഥിതം ശൂന്യം
hb_vector_svg_emit_color_stops (hb_vector_paint_t *paint,
				hb_vector_buf_t *buf,
				hb_vector_t<hb_color_stop_t> *stops)
{
  വേണ്ടി (unsigned i = 0; i < stops->length; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_അക്കം (stops->arrayZ[i].offset, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    എങ്കിൽ (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_അക്കം (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
hb_vector_svg_composite_mode_str (hb_paint_composite_mode_t mode)
{
  തിരഞ്ഞെടുപ്പ് (mode)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_PLUS: തിരികെ "screen";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_XOR: തിരികെ "difference";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_CLEAR:
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC:
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST:
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_IN:
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_IN:
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      തിരികെ nullptr;
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_OVER: തിരികെ "normal";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SCREEN: തിരികെ "screen";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_OVERLAY: തിരികെ "overlay";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DARKEN: തിരികെ "darken";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_LIGHTEN: തിരികെ "lighten";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: തിരികെ "color-dodge";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_COLOR_BURN: തിരികെ "color-burn";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: തിരികെ "hard-light";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: തിരികെ "soft-light";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DIFFERENCE: തിരികെ "difference";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_EXCLUSION: തിരികെ "exclusion";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_MULTIPLY: തിരികെ "multiply";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_HUE: തിരികെ "hue";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: തിരികെ "saturation";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_COLOR: തിരികെ "color";
    അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: തിരികെ "luminosity";
    സ്ഥിരസ്ഥിതി: തിരികെ nullptr;
  }
}

ഘടന hb_vector_svg_point_t { ദശാംശം x, y; };
ഘടന hb_vector_svg_rgba_t { ദശാംശം r, g, b, a; };

സ്ഥിതം വരിയിൽ ദശാംശം
hb_vector_svg_lerp (ദശാംശം a, ദശാംശം b, ദശാംശം t)
{ തിരികെ a + (b - a) * t; }

സ്ഥിതം വരിയിൽ ദശാംശം
hb_vector_svg_clamp01 (ദശാംശം v)
{
  എങ്കിൽ (v < 0.f) തിരികെ 0.f;
  എങ്കിൽ (v > 1.f) തിരികെ 1.f;
  തിരികെ v;
}

സ്ഥിതം വരിയിൽ hb_vector_svg_rgba_t
hb_vector_svg_rgba_from_hb_color (hb_color_t c)
{
  തിരികെ {(ദശാംശം) hb_color_get_red (c) / 255.f,
          (ദശാംശം) hb_color_get_green (c) / 255.f,
          (ദശാംശം) hb_color_get_blue (c) / 255.f,
          (ദശാംശം) hb_color_get_alpha (c) / 255.f};
}

സ്ഥിതം വരിയിൽ hb_color_t
hb_vector_svg_hb_color_from_rgba (സ്ഥിരം hb_vector_svg_rgba_t &c)
{
  unsigned r = (unsigned) roundf (hb_vector_svg_clamp01 (c.r) * 255.f);
  unsigned g = (unsigned) roundf (hb_vector_svg_clamp01 (c.g) * 255.f);
  unsigned b = (unsigned) roundf (hb_vector_svg_clamp01 (c.b) * 255.f);
  unsigned a = (unsigned) roundf (hb_vector_svg_clamp01 (c.a) * 255.f);
  തിരികെ HB_COLOR (b, g, r, a);
}

സ്ഥിതം വരിയിൽ hb_vector_svg_rgba_t
hb_vector_svg_lerp_rgba (സ്ഥിരം hb_vector_svg_rgba_t &c0,
			 സ്ഥിരം hb_vector_svg_rgba_t &c1,
			 ദശാംശം t)
{
  തിരികെ {hb_vector_svg_lerp (c0.r, c1.r, t),
          hb_vector_svg_lerp (c0.g, c1.g, t),
          hb_vector_svg_lerp (c0.b, c1.b, t),
          hb_vector_svg_lerp (c0.a, c1.a, t)};
}

സ്ഥിതം വരിയിൽ ദശാംശം hb_vector_svg_dot (സ്ഥിരം hb_vector_svg_point_t &p, സ്ഥിരം hb_vector_svg_point_t &q) { തിരികെ p.x * q.x + p.y * q.y; }
സ്ഥിതം വരിയിൽ hb_vector_svg_point_t hb_vector_svg_add (സ്ഥിരം hb_vector_svg_point_t &p, സ്ഥിരം hb_vector_svg_point_t &q) { തിരികെ {p.x + q.x, p.y + q.y}; }
സ്ഥിതം വരിയിൽ hb_vector_svg_point_t hb_vector_svg_sub (സ്ഥിരം hb_vector_svg_point_t &p, സ്ഥിരം hb_vector_svg_point_t &q) { തിരികെ {p.x - q.x, p.y - q.y}; }
സ്ഥിതം വരിയിൽ hb_vector_svg_point_t hb_vector_svg_scale (സ്ഥിരം hb_vector_svg_point_t &p, ദശാംശം f) { തിരികെ {p.x * f, p.y * f}; }

സ്ഥിതം വരിയിൽ hb_vector_svg_point_t
hb_vector_svg_normalize (സ്ഥിരം hb_vector_svg_point_t &p)
{
  ദശാംശം len = sqrtf (hb_vector_svg_dot (p, p));
  എങ്കിൽ (len == 0.f) തിരികെ {0.f, 0.f};
  തിരികെ hb_vector_svg_scale (p, 1.f / len);
}

സ്ഥിതം ശൂന്യം
hb_vector_svg_add_sweep_patch (hb_vector_buf_t *body,
			       unsigned precision,
			       ദശാംശം cx, ദശാംശം cy, ദശാംശം radius,
			       ദശാംശം a0, സ്ഥിരം hb_vector_svg_rgba_t &c0_in,
			       ദശാംശം a1, സ്ഥിരം hb_vector_svg_rgba_t &c1_in)
{
  സ്ഥിതം സ്ഥിരം ദശാംശം max_angle = HB_PI / 16.f;
  hb_vector_svg_point_t center = {cx, cy};
  പൂർണ്ണസംഖ്യ അക്കം_splits = (പൂർണ്ണസംഖ്യ) ceilf (fabsf (a1 - a0) / max_angle);
  എങ്കിൽ (അക്കം_splits < 1) അക്കം_splits = 1;

  hb_vector_svg_point_t p0 = {cosf (a0), sinf (a0)};
  hb_vector_svg_rgba_t color0 = c0_in;

  വേണ്ടി (പൂർണ്ണസംഖ്യ a = 0; a < അക്കം_splits; a++)
  {
    ദശാംശം k = (a + 1.f) / അക്കം_splits;
    ദശാംശം angle1 = hb_vector_svg_lerp (a0, a1, k);
    hb_vector_svg_rgba_t color1 = hb_vector_svg_lerp_rgba (c0_in, c1_in, k);

    hb_vector_svg_point_t p1 = {cosf (angle1), sinf (angle1)};
    hb_vector_svg_point_t sp0 = hb_vector_svg_add (center, hb_vector_svg_scale (p0, radius));
    hb_vector_svg_point_t sp1 = hb_vector_svg_add (center, hb_vector_svg_scale (p1, radius));

    hb_vector_svg_point_t A = hb_vector_svg_normalize (hb_vector_svg_add (p0, p1));
    hb_vector_svg_point_t U = {-A.y, A.x};
    ദശാംശം up0 = hb_vector_svg_dot (U, p0);
    ദശാംശം up1 = hb_vector_svg_dot (U, p1);
    എങ്കിൽ (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      തുടരുക;
    }
    hb_vector_svg_point_t C0 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p0, A), p0) / up0));
    hb_vector_svg_point_t C1 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p1, A), p1) / up1));

    hb_vector_svg_point_t sc0 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C0, hb_vector_svg_scale (hb_vector_svg_sub (C0, p0), 0.33333f)), radius));
    hb_vector_svg_point_t sc1 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C1, hb_vector_svg_scale (hb_vector_svg_sub (C1, p1), 0.33333f)), radius));

    hb_vector_svg_rgba_t mid_color = hb_vector_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_vector_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_അക്കം (center.x, precision);
    body->append_c (',');
    body->append_അക്കം (center.y, precision);
    body->append_str ("L");
    body->append_അക്കം (sp0.x, precision);
    body->append_c (',');
    body->append_അക്കം (sp0.y, precision);
    body->append_str ("C");
    body->append_അക്കം (sc0.x, precision);
    body->append_c (',');
    body->append_അക്കം (sc0.y, precision);
    body->append_c (' ');
    body->append_അക്കം (sc1.x, precision);
    body->append_c (',');
    body->append_അക്കം (sc1.y, precision);
    body->append_c (' ');
    body->append_അക്കം (sp1.x, precision);
    body->append_c (',');
    body->append_അക്കം (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, സത്യം);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
ഘടന hb_vector_svg_sweep_ctx_t
{
  hb_vector_buf_t *body;
  unsigned precision;
  ദശാംശം cx, cy, radius;
};

സ്ഥിതം ശൂന്യം
hb_vector_svg_sweep_emit_patch (ദശാംശം a0, hb_color_t c0,
				ദശാംശം a1, hb_color_t c1,
				ശൂന്യം *user_data)
{
  auto *ctx = (hb_vector_svg_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  എങ്കിൽ (unlikely (ctx->body->in_error ()))
    തിരികെ;
  hb_vector_svg_add_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_vector_svg_rgba_from_hb_color (c0),
				 a1, hb_vector_svg_rgba_from_hb_color (c1));
}


സ്ഥിതം ശൂന്യം hb_vector_paint_തള്ളുക_transform (hb_paint_funcs_t *, ശൂന്യം *,
                                            ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം,
                                            ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_എടുക്കുക_transform (hb_paint_funcs_t *, ശൂന്യം *, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_fill_glyph (hb_paint_funcs_t *, ശൂന്യം *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_തള്ളുക_clip_glyph (hb_paint_funcs_t *, ശൂന്യം *, hb_codepoint_t, hb_font_t *, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_തള്ളുക_clip_rectangle (hb_paint_funcs_t *, ശൂന്യം *, ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം, ശൂന്യം *);
സ്ഥിതം hb_draw_funcs_t * hb_vector_paint_തള്ളുക_clip_path_start (hb_paint_funcs_t *, ശൂന്യം *, ശൂന്യം **, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_തള്ളുക_clip_path_end (hb_paint_funcs_t *, ശൂന്യം *, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_എടുക്കുക_clip (hb_paint_funcs_t *, ശൂന്യം *, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_color (hb_paint_funcs_t *, ശൂന്യം *, hb_bool_t, hb_color_t, ശൂന്യം *);
സ്ഥിതം hb_bool_t hb_vector_paint_image (hb_paint_funcs_t *, ശൂന്യം *, hb_blob_t *, unsigned, unsigned, hb_tag_t, ദശാംശം, hb_glyph_extents_t *, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_linear_gradient (hb_paint_funcs_t *, ശൂന്യം *, hb_color_വരി_t *, ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_radial_gradient (hb_paint_funcs_t *, ശൂന്യം *, hb_color_വരി_t *, ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_sweep_gradient (hb_paint_funcs_t *, ശൂന്യം *, hb_color_വരി_t *, ദശാംശം, ദശാംശം, ദശാംശം, ദശാംശം, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_തള്ളുക_group (hb_paint_funcs_t *, ശൂന്യം *, ശൂന്യം *);
സ്ഥിതം ശൂന്യം hb_vector_paint_എടുക്കുക_group (hb_paint_funcs_t *, ശൂന്യം *, hb_paint_composite_mode_t, ശൂന്യം *);
സ്ഥിതം hb_bool_t hb_vector_paint_color_glyph (hb_paint_funcs_t *, ശൂന്യം *, hb_codepoint_t, hb_font_t *, ശൂന്യം *);
സ്ഥിതം hb_bool_t
hb_vector_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      ശൂന്യം *paint_data,
				      unsigned color_index,
				      hb_color_t *color,
				      ശൂന്യം *user_data HB_UNUSED)
{
  hb_vector_paint_t *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (!color)
    തിരികെ അസത്യം;

  hb_color_t *value = nullptr;
  എങ്കിൽ (!paint->custom_palette_colors.has (color_index, &value) || !value)
    തിരികെ അസത്യം;
  *color = *value;
  തിരികെ സത്യം;
}

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_vector_paint_funcs ();
സ്ഥിതം ഘടന hb_vector_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_vector_paint_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_തള്ളുക_transform_func (funcs, (hb_paint_തള്ളുക_transform_func_t) hb_vector_paint_തള്ളുക_transform, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_transform_func (funcs, (hb_paint_എടുക്കുക_transform_func_t) hb_vector_paint_എടുക്കുക_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_vector_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_glyph_func (funcs, (hb_paint_തള്ളുക_clip_glyph_func_t) hb_vector_paint_തള്ളുക_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_rectangle_func (funcs, (hb_paint_തള്ളുക_clip_rectangle_func_t) hb_vector_paint_തള്ളുക_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_path_start_func (funcs, (hb_paint_തള്ളുക_clip_path_start_func_t) hb_vector_paint_തള്ളുക_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_path_end_func (funcs, (hb_paint_തള്ളുക_clip_path_end_func_t) hb_vector_paint_തള്ളുക_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_clip_func (funcs, (hb_paint_എടുക്കുക_clip_func_t) hb_vector_paint_എടുക്കുക_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_vector_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_vector_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_vector_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_vector_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_vector_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_group_func (funcs, (hb_paint_തള്ളുക_group_func_t) hb_vector_paint_തള്ളുക_group, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_group_func (funcs, (hb_paint_എടുക്കുക_group_func_t) hb_vector_paint_എടുക്കുക_group, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_vector_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_vector_paint_custom_palette_color, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_paint_funcs);
    തിരികെ funcs;
  }
} static_vector_paint_funcs;

സ്ഥിതം വരിയിൽ ശൂന്യം
free_static_vector_paint_funcs ()
{
  static_vector_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_svg_funcs_get ()
{
  തിരികെ static_vector_paint_funcs.get_unconst ();
}


സ്ഥിതം ശൂന്യം
hb_vector_paint_തള്ളുക_transform (hb_paint_funcs_t *,
                                ശൂന്യം *paint_data,
                                ദശാംശം xx, ദശാംശം yx,
                                ദശാംശം xy, ദശാംശം yy,
                                ദശാംശം dx, ദശാംശം dy,
                                ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  എങ്കിൽ (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth++;
    തിരികെ;
  }
  എങ്കിൽ (unlikely (paint->transform_group_depth >= 64))
  {
    paint->transform_group_overflow_depth = 1;
    തിരികെ;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_group_open_mask = (paint->transform_group_open_mask << 1) | (opened ? 1ull : 0ull);
  paint->transform_group_depth++;

  എങ്കിൽ (!opened)
    തിരികെ;

  auto &body = paint->current_body ();
  unsigned sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_അക്കം (xx, sprec);
  body.append_c (',');
  body.append_അക്കം (yx, sprec);
  body.append_c (',');
  body.append_അക്കം (xy, sprec);
  body.append_c (',');
  body.append_അക്കം (yy, sprec);
  body.append_c (',');
  body.append_അക്കം (paint->sx (dx));
  body.append_c (',');
  body.append_അക്കം (paint->sy (dy));
  body.append_str (")\">\n");
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_എടുക്കുക_transform (hb_paint_funcs_t *,
                               ശൂന്യം *paint_data,
                               ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  എങ്കിൽ (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth--;
    തിരികെ;
  }
  എങ്കിൽ (!paint->transform_group_depth)
    തിരികെ;
  paint->transform_group_depth--;
  hb_bool_t opened = !!(paint->transform_group_open_mask & 1ull);
  paint->transform_group_open_mask >>= 1;
  എങ്കിൽ (opened)
    paint->current_body ().append_str ("</g>\n");
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_fill_glyph (hb_paint_funcs_t *,
                            ശൂന്യം *paint_data,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  എങ്കിൽ (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  auto &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->path.arrayZ, paint->path.length);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, സത്യം);
  body.append_str ("\"/>\n");
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_തള്ളുക_clip_glyph (hb_paint_funcs_t *,
                                 ശൂന്യം *paint_data,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  സ്ഥിരം അക്ഷരം *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  എങ്കിൽ (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  unsigned def_id = paint->path_def_എണ്ണം++;
  paint->defs.append_str ("<path id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\" d=\"");
  paint->defs.append_len (paint->path.arrayZ, paint->path.length);
  paint->defs.append_str ("\"/>\n");
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("clip-p");
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\"><use href=\"#");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("clip-p");
  paint->current_body ().append_unsigned (def_id);
  paint->current_body ().append_str (")\">\n");
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_തള്ളുക_clip_rectangle (hb_paint_funcs_t *,
                                     ശൂന്യം *paint_data,
                                     ദശാംശം xmin, ദശാംശം ymin,
                                     ദശാംശം xmax, ദശാംശം ymax,
                                     ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  സ്ഥിരം അക്ഷരം *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_അക്കം (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_അക്കം (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_അക്കം (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_അക്കം (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

സ്ഥിതം hb_draw_funcs_t *
hb_vector_paint_തള്ളുക_clip_path_start (hb_paint_funcs_t *,
                                      ശൂന്യം *paint_data,
                                      ശൂന്യം **draw_data,
                                      ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    തിരികെ nullptr;
  }

  paint->path.clear ();
  paint->clip_path_sink = {&paint->path, paint->get_precision (),
			   paint->x_scale_factor,
			   paint->y_scale_factor,
			   &paint->budget_remaining};
  *draw_data = &paint->clip_path_sink;
  തിരികെ hb_vector_svg_path_draw_funcs_get ();
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_തള്ളുക_clip_path_end (hb_paint_funcs_t *,
                                    ശൂന്യം *paint_data,
                                    ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  സ്ഥിരം അക്ഷരം *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned clip_id = paint->clip_path_counter++;

  /* The accumulated path is in font Y-up coords (the
   * convention used inside per-glyph <use scale(_,-sy)>
   * wrappers); this clip is emitted at base body level
   * (free-form between glyphs), so flip its geometry inside
   * the clipPath via the path's own transform attribute,
   * keeping the body's <g clip-path> at body Y-down. */
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("cp");
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><path transform=\"scale(1,-1)\" d=\"");
  paint->defs.append_len (paint->path.arrayZ, paint->path.length);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("cp");
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_എടുക്കുക_clip (hb_paint_funcs_t *,
                          ശൂന്യം *paint_data,
                          ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  paint->current_body ().append_str ("</g>\n");
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_color (hb_paint_funcs_t *,
                       ശൂന്യം *paint_data,
                       hb_bool_t,
                       hb_color_t color,
                       ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  auto &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, സത്യം);
  body.append_str ("\"/>\n");
}

സ്ഥിതം hb_bool_t
hb_vector_paint_image (hb_paint_funcs_t *,
                       ശൂന്യം *paint_data,
                       hb_blob_t *image,
                       unsigned width,
                       unsigned height,
                       hb_tag_t format,
                       ദശാംശം slant HB_UNUSED,
                       hb_glyph_extents_t *extents,
                       ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ അസത്യം;

  auto &body = paint->current_body ();
  എങ്കിൽ (format == HB_TAG ('p','n','g',' '))
  {
    എങ്കിൽ (!extents || !width || !height)
      തിരികെ അസത്യം;

    unsigned len = 0;
    സ്ഥിരം അക്ഷരം *png_data = hb_blob_get_data (image, &len);
    എങ്കിൽ (!png_data || !len)
      തിരികെ അസത്യം;

    body.append_str ("<g transform=\"translate(");
    body.append_അക്കം (paint->sx ((ദശാംശം) extents->x_bearing));
    body.append_c (',');
    body.append_അക്കം (paint->sy ((ദശാംശം) extents->y_bearing));
    body.append_str (") scale(");
    body.append_അക്കം (paint->sx ((ദശാംശം) extents->width) / width);
    body.append_c (',');
    body.append_അക്കം (paint->sy ((ദശാംശം) extents->height) / height);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((സ്ഥിരം uint8_t *) png_data, len);
    body.append_str ("\" width=\"");
    body.append_അക്കം ((ദശാംശം) width);
    body.append_str ("\" height=\"");
    body.append_അക്കം ((ദശാംശം) height);
    body.append_str ("\"/>\n</g>\n");

    തിരികെ സത്യം;
  }

  തിരികെ അസത്യം;
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_linear_gradient (hb_paint_funcs_t *,
                                 ശൂന്യം *paint_data,
                                 hb_color_വരി_t *color_വരി,
                                 ദശാംശം x0, ദശാംശം y0,
                                 ദശാംശം x1, ദശാംശം y1,
                                 ദശാംശം x2, ദശാംശം y2,
                                 ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  എങ്കിൽ (!paint->fetch_color_stops (color_വരി))
    തിരികെ;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  ദശാംശം mn, mx;
  hb_paint_normalize_color_വരി (stops.arrayZ, stops.length, &mn, &mx);

  സ്ഥിരം അക്ഷരം *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  ദശാംശം lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  ദശാംശം gx0 = lx0 + mn * (lx1 - lx0);
  ദശാംശം gy0 = ly0 + mn * (ly1 - ly0);
  ദശാംശം gx1 = lx0 + mx * (lx1 - lx0);
  ദശാംശം gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_അക്കം (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_അക്കം (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_അക്കം (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_അക്കം (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_വരി_get_extend (color_വരി)));
  paint->defs.append_str ("\">\n");
  hb_vector_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</linearGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_radial_gradient (hb_paint_funcs_t *,
                                 ശൂന്യം *paint_data,
                                 hb_color_വരി_t *color_വരി,
                                 ദശാംശം x0, ദശാംശം y0, ദശാംശം r0,
                                 ദശാംശം x1, ദശാംശം y1, ദശാംശം r1,
                                 ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  എങ്കിൽ (!paint->fetch_color_stops (color_വരി))
    തിരികെ;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ദശാംശം mn, mx;
  hb_paint_normalize_color_വരി (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  ദശാംശം gx0 = x0 + mn * (x1 - x0);
  ദശാംശം gy0 = y0 + mn * (y1 - y0);
  ദശാംശം gr0 = r0 + mn * (r1 - r0);
  ദശാംശം gx1 = x0 + mx * (x1 - x0);
  ദശാംശം gy1 = y0 + mx * (y1 - y0);
  ദശാംശം gr1 = r0 + mx * (r1 - r0);

  സ്ഥിരം അക്ഷരം *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_അക്കം (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_അക്കം (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_അക്കം (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_അക്കം (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_അക്കം (paint->sy (gy0));
  എങ്കിൽ (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_അക്കം (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_വരി_get_extend (color_വരി)));
  paint->defs.append_str ("\">\n");
  hb_vector_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</radialGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_sweep_gradient (hb_paint_funcs_t *,
                                ശൂന്യം *paint_data,
                                hb_color_വരി_t *color_വരി,
                                ദശാംശം cx, ദശാംശം cy,
                                ദശാംശം start_angle, ദശാംശം end_angle,
                                ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  എങ്കിൽ (!paint->fetch_color_stops (color_വരി))
    തിരികെ;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ദശാംശം mn, mx;
  hb_paint_normalize_color_വരി (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  ദശാംശം ga0 = start_angle + mn * (end_angle - start_angle);
  ദശാംശം ga1 = start_angle + mx * (end_angle - start_angle);

  hb_vector_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length,
				 hb_color_വരി_get_extend (color_വരി),
				 ga0, ga1,
				 hb_vector_svg_sweep_emit_patch,
				 &ctx);
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_തള്ളുക_group (hb_paint_funcs_t *,
                            ശൂന്യം *paint_data,
                            ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  എങ്കിൽ (unlikely (!paint->group_അട്ടി.തള്ളുക_or_fail (hb_vector_buf_t {})))
    തിരികെ;
}

സ്ഥിതം ശൂന്യം
hb_vector_paint_എടുക്കുക_group (hb_paint_funcs_t *,
                           ശൂന്യം *paint_data,
                           hb_paint_composite_mode_t mode,
                           ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  എങ്കിൽ (paint->group_അട്ടി.length < 2)
    തിരികെ;

  hb_vector_buf_t group = paint->group_അട്ടി.എടുക്കുക ();
  auto &body = paint->current_body ();

  സ്ഥിരം അക്ഷരം *blend = hb_vector_svg_composite_mode_str (mode);
  എങ്കിൽ (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (group.arrayZ, group.length);
    body.append_str ("</g>\n");
  }
  അല്ലെങ്കിൽ
    body.append_len (group.arrayZ, group.length);
}

സ്ഥിതം hb_bool_t
hb_vector_paint_color_glyph (hb_paint_funcs_t *,
                             ശൂന്യം *paint_data,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ അസത്യം;
  എങ്കിൽ (unlikely (paint->color_glyph_depth >= HB_MAX_NESTING_LEVEL))
    തിരികെ അസത്യം;
  എങ്കിൽ (unlikely (hb_set_has (paint->active_color_glyphs, glyph)))
    തിരികെ അസത്യം;

  paint->color_glyph_depth++;
  hb_set_add (paint->active_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_vector_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_set_del (paint->active_color_glyphs, glyph);
  paint->color_glyph_depth--;
  തിരികെ സത്യം;
}




hb_blob_t *
hb_vector_paint_render_svg (hb_vector_paint_t *paint)
{
  എങ്കിൽ (!paint->has_extents)
    തിരികെ nullptr;

  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ nullptr;

  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  unsigned estimated = paint->defs.length +
		       paint->group_അട്ടി.arrayZ[0].length +
		       320;
  out.alloc (estimated);
  ദശാംശം vb_x = paint->extents.x;
  ദശാംശം vb_y = -(paint->extents.y + paint->extents.height);
  ദശാംശം vb_w = paint->extents.width;
  ദശാംശം vb_h = paint->extents.height;
  out.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  out.append_അക്കം (vb_x);
  out.append_c (' ');
  out.append_അക്കം (vb_y);
  out.append_c (' ');
  out.append_അക്കം (vb_w);
  out.append_c (' ');
  out.append_അക്കം (vb_h);
  out.append_str ("\" width=\"");
  out.append_അക്കം (vb_w);
  out.append_str ("\" height=\"");
  out.append_അക്കം (vb_h);
  out.append_str ("\">\n");

  എങ്കിൽ (paint->defs.length)
  {
    out.append_str ("<defs>\n");
    out.append_len (paint->defs.arrayZ, paint->defs.length);
    out.append_str ("</defs>\n");
  }

  എങ്കിൽ (hb_color_get_alpha (paint->background))
  {
    out.append_str ("<rect x=\"");
    out.append_അക്കം (vb_x);
    out.append_str ("\" y=\"");
    out.append_അക്കം (vb_y);
    out.append_str ("\" width=\"");
    out.append_അക്കം (vb_w);
    out.append_str ("\" height=\"");
    out.append_അക്കം (vb_h);
    out.append_str ("\" fill=\"rgb(");
    out.append_unsigned (hb_color_get_red (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_green (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_blue (paint->background));
    out.append_str (")\"");
    എങ്കിൽ (hb_color_get_alpha (paint->background) < 255)
    {
      out.append_str (" fill-opacity=\"");
      out.append_അക്കം (hb_color_get_alpha (paint->background) / 255.f, 4);
      out.append_c ('"');
    }
    out.append_str ("/>\n");
  }

  out.append_str ("<g transform=\"scale(1,-1)\">\n");
  out.append_len (paint->group_അട്ടി.arrayZ[0].arrayZ, paint->group_അട്ടി.arrayZ[0].length);
  out.append_str ("</g>\n");

  out.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  തിരികെ blob;
}
