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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-raster-image.hh"

#अन्तर्भाव <math.h>

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
#अन्तर्भाव <png.h>
#endif

#ifdef HAVE_PNG
संरचना hb_raster_png_read_blob_t
{
  अचर uint8_t *data = nullptr;
  माप_प्रकार size = 0;
  माप_प्रकार offset = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

स्थिर शून्य
hb_raster_png_error (png_structp png,
		     png_const_charp सन्देश HB_UNUSED)
{
#ifdef PNG_SETJMP_SUPPORTED
  दीर्घ_लाँघ (png_jmpbuf (png), 1);
#endif
}

स्थिर शून्य
hb_raster_png_warning (png_structp png HB_UNUSED,
		       png_const_charp सन्देश HB_UNUSED)
{}

स्थिर शून्य
hb_raster_png_read_blob (png_structp png, png_bytep out, png_size_t length)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  यदि (!r || !r->data || length > r->size - r->offset)
    png_error (png, "read error");

  hb_memcpy (out, r->data + r->offset, length);
  r->offset += length;
}

स्थिर शून्य
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  यदि (!r)
    फर्काऊ;

  hb_free (r->rgba);
  hb_free (r->rows);
  hb_free (r);
}

संरचना hb_raster_png_write_blob_t
{
  वर्ण *data = nullptr;
  माप_प्रकार length = 0;
  माप_प्रकार allocated = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

स्थिर शून्य
hb_raster_png_write_blob (png_structp png, png_bytep in, png_size_t length)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  यदि (!w)
    png_error (png, "write error");

  माप_प्रकार old_length = w->length;
  यदि ((माप_प्रकार) length > (माप_प्रकार) -1 - old_length)
    png_error (png, "write error");

  माप_प्रकार new_length = old_length + length;
  यदि (new_length > w->allocated)
  {
    माप_प्रकार new_allocated = w->allocated ? w->allocated : 4096;
    जबसम्म (new_allocated < new_length)
    {
      माप_प्रकार next = new_allocated * 2;
      यदि (next <= new_allocated)
      {
	new_allocated = new_length;
	रोक;
      }
      new_allocated = next;
    }

    वर्ण *data = (वर्ण *) hb_realloc (w->data, new_allocated);
    यदि (!data)
      png_error (png, "write error");
    w->data = data;
    w->allocated = new_allocated;
  }

  hb_memcpy (w->data + old_length, in, length);
  w->length = new_length;
}

स्थिर शून्य
hb_raster_png_flush_blob (png_structp png HB_UNUSED)
{}

