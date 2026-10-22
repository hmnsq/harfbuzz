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

#شامل "hb-raster-image.hh"

#شامل <math.h>

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

#اگر_تعریف_شدہ HAVE_PNG
#شامل <png.h>
#اگر_ختم

#اگر_تعریف_شدہ HAVE_PNG
ساخت hb_raster_png_read_blob_t
{
  ثابت uint8_t *ڈیٹا = خالی_اشاریہ;
  size_t سائز = 0;
  size_t انحراف = 0;
  uint8_t *rgba = خالی_اشاریہ;
  png_bytep *صفیں = خالی_اشاریہ;
};

ساکن خالی
hb_raster_png_error (png_structp png,
		     png_const_charp پیغام_2 HB_غیر_مستعمل)
{
#اگر_تعریف_شدہ PNG_SETJMP_SUPPORTED
  longjmp (png_jmpbuf (png), 1);
#اگر_ختم
}

ساکن خالی
hb_raster_png_warning (png_structp png HB_غیر_مستعمل,
		       png_const_charp پیغام_2 HB_غیر_مستعمل)
{}

ساکن خالی
hb_raster_png_read_blob (png_structp png, png_bytep برآمد, png_سائز_t لمبائی)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  اگر (!r || !r->ڈیٹا || لمبائی > r->سائز - r->انحراف)
    png_error (png, "read error");

  hb_memcpy (برآمد, r->ڈیٹا + r->انحراف, لمبائی);
  r->انحراف += لمبائی;
}

ساکن خالی
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  اگر (!r)
    واپس;

  hb_free (r->rgba);
  hb_free (r->صفیں);
  hb_free (r);
}

ساخت hb_raster_png_write_blob_t
{
  حرف *ڈیٹا = خالی_اشاریہ;
  size_t لمبائی = 0;
  size_t allocated = 0;
  uint8_t *rgba = خالی_اشاریہ;
  png_bytep *صفیں = خالی_اشاریہ;
};

ساکن خالی
hb_raster_png_write_blob (png_structp png, png_bytep اندر, png_سائز_t لمبائی)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  اگر (!w)
    png_error (png, "write error");

  size_t old_لمبائی = w->لمبائی;
  اگر ((size_t) لمبائی > (size_t) -1 - old_لمبائی)
    png_error (png, "write error");

  size_t new_لمبائی = old_لمبائی + لمبائی;
  اگر (new_لمبائی > w->allocated)
  {
    size_t new_allocated = w->allocated ? w->allocated : 4096;
    جب (new_allocated < new_لمبائی)
    {
      size_t next = new_allocated * 2;
      اگر (next <= new_allocated)
      {
	new_allocated = new_لمبائی;
	توڑو;
      }
      new_allocated = next;
    }

    حرف *ڈیٹا = (حرف *) hb_realloc (w->ڈیٹا, new_allocated);
    اگر (!ڈیٹا)
      png_error (png, "write error");
    w->ڈیٹا = ڈیٹا;
    w->allocated = new_allocated;
  }

  hb_memcpy (w->ڈیٹا + old_لمبائی, اندر, لمبائی);
  w->لمبائی = new_لمبائی;
}

ساکن خالی
hb_raster_png_flush_blob (png_structp png HB_غیر_مستعمل)
{}

ساکن خالی
hb_raster_png_write_blob_fini (hb_raster_png_write_blob_t *w)
{
  اگر (!w)
    واپس;

  hb_free (w->ڈیٹا);
  hb_free (w->rgba);
  hb_free (w->صفیں);
  hb_free (w);
}
#اگر_ختم


/*
 * Image compositing
 */

