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

#ifdef HAVE_PNG
#شامل <png.h>
#endif

#ifdef HAVE_PNG
ساختار hb_raster_png_read_blob_t
{
  ثابت uint8_t *data = nullptr;
  size_t size = 0;
  size_t offset = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

ایستا تهی
hb_raster_png_error (png_structp png,
		     png_const_charp پیام HB_UNUSED)
{
#ifdef PNG_SETJMP_SUPPORTED
  longjmp (png_jmpbuf (png), 1);
#endif
}

ایستا تهی
hb_raster_png_warning (png_structp png HB_UNUSED,
		       png_const_charp پیام HB_UNUSED)
{}

ایستا تهی
hb_raster_png_read_blob (png_structp png, png_bytep out, png_size_t length)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  اگر (!r || !r->data || length > r->size - r->offset)
    png_error (png, "read error");

  hb_memcpy (out, r->data + r->offset, length);
  r->offset += length;
}

ایستا تهی
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  اگر (!r)
    بازگشت;

  hb_free (r->rgba);
  hb_free (r->rows);
  hb_free (r);
}

ساختار hb_raster_png_write_blob_t
{
  نویسه *data = nullptr;
  size_t length = 0;
  size_t allocated = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

ایستا تهی
hb_raster_png_write_blob (png_structp png, png_bytep in, png_size_t length)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  اگر (!w)
    png_error (png, "write error");

  size_t old_length = w->length;
  اگر ((size_t) length > (size_t) -1 - old_length)
    png_error (png, "write error");

  size_t new_length = old_length + length;
  اگر (new_length > w->allocated)
  {
    size_t new_allocated = w->allocated ? w->allocated : 4096;
    وقتی (new_allocated < new_length)
    {
      size_t next = new_allocated * 2;
      اگر (next <= new_allocated)
      {
	new_allocated = new_length;
	بشکن;
      }
      new_allocated = next;
    }

    نویسه *data = (نویسه *) hb_realloc (w->data, new_allocated);
    اگر (!data)
      png_error (png, "write error");
    w->data = data;
    w->allocated = new_allocated;
  }

  hb_memcpy (w->data + old_length, in, length);
  w->length = new_length;
}

ایستا تهی
hb_raster_png_flush_blob (png_structp png HB_UNUSED)
{}

ایستا تهی
hb_raster_png_write_blob_fini (hb_raster_png_write_blob_t *w)
{
  اگر (!w)
    بازگشت;

  hb_free (w->data);
  hb_free (w->rgba);
  hb_free (w->rows);
  hb_free (w);
}
#endif


/*
 * Image compositing
 */

