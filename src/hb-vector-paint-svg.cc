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
hb_سمتیہ_svg_extend_منوال_str (hb_paint_extend_t ext)
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
hb_سمتیہ_svg_emit_color_stops (hb_سمتیہ_paint_t *paint,
				hb_سمتیہ_buf_t *buf,
				hb_سمتیہ_t<hb_color_stop_t> *stops)
{
  لكل (بنانشانی i = 0; i < stops->لمائی; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_عدد (stops->arrayZ[i].وتھ, 4);
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
      buf->append_عدد (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

ساكن ثابت حرف *
hb_سمتیہ_svg_مرکب_منوال_str (hb_paint_مرکب_منوال_t منوال)
{
  بدل (منوال)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    حالة HB_PAINT_مرکب_منوال_PLUS: أرجع "screen";
    حالة HB_PAINT_مرکب_منوال_XOR: أرجع "difference";
    حالة HB_PAINT_مرکب_منوال_CLEAR:
    حالة HB_PAINT_مرکب_منوال_SRC:
    حالة HB_PAINT_مرکب_منوال_DEST:
    حالة HB_PAINT_مرکب_منوال_DEST_OVER:
    حالة HB_PAINT_مرکب_منوال_SRC_وچ:
    حالة HB_PAINT_مرکب_منوال_DEST_وچ:
    حالة HB_PAINT_مرکب_منوال_SRC_باہرلا:
    حالة HB_PAINT_مرکب_منوال_DEST_باہرلا:
    حالة HB_PAINT_مرکب_منوال_SRC_ATOP:
    حالة HB_PAINT_مرکب_منوال_DEST_ATOP:
      أرجع خالی_سوچک;
    حالة HB_PAINT_مرکب_منوال_SRC_OVER: أرجع "normal";
    حالة HB_PAINT_مرکب_منوال_SCREEN: أرجع "screen";
    حالة HB_PAINT_مرکب_منوال_OVERLAY: أرجع "overlay";
    حالة HB_PAINT_مرکب_منوال_DARKEN: أرجع "darken";
    حالة HB_PAINT_مرکب_منوال_LIGHTEN: أرجع "lighten";
    حالة HB_PAINT_مرکب_منوال_COLOR_DODGE: أرجع "color-dodge";
    حالة HB_PAINT_مرکب_منوال_COLOR_BURN: أرجع "color-burn";
    حالة HB_PAINT_مرکب_منوال_HARD_LIGHT: أرجع "hard-light";
    حالة HB_PAINT_مرکب_منوال_SOFT_LIGHT: أرجع "soft-light";
    حالة HB_PAINT_مرکب_منوال_DIFFERENCE: أرجع "difference";
    حالة HB_PAINT_مرکب_منوال_EXCLUSION: أرجع "exclusion";
    حالة HB_PAINT_مرکب_منوال_MULTIPLY: أرجع "multiply";
    حالة HB_PAINT_مرکب_منوال_HSL_HUE: أرجع "hue";
    حالة HB_PAINT_مرکب_منوال_HSL_SATURATION: أرجع "saturation";
    حالة HB_PAINT_مرکب_منوال_HSL_COLOR: أرجع "color";
    حالة HB_PAINT_مرکب_منوال_HSL_LUMINOSITY: أرجع "luminosity";
    افتراضي: أرجع خالی_سوچک;
  }
}

بنية hb_سمتیہ_svg_نقطہ_t { عائم x, y; };
بنية hb_سمتیہ_svg_rgba_t { عائم r, g, b, a; };

ساكن مضمن عائم
hb_سمتیہ_svg_lerp (عائم a, عائم b, عائم t)
{ أرجع a + (b - a) * t; }

ساكن مضمن عائم
hb_سمتیہ_svg_clamp01 (عائم v)
{
  إذا (v < 0.f) أرجع 0.f;
  إذا (v > 1.f) أرجع 1.f;
  أرجع v;
}

ساكن مضمن hb_سمتیہ_svg_rgba_t
hb_سمتیہ_svg_rgba_from_hb_color (hb_color_t c)
{
  أرجع {(عائم) hb_color_get_red (c) / 255.f,
          (عائم) hb_color_get_green (c) / 255.f,
          (عائم) hb_color_get_blue (c) / 255.f,
          (عائم) hb_color_get_alpha (c) / 255.f};
}

ساكن مضمن hb_color_t
hb_سمتیہ_svg_hb_color_from_rgba (ثابت hb_سمتیہ_svg_rgba_t &c)
{
  بنانشانی r = (بنانشانی) roundf (hb_سمتیہ_svg_clamp01 (c.r) * 255.f);
  بنانشانی g = (بنانشانی) roundf (hb_سمتیہ_svg_clamp01 (c.g) * 255.f);
  بنانشانی b = (بنانشانی) roundf (hb_سمتیہ_svg_clamp01 (c.b) * 255.f);
  بنانشانی a = (بنانشانی) roundf (hb_سمتیہ_svg_clamp01 (c.a) * 255.f);
  أرجع HB_COLOR (b, g, r, a);
}

ساكن مضمن hb_سمتیہ_svg_rgba_t
hb_سمتیہ_svg_lerp_rgba (ثابت hb_سمتیہ_svg_rgba_t &c0,
			 ثابت hb_سمتیہ_svg_rgba_t &c1,
			 عائم t)
{
  أرجع {hb_سمتیہ_svg_lerp (c0.r, c1.r, t),
          hb_سمتیہ_svg_lerp (c0.g, c1.g, t),
          hb_سمتیہ_svg_lerp (c0.b, c1.b, t),
          hb_سمتیہ_svg_lerp (c0.a, c1.a, t)};
}

ساكن مضمن عائم hb_سمتیہ_svg_dot (ثابت hb_سمتیہ_svg_نقطہ_t &p, ثابت hb_سمتیہ_svg_نقطہ_t &q) { أرجع p.x * q.x + p.y * q.y; }
ساكن مضمن hb_سمتیہ_svg_نقطہ_t hb_سمتیہ_svg_جوڑو (ثابت hb_سمتیہ_svg_نقطہ_t &p, ثابت hb_سمتیہ_svg_نقطہ_t &q) { أرجع {p.x + q.x, p.y + q.y}; }
ساكن مضمن hb_سمتیہ_svg_نقطہ_t hb_سمتیہ_svg_sub (ثابت hb_سمتیہ_svg_نقطہ_t &p, ثابت hb_سمتیہ_svg_نقطہ_t &q) { أرجع {p.x - q.x, p.y - q.y}; }
ساكن مضمن hb_سمتیہ_svg_نقطہ_t hb_سمتیہ_svg_scale (ثابت hb_سمتیہ_svg_نقطہ_t &p, عائم f) { أرجع {p.x * f, p.y * f}; }

ساكن مضمن hb_سمتیہ_svg_نقطہ_t
hb_سمتیہ_svg_normalize (ثابت hb_سمتیہ_svg_نقطہ_t &p)
{
  عائم len = sqrtf (hb_سمتیہ_svg_dot (p, p));
  إذا (len == 0.f) أرجع {0.f, 0.f};
  أرجع hb_سمتیہ_svg_scale (p, 1.f / len);
}

ساكن فراغ
hb_سمتیہ_svg_جوڑو_sweep_patch (hb_سمتیہ_buf_t *body,
			       بنانشانی precision,
			       عائم cx, عائم cy, عائم radius,
			       عائم a0, ثابت hb_سمتیہ_svg_rgba_t &c0_وچ,
			       عائم a1, ثابت hb_سمتیہ_svg_rgba_t &c1_وچ)
{
  ساكن ثابت عائم max_angle = HB_PI / 16.f;
  hb_سمتیہ_svg_نقطہ_t center = {cx, cy};
  صحيح عدد_splits = (صحيح) ceilf (fabsf (a1 - a0) / max_angle);
  إذا (عدد_splits < 1) عدد_splits = 1;

  hb_سمتیہ_svg_نقطہ_t p0 = {cosf (a0), sinf (a0)};
  hb_سمتیہ_svg_rgba_t color0 = c0_وچ;

  لكل (صحيح a = 0; a < عدد_splits; a++)
  {
    عائم k = (a + 1.f) / عدد_splits;
    عائم angle1 = hb_سمتیہ_svg_lerp (a0, a1, k);
    hb_سمتیہ_svg_rgba_t color1 = hb_سمتیہ_svg_lerp_rgba (c0_وچ, c1_وچ, k);

    hb_سمتیہ_svg_نقطہ_t p1 = {cosf (angle1), sinf (angle1)};
    hb_سمتیہ_svg_نقطہ_t sp0 = hb_سمتیہ_svg_جوڑو (center, hb_سمتیہ_svg_scale (p0, radius));
    hb_سمتیہ_svg_نقطہ_t sp1 = hb_سمتیہ_svg_جوڑو (center, hb_سمتیہ_svg_scale (p1, radius));

    hb_سمتیہ_svg_نقطہ_t A = hb_سمتیہ_svg_normalize (hb_سمتیہ_svg_جوڑو (p0, p1));
    hb_سمتیہ_svg_نقطہ_t U = {-A.y, A.x};
    عائم up0 = hb_سمتیہ_svg_dot (U, p0);
    عائم up1 = hb_سمتیہ_svg_dot (U, p1);
    إذا (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      استمر;
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
بنية hb_سمتیہ_svg_sweep_ctx_t
{
  hb_سمتیہ_buf_t *body;
  بنانشانی precision;
  عائم cx, cy, radius;
};

ساكن فراغ
hb_سمتیہ_svg_sweep_emit_patch (عائم a0, hb_color_t c0,
				عائم a1, hb_color_t c1,
				فراغ *user_انکڑے)
{
  خودکار *ctx = (hb_سمتیہ_svg_sweep_ctx_t *) user_انکڑے;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  إذا (unlikely (ctx->body->وچ_error ()))
    أرجع;
  hb_سمتیہ_svg_جوڑو_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_سمتیہ_svg_rgba_from_hb_color (c0),
				 a1, hb_سمتیہ_svg_rgba_from_hb_color (c1));
}


ساكن فراغ hb_سمتیہ_paint_ڈالو_transform (hb_paint_funcs_t *, فراغ *,
                                            عائم, عائم, عائم, عائم, عائم, عائم,
                                            فراغ *);
ساكن فراغ hb_سمتیہ_paint_نکالو_transform (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_سمتیہ_paint_fill_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, فراغ *);
ساكن فراغ hb_سمتیہ_paint_ڈالو_clip_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, فراغ *);
ساكن فراغ hb_سمتیہ_paint_ڈالو_clip_rectangle (hb_paint_funcs_t *, فراغ *, عائم, عائم, عائم, عائم, فراغ *);
ساكن hb_draw_funcs_t * hb_سمتیہ_paint_ڈالو_clip_رستہ_شروع (hb_paint_funcs_t *, فراغ *, فراغ **, فراغ *);
ساكن فراغ hb_سمتیہ_paint_ڈالو_clip_رستہ_سرا (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_سمتیہ_paint_نکالو_clip (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_سمتیہ_paint_color (hb_paint_funcs_t *, فراغ *, hb_bool_t, hb_color_t, فراغ *);
ساكن hb_bool_t hb_سمتیہ_paint_image (hb_paint_funcs_t *, فراغ *, hb_blob_t *, بنانشانی, بنانشانی, hb_ٹیگ_t, عائم, hb_glyph_extents_t *, فراغ *);
ساكن فراغ hb_سمتیہ_paint_خطی_gradient (hb_paint_funcs_t *, فراغ *, hb_color_سطر_t *, عائم, عائم, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_سمتیہ_paint_radial_gradient (hb_paint_funcs_t *, فراغ *, hb_color_سطر_t *, عائم, عائم, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_سمتیہ_paint_sweep_gradient (hb_paint_funcs_t *, فراغ *, hb_color_سطر_t *, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_سمتیہ_paint_ڈالو_گروہ (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_سمتیہ_paint_نکالو_گروہ (hb_paint_funcs_t *, فراغ *, hb_paint_مرکب_منوال_t, فراغ *);
ساكن hb_bool_t hb_سمتیہ_paint_color_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, فراغ *);
ساكن hb_bool_t
hb_سمتیہ_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				      فراغ *paint_انکڑے,
				      بنانشانی color_اشاریہ,
				      hb_color_t *color,
				      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_سمتیہ_paint_t *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (!color)
    أرجع خطأ;

  hb_color_t *value = خالی_سوچک;
  إذا (!paint->custom_palette_colors.has (color_اشاریہ, &value) || !value)
    أرجع خطأ;
  *color = *value;
  أرجع سچ;
}

ساكن مضمن فراغ free_static_سمتیہ_paint_funcs ();
ساكن بنية hb_سمتیہ_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_سمتیہ_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_سیٹ_ڈالو_transform_func (funcs, (hb_paint_ڈالو_transform_func_t) hb_سمتیہ_paint_ڈالو_transform, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_transform_func (funcs, (hb_paint_نکالو_transform_func_t) hb_سمتیہ_paint_نکالو_transform, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_سمتیہ_paint_fill_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, (hb_paint_ڈالو_clip_glyph_func_t) hb_سمتیہ_paint_ڈالو_clip_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, (hb_paint_ڈالو_clip_rectangle_func_t) hb_سمتیہ_paint_ڈالو_clip_rectangle, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_رستہ_شروع_func (funcs, (hb_paint_ڈالو_clip_رستہ_شروع_func_t) hb_سمتیہ_paint_ڈالو_clip_رستہ_شروع, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_رستہ_سرا_func (funcs, (hb_paint_ڈالو_clip_رستہ_سرا_func_t) hb_سمتیہ_paint_ڈالو_clip_رستہ_سرا, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, (hb_paint_نکالو_clip_func_t) hb_سمتیہ_paint_نکالو_clip, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_color_func (funcs, (hb_paint_color_func_t) hb_سمتیہ_paint_color, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_image_func (funcs, (hb_paint_image_func_t) hb_سمتیہ_paint_image, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, (hb_paint_خطی_gradient_func_t) hb_سمتیہ_paint_خطی_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_سمتیہ_paint_radial_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_سمتیہ_paint_sweep_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, (hb_paint_ڈالو_گروہ_func_t) hb_سمتیہ_paint_ڈالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, (hb_paint_نکالو_گروہ_func_t) hb_سمتیہ_paint_نکالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_سمتیہ_paint_color_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_سمتیہ_paint_custom_palette_color, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_سمتیہ_paint_سیٹ_budget_callback, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_سمتیہ_paint_get_budget_callback, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_سمتیہ_paint_get_budget_بچیا_callback, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_سمتیہ_paint_funcs);
    أرجع funcs;
  }
} static_سمتیہ_paint_funcs;

ساكن مضمن فراغ
free_static_سمتیہ_paint_funcs ()
{
  static_سمتیہ_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_سمتیہ_paint_svg_funcs_get ()
{
  أرجع static_سمتیہ_paint_funcs.get_unconst ();
}


ساكن فراغ
hb_سمتیہ_paint_ڈالو_transform (hb_paint_funcs_t *,
                                فراغ *paint_انکڑے,
                                عائم xx, عائم yx,
                                عائم xy, عائم yy,
                                عائم dx, عائم dy,
                                فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (unlikely (paint->transform_گروہ_حد_توں_ودھ_ڈونگھائی))
  {
    paint->transform_گروہ_حد_توں_ودھ_ڈونگھائی++;
    أرجع;
  }
  إذا (unlikely (paint->transform_گروہ_ڈونگھائی >= 64))
  {
    paint->transform_گروہ_حد_توں_ودھ_ڈونگھائی = 1;
    أرجع;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_گروہ_open_نقاب = (paint->transform_گروہ_open_نقاب << 1) | (opened ? 1ull : 0ull);
  paint->transform_گروہ_ڈونگھائی++;

  إذا (!opened)
    أرجع;

  خودکار &body = paint->current_body ();
  بنانشانی sprec = paint->defs.scale_precision ();
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

ساكن فراغ
hb_سمتیہ_paint_نکالو_transform (hb_paint_funcs_t *,
                               فراغ *paint_انکڑے,
                               فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (unlikely (paint->transform_گروہ_حد_توں_ودھ_ڈونگھائی))
  {
    paint->transform_گروہ_حد_توں_ودھ_ڈونگھائی--;
    أرجع;
  }
  إذا (!paint->transform_گروہ_ڈونگھائی)
    أرجع;
  paint->transform_گروہ_ڈونگھائی--;
  hb_bool_t opened = !!(paint->transform_گروہ_open_نقاب & 1ull);
  paint->transform_گروہ_open_نقاب >>= 1;
  إذا (opened)
    paint->current_body ().append_str ("</g>\n");
}

ساكن فراغ
hb_سمتیہ_paint_fill_glyph (hb_paint_funcs_t *,
                            فراغ *paint_انکڑے,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  paint->رستہ.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  إذا (likely (paint->budget_بچیا >= 0))
  {
    hb_سمتیہ_رستہ_sink_t sink = {&paint->رستہ, paint->get_precision (),
				 paint->x_scale_ضارب, paint->y_scale_ضارب,
				 &paint->budget_بچیا};
    hb_font_draw_glyph (font, glyph, hb_سمتیہ_svg_رستہ_draw_funcs_get (), &sink);
  }

  خودکار &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->رستہ.arrayZ, paint->رستہ.لمائی);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, سچ);
  body.append_str ("\"/>\n");
}

ساكن فراغ
hb_سمتیہ_paint_ڈالو_clip_glyph (hb_paint_funcs_t *,
                                 فراغ *paint_انکڑے,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_اگیتر;
  بنانشانی pfx_len = paint->id_اگیتر_لمائی;

  paint->رستہ.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  إذا (likely (paint->budget_بچیا >= 0))
  {
    hb_سمتیہ_رستہ_sink_t sink = {&paint->رستہ, paint->get_precision (),
				 paint->x_scale_ضارب, paint->y_scale_ضارب,
				 &paint->budget_بچیا};
    hb_font_draw_glyph (font, glyph, hb_سمتیہ_svg_رستہ_draw_funcs_get (), &sink);
  }

  بنانشانی def_id = paint->رستہ_def_گنتی++;
  paint->defs.append_str ("<path id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\" d=\"");
  paint->defs.append_len (paint->رستہ.arrayZ, paint->رستہ.لمائی);
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
hb_سمتیہ_paint_ڈالو_clip_rectangle (hb_paint_funcs_t *,
                                     فراغ *paint_انکڑے,
                                     عائم xmin, عائم ymin,
                                     عائم xmax, عائم ymax,
                                     فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_اگیتر;
  بنانشانی pfx_len = paint->id_اگیتر_لمائی;
  بنانشانی clip_id = paint->clip_rect_counter++;
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

ساكن hb_draw_funcs_t *
hb_سمتیہ_paint_ڈالو_clip_رستہ_شروع (hb_paint_funcs_t *,
                                      فراغ *paint_انکڑے,
                                      فراغ **draw_انکڑے,
                                      فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
  {
    *draw_انکڑے = خالی_سوچک;
    أرجع خالی_سوچک;
  }

  paint->رستہ.clear ();
  paint->clip_رستہ_sink = {&paint->رستہ, paint->get_precision (),
			   paint->x_scale_ضارب,
			   paint->y_scale_ضارب,
			   &paint->budget_بچیا};
  *draw_انکڑے = &paint->clip_رستہ_sink;
  أرجع hb_سمتیہ_svg_رستہ_draw_funcs_get ();
}

ساكن فراغ
hb_سمتیہ_paint_ڈالو_clip_رستہ_سرا (hb_paint_funcs_t *,
                                    فراغ *paint_انکڑے,
                                    فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_اگیتر;
  بنانشانی pfx_len = paint->id_اگیتر_لمائی;
  بنانشانی clip_id = paint->clip_رستہ_counter++;

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
  paint->defs.append_len (paint->رستہ.arrayZ, paint->رستہ.لمائی);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("cp");
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ساكن فراغ
hb_سمتیہ_paint_نکالو_clip (hb_paint_funcs_t *,
                          فراغ *paint_انکڑے,
                          فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("</g>\n");
}

ساكن فراغ
hb_سمتیہ_paint_color (hb_paint_funcs_t *,
                       فراغ *paint_انکڑے,
                       hb_bool_t,
                       hb_color_t color,
                       فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودکار &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, سچ);
  body.append_str ("\"/>\n");
}

ساكن hb_bool_t
hb_سمتیہ_paint_image (hb_paint_funcs_t *,
                       فراغ *paint_انکڑے,
                       hb_blob_t *image,
                       بنانشانی چوڑائی,
                       بنانشانی اچائی,
                       hb_ٹیگ_t format,
                       عائم slant HB_بن_ورتیا,
                       hb_glyph_extents_t *extents,
                       فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;

  خودکار &body = paint->current_body ();
  إذا (format == HB_ٹیگ ('p','n','g',' '))
  {
    إذا (!extents || !چوڑائی || !اچائی)
      أرجع خطأ;

    بنانشانی len = 0;
    ثابت حرف *png_انکڑے = hb_blob_get_انکڑے (image, &len);
    إذا (!png_انکڑے || !len)
      أرجع خطأ;

    body.append_str ("<g transform=\"translate(");
    body.append_عدد (paint->sx ((عائم) extents->x_bearing));
    body.append_c (',');
    body.append_عدد (paint->sy ((عائم) extents->y_bearing));
    body.append_str (") scale(");
    body.append_عدد (paint->sx ((عائم) extents->چوڑائی) / چوڑائی);
    body.append_c (',');
    body.append_عدد (paint->sy ((عائم) extents->اچائی) / اچائی);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ثابت uint8_t *) png_انکڑے, len);
    body.append_str ("\" width=\"");
    body.append_عدد ((عائم) چوڑائی);
    body.append_str ("\" height=\"");
    body.append_عدد ((عائم) اچائی);
    body.append_str ("\"/>\n</g>\n");

    أرجع سچ;
  }

  أرجع خطأ;
}

ساكن فراغ
hb_سمتیہ_paint_خطی_gradient (hb_paint_funcs_t *,
                                 فراغ *paint_انکڑے,
                                 hb_color_سطر_t *color_سطر,
                                 عائم x0, عائم y0,
                                 عائم x1, عائم y1,
                                 عائم x2, عائم y2,
                                 فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.لمائی, &mn, &mx);

  ثابت حرف *pfx = paint->id_اگیتر;
  بنانشانی pfx_len = paint->id_اگیتر_لمائی;
  بنانشانی grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  عائم lx0, ly0, lx1, ly1;
  hb_paint_reduce_خطی_anchors (x0, y0, x1, y1, x2, y2,
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

ساكن فراغ
hb_سمتیہ_paint_radial_gradient (hb_paint_funcs_t *,
                                 فراغ *paint_انکڑے,
                                 hb_color_سطر_t *color_سطر,
                                 عائم x0, عائم y0, عائم r0,
                                 عائم x1, عائم y1, عائم r1,
                                 فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.لمائی, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gr0 = r0 + mn * (r1 - r0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);
  عائم gr1 = r0 + mx * (r1 - r0);

  ثابت حرف *pfx = paint->id_اگیتر;
  بنانشانی pfx_len = paint->id_اگیتر_لمائی;
  بنانشانی grad_id = paint->gradient_counter++;

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
  إذا (gr0 > 0)
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

ساكن فراغ
hb_سمتیہ_paint_sweep_gradient (hb_paint_funcs_t *,
                                فراغ *paint_انکڑے,
                                hb_color_سطر_t *color_سطر,
                                عائم cx, عائم cy,
                                عائم شروع_angle, عائم سرا_angle,
                                فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.لمائی, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  عائم ga0 = شروع_angle + mn * (سرا_angle - شروع_angle);
  عائم ga1 = شروع_angle + mx * (سرا_angle - شروع_angle);

  hb_سمتیہ_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.لمائی,
				 hb_color_سطر_get_extend (color_سطر),
				 ga0, ga1,
				 hb_سمتیہ_svg_sweep_emit_patch,
				 &ctx);
}

ساكن فراغ
hb_سمتیہ_paint_ڈالو_گروہ (hb_paint_funcs_t *,
                            فراغ *paint_انکڑے,
                            فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (unlikely (!paint->گروہ_ڈھیر.ڈالو_or_fail (hb_سمتیہ_buf_t {})))
    أرجع;
}

ساكن فراغ
hb_سمتیہ_paint_نکالو_گروہ (hb_paint_funcs_t *,
                           فراغ *paint_انکڑے,
                           hb_paint_مرکب_منوال_t منوال,
                           فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (paint->گروہ_ڈھیر.لمائی < 2)
    أرجع;

  hb_سمتیہ_buf_t گروہ = paint->گروہ_ڈھیر.نکالو ();
  خودکار &body = paint->current_body ();

  ثابت حرف *blend = hb_سمتیہ_svg_مرکب_منوال_str (منوال);
  إذا (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (گروہ.arrayZ, گروہ.لمائی);
    body.append_str ("</g>\n");
  }
  وإلا
    body.append_len (گروہ.arrayZ, گروہ.لمائی);
}

ساكن hb_bool_t
hb_سمتیہ_paint_color_glyph (hb_paint_funcs_t *,
                             فراغ *paint_انکڑے,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;
  إذا (unlikely (paint->color_glyph_ڈونگھائی >= HB_MAX_NESTING_LEVEL))
    أرجع خطأ;
  إذا (unlikely (hb_سیٹ_has (paint->سرگرم_color_glyphs, glyph)))
    أرجع خطأ;

  paint->color_glyph_ڈونگھائی++;
  hb_سیٹ_جوڑو (paint->سرگرم_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_سمتیہ_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_سیٹ_del (paint->سرگرم_color_glyphs, glyph);
  paint->color_glyph_ڈونگھائی--;
  أرجع سچ;
}




hb_blob_t *
hb_سمتیہ_paint_render_svg (hb_سمتیہ_paint_t *paint)
{
  إذا (!paint->has_extents)
    أرجع خالی_سوچک;

  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خالی_سوچک;

  hb_سمتیہ_buf_t باہرلا;
  hb_buf_recover_recycled (paint->recycled_blob, &باہرلا);
  بنانشانی estimated = paint->defs.لمائی +
		       paint->گروہ_ڈھیر.arrayZ[0].لمائی +
		       320;
  باہرلا.alloc (estimated);
  عائم vb_x = paint->extents.x;
  عائم vb_y = -(paint->extents.y + paint->extents.اچائی);
  عائم vb_w = paint->extents.چوڑائی;
  عائم vb_h = paint->extents.اچائی;
  باہرلا.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  باہرلا.append_عدد (vb_x);
  باہرلا.append_c (' ');
  باہرلا.append_عدد (vb_y);
  باہرلا.append_c (' ');
  باہرلا.append_عدد (vb_w);
  باہرلا.append_c (' ');
  باہرلا.append_عدد (vb_h);
  باہرلا.append_str ("\" width=\"");
  باہرلا.append_عدد (vb_w);
  باہرلا.append_str ("\" height=\"");
  باہرلا.append_عدد (vb_h);
  باہرلا.append_str ("\">\n");

  إذا (paint->defs.لمائی)
  {
    باہرلا.append_str ("<defs>\n");
    باہرلا.append_len (paint->defs.arrayZ, paint->defs.لمائی);
    باہرلا.append_str ("</defs>\n");
  }

  إذا (hb_color_get_alpha (paint->background))
  {
    باہرلا.append_str ("<rect x=\"");
    باہرلا.append_عدد (vb_x);
    باہرلا.append_str ("\" y=\"");
    باہرلا.append_عدد (vb_y);
    باہرلا.append_str ("\" width=\"");
    باہرلا.append_عدد (vb_w);
    باہرلا.append_str ("\" height=\"");
    باہرلا.append_عدد (vb_h);
    باہرلا.append_str ("\" fill=\"rgb(");
    باہرلا.append_unsigned (hb_color_get_red (paint->background));
    باہرلا.append_c (',');
    باہرلا.append_unsigned (hb_color_get_green (paint->background));
    باہرلا.append_c (',');
    باہرلا.append_unsigned (hb_color_get_blue (paint->background));
    باہرلا.append_str (")\"");
    إذا (hb_color_get_alpha (paint->background) < 255)
    {
      باہرلا.append_str (" fill-opacity=\"");
      باہرلا.append_عدد (hb_color_get_alpha (paint->background) / 255.f, 4);
      باہرلا.append_c ('"');
    }
    باہرلا.append_str ("/>\n");
  }

  باہرلا.append_str ("<g transform=\"scale(1,-1)\">\n");
  باہرلا.append_len (paint->گروہ_ڈھیر.arrayZ[0].arrayZ, paint->گروہ_ڈھیر.arrayZ[0].لمائی);
  باہرلا.append_str ("</g>\n");

  باہرلا.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &باہرلا);

  أرجع blob;
}
