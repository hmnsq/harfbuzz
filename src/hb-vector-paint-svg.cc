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

#شامل "hb.hh"

#شامل "hb-vector-paint.hh"
#شامل "hb-paint.hh"
#شامل "hb-vector-path.hh"

#شامل <math.h>

ایستا ثابت نویسه *
hb_vector_svg_extend_mode_str (hb_paint_extend_t ext)
{
  انتخاب (ext)
  {
    حالت HB_PAINT_EXTEND_PAD: بازگشت "pad";
    حالت HB_PAINT_EXTEND_REPEAT: بازگشت "repeat";
    حالت HB_PAINT_EXTEND_REFLECT: بازگشت "reflect";
    پیشفرض: بازگشت "pad";
  }
}

ایستا تهی
hb_vector_svg_emit_color_stops (hb_vector_paint_t *paint,
				hb_vector_buf_t *buf,
				hb_vector_t<hb_color_stop_t> *stops)
{
  برای (unsigned i = 0; i < stops->length; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_رقم (stops->arrayZ[i].offset, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    اگر (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_رقم (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

ایستا ثابت نویسه *
hb_vector_svg_composite_mode_str (hb_paint_composite_mode_t mode)
{
  انتخاب (mode)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    حالت HB_PAINT_COMPOSITE_MODE_PLUS: بازگشت "screen";
    حالت HB_PAINT_COMPOSITE_MODE_XOR: بازگشت "difference";
    حالت HB_PAINT_COMPOSITE_MODE_CLEAR:
    حالت HB_PAINT_COMPOSITE_MODE_SRC:
    حالت HB_PAINT_COMPOSITE_MODE_DEST:
    حالت HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    حالت HB_PAINT_COMPOSITE_MODE_SRC_IN:
    حالت HB_PAINT_COMPOSITE_MODE_DEST_IN:
    حالت HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    حالت HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    حالت HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
    حالت HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      بازگشت nullptr;
    حالت HB_PAINT_COMPOSITE_MODE_SRC_OVER: بازگشت "normal";
    حالت HB_PAINT_COMPOSITE_MODE_SCREEN: بازگشت "screen";
    حالت HB_PAINT_COMPOSITE_MODE_OVERLAY: بازگشت "overlay";
    حالت HB_PAINT_COMPOSITE_MODE_DARKEN: بازگشت "darken";
    حالت HB_PAINT_COMPOSITE_MODE_LIGHTEN: بازگشت "lighten";
    حالت HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: بازگشت "color-dodge";
    حالت HB_PAINT_COMPOSITE_MODE_COLOR_BURN: بازگشت "color-burn";
    حالت HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: بازگشت "hard-light";
    حالت HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: بازگشت "soft-light";
    حالت HB_PAINT_COMPOSITE_MODE_DIFFERENCE: بازگشت "difference";
    حالت HB_PAINT_COMPOSITE_MODE_EXCLUSION: بازگشت "exclusion";
    حالت HB_PAINT_COMPOSITE_MODE_MULTIPLY: بازگشت "multiply";
    حالت HB_PAINT_COMPOSITE_MODE_HSL_HUE: بازگشت "hue";
    حالت HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: بازگشت "saturation";
    حالت HB_PAINT_COMPOSITE_MODE_HSL_COLOR: بازگشت "color";
    حالت HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: بازگشت "luminosity";
    پیشفرض: بازگشت nullptr;
  }
}

ساختار hb_vector_svg_point_t { اعشاری x, y; };
ساختار hb_vector_svg_rgba_t { اعشاری r, g, b, a; };

ایستا درونخطی اعشاری
hb_vector_svg_lerp (اعشاری a, اعشاری b, اعشاری t)
{ بازگشت a + (b - a) * t; }

ایستا درونخطی اعشاری
hb_vector_svg_clamp01 (اعشاری v)
{
  اگر (v < 0.f) بازگشت 0.f;
  اگر (v > 1.f) بازگشت 1.f;
  بازگشت v;
}

ایستا درونخطی hb_vector_svg_rgba_t
hb_vector_svg_rgba_from_hb_color (hb_color_t c)
{
  بازگشت {(اعشاری) hb_color_get_red (c) / 255.f,
          (اعشاری) hb_color_get_green (c) / 255.f,
          (اعشاری) hb_color_get_blue (c) / 255.f,
          (اعشاری) hb_color_get_alpha (c) / 255.f};
}

ایستا درونخطی hb_color_t
hb_vector_svg_hb_color_from_rgba (ثابت hb_vector_svg_rgba_t &c)
{
  unsigned r = (unsigned) roundf (hb_vector_svg_clamp01 (c.r) * 255.f);
  unsigned g = (unsigned) roundf (hb_vector_svg_clamp01 (c.g) * 255.f);
  unsigned b = (unsigned) roundf (hb_vector_svg_clamp01 (c.b) * 255.f);
  unsigned a = (unsigned) roundf (hb_vector_svg_clamp01 (c.a) * 255.f);
  بازگشت HB_COLOR (b, g, r, a);
}

ایستا درونخطی hb_vector_svg_rgba_t
hb_vector_svg_lerp_rgba (ثابت hb_vector_svg_rgba_t &c0,
			 ثابت hb_vector_svg_rgba_t &c1,
			 اعشاری t)
{
  بازگشت {hb_vector_svg_lerp (c0.r, c1.r, t),
          hb_vector_svg_lerp (c0.g, c1.g, t),
          hb_vector_svg_lerp (c0.b, c1.b, t),
          hb_vector_svg_lerp (c0.a, c1.a, t)};
}

ایستا درونخطی اعشاری hb_vector_svg_dot (ثابت hb_vector_svg_point_t &p, ثابت hb_vector_svg_point_t &q) { بازگشت p.x * q.x + p.y * q.y; }
ایستا درونخطی hb_vector_svg_point_t hb_vector_svg_add (ثابت hb_vector_svg_point_t &p, ثابت hb_vector_svg_point_t &q) { بازگشت {p.x + q.x, p.y + q.y}; }
ایستا درونخطی hb_vector_svg_point_t hb_vector_svg_sub (ثابت hb_vector_svg_point_t &p, ثابت hb_vector_svg_point_t &q) { بازگشت {p.x - q.x, p.y - q.y}; }
ایستا درونخطی hb_vector_svg_point_t hb_vector_svg_scale (ثابت hb_vector_svg_point_t &p, اعشاری f) { بازگشت {p.x * f, p.y * f}; }

ایستا درونخطی hb_vector_svg_point_t
hb_vector_svg_normalize (ثابت hb_vector_svg_point_t &p)
{
  اعشاری len = sqrtf (hb_vector_svg_dot (p, p));
  اگر (len == 0.f) بازگشت {0.f, 0.f};
  بازگشت hb_vector_svg_scale (p, 1.f / len);
}

ایستا تهی
hb_vector_svg_add_sweep_patch (hb_vector_buf_t *body,
			       unsigned precision,
			       اعشاری cx, اعشاری cy, اعشاری radius,
			       اعشاری a0, ثابت hb_vector_svg_rgba_t &c0_in,
			       اعشاری a1, ثابت hb_vector_svg_rgba_t &c1_in)
{
  ایستا ثابت اعشاری max_angle = HB_PI / 16.f;
  hb_vector_svg_point_t center = {cx, cy};
  صحیح رقم_splits = (صحیح) ceilf (fabsf (a1 - a0) / max_angle);
  اگر (رقم_splits < 1) رقم_splits = 1;

  hb_vector_svg_point_t p0 = {cosf (a0), sinf (a0)};
  hb_vector_svg_rgba_t color0 = c0_in;

  برای (صحیح a = 0; a < رقم_splits; a++)
  {
    اعشاری k = (a + 1.f) / رقم_splits;
    اعشاری angle1 = hb_vector_svg_lerp (a0, a1, k);
    hb_vector_svg_rgba_t color1 = hb_vector_svg_lerp_rgba (c0_in, c1_in, k);

    hb_vector_svg_point_t p1 = {cosf (angle1), sinf (angle1)};
    hb_vector_svg_point_t sp0 = hb_vector_svg_add (center, hb_vector_svg_scale (p0, radius));
    hb_vector_svg_point_t sp1 = hb_vector_svg_add (center, hb_vector_svg_scale (p1, radius));

    hb_vector_svg_point_t A = hb_vector_svg_normalize (hb_vector_svg_add (p0, p1));
    hb_vector_svg_point_t U = {-A.y, A.x};
    اعشاری up0 = hb_vector_svg_dot (U, p0);
    اعشاری up1 = hb_vector_svg_dot (U, p1);
    اگر (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      ادامه;
    }
    hb_vector_svg_point_t C0 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p0, A), p0) / up0));
    hb_vector_svg_point_t C1 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p1, A), p1) / up1));

    hb_vector_svg_point_t sc0 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C0, hb_vector_svg_scale (hb_vector_svg_sub (C0, p0), 0.33333f)), radius));
    hb_vector_svg_point_t sc1 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C1, hb_vector_svg_scale (hb_vector_svg_sub (C1, p1), 0.33333f)), radius));

    hb_vector_svg_rgba_t mid_color = hb_vector_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_vector_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_رقم (center.x, precision);
    body->append_c (',');
    body->append_رقم (center.y, precision);
    body->append_str ("L");
    body->append_رقم (sp0.x, precision);
    body->append_c (',');
    body->append_رقم (sp0.y, precision);
    body->append_str ("C");
    body->append_رقم (sc0.x, precision);
    body->append_c (',');
    body->append_رقم (sc0.y, precision);
    body->append_c (' ');
    body->append_رقم (sc1.x, precision);
    body->append_c (',');
    body->append_رقم (sc1.y, precision);
    body->append_c (' ');
    body->append_رقم (sp1.x, precision);
    body->append_c (',');
    body->append_رقم (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, درست);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
