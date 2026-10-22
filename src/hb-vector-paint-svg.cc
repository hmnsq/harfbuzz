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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-vector-paint.hh"
#अन्तर्भाव "hb-paint.hh"
#अन्तर्भाव "hb-vector-path.hh"

#अन्तर्भाव <math.h>

स्थिर ध्रुव वर्ण *
hb_vector_svg_extend_mode_str (hb_paint_extend_t ext)
{
  विकल्प (ext)
  {
    पक्ष HB_PAINT_EXTEND_PAD: प्रत्यागम "pad";
    पक्ष HB_PAINT_EXTEND_REPEAT: प्रत्यागम "repeat";
    पक्ष HB_PAINT_EXTEND_REFLECT: प्रत्यागम "reflect";
    स्वभाव: प्रत्यागम "pad";
  }
}

स्थिर शून्य
hb_vector_svg_emit_color_stops (hb_vector_paint_t *paint,
				hb_vector_buf_t *buf,
				hb_vector_t<hb_color_stop_t> *stops)
{
  प्रति (अचिन्हित i = 0; i < stops->length; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_अंक (stops->arrayZ[i].offset, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    यदि (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_अंक (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

स्थिर ध्रुव वर्ण *
hb_vector_svg_composite_mode_str (hb_paint_composite_mode_t mode)
{
  विकल्प (mode)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    पक्ष HB_PAINT_COMPOSITE_MODE_PLUS: प्रत्यागम "screen";
    पक्ष HB_PAINT_COMPOSITE_MODE_XOR: प्रत्यागम "difference";
    पक्ष HB_PAINT_COMPOSITE_MODE_CLEAR:
    पक्ष HB_PAINT_COMPOSITE_MODE_SRC:
    पक्ष HB_PAINT_COMPOSITE_MODE_DEST:
    पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    पक्ष HB_PAINT_COMPOSITE_MODE_SRC_IN:
    पक्ष HB_PAINT_COMPOSITE_MODE_DEST_IN:
    पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    पक्ष HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
    पक्ष HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      प्रत्यागम nullptr;
    पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OVER: प्रत्यागम "normal";
    पक्ष HB_PAINT_COMPOSITE_MODE_SCREEN: प्रत्यागम "screen";
    पक्ष HB_PAINT_COMPOSITE_MODE_OVERLAY: प्रत्यागम "overlay";
    पक्ष HB_PAINT_COMPOSITE_MODE_DARKEN: प्रत्यागम "darken";
    पक्ष HB_PAINT_COMPOSITE_MODE_LIGHTEN: प्रत्यागम "lighten";
    पक्ष HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: प्रत्यागम "color-dodge";
    पक्ष HB_PAINT_COMPOSITE_MODE_COLOR_BURN: प्रत्यागम "color-burn";
    पक्ष HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: प्रत्यागम "hard-light";
    पक्ष HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: प्रत्यागम "soft-light";
    पक्ष HB_PAINT_COMPOSITE_MODE_DIFFERENCE: प्रत्यागम "difference";
    पक्ष HB_PAINT_COMPOSITE_MODE_EXCLUSION: प्रत्यागम "exclusion";
    पक्ष HB_PAINT_COMPOSITE_MODE_MULTIPLY: प्रत्यागम "multiply";
    पक्ष HB_PAINT_COMPOSITE_MODE_HSL_HUE: प्रत्यागम "hue";
    पक्ष HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: प्रत्यागम "saturation";
    पक्ष HB_PAINT_COMPOSITE_MODE_HSL_COLOR: प्रत्यागम "color";
    पक्ष HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: प्रत्यागम "luminosity";
    स्वभाव: प्रत्यागम nullptr;
  }
}

संरचना hb_vector_svg_point_t { भिन्न x, y; };
संरचना hb_vector_svg_rgba_t { भिन्न r, g, b, a; };

स्थिर अन्तःस्थ भिन्न
hb_vector_svg_lerp (भिन्न a, भिन्न b, भिन्न t)
{ प्रत्यागम a + (b - a) * t; }

स्थिर अन्तःस्थ भिन्न
hb_vector_svg_clamp01 (भिन्न v)
{
  यदि (v < 0.f) प्रत्यागम 0.f;
  यदि (v > 1.f) प्रत्यागम 1.f;
  प्रत्यागम v;
}

स्थिर अन्तःस्थ hb_vector_svg_rgba_t
hb_vector_svg_rgba_from_hb_color (hb_color_t c)
{
  प्रत्यागम {(भिन्न) hb_color_get_red (c) / 255.f,
          (भिन्न) hb_color_get_green (c) / 255.f,
          (भिन्न) hb_color_get_blue (c) / 255.f,
          (भिन्न) hb_color_get_alpha (c) / 255.f};
}

स्थिर अन्तःस्थ hb_color_t
hb_vector_svg_hb_color_from_rgba (ध्रुव hb_vector_svg_rgba_t &c)
{
  अचिन्हित r = (अचिन्हित) roundf (hb_vector_svg_clamp01 (c.r) * 255.f);
  अचिन्हित g = (अचिन्हित) roundf (hb_vector_svg_clamp01 (c.g) * 255.f);
  अचिन्हित b = (अचिन्हित) roundf (hb_vector_svg_clamp01 (c.b) * 255.f);
  अचिन्हित a = (अचिन्हित) roundf (hb_vector_svg_clamp01 (c.a) * 255.f);
  प्रत्यागम HB_COLOR (b, g, r, a);
}

स्थिर अन्तःस्थ hb_vector_svg_rgba_t
hb_vector_svg_lerp_rgba (ध्रुव hb_vector_svg_rgba_t &c0,
			 ध्रुव hb_vector_svg_rgba_t &c1,
			 भिन्न t)
{
  प्रत्यागम {hb_vector_svg_lerp (c0.r, c1.r, t),
          hb_vector_svg_lerp (c0.g, c1.g, t),
          hb_vector_svg_lerp (c0.b, c1.b, t),
          hb_vector_svg_lerp (c0.a, c1.a, t)};
}

स्थिर अन्तःस्थ भिन्न hb_vector_svg_dot (ध्रुव hb_vector_svg_point_t &p, ध्रुव hb_vector_svg_point_t &q) { प्रत्यागम p.x * q.x + p.y * q.y; }
स्थिर अन्तःस्थ hb_vector_svg_point_t hb_vector_svg_add (ध्रुव hb_vector_svg_point_t &p, ध्रुव hb_vector_svg_point_t &q) { प्रत्यागम {p.x + q.x, p.y + q.y}; }
स्थिर अन्तःस्थ hb_vector_svg_point_t hb_vector_svg_sub (ध्रुव hb_vector_svg_point_t &p, ध्रुव hb_vector_svg_point_t &q) { प्रत्यागम {p.x - q.x, p.y - q.y}; }
स्थिर अन्तःस्थ hb_vector_svg_point_t hb_vector_svg_scale (ध्रुव hb_vector_svg_point_t &p, भिन्न f) { प्रत्यागम {p.x * f, p.y * f}; }

स्थिर अन्तःस्थ hb_vector_svg_point_t
hb_vector_svg_normalize (ध्रुव hb_vector_svg_point_t &p)
{
  भिन्न len = sqrtf (hb_vector_svg_dot (p, p));
  यदि (len == 0.f) प्रत्यागम {0.f, 0.f};
  प्रत्यागम hb_vector_svg_scale (p, 1.f / len);
}

स्थिर शून्य
hb_vector_svg_add_sweep_patch (hb_vector_buf_t *body,
			       अचिन्हित precision,
			       भिन्न cx, भिन्न cy, भिन्न radius,
			       भिन्न a0, ध्रुव hb_vector_svg_rgba_t &c0_in,
			       भिन्न a1, ध्रुव hb_vector_svg_rgba_t &c1_in)
{
  स्थिर ध्रुव भिन्न max_angle = HB_PI / 16.f;
  hb_vector_svg_point_t center = {cx, cy};
  पूर्णाङ्क अंक_splits = (पूर्णाङ्क) ceilf (fabsf (a1 - a0) / max_angle);
  यदि (अंक_splits < 1) अंक_splits = 1;

  hb_vector_svg_point_t p0 = {cosf (a0), sinf (a0)};
  hb_vector_svg_rgba_t color0 = c0_in;

  प्रति (पूर्णाङ्क a = 0; a < अंक_splits; a++)
  {
    भिन्न k = (a + 1.f) / अंक_splits;
    भिन्न angle1 = hb_vector_svg_lerp (a0, a1, k);
    hb_vector_svg_rgba_t color1 = hb_vector_svg_lerp_rgba (c0_in, c1_in, k);

    hb_vector_svg_point_t p1 = {cosf (angle1), sinf (angle1)};
    hb_vector_svg_point_t sp0 = hb_vector_svg_add (center, hb_vector_svg_scale (p0, radius));
    hb_vector_svg_point_t sp1 = hb_vector_svg_add (center, hb_vector_svg_scale (p1, radius));

    hb_vector_svg_point_t A = hb_vector_svg_normalize (hb_vector_svg_add (p0, p1));
    hb_vector_svg_point_t U = {-A.y, A.x};
    भिन्न up0 = hb_vector_svg_dot (U, p0);
    भिन्न up1 = hb_vector_svg_dot (U, p1);
    यदि (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      अनुवर्त;
    }
    hb_vector_svg_point_t C0 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p0, A), p0) / up0));
    hb_vector_svg_point_t C1 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p1, A), p1) / up1));

    hb_vector_svg_point_t sc0 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C0, hb_vector_svg_scale (hb_vector_svg_sub (C0, p0), 0.33333f)), radius));
    hb_vector_svg_point_t sc1 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C1, hb_vector_svg_scale (hb_vector_svg_sub (C1, p1), 0.33333f)), radius));

    hb_vector_svg_rgba_t mid_color = hb_vector_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_vector_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_अंक (center.x, precision);
    body->append_c (',');
    body->append_अंक (center.y, precision);
    body->append_str ("L");
    body->append_अंक (sp0.x, precision);
    body->append_c (',');
    body->append_अंक (sp0.y, precision);
    body->append_str ("C");
    body->append_अंक (sc0.x, precision);
    body->append_c (',');
    body->append_अंक (sc0.y, precision);
    body->append_c (' ');
    body->append_अंक (sc1.x, precision);
    body->append_c (',');
    body->append_अंक (sc1.y, precision);
    body->append_c (' ');
    body->append_अंक (sp1.x, precision);
    body->append_c (',');
    body->append_अंक (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, सत्य);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
