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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-raster-image.hh"

#அந்தர்பாவ <math.h>

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
#அந்தர்பாவ <png.h>
#endif

#ifdef HAVE_PNG
கட்டமைப்பு hb_raster_png_read_blob_t
{
  மாறிலி uint8_t *data = nullptr;
  size_t size = 0;
  size_t offset = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

நிலையான வெறுமை
hb_raster_png_error (png_structp png,
		     png_const_charp செய்தி HB_UNUSED)
{
#ifdef PNG_SETJMP_SUPPORTED
  longjmp (png_jmpbuf (png), 1);
#endif
}

நிலையான வெறுமை
hb_raster_png_warning (png_structp png HB_UNUSED,
		       png_const_charp செய்தி HB_UNUSED)
{}

நிலையான வெறுமை
hb_raster_png_read_blob (png_structp png, png_bytep out, png_size_t length)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  எனில் (!r || !r->data || length > r->size - r->offset)
    png_error (png, "read error");

  hb_memcpy (out, r->data + r->offset, length);
  r->offset += length;
}

நிலையான வெறுமை
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  எனில் (!r)
    திரும்பு;

  hb_free (r->rgba);
  hb_free (r->rows);
  hb_free (r);
}

கட்டமைப்பு hb_raster_png_write_blob_t
{
  எழுத்து *data = nullptr;
  size_t length = 0;
  size_t allocated = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

நிலையான வெறுமை
hb_raster_png_write_blob (png_structp png, png_bytep in, png_size_t length)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  எனில் (!w)
    png_error (png, "write error");

  size_t old_length = w->length;
  எனில் ((size_t) length > (size_t) -1 - old_length)
    png_error (png, "write error");

  size_t new_length = old_length + length;
  எனில் (new_length > w->allocated)
  {
    size_t new_allocated = w->allocated ? w->allocated : 4096;
    போது (new_allocated < new_length)
    {
      size_t next = new_allocated * 2;
      எனில் (next <= new_allocated)
      {
	new_allocated = new_length;
	நிறுத்து;
      }
      new_allocated = next;
    }

    எழுத்து *data = (எழுத்து *) hb_realloc (w->data, new_allocated);
    எனில் (!data)
      png_error (png, "write error");
    w->data = data;
    w->allocated = new_allocated;
  }

  hb_memcpy (w->data + old_length, in, length);
  w->length = new_length;
}

நிலையான வெறுமை
hb_raster_png_flush_blob (png_structp png HB_UNUSED)
{}

