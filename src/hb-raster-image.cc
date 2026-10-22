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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-raster-image.hh"

#অন্তর্ভাব <math.h>

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
#অন্তর্ভাব <png.h>
#endif

#ifdef HAVE_PNG
কাঠামো hb_raster_png_read_blob_t
{
  ধ্রুবক uint8_t *তথ্য = nullptr;
  size_t আকার_2 = 0;
  size_t offset = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

স্থির শূন্য
hb_raster_png_error (png_structp png,
		     png_const_charp বার্তা HB_UNUSED)
{
#ifdef PNG_SETJMP_SUPPORTED
  longjmp (png_jmpbuf (png), 1);
#endif
}

স্থির শূন্য
hb_raster_png_warning (png_structp png HB_UNUSED,
		       png_const_charp বার্তা HB_UNUSED)
{}

স্থির শূন্য
hb_raster_png_read_blob (png_structp png, png_bytep out, png_আকার_t দৈর্ঘ্য)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  যদি (!r || !r->তথ্য || দৈর্ঘ্য > r->আকার_2 - r->offset)
    png_error (png, "read error");

  hb_memcpy (out, r->তথ্য + r->offset, দৈর্ঘ্য);
  r->offset += দৈর্ঘ্য;
}

স্থির শূন্য
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  যদি (!r)
    ফেরত;

  hb_free (r->rgba);
  hb_free (r->rows);
  hb_free (r);
}

কাঠামো hb_raster_png_write_blob_t
{
  অক্ষর *তথ্য = nullptr;
  size_t দৈর্ঘ্য = 0;
  size_t allocated = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

স্থির শূন্য
hb_raster_png_write_blob (png_structp png, png_bytep in, png_আকার_t দৈর্ঘ্য)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  যদি (!w)
    png_error (png, "write error");

  size_t old_দৈর্ঘ্য = w->দৈর্ঘ্য;
  যদি ((size_t) দৈর্ঘ্য > (size_t) -1 - old_দৈর্ঘ্য)
    png_error (png, "write error");

  size_t new_দৈর্ঘ্য = old_দৈর্ঘ্য + দৈর্ঘ্য;
  যদি (new_দৈর্ঘ্য > w->allocated)
  {
    size_t new_allocated = w->allocated ? w->allocated : 4096;
    যখন (new_allocated < new_দৈর্ঘ্য)
    {
      size_t next = new_allocated * 2;
      যদি (next <= new_allocated)
      {
	new_allocated = new_দৈর্ঘ্য;
	বিরতি;
      }
      new_allocated = next;
    }

    অক্ষর *তথ্য = (অক্ষর *) hb_realloc (w->তথ্য, new_allocated);
    যদি (!তথ্য)
      png_error (png, "write error");
    w->তথ্য = তথ্য;
    w->allocated = new_allocated;
  }

  hb_memcpy (w->তথ্য + old_দৈর্ঘ্য, in, দৈর্ঘ্য);
  w->দৈর্ঘ্য = new_দৈর্ঘ্য;
}

স্থির শূন্য
hb_raster_png_flush_blob (png_structp png HB_UNUSED)
{}

স্থির শূন্য
hb_raster_png_write_blob_fini (hb_raster_png_write_blob_t *w)
{
  যদি (!w)
    ফেরত;

  hb_free (w->তথ্য);
  hb_free (w->rgba);
  hb_free (w->rows);
  hb_free (w);
}
#endif


/*
 * Image compositing
 */