संरचना hb_vector_svg_sweep_ctx_t
{
  hb_vector_buf_t *body;
  अचिन्हित precision;
  भिन्न cx, cy, radius;
};

स्थिर शून्य
hb_vector_svg_sweep_emit_patch (भिन्न a0, hb_color_t c0,
				भिन्न a1, hb_color_t c1,
				शून्य *user_data)
{
  स्वतः *ctx = (hb_vector_svg_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  यदि (unlikely (ctx->body->in_error ()))
    प्रत्यागम;
  hb_vector_svg_add_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_vector_svg_rgba_from_hb_color (c0),
				 a1, hb_vector_svg_rgba_from_hb_color (c1));
}


स्थिर शून्य hb_vector_paint_धकेलो_transform (hb_paint_funcs_t *, शून्य *,
                                            भिन्न, भिन्न, भिन्न, भिन्न, भिन्न, भिन्न,
                                            शून्य *);
स्थिर शून्य hb_vector_paint_निकालो_transform (hb_paint_funcs_t *, शून्य *, शून्य *);
स्थिर शून्य hb_vector_paint_fill_glyph (hb_paint_funcs_t *, शून्य *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, शून्य *);
स्थिर शून्य hb_vector_paint_धकेलो_clip_glyph (hb_paint_funcs_t *, शून्य *, hb_codepoint_t, hb_font_t *, शून्य *);
स्थिर शून्य hb_vector_paint_धकेलो_clip_rectangle (hb_paint_funcs_t *, शून्य *, भिन्न, भिन्न, भिन्न, भिन्न, शून्य *);
स्थिर hb_draw_funcs_t * hb_vector_paint_धकेलो_clip_path_start (hb_paint_funcs_t *, शून्य *, शून्य **, शून्य *);
स्थिर शून्य hb_vector_paint_धकेलो_clip_path_end (hb_paint_funcs_t *, शून्य *, शून्य *);
स्थिर शून्य hb_vector_paint_निकालो_clip (hb_paint_funcs_t *, शून्य *, शून्य *);
स्थिर शून्य hb_vector_paint_color (hb_paint_funcs_t *, शून्य *, hb_bool_t, hb_color_t, शून्य *);
स्थिर hb_bool_t hb_vector_paint_image (hb_paint_funcs_t *, शून्य *, hb_blob_t *, अचिन्हित, अचिन्हित, hb_tag_t, भिन्न, hb_glyph_extents_t *, शून्य *);
स्थिर शून्य hb_vector_paint_linear_gradient (hb_paint_funcs_t *, शून्य *, hb_color_पंक्ति_t *, भिन्न, भिन्न, भिन्न, भिन्न, भिन्न, भिन्न, शून्य *);
स्थिर शून्य hb_vector_paint_radial_gradient (hb_paint_funcs_t *, शून्य *, hb_color_पंक्ति_t *, भिन्न, भिन्न, भिन्न, भिन्न, भिन्न, भिन्न, शून्य *);
स्थिर शून्य hb_vector_paint_sweep_gradient (hb_paint_funcs_t *, शून्य *, hb_color_पंक्ति_t *, भिन्न, भिन्न, भिन्न, भिन्न, शून्य *);
स्थिर शून्य hb_vector_paint_धकेलो_group (hb_paint_funcs_t *, शून्य *, शून्य *);
स्थिर शून्य hb_vector_paint_निकालो_group (hb_paint_funcs_t *, शून्य *, hb_paint_composite_mode_t, शून्य *);
स्थिर hb_bool_t hb_vector_paint_color_glyph (hb_paint_funcs_t *, शून्य *, hb_codepoint_t, hb_font_t *, शून्य *);
स्थिर hb_bool_t
hb_vector_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      शून्य *paint_data,
				      अचिन्हित color_index,
				      hb_color_t *color,
				      शून्य *user_data HB_UNUSED)
{
  hb_vector_paint_t *paint = (hb_vector_paint_t *) paint_data;
  यदि (!color)
    प्रत्यागम असत्य;

  hb_color_t *value = nullptr;
  यदि (!paint->custom_palette_colors.has (color_index, &value) || !value)
    प्रत्यागम असत्य;
  *color = *value;
  प्रत्यागम सत्य;
}

