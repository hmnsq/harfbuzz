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

#ضمن "hb.hh"

#ضمن "hb-vector-paint.hh"
#ضمن "hb-paint.hh"
#ضمن "hb-vector-path.hh"

#ضمن <math.h>

ساكن ثابت حرف *
hb_vector_svg_extend_mode_str (hb_paint_extend_t ext)
{
  بدل (ext)
  {
    حالة HB_PAINT_EXTEND_PAD: أرجع "pad";
    حالة HB_PAINT_EXTEND_REPEAT: أرجع "repeat";
    حالة HB_PAINT_EXTEND_REFLECT: أرجع "reflect";
    افتراضي: أرجع "pad";
  }
}

ساكن فراغ
hb_vector_svg_emit_color_stops (hb_vector_paint_t *paint,
				hb_vector_buf_t *buf,
				hb_vector_t<hb_color_stop_t> *stops)
{
  لكل (unsigned i = 0; i < stops->length; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_ژمارە (stops->arrayZ[i].offset, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    إذا (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_ژمارە (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

ساكن ثابت حرف *
hb_vector_svg_composite_mode_str (hb_paint_composite_mode_t mode)
{
  بدل (mode)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    حالة HB_PAINT_COMPOSITE_MODE_PLUS: أرجع "screen";
    حالة HB_PAINT_COMPOSITE_MODE_XOR: أرجع "difference";
    حالة HB_PAINT_COMPOSITE_MODE_CLEAR:
    حالة HB_PAINT_COMPOSITE_MODE_SRC:
    حالة HB_PAINT_COMPOSITE_MODE_DEST:
    حالة HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    حالة HB_PAINT_COMPOSITE_MODE_SRC_IN:
    حالة HB_PAINT_COMPOSITE_MODE_DEST_IN:
    حالة HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    حالة HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    حالة HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
    حالة HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      أرجع nullptr;
    حالة HB_PAINT_COMPOSITE_MODE_SRC_OVER: أرجع "normal";
    حالة HB_PAINT_COMPOSITE_MODE_SCREEN: أرجع "screen";
    حالة HB_PAINT_COMPOSITE_MODE_OVERLAY: أرجع "overlay";
    حالة HB_PAINT_COMPOSITE_MODE_DARKEN: أرجع "darken";
    حالة HB_PAINT_COMPOSITE_MODE_LIGHTEN: أرجع "lighten";
    حالة HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: أرجع "color-dodge";
    حالة HB_PAINT_COMPOSITE_MODE_COLOR_BURN: أرجع "color-burn";
    حالة HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: أرجع "hard-light";
    حالة HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: أرجع "soft-light";
    حالة HB_PAINT_COMPOSITE_MODE_DIFFERENCE: أرجع "difference";
    حالة HB_PAINT_COMPOSITE_MODE_EXCLUSION: أرجع "exclusion";
    حالة HB_PAINT_COMPOSITE_MODE_MULTIPLY: أرجع "multiply";
    حالة HB_PAINT_COMPOSITE_MODE_HSL_HUE: أرجع "hue";
    حالة HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: أرجع "saturation";
    حالة HB_PAINT_COMPOSITE_MODE_HSL_COLOR: أرجع "color";
    حالة HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: أرجع "luminosity";
    افتراضي: أرجع nullptr;
  }
}

بنية hb_vector_svg_point_t { عائم x, y; };
بنية hb_vector_svg_rgba_t { عائم r, g, b, a; };

ساكن مضمن عائم
hb_vector_svg_lerp (عائم a, عائم b, عائم t)
{ أرجع a + (b - a) * t; }

ساكن مضمن عائم
hb_vector_svg_clamp01 (عائم v)
{
  إذا (v < 0.f) أرجع 0.f;
  إذا (v > 1.f) أرجع 1.f;
  أرجع v;
}

ساكن مضمن hb_vector_svg_rgba_t
hb_vector_svg_rgba_from_hb_color (hb_color_t c)
{
  أرجع {(عائم) hb_color_get_red (c) / 255.f,
          (عائم) hb_color_get_green (c) / 255.f,
          (عائم) hb_color_get_blue (c) / 255.f,
          (عائم) hb_color_get_alpha (c) / 255.f};
}

ساكن مضمن hb_color_t
hb_vector_svg_hb_color_from_rgba (ثابت hb_vector_svg_rgba_t &c)
{
  unsigned r = (unsigned) roundf (hb_vector_svg_clamp01 (c.r) * 255.f);
  unsigned g = (unsigned) roundf (hb_vector_svg_clamp01 (c.g) * 255.f);
  unsigned b = (unsigned) roundf (hb_vector_svg_clamp01 (c.b) * 255.f);
  unsigned a = (unsigned) roundf (hb_vector_svg_clamp01 (c.a) * 255.f);
  أرجع HB_COLOR (b, g, r, a);
}

ساكن مضمن hb_vector_svg_rgba_t
hb_vector_svg_lerp_rgba (ثابت hb_vector_svg_rgba_t &c0,
			 ثابت hb_vector_svg_rgba_t &c1,
			 عائم t)
{
  أرجع {hb_vector_svg_lerp (c0.r, c1.r, t),
          hb_vector_svg_lerp (c0.g, c1.g, t),
          hb_vector_svg_lerp (c0.b, c1.b, t),
          hb_vector_svg_lerp (c0.a, c1.a, t)};
}

ساكن مضمن عائم hb_vector_svg_dot (ثابت hb_vector_svg_point_t &p, ثابت hb_vector_svg_point_t &q) { أرجع p.x * q.x + p.y * q.y; }
ساكن مضمن hb_vector_svg_point_t hb_vector_svg_add (ثابت hb_vector_svg_point_t &p, ثابت hb_vector_svg_point_t &q) { أرجع {p.x + q.x, p.y + q.y}; }
ساكن مضمن hb_vector_svg_point_t hb_vector_svg_sub (ثابت hb_vector_svg_point_t &p, ثابت hb_vector_svg_point_t &q) { أرجع {p.x - q.x, p.y - q.y}; }
ساكن مضمن hb_vector_svg_point_t hb_vector_svg_scale (ثابت hb_vector_svg_point_t &p, عائم f) { أرجع {p.x * f, p.y * f}; }

ساكن مضمن hb_vector_svg_point_t
hb_vector_svg_normalize (ثابت hb_vector_svg_point_t &p)
{
  عائم len = sqrtf (hb_vector_svg_dot (p, p));
  إذا (len == 0.f) أرجع {0.f, 0.f};
  أرجع hb_vector_svg_scale (p, 1.f / len);
}

ساكن فراغ
hb_vector_svg_add_sweep_patch (hb_vector_buf_t *body,
			       unsigned precision,
			       عائم cx, عائم cy, عائم radius,
			       عائم a0, ثابت hb_vector_svg_rgba_t &c0_in,
			       عائم a1, ثابت hb_vector_svg_rgba_t &c1_in)
{
  ساكن ثابت عائم max_angle = HB_PI / 16.f;
  hb_vector_svg_point_t center = {cx, cy};
  صحيح ژمارە_splits = (صحيح) ceilf (fabsf (a1 - a0) / max_angle);
  إذا (ژمارە_splits < 1) ژمارە_splits = 1;

  hb_vector_svg_point_t p0 = {cosf (a0), sinf (a0)};
  hb_vector_svg_rgba_t color0 = c0_in;

  لكل (صحيح a = 0; a < ژمارە_splits; a++)
  {
    عائم k = (a + 1.f) / ژمارە_splits;
    عائم angle1 = hb_vector_svg_lerp (a0, a1, k);
    hb_vector_svg_rgba_t color1 = hb_vector_svg_lerp_rgba (c0_in, c1_in, k);

    hb_vector_svg_point_t p1 = {cosf (angle1), sinf (angle1)};
    hb_vector_svg_point_t sp0 = hb_vector_svg_add (center, hb_vector_svg_scale (p0, radius));
    hb_vector_svg_point_t sp1 = hb_vector_svg_add (center, hb_vector_svg_scale (p1, radius));

    hb_vector_svg_point_t A = hb_vector_svg_normalize (hb_vector_svg_add (p0, p1));
    hb_vector_svg_point_t U = {-A.y, A.x};
    عائم up0 = hb_vector_svg_dot (U, p0);
    عائم up1 = hb_vector_svg_dot (U, p1);
    إذا (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      استمر;
    }
    hb_vector_svg_point_t C0 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p0, A), p0) / up0));
    hb_vector_svg_point_t C1 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p1, A), p1) / up1));

    hb_vector_svg_point_t sc0 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C0, hb_vector_svg_scale (hb_vector_svg_sub (C0, p0), 0.33333f)), radius));
    hb_vector_svg_point_t sc1 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C1, hb_vector_svg_scale (hb_vector_svg_sub (C1, p1), 0.33333f)), radius));

    hb_vector_svg_rgba_t mid_color = hb_vector_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_vector_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_ژمارە (center.x, precision);
    body->append_c (',');
    body->append_ژمارە (center.y, precision);
    body->append_str ("L");
    body->append_ژمارە (sp0.x, precision);
    body->append_c (',');
    body->append_ژمارە (sp0.y, precision);
    body->append_str ("C");
    body->append_ژمارە (sc0.x, precision);
    body->append_c (',');
    body->append_ژمارە (sc0.y, precision);
    body->append_c (' ');
    body->append_ژمارە (sc1.x, precision);
    body->append_c (',');
    body->append_ژمارە (sc1.y, precision);
    body->append_c (' ');
    body->append_ژمارە (sp1.x, precision);
    body->append_c (',');
    body->append_ژمارە (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, true);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
