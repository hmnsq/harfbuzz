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

#അന്തര്ഭാവ "hb.hh"

#ifdef HAVE_CAIRO

#അന്തര്ഭാവ "hb-cairo-utils.hh"
#അന്തര്ഭാവ "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#define PREALLOCATED_COLOR_STOPS 16

തരനിർവചനം ഘടന {
  ദശാംശം r, g, b, a;
} hb_cairo_color_t;

സ്ഥിതം വരിയിൽ cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  തിരഞ്ഞെടുപ്പ് (extend)
    {
    അവസ്ഥ HB_PAINT_EXTEND_PAD: തിരികെ CAIRO_EXTEND_PAD;
    അവസ്ഥ HB_PAINT_EXTEND_REPEAT: തിരികെ CAIRO_EXTEND_REPEAT;
    അവസ്ഥ HB_PAINT_EXTEND_REFLECT: തിരികെ CAIRO_EXTEND_REFLECT;
    സ്ഥിരസ്ഥിതി: നിർത്തുക;
    }

  തിരികെ CAIRO_EXTEND_PAD;
}

#ifdef CAIRO_HAS_PNG_FUNCTIONS
തരനിർവചനം ഘടന
{
  hb_blob_t *blob;
  unsigned പൂർണ്ണസംഖ്യ offset;
} hb_cairo_read_blob_data_t;

സ്ഥിതം cairo_status_t
hb_cairo_read_blob (ശൂന്യം *closure,
		    unsigned അക്ഷരം *data,
		    unsigned പൂർണ്ണസംഖ്യ length)
{
  hb_cairo_read_blob_data_t *r = (hb_cairo_read_blob_data_t *) closure;
  സ്ഥിരം അക്ഷരം *d;
  unsigned പൂർണ്ണസംഖ്യ size;

  d = hb_blob_get_data (r->blob, &size);

  എങ്കിൽ (r->offset + length > size)
    തിരികെ CAIRO_STATUS_READ_ERROR;

  hb_memcpy (data, d + r->offset, length);
  r->offset += length;

  തിരികെ CAIRO_STATUS_SUCCESS;
}
#endif

സ്ഥിതം സ്ഥിരം cairo_user_data_താക്കോൽ_t *hb_cairo_surface_blob_user_data_താക്കോൽ = {0};

