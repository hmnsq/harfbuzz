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

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-raster-image.hh"

#అన్తర్భావ <math.h>

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

#నిర్వచితమైతే HAVE_PNG
#అన్తర్భావ <png.h>
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_PNG
నిర్మాణం hb_raster_png_read_blob_t
{
  స్థిరరాశి uint8_t *దత్తాంశం = శూన్యసూచిక;
  size_t కొలత = 0;
  size_t స్థానభ్రంశం = 0;
  uint8_t *rgba = శూన్యసూచిక;
  png_bytep *అడ్డువరుసలు = శూన్యసూచిక;
};

స్థిర శూన్య
hb_raster_png_error (png_structp png,
		     png_const_charp సందేశం_2 HB_వాడని)
{
#నిర్వచితమైతే PNG_SETJMP_SUPPORTED
  longjmp (png_jmpbuf (png), 1);
#అయితే_ముగింపు
}

స్థిర శూన్య
hb_raster_png_warning (png_structp png HB_వాడని,
		       png_const_charp సందేశం_2 HB_వాడని)
{}

స్థిర శూన్య
hb_raster_png_read_blob (png_structp png, png_bytep బహిర్గతం, png_కొలత_t నిడివి)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  ఒకవేళ (!r || !r->దత్తాంశం || నిడివి > r->కొలత - r->స్థానభ్రంశం)
    png_error (png, "read error");

  hb_memcpy (బహిర్గతం, r->దత్తాంశం + r->స్థానభ్రంశం, నిడివి);
  r->స్థానభ్రంశం += నిడివి;
}

స్థిర శూన్య
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  ఒకవేళ (!r)
    తిరిగి;

  hb_free (r->rgba);
  hb_free (r->అడ్డువరుసలు);
  hb_free (r);
}

నిర్మాణం hb_raster_png_write_blob_t
{
  అక్షరం *దత్తాంశం = శూన్యసూచిక;
  size_t నిడివి = 0;
  size_t allocated = 0;
  uint8_t *rgba = శూన్యసూచిక;
  png_bytep *అడ్డువరుసలు = శూన్యసూచిక;
};

స్థిర శూన్య
hb_raster_png_write_blob (png_structp png, png_bytep లోపల, png_కొలత_t నిడివి)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  ఒకవేళ (!w)
    png_error (png, "write error");

  size_t old_నిడివి = w->నిడివి;
  ఒకవేళ ((size_t) నిడివి > (size_t) -1 - old_నిడివి)
    png_error (png, "write error");

  size_t new_నిడివి = old_నిడివి + నిడివి;
  ఒకవేళ (new_నిడివి > w->allocated)
  {
    size_t new_allocated = w->allocated ? w->allocated : 4096;
    అయితే (new_allocated < new_నిడివి)
    {
      size_t next = new_allocated * 2;
      ఒకవేళ (next <= new_allocated)
      {
	new_allocated = new_నిడివి;
	ఆపు;
      }
      new_allocated = next;
    }

    అక్షరం *దత్తాంశం = (అక్షరం *) hb_realloc (w->దత్తాంశం, new_allocated);
    ఒకవేళ (!దత్తాంశం)
      png_error (png, "write error");
    w->దత్తాంశం = దత్తాంశం;
    w->allocated = new_allocated;
  }

  hb_memcpy (w->దత్తాంశం + old_నిడివి, లోపల, నిడివి);
  w->నిడివి = new_నిడివి;
}

స్థిర శూన్య
hb_raster_png_flush_blob (png_structp png HB_వాడని)
{}

స్థిర శూన్య
hb_raster_png_write_blob_fini (hb_raster_png_write_blob_t *w)
{
  ఒకవేళ (!w)
    తిరిగి;

  hb_free (w->దత్తాంశం);
  hb_free (w->rgba);
  hb_free (w->అడ్డువరుసలు);
  hb_free (w);
}
#అయితే_ముగింపు


/*
 * Image compositing
 */

