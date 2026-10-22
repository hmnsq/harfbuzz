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

#ضمن "hb-raster-image.hh"

#ضمن <math.h>

/**
 * SECTION:hb-raster
 * @title: hb-raster
 * @short_description: Glyph rasterization
 * @include: hb-raster.h
 *
 * Functions for rasterizing glyph outlines into pixel buffers.
 *
 * #hb_raster_draw_t rasterizes outline geometry and always outputs
 * @HB_RASTER_FORMAT_A8. Typical flow:
 *
 * |[<!-- language="plain" -->
 * hb_raster_draw_t *draw = hb_raster_draw_create_or_fail ();
 * hb_raster_draw_set_scale_factor (draw, 64.f, 64.f);
 * hb_raster_draw_set_transform (draw, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f);
 * hb_glyph_extents_t glyph_extents;
 * hb_font_get_glyph_extents (font, gid, &glyph_extents);
 * hb_raster_draw_set_glyph_extents (draw, &glyph_extents);
 * hb_raster_draw_glyph (draw, font, gid);
 * hb_raster_image_t *mask = hb_raster_draw_render (draw);
 * hb_raster_extents_t image_extents;
 * hb_raster_image_get_extents (mask, &image_extents);
 * ]|
 *
 * #hb_raster_paint_t renders color paint graphs and always outputs
 * @HB_RASTER_FORMAT_BGRA32. Typical flow:
 *
 * |[<!-- language="plain" -->
 * hb_raster_paint_t *paint = hb_raster_paint_create_or_fail ();
 * hb_raster_paint_set_scale_factor (paint, 64.f, 64.f);
 * hb_raster_paint_set_transform (paint, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f);
 * hb_raster_paint_set_foreground (paint, foreground);
 * hb_glyph_extents_t glyph_extents;
 * hb_font_get_glyph_extents (font, gid, &glyph_extents);
 * hb_raster_paint_set_glyph_extents (paint, &glyph_extents);
 * hb_raster_paint_glyph (paint, font, gid);
 * hb_raster_image_t *img = hb_raster_paint_render (paint);
 * hb_raster_extents_t image_extents;
 * hb_raster_image_get_extents (img, &image_extents);
 * ]|
 *
 * In both modes, set extents explicitly (or via glyph extents) before
 * rendering to avoid implicit allocations and to get deterministic bounds.
 * When caching individual glyph images for later composition, position each
 * image using the `x_origin` and `y_origin` members returned by
 * hb_raster_image_get_extents().  These are the pixel-space left and bottom
 * edges after raster-bound rounding; do not reconstruct them from glyph
 * bearings and image dimensions.
 **/

#جے_تعریف_شدہ HAVE_PNG
#ضمن <png.h>
#جے_انت

#جے_تعریف_شدہ HAVE_PNG
بنية hb_raster_png_read_blob_t
{
  ثابت uint8_t *انکڑے = خالی_سوچک;
  size_t ناپ = 0;
  size_t وتھ = 0;
  uint8_t *rgba = خالی_سوچک;
  png_bytep *صفاں = خالی_سوچک;
};

ساكن فراغ
hb_raster_png_error (png_structp png,
		     png_const_charp پیغام HB_بن_ورتیا)
{
#جے_تعریف_شدہ PNG_SETJMP_SUPPORTED
  longjmp (png_jmpbuf (png), 1);
#جے_انت
}

ساكن فراغ
hb_raster_png_warning (png_structp png HB_بن_ورتیا,
		       png_const_charp پیغام HB_بن_ورتیا)
{}

ساكن فراغ
hb_raster_png_read_blob (png_structp png, png_bytep باہرلا, png_ناپ_t لمائی)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  إذا (!r || !r->انکڑے || لمائی > r->ناپ - r->وتھ)
    png_error (png, "read error");

  hb_memcpy (باہرلا, r->انکڑے + r->وتھ, لمائی);
  r->وتھ += لمائی;
}

ساكن فراغ
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  إذا (!r)
    أرجع;

  hb_free (r->rgba);
  hb_free (r->صفاں);
  hb_free (r);
}

بنية hb_raster_png_write_blob_t
{
  حرف *انکڑے = خالی_سوچک;
  size_t لمائی = 0;
  size_t allocated = 0;
  uint8_t *rgba = خالی_سوچک;
  png_bytep *صفاں = خالی_سوچک;
};