/* Unpack premultiplied pixel to float RGBA [0,1]. */
ساکن اندرونی خالی
unpack_مقصد_float (uint32_t px, اعشاری &r, اعشاری &g, اعشاری &b, اعشاری &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
ساکن اندرونی uint32_t
pack_from_float (اعشاری r, اعشاری g, اعشاری b, اعشاری a)
{
  واپس hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
ساکن اندرونی اعشاری
blend_multiply (اعشاری sc, اعشاری dc) { واپس sc * dc; }
ساکن اندرونی اعشاری
blend_screen (اعشاری sc, اعشاری dc) { واپس sc + dc - sc * dc; }
ساکن اندرونی اعشاری
blend_overlay (اعشاری sc, اعشاری dc)
{ واپس dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
ساکن اندرونی اعشاری
blend_darken (اعشاری sc, اعشاری dc) { واپس hb_min (sc, dc); }
ساکن اندرونی اعشاری
blend_lighten (اعشاری sc, اعشاری dc) { واپس hb_max (sc, dc); }
ساکن اندرونی اعشاری
blend_color_dodge (اعشاری sc, اعشاری dc)
{
  اگر (dc <= 0.f) واپس 0.f;
  اگر (sc >= 1.f) واپس 1.f;
  واپس hb_min (1.f, dc / (1.f - sc));
}
ساکن اندرونی اعشاری
blend_color_burn (اعشاری sc, اعشاری dc)
{
  اگر (dc >= 1.f) واپس 1.f;
  اگر (sc <= 0.f) واپس 0.f;
  واپس 1.f - hb_min (1.f, (1.f - dc) / sc);
}
ساکن اندرونی اعشاری
blend_hard_light (اعشاری sc, اعشاری dc)
{ واپس sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
ساکن اندرونی اعشاری
blend_soft_light (اعشاری sc, اعشاری dc)
{
  اگر (sc <= 0.5f)
    واپس dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  اعشاری d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  واپس dc + (2.f * sc - 1.f) * (d - dc);
}
ساکن اندرونی اعشاری
blend_difference (اعشاری sc, اعشاری dc) { واپس fabsf (sc - dc); }
ساکن اندرونی اعشاری
blend_exclusion (اعشاری sc, اعشاری dc) { واپس sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
ساکن اندرونی uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       اعشاری (*blend_fn)(اعشاری, اعشاری))
{
  اعشاری sr, sg, sb, sa;
  اعشاری dr, dg, db, da;
  unpack_مقصد_float (src, sr, sg, sb, sa);
  unpack_مقصد_float (dst, dr, dg, db, da);

  اعشاری usr = sa > 0.f ? sr / sa : 0.f;
  اعشاری usg = sa > 0.f ? sg / sa : 0.f;
  اعشاری usb = sa > 0.f ? sb / sa : 0.f;
  اعشاری udr = da > 0.f ? dr / da : 0.f;
  اعشاری udg = da > 0.f ? dg / da : 0.f;
  اعشاری udb = da > 0.f ? db / da : 0.f;

  اعشاری br = blend_fn (usr, udr);
  اعشاری bg = blend_fn (usg, udg);
  اعشاری bb = blend_fn (usb, udb);

  اعشاری ra = sa + da - sa * da;
  اعشاری rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  اعشاری rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  اعشاری rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  واپس pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
ساکن اندرونی اعشاری
hsl_luminosity (اعشاری r, اعشاری g, اعشاری b)
{ واپس 0.299f * r + 0.587f * g + 0.114f * b; }

ساکن اندرونی اعشاری
hsl_saturation (اعشاری r, اعشاری g, اعشاری b)
{ واپس hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

ساکن اندرونی خالی
hsl_clip_color (اعشاری &r, اعشاری &g, اعشاری &b)
{
  اعشاری l = hsl_luminosity (r, g, b);
  اعشاری mn = hb_min (hb_min (r, g), b);
  اعشاری mx = hb_max (hb_max (r, g), b);
  اگر (mn < 0.f)
  {
    اعشاری d = l - mn;
    اگر (d > 0.f) { r = l + (r - l) * l / d; g = l + (g - l) * l / d; b = l + (b - l) * l / d; }
  }
  اگر (mx > 1.f)
  {
    اعشاری d = mx - l;
    اگر (d > 0.f) { r = l + (r - l) * (1.f - l) / d; g = l + (g - l) * (1.f - l) / d; b = l + (b - l) * (1.f - l) / d; }
  }
}

ساکن اندرونی خالی
hsl_سیٹ_luminosity (اعشاری &r, اعشاری &g, اعشاری &b, اعشاری l)
{
  اعشاری d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

ساکن اندرونی خالی
hsl_سیٹ_saturation_inner (اعشاری &mn, اعشاری &mid, اعشاری &mx, اعشاری s)
{
  اگر (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  ورنہ
    mid = mx = 0.f;
  mn = 0.f;
}

ساکن اندرونی خالی
hsl_سیٹ_saturation (اعشاری &r, اعشاری &g, اعشاری &b, اعشاری s)
{
  اگر (r <= g)
  {
    اگر (g <= b)      hsl_سیٹ_saturation_inner (r, g, b, s);
    ورنہ اگر (r <= b) hsl_سیٹ_saturation_inner (r, b, g, s);
    ورنہ             hsl_سیٹ_saturation_inner (b, r, g, s);
  }
  ورنہ
  {
    اگر (r <= b)      hsl_سیٹ_saturation_inner (g, r, b, s);
    ورنہ اگر (g <= b) hsl_سیٹ_saturation_inner (g, b, r, s);
    ورنہ             hsl_سیٹ_saturation_inner (b, g, r, s);
  }
}

ساکن اندرونی uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_مرکب_منوال_t منوال)
{
  اعشاری sr, sg, sb, sa;
  اعشاری dr, dg, db, da;
  unpack_مقصد_float (src, sr, sg, sb, sa);
  unpack_مقصد_float (dst, dr, dg, db, da);

  اعشاری usr = sa > 0.f ? sr / sa : 0.f;
  اعشاری usg = sa > 0.f ? sg / sa : 0.f;
  اعشاری usb = sa > 0.f ? sb / sa : 0.f;
  اعشاری udr = da > 0.f ? dr / da : 0.f;
  اعشاری udg = da > 0.f ? dg / da : 0.f;
  اعشاری udb = da > 0.f ? db / da : 0.f;

  اعشاری br = udr, bg = udg, bb = udb;

  اگر (منوال == HB_PAINT_مرکب_منوال_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_سیٹ_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_سیٹ_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  ورنہ اگر (منوال == HB_PAINT_مرکب_منوال_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_سیٹ_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_سیٹ_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  ورنہ اگر (منوال == HB_PAINT_مرکب_منوال_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_سیٹ_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  ورنہ /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_سیٹ_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  اعشاری ra = sa + da - sa * da;
  اعشاری rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  اعشاری rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  اعشاری rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  واپس pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
ساکن اندرونی uint32_t
مرکب_pixel (uint32_t src, uint32_t dst,
		 hb_paint_مرکب_منوال_t منوال)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  انتخاب (منوال)
  {
  حالت HB_PAINT_مرکب_منوال_CLEAR:
    واپس 0;
  حالت HB_PAINT_مرکب_منوال_SRC:
    واپس src;
  حالت HB_PAINT_مرکب_منوال_DEST:
    واپس dst;
  حالت HB_PAINT_مرکب_منوال_SRC_OVER:
    واپس hb_raster_src_over (src, dst);
  حالت HB_PAINT_مرکب_منوال_DEST_OVER:
    واپس hb_raster_src_over (dst, src);
  حالت HB_PAINT_مرکب_منوال_SRC_اندر:
    واپس hb_raster_alpha_mul (src, da);
  حالت HB_PAINT_مرکب_منوال_DEST_اندر:
    واپس hb_raster_alpha_mul (dst, sa);
  حالت HB_PAINT_مرکب_منوال_SRC_برآمد:
    واپس hb_raster_alpha_mul (src, 255 - da);
  حالت HB_PAINT_مرکب_منوال_DEST_برآمد:
    واپس hb_raster_alpha_mul (dst, 255 - sa);
  حالت HB_PAINT_مرکب_منوال_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (غیرعلامتی) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (غیرعلامتی) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (غیرعلامتی) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (غیرعلامتی) (a >> 24) + (b >> 24));
    واپس hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  حالت HB_PAINT_مرکب_منوال_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (غیرعلامتی) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (غیرعلامتی) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (غیرعلامتی) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (غیرعلامتی) (a >> 24) + (b >> 24));
    واپس hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  حالت HB_PAINT_مرکب_منوال_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (غیرعلامتی) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (غیرعلامتی) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (غیرعلامتی) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (غیرعلامتی) (a >> 24) + (b >> 24));
    واپس hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  حالت HB_PAINT_مرکب_منوال_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (غیرعلامتی) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (غیرعلامتی) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (غیرعلامتی) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (غیرعلامتی) (src >> 24) + (dst >> 24));
    واپس hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  حالت HB_PAINT_مرکب_منوال_MULTIPLY:  واپس apply_separable_blend (src, dst, blend_multiply);
  حالت HB_PAINT_مرکب_منوال_SCREEN:    واپس apply_separable_blend (src, dst, blend_screen);
  حالت HB_PAINT_مرکب_منوال_OVERLAY:   واپس apply_separable_blend (src, dst, blend_overlay);
  حالت HB_PAINT_مرکب_منوال_DARKEN:    واپس apply_separable_blend (src, dst, blend_darken);
  حالت HB_PAINT_مرکب_منوال_LIGHTEN:   واپس apply_separable_blend (src, dst, blend_lighten);
  حالت HB_PAINT_مرکب_منوال_COLOR_DODGE: واپس apply_separable_blend (src, dst, blend_color_dodge);
  حالت HB_PAINT_مرکب_منوال_COLOR_BURN:  واپس apply_separable_blend (src, dst, blend_color_burn);
  حالت HB_PAINT_مرکب_منوال_HARD_LIGHT:  واپس apply_separable_blend (src, dst, blend_hard_light);
  حالت HB_PAINT_مرکب_منوال_SOFT_LIGHT:  واپس apply_separable_blend (src, dst, blend_soft_light);
  حالت HB_PAINT_مرکب_منوال_DIFFERENCE:  واپس apply_separable_blend (src, dst, blend_difference);
  حالت HB_PAINT_مرکب_منوال_EXCLUSION:   واپس apply_separable_blend (src, dst, blend_exclusion);

  حالت HB_PAINT_مرکب_منوال_HSL_HUE:
  حالت HB_PAINT_مرکب_منوال_HSL_SATURATION:
  حالت HB_PAINT_مرکب_منوال_HSL_COLOR:
  حالت HB_PAINT_مرکب_منوال_HSL_LUMINOSITY:
    واپس apply_hsl_blend (src, dst, منوال);

  طےشدہ:
    واپس hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

غیرعلامتی
hb_raster_image_t::بائٹیں_فی_pixel (hb_raster_format_t format)
{
  واپس format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

منطقی
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  اگر (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  غیرعلامتی bpp = بائٹیں_فی_pixel (format);
  اگر (extents.چوڑائی > UINT_MAX / bpp)
    واپس جھوٹ;

  غیرعلامتی min_stride = extents.چوڑائی * bpp;
  اگر (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  اگر (extents.اونچائی && extents.stride > (size_t) -1 / extents.اونچائی)
    واپس جھوٹ;

  size_t buf_سائز = (size_t) extents.stride * extents.اونچائی;
  اگر (buf_سائز > HB_RASTER_MAX_بفر_سائز)
    واپس جھوٹ;
  اگر (unlikely (!بفر.resize_dirty (buf_سائز)))
    واپس جھوٹ;

  یہ->format = format;
  یہ->extents = extents;
  واپس سچ;
}

منطقی
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#اگر_غیر_تعریف_شدہ HAVE_PNG
  واپس جھوٹ;
#ورنہ
  اگر (!blob)
    واپس جھوٹ;

  غیرعلامتی blob_len = 0;
  ثابت uint8_t *blob_ڈیٹا = (ثابت uint8_t *) hb_blob_get_ڈیٹا (blob, &blob_len);
  اگر (!blob_ڈیٹا || !blob_len)
    واپس جھوٹ;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, خالی_اشاریہ,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  اگر (!png)
    واپس جھوٹ;

  png_infop info = png_create_info_struct (png);
  اگر (!info)
  {
    png_destroy_read_struct (&png, خالی_اشاریہ, خالی_اشاریہ);
    واپس جھوٹ;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, حجم (*reader));
  اگر (!reader)
  {
    png_destroy_read_struct (&png, &info, خالی_اشاریہ);
    واپس جھوٹ;
  }

  reader->ڈیٹا = blob_ڈیٹا;
  reader->سائز = (size_t) blob_len;
  png_سیٹ_read_fn (png, reader, hb_raster_png_read_blob);
  اگر (setjmp (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, خالی_اشاریہ);
    hb_raster_png_read_blob_fini (reader);
    واپس جھوٹ;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  صحیح ثنائی_ہندسہ_گہرائی_2 = 0, color_type = 0;
  صحیح interlace_type = 0, compression_type = 0, filter_طریقہ_2 = 0;
  png_get_IHDR (png, info, &w, &h, &ثنائی_ہندسہ_گہرائی_2, &color_type,
		&interlace_type, &compression_type, &filter_طریقہ_2);

  اگر (!w || !h || w > (png_uint_32) زیادہ_سے_زیادہ_عدد || h > (png_uint_32) زیادہ_سے_زیادہ_عدد)
  {
    png_destroy_read_struct (&png, &info, خالی_اشاریہ);
    hb_raster_png_read_blob_fini (reader);
    واپس جھوٹ;
  }

  منطقی has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  اگر (ثنائی_ہندسہ_گہرائی_2 == 16)
    png_سیٹ_strip_16 (png);
  اگر (color_type == PNG_COLOR_TYPE_PALETTE)
    png_سیٹ_palette_مقصد_rgb (png);
  اگر (color_type == PNG_COLOR_TYPE_سرمئی && ثنائی_ہندسہ_گہرائی_2 < 8)
    png_سیٹ_expand_سرمئی_1_2_4_مقصد_8 (png);
  اگر (has_trns)
    png_سیٹ_t_RNS_مقصد_alpha (png);
  اگر (color_type == PNG_COLOR_TYPE_سرمئی || color_type == PNG_COLOR_TYPE_سرمئی_ALPHA)
    png_سیٹ_سرمئی_مقصد_rgb (png);
  اگر (!(color_type & PNG_COLOR_نقاب_ALPHA) && !has_trns)
    png_سیٹ_جوڑو_alpha (png, 0xff, PNG_FILLER_AFTER);
  اگر (interlace_type != PNG_INTERLACE_NONE)
    png_سیٹ_interlace_handling (png);

  png_read_update_info (png, info);

  اگر (png_get_ثنائی_ہندسہ_گہرائی (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, خالی_اشاریہ);
    hb_raster_png_read_blob_fini (reader);
    واپس جھوٹ;
  }

  png_سائز_t rowbytes = png_get_rowbytes (png, info);
  اگر (rowbytes < (png_سائز_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, خالی_اشاریہ);
    hb_raster_png_read_blob_fini (reader);
    واپس جھوٹ;
  }

  size_t rgba_سائز = (size_t) rowbytes * (size_t) h;
  اگر ((h && rgba_سائز / (size_t) h != (size_t) rowbytes) ||
      rgba_سائز > HB_RASTER_MAX_بفر_سائز)
  {
    png_destroy_read_struct (&png, &info, خالی_اشاریہ);
    hb_raster_png_read_blob_fini (reader);
    واپس جھوٹ;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_سائز);
  اگر (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, خالی_اشاریہ);
    hb_raster_png_read_blob_fini (reader);
    واپس جھوٹ;
  }

  size_t صفیں_سائز = (size_t) h * حجم (png_bytep);
  اگر (h && صفیں_سائز / (size_t) h != حجم (png_bytep))
  {
    png_destroy_read_struct (&png, &info, خالی_اشاریہ);
    hb_raster_png_read_blob_fini (reader);
    واپس جھوٹ;
  }

  reader->صفیں = (png_bytep *) hb_malloc (صفیں_سائز);
  اگر (!reader->صفیں)
  {
    png_destroy_read_struct (&png, &info, خالی_اشاریہ);
    hb_raster_png_read_blob_fini (reader);
    واپس جھوٹ;
  }

  برائے (غیرعلامتی y = 0; y < (غیرعلامتی) h; y++)
    reader->صفیں[y] = (png_bytep) (reader->rgba + (size_t) y * (size_t) rowbytes);

  png_read_image (png, reader->صفیں);
  png_read_انتہا (png, خالی_اشاریہ);
  png_destroy_read_struct (&png, &info, خالی_اشاریہ);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (غیرعلامتی) w, (غیرعلامتی) h, 0};
  اگر (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    واپس جھوٹ;
  }

  برائے (غیرعلامتی y = 0; y < (غیرعلامتی) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.بفر.arrayZ + (size_t) ((غیرعلامتی) h - 1 - y) * decoded.extents.stride);
    ثابت uint8_t *src = reader->rgba + (size_t) y * (size_t) rowbytes;
    برائے (غیرعلامتی x = 0; x < (غیرعلامتی) w; x++)
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

  hb_تبادلہ (بفر, decoded.بفر);
  hb_تبادلہ (یہ->extents, decoded.extents);
  hb_تبادلہ (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  واپس سچ;
#اگر_ختم
}

hb_blob_t *
hb_raster_image_t::serialize_مقصد_png_or_fail () ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_PNG
  واپس خالی_اشاریہ;
#ورنہ
  اگر (format != HB_RASTER_FORMAT_BGRA32 || !extents.چوڑائی || !extents.اونچائی)
    واپس خالی_اشاریہ;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, خالی_اشاریہ,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  اگر (!png)
    واپس خالی_اشاریہ;

  png_infop info = png_create_info_struct (png);
  اگر (!info)
  {
    png_destroy_write_struct (&png, خالی_اشاریہ);
    واپس خالی_اشاریہ;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, حجم (*writer));
  اگر (!writer)
  {
    png_destroy_write_struct (&png, &info);
    واپس خالی_اشاریہ;
  }

  png_سیٹ_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  اگر (setjmp (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    واپس خالی_اشاریہ;
  }

  png_سیٹ_IHDR (png, info,
		extents.چوڑائی, extents.اونچائی,
		8, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  size_t rowbytes = (size_t) extents.چوڑائی * 4u;
  size_t rgba_سائز = rowbytes * (size_t) extents.اونچائی;
  اگر (extents.اونچائی && rgba_سائز / (size_t) extents.اونچائی != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    واپس خالی_اشاریہ;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_سائز);
  اگر (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    واپس خالی_اشاریہ;
  }

  size_t صفیں_سائز = (size_t) extents.اونچائی * حجم (png_bytep);
  اگر (extents.اونچائی && صفیں_سائز / (size_t) extents.اونچائی != حجم (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    واپس خالی_اشاریہ;
  }

  writer->صفیں = (png_bytep *) hb_malloc (صفیں_سائز);
  اگر (!writer->صفیں)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    واپس خالی_اشاریہ;
  }

  برائے (غیرعلامتی y = 0; y < extents.اونچائی; y++)
  {
    uint8_t *dst = writer->rgba + (size_t) y * rowbytes;
    ثابت uint8_t *src = بفر.arrayZ + (size_t) (extents.اونچائی - 1 - y) * extents.stride;

    برائے (غیرعلامتی x = 0; x < extents.چوڑائی; x++)
    {
      uint32_t px;
      hb_memcpy (&px, src + x * 4, 4);
      uint8_t b = (uint8_t) (px & 0xFF);
      uint8_t g = (uint8_t) ((px >> 8) & 0xFF);
      uint8_t r = (uint8_t) ((px >> 16) & 0xFF);
      uint8_t a = (uint8_t) (px >> 24);

      dst[4 * x + 3] = a;
      اگر (a)
      {
	dst[4 * x + 0] = (uint8_t) hb_min (255u, ((غیرعلامتی) r * 255u + a / 2u) / a);
	dst[4 * x + 1] = (uint8_t) hb_min (255u, ((غیرعلامتی) g * 255u + a / 2u) / a);
	dst[4 * x + 2] = (uint8_t) hb_min (255u, ((غیرعلامتی) b * 255u + a / 2u) / a);
      }
      ورنہ
	dst[4 * x + 0] = dst[4 * x + 1] = dst[4 * x + 2] = 0;
    }

    writer->صفیں[y] = (png_bytep) dst;
  }

  png_write_image (png, writer->صفیں);
  png_write_انتہا (png, info);
  png_destroy_write_struct (&png, &info);

  اگر (writer->لمبائی > (size_t) (غیرعلامتی) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    واپس خالی_اشاریہ;
  }

  غیرعلامتی لمبائی = (غیرعلامتی) writer->لمبائی;
  حرف *ڈیٹا = writer->ڈیٹا;
  writer->ڈیٹا = خالی_اشاریہ;
  hb_raster_png_write_blob_fini (writer);
  اگر (!ڈیٹا && لمبائی)
    واپس خالی_اشاریہ;

  hb_blob_t *blob = hb_blob_create_or_fail (ڈیٹا, لمبائی,
					    HB_حافظہ_منوال_WRITABLE,
					    ڈیٹا, hb_free);
  اگر (!blob)
    hb_free (ڈیٹا);
  واپس blob;
#اگر_ختم
}

خالی
hb_raster_image_t::clear ()
{
  size_t buf_سائز = (size_t) extents.stride * extents.اونچائی;
  hb_memset (بفر.arrayZ, 0, buf_سائز);
}

ثابت uint8_t *
hb_raster_image_t::get_بفر () ثابت
{
  واپس بفر.arrayZ;
}

خالی
hb_raster_image_t::مرکب_from (ثابت hb_raster_image_t *src,
				   hb_paint_مرکب_منوال_t منوال)
{
  غیرعلامتی w = extents.چوڑائی;
  غیرعلامتی h = extents.اونچائی;
  غیرعلامتی stride = extents.stride;

  برائے (غیرعلامتی y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (بفر.arrayZ + y * stride);
    ثابت hb_packed_t<uint32_t> *sp = (ثابت hb_packed_t<uint32_t> *) (src->بفر.arrayZ + y * stride);
    برائے (غیرعلامتی x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (مرکب_pixel ((uint32_t) sp[x], (uint32_t) dp[x], منوال));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
خالی
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
hb_raster_image_create_or_fail (خالی)
{
  واپس hb_object_create<hb_raster_image_t> ();
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
  واپس hb_object_reference (image);
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
خالی
hb_raster_image_destroy (hb_raster_image_t *image)
{
  اگر (!hb_object_destroy (image)) واپس;
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
hb_raster_image_سیٹ_user_ڈیٹا (hb_raster_image_t  *image,
			       hb_user_ڈیٹا_کلید_t *key,
			       خالی               *ڈیٹا,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  واپس hb_object_سیٹ_user_ڈیٹا (image, key, ڈیٹا, destroy, replace);
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
خالی *
hb_raster_image_get_user_ڈیٹا (ثابت hb_raster_image_t  *image,
			       hb_user_ڈیٹا_کلید_t *key)
{
  واپس hb_object_get_user_ڈیٹا (image, key);
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
  اگر (unlikely (!extents))
  {
    image->extents = {};
    image->بفر.resize_exact (0);
    واپس سچ;
  }
  واپس image->configure (format, *extents);
}

/**
 * hb_raster_image_clear:
 * @image: a raster image
 *
 * Clears @image pixels to zero while keeping current extents and format.
 *
 * Since: 13.0.0
 **/
خالی
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
  واپس image->get_بفر ();
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
خالی
hb_raster_image_get_extents (ثابت hb_raster_image_t   *image,
			     hb_raster_extents_t *extents)
{
  اگر (extents)
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
  واپس image->format;
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
  واپس image->deserialize_from_png (png);
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
hb_raster_image_serialize_مقصد_png_or_fail (ثابت hb_raster_image_t *image)
{
  واپس image->serialize_مقصد_png_or_fail ();
}
