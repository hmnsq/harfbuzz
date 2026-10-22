/*
 * Copyright © 2022  Red Hat, Inc
 * Copyright © 2021, 2022  Black Foundry
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
 * Google Author(s): Matthias Clasen
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAIRO

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cairo-utils.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PREALLOCATED_COLOR_STOPS 16

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
  ਦਸ਼ਮਲਵ r, g, b, a;
} hb_cairo_color_t;

ਸਥਿਰ ਸਤਰਅੰਦਰ cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  ਚੋਣ (extend)
    {
    ਹਾਲਤ HB_PAINT_EXTEND_PAD: ਵਾਪਸ CAIRO_EXTEND_PAD;
    ਹਾਲਤ HB_PAINT_EXTEND_REPEAT: ਵਾਪਸ CAIRO_EXTEND_REPEAT;
    ਹਾਲਤ HB_PAINT_EXTEND_REFLECT: ਵਾਪਸ CAIRO_EXTEND_REFLECT;
    ਮੂਲ: ਰੋਕੋ;
    }

  ਵਾਪਸ CAIRO_EXTEND_PAD;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CAIRO_HAS_PNG_FUNCTIONS
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  hb_blob_t *blob;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ;
} hb_cairo_read_blob_ਅੰਕੜੇ_t;

ਸਥਿਰ cairo_ਸਥਿਤੀ_t
hb_cairo_read_blob (ਖਾਲੀ *ਘੇਰਾ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ)
{
  hb_cairo_read_blob_ਅੰਕੜੇ_t *r = (hb_cairo_read_blob_ਅੰਕੜੇ_t *) ਘੇਰਾ;
  ਅਚਲ ਅੱਖਰ *d;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ;

  d = hb_blob_get_ਅੰਕੜੇ (r->blob, &ਮਾਪ);

  ਜੇ (r->ਵਿੱਥ + ਲੰਮਾਈ > ਮਾਪ)
    ਵਾਪਸ CAIRO_ਸਥਿਤੀ_READ_ERROR;

  hb_memcpy (ਅੰਕੜੇ, d + r->ਵਿੱਥ, ਲੰਮਾਈ);
  r->ਵਿੱਥ += ਲੰਮਾਈ;

  ਵਾਪਸ CAIRO_ਸਥਿਤੀ_SUCCESS;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਅਚਲ cairo_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *hb_cairo_surface_blob_user_ਅੰਕੜੇ_ਕੁੰਜੀ = {0};

ਸਥਿਰ ਖਾਲੀ
_hb_cairo_destroy_blob (ਖਾਲੀ *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

ਖਾਲੀ
hb_cairo_ਸੈੱਟ_ਸਰੋਤ_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  ਜੇ (use_foreground)
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_ਸਰੋਤ
    ਦੁੱਗਣਾ r, g, b, a;
    cairo_ਨਮੂਨਾ_t *foreground = cairo_user_scaled_font_get_foreground_ਸਰੋਤ (c->scaled_font);
    ਜੇ (cairo_ਨਮੂਨਾ_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_ਸਥਿਤੀ_SUCCESS)
      cairo_ਸੈੱਟ_ਸਰੋਤ_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
      cairo_ਸੈੱਟ_ਸਰੋਤ_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  ਨਹੀਂਤਾਂ
    cairo_ਸੈੱਟ_ਸਰੋਤ_rgba (cr,
			   hb_color_get_red (color) / 255.,
			   hb_color_get_green (color) / 255.,
			   hb_color_get_blue (color) / 255.,
			   hb_color_get_alpha (color) / 255.);
}

hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
			     hb_ਟੈਗ_t format,
			     HB_ਅਣਵਰਤਿਆ ਦਸ਼ਮਲਵ slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  ਜੇ (!extents) /* SVG currently. */
    ਵਾਪਸ ਝੂਠ;

  cairo_surface_t *surface = ਖਾਲੀ_ਸੂਚਕ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CAIRO_HAS_PNG_FUNCTIONS
  ਜੇ (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
    hb_cairo_read_blob_ਅੰਕੜੇ_t r;
    r.blob = blob;
    r.ਵਿੱਥ = 0;
    surface = cairo_image_surface_create_from_png_ਧਾਰਾ (hb_cairo_read_blob, &r);

    /* For PNG, width,height can be unreliable, as is the case for NotoColorEmoji :(.
     * Just pull them out of the surface. */
    ਚੌੜਾਈ = cairo_image_surface_get_ਚੌੜਾਈ (surface);
    ਉਚਾਈ = cairo_image_surface_get_ਚੌੜਾਈ (surface);
  }
  ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
  ਜੇ (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ = hb_blob_get_ਲੰਮਾਈ (blob);
    ਜੇ (ਅੰਕੜੇ_ਮਾਪ < ਚੌੜਾਈ * ਉਚਾਈ * 4)
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __BYTE_ORDER
    ਜੇ (__BYTE_ORDER == __BIG_ENDIAN)
    {
      ਅੰਕੜੇ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) hb_blob_get_ਅੰਕੜੇ_writable (blob, ਖਾਲੀ_ਸੂਚਕ);
      ਜੇ (!ਅੰਕੜੇ)
        ਵਾਪਸ ਝੂਠ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਚੌੜਾਈ * ਉਚਾਈ * 4;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i += 4)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ b;
	b = ਅੰਕੜੇ[i];
	ਅੰਕੜੇ[i] = ਅੰਕੜੇ[i+3];
	ਅੰਕੜੇ[i+3] = b;
	b = ਅੰਕੜੇ[i+1];
	ਅੰਕੜੇ[i+1] = ਅੰਕੜੇ[i+2];
	ਅੰਕੜੇ[i+2] = b;
      }
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
      ਅੰਕੜੇ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) hb_blob_get_ਅੰਕੜੇ (blob, ਖਾਲੀ_ਸੂਚਕ);

    surface = cairo_image_surface_create_for_ਅੰਕੜੇ (ਅੰਕੜੇ,
						   CAIRO_FORMAT_ARGB32,
						   ਚੌੜਾਈ, ਉਚਾਈ,
						   ਚੌੜਾਈ * 4);

    cairo_surface_ਸੈੱਟ_user_ਅੰਕੜੇ (surface,
				 hb_cairo_surface_blob_user_ਅੰਕੜੇ_ਕੁੰਜੀ,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  ਜੇ (!surface)
    ਵਾਪਸ ਝੂਠ;

  cairo_save (cr);
  /* this clip is here to work around recording surface limitations */
  cairo_rectangle (cr,
                   extents->x_bearing,
                   extents->y_bearing,
                   extents->ਚੌੜਾਈ,
                   extents->ਉਚਾਈ);
  cairo_clip (cr);

  cairo_ਨਮੂਨਾ_t *ਨਮੂਨਾ = cairo_ਨਮੂਨਾ_create_for_surface (surface);
  cairo_ਨਮੂਨਾ_ਸੈੱਟ_extend (ਨਮੂਨਾ, CAIRO_EXTEND_PAD);

  cairo_ਮੈਟ੍ਰਿਕਸ_t ਮੈਟ੍ਰਿਕਸ = {(ਦੁੱਗਣਾ) ਚੌੜਾਈ, 0, 0, (ਦੁੱਗਣਾ) ਉਚਾਈ, 0, 0};
  cairo_ਨਮੂਨਾ_ਸੈੱਟ_ਮੈਟ੍ਰਿਕਸ (ਨਮੂਨਾ, &ਮੈਟ੍ਰਿਕਸ);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->ਚੌੜਾਈ, extents->ਉਚਾਈ);
  cairo_ਸੈੱਟ_ਸਰੋਤ (cr, ਨਮੂਨਾ);

  cairo_paint (cr);

  cairo_ਨਮੂਨਾ_destroy (ਨਮੂਨਾ);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_ਸਤਰ_t *color_ਸਤਰ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ *ਗਿਣਤੀ_2,
			   hb_color_stop_t **stops)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ len = hb_color_ਸਤਰ_get_color_stops (color_ਸਤਰ, 0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  ਤਾਰਕਿਕ allocated = ਝੂਠ;
  ਜੇ (unlikely (!len))
    ਵਾਪਸ ਝੂਠ;
  ਜੇ (len > *ਗਿਣਤੀ_2)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, ਆਕਾਰ (hb_color_stop_t));
    ਜੇ (unlikely (!*stops))
      ਵਾਪਸ ਝੂਠ;
    allocated = ਸੱਚ;
  }
  hb_color_ਸਤਰ_get_color_stops (color_ਸਤਰ, 0, &len, *stops);
  ਜੇ (unlikely (!len))
  {
    ਜੇ (allocated)
    {
      hb_free (*stops);
      *stops = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ ਝੂਠ;
  }
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len; i++)
    ਜੇ ((*stops)[i].is_foreground)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_ਸਰੋਤ
      ਦੁੱਗਣਾ r, g, b, a;
      cairo_ਨਮੂਨਾ_t *foreground = cairo_user_scaled_font_get_foreground_ਸਰੋਤ (c->scaled_font);
      ਜੇ (cairo_ਨਮੂਨਾ_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_ਸਥਿਤੀ_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *ਗਿਣਤੀ_2 = len;
  ਵਾਪਸ ਸੱਚ;
}

