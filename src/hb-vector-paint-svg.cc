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

#समावेश "hb.hh"

#समावेश "hb-vector-paint.hh"
#समावेश "hb-paint.hh"
#समावेश "hb-vector-path.hh"

#समावेश <math.h>

अटल स्थिर अक्षर *
hb_vector_svg_extend_mode_str (hb_paint_extend_t ext)
{
  चयन (ext)
  {
    हाल HB_PAINT_EXTEND_PAD: वापस "pad";
    हाल HB_PAINT_EXTEND_REPEAT: वापस "repeat";
    हाल HB_PAINT_EXTEND_REFLECT: वापस "reflect";
    शेष: वापस "pad";
  }
}

अटल व्योम
hb_vector_svg_emit_color_stops (hb_vector_paint_t *paint,
				hb_vector_buf_t *buf,
				hb_vector_t<hb_color_stop_t> *stops)
{
  क्रम (अचिन्हित i = 0; i < stops->लंबाई; i++)
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
    अगर (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_अंक (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

अटल स्थिर अक्षर *
hb_vector_svg_composite_mode_str (hb_paint_composite_mode_t mode)
{
  चयन (mode)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    हाल HB_PAINT_COMPOSITE_MODE_PLUS: वापस "screen";
    हाल HB_PAINT_COMPOSITE_MODE_XOR: वापस "difference";
    हाल HB_PAINT_COMPOSITE_MODE_CLEAR:
    हाल HB_PAINT_COMPOSITE_MODE_SRC:
    हाल HB_PAINT_COMPOSITE_MODE_DEST:
    हाल HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    हाल HB_PAINT_COMPOSITE_MODE_SRC_IN:
    हाल HB_PAINT_COMPOSITE_MODE_DEST_IN:
    हाल HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    हाल HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    हाल HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
    हाल HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      वापस nullptr;
    हाल HB_PAINT_COMPOSITE_MODE_SRC_OVER: वापस "normal";
    हाल HB_PAINT_COMPOSITE_MODE_SCREEN: वापस "screen";
    हाल HB_PAINT_COMPOSITE_MODE_OVERLAY: वापस "overlay";
    हाल HB_PAINT_COMPOSITE_MODE_DARKEN: वापस "darken";
    हाल HB_PAINT_COMPOSITE_MODE_LIGHTEN: वापस "lighten";
    हाल HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: वापस "color-dodge";
    हाल HB_PAINT_COMPOSITE_MODE_COLOR_BURN: वापस "color-burn";
    हाल HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: वापस "hard-light";
    हाल HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: वापस "soft-light";
    हाल HB_PAINT_COMPOSITE_MODE_DIFFERENCE: वापस "difference";
    हाल HB_PAINT_COMPOSITE_MODE_EXCLUSION: वापस "exclusion";
    हाल HB_PAINT_COMPOSITE_MODE_MULTIPLY: वापस "multiply";
    हाल HB_PAINT_COMPOSITE_MODE_HSL_HUE: वापस "hue";
    हाल HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: वापस "saturation";
    हाल HB_PAINT_COMPOSITE_MODE_HSL_COLOR: वापस "color";
    हाल HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: वापस "luminosity";
    शेष: वापस nullptr;
  }
}

काष्ठा hb_vector_svg_point_t { भग्न x, y; };
काष्ठा hb_vector_svg_rgba_t { भग्न r, g, b, a; };

अटल अंतरभूत भग्न
hb_vector_svg_lerp (भग्न a, भग्न b, भग्न t)
{ वापस a + (b - a) * t; }

अटल अंतरभूत भग्न
hb_vector_svg_clamp01 (भग्न v)
{
  अगर (v < 0.f) वापस 0.f;
  अगर (v > 1.f) वापस 1.f;
  वापस v;
}

अटल अंतरभूत hb_vector_svg_rgba_t
hb_vector_svg_rgba_from_hb_color (hb_color_t c)
{
  वापस {(भग्न) hb_color_get_red (c) / 255.f,
          (भग्न) hb_color_get_green (c) / 255.f,
          (भग्न) hb_color_get_blue (c) / 255.f,
          (भग्न) hb_color_get_alpha (c) / 255.f};
}

अटल अंतरभूत hb_color_t
hb_vector_svg_hb_color_from_rgba (स्थिर hb_vector_svg_rgba_t &c)
{
  अचिन्हित r = (अचिन्हित) roundf (hb_vector_svg_clamp01 (c.r) * 255.f);
  अचिन्हित g = (अचिन्हित) roundf (hb_vector_svg_clamp01 (c.g) * 255.f);
  अचिन्हित b = (अचिन्हित) roundf (hb_vector_svg_clamp01 (c.b) * 255.f);
  अचिन्हित a = (अचिन्हित) roundf (hb_vector_svg_clamp01 (c.a) * 255.f);
  वापस HB_COLOR (b, g, r, a);
}

अटल अंतरभूत hb_vector_svg_rgba_t
hb_vector_svg_lerp_rgba (स्थिर hb_vector_svg_rgba_t &c0,
			 स्थिर hb_vector_svg_rgba_t &c1,
			 भग्न t)
{
  वापस {hb_vector_svg_lerp (c0.r, c1.r, t),
          hb_vector_svg_lerp (c0.g, c1.g, t),
          hb_vector_svg_lerp (c0.b, c1.b, t),
          hb_vector_svg_lerp (c0.a, c1.a, t)};
}

अटल अंतरभूत भग्न hb_vector_svg_dot (स्थिर hb_vector_svg_point_t &p, स्थिर hb_vector_svg_point_t &q) { वापस p.x * q.x + p.y * q.y; }
अटल अंतरभूत hb_vector_svg_point_t hb_vector_svg_add (स्थिर hb_vector_svg_point_t &p, स्थिर hb_vector_svg_point_t &q) { वापस {p.x + q.x, p.y + q.y}; }
अटल अंतरभूत hb_vector_svg_point_t hb_vector_svg_sub (स्थिर hb_vector_svg_point_t &p, स्थिर hb_vector_svg_point_t &q) { वापस {p.x - q.x, p.y - q.y}; }
अटल अंतरभूत hb_vector_svg_point_t hb_vector_svg_scale (स्थिर hb_vector_svg_point_t &p, भग्न f) { वापस {p.x * f, p.y * f}; }

अटल अंतरभूत hb_vector_svg_point_t
hb_vector_svg_normalize (स्थिर hb_vector_svg_point_t &p)
{
  भग्न len = sqrtf (hb_vector_svg_dot (p, p));
  अगर (len == 0.f) वापस {0.f, 0.f};
  वापस hb_vector_svg_scale (p, 1.f / len);
}

अटल व्योम
hb_vector_svg_add_sweep_patch (hb_vector_buf_t *body,
			       अचिन्हित precision,
			       भग्न cx, भग्न cy, भग्न radius,
			       भग्न a0, स्थिर hb_vector_svg_rgba_t &c0_in,
			       भग्न a1, स्थिर hb_vector_svg_rgba_t &c1_in)
{
  अटल स्थिर भग्न max_angle = HB_PI / 16.f;
  hb_vector_svg_point_t center = {cx, cy};
  पूर्णांक अंक_splits = (पूर्णांक) ceilf (fabsf (a1 - a0) / max_angle);
  अगर (अंक_splits < 1) अंक_splits = 1;

  hb_vector_svg_point_t p0 = {cosf (a0), sinf (a0)};
  hb_vector_svg_rgba_t color0 = c0_in;

  क्रम (पूर्णांक a = 0; a < अंक_splits; a++)
  {
    भग्न k = (a + 1.f) / अंक_splits;
    भग्न angle1 = hb_vector_svg_lerp (a0, a1, k);
    hb_vector_svg_rgba_t color1 = hb_vector_svg_lerp_rgba (c0_in, c1_in, k);

    hb_vector_svg_point_t p1 = {cosf (angle1), sinf (angle1)};
    hb_vector_svg_point_t sp0 = hb_vector_svg_add (center, hb_vector_svg_scale (p0, radius));
    hb_vector_svg_point_t sp1 = hb_vector_svg_add (center, hb_vector_svg_scale (p1, radius));

    hb_vector_svg_point_t A = hb_vector_svg_normalize (hb_vector_svg_add (p0, p1));
    hb_vector_svg_point_t U = {-A.y, A.x};
    भग्न up0 = hb_vector_svg_dot (U, p0);
    भग्न up1 = hb_vector_svg_dot (U, p1);
    अगर (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      जारी;
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
    body->append_svg_color (mid, true);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
काष्ठा hb_vector_svg_sweep_ctx_t
{
  hb_vector_buf_t *body;
  अचिन्हित precision;
  भग्न cx, cy, radius;
};

अटल व्योम
hb_vector_svg_sweep_emit_patch (भग्न a0, hb_color_t c0,
				भग्न a1, hb_color_t c1,
				व्योम *user_तथ्य)
{
  स्वतः *ctx = (hb_vector_svg_sweep_ctx_t *) user_तथ्य;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  अगर (unlikely (ctx->body->in_error ()))
    वापस;
  hb_vector_svg_add_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_vector_svg_rgba_from_hb_color (c0),
				 a1, hb_vector_svg_rgba_from_hb_color (c1));
}


अटल व्योम hb_vector_paint_धकेलो_transform (hb_paint_funcs_t *, व्योम *,
                                            भग्न, भग्न, भग्न, भग्न, भग्न, भग्न,
                                            व्योम *);
अटल व्योम hb_vector_paint_निकालो_transform (hb_paint_funcs_t *, व्योम *, व्योम *);
अटल व्योम hb_vector_paint_fill_glyph (hb_paint_funcs_t *, व्योम *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, व्योम *);
अटल व्योम hb_vector_paint_धकेलो_clip_glyph (hb_paint_funcs_t *, व्योम *, hb_codepoint_t, hb_font_t *, व्योम *);
अटल व्योम hb_vector_paint_धकेलो_clip_rectangle (hb_paint_funcs_t *, व्योम *, भग्न, भग्न, भग्न, भग्न, व्योम *);
अटल hb_draw_funcs_t * hb_vector_paint_धकेलो_clip_path_start (hb_paint_funcs_t *, व्योम *, व्योम **, व्योम *);
अटल व्योम hb_vector_paint_धकेलो_clip_path_end (hb_paint_funcs_t *, व्योम *, व्योम *);
अटल व्योम hb_vector_paint_निकालो_clip (hb_paint_funcs_t *, व्योम *, व्योम *);
अटल व्योम hb_vector_paint_color (hb_paint_funcs_t *, व्योम *, hb_bool_t, hb_color_t, व्योम *);
अटल hb_bool_t hb_vector_paint_image (hb_paint_funcs_t *, व्योम *, hb_blob_t *, अचिन्हित, अचिन्हित, hb_tag_t, भग्न, hb_glyph_extents_t *, व्योम *);
अटल व्योम hb_vector_paint_linear_gradient (hb_paint_funcs_t *, व्योम *, hb_color_पंक्ति_t *, भग्न, भग्न, भग्न, भग्न, भग्न, भग्न, व्योम *);
अटल व्योम hb_vector_paint_radial_gradient (hb_paint_funcs_t *, व्योम *, hb_color_पंक्ति_t *, भग्न, भग्न, भग्न, भग्न, भग्न, भग्न, व्योम *);
अटल व्योम hb_vector_paint_sweep_gradient (hb_paint_funcs_t *, व्योम *, hb_color_पंक्ति_t *, भग्न, भग्न, भग्न, भग्न, व्योम *);
अटल व्योम hb_vector_paint_धकेलो_group (hb_paint_funcs_t *, व्योम *, व्योम *);
अटल व्योम hb_vector_paint_निकालो_group (hb_paint_funcs_t *, व्योम *, hb_paint_composite_mode_t, व्योम *);
अटल hb_bool_t hb_vector_paint_color_glyph (hb_paint_funcs_t *, व्योम *, hb_codepoint_t, hb_font_t *, व्योम *);
अटल hb_bool_t
hb_vector_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      व्योम *paint_तथ्य,
				      अचिन्हित color_सूचकांक,
				      hb_color_t *color,
				      व्योम *user_तथ्य HB_UNUSED)
{
  hb_vector_paint_t *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (!color)
    वापस false;

  hb_color_t *value = nullptr;
  अगर (!paint->custom_palette_colors.has (color_सूचकांक, &value) || !value)
    वापस false;
  *color = *value;
  वापस true;
}

अटल अंतरभूत व्योम free_static_vector_paint_funcs ();
अटल काष्ठा hb_vector_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_vector_paint_funcs_lazy_loader_t>
{
  अटल hb_paint_funcs_t *create ()
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
    वापस funcs;
  }
} static_vector_paint_funcs;

अटल अंतरभूत व्योम
free_static_vector_paint_funcs ()
{
  static_vector_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_svg_funcs_get ()
{
  वापस static_vector_paint_funcs.get_unconst ();
}


अटल व्योम
hb_vector_paint_धकेलो_transform (hb_paint_funcs_t *,
                                व्योम *paint_तथ्य,
                                भग्न xx, भग्न yx,
                                भग्न xy, भग्न yy,
                                भग्न dx, भग्न dy,
                                व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;

  अगर (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth++;
    वापस;
  }
  अगर (unlikely (paint->transform_group_depth >= 64))
  {
    paint->transform_group_overflow_depth = 1;
    वापस;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_group_open_mask = (paint->transform_group_open_mask << 1) | (opened ? 1ull : 0ull);
  paint->transform_group_depth++;

  अगर (!opened)
    वापस;

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

अटल व्योम
hb_vector_paint_निकालो_transform (hb_paint_funcs_t *,
                               व्योम *paint_तथ्य,
                               व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;
  अगर (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth--;
    वापस;
  }
  अगर (!paint->transform_group_depth)
    वापस;
  paint->transform_group_depth--;
  hb_bool_t opened = !!(paint->transform_group_open_mask & 1ull);
  paint->transform_group_open_mask >>= 1;
  अगर (opened)
    paint->current_body ().append_str ("</g>\n");
}

अटल व्योम
hb_vector_paint_fill_glyph (hb_paint_funcs_t *,
                            व्योम *paint_तथ्य,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;

  paint->path.सफा ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  अगर (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  स्वतः &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->path.arrayZ, paint->path.लंबाई);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, true);
  body.append_str ("\"/>\n");
}

अटल व्योम
hb_vector_paint_धकेलो_clip_glyph (hb_paint_funcs_t *,
                                 व्योम *paint_तथ्य,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;

  स्थिर अक्षर *pfx = paint->id_prefix;
  अचिन्हित pfx_len = paint->id_prefix_लंबाई;

  paint->path.सफा ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  अगर (likely (paint->budget_remaining >= 0))
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
  paint->defs.append_len (paint->path.arrayZ, paint->path.लंबाई);
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

अटल व्योम
hb_vector_paint_धकेलो_clip_rectangle (hb_paint_funcs_t *,
                                     व्योम *paint_तथ्य,
                                     भग्न xmin, भग्न ymin,
                                     भग्न xmax, भग्न ymax,
                                     व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;

  स्थिर अक्षर *pfx = paint->id_prefix;
  अचिन्हित pfx_len = paint->id_prefix_लंबाई;
  अचिन्हित clip_id = paint->clip_rect_गणक++;
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

अटल hb_draw_funcs_t *
hb_vector_paint_धकेलो_clip_path_start (hb_paint_funcs_t *,
                                      व्योम *paint_तथ्य,
                                      व्योम **draw_तथ्य,
                                      व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
  {
    *draw_तथ्य = nullptr;
    वापस nullptr;
  }

  paint->path.सफा ();
  paint->clip_path_sink = {&paint->path, paint->get_precision (),
			   paint->x_scale_factor,
			   paint->y_scale_factor,
			   &paint->budget_remaining};
  *draw_तथ्य = &paint->clip_path_sink;
  वापस hb_vector_svg_path_draw_funcs_get ();
}

अटल व्योम
hb_vector_paint_धकेलो_clip_path_end (hb_paint_funcs_t *,
                                    व्योम *paint_तथ्य,
                                    व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;

  स्थिर अक्षर *pfx = paint->id_prefix;
  अचिन्हित pfx_len = paint->id_prefix_लंबाई;
  अचिन्हित clip_id = paint->clip_path_गणक++;

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
  paint->defs.append_len (paint->path.arrayZ, paint->path.लंबाई);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("cp");
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

अटल व्योम
hb_vector_paint_निकालो_clip (hb_paint_funcs_t *,
                          व्योम *paint_तथ्य,
                          व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;
  paint->current_body ().append_str ("</g>\n");
}

अटल व्योम
hb_vector_paint_color (hb_paint_funcs_t *,
                       व्योम *paint_तथ्य,
                       hb_bool_t,
                       hb_color_t color,
                       व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;

  स्वतः &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, true);
  body.append_str ("\"/>\n");
}

अटल hb_bool_t
hb_vector_paint_image (hb_paint_funcs_t *,
                       व्योम *paint_तथ्य,
                       hb_blob_t *image,
                       अचिन्हित width,
                       अचिन्हित height,
                       hb_tag_t format,
                       भग्न slant HB_UNUSED,
                       hb_glyph_extents_t *extents,
                       व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस false;

  स्वतः &body = paint->current_body ();
  अगर (format == HB_TAG ('p','n','g',' '))
  {
    अगर (!extents || !width || !height)
      वापस false;

    अचिन्हित len = 0;
    स्थिर अक्षर *png_तथ्य = hb_blob_get_तथ्य (image, &len);
    अगर (!png_तथ्य || !len)
      वापस false;

    body.append_str ("<g transform=\"translate(");
    body.append_अंक (paint->sx ((भग्न) extents->x_bearing));
    body.append_c (',');
    body.append_अंक (paint->sy ((भग्न) extents->y_bearing));
    body.append_str (") scale(");
    body.append_अंक (paint->sx ((भग्न) extents->width) / width);
    body.append_c (',');
    body.append_अंक (paint->sy ((भग्न) extents->height) / height);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((स्थिर uint8_t *) png_तथ्य, len);
    body.append_str ("\" width=\"");
    body.append_अंक ((भग्न) width);
    body.append_str ("\" height=\"");
    body.append_अंक ((भग्न) height);
    body.append_str ("\"/>\n</g>\n");

    वापस true;
  }

  वापस false;
}

अटल व्योम
hb_vector_paint_linear_gradient (hb_paint_funcs_t *,
                                 व्योम *paint_तथ्य,
                                 hb_color_पंक्ति_t *color_पंक्ति,
                                 भग्न x0, भग्न y0,
                                 भग्न x1, भग्न y1,
                                 भग्न x2, भग्न y2,
                                 व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;

  अगर (!paint->fetch_color_stops (color_पंक्ति))
    वापस;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  भग्न mn, mx;
  hb_paint_normalize_color_पंक्ति (stops.arrayZ, stops.लंबाई, &mn, &mx);

  स्थिर अक्षर *pfx = paint->id_prefix;
  अचिन्हित pfx_len = paint->id_prefix_लंबाई;
  अचिन्हित grad_id = paint->gradient_गणक++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  भग्न lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  भग्न gx0 = lx0 + mn * (lx1 - lx0);
  भग्न gy0 = ly0 + mn * (ly1 - ly0);
  भग्न gx1 = lx0 + mx * (lx1 - lx0);
  भग्न gy1 = ly0 + mx * (ly1 - ly0);

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

अटल व्योम
hb_vector_paint_radial_gradient (hb_paint_funcs_t *,
                                 व्योम *paint_तथ्य,
                                 hb_color_पंक्ति_t *color_पंक्ति,
                                 भग्न x0, भग्न y0, भग्न r0,
                                 भग्न x1, भग्न y1, भग्न r1,
                                 व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;

  अगर (!paint->fetch_color_stops (color_पंक्ति))
    वापस;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  भग्न mn, mx;
  hb_paint_normalize_color_पंक्ति (stops.arrayZ, stops.लंबाई, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  भग्न gx0 = x0 + mn * (x1 - x0);
  भग्न gy0 = y0 + mn * (y1 - y0);
  भग्न gr0 = r0 + mn * (r1 - r0);
  भग्न gx1 = x0 + mx * (x1 - x0);
  भग्न gy1 = y0 + mx * (y1 - y0);
  भग्न gr1 = r0 + mx * (r1 - r0);

  स्थिर अक्षर *pfx = paint->id_prefix;
  अचिन्हित pfx_len = paint->id_prefix_लंबाई;
  अचिन्हित grad_id = paint->gradient_गणक++;

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
  अगर (gr0 > 0)
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

अटल व्योम
hb_vector_paint_sweep_gradient (hb_paint_funcs_t *,
                                व्योम *paint_तथ्य,
                                hb_color_पंक्ति_t *color_पंक्ति,
                                भग्न cx, भग्न cy,
                                भग्न start_angle, भग्न end_angle,
                                व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;

  अगर (!paint->fetch_color_stops (color_पंक्ति))
    वापस;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  भग्न mn, mx;
  hb_paint_normalize_color_पंक्ति (stops.arrayZ, stops.लंबाई, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  भग्न ga0 = start_angle + mn * (end_angle - start_angle);
  भग्न ga1 = start_angle + mx * (end_angle - start_angle);

  hb_vector_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.लंबाई,
				 hb_color_पंक्ति_get_extend (color_पंक्ति),
				 ga0, ga1,
				 hb_vector_svg_sweep_emit_patch,
				 &ctx);
}

अटल व्योम
hb_vector_paint_धकेलो_group (hb_paint_funcs_t *,
                            व्योम *paint_तथ्य,
                            व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;
  अगर (unlikely (!paint->group_ढेर.धकेलो_or_fail (hb_vector_buf_t {})))
    वापस;
}

अटल व्योम
hb_vector_paint_निकालो_group (hb_paint_funcs_t *,
                           व्योम *paint_तथ्य,
                           hb_paint_composite_mode_t mode,
                           व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस;
  अगर (paint->group_ढेर.लंबाई < 2)
    वापस;

  hb_vector_buf_t group = paint->group_ढेर.निकालो ();
  स्वतः &body = paint->current_body ();

  स्थिर अक्षर *blend = hb_vector_svg_composite_mode_str (mode);
  अगर (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (group.arrayZ, group.लंबाई);
    body.append_str ("</g>\n");
  }
  अन्यथा
    body.append_len (group.arrayZ, group.लंबाई);
}

अटल hb_bool_t
hb_vector_paint_color_glyph (hb_paint_funcs_t *,
                             व्योम *paint_तथ्य,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             व्योम *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_तथ्य;
  अगर (unlikely (!paint->ensure_initialized ()))
    वापस false;
  अगर (unlikely (paint->color_glyph_depth >= HB_MAX_NESTING_LEVEL))
    वापस false;
  अगर (unlikely (hb_set_has (paint->active_color_glyphs, glyph)))
    वापस false;

  paint->color_glyph_depth++;
  hb_set_add (paint->active_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_vector_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_set_del (paint->active_color_glyphs, glyph);
  paint->color_glyph_depth--;
  वापस true;
}




hb_blob_t *
hb_vector_paint_render_svg (hb_vector_paint_t *paint)
{
  अगर (!paint->has_extents)
    वापस nullptr;

  अगर (unlikely (!paint->ensure_initialized ()))
    वापस nullptr;

  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  अचिन्हित estimated = paint->defs.लंबाई +
		       paint->group_ढेर.arrayZ[0].लंबाई +
		       320;
  out.alloc (estimated);
  भग्न vb_x = paint->extents.x;
  भग्न vb_y = -(paint->extents.y + paint->extents.height);
  भग्न vb_w = paint->extents.width;
  भग्न vb_h = paint->extents.height;
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

  अगर (paint->defs.लंबाई)
  {
    out.append_str ("<defs>\n");
    out.append_len (paint->defs.arrayZ, paint->defs.लंबाई);
    out.append_str ("</defs>\n");
  }

  अगर (hb_color_get_alpha (paint->background))
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
    अगर (hb_color_get_alpha (paint->background) < 255)
    {
      out.append_str (" fill-opacity=\"");
      out.append_अंक (hb_color_get_alpha (paint->background) / 255.f, 4);
      out.append_c ('"');
    }
    out.append_str ("/>\n");
  }

  out.append_str ("<g transform=\"scale(1,-1)\">\n");
  out.append_len (paint->group_ढेर.arrayZ[0].arrayZ, paint->group_ढेर.arrayZ[0].लंबाई);
  out.append_str ("</g>\n");

  out.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  वापस blob;
}