ساختار hb_vector_svg_sweep_ctx_t
{
  hb_vector_buf_t *body;
  unsigned precision;
  اعشاری cx, cy, radius;
};

ایستا تهی
hb_vector_svg_sweep_emit_patch (اعشاری a0, hb_color_t c0,
				اعشاری a1, hb_color_t c1,
				تهی *user_data)
{
  auto *ctx = (hb_vector_svg_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  اگر (unlikely (ctx->body->in_error ()))
    بازگشت;
  hb_vector_svg_add_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_vector_svg_rgba_from_hb_color (c0),
				 a1, hb_vector_svg_rgba_from_hb_color (c1));
}


ایستا تهی hb_vector_paint_افزودن_transform (hb_paint_funcs_t *, تهی *,
                                            اعشاری, اعشاری, اعشاری, اعشاری, اعشاری, اعشاری,
                                            تهی *);
ایستا تهی hb_vector_paint_برداشت_transform (hb_paint_funcs_t *, تهی *, تهی *);
ایستا تهی hb_vector_paint_fill_glyph (hb_paint_funcs_t *, تهی *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, تهی *);
ایستا تهی hb_vector_paint_افزودن_clip_glyph (hb_paint_funcs_t *, تهی *, hb_codepoint_t, hb_font_t *, تهی *);
ایستا تهی hb_vector_paint_افزودن_clip_rectangle (hb_paint_funcs_t *, تهی *, اعشاری, اعشاری, اعشاری, اعشاری, تهی *);
ایستا hb_draw_funcs_t * hb_vector_paint_افزودن_clip_path_start (hb_paint_funcs_t *, تهی *, تهی **, تهی *);
ایستا تهی hb_vector_paint_افزودن_clip_path_end (hb_paint_funcs_t *, تهی *, تهی *);
ایستا تهی hb_vector_paint_برداشت_clip (hb_paint_funcs_t *, تهی *, تهی *);
ایستا تهی hb_vector_paint_color (hb_paint_funcs_t *, تهی *, hb_bool_t, hb_color_t, تهی *);
ایستا hb_bool_t hb_vector_paint_image (hb_paint_funcs_t *, تهی *, hb_blob_t *, unsigned, unsigned, hb_tag_t, اعشاری, hb_glyph_extents_t *, تهی *);
ایستا تهی hb_vector_paint_linear_gradient (hb_paint_funcs_t *, تهی *, hb_color_سطر_t *, اعشاری, اعشاری, اعشاری, اعشاری, اعشاری, اعشاری, تهی *);
ایستا تهی hb_vector_paint_radial_gradient (hb_paint_funcs_t *, تهی *, hb_color_سطر_t *, اعشاری, اعشاری, اعشاری, اعشاری, اعشاری, اعشاری, تهی *);
ایستا تهی hb_vector_paint_sweep_gradient (hb_paint_funcs_t *, تهی *, hb_color_سطر_t *, اعشاری, اعشاری, اعشاری, اعشاری, تهی *);
ایستا تهی hb_vector_paint_افزودن_group (hb_paint_funcs_t *, تهی *, تهی *);
ایستا تهی hb_vector_paint_برداشت_group (hb_paint_funcs_t *, تهی *, hb_paint_composite_mode_t, تهی *);
ایستا hb_bool_t hb_vector_paint_color_glyph (hb_paint_funcs_t *, تهی *, hb_codepoint_t, hb_font_t *, تهی *);
ایستا hb_bool_t
hb_vector_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      تهی *paint_data,
				      unsigned color_index,
				      hb_color_t *color,
				      تهی *user_data HB_UNUSED)
{
  hb_vector_paint_t *paint = (hb_vector_paint_t *) paint_data;
  اگر (!color)
    بازگشت نادرست;

  hb_color_t *value = nullptr;
  اگر (!paint->custom_palette_colors.has (color_index, &value) || !value)
    بازگشت نادرست;
  *color = *value;
  بازگشت درست;
}