بنية hb_vector_svg_sweep_ctx_t
{
  hb_vector_buf_t *body;
  unsigned precision;
  عائم cx, cy, radius;
};

ساكن فراغ
hb_vector_svg_sweep_emit_patch (عائم a0, hb_color_t c0,
				عائم a1, hb_color_t c1,
				فراغ *user_data)
{
  auto *ctx = (hb_vector_svg_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  إذا (unlikely (ctx->body->in_error ()))
    أرجع;
  hb_vector_svg_add_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_vector_svg_rgba_from_hb_color (c0),
				 a1, hb_vector_svg_rgba_from_hb_color (c1));
}


ساكن فراغ hb_vector_paint_پاڵنان_transform (hb_paint_funcs_t *, فراغ *,
                                            عائم, عائم, عائم, عائم, عائم, عائم,
                                            فراغ *);
ساكن فراغ hb_vector_paint_دەرهێنان_transform (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_vector_paint_fill_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, فراغ *);
ساكن فراغ hb_vector_paint_پاڵنان_clip_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, فراغ *);
ساكن فراغ hb_vector_paint_پاڵنان_clip_rectangle (hb_paint_funcs_t *, فراغ *, عائم, عائم, عائم, عائم, فراغ *);
ساكن hb_draw_funcs_t * hb_vector_paint_پاڵنان_clip_path_start (hb_paint_funcs_t *, فراغ *, فراغ **, فراغ *);
ساكن فراغ hb_vector_paint_پاڵنان_clip_path_end (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_vector_paint_دەرهێنان_clip (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_vector_paint_color (hb_paint_funcs_t *, فراغ *, hb_bool_t, hb_color_t, فراغ *);
ساكن hb_bool_t hb_vector_paint_image (hb_paint_funcs_t *, فراغ *, hb_blob_t *, unsigned, unsigned, hb_tag_t, عائم, hb_glyph_extents_t *, فراغ *);
ساكن فراغ hb_vector_paint_linear_gradient (hb_paint_funcs_t *, فراغ *, hb_color_هێڵ_t *, عائم, عائم, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_vector_paint_radial_gradient (hb_paint_funcs_t *, فراغ *, hb_color_هێڵ_t *, عائم, عائم, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_vector_paint_sweep_gradient (hb_paint_funcs_t *, فراغ *, hb_color_هێڵ_t *, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_vector_paint_پاڵنان_group (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_vector_paint_دەرهێنان_group (hb_paint_funcs_t *, فراغ *, hb_paint_composite_mode_t, فراغ *);
ساكن hb_bool_t hb_vector_paint_color_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, فراغ *);
ساكن hb_bool_t
hb_vector_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      فراغ *paint_data,
				      unsigned color_index,
				      hb_color_t *color,
				      فراغ *user_data HB_UNUSED)
{
  hb_vector_paint_t *paint = (hb_vector_paint_t *) paint_data;
  إذا (!color)
    أرجع خطأ;

  hb_color_t *value = nullptr;
  إذا (!paint->custom_palette_colors.has (color_index, &value) || !value)
    أرجع خطأ;
  *color = *value;
  أرجع true;
}

ساكن مضمن فراغ free_static_vector_paint_funcs ();
ساكن بنية hb_vector_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_vector_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_پاڵنان_transform_func (funcs, (hb_paint_پاڵنان_transform_func_t) hb_vector_paint_پاڵنان_transform, nullptr, nullptr);
    hb_paint_funcs_set_دەرهێنان_transform_func (funcs, (hb_paint_دەرهێنان_transform_func_t) hb_vector_paint_دەرهێنان_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_vector_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_پاڵنان_clip_glyph_func (funcs, (hb_paint_پاڵنان_clip_glyph_func_t) hb_vector_paint_پاڵنان_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_پاڵنان_clip_rectangle_func (funcs, (hb_paint_پاڵنان_clip_rectangle_func_t) hb_vector_paint_پاڵنان_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_پاڵنان_clip_path_start_func (funcs, (hb_paint_پاڵنان_clip_path_start_func_t) hb_vector_paint_پاڵنان_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_پاڵنان_clip_path_end_func (funcs, (hb_paint_پاڵنان_clip_path_end_func_t) hb_vector_paint_پاڵنان_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_دەرهێنان_clip_func (funcs, (hb_paint_دەرهێنان_clip_func_t) hb_vector_paint_دەرهێنان_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_vector_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_vector_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_vector_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_vector_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_vector_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_پاڵنان_group_func (funcs, (hb_paint_پاڵنان_group_func_t) hb_vector_paint_پاڵنان_group, nullptr, nullptr);
    hb_paint_funcs_set_دەرهێنان_group_func (funcs, (hb_paint_دەرهێنان_group_func_t) hb_vector_paint_دەرهێنان_group, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_vector_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_vector_paint_custom_palette_color, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_paint_funcs);
    أرجع funcs;
  }
} static_vector_paint_funcs;

ساكن مضمن فراغ
free_static_vector_paint_funcs ()
{
  static_vector_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_svg_funcs_get ()
{
  أرجع static_vector_paint_funcs.get_unconst ();
}


ساكن فراغ
hb_vector_paint_پاڵنان_transform (hb_paint_funcs_t *,
                                فراغ *paint_data,
                                عائم xx, عائم yx,
                                عائم xy, عائم yy,
                                عائم dx, عائم dy,
                                فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth++;
    أرجع;
  }
  إذا (unlikely (paint->transform_group_depth >= 64))
  {
    paint->transform_group_overflow_depth = 1;
    أرجع;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_group_open_mask = (paint->transform_group_open_mask << 1) | (opened ? 1ull : 0ull);
  paint->transform_group_depth++;

  إذا (!opened)
    أرجع;

  auto &body = paint->current_body ();
  unsigned sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_ژمارە (xx, sprec);
  body.append_c (',');
  body.append_ژمارە (yx, sprec);
  body.append_c (',');
  body.append_ژمارە (xy, sprec);
  body.append_c (',');
  body.append_ژمارە (yy, sprec);
  body.append_c (',');
  body.append_ژمارە (paint->sx (dx));
  body.append_c (',');
  body.append_ژمارە (paint->sy (dy));
  body.append_str (")\">\n");
}

ساكن فراغ
hb_vector_paint_دەرهێنان_transform (hb_paint_funcs_t *,
                               فراغ *paint_data,
                               فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth--;
    أرجع;
  }
  إذا (!paint->transform_group_depth)
    أرجع;
  paint->transform_group_depth--;
  hb_bool_t opened = !!(paint->transform_group_open_mask & 1ull);
  paint->transform_group_open_mask >>= 1;
  إذا (opened)
    paint->current_body ().append_str ("</g>\n");
}

ساكن فراغ
hb_vector_paint_fill_glyph (hb_paint_funcs_t *,
                            فراغ *paint_data,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  إذا (likely (paint->budget_remaining >= 0))
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
  body.append_svg_color (color, true);
  body.append_str ("\"/>\n");
}

ساكن فراغ
hb_vector_paint_پاڵنان_clip_glyph (hb_paint_funcs_t *,
                                 فراغ *paint_data,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  إذا (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  unsigned def_id = paint->path_def_ژماردن++;
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

ساكن فراغ
hb_vector_paint_پاڵنان_clip_rectangle (hb_paint_funcs_t *,
                                     فراغ *paint_data,
                                     عائم xmin, عائم ymin,
                                     عائم xmax, عائم ymax,
                                     فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_ژمارە (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_ژمارە (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_ژمارە (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_ژمارە (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ساكن hb_draw_funcs_t *
hb_vector_paint_پاڵنان_clip_path_start (hb_paint_funcs_t *,
                                      فراغ *paint_data,
                                      فراغ **draw_data,
                                      فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    أرجع nullptr;
  }

  paint->path.clear ();
  paint->clip_path_sink = {&paint->path, paint->get_precision (),
			   paint->x_scale_factor,
			   paint->y_scale_factor,
			   &paint->budget_remaining};
  *draw_data = &paint->clip_path_sink;
  أرجع hb_vector_svg_path_draw_funcs_get ();
}

ساكن فراغ
hb_vector_paint_پاڵنان_clip_path_end (hb_paint_funcs_t *,
                                    فراغ *paint_data,
                                    فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_prefix;
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

ساكن فراغ
hb_vector_paint_دەرهێنان_clip (hb_paint_funcs_t *,
                          فراغ *paint_data,
                          فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("</g>\n");
}

ساكن فراغ
hb_vector_paint_color (hb_paint_funcs_t *,
                       فراغ *paint_data,
                       hb_bool_t,
                       hb_color_t color,
                       فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  auto &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, true);
  body.append_str ("\"/>\n");
}

ساكن hb_bool_t
hb_vector_paint_image (hb_paint_funcs_t *,
                       فراغ *paint_data,
                       hb_blob_t *image,
                       unsigned width,
                       unsigned height,
                       hb_tag_t format,
                       عائم slant HB_UNUSED,
                       hb_glyph_extents_t *extents,
                       فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;

  auto &body = paint->current_body ();
  إذا (format == HB_TAG ('p','n','g',' '))
  {
    إذا (!extents || !width || !height)
      أرجع خطأ;

    unsigned len = 0;
    ثابت حرف *png_data = hb_blob_get_data (image, &len);
    إذا (!png_data || !len)
      أرجع خطأ;

    body.append_str ("<g transform=\"translate(");
    body.append_ژمارە (paint->sx ((عائم) extents->x_bearing));
    body.append_c (',');
    body.append_ژمارە (paint->sy ((عائم) extents->y_bearing));
    body.append_str (") scale(");
    body.append_ژمارە (paint->sx ((عائم) extents->width) / width);
    body.append_c (',');
    body.append_ژمارە (paint->sy ((عائم) extents->height) / height);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ثابت uint8_t *) png_data, len);
    body.append_str ("\" width=\"");
    body.append_ژمارە ((عائم) width);
    body.append_str ("\" height=\"");
    body.append_ژمارە ((عائم) height);
    body.append_str ("\"/>\n</g>\n");

    أرجع true;
  }

  أرجع خطأ;
}

ساكن فراغ
hb_vector_paint_linear_gradient (hb_paint_funcs_t *,
                                 فراغ *paint_data,
                                 hb_color_هێڵ_t *color_هێڵ,
                                 عائم x0, عائم y0,
                                 عائم x1, عائم y1,
                                 عائم x2, عائم y2,
                                 فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_هێڵ))
    أرجع;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  عائم mn, mx;
  hb_paint_normalize_color_هێڵ (stops.arrayZ, stops.length, &mn, &mx);

  ثابت حرف *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  عائم lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  عائم gx0 = lx0 + mn * (lx1 - lx0);
  عائم gy0 = ly0 + mn * (ly1 - ly0);
  عائم gx1 = lx0 + mx * (lx1 - lx0);
  عائم gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_ژمارە (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_ژمارە (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_ژمارە (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_ژمارە (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_هێڵ_get_extend (color_هێڵ)));
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

ساكن فراغ
hb_vector_paint_radial_gradient (hb_paint_funcs_t *,
                                 فراغ *paint_data,
                                 hb_color_هێڵ_t *color_هێڵ,
                                 عائم x0, عائم y0, عائم r0,
                                 عائم x1, عائم y1, عائم r1,
                                 فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_هێڵ))
    أرجع;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_هێڵ (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gr0 = r0 + mn * (r1 - r0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);
  عائم gr1 = r0 + mx * (r1 - r0);

  ثابت حرف *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_ژمارە (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_ژمارە (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_ژمارە (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_ژمارە (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_ژمارە (paint->sy (gy0));
  إذا (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_ژمارە (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_هێڵ_get_extend (color_هێڵ)));
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

ساكن فراغ
hb_vector_paint_sweep_gradient (hb_paint_funcs_t *,
                                فراغ *paint_data,
                                hb_color_هێڵ_t *color_هێڵ,
                                عائم cx, عائم cy,
                                عائم start_angle, عائم end_angle,
                                فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_هێڵ))
    أرجع;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_هێڵ (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  عائم ga0 = start_angle + mn * (end_angle - start_angle);
  عائم ga1 = start_angle + mx * (end_angle - start_angle);

  hb_vector_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length,
				 hb_color_هێڵ_get_extend (color_هێڵ),
				 ga0, ga1,
				 hb_vector_svg_sweep_emit_patch,
				 &ctx);
}

ساكن فراغ
hb_vector_paint_پاڵنان_group (hb_paint_funcs_t *,
                            فراغ *paint_data,
                            فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (unlikely (!paint->group_کەڵەکە.پاڵنان_or_fail (hb_vector_buf_t {})))
    أرجع;
}

ساكن فراغ
hb_vector_paint_دەرهێنان_group (hb_paint_funcs_t *,
                           فراغ *paint_data,
                           hb_paint_composite_mode_t mode,
                           فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (paint->group_کەڵەکە.length < 2)
    أرجع;

  hb_vector_buf_t group = paint->group_کەڵەکە.دەرهێنان ();
  auto &body = paint->current_body ();

  ثابت حرف *blend = hb_vector_svg_composite_mode_str (mode);
  إذا (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (group.arrayZ, group.length);
    body.append_str ("</g>\n");
  }
  وإلا
    body.append_len (group.arrayZ, group.length);
}

ساكن hb_bool_t
hb_vector_paint_color_glyph (hb_paint_funcs_t *,
                             فراغ *paint_data,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             فراغ *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;
  إذا (unlikely (paint->color_glyph_depth >= HB_MAX_NESTING_LEVEL))
    أرجع خطأ;
  إذا (unlikely (hb_set_has (paint->active_color_glyphs, glyph)))
    أرجع خطأ;

  paint->color_glyph_depth++;
  hb_set_add (paint->active_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_vector_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_set_del (paint->active_color_glyphs, glyph);
  paint->color_glyph_depth--;
  أرجع true;
}




hb_blob_t *
hb_vector_paint_render_svg (hb_vector_paint_t *paint)
{
  إذا (!paint->has_extents)
    أرجع nullptr;

  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع nullptr;

  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  unsigned estimated = paint->defs.length +
		       paint->group_کەڵەکە.arrayZ[0].length +
		       320;
  out.alloc (estimated);
  عائم vb_x = paint->extents.x;
  عائم vb_y = -(paint->extents.y + paint->extents.height);
  عائم vb_w = paint->extents.width;
  عائم vb_h = paint->extents.height;
  out.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  out.append_ژمارە (vb_x);
  out.append_c (' ');
  out.append_ژمارە (vb_y);
  out.append_c (' ');
  out.append_ژمارە (vb_w);
  out.append_c (' ');
  out.append_ژمارە (vb_h);
  out.append_str ("\" width=\"");
  out.append_ژمارە (vb_w);
  out.append_str ("\" height=\"");
  out.append_ژمارە (vb_h);
  out.append_str ("\">\n");

  إذا (paint->defs.length)
  {
    out.append_str ("<defs>\n");
    out.append_len (paint->defs.arrayZ, paint->defs.length);
    out.append_str ("</defs>\n");
  }

  إذا (hb_color_get_alpha (paint->background))
  {
    out.append_str ("<rect x=\"");
    out.append_ژمارە (vb_x);
    out.append_str ("\" y=\"");
    out.append_ژمارە (vb_y);
    out.append_str ("\" width=\"");
    out.append_ژمارە (vb_w);
    out.append_str ("\" height=\"");
    out.append_ژمارە (vb_h);
    out.append_str ("\" fill=\"rgb(");
    out.append_unsigned (hb_color_get_red (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_green (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_blue (paint->background));
    out.append_str (")\"");
    إذا (hb_color_get_alpha (paint->background) < 255)
    {
      out.append_str (" fill-opacity=\"");
      out.append_ژمارە (hb_color_get_alpha (paint->background) / 255.f, 4);
      out.append_c ('"');
    }
    out.append_str ("/>\n");
  }

  out.append_str ("<g transform=\"scale(1,-1)\">\n");
  out.append_len (paint->group_کەڵەکە.arrayZ[0].arrayZ, paint->group_کەڵەکە.arrayZ[0].length);
  out.append_str ("</g>\n");

  out.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  أرجع blob;
}
