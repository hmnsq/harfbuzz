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

#અન્તર્ભાવ "hb.hh"

#ifdef HAVE_CAIRO

#અન્તર્ભાવ "hb-cairo-utils.hh"
#અન્તર્ભાવ "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#define PREALLOCATED_COLOR_STOPS 16

પ્રકારવ્યાખ્યા સંરચના {
  દશાંશ r, g, b, a;
} hb_cairo_color_t;

સ્થિર અંતઃરેખ cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  પસંદગી (extend)
    {
    સ્થિતિ HB_PAINT_EXTEND_PAD: પાછા CAIRO_EXTEND_PAD;
    સ્થિતિ HB_PAINT_EXTEND_REPEAT: પાછા CAIRO_EXTEND_REPEAT;
    સ્થિતિ HB_PAINT_EXTEND_REFLECT: પાછા CAIRO_EXTEND_REFLECT;
    મૂળભૂત: તોડો;
    }

  પાછા CAIRO_EXTEND_PAD;
}

#ifdef CAIRO_HAS_PNG_FUNCTIONS
પ્રકારવ્યાખ્યા સંરચના
{
  hb_blob_t *blob;
  unsigned પૂર્ણાંક offset;
} hb_cairo_read_blob_data_t;

સ્થિર cairo_status_t
hb_cairo_read_blob (શૂન્ય *closure,
		    unsigned અક્ષર *data,
		    unsigned પૂર્ણાંક length)
{
  hb_cairo_read_blob_data_t *r = (hb_cairo_read_blob_data_t *) closure;
  અચલ અક્ષર *d;
  unsigned પૂર્ણાંક size;

  d = hb_blob_get_data (r->blob, &size);

  જો (r->offset + length > size)
    પાછા CAIRO_STATUS_READ_ERROR;

  hb_memcpy (data, d + r->offset, length);
  r->offset += length;

  પાછા CAIRO_STATUS_SUCCESS;
}
#endif

સ્થિર અચલ cairo_user_data_ચાવી_t *hb_cairo_surface_blob_user_data_ચાવી = {0};

