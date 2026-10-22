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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_CAIRO

#अन्तर्भाव "hb-cairo-utils.hh"
#अन्तर्भाव "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#define PREALLOCATED_COLOR_STOPS 16

प्रकारनिर्देश संरचना {
  भिन्न r, g, b, a;
} hb_cairo_color_t;

स्थिर अन्तःस्थ cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  विकल्प (extend)
    {
    पक्ष HB_PAINT_EXTEND_PAD: प्रत्यागम CAIRO_EXTEND_PAD;
    पक्ष HB_PAINT_EXTEND_REPEAT: प्रत्यागम CAIRO_EXTEND_REPEAT;
    पक्ष HB_PAINT_EXTEND_REFLECT: प्रत्यागम CAIRO_EXTEND_REFLECT;
    स्वभाव: भङ्ग;
    }

  प्रत्यागम CAIRO_EXTEND_PAD;
}

#ifdef CAIRO_HAS_PNG_FUNCTIONS
प्रकारनिर्देश संरचना
{
  hb_blob_t *blob;
  unsigned पूर्णाङ्क offset;
} hb_cairo_read_blob_data_t;

स्थिर cairo_status_t
hb_cairo_read_blob (शून्य *closure,
		    unsigned वर्ण *data,
		    unsigned पूर्णाङ्क length)
{
  hb_cairo_read_blob_data_t *r = (hb_cairo_read_blob_data_t *) closure;
  ध्रुव वर्ण *d;
  unsigned पूर्णाङ्क size;

  d = hb_blob_get_data (r->blob, &size);

  जइ (r->offset + length > size)
    प्रत्यागम CAIRO_STATUS_READ_ERROR;

  hb_memcpy (data, d + r->offset, length);
  r->offset += length;

  प्रत्यागम CAIRO_STATUS_SUCCESS;
}
#endif

स्थिर ध्रुव cairo_user_data_कुञ्चिका_t *hb_cairo_surface_blob_user_data_कुञ्चिका = {0};

