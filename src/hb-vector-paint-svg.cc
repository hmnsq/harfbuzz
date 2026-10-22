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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-vector-paint.hh"
#অন্তর্ভাব "hb-paint.hh"
#অন্তর্ভাব "hb-vector-path.hh"

#অন্তর্ভাব <math.h>

স্থির ধ্রুব বর্ণ *
hb_vector_svg_extend_mode_str (hb_paint_extend_t ext)
{
  বিকল্প (ext)
  {
    পক্ষ HB_PAINT_EXTEND_PAD: প্রত্যাগম "pad";
    পক্ষ HB_PAINT_EXTEND_REPEAT: প্রত্যাগম "repeat";
    পক্ষ HB_PAINT_EXTEND_REFLECT: প্রত্যাগম "reflect";
    স্বভাব: প্রত্যাগম "pad";
  }
}

স্থির শূন্য
hb_vector_svg_emit_color_stops (hb_vector_paint_t *paint,
				hb_vector_buf_t *buf,
				hb_vector_t<hb_color_stop_t> *stops)
{
  প্রতি (unsigned i = 0; i < stops->length; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_অঙ্ক (stops->arrayZ[i].offset, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    যদি (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_অঙ্ক (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

স্থির ধ্রুব বর্ণ *
hb_vector_svg_composite_mode_str (hb_paint_composite_mode_t mode)
{
  বিকল্প (mode)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    পক্ষ HB_PAINT_COMPOSITE_MODE_PLUS: প্রত্যাগম "screen";
    পক্ষ HB_PAINT_COMPOSITE_MODE_XOR: প্রত্যাগম "difference";
    পক্ষ HB_PAINT_COMPOSITE_MODE_CLEAR:
    পক্ষ HB_PAINT_COMPOSITE_MODE_SRC:
    পক্ষ HB_PAINT_COMPOSITE_MODE_DEST:
    পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_IN:
    পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_IN:
    পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
    পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      প্রত্যাগম nullptr;
    পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_OVER: প্রত্যাগম "normal";
    পক্ষ HB_PAINT_COMPOSITE_MODE_SCREEN: প্রত্যাগম "screen";
    পক্ষ HB_PAINT_COMPOSITE_MODE_OVERLAY: প্রত্যাগম "overlay";
    পক্ষ HB_PAINT_COMPOSITE_MODE_DARKEN: প্রত্যাগম "darken";
    পক্ষ HB_PAINT_COMPOSITE_MODE_LIGHTEN: প্রত্যাগম "lighten";
    পক্ষ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: প্রত্যাগম "color-dodge";
    পক্ষ HB_PAINT_COMPOSITE_MODE_COLOR_BURN: প্রত্যাগম "color-burn";
    পক্ষ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: প্রত্যাগম "hard-light";
    পক্ষ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: প্রত্যাগম "soft-light";
    পক্ষ HB_PAINT_COMPOSITE_MODE_DIFFERENCE: প্রত্যাগম "difference";
    পক্ষ HB_PAINT_COMPOSITE_MODE_EXCLUSION: প্রত্যাগম "exclusion";
    পক্ষ HB_PAINT_COMPOSITE_MODE_MULTIPLY: প্রত্যাগম "multiply";
    পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_HUE: প্রত্যাগম "hue";
    পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: প্রত্যাগম "saturation";
    পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_COLOR: প্রত্যাগম "color";
    পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: প্রত্যাগম "luminosity";
    স্বভাব: প্রত্যাগম nullptr;
  }
}

সংরচনা hb_vector_svg_point_t { ভিন্ন x, y; };
সংরচনা hb_vector_svg_rgba_t { ভিন্ন r, g, b, a; };

স্থির অন্তঃস্থ ভিন্ন
hb_vector_svg_lerp (ভিন্ন a, ভিন্ন b, ভিন্ন t)
{ প্রত্যাগম a + (b - a) * t; }

স্থির অন্তঃস্থ ভিন্ন
hb_vector_svg_clamp01 (ভিন্ন v)
{
  যদি (v < 0.f) প্রত্যাগম 0.f;
  যদি (v > 1.f) প্রত্যাগম 1.f;
  প্রত্যাগম v;
}

স্থির অন্তঃস্থ hb_vector_svg_rgba_t
hb_vector_svg_rgba_from_hb_color (hb_color_t c)
{
  প্রত্যাগম {(ভিন্ন) hb_color_get_red (c) / 255.f,
          (ভিন্ন) hb_color_get_green (c) / 255.f,
          (ভিন্ন) hb_color_get_blue (c) / 255.f,
          (ভিন্ন) hb_color_get_alpha (c) / 255.f};
}

স্থির অন্তঃস্থ hb_color_t
hb_vector_svg_hb_color_from_rgba (ধ্রুব hb_vector_svg_rgba_t &c)
{
  unsigned r = (unsigned) roundf (hb_vector_svg_clamp01 (c.r) * 255.f);
  unsigned g = (unsigned) roundf (hb_vector_svg_clamp01 (c.g) * 255.f);
  unsigned b = (unsigned) roundf (hb_vector_svg_clamp01 (c.b) * 255.f);
  unsigned a = (unsigned) roundf (hb_vector_svg_clamp01 (c.a) * 255.f);
  প্রত্যাগম HB_COLOR (b, g, r, a);
}

স্থির অন্তঃস্থ hb_vector_svg_rgba_t
hb_vector_svg_lerp_rgba (ধ্রুব hb_vector_svg_rgba_t &c0,
			 ধ্রুব hb_vector_svg_rgba_t &c1,
			 ভিন্ন t)
{
  প্রত্যাগম {hb_vector_svg_lerp (c0.r, c1.r, t),
          hb_vector_svg_lerp (c0.g, c1.g, t),
          hb_vector_svg_lerp (c0.b, c1.b, t),
          hb_vector_svg_lerp (c0.a, c1.a, t)};
}

স্থির অন্তঃস্থ ভিন্ন hb_vector_svg_dot (ধ্রুব hb_vector_svg_point_t &p, ধ্রুব hb_vector_svg_point_t &q) { প্রত্যাগম p.x * q.x + p.y * q.y; }
স্থির অন্তঃস্থ hb_vector_svg_point_t hb_vector_svg_add (ধ্রুব hb_vector_svg_point_t &p, ধ্রুব hb_vector_svg_point_t &q) { প্রত্যাগম {p.x + q.x, p.y + q.y}; }
স্থির অন্তঃস্থ hb_vector_svg_point_t hb_vector_svg_sub (ধ্রুব hb_vector_svg_point_t &p, ধ্রুব hb_vector_svg_point_t &q) { প্রত্যাগম {p.x - q.x, p.y - q.y}; }
স্থির অন্তঃস্থ hb_vector_svg_point_t hb_vector_svg_scale (ধ্রুব hb_vector_svg_point_t &p, ভিন্ন f) { প্রত্যাগম {p.x * f, p.y * f}; }

স্থির অন্তঃস্থ hb_vector_svg_point_t
hb_vector_svg_normalize (ধ্রুব hb_vector_svg_point_t &p)
{
  ভিন্ন len = sqrtf (hb_vector_svg_dot (p, p));
  যদি (len == 0.f) প্রত্যাগম {0.f, 0.f};
  প্রত্যাগম hb_vector_svg_scale (p, 1.f / len);
}

স্থির শূন্য
hb_vector_svg_add_sweep_patch (hb_vector_buf_t *body,
			       unsigned precision,
			       ভিন্ন cx, ভিন্ন cy, ভিন্ন radius,
			       ভিন্ন a0, ধ্রুব hb_vector_svg_rgba_t &c0_in,
			       ভিন্ন a1, ধ্রুব hb_vector_svg_rgba_t &c1_in)
{
  স্থির ধ্রুব ভিন্ন max_angle = HB_PI / 16.f;
  hb_vector_svg_point_t center = {cx, cy};
  পূর্ণাঙ্ক অঙ্ক_splits = (পূর্ণাঙ্ক) ceilf (fabsf (a1 - a0) / max_angle);
  যদি (অঙ্ক_splits < 1) অঙ্ক_splits = 1;

  hb_vector_svg_point_t p0 = {cosf (a0), sinf (a0)};
  hb_vector_svg_rgba_t color0 = c0_in;

  প্রতি (পূর্ণাঙ্ক a = 0; a < অঙ্ক_splits; a++)
  {
    ভিন্ন k = (a + 1.f) / অঙ্ক_splits;
    ভিন্ন angle1 = hb_vector_svg_lerp (a0, a1, k);
    hb_vector_svg_rgba_t color1 = hb_vector_svg_lerp_rgba (c0_in, c1_in, k);

    hb_vector_svg_point_t p1 = {cosf (angle1), sinf (angle1)};
    hb_vector_svg_point_t sp0 = hb_vector_svg_add (center, hb_vector_svg_scale (p0, radius));
    hb_vector_svg_point_t sp1 = hb_vector_svg_add (center, hb_vector_svg_scale (p1, radius));

    hb_vector_svg_point_t A = hb_vector_svg_normalize (hb_vector_svg_add (p0, p1));
    hb_vector_svg_point_t U = {-A.y, A.x};
    ভিন্ন up0 = hb_vector_svg_dot (U, p0);
    ভিন্ন up1 = hb_vector_svg_dot (U, p1);
    যদি (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      অনুবর্ত;
    }
    hb_vector_svg_point_t C0 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p0, A), p0) / up0));
    hb_vector_svg_point_t C1 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p1, A), p1) / up1));

    hb_vector_svg_point_t sc0 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C0, hb_vector_svg_scale (hb_vector_svg_sub (C0, p0), 0.33333f)), radius));
    hb_vector_svg_point_t sc1 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C1, hb_vector_svg_scale (hb_vector_svg_sub (C1, p1), 0.33333f)), radius));

    hb_vector_svg_rgba_t mid_color = hb_vector_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_vector_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_অঙ্ক (center.x, precision);
    body->append_c (',');
    body->append_অঙ্ক (center.y, precision);
    body->append_str ("L");
    body->append_অঙ্ক (sp0.x, precision);
    body->append_c (',');
    body->append_অঙ্ক (sp0.y, precision);
    body->append_str ("C");
    body->append_অঙ্ক (sc0.x, precision);
    body->append_c (',');
    body->append_অঙ্ক (sc0.y, precision);
    body->append_c (' ');
    body->append_অঙ্ক (sc1.x, precision);
    body->append_c (',');
    body->append_অঙ্ক (sc1.y, precision);
    body->append_c (' ');
    body->append_অঙ্ক (sp1.x, precision);
    body->append_c (',');
    body->append_অঙ্ক (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, সত্য);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
