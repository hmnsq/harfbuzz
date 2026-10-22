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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifdef HAVE_CAIRO

#ಅನ್ತರ್ಭಾವ "hb-cairo-utils.hh"
#ಅನ್ತರ್ಭಾವ "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#define PREALLOCATED_COLOR_STOPS 16

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
  ಭಿನ್ನ r, g, b, a;
} hb_cairo_color_t;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  ವಿಕಲ್ಪ (extend)
    {
    ಪಕ್ಷ HB_PAINT_EXTEND_PAD: ಪ್ರತ್ಯಾಗಮ CAIRO_EXTEND_PAD;
    ಪಕ್ಷ HB_PAINT_EXTEND_REPEAT: ಪ್ರತ್ಯಾಗಮ CAIRO_EXTEND_REPEAT;
    ಪಕ್ಷ HB_PAINT_EXTEND_REFLECT: ಪ್ರತ್ಯಾಗಮ CAIRO_EXTEND_REFLECT;
    ಸ್ವಭಾವ: ಭಙ್ಗ;
    }

  ಪ್ರತ್ಯಾಗಮ CAIRO_EXTEND_PAD;
}

#ifdef CAIRO_HAS_PNG_FUNCTIONS
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  hb_blob_t *blob;
  unsigned ಪೂರ್ಣಾಙ್ಕ offset;
} hb_cairo_read_blob_data_t;

ಸ್ಥಿರ cairo_status_t
hb_cairo_read_blob (ಶೂನ್ಯ *closure,
		    unsigned ವರ್ಣ *data,
		    unsigned ಪೂರ್ಣಾಙ್ಕ length)
{
  hb_cairo_read_blob_data_t *r = (hb_cairo_read_blob_data_t *) closure;
  ಧ್ರುವ ವರ್ಣ *d;
  unsigned ಪೂರ್ಣಾಙ್ಕ size;

  d = hb_blob_get_data (r->blob, &size);

  ಯದಿ (r->offset + length > size)
    ಪ್ರತ್ಯಾಗಮ CAIRO_STATUS_READ_ERROR;

  hb_memcpy (data, d + r->offset, length);
  r->offset += length;

  ಪ್ರತ್ಯಾಗಮ CAIRO_STATUS_SUCCESS;
}
#endif

