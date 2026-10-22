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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-vector-paint.hh"
#અન્તર્ભાવ "hb-paint.hh"
#અન્તર્ભાવ "hb-vector-path.hh"

#અન્તર્ભાવ <math.h>

સ્થિર અચલ અક્ષર *
hb_vector_svg_extend_mode_str (hb_paint_extend_t ext)
{
  પસંદગી (ext)
  {
    સ્થિતિ HB_PAINT_EXTEND_PAD: પાછા "pad";
    સ્થિતિ HB_PAINT_EXTEND_REPEAT: પાછા "repeat";
    સ્થિતિ HB_PAINT_EXTEND_REFLECT: પાછા "reflect";
    મૂળભૂત: પાછા "pad";
  }
}

સ્થિર શૂન્ય
hb_vector_svg_emit_color_stops (hb_vector_paint_t *paint,
				hb_vector_buf_t *buf,
				hb_vector_t<hb_color_stop_t> *stops)
{
  માટે (unsigned i = 0; i < stops->length; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_અંક (stops->arrayZ[i].offset, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    જો (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_અંક (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

સ્થિર અચલ અક્ષર *
hb_vector_svg_composite_mode_str (hb_paint_composite_mode_t mode)
{
  પસંદગી (mode)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_PLUS: પાછા "screen";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_XOR: પાછા "difference";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_CLEAR:
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC:
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST:
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_IN:
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_IN:
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      પાછા nullptr;
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_OVER: પાછા "normal";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SCREEN: પાછા "screen";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_OVERLAY: પાછા "overlay";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DARKEN: પાછા "darken";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_LIGHTEN: પાછા "lighten";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: પાછા "color-dodge";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_COLOR_BURN: પાછા "color-burn";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: પાછા "hard-light";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: પાછા "soft-light";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_DIFFERENCE: પાછા "difference";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_EXCLUSION: પાછા "exclusion";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_MULTIPLY: પાછા "multiply";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_HUE: પાછા "hue";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: પાછા "saturation";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_COLOR: પાછા "color";
    સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: પાછા "luminosity";
    મૂળભૂત: પાછા nullptr;
  }
}

સંરચના hb_vector_svg_point_t { દશાંશ x, y; };
સંરચના hb_vector_svg_rgba_t { દશાંશ r, g, b, a; };

સ્થિર અંતઃરેખ દશાંશ
hb_vector_svg_lerp (દશાંશ a, દશાંશ b, દશાંશ t)
{ પાછા a + (b - a) * t; }

સ્થિર અંતઃરેખ દશાંશ
hb_vector_svg_clamp01 (દશાંશ v)
{
  જો (v < 0.f) પાછા 0.f;
  જો (v > 1.f) પાછા 1.f;
  પાછા v;
}

સ્થિર અંતઃરેખ hb_vector_svg_rgba_t
hb_vector_svg_rgba_from_hb_color (hb_color_t c)
{
  પાછા {(દશાંશ) hb_color_get_red (c) / 255.f,
          (દશાંશ) hb_color_get_green (c) / 255.f,
          (દશાંશ) hb_color_get_blue (c) / 255.f,
          (દશાંશ) hb_color_get_alpha (c) / 255.f};
}

સ્થિર અંતઃરેખ hb_color_t
hb_vector_svg_hb_color_from_rgba (અચલ hb_vector_svg_rgba_t &c)
{
  unsigned r = (unsigned) roundf (hb_vector_svg_clamp01 (c.r) * 255.f);
  unsigned g = (unsigned) roundf (hb_vector_svg_clamp01 (c.g) * 255.f);
  unsigned b = (unsigned) roundf (hb_vector_svg_clamp01 (c.b) * 255.f);
  unsigned a = (unsigned) roundf (hb_vector_svg_clamp01 (c.a) * 255.f);
  પાછા HB_COLOR (b, g, r, a);
}

સ્થિર અંતઃરેખ hb_vector_svg_rgba_t
hb_vector_svg_lerp_rgba (અચલ hb_vector_svg_rgba_t &c0,
			 અચલ hb_vector_svg_rgba_t &c1,
			 દશાંશ t)
{
  પાછા {hb_vector_svg_lerp (c0.r, c1.r, t),
          hb_vector_svg_lerp (c0.g, c1.g, t),
          hb_vector_svg_lerp (c0.b, c1.b, t),
          hb_vector_svg_lerp (c0.a, c1.a, t)};
}

સ્થિર અંતઃરેખ દશાંશ hb_vector_svg_dot (અચલ hb_vector_svg_point_t &p, અચલ hb_vector_svg_point_t &q) { પાછા p.x * q.x + p.y * q.y; }
સ્થિર અંતઃરેખ hb_vector_svg_point_t hb_vector_svg_add (અચલ hb_vector_svg_point_t &p, અચલ hb_vector_svg_point_t &q) { પાછા {p.x + q.x, p.y + q.y}; }
સ્થિર અંતઃરેખ hb_vector_svg_point_t hb_vector_svg_sub (અચલ hb_vector_svg_point_t &p, અચલ hb_vector_svg_point_t &q) { પાછા {p.x - q.x, p.y - q.y}; }
સ્થિર અંતઃરેખ hb_vector_svg_point_t hb_vector_svg_scale (અચલ hb_vector_svg_point_t &p, દશાંશ f) { પાછા {p.x * f, p.y * f}; }

સ્થિર અંતઃરેખ hb_vector_svg_point_t
hb_vector_svg_normalize (અચલ hb_vector_svg_point_t &p)
{
  દશાંશ len = sqrtf (hb_vector_svg_dot (p, p));
  જો (len == 0.f) પાછા {0.f, 0.f};
  પાછા hb_vector_svg_scale (p, 1.f / len);
}

સ્થિર શૂન્ય
hb_vector_svg_add_sweep_patch (hb_vector_buf_t *body,
			       unsigned precision,
			       દશાંશ cx, દશાંશ cy, દશાંશ radius,
			       દશાંશ a0, અચલ hb_vector_svg_rgba_t &c0_in,
			       દશાંશ a1, અચલ hb_vector_svg_rgba_t &c1_in)
{
  સ્થિર અચલ દશાંશ max_angle = HB_PI / 16.f;
  hb_vector_svg_point_t center = {cx, cy};
  પૂર્ણાંક અંક_splits = (પૂર્ણાંક) ceilf (fabsf (a1 - a0) / max_angle);
  જો (અંક_splits < 1) અંક_splits = 1;

  hb_vector_svg_point_t p0 = {cosf (a0), sinf (a0)};
  hb_vector_svg_rgba_t color0 = c0_in;

  માટે (પૂર્ણાંક a = 0; a < અંક_splits; a++)
  {
    દશાંશ k = (a + 1.f) / અંક_splits;
    દશાંશ angle1 = hb_vector_svg_lerp (a0, a1, k);
    hb_vector_svg_rgba_t color1 = hb_vector_svg_lerp_rgba (c0_in, c1_in, k);

    hb_vector_svg_point_t p1 = {cosf (angle1), sinf (angle1)};
    hb_vector_svg_point_t sp0 = hb_vector_svg_add (center, hb_vector_svg_scale (p0, radius));
    hb_vector_svg_point_t sp1 = hb_vector_svg_add (center, hb_vector_svg_scale (p1, radius));

    hb_vector_svg_point_t A = hb_vector_svg_normalize (hb_vector_svg_add (p0, p1));
    hb_vector_svg_point_t U = {-A.y, A.x};
    દશાંશ up0 = hb_vector_svg_dot (U, p0);
    દશાંશ up1 = hb_vector_svg_dot (U, p1);
    જો (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      ચાલુ;
    }
    hb_vector_svg_point_t C0 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p0, A), p0) / up0));
    hb_vector_svg_point_t C1 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p1, A), p1) / up1));

    hb_vector_svg_point_t sc0 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C0, hb_vector_svg_scale (hb_vector_svg_sub (C0, p0), 0.33333f)), radius));
    hb_vector_svg_point_t sc1 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C1, hb_vector_svg_scale (hb_vector_svg_sub (C1, p1), 0.33333f)), radius));

    hb_vector_svg_rgba_t mid_color = hb_vector_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_vector_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_અંક (center.x, precision);
    body->append_c (',');
    body->append_અંક (center.y, precision);
    body->append_str ("L");
    body->append_અંક (sp0.x, precision);
    body->append_c (',');
    body->append_અંક (sp0.y, precision);
    body->append_str ("C");
    body->append_અંક (sc0.x, precision);
    body->append_c (',');
    body->append_અંક (sc0.y, precision);
    body->append_c (' ');
    body->append_અંક (sc1.x, precision);
    body->append_c (',');
    body->append_અંક (sc1.y, precision);
    body->append_c (' ');
    body->append_અંક (sp1.x, precision);
    body->append_c (',');
    body->append_અંક (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, સાચું);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
