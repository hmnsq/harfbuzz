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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-vector-paint.hh"
#ଅନ୍ତର୍ଭାଵ "hb-paint.hh"
#ଅନ୍ତର୍ଭାଵ "hb-vector-path.hh"

#ଅନ୍ତର୍ଭାଵ <math.h>

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
hb_vector_svg_extend_mode_str (hb_paint_extend_t ext)
{
  ବାଛ (ext)
  {
    ସ୍ଥିତି HB_PAINT_EXTEND_PAD: ଫେରନ୍ତୁ "pad";
    ସ୍ଥିତି HB_PAINT_EXTEND_REPEAT: ଫେରନ୍ତୁ "repeat";
    ସ୍ଥିତି HB_PAINT_EXTEND_REFLECT: ଫେରନ୍ତୁ "reflect";
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ "pad";
  }
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_svg_emit_color_stops (hb_vector_paint_t *paint,
				hb_vector_buf_t *buf,
				hb_vector_t<hb_color_stop_t> *stops)
{
  ପାଇଁ (unsigned i = 0; i < stops->length; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_ଅଙ୍କ (stops->arrayZ[i].offset, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    ଯଦି (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_ଅଙ୍କ (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
hb_vector_svg_composite_mode_str (hb_paint_composite_mode_t mode)
{
  ବାଛ (mode)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_PLUS: ଫେରନ୍ତୁ "screen";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_XOR: ଫେରନ୍ତୁ "difference";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_CLEAR:
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC:
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST:
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_IN:
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_IN:
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      ଫେରନ୍ତୁ nullptr;
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_OVER: ଫେରନ୍ତୁ "normal";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SCREEN: ଫେରନ୍ତୁ "screen";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_OVERLAY: ଫେରନ୍ତୁ "overlay";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DARKEN: ଫେରନ୍ତୁ "darken";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_LIGHTEN: ଫେରନ୍ତୁ "lighten";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: ଫେରନ୍ତୁ "color-dodge";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_COLOR_BURN: ଫେରନ୍ତୁ "color-burn";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: ଫେରନ୍ତୁ "hard-light";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: ଫେରନ୍ତୁ "soft-light";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DIFFERENCE: ଫେରନ୍ତୁ "difference";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_EXCLUSION: ଫେରନ୍ତୁ "exclusion";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_MULTIPLY: ଫେରନ୍ତୁ "multiply";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_HUE: ଫେରନ୍ତୁ "hue";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: ଫେରନ୍ତୁ "saturation";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_COLOR: ଫେରନ୍ତୁ "color";
    ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: ଫେରନ୍ତୁ "luminosity";
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ nullptr;
  }
}

ସଂରଚନା hb_vector_svg_point_t { ଦଶମିକ x, y; };
ସଂରଚନା hb_vector_svg_rgba_t { ଦଶମିକ r, g, b, a; };

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ
hb_vector_svg_lerp (ଦଶମିକ a, ଦଶମିକ b, ଦଶମିକ t)
{ ଫେରନ୍ତୁ a + (b - a) * t; }

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ
hb_vector_svg_clamp01 (ଦଶମିକ v)
{
  ଯଦି (v < 0.f) ଫେରନ୍ତୁ 0.f;
  ଯଦି (v > 1.f) ଫେରନ୍ତୁ 1.f;
  ଫେରନ୍ତୁ v;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_vector_svg_rgba_t
hb_vector_svg_rgba_from_hb_color (hb_color_t c)
{
  ଫେରନ୍ତୁ {(ଦଶମିକ) hb_color_get_red (c) / 255.f,
          (ଦଶମିକ) hb_color_get_green (c) / 255.f,
          (ଦଶମିକ) hb_color_get_blue (c) / 255.f,
          (ଦଶମିକ) hb_color_get_alpha (c) / 255.f};
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_color_t
hb_vector_svg_hb_color_from_rgba (ଅଚଳ hb_vector_svg_rgba_t &c)
{
  unsigned r = (unsigned) roundf (hb_vector_svg_clamp01 (c.r) * 255.f);
  unsigned g = (unsigned) roundf (hb_vector_svg_clamp01 (c.g) * 255.f);
  unsigned b = (unsigned) roundf (hb_vector_svg_clamp01 (c.b) * 255.f);
  unsigned a = (unsigned) roundf (hb_vector_svg_clamp01 (c.a) * 255.f);
  ଫେରନ୍ତୁ HB_COLOR (b, g, r, a);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_vector_svg_rgba_t
hb_vector_svg_lerp_rgba (ଅଚଳ hb_vector_svg_rgba_t &c0,
			 ଅଚଳ hb_vector_svg_rgba_t &c1,
			 ଦଶମିକ t)
{
  ଫେରନ୍ତୁ {hb_vector_svg_lerp (c0.r, c1.r, t),
          hb_vector_svg_lerp (c0.g, c1.g, t),
          hb_vector_svg_lerp (c0.b, c1.b, t),
          hb_vector_svg_lerp (c0.a, c1.a, t)};
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ hb_vector_svg_dot (ଅଚଳ hb_vector_svg_point_t &p, ଅଚଳ hb_vector_svg_point_t &q) { ଫେରନ୍ତୁ p.x * q.x + p.y * q.y; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_vector_svg_point_t hb_vector_svg_add (ଅଚଳ hb_vector_svg_point_t &p, ଅଚଳ hb_vector_svg_point_t &q) { ଫେରନ୍ତୁ {p.x + q.x, p.y + q.y}; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_vector_svg_point_t hb_vector_svg_sub (ଅଚଳ hb_vector_svg_point_t &p, ଅଚଳ hb_vector_svg_point_t &q) { ଫେରନ୍ତୁ {p.x - q.x, p.y - q.y}; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_vector_svg_point_t hb_vector_svg_scale (ଅଚଳ hb_vector_svg_point_t &p, ଦଶମିକ f) { ଫେରନ୍ତୁ {p.x * f, p.y * f}; }

ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_vector_svg_point_t
hb_vector_svg_normalize (ଅଚଳ hb_vector_svg_point_t &p)
{
  ଦଶମିକ len = sqrtf (hb_vector_svg_dot (p, p));
  ଯଦି (len == 0.f) ଫେରନ୍ତୁ {0.f, 0.f};
  ଫେରନ୍ତୁ hb_vector_svg_scale (p, 1.f / len);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_svg_add_sweep_patch (hb_vector_buf_t *body,
			       unsigned precision,
			       ଦଶମିକ cx, ଦଶମିକ cy, ଦଶମିକ radius,
			       ଦଶମିକ a0, ଅଚଳ hb_vector_svg_rgba_t &c0_in,
			       ଦଶମିକ a1, ଅଚଳ hb_vector_svg_rgba_t &c1_in)
{
  ସ୍ଥିର ଅଚଳ ଦଶମିକ max_angle = HB_PI / 16.f;
  hb_vector_svg_point_t center = {cx, cy};
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_splits = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ceilf (fabsf (a1 - a0) / max_angle);
  ଯଦି (ଅଙ୍କ_splits < 1) ଅଙ୍କ_splits = 1;

  hb_vector_svg_point_t p0 = {cosf (a0), sinf (a0)};
  hb_vector_svg_rgba_t color0 = c0_in;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = 0; a < ଅଙ୍କ_splits; a++)
  {
    ଦଶମିକ k = (a + 1.f) / ଅଙ୍କ_splits;
    ଦଶମିକ angle1 = hb_vector_svg_lerp (a0, a1, k);
    hb_vector_svg_rgba_t color1 = hb_vector_svg_lerp_rgba (c0_in, c1_in, k);

    hb_vector_svg_point_t p1 = {cosf (angle1), sinf (angle1)};
    hb_vector_svg_point_t sp0 = hb_vector_svg_add (center, hb_vector_svg_scale (p0, radius));
    hb_vector_svg_point_t sp1 = hb_vector_svg_add (center, hb_vector_svg_scale (p1, radius));

    hb_vector_svg_point_t A = hb_vector_svg_normalize (hb_vector_svg_add (p0, p1));
    hb_vector_svg_point_t U = {-A.y, A.x};
    ଦଶମିକ up0 = hb_vector_svg_dot (U, p0);
    ଦଶମିକ up1 = hb_vector_svg_dot (U, p1);
    ଯଦି (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      ଜାରି;
    }
    hb_vector_svg_point_t C0 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p0, A), p0) / up0));
    hb_vector_svg_point_t C1 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p1, A), p1) / up1));

    hb_vector_svg_point_t sc0 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C0, hb_vector_svg_scale (hb_vector_svg_sub (C0, p0), 0.33333f)), radius));
    hb_vector_svg_point_t sc1 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C1, hb_vector_svg_scale (hb_vector_svg_sub (C1, p1), 0.33333f)), radius));

    hb_vector_svg_rgba_t mid_color = hb_vector_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_vector_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_ଅଙ୍କ (center.x, precision);
    body->append_c (',');
    body->append_ଅଙ୍କ (center.y, precision);
    body->append_str ("L");
    body->append_ଅଙ୍କ (sp0.x, precision);
    body->append_c (',');
    body->append_ଅଙ୍କ (sp0.y, precision);
    body->append_str ("C");
    body->append_ଅଙ୍କ (sc0.x, precision);
    body->append_c (',');
    body->append_ଅଙ୍କ (sc0.y, precision);
    body->append_c (' ');
    body->append_ଅଙ୍କ (sc1.x, precision);
    body->append_c (',');
    body->append_ଅଙ୍କ (sc1.y, precision);
    body->append_c (' ');
    body->append_ଅଙ୍କ (sp1.x, precision);
    body->append_c (',');
    body->append_ଅଙ୍କ (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, ସତ୍ୟ);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
