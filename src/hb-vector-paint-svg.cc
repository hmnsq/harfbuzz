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

ساکن ثابت حرف *
hb_سمتیہ_svg_extend_منوال_str (hb_paint_extend_t ext)
{
  انتخاب (ext)
  {
    حالت HB_PAINT_EXTEND_PAD: واپس "pad";
    حالت HB_PAINT_EXTEND_REPEAT: واپس "repeat";
    حالت HB_PAINT_EXTEND_REFLECT: واپس "reflect";
    طےشدہ: واپس "pad";
  }
}

ساکن خالی
hb_سمتیہ_svg_emit_color_stops (hb_سمتیہ_paint_t *paint,
				hb_سمتیہ_buf_t *buf,
				hb_سمتیہ_t<hb_color_stop_t> *stops)
{
  برائے (غیرعلامتی i = 0; i < stops->لمبائی; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_عدد (stops->arrayZ[i].انحراف, 4);
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
      buf->append_عدد (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

ساکن ثابت حرف *
hb_سمتیہ_svg_مرکب_منوال_str (hb_paint_مرکب_منوال_t منوال)
{
  انتخاب (منوال)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    حالت HB_PAINT_مرکب_منوال_PLUS: واپس "screen";
    حالت HB_PAINT_مرکب_منوال_XOR: واپس "difference";
    حالت HB_PAINT_مرکب_منوال_CLEAR:
    حالت HB_PAINT_مرکب_منوال_SRC:
    حالت HB_PAINT_مرکب_منوال_DEST:
    حالت HB_PAINT_مرکب_منوال_DEST_OVER:
    حالت HB_PAINT_مرکب_منوال_SRC_اندر:
    حالت HB_PAINT_مرکب_منوال_DEST_اندر:
    حالت HB_PAINT_مرکب_منوال_SRC_برآمد:
    حالت HB_PAINT_مرکب_منوال_DEST_برآمد:
    حالت HB_PAINT_مرکب_منوال_SRC_ATOP:
    حالت HB_PAINT_مرکب_منوال_DEST_ATOP:
      واپس خالی_اشاریہ;
    حالت HB_PAINT_مرکب_منوال_SRC_OVER: واپس "normal";
    حالت HB_PAINT_مرکب_منوال_SCREEN: واپس "screen";
    حالت HB_PAINT_مرکب_منوال_OVERLAY: واپس "overlay";
    حالت HB_PAINT_مرکب_منوال_DARKEN: واپس "darken";
    حالت HB_PAINT_مرکب_منوال_LIGHTEN: واپس "lighten";
    حالت HB_PAINT_مرکب_منوال_COLOR_DODGE: واپس "color-dodge";
    حالت HB_PAINT_مرکب_منوال_COLOR_BURN: واپس "color-burn";
    حالت HB_PAINT_مرکب_منوال_HARD_LIGHT: واپس "hard-light";
    حالت HB_PAINT_مرکب_منوال_SOFT_LIGHT: واپس "soft-light";
    حالت HB_PAINT_مرکب_منوال_DIFFERENCE: واپس "difference";
    حالت HB_PAINT_مرکب_منوال_EXCLUSION: واپس "exclusion";
    حالت HB_PAINT_مرکب_منوال_MULTIPLY: واپس "multiply";
    حالت HB_PAINT_مرکب_منوال_HSL_HUE: واپس "hue";
    حالت HB_PAINT_مرکب_منوال_HSL_SATURATION: واپس "saturation";
    حالت HB_PAINT_مرکب_منوال_HSL_COLOR: واپس "color";
    حالت HB_PAINT_مرکب_منوال_HSL_LUMINOSITY: واپس "luminosity";
    طےشدہ: واپس خالی_اشاریہ;
  }
}

ساخت hb_سمتیہ_svg_نقطہ_t { اعشاری x, y; };
ساخت hb_سمتیہ_svg_rgba_t { اعشاری r, g, b, a; };

ساکن اندرونی اعشاری
hb_سمتیہ_svg_lerp (اعشاری a, اعشاری b, اعشاری t)
{ واپس a + (b - a) * t; }

ساکن اندرونی اعشاری
hb_سمتیہ_svg_clamp01 (اعشاری v)
{
  اگر (v < 0.f) واپس 0.f;
  اگر (v > 1.f) واپس 1.f;
  واپس v;
}

ساکن اندرونی hb_سمتیہ_svg_rgba_t
hb_سمتیہ_svg_rgba_from_hb_color (hb_color_t c)
{
  واپس {(اعشاری) hb_color_get_red (c) / 255.f,
          (اعشاری) hb_color_get_green (c) / 255.f,
          (اعشاری) hb_color_get_blue (c) / 255.f,
          (اعشاری) hb_color_get_alpha (c) / 255.f};
}

ساکن اندرونی hb_color_t
hb_سمتیہ_svg_hb_color_from_rgba (ثابت hb_سمتیہ_svg_rgba_t &c)
{
  غیرعلامتی r = (غیرعلامتی) roundf (hb_سمتیہ_svg_clamp01 (c.r) * 255.f);
  غیرعلامتی g = (غیرعلامتی) roundf (hb_سمتیہ_svg_clamp01 (c.g) * 255.f);
  غیرعلامتی b = (غیرعلامتی) roundf (hb_سمتیہ_svg_clamp01 (c.b) * 255.f);
  غیرعلامتی a = (غیرعلامتی) roundf (hb_سمتیہ_svg_clamp01 (c.a) * 255.f);
  واپس HB_COLOR (b, g, r, a);
}

ساکن اندرونی hb_سمتیہ_svg_rgba_t
hb_سمتیہ_svg_lerp_rgba (ثابت hb_سمتیہ_svg_rgba_t &c0,
			 ثابت hb_سمتیہ_svg_rgba_t &c1,
			 اعشاری t)
{
  واپس {hb_سمتیہ_svg_lerp (c0.r, c1.r, t),
          hb_سمتیہ_svg_lerp (c0.g, c1.g, t),
          hb_سمتیہ_svg_lerp (c0.b, c1.b, t),
          hb_سمتیہ_svg_lerp (c0.a, c1.a, t)};
}

ساکن اندرونی اعشاری hb_سمتیہ_svg_dot (ثابت hb_سمتیہ_svg_نقطہ_t &p, ثابت hb_سمتیہ_svg_نقطہ_t &q) { واپس p.x * q.x + p.y * q.y; }
ساکن اندرونی hb_سمتیہ_svg_نقطہ_t hb_سمتیہ_svg_جوڑو (ثابت hb_سمتیہ_svg_نقطہ_t &p, ثابت hb_سمتیہ_svg_نقطہ_t &q) { واپس {p.x + q.x, p.y + q.y}; }
ساکن اندرونی hb_سمتیہ_svg_نقطہ_t hb_سمتیہ_svg_sub (ثابت hb_سمتیہ_svg_نقطہ_t &p, ثابت hb_سمتیہ_svg_نقطہ_t &q) { واپس {p.x - q.x, p.y - q.y}; }
ساکن اندرونی hb_سمتیہ_svg_نقطہ_t hb_سمتیہ_svg_scale (ثابت hb_سمتیہ_svg_نقطہ_t &p, اعشاری f) { واپس {p.x * f, p.y * f}; }

ساکن اندرونی hb_سمتیہ_svg_نقطہ_t
hb_سمتیہ_svg_normalize (ثابت hb_سمتیہ_svg_نقطہ_t &p)
{
  اعشاری len = sqrtf (hb_سمتیہ_svg_dot (p, p));
  اگر (len == 0.f) واپس {0.f, 0.f};
  واپس hb_سمتیہ_svg_scale (p, 1.f / len);
}

ساکن خالی
hb_سمتیہ_svg_جوڑو_sweep_patch (hb_سمتیہ_buf_t *body,
			       غیرعلامتی precision,
			       اعشاری cx, اعشاری cy, اعشاری radius,
			       اعشاری a0, ثابت hb_سمتیہ_svg_rgba_t &c0_اندر,
			       اعشاری a1, ثابت hb_سمتیہ_svg_rgba_t &c1_اندر)
{
  ساکن ثابت اعشاری max_angle = HB_PI / 16.f;
  hb_سمتیہ_svg_نقطہ_t center = {cx, cy};
  صحیح عدد_splits = (صحیح) ceilf (fabsf (a1 - a0) / max_angle);
  اگر (عدد_splits < 1) عدد_splits = 1;

  hb_سمتیہ_svg_نقطہ_t p0 = {cosf (a0), sinf (a0)};
  hb_سمتیہ_svg_rgba_t color0 = c0_اندر;

  برائے (صحیح a = 0; a < عدد_splits; a++)
  {
    اعشاری k = (a + 1.f) / عدد_splits;
    اعشاری angle1 = hb_سمتیہ_svg_lerp (a0, a1, k);
    hb_سمتیہ_svg_rgba_t color1 = hb_سمتیہ_svg_lerp_rgba (c0_اندر, c1_اندر, k);

    hb_سمتیہ_svg_نقطہ_t p1 = {cosf (angle1), sinf (angle1)};
    hb_سمتیہ_svg_نقطہ_t sp0 = hb_سمتیہ_svg_جوڑو (center, hb_سمتیہ_svg_scale (p0, radius));
    hb_سمتیہ_svg_نقطہ_t sp1 = hb_سمتیہ_svg_جوڑو (center, hb_سمتیہ_svg_scale (p1, radius));

    hb_سمتیہ_svg_نقطہ_t A = hb_سمتیہ_svg_normalize (hb_سمتیہ_svg_جوڑو (p0, p1));
    hb_سمتیہ_svg_نقطہ_t U = {-A.y, A.x};
    اعشاری up0 = hb_سمتیہ_svg_dot (U, p0);
    اعشاری up1 = hb_سمتیہ_svg_dot (U, p1);
    اگر (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      جاری;
    }
    hb_سمتیہ_svg_نقطہ_t C0 = hb_سمتیہ_svg_جوڑو (A, hb_سمتیہ_svg_scale (U, hb_سمتیہ_svg_dot (hb_سمتیہ_svg_sub (p0, A), p0) / up0));
    hb_سمتیہ_svg_نقطہ_t C1 = hb_سمتیہ_svg_جوڑو (A, hb_سمتیہ_svg_scale (U, hb_سمتیہ_svg_dot (hb_سمتیہ_svg_sub (p1, A), p1) / up1));

    hb_سمتیہ_svg_نقطہ_t sc0 = hb_سمتیہ_svg_جوڑو (center, hb_سمتیہ_svg_scale (hb_سمتیہ_svg_جوڑو (C0, hb_سمتیہ_svg_scale (hb_سمتیہ_svg_sub (C0, p0), 0.33333f)), radius));
    hb_سمتیہ_svg_نقطہ_t sc1 = hb_سمتیہ_svg_جوڑو (center, hb_سمتیہ_svg_scale (hb_سمتیہ_svg_جوڑو (C1, hb_سمتیہ_svg_scale (hb_سمتیہ_svg_sub (C1, p1), 0.33333f)), radius));

    hb_سمتیہ_svg_rgba_t mid_color = hb_سمتیہ_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_سمتیہ_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_عدد (center.x, precision);
    body->append_c (',');
    body->append_عدد (center.y, precision);
    body->append_str ("L");
    body->append_عدد (sp0.x, precision);
    body->append_c (',');
    body->append_عدد (sp0.y, precision);
    body->append_str ("C");
    body->append_عدد (sc0.x, precision);
    body->append_c (',');
    body->append_عدد (sc0.y, precision);
    body->append_c (' ');
    body->append_عدد (sc1.x, precision);
    body->append_c (',');
    body->append_عدد (sc1.y, precision);
    body->append_c (' ');
    body->append_عدد (sp1.x, precision);
    body->append_c (',');
    body->append_عدد (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, سچ);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
ساخت hb_سمتیہ_svg_sweep_ctx_t
{
  hb_سمتیہ_buf_t *body;
  غیرعلامتی precision;
  اعشاری cx, cy, radius;
};

ساکن خالی
hb_سمتیہ_svg_sweep_emit_patch (اعشاری a0, hb_color_t c0,
				اعشاری a1, hb_color_t c1,
				خالی *user_ڈیٹا)
{
  خودکار *ctx = (hb_سمتیہ_svg_sweep_ctx_t *) user_ڈیٹا;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  اگر (unlikely (ctx->body->اندر_error ()))
    واپس;
  hb_سمتیہ_svg_جوڑو_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_سمتیہ_svg_rgba_from_hb_color (c0),
				 a1, hb_سمتیہ_svg_rgba_from_hb_color (c1));
}


ساکن خالی hb_سمتیہ_paint_ڈالو_transform (hb_paint_funcs_t *, خالی *,
                                            اعشاری, اعشاری, اعشاری, اعشاری, اعشاری, اعشاری,
                                            خالی *);
ساکن خالی hb_سمتیہ_paint_نکالو_transform (hb_paint_funcs_t *, خالی *, خالی *);
ساکن خالی hb_سمتیہ_paint_fill_glyph (hb_paint_funcs_t *, خالی *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, خالی *);
ساکن خالی hb_سمتیہ_paint_ڈالو_clip_glyph (hb_paint_funcs_t *, خالی *, hb_codepoint_t, hb_font_t *, خالی *);
ساکن خالی hb_سمتیہ_paint_ڈالو_clip_rectangle (hb_paint_funcs_t *, خالی *, اعشاری, اعشاری, اعشاری, اعشاری, خالی *);
ساکن hb_draw_funcs_t * hb_سمتیہ_paint_ڈالو_clip_راستہ_آغاز (hb_paint_funcs_t *, خالی *, خالی **, خالی *);
ساکن خالی hb_سمتیہ_paint_ڈالو_clip_راستہ_انتہا (hb_paint_funcs_t *, خالی *, خالی *);
ساکن خالی hb_سمتیہ_paint_نکالو_clip (hb_paint_funcs_t *, خالی *, خالی *);
ساکن خالی hb_سمتیہ_paint_color (hb_paint_funcs_t *, خالی *, hb_bool_t, hb_color_t, خالی *);
ساکن hb_bool_t hb_سمتیہ_paint_image (hb_paint_funcs_t *, خالی *, hb_blob_t *, غیرعلامتی, غیرعلامتی, hb_لیبل_t, اعشاری, hb_glyph_extents_t *, خالی *);
ساکن خالی hb_سمتیہ_paint_خطی_gradient (hb_paint_funcs_t *, خالی *, hb_color_سطر_t *, اعشاری, اعشاری, اعشاری, اعشاری, اعشاری, اعشاری, خالی *);
ساکن خالی hb_سمتیہ_paint_radial_gradient (hb_paint_funcs_t *, خالی *, hb_color_سطر_t *, اعشاری, اعشاری, اعشاری, اعشاری, اعشاری, اعشاری, خالی *);
ساکن خالی hb_سمتیہ_paint_sweep_gradient (hb_paint_funcs_t *, خالی *, hb_color_سطر_t *, اعشاری, اعشاری, اعشاری, اعشاری, خالی *);
ساکن خالی hb_سمتیہ_paint_ڈالو_گروہ (hb_paint_funcs_t *, خالی *, خالی *);
ساکن خالی hb_سمتیہ_paint_نکالو_گروہ (hb_paint_funcs_t *, خالی *, hb_paint_مرکب_منوال_t, خالی *);
ساکن hb_bool_t hb_سمتیہ_paint_color_glyph (hb_paint_funcs_t *, خالی *, hb_codepoint_t, hb_font_t *, خالی *);
ساکن hb_bool_t
hb_سمتیہ_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
				      خالی *paint_ڈیٹا,
				      غیرعلامتی color_اشاریہ,
				      hb_color_t *color,
				      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_سمتیہ_paint_t *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (!color)
    واپس جھوٹ;

  hb_color_t *value = خالی_اشاریہ;
  اگر (!paint->custom_palette_colors.has (color_اشاریہ, &value) || !value)
    واپس جھوٹ;
  *color = *value;
  واپس سچ;
}

ساکن اندرونی خالی free_static_سمتیہ_paint_funcs ();
ساکن ساخت hb_سمتیہ_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_سمتیہ_paint_funcs_lazy_loader_t>
{
  ساکن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_سیٹ_ڈالو_transform_func (funcs, (hb_paint_ڈالو_transform_func_t) hb_سمتیہ_paint_ڈالو_transform, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_transform_func (funcs, (hb_paint_نکالو_transform_func_t) hb_سمتیہ_paint_نکالو_transform, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_سمتیہ_paint_fill_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, (hb_paint_ڈالو_clip_glyph_func_t) hb_سمتیہ_paint_ڈالو_clip_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, (hb_paint_ڈالو_clip_rectangle_func_t) hb_سمتیہ_paint_ڈالو_clip_rectangle, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_راستہ_آغاز_func (funcs, (hb_paint_ڈالو_clip_راستہ_آغاز_func_t) hb_سمتیہ_paint_ڈالو_clip_راستہ_آغاز, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_راستہ_انتہا_func (funcs, (hb_paint_ڈالو_clip_راستہ_انتہا_func_t) hb_سمتیہ_paint_ڈالو_clip_راستہ_انتہا, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, (hb_paint_نکالو_clip_func_t) hb_سمتیہ_paint_نکالو_clip, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_color_func (funcs, (hb_paint_color_func_t) hb_سمتیہ_paint_color, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_image_func (funcs, (hb_paint_image_func_t) hb_سمتیہ_paint_image, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, (hb_paint_خطی_gradient_func_t) hb_سمتیہ_paint_خطی_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_سمتیہ_paint_radial_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_سمتیہ_paint_sweep_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, (hb_paint_ڈالو_گروہ_func_t) hb_سمتیہ_paint_ڈالو_گروہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, (hb_paint_نکالو_گروہ_func_t) hb_سمتیہ_paint_نکالو_گروہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_سمتیہ_paint_color_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_سمتیہ_paint_custom_palette_color, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_سمتیہ_paint_سیٹ_budget_callback, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_سمتیہ_paint_get_budget_callback, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_سمتیہ_paint_get_budget_بقایا_callback, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_سمتیہ_paint_funcs);
    واپس funcs;
  }
} static_سمتیہ_paint_funcs;

ساکن اندرونی خالی
free_static_سمتیہ_paint_funcs ()
{
  static_سمتیہ_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_سمتیہ_paint_svg_funcs_get ()
{
  واپس static_سمتیہ_paint_funcs.get_unconst ();
}


ساکن خالی
hb_سمتیہ_paint_ڈالو_transform (hb_paint_funcs_t *,
                                خالی *paint_ڈیٹا,
                                اعشاری xx, اعشاری yx,
                                اعشاری xy, اعشاری yy,
                                اعشاری dx, اعشاری dy,
                                خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  اگر (unlikely (paint->transform_گروہ_حد_سے_تجاوز_گہرائی))
  {
    paint->transform_گروہ_حد_سے_تجاوز_گہرائی++;
    واپس;
  }
  اگر (unlikely (paint->transform_گروہ_گہرائی >= 64))
  {
    paint->transform_گروہ_حد_سے_تجاوز_گہرائی = 1;
    واپس;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_گروہ_open_نقاب = (paint->transform_گروہ_open_نقاب << 1) | (opened ? 1ull : 0ull);
  paint->transform_گروہ_گہرائی++;

  اگر (!opened)
    واپس;

  خودکار &body = paint->current_body ();
  غیرعلامتی sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_عدد (xx, sprec);
  body.append_c (',');
  body.append_عدد (yx, sprec);
  body.append_c (',');
  body.append_عدد (xy, sprec);
  body.append_c (',');
  body.append_عدد (yy, sprec);
  body.append_c (',');
  body.append_عدد (paint->sx (dx));
  body.append_c (',');
  body.append_عدد (paint->sy (dy));
  body.append_str (")\">\n");
}

ساکن خالی
hb_سمتیہ_paint_نکالو_transform (hb_paint_funcs_t *,
                               خالی *paint_ڈیٹا,
                               خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  اگر (unlikely (paint->transform_گروہ_حد_سے_تجاوز_گہرائی))
  {
    paint->transform_گروہ_حد_سے_تجاوز_گہرائی--;
    واپس;
  }
  اگر (!paint->transform_گروہ_گہرائی)
    واپس;
  paint->transform_گروہ_گہرائی--;
  hb_bool_t opened = !!(paint->transform_گروہ_open_نقاب & 1ull);
  paint->transform_گروہ_open_نقاب >>= 1;
  اگر (opened)
    paint->current_body ().append_str ("</g>\n");
}

ساکن خالی
hb_سمتیہ_paint_fill_glyph (hb_paint_funcs_t *,
                            خالی *paint_ڈیٹا,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  paint->راستہ.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  اگر (likely (paint->budget_بقایا >= 0))
  {
    hb_سمتیہ_راستہ_sink_t sink = {&paint->راستہ, paint->get_precision (),
				 paint->x_scale_ضربی_عدد, paint->y_scale_ضربی_عدد,
				 &paint->budget_بقایا};
    hb_font_draw_glyph (font, glyph, hb_سمتیہ_svg_راستہ_draw_funcs_get (), &sink);
  }

  خودکار &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->راستہ.arrayZ, paint->راستہ.لمبائی);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, سچ);
  body.append_str ("\"/>\n");
}

ساکن خالی
hb_سمتیہ_paint_ڈالو_clip_glyph (hb_paint_funcs_t *,
                                 خالی *paint_ڈیٹا,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  ثابت حرف *pfx = paint->id_سابقہ;
  غیرعلامتی pfx_len = paint->id_سابقہ_لمبائی;

  paint->راستہ.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  اگر (likely (paint->budget_بقایا >= 0))
  {
    hb_سمتیہ_راستہ_sink_t sink = {&paint->راستہ, paint->get_precision (),
				 paint->x_scale_ضربی_عدد, paint->y_scale_ضربی_عدد,
				 &paint->budget_بقایا};
    hb_font_draw_glyph (font, glyph, hb_سمتیہ_svg_راستہ_draw_funcs_get (), &sink);
  }

  غیرعلامتی def_id = paint->راستہ_def_گنتی++;
  paint->defs.append_str ("<path id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\" d=\"");
  paint->defs.append_len (paint->راستہ.arrayZ, paint->راستہ.لمبائی);
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

ساکن خالی
hb_سمتیہ_paint_ڈالو_clip_rectangle (hb_paint_funcs_t *,
                                     خالی *paint_ڈیٹا,
                                     اعشاری xmin, اعشاری ymin,
                                     اعشاری xmax, اعشاری ymax,
                                     خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  ثابت حرف *pfx = paint->id_سابقہ;
  غیرعلامتی pfx_len = paint->id_سابقہ_لمبائی;
  غیرعلامتی clip_id = paint->clip_rect_شمار++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_عدد (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_عدد (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_عدد (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_عدد (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ساکن hb_draw_funcs_t *
hb_سمتیہ_paint_ڈالو_clip_راستہ_آغاز (hb_paint_funcs_t *,
                                      خالی *paint_ڈیٹا,
                                      خالی **draw_ڈیٹا,
                                      خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
  {
    *draw_ڈیٹا = خالی_اشاریہ;
    واپس خالی_اشاریہ;
  }

  paint->راستہ.clear ();
  paint->clip_راستہ_sink = {&paint->راستہ, paint->get_precision (),
			   paint->x_scale_ضربی_عدد,
			   paint->y_scale_ضربی_عدد,
			   &paint->budget_بقایا};
  *draw_ڈیٹا = &paint->clip_راستہ_sink;
  واپس hb_سمتیہ_svg_راستہ_draw_funcs_get ();
}

ساکن خالی
hb_سمتیہ_paint_ڈالو_clip_راستہ_انتہا (hb_paint_funcs_t *,
                                    خالی *paint_ڈیٹا,
                                    خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  ثابت حرف *pfx = paint->id_سابقہ;
  غیرعلامتی pfx_len = paint->id_سابقہ_لمبائی;
  غیرعلامتی clip_id = paint->clip_راستہ_شمار++;

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
  paint->defs.append_len (paint->راستہ.arrayZ, paint->راستہ.لمبائی);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("cp");
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ساکن خالی
hb_سمتیہ_paint_نکالو_clip (hb_paint_funcs_t *,
                          خالی *paint_ڈیٹا,
                          خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  paint->current_body ().append_str ("</g>\n");
}

ساکن خالی
hb_سمتیہ_paint_color (hb_paint_funcs_t *,
                       خالی *paint_ڈیٹا,
                       hb_bool_t,
                       hb_color_t color,
                       خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  خودکار &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, سچ);
  body.append_str ("\"/>\n");
}

ساکن hb_bool_t
hb_سمتیہ_paint_image (hb_paint_funcs_t *,
                       خالی *paint_ڈیٹا,
                       hb_blob_t *image,
                       غیرعلامتی چوڑائی,
                       غیرعلامتی اونچائی,
                       hb_لیبل_t format,
                       اعشاری slant HB_غیر_مستعمل,
                       hb_glyph_extents_t *extents,
                       خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس جھوٹ;

  خودکار &body = paint->current_body ();
  اگر (format == HB_لیبل ('p','n','g',' '))
  {
    اگر (!extents || !چوڑائی || !اونچائی)
      واپس جھوٹ;

    غیرعلامتی len = 0;
    ثابت حرف *png_ڈیٹا = hb_blob_get_ڈیٹا (image, &len);
    اگر (!png_ڈیٹا || !len)
      واپس جھوٹ;

    body.append_str ("<g transform=\"translate(");
    body.append_عدد (paint->sx ((اعشاری) extents->x_bearing));
    body.append_c (',');
    body.append_عدد (paint->sy ((اعشاری) extents->y_bearing));
    body.append_str (") scale(");
    body.append_عدد (paint->sx ((اعشاری) extents->چوڑائی) / چوڑائی);
    body.append_c (',');
    body.append_عدد (paint->sy ((اعشاری) extents->اونچائی) / اونچائی);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ثابت uint8_t *) png_ڈیٹا, len);
    body.append_str ("\" width=\"");
    body.append_عدد ((اعشاری) چوڑائی);
    body.append_str ("\" height=\"");
    body.append_عدد ((اعشاری) اونچائی);
    body.append_str ("\"/>\n</g>\n");

    واپس سچ;
  }

  واپس جھوٹ;
}

ساکن خالی
hb_سمتیہ_paint_خطی_gradient (hb_paint_funcs_t *,
                                 خالی *paint_ڈیٹا,
                                 hb_color_سطر_t *color_سطر,
                                 اعشاری x0, اعشاری y0,
                                 اعشاری x1, اعشاری y1,
                                 اعشاری x2, اعشاری y2,
                                 خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  اگر (!paint->fetch_color_stops (color_سطر))
    واپس;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.لمبائی, &mn, &mx);

  ثابت حرف *pfx = paint->id_سابقہ;
  غیرعلامتی pfx_len = paint->id_سابقہ_لمبائی;
  غیرعلامتی grad_id = paint->gradient_شمار++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  اعشاری lx0, ly0, lx1, ly1;
  hb_paint_reduce_خطی_anchors (x0, y0, x1, y1, x2, y2,
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
  paint->defs.append_عدد (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_عدد (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_عدد (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_عدد (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_سمتیہ_svg_extend_منوال_str (hb_color_سطر_get_extend (color_سطر)));
  paint->defs.append_str ("\">\n");
  hb_سمتیہ_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</linearGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

ساکن خالی
hb_سمتیہ_paint_radial_gradient (hb_paint_funcs_t *,
                                 خالی *paint_ڈیٹا,
                                 hb_color_سطر_t *color_سطر,
                                 اعشاری x0, اعشاری y0, اعشاری r0,
                                 اعشاری x1, اعشاری y1, اعشاری r1,
                                 خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  اگر (!paint->fetch_color_stops (color_سطر))
    واپس;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.لمبائی, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  اعشاری gx0 = x0 + mn * (x1 - x0);
  اعشاری gy0 = y0 + mn * (y1 - y0);
  اعشاری gr0 = r0 + mn * (r1 - r0);
  اعشاری gx1 = x0 + mx * (x1 - x0);
  اعشاری gy1 = y0 + mx * (y1 - y0);
  اعشاری gr1 = r0 + mx * (r1 - r0);

  ثابت حرف *pfx = paint->id_سابقہ;
  غیرعلامتی pfx_len = paint->id_سابقہ_لمبائی;
  غیرعلامتی grad_id = paint->gradient_شمار++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_عدد (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_عدد (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_عدد (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_عدد (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_عدد (paint->sy (gy0));
  اگر (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_عدد (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_سمتیہ_svg_extend_منوال_str (hb_color_سطر_get_extend (color_سطر)));
  paint->defs.append_str ("\">\n");
  hb_سمتیہ_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</radialGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

ساکن خالی
hb_سمتیہ_paint_sweep_gradient (hb_paint_funcs_t *,
                                خالی *paint_ڈیٹا,
                                hb_color_سطر_t *color_سطر,
                                اعشاری cx, اعشاری cy,
                                اعشاری آغاز_angle, اعشاری انتہا_angle,
                                خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  اگر (!paint->fetch_color_stops (color_سطر))
    واپس;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.لمبائی, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  اعشاری ga0 = آغاز_angle + mn * (انتہا_angle - آغاز_angle);
  اعشاری ga1 = آغاز_angle + mx * (انتہا_angle - آغاز_angle);

  hb_سمتیہ_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.لمبائی,
				 hb_color_سطر_get_extend (color_سطر),
				 ga0, ga1,
				 hb_سمتیہ_svg_sweep_emit_patch,
				 &ctx);
}

ساکن خالی
hb_سمتیہ_paint_ڈالو_گروہ (hb_paint_funcs_t *,
                            خالی *paint_ڈیٹا,
                            خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  اگر (unlikely (!paint->گروہ_ڈھیر.ڈالو_or_fail (hb_سمتیہ_buf_t {})))
    واپس;
}

ساکن خالی
hb_سمتیہ_paint_نکالو_گروہ (hb_paint_funcs_t *,
                           خالی *paint_ڈیٹا,
                           hb_paint_مرکب_منوال_t منوال,
                           خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  اگر (paint->گروہ_ڈھیر.لمبائی < 2)
    واپس;

  hb_سمتیہ_buf_t گروہ = paint->گروہ_ڈھیر.نکالو ();
  خودکار &body = paint->current_body ();

  ثابت حرف *blend = hb_سمتیہ_svg_مرکب_منوال_str (منوال);
  اگر (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (گروہ.arrayZ, گروہ.لمبائی);
    body.append_str ("</g>\n");
  }
  ورنہ
    body.append_len (گروہ.arrayZ, گروہ.لمبائی);
}

ساکن hb_bool_t
hb_سمتیہ_paint_color_glyph (hb_paint_funcs_t *,
                             خالی *paint_ڈیٹا,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس جھوٹ;
  اگر (unlikely (paint->color_glyph_گہرائی >= HB_MAX_NESTING_LEVEL))
    واپس جھوٹ;
  اگر (unlikely (hb_سیٹ_has (paint->فعال_color_glyphs, glyph)))
    واپس جھوٹ;

  paint->color_glyph_گہرائی++;
  hb_سیٹ_جوڑو (paint->فعال_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_سمتیہ_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_سیٹ_del (paint->فعال_color_glyphs, glyph);
  paint->color_glyph_گہرائی--;
  واپس سچ;
}




hb_blob_t *
hb_سمتیہ_paint_render_svg (hb_سمتیہ_paint_t *paint)
{
  اگر (!paint->has_extents)
    واپس خالی_اشاریہ;

  اگر (unlikely (!paint->ensure_initialized ()))
    واپس خالی_اشاریہ;

  hb_سمتیہ_buf_t برآمد;
  hb_buf_recover_recycled (paint->recycled_blob, &برآمد);
  غیرعلامتی estimated = paint->defs.لمبائی +
		       paint->گروہ_ڈھیر.arrayZ[0].لمبائی +
		       320;
  برآمد.alloc (estimated);
  اعشاری vb_x = paint->extents.x;
  اعشاری vb_y = -(paint->extents.y + paint->extents.اونچائی);
  اعشاری vb_w = paint->extents.چوڑائی;
  اعشاری vb_h = paint->extents.اونچائی;
  برآمد.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  برآمد.append_عدد (vb_x);
  برآمد.append_c (' ');
  برآمد.append_عدد (vb_y);
  برآمد.append_c (' ');
  برآمد.append_عدد (vb_w);
  برآمد.append_c (' ');
  برآمد.append_عدد (vb_h);
  برآمد.append_str ("\" width=\"");
  برآمد.append_عدد (vb_w);
  برآمد.append_str ("\" height=\"");
  برآمد.append_عدد (vb_h);
  برآمد.append_str ("\">\n");

  اگر (paint->defs.لمبائی)
  {
    برآمد.append_str ("<defs>\n");
    برآمد.append_len (paint->defs.arrayZ, paint->defs.لمبائی);
    برآمد.append_str ("</defs>\n");
  }

  اگر (hb_color_get_alpha (paint->background))
  {
    برآمد.append_str ("<rect x=\"");
    برآمد.append_عدد (vb_x);
    برآمد.append_str ("\" y=\"");
    برآمد.append_عدد (vb_y);
    برآمد.append_str ("\" width=\"");
    برآمد.append_عدد (vb_w);
    برآمد.append_str ("\" height=\"");
    برآمد.append_عدد (vb_h);
    برآمد.append_str ("\" fill=\"rgb(");
    برآمد.append_unsigned (hb_color_get_red (paint->background));
    برآمد.append_c (',');
    برآمد.append_unsigned (hb_color_get_green (paint->background));
    برآمد.append_c (',');
    برآمد.append_unsigned (hb_color_get_blue (paint->background));
    برآمد.append_str (")\"");
    اگر (hb_color_get_alpha (paint->background) < 255)
    {
      برآمد.append_str (" fill-opacity=\"");
      برآمد.append_عدد (hb_color_get_alpha (paint->background) / 255.f, 4);
      برآمد.append_c ('"');
    }
    برآمد.append_str ("/>\n");
  }

  برآمد.append_str ("<g transform=\"scale(1,-1)\">\n");
  برآمد.append_len (paint->گروہ_ڈھیر.arrayZ[0].arrayZ, paint->گروہ_ڈھیر.arrayZ[0].لمبائی);
  برآمد.append_str ("</g>\n");

  برآمد.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &برآمد);

  واپس blob;
}