સંરચના hb_vector_svg_sweep_ctx_t
{
  hb_vector_buf_t *body;
  unsigned precision;
  દશાંશ cx, cy, radius;
};

સ્થિર શૂન્ય
hb_vector_svg_sweep_emit_patch (દશાંશ a0, hb_color_t c0,
				દશાંશ a1, hb_color_t c1,
				શૂન્ય *user_data)
{
  auto *ctx = (hb_vector_svg_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  જો (unlikely (ctx->body->in_error ()))
    પાછા;
  hb_vector_svg_add_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_vector_svg_rgba_from_hb_color (c0),
				 a1, hb_vector_svg_rgba_from_hb_color (c1));
}


સ્થિર શૂન્ય hb_vector_paint_ધકેલો_transform (hb_paint_funcs_t *, શૂન્ય *,
                                            દશાંશ, દશાંશ, દશાંશ, દશાંશ, દશાંશ, દશાંશ,
                                            શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_કાઢો_transform (hb_paint_funcs_t *, શૂન્ય *, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_fill_glyph (hb_paint_funcs_t *, શૂન્ય *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_ધકેલો_clip_glyph (hb_paint_funcs_t *, શૂન્ય *, hb_codepoint_t, hb_font_t *, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_ધકેલો_clip_rectangle (hb_paint_funcs_t *, શૂન્ય *, દશાંશ, દશાંશ, દશાંશ, દશાંશ, શૂન્ય *);
સ્થિર hb_draw_funcs_t * hb_vector_paint_ધકેલો_clip_path_start (hb_paint_funcs_t *, શૂન્ય *, શૂન્ય **, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_ધકેલો_clip_path_end (hb_paint_funcs_t *, શૂન્ય *, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_કાઢો_clip (hb_paint_funcs_t *, શૂન્ય *, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_color (hb_paint_funcs_t *, શૂન્ય *, hb_bool_t, hb_color_t, શૂન્ય *);
સ્થિર hb_bool_t hb_vector_paint_image (hb_paint_funcs_t *, શૂન્ય *, hb_blob_t *, unsigned, unsigned, hb_tag_t, દશાંશ, hb_glyph_extents_t *, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_linear_gradient (hb_paint_funcs_t *, શૂન્ય *, hb_color_લીટી_t *, દશાંશ, દશાંશ, દશાંશ, દશાંશ, દશાંશ, દશાંશ, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_radial_gradient (hb_paint_funcs_t *, શૂન્ય *, hb_color_લીટી_t *, દશાંશ, દશાંશ, દશાંશ, દશાંશ, દશાંશ, દશાંશ, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_sweep_gradient (hb_paint_funcs_t *, શૂન્ય *, hb_color_લીટી_t *, દશાંશ, દશાંશ, દશાંશ, દશાંશ, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_ધકેલો_group (hb_paint_funcs_t *, શૂન્ય *, શૂન્ય *);
સ્થિર શૂન્ય hb_vector_paint_કાઢો_group (hb_paint_funcs_t *, શૂન્ય *, hb_paint_composite_mode_t, શૂન્ય *);
સ્થિર hb_bool_t hb_vector_paint_color_glyph (hb_paint_funcs_t *, શૂન્ય *, hb_codepoint_t, hb_font_t *, શૂન્ય *);
સ્થિર hb_bool_t
hb_vector_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      શૂન્ય *paint_data,
				      unsigned color_index,
				      hb_color_t *color,
				      શૂન્ય *user_data HB_UNUSED)
{
  hb_vector_paint_t *paint = (hb_vector_paint_t *) paint_data;
  જો (!color)
    પાછા ખોટું;

  hb_color_t *value = nullptr;
  જો (!paint->custom_palette_colors.has (color_index, &value) || !value)
    પાછા ખોટું;
  *color = *value;
  પાછા સાચું;
}

સ્થિર અંતઃરેખ શૂન્ય free_static_vector_paint_funcs ();
સ્થિર સંરચના hb_vector_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_vector_paint_funcs_lazy_loader_t>
{
  સ્થિર hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_ધકેલો_transform_func (funcs, (hb_paint_ધકેલો_transform_func_t) hb_vector_paint_ધકેલો_transform, nullptr, nullptr);
    hb_paint_funcs_set_કાઢો_transform_func (funcs, (hb_paint_કાઢો_transform_func_t) hb_vector_paint_કાઢો_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_vector_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ધકેલો_clip_glyph_func (funcs, (hb_paint_ધકેલો_clip_glyph_func_t) hb_vector_paint_ધકેલો_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ધકેલો_clip_rectangle_func (funcs, (hb_paint_ધકેલો_clip_rectangle_func_t) hb_vector_paint_ધકેલો_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_ધકેલો_clip_path_start_func (funcs, (hb_paint_ધકેલો_clip_path_start_func_t) hb_vector_paint_ધકેલો_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_ધકેલો_clip_path_end_func (funcs, (hb_paint_ધકેલો_clip_path_end_func_t) hb_vector_paint_ધકેલો_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_કાઢો_clip_func (funcs, (hb_paint_કાઢો_clip_func_t) hb_vector_paint_કાઢો_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_vector_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_vector_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_vector_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_vector_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_vector_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_ધકેલો_group_func (funcs, (hb_paint_ધકેલો_group_func_t) hb_vector_paint_ધકેલો_group, nullptr, nullptr);
    hb_paint_funcs_set_કાઢો_group_func (funcs, (hb_paint_કાઢો_group_func_t) hb_vector_paint_કાઢો_group, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_vector_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_vector_paint_custom_palette_color, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_paint_funcs);
    પાછા funcs;
  }
} static_vector_paint_funcs;

સ્થિર અંતઃરેખ શૂન્ય
free_static_vector_paint_funcs ()
{
  static_vector_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_svg_funcs_get ()
{
  પાછા static_vector_paint_funcs.get_unconst ();
}


સ્થિર શૂન્ય
hb_vector_paint_ધકેલો_transform (hb_paint_funcs_t *,
                                શૂન્ય *paint_data,
                                દશાંશ xx, દશાંશ yx,
                                દશાંશ xy, દશાંશ yy,
                                દશાંશ dx, દશાંશ dy,
                                શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;

  જો (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth++;
    પાછા;
  }
  જો (unlikely (paint->transform_group_depth >= 64))
  {
    paint->transform_group_overflow_depth = 1;
    પાછા;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_group_open_mask = (paint->transform_group_open_mask << 1) | (opened ? 1ull : 0ull);
  paint->transform_group_depth++;

  જો (!opened)
    પાછા;

  auto &body = paint->current_body ();
  unsigned sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_અંક (xx, sprec);
  body.append_c (',');
  body.append_અંક (yx, sprec);
  body.append_c (',');
  body.append_અંક (xy, sprec);
  body.append_c (',');
  body.append_અંક (yy, sprec);
  body.append_c (',');
  body.append_અંક (paint->sx (dx));
  body.append_c (',');
  body.append_અંક (paint->sy (dy));
  body.append_str (")\">\n");
}

સ્થિર શૂન્ય
hb_vector_paint_કાઢો_transform (hb_paint_funcs_t *,
                               શૂન્ય *paint_data,
                               શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;
  જો (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth--;
    પાછા;
  }
  જો (!paint->transform_group_depth)
    પાછા;
  paint->transform_group_depth--;
  hb_bool_t opened = !!(paint->transform_group_open_mask & 1ull);
  paint->transform_group_open_mask >>= 1;
  જો (opened)
    paint->current_body ().append_str ("</g>\n");
}

સ્થિર શૂન્ય
hb_vector_paint_fill_glyph (hb_paint_funcs_t *,
                            શૂન્ય *paint_data,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  જો (likely (paint->budget_remaining >= 0))
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
  body.append_svg_color (color, સાચું);
  body.append_str ("\"/>\n");
}

સ્થિર શૂન્ય
hb_vector_paint_ધકેલો_clip_glyph (hb_paint_funcs_t *,
                                 શૂન્ય *paint_data,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;

  અચલ અક્ષર *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  જો (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  unsigned def_id = paint->path_def_ગણતરી++;
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

સ્થિર શૂન્ય
hb_vector_paint_ધકેલો_clip_rectangle (hb_paint_funcs_t *,
                                     શૂન્ય *paint_data,
                                     દશાંશ xmin, દશાંશ ymin,
                                     દશાંશ xmax, દશાંશ ymax,
                                     શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;

  અચલ અક્ષર *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_અંક (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_અંક (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_અંક (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_અંક (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

સ્થિર hb_draw_funcs_t *
hb_vector_paint_ધકેલો_clip_path_start (hb_paint_funcs_t *,
                                      શૂન્ય *paint_data,
                                      શૂન્ય **draw_data,
                                      શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    પાછા nullptr;
  }

  paint->path.clear ();
  paint->clip_path_sink = {&paint->path, paint->get_precision (),
			   paint->x_scale_factor,
			   paint->y_scale_factor,
			   &paint->budget_remaining};
  *draw_data = &paint->clip_path_sink;
  પાછા hb_vector_svg_path_draw_funcs_get ();
}

સ્થિર શૂન્ય
hb_vector_paint_ધકેલો_clip_path_end (hb_paint_funcs_t *,
                                    શૂન્ય *paint_data,
                                    શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;

  અચલ અક્ષર *pfx = paint->id_prefix;
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

સ્થિર શૂન્ય
hb_vector_paint_કાઢો_clip (hb_paint_funcs_t *,
                          શૂન્ય *paint_data,
                          શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;
  paint->current_body ().append_str ("</g>\n");
}

સ્થિર શૂન્ય
hb_vector_paint_color (hb_paint_funcs_t *,
                       શૂન્ય *paint_data,
                       hb_bool_t,
                       hb_color_t color,
                       શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;

  auto &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, સાચું);
  body.append_str ("\"/>\n");
}

સ્થિર hb_bool_t
hb_vector_paint_image (hb_paint_funcs_t *,
                       શૂન્ય *paint_data,
                       hb_blob_t *image,
                       unsigned width,
                       unsigned height,
                       hb_tag_t format,
                       દશાંશ slant HB_UNUSED,
                       hb_glyph_extents_t *extents,
                       શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા ખોટું;

  auto &body = paint->current_body ();
  જો (format == HB_TAG ('p','n','g',' '))
  {
    જો (!extents || !width || !height)
      પાછા ખોટું;

    unsigned len = 0;
    અચલ અક્ષર *png_data = hb_blob_get_data (image, &len);
    જો (!png_data || !len)
      પાછા ખોટું;

    body.append_str ("<g transform=\"translate(");
    body.append_અંક (paint->sx ((દશાંશ) extents->x_bearing));
    body.append_c (',');
    body.append_અંક (paint->sy ((દશાંશ) extents->y_bearing));
    body.append_str (") scale(");
    body.append_અંક (paint->sx ((દશાંશ) extents->width) / width);
    body.append_c (',');
    body.append_અંક (paint->sy ((દશાંશ) extents->height) / height);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((અચલ uint8_t *) png_data, len);
    body.append_str ("\" width=\"");
    body.append_અંક ((દશાંશ) width);
    body.append_str ("\" height=\"");
    body.append_અંક ((દશાંશ) height);
    body.append_str ("\"/>\n</g>\n");

    પાછા સાચું;
  }

  પાછા ખોટું;
}

સ્થિર શૂન્ય
hb_vector_paint_linear_gradient (hb_paint_funcs_t *,
                                 શૂન્ય *paint_data,
                                 hb_color_લીટી_t *color_લીટી,
                                 દશાંશ x0, દશાંશ y0,
                                 દશાંશ x1, દશાંશ y1,
                                 દશાંશ x2, દશાંશ y2,
                                 શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;

  જો (!paint->fetch_color_stops (color_લીટી))
    પાછા;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  દશાંશ mn, mx;
  hb_paint_normalize_color_લીટી (stops.arrayZ, stops.length, &mn, &mx);

  અચલ અક્ષર *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  દશાંશ lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  દશાંશ gx0 = lx0 + mn * (lx1 - lx0);
  દશાંશ gy0 = ly0 + mn * (ly1 - ly0);
  દશાંશ gx1 = lx0 + mx * (lx1 - lx0);
  દશાંશ gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_અંક (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_અંક (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_અંક (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_અંક (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_લીટી_get_extend (color_લીટી)));
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

સ્થિર શૂન્ય
hb_vector_paint_radial_gradient (hb_paint_funcs_t *,
                                 શૂન્ય *paint_data,
                                 hb_color_લીટી_t *color_લીટી,
                                 દશાંશ x0, દશાંશ y0, દશાંશ r0,
                                 દશાંશ x1, દશાંશ y1, દશાંશ r1,
                                 શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;

  જો (!paint->fetch_color_stops (color_લીટી))
    પાછા;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  દશાંશ mn, mx;
  hb_paint_normalize_color_લીટી (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  દશાંશ gx0 = x0 + mn * (x1 - x0);
  દશાંશ gy0 = y0 + mn * (y1 - y0);
  દશાંશ gr0 = r0 + mn * (r1 - r0);
  દશાંશ gx1 = x0 + mx * (x1 - x0);
  દશાંશ gy1 = y0 + mx * (y1 - y0);
  દશાંશ gr1 = r0 + mx * (r1 - r0);

  અચલ અક્ષર *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_અંક (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_અંક (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_અંક (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_અંક (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_અંક (paint->sy (gy0));
  જો (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_અંક (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_લીટી_get_extend (color_લીટી)));
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

સ્થિર શૂન્ય
hb_vector_paint_sweep_gradient (hb_paint_funcs_t *,
                                શૂન્ય *paint_data,
                                hb_color_લીટી_t *color_લીટી,
                                દશાંશ cx, દશાંશ cy,
                                દશાંશ start_angle, દશાંશ end_angle,
                                શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;

  જો (!paint->fetch_color_stops (color_લીટી))
    પાછા;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  દશાંશ mn, mx;
  hb_paint_normalize_color_લીટી (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  દશાંશ ga0 = start_angle + mn * (end_angle - start_angle);
  દશાંશ ga1 = start_angle + mx * (end_angle - start_angle);

  hb_vector_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length,
				 hb_color_લીટી_get_extend (color_લીટી),
				 ga0, ga1,
				 hb_vector_svg_sweep_emit_patch,
				 &ctx);
}

સ્થિર શૂન્ય
hb_vector_paint_ધકેલો_group (hb_paint_funcs_t *,
                            શૂન્ય *paint_data,
                            શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;
  જો (unlikely (!paint->group_ઢગલો.ધકેલો_or_fail (hb_vector_buf_t {})))
    પાછા;
}

સ્થિર શૂન્ય
hb_vector_paint_કાઢો_group (hb_paint_funcs_t *,
                           શૂન્ય *paint_data,
                           hb_paint_composite_mode_t mode,
                           શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા;
  જો (paint->group_ઢગલો.length < 2)
    પાછા;

  hb_vector_buf_t group = paint->group_ઢગલો.કાઢો ();
  auto &body = paint->current_body ();

  અચલ અક્ષર *blend = hb_vector_svg_composite_mode_str (mode);
  જો (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (group.arrayZ, group.length);
    body.append_str ("</g>\n");
  }
  નહીંતર
    body.append_len (group.arrayZ, group.length);
}

સ્થિર hb_bool_t
hb_vector_paint_color_glyph (hb_paint_funcs_t *,
                             શૂન્ય *paint_data,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             શૂન્ય *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  જો (unlikely (!paint->ensure_initialized ()))
    પાછા ખોટું;
  જો (unlikely (paint->color_glyph_depth >= HB_MAX_NESTING_LEVEL))
    પાછા ખોટું;
  જો (unlikely (hb_set_has (paint->active_color_glyphs, glyph)))
    પાછા ખોટું;

  paint->color_glyph_depth++;
  hb_set_add (paint->active_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_vector_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_set_del (paint->active_color_glyphs, glyph);
  paint->color_glyph_depth--;
  પાછા સાચું;
}




hb_blob_t *
hb_vector_paint_render_svg (hb_vector_paint_t *paint)
{
  જો (!paint->has_extents)
    પાછા nullptr;

  જો (unlikely (!paint->ensure_initialized ()))
    પાછા nullptr;

  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  unsigned estimated = paint->defs.length +
		       paint->group_ઢગલો.arrayZ[0].length +
		       320;
  out.alloc (estimated);
  દશાંશ vb_x = paint->extents.x;
  દશાંશ vb_y = -(paint->extents.y + paint->extents.height);
  દશાંશ vb_w = paint->extents.width;
  દશાંશ vb_h = paint->extents.height;
  out.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  out.append_અંક (vb_x);
  out.append_c (' ');
  out.append_અંક (vb_y);
  out.append_c (' ');
  out.append_અંક (vb_w);
  out.append_c (' ');
  out.append_અંક (vb_h);
  out.append_str ("\" width=\"");
  out.append_અંક (vb_w);
  out.append_str ("\" height=\"");
  out.append_અંક (vb_h);
  out.append_str ("\">\n");

  જો (paint->defs.length)
  {
    out.append_str ("<defs>\n");
    out.append_len (paint->defs.arrayZ, paint->defs.length);
    out.append_str ("</defs>\n");
  }

  જો (hb_color_get_alpha (paint->background))
  {
    out.append_str ("<rect x=\"");
    out.append_અંક (vb_x);
    out.append_str ("\" y=\"");
    out.append_અંક (vb_y);
    out.append_str ("\" width=\"");
    out.append_અંક (vb_w);
    out.append_str ("\" height=\"");
    out.append_અંક (vb_h);
    out.append_str ("\" fill=\"rgb(");
    out.append_unsigned (hb_color_get_red (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_green (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_blue (paint->background));
    out.append_str (")\"");
    જો (hb_color_get_alpha (paint->background) < 255)
    {
      out.append_str (" fill-opacity=\"");
      out.append_અંક (hb_color_get_alpha (paint->background) / 255.f, 4);
      out.append_c ('"');
    }
    out.append_str ("/>\n");
  }

  out.append_str ("<g transform=\"scale(1,-1)\">\n");
  out.append_len (paint->group_ઢગલો.arrayZ[0].arrayZ, paint->group_ઢગલો.arrayZ[0].length);
  out.append_str ("</g>\n");

  out.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  પાછા blob;
}