स्थिर अन्तःस्थ शून्य free_static_vector_paint_funcs ();
स्थिर संरचना hb_vector_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_vector_paint_funcs_lazy_loader_t>
{
  स्थिर hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_धकेलो_transform_func (funcs, (hb_paint_धकेलो_transform_func_t) hb_vector_paint_धकेलो_transform, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_transform_func (funcs, (hb_paint_निकालो_transform_func_t) hb_vector_paint_निकालो_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_vector_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_glyph_func (funcs, (hb_paint_धकेलो_clip_glyph_func_t) hb_vector_paint_धकेलो_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_rectangle_func (funcs, (hb_paint_धकेलो_clip_rectangle_func_t) hb_vector_paint_धकेलो_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_path_start_func (funcs, (hb_paint_धकेलो_clip_path_start_func_t) hb_vector_paint_धकेलो_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_path_end_func (funcs, (hb_paint_धकेलो_clip_path_end_func_t) hb_vector_paint_धकेलो_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_clip_func (funcs, (hb_paint_निकालो_clip_func_t) hb_vector_paint_निकालो_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_vector_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_vector_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_vector_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_vector_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_vector_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_group_func (funcs, (hb_paint_धकेलो_group_func_t) hb_vector_paint_धकेलो_group, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_group_func (funcs, (hb_paint_निकालो_group_func_t) hb_vector_paint_निकालो_group, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_vector_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_vector_paint_custom_palette_color, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_paint_funcs);
    प्रत्यागम funcs;
  }
} static_vector_paint_funcs;

स्थिर अन्तःस्थ शून्य
free_static_vector_paint_funcs ()
{
  static_vector_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_svg_funcs_get ()
{
  प्रत्यागम static_vector_paint_funcs.get_unconst ();
}


स्थिर शून्य
hb_vector_paint_धकेलो_transform (hb_paint_funcs_t *,
                                शून्य *paint_data,
                                भिन्न xx, भिन्न yx,
                                भिन्न xy, भिन्न yy,
                                भिन्न dx, भिन्न dy,
                                शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  यदि (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth++;
    प्रत्यागम;
  }
  यदि (unlikely (paint->transform_group_depth >= 64))
  {
    paint->transform_group_overflow_depth = 1;
    प्रत्यागम;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_group_open_mask = (paint->transform_group_open_mask << 1) | (opened ? 1ull : 0ull);
  paint->transform_group_depth++;

  यदि (!opened)
    प्रत्यागम;

  स्वतः &body = paint->current_body ();
  अचिन्हित sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_अंक (xx, sprec);
  body.append_c (',');
  body.append_अंक (yx, sprec);
  body.append_c (',');
  body.append_अंक (xy, sprec);
  body.append_c (',');
  body.append_अंक (yy, sprec);
  body.append_c (',');
  body.append_अंक (paint->sx (dx));
  body.append_c (',');
  body.append_अंक (paint->sy (dy));
  body.append_str (")\">\n");
}

स्थिर शून्य
hb_vector_paint_निकालो_transform (hb_paint_funcs_t *,
                               शून्य *paint_data,
                               शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  यदि (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth--;
    प्रत्यागम;
  }
  यदि (!paint->transform_group_depth)
    प्रत्यागम;
  paint->transform_group_depth--;
  hb_bool_t opened = !!(paint->transform_group_open_mask & 1ull);
  paint->transform_group_open_mask >>= 1;
  यदि (opened)
    paint->current_body ().append_str ("</g>\n");
}

स्थिर शून्य
hb_vector_paint_fill_glyph (hb_paint_funcs_t *,
                            शून्य *paint_data,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  यदि (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  स्वतः &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->path.arrayZ, paint->path.length);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, सत्य);
  body.append_str ("\"/>\n");
}

स्थिर शून्य
hb_vector_paint_धकेलो_clip_glyph (hb_paint_funcs_t *,
                                 शून्य *paint_data,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  ध्रुव वर्ण *pfx = paint->id_prefix;
  अचिन्हित pfx_len = paint->id_prefix_length;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  यदि (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  अचिन्हित def_id = paint->path_def_गिनती++;
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

स्थिर शून्य
hb_vector_paint_धकेलो_clip_rectangle (hb_paint_funcs_t *,
                                     शून्य *paint_data,
                                     भिन्न xmin, भिन्न ymin,
                                     भिन्न xmax, भिन्न ymax,
                                     शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  ध्रुव वर्ण *pfx = paint->id_prefix;
  अचिन्हित pfx_len = paint->id_prefix_length;
  अचिन्हित clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_अंक (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_अंक (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_अंक (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_अंक (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

स्थिर hb_draw_funcs_t *
hb_vector_paint_धकेलो_clip_path_start (hb_paint_funcs_t *,
                                      शून्य *paint_data,
                                      शून्य **draw_data,
                                      शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    प्रत्यागम nullptr;
  }

  paint->path.clear ();
  paint->clip_path_sink = {&paint->path, paint->get_precision (),
			   paint->x_scale_factor,
			   paint->y_scale_factor,
			   &paint->budget_remaining};
  *draw_data = &paint->clip_path_sink;
  प्रत्यागम hb_vector_svg_path_draw_funcs_get ();
}

स्थिर शून्य
hb_vector_paint_धकेलो_clip_path_end (hb_paint_funcs_t *,
                                    शून्य *paint_data,
                                    शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  ध्रुव वर्ण *pfx = paint->id_prefix;
  अचिन्हित pfx_len = paint->id_prefix_length;
  अचिन्हित clip_id = paint->clip_path_counter++;

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

स्थिर शून्य
hb_vector_paint_निकालो_clip (hb_paint_funcs_t *,
                          शून्य *paint_data,
                          शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  paint->current_body ().append_str ("</g>\n");
}

स्थिर शून्य
hb_vector_paint_color (hb_paint_funcs_t *,
                       शून्य *paint_data,
                       hb_bool_t,
                       hb_color_t color,
                       शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  स्वतः &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, सत्य);
  body.append_str ("\"/>\n");
}

स्थिर hb_bool_t
hb_vector_paint_image (hb_paint_funcs_t *,
                       शून्य *paint_data,
                       hb_blob_t *image,
                       अचिन्हित width,
                       अचिन्हित height,
                       hb_tag_t format,
                       भिन्न slant HB_UNUSED,
                       hb_glyph_extents_t *extents,
                       शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम असत्य;

  स्वतः &body = paint->current_body ();
  यदि (format == HB_TAG ('p','n','g',' '))
  {
    यदि (!extents || !width || !height)
      प्रत्यागम असत्य;

    अचिन्हित len = 0;
    ध्रुव वर्ण *png_data = hb_blob_get_data (image, &len);
    यदि (!png_data || !len)
      प्रत्यागम असत्य;

    body.append_str ("<g transform=\"translate(");
    body.append_अंक (paint->sx ((भिन्न) extents->x_bearing));
    body.append_c (',');
    body.append_अंक (paint->sy ((भिन्न) extents->y_bearing));
    body.append_str (") scale(");
    body.append_अंक (paint->sx ((भिन्न) extents->width) / width);
    body.append_c (',');
    body.append_अंक (paint->sy ((भिन्न) extents->height) / height);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ध्रुव uint8_t *) png_data, len);
    body.append_str ("\" width=\"");
    body.append_अंक ((भिन्न) width);
    body.append_str ("\" height=\"");
    body.append_अंक ((भिन्न) height);
    body.append_str ("\"/>\n</g>\n");

    प्रत्यागम सत्य;
  }

  प्रत्यागम असत्य;
}

स्थिर शून्य
hb_vector_paint_linear_gradient (hb_paint_funcs_t *,
                                 शून्य *paint_data,
                                 hb_color_पंक्ति_t *color_पंक्ति,
                                 भिन्न x0, भिन्न y0,
                                 भिन्न x1, भिन्न y1,
                                 भिन्न x2, भिन्न y2,
                                 शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  यदि (!paint->fetch_color_stops (color_पंक्ति))
    प्रत्यागम;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  भिन्न mn, mx;
  hb_paint_normalize_color_पंक्ति (stops.arrayZ, stops.length, &mn, &mx);

  ध्रुव वर्ण *pfx = paint->id_prefix;
  अचिन्हित pfx_len = paint->id_prefix_length;
  अचिन्हित grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  भिन्न lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  भिन्न gx0 = lx0 + mn * (lx1 - lx0);
  भिन्न gy0 = ly0 + mn * (ly1 - ly0);
  भिन्न gx1 = lx0 + mx * (lx1 - lx0);
  भिन्न gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_अंक (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_अंक (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_अंक (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_अंक (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_पंक्ति_get_extend (color_पंक्ति)));
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

स्थिर शून्य
hb_vector_paint_radial_gradient (hb_paint_funcs_t *,
                                 शून्य *paint_data,
                                 hb_color_पंक्ति_t *color_पंक्ति,
                                 भिन्न x0, भिन्न y0, भिन्न r0,
                                 भिन्न x1, भिन्न y1, भिन्न r1,
                                 शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  यदि (!paint->fetch_color_stops (color_पंक्ति))
    प्रत्यागम;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  भिन्न mn, mx;
  hb_paint_normalize_color_पंक्ति (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  भिन्न gx0 = x0 + mn * (x1 - x0);
  भिन्न gy0 = y0 + mn * (y1 - y0);
  भिन्न gr0 = r0 + mn * (r1 - r0);
  भिन्न gx1 = x0 + mx * (x1 - x0);
  भिन्न gy1 = y0 + mx * (y1 - y0);
  भिन्न gr1 = r0 + mx * (r1 - r0);

  ध्रुव वर्ण *pfx = paint->id_prefix;
  अचिन्हित pfx_len = paint->id_prefix_length;
  अचिन्हित grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_अंक (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_अंक (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_अंक (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_अंक (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_अंक (paint->sy (gy0));
  यदि (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_अंक (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_पंक्ति_get_extend (color_पंक्ति)));
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

स्थिर शून्य
hb_vector_paint_sweep_gradient (hb_paint_funcs_t *,
                                शून्य *paint_data,
                                hb_color_पंक्ति_t *color_पंक्ति,
                                भिन्न cx, भिन्न cy,
                                भिन्न start_angle, भिन्न end_angle,
                                शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  यदि (!paint->fetch_color_stops (color_पंक्ति))
    प्रत्यागम;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  भिन्न mn, mx;
  hb_paint_normalize_color_पंक्ति (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  भिन्न ga0 = start_angle + mn * (end_angle - start_angle);
  भिन्न ga1 = start_angle + mx * (end_angle - start_angle);

  hb_vector_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length,
				 hb_color_पंक्ति_get_extend (color_पंक्ति),
				 ga0, ga1,
				 hb_vector_svg_sweep_emit_patch,
				 &ctx);
}

स्थिर शून्य
hb_vector_paint_धकेलो_group (hb_paint_funcs_t *,
                            शून्य *paint_data,
                            शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  यदि (unlikely (!paint->group_ढेर.धकेलो_or_fail (hb_vector_buf_t {})))
    प्रत्यागम;
}

स्थिर शून्य
hb_vector_paint_निकालो_group (hb_paint_funcs_t *,
                           शून्य *paint_data,
                           hb_paint_composite_mode_t mode,
                           शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  यदि (paint->group_ढेर.length < 2)
    प्रत्यागम;

  hb_vector_buf_t group = paint->group_ढेर.निकालो ();
  स्वतः &body = paint->current_body ();

  ध्रुव वर्ण *blend = hb_vector_svg_composite_mode_str (mode);
  यदि (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (group.arrayZ, group.length);
    body.append_str ("</g>\n");
  }
  अन्यथा
    body.append_len (group.arrayZ, group.length);
}

स्थिर hb_bool_t
hb_vector_paint_color_glyph (hb_paint_funcs_t *,
                             शून्य *paint_data,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम असत्य;
  यदि (unlikely (paint->color_glyph_depth >= HB_MAX_NESTING_LEVEL))
    प्रत्यागम असत्य;
  यदि (unlikely (hb_set_has (paint->active_color_glyphs, glyph)))
    प्रत्यागम असत्य;

  paint->color_glyph_depth++;
  hb_set_add (paint->active_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_vector_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_set_del (paint->active_color_glyphs, glyph);
  paint->color_glyph_depth--;
  प्रत्यागम सत्य;
}




hb_blob_t *
hb_vector_paint_render_svg (hb_vector_paint_t *paint)
{
  यदि (!paint->has_extents)
    प्रत्यागम nullptr;

  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम nullptr;

  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  अचिन्हित estimated = paint->defs.length +
		       paint->group_ढेर.arrayZ[0].length +
		       320;
  out.alloc (estimated);
  भिन्न vb_x = paint->extents.x;
  भिन्न vb_y = -(paint->extents.y + paint->extents.height);
  भिन्न vb_w = paint->extents.width;
  भिन्न vb_h = paint->extents.height;
  out.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  out.append_अंक (vb_x);
  out.append_c (' ');
  out.append_अंक (vb_y);
  out.append_c (' ');
  out.append_अंक (vb_w);
  out.append_c (' ');
  out.append_अंक (vb_h);
  out.append_str ("\" width=\"");
  out.append_अंक (vb_w);
  out.append_str ("\" height=\"");
  out.append_अंक (vb_h);
  out.append_str ("\">\n");

  यदि (paint->defs.length)
  {
    out.append_str ("<defs>\n");
    out.append_len (paint->defs.arrayZ, paint->defs.length);
    out.append_str ("</defs>\n");
  }

  यदि (hb_color_get_alpha (paint->background))
  {
    out.append_str ("<rect x=\"");
    out.append_अंक (vb_x);
    out.append_str ("\" y=\"");
    out.append_अंक (vb_y);
    out.append_str ("\" width=\"");
    out.append_अंक (vb_w);
    out.append_str ("\" height=\"");
    out.append_अंक (vb_h);
    out.append_str ("\" fill=\"rgb(");
    out.append_unsigned (hb_color_get_red (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_green (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_blue (paint->background));
    out.append_str (")\"");
    यदि (hb_color_get_alpha (paint->background) < 255)
    {
      out.append_str (" fill-opacity=\"");
      out.append_अंक (hb_color_get_alpha (paint->background) / 255.f, 4);
      out.append_c ('"');
    }
    out.append_str ("/>\n");
  }

  out.append_str ("<g transform=\"scale(1,-1)\">\n");
  out.append_len (paint->group_ढेर.arrayZ[0].arrayZ, paint->group_ढेर.arrayZ[0].length);
  out.append_str ("</g>\n");

  out.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  प्रत्यागम blob;
}
