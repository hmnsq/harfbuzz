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

#समावेश "hb.hh"

#समावेश "hb-raster-image.hh"

#समावेश <math.h>

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

#अगर_घोषित HAVE_PNG
#समावेश <png.h>
#पूर्ण_अगर

#अगर_घोषित HAVE_PNG
काष्ठा hb_raster_png_read_blob_t
{
  स्थिर uint8_t *तथ्य = nullptr;
  माप_प्रकार आकार = 0;
  माप_प्रकार offset = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

अटल व्योम
hb_raster_png_error (png_structp png,
		     png_const_charp संदेश HB_UNUSED)
{
#अगर_घोषित PNG_SETJMP_SUPPORTED
  दीर्घ_लाँघ (png_jmpbuf (png), 1);
#पूर्ण_अगर
}

अटल व्योम
hb_raster_png_warning (png_structp png HB_UNUSED,
		       png_const_charp संदेश HB_UNUSED)
{}

अटल व्योम
hb_raster_png_read_blob (png_structp png, png_bytep out, png_आकार_t लंबाई)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  अगर (!r || !r->तथ्य || लंबाई > r->आकार - r->offset)
    png_error (png, "read error");

  hb_memcpy (out, r->तथ्य + r->offset, लंबाई);
  r->offset += लंबाई;
}

अटल व्योम
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  अगर (!r)
    वापस;

  hb_free (r->rgba);
  hb_free (r->rows);
  hb_free (r);
}

काष्ठा hb_raster_png_write_blob_t
{
  अक्षर *तथ्य = nullptr;
  माप_प्रकार लंबाई = 0;
  माप_प्रकार allocated = 0;
  uint8_t *rgba = nullptr;
  png_bytep *rows = nullptr;
};

अटल व्योम
hb_raster_png_write_blob (png_structp png, png_bytep in, png_आकार_t लंबाई)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  अगर (!w)
    png_error (png, "write error");

  माप_प्रकार old_लंबाई = w->लंबाई;
  अगर ((माप_प्रकार) लंबाई > (माप_प्रकार) -1 - old_लंबाई)
    png_error (png, "write error");

  माप_प्रकार new_लंबाई = old_लंबाई + लंबाई;
  अगर (new_लंबाई > w->allocated)
  {
    माप_प्रकार new_allocated = w->allocated ? w->allocated : 4096;
    जबतक (new_allocated < new_लंबाई)
    {
      माप_प्रकार next = new_allocated * 2;
      अगर (next <= new_allocated)
      {
	new_allocated = new_लंबाई;
	अवरोध;
      }
      new_allocated = next;
    }

    अक्षर *तथ्य = (अक्षर *) hb_realloc (w->तथ्य, new_allocated);
    अगर (!तथ्य)
      png_error (png, "write error");
    w->तथ्य = तथ्य;
    w->allocated = new_allocated;
  }

  hb_memcpy (w->तथ्य + old_लंबाई, in, लंबाई);
  w->लंबाई = new_लंबाई;
}

अटल व्योम
hb_raster_png_flush_blob (png_structp png HB_UNUSED)
{}

अटल व्योम
hb_raster_png_write_blob_fini (hb_raster_png_write_blob_t *w)
{
  अगर (!w)
    वापस;

  hb_free (w->तथ्य);
  hb_free (w->rgba);
  hb_free (w->rows);
  hb_free (w);
}
#पूर्ण_अगर


/*
 * Image compositing
 */