সংরচনা hb_vector_svg_sweep_ctx_t
{
  hb_vector_buf_t *body;
  unsigned precision;
  ভিন্ন cx, cy, radius;
};

স্থির শূন্য
hb_vector_svg_sweep_emit_patch (ভিন্ন a0, hb_color_t c0,
				ভিন্ন a1, hb_color_t c1,
				শূন্য *user_data)
{
  auto *ctx = (hb_vector_svg_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  যদি (unlikely (ctx->body->in_error ()))
    প্রত্যাগম;
  hb_vector_svg_add_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_vector_svg_rgba_from_hb_color (c0),
				 a1, hb_vector_svg_rgba_from_hb_color (c1));
}


স্থির শূন্য hb_vector_paint_ঠেলো_transform (hb_paint_funcs_t *, শূন্য *,
                                            ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন,
                                            শূন্য *);
স্থির শূন্য hb_vector_paint_বের_করো_transform (hb_paint_funcs_t *, শূন্য *, শূন্য *);
স্থির শূন্য hb_vector_paint_fill_glyph (hb_paint_funcs_t *, শূন্য *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, শূন্য *);
স্থির শূন্য hb_vector_paint_ঠেলো_clip_glyph (hb_paint_funcs_t *, শূন্য *, hb_codepoint_t, hb_font_t *, শূন্য *);
স্থির শূন্য hb_vector_paint_ঠেলো_clip_rectangle (hb_paint_funcs_t *, শূন্য *, ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন, শূন্য *);
স্থির hb_draw_funcs_t * hb_vector_paint_ঠেলো_clip_path_start (hb_paint_funcs_t *, শূন্য *, শূন্য **, শূন্য *);
স্থির শূন্য hb_vector_paint_ঠেলো_clip_path_end (hb_paint_funcs_t *, শূন্য *, শূন্য *);
স্থির শূন্য hb_vector_paint_বের_করো_clip (hb_paint_funcs_t *, শূন্য *, শূন্য *);
স্থির শূন্য hb_vector_paint_color (hb_paint_funcs_t *, শূন্য *, hb_bool_t, hb_color_t, শূন্য *);
স্থির hb_bool_t hb_vector_paint_image (hb_paint_funcs_t *, শূন্য *, hb_blob_t *, unsigned, unsigned, hb_tag_t, ভিন্ন, hb_glyph_extents_t *, শূন্য *);
স্থির শূন্য hb_vector_paint_linear_gradient (hb_paint_funcs_t *, শূন্য *, hb_color_সারি_t *, ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন, শূন্য *);
স্থির শূন্য hb_vector_paint_radial_gradient (hb_paint_funcs_t *, শূন্য *, hb_color_সারি_t *, ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন, শূন্য *);
স্থির শূন্য hb_vector_paint_sweep_gradient (hb_paint_funcs_t *, শূন্য *, hb_color_সারি_t *, ভিন্ন, ভিন্ন, ভিন্ন, ভিন্ন, শূন্য *);
স্থির শূন্য hb_vector_paint_ঠেলো_group (hb_paint_funcs_t *, শূন্য *, শূন্য *);
স্থির শূন্য hb_vector_paint_বের_করো_group (hb_paint_funcs_t *, শূন্য *, hb_paint_composite_mode_t, শূন্য *);
স্থির hb_bool_t hb_vector_paint_color_glyph (hb_paint_funcs_t *, শূন্য *, hb_codepoint_t, hb_font_t *, শূন্য *);
স্থির hb_bool_t
hb_vector_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      শূন্য *paint_data,
				      unsigned color_index,
				      hb_color_t *color,
				      শূন্য *user_data HB_UNUSED)
{
  hb_vector_paint_t *paint = (hb_vector_paint_t *) paint_data;
  যদি (!color)
    প্রত্যাগম অসত্য;

  hb_color_t *value = nullptr;
  যদি (!paint->custom_palette_colors.has (color_index, &value) || !value)
    প্রত্যাগম অসত্য;
  *color = *value;
  প্রত্যাগম সত্য;
}