സ്ഥിതം ശൂന്യം
_hb_cairo_destroy_blob (ശൂന്യം *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

ശൂന്യം
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  എങ്കിൽ (use_foreground)
  {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
    ഇരട്ട r, g, b, a;
    cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
    എങ്കിൽ (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
      cairo_set_source_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    അല്ലെങ്കിൽ
#endif
      cairo_set_source_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  അല്ലെങ്കിൽ
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
			     HB_UNUSED ദശാംശം slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  എങ്കിൽ (!extents) /* SVG currently. */
    തിരികെ അസത്യം;

  cairo_surface_t *surface = nullptr;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
  എങ്കിൽ (format == HB_PAINT_IMAGE_FORMAT_PNG)
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
  അല്ലെങ്കിൽ
#endif
  എങ്കിൽ (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    unsigned data_size = hb_blob_get_length (blob);
    എങ്കിൽ (data_size < width * height * 4)
      തിരികെ അസത്യം;

    unsigned അക്ഷരം *data;
#ifdef __BYTE_ORDER
    എങ്കിൽ (__BYTE_ORDER == __BIG_ENDIAN)
    {
      data = (unsigned അക്ഷരം *) hb_blob_get_data_writable (blob, nullptr);
      എങ്കിൽ (!data)
        തിരികെ അസത്യം;

      unsigned എണ്ണം = width * height * 4;
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i += 4)
      {
        unsigned അക്ഷരം b;
	b = data[i];
	data[i] = data[i+3];
	data[i+3] = b;
	b = data[i+1];
	data[i+1] = data[i+2];
	data[i+2] = b;
      }
    }
    അല്ലെങ്കിൽ
#endif
      data = (unsigned അക്ഷരം *) hb_blob_get_data (blob, nullptr);

    surface = cairo_image_surface_create_for_data (data,
						   CAIRO_FORMAT_ARGB32,
						   width, height,
						   width * 4);

    cairo_surface_set_user_data (surface,
				 hb_cairo_surface_blob_user_data_താക്കോൽ,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  എങ്കിൽ (!surface)
    തിരികെ അസത്യം;

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

  cairo_matrix_t matrix = {(ഇരട്ട) width, 0, 0, (ഇരട്ട) height, 0, 0};
  cairo_pattern_set_matrix (pattern, &matrix);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->width, extents->height);
  cairo_set_source (cr, pattern);

  cairo_paint (cr);

  cairo_pattern_destroy (pattern);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  തിരികെ സത്യം;
}

സ്ഥിതം തർക്കം
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_വരി_t *color_വരി,
			   unsigned *എണ്ണം,
			   hb_color_stop_t **stops)
{
  unsigned len = hb_color_വരി_get_color_stops (color_വരി, 0, nullptr, nullptr);
  തർക്കം allocated = അസത്യം;
  എങ്കിൽ (unlikely (!len))
    തിരികെ അസത്യം;
  എങ്കിൽ (len > *എണ്ണം)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, വലുപ്പം (hb_color_stop_t));
    എങ്കിൽ (unlikely (!*stops))
      തിരികെ അസത്യം;
    allocated = സത്യം;
  }
  hb_color_വരി_get_color_stops (color_വരി, 0, &len, *stops);
  എങ്കിൽ (unlikely (!len))
  {
    എങ്കിൽ (allocated)
    {
      hb_free (*stops);
      *stops = nullptr;
    }
    തിരികെ അസത്യം;
  }
  വേണ്ടി (unsigned i = 0; i < len; i++)
    എങ്കിൽ ((*stops)[i].is_foreground)
    {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
      ഇരട്ട r, g, b, a;
      cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
      എങ്കിൽ (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      അല്ലെങ്കിൽ
#endif
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *എണ്ണം = len;
  തിരികെ സത്യം;
}

ശൂന്യം
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_വരി_t *color_വരി,
				 ദശാംശം x0, ദശാംശം y0,
				 ദശാംശം x1, ദശാംശം y1,
				 ദശാംശം x2, ദശാംശം y2)
{
  cairo_t *cr = c->cr;

  unsigned പൂർണ്ണസംഖ്യ len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  ദശാംശം xx0, yy0, xx1, yy1;
  ദശാംശം xxx0, yyy0, xxx1, yyy1;
  ദശാംശം min, max;
  cairo_pattern_t *pattern;

  എങ്കിൽ (unlikely (!_hb_cairo_get_color_stops (c, color_വരി, &len, &stops)))
    തിരികെ;
  hb_paint_normalize_color_വരി (stops, len, &min, &max);

  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  pattern = cairo_pattern_create_linear ((ഇരട്ട) xxx0, (ഇരട്ട) yyy0, (ഇരട്ട) xxx1, (ഇരട്ട) yyy1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_വരി_get_extend (color_വരി)));
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
    {
      ഇരട്ട r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (ഇരട്ട) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  എങ്കിൽ (stops != stops_)
    hb_free (stops);
}