/* Unpack premultiplied pixel to float RGBA [0,1]. */
अटल अंतरभूत व्योम
unpack_to_float (uint32_t px, भग्न &r, भग्न &g, भग्न &b, भग्न &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
अटल अंतरभूत uint32_t
pack_from_float (भग्न r, भग्न g, भग्न b, भग्न a)
{
  वापस hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
अटल अंतरभूत भग्न
blend_multiply (भग्न sc, भग्न dc) { वापस sc * dc; }
अटल अंतरभूत भग्न
blend_screen (भग्न sc, भग्न dc) { वापस sc + dc - sc * dc; }
अटल अंतरभूत भग्न
blend_overlay (भग्न sc, भग्न dc)
{ वापस dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
अटल अंतरभूत भग्न
blend_darken (भग्न sc, भग्न dc) { वापस hb_min (sc, dc); }
अटल अंतरभूत भग्न
blend_lighten (भग्न sc, भग्न dc) { वापस hb_max (sc, dc); }
अटल अंतरभूत भग्न
blend_color_dodge (भग्न sc, भग्न dc)
{
  अगर (dc <= 0.f) वापस 0.f;
  अगर (sc >= 1.f) वापस 1.f;
  वापस hb_min (1.f, dc / (1.f - sc));
}
अटल अंतरभूत भग्न
blend_color_burn (भग्न sc, भग्न dc)
{
  अगर (dc >= 1.f) वापस 1.f;
  अगर (sc <= 0.f) वापस 0.f;
  वापस 1.f - hb_min (1.f, (1.f - dc) / sc);
}
अटल अंतरभूत भग्न
blend_hard_light (भग्न sc, भग्न dc)
{ वापस sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
अटल अंतरभूत भग्न
blend_soft_light (भग्न sc, भग्न dc)
{
  अगर (sc <= 0.5f)
    वापस dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  भग्न d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  वापस dc + (2.f * sc - 1.f) * (d - dc);
}
अटल अंतरभूत भग्न
blend_difference (भग्न sc, भग्न dc) { वापस fabsf (sc - dc); }
अटल अंतरभूत भग्न
blend_exclusion (भग्न sc, भग्न dc) { वापस sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
अटल अंतरभूत uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       भग्न (*blend_fn)(भग्न, भग्न))
{
  भग्न sr, sg, sb, sa;
  भग्न dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  भग्न usr = sa > 0.f ? sr / sa : 0.f;
  भग्न usg = sa > 0.f ? sg / sa : 0.f;
  भग्न usb = sa > 0.f ? sb / sa : 0.f;
  भग्न udr = da > 0.f ? dr / da : 0.f;
  भग्न udg = da > 0.f ? dg / da : 0.f;
  भग्न udb = da > 0.f ? db / da : 0.f;

  भग्न br = blend_fn (usr, udr);
  भग्न bg = blend_fn (usg, udg);
  भग्न bb = blend_fn (usb, udb);

  भग्न ra = sa + da - sa * da;
  भग्न rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  भग्न rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  भग्न rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  वापस pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
अटल अंतरभूत भग्न
hsl_luminosity (भग्न r, भग्न g, भग्न b)
{ वापस 0.299f * r + 0.587f * g + 0.114f * b; }

अटल अंतरभूत भग्न
hsl_saturation (भग्न r, भग्न g, भग्न b)
{ वापस hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

अटल अंतरभूत व्योम
hsl_clip_color (भग्न &r, भग्न &g, भग्न &b)
{
  भग्न l = hsl_luminosity (r, g, b);
  भग्न mn = hb_min (hb_min (r, g), b);
  भग्न mx = hb_max (hb_max (r, g), b);
  अगर (mn < 0.f)
  {
    भग्न d = l - mn;
    अगर (d > 0.f) { r = l + (r - l) * l / d; g = l + (g - l) * l / d; b = l + (b - l) * l / d; }
  }
  अगर (mx > 1.f)
  {
    भग्न d = mx - l;
    अगर (d > 0.f) { r = l + (r - l) * (1.f - l) / d; g = l + (g - l) * (1.f - l) / d; b = l + (b - l) * (1.f - l) / d; }
  }
}

अटल अंतरभूत व्योम
hsl_set_luminosity (भग्न &r, भग्न &g, भग्न &b, भग्न l)
{
  भग्न d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

अटल अंतरभूत व्योम
hsl_set_saturation_inner (भग्न &mn, भग्न &mid, भग्न &mx, भग्न s)
{
  अगर (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  अन्यथा
    mid = mx = 0.f;
  mn = 0.f;
}

अटल अंतरभूत व्योम
hsl_set_saturation (भग्न &r, भग्न &g, भग्न &b, भग्न s)
{
  अगर (r <= g)
  {
    अगर (g <= b)      hsl_set_saturation_inner (r, g, b, s);
    अन्यथा अगर (r <= b) hsl_set_saturation_inner (r, b, g, s);
    अन्यथा             hsl_set_saturation_inner (b, r, g, s);
  }
  अन्यथा
  {
    अगर (r <= b)      hsl_set_saturation_inner (g, r, b, s);
    अन्यथा अगर (g <= b) hsl_set_saturation_inner (g, b, r, s);
    अन्यथा             hsl_set_saturation_inner (b, g, r, s);
  }
}

अटल अंतरभूत uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  भग्न sr, sg, sb, sa;
  भग्न dr, dg, db, da;
  unpack_to_float (src, sr, sg, sb, sa);
  unpack_to_float (dst, dr, dg, db, da);

  भग्न usr = sa > 0.f ? sr / sa : 0.f;
  भग्न usg = sa > 0.f ? sg / sa : 0.f;
  भग्न usb = sa > 0.f ? sb / sa : 0.f;
  भग्न udr = da > 0.f ? dr / da : 0.f;
  भग्न udg = da > 0.f ? dg / da : 0.f;
  भग्न udb = da > 0.f ? db / da : 0.f;

  भग्न br = udr, bg = udg, bb = udb;

  अगर (mode == HB_PAINT_COMPOSITE_MODE_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  अन्यथा अगर (mode == HB_PAINT_COMPOSITE_MODE_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  अन्यथा अगर (mode == HB_PAINT_COMPOSITE_MODE_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  अन्यथा /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_set_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  भग्न ra = sa + da - sa * da;
  भग्न rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  भग्न rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  भग्न rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  वापस pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
अटल अंतरभूत uint32_t
composite_pixel (uint32_t src, uint32_t dst,
		 hb_paint_composite_mode_t mode)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  चयन (mode)
  {
  हाल HB_PAINT_COMPOSITE_MODE_CLEAR:
    वापस 0;
  हाल HB_PAINT_COMPOSITE_MODE_SRC:
    वापस src;
  हाल HB_PAINT_COMPOSITE_MODE_DEST:
    वापस dst;
  हाल HB_PAINT_COMPOSITE_MODE_SRC_OVER:
    वापस hb_raster_src_over (src, dst);
  हाल HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    वापस hb_raster_src_over (dst, src);
  हाल HB_PAINT_COMPOSITE_MODE_SRC_IN:
    वापस hb_raster_alpha_mul (src, da);
  हाल HB_PAINT_COMPOSITE_MODE_DEST_IN:
    वापस hb_raster_alpha_mul (dst, sa);
  हाल HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    वापस hb_raster_alpha_mul (src, 255 - da);
  हाल HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    वापस hb_raster_alpha_mul (dst, 255 - sa);
  हाल HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (a >> 24) + (b >> 24));
    वापस hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  हाल HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (a >> 24) + (b >> 24));
    वापस hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  हाल HB_PAINT_COMPOSITE_MODE_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (a >> 24) + (b >> 24));
    वापस hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  हाल HB_PAINT_COMPOSITE_MODE_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (अचिन्हित) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (अचिन्हित) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (अचिन्हित) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (अचिन्हित) (src >> 24) + (dst >> 24));
    वापस hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  हाल HB_PAINT_COMPOSITE_MODE_MULTIPLY:  वापस apply_separable_blend (src, dst, blend_multiply);
  हाल HB_PAINT_COMPOSITE_MODE_SCREEN:    वापस apply_separable_blend (src, dst, blend_screen);
  हाल HB_PAINT_COMPOSITE_MODE_OVERLAY:   वापस apply_separable_blend (src, dst, blend_overlay);
  हाल HB_PAINT_COMPOSITE_MODE_DARKEN:    वापस apply_separable_blend (src, dst, blend_darken);
  हाल HB_PAINT_COMPOSITE_MODE_LIGHTEN:   वापस apply_separable_blend (src, dst, blend_lighten);
  हाल HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: वापस apply_separable_blend (src, dst, blend_color_dodge);
  हाल HB_PAINT_COMPOSITE_MODE_COLOR_BURN:  वापस apply_separable_blend (src, dst, blend_color_burn);
  हाल HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:  वापस apply_separable_blend (src, dst, blend_hard_light);
  हाल HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:  वापस apply_separable_blend (src, dst, blend_soft_light);
  हाल HB_PAINT_COMPOSITE_MODE_DIFFERENCE:  वापस apply_separable_blend (src, dst, blend_difference);
  हाल HB_PAINT_COMPOSITE_MODE_EXCLUSION:   वापस apply_separable_blend (src, dst, blend_exclusion);

  हाल HB_PAINT_COMPOSITE_MODE_HSL_HUE:
  हाल HB_PAINT_COMPOSITE_MODE_HSL_SATURATION:
  हाल HB_PAINT_COMPOSITE_MODE_HSL_COLOR:
  हाल HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY:
    वापस apply_hsl_blend (src, dst, mode);

  शेष:
    वापस hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

अचिन्हित
hb_raster_image_t::bytes_per_pixel (hb_raster_format_t format)
{
  वापस format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

bool
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  अगर (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  अचिन्हित bpp = bytes_per_pixel (format);
  अगर (extents.width > अच_पूर्णांक_उच्च / bpp)
    वापस false;

  अचिन्हित min_stride = extents.width * bpp;
  अगर (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  अगर (extents.height && extents.stride > (माप_प्रकार) -1 / extents.height)
    वापस false;

  माप_प्रकार buf_आकार = (माप_प्रकार) extents.stride * extents.height;
  अगर (buf_आकार > HB_RASTER_MAX_बफर_आकार)
    वापस false;
  अगर (unlikely (!बफर.resize_dirty (buf_आकार)))
    वापस false;

  यह->format = format;
  यह->extents = extents;
  वापस true;
}

bool
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#अगर_अघोषित HAVE_PNG
  वापस false;
#अन्यथा
  अगर (!blob)
    वापस false;

  अचिन्हित blob_len = 0;
  स्थिर uint8_t *blob_तथ्य = (स्थिर uint8_t *) hb_blob_get_तथ्य (blob, &blob_len);
  अगर (!blob_तथ्य || !blob_len)
    वापस false;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, nullptr,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  अगर (!png)
    वापस false;

  png_infop info = png_create_info_struct (png);
  अगर (!info)
  {
    png_destroy_read_struct (&png, nullptr, nullptr);
    वापस false;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, माप (*reader));
  अगर (!reader)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    वापस false;
  }

  reader->तथ्य = blob_तथ्य;
  reader->आकार = (माप_प्रकार) blob_len;
  png_set_read_fn (png, reader, hb_raster_png_read_blob);
  अगर (बनाओ_लाँघ (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    वापस false;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  पूर्णांक bit_depth = 0, color_type = 0;
  पूर्णांक interlace_type = 0, compression_type = 0, filter_method = 0;
  png_get_IHDR (png, info, &w, &h, &bit_depth, &color_type,
		&interlace_type, &compression_type, &filter_method);

  अगर (!w || !h || w > (png_uint_32) पूर्णांक_उच्च || h > (png_uint_32) पूर्णांक_उच्च)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    वापस false;
  }

  bool has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  अगर (bit_depth == 16)
    png_set_strip_16 (png);
  अगर (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png);
  अगर (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8 (png);
  अगर (has_trns)
    png_set_tRNS_to_alpha (png);
  अगर (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb (png);
  अगर (!(color_type & PNG_COLOR_MASK_ALPHA) && !has_trns)
    png_set_add_alpha (png, 0xff, PNG_FILLER_AFTER);
  अगर (interlace_type != PNG_INTERLACE_NONE)
    png_set_interlace_handling (png);

  png_read_update_info (png, info);

  अगर (png_get_bit_depth (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    वापस false;
  }

  png_आकार_t rowbytes = png_get_rowbytes (png, info);
  अगर (rowbytes < (png_आकार_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    वापस false;
  }

  माप_प्रकार rgba_आकार = (माप_प्रकार) rowbytes * (माप_प्रकार) h;
  अगर ((h && rgba_आकार / (माप_प्रकार) h != (माप_प्रकार) rowbytes) ||
      rgba_आकार > HB_RASTER_MAX_बफर_आकार)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    वापस false;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_आकार);
  अगर (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    वापस false;
  }

  माप_प्रकार rows_आकार = (माप_प्रकार) h * माप (png_bytep);
  अगर (h && rows_आकार / (माप_प्रकार) h != माप (png_bytep))
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    वापस false;
  }

  reader->rows = (png_bytep *) hb_malloc (rows_आकार);
  अगर (!reader->rows)
  {
    png_destroy_read_struct (&png, &info, nullptr);
    hb_raster_png_read_blob_fini (reader);
    वापस false;
  }

  क्रम (अचिन्हित y = 0; y < (अचिन्हित) h; y++)
    reader->rows[y] = (png_bytep) (reader->rgba + (माप_प्रकार) y * (माप_प्रकार) rowbytes);

  png_read_image (png, reader->rows);
  png_read_end (png, nullptr);
  png_destroy_read_struct (&png, &info, nullptr);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (अचिन्हित) w, (अचिन्हित) h, 0};
  अगर (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    वापस false;
  }

  क्रम (अचिन्हित y = 0; y < (अचिन्हित) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.बफर.arrayZ + (माप_प्रकार) ((अचिन्हित) h - 1 - y) * decoded.extents.stride);
    स्थिर uint8_t *src = reader->rgba + (माप_प्रकार) y * (माप_प्रकार) rowbytes;
    क्रम (अचिन्हित x = 0; x < (अचिन्हित) w; x++)
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

  hb_swap (बफर, decoded.बफर);
  hb_swap (यह->extents, decoded.extents);
  hb_swap (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  वापस true;
#पूर्ण_अगर
}

hb_blob_t *
hb_raster_image_t::serialize_to_png_or_fail () स्थिर
{
#अगर_अघोषित HAVE_PNG
  वापस nullptr;
#अन्यथा
  अगर (format != HB_RASTER_FORMAT_BGRA32 || !extents.width || !extents.height)
    वापस nullptr;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, nullptr,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  अगर (!png)
    वापस nullptr;

  png_infop info = png_create_info_struct (png);
  अगर (!info)
  {
    png_destroy_write_struct (&png, nullptr);
    वापस nullptr;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, माप (*writer));
  अगर (!writer)
  {
    png_destroy_write_struct (&png, &info);
    वापस nullptr;
  }

  png_set_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  अगर (बनाओ_लाँघ (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    वापस nullptr;
  }

  png_set_IHDR (png, info,
		extents.width, extents.height,
		8, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  माप_प्रकार rowbytes = (माप_प्रकार) extents.width * 4u;
  माप_प्रकार rgba_आकार = rowbytes * (माप_प्रकार) extents.height;
  अगर (extents.height && rgba_आकार / (माप_प्रकार) extents.height != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    वापस nullptr;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_आकार);
  अगर (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    वापस nullptr;
  }

  माप_प्रकार rows_आकार = (माप_प्रकार) extents.height * माप (png_bytep);
  अगर (extents.height && rows_आकार / (माप_प्रकार) extents.height != माप (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    वापस nullptr;
  }

  writer->rows = (png_bytep *) hb_malloc (rows_आकार);
  अगर (!writer->rows)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    वापस nullptr;
  }

  क्रम (अचिन्हित y = 0; y < extents.height; y++)
  {
    uint8_t *dst = writer->rgba + (माप_प्रकार) y * rowbytes;
    स्थिर uint8_t *src = बफर.arrayZ + (माप_प्रकार) (extents.height - 1 - y) * extents.stride;

    क्रम (अचिन्हित x = 0; x < extents.width; x++)
    {
      uint32_t px;
      hb_memcpy (&px, src + x * 4, 4);
      uint8_t b = (uint8_t) (px & 0xFF);
      uint8_t g = (uint8_t) ((px >> 8) & 0xFF);
      uint8_t r = (uint8_t) ((px >> 16) & 0xFF);
      uint8_t a = (uint8_t) (px >> 24);

      dst[4 * x + 3] = a;
      अगर (a)
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

  अगर (writer->लंबाई > (माप_प्रकार) (अचिन्हित) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    वापस nullptr;
  }

  अचिन्हित लंबाई = (अचिन्हित) writer->लंबाई;
  अक्षर *तथ्य = writer->तथ्य;
  writer->तथ्य = nullptr;
  hb_raster_png_write_blob_fini (writer);
  अगर (!तथ्य && लंबाई)
    वापस nullptr;

  hb_blob_t *blob = hb_blob_create_or_fail (तथ्य, लंबाई,
					    HB_MEMORY_MODE_WRITABLE,
					    तथ्य, hb_free);
  अगर (!blob)
    hb_free (तथ्य);
  वापस blob;
#पूर्ण_अगर
}

व्योम
hb_raster_image_t::सफा ()
{
  माप_प्रकार buf_आकार = (माप_प्रकार) extents.stride * extents.height;
  hb_memset (बफर.arrayZ, 0, buf_आकार);
}

स्थिर uint8_t *
hb_raster_image_t::get_बफर () स्थिर
{
  वापस बफर.arrayZ;
}

व्योम
hb_raster_image_t::composite_from (स्थिर hb_raster_image_t *src,
				   hb_paint_composite_mode_t mode)
{
  अचिन्हित w = extents.width;
  अचिन्हित h = extents.height;
  अचिन्हित stride = extents.stride;

  क्रम (अचिन्हित y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (बफर.arrayZ + y * stride);
    स्थिर hb_packed_t<uint32_t> *sp = (स्थिर hb_packed_t<uint32_t> *) (src->बफर.arrayZ + y * stride);
    क्रम (अचिन्हित x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (composite_pixel ((uint32_t) sp[x], (uint32_t) dp[x], mode));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
व्योम
hb_raster_image_composite (hb_raster_image_t *dst,
			   स्थिर hb_raster_image_t *src,
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
hb_raster_image_create_or_fail (व्योम)
{
  वापस hb_object_create<hb_raster_image_t> ();
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
  वापस hb_object_reference (image);
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
व्योम
hb_raster_image_destroy (hb_raster_image_t *image)
{
  अगर (!hb_object_destroy (image)) वापस;
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
hb_raster_image_set_user_तथ्य (hb_raster_image_t  *image,
			       hb_user_तथ्य_कुंजी_t *key,
			       व्योम               *तथ्य,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  वापस hb_object_set_user_तथ्य (image, key, तथ्य, destroy, replace);
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
व्योम *
hb_raster_image_get_user_तथ्य (स्थिर hb_raster_image_t  *image,
			       hb_user_तथ्य_कुंजी_t *key)
{
  वापस hb_object_get_user_तथ्य (image, key);
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
			   स्थिर hb_raster_extents_t *extents)
{
  अगर (unlikely (!extents))
  {
    image->extents = {};
    image->बफर.resize_exact (0);
    वापस true;
  }
  वापस image->configure (format, *extents);
}

/**
 * hb_raster_image_clear:
 * @image: a raster image
 *
 * Clears @image pixels to zero while keeping current extents and format.
 *
 * Since: 13.0.0
 **/
व्योम
hb_raster_image_clear (hb_raster_image_t *image)
{
  image->सफा ();
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
स्थिर uint8_t *
hb_raster_image_get_बफर (स्थिर hb_raster_image_t *image)
{
  वापस image->get_बफर ();
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
व्योम
hb_raster_image_get_extents (स्थिर hb_raster_image_t   *image,
			     hb_raster_extents_t *extents)
{
  अगर (extents)
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
hb_raster_image_get_format (स्थिर hb_raster_image_t *image)
{
  वापस image->format;
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
  वापस image->deserialize_from_png (png);
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
hb_raster_image_serialize_to_png_or_fail (स्थिर hb_raster_image_t *image)
{
  वापस image->serialize_to_png_or_fail ();
}
