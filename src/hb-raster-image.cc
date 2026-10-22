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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-raster-image.hh"

#અન્તર્ભાવ <math.h>

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
#અન્તર્ભાવ <png.h>
#endif

#ifdef HAVE_PNG
સંરચના hb_raster_png_read_blob_t
{
  અચલ uint8_t *data = nullptr;
  size_t size = 0;
  size_t offset = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

સ્થિર શૂન્ય
hb_raster_png_error (png_structp png,
		     png_const_charp સંદેશ HB_UNUSED)
{
#ifdef PNG_SETJMP_SUPPORTED
  longjmp (png_jmpbuf (png), 1);
#endif
}

સ્થિર શૂન્ય
hb_raster_png_warning (png_structp png HB_UNUSED,
		       png_const_charp સંદેશ HB_UNUSED)
{}

સ્થિર શૂન્ય
hb_raster_png_read_blob (png_structp png, png_bytep out, png_size_t length)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  જો (!r || !r->data || length > r->size - r->offset)
    png_error (png, "read error");

  hb_memcpy (out, r->data + r->offset, length);
  r->offset += length;
}

સ્થિર શૂન્ય
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  જો (!r)
    પાછા;

  hb_free (r->rgba);
  hb_free (r->rows);
  hb_free (r);
}

સંરચના hb_raster_png_write_blob_t
{
  અક્ષર *data = nullptr;
  size_t length = 0;
  size_t allocated = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

સ્થિર શૂન્ય
hb_raster_png_write_blob (png_structp png, png_bytep in, png_size_t length)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  જો (!w)
    png_error (png, "write error");

  size_t old_length = w->length;
  જો ((size_t) length > (size_t) -1 - old_length)
    png_error (png, "write error");

  size_t new_length = old_length + length;
  જો (new_length > w->allocated)
  {
    size_t new_allocated = w->allocated ? w->allocated : 4096;
    જ્યારે (new_allocated < new_length)
    {
      size_t next = new_allocated * 2;
      જો (next <= new_allocated)
      {
	new_allocated = new_length;
	તોડો;
      }
      new_allocated = next;
    }

    અક્ષર *data = (અક્ષર *) hb_realloc (w->data, new_allocated);
    જો (!data)
      png_error (png, "write error");
    w->data = data;
    w->allocated = new_allocated;
  }

  hb_memcpy (w->data + old_length, in, length);
  w->length = new_length;
}

સ્થિર શૂન્ય
hb_raster_png_flush_blob (png_structp png HB_UNUSED)
{}