/* Unpack premultiplied pixel to float RGBA [0,1]. */
ایستا درونخطی تهی
unpack_to_float (uint32_t px, اعشاری &r, اعشاری &g, اعشاری &b, اعشاری &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
ایستا درونخطی uint32_t
pack_from_float (اعشاری r, اعشاری g, اعشاری b, اعشاری a)
{
  بازگشت hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
ایستا درونخطی اعشاری
blend_multiply (اعشاری sc, اعشاری dc) { بازگشت sc * dc; }
ایستا درونخطی اعشاری
blend_screen (اعشاری sc, اعشاری dc) { بازگشت sc + dc - sc * dc; }
ایستا درونخطی اعشاری
blend_overlay (اعشاری sc, اعشاری dc)
{ بازگشت dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
ایستا درونخطی اعشاری
blend_darken (اعشاری sc, اعشاری dc) { بازگشت hb_min (sc, dc); }
ایستا درونخطی اعشاری
blend_lighten (اعشاری sc, اعشاری dc) { بازگشت hb_max (sc, dc); }
ایستا درونخطی اعشاری
blend_color_dodge (اعشاری sc, اعشاری dc)
{
  اگر (dc <= 0.f) بازگشت 0.f;
  اگر (sc >= 1.f) بازگشت 1.f;
  بازگشت hb_min (1.f, dc / (1.f - sc));
}
ایستا درونخطی اعشاری
blend_color_burn (اعشاری sc, اعشاری dc)
{
  اگر (dc >= 1.f) بازگشت 1.f;
  اگر (sc <= 0.f) بازگشت 0.f;
  بازگشت 1.f - hb_min (1.f, (1.f - dc) / sc);
}
ایستا درونخطی اعشاری
blend_hard_light (اعشاری sc, اعشاری dc)
{ بازگشت sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
ایستا درونخطی اعشاری
blend_soft_light (اعشاری sc, اعشاری dc)
{
  اگر (sc <= 0.5f)
    بازگشت dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  اعشاری d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  بازگشت dc + (2.f * sc - 1.f) * (d - dc);
}
ایستا درونخطی اعشاری
blend_difference (اعشاری sc, اعشاری dc) { بازگشت fabsf (sc - dc); }
ایستا درونخطی اعشاری
blend_exclusion (اعشاری sc, اعشاری dc) { بازگشت sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
ایستا درونخطی uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       اعشاری (*blend_fn)(اعشاری, اعشاری))
{
  اعشاری sr, sg, sb, sa;
  اعشاری dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

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

  بازگشت pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
ایستا درونخطی اعشاری
hsl_luminosity (اعشاری r, اعشاری g, اعشاری b)
{ بازگشت 0.299f * r + 0.587f * g + 0.114f * b; }

ایستا درونخطی اعشاری
hsl_saturation (اعشاری r, اعشاری g, اعشاری b)
{ بازگشت hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

ایستا درونخطی تهی
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

ایستا درونخطی تهی
hsl_set_luminosity (اعشاری &r, اعشاری &g, اعشاری &b, اعشاری l)
{
  اعشاری d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

ایستا درونخطی تهی
hsl_set_saturation_inner (اعشاری &mn, اعشاری &mid, اعشاری &mx, اعشاری s)
{
  اگر (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  وگرنه
    mid = mx = 0.f;
  mn = 0.f;
}

ایستا درونخطی تهی
hsl_set_saturation (اعشاری &r, اعشاری &g, اعشاری &b, اعشاری s)
{
  اگر (r <= g)
  {
    اگر (g <= b)      hsl_set_saturation_inner (r, g, b, s);
    وگرنه اگر (r <= b) hsl_set_saturation_inner (r, b, g, s);
    وگرنه             hsl_set_saturation_inner (b, r, g, s);
  }
  وگرنه
  {
    اگر (r <= b)      hsl_set_saturation_inner (g, r, b, s);
    وگرنه اگر (g <= b) hsl_set_saturation_inner (g, b, r, s);
    وگرنه             hsl_set_saturation_inner (b, g, r, s);
  }
}

ایستا درونخطی uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  اعشاری sr, sg, sb, sa;
  اعشاری dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  اعشاری usr = sa > 0.f ? sr / sa : 0.f;
  اعشاری usg = sa > 0.f ? sg / sa : 0.f;
  اعشاری usb = sa > 0.f ? sb / sa : 0.f;
  اعشاری udr = da > 0.f ? dr / da : 0.f;
  اعشاری udg = da > 0.f ? dg / da : 0.f;
  اعشاری udb = da > 0.f ? db / da : 0.f;

  اعشاری br = udr, bg = udg, bb = udb;

  اگر (mode == HB_PAINT_COMPOSITE_MODE_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  وگرنه اگر (mode == HB_PAINT_COMPOSITE_MODE_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  وگرنه اگر (mode == HB_PAINT_COMPOSITE_MODE_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  وگرنه /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  اعشاری ra = sa + da - sa * da;
  اعشاری rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  اعشاری rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  اعشاری rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  بازگشت pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
ایستا درونخطی uint32_t
composite_pixel (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  انتخاب (mode)
  {
  حالت HB_PAINT_COMPOSITE_MODE_CLEAR:
    بازگشت 0;
  حالت HB_PAINT_COMPOSITE_MODE_SRC:
    بازگشت src;
  حالت HB_PAINT_COMPOSITE_MODE_DEST:
    بازگشت dst;
  حالت HB_PAINT_COMPOSITE_MODE_SRC_OVER:
    بازگشت hb_raster_src_over (src, dst);
  حالت HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    بازگشت hb_raster_src_over (dst, src);
  حالت HB_PAINT_COMPOSITE_MODE_SRC_IN:
    بازگشت hb_raster_alpha_mul (src, da);
  حالت HB_PAINT_COMPOSITE_MODE_DEST_IN:
    بازگشت hb_raster_alpha_mul (dst, sa);
  حالت HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    بازگشت hb_raster_alpha_mul (src, 255 - da);
  حالت HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    بازگشت hb_raster_alpha_mul (dst, 255 - sa);
  حالت HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    بازگشت hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  حالت HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    بازگشت hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  حالت HB_PAINT_COMPOSITE_MODE_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    بازگشت hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  حالت HB_PAINT_COMPOSITE_MODE_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (src >> 24) + (dst >> 24));
    بازگشت hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  حالت HB_PAINT_COMPOSITE_MODE_MULTIPLY:  بازگشت apply_separable_blend (src, dst, blend_multiply);
  حالت HB_PAINT_COMPOSITE_MODE_SCREEN:    بازگشت apply_separable_blend (src, dst, blend_screen);
  حالت HB_PAINT_COMPOSITE_MODE_OVERLAY:   بازگشت apply_separable_blend (src, dst, blend_overlay);
  حالت HB_PAINT_COMPOSITE_MODE_DARKEN:    بازگشت apply_separable_blend (src, dst, blend_darken);
  حالت HB_PAINT_COMPOSITE_MODE_LIGHTEN:   بازگشت apply_separable_blend (src, dst, blend_lighten);
  حالت HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: بازگشت apply_separable_blend (src, dst, blend_color_dodge);
  حالت HB_PAINT_COMPOSITE_MODE_COLOR_BURN:  بازگشت apply_separable_blend (src, dst, blend_color_burn);
  حالت HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:  بازگشت apply_separable_blend (src, dst, blend_hard_light);
  حالت HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:  بازگشت apply_separable_blend (src, dst, blend_soft_light);
  حالت HB_PAINT_COMPOSITE_MODE_DIFFERENCE:  بازگشت apply_separable_blend (src, dst, blend_difference);
  حالت HB_PAINT_COMPOSITE_MODE_EXCLUSION:   بازگشت apply_separable_blend (src, dst, blend_exclusion);

  حالت HB_PAINT_COMPOSITE_MODE_HSL_HUE:
  حالت HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
  حالت HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
  حالت HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
    بازگشت apply_hsl_blend (src, dst, mode);

  پیشفرض:
    بازگشت hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

unsigned
hb_raster_image_t::bytes_per_pixel (hb_raster_format_t format)
{
  بازگشت format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

منطقی
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  اگر (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  unsigned bpp = bytes_per_pixel (format);
  اگر (extents.width > UINT_MAX / bpp)
    بازگشت نادرست;

  unsigned min_stride = extents.width * bpp;
  اگر (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  اگر (extents.height && extents.stride > (size_t) -1 / extents.height)
    بازگشت نادرست;

  size_t buf_size = (size_t) extents.stride * extents.height;
  اگر (buf_size > HB_RASTER_MAX_BUFFER_SIZE)
    بازگشت نادرست;
  اگر (unlikely (!buffer.resize_dirty (buf_size)))
    بازگشت نادرست;

  این->format = format;
  این->extents = extents;
  بازگشت درست;
}

منطقی
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#ifndef HAVE_PNG
  بازگشت نادرست;
#وگرنه
  اگر (!blob)
    بازگشت نادرست;

  unsigned blob_len = 0;
  ثابت uint8_t *blob_data = (ثابت uint8_t *) hb_blob_get_data (blob, &blob_len);
  اگر (!blob_data || !blob_len)
    بازگشت نادرست;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, nullptr,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  اگر (!png)
    بازگشت نادرست;

  png_infop info = png_create_info_struct (png);
  اگر (!info)
  {
    png_destroy_read_struct (&png, nullptr, nullptr);
    بازگشت نادرست;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, اندازه (*reader));
  اگر (!reader)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    بازگشت نادرست;
  }

  reader->data = blob_data;
  reader->size = (size_t) blob_len;
  png_set_read_fn (png, reader, hb_raster_png_read_blob);
  اگر (setjmp (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    بازگشت نادرست;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  صحیح bit_depth = 0, color_type = 0;
  صحیح interlace_type = 0, compression_type = 0, filter_method = 0;
  png_get_IHDR (png, info, &w, &h, &bit_depth, &color_type,
		&interlace_type, &compression_type, &filter_method);

  اگر (!w || !h || w > (png_uint_32) INT_MAX || h > (png_uint_32) INT_MAX)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    بازگشت نادرست;
  }

  منطقی has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  اگر (bit_depth == 16)
    png_set_strip_16 (png);
  اگر (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png);
  اگر (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8 (png);
  اگر (has_trns)
    png_set_tRNS_to_alpha (png);
  اگر (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb (png);
  اگر (!(color_type & PNG_COLOR_MASK_ALPHA) && !has_trns)
    png_set_add_alpha (png, 0xff, PNG_FILLER_AFTER);
  اگر (interlace_type != PNG_INTERLACE_NONE)
    png_set_interlace_handling (png);

  png_read_update_info (png, info);

  اگر (png_get_bit_depth (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    بازگشت نادرست;
  }

  png_size_t rowbytes = png_get_rowbytes (png, info);
  اگر (rowbytes < (png_size_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    بازگشت نادرست;
  }

  size_t rgba_size = (size_t) rowbytes * (size_t) h;
  اگر ((h && rgba_size / (size_t) h != (size_t) rowbytes) ||
      rgba_size > HB_RASTER_MAX_BUFFER_SIZE)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    بازگشت نادرست;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_size);
  اگر (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    بازگشت نادرست;
  }

  size_t rows_size = (size_t) h * اندازه (png_bytep);
  اگر (h && rows_size / (size_t) h != اندازه (png_bytep))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    بازگشت نادرست;
  }

  reader->rows = (png_bytep *) hb_malloc (rows_size);
  اگر (!reader->rows)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    بازگشت نادرست;
  }

  برای (unsigned y = 0; y < (unsigned) h; y++)
    reader->rows[y] = (png_bytep) (reader->rgba + (size_t) y * (size_t) rowbytes);

  png_read_image (png, reader->rows);
  png_read_end (png, nullptr);
  png_destroy_read_struct (&png, &info, nullptr);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (unsigned) w, (unsigned) h, 0};
  اگر (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    بازگشت نادرست;
  }

  برای (unsigned y = 0; y < (unsigned) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.buffer.arrayZ + (size_t) ((unsigned) h - 1 - y) * decoded.extents.stride);
    ثابت uint8_t *src = reader->rgba + (size_t) y * (size_t) rowbytes;
    برای (unsigned x = 0; x < (unsigned) w; x++)
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

  hb_swap (buffer, decoded.buffer);
  hb_swap (این->extents, decoded.extents);
  hb_swap (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  بازگشت درست;
#endif
}

hb_blob_t *
hb_raster_image_t::serialize_to_png_or_fail () ثابت
{
#ifndef HAVE_PNG
  بازگشت nullptr;
#وگرنه
  اگر (format != HB_RASTER_FORMAT_BGRA32 || !extents.width || !extents.height)
    بازگشت nullptr;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, nullptr,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  اگر (!png)
    بازگشت nullptr;

  png_infop info = png_create_info_struct (png);
  اگر (!info)
  {
    png_destroy_write_struct (&png, nullptr);
    بازگشت nullptr;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, اندازه (*writer));
  اگر (!writer)
  {
    png_destroy_write_struct (&png, &info);
    بازگشت nullptr;
  }

  png_set_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  اگر (setjmp (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    بازگشت nullptr;
  }

  png_set_IHDR (png, info,
		extents.width, extents.height,
		8, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  size_t rowbytes = (size_t) extents.width * 4u;
  size_t rgba_size = rowbytes * (size_t) extents.height;
  اگر (extents.height && rgba_size / (size_t) extents.height != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    بازگشت nullptr;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_size);
  اگر (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    بازگشت nullptr;
  }

  size_t rows_size = (size_t) extents.height * اندازه (png_bytep);
  اگر (extents.height && rows_size / (size_t) extents.height != اندازه (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    بازگشت nullptr;
  }

  writer->rows = (png_bytep *) hb_malloc (rows_size);
  اگر (!writer->rows)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    بازگشت nullptr;
  }

  برای (unsigned y = 0; y < extents.height; y++)
  {
    uint8_t *dst = writer->rgba + (size_t) y * rowbytes;
    ثابت uint8_t *src = buffer.arrayZ + (size_t) (extents.height - 1 - y) * extents.stride;

    برای (unsigned x = 0; x < extents.width; x++)
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
	dst[4 * x + 0] = (uint8_t) hb_min (255u, ((unsigned) r * 255u + a / 2u) / a);
	dst[4 * x + 1] = (uint8_t) hb_min (255u, ((unsigned) g * 255u + a / 2u) / a);
	dst[4 * x + 2] = (uint8_t) hb_min (255u, ((unsigned) b * 255u + a / 2u) / a);
      }
      وگرنه
	dst[4 * x + 0] = dst[4 * x + 1] = dst[4 * x + 2] = 0;
    }

    writer->rows[y] = (png_bytep) dst;
  }

  png_write_image (png, writer->rows);
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);

  اگر (writer->length > (size_t) (unsigned) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    بازگشت nullptr;
  }

  unsigned length = (unsigned) writer->length;
  نویسه *data = writer->data;
  writer->data = nullptr;
  hb_raster_png_write_blob_fini (writer);
  اگر (!data && length)
    بازگشت nullptr;

  hb_blob_t *blob = hb_blob_create_or_fail (data, length,
					    HB_MEMORY_MODE_WRITABLE,
					    data, hb_free);
  اگر (!blob)
    hb_free (data);
  بازگشت blob;
#endif
}

تهی
hb_raster_image_t::clear ()
{
  size_t buf_size = (size_t) extents.stride * extents.height;
  hb_memset (buffer.arrayZ, 0, buf_size);
}

ثابت uint8_t *
hb_raster_image_t::get_buffer () ثابت
{
  بازگشت buffer.arrayZ;
}

تهی
hb_raster_image_t::composite_from (ثابت hb_raster_image_t *src,
				   hb_paint_composite_mode_t mode)
{
  unsigned w = extents.width;
  unsigned h = extents.height;
  unsigned stride = extents.stride;

  برای (unsigned y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (buffer.arrayZ + y * stride);
    ثابت hb_packed_t<uint32_t> *sp = (ثابت hb_packed_t<uint32_t> *) (src->buffer.arrayZ + y * stride);
    برای (unsigned x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (composite_pixel ((uint32_t) sp[x], (uint32_t) dp[x], mode));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
تهی
hb_raster_image_composite (hb_raster_image_t *dst,
			   ثابت hb_raster_image_t *src,
			   hb_paint_composite_mode_t mode)
{
  dst->composite_from (src, mode);
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
hb_raster_image_create_or_fail (تهی)
{
  بازگشت hb_object_create<hb_raster_image_t> ();
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
  بازگشت hb_object_reference (image);
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
تهی
hb_raster_image_destroy (hb_raster_image_t *image)
{
  اگر (!hb_object_destroy (image)) بازگشت;
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
hb_raster_image_set_user_data (hb_raster_image_t  *image,
			       hb_user_data_کلید_t *key,
			       تهی               *data,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  بازگشت hb_object_set_user_data (image, key, data, destroy, replace);
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
تهی *
hb_raster_image_get_user_data (ثابت hb_raster_image_t  *image,
			       hb_user_data_کلید_t *key)
{
  بازگشت hb_object_get_user_data (image, key);
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
    image->buffer.resize_exact (0);
    بازگشت درست;
  }
  بازگشت image->configure (format, *extents);
}

/**
 * hb_raster_image_clear:
 * @image: a raster image
 *
 * Clears @image pixels to zero while keeping current extents and format.
 *
 * Since: 13.0.0
 **/
تهی
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
hb_raster_image_get_buffer (ثابت hb_raster_image_t *image)
{
  بازگشت image->get_buffer ();
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
تهی
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
  بازگشت image->format;
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
  بازگشت image->deserialize_from_png (png);
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
hb_raster_image_serialize_to_png_or_fail (ثابت hb_raster_image_t *image)
{
  بازگشت image->serialize_to_png_or_fail ();
}
