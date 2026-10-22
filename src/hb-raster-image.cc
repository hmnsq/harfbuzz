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
  स्थिरांक uint8_t *data = nullptr;
  माप_प्रकार size = 0;
  माप_प्रकार offset = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

स्थिर शून्य
hb_raster_png_error (png_structp png,
		     png_const_charp संदेश HB_UNUSED)
{
#ifdef PNG_SETJMP_SUPPORTED
  दीर्घ_लाँघ (png_jmpbuf (png), 1);
#endif
}

स्थिर शून्य
hb_raster_png_warning (png_structp png HB_UNUSED,
		       png_const_charp संदेश HB_UNUSED)
{}

स्थिर शून्य
hb_raster_png_read_blob (png_structp png, png_bytep out, png_size_t length)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  जर (!r || !r->data || length > r->size - r->offset)
    png_error (png, "read error");

  hb_memcpy (out, r->data + r->offset, length);
  r->offset += length;
}

स्थिर शून्य
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  जर (!r)
    परत;

  hb_free (r->rgba);
  hb_free (r->rows);
  hb_free (r);
}

संरचना hb_raster_png_write_blob_t
{
  अक्षर *data = nullptr;
  माप_प्रकार length = 0;
  माप_प्रकार allocated = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

स्थिर शून्य
hb_raster_png_write_blob (png_structp png, png_bytep in, png_size_t length)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  जर (!w)
    png_error (png, "write error");

  माप_प्रकार old_length = w->length;
  जर ((माप_प्रकार) length > (माप_प्रकार) -1 - old_length)
    png_error (png, "write error");

  माप_प्रकार new_length = old_length + length;
  जर (new_length > w->allocated)
  {
    माप_प्रकार new_allocated = w->allocated ? w->allocated : 4096;
    पर्यंत (new_allocated < new_length)
    {
      माप_प्रकार next = new_allocated * 2;
      जर (next <= new_allocated)
      {
	new_allocated = new_length;
	थांबा;
      }
      new_allocated = next;
    }

    अक्षर *data = (अक्षर *) hb_realloc (w->data, new_allocated);
    जर (!data)
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
  जर (!w)
    परत;

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
स्थिर ओळीत शून्य
unpack_to_float (uint32_t px, दशांश &r, दशांश &g, दशांश &b, दशांश &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
स्थिर ओळीत uint32_t
pack_from_float (दशांश r, दशांश g, दशांश b, दशांश a)
{
  परत hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
स्थिर ओळीत दशांश
blend_multiply (दशांश sc, दशांश dc) { परत sc * dc; }
स्थिर ओळीत दशांश
blend_screen (दशांश sc, दशांश dc) { परत sc + dc - sc * dc; }
स्थिर ओळीत दशांश
blend_overlay (दशांश sc, दशांश dc)
{ परत dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
स्थिर ओळीत दशांश
blend_darken (दशांश sc, दशांश dc) { परत hb_min (sc, dc); }
स्थिर ओळीत दशांश
blend_lighten (दशांश sc, दशांश dc) { परत hb_max (sc, dc); }
स्थिर ओळीत दशांश
blend_color_dodge (दशांश sc, दशांश dc)
{
  जर (dc <= 0.f) परत 0.f;
  जर (sc >= 1.f) परत 1.f;
  परत hb_min (1.f, dc / (1.f - sc));
}
स्थिर ओळीत दशांश
blend_color_burn (दशांश sc, दशांश dc)
{
  जर (dc >= 1.f) परत 1.f;
  जर (sc <= 0.f) परत 0.f;
  परत 1.f - hb_min (1.f, (1.f - dc) / sc);
}
स्थिर ओळीत दशांश
blend_hard_light (दशांश sc, दशांश dc)
{ परत sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
स्थिर ओळीत दशांश
blend_soft_light (दशांश sc, दशांश dc)
{
  जर (sc <= 0.5f)
    परत dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  दशांश d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  परत dc + (2.f * sc - 1.f) * (d - dc);
}
स्थिर ओळीत दशांश
blend_difference (दशांश sc, दशांश dc) { परत fabsf (sc - dc); }
स्थिर ओळीत दशांश
blend_exclusion (दशांश sc, दशांश dc) { परत sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
स्थिर ओळीत uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       दशांश (*blend_fn)(दशांश, दशांश))
{
  दशांश sr, sg, sb, sa;
  दशांश dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  दशांश usr = sa > 0.f ? sr / sa : 0.f;
  दशांश usg = sa > 0.f ? sg / sa : 0.f;
  दशांश usb = sa > 0.f ? sb / sa : 0.f;
  दशांश udr = da > 0.f ? dr / da : 0.f;
  दशांश udg = da > 0.f ? dg / da : 0.f;
  दशांश udb = da > 0.f ? db / da : 0.f;

  दशांश br = blend_fn (usr, udr);
  दशांश bg = blend_fn (usg, udg);
  दशांश bb = blend_fn (usb, udb);

  दशांश ra = sa + da - sa * da;
  दशांश rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  दशांश rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  दशांश rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  परत pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
स्थिर ओळीत दशांश
hsl_luminosity (दशांश r, दशांश g, दशांश b)
{ परत 0.299f * r + 0.587f * g + 0.114f * b; }

स्थिर ओळीत दशांश
hsl_saturation (दशांश r, दशांश g, दशांश b)
{ परत hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

स्थिर ओळीत शून्य
hsl_clip_color (दशांश &r, दशांश &g, दशांश &b)
{
  दशांश l = hsl_luminosity (r, g, b);
  दशांश mn = hb_min (hb_min (r, g), b);
  दशांश mx = hb_max (hb_max (r, g), b);
  जर (mn < 0.f)
  {
    दशांश d = l - mn;
    जर (d > 0.f) { r = l + (r - l) * l / d; g = l + (g - l) * l / d; b = l + (b - l) * l / d; }
  }
  जर (mx > 1.f)
  {
    दशांश d = mx - l;
    जर (d > 0.f) { r = l + (r - l) * (1.f - l) / d; g = l + (g - l) * (1.f - l) / d; b = l + (b - l) * (1.f - l) / d; }
  }
}

स्थिर ओळीत शून्य
hsl_set_luminosity (दशांश &r, दशांश &g, दशांश &b, दशांश l)
{
  दशांश d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

स्थिर ओळीत शून्य
hsl_set_saturation_inner (दशांश &mn, दशांश &mid, दशांश &mx, दशांश s)
{
  जर (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  अन्यथा
    mid = mx = 0.f;
  mn = 0.f;
}

स्थिर ओळीत शून्य
hsl_set_saturation (दशांश &r, दशांश &g, दशांश &b, दशांश s)
{
  जर (r <= g)
  {
    जर (g <= b)      hsl_set_saturation_inner (r, g, b, s);
    अन्यथा जर (r <= b) hsl_set_saturation_inner (r, b, g, s);
    अन्यथा             hsl_set_saturation_inner (b, r, g, s);
  }
  अन्यथा
  {
    जर (r <= b)      hsl_set_saturation_inner (g, r, b, s);
    अन्यथा जर (g <= b) hsl_set_saturation_inner (g, b, r, s);
    अन्यथा             hsl_set_saturation_inner (b, g, r, s);
  }
}

स्थिर ओळीत uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  दशांश sr, sg, sb, sa;
  दशांश dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  दशांश usr = sa > 0.f ? sr / sa : 0.f;
  दशांश usg = sa > 0.f ? sg / sa : 0.f;
  दशांश usb = sa > 0.f ? sb / sa : 0.f;
  दशांश udr = da > 0.f ? dr / da : 0.f;
  दशांश udg = da > 0.f ? dg / da : 0.f;
  दशांश udb = da > 0.f ? db / da : 0.f;

  दशांश br = udr, bg = udg, bb = udb;

  जर (mode == HB_PAINT_COMPOSITE_MODE_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  अन्यथा जर (mode == HB_PAINT_COMPOSITE_MODE_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  अन्यथा जर (mode == HB_PAINT_COMPOSITE_MODE_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  अन्यथा /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  दशांश ra = sa + da - sa * da;
  दशांश rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  दशांश rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  दशांश rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  परत pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
स्थिर ओळीत uint32_t
composite_pixel (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  निवड (mode)
  {
  स्थिती HB_PAINT_COMPOSITE_MODE_CLEAR:
    परत 0;
  स्थिती HB_PAINT_COMPOSITE_MODE_SRC:
    परत src;
  स्थिती HB_PAINT_COMPOSITE_MODE_DEST:
    परत dst;
  स्थिती HB_PAINT_COMPOSITE_MODE_SRC_OVER:
    परत hb_raster_src_over (src, dst);
  स्थिती HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    परत hb_raster_src_over (dst, src);
  स्थिती HB_PAINT_COMPOSITE_MODE_SRC_IN:
    परत hb_raster_alpha_mul (src, da);
  स्थिती HB_PAINT_COMPOSITE_MODE_DEST_IN:
    परत hb_raster_alpha_mul (dst, sa);
  स्थिती HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    परत hb_raster_alpha_mul (src, 255 - da);
  स्थिती HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    परत hb_raster_alpha_mul (dst, 255 - sa);
  स्थिती HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (a >> 24) + (b >> 24));
    परत hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  स्थिती HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (a >> 24) + (b >> 24));
    परत hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  स्थिती HB_PAINT_COMPOSITE_MODE_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (a >> 24) + (b >> 24));
    परत hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  स्थिती HB_PAINT_COMPOSITE_MODE_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (src >> 24) + (dst >> 24));
    परत hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  स्थिती HB_PAINT_COMPOSITE_MODE_MULTIPLY:  परत apply_separable_blend (src, dst, blend_multiply);
  स्थिती HB_PAINT_COMPOSITE_MODE_SCREEN:    परत apply_separable_blend (src, dst, blend_screen);
  स्थिती HB_PAINT_COMPOSITE_MODE_OVERLAY:   परत apply_separable_blend (src, dst, blend_overlay);
  स्थिती HB_PAINT_COMPOSITE_MODE_DARKEN:    परत apply_separable_blend (src, dst, blend_darken);
  स्थिती HB_PAINT_COMPOSITE_MODE_LIGHTEN:   परत apply_separable_blend (src, dst, blend_lighten);
  स्थिती HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: परत apply_separable_blend (src, dst, blend_color_dodge);
  स्थिती HB_PAINT_COMPOSITE_MODE_COLOR_BURN:  परत apply_separable_blend (src, dst, blend_color_burn);
  स्थिती HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:  परत apply_separable_blend (src, dst, blend_hard_light);
  स्थिती HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:  परत apply_separable_blend (src, dst, blend_soft_light);
  स्थिती HB_PAINT_COMPOSITE_MODE_DIFFERENCE:  परत apply_separable_blend (src, dst, blend_difference);
  स्थिती HB_PAINT_COMPOSITE_MODE_EXCLUSION:   परत apply_separable_blend (src, dst, blend_exclusion);

  स्थिती HB_PAINT_COMPOSITE_MODE_HSL_HUE:
  स्थिती HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
  स्थिती HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
  स्थिती HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
    परत apply_hsl_blend (src, dst, mode);

  पूर्वनिर्धारित:
    परत hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

अचिन्हित
hb_raster_image_t::bytes_per_pixel (hb_raster_format_t format)
{
  परत format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

तार्किक
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  जर (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  अचिन्हित bpp = bytes_per_pixel (format);
  जर (extents.width > अच_पूर्णांक_उच्च / bpp)
    परत असत्य;

  अचिन्हित min_stride = extents.width * bpp;
  जर (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  जर (extents.height && extents.stride > (माप_प्रकार) -1 / extents.height)
    परत असत्य;

  माप_प्रकार buf_size = (माप_प्रकार) extents.stride * extents.height;
  जर (buf_size > HB_RASTER_MAX_BUFFER_SIZE)
    परत असत्य;
  जर (unlikely (!buffer.resize_dirty (buf_size)))
    परत असत्य;

  हे->format = format;
  हे->extents = extents;
  परत सत्य;
}

तार्किक
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#ifndef HAVE_PNG
  परत असत्य;
#अन्यथा
  जर (!blob)
    परत असत्य;

  अचिन्हित blob_len = 0;
  स्थिरांक uint8_t *blob_data = (स्थिरांक uint8_t *) hb_blob_get_data (blob, &blob_len);
  जर (!blob_data || !blob_len)
    परत असत्य;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, nullptr,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  जर (!png)
    परत असत्य;

  png_infop info = png_create_info_struct (png);
  जर (!info)
  {
    png_destroy_read_struct (&png, nullptr, nullptr);
    परत असत्य;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, आकार (*reader));
  जर (!reader)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    परत असत्य;
  }

  reader->data = blob_data;
  reader->size = (माप_प्रकार) blob_len;
  png_set_read_fn (png, reader, hb_raster_png_read_blob);
  जर (बनाओ_लाँघ (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    परत असत्य;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  पूर्णांक bit_depth = 0, color_type = 0;
  पूर्णांक interlace_type = 0, compression_type = 0, filter_method = 0;
  png_get_IHDR (png, info, &w, &h, &bit_depth, &color_type,
		&interlace_type, &compression_type, &filter_method);

  जर (!w || !h || w > (png_uint_32) पूर्णांक_उच्च || h > (png_uint_32) पूर्णांक_उच्च)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    परत असत्य;
  }

  तार्किक has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  जर (bit_depth == 16)
    png_set_strip_16 (png);
  जर (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png);
  जर (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8 (png);
  जर (has_trns)
    png_set_tRNS_to_alpha (png);
  जर (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb (png);
  जर (!(color_type & PNG_COLOR_MASK_ALPHA) && !has_trns)
    png_set_add_alpha (png, 0xff, PNG_FILLER_AFTER);
  जर (interlace_type != PNG_INTERLACE_NONE)
    png_set_interlace_handling (png);

  png_read_update_info (png, info);

  जर (png_get_bit_depth (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    परत असत्य;
  }

  png_size_t rowbytes = png_get_rowbytes (png, info);
  जर (rowbytes < (png_size_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    परत असत्य;
  }

  माप_प्रकार rgba_size = (माप_प्रकार) rowbytes * (माप_प्रकार) h;
  जर ((h && rgba_size / (माप_प्रकार) h != (माप_प्रकार) rowbytes) ||
      rgba_size > HB_RASTER_MAX_BUFFER_SIZE)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    परत असत्य;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_size);
  जर (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    परत असत्य;
  }

  माप_प्रकार rows_size = (माप_प्रकार) h * आकार (png_bytep);
  जर (h && rows_size / (माप_प्रकार) h != आकार (png_bytep))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    परत असत्य;
  }

  reader->rows = (png_bytep *) hb_malloc (rows_size);
  जर (!reader->rows)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    परत असत्य;
  }

  साठी (अचिन्हित y = 0; y < (अचिन्हित) h; y++)
    reader->rows[y] = (png_bytep) (reader->rgba + (माप_प्रकार) y * (माप_प्रकार) rowbytes);

  png_read_image (png, reader->rows);
  png_read_end (png, nullptr);
  png_destroy_read_struct (&png, &info, nullptr);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (अचिन्हित) w, (अचिन्हित) h, 0};
  जर (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    परत असत्य;
  }

  साठी (अचिन्हित y = 0; y < (अचिन्हित) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.buffer.arrayZ + (माप_प्रकार) ((अचिन्हित) h - 1 - y) * decoded.extents.stride);
    स्थिरांक uint8_t *src = reader->rgba + (माप_प्रकार) y * (माप_प्रकार) rowbytes;
    साठी (अचिन्हित x = 0; x < (अचिन्हित) w; x++)
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
  hb_swap (हे->extents, decoded.extents);
  hb_swap (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  परत सत्य;
#endif
}

hb_blob_t *
hb_raster_image_t::serialize_to_png_or_fail () स्थिरांक
{
#ifndef HAVE_PNG
  परत nullptr;
#अन्यथा
  जर (format != HB_RASTER_FORMAT_BGRA32 || !extents.width || !extents.height)
    परत nullptr;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, nullptr,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  जर (!png)
    परत nullptr;

  png_infop info = png_create_info_struct (png);
  जर (!info)
  {
    png_destroy_write_struct (&png, nullptr);
    परत nullptr;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, आकार (*writer));
  जर (!writer)
  {
    png_destroy_write_struct (&png, &info);
    परत nullptr;
  }

  png_set_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  जर (बनाओ_लाँघ (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    परत nullptr;
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
  जर (extents.height && rgba_size / (माप_प्रकार) extents.height != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    परत nullptr;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_size);
  जर (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    परत nullptr;
  }

  माप_प्रकार rows_size = (माप_प्रकार) extents.height * आकार (png_bytep);
  जर (extents.height && rows_size / (माप_प्रकार) extents.height != आकार (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    परत nullptr;
  }

  writer->rows = (png_bytep *) hb_malloc (rows_size);
  जर (!writer->rows)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    परत nullptr;
  }

  साठी (अचिन्हित y = 0; y < extents.height; y++)
  {
    uint8_t *dst = writer->rgba + (माप_प्रकार) y * rowbytes;
    स्थिरांक uint8_t *src = buffer.arrayZ + (माप_प्रकार) (extents.height - 1 - y) * extents.stride;

    साठी (अचिन्हित x = 0; x < extents.width; x++)
    {
      uint32_t px;
      hb_memcpy (&px, src + x * 4, 4);
      uint8_t b = (uint8_t) (px & 0xFF);
      uint8_t g = (uint8_t) ((px >> 8) & 0xFF);
      uint8_t r = (uint8_t) ((px >> 16) & 0xFF);
      uint8_t a = (uint8_t) (px >> 24);

      dst[4 * x + 3] = a;
      जर (a)
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

  जर (writer->length > (माप_प्रकार) (अचिन्हित) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    परत nullptr;
  }

  अचिन्हित length = (अचिन्हित) writer->length;
  अक्षर *data = writer->data;
  writer->data = nullptr;
  hb_raster_png_write_blob_fini (writer);
  जर (!data && length)
    परत nullptr;

  hb_blob_t *blob = hb_blob_create_or_fail (data, length,
					    HB_MEMORY_MODE_WRITABLE,
					    data, hb_free);
  जर (!blob)
    hb_free (data);
  परत blob;
#endif
}

शून्य
hb_raster_image_t::clear ()
{
  माप_प्रकार buf_size = (माप_प्रकार) extents.stride * extents.height;
  hb_memset (buffer.arrayZ, 0, buf_size);
}

स्थिरांक uint8_t *
hb_raster_image_t::get_buffer () स्थिरांक
{
  परत buffer.arrayZ;
}

शून्य
hb_raster_image_t::composite_from (स्थिरांक hb_raster_image_t *src,
				   hb_paint_composite_mode_t mode)
{
  अचिन्हित w = extents.width;
  अचिन्हित h = extents.height;
  अचिन्हित stride = extents.stride;

  साठी (अचिन्हित y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (buffer.arrayZ + y * stride);
    स्थिरांक hb_packed_t<uint32_t> *sp = (स्थिरांक hb_packed_t<uint32_t> *) (src->buffer.arrayZ + y * stride);
    साठी (अचिन्हित x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (composite_pixel ((uint32_t) sp[x], (uint32_t) dp[x], mode));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
शून्य
hb_raster_image_composite (hb_raster_image_t *dst,
			   स्थिरांक hb_raster_image_t *src,
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
  परत hb_object_create<hb_raster_image_t> ();
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
  परत hb_object_reference (image);
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
  जर (!hb_object_destroy (image)) परत;
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
			       hb_user_data_किल्ली_t *key,
			       शून्य               *data,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  परत hb_object_set_user_data (image, key, data, destroy, replace);
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
hb_raster_image_get_user_data (स्थिरांक hb_raster_image_t  *image,
			       hb_user_data_किल्ली_t *key)
{
  परत hb_object_get_user_data (image, key);
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
			   स्थिरांक hb_raster_extents_t *extents)
{
  जर (unlikely (!extents))
  {
    image->extents = {};
    image->buffer.resize_exact (0);
    परत सत्य;
  }
  परत image->configure (format, *extents);
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
स्थिरांक uint8_t *
hb_raster_image_get_buffer (स्थिरांक hb_raster_image_t *image)
{
  परत image->get_buffer ();
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
hb_raster_image_get_extents (स्थिरांक hb_raster_image_t   *image,
			     hb_raster_extents_t *extents)
{
  जर (extents)
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
hb_raster_image_get_format (स्थिरांक hb_raster_image_t *image)
{
  परत image->format;
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
  परत image->deserialize_from_png (png);
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
hb_raster_image_serialize_to_png_or_fail (स्थिरांक hb_raster_image_t *image)
{
  परत image->serialize_to_png_or_fail ();
}