ایستا درونخطی تهی free_static_vector_paint_funcs ();
ایستا ساختار hb_vector_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_vector_paint_funcs_lazy_loader_t>
{
  ایستا hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_افزودن_transform_func (funcs, (hb_paint_افزودن_transform_func_t) hb_vector_paint_افزودن_transform, nullptr, nullptr);
    hb_paint_funcs_set_برداشت_transform_func (funcs, (hb_paint_برداشت_transform_func_t) hb_vector_paint_برداشت_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_vector_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_افزودن_clip_glyph_func (funcs, (hb_paint_افزودن_clip_glyph_func_t) hb_vector_paint_افزودن_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_افزودن_clip_rectangle_func (funcs, (hb_paint_افزودن_clip_rectangle_func_t) hb_vector_paint_افزودن_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_افزودن_clip_path_start_func (funcs, (hb_paint_افزودن_clip_path_start_func_t) hb_vector_paint_افزودن_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_افزودن_clip_path_end_func (funcs, (hb_paint_افزودن_clip_path_end_func_t) hb_vector_paint_افزودن_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_برداشت_clip_func (funcs, (hb_paint_برداشت_clip_func_t) hb_vector_paint_برداشت_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_vector_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_vector_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_vector_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_vector_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_vector_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_افزودن_group_func (funcs, (hb_paint_افزودن_group_func_t) hb_vector_paint_افزودن_group, nullptr, nullptr);
    hb_paint_funcs_set_برداشت_group_func (funcs, (hb_paint_برداشت_group_func_t) hb_vector_paint_برداشت_group, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_vector_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_vector_paint_custom_palette_color, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_paint_funcs);
    بازگشت funcs;
  }
} static_vector_paint_funcs;

ایستا درونخطی تهی
free_static_vector_paint_funcs ()
{
  static_vector_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_svg_funcs_get ()
{
  بازگشت static_vector_paint_funcs.get_unconst ();
}


ایستا تهی
hb_vector_paint_افزودن_transform (hb_paint_funcs_t *,
                                تهی *paint_data,
                                اعشاری xx, اعشاری yx,
                                اعشاری xy, اعشاری yy,
                                اعشاری dx, اعشاری dy,
                                تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;

  اگر (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth++;
    بازگشت;
  }
  اگر (unlikely (paint->transform_group_depth >= 64))
  {
    paint->transform_group_overflow_depth = 1;
    بازگشت;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_group_open_mask = (paint->transform_group_open_mask << 1) | (opened ? 1ull : 0ull);
  paint->transform_group_depth++;

  اگر (!opened)
    بازگشت;

  auto &body = paint->current_body ();
  unsigned sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_رقم (xx, sprec);
  body.append_c (',');
  body.append_رقم (yx, sprec);
  body.append_c (',');
  body.append_رقم (xy, sprec);
  body.append_c (',');
  body.append_رقم (yy, sprec);
  body.append_c (',');
  body.append_رقم (paint->sx (dx));
  body.append_c (',');
  body.append_رقم (paint->sy (dy));
  body.append_str (")\">\n");
}

ایستا تهی
hb_vector_paint_برداشت_transform (hb_paint_funcs_t *,
                               تهی *paint_data,
                               تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;
  اگر (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth--;
    بازگشت;
  }
  اگر (!paint->transform_group_depth)
    بازگشت;
  paint->transform_group_depth--;
  hb_bool_t opened = !!(paint->transform_group_open_mask & 1ull);
  paint->transform_group_open_mask >>= 1;
  اگر (opened)
    paint->current_body ().append_str ("</g>\n");
}

ایستا تهی
hb_vector_paint_fill_glyph (hb_paint_funcs_t *,
                            تهی *paint_data,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  اگر (likely (paint->budget_remaining >= 0))
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
  body.append_svg_color (color, درست);
  body.append_str ("\"/>\n");
}

ایستا تهی
hb_vector_paint_افزودن_clip_glyph (hb_paint_funcs_t *,
                                 تهی *paint_data,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;

  ثابت نویسه *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  اگر (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  unsigned def_id = paint->path_def_شمار++;
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

ایستا تهی
hb_vector_paint_افزودن_clip_rectangle (hb_paint_funcs_t *,
                                     تهی *paint_data,
                                     اعشاری xmin, اعشاری ymin,
                                     اعشاری xmax, اعشاری ymax,
                                     تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;

  ثابت نویسه *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_رقم (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_رقم (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_رقم (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_رقم (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ایستا hb_draw_funcs_t *
hb_vector_paint_افزودن_clip_path_start (hb_paint_funcs_t *,
                                      تهی *paint_data,
                                      تهی **draw_data,
                                      تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    بازگشت nullptr;
  }

  paint->path.clear ();
  paint->clip_path_sink = {&paint->path, paint->get_precision (),
			   paint->x_scale_factor,
			   paint->y_scale_factor,
			   &paint->budget_remaining};
  *draw_data = &paint->clip_path_sink;
  بازگشت hb_vector_svg_path_draw_funcs_get ();
}

ایستا تهی
hb_vector_paint_افزودن_clip_path_end (hb_paint_funcs_t *,
                                    تهی *paint_data,
                                    تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;

  ثابت نویسه *pfx = paint->id_prefix;
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

ایستا تهی
hb_vector_paint_برداشت_clip (hb_paint_funcs_t *,
                          تهی *paint_data,
                          تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;
  paint->current_body ().append_str ("</g>\n");
}

ایستا تهی
hb_vector_paint_color (hb_paint_funcs_t *,
                       تهی *paint_data,
                       hb_bool_t,
                       hb_color_t color,
                       تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;

  auto &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, درست);
  body.append_str ("\"/>\n");
}

ایستا hb_bool_t
hb_vector_paint_image (hb_paint_funcs_t *,
                       تهی *paint_data,
                       hb_blob_t *image,
                       unsigned width,
                       unsigned height,
                       hb_tag_t format,
                       اعشاری slant HB_UNUSED,
                       hb_glyph_extents_t *extents,
                       تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت نادرست;

  auto &body = paint->current_body ();
  اگر (format == HB_TAG ('p','n','g',' '))
  {
    اگر (!extents || !width || !height)
      بازگشت نادرست;

    unsigned len = 0;
    ثابت نویسه *png_data = hb_blob_get_data (image, &len);
    اگر (!png_data || !len)
      بازگشت نادرست;

    body.append_str ("<g transform=\"translate(");
    body.append_رقم (paint->sx ((اعشاری) extents->x_bearing));
    body.append_c (',');
    body.append_رقم (paint->sy ((اعشاری) extents->y_bearing));
    body.append_str (") scale(");
    body.append_رقم (paint->sx ((اعشاری) extents->width) / width);
    body.append_c (',');
    body.append_رقم (paint->sy ((اعشاری) extents->height) / height);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ثابت uint8_t *) png_data, len);
    body.append_str ("\" width=\"");
    body.append_رقم ((اعشاری) width);
    body.append_str ("\" height=\"");
    body.append_رقم ((اعشاری) height);
    body.append_str ("\"/>\n</g>\n");

    بازگشت درست;
  }

  بازگشت نادرست;
}

ایستا تهی
hb_vector_paint_linear_gradient (hb_paint_funcs_t *,
                                 تهی *paint_data,
                                 hb_color_سطر_t *color_سطر,
                                 اعشاری x0, اعشاری y0,
                                 اعشاری x1, اعشاری y1,
                                 اعشاری x2, اعشاری y2,
                                 تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;

  اگر (!paint->fetch_color_stops (color_سطر))
    بازگشت;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.length, &mn, &mx);

  ثابت نویسه *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  اعشاری lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  اعشاری gx0 = lx0 + mn * (lx1 - lx0);
  اعشاری gy0 = ly0 + mn * (ly1 - ly0);
  اعشاری gx1 = lx0 + mx * (lx1 - lx0);
  اعشاری gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_رقم (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_رقم (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_رقم (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_رقم (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_سطر_get_extend (color_سطر)));
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

ایستا تهی
hb_vector_paint_radial_gradient (hb_paint_funcs_t *,
                                 تهی *paint_data,
                                 hb_color_سطر_t *color_سطر,
                                 اعشاری x0, اعشاری y0, اعشاری r0,
                                 اعشاری x1, اعشاری y1, اعشاری r1,
                                 تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;

  اگر (!paint->fetch_color_stops (color_سطر))
    بازگشت;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  اعشاری gx0 = x0 + mn * (x1 - x0);
  اعشاری gy0 = y0 + mn * (y1 - y0);
  اعشاری gr0 = r0 + mn * (r1 - r0);
  اعشاری gx1 = x0 + mx * (x1 - x0);
  اعشاری gy1 = y0 + mx * (y1 - y0);
  اعشاری gr1 = r0 + mx * (r1 - r0);

  ثابت نویسه *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_رقم (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_رقم (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_رقم (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_رقم (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_رقم (paint->sy (gy0));
  اگر (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_رقم (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_سطر_get_extend (color_سطر)));
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

ایستا تهی
hb_vector_paint_sweep_gradient (hb_paint_funcs_t *,
                                تهی *paint_data,
                                hb_color_سطر_t *color_سطر,
                                اعشاری cx, اعشاری cy,
                                اعشاری start_angle, اعشاری end_angle,
                                تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;

  اگر (!paint->fetch_color_stops (color_سطر))
    بازگشت;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  اعشاری ga0 = start_angle + mn * (end_angle - start_angle);
  اعشاری ga1 = start_angle + mx * (end_angle - start_angle);

  hb_vector_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length,
				 hb_color_سطر_get_extend (color_سطر),
				 ga0, ga1,
				 hb_vector_svg_sweep_emit_patch,
				 &ctx);
}

ایستا تهی
hb_vector_paint_افزودن_group (hb_paint_funcs_t *,
                            تهی *paint_data,
                            تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;
  اگر (unlikely (!paint->group_پشته.افزودن_or_fail (hb_vector_buf_t {})))
    بازگشت;
}

ایستا تهی
hb_vector_paint_برداشت_group (hb_paint_funcs_t *,
                           تهی *paint_data,
                           hb_paint_composite_mode_t mode,
                           تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت;
  اگر (paint->group_پشته.length < 2)
    بازگشت;

  hb_vector_buf_t group = paint->group_پشته.برداشت ();
  auto &body = paint->current_body ();

  ثابت نویسه *blend = hb_vector_svg_composite_mode_str (mode);
  اگر (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (group.arrayZ, group.length);
    body.append_str ("</g>\n");
  }
  وگرنه
    body.append_len (group.arrayZ, group.length);
}

ایستا hb_bool_t
hb_vector_paint_color_glyph (hb_paint_funcs_t *,
                             تهی *paint_data,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             تهی *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت نادرست;
  اگر (unlikely (paint->color_glyph_depth >= HB_MAX_NESTING_LEVEL))
    بازگشت نادرست;
  اگر (unlikely (hb_set_has (paint->active_color_glyphs, glyph)))
    بازگشت نادرست;

  paint->color_glyph_depth++;
  hb_set_add (paint->active_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_vector_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_set_del (paint->active_color_glyphs, glyph);
  paint->color_glyph_depth--;
  بازگشت درست;
}




hb_blob_t *
hb_vector_paint_render_svg (hb_vector_paint_t *paint)
{
  اگر (!paint->has_extents)
    بازگشت nullptr;

  اگر (unlikely (!paint->ensure_initialized ()))
    بازگشت nullptr;

  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  unsigned estimated = paint->defs.length +
		       paint->group_پشته.arrayZ[0].length +
		       320;
  out.alloc (estimated);
  اعشاری vb_x = paint->extents.x;
  اعشاری vb_y = -(paint->extents.y + paint->extents.height);
  اعشاری vb_w = paint->extents.width;
  اعشاری vb_h = paint->extents.height;
  out.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  out.append_رقم (vb_x);
  out.append_c (' ');
  out.append_رقم (vb_y);
  out.append_c (' ');
  out.append_رقم (vb_w);
  out.append_c (' ');
  out.append_رقم (vb_h);
  out.append_str ("\" width=\"");
  out.append_رقم (vb_w);
  out.append_str ("\" height=\"");
  out.append_رقم (vb_h);
  out.append_str ("\">\n");

  اگر (paint->defs.length)
  {
    out.append_str ("<defs>\n");
    out.append_len (paint->defs.arrayZ, paint->defs.length);
    out.append_str ("</defs>\n");
  }

  اگر (hb_color_get_alpha (paint->background))
  {
    out.append_str ("<rect x=\"");
    out.append_رقم (vb_x);
    out.append_str ("\" y=\"");
    out.append_رقم (vb_y);
    out.append_str ("\" width=\"");
    out.append_رقم (vb_w);
    out.append_str ("\" height=\"");
    out.append_رقم (vb_h);
    out.append_str ("\" fill=\"rgb(");
    out.append_unsigned (hb_color_get_red (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_green (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_blue (paint->background));
    out.append_str (")\"");
    اگر (hb_color_get_alpha (paint->background) < 255)
    {
      out.append_str (" fill-opacity=\"");
      out.append_رقم (hb_color_get_alpha (paint->background) / 255.f, 4);
      out.append_c ('"');
    }
    out.append_str ("/>\n");
  }

  out.append_str ("<g transform=\"scale(1,-1)\">\n");
  out.append_len (paint->group_پشته.arrayZ[0].arrayZ, paint->group_پشته.arrayZ[0].length);
  out.append_str ("</g>\n");

  out.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  بازگشت blob;
}