ساكن فراغ
hb_raster_png_write_blob (png_structp png, png_bytep وچ, png_ناپ_t لمائی)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  إذا (!w)
    png_error (png, "write error");

  size_t old_لمائی = w->لمائی;
  إذا ((size_t) لمائی > (size_t) -1 - old_لمائی)
    png_error (png, "write error");

  size_t new_لمائی = old_لمائی + لمائی;
  إذا (new_لمائی > w->allocated)
  {
    size_t new_allocated = w->allocated ? w->allocated : 4096;
    طالما (new_allocated < new_لمائی)
    {
      size_t next = new_allocated * 2;
      إذا (next <= new_allocated)
      {
	new_allocated = new_لمائی;
	اكسر;
      }
      new_allocated = next;
    }

    حرف *انکڑے = (حرف *) hb_realloc (w->انکڑے, new_allocated);
    إذا (!انکڑے)
      png_error (png, "write error");
    w->انکڑے = انکڑے;
    w->allocated = new_allocated;
  }

  hb_memcpy (w->انکڑے + old_لمائی, وچ, لمائی);
  w->لمائی = new_لمائی;
}

ساكن فراغ
hb_raster_png_flush_blob (png_structp png HB_بن_ورتیا)
{}

ساكن فراغ
hb_raster_png_write_blob_fini (hb_raster_png_write_blob_t *w)
{
  إذا (!w)
    أرجع;

  hb_free (w->انکڑے);
  hb_free (w->rgba);
  hb_free (w->صفاں);
  hb_free (w);
}
#جے_انت


/*
 * Image compositing
 */