স্থির অন্তঃস্থ শূন্য free_static_vector_paint_funcs ();
স্থির সংরচনা hb_vector_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_vector_paint_funcs_lazy_loader_t>
{
  স্থির hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_ঠেলো_transform_func (funcs, (hb_paint_ঠেলো_transform_func_t) hb_vector_paint_ঠেলো_transform, nullptr, nullptr);
    hb_paint_funcs_set_বের_করো_transform_func (funcs, (hb_paint_বের_করো_transform_func_t) hb_vector_paint_বের_করো_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_vector_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_glyph_func (funcs, (hb_paint_ঠেলো_clip_glyph_func_t) hb_vector_paint_ঠেলো_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_rectangle_func (funcs, (hb_paint_ঠেলো_clip_rectangle_func_t) hb_vector_paint_ঠেলো_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_path_start_func (funcs, (hb_paint_ঠেলো_clip_path_start_func_t) hb_vector_paint_ঠেলো_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_path_end_func (funcs, (hb_paint_ঠেলো_clip_path_end_func_t) hb_vector_paint_ঠেলো_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_বের_করো_clip_func (funcs, (hb_paint_বের_করো_clip_func_t) hb_vector_paint_বের_করো_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_vector_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_vector_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_vector_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_vector_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_vector_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_group_func (funcs, (hb_paint_ঠেলো_group_func_t) hb_vector_paint_ঠেলো_group, nullptr, nullptr);
    hb_paint_funcs_set_বের_করো_group_func (funcs, (hb_paint_বের_করো_group_func_t) hb_vector_paint_বের_করো_group, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_vector_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_vector_paint_custom_palette_color, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_paint_funcs);
    প্রত্যাগম funcs;
  }
} static_vector_paint_funcs;

স্থির অন্তঃস্থ শূন্য
free_static_vector_paint_funcs ()
{
  static_vector_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_svg_funcs_get ()
{
  প্রত্যাগম static_vector_paint_funcs.get_unconst ();
}


স্থির শূন্য
hb_vector_paint_ঠেলো_transform (hb_paint_funcs_t *,
                                শূন্য *paint_data,
                                ভিন্ন xx, ভিন্ন yx,
                                ভিন্ন xy, ভিন্ন yy,
                                ভিন্ন dx, ভিন্ন dy,
                                শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  যদি (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth++;
    প্রত্যাগম;
  }
  যদি (unlikely (paint->transform_group_depth >= 64))
  {
    paint->transform_group_overflow_depth = 1;
    প্রত্যাগম;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_group_open_mask = (paint->transform_group_open_mask << 1) | (opened ? 1ull : 0ull);
  paint->transform_group_depth++;

  যদি (!opened)
    প্রত্যাগম;

  auto &body = paint->current_body ();
  unsigned sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_অঙ্ক (xx, sprec);
  body.append_c (',');
  body.append_অঙ্ক (yx, sprec);
  body.append_c (',');
  body.append_অঙ্ক (xy, sprec);
  body.append_c (',');
  body.append_অঙ্ক (yy, sprec);
  body.append_c (',');
  body.append_অঙ্ক (paint->sx (dx));
  body.append_c (',');
  body.append_অঙ্ক (paint->sy (dy));
  body.append_str (")\">\n");
}

স্থির শূন্য
hb_vector_paint_বের_করো_transform (hb_paint_funcs_t *,
                               শূন্য *paint_data,
                               শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  যদি (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth--;
    প্রত্যাগম;
  }
  যদি (!paint->transform_group_depth)
    প্রত্যাগম;
  paint->transform_group_depth--;
  hb_bool_t opened = !!(paint->transform_group_open_mask & 1ull);
  paint->transform_group_open_mask >>= 1;
  যদি (opened)
    paint->current_body ().append_str ("</g>\n");
}

স্থির শূন্য
hb_vector_paint_fill_glyph (hb_paint_funcs_t *,
                            শূন্য *paint_data,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  যদি (likely (paint->budget_remaining >= 0))
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
  body.append_svg_color (color, সত্য);
  body.append_str ("\"/>\n");
}

স্থির শূন্য
hb_vector_paint_ঠেলো_clip_glyph (hb_paint_funcs_t *,
                                 শূন্য *paint_data,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  ধ্রুব বর্ণ *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  যদি (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  unsigned def_id = paint->path_def_গুনতি++;
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

স্থির শূন্য
hb_vector_paint_ঠেলো_clip_rectangle (hb_paint_funcs_t *,
                                     শূন্য *paint_data,
                                     ভিন্ন xmin, ভিন্ন ymin,
                                     ভিন্ন xmax, ভিন্ন ymax,
                                     শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  ধ্রুব বর্ণ *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_অঙ্ক (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_অঙ্ক (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_অঙ্ক (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_অঙ্ক (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

স্থির hb_draw_funcs_t *
hb_vector_paint_ঠেলো_clip_path_start (hb_paint_funcs_t *,
                                      শূন্য *paint_data,
                                      শূন্য **draw_data,
                                      শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    প্রত্যাগম nullptr;
  }

  paint->path.clear ();
  paint->clip_path_sink = {&paint->path, paint->get_precision (),
			   paint->x_scale_factor,
			   paint->y_scale_factor,
			   &paint->budget_remaining};
  *draw_data = &paint->clip_path_sink;
  প্রত্যাগম hb_vector_svg_path_draw_funcs_get ();
}

স্থির শূন্য
hb_vector_paint_ঠেলো_clip_path_end (hb_paint_funcs_t *,
                                    শূন্য *paint_data,
                                    শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  ধ্রুব বর্ণ *pfx = paint->id_prefix;
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

স্থির শূন্য
hb_vector_paint_বের_করো_clip (hb_paint_funcs_t *,
                          শূন্য *paint_data,
                          শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  paint->current_body ().append_str ("</g>\n");
}

স্থির শূন্য
hb_vector_paint_color (hb_paint_funcs_t *,
                       শূন্য *paint_data,
                       hb_bool_t,
                       hb_color_t color,
                       শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  auto &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, সত্য);
  body.append_str ("\"/>\n");
}

স্থির hb_bool_t
hb_vector_paint_image (hb_paint_funcs_t *,
                       শূন্য *paint_data,
                       hb_blob_t *image,
                       unsigned width,
                       unsigned height,
                       hb_tag_t format,
                       ভিন্ন slant HB_UNUSED,
                       hb_glyph_extents_t *extents,
                       শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম অসত্য;

  auto &body = paint->current_body ();
  যদি (format == HB_TAG ('p','n','g',' '))
  {
    যদি (!extents || !width || !height)
      প্রত্যাগম অসত্য;

    unsigned len = 0;
    ধ্রুব বর্ণ *png_data = hb_blob_get_data (image, &len);
    যদি (!png_data || !len)
      প্রত্যাগম অসত্য;

    body.append_str ("<g transform=\"translate(");
    body.append_অঙ্ক (paint->sx ((ভিন্ন) extents->x_bearing));
    body.append_c (',');
    body.append_অঙ্ক (paint->sy ((ভিন্ন) extents->y_bearing));
    body.append_str (") scale(");
    body.append_অঙ্ক (paint->sx ((ভিন্ন) extents->width) / width);
    body.append_c (',');
    body.append_অঙ্ক (paint->sy ((ভিন্ন) extents->height) / height);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ধ্রুব uint8_t *) png_data, len);
    body.append_str ("\" width=\"");
    body.append_অঙ্ক ((ভিন্ন) width);
    body.append_str ("\" height=\"");
    body.append_অঙ্ক ((ভিন্ন) height);
    body.append_str ("\"/>\n</g>\n");

    প্রত্যাগম সত্য;
  }

  প্রত্যাগম অসত্য;
}

স্থির শূন্য
hb_vector_paint_linear_gradient (hb_paint_funcs_t *,
                                 শূন্য *paint_data,
                                 hb_color_সারি_t *color_সারি,
                                 ভিন্ন x0, ভিন্ন y0,
                                 ভিন্ন x1, ভিন্ন y1,
                                 ভিন্ন x2, ভিন্ন y2,
                                 শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  যদি (!paint->fetch_color_stops (color_সারি))
    প্রত্যাগম;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  ভিন্ন mn, mx;
  hb_paint_normalize_color_সারি (stops.arrayZ, stops.length, &mn, &mx);

  ধ্রুব বর্ণ *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  ভিন্ন lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  ভিন্ন gx0 = lx0 + mn * (lx1 - lx0);
  ভিন্ন gy0 = ly0 + mn * (ly1 - ly0);
  ভিন্ন gx1 = lx0 + mx * (lx1 - lx0);
  ভিন্ন gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_অঙ্ক (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_অঙ্ক (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_অঙ্ক (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_অঙ্ক (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_সারি_get_extend (color_সারি)));
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

স্থির শূন্য
hb_vector_paint_radial_gradient (hb_paint_funcs_t *,
                                 শূন্য *paint_data,
                                 hb_color_সারি_t *color_সারি,
                                 ভিন্ন x0, ভিন্ন y0, ভিন্ন r0,
                                 ভিন্ন x1, ভিন্ন y1, ভিন্ন r1,
                                 শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  যদি (!paint->fetch_color_stops (color_সারি))
    প্রত্যাগম;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ভিন্ন mn, mx;
  hb_paint_normalize_color_সারি (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  ভিন্ন gx0 = x0 + mn * (x1 - x0);
  ভিন্ন gy0 = y0 + mn * (y1 - y0);
  ভিন্ন gr0 = r0 + mn * (r1 - r0);
  ভিন্ন gx1 = x0 + mx * (x1 - x0);
  ভিন্ন gy1 = y0 + mx * (y1 - y0);
  ভিন্ন gr1 = r0 + mx * (r1 - r0);

  ধ্রুব বর্ণ *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_অঙ্ক (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_অঙ্ক (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_অঙ্ক (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_অঙ্ক (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_অঙ্ক (paint->sy (gy0));
  যদি (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_অঙ্ক (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_সারি_get_extend (color_সারি)));
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

স্থির শূন্য
hb_vector_paint_sweep_gradient (hb_paint_funcs_t *,
                                শূন্য *paint_data,
                                hb_color_সারি_t *color_সারি,
                                ভিন্ন cx, ভিন্ন cy,
                                ভিন্ন start_angle, ভিন্ন end_angle,
                                শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  যদি (!paint->fetch_color_stops (color_সারি))
    প্রত্যাগম;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ভিন্ন mn, mx;
  hb_paint_normalize_color_সারি (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  ভিন্ন ga0 = start_angle + mn * (end_angle - start_angle);
  ভিন্ন ga1 = start_angle + mx * (end_angle - start_angle);

  hb_vector_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length,
				 hb_color_সারি_get_extend (color_সারি),
				 ga0, ga1,
				 hb_vector_svg_sweep_emit_patch,
				 &ctx);
}

স্থির শূন্য
hb_vector_paint_ঠেলো_group (hb_paint_funcs_t *,
                            শূন্য *paint_data,
                            শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  যদি (unlikely (!paint->group_স্তূপ.ঠেলো_or_fail (hb_vector_buf_t {})))
    প্রত্যাগম;
}

স্থির শূন্য
hb_vector_paint_বের_করো_group (hb_paint_funcs_t *,
                           শূন্য *paint_data,
                           hb_paint_composite_mode_t mode,
                           শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  যদি (paint->group_স্তূপ.length < 2)
    প্রত্যাগম;

  hb_vector_buf_t group = paint->group_স্তূপ.বের_করো ();
  auto &body = paint->current_body ();

  ধ্রুব বর্ণ *blend = hb_vector_svg_composite_mode_str (mode);
  যদি (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (group.arrayZ, group.length);
    body.append_str ("</g>\n");
  }
  অন্যথা
    body.append_len (group.arrayZ, group.length);
}

স্থির hb_bool_t
hb_vector_paint_color_glyph (hb_paint_funcs_t *,
                             শূন্য *paint_data,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম অসত্য;
  যদি (unlikely (paint->color_glyph_depth >= HB_MAX_NESTING_LEVEL))
    প্রত্যাগম অসত্য;
  যদি (unlikely (hb_set_has (paint->active_color_glyphs, glyph)))
    প্রত্যাগম অসত্য;

  paint->color_glyph_depth++;
  hb_set_add (paint->active_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_vector_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_set_del (paint->active_color_glyphs, glyph);
  paint->color_glyph_depth--;
  প্রত্যাগম সত্য;
}




hb_blob_t *
hb_vector_paint_render_svg (hb_vector_paint_t *paint)
{
  যদি (!paint->has_extents)
    প্রত্যাগম nullptr;

  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম nullptr;

  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  unsigned estimated = paint->defs.length +
		       paint->group_স্তূপ.arrayZ[0].length +
		       320;
  out.alloc (estimated);
  ভিন্ন vb_x = paint->extents.x;
  ভিন্ন vb_y = -(paint->extents.y + paint->extents.height);
  ভিন্ন vb_w = paint->extents.width;
  ভিন্ন vb_h = paint->extents.height;
  out.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  out.append_অঙ্ক (vb_x);
  out.append_c (' ');
  out.append_অঙ্ক (vb_y);
  out.append_c (' ');
  out.append_অঙ্ক (vb_w);
  out.append_c (' ');
  out.append_অঙ্ক (vb_h);
  out.append_str ("\" width=\"");
  out.append_অঙ্ক (vb_w);
  out.append_str ("\" height=\"");
  out.append_অঙ্ক (vb_h);
  out.append_str ("\">\n");

  যদি (paint->defs.length)
  {
    out.append_str ("<defs>\n");
    out.append_len (paint->defs.arrayZ, paint->defs.length);
    out.append_str ("</defs>\n");
  }

  যদি (hb_color_get_alpha (paint->background))
  {
    out.append_str ("<rect x=\"");
    out.append_অঙ্ক (vb_x);
    out.append_str ("\" y=\"");
    out.append_অঙ্ক (vb_y);
    out.append_str ("\" width=\"");
    out.append_অঙ্ক (vb_w);
    out.append_str ("\" height=\"");
    out.append_অঙ্ক (vb_h);
    out.append_str ("\" fill=\"rgb(");
    out.append_unsigned (hb_color_get_red (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_green (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_blue (paint->background));
    out.append_str (")\"");
    যদি (hb_color_get_alpha (paint->background) < 255)
    {
      out.append_str (" fill-opacity=\"");
      out.append_অঙ্ক (hb_color_get_alpha (paint->background) / 255.f, 4);
      out.append_c ('"');
    }
    out.append_str ("/>\n");
  }

  out.append_str ("<g transform=\"scale(1,-1)\">\n");
  out.append_len (paint->group_স্তূপ.arrayZ[0].arrayZ, paint->group_স্তূপ.arrayZ[0].length);
  out.append_str ("</g>\n");

  out.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  প্রত্যাগম blob;
}