ਖਾਲੀ
hb_cairo_paint_ਰੇਖਿਕ_gradient (hb_cairo_context_t *c,
				 hb_color_ਸਤਰ_t *color_ਸਤਰ,
				 ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
				 ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
				 ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2)
{
  cairo_t *cr = c->cr;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  ਦਸ਼ਮਲਵ xx0, yy0, xx1, yy1;
  ਦਸ਼ਮਲਵ xxx0, yyy0, xxx1, yyy1;
  ਦਸ਼ਮਲਵ min, max;
  cairo_ਨਮੂਨਾ_t *ਨਮੂਨਾ;

  ਜੇ (unlikely (!_hb_cairo_get_color_stops (c, color_ਸਤਰ, &len, &stops)))
    ਵਾਪਸ;
  hb_paint_normalize_color_ਸਤਰ (stops, len, &min, &max);

  hb_paint_reduce_ਰੇਖਿਕ_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  ਨਮੂਨਾ = cairo_ਨਮੂਨਾ_create_ਰੇਖਿਕ ((ਦੁੱਗਣਾ) xxx0, (ਦੁੱਗਣਾ) yyy0, (ਦੁੱਗਣਾ) xxx1, (ਦੁੱਗਣਾ) yyy1);
  cairo_ਨਮੂਨਾ_ਸੈੱਟ_extend (ਨਮੂਨਾ, hb_cairo_extend (hb_color_ਸਤਰ_get_extend (color_ਸਤਰ)));
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < len; i++)
    {
      ਦੁੱਗਣਾ r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_ਨਮੂਨਾ_ਜੋੜੋ_color_stop_rgba (ਨਮੂਨਾ, (ਦੁੱਗਣਾ) stops[i].ਵਿੱਥ, r, g, b, a);
    }

  cairo_ਸੈੱਟ_ਸਰੋਤ (cr, ਨਮੂਨਾ);
  cairo_paint (cr);

  cairo_ਨਮੂਨਾ_destroy (ਨਮੂਨਾ);

  ਜੇ (stops != stops_)
    hb_free (stops);
}

