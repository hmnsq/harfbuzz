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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-raster-image.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>

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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PNG
#ਅਨ੍ਤਰ੍ਭਾਵ <png.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PNG
ਬਣਤਰ hb_raster_png_read_blob_t
{
  ਅਚਲ uint8_t *ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  size_t ਮਾਪ = 0;
  size_t ਵਿੱਥ = 0;
  uint8_t *rgba = ਖਾਲੀ_ਸੂਚਕ;
  png_bytep *ਪੰਕਤੀਆਂ = ਖਾਲੀ_ਸੂਚਕ;
};

ਸਥਿਰ ਖਾਲੀ
hb_raster_png_error (png_structp png,
		     png_const_charp ਸੁਨੇਹਾ_2 HB_ਅਣਵਰਤਿਆ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ PNG_SETJMP_SUPPORTED
  longjmp (png_jmpbuf (png), 1);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_png_warning (png_structp png HB_ਅਣਵਰਤਿਆ,
		       png_const_charp ਸੁਨੇਹਾ_2 HB_ਅਣਵਰਤਿਆ)
{}

ਸਥਿਰ ਖਾਲੀ
hb_raster_png_read_blob (png_structp png, png_bytep ਬਾਹਰਲਾ, png_ਮਾਪ_t ਲੰਮਾਈ)
{
  hb_raster_png_read_blob_t *r = (hb_raster_png_read_blob_t *) png_get_io_ptr (png);

  ਜੇ (!r || !r->ਅੰਕੜੇ || ਲੰਮਾਈ > r->ਮਾਪ - r->ਵਿੱਥ)
    png_error (png, "read error");

  hb_memcpy (ਬਾਹਰਲਾ, r->ਅੰਕੜੇ + r->ਵਿੱਥ, ਲੰਮਾਈ);
  r->ਵਿੱਥ += ਲੰਮਾਈ;
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_png_read_blob_fini (hb_raster_png_read_blob_t *r)
{
  ਜੇ (!r)
    ਵਾਪਸ;

  hb_free (r->rgba);
  hb_free (r->ਪੰਕਤੀਆਂ);
  hb_free (r);
}

ਬਣਤਰ hb_raster_png_write_blob_t
{
  ਅੱਖਰ *ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  size_t ਲੰਮਾਈ = 0;
  size_t allocated = 0;
  uint8_t *rgba = ਖਾਲੀ_ਸੂਚਕ;
  png_bytep *ਪੰਕਤੀਆਂ = ਖਾਲੀ_ਸੂਚਕ;
};

ਸਥਿਰ ਖਾਲੀ
hb_raster_png_write_blob (png_structp png, png_bytep ਵਿੱਚ, png_ਮਾਪ_t ਲੰਮਾਈ)
{
  hb_raster_png_write_blob_t *w = (hb_raster_png_write_blob_t *) png_get_io_ptr (png);
  ਜੇ (!w)
    png_error (png, "write error");

  size_t old_ਲੰਮਾਈ = w->ਲੰਮਾਈ;
  ਜੇ ((size_t) ਲੰਮਾਈ > (size_t) -1 - old_ਲੰਮਾਈ)
    png_error (png, "write error");

  size_t new_ਲੰਮਾਈ = old_ਲੰਮਾਈ + ਲੰਮਾਈ;
  ਜੇ (new_ਲੰਮਾਈ > w->allocated)
  {
    size_t new_allocated = w->allocated ? w->allocated : 4096;
    ਜਦੋਂ (new_allocated < new_ਲੰਮਾਈ)
    {
      size_t next = new_allocated * 2;
      ਜੇ (next <= new_allocated)
      {
	new_allocated = new_ਲੰਮਾਈ;
	ਰੋਕੋ;
      }
      new_allocated = next;
    }

    ਅੱਖਰ *ਅੰਕੜੇ = (ਅੱਖਰ *) hb_realloc (w->ਅੰਕੜੇ, new_allocated);
    ਜੇ (!ਅੰਕੜੇ)
      png_error (png, "write error");
    w->ਅੰਕੜੇ = ਅੰਕੜੇ;
    w->allocated = new_allocated;
  }

  hb_memcpy (w->ਅੰਕੜੇ + old_ਲੰਮਾਈ, ਵਿੱਚ, ਲੰਮਾਈ);
  w->ਲੰਮਾਈ = new_ਲੰਮਾਈ;
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_png_flush_blob (png_structp png HB_ਅਣਵਰਤਿਆ)
{}

ਸਥਿਰ ਖਾਲੀ
hb_raster_png_write_blob_fini (hb_raster_png_write_blob_t *w)
{
  ਜੇ (!w)
    ਵਾਪਸ;

  hb_free (w->ਅੰਕੜੇ);
  hb_free (w->rgba);
  hb_free (w->ਪੰਕਤੀਆਂ);
  hb_free (w);
}
#ਜੇ_ਅੰਤ


/*
 * Image compositing
 */

/* Unpack premultiplied pixel to float RGBA [0,1]. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
unpack_ਮੰਜ਼ਿਲ_float (uint32_t px, ਦਸ਼ਮਲਵ &r, ਦਸ਼ਮਲਵ &g, ਦਸ਼ਮਲਵ &b, ਦਸ਼ਮਲਵ &a)
{
  b = (px & 0xFF) / 255.f;
  g = ((px >> 8) & 0xFF) / 255.f;
  r = ((px >> 16) & 0xFF) / 255.f;
  a = (px >> 24) / 255.f;
}

/* Pack float RGBA [0,1] premultiplied back to uint32_t. */
ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t
pack_from_float (ਦਸ਼ਮਲਵ r, ਦਸ਼ਮਲਵ g, ਦਸ਼ਮਲਵ b, ਦਸ਼ਮਲਵ a)
{
  ਵਾਪਸ hb_raster_pack_pixel ((uint8_t) (hb_clamp (b, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (g, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (r, 0.f, 1.f) * 255.f + 0.5f),
			       (uint8_t) (hb_clamp (a, 0.f, 1.f) * 255.f + 0.5f));
}

/* Separable blend mode functions: operate on unpremultiplied [0,1] channels. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_multiply (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc) { ਵਾਪਸ sc * dc; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_screen (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc) { ਵਾਪਸ sc + dc - sc * dc; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_overlay (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc)
{ ਵਾਪਸ dc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_darken (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc) { ਵਾਪਸ hb_min (sc, dc); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_lighten (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc) { ਵਾਪਸ hb_max (sc, dc); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_color_dodge (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc)
{
  ਜੇ (dc <= 0.f) ਵਾਪਸ 0.f;
  ਜੇ (sc >= 1.f) ਵਾਪਸ 1.f;
  ਵਾਪਸ hb_min (1.f, dc / (1.f - sc));
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_color_burn (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc)
{
  ਜੇ (dc >= 1.f) ਵਾਪਸ 1.f;
  ਜੇ (sc <= 0.f) ਵਾਪਸ 0.f;
  ਵਾਪਸ 1.f - hb_min (1.f, (1.f - dc) / sc);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_hard_light (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc)
{ ਵਾਪਸ sc <= 0.5f ? 2.f * sc * dc : 1.f - 2.f * (1.f - sc) * (1.f - dc); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_soft_light (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc)
{
  ਜੇ (sc <= 0.5f)
    ਵਾਪਸ dc - (1.f - 2.f * sc) * dc * (1.f - dc);
  ਦਸ਼ਮਲਵ d = (dc <= 0.25f) ? ((16.f * dc - 12.f) * dc + 4.f) * dc
			   : sqrtf (dc);
  ਵਾਪਸ dc + (2.f * sc - 1.f) * (d - dc);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_difference (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc) { ਵਾਪਸ fabsf (sc - dc); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
blend_exclusion (ਦਸ਼ਮਲਵ sc, ਦਸ਼ਮਲਵ dc) { ਵਾਪਸ sc + dc - 2.f * sc * dc; }

/* Apply a separable blend mode per-pixel.
 * Both src and dst are premultiplied BGRA32. */
ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t
apply_separable_blend (uint32_t src, uint32_t dst,
		       ਦਸ਼ਮਲਵ (*blend_fn)(ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ))
{
  ਦਸ਼ਮਲਵ sr, sg, sb, sa;
  ਦਸ਼ਮਲਵ dr, dg, db, da;
  unpack_ਮੰਜ਼ਿਲ_float (src, sr, sg, sb, sa);
  unpack_ਮੰਜ਼ਿਲ_float (dst, dr, dg, db, da);

  ਦਸ਼ਮਲਵ usr = sa > 0.f ? sr / sa : 0.f;
  ਦਸ਼ਮਲਵ usg = sa > 0.f ? sg / sa : 0.f;
  ਦਸ਼ਮਲਵ usb = sa > 0.f ? sb / sa : 0.f;
  ਦਸ਼ਮਲਵ udr = da > 0.f ? dr / da : 0.f;
  ਦਸ਼ਮਲਵ udg = da > 0.f ? dg / da : 0.f;
  ਦਸ਼ਮਲਵ udb = da > 0.f ? db / da : 0.f;

  ਦਸ਼ਮਲਵ br = blend_fn (usr, udr);
  ਦਸ਼ਮਲਵ bg = blend_fn (usg, udg);
  ਦਸ਼ਮਲਵ bb = blend_fn (usb, udb);

  ਦਸ਼ਮਲਵ ra = sa + da - sa * da;
  ਦਸ਼ਮਲਵ rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  ਦਸ਼ਮਲਵ rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  ਦਸ਼ਮਲਵ rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  ਵਾਪਸ pack_from_float (rr, rg, rb, ra);
}

/* HSL helpers */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
hsl_luminosity (ਦਸ਼ਮਲਵ r, ਦਸ਼ਮਲਵ g, ਦਸ਼ਮਲਵ b)
{ ਵਾਪਸ 0.299f * r + 0.587f * g + 0.114f * b; }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
hsl_saturation (ਦਸ਼ਮਲਵ r, ਦਸ਼ਮਲਵ g, ਦਸ਼ਮਲਵ b)
{ ਵਾਪਸ hb_max (hb_max (r, g), b) - hb_min (hb_min (r, g), b); }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hsl_clip_color (ਦਸ਼ਮਲਵ &r, ਦਸ਼ਮਲਵ &g, ਦਸ਼ਮਲਵ &b)
{
  ਦਸ਼ਮਲਵ l = hsl_luminosity (r, g, b);
  ਦਸ਼ਮਲਵ mn = hb_min (hb_min (r, g), b);
  ਦਸ਼ਮਲਵ mx = hb_max (hb_max (r, g), b);
  ਜੇ (mn < 0.f)
  {
    ਦਸ਼ਮਲਵ d = l - mn;
    ਜੇ (d > 0.f) { r = l + (r - l) * l / d; g = l + (g - l) * l / d; b = l + (b - l) * l / d; }
  }
  ਜੇ (mx > 1.f)
  {
    ਦਸ਼ਮਲਵ d = mx - l;
    ਜੇ (d > 0.f) { r = l + (r - l) * (1.f - l) / d; g = l + (g - l) * (1.f - l) / d; b = l + (b - l) * (1.f - l) / d; }
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hsl_ਸੈੱਟ_luminosity (ਦਸ਼ਮਲਵ &r, ਦਸ਼ਮਲਵ &g, ਦਸ਼ਮਲਵ &b, ਦਸ਼ਮਲਵ l)
{
  ਦਸ਼ਮਲਵ d = l - hsl_luminosity (r, g, b);
  r += d; g += d; b += d;
  hsl_clip_color (r, g, b);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hsl_ਸੈੱਟ_saturation_inner (ਦਸ਼ਮਲਵ &mn, ਦਸ਼ਮਲਵ &mid, ਦਸ਼ਮਲਵ &mx, ਦਸ਼ਮਲਵ s)
{
  ਜੇ (mx > mn)
  {
    mid = (mid - mn) * s / (mx - mn);
    mx = s;
  }
  ਨਹੀਂਤਾਂ
    mid = mx = 0.f;
  mn = 0.f;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hsl_ਸੈੱਟ_saturation (ਦਸ਼ਮਲਵ &r, ਦਸ਼ਮਲਵ &g, ਦਸ਼ਮਲਵ &b, ਦਸ਼ਮਲਵ s)
{
  ਜੇ (r <= g)
  {
    ਜੇ (g <= b)      hsl_ਸੈੱਟ_saturation_inner (r, g, b, s);
    ਨਹੀਂਤਾਂ ਜੇ (r <= b) hsl_ਸੈੱਟ_saturation_inner (r, b, g, s);
    ਨਹੀਂਤਾਂ             hsl_ਸੈੱਟ_saturation_inner (b, r, g, s);
  }
  ਨਹੀਂਤਾਂ
  {
    ਜੇ (r <= b)      hsl_ਸੈੱਟ_saturation_inner (g, r, b, s);
    ਨਹੀਂਤਾਂ ਜੇ (g <= b) hsl_ਸੈੱਟ_saturation_inner (g, b, r, s);
    ਨਹੀਂਤਾਂ             hsl_ਸੈੱਟ_saturation_inner (b, g, r, s);
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t
apply_hsl_blend (uint32_t src, uint32_t dst,
		 hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
  ਦਸ਼ਮਲਵ sr, sg, sb, sa;
  ਦਸ਼ਮਲਵ dr, dg, db, da;
  unpack_ਮੰਜ਼ਿਲ_float (src, sr, sg, sb, sa);
  unpack_ਮੰਜ਼ਿਲ_float (dst, dr, dg, db, da);

  ਦਸ਼ਮਲਵ usr = sa > 0.f ? sr / sa : 0.f;
  ਦਸ਼ਮਲਵ usg = sa > 0.f ? sg / sa : 0.f;
  ਦਸ਼ਮਲਵ usb = sa > 0.f ? sb / sa : 0.f;
  ਦਸ਼ਮਲਵ udr = da > 0.f ? dr / da : 0.f;
  ਦਸ਼ਮਲਵ udg = da > 0.f ? dg / da : 0.f;
  ਦਸ਼ਮਲਵ udb = da > 0.f ? db / da : 0.f;

  ਦਸ਼ਮਲਵ br = udr, bg = udg, bb = udb;

  ਜੇ (ਬਹੁਲਕ == HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_HUE)
  {
    br = usr; bg = usg; bb = usb;
    hsl_ਸੈੱਟ_saturation (br, bg, bb, hsl_saturation (udr, udg, udb));
    hsl_ਸੈੱਟ_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  ਨਹੀਂਤਾਂ ਜੇ (ਬਹੁਲਕ == HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_SATURATION)
  {
    br = udr; bg = udg; bb = udb;
    hsl_ਸੈੱਟ_saturation (br, bg, bb, hsl_saturation (usr, usg, usb));
    hsl_ਸੈੱਟ_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  ਨਹੀਂਤਾਂ ਜੇ (ਬਹੁਲਕ == HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_COLOR)
  {
    br = usr; bg = usg; bb = usb;
    hsl_ਸੈੱਟ_luminosity (br, bg, bb, hsl_luminosity (udr, udg, udb));
  }
  ਨਹੀਂਤਾਂ /* HSL_LUMINOSITY */
  {
    br = udr; bg = udg; bb = udb;
    hsl_ਸੈੱਟ_luminosity (br, bg, bb, hsl_luminosity (usr, usg, usb));
  }

  ਦਸ਼ਮਲਵ ra = sa + da - sa * da;
  ਦਸ਼ਮਲਵ rr = sa * da * br + sa * (1.f - da) * usr + (1.f - sa) * da * udr;
  ਦਸ਼ਮਲਵ rg = sa * da * bg + sa * (1.f - da) * usg + (1.f - sa) * da * udg;
  ਦਸ਼ਮਲਵ rb = sa * da * bb + sa * (1.f - da) * usb + (1.f - sa) * da * udb;

  ਵਾਪਸ pack_from_float (rr, rg, rb, ra);
}

/* Composite per-pixel with full blend mode support. */
ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t
ਸੰਯੁਕਤ_pixel (uint32_t src, uint32_t dst,
		 hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
  uint8_t sa = (uint8_t) (src >> 24);
  uint8_t da = (uint8_t) (dst >> 24);

  ਚੋਣ (ਬਹੁਲਕ)
  {
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_CLEAR:
    ਵਾਪਸ 0;
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC:
    ਵਾਪਸ src;
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST:
    ਵਾਪਸ dst;
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_OVER:
    ਵਾਪਸ hb_raster_src_over (src, dst);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_OVER:
    ਵਾਪਸ hb_raster_src_over (dst, src);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਵਿੱਚ:
    ਵਾਪਸ hb_raster_alpha_mul (src, da);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਵਿੱਚ:
    ਵਾਪਸ hb_raster_alpha_mul (dst, sa);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਬਾਹਰਲਾ:
    ਵਾਪਸ hb_raster_alpha_mul (src, 255 - da);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਬਾਹਰਲਾ:
    ਵਾਪਸ hb_raster_alpha_mul (dst, 255 - sa);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ATOP:
  {
    /* Fa=Da, Fb=1-Sa */
    uint32_t a = hb_raster_alpha_mul (src, da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) (a >> 24) + (b >> 24));
    ਵਾਪਸ hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ATOP:
  {
    uint32_t a = hb_raster_alpha_mul (dst, sa);
    uint32_t b = hb_raster_alpha_mul (src, 255 - da);
    uint8_t rb = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) (a >> 24) + (b >> 24));
    ਵਾਪਸ hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_XOR:
  {
    uint32_t a = hb_raster_alpha_mul (src, 255 - da);
    uint32_t b = hb_raster_alpha_mul (dst, 255 - sa);
    uint8_t rb = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) (a & 0xFF) + (b & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((a >> 8) & 0xFF) + ((b >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((a >> 16) & 0xFF) + ((b >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) (a >> 24) + (b >> 24));
    ਵਾਪਸ hb_raster_pack_pixel (rb, rg, rr, ra);
  }
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_PLUS:
  {
    uint8_t rb = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) (src & 0xFF) + (dst & 0xFF));
    uint8_t rg = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((src >> 8) & 0xFF) + ((dst >> 8) & 0xFF));
    uint8_t rr = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((src >> 16) & 0xFF) + ((dst >> 16) & 0xFF));
    uint8_t ra = (uint8_t) hb_min (255u, (ਬਿਨਾਨਿਸ਼ਾਨੀ) (src >> 24) + (dst >> 24));
    ਵਾਪਸ hb_raster_pack_pixel (rb, rg, rr, ra);
  }

  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_MULTIPLY:  ਵਾਪਸ apply_separable_blend (src, dst, blend_multiply);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SCREEN:    ਵਾਪਸ apply_separable_blend (src, dst, blend_screen);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_OVERLAY:   ਵਾਪਸ apply_separable_blend (src, dst, blend_overlay);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DARKEN:    ਵਾਪਸ apply_separable_blend (src, dst, blend_darken);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_LIGHTEN:   ਵਾਪਸ apply_separable_blend (src, dst, blend_lighten);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_DODGE: ਵਾਪਸ apply_separable_blend (src, dst, blend_color_dodge);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_BURN:  ਵਾਪਸ apply_separable_blend (src, dst, blend_color_burn);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HARD_LIGHT:  ਵਾਪਸ apply_separable_blend (src, dst, blend_hard_light);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SOFT_LIGHT:  ਵਾਪਸ apply_separable_blend (src, dst, blend_soft_light);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DIFFERENCE:  ਵਾਪਸ apply_separable_blend (src, dst, blend_difference);
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_EXCLUSION:   ਵਾਪਸ apply_separable_blend (src, dst, blend_exclusion);

  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_HUE:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_SATURATION:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_COLOR:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_LUMINOSITY:
    ਵਾਪਸ apply_hsl_blend (src, dst, ਬਹੁਲਕ);

  ਮੂਲ:
    ਵਾਪਸ hb_raster_src_over (src, dst);
  }
}

/* hb_raster_image_t */

ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_raster_image_t::ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_pixel (hb_raster_format_t format)
{
  ਵਾਪਸ format == HB_RASTER_FORMAT_BGRA32 ? 4u : 1u;
}

ਤਾਰਕਿਕ
hb_raster_image_t::configure (hb_raster_format_t format,
			      hb_raster_extents_t extents)
{
  ਜੇ (format != HB_RASTER_FORMAT_A8 &&
      format != HB_RASTER_FORMAT_BGRA32)
    format = HB_RASTER_FORMAT_A8;

  ਬਿਨਾਨਿਸ਼ਾਨੀ bpp = ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_pixel (format);
  ਜੇ (extents.ਚੌੜਾਈ > UINT_MAX / bpp)
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ min_stride = extents.ਚੌੜਾਈ * bpp;
  ਜੇ (extents.stride == 0 || extents.stride < min_stride)
    extents.stride = min_stride;

  ਜੇ (extents.ਉਚਾਈ && extents.stride > (size_t) -1 / extents.ਉਚਾਈ)
    ਵਾਪਸ ਝੂਠ;

  size_t buf_ਮਾਪ = (size_t) extents.stride * extents.ਉਚਾਈ;
  ਜੇ (buf_ਮਾਪ > HB_RASTER_MAX_ਬਫਰ_ਮਾਪ)
    ਵਾਪਸ ਝੂਠ;
  ਜੇ (unlikely (!ਬਫਰ.resize_dirty (buf_ਮਾਪ)))
    ਵਾਪਸ ਝੂਠ;

  ਇਹ->format = format;
  ਇਹ->extents = extents;
  ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ
hb_raster_image_t::deserialize_from_png (hb_blob_t *blob)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_PNG
  ਵਾਪਸ ਝੂਠ;
#ਨਹੀਂਤਾਂ
  ਜੇ (!blob)
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ blob_len = 0;
  ਅਚਲ uint8_t *blob_ਅੰਕੜੇ = (ਅਚਲ uint8_t *) hb_blob_get_ਅੰਕੜੇ (blob, &blob_len);
  ਜੇ (!blob_ਅੰਕੜੇ || !blob_len)
    ਵਾਪਸ ਝੂਠ;

  png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, ਖਾਲੀ_ਸੂਚਕ,
					    hb_raster_png_error,
					    hb_raster_png_warning);
  ਜੇ (!png)
    ਵਾਪਸ ਝੂਠ;

  png_infop info = png_create_info_struct (png);
  ਜੇ (!info)
  {
    png_destroy_read_struct (&png, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    ਵਾਪਸ ਝੂਠ;
  }

  hb_raster_png_read_blob_t *reader = (hb_raster_png_read_blob_t *) hb_calloc (1, ਆਕਾਰ (*reader));
  ਜੇ (!reader)
  {
    png_destroy_read_struct (&png, &info, ਖਾਲੀ_ਸੂਚਕ);
    ਵਾਪਸ ਝੂਠ;
  }

  reader->ਅੰਕੜੇ = blob_ਅੰਕੜੇ;
  reader->ਮਾਪ = (size_t) blob_len;
  png_ਸੈੱਟ_read_fn (png, reader, hb_raster_png_read_blob);
  ਜੇ (setjmp (png_jmpbuf (png)))
  {
    png_destroy_read_struct (&png, &info, ਖਾਲੀ_ਸੂਚਕ);
    hb_raster_png_read_blob_fini (reader);
    ਵਾਪਸ ਝੂਠ;
  }

  png_read_info (png, info);

  png_uint_32 w = 0, h = 0;
  ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_ਡੂੰਘਾਈ_2 = 0, color_type = 0;
  ਪੂਰਨ interlace_type = 0, compression_type = 0, filter_ਵਿਧੀ_2 = 0;
  png_get_IHDR (png, info, &w, &h, &ਦੋਆਧਾਰੀ_ਅੰਕ_ਡੂੰਘਾਈ_2, &color_type,
		&interlace_type, &compression_type, &filter_ਵਿਧੀ_2);

  ਜੇ (!w || !h || w > (png_uint_32) ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ || h > (png_uint_32) ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
  {
    png_destroy_read_struct (&png, &info, ਖਾਲੀ_ਸੂਚਕ);
    hb_raster_png_read_blob_fini (reader);
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ has_trns = png_get_valid (png, info, PNG_INFO_tRNS);

  ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ_ਡੂੰਘਾਈ_2 == 16)
    png_ਸੈੱਟ_strip_16 (png);
  ਜੇ (color_type == PNG_COLOR_TYPE_PALETTE)
    png_ਸੈੱਟ_palette_ਮੰਜ਼ਿਲ_rgb (png);
  ਜੇ (color_type == PNG_COLOR_TYPE_ਸਲੇਟੀ && ਦੋਆਧਾਰੀ_ਅੰਕ_ਡੂੰਘਾਈ_2 < 8)
    png_ਸੈੱਟ_expand_ਸਲੇਟੀ_1_2_4_ਮੰਜ਼ਿਲ_8 (png);
  ਜੇ (has_trns)
    png_ਸੈੱਟ_t_RNS_ਮੰਜ਼ਿਲ_alpha (png);
  ਜੇ (color_type == PNG_COLOR_TYPE_ਸਲੇਟੀ || color_type == PNG_COLOR_TYPE_ਸਲੇਟੀ_ALPHA)
    png_ਸੈੱਟ_ਸਲੇਟੀ_ਮੰਜ਼ਿਲ_rgb (png);
  ਜੇ (!(color_type & PNG_COLOR_ਨਕਾਬ_ALPHA) && !has_trns)
    png_ਸੈੱਟ_ਜੋੜੋ_alpha (png, 0xff, PNG_FILLER_AFTER);
  ਜੇ (interlace_type != PNG_INTERLACE_NONE)
    png_ਸੈੱਟ_interlace_handling (png);

  png_read_update_info (png, info);

  ਜੇ (png_get_ਦੋਆਧਾਰੀ_ਅੰਕ_ਡੂੰਘਾਈ (png, info) != 8 || png_get_channels (png, info) != 4)
  {
    png_destroy_read_struct (&png, &info, ਖਾਲੀ_ਸੂਚਕ);
    hb_raster_png_read_blob_fini (reader);
    ਵਾਪਸ ਝੂਠ;
  }

  png_ਮਾਪ_t rowbytes = png_get_rowbytes (png, info);
  ਜੇ (rowbytes < (png_ਮਾਪ_t) w * 4u)
  {
    png_destroy_read_struct (&png, &info, ਖਾਲੀ_ਸੂਚਕ);
    hb_raster_png_read_blob_fini (reader);
    ਵਾਪਸ ਝੂਠ;
  }

  size_t rgba_ਮਾਪ = (size_t) rowbytes * (size_t) h;
  ਜੇ ((h && rgba_ਮਾਪ / (size_t) h != (size_t) rowbytes) ||
      rgba_ਮਾਪ > HB_RASTER_MAX_ਬਫਰ_ਮਾਪ)
  {
    png_destroy_read_struct (&png, &info, ਖਾਲੀ_ਸੂਚਕ);
    hb_raster_png_read_blob_fini (reader);
    ਵਾਪਸ ਝੂਠ;
  }

  reader->rgba = (uint8_t *) hb_malloc (rgba_ਮਾਪ);
  ਜੇ (!reader->rgba)
  {
    png_destroy_read_struct (&png, &info, ਖਾਲੀ_ਸੂਚਕ);
    hb_raster_png_read_blob_fini (reader);
    ਵਾਪਸ ਝੂਠ;
  }

  size_t ਪੰਕਤੀਆਂ_ਮਾਪ = (size_t) h * ਆਕਾਰ (png_bytep);
  ਜੇ (h && ਪੰਕਤੀਆਂ_ਮਾਪ / (size_t) h != ਆਕਾਰ (png_bytep))
  {
    png_destroy_read_struct (&png, &info, ਖਾਲੀ_ਸੂਚਕ);
    hb_raster_png_read_blob_fini (reader);
    ਵਾਪਸ ਝੂਠ;
  }

  reader->ਪੰਕਤੀਆਂ = (png_bytep *) hb_malloc (ਪੰਕਤੀਆਂ_ਮਾਪ);
  ਜੇ (!reader->ਪੰਕਤੀਆਂ)
  {
    png_destroy_read_struct (&png, &info, ਖਾਲੀ_ਸੂਚਕ);
    hb_raster_png_read_blob_fini (reader);
    ਵਾਪਸ ਝੂਠ;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = 0; y < (ਬਿਨਾਨਿਸ਼ਾਨੀ) h; y++)
    reader->ਪੰਕਤੀਆਂ[y] = (png_bytep) (reader->rgba + (size_t) y * (size_t) rowbytes);

  png_read_image (png, reader->ਪੰਕਤੀਆਂ);
  png_read_ਸਿਰਾ (png, ਖਾਲੀ_ਸੂਚਕ);
  png_destroy_read_struct (&png, &info, ਖਾਲੀ_ਸੂਚਕ);

  hb_raster_image_t decoded;
  hb_raster_extents_t decoded_extents = {0, 0, (ਬਿਨਾਨਿਸ਼ਾਨੀ) w, (ਬਿਨਾਨਿਸ਼ਾਨੀ) h, 0};
  ਜੇ (!decoded.configure (HB_RASTER_FORMAT_BGRA32, decoded_extents))
  {
    hb_raster_png_read_blob_fini (reader);
    ਵਾਪਸ ਝੂਠ;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = 0; y < (ਬਿਨਾਨਿਸ਼ਾਨੀ) h; y++)
  {
    hb_packed_t<uint32_t> *dst = (hb_packed_t<uint32_t> *) (decoded.ਬਫਰ.arrayZ + (size_t) ((ਬਿਨਾਨਿਸ਼ਾਨੀ) h - 1 - y) * decoded.extents.stride);
    ਅਚਲ uint8_t *src = reader->rgba + (size_t) y * (size_t) rowbytes;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = 0; x < (ਬਿਨਾਨਿਸ਼ਾਨੀ) w; x++)
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

  hb_ਅਦਲਾ_ਬਦਲੀ (ਬਫਰ, decoded.ਬਫਰ);
  hb_ਅਦਲਾ_ਬਦਲੀ (ਇਹ->extents, decoded.extents);
  hb_ਅਦਲਾ_ਬਦਲੀ (format, decoded.format);
  hb_raster_png_read_blob_fini (reader);
  ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
}

hb_blob_t *
hb_raster_image_t::serialize_ਮੰਜ਼ਿਲ_png_or_fail () ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_PNG
  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
#ਨਹੀਂਤਾਂ
  ਜੇ (format != HB_RASTER_FORMAT_BGRA32 || !extents.ਚੌੜਾਈ || !extents.ਉਚਾਈ)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  png_structp png = png_create_write_struct (PNG_LIBPNG_VER_STRING, ਖਾਲੀ_ਸੂਚਕ,
					     hb_raster_png_error,
					     hb_raster_png_warning);
  ਜੇ (!png)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  png_infop info = png_create_info_struct (png);
  ਜੇ (!info)
  {
    png_destroy_write_struct (&png, ਖਾਲੀ_ਸੂਚਕ);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_raster_png_write_blob_t *writer = (hb_raster_png_write_blob_t *) hb_calloc (1, ਆਕਾਰ (*writer));
  ਜੇ (!writer)
  {
    png_destroy_write_struct (&png, &info);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  png_ਸੈੱਟ_write_fn (png, writer, hb_raster_png_write_blob, hb_raster_png_flush_blob);
  ਜੇ (setjmp (png_jmpbuf (png)))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  png_ਸੈੱਟ_IHDR (png, info,
		extents.ਚੌੜਾਈ, extents.ਉਚਾਈ,
		8, PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  size_t rowbytes = (size_t) extents.ਚੌੜਾਈ * 4u;
  size_t rgba_ਮਾਪ = rowbytes * (size_t) extents.ਉਚਾਈ;
  ਜੇ (extents.ਉਚਾਈ && rgba_ਮਾਪ / (size_t) extents.ਉਚਾਈ != rowbytes)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  writer->rgba = (uint8_t *) hb_malloc (rgba_ਮਾਪ);
  ਜੇ (!writer->rgba)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  size_t ਪੰਕਤੀਆਂ_ਮਾਪ = (size_t) extents.ਉਚਾਈ * ਆਕਾਰ (png_bytep);
  ਜੇ (extents.ਉਚਾਈ && ਪੰਕਤੀਆਂ_ਮਾਪ / (size_t) extents.ਉਚਾਈ != ਆਕਾਰ (png_bytep))
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  writer->ਪੰਕਤੀਆਂ = (png_bytep *) hb_malloc (ਪੰਕਤੀਆਂ_ਮਾਪ);
  ਜੇ (!writer->ਪੰਕਤੀਆਂ)
  {
    png_destroy_write_struct (&png, &info);
    hb_raster_png_write_blob_fini (writer);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = 0; y < extents.ਉਚਾਈ; y++)
  {
    uint8_t *dst = writer->rgba + (size_t) y * rowbytes;
    ਅਚਲ uint8_t *src = ਬਫਰ.arrayZ + (size_t) (extents.ਉਚਾਈ - 1 - y) * extents.stride;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = 0; x < extents.ਚੌੜਾਈ; x++)
    {
      uint32_t px;
      hb_memcpy (&px, src + x * 4, 4);
      uint8_t b = (uint8_t) (px & 0xFF);
      uint8_t g = (uint8_t) ((px >> 8) & 0xFF);
      uint8_t r = (uint8_t) ((px >> 16) & 0xFF);
      uint8_t a = (uint8_t) (px >> 24);

      dst[4 * x + 3] = a;
      ਜੇ (a)
      {
	dst[4 * x + 0] = (uint8_t) hb_min (255u, ((ਬਿਨਾਨਿਸ਼ਾਨੀ) r * 255u + a / 2u) / a);
	dst[4 * x + 1] = (uint8_t) hb_min (255u, ((ਬਿਨਾਨਿਸ਼ਾਨੀ) g * 255u + a / 2u) / a);
	dst[4 * x + 2] = (uint8_t) hb_min (255u, ((ਬਿਨਾਨਿਸ਼ਾਨੀ) b * 255u + a / 2u) / a);
      }
      ਨਹੀਂਤਾਂ
	dst[4 * x + 0] = dst[4 * x + 1] = dst[4 * x + 2] = 0;
    }

    writer->ਪੰਕਤੀਆਂ[y] = (png_bytep) dst;
  }

  png_write_image (png, writer->ਪੰਕਤੀਆਂ);
  png_write_ਸਿਰਾ (png, info);
  png_destroy_write_struct (&png, &info);

  ਜੇ (writer->ਲੰਮਾਈ > (size_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)
  {
    hb_raster_png_write_blob_fini (writer);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) writer->ਲੰਮਾਈ;
  ਅੱਖਰ *ਅੰਕੜੇ = writer->ਅੰਕੜੇ;
  writer->ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  hb_raster_png_write_blob_fini (writer);
  ਜੇ (!ਅੰਕੜੇ && ਲੰਮਾਈ)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_blob_t *blob = hb_blob_create_or_fail (ਅੰਕੜੇ, ਲੰਮਾਈ,
					    HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE,
					    ਅੰਕੜੇ, hb_free);
  ਜੇ (!blob)
    hb_free (ਅੰਕੜੇ);
  ਵਾਪਸ blob;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ
hb_raster_image_t::clear ()
{
  size_t buf_ਮਾਪ = (size_t) extents.stride * extents.ਉਚਾਈ;
  hb_memset (ਬਫਰ.arrayZ, 0, buf_ਮਾਪ);
}

ਅਚਲ uint8_t *
hb_raster_image_t::get_ਬਫਰ () ਅਚਲ
{
  ਵਾਪਸ ਬਫਰ.arrayZ;
}

ਖਾਲੀ
hb_raster_image_t::ਸੰਯੁਕਤ_from (ਅਚਲ hb_raster_image_t *src,
				   hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ w = extents.ਚੌੜਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ h = extents.ਉਚਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ stride = extents.stride;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = 0; y < h; y++)
  {
    hb_packed_t<uint32_t> *dp = (hb_packed_t<uint32_t> *) (ਬਫਰ.arrayZ + y * stride);
    ਅਚਲ hb_packed_t<uint32_t> *sp = (ਅਚਲ hb_packed_t<uint32_t> *) (src->ਬਫਰ.arrayZ + y * stride);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = 0; x < w; x++)
      dp[x] = hb_packed_t<uint32_t> (ਸੰਯੁਕਤ_pixel ((uint32_t) sp[x], (uint32_t) dp[x], ਬਹੁਲਕ));
  }
}

/* Composite src image onto dst image.
 * Both images must have the same extents and BGRA32 format. */
ਖਾਲੀ
hb_raster_image_ਸੰਯੁਕਤ (hb_raster_image_t *dst,
			   ਅਚਲ hb_raster_image_t *src,
			   hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
  dst->ਸੰਯੁਕਤ_from (src, ਬਹੁਲਕ);
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
hb_raster_image_create_or_fail (ਖਾਲੀ)
{
  ਵਾਪਸ hb_object_create<hb_raster_image_t> ();
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
  ਵਾਪਸ hb_object_reference (image);
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
ਖਾਲੀ
hb_raster_image_destroy (hb_raster_image_t *image)
{
  ਜੇ (!hb_object_destroy (image)) ਵਾਪਸ;
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
hb_raster_image_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_raster_image_t  *image,
			       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			       ਖਾਲੀ               *ਅੰਕੜੇ,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (image, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_raster_image_get_user_ਅੰਕੜੇ (ਅਚਲ hb_raster_image_t  *image,
			       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (image, key);
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
			   ਅਚਲ hb_raster_extents_t *extents)
{
  ਜੇ (unlikely (!extents))
  {
    image->extents = {};
    image->ਬਫਰ.resize_exact (0);
    ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ image->configure (format, *extents);
}

/**
 * hb_raster_image_clear:
 * @image: a raster image
 *
 * Clears @image pixels to zero while keeping current extents and format.
 *
 * Since: 13.0.0
 **/
ਖਾਲੀ
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
ਅਚਲ uint8_t *
hb_raster_image_get_ਬਫਰ (ਅਚਲ hb_raster_image_t *image)
{
  ਵਾਪਸ image->get_ਬਫਰ ();
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
ਖਾਲੀ
hb_raster_image_get_extents (ਅਚਲ hb_raster_image_t   *image,
			     hb_raster_extents_t *extents)
{
  ਜੇ (extents)
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
hb_raster_image_get_format (ਅਚਲ hb_raster_image_t *image)
{
  ਵਾਪਸ image->format;
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
  ਵਾਪਸ image->deserialize_from_png (png);
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
hb_raster_image_serialize_ਮੰਜ਼ਿਲ_png_or_fail (ਅਚਲ hb_raster_image_t *image)
{
  ਵਾਪਸ image->serialize_ਮੰਜ਼ਿਲ_png_or_fail ();
}