/* Unpack premultiplied pixel to float RGBA [0,1]. */
স্থির অন্তঃরেখ শূন্য
unpack_to_float (uint32_t px, দশমিক &r, দশমিক &g, দশমিক &b, দশমিক &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
স্থির অন্তঃরেখ uint32_t
pack_from_float (দশমিক r, দশমিক g, দশমিক b, দশমিক a)
{
  ফেরত hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
স্থির অন্তঃরেখ দশমিক
blend_multiply (দশমিক sc, দশমিক dc) { ফেরত sc * dc; }
স্থির অন্তঃরেখ দশমিক
blend_screen (দশমিক sc, দশমিক dc) { ফেরত sc + dc - sc * dc; }
স্থির অন্তঃরেখ দশমিক
blend_overlay (দশমিক sc, দশমিক dc)
{ ফেরত dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
স্থির অন্তঃরেখ দশমিক
blend_darken (দশমিক sc, দশমিক dc) { ফেরত hb_min (sc, dc); }
স্থির অন্তঃরেখ দশমিক
blend_lighten (দশমিক sc, দশমিক dc) { ফেরত hb_max (sc, dc); }
স্থির অন্তঃরেখ দশমিক
blend_color_dodge (দশমিক sc, দশমিক dc)
{
  যদি (dc <= 0.f) ফেরত 0.f;
  যদি (sc >= 1.f) ফেরত 1.f;
  ফেরত hb_min (1.f, dc / (1.f - sc));
}
স্থির অন্তঃরেখ দশমিক
blend_color_burn (দশমিক sc, দশমিক dc)
{
  যদি (dc >= 1.f) ফেরত 1.f;
  যদি (sc <= 0.f) ফেরত 0.f;
  ফেরত 1.f - hb_min (1.f, (1.f - dc) / sc);
}
স্থির অন্তঃরেখ দশমিক
blend_hard_light (দশমিক sc, দশমিক dc)
{ ফেরত sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
স্থির অন্তঃরেখ দশমিক
blend_soft_light (দশমিক sc, দশমিক dc)
{
  যদি (sc <= 0.5f)
    ফেরত dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  দশমিক d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  ফেরত dc + (2.f * sc - 1.f) * (d - dc);
}
স্থির অন্তঃরেখ দশমিক
blend_difference (দশমিক sc, দশমিক dc) { ফেরত fabsf (sc - dc); }
স্থির অন্তঃরেখ দশমিক
blend_exclusion (দশমিক sc, দশমিক dc) { ফেরত sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
স্থির অন্তঃরেখ uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       দশমিক (*blend_fn)(দশমিক, দশমিক))
{
  দশমিক sr, sg, sb, sa;
  দশমিক dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  দশমিক usr = sa > 0.f ? sr / sa : 0.f;
  দশমিক usg = sa > 0.f ? sg / sa : 0.f;
  দশমিক usb = sa > 0.f ? sb / sa : 0.f;
  দশমিক udr = da > 0.f ? dr / da : 0.f;
  দশমিক udg = da > 0.f ? dg / da : 0.f;
  দশমিক udb = da > 0.f ? db / da : 0.f;

  দশমিক br = blend_fn (usr, udr);
  দশমিক bg = blend_fn (usg, udg);
  দশমিক bb = blend_fn (usb, udb);

  দশমিক ra = sa + da - sa * da;
  দশমিক rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  দশমিক rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  দশমিক rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  ফেরত pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
স্থির অন্তঃরেখ দশমিক
hsl_luminosity (দশমিক r, দশমিক g, দশমিক b)
{ ফেরত 0.299f * r + 0.587f * g + 0.114f * b; }

স্থির অন্তঃরেখ দশমিক
hsl_saturation (দশমিক r, দশমিক g, দশমিক b)
{ ফেরত hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

স্থির অন্তঃরেখ শূন্য
hsl_clip_color (দশমিক &r, দশমিক &g, দশমিক &b)
{
  দশমিক l = hsl_luminosity (r, g, b);
  দশমিক mn = hb_min (hb_min (r, g), b);
  দশমিক mx = hb_max (hb_max (r, g), b);
  যদি (mn < 0.f)
  {
    দশমিক d = l - mn;
    যদি (d > 0.f) { r = l + (r - l) * l / d; g = l + (g - l) * l / d; b = l + (b - l) * l / d; }
  }
  যদি (mx > 1.f)
  {
    দশমিক d = mx - l;
    যদি (d > 0.f) { r = l + (r - l) * (1.f - l) / d; g = l + (g - l) * (1.f - l) / d; b = l + (b - l) * (1.f - l) / d; }
  }
}

স্থির অন্তঃরেখ শূন্য
hsl_set_luminosity (দশমিক &r, দশমিক &g, দশমিক &b, দশমিক l)
{
  দশমিক d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

স্থির অন্তঃরেখ শূন্য
hsl_set_saturation_inner (দশমিক &mn, দশমিক &mid, দশমিক &mx, দশমিক s)
{
  যদি (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  অন্যথা
    mid = mx = 0.f;
  mn = 0.f;
}

স্থির অন্তঃরেখ শূন্য
hsl_set_saturation (দশমিক &r, দশমিক &g, দশমিক &b, দশমিক s)
{
  যদি (r <= g)
  {
    যদি (g <= b)      hsl_set_saturation_inner (r, g, b, s);
    অন্যথা যদি (r <= b) hsl_set_saturation_inner (r, b, g, s);
    অন্যথা             hsl_set_saturation_inner (b, r, g, s);
  }
  অন্যথা
  {
    যদি (r <= b)      hsl_set_saturation_inner (g, r, b, s);
    অন্যথা যদি (g <= b) hsl_set_saturation_inner (g, b, r, s);
    অন্যথা             hsl_set_saturation_inner (b, g, r, s);
  }
}

স্থির অন্তঃরেখ uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  দশমিক sr, sg, sb, sa;
  দশমিক dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  দশমিক usr = sa > 0.f ? sr / sa : 0.f;
  দশমিক usg = sa > 0.f ? sg / sa : 0.f;
  দশমিক usb = sa > 0.f ? sb / sa : 0.f;
  দশমিক udr = da > 0.f ? dr / da : 0.f;
  দশমিক udg = da > 0.f ? dg / da : 0.f;
  দশমিক udb = da > 0.f ? db / da : 0.f;

  দশমিক br = udr, bg = udg, bb = udb;

  যদি (mode == HB_PAINT_COMPOSITE_MODE_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  অন্যথা যদি (mode == HB_PAINT_COMPOSITE_MODE_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  অন্যথা যদি (mode == HB_PAINT_COMPOSITE_MODE_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  অন্যথা /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  দশমিক ra = sa + da - sa * da;
  দশমিক rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  দশমিক rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  দশমিক rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  ফেরত pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
স্থির অন্তঃরেখ uint32_t
composite_pixel (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  নির্বাচন (mode)
  {
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_CLEAR:
    ফেরত 0;
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC:
    ফেরত src;
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST:
    ফেরত dst;
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_OVER:
    ফেরত hb_raster_src_over (src, dst);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    ফেরত hb_raster_src_over (dst, src);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_IN:
    ফেরত hb_raster_alpha_mul (src, da);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_IN:
    ফেরত hb_raster_alpha_mul (dst, sa);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    ফেরত hb_raster_alpha_mul (src, 255 - da);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    ফেরত hb_raster_alpha_mul (dst, 255 - sa);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    ফেরত hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    ফেরত hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (a >> 24) + (b >> 24));
    ফেরত hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (unsigned) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (unsigned) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (unsigned) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (unsigned) (src >> 24) + (dst >> 24));
    ফেরত hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_MULTIPLY:  ফেরত apply_separable_blend (src, dst, blend_multiply);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SCREEN:    ফেরত apply_separable_blend (src, dst, blend_screen);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_OVERLAY:   ফেরত apply_separable_blend (src, dst, blend_overlay);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DARKEN:    ফেরত apply_separable_blend (src, dst, blend_darken);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_LIGHTEN:   ফেরত apply_separable_blend (src, dst, blend_lighten);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: ফেরত apply_separable_blend (src, dst, blend_color_dodge);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_COLOR_BURN:  ফেরত apply_separable_blend (src, dst, blend_color_burn);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:  ফেরত apply_separable_blend (src, dst, blend_hard_light);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:  ফেরত apply_separable_blend (src, dst, blend_soft_light);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DIFFERENCE:  ফেরত apply_separable_blend (src, dst, blend_difference);
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_EXCLUSION:   ফেরত apply_separable_blend (src, dst, blend_exclusion);

  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_HUE:
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
  ক্ষেত্র HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
    ফেরত apply_hsl_blend (src, dst, mode);

  পূর্বনির্ধারিত:
    ফেরত hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

unsigned
hb_raster_image_t::bytes_per_pixel (hb_raster_format_t format)
{
  ফেরত format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

যৌক্তিক
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  যদি (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  unsigned bpp = bytes_per_pixel (format);
  যদি (extents.width > UINT_MAX / bpp)
    ফেরত মিথ্যা;

  unsigned min_stride = extents.width * bpp;
  যদি (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  যদি (extents.height && extents.stride > (size_t) -1 / extents.height)
    ফেরত মিথ্যা;

  size_t buf_আকার = (size_t) extents.stride * extents.height;
  যদি (buf_আকার > HB_RASTER_MAX_বাফার_আকার)
    ফেরত মিথ্যা;
  যদি (unlikely (!বাফার.resize_dirty (buf_আকার)))
    ফেরত মিথ্যা;

  এই->format = format;
  এই->extents = extents;
  ফেরত সত্য;
}

যৌক্তিক
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#ifndef HAVE_PNG
  ফেরত মিথ্যা;
#অন্যথা
  যদি (!blob)
    ফেরত মিথ্যা;

  unsigned blob_len = 0;
  ধ্রুবক uint8_t *blob_তথ্য = (ধ্রুবক uint8_t *) hb_blob_get_তথ্য (blob, &blob_len);
  যদি (!blob_তথ্য || !blob_len)
    ফেরত মিথ্যা;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, nullptr,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  যদি (!png)
    ফেরত মিথ্যা;

  png_infop info = png_create_info_struct (png);
  যদি (!info)
  {
    png_destroy_read_struct (&png, nullptr, nullptr);
    ফেরত মিথ্যা;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, আকার (*reader));
  যদি (!reader)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    ফেরত মিথ্যা;
  }

  reader->তথ্য = blob_তথ্য;
  reader->আকার_2 = (size_t) blob_len;
  png_set_read_fn (png, reader, hb_raster_png_read_blob);
  যদি (setjmp (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    ফেরত মিথ্যা;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  পূর্ণসংখ্যা bit_depth = 0, color_type = 0;
  পূর্ণসংখ্যা interlace_type = 0, compression_type = 0, filter_method = 0;
  png_get_IHDR (png, info, &w, &h, &bit_depth, &color_type,
		&interlace_type, &compression_type, &filter_method);

  যদি (!w || !h || w > (png_uint_32) INT_MAX || h > (png_uint_32) INT_MAX)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    ফেরত মিথ্যা;
  }

  যৌক্তিক has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  যদি (bit_depth == 16)
    png_set_strip_16 (png);
  যদি (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png);
  যদি (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8 (png);
  যদি (has_trns)
    png_set_tRNS_to_alpha (png);
  যদি (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb (png);
  যদি (!(color_type & PNG_COLOR_MASK_ALPHA) && !has_trns)
    png_set_add_alpha (png, 0xff, PNG_FILLER_AFTER);
  যদি (interlace_type != PNG_INTERLACE_NONE)
    png_set_interlace_handling (png);

  png_read_update_info (png, info);

  যদি (png_get_bit_depth (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    ফেরত মিথ্যা;
  }

  png_আকার_t rowbytes = png_get_rowbytes (png, info);
  যদি (rowbytes < (png_আকার_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    ফেরত মিথ্যা;
  }

  size_t rgba_আকার = (size_t) rowbytes * (size_t) h;
  যদি ((h && rgba_আকার / (size_t) h != (size_t) rowbytes) ||
      rgba_আকার > HB_RASTER_MAX_বাফার_আকার)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    ফেরত মিথ্যা;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_আকার);
  যদি (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    ফেরত মিথ্যা;
  }

  size_t rows_আকার = (size_t) h * আকার (png_bytep);
  যদি (h && rows_আকার / (size_t) h != আকার (png_bytep))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    ফেরত মিথ্যা;
  }

  reader->rows = (png_bytep *) hb_malloc (rows_আকার);
  যদি (!reader->rows)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    ফেরত মিথ্যা;
  }

  জন্য (unsigned y = 0; y < (unsigned) h; y++)
    reader->rows[y] = (png_bytep) (reader->rgba + (size_t) y * (size_t) rowbytes);

  png_read_image (png, reader->rows);
  png_read_end (png, nullptr);
  png_destroy_read_struct (&png, &info, nullptr);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (unsigned) w, (unsigned) h, 0};
  যদি (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    ফেরত মিথ্যা;
  }

  জন্য (unsigned y = 0; y < (unsigned) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.বাফার.arrayZ + (size_t) ((unsigned) h - 1 - y) * decoded.extents.stride);
    ধ্রুবক uint8_t *src = reader->rgba + (size_t) y * (size_t) rowbytes;
    জন্য (unsigned x = 0; x < (unsigned) w; x++)
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

  hb_swap (বাফার, decoded.বাফার);
  hb_swap (এই->extents, decoded.extents);
  hb_swap (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  ফেরত সত্য;
#endif
}

hb_blob_t *
hb_raster_image_t::serialize_to_png_or_fail () ধ্রুবক
{
#ifndef HAVE_PNG
  ফেরত nullptr;
#অন্যথা
  যদি (format != HB_RASTER_FORMAT_BGRA32 || !extents.width || !extents.height)
    ফেরত nullptr;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, nullptr,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  যদি (!png)
    ফেরত nullptr;

  png_infop info = png_create_info_struct (png);
  যদি (!info)
  {
    png_destroy_write_struct (&png, nullptr);
    ফেরত nullptr;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, আকার (*writer));
  যদি (!writer)
  {
    png_destroy_write_struct (&png, &info);
    ফেরত nullptr;
  }

  png_set_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  যদি (setjmp (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    ফেরত nullptr;
  }

  png_set_IHDR (png, info,
		extents.width, extents.height,
		8, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  size_t rowbytes = (size_t) extents.width * 4u;
  size_t rgba_আকার = rowbytes * (size_t) extents.height;
  যদি (extents.height && rgba_আকার / (size_t) extents.height != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    ফেরত nullptr;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_আকার);
  যদি (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    ফেরত nullptr;
  }

  size_t rows_আকার = (size_t) extents.height * আকার (png_bytep);
  যদি (extents.height && rows_আকার / (size_t) extents.height != আকার (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    ফেরত nullptr;
  }

  writer->rows = (png_bytep *) hb_malloc (rows_আকার);
  যদি (!writer->rows)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    ফেরত nullptr;
  }

  জন্য (unsigned y = 0; y < extents.height; y++)
  {
    uint8_t *dst = writer->rgba + (size_t) y * rowbytes;
    ধ্রুবক uint8_t *src = বাফার.arrayZ + (size_t) (extents.height - 1 - y) * extents.stride;

    জন্য (unsigned x = 0; x < extents.width; x++)
    {
      uint32_t px;
      hb_memcpy (&px, src + x * 4, 4);
      uint8_t b = (uint8_t) (px & 0xFF);
      uint8_t g = (uint8_t) ((px >> 8) & 0xFF);
      uint8_t r = (uint8_t) ((px >> 16) & 0xFF);
      uint8_t a = (uint8_t) (px >> 24);

      dst[4 * x + 3] = a;
      যদি (a)
      {
	dst[4 * x + 0] = (uint8_t) hb_min (255u, ((unsigned) r * 255u + a / 2u) / a);
	dst[4 * x + 1] = (uint8_t) hb_min (255u, ((unsigned) g * 255u + a / 2u) / a);
	dst[4 * x + 2] = (uint8_t) hb_min (255u, ((unsigned) b * 255u + a / 2u) / a);
      }
      অন্যথা
	dst[4 * x + 0] = dst[4 * x + 1] = dst[4 * x + 2] = 0;
    }

    writer->rows[y] = (png_bytep) dst;
  }

  png_write_image (png, writer->rows);
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);

  যদি (writer->দৈর্ঘ্য > (size_t) (unsigned) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    ফেরত nullptr;
  }

  unsigned দৈর্ঘ্য = (unsigned) writer->দৈর্ঘ্য;
  অক্ষর *তথ্য = writer->তথ্য;
  writer->তথ্য = nullptr;
  hb_raster_png_write_blob_fini (writer);
  যদি (!তথ্য && দৈর্ঘ্য)
    ফেরত nullptr;

  hb_blob_t *blob = hb_blob_create_or_fail (তথ্য, দৈর্ঘ্য,
					    HB_MEMORY_MODE_WRITABLE,
					    তথ্য, hb_free);
  যদি (!blob)
    hb_free (তথ্য);
  ফেরত blob;
#endif
}

শূন্য
hb_raster_image_t::clear ()
{
  size_t buf_আকার = (size_t) extents.stride * extents.height;
  hb_memset (বাফার.arrayZ, 0, buf_আকার);
}

ধ্রুবক uint8_t *
hb_raster_image_t::get_বাফার () ধ্রুবক
{
  ফেরত বাফার.arrayZ;
}

শূন্য
hb_raster_image_t::composite_from (ধ্রুবক hb_raster_image_t *src,
				   hb_paint_composite_mode_t mode)
{
  unsigned w = extents.width;
  unsigned h = extents.height;
  unsigned stride = extents.stride;

  জন্য (unsigned y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (বাফার.arrayZ + y * stride);
    ধ্রুবক hb_packed_t<uint32_t> *sp = (ধ্রুবক hb_packed_t<uint32_t> *) (src->বাফার.arrayZ + y * stride);
    জন্য (unsigned x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (composite_pixel ((uint32_t) sp[x], (uint32_t) dp[x], mode));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
শূন্য
hb_raster_image_composite (hb_raster_image_t *dst,
			   ধ্রুবক hb_raster_image_t *src,
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
hb_raster_image_create_or_fail (শূন্য)
{
  ফেরত hb_object_create<hb_raster_image_t> ();
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
  ফেরত hb_object_reference (image);
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
শূন্য
hb_raster_image_destroy (hb_raster_image_t *image)
{
  যদি (!hb_object_destroy (image)) ফেরত;
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
hb_raster_image_set_user_তথ্য (hb_raster_image_t  *image,
			       hb_user_তথ্য_চাবি_t *key,
			       শূন্য               *তথ্য,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  ফেরত hb_object_set_user_তথ্য (image, key, তথ্য, destroy, replace);
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
শূন্য *
hb_raster_image_get_user_তথ্য (ধ্রুবক hb_raster_image_t  *image,
			       hb_user_তথ্য_চাবি_t *key)
{
  ফেরত hb_object_get_user_তথ্য (image, key);
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
			   ধ্রুবক hb_raster_extents_t *extents)
{
  যদি (unlikely (!extents))
  {
    image->extents = {};
    image->বাফার.resize_exact (0);
    ফেরত সত্য;
  }
  ফেরত image->configure (format, *extents);
}

/**
 * hb_raster_image_clear:
 * @image: a raster image
 *
 * Clears @image pixels to zero while keeping current extents and format.
 *
 * Since: 13.0.0
 **/
শূন্য
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
ধ্রুবক uint8_t *
hb_raster_image_get_বাফার (ধ্রুবক hb_raster_image_t *image)
{
  ফেরত image->get_বাফার ();
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
শূন্য
hb_raster_image_get_extents (ধ্রুবক hb_raster_image_t   *image,
			     hb_raster_extents_t *extents)
{
  যদি (extents)
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
hb_raster_image_get_format (ধ্রুবক hb_raster_image_t *image)
{
  ফেরত image->format;
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
  ফেরত image->deserialize_from_png (png);
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
hb_raster_image_serialize_to_png_or_fail (ধ্রুবক hb_raster_image_t *image)
{
  ফেরত image->serialize_to_png_or_fail ();
}