ಸ್ಥಿರ ಧ್ರುವ cairo_user_data_ಕೀಲಿ_t *hb_cairo_surface_blob_user_data_ಕೀಲಿ = {0};

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_cairo_destroy_blob (ಶೂನ್ಯ *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

ಶೂನ್ಯ
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  ಯದಿ (use_foreground)
  {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
    ದ್ವಿಗುಣ r, g, b, a;
    cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
    ಯದಿ (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
      cairo_set_source_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    ಅನ್ಯಥಾ
#endif
      cairo_set_source_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  ಅನ್ಯಥಾ
    cairo_set_source_rgba (cr,
			   hb_color_get_red (color) / 255.,
			   hb_color_get_green (color) / 255.,
			   hb_color_get_blue (color) / 255.,
			   hb_color_get_alpha (color) / 255.);
}

hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     unsigned width,
			     unsigned height,
			     hb_tag_t format,
			     HB_UNUSED ಭಿನ್ನ slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  ಯದಿ (!extents) /* SVG currently. */
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  cairo_surface_t *surface = nullptr;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
  ಯದಿ (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
    hb_cairo_read_blob_data_t r;
    r.blob = blob;
    r.offset = 0;
    surface = cairo_image_surface_create_from_png_stream (hb_cairo_read_blob, &r);

    /* For PNG, width,height can be unreliable, as is the case for NotoColorEmoji :(.
     * Just pull them out of the surface. */
    width = cairo_image_surface_get_width (surface);
    height = cairo_image_surface_get_width (surface);
  }
  ಅನ್ಯಥಾ
#endif
  ಯದಿ (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    unsigned data_size = hb_blob_get_length (blob);
    ಯದಿ (data_size < width * height * 4)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned ವರ್ಣ *data;
#ifdef __BYTE_ORDER
    ಯದಿ (__BYTE_ORDER == __BIG_ENDIAN)
    {
      data = (unsigned ವರ್ಣ *) hb_blob_get_data_writable (blob, nullptr);
      ಯದಿ (!data)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      unsigned ಎಣಿಕೆ = width * height * 4;
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i += 4)
      {
        unsigned ವರ್ಣ b;
	b = data[i];
	data[i] = data[i+3];
	data[i+3] = b;
	b = data[i+1];
	data[i+1] = data[i+2];
	data[i+2] = b;
      }
    }
    ಅನ್ಯಥಾ
#endif
      data = (unsigned ವರ್ಣ *) hb_blob_get_data (blob, nullptr);

    surface = cairo_image_surface_create_for_data (data,
						   CAIRO_FORMAT_ARGB32,
						   width, height,
						   width * 4);

    cairo_surface_set_user_data (surface,
				 hb_cairo_surface_blob_user_data_ಕೀಲಿ,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  ಯದಿ (!surface)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  cairo_save (cr);
  /* this clip is here to work around recording surface limitations */
  cairo_rectangle (cr,
                   extents->x_bearing,
                   extents->y_bearing,
                   extents->width,
                   extents->height);
  cairo_clip (cr);

  cairo_pattern_t *pattern = cairo_pattern_create_for_surface (surface);
  cairo_pattern_set_extend (pattern, CAIRO_EXTEND_PAD);

  cairo_matrix_t matrix = {(ದ್ವಿಗುಣ) width, 0, 0, (ದ್ವಿಗುಣ) height, 0, 0};
  cairo_pattern_set_matrix (pattern, &matrix);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->width, extents->height);
  cairo_set_source (cr, pattern);

  cairo_paint (cr);

  cairo_pattern_destroy (pattern);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_ಸಾಲು_t *color_ಸಾಲು,
			   unsigned *ಎಣಿಕೆ,
			   hb_color_stop_t **stops)
{
  unsigned len = hb_color_ಸಾಲು_get_color_stops (color_ಸಾಲು, 0, nullptr, nullptr);
  ತಾರ್ಕಿಕ allocated = ಅಸತ್ಯ;
  ಯದಿ (unlikely (!len))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಯದಿ (len > *ಎಣಿಕೆ)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, ಪರಿಮಾಣ (hb_color_stop_t));
    ಯದಿ (unlikely (!*stops))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    allocated = ಸತ್ಯ;
  }
  hb_color_ಸಾಲು_get_color_stops (color_ಸಾಲು, 0, &len, *stops);
  ಯದಿ (unlikely (!len))
  {
    ಯದಿ (allocated)
    {
      hb_free (*stops);
      *stops = nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ಪ್ರತಿ (unsigned i = 0; i < len; i++)
    ಯದಿ ((*stops)[i].is_foreground)
    {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
      ದ್ವಿಗುಣ r, g, b, a;
      cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
      ಯದಿ (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      ಅನ್ಯಥಾ
#endif
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *ಎಣಿಕೆ = len;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಶೂನ್ಯ
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_ಸಾಲು_t *color_ಸಾಲು,
				 ಭಿನ್ನ x0, ಭಿನ್ನ y0,
				 ಭಿನ್ನ x1, ಭಿನ್ನ y1,
				 ಭಿನ್ನ x2, ಭಿನ್ನ y2)
{
  cairo_t *cr = c->cr;

  unsigned ಪೂರ್ಣಾಙ್ಕ len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  ಭಿನ್ನ xx0, yy0, xx1, yy1;
  ಭಿನ್ನ xxx0, yyy0, xxx1, yyy1;
  ಭಿನ್ನ min, max;
  cairo_pattern_t *pattern;

  ಯದಿ (unlikely (!_hb_cairo_get_color_stops (c, color_ಸಾಲು, &len, &stops)))
    ಪ್ರತ್ಯಾಗಮ;
  hb_paint_normalize_color_ಸಾಲು (stops, len, &min, &max);

  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  pattern = cairo_pattern_create_linear ((ದ್ವಿಗುಣ) xxx0, (ದ್ವಿಗುಣ) yyy0, (ದ್ವಿಗುಣ) xxx1, (ದ್ವಿಗುಣ) yyy1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_ಸಾಲು_get_extend (color_ಸಾಲು)));
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
    {
      ದ್ವಿಗುಣ r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (ದ್ವಿಗುಣ) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  ಯದಿ (stops != stops_)
    hb_free (stops);
}

ಶೂನ್ಯ
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_ಸಾಲು_t *color_ಸಾಲು,
				 ಭಿನ್ನ x0, ಭಿನ್ನ y0, ಭಿನ್ನ r0,
				 ಭಿನ್ನ x1, ಭಿನ್ನ y1, ಭಿನ್ನ r1)
{
  cairo_t *cr = c->cr;

  unsigned ಪೂರ್ಣಾಙ್ಕ len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  ಭಿನ್ನ min, max;
  ಭಿನ್ನ xx0, yy0, xx1, yy1;
  ಭಿನ್ನ rr0, rr1;
  cairo_pattern_t *pattern;

  ಯದಿ (unlikely (!_hb_cairo_get_color_stops (c, color_ಸಾಲು, &len, &stops)))
    ಪ್ರತ್ಯಾಗಮ;
  hb_paint_normalize_color_ಸಾಲು (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  pattern = cairo_pattern_create_radial ((ದ್ವಿಗುಣ) xx0, (ದ್ವಿಗುಣ) yy0, (ದ್ವಿಗುಣ) rr0, (ದ್ವಿಗುಣ) xx1, (ದ್ವಿಗುಣ) yy1, (ದ್ವಿಗುಣ) rr1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_ಸಾಲು_get_extend (color_ಸಾಲು)));

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++)
    {
      ದ್ವಿಗುಣ r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (ದ್ವಿಗುಣ) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  ಯದಿ (stops != stops_)
    hb_free (stops);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
  ಭಿನ್ನ x, y;
} hb_cairo_point_t;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಭಿನ್ನ
_hb_cairo_interpolate (ಭಿನ್ನ f0, ಭಿನ್ನ f1, ಭಿನ್ನ f)
{
  ಪ್ರತ್ಯಾಗಮ f0 + f * (f1 - f0);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  ಯದಿ (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, ಭಿನ್ನ k, hb_cairo_color_t *c)
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

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಭಿನ್ನ
_hb_cairo_dot (hb_cairo_point_t p, hb_cairo_point_t q)
{
  ಪ್ರತ್ಯಾಗಮ p.x * q.x + p.y * q.y;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_cairo_point_t
_hb_cairo_normalize (hb_cairo_point_t p)
{
  ಭಿನ್ನ len = sqrtf (_hb_cairo_dot (p, p));

  ಪ್ರತ್ಯಾಗಮ hb_cairo_point_t { p.x / len, p.y / len };
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_cairo_point_t
_hb_cairo_sum (hb_cairo_point_t p, hb_cairo_point_t q)
{
  ಪ್ರತ್ಯಾಗಮ hb_cairo_point_t { p.x + q.x, p.y + q.y };
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_cairo_point_t
_hb_cairo_difference (hb_cairo_point_t p, hb_cairo_point_t q)
{
  ಪ್ರತ್ಯಾಗಮ hb_cairo_point_t { p.x - q.x, p.y - q.y };
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_cairo_point_t
_hb_cairo_scale (hb_cairo_point_t p, ಭಿನ್ನ f)
{
  ಪ್ರತ್ಯಾಗಮ hb_cairo_point_t { p.x * f, p.y * f };
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
  hb_cairo_point_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_cairo_add_patch (cairo_pattern_t *pattern, hb_cairo_point_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_pattern_begin_patch (pattern);
  cairo_mesh_pattern_move_to (pattern, (ದ್ವಿಗುಣ) center->x, (ದ್ವಿಗುಣ) center->y);
  cairo_mesh_pattern_ಸಾಲು_to (pattern, (ದ್ವಿಗುಣ) p->p0.x, (ದ್ವಿಗುಣ) p->p0.y);
  cairo_mesh_pattern_curve_to (pattern,
                               (ದ್ವಿಗುಣ) p->c0.x, (ದ್ವಿಗುಣ) p->c0.y,
                               (ದ್ವಿಗುಣ) p->c1.x, (ದ್ವಿಗುಣ) p->c1.y,
                               (ದ್ವಿಗುಣ) p->p1.x, (ದ್ವಿಗುಣ) p->p1.y);
  cairo_mesh_pattern_ಸಾಲು_to (pattern, (ದ್ವಿಗುಣ) center->x, (ದ್ವಿಗುಣ) center->y);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 0,
                                            (ದ್ವಿಗುಣ) p->color0.r,
                                            (ದ್ವಿಗುಣ) p->color0.g,
                                            (ದ್ವಿಗುಣ) p->color0.b,
                                            (ದ್ವಿಗುಣ) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 1,
                                            (ದ್ವಿಗುಣ) p->color0.r,
                                            (ದ್ವಿಗುಣ) p->color0.g,
                                            (ದ್ವಿಗುಣ) p->color0.b,
                                            (ದ್ವಿಗುಣ) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 2,
                                            (ದ್ವಿಗುಣ) p->color1.r,
                                            (ದ್ವಿಗುಣ) p->color1.g,
                                            (ದ್ವಿಗುಣ) p->color1.b,
                                            (ದ್ವಿಗುಣ) p->color1.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 3,
                                            (ದ್ವಿಗುಣ) p->color1.r,
                                            (ದ್ವಿಗುಣ) p->color1.g,
                                            (ದ್ವಿಗುಣ) p->color1.b,
                                            (ದ್ವಿಗುಣ) p->color1.a);
  cairo_mesh_pattern_end_patch (pattern);
}

#define MAX_ANGLE (HB_PI / 8.f)

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_cairo_add_sweep_gradient_patches1 (ಭಿನ್ನ cx, ಭಿನ್ನ cy, ಭಿನ್ನ radius,
				       ಭಿನ್ನ a0, hb_cairo_color_t *c0,
				       ಭಿನ್ನ a1, hb_cairo_color_t *c1,
				       cairo_pattern_t *pattern)
{
  hb_cairo_point_t center = hb_cairo_point_t { cx, cy };
  ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_splits;
  hb_cairo_point_t p0;
  hb_cairo_color_t color0, color1;

  ಅಂಕೆ_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_point_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ a = 0; a < ಅಂಕೆ_splits; a++)
    {
      ಭಿನ್ನ k = (a + 1.) / ಅಂಕೆ_splits;
      ಭಿನ್ನ angle1;
      hb_cairo_point_t p1;
      hb_cairo_point_t A, U;
      hb_cairo_point_t C0, C1;
      hb_cairo_patch_t patch;

      angle1 = _hb_cairo_interpolate (a0, a1, k);
      _hb_cairo_interpolate_colors (c0, c1, k, &color1);

      patch.color0 = color0;
      patch.color1 = color1;

      p1 = hb_cairo_point_t { cosf (angle1), sinf (angle1) };
      patch.p0 = _hb_cairo_sum (center, _hb_cairo_scale (p0, radius));
      patch.p1 = _hb_cairo_sum (center, _hb_cairo_scale (p1, radius));

      A = _hb_cairo_normalize (_hb_cairo_sum (p0, p1));
      U = hb_cairo_point_t { -A.y, A.x };
      C0 = _hb_cairo_sum (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p0, A), p0) / _hb_cairo_dot (U, p0)));
      C1 = _hb_cairo_sum (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p1, A), p1) / _hb_cairo_dot (U, p1)));

      patch.c0 = _hb_cairo_sum (center, _hb_cairo_scale (_hb_cairo_sum (C0, _hb_cairo_scale (_hb_cairo_difference (C0, p0), 0.33333f)), radius));
      patch.c1 = _hb_cairo_sum (center, _hb_cairo_scale (_hb_cairo_sum (C1, _hb_cairo_scale (_hb_cairo_difference (C1, p1), 0.33333f)), radius));

      _hb_cairo_add_patch (pattern, &center, &patch);

      p0 = p1;
      color0 = color1;
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  ಪ್ರತ್ಯಾಗಮ {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

ಸಂರಚನಾ _hb_cairo_sweep_patch_ctx {
  ಭಿನ್ನ cx, cy, radius;
  cairo_pattern_t *pattern;
};

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_cairo_sweep_emit_patch (ಭಿನ್ನ a0, hb_color_t c0,
			    ಭಿನ್ನ a1, hb_color_t c1,
			    ಶೂನ್ಯ *user_data)
{
  auto *ctx = (_hb_cairo_sweep_patch_ctx *) user_data;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  _hb_cairo_add_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->pattern);
}

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_cairo_add_sweep_gradient_patches (hb_color_stop_t *stops,
				      unsigned ಪೂರ್ಣಾಙ್ಕ n_stops,
				      cairo_extend_t extend,
				      ಭಿನ್ನ cx, ಭಿನ್ನ cy,
				      ಭಿನ್ನ radius,
				      ಭಿನ್ನ start_angle,
				      ಭಿನ್ನ end_angle,
				      cairo_pattern_t *pattern)
{
  hb_paint_extend_t hb_extend =
    extend == CAIRO_EXTEND_REPEAT  ? HB_PAINT_EXTEND_REPEAT  :
    extend == CAIRO_EXTEND_REFLECT ? HB_PAINT_EXTEND_REFLECT :
				     HB_PAINT_EXTEND_PAD;

  _hb_cairo_sweep_patch_ctx ctx { cx, cy, radius, pattern };
  hb_paint_sweep_gradient_tiles (stops, n_stops, hb_extend,
				 start_angle, end_angle,
				 _hb_cairo_sweep_emit_patch, &ctx);
}

ಶೂನ್ಯ
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_ಸಾಲು_t *color_ಸಾಲು,
				ಭಿನ್ನ cx, ಭಿನ್ನ cy,
				ಭಿನ್ನ start_angle,
				ಭಿನ್ನ end_angle)
{
  cairo_t *cr = c->cr;

  unsigned ಪೂರ್ಣಾಙ್ಕ len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  ದ್ವಿಗುಣ x1, y1, x2, y2;
  ಭಿನ್ನ max_x, max_y, radius;
  cairo_pattern_t *pattern;

  ಯದಿ (unlikely (!_hb_cairo_get_color_stops (c, color_ಸಾಲು, &len, &stops)))
    ಪ್ರತ್ಯಾಗಮ;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (ಧ್ರುವ hb_color_stop_t &a, ಧ್ರುವ hb_color_stop_t &b) {
      ಪ್ರತ್ಯಾಗಮ (a.offset > b.offset) - (a.offset < b.offset);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (ಭಿನ್ನ) hb_max ((x1 - (ದ್ವಿಗುಣ) cx) * (x1 - (ದ್ವಿಗುಣ) cx), (x2 - (ದ್ವಿಗುಣ) cx) * (x2 - (ದ್ವಿಗುಣ) cx));
  max_y = (ಭಿನ್ನ) hb_max ((y1 - (ದ್ವಿಗುಣ) cy) * (y1 - (ದ್ವಿಗುಣ) cy), (y2 - (ದ್ವಿಗುಣ) cy) * (y2 - (ದ್ವಿಗುಣ) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_ಸಾಲು_get_extend (color_ಸಾಲು));
  pattern = cairo_pattern_create_mesh ();

  _hb_cairo_add_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, start_angle, end_angle, pattern);

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  ಯದಿ (stops != stops_)
    hb_free (stops);
}

#endif