/* Unpack premultiplied pixel to float RGBA [0,1]. */
స్థిర అంతఃవరుస శూన్య
unpack_గమ్యం_float (uint32_t px, దశాంశం &r, దశాంశం &g, దశాంశం &b, దశాంశం &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
స్థిర అంతఃవరుస uint32_t
pack_from_float (దశాంశం r, దశాంశం g, దశాంశం b, దశాంశం a)
{
  తిరిగి hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
స్థిర అంతఃవరుస దశాంశం
blend_multiply (దశాంశం sc, దశాంశం dc) { తిరిగి sc * dc; }
స్థిర అంతఃవరుస దశాంశం
blend_screen (దశాంశం sc, దశాంశం dc) { తిరిగి sc + dc - sc * dc; }
స్థిర అంతఃవరుస దశాంశం
blend_overlay (దశాంశం sc, దశాంశం dc)
{ తిరిగి dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
స్థిర అంతఃవరుస దశాంశం
blend_darken (దశాంశం sc, దశాంశం dc) { తిరిగి hb_min (sc, dc); }
స్థిర అంతఃవరుస దశాంశం
blend_lighten (దశాంశం sc, దశాంశం dc) { తిరిగి hb_max (sc, dc); }
స్థిర అంతఃవరుస దశాంశం
blend_color_dodge (దశాంశం sc, దశాంశం dc)
{
  ఒకవేళ (dc <= 0.f) తిరిగి 0.f;
  ఒకవేళ (sc >= 1.f) తిరిగి 1.f;
  తిరిగి hb_min (1.f, dc / (1.f - sc));
}
స్థిర అంతఃవరుస దశాంశం
blend_color_burn (దశాంశం sc, దశాంశం dc)
{
  ఒకవేళ (dc >= 1.f) తిరిగి 1.f;
  ఒకవేళ (sc <= 0.f) తిరిగి 0.f;
  తిరిగి 1.f - hb_min (1.f, (1.f - dc) / sc);
}
స్థిర అంతఃవరుస దశాంశం
blend_hard_light (దశాంశం sc, దశాంశం dc)
{ తిరిగి sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
స్థిర అంతఃవరుస దశాంశం
blend_soft_light (దశాంశం sc, దశాంశం dc)
{
  ఒకవేళ (sc <= 0.5f)
    తిరిగి dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  దశాంశం d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  తిరిగి dc + (2.f * sc - 1.f) * (d - dc);
}
స్థిర అంతఃవరుస దశాంశం
blend_difference (దశాంశం sc, దశాంశం dc) { తిరిగి fabsf (sc - dc); }
స్థిర అంతఃవరుస దశాంశం
blend_exclusion (దశాంశం sc, దశాంశం dc) { తిరిగి sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
స్థిర అంతఃవరుస uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       దశాంశం (*blend_fn)(దశాంశం, దశాంశం))
{
  దశాంశం sr, sg, sb, sa;
  దశాంశం dr, dg, db, da;
  unpack_గమ్యం_float (src, sr, sg, sb, sa);
  unpack_గమ్యం_float (dst, dr, dg, db, da);

  దశాంశం usr = sa > 0.f ? sr / sa : 0.f;
  దశాంశం usg = sa > 0.f ? sg / sa : 0.f;
  దశాంశం usb = sa > 0.f ? sb / sa : 0.f;
  దశాంశం udr = da > 0.f ? dr / da : 0.f;
  దశాంశం udg = da > 0.f ? dg / da : 0.f;
  దశాంశం udb = da > 0.f ? db / da : 0.f;

  దశాంశం br = blend_fn (usr, udr);
  దశాంశం bg = blend_fn (usg, udg);
  దశాంశం bb = blend_fn (usb, udb);

  దశాంశం ra = sa + da - sa * da;
  దశాంశం rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  దశాంశం rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  దశాంశం rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  తిరిగి pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
స్థిర అంతఃవరుస దశాంశం
hsl_luminosity (దశాంశం r, దశాంశం g, దశాంశం b)
{ తిరిగి 0.299f * r + 0.587f * g + 0.114f * b; }

స్థిర అంతఃవరుస దశాంశం
hsl_saturation (దశాంశం r, దశాంశం g, దశాంశం b)
{ తిరిగి hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

స్థిర అంతఃవరుస శూన్య
hsl_clip_color (దశాంశం &r, దశాంశం &g, దశాంశం &b)
{
  దశాంశం l = hsl_luminosity (r, g, b);
  దశాంశం mn = hb_min (hb_min (r, g), b);
  దశాంశం mx = hb_max (hb_max (r, g), b);
  ఒకవేళ (mn < 0.f)
  {
    దశాంశం d = l - mn;
    ఒకవేళ (d > 0.f) { r = l + (r - l) * l / d; g = l + (g - l) * l / d; b = l + (b - l) * l / d; }
  }
  ఒకవేళ (mx > 1.f)
  {
    దశాంశం d = mx - l;
    ఒకవేళ (d > 0.f) { r = l + (r - l) * (1.f - l) / d; g = l + (g - l) * (1.f - l) / d; b = l + (b - l) * (1.f - l) / d; }
  }
}

స్థిర అంతఃవరుస శూన్య
hsl_సమితి_luminosity (దశాంశం &r, దశాంశం &g, దశాంశం &b, దశాంశం l)
{
  దశాంశం d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

స్థిర అంతఃవరుస శూన్య
hsl_సమితి_saturation_inner (దశాంశం &mn, దశాంశం &mid, దశాంశం &mx, దశాంశం s)
{
  ఒకవేళ (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  లేకపోతే
    mid = mx = 0.f;
  mn = 0.f;
}

స్థిర అంతఃవరుస శూన్య
hsl_సమితి_saturation (దశాంశం &r, దశాంశం &g, దశాంశం &b, దశాంశం s)
{
  ఒకవేళ (r <= g)
  {
    ఒకవేళ (g <= b)      hsl_సమితి_saturation_inner (r, g, b, s);
    లేకపోతే ఒకవేళ (r <= b) hsl_సమితి_saturation_inner (r, b, g, s);
    లేకపోతే             hsl_సమితి_saturation_inner (b, r, g, s);
  }
  లేకపోతే
  {
    ఒకవేళ (r <= b)      hsl_సమితి_saturation_inner (g, r, b, s);
    లేకపోతే ఒకవేళ (g <= b) hsl_సమితి_saturation_inner (g, b, r, s);
    లేకపోతే             hsl_సమితి_saturation_inner (b, g, r, s);
  }
}

స్థిర అంతఃవరుస uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
{
  దశాంశం sr, sg, sb, sa;
  దశాంశం dr, dg, db, da;
  unpack_గమ్యం_float (src, sr, sg, sb, sa);
  unpack_గమ్యం_float (dst, dr, dg, db, da);

  దశాంశం usr = sa > 0.f ? sr / sa : 0.f;
  దశాంశం usg = sa > 0.f ? sg / sa : 0.f;
  దశాంశం usb = sa > 0.f ? sb / sa : 0.f;
  దశాంశం udr = da > 0.f ? dr / da : 0.f;
  దశాంశం udg = da > 0.f ? dg / da : 0.f;
  దశాంశం udb = da > 0.f ? db / da : 0.f;

  దశాంశం br = udr, bg = udg, bb = udb;

  ఒకవేళ (బహుళకం == HB_PAINT_సంయుక్త_బహుళకం_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_సమితి_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_సమితి_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  లేకపోతే ఒకవేళ (బహుళకం == HB_PAINT_సంయుక్త_బహుళకం_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_సమితి_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_సమితి_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  లేకపోతే ఒకవేళ (బహుళకం == HB_PAINT_సంయుక్త_బహుళకం_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_సమితి_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  లేకపోతే /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_సమితి_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  దశాంశం ra = sa + da - sa * da;
  దశాంశం rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  దశాంశం rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  దశాంశం rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  తిరిగి pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
స్థిర అంతఃవరుస uint32_t
సంయుక్త_pixel (uint32_t src, uint32_t dst,
		 hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  ఎంపిక (బహుళకం)
  {
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_CLEAR:
    తిరిగి 0;
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC:
    తిరిగి src;
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST:
    తిరిగి dst;
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_OVER:
    తిరిగి hb_raster_src_over (src, dst);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_OVER:
    తిరిగి hb_raster_src_over (dst, src);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_లోపల:
    తిరిగి hb_raster_alpha_mul (src, da);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_లోపల:
    తిరిగి hb_raster_alpha_mul (dst, sa);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_బహిర్గతం:
    తిరిగి hb_raster_alpha_mul (src, 255 - da);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_బహిర్గతం:
    తిరిగి hb_raster_alpha_mul (dst, 255 - sa);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (గుర్తులేని) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (గుర్తులేని) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (గుర్తులేని) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (గుర్తులేని) (a >> 24) + (b >> 24));
    తిరిగి hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (గుర్తులేని) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (గుర్తులేని) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (గుర్తులేని) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (గుర్తులేని) (a >> 24) + (b >> 24));
    తిరిగి hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (గుర్తులేని) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (గుర్తులేని) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (గుర్తులేని) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (గుర్తులేని) (a >> 24) + (b >> 24));
    తిరిగి hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (గుర్తులేని) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (గుర్తులేని) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (గుర్తులేని) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (గుర్తులేని) (src >> 24) + (dst >> 24));
    తిరిగి hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  స్థితి HB_PAINT_సంయుక్త_బహుళకం_MULTIPLY:  తిరిగి apply_separable_blend (src, dst, blend_multiply);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SCREEN:    తిరిగి apply_separable_blend (src, dst, blend_screen);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_OVERLAY:   తిరిగి apply_separable_blend (src, dst, blend_overlay);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DARKEN:    తిరిగి apply_separable_blend (src, dst, blend_darken);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_LIGHTEN:   తిరిగి apply_separable_blend (src, dst, blend_lighten);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_COLOR_DODGE: తిరిగి apply_separable_blend (src, dst, blend_color_dodge);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_COLOR_BURN:  తిరిగి apply_separable_blend (src, dst, blend_color_burn);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_HARD_LIGHT:  తిరిగి apply_separable_blend (src, dst, blend_hard_light);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SOFT_LIGHT:  తిరిగి apply_separable_blend (src, dst, blend_soft_light);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DIFFERENCE:  తిరిగి apply_separable_blend (src, dst, blend_difference);
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_EXCLUSION:   తిరిగి apply_separable_blend (src, dst, blend_exclusion);

  స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_HUE:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_SATURATION:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_COLOR:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_LUMINOSITY:
    తిరిగి apply_hsl_blend (src, dst, బహుళకం);

  అప్రమేయ:
    తిరిగి hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

గుర్తులేని
hb_raster_image_t::బైట్లు_చొప్పున_pixel (hb_raster_format_t format)
{
  తిరిగి format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

తార్కిక
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  ఒకవేళ (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  గుర్తులేని bpp = బైట్లు_చొప్పున_pixel (format);
  ఒకవేళ (extents.వెడల్పు > UINT_MAX / bpp)
    తిరిగి అబద్ధం;

  గుర్తులేని min_stride = extents.వెడల్పు * bpp;
  ఒకవేళ (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  ఒకవేళ (extents.ఎత్తు && extents.stride > (size_t) -1 / extents.ఎత్తు)
    తిరిగి అబద్ధం;

  size_t buf_కొలత = (size_t) extents.stride * extents.ఎత్తు;
  ఒకవేళ (buf_కొలత > HB_RASTER_MAX_బఫరు_కొలత)
    తిరిగి అబద్ధం;
  ఒకవేళ (unlikely (!బఫరు.resize_dirty (buf_కొలత)))
    తిరిగి అబద్ధం;

  ఇది->format = format;
  ఇది->extents = extents;
  తిరిగి నిజం;
}

తార్కిక
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#నిర్వచితం_కాకపోతే HAVE_PNG
  తిరిగి అబద్ధం;
#లేకపోతే
  ఒకవేళ (!blob)
    తిరిగి అబద్ధం;

  గుర్తులేని blob_len = 0;
  స్థిరరాశి uint8_t *blob_దత్తాంశం = (స్థిరరాశి uint8_t *) hb_blob_get_దత్తాంశం (blob, &blob_len);
  ఒకవేళ (!blob_దత్తాంశం || !blob_len)
    తిరిగి అబద్ధం;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, శూన్యసూచిక,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  ఒకవేళ (!png)
    తిరిగి అబద్ధం;

  png_infop info = png_create_info_struct (png);
  ఒకవేళ (!info)
  {
    png_destroy_read_struct (&png, శూన్యసూచిక, శూన్యసూచిక);
    తిరిగి అబద్ధం;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, పరిమాణం (*reader));
  ఒకవేళ (!reader)
  {
    png_destroy_read_struct (&png, &info, శూన్యసూచిక);
    తిరిగి అబద్ధం;
  }

  reader->దత్తాంశం = blob_దత్తాంశం;
  reader->కొలత = (size_t) blob_len;
  png_సమితి_read_fn (png, reader, hb_raster_png_read_blob);
  ఒకవేళ (setjmp (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, శూన్యసూచిక);
    hb_raster_png_read_blob_fini (reader);
    తిరిగి అబద్ధం;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  పూర్ణాంకం బిట్టు_లోతు_2 = 0, color_type = 0;
  పూర్ణాంకం interlace_type = 0, compression_type = 0, filter_పద్ధతి_2 = 0;
  png_get_IHDR (png, info, &w, &h, &బిట్టు_లోతు_2, &color_type,
		&interlace_type, &compression_type, &filter_పద్ధతి_2);

  ఒకవేళ (!w || !h || w > (png_uint_32) గరిష్ఠ_పూర్ణాంకం || h > (png_uint_32) గరిష్ఠ_పూర్ణాంకం)
  {
    png_destroy_read_struct (&png, &info, శూన్యసూచిక);
    hb_raster_png_read_blob_fini (reader);
    తిరిగి అబద్ధం;
  }

  తార్కిక has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  ఒకవేళ (బిట్టు_లోతు_2 == 16)
    png_సమితి_strip_16 (png);
  ఒకవేళ (color_type == PNG_COLOR_TYPE_PALETTE)
    png_సమితి_palette_గమ్యం_rgb (png);
  ఒకవేళ (color_type == PNG_COLOR_TYPE_బూడిద && బిట్టు_లోతు_2 < 8)
    png_సమితి_expand_బూడిద_1_2_4_గమ్యం_8 (png);
  ఒకవేళ (has_trns)
    png_సమితి_t_RNS_గమ్యం_alpha (png);
  ఒకవేళ (color_type == PNG_COLOR_TYPE_బూడిద || color_type == PNG_COLOR_TYPE_బూడిద_ALPHA)
    png_సమితి_బూడిద_గమ్యం_rgb (png);
  ఒకవేళ (!(color_type & PNG_COLOR_ముసుగు_ALPHA) && !has_trns)
    png_సమితి_కూడు_alpha (png, 0xff, PNG_FILLER_AFTER);
  ఒకవేళ (interlace_type != PNG_INTERLACE_NONE)
    png_సమితి_interlace_handling (png);

  png_read_update_info (png, info);

  ఒకవేళ (png_get_బిట్టు_లోతు (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, శూన్యసూచిక);
    hb_raster_png_read_blob_fini (reader);
    తిరిగి అబద్ధం;
  }

  png_కొలత_t rowbytes = png_get_rowbytes (png, info);
  ఒకవేళ (rowbytes < (png_కొలత_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, శూన్యసూచిక);
    hb_raster_png_read_blob_fini (reader);
    తిరిగి అబద్ధం;
  }

  size_t rgba_కొలత = (size_t) rowbytes * (size_t) h;
  ఒకవేళ ((h && rgba_కొలత / (size_t) h != (size_t) rowbytes) ||
      rgba_కొలత > HB_RASTER_MAX_బఫరు_కొలత)
  {
    png_destroy_read_struct (&png, &info, శూన్యసూచిక);
    hb_raster_png_read_blob_fini (reader);
    తిరిగి అబద్ధం;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_కొలత);
  ఒకవేళ (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, శూన్యసూచిక);
    hb_raster_png_read_blob_fini (reader);
    తిరిగి అబద్ధం;
  }

  size_t అడ్డువరుసలు_కొలత = (size_t) h * పరిమాణం (png_bytep);
  ఒకవేళ (h && అడ్డువరుసలు_కొలత / (size_t) h != పరిమాణం (png_bytep))
  {
    png_destroy_read_struct (&png, &info, శూన్యసూచిక);
    hb_raster_png_read_blob_fini (reader);
    తిరిగి అబద్ధం;
  }

  reader->అడ్డువరుసలు = (png_bytep *) hb_malloc (అడ్డువరుసలు_కొలత);
  ఒకవేళ (!reader->అడ్డువరుసలు)
  {
    png_destroy_read_struct (&png, &info, శూన్యసూచిక);
    hb_raster_png_read_blob_fini (reader);
    తిరిగి అబద్ధం;
  }

  కొరకు (గుర్తులేని y = 0; y < (గుర్తులేని) h; y++)
    reader->అడ్డువరుసలు[y] = (png_bytep) (reader->rgba + (size_t) y * (size_t) rowbytes);

  png_read_image (png, reader->అడ్డువరుసలు);
  png_read_చివర (png, శూన్యసూచిక);
  png_destroy_read_struct (&png, &info, శూన్యసూచిక);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (గుర్తులేని) w, (గుర్తులేని) h, 0};
  ఒకవేళ (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    తిరిగి అబద్ధం;
  }

  కొరకు (గుర్తులేని y = 0; y < (గుర్తులేని) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.బఫరు.arrayZ + (size_t) ((గుర్తులేని) h - 1 - y) * decoded.extents.stride);
    స్థిరరాశి uint8_t *src = reader->rgba + (size_t) y * (size_t) rowbytes;
    కొరకు (గుర్తులేని x = 0; x < (గుర్తులేని) w; x++)
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

  hb_మార్పిడి (బఫరు, decoded.బఫరు);
  hb_మార్పిడి (ఇది->extents, decoded.extents);
  hb_మార్పిడి (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  తిరిగి నిజం;
#అయితే_ముగింపు
}

hb_blob_t *
hb_raster_image_t::serialize_గమ్యం_png_or_fail () స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_PNG
  తిరిగి శూన్యసూచిక;
#లేకపోతే
  ఒకవేళ (format != HB_RASTER_FORMAT_BGRA32 || !extents.వెడల్పు || !extents.ఎత్తు)
    తిరిగి శూన్యసూచిక;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, శూన్యసూచిక,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  ఒకవేళ (!png)
    తిరిగి శూన్యసూచిక;

  png_infop info = png_create_info_struct (png);
  ఒకవేళ (!info)
  {
    png_destroy_write_struct (&png, శూన్యసూచిక);
    తిరిగి శూన్యసూచిక;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, పరిమాణం (*writer));
  ఒకవేళ (!writer)
  {
    png_destroy_write_struct (&png, &info);
    తిరిగి శూన్యసూచిక;
  }

  png_సమితి_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  ఒకవేళ (setjmp (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    తిరిగి శూన్యసూచిక;
  }

  png_సమితి_IHDR (png, info,
		extents.వెడల్పు, extents.ఎత్తు,
		8, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  size_t rowbytes = (size_t) extents.వెడల్పు * 4u;
  size_t rgba_కొలత = rowbytes * (size_t) extents.ఎత్తు;
  ఒకవేళ (extents.ఎత్తు && rgba_కొలత / (size_t) extents.ఎత్తు != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    తిరిగి శూన్యసూచిక;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_కొలత);
  ఒకవేళ (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    తిరిగి శూన్యసూచిక;
  }

  size_t అడ్డువరుసలు_కొలత = (size_t) extents.ఎత్తు * పరిమాణం (png_bytep);
  ఒకవేళ (extents.ఎత్తు && అడ్డువరుసలు_కొలత / (size_t) extents.ఎత్తు != పరిమాణం (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    తిరిగి శూన్యసూచిక;
  }

  writer->అడ్డువరుసలు = (png_bytep *) hb_malloc (అడ్డువరుసలు_కొలత);
  ఒకవేళ (!writer->అడ్డువరుసలు)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    తిరిగి శూన్యసూచిక;
  }

  కొరకు (గుర్తులేని y = 0; y < extents.ఎత్తు; y++)
  {
    uint8_t *dst = writer->rgba + (size_t) y * rowbytes;
    స్థిరరాశి uint8_t *src = బఫరు.arrayZ + (size_t) (extents.ఎత్తు - 1 - y) * extents.stride;

    కొరకు (గుర్తులేని x = 0; x < extents.వెడల్పు; x++)
    {
      uint32_t px;
      hb_memcpy (&px, src + x * 4, 4);
      uint8_t b = (uint8_t) (px & 0xFF);
      uint8_t g = (uint8_t) ((px >> 8) & 0xFF);
      uint8_t r = (uint8_t) ((px >> 16) & 0xFF);
      uint8_t a = (uint8_t) (px >> 24);

      dst[4 * x + 3] = a;
      ఒకవేళ (a)
      {
	dst[4 * x + 0] = (uint8_t) hb_min (255u, ((గుర్తులేని) r * 255u + a / 2u) / a);
	dst[4 * x + 1] = (uint8_t) hb_min (255u, ((గుర్తులేని) g * 255u + a / 2u) / a);
	dst[4 * x + 2] = (uint8_t) hb_min (255u, ((గుర్తులేని) b * 255u + a / 2u) / a);
      }
      లేకపోతే
	dst[4 * x + 0] = dst[4 * x + 1] = dst[4 * x + 2] = 0;
    }

    writer->అడ్డువరుసలు[y] = (png_bytep) dst;
  }

  png_write_image (png, writer->అడ్డువరుసలు);
  png_write_చివర (png, info);
  png_destroy_write_struct (&png, &info);

  ఒకవేళ (writer->నిడివి > (size_t) (గుర్తులేని) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    తిరిగి శూన్యసూచిక;
  }

  గుర్తులేని నిడివి = (గుర్తులేని) writer->నిడివి;
  అక్షరం *దత్తాంశం = writer->దత్తాంశం;
  writer->దత్తాంశం = శూన్యసూచిక;
  hb_raster_png_write_blob_fini (writer);
  ఒకవేళ (!దత్తాంశం && నిడివి)
    తిరిగి శూన్యసూచిక;

  hb_blob_t *blob = hb_blob_create_or_fail (దత్తాంశం, నిడివి,
					    HB_జ్ఞాపకం_బహుళకం_WRITABLE,
					    దత్తాంశం, hb_free);
  ఒకవేళ (!blob)
    hb_free (దత్తాంశం);
  తిరిగి blob;
#అయితే_ముగింపు
}

శూన్య
hb_raster_image_t::clear ()
{
  size_t buf_కొలత = (size_t) extents.stride * extents.ఎత్తు;
  hb_memset (బఫరు.arrayZ, 0, buf_కొలత);
}

స్థిరరాశి uint8_t *
hb_raster_image_t::get_బఫరు () స్థిరరాశి
{
  తిరిగి బఫరు.arrayZ;
}

శూన్య
hb_raster_image_t::సంయుక్త_from (స్థిరరాశి hb_raster_image_t *src,
				   hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
{
  గుర్తులేని w = extents.వెడల్పు;
  గుర్తులేని h = extents.ఎత్తు;
  గుర్తులేని stride = extents.stride;

  కొరకు (గుర్తులేని y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (బఫరు.arrayZ + y * stride);
    స్థిరరాశి hb_packed_t<uint32_t> *sp = (స్థిరరాశి hb_packed_t<uint32_t> *) (src->బఫరు.arrayZ + y * stride);
    కొరకు (గుర్తులేని x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (సంయుక్త_pixel ((uint32_t) sp[x], (uint32_t) dp[x], బహుళకం));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
శూన్య
hb_raster_image_సంయుక్త (hb_raster_image_t *dst,
			   స్థిరరాశి hb_raster_image_t *src,
			   hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
{
  dst->సంయుక్త_from (src, బహుళకం);
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
hb_raster_image_create_or_fail (శూన్య)
{
  తిరిగి hb_object_create<hb_raster_image_t> ();
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
  తిరిగి hb_object_reference (image);
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
శూన్య
hb_raster_image_destroy (hb_raster_image_t *image)
{
  ఒకవేళ (!hb_object_destroy (image)) తిరిగి;
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
hb_raster_image_సమితి_user_దత్తాంశం (hb_raster_image_t  *image,
			       hb_user_దత్తాంశం_కీలకం_t *key,
			       శూన్య               *దత్తాంశం,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (image, key, దత్తాంశం, destroy, replace);
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
శూన్య *
hb_raster_image_get_user_దత్తాంశం (స్థిరరాశి hb_raster_image_t  *image,
			       hb_user_దత్తాంశం_కీలకం_t *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (image, key);
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
			   స్థిరరాశి hb_raster_extents_t *extents)
{
  ఒకవేళ (unlikely (!extents))
  {
    image->extents = {};
    image->బఫరు.resize_exact (0);
    తిరిగి నిజం;
  }
  తిరిగి image->configure (format, *extents);
}

/**
 * hb_raster_image_clear:
 * @image: a raster image
 *
 * Clears @image pixels to zero while keeping current extents and format.
 *
 * Since: 13.0.0
 **/
శూన్య
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
స్థిరరాశి uint8_t *
hb_raster_image_get_బఫరు (స్థిరరాశి hb_raster_image_t *image)
{
  తిరిగి image->get_బఫరు ();
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
శూన్య
hb_raster_image_get_extents (స్థిరరాశి hb_raster_image_t   *image,
			     hb_raster_extents_t *extents)
{
  ఒకవేళ (extents)
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
hb_raster_image_get_format (స్థిరరాశి hb_raster_image_t *image)
{
  తిరిగి image->format;
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
  తిరిగి image->deserialize_from_png (png);
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
hb_raster_image_serialize_గమ్యం_png_or_fail (స్థిరరాశి hb_raster_image_t *image)
{
  తిరిగి image->serialize_గమ్యం_png_or_fail ();
}