સ્થિર શૂન્ય
hb_raster_png_write_blob_fini (hb_raster_png_write_blob_t *w)
{
  જો (!w)
    પાછા;

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
સ્થિર અંતઃરેખ શૂન્ય
unpack_to_float (uint32_t px, દશાંશ &r, દશાંશ &g, દશાંશ &b, દશાંશ &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
સ્થિર અંતઃરેખ uint32_t
pack_from_float (દશાંશ r, દશાંશ g, દશાંશ b, દશાંશ a)
{
  પાછા hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
સ્થિર અંતઃરેખ દશાંશ
blend_multiply (દશાંશ sc, દશાંશ dc) { પાછા sc * dc; }
સ્થિર અંતઃરેખ દશાંશ
blend_screen (દશાંશ sc, દશાંશ dc) { પાછા sc + dc - sc * dc; }
સ્થિર અંતઃરેખ દશાંશ
blend_overlay (દશાંશ sc, દશાંશ dc)
{ પાછા dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
સ્થિર અંતઃરેખ દશાંશ
blend_darken (દશાંશ sc, દશાંશ dc) { પાછા hb_min (sc, dc); }
સ્થિર અંતઃરેખ દશાંશ
blend_lighten (દશાંશ sc, દશાંશ dc) { પાછા hb_max (sc, dc); }
સ્થિર અંતઃરેખ દશાંશ
blend_color_dodge (દશાંશ sc, દશાંશ dc)
{
  જો (dc <= 0.f) પાછા 0.f;
  જો (sc >= 1.f) પાછા 1.f;
  પાછા hb_min (1.f, dc / (1.f - sc));
}
સ્થિર અંતઃરેખ દશાંશ
blend_color_burn (દશાંશ sc, દશાંશ dc)
{
  જો (dc >= 1.f) પાછા 1.f;
  જો (sc <= 0.f) પાછા 0.f;
  પાછા 1.f - hb_min (1.f, (1.f - dc) / sc);
}
સ્થિર અંતઃરેખ દશાંશ
blend_hard_light (દશાંશ sc, દશાંશ dc)
{ પાછા sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
સ્થિર અંતઃરેખ દશાંશ
blend_soft_light (દશાંશ sc, દશાંશ dc)
{
  જો (sc <= 0.5f)
    પાછા dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  દશાંશ d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  પાછા dc + (2.f * sc - 1.f) * (d - dc);
}
સ્થિર અંતઃરેખ દશાંશ
blend_difference (દશાંશ sc, દશાંશ dc) { પાછા fabsf (sc - dc); }
સ્થિર અંતઃરેખ દશાંશ
blend_exclusion (દશાંશ sc, દશાંશ dc) { પાછા sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
સ્થિર અંતઃરેખ uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       દશાંશ (*blend_fn)(દશાંશ, દશાંશ))
{
  દશાંશ sr, sg, sb, sa;
  દશાંશ dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  દશાંશ usr = sa > 0.f ? sr / sa : 0.f;
  દશાંશ usg = sa > 0.f ? sg / sa : 0.f;
  દશાંશ usb = sa > 0.f ? sb / sa : 0.f;
  દશાંશ udr = da > 0.f ? dr / da : 0.f;
  દશાંશ udg = da > 0.f ? dg / da : 0.f;
  દશાંશ udb = da > 0.f ? db / da : 0.f;

  દશાંશ br = blend_fn (usr, udr);
  દશાંશ bg = blend_fn (usg, udg);
  દશાંશ bb = blend_fn (usb, udb);

  દશાંશ ra = sa + da - sa * da;
  દશાંશ rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  દશાંશ rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  દશાંશ rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  પાછા pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
સ્થિર અંતઃરેખ દશાંશ
hsl_luminosity (દશાંશ r, દશાંશ g, દશાંશ b)
{ પાછા 0.299f * r + 0.587f * g + 0.114f * b; }

સ્થિર અંતઃરેખ દશાંશ
hsl_saturation (દશાંશ r, દશાંશ g, દશાંશ b)
{ પાછા hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

સ્થિર અંતઃરેખ શૂન્ય
hsl_clip_color (દશાંશ &r, દશાંશ &g, દશાંશ &b)
{
  દશાંશ l = hsl_luminosity (r, g, b);
  દશાંશ mn = hb_min (hb_min (r, g), b);
  દશાંશ mx = hb_max (hb_max (r, g), b);
  જો (mn < 0.f)
  {
    દશાંશ d = l - mn;
    જો (d > 0.f) { r = l + (r - l) * l / d; g = l + (g - l) * l / d; b = l + (b - l) * l / d; }
  }
  જો (mx > 1.f)
  {
    દશાંશ d = mx - l;
    જો (d > 0.f) { r = l + (r - l) * (1.f - l) / d; g = l + (g - l) * (1.f - l) / d; b = l + (b - l) * (1.f - l) / d; }
  }
}

સ્થિર અંતઃરેખ શૂન્ય
hsl_set_luminosity (દશાંશ &r, દશાંશ &g, દશાંશ &b, દશાંશ l)
{
  દશાંશ d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

સ્થિર અંતઃરેખ શૂન્ય
hsl_set_saturation_inner (દશાંશ &mn, દશાંશ &mid, દશાંશ &mx, દશાંશ s)
{
  જો (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  નહીંતર
    mid = mx = 0.f;
  mn = 0.f;
}

સ્થિર અંતઃરેખ શૂન્ય
hsl_set_saturation (દશાંશ &r, દશાંશ &g, દશાંશ &b, દશાંશ s)
{
  જો (r <= g)
  {
    જો (g <= b)      hsl_set_saturation_inner (r, g, b, s);
    નહીંતર જો (r <= b) hsl_set_saturation_inner (r, b, g, s);
    નહીંતર             hsl_set_saturation_inner (b, r, g, s);
  }
  નહીંતર
  {
    જો (r <= b)      hsl_set_saturation_inner (g, r, b, s);
    નહીંતર જો (g <= b) hsl_set_saturation_inner (g, b, r, s);
    નહીંતર             hsl_set_saturation_inner (b, g, r, s);
  }
}

સ્થિર અંતઃરેખ uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  દશાંશ sr, sg, sb, sa;
  દશાંશ dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  દશાંશ usr = sa > 0.f ? sr / sa : 0.f;
  દશાંશ usg = sa > 0.f ? sg / sa : 0.f;
  દશાંશ usb = sa > 0.f ? sb / sa : 0.f;
  દશાંશ udr = da > 0.f ? dr / da : 0.f;
  દશાંશ udg = da > 0.f ? dg / da : 0.f;
  દશાંશ udb = da > 0.f ? db / da : 0.f;

  દશાંશ br = udr, bg = udg, bb = udb;

  જો (mode == HB_PAINT_COMPOSITE_MODE_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  નહીંતર જો (mode == HB_PAINT_COMPOSITE_MODE_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  નહીંતર જો (mode == HB_PAINT_COMPOSITE_MODE_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  નહીંતર /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  દશાંશ ra = sa + da - sa * da;
  દશાંશ rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  દશાંશ rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  દશાંશ rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  પાછા pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
સ્થિર અંતઃરેખ uint32_t
composite_pixel (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  પસંદગી (mode)
  {
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_CLEAR:
    પાછા 0;
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC:
    પાછા src;
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST:
    પાછા dst;
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_OVER:
    પાછા hb_raster_src_over (src, dst);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    પાછા hb_raster_src_over (dst, src);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_IN:
    પાછા hb_raster_alpha_mul (src, da);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_IN:
    પાછા hb_raster_alpha_mul (dst, sa);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    પાછા hb_raster_alpha_mul (src, 255 - da);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    પાછા hb_raster_alpha_mul (dst, 255 - sa);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    પાછા hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    પાછા hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    પાછા hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (src >> 24) + (dst >> 24));
    પાછા hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  સ્થિતિ HB_PAINT_COMPOSITE_MODE_MULTIPLY:  પાછા apply_separable_blend (src, dst, blend_multiply);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_SCREEN:    પાછા apply_separable_blend (src, dst, blend_screen);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_OVERLAY:   પાછા apply_separable_blend (src, dst, blend_overlay);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_DARKEN:    પાછા apply_separable_blend (src, dst, blend_darken);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_LIGHTEN:   પાછા apply_separable_blend (src, dst, blend_lighten);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: પાછા apply_separable_blend (src, dst, blend_color_dodge);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_COLOR_BURN:  પાછા apply_separable_blend (src, dst, blend_color_burn);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:  પાછા apply_separable_blend (src, dst, blend_hard_light);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:  પાછા apply_separable_blend (src, dst, blend_soft_light);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_DIFFERENCE:  પાછા apply_separable_blend (src, dst, blend_difference);
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_EXCLUSION:   પાછા apply_separable_blend (src, dst, blend_exclusion);

  સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_HUE:
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
  સ્થિતિ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
    પાછા apply_hsl_blend (src, dst, mode);

  મૂળભૂત:
    પાછા hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

unsigned
hb_raster_image_t::bytes_per_pixel (hb_raster_format_t format)
{
  પાછા format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

તાર્કિક
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  જો (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  unsigned bpp = bytes_per_pixel (format);
  જો (extents.width > UINT_MAX / bpp)
    પાછા ખોટું;

  unsigned min_stride = extents.width * bpp;
  જો (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  જો (extents.height && extents.stride > (size_t) -1 / extents.height)
    પાછા ખોટું;

  size_t buf_size = (size_t) extents.stride * extents.height;
  જો (buf_size > HB_RASTER_MAX_BUFFER_SIZE)
    પાછા ખોટું;
  જો (unlikely (!buffer.resize_dirty (buf_size)))
    પાછા ખોટું;

  આ->format = format;
  આ->extents = extents;
  પાછા સાચું;
}

તાર્કિક
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#ifndef HAVE_PNG
  પાછા ખોટું;
#નહીંતર
  જો (!blob)
    પાછા ખોટું;

  unsigned blob_len = 0;
  અચલ uint8_t *blob_data = (અચલ uint8_t *) hb_blob_get_data (blob, &blob_len);
  જો (!blob_data || !blob_len)
    પાછા ખોટું;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, nullptr,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  જો (!png)
    પાછા ખોટું;

  png_infop info = png_create_info_struct (png);
  જો (!info)
  {
    png_destroy_read_struct (&png, nullptr, nullptr);
    પાછા ખોટું;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, કદ (*reader));
  જો (!reader)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    પાછા ખોટું;
  }

  reader->data = blob_data;
  reader->size = (size_t) blob_len;
  png_set_read_fn (png, reader, hb_raster_png_read_blob);
  જો (setjmp (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    પાછા ખોટું;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  પૂર્ણાંક bit_depth = 0, color_type = 0;
  પૂર્ણાંક interlace_type = 0, compression_type = 0, filter_method = 0;
  png_get_IHDR (png, info, &w, &h, &bit_depth, &color_type,
		&interlace_type, &compression_type, &filter_method);

  જો (!w || !h || w > (png_uint_32) INT_MAX || h > (png_uint_32) INT_MAX)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    પાછા ખોટું;
  }

  તાર્કિક has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  જો (bit_depth == 16)
    png_set_strip_16 (png);
  જો (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png);
  જો (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8 (png);
  જો (has_trns)
    png_set_tRNS_to_alpha (png);
  જો (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb (png);
  જો (!(color_type & PNG_COLOR_MASK_ALPHA) && !has_trns)
    png_set_add_alpha (png, 0xff, PNG_FILLER_AFTER);
  જો (interlace_type != PNG_INTERLACE_NONE)
    png_set_interlace_handling (png);

  png_read_update_info (png, info);

  જો (png_get_bit_depth (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    પાછા ખોટું;
  }

  png_size_t rowbytes = png_get_rowbytes (png, info);
  જો (rowbytes < (png_size_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    પાછા ખોટું;
  }

  size_t rgba_size = (size_t) rowbytes * (size_t) h;
  જો ((h && rgba_size / (size_t) h != (size_t) rowbytes) ||
      rgba_size > HB_RASTER_MAX_BUFFER_SIZE)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    પાછા ખોટું;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_size);
  જો (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    પાછા ખોટું;
  }

  size_t rows_size = (size_t) h * કદ (png_bytep);
  જો (h && rows_size / (size_t) h != કદ (png_bytep))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    પાછા ખોટું;
  }

  reader->rows = (png_bytep *) hb_malloc (rows_size);
  જો (!reader->rows)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    પાછા ખોટું;
  }

  માટે (unsigned y = 0; y < (unsigned) h; y++)
    reader->rows[y] = (png_bytep) (reader->rgba + (size_t) y * (size_t) rowbytes);

  png_read_image (png, reader->rows);
  png_read_end (png, nullptr);
  png_destroy_read_struct (&png, &info, nullptr);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (unsigned) w, (unsigned) h, 0};
  જો (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    પાછા ખોટું;
  }

  માટે (unsigned y = 0; y < (unsigned) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.buffer.arrayZ + (size_t) ((unsigned) h - 1 - y) * decoded.extents.stride);
    અચલ uint8_t *src = reader->rgba + (size_t) y * (size_t) rowbytes;
    માટે (unsigned x = 0; x < (unsigned) w; x++)
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
  hb_swap (આ->extents, decoded.extents);
  hb_swap (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  પાછા સાચું;
#endif
}

hb_blob_t *
hb_raster_image_t::serialize_to_png_or_fail () અચલ
{
#ifndef HAVE_PNG
  પાછા nullptr;
#નહીંતર
  જો (format != HB_RASTER_FORMAT_BGRA32 || !extents.width || !extents.height)
    પાછા nullptr;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, nullptr,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  જો (!png)
    પાછા nullptr;

  png_infop info = png_create_info_struct (png);
  જો (!info)
  {
    png_destroy_write_struct (&png, nullptr);
    પાછા nullptr;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, કદ (*writer));
  જો (!writer)
  {
    png_destroy_write_struct (&png, &info);
    પાછા nullptr;
  }

  png_set_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  જો (setjmp (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    પાછા nullptr;
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
  જો (extents.height && rgba_size / (size_t) extents.height != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    પાછા nullptr;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_size);
  જો (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    પાછા nullptr;
  }

  size_t rows_size = (size_t) extents.height * કદ (png_bytep);
  જો (extents.height && rows_size / (size_t) extents.height != કદ (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    પાછા nullptr;
  }

  writer->rows = (png_bytep *) hb_malloc (rows_size);
  જો (!writer->rows)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    પાછા nullptr;
  }

  માટે (unsigned y = 0; y < extents.height; y++)
  {
    uint8_t *dst = writer->rgba + (size_t) y * rowbytes;
    અચલ uint8_t *src = buffer.arrayZ + (size_t) (extents.height - 1 - y) * extents.stride;

    માટે (unsigned x = 0; x < extents.width; x++)
    {
      uint32_t px;
      hb_memcpy (&px, src + x * 4, 4);
      uint8_t b = (uint8_t) (px & 0xFF);
      uint8_t g = (uint8_t) ((px >> 8) & 0xFF);
      uint8_t r = (uint8_t) ((px >> 16) & 0xFF);
      uint8_t a = (uint8_t) (px >> 24);

      dst[4 * x + 3] = a;
      જો (a)
      {
	dst[4 * x + 0] = (uint8_t) hb_min (255u, ((unsigned) r * 255u + a / 2u) / a);
	dst[4 * x + 1] = (uint8_t) hb_min (255u, ((unsigned) g * 255u + a / 2u) / a);
	dst[4 * x + 2] = (uint8_t) hb_min (255u, ((unsigned) b * 255u + a / 2u) / a);
      }
      નહીંતર
	dst[4 * x + 0] = dst[4 * x + 1] = dst[4 * x + 2] = 0;
    }

    writer->rows[y] = (png_bytep) dst;
  }

  png_write_image (png, writer->rows);
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);

  જો (writer->length > (size_t) (unsigned) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    પાછા nullptr;
  }

  unsigned length = (unsigned) writer->length;
  અક્ષર *data = writer->data;
  writer->data = nullptr;
  hb_raster_png_write_blob_fini (writer);
  જો (!data && length)
    પાછા nullptr;

  hb_blob_t *blob = hb_blob_create_or_fail (data, length,
					    HB_MEMORY_MODE_WRITABLE,
					    data, hb_free);
  જો (!blob)
    hb_free (data);
  પાછા blob;
#endif
}

શૂન્ય
hb_raster_image_t::clear ()
{
  size_t buf_size = (size_t) extents.stride * extents.height;
  hb_memset (buffer.arrayZ, 0, buf_size);
}

અચલ uint8_t *
hb_raster_image_t::get_buffer () અચલ
{
  પાછા buffer.arrayZ;
}

શૂન્ય
hb_raster_image_t::composite_from (અચલ hb_raster_image_t *src,
				   hb_paint_composite_mode_t mode)
{
  unsigned w = extents.width;
  unsigned h = extents.height;
  unsigned stride = extents.stride;

  માટે (unsigned y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (buffer.arrayZ + y * stride);
    અચલ hb_packed_t<uint32_t> *sp = (અચલ hb_packed_t<uint32_t> *) (src->buffer.arrayZ + y * stride);
    માટે (unsigned x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (composite_pixel ((uint32_t) sp[x], (uint32_t) dp[x], mode));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
શૂન્ય
hb_raster_image_composite (hb_raster_image_t *dst,
			   અચલ hb_raster_image_t *src,
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
hb_raster_image_create_or_fail (શૂન્ય)
{
  પાછા hb_object_create<hb_raster_image_t> ();
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
  પાછા hb_object_reference (image);
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
શૂન્ય
hb_raster_image_destroy (hb_raster_image_t *image)
{
  જો (!hb_object_destroy (image)) પાછા;
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
			       hb_user_data_ચાવી_t *key,
			       શૂન્ય               *data,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  પાછા hb_object_set_user_data (image, key, data, destroy, replace);
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
શૂન્ય *
hb_raster_image_get_user_data (અચલ hb_raster_image_t  *image,
			       hb_user_data_ચાવી_t *key)
{
  પાછા hb_object_get_user_data (image, key);
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
			   અચલ hb_raster_extents_t *extents)
{
  જો (unlikely (!extents))
  {
    image->extents = {};
    image->buffer.resize_exact (0);
    પાછા સાચું;
  }
  પાછા image->configure (format, *extents);
}

/**
 * hb_raster_image_clear:
 * @image: a raster image
 *
 * Clears @image pixels to zero while keeping current extents and format.
 *
 * Since: 13.0.0
 **/
શૂન્ય
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
અચલ uint8_t *
hb_raster_image_get_buffer (અચલ hb_raster_image_t *image)
{
  પાછા image->get_buffer ();
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
શૂન્ય
hb_raster_image_get_extents (અચલ hb_raster_image_t   *image,
			     hb_raster_extents_t *extents)
{
  જો (extents)
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
hb_raster_image_get_format (અચલ hb_raster_image_t *image)
{
  પાછા image->format;
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
  પાછા image->deserialize_from_png (png);
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
hb_raster_image_serialize_to_png_or_fail (અચલ hb_raster_image_t *image)
{
  પાછા image->serialize_to_png_or_fail ();
}