ਖਾਲੀ
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_ਸਤਰ_t *color_ਸਤਰ,
				 ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
				 ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1)
{
  cairo_t *cr = c->cr;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  ਦਸ਼ਮਲਵ min, max;
  ਦਸ਼ਮਲਵ xx0, yy0, xx1, yy1;
  ਦਸ਼ਮਲਵ rr0, rr1;
  cairo_ਨਮੂਨਾ_t *ਨਮੂਨਾ;

  ਜੇ (unlikely (!_hb_cairo_get_color_stops (c, color_ਸਤਰ, &len, &stops)))
    ਵਾਪਸ;
  hb_paint_normalize_color_ਸਤਰ (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  ਨਮੂਨਾ = cairo_ਨਮੂਨਾ_create_radial ((ਦੁੱਗਣਾ) xx0, (ਦੁੱਗਣਾ) yy0, (ਦੁੱਗਣਾ) rr0, (ਦੁੱਗਣਾ) xx1, (ਦੁੱਗਣਾ) yy1, (ਦੁੱਗਣਾ) rr1);
  cairo_ਨਮੂਨਾ_ਸੈੱਟ_extend (ਨਮੂਨਾ, hb_cairo_extend (hb_color_ਸਤਰ_get_extend (color_ਸਤਰ)));

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < len; i++)
    {
      ਦੁੱਗਣਾ r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_ਨਮੂਨਾ_ਜੋੜੋ_color_stop_rgba (ਨਮੂਨਾ, (ਦੁੱਗਣਾ) stops[i].ਵਿੱਥ, r, g, b, a);
    }

  cairo_ਸੈੱਟ_ਸਰੋਤ (cr, ਨਮੂਨਾ);
  cairo_paint (cr);

  cairo_ਨਮੂਨਾ_destroy (ਨਮੂਨਾ);

  ਜੇ (stops != stops_)
    hb_free (stops);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
  ਦਸ਼ਮਲਵ x, y;
} hb_cairo_ਬਿੰਦੂ_t;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
_hb_cairo_interpolate (ਦਸ਼ਮਲਵ f0, ਦਸ਼ਮਲਵ f1, ਦਸ਼ਮਲਵ f)
{
  ਵਾਪਸ f0 + f * (f1 - f0);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  ਜੇ (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

ਸਥਿਰ ਖਾਲੀ
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, ਦਸ਼ਮਲਵ k, hb_cairo_color_t *c)
{
  // According to the COLR specification, gradients
  // should be interpolated in premultiplied form
  _hb_cairo_premultiply (c0);
  _hb_cairo_premultiply (c1);
  c->r = c0->r + k * (c1->r - c0->r);
  c->g = c0->g + k * (c1->g - c0->g);
  c->b = c0->b + k * (c1->b - c0->b);
  c->a = c0->a + k * (c1->a - c0->a);
  _hb_cairo_unpremultiply (c);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
_hb_cairo_dot (hb_cairo_ਬਿੰਦੂ_t p, hb_cairo_ਬਿੰਦੂ_t q)
{
  ਵਾਪਸ p.x * q.x + p.y * q.y;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_cairo_ਬਿੰਦੂ_t
_hb_cairo_normalize (hb_cairo_ਬਿੰਦੂ_t p)
{
  ਦਸ਼ਮਲਵ len = sqrtf (_hb_cairo_dot (p, p));

  ਵਾਪਸ hb_cairo_ਬਿੰਦੂ_t { p.x / len, p.y / len };
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_cairo_ਬਿੰਦੂ_t
hb_cairo_ਜੋੜ (hb_cairo_ਬਿੰਦੂ_t p, hb_cairo_ਬਿੰਦੂ_t q)
{
  ਵਾਪਸ hb_cairo_ਬਿੰਦੂ_t { p.x + q.x, p.y + q.y };
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_cairo_ਬਿੰਦੂ_t
_hb_cairo_difference (hb_cairo_ਬਿੰਦੂ_t p, hb_cairo_ਬਿੰਦੂ_t q)
{
  ਵਾਪਸ hb_cairo_ਬਿੰਦੂ_t { p.x - q.x, p.y - q.y };
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_cairo_ਬਿੰਦੂ_t
_hb_cairo_scale (hb_cairo_ਬਿੰਦੂ_t p, ਦਸ਼ਮਲਵ f)
{
  ਵਾਪਸ hb_cairo_ਬਿੰਦੂ_t { p.x * f, p.y * f };
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
  hb_cairo_ਬਿੰਦੂ_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਜੋੜੋ_patch (cairo_ਨਮੂਨਾ_t *ਨਮੂਨਾ, hb_cairo_ਬਿੰਦੂ_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_ਨਮੂਨਾ_begin_patch (ਨਮੂਨਾ);
  cairo_mesh_ਨਮੂਨਾ_move_ਮੰਜ਼ਿਲ (ਨਮੂਨਾ, (ਦੁੱਗਣਾ) center->x, (ਦੁੱਗਣਾ) center->y);
  cairo_mesh_ਨਮੂਨਾ_ਸਤਰ_ਮੰਜ਼ਿਲ (ਨਮੂਨਾ, (ਦੁੱਗਣਾ) p->p0.x, (ਦੁੱਗਣਾ) p->p0.y);
  cairo_mesh_ਨਮੂਨਾ_ਵਕਰ_ਮੰਜ਼ਿਲ (ਨਮੂਨਾ,
                               (ਦੁੱਗਣਾ) p->c0.x, (ਦੁੱਗਣਾ) p->c0.y,
                               (ਦੁੱਗਣਾ) p->c1.x, (ਦੁੱਗਣਾ) p->c1.y,
                               (ਦੁੱਗਣਾ) p->p1.x, (ਦੁੱਗਣਾ) p->p1.y);
  cairo_mesh_ਨਮੂਨਾ_ਸਤਰ_ਮੰਜ਼ਿਲ (ਨਮੂਨਾ, (ਦੁੱਗਣਾ) center->x, (ਦੁੱਗਣਾ) center->y);
  cairo_mesh_ਨਮੂਨਾ_ਸੈੱਟ_ਕੋਨਾ_color_rgba (ਨਮੂਨਾ, 0,
                                            (ਦੁੱਗਣਾ) p->color0.r,
                                            (ਦੁੱਗਣਾ) p->color0.g,
                                            (ਦੁੱਗਣਾ) p->color0.b,
                                            (ਦੁੱਗਣਾ) p->color0.a);
  cairo_mesh_ਨਮੂਨਾ_ਸੈੱਟ_ਕੋਨਾ_color_rgba (ਨਮੂਨਾ, 1,
                                            (ਦੁੱਗਣਾ) p->color0.r,
                                            (ਦੁੱਗਣਾ) p->color0.g,
                                            (ਦੁੱਗਣਾ) p->color0.b,
                                            (ਦੁੱਗਣਾ) p->color0.a);
  cairo_mesh_ਨਮੂਨਾ_ਸੈੱਟ_ਕੋਨਾ_color_rgba (ਨਮੂਨਾ, 2,
                                            (ਦੁੱਗਣਾ) p->color1.r,
                                            (ਦੁੱਗਣਾ) p->color1.g,
                                            (ਦੁੱਗਣਾ) p->color1.b,
                                            (ਦੁੱਗਣਾ) p->color1.a);
  cairo_mesh_ਨਮੂਨਾ_ਸੈੱਟ_ਕੋਨਾ_color_rgba (ਨਮੂਨਾ, 3,
                                            (ਦੁੱਗਣਾ) p->color1.r,
                                            (ਦੁੱਗਣਾ) p->color1.g,
                                            (ਦੁੱਗਣਾ) p->color1.b,
                                            (ਦੁੱਗਣਾ) p->color1.a);
  cairo_mesh_ਨਮੂਨਾ_ਸਿਰਾ_patch (ਨਮੂਨਾ);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_ANGLE (HB_PI / 8.f)

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਜੋੜੋ_sweep_gradient_patches1 (ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy, ਦਸ਼ਮਲਵ radius,
				       ਦਸ਼ਮਲਵ a0, hb_cairo_color_t *c0,
				       ਦਸ਼ਮਲਵ a1, hb_cairo_color_t *c1,
				       cairo_ਨਮੂਨਾ_t *ਨਮੂਨਾ)
{
  hb_cairo_ਬਿੰਦੂ_t center = hb_cairo_ਬਿੰਦੂ_t { cx, cy };
  ਪੂਰਨ ਅੰਕ_splits;
  hb_cairo_ਬਿੰਦੂ_t p0;
  hb_cairo_color_t color0, color1;

  ਅੰਕ_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_ਬਿੰਦੂ_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  ਲਈ (ਪੂਰਨ a = 0; a < ਅੰਕ_splits; a++)
    {
      ਦਸ਼ਮਲਵ k = (a + 1.) / ਅੰਕ_splits;
      ਦਸ਼ਮਲਵ angle1;
      hb_cairo_ਬਿੰਦੂ_t p1;
      hb_cairo_ਬਿੰਦੂ_t A, U;
      hb_cairo_ਬਿੰਦੂ_t C0, C1;
      hb_cairo_patch_t patch;

      angle1 = _hb_cairo_interpolate (a0, a1, k);
      _hb_cairo_interpolate_colors (c0, c1, k, &color1);

      patch.color0 = color0;
      patch.color1 = color1;

      p1 = hb_cairo_ਬਿੰਦੂ_t { cosf (angle1), sinf (angle1) };
      patch.p0 = hb_cairo_ਜੋੜ (center, _hb_cairo_scale (p0, radius));
      patch.p1 = hb_cairo_ਜੋੜ (center, _hb_cairo_scale (p1, radius));

      A = _hb_cairo_normalize (hb_cairo_ਜੋੜ (p0, p1));
      U = hb_cairo_ਬਿੰਦੂ_t { -A.y, A.x };
      C0 = hb_cairo_ਜੋੜ (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p0, A), p0) / _hb_cairo_dot (U, p0)));
      C1 = hb_cairo_ਜੋੜ (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p1, A), p1) / _hb_cairo_dot (U, p1)));

      patch.c0 = hb_cairo_ਜੋੜ (center, _hb_cairo_scale (hb_cairo_ਜੋੜ (C0, _hb_cairo_scale (_hb_cairo_difference (C0, p0), 0.33333f)), radius));
      patch.c1 = hb_cairo_ਜੋੜ (center, _hb_cairo_scale (hb_cairo_ਜੋੜ (C1, _hb_cairo_scale (_hb_cairo_difference (C1, p1), 0.33333f)), radius));

      hb_cairo_ਜੋੜੋ_patch (ਨਮੂਨਾ, &center, &patch);

      p0 = p1;
      color0 = color1;
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  ਵਾਪਸ {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

ਬਣਤਰ _hb_cairo_sweep_patch_ctx {
  ਦਸ਼ਮਲਵ cx, cy, radius;
  cairo_ਨਮੂਨਾ_t *ਨਮੂਨਾ;
};

ਸਥਿਰ ਖਾਲੀ
_hb_cairo_sweep_emit_patch (ਦਸ਼ਮਲਵ a0, hb_color_t c0,
			    ਦਸ਼ਮਲਵ a1, hb_color_t c1,
			    ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  ਸਵੈਚਾਲਿਤ *ctx = (_hb_cairo_sweep_patch_ctx *) user_ਅੰਕੜੇ;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  hb_cairo_ਜੋੜੋ_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->ਨਮੂਨਾ);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਜੋੜੋ_sweep_gradient_patches (hb_color_stop_t *stops,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n_stops,
				      cairo_extend_t extend,
				      ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy,
				      ਦਸ਼ਮਲਵ radius,
				      ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle,
				      ਦਸ਼ਮਲਵ ਸਿਰਾ_angle,
				      cairo_ਨਮੂਨਾ_t *ਨਮੂਨਾ)
{
  hb_paint_extend_t hb_extend =
    extend == CAIRO_EXTEND_REPEAT  ? HB_PAINT_EXTEND_REPEAT  :
    extend == CAIRO_EXTEND_REFLECT ? HB_PAINT_EXTEND_REFLECT :
				     HB_PAINT_EXTEND_PAD;

  _hb_cairo_sweep_patch_ctx ctx { cx, cy, radius, ਨਮੂਨਾ };
  hb_paint_sweep_gradient_tiles (stops, n_stops, hb_extend,
				 ਸ਼ੁਰੂ_angle, ਸਿਰਾ_angle,
				 _hb_cairo_sweep_emit_patch, &ctx);
}

ਖਾਲੀ
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_ਸਤਰ_t *color_ਸਤਰ,
				ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy,
				ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle,
				ਦਸ਼ਮਲਵ ਸਿਰਾ_angle)
{
  cairo_t *cr = c->cr;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  ਦੁੱਗਣਾ x1, y1, x2, y2;
  ਦਸ਼ਮਲਵ max_x, max_y, radius;
  cairo_ਨਮੂਨਾ_t *ਨਮੂਨਾ;

  ਜੇ (unlikely (!_hb_cairo_get_color_stops (c, color_ਸਤਰ, &len, &stops)))
    ਵਾਪਸ;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (ਅਚਲ hb_color_stop_t &a, ਅਚਲ hb_color_stop_t &b) {
      ਵਾਪਸ (a.ਵਿੱਥ > b.ਵਿੱਥ) - (a.ਵਿੱਥ < b.ਵਿੱਥ);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (ਦਸ਼ਮਲਵ) hb_max ((x1 - (ਦੁੱਗਣਾ) cx) * (x1 - (ਦੁੱਗਣਾ) cx), (x2 - (ਦੁੱਗਣਾ) cx) * (x2 - (ਦੁੱਗਣਾ) cx));
  max_y = (ਦਸ਼ਮਲਵ) hb_max ((y1 - (ਦੁੱਗਣਾ) cy) * (y1 - (ਦੁੱਗਣਾ) cy), (y2 - (ਦੁੱਗਣਾ) cy) * (y2 - (ਦੁੱਗਣਾ) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_ਸਤਰ_get_extend (color_ਸਤਰ));
  ਨਮੂਨਾ = cairo_ਨਮੂਨਾ_create_mesh ();

  hb_cairo_ਜੋੜੋ_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, ਸ਼ੁਰੂ_angle, ਸਿਰਾ_angle, ਨਮੂਨਾ);

  cairo_ਸੈੱਟ_ਸਰੋਤ (cr, ਨਮੂਨਾ);
  cairo_paint (cr);

  cairo_ਨਮੂਨਾ_destroy (ਨਮੂਨਾ);

  ਜੇ (stops != stops_)
    hb_free (stops);
}

#ਜੇ_ਅੰਤ