/* Unpack premultiplied pixel to float RGBA [0,1]. */
ساكن مضمن فراغ
unpack_منزل_float (uint32_t px, عائم &r, عائم &g, عائم &b, عائم &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
ساكن مضمن uint32_t
pack_from_float (عائم r, عائم g, عائم b, عائم a)
{
  أرجع hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
ساكن مضمن عائم
blend_multiply (عائم sc, عائم dc) { أرجع sc * dc; }
ساكن مضمن عائم
blend_screen (عائم sc, عائم dc) { أرجع sc + dc - sc * dc; }
ساكن مضمن عائم
blend_overlay (عائم sc, عائم dc)
{ أرجع dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
ساكن مضمن عائم
blend_darken (عائم sc, عائم dc) { أرجع hb_min (sc, dc); }
ساكن مضمن عائم
blend_lighten (عائم sc, عائم dc) { أرجع hb_max (sc, dc); }
ساكن مضمن عائم
blend_color_dodge (عائم sc, عائم dc)
{
  إذا (dc <= 0.f) أرجع 0.f;
  إذا (sc >= 1.f) أرجع 1.f;
  أرجع hb_min (1.f, dc / (1.f - sc));
}
ساكن مضمن عائم
blend_color_burn (عائم sc, عائم dc)
{
  إذا (dc >= 1.f) أرجع 1.f;
  إذا (sc <= 0.f) أرجع 0.f;
  أرجع 1.f - hb_min (1.f, (1.f - dc) / sc);
}
ساكن مضمن عائم
blend_hard_light (عائم sc, عائم dc)
{ أرجع sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
ساكن مضمن عائم
blend_soft_light (عائم sc, عائم dc)
{
  إذا (sc <= 0.5f)
    أرجع dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  عائم d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  أرجع dc + (2.f * sc - 1.f) * (d - dc);
}
ساكن مضمن عائم
blend_difference (عائم sc, عائم dc) { أرجع fabsf (sc - dc); }
ساكن مضمن عائم
blend_exclusion (عائم sc, عائم dc) { أرجع sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
ساكن مضمن uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       عائم (*blend_fn)(عائم, عائم))
{
  عائم sr, sg, sb, sa;
  عائم dr, dg, db, da;
  unpack_منزل_float (src, sr, sg, sb, sa);
  unpack_منزل_float (dst, dr, dg, db, da);

  عائم usr = sa > 0.f ? sr / sa : 0.f;
  عائم usg = sa > 0.f ? sg / sa : 0.f;
  عائم usb = sa > 0.f ? sb / sa : 0.f;
  عائم udr = da > 0.f ? dr / da : 0.f;
  عائم udg = da > 0.f ? dg / da : 0.f;
  عائم udb = da > 0.f ? db / da : 0.f;

  عائم br = blend_fn (usr, udr);
  عائم bg = blend_fn (usg, udg);
  عائم bb = blend_fn (usb, udb);

  عائم ra = sa + da - sa * da;
  عائم rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  عائم rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  عائم rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  أرجع pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
ساكن مضمن عائم
hsl_luminosity (عائم r, عائم g, عائم b)
{ أرجع 0.299f * r + 0.587f * g + 0.114f * b; }

ساكن مضمن عائم
hsl_saturation (عائم r, عائم g, عائم b)
{ أرجع hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

ساكن مضمن فراغ
hsl_clip_color (عائم &r, عائم &g, عائم &b)
{
  عائم l = hsl_luminosity (r, g, b);
  عائم mn = hb_min (hb_min (r, g), b);
  عائم mx = hb_max (hb_max (r, g), b);
  إذا (mn < 0.f)
  {
    عائم d = l - mn;
    إذا (d > 0.f) { r = l + (r - l) * l / d; g = l + (g - l) * l / d; b = l + (b - l) * l / d; }
  }
  إذا (mx > 1.f)
  {
    عائم d = mx - l;
    إذا (d > 0.f) { r = l + (r - l) * (1.f - l) / d; g = l + (g - l) * (1.f - l) / d; b = l + (b - l) * (1.f - l) / d; }
  }
}

ساكن مضمن فراغ
hsl_سیٹ_luminosity (عائم &r, عائم &g, عائم &b, عائم l)
{
  عائم d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

ساكن مضمن فراغ
hsl_سیٹ_saturation_inner (عائم &mn, عائم &mid, عائم &mx, عائم s)
{
  إذا (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  وإلا
    mid = mx = 0.f;
  mn = 0.f;
}

ساكن مضمن فراغ
hsl_سیٹ_saturation (عائم &r, عائم &g, عائم &b, عائم s)
{
  إذا (r <= g)
  {
    إذا (g <= b)      hsl_سیٹ_saturation_inner (r, g, b, s);
    وإلا إذا (r <= b) hsl_سیٹ_saturation_inner (r, b, g, s);
    وإلا             hsl_سیٹ_saturation_inner (b, r, g, s);
  }
  وإلا
  {
    إذا (r <= b)      hsl_سیٹ_saturation_inner (g, r, b, s);
    وإلا إذا (g <= b) hsl_سیٹ_saturation_inner (g, b, r, s);
    وإلا             hsl_سیٹ_saturation_inner (b, g, r, s);
  }
}

ساكن مضمن uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_مرکب_منوال_t منوال)
{
  عائم sr, sg, sb, sa;
  عائم dr, dg, db, da;
  unpack_منزل_float (src, sr, sg, sb, sa);
  unpack_منزل_float (dst, dr, dg, db, da);

  عائم usr = sa > 0.f ? sr / sa : 0.f;
  عائم usg = sa > 0.f ? sg / sa : 0.f;
  عائم usb = sa > 0.f ? sb / sa : 0.f;
  عائم udr = da > 0.f ? dr / da : 0.f;
  عائم udg = da > 0.f ? dg / da : 0.f;
  عائم udb = da > 0.f ? db / da : 0.f;

  عائم br = udr, bg = udg, bb = udb;

  إذا (منوال == HB_PAINT_مرکب_منوال_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_سیٹ_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_سیٹ_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  وإلا إذا (منوال == HB_PAINT_مرکب_منوال_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_سیٹ_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_سیٹ_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  وإلا إذا (منوال == HB_PAINT_مرکب_منوال_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_سیٹ_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  وإلا /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_سیٹ_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  عائم ra = sa + da - sa * da;
  عائم rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  عائم rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  عائم rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  أرجع pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
ساكن مضمن uint32_t
مرکب_pixel (uint32_t src, uint32_t dst,
		 hb_paint_مرکب_منوال_t منوال)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  بدل (منوال)
  {
  حالة HB_PAINT_مرکب_منوال_CLEAR:
    أرجع 0;
  حالة HB_PAINT_مرکب_منوال_SRC:
    أرجع src;
  حالة HB_PAINT_مرکب_منوال_DEST:
    أرجع dst;
  حالة HB_PAINT_مرکب_منوال_SRC_OVER:
    أرجع hb_raster_src_over (src, dst);
  حالة HB_PAINT_مرکب_منوال_DEST_OVER:
    أرجع hb_raster_src_over (dst, src);
  حالة HB_PAINT_مرکب_منوال_SRC_وچ:
    أرجع hb_raster_alpha_mul (src, da);
  حالة HB_PAINT_مرکب_منوال_DEST_وچ:
    أرجع hb_raster_alpha_mul (dst, sa);
  حالة HB_PAINT_مرکب_منوال_SRC_باہرلا:
    أرجع hb_raster_alpha_mul (src, 255 - da);
  حالة HB_PAINT_مرکب_منوال_DEST_باہرلا:
    أرجع hb_raster_alpha_mul (dst, 255 - sa);
  حالة HB_PAINT_مرکب_منوال_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (بنانشانی) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (بنانشانی) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (بنانشانی) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (بنانشانی) (a >> 24) + (b >> 24));
    أرجع hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  حالة HB_PAINT_مرکب_منوال_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (بنانشانی) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (بنانشانی) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (بنانشانی) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (بنانشانی) (a >> 24) + (b >> 24));
    أرجع hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  حالة HB_PAINT_مرکب_منوال_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (بنانشانی) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (بنانشانی) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (بنانشانی) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (بنانشانی) (a >> 24) + (b >> 24));
    أرجع hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  حالة HB_PAINT_مرکب_منوال_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (بنانشانی) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (بنانشانی) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (بنانشانی) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (بنانشانی) (src >> 24) + (dst >> 24));
    أرجع hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  حالة HB_PAINT_مرکب_منوال_MULTIPLY:  أرجع apply_separable_blend (src, dst, blend_multiply);
  حالة HB_PAINT_مرکب_منوال_SCREEN:    أرجع apply_separable_blend (src, dst, blend_screen);
  حالة HB_PAINT_مرکب_منوال_OVERLAY:   أرجع apply_separable_blend (src, dst, blend_overlay);
  حالة HB_PAINT_مرکب_منوال_DARKEN:    أرجع apply_separable_blend (src, dst, blend_darken);
  حالة HB_PAINT_مرکب_منوال_LIGHTEN:   أرجع apply_separable_blend (src, dst, blend_lighten);
  حالة HB_PAINT_مرکب_منوال_COLOR_DODGE: أرجع apply_separable_blend (src, dst, blend_color_dodge);
  حالة HB_PAINT_مرکب_منوال_COLOR_BURN:  أرجع apply_separable_blend (src, dst, blend_color_burn);
  حالة HB_PAINT_مرکب_منوال_HARD_LIGHT:  أرجع apply_separable_blend (src, dst, blend_hard_light);
  حالة HB_PAINT_مرکب_منوال_SOFT_LIGHT:  أرجع apply_separable_blend (src, dst, blend_soft_light);
  حالة HB_PAINT_مرکب_منوال_DIFFERENCE:  أرجع apply_separable_blend (src, dst, blend_difference);
  حالة HB_PAINT_مرکب_منوال_EXCLUSION:   أرجع apply_separable_blend (src, dst, blend_exclusion);

  حالة HB_PAINT_مرکب_منوال_HSL_HUE:
  حالة HB_PAINT_مرکب_منوال_HSL_SATURATION:
  حالة HB_PAINT_مرکب_منوال_HSL_COLOR:
  حالة HB_PAINT_مرکب_منوال_HSL_LUMINOSITY:
    أرجع apply_hsl_blend (src, dst, منوال);

  افتراضي:
    أرجع hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

بنانشانی
hb_raster_image_t::بائٹاں_فی_pixel (hb_raster_format_t format)
{
  أرجع format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

منطقي
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  إذا (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  بنانشانی bpp = بائٹاں_فی_pixel (format);
  إذا (extents.چوڑائی > UINT_MAX / bpp)
    أرجع خطأ;

  بنانشانی min_stride = extents.چوڑائی * bpp;
  إذا (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  إذا (extents.اچائی && extents.stride > (size_t) -1 / extents.اچائی)
    أرجع خطأ;

  size_t buf_ناپ = (size_t) extents.stride * extents.اچائی;
  إذا (buf_ناپ > HB_RASTER_MAX_بفر_ناپ)
    أرجع خطأ;
  إذا (unlikely (!بفر.resize_dirty (buf_ناپ)))
    أرجع خطأ;

  هذا->format = format;
  هذا->extents = extents;
  أرجع سچ;
}

منطقي
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#جے_بنا_تعریف HAVE_PNG
  أرجع خطأ;
#وإلا
  إذا (!blob)
    أرجع خطأ;

  بنانشانی blob_len = 0;
  ثابت uint8_t *blob_انکڑے = (ثابت uint8_t *) hb_blob_get_انکڑے (blob, &blob_len);
  إذا (!blob_انکڑے || !blob_len)
    أرجع خطأ;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, خالی_سوچک,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  إذا (!png)
    أرجع خطأ;

  png_infop info = png_create_info_struct (png);
  إذا (!info)
  {
    png_destroy_read_struct (&png, خالی_سوچک, خالی_سوچک);
    أرجع خطأ;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, حجم (*reader));
  إذا (!reader)
  {
    png_destroy_read_struct (&png, &info, خالی_سوچک);
    أرجع خطأ;
  }

  reader->انکڑے = blob_انکڑے;
  reader->ناپ = (size_t) blob_len;
  png_سیٹ_read_fn (png, reader, hb_raster_png_read_blob);
  إذا (setjmp (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, خالی_سوچک);
    hb_raster_png_read_blob_fini (reader);
    أرجع خطأ;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  صحيح ثنائی_ہندسہ_ڈونگھائی_2 = 0, color_type = 0;
  صحيح interlace_type = 0, compression_type = 0, filter_طریقہ_2 = 0;
  png_get_IHDR (png, info, &w, &h, &ثنائی_ہندسہ_ڈونگھائی_2, &color_type,
		&interlace_type, &compression_type, &filter_طریقہ_2);

  إذا (!w || !h || w > (png_uint_32) ودھ_توں_ودھ_عدد || h > (png_uint_32) ودھ_توں_ودھ_عدد)
  {
    png_destroy_read_struct (&png, &info, خالی_سوچک);
    hb_raster_png_read_blob_fini (reader);
    أرجع خطأ;
  }

  منطقي has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  إذا (ثنائی_ہندسہ_ڈونگھائی_2 == 16)
    png_سیٹ_strip_16 (png);
  إذا (color_type == PNG_COLOR_TYPE_PALETTE)
    png_سیٹ_palette_منزل_rgb (png);
  إذا (color_type == PNG_COLOR_TYPE_سلیٹی && ثنائی_ہندسہ_ڈونگھائی_2 < 8)
    png_سیٹ_expand_سلیٹی_1_2_4_منزل_8 (png);
  إذا (has_trns)
    png_سیٹ_t_RNS_منزل_alpha (png);
  إذا (color_type == PNG_COLOR_TYPE_سلیٹی || color_type == PNG_COLOR_TYPE_سلیٹی_ALPHA)
    png_سیٹ_سلیٹی_منزل_rgb (png);
  إذا (!(color_type & PNG_COLOR_نقاب_ALPHA) && !has_trns)
    png_سیٹ_جوڑو_alpha (png, 0xff, PNG_FILLER_AFTER);
  إذا (interlace_type != PNG_INTERLACE_NONE)
    png_سیٹ_interlace_handling (png);

  png_read_update_info (png, info);

  إذا (png_get_ثنائی_ہندسہ_ڈونگھائی (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, خالی_سوچک);
    hb_raster_png_read_blob_fini (reader);
    أرجع خطأ;
  }

  png_ناپ_t rowbytes = png_get_rowbytes (png, info);
  إذا (rowbytes < (png_ناپ_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, خالی_سوچک);
    hb_raster_png_read_blob_fini (reader);
    أرجع خطأ;
  }

  size_t rgba_ناپ = (size_t) rowbytes * (size_t) h;
  إذا ((h && rgba_ناپ / (size_t) h != (size_t) rowbytes) ||
      rgba_ناپ > HB_RASTER_MAX_بفر_ناپ)
  {
    png_destroy_read_struct (&png, &info, خالی_سوچک);
    hb_raster_png_read_blob_fini (reader);
    أرجع خطأ;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_ناپ);
  إذا (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, خالی_سوچک);
    hb_raster_png_read_blob_fini (reader);
    أرجع خطأ;
  }

  size_t صفاں_ناپ = (size_t) h * حجم (png_bytep);
  إذا (h && صفاں_ناپ / (size_t) h != حجم (png_bytep))
  {
    png_destroy_read_struct (&png, &info, خالی_سوچک);
    hb_raster_png_read_blob_fini (reader);
    أرجع خطأ;
  }

  reader->صفاں = (png_bytep *) hb_malloc (صفاں_ناپ);
  إذا (!reader->صفاں)
  {
    png_destroy_read_struct (&png, &info, خالی_سوچک);
    hb_raster_png_read_blob_fini (reader);
    أرجع خطأ;
  }

  لكل (بنانشانی y = 0; y < (بنانشانی) h; y++)
    reader->صفاں[y] = (png_bytep) (reader->rgba + (size_t) y * (size_t) rowbytes);

  png_read_image (png, reader->صفاں);
  png_read_سرا (png, خالی_سوچک);
  png_destroy_read_struct (&png, &info, خالی_سوچک);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (بنانشانی) w, (بنانشانی) h, 0};
  إذا (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    أرجع خطأ;
  }

  لكل (بنانشانی y = 0; y < (بنانشانی) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.بفر.arrayZ + (size_t) ((بنانشانی) h - 1 - y) * decoded.extents.stride);
    ثابت uint8_t *src = reader->rgba + (size_t) y * (size_t) rowbytes;
    لكل (بنانشانی x = 0; x < (بنانشانی) w; x++)
    {
      uint8_t r = src[4 * x + 0];
      uint8_t g = src[4 * x + 1];
      uint8_t b = src[4 * x + 2];
      uint8_t a = src[4 * x + 3];
      dst[x] = hb_packed_t<uint32_t> ((uint32_t) hb_raster_div255 (b * a)
				    | ((uint32_t) hb_raster_div255 (g * a) << 8)
				    | ((uint32_t) hb_raster_div255 (r * a) << 16)
				    | ((uint32_t) a << 24));
    }
  }

  hb_وٹاندرا (بفر, decoded.بفر);
  hb_وٹاندرا (هذا->extents, decoded.extents);
  hb_وٹاندرا (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  أرجع سچ;
#جے_انت
}

hb_blob_t *
hb_raster_image_t::serialize_منزل_png_or_fail () ثابت
{
#جے_بنا_تعریف HAVE_PNG
  أرجع خالی_سوچک;
#وإلا
  إذا (format != HB_RASTER_FORMAT_BGRA32 || !extents.چوڑائی || !extents.اچائی)
    أرجع خالی_سوچک;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, خالی_سوچک,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  إذا (!png)
    أرجع خالی_سوچک;

  png_infop info = png_create_info_struct (png);
  إذا (!info)
  {
    png_destroy_write_struct (&png, خالی_سوچک);
    أرجع خالی_سوچک;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, حجم (*writer));
  إذا (!writer)
  {
    png_destroy_write_struct (&png, &info);
    أرجع خالی_سوچک;
  }

  png_سیٹ_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  إذا (setjmp (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    أرجع خالی_سوچک;
  }

  png_سیٹ_IHDR (png, info,
		extents.چوڑائی, extents.اچائی,
		8, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  size_t rowbytes = (size_t) extents.چوڑائی * 4u;
  size_t rgba_ناپ = rowbytes * (size_t) extents.اچائی;
  إذا (extents.اچائی && rgba_ناپ / (size_t) extents.اچائی != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    أرجع خالی_سوچک;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_ناپ);
  إذا (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    أرجع خالی_سوچک;
  }

  size_t صفاں_ناپ = (size_t) extents.اچائی * حجم (png_bytep);
  إذا (extents.اچائی && صفاں_ناپ / (size_t) extents.اچائی != حجم (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    أرجع خالی_سوچک;
  }

  writer->صفاں = (png_bytep *) hb_malloc (صفاں_ناپ);
  إذا (!writer->صفاں)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    أرجع خالی_سوچک;
  }

  لكل (بنانشانی y = 0; y < extents.اچائی; y++)
  {
    uint8_t *dst = writer->rgba + (size_t) y * rowbytes;
    ثابت uint8_t *src = بفر.arrayZ + (size_t) (extents.اچائی - 1 - y) * extents.stride;

    لكل (بنانشانی x = 0; x < extents.چوڑائی; x++)
    {
      uint32_t px;
      hb_memcpy (&px, src + x * 4, 4);
      uint8_t b = (uint8_t) (px & 0xFF);
      uint8_t g = (uint8_t) ((px >> 8) & 0xFF);
      uint8_t r = (uint8_t) ((px >> 16) & 0xFF);
      uint8_t a = (uint8_t) (px >> 24);

      dst[4 * x + 3] = a;
      إذا (a)
      {
	dst[4 * x + 0] = (uint8_t) hb_min (255u, ((بنانشانی) r * 255u + a / 2u) / a);
	dst[4 * x + 1] = (uint8_t) hb_min (255u, ((بنانشانی) g * 255u + a / 2u) / a);
	dst[4 * x + 2] = (uint8_t) hb_min (255u, ((بنانشانی) b * 255u + a / 2u) / a);
      }
      وإلا
	dst[4 * x + 0] = dst[4 * x + 1] = dst[4 * x + 2] = 0;
    }

    writer->صفاں[y] = (png_bytep) dst;
  }

  png_write_image (png, writer->صفاں);
  png_write_سرا (png, info);
  png_destroy_write_struct (&png, &info);

  إذا (writer->لمائی > (size_t) (بنانشانی) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    أرجع خالی_سوچک;
  }

  بنانشانی لمائی = (بنانشانی) writer->لمائی;
  حرف *انکڑے = writer->انکڑے;
  writer->انکڑے = خالی_سوچک;
  hb_raster_png_write_blob_fini (writer);
  إذا (!انکڑے && لمائی)
    أرجع خالی_سوچک;

  hb_blob_t *blob = hb_blob_create_or_fail (انکڑے, لمائی,
					    HB_حافظہ_منوال_WRITABLE,
					    انکڑے, hb_free);
  إذا (!blob)
    hb_free (انکڑے);
  أرجع blob;
#جے_انت
}

فراغ
hb_raster_image_t::clear ()
{
  size_t buf_ناپ = (size_t) extents.stride * extents.اچائی;
  hb_memset (بفر.arrayZ, 0, buf_ناپ);
}

ثابت uint8_t *
hb_raster_image_t::get_بفر () ثابت
{
  أرجع بفر.arrayZ;
}

فراغ
hb_raster_image_t::مرکب_from (ثابت hb_raster_image_t *src,
				   hb_paint_مرکب_منوال_t منوال)
{
  بنانشانی w = extents.چوڑائی;
  بنانشانی h = extents.اچائی;
  بنانشانی stride = extents.stride;

  لكل (بنانشانی y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (بفر.arrayZ + y * stride);
    ثابت hb_packed_t<uint32_t> *sp = (ثابت hb_packed_t<uint32_t> *) (src->بفر.arrayZ + y * stride);
    لكل (بنانشانی x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (مرکب_pixel ((uint32_t) sp[x], (uint32_t) dp[x], منوال));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
فراغ
hb_raster_image_مرکب (hb_raster_image_t *dst,
			   ثابت hb_raster_image_t *src,
			   hb_paint_مرکب_منوال_t منوال)
{
  dst->مرکب_from (src, منوال);
}

/**
 * hb_raster_image_create_or_fail:
 *
 * Creates a new raster image object.
 *
 * Return value: (transfer full):
 * A newly allocated #hb_raster_image_t with a reference count of 1,
 * or `NULL` on allocation failure.
 *
 * The returned image can be released with hb_raster_image_destroy(), or
 * transferred for reuse with hb_raster_draw_recycle_image() or
 * hb_raster_paint_recycle_image().
 *
 * Since: 13.0.0
 **/
hb_raster_image_t *
hb_raster_image_create_or_fail (فراغ)
{
  أرجع hb_object_create<hb_raster_image_t> ();
}

/**
 * hb_raster_image_reference: (skip)
 * @image: a raster image
 *
 * Increases the reference count on @image by one.
 *
 * This prevents @image from being destroyed until a matching
 * call to hb_raster_image_destroy() is made.
 *
 * Return value: (transfer full):
 * The referenced #hb_raster_image_t.
 *
 * Since: 13.0.0
 **/
hb_raster_image_t *
hb_raster_image_reference (hb_raster_image_t *image)
{
  أرجع hb_object_reference (image);
}

/**
 * hb_raster_image_destroy: (skip)
 * @image: a raster image
 *
 * Decreases the reference count on @image by one. When the
 * reference count reaches zero, the image and its pixel buffer
 * are freed.
 *
 * Since: 13.0.0
 **/
فراغ
hb_raster_image_destroy (hb_raster_image_t *image)
{
  إذا (!hb_object_destroy (image)) أرجع;
  hb_free (image);
}

/**
 * hb_raster_image_set_user_data: (skip)
 * @image: a raster image
 * @key: the user-data key
 * @data: a pointer to the user data
 * @destroy: (nullable): a callback to call when @data is not needed anymore
 * @replace: whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified raster image.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 13.0.0
 **/
hb_bool_t
hb_raster_image_سیٹ_user_انکڑے (hb_raster_image_t  *image,
			       hb_user_انکڑے_کلید_t *key,
			       فراغ               *انکڑے,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  أرجع hb_object_سیٹ_user_انکڑے (image, key, انکڑے, destroy, replace);
}

/**
 * hb_raster_image_get_user_data: (skip)
 * @image: a raster image
 * @key: the user-data key
 *
 * Fetches the user-data associated with the specified key,
 * attached to the specified raster image.
 *
 * Return value: (transfer none):
 * A pointer to the user data
 *
 * Since: 13.0.0
 **/
فراغ *
hb_raster_image_get_user_انکڑے (ثابت hb_raster_image_t  *image,
			       hb_user_انکڑے_کلید_t *key)
{
  أرجع hb_object_get_user_انکڑے (image, key);
}

/**
 * hb_raster_image_configure:
 * @image: a raster image
 * @format: the pixel format
 * @extents: (nullable): desired image extents
 *
 * Configures @image format and extents together, resizing backing storage
 * at most once. This function does not clear pixel contents.
 *
 * Passing `NULL` for @extents clears extents and releases the backing
 * allocation.
 *
 * Return value: `true` if configuration succeeds, `false` on allocation
 * failure
 *
 * Since: 13.0.0
 **/
hb_bool_t
hb_raster_image_configure (hb_raster_image_t         *image,
			   hb_raster_format_t        format,
			   ثابت hb_raster_extents_t *extents)
{
  إذا (unlikely (!extents))
  {
    image->extents = {};
    image->بفر.resize_exact (0);
    أرجع سچ;
  }
  أرجع image->configure (format, *extents);
}

/**
 * hb_raster_image_clear:
 * @image: a raster image
 *
 * Clears @image pixels to zero while keeping current extents and format.
 *
 * Since: 13.0.0
 **/
فراغ
hb_raster_image_clear (hb_raster_image_t *image)
{
  image->clear ();
}

/**
 * hb_raster_image_get_buffer:
 * @image: a raster image
 *
 * Fetches the raw pixel buffer of @image.  The buffer layout is
 * described by the extents obtained from hb_raster_image_get_extents()
 * and the format from hb_raster_image_get_format(). Rows are stored
 * bottom-to-top.
 *
 * Return value: (transfer none) (array):
 * The pixel buffer, or `NULL`
 *
 * Since: 13.0.0
 **/
ثابت uint8_t *
hb_raster_image_get_بفر (ثابت hb_raster_image_t *image)
{
  أرجع image->get_بفر ();
}

/**
 * hb_raster_image_get_extents:
 * @image: a raster image
 * @extents: (out) (nullable): the image extents
 *
 * Fetches the pixel-buffer extents of @image.
 *
 * Since: 13.0.0
 **/
فراغ
hb_raster_image_get_extents (ثابت hb_raster_image_t   *image,
			     hb_raster_extents_t *extents)
{
  إذا (extents)
    *extents = image->extents;
}

/**
 * hb_raster_image_get_format:
 * @image: a raster image
 *
 * Fetches the pixel format of @image.
 *
 * Return value:
 * The #hb_raster_format_t of the image
 *
 * Since: 13.0.0
 **/
hb_raster_format_t
hb_raster_image_get_format (ثابت hb_raster_image_t *image)
{
  أرجع image->format;
}

/**
 * hb_raster_image_deserialize_from_png_or_fail:
 * @image: a raster image
 * @png: PNG data
 *
 * Replaces @image contents by deserializing a PNG blob into a
 * #HB_RASTER_FORMAT_BGRA32 raster image.
 *
 * On success, @image extents are reset to pixel extents with origin
 * `(0, 0)`. Rows in the resulting image buffer are stored bottom-to-top.
 * On failure, @image is left unchanged.
 *
 * Return value: `true` if deserialization succeeded, `false` otherwise
 *
 * Since: 13.1.0
 **/
hb_bool_t
hb_raster_image_deserialize_from_png_or_fail (hb_raster_image_t *image,
					      hb_blob_t         *png)
{
  أرجع image->deserialize_from_png (png);
}

/**
 * hb_raster_image_serialize_to_png_or_fail:
 * @image: a raster image
 *
 * Serializes @image to a PNG blob.
 *
 * Currently only #HB_RASTER_FORMAT_BGRA32 images are supported.
 *
 * Return value: (transfer full):
 * A newly allocated PNG #hb_blob_t, or `NULL` on failure
 *
 * Since: 13.1.0
 **/
hb_blob_t *
hb_raster_image_serialize_منزل_png_or_fail (ثابت hb_raster_image_t *image)
{
  أرجع image->serialize_منزل_png_or_fail ();
}