நிலையான வெறுமை
hb_raster_png_write_blob_fini (hb_raster_png_write_blob_t *w)
{
  எனில் (!w)
    திரும்பு;

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
நிலையான உள்வரி வெறுமை
unpack_to_float (uint32_t px, தசம &r, தசம &g, தசம &b, தசம &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
நிலையான உள்வரி uint32_t
pack_from_float (தசம r, தசம g, தசம b, தசம a)
{
  திரும்பு hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
நிலையான உள்வரி தசம
blend_multiply (தசம sc, தசம dc) { திரும்பு sc * dc; }
நிலையான உள்வரி தசம
blend_screen (தசம sc, தசம dc) { திரும்பு sc + dc - sc * dc; }
நிலையான உள்வரி தசம
blend_overlay (தசம sc, தசம dc)
{ திரும்பு dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
நிலையான உள்வரி தசம
blend_darken (தசம sc, தசம dc) { திரும்பு hb_min (sc, dc); }
நிலையான உள்வரி தசம
blend_lighten (தசம sc, தசம dc) { திரும்பு hb_max (sc, dc); }
நிலையான உள்வரி தசம
blend_color_dodge (தசம sc, தசம dc)
{
  எனில் (dc <= 0.f) திரும்பு 0.f;
  எனில் (sc >= 1.f) திரும்பு 1.f;
  திரும்பு hb_min (1.f, dc / (1.f - sc));
}
நிலையான உள்வரி தசம
blend_color_burn (தசம sc, தசம dc)
{
  எனில் (dc >= 1.f) திரும்பு 1.f;
  எனில் (sc <= 0.f) திரும்பு 0.f;
  திரும்பு 1.f - hb_min (1.f, (1.f - dc) / sc);
}
நிலையான உள்வரி தசம
blend_hard_light (தசம sc, தசம dc)
{ திரும்பு sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
நிலையான உள்வரி தசம
blend_soft_light (தசம sc, தசம dc)
{
  எனில் (sc <= 0.5f)
    திரும்பு dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  தசம d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  திரும்பு dc + (2.f * sc - 1.f) * (d - dc);
}
நிலையான உள்வரி தசம
blend_difference (தசம sc, தசம dc) { திரும்பு fabsf (sc - dc); }
நிலையான உள்வரி தசம
blend_exclusion (தசம sc, தசம dc) { திரும்பு sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
நிலையான உள்வரி uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       தசம (*blend_fn)(தசம, தசம))
{
  தசம sr, sg, sb, sa;
  தசம dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  தசம usr = sa > 0.f ? sr / sa : 0.f;
  தசம usg = sa > 0.f ? sg / sa : 0.f;
  தசம usb = sa > 0.f ? sb / sa : 0.f;
  தசம udr = da > 0.f ? dr / da : 0.f;
  தசம udg = da > 0.f ? dg / da : 0.f;
  தசம udb = da > 0.f ? db / da : 0.f;

  தசம br = blend_fn (usr, udr);
  தசம bg = blend_fn (usg, udg);
  தசம bb = blend_fn (usb, udb);

  தசம ra = sa + da - sa * da;
  தசம rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  தசம rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  தசம rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  திரும்பு pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
நிலையான உள்வரி தசம
hsl_luminosity (தசம r, தசம g, தசம b)
{ திரும்பு 0.299f * r + 0.587f * g + 0.114f * b; }

நிலையான உள்வரி தசம
hsl_saturation (தசம r, தசம g, தசம b)
{ திரும்பு hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

நிலையான உள்வரி வெறுமை
hsl_clip_color (தசம &r, தசம &g, தசம &b)
{
  தசம l = hsl_luminosity (r, g, b);
  தசம mn = hb_min (hb_min (r, g), b);
  தசம mx = hb_max (hb_max (r, g), b);
  எனில் (mn < 0.f)
  {
    தசம d = l - mn;
    எனில் (d > 0.f) { r = l + (r - l) * l / d; g = l + (g - l) * l / d; b = l + (b - l) * l / d; }
  }
  எனில் (mx > 1.f)
  {
    தசம d = mx - l;
    எனில் (d > 0.f) { r = l + (r - l) * (1.f - l) / d; g = l + (g - l) * (1.f - l) / d; b = l + (b - l) * (1.f - l) / d; }
  }
}

நிலையான உள்வரி வெறுமை
hsl_set_luminosity (தசம &r, தசம &g, தசம &b, தசம l)
{
  தசம d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

நிலையான உள்வரி வெறுமை
hsl_set_saturation_inner (தசம &mn, தசம &mid, தசம &mx, தசம s)
{
  எனில் (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  இல்லையெனில்
    mid = mx = 0.f;
  mn = 0.f;
}

நிலையான உள்வரி வெறுமை
hsl_set_saturation (தசம &r, தசம &g, தசம &b, தசம s)
{
  எனில் (r <= g)
  {
    எனில் (g <= b)      hsl_set_saturation_inner (r, g, b, s);
    இல்லையெனில் எனில் (r <= b) hsl_set_saturation_inner (r, b, g, s);
    இல்லையெனில்             hsl_set_saturation_inner (b, r, g, s);
  }
  இல்லையெனில்
  {
    எனில் (r <= b)      hsl_set_saturation_inner (g, r, b, s);
    இல்லையெனில் எனில் (g <= b) hsl_set_saturation_inner (g, b, r, s);
    இல்லையெனில்             hsl_set_saturation_inner (b, g, r, s);
  }
}

நிலையான உள்வரி uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  தசம sr, sg, sb, sa;
  தசம dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  தசம usr = sa > 0.f ? sr / sa : 0.f;
  தசம usg = sa > 0.f ? sg / sa : 0.f;
  தசம usb = sa > 0.f ? sb / sa : 0.f;
  தசம udr = da > 0.f ? dr / da : 0.f;
  தசம udg = da > 0.f ? dg / da : 0.f;
  தசம udb = da > 0.f ? db / da : 0.f;

  தசம br = udr, bg = udg, bb = udb;

  எனில் (mode == HB_PAINT_COMPOSITE_MODE_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  இல்லையெனில் எனில் (mode == HB_PAINT_COMPOSITE_MODE_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  இல்லையெனில் எனில் (mode == HB_PAINT_COMPOSITE_MODE_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  இல்லையெனில் /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  தசம ra = sa + da - sa * da;
  தசம rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  தசம rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  தசம rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  திரும்பு pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
நிலையான உள்வரி uint32_t
composite_pixel (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  தேர்வு (mode)
  {
  நிலை HB_PAINT_COMPOSITE_MODE_CLEAR:
    திரும்பு 0;
  நிலை HB_PAINT_COMPOSITE_MODE_SRC:
    திரும்பு src;
  நிலை HB_PAINT_COMPOSITE_MODE_DEST:
    திரும்பு dst;
  நிலை HB_PAINT_COMPOSITE_MODE_SRC_OVER:
    திரும்பு hb_raster_src_over (src, dst);
  நிலை HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    திரும்பு hb_raster_src_over (dst, src);
  நிலை HB_PAINT_COMPOSITE_MODE_SRC_IN:
    திரும்பு hb_raster_alpha_mul (src, da);
  நிலை HB_PAINT_COMPOSITE_MODE_DEST_IN:
    திரும்பு hb_raster_alpha_mul (dst, sa);
  நிலை HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    திரும்பு hb_raster_alpha_mul (src, 255 - da);
  நிலை HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    திரும்பு hb_raster_alpha_mul (dst, 255 - sa);
  நிலை HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    திரும்பு hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  நிலை HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    திரும்பு hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  நிலை HB_PAINT_COMPOSITE_MODE_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    திரும்பு hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  நிலை HB_PAINT_COMPOSITE_MODE_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (src >> 24) + (dst >> 24));
    திரும்பு hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  நிலை HB_PAINT_COMPOSITE_MODE_MULTIPLY:  திரும்பு apply_separable_blend (src, dst, blend_multiply);
  நிலை HB_PAINT_COMPOSITE_MODE_SCREEN:    திரும்பு apply_separable_blend (src, dst, blend_screen);
  நிலை HB_PAINT_COMPOSITE_MODE_OVERLAY:   திரும்பு apply_separable_blend (src, dst, blend_overlay);
  நிலை HB_PAINT_COMPOSITE_MODE_DARKEN:    திரும்பு apply_separable_blend (src, dst, blend_darken);
  நிலை HB_PAINT_COMPOSITE_MODE_LIGHTEN:   திரும்பு apply_separable_blend (src, dst, blend_lighten);
  நிலை HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: திரும்பு apply_separable_blend (src, dst, blend_color_dodge);
  நிலை HB_PAINT_COMPOSITE_MODE_COLOR_BURN:  திரும்பு apply_separable_blend (src, dst, blend_color_burn);
  நிலை HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:  திரும்பு apply_separable_blend (src, dst, blend_hard_light);
  நிலை HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:  திரும்பு apply_separable_blend (src, dst, blend_soft_light);
  நிலை HB_PAINT_COMPOSITE_MODE_DIFFERENCE:  திரும்பு apply_separable_blend (src, dst, blend_difference);
  நிலை HB_PAINT_COMPOSITE_MODE_EXCLUSION:   திரும்பு apply_separable_blend (src, dst, blend_exclusion);

  நிலை HB_PAINT_COMPOSITE_MODE_HSL_HUE:
  நிலை HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
  நிலை HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
  நிலை HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
    திரும்பு apply_hsl_blend (src, dst, mode);

  இயல்பு:
    திரும்பு hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

unsigned
hb_raster_image_t::bytes_per_pixel (hb_raster_format_t format)
{
  திரும்பு format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

தருக்க
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  எனில் (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  unsigned bpp = bytes_per_pixel (format);
  எனில் (extents.width > UINT_MAX / bpp)
    திரும்பு பொய்;

  unsigned min_stride = extents.width * bpp;
  எனில் (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  எனில் (extents.height && extents.stride > (size_t) -1 / extents.height)
    திரும்பு பொய்;

  size_t buf_size = (size_t) extents.stride * extents.height;
  எனில் (buf_size > HB_RASTER_MAX_BUFFER_SIZE)
    திரும்பு பொய்;
  எனில் (unlikely (!buffer.resize_dirty (buf_size)))
    திரும்பு பொய்;

  இது->format = format;
  இது->extents = extents;
  திரும்பு உண்மை;
}

தருக்க
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#ifndef HAVE_PNG
  திரும்பு பொய்;
#இல்லையெனில்
  எனில் (!blob)
    திரும்பு பொய்;

  unsigned blob_len = 0;
  மாறிலி uint8_t *blob_data = (மாறிலி uint8_t *) hb_blob_get_data (blob, &blob_len);
  எனில் (!blob_data || !blob_len)
    திரும்பு பொய்;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, nullptr,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  எனில் (!png)
    திரும்பு பொய்;

  png_infop info = png_create_info_struct (png);
  எனில் (!info)
  {
    png_destroy_read_struct (&png, nullptr, nullptr);
    திரும்பு பொய்;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, அளவு (*reader));
  எனில் (!reader)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    திரும்பு பொய்;
  }

  reader->data = blob_data;
  reader->size = (size_t) blob_len;
  png_set_read_fn (png, reader, hb_raster_png_read_blob);
  எனில் (setjmp (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    திரும்பு பொய்;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  முழு bit_depth = 0, color_type = 0;
  முழு interlace_type = 0, compression_type = 0, filter_method = 0;
  png_get_IHDR (png, info, &w, &h, &bit_depth, &color_type,
		&interlace_type, &compression_type, &filter_method);

  எனில் (!w || !h || w > (png_uint_32) INT_MAX || h > (png_uint_32) INT_MAX)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    திரும்பு பொய்;
  }

  தருக்க has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  எனில் (bit_depth == 16)
    png_set_strip_16 (png);
  எனில் (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png);
  எனில் (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8 (png);
  எனில் (has_trns)
    png_set_tRNS_to_alpha (png);
  எனில் (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb (png);
  எனில் (!(color_type & PNG_COLOR_MASK_ALPHA) && !has_trns)
    png_set_add_alpha (png, 0xff, PNG_FILLER_AFTER);
  எனில் (interlace_type != PNG_INTERLACE_NONE)
    png_set_interlace_handling (png);

  png_read_update_info (png, info);

  எனில் (png_get_bit_depth (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    திரும்பு பொய்;
  }

  png_size_t rowbytes = png_get_rowbytes (png, info);
  எனில் (rowbytes < (png_size_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    திரும்பு பொய்;
  }

  size_t rgba_size = (size_t) rowbytes * (size_t) h;
  எனில் ((h && rgba_size / (size_t) h != (size_t) rowbytes) ||
      rgba_size > HB_RASTER_MAX_BUFFER_SIZE)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    திரும்பு பொய்;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_size);
  எனில் (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    திரும்பு பொய்;
  }

  size_t rows_size = (size_t) h * அளவு (png_bytep);
  எனில் (h && rows_size / (size_t) h != அளவு (png_bytep))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    திரும்பு பொய்;
  }

  reader->rows = (png_bytep *) hb_malloc (rows_size);
  எனில் (!reader->rows)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    திரும்பு பொய்;
  }

  க்கு (unsigned y = 0; y < (unsigned) h; y++)
    reader->rows[y] = (png_bytep) (reader->rgba + (size_t) y * (size_t) rowbytes);

  png_read_image (png, reader->rows);
  png_read_end (png, nullptr);
  png_destroy_read_struct (&png, &info, nullptr);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (unsigned) w, (unsigned) h, 0};
  எனில் (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    திரும்பு பொய்;
  }

  க்கு (unsigned y = 0; y < (unsigned) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.buffer.arrayZ + (size_t) ((unsigned) h - 1 - y) * decoded.extents.stride);
    மாறிலி uint8_t *src = reader->rgba + (size_t) y * (size_t) rowbytes;
    க்கு (unsigned x = 0; x < (unsigned) w; x++)
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
  hb_swap (இது->extents, decoded.extents);
  hb_swap (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  திரும்பு உண்மை;
#endif
}

hb_blob_t *
hb_raster_image_t::serialize_to_png_or_fail () மாறிலி
{
#ifndef HAVE_PNG
  திரும்பு nullptr;
#இல்லையெனில்
  எனில் (format != HB_RASTER_FORMAT_BGRA32 || !extents.width || !extents.height)
    திரும்பு nullptr;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, nullptr,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  எனில் (!png)
    திரும்பு nullptr;

  png_infop info = png_create_info_struct (png);
  எனில் (!info)
  {
    png_destroy_write_struct (&png, nullptr);
    திரும்பு nullptr;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, அளவு (*writer));
  எனில் (!writer)
  {
    png_destroy_write_struct (&png, &info);
    திரும்பு nullptr;
  }

  png_set_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  எனில் (setjmp (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    திரும்பு nullptr;
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
  எனில் (extents.height && rgba_size / (size_t) extents.height != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    திரும்பு nullptr;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_size);
  எனில் (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    திரும்பு nullptr;
  }

  size_t rows_size = (size_t) extents.height * அளவு (png_bytep);
  எனில் (extents.height && rows_size / (size_t) extents.height != அளவு (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    திரும்பு nullptr;
  }

  writer->rows = (png_bytep *) hb_malloc (rows_size);
  எனில் (!writer->rows)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    திரும்பு nullptr;
  }

  க்கு (unsigned y = 0; y < extents.height; y++)
  {
    uint8_t *dst = writer->rgba + (size_t) y * rowbytes;
    மாறிலி uint8_t *src = buffer.arrayZ + (size_t) (extents.height - 1 - y) * extents.stride;

    க்கு (unsigned x = 0; x < extents.width; x++)
    {
      uint32_t px;
      hb_memcpy (&px, src + x * 4, 4);
      uint8_t b = (uint8_t) (px & 0xFF);
      uint8_t g = (uint8_t) ((px >> 8) & 0xFF);
      uint8_t r = (uint8_t) ((px >> 16) & 0xFF);
      uint8_t a = (uint8_t) (px >> 24);

      dst[4 * x + 3] = a;
      எனில் (a)
      {
	dst[4 * x + 0] = (uint8_t) hb_min (255u, ((unsigned) r * 255u + a / 2u) / a);
	dst[4 * x + 1] = (uint8_t) hb_min (255u, ((unsigned) g * 255u + a / 2u) / a);
	dst[4 * x + 2] = (uint8_t) hb_min (255u, ((unsigned) b * 255u + a / 2u) / a);
      }
      இல்லையெனில்
	dst[4 * x + 0] = dst[4 * x + 1] = dst[4 * x + 2] = 0;
    }

    writer->rows[y] = (png_bytep) dst;
  }

  png_write_image (png, writer->rows);
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);

  எனில் (writer->length > (size_t) (unsigned) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    திரும்பு nullptr;
  }

  unsigned length = (unsigned) writer->length;
  எழுத்து *data = writer->data;
  writer->data = nullptr;
  hb_raster_png_write_blob_fini (writer);
  எனில் (!data && length)
    திரும்பு nullptr;

  hb_blob_t *blob = hb_blob_create_or_fail (data, length,
					    HB_MEMORY_MODE_WRITABLE,
					    data, hb_free);
  எனில் (!blob)
    hb_free (data);
  திரும்பு blob;
#endif
}

வெறுமை
hb_raster_image_t::clear ()
{
  size_t buf_size = (size_t) extents.stride * extents.height;
  hb_memset (buffer.arrayZ, 0, buf_size);
}

மாறிலி uint8_t *
hb_raster_image_t::get_buffer () மாறிலி
{
  திரும்பு buffer.arrayZ;
}

வெறுமை
hb_raster_image_t::composite_from (மாறிலி hb_raster_image_t *src,
				   hb_paint_composite_mode_t mode)
{
  unsigned w = extents.width;
  unsigned h = extents.height;
  unsigned stride = extents.stride;

  க்கு (unsigned y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (buffer.arrayZ + y * stride);
    மாறிலி hb_packed_t<uint32_t> *sp = (மாறிலி hb_packed_t<uint32_t> *) (src->buffer.arrayZ + y * stride);
    க்கு (unsigned x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (composite_pixel ((uint32_t) sp[x], (uint32_t) dp[x], mode));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
வெறுமை
hb_raster_image_composite (hb_raster_image_t *dst,
			   மாறிலி hb_raster_image_t *src,
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
hb_raster_image_create_or_fail (வெறுமை)
{
  திரும்பு hb_object_create<hb_raster_image_t> ();
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
  திரும்பு hb_object_reference (image);
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
வெறுமை
hb_raster_image_destroy (hb_raster_image_t *image)
{
  எனில் (!hb_object_destroy (image)) திரும்பு;
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
			       hb_user_data_சாவி_t *key,
			       வெறுமை               *data,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  திரும்பு hb_object_set_user_data (image, key, data, destroy, replace);
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
வெறுமை *
hb_raster_image_get_user_data (மாறிலி hb_raster_image_t  *image,
			       hb_user_data_சாவி_t *key)
{
  திரும்பு hb_object_get_user_data (image, key);
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
			   மாறிலி hb_raster_extents_t *extents)
{
  எனில் (unlikely (!extents))
  {
    image->extents = {};
    image->buffer.resize_exact (0);
    திரும்பு உண்மை;
  }
  திரும்பு image->configure (format, *extents);
}

/**
 * hb_raster_image_clear:
 * @image: a raster image
 *
 * Clears @image pixels to zero while keeping current extents and format.
 *
 * Since: 13.0.0
 **/
வெறுமை
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
மாறிலி uint8_t *
hb_raster_image_get_buffer (மாறிலி hb_raster_image_t *image)
{
  திரும்பு image->get_buffer ();
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
வெறுமை
hb_raster_image_get_extents (மாறிலி hb_raster_image_t   *image,
			     hb_raster_extents_t *extents)
{
  எனில் (extents)
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
hb_raster_image_get_format (மாறிலி hb_raster_image_t *image)
{
  திரும்பு image->format;
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
  திரும்பு image->deserialize_from_png (png);
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
hb_raster_image_serialize_to_png_or_fail (மாறிலி hb_raster_image_t *image)
{
  திரும்பு image->serialize_to_png_or_fail ();
}