ശൂന്യം
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_വരി_t *color_വരി,
				 ദശാംശം x0, ദശാംശം y0, ദശാംശം r0,
				 ദശാംശം x1, ദശാംശം y1, ദശാംശം r1)
{
  cairo_t *cr = c->cr;

  unsigned പൂർണ്ണസംഖ്യ len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  ദശാംശം min, max;
  ദശാംശം xx0, yy0, xx1, yy1;
  ദശാംശം rr0, rr1;
  cairo_pattern_t *pattern;

  എങ്കിൽ (unlikely (!_hb_cairo_get_color_stops (c, color_വരി, &len, &stops)))
    തിരികെ;
  hb_paint_normalize_color_വരി (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  pattern = cairo_pattern_create_radial ((ഇരട്ട) xx0, (ഇരട്ട) yy0, (ഇരട്ട) rr0, (ഇരട്ട) xx1, (ഇരട്ട) yy1, (ഇരട്ട) rr1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_വരി_get_extend (color_വരി)));

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < len; i++)
    {
      ഇരട്ട r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (ഇരട്ട) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  എങ്കിൽ (stops != stops_)
    hb_free (stops);
}

തരനിർവചനം ഘടന {
  ദശാംശം x, y;
} hb_cairo_point_t;

സ്ഥിതം വരിയിൽ ദശാംശം
_hb_cairo_interpolate (ദശാംശം f0, ദശാംശം f1, ദശാംശം f)
{
  തിരികെ f0 + f * (f1 - f0);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  എങ്കിൽ (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

സ്ഥിതം ശൂന്യം
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, ദശാംശം k, hb_cairo_color_t *c)
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

സ്ഥിതം വരിയിൽ ദശാംശം
_hb_cairo_dot (hb_cairo_point_t p, hb_cairo_point_t q)
{
  തിരികെ p.x * q.x + p.y * q.y;
}

സ്ഥിതം വരിയിൽ hb_cairo_point_t
_hb_cairo_normalize (hb_cairo_point_t p)
{
  ദശാംശം len = sqrtf (_hb_cairo_dot (p, p));

  തിരികെ hb_cairo_point_t { p.x / len, p.y / len };
}

സ്ഥിതം വരിയിൽ hb_cairo_point_t
_hb_cairo_sum (hb_cairo_point_t p, hb_cairo_point_t q)
{
  തിരികെ hb_cairo_point_t { p.x + q.x, p.y + q.y };
}

സ്ഥിതം വരിയിൽ hb_cairo_point_t
_hb_cairo_difference (hb_cairo_point_t p, hb_cairo_point_t q)
{
  തിരികെ hb_cairo_point_t { p.x - q.x, p.y - q.y };
}

സ്ഥിതം വരിയിൽ hb_cairo_point_t
_hb_cairo_scale (hb_cairo_point_t p, ദശാംശം f)
{
  തിരികെ hb_cairo_point_t { p.x * f, p.y * f };
}

തരനിർവചനം ഘടന {
  hb_cairo_point_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

സ്ഥിതം ശൂന്യം
_hb_cairo_add_patch (cairo_pattern_t *pattern, hb_cairo_point_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_pattern_begin_patch (pattern);
  cairo_mesh_pattern_move_to (pattern, (ഇരട്ട) center->x, (ഇരട്ട) center->y);
  cairo_mesh_pattern_വരി_to (pattern, (ഇരട്ട) p->p0.x, (ഇരട്ട) p->p0.y);
  cairo_mesh_pattern_curve_to (pattern,
                               (ഇരട്ട) p->c0.x, (ഇരട്ട) p->c0.y,
                               (ഇരട്ട) p->c1.x, (ഇരട്ട) p->c1.y,
                               (ഇരട്ട) p->p1.x, (ഇരട്ട) p->p1.y);
  cairo_mesh_pattern_വരി_to (pattern, (ഇരട്ട) center->x, (ഇരട്ട) center->y);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 0,
                                            (ഇരട്ട) p->color0.r,
                                            (ഇരട്ട) p->color0.g,
                                            (ഇരട്ട) p->color0.b,
                                            (ഇരട്ട) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 1,
                                            (ഇരട്ട) p->color0.r,
                                            (ഇരട്ട) p->color0.g,
                                            (ഇരട്ട) p->color0.b,
                                            (ഇരട്ട) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 2,
                                            (ഇരട്ട) p->color1.r,
                                            (ഇരട്ട) p->color1.g,
                                            (ഇരട്ട) p->color1.b,
                                            (ഇരട്ട) p->color1.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 3,
                                            (ഇരട്ട) p->color1.r,
                                            (ഇരട്ട) p->color1.g,
                                            (ഇരട്ട) p->color1.b,
                                            (ഇരട്ട) p->color1.a);
  cairo_mesh_pattern_end_patch (pattern);
}

#define MAX_ANGLE (HB_PI / 8.f)

സ്ഥിതം ശൂന്യം
_hb_cairo_add_sweep_gradient_patches1 (ദശാംശം cx, ദശാംശം cy, ദശാംശം radius,
				       ദശാംശം a0, hb_cairo_color_t *c0,
				       ദശാംശം a1, hb_cairo_color_t *c1,
				       cairo_pattern_t *pattern)
{
  hb_cairo_point_t center = hb_cairo_point_t { cx, cy };
  പൂർണ്ണസംഖ്യ അക്കം_splits;
  hb_cairo_point_t p0;
  hb_cairo_color_t color0, color1;

  അക്കം_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_point_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  വേണ്ടി (പൂർണ്ണസംഖ്യ a = 0; a < അക്കം_splits; a++)
    {
      ദശാംശം k = (a + 1.) / അക്കം_splits;
      ദശാംശം angle1;
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

സ്ഥിതം വരിയിൽ hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  തിരികെ {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

ഘടന _hb_cairo_sweep_patch_ctx {
  ദശാംശം cx, cy, radius;
  cairo_pattern_t *pattern;
};

സ്ഥിതം ശൂന്യം
_hb_cairo_sweep_emit_patch (ദശാംശം a0, hb_color_t c0,
			    ദശാംശം a1, hb_color_t c1,
			    ശൂന്യം *user_data)
{
  auto *ctx = (_hb_cairo_sweep_patch_ctx *) user_data;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  _hb_cairo_add_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->pattern);
}

സ്ഥിതം ശൂന്യം
_hb_cairo_add_sweep_gradient_patches (hb_color_stop_t *stops,
				      unsigned പൂർണ്ണസംഖ്യ n_stops,
				      cairo_extend_t extend,
				      ദശാംശം cx, ദശാംശം cy,
				      ദശാംശം radius,
				      ദശാംശം start_angle,
				      ദശാംശം end_angle,
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

ശൂന്യം
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_വരി_t *color_വരി,
				ദശാംശം cx, ദശാംശം cy,
				ദശാംശം start_angle,
				ദശാംശം end_angle)
{
  cairo_t *cr = c->cr;

  unsigned പൂർണ്ണസംഖ്യ len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  ഇരട്ട x1, y1, x2, y2;
  ദശാംശം max_x, max_y, radius;
  cairo_pattern_t *pattern;

  എങ്കിൽ (unlikely (!_hb_cairo_get_color_stops (c, color_വരി, &len, &stops)))
    തിരികെ;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (സ്ഥിരം hb_color_stop_t &a, സ്ഥിരം hb_color_stop_t &b) {
      തിരികെ (a.offset > b.offset) - (a.offset < b.offset);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (ദശാംശം) hb_max ((x1 - (ഇരട്ട) cx) * (x1 - (ഇരട്ട) cx), (x2 - (ഇരട്ട) cx) * (x2 - (ഇരട്ട) cx));
  max_y = (ദശാംശം) hb_max ((y1 - (ഇരട്ട) cy) * (y1 - (ഇരട്ട) cy), (y2 - (ഇരട്ട) cy) * (y2 - (ഇരട്ട) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_വരി_get_extend (color_വരി));
  pattern = cairo_pattern_create_mesh ();

  _hb_cairo_add_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, start_angle, end_angle, pattern);

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  എങ്കിൽ (stops != stops_)
    hb_free (stops);
}

#endif