ସଂରଚନା hb_vector_svg_sweep_ctx_t
{
  hb_vector_buf_t *body;
  unsigned precision;
  ଦଶମିକ cx, cy, radius;
};

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_svg_sweep_emit_patch (ଦଶମିକ a0, hb_color_t c0,
				ଦଶମିକ a1, hb_color_t c1,
				ଶୂନ୍ୟ *user_data)
{
  auto *ctx = (hb_vector_svg_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  ଯଦି (unlikely (ctx->body->in_error ()))
    ଫେରନ୍ତୁ;
  hb_vector_svg_add_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_vector_svg_rgba_from_hb_color (c0),
				 a1, hb_vector_svg_rgba_from_hb_color (c1));
}


ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_ଠେଲ_transform (hb_paint_funcs_t *, ଶୂନ୍ୟ *,
                                            ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ,
                                            ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_କାଢ଼_transform (hb_paint_funcs_t *, ଶୂନ୍ୟ *, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_fill_glyph (hb_paint_funcs_t *, ଶୂନ୍ୟ *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_ଠେଲ_clip_glyph (hb_paint_funcs_t *, ଶୂନ୍ୟ *, hb_codepoint_t, hb_font_t *, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_ଠେଲ_clip_rectangle (hb_paint_funcs_t *, ଶୂନ୍ୟ *, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଶୂନ୍ୟ *);
ସ୍ଥିର hb_draw_funcs_t * hb_vector_paint_ଠେଲ_clip_path_start (hb_paint_funcs_t *, ଶୂନ୍ୟ *, ଶୂନ୍ୟ **, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_ଠେଲ_clip_path_end (hb_paint_funcs_t *, ଶୂନ୍ୟ *, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_କାଢ଼_clip (hb_paint_funcs_t *, ଶୂନ୍ୟ *, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_color (hb_paint_funcs_t *, ଶୂନ୍ୟ *, hb_bool_t, hb_color_t, ଶୂନ୍ୟ *);
ସ୍ଥିର hb_bool_t hb_vector_paint_image (hb_paint_funcs_t *, ଶୂନ୍ୟ *, hb_blob_t *, unsigned, unsigned, hb_tag_t, ଦଶମିକ, hb_glyph_extents_t *, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_linear_gradient (hb_paint_funcs_t *, ଶୂନ୍ୟ *, hb_color_ଧାଡ଼ି_t *, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_radial_gradient (hb_paint_funcs_t *, ଶୂନ୍ୟ *, hb_color_ଧାଡ଼ି_t *, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_sweep_gradient (hb_paint_funcs_t *, ଶୂନ୍ୟ *, hb_color_ଧାଡ଼ି_t *, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଦଶମିକ, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_ଠେଲ_group (hb_paint_funcs_t *, ଶୂନ୍ୟ *, ଶୂନ୍ୟ *);
ସ୍ଥିର ଶୂନ୍ୟ hb_vector_paint_କାଢ଼_group (hb_paint_funcs_t *, ଶୂନ୍ୟ *, hb_paint_composite_mode_t, ଶୂନ୍ୟ *);
ସ୍ଥିର hb_bool_t hb_vector_paint_color_glyph (hb_paint_funcs_t *, ଶୂନ୍ୟ *, hb_codepoint_t, hb_font_t *, ଶୂନ୍ୟ *);
ସ୍ଥିର hb_bool_t
hb_vector_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      ଶୂନ୍ୟ *paint_data,
				      unsigned color_index,
				      hb_color_t *color,
				      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_vector_paint_t *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (!color)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_color_t *value = nullptr;
  ଯଦି (!paint->custom_palette_colors.has (color_index, &value) || !value)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  *color = *value;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_vector_paint_funcs ();
ସ୍ଥିର ସଂରଚନା hb_vector_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_vector_paint_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_ଠେଲ_transform_func (funcs, (hb_paint_ଠେଲ_transform_func_t) hb_vector_paint_ଠେଲ_transform, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_transform_func (funcs, (hb_paint_କାଢ଼_transform_func_t) hb_vector_paint_କାଢ଼_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_vector_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_glyph_func (funcs, (hb_paint_ଠେଲ_clip_glyph_func_t) hb_vector_paint_ଠେଲ_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_rectangle_func (funcs, (hb_paint_ଠେଲ_clip_rectangle_func_t) hb_vector_paint_ଠେଲ_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_path_start_func (funcs, (hb_paint_ଠେଲ_clip_path_start_func_t) hb_vector_paint_ଠେଲ_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_path_end_func (funcs, (hb_paint_ଠେଲ_clip_path_end_func_t) hb_vector_paint_ଠେଲ_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_clip_func (funcs, (hb_paint_କାଢ଼_clip_func_t) hb_vector_paint_କାଢ଼_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_vector_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_vector_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_vector_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_vector_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_vector_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_group_func (funcs, (hb_paint_ଠେଲ_group_func_t) hb_vector_paint_ଠେଲ_group, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_group_func (funcs, (hb_paint_କାଢ଼_group_func_t) hb_vector_paint_କାଢ଼_group, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_vector_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_vector_paint_custom_palette_color, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_paint_funcs);
    ଫେରନ୍ତୁ funcs;
  }
} static_vector_paint_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
free_static_vector_paint_funcs ()
{
  static_vector_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_svg_funcs_get ()
{
  ଫେରନ୍ତୁ static_vector_paint_funcs.get_unconst ();
}


ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_ଠେଲ_transform (hb_paint_funcs_t *,
                                ଶୂନ୍ୟ *paint_data,
                                ଦଶମିକ xx, ଦଶମିକ yx,
                                ଦଶମିକ xy, ଦଶମିକ yy,
                                ଦଶମିକ dx, ଦଶମିକ dy,
                                ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;

  ଯଦି (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth++;
    ଫେରନ୍ତୁ;
  }
  ଯଦି (unlikely (paint->transform_group_depth >= 64))
  {
    paint->transform_group_overflow_depth = 1;
    ଫେରନ୍ତୁ;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_group_open_mask = (paint->transform_group_open_mask << 1) | (opened ? 1ull : 0ull);
  paint->transform_group_depth++;

  ଯଦି (!opened)
    ଫେରନ୍ତୁ;

  auto &body = paint->current_body ();
  unsigned sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_ଅଙ୍କ (xx, sprec);
  body.append_c (',');
  body.append_ଅଙ୍କ (yx, sprec);
  body.append_c (',');
  body.append_ଅଙ୍କ (xy, sprec);
  body.append_c (',');
  body.append_ଅଙ୍କ (yy, sprec);
  body.append_c (',');
  body.append_ଅଙ୍କ (paint->sx (dx));
  body.append_c (',');
  body.append_ଅଙ୍କ (paint->sy (dy));
  body.append_str (")\">\n");
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_କାଢ଼_transform (hb_paint_funcs_t *,
                               ଶୂନ୍ୟ *paint_data,
                               ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;
  ଯଦି (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth--;
    ଫେରନ୍ତୁ;
  }
  ଯଦି (!paint->transform_group_depth)
    ଫେରନ୍ତୁ;
  paint->transform_group_depth--;
  hb_bool_t opened = !!(paint->transform_group_open_mask & 1ull);
  paint->transform_group_open_mask >>= 1;
  ଯଦି (opened)
    paint->current_body ().append_str ("</g>\n");
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_fill_glyph (hb_paint_funcs_t *,
                            ଶୂନ୍ୟ *paint_data,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  ଯଦି (likely (paint->budget_remaining >= 0))
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
  body.append_svg_color (color, ସତ୍ୟ);
  body.append_str ("\"/>\n");
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_ଠେଲ_clip_glyph (hb_paint_funcs_t *,
                                 ଶୂନ୍ୟ *paint_data,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;

  ଅଚଳ ଅକ୍ଷର *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  ଯଦି (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  unsigned def_id = paint->path_def_ଗଣନା++;
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

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_ଠେଲ_clip_rectangle (hb_paint_funcs_t *,
                                     ଶୂନ୍ୟ *paint_data,
                                     ଦଶମିକ xmin, ଦଶମିକ ymin,
                                     ଦଶମିକ xmax, ଦଶମିକ ymax,
                                     ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;

  ଅଚଳ ଅକ୍ଷର *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_ଅଙ୍କ (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_ଅଙ୍କ (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_ଅଙ୍କ (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_ଅଙ୍କ (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ସ୍ଥିର hb_draw_funcs_t *
hb_vector_paint_ଠେଲ_clip_path_start (hb_paint_funcs_t *,
                                      ଶୂନ୍ୟ *paint_data,
                                      ଶୂନ୍ୟ **draw_data,
                                      ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    ଫେରନ୍ତୁ nullptr;
  }

  paint->path.clear ();
  paint->clip_path_sink = {&paint->path, paint->get_precision (),
			   paint->x_scale_factor,
			   paint->y_scale_factor,
			   &paint->budget_remaining};
  *draw_data = &paint->clip_path_sink;
  ଫେରନ୍ତୁ hb_vector_svg_path_draw_funcs_get ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_ଠେଲ_clip_path_end (hb_paint_funcs_t *,
                                    ଶୂନ୍ୟ *paint_data,
                                    ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;

  ଅଚଳ ଅକ୍ଷର *pfx = paint->id_prefix;
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

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_କାଢ଼_clip (hb_paint_funcs_t *,
                          ଶୂନ୍ୟ *paint_data,
                          ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;
  paint->current_body ().append_str ("</g>\n");
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_color (hb_paint_funcs_t *,
                       ଶୂନ୍ୟ *paint_data,
                       hb_bool_t,
                       hb_color_t color,
                       ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;

  auto &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, ସତ୍ୟ);
  body.append_str ("\"/>\n");
}

ସ୍ଥିର hb_bool_t
hb_vector_paint_image (hb_paint_funcs_t *,
                       ଶୂନ୍ୟ *paint_data,
                       hb_blob_t *image,
                       unsigned width,
                       unsigned height,
                       hb_tag_t format,
                       ଦଶମିକ slant HB_UNUSED,
                       hb_glyph_extents_t *extents,
                       ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  auto &body = paint->current_body ();
  ଯଦି (format == HB_TAG ('p','n','g',' '))
  {
    ଯଦି (!extents || !width || !height)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned len = 0;
    ଅଚଳ ଅକ୍ଷର *png_data = hb_blob_get_data (image, &len);
    ଯଦି (!png_data || !len)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    body.append_str ("<g transform=\"translate(");
    body.append_ଅଙ୍କ (paint->sx ((ଦଶମିକ) extents->x_bearing));
    body.append_c (',');
    body.append_ଅଙ୍କ (paint->sy ((ଦଶମିକ) extents->y_bearing));
    body.append_str (") scale(");
    body.append_ଅଙ୍କ (paint->sx ((ଦଶମିକ) extents->width) / width);
    body.append_c (',');
    body.append_ଅଙ୍କ (paint->sy ((ଦଶମିକ) extents->height) / height);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ଅଚଳ uint8_t *) png_data, len);
    body.append_str ("\" width=\"");
    body.append_ଅଙ୍କ ((ଦଶମିକ) width);
    body.append_str ("\" height=\"");
    body.append_ଅଙ୍କ ((ଦଶମିକ) height);
    body.append_str ("\"/>\n</g>\n");

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_linear_gradient (hb_paint_funcs_t *,
                                 ଶୂନ୍ୟ *paint_data,
                                 hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
                                 ଦଶମିକ x0, ଦଶମିକ y0,
                                 ଦଶମିକ x1, ଦଶମିକ y1,
                                 ଦଶମିକ x2, ଦଶମିକ y2,
                                 ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;

  ଯଦି (!paint->fetch_color_stops (color_ଧାଡ଼ି))
    ଫେରନ୍ତୁ;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  ଦଶମିକ mn, mx;
  hb_paint_normalize_color_ଧାଡ଼ି (stops.arrayZ, stops.length, &mn, &mx);

  ଅଚଳ ଅକ୍ଷର *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  ଦଶମିକ lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  ଦଶମିକ gx0 = lx0 + mn * (lx1 - lx0);
  ଦଶମିକ gy0 = ly0 + mn * (ly1 - ly0);
  ଦଶମିକ gx1 = lx0 + mx * (lx1 - lx0);
  ଦଶମିକ gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_ଅଙ୍କ (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_ଅଙ୍କ (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_ଅଙ୍କ (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_ଅଙ୍କ (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_ଧାଡ଼ି_get_extend (color_ଧାଡ଼ି)));
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

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_radial_gradient (hb_paint_funcs_t *,
                                 ଶୂନ୍ୟ *paint_data,
                                 hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
                                 ଦଶମିକ x0, ଦଶମିକ y0, ଦଶମିକ r0,
                                 ଦଶମିକ x1, ଦଶମିକ y1, ଦଶମିକ r1,
                                 ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;

  ଯଦି (!paint->fetch_color_stops (color_ଧାଡ଼ି))
    ଫେରନ୍ତୁ;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ଦଶମିକ mn, mx;
  hb_paint_normalize_color_ଧାଡ଼ି (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  ଦଶମିକ gx0 = x0 + mn * (x1 - x0);
  ଦଶମିକ gy0 = y0 + mn * (y1 - y0);
  ଦଶମିକ gr0 = r0 + mn * (r1 - r0);
  ଦଶମିକ gx1 = x0 + mx * (x1 - x0);
  ଦଶମିକ gy1 = y0 + mx * (y1 - y0);
  ଦଶମିକ gr1 = r0 + mx * (r1 - r0);

  ଅଚଳ ଅକ୍ଷର *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_ଅଙ୍କ (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_ଅଙ୍କ (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_ଅଙ୍କ (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_ଅଙ୍କ (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_ଅଙ୍କ (paint->sy (gy0));
  ଯଦି (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_ଅଙ୍କ (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_ଧାଡ଼ି_get_extend (color_ଧାଡ଼ି)));
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

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_sweep_gradient (hb_paint_funcs_t *,
                                ଶୂନ୍ୟ *paint_data,
                                hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
                                ଦଶମିକ cx, ଦଶମିକ cy,
                                ଦଶମିକ start_angle, ଦଶମିକ end_angle,
                                ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;

  ଯଦି (!paint->fetch_color_stops (color_ଧାଡ଼ି))
    ଫେରନ୍ତୁ;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ଦଶମିକ mn, mx;
  hb_paint_normalize_color_ଧାଡ଼ି (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  ଦଶମିକ ga0 = start_angle + mn * (end_angle - start_angle);
  ଦଶମିକ ga1 = start_angle + mx * (end_angle - start_angle);

  hb_vector_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length,
				 hb_color_ଧାଡ଼ି_get_extend (color_ଧାଡ଼ି),
				 ga0, ga1,
				 hb_vector_svg_sweep_emit_patch,
				 &ctx);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_ଠେଲ_group (hb_paint_funcs_t *,
                            ଶୂନ୍ୟ *paint_data,
                            ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;
  ଯଦି (unlikely (!paint->group_ଗଦା.ଠେଲ_or_fail (hb_vector_buf_t {})))
    ଫେରନ୍ତୁ;
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_vector_paint_କାଢ଼_group (hb_paint_funcs_t *,
                           ଶୂନ୍ୟ *paint_data,
                           hb_paint_composite_mode_t mode,
                           ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ;
  ଯଦି (paint->group_ଗଦା.length < 2)
    ଫେରନ୍ତୁ;

  hb_vector_buf_t group = paint->group_ଗଦା.କାଢ଼ ();
  auto &body = paint->current_body ();

  ଅଚଳ ଅକ୍ଷର *blend = hb_vector_svg_composite_mode_str (mode);
  ଯଦି (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (group.arrayZ, group.length);
    body.append_str ("</g>\n");
  }
  ଅନ୍ୟଥା
    body.append_len (group.arrayZ, group.length);
}

ସ୍ଥିର hb_bool_t
hb_vector_paint_color_glyph (hb_paint_funcs_t *,
                             ଶୂନ୍ୟ *paint_data,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             ଶୂନ୍ୟ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଯଦି (unlikely (paint->color_glyph_depth >= HB_MAX_NESTING_LEVEL))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଯଦି (unlikely (hb_set_has (paint->active_color_glyphs, glyph)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  paint->color_glyph_depth++;
  hb_set_add (paint->active_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_vector_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_set_del (paint->active_color_glyphs, glyph);
  paint->color_glyph_depth--;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}




hb_blob_t *
hb_vector_paint_render_svg (hb_vector_paint_t *paint)
{
  ଯଦି (!paint->has_extents)
    ଫେରନ୍ତୁ nullptr;

  ଯଦି (unlikely (!paint->ensure_initialized ()))
    ଫେରନ୍ତୁ nullptr;

  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  unsigned estimated = paint->defs.length +
		       paint->group_ଗଦା.arrayZ[0].length +
		       320;
  out.alloc (estimated);
  ଦଶମିକ vb_x = paint->extents.x;
  ଦଶମିକ vb_y = -(paint->extents.y + paint->extents.height);
  ଦଶମିକ vb_w = paint->extents.width;
  ଦଶମିକ vb_h = paint->extents.height;
  out.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  out.append_ଅଙ୍କ (vb_x);
  out.append_c (' ');
  out.append_ଅଙ୍କ (vb_y);
  out.append_c (' ');
  out.append_ଅଙ୍କ (vb_w);
  out.append_c (' ');
  out.append_ଅଙ୍କ (vb_h);
  out.append_str ("\" width=\"");
  out.append_ଅଙ୍କ (vb_w);
  out.append_str ("\" height=\"");
  out.append_ଅଙ୍କ (vb_h);
  out.append_str ("\">\n");

  ଯଦି (paint->defs.length)
  {
    out.append_str ("<defs>\n");
    out.append_len (paint->defs.arrayZ, paint->defs.length);
    out.append_str ("</defs>\n");
  }

  ଯଦି (hb_color_get_alpha (paint->background))
  {
    out.append_str ("<rect x=\"");
    out.append_ଅଙ୍କ (vb_x);
    out.append_str ("\" y=\"");
    out.append_ଅଙ୍କ (vb_y);
    out.append_str ("\" width=\"");
    out.append_ଅଙ୍କ (vb_w);
    out.append_str ("\" height=\"");
    out.append_ଅଙ୍କ (vb_h);
    out.append_str ("\" fill=\"rgb(");
    out.append_unsigned (hb_color_get_red (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_green (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_blue (paint->background));
    out.append_str (")\"");
    ଯଦି (hb_color_get_alpha (paint->background) < 255)
    {
      out.append_str (" fill-opacity=\"");
      out.append_ଅଙ୍କ (hb_color_get_alpha (paint->background) / 255.f, 4);
      out.append_c ('"');
    }
    out.append_str ("/>\n");
  }

  out.append_str ("<g transform=\"scale(1,-1)\">\n");
  out.append_len (paint->group_ଗଦା.arrayZ[0].arrayZ, paint->group_ଗଦା.arrayZ[0].length);
  out.append_str ("</g>\n");

  out.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  ଫେରନ୍ତୁ blob;
}