સ્થિર શૂન્ય
_hb_cairo_destroy_blob (શૂન્ય *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

શૂન્ય
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  જો (use_foreground)
  {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
    દ્વિગુણ r, g, b, a;
    cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
    જો (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
      cairo_set_source_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    નહીંતર
#endif
      cairo_set_source_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  નહીંતર
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
			     HB_UNUSED દશાંશ slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  જો (!extents) /* SVG currently. */
    પાછા ખોટું;

  cairo_surface_t *surface = nullptr;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
  જો (format == HB_PAINT_IMAGE_FORMAT_PNG)
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
  નહીંતર
#endif
  જો (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    unsigned data_size = hb_blob_get_length (blob);
    જો (data_size < width * height * 4)
      પાછા ખોટું;

    unsigned અક્ષર *data;
#ifdef __BYTE_ORDER
    જો (__BYTE_ORDER == __BIG_ENDIAN)
    {
      data = (unsigned અક્ષર *) hb_blob_get_data_writable (blob, nullptr);
      જો (!data)
        પાછા ખોટું;

      unsigned ગણતરી = width * height * 4;
      માટે (unsigned i = 0; i < ગણતરી; i += 4)
      {
        unsigned અક્ષર b;
	b = data[i];
	data[i] = data[i+3];
	data[i+3] = b;
	b = data[i+1];
	data[i+1] = data[i+2];
	data[i+2] = b;
      }
    }
    નહીંતર
#endif
      data = (unsigned અક્ષર *) hb_blob_get_data (blob, nullptr);

    surface = cairo_image_surface_create_for_data (data,
						   CAIRO_FORMAT_ARGB32,
						   width, height,
						   width * 4);

    cairo_surface_set_user_data (surface,
				 hb_cairo_surface_blob_user_data_ચાવી,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  જો (!surface)
    પાછા ખોટું;

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

  cairo_matrix_t matrix = {(દ્વિગુણ) width, 0, 0, (દ્વિગુણ) height, 0, 0};
  cairo_pattern_set_matrix (pattern, &matrix);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->width, extents->height);
  cairo_set_source (cr, pattern);

  cairo_paint (cr);

  cairo_pattern_destroy (pattern);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  પાછા સાચું;
}

સ્થિર તાર્કિક
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_લીટી_t *color_લીટી,
			   unsigned *ગણતરી,
			   hb_color_stop_t **stops)
{
  unsigned len = hb_color_લીટી_get_color_stops (color_લીટી, 0, nullptr, nullptr);
  તાર્કિક allocated = ખોટું;
  જો (unlikely (!len))
    પાછા ખોટું;
  જો (len > *ગણતરી)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, કદ (hb_color_stop_t));
    જો (unlikely (!*stops))
      પાછા ખોટું;
    allocated = સાચું;
  }
  hb_color_લીટી_get_color_stops (color_લીટી, 0, &len, *stops);
  જો (unlikely (!len))
  {
    જો (allocated)
    {
      hb_free (*stops);
      *stops = nullptr;
    }
    પાછા ખોટું;
  }
  માટે (unsigned i = 0; i < len; i++)
    જો ((*stops)[i].is_foreground)
    {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
      દ્વિગુણ r, g, b, a;
      cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
      જો (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      નહીંતર
#endif
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *ગણતરી = len;
  પાછા સાચું;
}

શૂન્ય
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_લીટી_t *color_લીટી,
				 દશાંશ x0, દશાંશ y0,
				 દશાંશ x1, દશાંશ y1,
				 દશાંશ x2, દશાંશ y2)
{
  cairo_t *cr = c->cr;

  unsigned પૂર્ણાંક len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  દશાંશ xx0, yy0, xx1, yy1;
  દશાંશ xxx0, yyy0, xxx1, yyy1;
  દશાંશ min, max;
  cairo_pattern_t *pattern;

  જો (unlikely (!_hb_cairo_get_color_stops (c, color_લીટી, &len, &stops)))
    પાછા;
  hb_paint_normalize_color_લીટી (stops, len, &min, &max);

  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  pattern = cairo_pattern_create_linear ((દ્વિગુણ) xxx0, (દ્વિગુણ) yyy0, (દ્વિગુણ) xxx1, (દ્વિગુણ) yyy1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_લીટી_get_extend (color_લીટી)));
  માટે (unsigned પૂર્ણાંક i = 0; i < len; i++)
    {
      દ્વિગુણ r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (દ્વિગુણ) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  જો (stops != stops_)
    hb_free (stops);
}

શૂન્ય
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_લીટી_t *color_લીટી,
				 દશાંશ x0, દશાંશ y0, દશાંશ r0,
				 દશાંશ x1, દશાંશ y1, દશાંશ r1)
{
  cairo_t *cr = c->cr;

  unsigned પૂર્ણાંક len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  દશાંશ min, max;
  દશાંશ xx0, yy0, xx1, yy1;
  દશાંશ rr0, rr1;
  cairo_pattern_t *pattern;

  જો (unlikely (!_hb_cairo_get_color_stops (c, color_લીટી, &len, &stops)))
    પાછા;
  hb_paint_normalize_color_લીટી (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  pattern = cairo_pattern_create_radial ((દ્વિગુણ) xx0, (દ્વિગુણ) yy0, (દ્વિગુણ) rr0, (દ્વિગુણ) xx1, (દ્વિગુણ) yy1, (દ્વિગુણ) rr1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_લીટી_get_extend (color_લીટી)));

  માટે (unsigned પૂર્ણાંક i = 0; i < len; i++)
    {
      દ્વિગુણ r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (દ્વિગુણ) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  જો (stops != stops_)
    hb_free (stops);
}

પ્રકારવ્યાખ્યા સંરચના {
  દશાંશ x, y;
} hb_cairo_point_t;

સ્થિર અંતઃરેખ દશાંશ
_hb_cairo_interpolate (દશાંશ f0, દશાંશ f1, દશાંશ f)
{
  પાછા f0 + f * (f1 - f0);
}

સ્થિર અંતઃરેખ શૂન્ય
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

સ્થિર અંતઃરેખ શૂન્ય
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  જો (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

સ્થિર શૂન્ય
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, દશાંશ k, hb_cairo_color_t *c)
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

સ્થિર અંતઃરેખ દશાંશ
_hb_cairo_dot (hb_cairo_point_t p, hb_cairo_point_t q)
{
  પાછા p.x * q.x + p.y * q.y;
}

સ્થિર અંતઃરેખ hb_cairo_point_t
_hb_cairo_normalize (hb_cairo_point_t p)
{
  દશાંશ len = sqrtf (_hb_cairo_dot (p, p));

  પાછા hb_cairo_point_t { p.x / len, p.y / len };
}

સ્થિર અંતઃરેખ hb_cairo_point_t
_hb_cairo_sum (hb_cairo_point_t p, hb_cairo_point_t q)
{
  પાછા hb_cairo_point_t { p.x + q.x, p.y + q.y };
}

સ્થિર અંતઃરેખ hb_cairo_point_t
_hb_cairo_difference (hb_cairo_point_t p, hb_cairo_point_t q)
{
  પાછા hb_cairo_point_t { p.x - q.x, p.y - q.y };
}

સ્થિર અંતઃરેખ hb_cairo_point_t
_hb_cairo_scale (hb_cairo_point_t p, દશાંશ f)
{
  પાછા hb_cairo_point_t { p.x * f, p.y * f };
}

પ્રકારવ્યાખ્યા સંરચના {
  hb_cairo_point_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

સ્થિર શૂન્ય
_hb_cairo_add_patch (cairo_pattern_t *pattern, hb_cairo_point_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_pattern_begin_patch (pattern);
  cairo_mesh_pattern_move_to (pattern, (દ્વિગુણ) center->x, (દ્વિગુણ) center->y);
  cairo_mesh_pattern_લીટી_to (pattern, (દ્વિગુણ) p->p0.x, (દ્વિગુણ) p->p0.y);
  cairo_mesh_pattern_curve_to (pattern,
                               (દ્વિગુણ) p->c0.x, (દ્વિગુણ) p->c0.y,
                               (દ્વિગુણ) p->c1.x, (દ્વિગુણ) p->c1.y,
                               (દ્વિગુણ) p->p1.x, (દ્વિગુણ) p->p1.y);
  cairo_mesh_pattern_લીટી_to (pattern, (દ્વિગુણ) center->x, (દ્વિગુણ) center->y);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 0,
                                            (દ્વિગુણ) p->color0.r,
                                            (દ્વિગુણ) p->color0.g,
                                            (દ્વિગુણ) p->color0.b,
                                            (દ્વિગુણ) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 1,
                                            (દ્વિગુણ) p->color0.r,
                                            (દ્વિગુણ) p->color0.g,
                                            (દ્વિગુણ) p->color0.b,
                                            (દ્વિગુણ) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 2,
                                            (દ્વિગુણ) p->color1.r,
                                            (દ્વિગુણ) p->color1.g,
                                            (દ્વિગુણ) p->color1.b,
                                            (દ્વિગુણ) p->color1.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 3,
                                            (દ્વિગુણ) p->color1.r,
                                            (દ્વિગુણ) p->color1.g,
                                            (દ્વિગુણ) p->color1.b,
                                            (દ્વિગુણ) p->color1.a);
  cairo_mesh_pattern_end_patch (pattern);
}

#define MAX_ANGLE (HB_PI / 8.f)

સ્થિર શૂન્ય
_hb_cairo_add_sweep_gradient_patches1 (દશાંશ cx, દશાંશ cy, દશાંશ radius,
				       દશાંશ a0, hb_cairo_color_t *c0,
				       દશાંશ a1, hb_cairo_color_t *c1,
				       cairo_pattern_t *pattern)
{
  hb_cairo_point_t center = hb_cairo_point_t { cx, cy };
  પૂર્ણાંક અંક_splits;
  hb_cairo_point_t p0;
  hb_cairo_color_t color0, color1;

  અંક_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_point_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  માટે (પૂર્ણાંક a = 0; a < અંક_splits; a++)
    {
      દશાંશ k = (a + 1.) / અંક_splits;
      દશાંશ angle1;
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

સ્થિર અંતઃરેખ hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  પાછા {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

સંરચના _hb_cairo_sweep_patch_ctx {
  દશાંશ cx, cy, radius;
  cairo_pattern_t *pattern;
};

સ્થિર શૂન્ય
_hb_cairo_sweep_emit_patch (દશાંશ a0, hb_color_t c0,
			    દશાંશ a1, hb_color_t c1,
			    શૂન્ય *user_data)
{
  auto *ctx = (_hb_cairo_sweep_patch_ctx *) user_data;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  _hb_cairo_add_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->pattern);
}

સ્થિર શૂન્ય
_hb_cairo_add_sweep_gradient_patches (hb_color_stop_t *stops,
				      unsigned પૂર્ણાંક n_stops,
				      cairo_extend_t extend,
				      દશાંશ cx, દશાંશ cy,
				      દશાંશ radius,
				      દશાંશ start_angle,
				      દશાંશ end_angle,
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

શૂન્ય
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_લીટી_t *color_લીટી,
				દશાંશ cx, દશાંશ cy,
				દશાંશ start_angle,
				દશાંશ end_angle)
{
  cairo_t *cr = c->cr;

  unsigned પૂર્ણાંક len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  દ્વિગુણ x1, y1, x2, y2;
  દશાંશ max_x, max_y, radius;
  cairo_pattern_t *pattern;

  જો (unlikely (!_hb_cairo_get_color_stops (c, color_લીટી, &len, &stops)))
    પાછા;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (અચલ hb_color_stop_t &a, અચલ hb_color_stop_t &b) {
      પાછા (a.offset > b.offset) - (a.offset < b.offset);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (દશાંશ) hb_max ((x1 - (દ્વિગુણ) cx) * (x1 - (દ્વિગુણ) cx), (x2 - (દ્વિગુણ) cx) * (x2 - (દ્વિગુણ) cx));
  max_y = (દશાંશ) hb_max ((y1 - (દ્વિગુણ) cy) * (y1 - (દ્વિગુણ) cy), (y2 - (દ્વિગુણ) cy) * (y2 - (દ્વિગુણ) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_લીટી_get_extend (color_લીટી));
  pattern = cairo_pattern_create_mesh ();

  _hb_cairo_add_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, start_angle, end_angle, pattern);

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  જો (stops != stops_)
    hb_free (stops);
}

#endif