स्थिर शून्य
_hb_cairo_destroy_blob (शून्य *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

शून्य
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  जइ (use_foreground)
  {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
    द्विगुण r, g, b, a;
    cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
    जइ (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
      cairo_set_source_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    अण्णहा
#endif
      cairo_set_source_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  अण्णहा
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
			     HB_UNUSED भिन्न slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  जइ (!extents) /* SVG currently. */
    प्रत्यागम मिच्छ;

  cairo_surface_t *surface = nullptr;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
  जइ (format == HB_PAINT_IMAGE_FORMAT_PNG)
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
  अण्णहा
#endif
  जइ (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    unsigned data_size = hb_blob_get_length (blob);
    जइ (data_size < width * height * 4)
      प्रत्यागम मिच्छ;

    unsigned वर्ण *data;
#ifdef __BYTE_ORDER
    जइ (__BYTE_ORDER == __BIG_ENDIAN)
    {
      data = (unsigned वर्ण *) hb_blob_get_data_writable (blob, nullptr);
      जइ (!data)
        प्रत्यागम मिच्छ;

      unsigned गणनम् = width * height * 4;
      प्रति (unsigned i = 0; i < गणनम्; i += 4)
      {
        unsigned वर्ण b;
	b = data[i];
	data[i] = data[i+3];
	data[i+3] = b;
	b = data[i+1];
	data[i+1] = data[i+2];
	data[i+2] = b;
      }
    }
    अण्णहा
#endif
      data = (unsigned वर्ण *) hb_blob_get_data (blob, nullptr);

    surface = cairo_image_surface_create_for_data (data,
						   CAIRO_FORMAT_ARGB32,
						   width, height,
						   width * 4);

    cairo_surface_set_user_data (surface,
				 hb_cairo_surface_blob_user_data_कुञ्चिका,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  जइ (!surface)
    प्रत्यागम मिच्छ;

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

  cairo_matrix_t matrix = {(द्विगुण) width, 0, 0, (द्विगुण) height, 0, 0};
  cairo_pattern_set_matrix (pattern, &matrix);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->width, extents->height);
  cairo_set_source (cr, pattern);

  cairo_paint (cr);

  cairo_pattern_destroy (pattern);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  प्रत्यागम सच्च;
}

स्थिर तार्किक
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_पङ्क्तिः_t *color_पङ्क्तिः,
			   unsigned *गणनम्,
			   hb_color_stop_t **stops)
{
  unsigned len = hb_color_पङ्क्तिः_get_color_stops (color_पङ्क्तिः, 0, nullptr, nullptr);
  तार्किक allocated = मिच्छ;
  जइ (unlikely (!len))
    प्रत्यागम मिच्छ;
  जइ (len > *गणनम्)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, परिमाण (hb_color_stop_t));
    जइ (unlikely (!*stops))
      प्रत्यागम मिच्छ;
    allocated = सच्च;
  }
  hb_color_पङ्क्तिः_get_color_stops (color_पङ्क्तिः, 0, &len, *stops);
  जइ (unlikely (!len))
  {
    जइ (allocated)
    {
      hb_free (*stops);
      *stops = nullptr;
    }
    प्रत्यागम मिच्छ;
  }
  प्रति (unsigned i = 0; i < len; i++)
    जइ ((*stops)[i].is_foreground)
    {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
      द्विगुण r, g, b, a;
      cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
      जइ (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      अण्णहा
#endif
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *गणनम् = len;
  प्रत्यागम सच्च;
}

शून्य
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_पङ्क्तिः_t *color_पङ्क्तिः,
				 भिन्न x0, भिन्न y0,
				 भिन्न x1, भिन्न y1,
				 भिन्न x2, भिन्न y2)
{
  cairo_t *cr = c->cr;

  unsigned पूर्णाङ्क len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  भिन्न xx0, yy0, xx1, yy1;
  भिन्न xxx0, yyy0, xxx1, yyy1;
  भिन्न min, max;
  cairo_pattern_t *pattern;

  जइ (unlikely (!_hb_cairo_get_color_stops (c, color_पङ्क्तिः, &len, &stops)))
    प्रत्यागम;
  hb_paint_normalize_color_पङ्क्तिः (stops, len, &min, &max);

  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  pattern = cairo_pattern_create_linear ((द्विगुण) xxx0, (द्विगुण) yyy0, (द्विगुण) xxx1, (द्विगुण) yyy1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_पङ्क्तिः_get_extend (color_पङ्क्तिः)));
  प्रति (unsigned पूर्णाङ्क i = 0; i < len; i++)
    {
      द्विगुण r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (द्विगुण) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  जइ (stops != stops_)
    hb_free (stops);
}

शून्य
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_पङ्क्तिः_t *color_पङ्क्तिः,
				 भिन्न x0, भिन्न y0, भिन्न r0,
				 भिन्न x1, भिन्न y1, भिन्न r1)
{
  cairo_t *cr = c->cr;

  unsigned पूर्णाङ्क len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  भिन्न min, max;
  भिन्न xx0, yy0, xx1, yy1;
  भिन्न rr0, rr1;
  cairo_pattern_t *pattern;

  जइ (unlikely (!_hb_cairo_get_color_stops (c, color_पङ्क्तिः, &len, &stops)))
    प्रत्यागम;
  hb_paint_normalize_color_पङ्क्तिः (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  pattern = cairo_pattern_create_radial ((द्विगुण) xx0, (द्विगुण) yy0, (द्विगुण) rr0, (द्विगुण) xx1, (द्विगुण) yy1, (द्विगुण) rr1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_पङ्क्तिः_get_extend (color_पङ्क्तिः)));

  प्रति (unsigned पूर्णाङ्क i = 0; i < len; i++)
    {
      द्विगुण r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (द्विगुण) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  जइ (stops != stops_)
    hb_free (stops);
}

प्रकारनिर्देश संरचना {
  भिन्न x, y;
} hb_cairo_point_t;

स्थिर अन्तःस्थ भिन्न
_hb_cairo_interpolate (भिन्न f0, भिन्न f1, भिन्न f)
{
  प्रत्यागम f0 + f * (f1 - f0);
}

स्थिर अन्तःस्थ शून्य
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

स्थिर अन्तःस्थ शून्य
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  जइ (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

स्थिर शून्य
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, भिन्न k, hb_cairo_color_t *c)
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

स्थिर अन्तःस्थ भिन्न
_hb_cairo_dot (hb_cairo_point_t p, hb_cairo_point_t q)
{
  प्रत्यागम p.x * q.x + p.y * q.y;
}

स्थिर अन्तःस्थ hb_cairo_point_t
_hb_cairo_normalize (hb_cairo_point_t p)
{
  भिन्न len = sqrtf (_hb_cairo_dot (p, p));

  प्रत्यागम hb_cairo_point_t { p.x / len, p.y / len };
}

स्थिर अन्तःस्थ hb_cairo_point_t
_hb_cairo_sum (hb_cairo_point_t p, hb_cairo_point_t q)
{
  प्रत्यागम hb_cairo_point_t { p.x + q.x, p.y + q.y };
}

स्थिर अन्तःस्थ hb_cairo_point_t
_hb_cairo_difference (hb_cairo_point_t p, hb_cairo_point_t q)
{
  प्रत्यागम hb_cairo_point_t { p.x - q.x, p.y - q.y };
}

स्थिर अन्तःस्थ hb_cairo_point_t
_hb_cairo_scale (hb_cairo_point_t p, भिन्न f)
{
  प्रत्यागम hb_cairo_point_t { p.x * f, p.y * f };
}

प्रकारनिर्देश संरचना {
  hb_cairo_point_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

स्थिर शून्य
_hb_cairo_add_patch (cairo_pattern_t *pattern, hb_cairo_point_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_pattern_begin_patch (pattern);
  cairo_mesh_pattern_move_to (pattern, (द्विगुण) center->x, (द्विगुण) center->y);
  cairo_mesh_pattern_पङ्क्तिः_to (pattern, (द्विगुण) p->p0.x, (द्विगुण) p->p0.y);
  cairo_mesh_pattern_curve_to (pattern,
                               (द्विगुण) p->c0.x, (द्विगुण) p->c0.y,
                               (द्विगुण) p->c1.x, (द्विगुण) p->c1.y,
                               (द्विगुण) p->p1.x, (द्विगुण) p->p1.y);
  cairo_mesh_pattern_पङ्क्तिः_to (pattern, (द्विगुण) center->x, (द्विगुण) center->y);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 0,
                                            (द्विगुण) p->color0.r,
                                            (द्विगुण) p->color0.g,
                                            (द्विगुण) p->color0.b,
                                            (द्विगुण) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 1,
                                            (द्विगुण) p->color0.r,
                                            (द्विगुण) p->color0.g,
                                            (द्विगुण) p->color0.b,
                                            (द्विगुण) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 2,
                                            (द्विगुण) p->color1.r,
                                            (द्विगुण) p->color1.g,
                                            (द्विगुण) p->color1.b,
                                            (द्विगुण) p->color1.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 3,
                                            (द्विगुण) p->color1.r,
                                            (द्विगुण) p->color1.g,
                                            (द्विगुण) p->color1.b,
                                            (द्विगुण) p->color1.a);
  cairo_mesh_pattern_end_patch (pattern);
}

#define MAX_ANGLE (HB_PI / 8.f)

स्थिर शून्य
_hb_cairo_add_sweep_gradient_patches1 (भिन्न cx, भिन्न cy, भिन्न radius,
				       भिन्न a0, hb_cairo_color_t *c0,
				       भिन्न a1, hb_cairo_color_t *c1,
				       cairo_pattern_t *pattern)
{
  hb_cairo_point_t center = hb_cairo_point_t { cx, cy };
  पूर्णाङ्क अङ्कः_splits;
  hb_cairo_point_t p0;
  hb_cairo_color_t color0, color1;

  अङ्कः_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_point_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  प्रति (पूर्णाङ्क a = 0; a < अङ्कः_splits; a++)
    {
      भिन्न k = (a + 1.) / अङ्कः_splits;
      भिन्न angle1;
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

स्थिर अन्तःस्थ hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  प्रत्यागम {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

संरचना _hb_cairo_sweep_patch_ctx {
  भिन्न cx, cy, radius;
  cairo_pattern_t *pattern;
};

स्थिर शून्य
_hb_cairo_sweep_emit_patch (भिन्न a0, hb_color_t c0,
			    भिन्न a1, hb_color_t c1,
			    शून्य *user_data)
{
  auto *ctx = (_hb_cairo_sweep_patch_ctx *) user_data;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  _hb_cairo_add_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->pattern);
}

स्थिर शून्य
_hb_cairo_add_sweep_gradient_patches (hb_color_stop_t *stops,
				      unsigned पूर्णाङ्क n_stops,
				      cairo_extend_t extend,
				      भिन्न cx, भिन्न cy,
				      भिन्न radius,
				      भिन्न start_angle,
				      भिन्न end_angle,
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

शून्य
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_पङ्क्तिः_t *color_पङ्क्तिः,
				भिन्न cx, भिन्न cy,
				भिन्न start_angle,
				भिन्न end_angle)
{
  cairo_t *cr = c->cr;

  unsigned पूर्णाङ्क len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  द्विगुण x1, y1, x2, y2;
  भिन्न max_x, max_y, radius;
  cairo_pattern_t *pattern;

  जइ (unlikely (!_hb_cairo_get_color_stops (c, color_पङ्क्तिः, &len, &stops)))
    प्रत्यागम;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (ध्रुव hb_color_stop_t &a, ध्रुव hb_color_stop_t &b) {
      प्रत्यागम (a.offset > b.offset) - (a.offset < b.offset);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (भिन्न) hb_max ((x1 - (द्विगुण) cx) * (x1 - (द्विगुण) cx), (x2 - (द्विगुण) cx) * (x2 - (द्विगुण) cx));
  max_y = (भिन्न) hb_max ((y1 - (द्विगुण) cy) * (y1 - (द्विगुण) cy), (y2 - (द्विगुण) cy) * (y2 - (द्विगुण) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_पङ्क्तिः_get_extend (color_पङ्क्तिः));
  pattern = cairo_pattern_create_mesh ();

  _hb_cairo_add_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, start_angle, end_angle, pattern);

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  जइ (stops != stops_)
    hb_free (stops);
}

#endif
