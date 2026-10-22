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
hb_ويڪٽر_svg_extend_گهڻائي_str (hb_paint_extend_t ext)
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
hb_ويڪٽر_svg_emit_color_stops (hb_ويڪٽر_paint_t *paint,
				hb_ويڪٽر_buf_t *buf,
				hb_ويڪٽر_t<hb_color_stop_t> *stops)
{
  لكل (بنانشان i = 0; i < stops->ڊيگهائي; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_انگ (stops->arrayZ[i].وٿي, 4);
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
      buf->append_انگ (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

ساكن ثابت حرف *
hb_ويڪٽر_svg_مرکب_گهڻائي_str (hb_paint_مرکب_گهڻائي_t گهڻائي)
{
  بدل (گهڻائي)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    حالة HB_PAINT_مرکب_گهڻائي_PLUS: أرجع "screen";
    حالة HB_PAINT_مرکب_گهڻائي_XOR: أرجع "difference";
    حالة HB_PAINT_مرکب_گهڻائي_CLEAR:
    حالة HB_PAINT_مرکب_گهڻائي_SRC:
    حالة HB_PAINT_مرکب_گهڻائي_DEST:
    حالة HB_PAINT_مرکب_گهڻائي_DEST_OVER:
    حالة HB_PAINT_مرکب_گهڻائي_SRC_اندريون:
    حالة HB_PAINT_مرکب_گهڻائي_DEST_اندريون:
    حالة HB_PAINT_مرکب_گهڻائي_SRC_ٻاهريون:
    حالة HB_PAINT_مرکب_گهڻائي_DEST_ٻاهريون:
    حالة HB_PAINT_مرکب_گهڻائي_SRC_ATOP:
    حالة HB_PAINT_مرکب_گهڻائي_DEST_ATOP:
      أرجع خالي_اشارو;
    حالة HB_PAINT_مرکب_گهڻائي_SRC_OVER: أرجع "normal";
    حالة HB_PAINT_مرکب_گهڻائي_SCREEN: أرجع "screen";
    حالة HB_PAINT_مرکب_گهڻائي_OVERLAY: أرجع "overlay";
    حالة HB_PAINT_مرکب_گهڻائي_DARKEN: أرجع "darken";
    حالة HB_PAINT_مرکب_گهڻائي_LIGHTEN: أرجع "lighten";
    حالة HB_PAINT_مرکب_گهڻائي_COLOR_DODGE: أرجع "color-dodge";
    حالة HB_PAINT_مرکب_گهڻائي_COLOR_BURN: أرجع "color-burn";
    حالة HB_PAINT_مرکب_گهڻائي_HARD_LIGHT: أرجع "hard-light";
    حالة HB_PAINT_مرکب_گهڻائي_SOFT_LIGHT: أرجع "soft-light";
    حالة HB_PAINT_مرکب_گهڻائي_DIFFERENCE: أرجع "difference";
    حالة HB_PAINT_مرکب_گهڻائي_EXCLUSION: أرجع "exclusion";
    حالة HB_PAINT_مرکب_گهڻائي_MULTIPLY: أرجع "multiply";
    حالة HB_PAINT_مرکب_گهڻائي_HSL_HUE: أرجع "hue";
    حالة HB_PAINT_مرکب_گهڻائي_HSL_SATURATION: أرجع "saturation";
    حالة HB_PAINT_مرکب_گهڻائي_HSL_COLOR: أرجع "color";
    حالة HB_PAINT_مرکب_گهڻائي_HSL_LUMINOSITY: أرجع "luminosity";
    افتراضي: أرجع خالي_اشارو;
  }
}

بنية hb_ويڪٽر_svg_نقطو_t { عائم x, y; };
بنية hb_ويڪٽر_svg_rgba_t { عائم r, g, b, a; };

ساكن مضمن عائم
hb_ويڪٽر_svg_lerp (عائم a, عائم b, عائم t)
{ أرجع a + (b - a) * t; }

ساكن مضمن عائم
hb_ويڪٽر_svg_clamp01 (عائم v)
{
  إذا (v < 0.f) أرجع 0.f;
  إذا (v > 1.f) أرجع 1.f;
  أرجع v;
}

ساكن مضمن hb_ويڪٽر_svg_rgba_t
hb_ويڪٽر_svg_rgba_from_hb_color (hb_color_t c)
{
  أرجع {(عائم) hb_color_get_red (c) / 255.f,
          (عائم) hb_color_get_green (c) / 255.f,
          (عائم) hb_color_get_blue (c) / 255.f,
          (عائم) hb_color_get_alpha (c) / 255.f};
}

ساكن مضمن hb_color_t
hb_ويڪٽر_svg_hb_color_from_rgba (ثابت hb_ويڪٽر_svg_rgba_t &c)
{
  بنانشان r = (بنانشان) roundf (hb_ويڪٽر_svg_clamp01 (c.r) * 255.f);
  بنانشان g = (بنانشان) roundf (hb_ويڪٽر_svg_clamp01 (c.g) * 255.f);
  بنانشان b = (بنانشان) roundf (hb_ويڪٽر_svg_clamp01 (c.b) * 255.f);
  بنانشان a = (بنانشان) roundf (hb_ويڪٽر_svg_clamp01 (c.a) * 255.f);
  أرجع HB_COLOR (b, g, r, a);
}

ساكن مضمن hb_ويڪٽر_svg_rgba_t
hb_ويڪٽر_svg_lerp_rgba (ثابت hb_ويڪٽر_svg_rgba_t &c0,
			 ثابت hb_ويڪٽر_svg_rgba_t &c1,
			 عائم t)
{
  أرجع {hb_ويڪٽر_svg_lerp (c0.r, c1.r, t),
          hb_ويڪٽر_svg_lerp (c0.g, c1.g, t),
          hb_ويڪٽر_svg_lerp (c0.b, c1.b, t),
          hb_ويڪٽر_svg_lerp (c0.a, c1.a, t)};
}

ساكن مضمن عائم hb_ويڪٽر_svg_dot (ثابت hb_ويڪٽر_svg_نقطو_t &p, ثابت hb_ويڪٽر_svg_نقطو_t &q) { أرجع p.x * q.x + p.y * q.y; }
ساكن مضمن hb_ويڪٽر_svg_نقطو_t hb_ويڪٽر_svg_جوڙيو (ثابت hb_ويڪٽر_svg_نقطو_t &p, ثابت hb_ويڪٽر_svg_نقطو_t &q) { أرجع {p.x + q.x, p.y + q.y}; }
ساكن مضمن hb_ويڪٽر_svg_نقطو_t hb_ويڪٽر_svg_sub (ثابت hb_ويڪٽر_svg_نقطو_t &p, ثابت hb_ويڪٽر_svg_نقطو_t &q) { أرجع {p.x - q.x, p.y - q.y}; }
ساكن مضمن hb_ويڪٽر_svg_نقطو_t hb_ويڪٽر_svg_scale (ثابت hb_ويڪٽر_svg_نقطو_t &p, عائم f) { أرجع {p.x * f, p.y * f}; }

ساكن مضمن hb_ويڪٽر_svg_نقطو_t
hb_ويڪٽر_svg_normalize (ثابت hb_ويڪٽر_svg_نقطو_t &p)
{
  عائم len = sqrtf (hb_ويڪٽر_svg_dot (p, p));
  إذا (len == 0.f) أرجع {0.f, 0.f};
  أرجع hb_ويڪٽر_svg_scale (p, 1.f / len);
}

ساكن فراغ
hb_ويڪٽر_svg_جوڙيو_sweep_patch (hb_ويڪٽر_buf_t *body,
			       بنانشان precision,
			       عائم cx, عائم cy, عائم radius,
			       عائم a0, ثابت hb_ويڪٽر_svg_rgba_t &c0_اندريون,
			       عائم a1, ثابت hb_ويڪٽر_svg_rgba_t &c1_اندريون)
{
  ساكن ثابت عائم max_angle = HB_PI / 16.f;
  hb_ويڪٽر_svg_نقطو_t center = {cx, cy};
  صحيح انگ_splits = (صحيح) ceilf (fabsf (a1 - a0) / max_angle);
  إذا (انگ_splits < 1) انگ_splits = 1;

  hb_ويڪٽر_svg_نقطو_t p0 = {cosf (a0), sinf (a0)};
  hb_ويڪٽر_svg_rgba_t color0 = c0_اندريون;

  لكل (صحيح a = 0; a < انگ_splits; a++)
  {
    عائم k = (a + 1.f) / انگ_splits;
    عائم angle1 = hb_ويڪٽر_svg_lerp (a0, a1, k);
    hb_ويڪٽر_svg_rgba_t color1 = hb_ويڪٽر_svg_lerp_rgba (c0_اندريون, c1_اندريون, k);

    hb_ويڪٽر_svg_نقطو_t p1 = {cosf (angle1), sinf (angle1)};
    hb_ويڪٽر_svg_نقطو_t sp0 = hb_ويڪٽر_svg_جوڙيو (center, hb_ويڪٽر_svg_scale (p0, radius));
    hb_ويڪٽر_svg_نقطو_t sp1 = hb_ويڪٽر_svg_جوڙيو (center, hb_ويڪٽر_svg_scale (p1, radius));

    hb_ويڪٽر_svg_نقطو_t A = hb_ويڪٽر_svg_normalize (hb_ويڪٽر_svg_جوڙيو (p0, p1));
    hb_ويڪٽر_svg_نقطو_t U = {-A.y, A.x};
    عائم up0 = hb_ويڪٽر_svg_dot (U, p0);
    عائم up1 = hb_ويڪٽر_svg_dot (U, p1);
    إذا (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      استمر;
    }
    hb_ويڪٽر_svg_نقطو_t C0 = hb_ويڪٽر_svg_جوڙيو (A, hb_ويڪٽر_svg_scale (U, hb_ويڪٽر_svg_dot (hb_ويڪٽر_svg_sub (p0, A), p0) / up0));
    hb_ويڪٽر_svg_نقطو_t C1 = hb_ويڪٽر_svg_جوڙيو (A, hb_ويڪٽر_svg_scale (U, hb_ويڪٽر_svg_dot (hb_ويڪٽر_svg_sub (p1, A), p1) / up1));

    hb_ويڪٽر_svg_نقطو_t sc0 = hb_ويڪٽر_svg_جوڙيو (center, hb_ويڪٽر_svg_scale (hb_ويڪٽر_svg_جوڙيو (C0, hb_ويڪٽر_svg_scale (hb_ويڪٽر_svg_sub (C0, p0), 0.33333f)), radius));
    hb_ويڪٽر_svg_نقطو_t sc1 = hb_ويڪٽر_svg_جوڙيو (center, hb_ويڪٽر_svg_scale (hb_ويڪٽر_svg_جوڙيو (C1, hb_ويڪٽر_svg_scale (hb_ويڪٽر_svg_sub (C1, p1), 0.33333f)), radius));

    hb_ويڪٽر_svg_rgba_t mid_color = hb_ويڪٽر_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_ويڪٽر_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_انگ (center.x, precision);
    body->append_c (',');
    body->append_انگ (center.y, precision);
    body->append_str ("L");
    body->append_انگ (sp0.x, precision);
    body->append_c (',');
    body->append_انگ (sp0.y, precision);
    body->append_str ("C");
    body->append_انگ (sc0.x, precision);
    body->append_c (',');
    body->append_انگ (sc0.y, precision);
    body->append_c (' ');
    body->append_انگ (sc1.x, precision);
    body->append_c (',');
    body->append_انگ (sc1.y, precision);
    body->append_c (' ');
    body->append_انگ (sp1.x, precision);
    body->append_c (',');
    body->append_انگ (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, سچ);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
بنية hb_ويڪٽر_svg_sweep_ctx_t
{
  hb_ويڪٽر_buf_t *body;
  بنانشان precision;
  عائم cx, cy, radius;
};

ساكن فراغ
hb_ويڪٽر_svg_sweep_emit_patch (عائم a0, hb_color_t c0,
				عائم a1, hb_color_t c1,
				فراغ *user_معلومات)
{
  خودڪار *ctx = (hb_ويڪٽر_svg_sweep_ctx_t *) user_معلومات;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  إذا (unlikely (ctx->body->اندريون_error ()))
    أرجع;
  hb_ويڪٽر_svg_جوڙيو_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_ويڪٽر_svg_rgba_from_hb_color (c0),
				 a1, hb_ويڪٽر_svg_rgba_from_hb_color (c1));
}


ساكن فراغ hb_ويڪٽر_paint_ڌڪو_transform (hb_paint_funcs_t *, فراغ *,
                                            عائم, عائم, عائم, عائم, عائم, عائم,
                                            فراغ *);
ساكن فراغ hb_ويڪٽر_paint_ڪڍو_transform (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_fill_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_ڌڪو_clip_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_ڌڪو_clip_rectangle (hb_paint_funcs_t *, فراغ *, عائم, عائم, عائم, عائم, فراغ *);
ساكن hb_draw_funcs_t * hb_ويڪٽر_paint_ڌڪو_clip_رستو_شروعات (hb_paint_funcs_t *, فراغ *, فراغ **, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_ڌڪو_clip_رستو_پڇاڙي (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_ڪڍو_clip (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_color (hb_paint_funcs_t *, فراغ *, hb_bool_t, hb_color_t, فراغ *);
ساكن hb_bool_t hb_ويڪٽر_paint_image (hb_paint_funcs_t *, فراغ *, hb_blob_t *, بنانشان, بنانشان, hb_ٽيگ_t, عائم, hb_glyph_extents_t *, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_لڪيري_gradient (hb_paint_funcs_t *, فراغ *, hb_color_سٽ_t *, عائم, عائم, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_radial_gradient (hb_paint_funcs_t *, فراغ *, hb_color_سٽ_t *, عائم, عائم, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_sweep_gradient (hb_paint_funcs_t *, فراغ *, hb_color_سٽ_t *, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_ڌڪو_گروهه (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_ويڪٽر_paint_ڪڍو_گروهه (hb_paint_funcs_t *, فراغ *, hb_paint_مرکب_گهڻائي_t, فراغ *);
ساكن hb_bool_t hb_ويڪٽر_paint_color_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, فراغ *);
ساكن hb_bool_t
hb_ويڪٽر_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
				      فراغ *paint_معلومات,
				      بنانشان color_اشاريو,
				      hb_color_t *color,
				      فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_ويڪٽر_paint_t *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (!color)
    أرجع خطأ;

  hb_color_t *value = خالي_اشارو;
  إذا (!paint->custom_palette_colors.has (color_اشاريو, &value) || !value)
    أرجع خطأ;
  *color = *value;
  أرجع سچ;
}

ساكن مضمن فراغ free_static_ويڪٽر_paint_funcs ();
ساكن بنية hb_ويڪٽر_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_ويڪٽر_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_سيٽ_ڌڪو_transform_func (funcs, (hb_paint_ڌڪو_transform_func_t) hb_ويڪٽر_paint_ڌڪو_transform, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_transform_func (funcs, (hb_paint_ڪڍو_transform_func_t) hb_ويڪٽر_paint_ڪڍو_transform, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_ويڪٽر_paint_fill_glyph, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_glyph_func (funcs, (hb_paint_ڌڪو_clip_glyph_func_t) hb_ويڪٽر_paint_ڌڪو_clip_glyph, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_rectangle_func (funcs, (hb_paint_ڌڪو_clip_rectangle_func_t) hb_ويڪٽر_paint_ڌڪو_clip_rectangle, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_رستو_شروعات_func (funcs, (hb_paint_ڌڪو_clip_رستو_شروعات_func_t) hb_ويڪٽر_paint_ڌڪو_clip_رستو_شروعات, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_رستو_پڇاڙي_func (funcs, (hb_paint_ڌڪو_clip_رستو_پڇاڙي_func_t) hb_ويڪٽر_paint_ڌڪو_clip_رستو_پڇاڙي, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_clip_func (funcs, (hb_paint_ڪڍو_clip_func_t) hb_ويڪٽر_paint_ڪڍو_clip, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_color_func (funcs, (hb_paint_color_func_t) hb_ويڪٽر_paint_color, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_image_func (funcs, (hb_paint_image_func_t) hb_ويڪٽر_paint_image, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_لڪيري_gradient_func (funcs, (hb_paint_لڪيري_gradient_func_t) hb_ويڪٽر_paint_لڪيري_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_ويڪٽر_paint_radial_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_ويڪٽر_paint_sweep_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_گروهه_func (funcs, (hb_paint_ڌڪو_گروهه_func_t) hb_ويڪٽر_paint_ڌڪو_گروهه, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_گروهه_func (funcs, (hb_paint_ڪڍو_گروهه_func_t) hb_ويڪٽر_paint_ڪڍو_گروهه, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_ويڪٽر_paint_color_glyph, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_ويڪٽر_paint_custom_palette_color, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_سيٽ_budget_func (funcs, hb_ويڪٽر_paint_سيٽ_budget_callback, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_get_budget_func (funcs, hb_ويڪٽر_paint_get_budget_callback, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_ويڪٽر_paint_get_budget_بچيل_callback, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_ويڪٽر_paint_funcs);
    أرجع funcs;
  }
} static_ويڪٽر_paint_funcs;

ساكن مضمن فراغ
free_static_ويڪٽر_paint_funcs ()
{
  static_ويڪٽر_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_ويڪٽر_paint_svg_funcs_get ()
{
  أرجع static_ويڪٽر_paint_funcs.get_unconst ();
}


ساكن فراغ
hb_ويڪٽر_paint_ڌڪو_transform (hb_paint_funcs_t *,
                                فراغ *paint_معلومات,
                                عائم xx, عائم yx,
                                عائم xy, عائم yy,
                                عائم dx, عائم dy,
                                فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (unlikely (paint->transform_گروهه_حد_کان_مٿي_اونهائي))
  {
    paint->transform_گروهه_حد_کان_مٿي_اونهائي++;
    أرجع;
  }
  إذا (unlikely (paint->transform_گروهه_اونهائي >= 64))
  {
    paint->transform_گروهه_حد_کان_مٿي_اونهائي = 1;
    أرجع;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_گروهه_open_نقاب = (paint->transform_گروهه_open_نقاب << 1) | (opened ? 1ull : 0ull);
  paint->transform_گروهه_اونهائي++;

  إذا (!opened)
    أرجع;

  خودڪار &body = paint->current_body ();
  بنانشان sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_انگ (xx, sprec);
  body.append_c (',');
  body.append_انگ (yx, sprec);
  body.append_c (',');
  body.append_انگ (xy, sprec);
  body.append_c (',');
  body.append_انگ (yy, sprec);
  body.append_c (',');
  body.append_انگ (paint->sx (dx));
  body.append_c (',');
  body.append_انگ (paint->sy (dy));
  body.append_str (")\">\n");
}

ساكن فراغ
hb_ويڪٽر_paint_ڪڍو_transform (hb_paint_funcs_t *,
                               فراغ *paint_معلومات,
                               فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (unlikely (paint->transform_گروهه_حد_کان_مٿي_اونهائي))
  {
    paint->transform_گروهه_حد_کان_مٿي_اونهائي--;
    أرجع;
  }
  إذا (!paint->transform_گروهه_اونهائي)
    أرجع;
  paint->transform_گروهه_اونهائي--;
  hb_bool_t opened = !!(paint->transform_گروهه_open_نقاب & 1ull);
  paint->transform_گروهه_open_نقاب >>= 1;
  إذا (opened)
    paint->current_body ().append_str ("</g>\n");
}

ساكن فراغ
hb_ويڪٽر_paint_fill_glyph (hb_paint_funcs_t *,
                            فراغ *paint_معلومات,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  paint->رستو.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  إذا (likely (paint->budget_بچيل >= 0))
  {
    hb_ويڪٽر_رستو_sink_t sink = {&paint->رستو, paint->get_precision (),
				 paint->x_scale_ضرب_ڪندڙ, paint->y_scale_ضرب_ڪندڙ,
				 &paint->budget_بچيل};
    hb_font_draw_glyph (font, glyph, hb_ويڪٽر_svg_رستو_draw_funcs_get (), &sink);
  }

  خودڪار &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->رستو.arrayZ, paint->رستو.ڊيگهائي);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, سچ);
  body.append_str ("\"/>\n");
}

ساكن فراغ
hb_ويڪٽر_paint_ڌڪو_clip_glyph (hb_paint_funcs_t *,
                                 فراغ *paint_معلومات,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_اڳياڙي;
  بنانشان pfx_len = paint->id_اڳياڙي_ڊيگهائي;

  paint->رستو.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  إذا (likely (paint->budget_بچيل >= 0))
  {
    hb_ويڪٽر_رستو_sink_t sink = {&paint->رستو, paint->get_precision (),
				 paint->x_scale_ضرب_ڪندڙ, paint->y_scale_ضرب_ڪندڙ,
				 &paint->budget_بچيل};
    hb_font_draw_glyph (font, glyph, hb_ويڪٽر_svg_رستو_draw_funcs_get (), &sink);
  }

  بنانشان def_id = paint->رستو_def_ڳڻپ++;
  paint->defs.append_str ("<path id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\" d=\"");
  paint->defs.append_len (paint->رستو.arrayZ, paint->رستو.ڊيگهائي);
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
hb_ويڪٽر_paint_ڌڪو_clip_rectangle (hb_paint_funcs_t *,
                                     فراغ *paint_معلومات,
                                     عائم xmin, عائم ymin,
                                     عائم xmax, عائم ymax,
                                     فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_اڳياڙي;
  بنانشان pfx_len = paint->id_اڳياڙي_ڊيگهائي;
  بنانشان clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_انگ (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_انگ (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_انگ (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_انگ (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ساكن hb_draw_funcs_t *
hb_ويڪٽر_paint_ڌڪو_clip_رستو_شروعات (hb_paint_funcs_t *,
                                      فراغ *paint_معلومات,
                                      فراغ **draw_معلومات,
                                      فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
  {
    *draw_معلومات = خالي_اشارو;
    أرجع خالي_اشارو;
  }

  paint->رستو.clear ();
  paint->clip_رستو_sink = {&paint->رستو, paint->get_precision (),
			   paint->x_scale_ضرب_ڪندڙ,
			   paint->y_scale_ضرب_ڪندڙ,
			   &paint->budget_بچيل};
  *draw_معلومات = &paint->clip_رستو_sink;
  أرجع hb_ويڪٽر_svg_رستو_draw_funcs_get ();
}

ساكن فراغ
hb_ويڪٽر_paint_ڌڪو_clip_رستو_پڇاڙي (hb_paint_funcs_t *,
                                    فراغ *paint_معلومات,
                                    فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_اڳياڙي;
  بنانشان pfx_len = paint->id_اڳياڙي_ڊيگهائي;
  بنانشان clip_id = paint->clip_رستو_counter++;

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
  paint->defs.append_len (paint->رستو.arrayZ, paint->رستو.ڊيگهائي);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("cp");
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ساكن فراغ
hb_ويڪٽر_paint_ڪڍو_clip (hb_paint_funcs_t *,
                          فراغ *paint_معلومات,
                          فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("</g>\n");
}

ساكن فراغ
hb_ويڪٽر_paint_color (hb_paint_funcs_t *,
                       فراغ *paint_معلومات,
                       hb_bool_t,
                       hb_color_t color,
                       فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودڪار &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, سچ);
  body.append_str ("\"/>\n");
}

ساكن hb_bool_t
hb_ويڪٽر_paint_image (hb_paint_funcs_t *,
                       فراغ *paint_معلومات,
                       hb_blob_t *image,
                       بنانشان ويڪر,
                       بنانشان اوچائي,
                       hb_ٽيگ_t format,
                       عائم slant HB_اڻ_استعمال_ٿيل,
                       hb_glyph_extents_t *extents,
                       فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;

  خودڪار &body = paint->current_body ();
  إذا (format == HB_ٽيگ ('p','n','g',' '))
  {
    إذا (!extents || !ويڪر || !اوچائي)
      أرجع خطأ;

    بنانشان len = 0;
    ثابت حرف *png_معلومات = hb_blob_get_معلومات (image, &len);
    إذا (!png_معلومات || !len)
      أرجع خطأ;

    body.append_str ("<g transform=\"translate(");
    body.append_انگ (paint->sx ((عائم) extents->x_bearing));
    body.append_c (',');
    body.append_انگ (paint->sy ((عائم) extents->y_bearing));
    body.append_str (") scale(");
    body.append_انگ (paint->sx ((عائم) extents->ويڪر) / ويڪر);
    body.append_c (',');
    body.append_انگ (paint->sy ((عائم) extents->اوچائي) / اوچائي);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ثابت uint8_t *) png_معلومات, len);
    body.append_str ("\" width=\"");
    body.append_انگ ((عائم) ويڪر);
    body.append_str ("\" height=\"");
    body.append_انگ ((عائم) اوچائي);
    body.append_str ("\"/>\n</g>\n");

    أرجع سچ;
  }

  أرجع خطأ;
}

ساكن فراغ
hb_ويڪٽر_paint_لڪيري_gradient (hb_paint_funcs_t *,
                                 فراغ *paint_معلومات,
                                 hb_color_سٽ_t *color_سٽ,
                                 عائم x0, عائم y0,
                                 عائم x1, عائم y1,
                                 عائم x2, عائم y2,
                                 فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_سٽ))
    أرجع;
  hb_ويڪٽر_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  عائم mn, mx;
  hb_paint_normalize_color_سٽ (stops.arrayZ, stops.ڊيگهائي, &mn, &mx);

  ثابت حرف *pfx = paint->id_اڳياڙي;
  بنانشان pfx_len = paint->id_اڳياڙي_ڊيگهائي;
  بنانشان grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  عائم lx0, ly0, lx1, ly1;
  hb_paint_reduce_لڪيري_anchors (x0, y0, x1, y1, x2, y2,
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
  paint->defs.append_انگ (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_انگ (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_انگ (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_انگ (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_ويڪٽر_svg_extend_گهڻائي_str (hb_color_سٽ_get_extend (color_سٽ)));
  paint->defs.append_str ("\">\n");
  hb_ويڪٽر_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</linearGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

ساكن فراغ
hb_ويڪٽر_paint_radial_gradient (hb_paint_funcs_t *,
                                 فراغ *paint_معلومات,
                                 hb_color_سٽ_t *color_سٽ,
                                 عائم x0, عائم y0, عائم r0,
                                 عائم x1, عائم y1, عائم r1,
                                 فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_سٽ))
    أرجع;
  hb_ويڪٽر_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سٽ (stops.arrayZ, stops.ڊيگهائي, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gr0 = r0 + mn * (r1 - r0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);
  عائم gr1 = r0 + mx * (r1 - r0);

  ثابت حرف *pfx = paint->id_اڳياڙي;
  بنانشان pfx_len = paint->id_اڳياڙي_ڊيگهائي;
  بنانشان grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_انگ (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_انگ (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_انگ (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_انگ (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_انگ (paint->sy (gy0));
  إذا (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_انگ (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_ويڪٽر_svg_extend_گهڻائي_str (hb_color_سٽ_get_extend (color_سٽ)));
  paint->defs.append_str ("\">\n");
  hb_ويڪٽر_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</radialGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

ساكن فراغ
hb_ويڪٽر_paint_sweep_gradient (hb_paint_funcs_t *,
                                فراغ *paint_معلومات,
                                hb_color_سٽ_t *color_سٽ,
                                عائم cx, عائم cy,
                                عائم شروعات_angle, عائم پڇاڙي_angle,
                                فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_سٽ))
    أرجع;
  hb_ويڪٽر_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سٽ (stops.arrayZ, stops.ڊيگهائي, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  عائم ga0 = شروعات_angle + mn * (پڇاڙي_angle - شروعات_angle);
  عائم ga1 = شروعات_angle + mx * (پڇاڙي_angle - شروعات_angle);

  hb_ويڪٽر_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.ڊيگهائي,
				 hb_color_سٽ_get_extend (color_سٽ),
				 ga0, ga1,
				 hb_ويڪٽر_svg_sweep_emit_patch,
				 &ctx);
}

ساكن فراغ
hb_ويڪٽر_paint_ڌڪو_گروهه (hb_paint_funcs_t *,
                            فراغ *paint_معلومات,
                            فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (unlikely (!paint->گروهه_ڍير.ڌڪو_or_fail (hb_ويڪٽر_buf_t {})))
    أرجع;
}

ساكن فراغ
hb_ويڪٽر_paint_ڪڍو_گروهه (hb_paint_funcs_t *,
                           فراغ *paint_معلومات,
                           hb_paint_مرکب_گهڻائي_t گهڻائي,
                           فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (paint->گروهه_ڍير.ڊيگهائي < 2)
    أرجع;

  hb_ويڪٽر_buf_t گروهه = paint->گروهه_ڍير.ڪڍو ();
  خودڪار &body = paint->current_body ();

  ثابت حرف *blend = hb_ويڪٽر_svg_مرکب_گهڻائي_str (گهڻائي);
  إذا (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (گروهه.arrayZ, گروهه.ڊيگهائي);
    body.append_str ("</g>\n");
  }
  وإلا
    body.append_len (گروهه.arrayZ, گروهه.ڊيگهائي);
}

ساكن hb_bool_t
hb_ويڪٽر_paint_color_glyph (hb_paint_funcs_t *,
                             فراغ *paint_معلومات,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;
  إذا (unlikely (paint->color_glyph_اونهائي >= HB_MAX_NESTING_LEVEL))
    أرجع خطأ;
  إذا (unlikely (hb_سيٽ_has (paint->سرگرم_color_glyphs, glyph)))
    أرجع خطأ;

  paint->color_glyph_اونهائي++;
  hb_سيٽ_جوڙيو (paint->سرگرم_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_ويڪٽر_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_سيٽ_del (paint->سرگرم_color_glyphs, glyph);
  paint->color_glyph_اونهائي--;
  أرجع سچ;
}




hb_blob_t *
hb_ويڪٽر_paint_render_svg (hb_ويڪٽر_paint_t *paint)
{
  إذا (!paint->has_extents)
    أرجع خالي_اشارو;

  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خالي_اشارو;

  hb_ويڪٽر_buf_t ٻاهريون;
  hb_buf_recover_recycled (paint->recycled_blob, &ٻاهريون);
  بنانشان estimated = paint->defs.ڊيگهائي +
		       paint->گروهه_ڍير.arrayZ[0].ڊيگهائي +
		       320;
  ٻاهريون.alloc (estimated);
  عائم vb_x = paint->extents.x;
  عائم vb_y = -(paint->extents.y + paint->extents.اوچائي);
  عائم vb_w = paint->extents.ويڪر;
  عائم vb_h = paint->extents.اوچائي;
  ٻاهريون.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  ٻاهريون.append_انگ (vb_x);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (vb_y);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (vb_w);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (vb_h);
  ٻاهريون.append_str ("\" width=\"");
  ٻاهريون.append_انگ (vb_w);
  ٻاهريون.append_str ("\" height=\"");
  ٻاهريون.append_انگ (vb_h);
  ٻاهريون.append_str ("\">\n");

  إذا (paint->defs.ڊيگهائي)
  {
    ٻاهريون.append_str ("<defs>\n");
    ٻاهريون.append_len (paint->defs.arrayZ, paint->defs.ڊيگهائي);
    ٻاهريون.append_str ("</defs>\n");
  }

  إذا (hb_color_get_alpha (paint->background))
  {
    ٻاهريون.append_str ("<rect x=\"");
    ٻاهريون.append_انگ (vb_x);
    ٻاهريون.append_str ("\" y=\"");
    ٻاهريون.append_انگ (vb_y);
    ٻاهريون.append_str ("\" width=\"");
    ٻاهريون.append_انگ (vb_w);
    ٻاهريون.append_str ("\" height=\"");
    ٻاهريون.append_انگ (vb_h);
    ٻاهريون.append_str ("\" fill=\"rgb(");
    ٻاهريون.append_unsigned (hb_color_get_red (paint->background));
    ٻاهريون.append_c (',');
    ٻاهريون.append_unsigned (hb_color_get_green (paint->background));
    ٻاهريون.append_c (',');
    ٻاهريون.append_unsigned (hb_color_get_blue (paint->background));
    ٻاهريون.append_str (")\"");
    إذا (hb_color_get_alpha (paint->background) < 255)
    {
      ٻاهريون.append_str (" fill-opacity=\"");
      ٻاهريون.append_انگ (hb_color_get_alpha (paint->background) / 255.f, 4);
      ٻاهريون.append_c ('"');
    }
    ٻاهريون.append_str ("/>\n");
  }

  ٻاهريون.append_str ("<g transform=\"scale(1,-1)\">\n");
  ٻاهريون.append_len (paint->گروهه_ڍير.arrayZ[0].arrayZ, paint->گروهه_ڍير.arrayZ[0].ڊيگهائي);
  ٻاهريون.append_str ("</g>\n");

  ٻاهريون.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &ٻاهريون);

  أرجع blob;
}