स्थिर शून्य
hb_raster_png_write_blob_fini (hb_raster_png_write_blob_t *w)
{
  यदि (!w)
    फर्काऊ;

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
स्थिर अन्तःस्थ शून्य
unpack_to_float (uint32_t px, भिन्न &r, भिन्न &g, भिन्न &b, भिन्न &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
स्थिर अन्तःस्थ uint32_t
pack_from_float (भिन्न r, भिन्न g, भिन्न b, भिन्न a)
{
  फर्काऊ hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
स्थिर अन्तःस्थ भिन्न
blend_multiply (भिन्न sc, भिन्न dc) { फर्काऊ sc * dc; }
स्थिर अन्तःस्थ भिन्न
blend_screen (भिन्न sc, भिन्न dc) { फर्काऊ sc + dc - sc * dc; }
स्थिर अन्तःस्थ भिन्न
blend_overlay (भिन्न sc, भिन्न dc)
{ फर्काऊ dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
स्थिर अन्तःस्थ भिन्न
blend_darken (भिन्न sc, भिन्न dc) { फर्काऊ hb_min (sc, dc); }
स्थिर अन्तःस्थ भिन्न
blend_lighten (भिन्न sc, भिन्न dc) { फर्काऊ hb_max (sc, dc); }
स्थिर अन्तःस्थ भिन्न
blend_color_dodge (भिन्न sc, भिन्न dc)
{
  यदि (dc <= 0.f) फर्काऊ 0.f;
  यदि (sc >= 1.f) फर्काऊ 1.f;
  फर्काऊ hb_min (1.f, dc / (1.f - sc));
}
स्थिर अन्तःस्थ भिन्न
blend_color_burn (भिन्न sc, भिन्न dc)
{
  यदि (dc >= 1.f) फर्काऊ 1.f;
  यदि (sc <= 0.f) फर्काऊ 0.f;
  फर्काऊ 1.f - hb_min (1.f, (1.f - dc) / sc);
}
स्थिर अन्तःस्थ भिन्न
blend_hard_light (भिन्न sc, भिन्न dc)
{ फर्काऊ sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
स्थिर अन्तःस्थ भिन्न
blend_soft_light (भिन्न sc, भिन्न dc)
{
  यदि (sc <= 0.5f)
    फर्काऊ dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  भिन्न d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  फर्काऊ dc + (2.f * sc - 1.f) * (d - dc);
}
स्थिर अन्तःस्थ भिन्न
blend_difference (भिन्न sc, भिन्न dc) { फर्काऊ fabsf (sc - dc); }
स्थिर अन्तःस्थ भिन्न
blend_exclusion (भिन्न sc, भिन्न dc) { फर्काऊ sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
स्थिर अन्तःस्थ uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       भिन्न (*blend_fn)(भिन्न, भिन्न))
{
  भिन्न sr, sg, sb, sa;
  भिन्न dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  भिन्न usr = sa > 0.f ? sr / sa : 0.f;
  भिन्न usg = sa > 0.f ? sg / sa : 0.f;
  भिन्न usb = sa > 0.f ? sb / sa : 0.f;
  भिन्न udr = da > 0.f ? dr / da : 0.f;
  भिन्न udg = da > 0.f ? dg / da : 0.f;
  भिन्न udb = da > 0.f ? db / da : 0.f;

  भिन्न br = blend_fn (usr, udr);
  भिन्न bg = blend_fn (usg, udg);
  भिन्न bb = blend_fn (usb, udb);

  भिन्न ra = sa + da - sa * da;
  भिन्न rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  भिन्न rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  भिन्न rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  फर्काऊ pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
स्थिर अन्तःस्थ भिन्न
hsl_luminosity (भिन्न r, भिन्न g, भिन्न b)
{ फर्काऊ 0.299f * r + 0.587f * g + 0.114f * b; }

स्थिर अन्तःस्थ भिन्न
hsl_saturation (भिन्न r, भिन्न g, भिन्न b)
{ फर्काऊ hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

स्थिर अन्तःस्थ शून्य
hsl_clip_color (भिन्न &r, भिन्न &g, भिन्न &b)
{
  भिन्न l = hsl_luminosity (r, g, b);
  भिन्न mn = hb_min (hb_min (r, g), b);
  भिन्न mx = hb_max (hb_max (r, g), b);
  यदि (mn < 0.f)
  {
    भिन्न d = l - mn;
    यदि (d > 0.f) { r = l + (r - l) * l / d; g = l + (g - l) * l / d; b = l + (b - l) * l / d; }
  }
  यदि (mx > 1.f)
  {
    भिन्न d = mx - l;
    यदि (d > 0.f) { r = l + (r - l) * (1.f - l) / d; g = l + (g - l) * (1.f - l) / d; b = l + (b - l) * (1.f - l) / d; }
  }
}

स्थिर अन्तःस्थ शून्य
hsl_set_luminosity (भिन्न &r, भिन्न &g, भिन्न &b, भिन्न l)
{
  भिन्न d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

स्थिर अन्तःस्थ शून्य
hsl_set_saturation_inner (भिन्न &mn, भिन्न &mid, भिन्न &mx, भिन्न s)
{
  यदि (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  अन्यथा
    mid = mx = 0.f;
  mn = 0.f;
}

स्थिर अन्तःस्थ शून्य
hsl_set_saturation (भिन्न &r, भिन्न &g, भिन्न &b, भिन्न s)
{
  यदि (r <= g)
  {
    यदि (g <= b)      hsl_set_saturation_inner (r, g, b, s);
    अन्यथा यदि (r <= b) hsl_set_saturation_inner (r, b, g, s);
    अन्यथा             hsl_set_saturation_inner (b, r, g, s);
  }
  अन्यथा
  {
    यदि (r <= b)      hsl_set_saturation_inner (g, r, b, s);
    अन्यथा यदि (g <= b) hsl_set_saturation_inner (g, b, r, s);
    अन्यथा             hsl_set_saturation_inner (b, g, r, s);
  }
}

स्थिर अन्तःस्थ uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  भिन्न sr, sg, sb, sa;
  भिन्न dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  भिन्न usr = sa > 0.f ? sr / sa : 0.f;
  भिन्न usg = sa > 0.f ? sg / sa : 0.f;
  भिन्न usb = sa > 0.f ? sb / sa : 0.f;
  भिन्न udr = da > 0.f ? dr / da : 0.f;
  भिन्न udg = da > 0.f ? dg / da : 0.f;
  भिन्न udb = da > 0.f ? db / da : 0.f;

  भिन्न br = udr, bg = udg, bb = udb;

  यदि (mode == HB_PAINT_COMPOSITE_MODE_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  अन्यथा यदि (mode == HB_PAINT_COMPOSITE_MODE_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  अन्यथा यदि (mode == HB_PAINT_COMPOSITE_MODE_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  अन्यथा /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  भिन्न ra = sa + da - sa * da;
  भिन्न rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  भिन्न rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  भिन्न rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  फर्काऊ pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
स्थिर अन्तःस्थ uint32_t
composite_pixel (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  विकल्प (mode)
  {
  पक्ष HB_PAINT_COMPOSITE_MODE_CLEAR:
    फर्काऊ 0;
  पक्ष HB_PAINT_COMPOSITE_MODE_SRC:
    फर्काऊ src;
  पक्ष HB_PAINT_COMPOSITE_MODE_DEST:
    फर्काऊ dst;
  पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OVER:
    फर्काऊ hb_raster_src_over (src, dst);
  पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    फर्काऊ hb_raster_src_over (dst, src);
  पक्ष HB_PAINT_COMPOSITE_MODE_SRC_IN:
    फर्काऊ hb_raster_alpha_mul (src, da);
  पक्ष HB_PAINT_COMPOSITE_MODE_DEST_IN:
    फर्काऊ hb_raster_alpha_mul (dst, sa);
  पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    फर्काऊ hb_raster_alpha_mul (src, 255 - da);
  पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    फर्काऊ hb_raster_alpha_mul (dst, 255 - sa);
  पक्ष HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (a >> 24) + (b >> 24));
    फर्काऊ hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  पक्ष HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (a >> 24) + (b >> 24));
    फर्काऊ hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  पक्ष HB_PAINT_COMPOSITE_MODE_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (a >> 24) + (b >> 24));
    फर्काऊ hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  पक्ष HB_PAINT_COMPOSITE_MODE_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (src >> 24) + (dst >> 24));
    फर्काऊ hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  पक्ष HB_PAINT_COMPOSITE_MODE_MULTIPLY:  फर्काऊ apply_separable_blend (src, dst, blend_multiply);
  पक्ष HB_PAINT_COMPOSITE_MODE_SCREEN:    फर्काऊ apply_separable_blend (src, dst, blend_screen);
  पक्ष HB_PAINT_COMPOSITE_MODE_OVERLAY:   फर्काऊ apply_separable_blend (src, dst, blend_overlay);
  पक्ष HB_PAINT_COMPOSITE_MODE_DARKEN:    फर्काऊ apply_separable_blend (src, dst, blend_darken);
  पक्ष HB_PAINT_COMPOSITE_MODE_LIGHTEN:   फर्काऊ apply_separable_blend (src, dst, blend_lighten);
  पक्ष HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: फर्काऊ apply_separable_blend (src, dst, blend_color_dodge);
  पक्ष HB_PAINT_COMPOSITE_MODE_COLOR_BURN:  फर्काऊ apply_separable_blend (src, dst, blend_color_burn);
  पक्ष HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:  फर्काऊ apply_separable_blend (src, dst, blend_hard_light);
  पक्ष HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:  फर्काऊ apply_separable_blend (src, dst, blend_soft_light);
  पक्ष HB_PAINT_COMPOSITE_MODE_DIFFERENCE:  फर्काऊ apply_separable_blend (src, dst, blend_difference);
  पक्ष HB_PAINT_COMPOSITE_MODE_EXCLUSION:   फर्काऊ apply_separable_blend (src, dst, blend_exclusion);

  पक्ष HB_PAINT_COMPOSITE_MODE_HSL_HUE:
  पक्ष HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
  पक्ष HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
  पक्ष HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
    फर्काऊ apply_hsl_blend (src, dst, mode);

  स्वभाव:
    फर्काऊ hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

अचिन्हित
hb_raster_image_t::bytes_per_pixel (hb_raster_format_t format)
{
  फर्काऊ format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

तार्किक
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  यदि (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  अचिन्हित bpp = bytes_per_pixel (format);
  यदि (extents.width > अच_पूर्णांक_उच्च / bpp)
    फर्काऊ असत्य;

  अचिन्हित min_stride = extents.width * bpp;
  यदि (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  यदि (extents.height && extents.stride > (माप_प्रकार) -1 / extents.height)
    फर्काऊ असत्य;

  माप_प्रकार buf_size = (माप_प्रकार) extents.stride * extents.height;
  यदि (buf_size > HB_RASTER_MAX_BUFFER_SIZE)
    फर्काऊ असत्य;
  यदि (unlikely (!buffer.resize_dirty (buf_size)))
    फर्काऊ असत्य;

  एतत्->format = format;
  एतत्->extents = extents;
  फर्काऊ सत्य;
}

तार्किक
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#ifndef HAVE_PNG
  फर्काऊ असत्य;
#अन्यथा
  यदि (!blob)
    फर्काऊ असत्य;

  अचिन्हित blob_len = 0;
  अचर uint8_t *blob_data = (अचर uint8_t *) hb_blob_get_data (blob, &blob_len);
  यदि (!blob_data || !blob_len)
    फर्काऊ असत्य;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, nullptr,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  यदि (!png)
    फर्काऊ असत्य;

  png_infop info = png_create_info_struct (png);
  यदि (!info)
  {
    png_destroy_read_struct (&png, nullptr, nullptr);
    फर्काऊ असत्य;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, परिमाण (*reader));
  यदि (!reader)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    फर्काऊ असत्य;
  }

  reader->data = blob_data;
  reader->size = (माप_प्रकार) blob_len;
  png_set_read_fn (png, reader, hb_raster_png_read_blob);
  यदि (बनाओ_लाँघ (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    फर्काऊ असत्य;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  पूर्णाङ्क bit_depth = 0, color_type = 0;
  पूर्णाङ्क interlace_type = 0, compression_type = 0, filter_method = 0;
  png_get_IHDR (png, info, &w, &h, &bit_depth, &color_type,
		&interlace_type, &compression_type, &filter_method);

  यदि (!w || !h || w > (png_uint_32) पूर्णांक_उच्च || h > (png_uint_32) पूर्णांक_उच्च)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    फर्काऊ असत्य;
  }

  तार्किक has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  यदि (bit_depth == 16)
    png_set_strip_16 (png);
  यदि (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png);
  यदि (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8 (png);
  यदि (has_trns)
    png_set_tRNS_to_alpha (png);
  यदि (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb (png);
  यदि (!(color_type & PNG_COLOR_MASK_ALPHA) && !has_trns)
    png_set_add_alpha (png, 0xff, PNG_FILLER_AFTER);
  यदि (interlace_type != PNG_INTERLACE_NONE)
    png_set_interlace_handling (png);

  png_read_update_info (png, info);

  यदि (png_get_bit_depth (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    फर्काऊ असत्य;
  }

  png_size_t rowbytes = png_get_rowbytes (png, info);
  यदि (rowbytes < (png_size_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    फर्काऊ असत्य;
  }

  माप_प्रकार rgba_size = (माप_प्रकार) rowbytes * (माप_प्रकार) h;
  यदि ((h && rgba_size / (माप_प्रकार) h != (माप_प्रकार) rowbytes) ||
      rgba_size > HB_RASTER_MAX_BUFFER_SIZE)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    फर्काऊ असत्य;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_size);
  यदि (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    फर्काऊ असत्य;
  }

  माप_प्रकार rows_size = (माप_प्रकार) h * परिमाण (png_bytep);
  यदि (h && rows_size / (माप_प्रकार) h != परिमाण (png_bytep))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    फर्काऊ असत्य;
  }

  reader->rows = (png_bytep *) hb_malloc (rows_size);
  यदि (!reader->rows)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    फर्काऊ असत्य;
  }

  लागि (अचिन्हित y = 0; y < (अचिन्हित) h; y++)
    reader->rows[y] = (png_bytep) (reader->rgba + (माप_प्रकार) y * (माप_प्रकार) rowbytes);

  png_read_image (png, reader->rows);
  png_read_end (png, nullptr);
  png_destroy_read_struct (&png, &info, nullptr);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (अचिन्हित) w, (अचिन्हित) h, 0};
  यदि (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    फर्काऊ असत्य;
  }

  लागि (अचिन्हित y = 0; y < (अचिन्हित) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.buffer.arrayZ + (माप_प्रकार) ((अचिन्हित) h - 1 - y) * decoded.extents.stride);
    अचर uint8_t *src = reader->rgba + (माप_प्रकार) y * (माप_प्रकार) rowbytes;
    लागि (अचिन्हित x = 0; x < (अचिन्हित) w; x++)
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
  hb_swap (एतत्->extents, decoded.extents);
  hb_swap (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  फर्काऊ सत्य;
#endif
}

hb_blob_t *
hb_raster_image_t::serialize_to_png_or_fail () अचर
{
#ifndef HAVE_PNG
  फर्काऊ nullptr;
#अन्यथा
  यदि (format != HB_RASTER_FORMAT_BGRA32 || !extents.width || !extents.height)
    फर्काऊ nullptr;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, nullptr,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  यदि (!png)
    फर्काऊ nullptr;

  png_infop info = png_create_info_struct (png);
  यदि (!info)
  {
    png_destroy_write_struct (&png, nullptr);
    फर्काऊ nullptr;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, परिमाण (*writer));
  यदि (!writer)
  {
    png_destroy_write_struct (&png, &info);
    फर्काऊ nullptr;
  }

  png_set_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  यदि (बनाओ_लाँघ (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    फर्काऊ nullptr;
  }

  png_set_IHDR (png, info,
		extents.width, extents.height,
		8, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  माप_प्रकार rowbytes = (माप_प्रकार) extents.width * 4u;
  माप_प्रकार rgba_size = rowbytes * (माप_प्रकार) extents.height;
  यदि (extents.height && rgba_size / (माप_प्रकार) extents.height != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    फर्काऊ nullptr;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_size);
  यदि (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    फर्काऊ nullptr;
  }

  माप_प्रकार rows_size = (माप_प्रकार) extents.height * परिमाण (png_bytep);
  यदि (extents.height && rows_size / (माप_प्रकार) extents.height != परिमाण (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    फर्काऊ nullptr;
  }

  writer->rows = (png_bytep *) hb_malloc (rows_size);
  यदि (!writer->rows)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    फर्काऊ nullptr;
  }

  लागि (अचिन्हित y = 0; y < extents.height; y++)
  {
    uint8_t *dst = writer->rgba + (माप_प्रकार) y * rowbytes;
    अचर uint8_t *src = buffer.arrayZ + (माप_प्रकार) (extents.height - 1 - y) * extents.stride;

    लागि (अचिन्हित x = 0; x < extents.width; x++)
    {
      uint32_t px;
      hb_memcpy (&px, src + x * 4, 4);
      uint8_t b = (uint8_t) (px & 0xFF);
      uint8_t g = (uint8_t) ((px >> 8) & 0xFF);
      uint8_t r = (uint8_t) ((px >> 16) & 0xFF);
      uint8_t a = (uint8_t) (px >> 24);

      dst[4 * x + 3] = a;
      यदि (a)
      {
	dst[4 * x + 0] = (uint8_t) hb_min (255u, ((अचिन्हित) r * 255u + a / 2u) / a);
	dst[4 * x + 1] = (uint8_t) hb_min (255u, ((अचिन्हित) g * 255u + a / 2u) / a);
	dst[4 * x + 2] = (uint8_t) hb_min (255u, ((अचिन्हित) b * 255u + a / 2u) / a);
      }
      अन्यथा
	dst[4 * x + 0] = dst[4 * x + 1] = dst[4 * x + 2] = 0;
    }

    writer->rows[y] = (png_bytep) dst;
  }

  png_write_image (png, writer->rows);
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);

  यदि (writer->length > (माप_प्रकार) (अचिन्हित) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    फर्काऊ nullptr;
  }

  अचिन्हित length = (अचिन्हित) writer->length;
  वर्ण *data = writer->data;
  writer->data = nullptr;
  hb_raster_png_write_blob_fini (writer);
  यदि (!data && length)
    फर्काऊ nullptr;

  hb_blob_t *blob = hb_blob_create_or_fail (data, length,
					    HB_MEMORY_MODE_WRITABLE,
					    data, hb_free);
  यदि (!blob)
    hb_free (data);
  फर्काऊ blob;
#endif
}

शून्य
hb_raster_image_t::clear ()
{
  माप_प्रकार buf_size = (माप_प्रकार) extents.stride * extents.height;
  hb_memset (buffer.arrayZ, 0, buf_size);
}

अचर uint8_t *
hb_raster_image_t::get_buffer () अचर
{
  फर्काऊ buffer.arrayZ;
}

शून्य
hb_raster_image_t::composite_from (अचर hb_raster_image_t *src,
				   hb_paint_composite_mode_t mode)
{
  अचिन्हित w = extents.width;
  अचिन्हित h = extents.height;
  अचिन्हित stride = extents.stride;

  लागि (अचिन्हित y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (buffer.arrayZ + y * stride);
    अचर hb_packed_t<uint32_t> *sp = (अचर hb_packed_t<uint32_t> *) (src->buffer.arrayZ + y * stride);
    लागि (अचिन्हित x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (composite_pixel ((uint32_t) sp[x], (uint32_t) dp[x], mode));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
शून्य
hb_raster_image_composite (hb_raster_image_t *dst,
			   अचर hb_raster_image_t *src,
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
hb_raster_image_create_or_fail (शून्य)
{
  फर्काऊ hb_object_create<hb_raster_image_t> ();
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
  फर्काऊ hb_object_reference (image);
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
शून्य
hb_raster_image_destroy (hb_raster_image_t *image)
{
  यदि (!hb_object_destroy (image)) फर्काऊ;
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
			       hb_user_data_साँचो_t *key,
			       शून्य               *data,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  फर्काऊ hb_object_set_user_data (image, key, data, destroy, replace);
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
शून्य *
hb_raster_image_get_user_data (अचर hb_raster_image_t  *image,
			       hb_user_data_साँचो_t *key)
{
  फर्काऊ hb_object_get_user_data (image, key);
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
			   अचर hb_raster_extents_t *extents)
{
  यदि (unlikely (!extents))
  {
    image->extents = {};
    image->buffer.resize_exact (0);
    फर्काऊ सत्य;
  }
  फर्काऊ image->configure (format, *extents);
}

/**
 * hb_raster_image_clear:
 * @image: a raster image
 *
 * Clears @image pixels to zero while keeping current extents and format.
 *
 * Since: 13.0.0
 **/
शून्य
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
अचर uint8_t *
hb_raster_image_get_buffer (अचर hb_raster_image_t *image)
{
  फर्काऊ image->get_buffer ();
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
शून्य
hb_raster_image_get_extents (अचर hb_raster_image_t   *image,
			     hb_raster_extents_t *extents)
{
  यदि (extents)
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
hb_raster_image_get_format (अचर hb_raster_image_t *image)
{
  फर्काऊ image->format;
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
  फर्काऊ image->deserialize_from_png (png);
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
hb_raster_image_serialize_to_png_or_fail (अचर hb_raster_image_t *image)
{
  फर्काऊ image->serialize_to_png_or_fail ();
}
