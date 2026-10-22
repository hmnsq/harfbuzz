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
hb_ویکټور_svg_extend_ډېرپېښ_str (hb_paint_extend_t ext)
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
hb_ویکټور_svg_emit_color_stops (hb_ویکټور_paint_t *paint,
				hb_ویکټور_buf_t *buf,
				hb_ویکټور_t<hb_color_stop_t> *stops)
{
  لكل (بینښه i = 0; i < stops->اوږدتیا; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_عدد (stops->arrayZ[i].دننه_واټن, 4);
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
hb_ویکټور_svg_مرکب_ډېرپېښ_str (hb_paint_مرکب_ډېرپېښ_t ډېرپېښ)
{
  بدل (ډېرپېښ)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    حالة HB_PAINT_مرکب_ډېرپېښ_PLUS: أرجع "screen";
    حالة HB_PAINT_مرکب_ډېرپېښ_XOR: أرجع "difference";
    حالة HB_PAINT_مرکب_ډېرپېښ_CLEAR:
    حالة HB_PAINT_مرکب_ډېرپېښ_SRC:
    حالة HB_PAINT_مرکب_ډېرپېښ_DEST:
    حالة HB_PAINT_مرکب_ډېرپېښ_DEST_OVER:
    حالة HB_PAINT_مرکب_ډېرپېښ_SRC_دننه:
    حالة HB_PAINT_مرکب_ډېرپېښ_DEST_دننه:
    حالة HB_PAINT_مرکب_ډېرپېښ_SRC_بهر:
    حالة HB_PAINT_مرکب_ډېرپېښ_DEST_بهر:
    حالة HB_PAINT_مرکب_ډېرپېښ_SRC_ATOP:
    حالة HB_PAINT_مرکب_ډېرپېښ_DEST_ATOP:
      أرجع تش_اشاره;
    حالة HB_PAINT_مرکب_ډېرپېښ_SRC_OVER: أرجع "normal";
    حالة HB_PAINT_مرکب_ډېرپېښ_SCREEN: أرجع "screen";
    حالة HB_PAINT_مرکب_ډېرپېښ_OVERLAY: أرجع "overlay";
    حالة HB_PAINT_مرکب_ډېرپېښ_DARKEN: أرجع "darken";
    حالة HB_PAINT_مرکب_ډېرپېښ_LIGHTEN: أرجع "lighten";
    حالة HB_PAINT_مرکب_ډېرپېښ_COLOR_DODGE: أرجع "color-dodge";
    حالة HB_PAINT_مرکب_ډېرپېښ_COLOR_BURN: أرجع "color-burn";
    حالة HB_PAINT_مرکب_ډېرپېښ_HARD_LIGHT: أرجع "hard-light";
    حالة HB_PAINT_مرکب_ډېرپېښ_SOFT_LIGHT: أرجع "soft-light";
    حالة HB_PAINT_مرکب_ډېرپېښ_DIFFERENCE: أرجع "difference";
    حالة HB_PAINT_مرکب_ډېرپېښ_EXCLUSION: أرجع "exclusion";
    حالة HB_PAINT_مرکب_ډېرپېښ_MULTIPLY: أرجع "multiply";
    حالة HB_PAINT_مرکب_ډېرپېښ_HSL_HUE: أرجع "hue";
    حالة HB_PAINT_مرکب_ډېرپېښ_HSL_SATURATION: أرجع "saturation";
    حالة HB_PAINT_مرکب_ډېرپېښ_HSL_COLOR: أرجع "color";
    حالة HB_PAINT_مرکب_ډېرپېښ_HSL_LUMINOSITY: أرجع "luminosity";
    افتراضي: أرجع تش_اشاره;
  }
}

بنية hb_ویکټور_svg_ټکی_t { عائم x, y; };
بنية hb_ویکټور_svg_rgba_t { عائم r, g, b, a; };

ساكن مضمن عائم
hb_ویکټور_svg_lerp (عائم a, عائم b, عائم t)
{ أرجع a + (b - a) * t; }

ساكن مضمن عائم
hb_ویکټور_svg_clamp01 (عائم v)
{
  إذا (v < 0.f) أرجع 0.f;
  إذا (v > 1.f) أرجع 1.f;
  أرجع v;
}

ساكن مضمن hb_ویکټور_svg_rgba_t
hb_ویکټور_svg_rgba_from_hb_color (hb_color_t c)
{
  أرجع {(عائم) hb_color_get_red (c) / 255.f,
          (عائم) hb_color_get_green (c) / 255.f,
          (عائم) hb_color_get_blue (c) / 255.f,
          (عائم) hb_color_get_alpha (c) / 255.f};
}

ساكن مضمن hb_color_t
hb_ویکټور_svg_hb_color_from_rgba (ثابت hb_ویکټور_svg_rgba_t &c)
{
  بینښه r = (بینښه) roundf (hb_ویکټور_svg_clamp01 (c.r) * 255.f);
  بینښه g = (بینښه) roundf (hb_ویکټور_svg_clamp01 (c.g) * 255.f);
  بینښه b = (بینښه) roundf (hb_ویکټور_svg_clamp01 (c.b) * 255.f);
  بینښه a = (بینښه) roundf (hb_ویکټور_svg_clamp01 (c.a) * 255.f);
  أرجع HB_COLOR (b, g, r, a);
}

ساكن مضمن hb_ویکټور_svg_rgba_t
hb_ویکټور_svg_lerp_rgba (ثابت hb_ویکټور_svg_rgba_t &c0,
			 ثابت hb_ویکټور_svg_rgba_t &c1,
			 عائم t)
{
  أرجع {hb_ویکټور_svg_lerp (c0.r, c1.r, t),
          hb_ویکټور_svg_lerp (c0.g, c1.g, t),
          hb_ویکټور_svg_lerp (c0.b, c1.b, t),
          hb_ویکټور_svg_lerp (c0.a, c1.a, t)};
}

ساكن مضمن عائم hb_ویکټور_svg_dot (ثابت hb_ویکټور_svg_ټکی_t &p, ثابت hb_ویکټور_svg_ټکی_t &q) { أرجع p.x * q.x + p.y * q.y; }
ساكن مضمن hb_ویکټور_svg_ټکی_t hb_ویکټور_svg_جمع_کړه (ثابت hb_ویکټور_svg_ټکی_t &p, ثابت hb_ویکټور_svg_ټکی_t &q) { أرجع {p.x + q.x, p.y + q.y}; }
ساكن مضمن hb_ویکټور_svg_ټکی_t hb_ویکټور_svg_sub (ثابت hb_ویکټور_svg_ټکی_t &p, ثابت hb_ویکټور_svg_ټکی_t &q) { أرجع {p.x - q.x, p.y - q.y}; }
ساكن مضمن hb_ویکټور_svg_ټکی_t hb_ویکټور_svg_scale (ثابت hb_ویکټور_svg_ټکی_t &p, عائم f) { أرجع {p.x * f, p.y * f}; }

ساكن مضمن hb_ویکټور_svg_ټکی_t
hb_ویکټور_svg_normalize (ثابت hb_ویکټور_svg_ټکی_t &p)
{
  عائم len = sqrtf (hb_ویکټور_svg_dot (p, p));
  إذا (len == 0.f) أرجع {0.f, 0.f};
  أرجع hb_ویکټور_svg_scale (p, 1.f / len);
}

ساكن فراغ
hb_ویکټور_svg_جمع_کړه_sweep_patch (hb_ویکټور_buf_t *body,
			       بینښه precision,
			       عائم cx, عائم cy, عائم radius,
			       عائم a0, ثابت hb_ویکټور_svg_rgba_t &c0_دننه,
			       عائم a1, ثابت hb_ویکټور_svg_rgba_t &c1_دننه)
{
  ساكن ثابت عائم max_angle = HB_PI / 16.f;
  hb_ویکټور_svg_ټکی_t center = {cx, cy};
  صحيح عدد_splits = (صحيح) ceilf (fabsf (a1 - a0) / max_angle);
  إذا (عدد_splits < 1) عدد_splits = 1;

  hb_ویکټور_svg_ټکی_t p0 = {cosf (a0), sinf (a0)};
  hb_ویکټور_svg_rgba_t color0 = c0_دننه;

  لكل (صحيح a = 0; a < عدد_splits; a++)
  {
    عائم k = (a + 1.f) / عدد_splits;
    عائم angle1 = hb_ویکټور_svg_lerp (a0, a1, k);
    hb_ویکټور_svg_rgba_t color1 = hb_ویکټور_svg_lerp_rgba (c0_دننه, c1_دننه, k);

    hb_ویکټور_svg_ټکی_t p1 = {cosf (angle1), sinf (angle1)};
    hb_ویکټور_svg_ټکی_t sp0 = hb_ویکټور_svg_جمع_کړه (center, hb_ویکټور_svg_scale (p0, radius));
    hb_ویکټور_svg_ټکی_t sp1 = hb_ویکټور_svg_جمع_کړه (center, hb_ویکټور_svg_scale (p1, radius));

    hb_ویکټور_svg_ټکی_t A = hb_ویکټور_svg_normalize (hb_ویکټور_svg_جمع_کړه (p0, p1));
    hb_ویکټور_svg_ټکی_t U = {-A.y, A.x};
    عائم up0 = hb_ویکټور_svg_dot (U, p0);
    عائم up1 = hb_ویکټور_svg_dot (U, p1);
    إذا (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      استمر;
    }
    hb_ویکټور_svg_ټکی_t C0 = hb_ویکټور_svg_جمع_کړه (A, hb_ویکټور_svg_scale (U, hb_ویکټور_svg_dot (hb_ویکټور_svg_sub (p0, A), p0) / up0));
    hb_ویکټور_svg_ټکی_t C1 = hb_ویکټور_svg_جمع_کړه (A, hb_ویکټور_svg_scale (U, hb_ویکټور_svg_dot (hb_ویکټور_svg_sub (p1, A), p1) / up1));

    hb_ویکټور_svg_ټکی_t sc0 = hb_ویکټور_svg_جمع_کړه (center, hb_ویکټور_svg_scale (hb_ویکټور_svg_جمع_کړه (C0, hb_ویکټور_svg_scale (hb_ویکټور_svg_sub (C0, p0), 0.33333f)), radius));
    hb_ویکټور_svg_ټکی_t sc1 = hb_ویکټور_svg_جمع_کړه (center, hb_ویکټور_svg_scale (hb_ویکټور_svg_جمع_کړه (C1, hb_ویکټور_svg_scale (hb_ویکټور_svg_sub (C1, p1), 0.33333f)), radius));

    hb_ویکټور_svg_rgba_t mid_color = hb_ویکټور_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_ویکټور_svg_hb_color_from_rgba (mid_color);

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
    body->append_svg_color (mid, ریښتیا);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
بنية hb_ویکټور_svg_sweep_ctx_t
{
  hb_ویکټور_buf_t *body;
  بینښه precision;
  عائم cx, cy, radius;
};

ساكن فراغ
hb_ویکټور_svg_sweep_emit_patch (عائم a0, hb_color_t c0,
				عائم a1, hb_color_t c1,
				فراغ *user_معلومات)
{
  خپلکاری *ctx = (hb_ویکټور_svg_sweep_ctx_t *) user_معلومات;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  إذا (unlikely (ctx->body->دننه_error ()))
    أرجع;
  hb_ویکټور_svg_جمع_کړه_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_ویکټور_svg_rgba_from_hb_color (c0),
				 a1, hb_ویکټور_svg_rgba_from_hb_color (c1));
}


ساكن فراغ hb_ویکټور_paint_ټېل_transform (hb_paint_funcs_t *, فراغ *,
                                            عائم, عائم, عائم, عائم, عائم, عائم,
                                            فراغ *);
ساكن فراغ hb_ویکټور_paint_راوباسه_transform (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_ویکټور_paint_fill_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, فراغ *);
ساكن فراغ hb_ویکټور_paint_ټېل_clip_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, فراغ *);
ساكن فراغ hb_ویکټور_paint_ټېل_clip_rectangle (hb_paint_funcs_t *, فراغ *, عائم, عائم, عائم, عائم, فراغ *);
ساكن hb_draw_funcs_t * hb_ویکټور_paint_ټېل_clip_لار_پیل (hb_paint_funcs_t *, فراغ *, فراغ **, فراغ *);
ساكن فراغ hb_ویکټور_paint_ټېل_clip_لار_پای_ټکی (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_ویکټور_paint_راوباسه_clip (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_ویکټور_paint_color (hb_paint_funcs_t *, فراغ *, hb_bool_t, hb_color_t, فراغ *);
ساكن hb_bool_t hb_ویکټور_paint_image (hb_paint_funcs_t *, فراغ *, hb_blob_t *, بینښه, بینښه, hb_نښان_t, عائم, hb_glyph_extents_t *, فراغ *);
ساكن فراغ hb_ویکټور_paint_خطي_gradient (hb_paint_funcs_t *, فراغ *, hb_color_کرښه_t *, عائم, عائم, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_ویکټور_paint_radial_gradient (hb_paint_funcs_t *, فراغ *, hb_color_کرښه_t *, عائم, عائم, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_ویکټور_paint_sweep_gradient (hb_paint_funcs_t *, فراغ *, hb_color_کرښه_t *, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_ویکټور_paint_ټېل_ډله (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_ویکټور_paint_راوباسه_ډله (hb_paint_funcs_t *, فراغ *, hb_paint_مرکب_ډېرپېښ_t, فراغ *);
ساكن hb_bool_t hb_ویکټور_paint_color_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, فراغ *);
ساكن hb_bool_t
hb_ویکټور_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_ناکارول_شوی,
				      فراغ *paint_معلومات,
				      بینښه color_شاخص,
				      hb_color_t *color,
				      فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_ویکټور_paint_t *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (!color)
    أرجع خطأ;

  hb_color_t *value = تش_اشاره;
  إذا (!paint->custom_palette_colors.has (color_شاخص, &value) || !value)
    أرجع خطأ;
  *color = *value;
  أرجع ریښتیا;
}

ساكن مضمن فراغ free_static_ویکټور_paint_funcs ();
ساكن بنية hb_ویکټور_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_ویکټور_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_سېټ_ټېل_transform_func (funcs, (hb_paint_ټېل_transform_func_t) hb_ویکټور_paint_ټېل_transform, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_transform_func (funcs, (hb_paint_راوباسه_transform_func_t) hb_ویکټور_paint_راوباسه_transform, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_ویکټور_paint_fill_glyph, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_clip_glyph_func (funcs, (hb_paint_ټېل_clip_glyph_func_t) hb_ویکټور_paint_ټېل_clip_glyph, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_clip_rectangle_func (funcs, (hb_paint_ټېل_clip_rectangle_func_t) hb_ویکټور_paint_ټېل_clip_rectangle, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_clip_لار_پیل_func (funcs, (hb_paint_ټېل_clip_لار_پیل_func_t) hb_ویکټور_paint_ټېل_clip_لار_پیل, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_clip_لار_پای_ټکی_func (funcs, (hb_paint_ټېل_clip_لار_پای_ټکی_func_t) hb_ویکټور_paint_ټېل_clip_لار_پای_ټکی, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_clip_func (funcs, (hb_paint_راوباسه_clip_func_t) hb_ویکټور_paint_راوباسه_clip, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_color_func (funcs, (hb_paint_color_func_t) hb_ویکټور_paint_color, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_image_func (funcs, (hb_paint_image_func_t) hb_ویکټور_paint_image, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_خطي_gradient_func (funcs, (hb_paint_خطي_gradient_func_t) hb_ویکټور_paint_خطي_gradient, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_ویکټور_paint_radial_gradient, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_ویکټور_paint_sweep_gradient, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_ډله_func (funcs, (hb_paint_ټېل_ډله_func_t) hb_ویکټور_paint_ټېل_ډله, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_ډله_func (funcs, (hb_paint_راوباسه_ډله_func_t) hb_ویکټور_paint_راوباسه_ډله, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_ویکټور_paint_color_glyph, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_ویکټور_paint_custom_palette_color, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_سېټ_budget_func (funcs, hb_ویکټور_paint_سېټ_budget_callback, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_get_budget_func (funcs, hb_ویکټور_paint_get_budget_callback, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_get_budget_پاتې_شوی_func (funcs, hb_ویکټور_paint_get_budget_پاتې_شوی_callback, تش_اشاره, تش_اشاره);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_ویکټور_paint_funcs);
    أرجع funcs;
  }
} static_ویکټور_paint_funcs;

ساكن مضمن فراغ
free_static_ویکټور_paint_funcs ()
{
  static_ویکټور_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_ویکټور_paint_svg_funcs_get ()
{
  أرجع static_ویکټور_paint_funcs.get_unconst ();
}


ساكن فراغ
hb_ویکټور_paint_ټېل_transform (hb_paint_funcs_t *,
                                فراغ *paint_معلومات,
                                عائم xx, عائم yx,
                                عائم xy, عائم yy,
                                عائم dx, عائم dy,
                                فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (unlikely (paint->transform_ډله_ډکېدنه_ژوروالی))
  {
    paint->transform_ډله_ډکېدنه_ژوروالی++;
    أرجع;
  }
  إذا (unlikely (paint->transform_ډله_ژوروالی >= 64))
  {
    paint->transform_ډله_ډکېدنه_ژوروالی = 1;
    أرجع;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_ډله_open_ماسک = (paint->transform_ډله_open_ماسک << 1) | (opened ? 1ull : 0ull);
  paint->transform_ډله_ژوروالی++;

  إذا (!opened)
    أرجع;

  خپلکاری &body = paint->current_body ();
  بینښه sprec = paint->defs.scale_precision ();
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
hb_ویکټور_paint_راوباسه_transform (hb_paint_funcs_t *,
                               فراغ *paint_معلومات,
                               فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (unlikely (paint->transform_ډله_ډکېدنه_ژوروالی))
  {
    paint->transform_ډله_ډکېدنه_ژوروالی--;
    أرجع;
  }
  إذا (!paint->transform_ډله_ژوروالی)
    أرجع;
  paint->transform_ډله_ژوروالی--;
  hb_bool_t opened = !!(paint->transform_ډله_open_ماسک & 1ull);
  paint->transform_ډله_open_ماسک >>= 1;
  إذا (opened)
    paint->current_body ().append_str ("</g>\n");
}

ساكن فراغ
hb_ویکټور_paint_fill_glyph (hb_paint_funcs_t *,
                            فراغ *paint_معلومات,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  paint->لار.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  إذا (likely (paint->budget_پاتې_شوی >= 0))
  {
    hb_ویکټور_لار_sink_t sink = {&paint->لار, paint->get_precision (),
				 paint->x_scale_ضریب, paint->y_scale_ضریب,
				 &paint->budget_پاتې_شوی};
    hb_font_draw_glyph (font, glyph, hb_ویکټور_svg_لار_draw_funcs_get (), &sink);
  }

  خپلکاری &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->لار.arrayZ, paint->لار.اوږدتیا);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, ریښتیا);
  body.append_str ("\"/>\n");
}

ساكن فراغ
hb_ویکټور_paint_ټېل_clip_glyph (hb_paint_funcs_t *,
                                 فراغ *paint_معلومات,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_مختاړی;
  بینښه pfx_len = paint->id_مختاړی_اوږدتیا;

  paint->لار.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  إذا (likely (paint->budget_پاتې_شوی >= 0))
  {
    hb_ویکټور_لار_sink_t sink = {&paint->لار, paint->get_precision (),
				 paint->x_scale_ضریب, paint->y_scale_ضریب,
				 &paint->budget_پاتې_شوی};
    hb_font_draw_glyph (font, glyph, hb_ویکټور_svg_لار_draw_funcs_get (), &sink);
  }

  بینښه def_id = paint->لار_def_شمېر++;
  paint->defs.append_str ("<path id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\" d=\"");
  paint->defs.append_len (paint->لار.arrayZ, paint->لار.اوږدتیا);
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
hb_ویکټور_paint_ټېل_clip_rectangle (hb_paint_funcs_t *,
                                     فراغ *paint_معلومات,
                                     عائم xmin, عائم ymin,
                                     عائم xmax, عائم ymax,
                                     فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_مختاړی;
  بینښه pfx_len = paint->id_مختاړی_اوږدتیا;
  بینښه clip_id = paint->clip_rect_counter++;
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
hb_ویکټور_paint_ټېل_clip_لار_پیل (hb_paint_funcs_t *,
                                      فراغ *paint_معلومات,
                                      فراغ **draw_معلومات,
                                      فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
  {
    *draw_معلومات = تش_اشاره;
    أرجع تش_اشاره;
  }

  paint->لار.clear ();
  paint->clip_لار_sink = {&paint->لار, paint->get_precision (),
			   paint->x_scale_ضریب,
			   paint->y_scale_ضریب,
			   &paint->budget_پاتې_شوی};
  *draw_معلومات = &paint->clip_لار_sink;
  أرجع hb_ویکټور_svg_لار_draw_funcs_get ();
}

ساكن فراغ
hb_ویکټور_paint_ټېل_clip_لار_پای_ټکی (hb_paint_funcs_t *,
                                    فراغ *paint_معلومات,
                                    فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_مختاړی;
  بینښه pfx_len = paint->id_مختاړی_اوږدتیا;
  بینښه clip_id = paint->clip_لار_counter++;

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
  paint->defs.append_len (paint->لار.arrayZ, paint->لار.اوږدتیا);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("cp");
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ساكن فراغ
hb_ویکټور_paint_راوباسه_clip (hb_paint_funcs_t *,
                          فراغ *paint_معلومات,
                          فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("</g>\n");
}

ساكن فراغ
hb_ویکټور_paint_color (hb_paint_funcs_t *,
                       فراغ *paint_معلومات,
                       hb_bool_t,
                       hb_color_t color,
                       فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خپلکاری &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, ریښتیا);
  body.append_str ("\"/>\n");
}

ساكن hb_bool_t
hb_ویکټور_paint_image (hb_paint_funcs_t *,
                       فراغ *paint_معلومات,
                       hb_blob_t *image,
                       بینښه پلنوالی,
                       بینښه لوړوالی,
                       hb_نښان_t format,
                       عائم slant HB_ناکارول_شوی,
                       hb_glyph_extents_t *extents,
                       فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;

  خپلکاری &body = paint->current_body ();
  إذا (format == HB_نښان ('p','n','g',' '))
  {
    إذا (!extents || !پلنوالی || !لوړوالی)
      أرجع خطأ;

    بینښه len = 0;
    ثابت حرف *png_معلومات = hb_blob_get_معلومات (image, &len);
    إذا (!png_معلومات || !len)
      أرجع خطأ;

    body.append_str ("<g transform=\"translate(");
    body.append_عدد (paint->sx ((عائم) extents->x_bearing));
    body.append_c (',');
    body.append_عدد (paint->sy ((عائم) extents->y_bearing));
    body.append_str (") scale(");
    body.append_عدد (paint->sx ((عائم) extents->پلنوالی) / پلنوالی);
    body.append_c (',');
    body.append_عدد (paint->sy ((عائم) extents->لوړوالی) / لوړوالی);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ثابت uint8_t *) png_معلومات, len);
    body.append_str ("\" width=\"");
    body.append_عدد ((عائم) پلنوالی);
    body.append_str ("\" height=\"");
    body.append_عدد ((عائم) لوړوالی);
    body.append_str ("\"/>\n</g>\n");

    أرجع ریښتیا;
  }

  أرجع خطأ;
}

ساكن فراغ
hb_ویکټور_paint_خطي_gradient (hb_paint_funcs_t *,
                                 فراغ *paint_معلومات,
                                 hb_color_کرښه_t *color_کرښه,
                                 عائم x0, عائم y0,
                                 عائم x1, عائم y1,
                                 عائم x2, عائم y2,
                                 فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_کرښه))
    أرجع;
  hb_ویکټور_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  عائم mn, mx;
  hb_paint_normalize_color_کرښه (stops.arrayZ, stops.اوږدتیا, &mn, &mx);

  ثابت حرف *pfx = paint->id_مختاړی;
  بینښه pfx_len = paint->id_مختاړی_اوږدتیا;
  بینښه grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  عائم lx0, ly0, lx1, ly1;
  hb_paint_reduce_خطي_anchors (x0, y0, x1, y1, x2, y2,
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
  paint->defs.append_str (hb_ویکټور_svg_extend_ډېرپېښ_str (hb_color_کرښه_get_extend (color_کرښه)));
  paint->defs.append_str ("\">\n");
  hb_ویکټور_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</linearGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

ساكن فراغ
hb_ویکټور_paint_radial_gradient (hb_paint_funcs_t *,
                                 فراغ *paint_معلومات,
                                 hb_color_کرښه_t *color_کرښه,
                                 عائم x0, عائم y0, عائم r0,
                                 عائم x1, عائم y1, عائم r1,
                                 فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_کرښه))
    أرجع;
  hb_ویکټور_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_کرښه (stops.arrayZ, stops.اوږدتیا, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gr0 = r0 + mn * (r1 - r0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);
  عائم gr1 = r0 + mx * (r1 - r0);

  ثابت حرف *pfx = paint->id_مختاړی;
  بینښه pfx_len = paint->id_مختاړی_اوږدتیا;
  بینښه grad_id = paint->gradient_counter++;

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
  paint->defs.append_str (hb_ویکټور_svg_extend_ډېرپېښ_str (hb_color_کرښه_get_extend (color_کرښه)));
  paint->defs.append_str ("\">\n");
  hb_ویکټور_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</radialGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

ساكن فراغ
hb_ویکټور_paint_sweep_gradient (hb_paint_funcs_t *,
                                فراغ *paint_معلومات,
                                hb_color_کرښه_t *color_کرښه,
                                عائم cx, عائم cy,
                                عائم پیل_angle, عائم پای_ټکی_angle,
                                فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_کرښه))
    أرجع;
  hb_ویکټور_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_کرښه (stops.arrayZ, stops.اوږدتیا, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  عائم ga0 = پیل_angle + mn * (پای_ټکی_angle - پیل_angle);
  عائم ga1 = پیل_angle + mx * (پای_ټکی_angle - پیل_angle);

  hb_ویکټور_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.اوږدتیا,
				 hb_color_کرښه_get_extend (color_کرښه),
				 ga0, ga1,
				 hb_ویکټور_svg_sweep_emit_patch,
				 &ctx);
}

ساكن فراغ
hb_ویکټور_paint_ټېل_ډله (hb_paint_funcs_t *,
                            فراغ *paint_معلومات,
                            فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (unlikely (!paint->ډله_ډېری.ټېل_or_fail (hb_ویکټور_buf_t {})))
    أرجع;
}

ساكن فراغ
hb_ویکټور_paint_راوباسه_ډله (hb_paint_funcs_t *,
                           فراغ *paint_معلومات,
                           hb_paint_مرکب_ډېرپېښ_t ډېرپېښ,
                           فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (paint->ډله_ډېری.اوږدتیا < 2)
    أرجع;

  hb_ویکټور_buf_t ډله = paint->ډله_ډېری.راوباسه ();
  خپلکاری &body = paint->current_body ();

  ثابت حرف *blend = hb_ویکټور_svg_مرکب_ډېرپېښ_str (ډېرپېښ);
  إذا (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (ډله.arrayZ, ډله.اوږدتیا);
    body.append_str ("</g>\n");
  }
  وإلا
    body.append_len (ډله.arrayZ, ډله.اوږدتیا);
}

ساكن hb_bool_t
hb_ویکټور_paint_color_glyph (hb_paint_funcs_t *,
                             فراغ *paint_معلومات,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             فراغ *)
{
  خپلکاری *paint = (hb_ویکټور_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;
  إذا (unlikely (paint->color_glyph_ژوروالی >= HB_MAX_NESTING_LEVEL))
    أرجع خطأ;
  إذا (unlikely (hb_سېټ_has (paint->فعال_color_glyphs, glyph)))
    أرجع خطأ;

  paint->color_glyph_ژوروالی++;
  hb_سېټ_جمع_کړه (paint->فعال_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_ویکټور_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_سېټ_del (paint->فعال_color_glyphs, glyph);
  paint->color_glyph_ژوروالی--;
  أرجع ریښتیا;
}




hb_blob_t *
hb_ویکټور_paint_render_svg (hb_ویکټور_paint_t *paint)
{
  إذا (!paint->has_extents)
    أرجع تش_اشاره;

  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع تش_اشاره;

  hb_ویکټور_buf_t بهر;
  hb_buf_recover_recycled (paint->recycled_blob, &بهر);
  بینښه estimated = paint->defs.اوږدتیا +
		       paint->ډله_ډېری.arrayZ[0].اوږدتیا +
		       320;
  بهر.alloc (estimated);
  عائم vb_x = paint->extents.x;
  عائم vb_y = -(paint->extents.y + paint->extents.لوړوالی);
  عائم vb_w = paint->extents.پلنوالی;
  عائم vb_h = paint->extents.لوړوالی;
  بهر.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  بهر.append_عدد (vb_x);
  بهر.append_c (' ');
  بهر.append_عدد (vb_y);
  بهر.append_c (' ');
  بهر.append_عدد (vb_w);
  بهر.append_c (' ');
  بهر.append_عدد (vb_h);
  بهر.append_str ("\" width=\"");
  بهر.append_عدد (vb_w);
  بهر.append_str ("\" height=\"");
  بهر.append_عدد (vb_h);
  بهر.append_str ("\">\n");

  إذا (paint->defs.اوږدتیا)
  {
    بهر.append_str ("<defs>\n");
    بهر.append_len (paint->defs.arrayZ, paint->defs.اوږدتیا);
    بهر.append_str ("</defs>\n");
  }

  إذا (hb_color_get_alpha (paint->background))
  {
    بهر.append_str ("<rect x=\"");
    بهر.append_عدد (vb_x);
    بهر.append_str ("\" y=\"");
    بهر.append_عدد (vb_y);
    بهر.append_str ("\" width=\"");
    بهر.append_عدد (vb_w);
    بهر.append_str ("\" height=\"");
    بهر.append_عدد (vb_h);
    بهر.append_str ("\" fill=\"rgb(");
    بهر.append_unsigned (hb_color_get_red (paint->background));
    بهر.append_c (',');
    بهر.append_unsigned (hb_color_get_green (paint->background));
    بهر.append_c (',');
    بهر.append_unsigned (hb_color_get_blue (paint->background));
    بهر.append_str (")\"");
    إذا (hb_color_get_alpha (paint->background) < 255)
    {
      بهر.append_str (" fill-opacity=\"");
      بهر.append_عدد (hb_color_get_alpha (paint->background) / 255.f, 4);
      بهر.append_c ('"');
    }
    بهر.append_str ("/>\n");
  }

  بهر.append_str ("<g transform=\"scale(1,-1)\">\n");
  بهر.append_len (paint->ډله_ډېری.arrayZ[0].arrayZ, paint->ډله_ډېری.arrayZ[0].اوږدتیا);
  بهر.append_str ("</g>\n");

  بهر.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &بهر);

  أرجع blob;
}
