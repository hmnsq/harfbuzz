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

#அந்தர்பாவ "hb.hh"

#ifdef HAVE_CAIRO

#அந்தர்பாவ "hb-cairo-utils.hh"
#அந்தர்பாவ "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#define PREALLOCATED_COLOR_STOPS 16

வகைவரை கட்டமைப்பு {
  தசம r, g, b, a;
} hb_cairo_color_t;

நிலையான உள்வரி cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  தேர்வு (extend)
    {
    நிலை HB_PAINT_EXTEND_PAD: திரும்பு CAIRO_EXTEND_PAD;
    நிலை HB_PAINT_EXTEND_REPEAT: திரும்பு CAIRO_EXTEND_REPEAT;
    நிலை HB_PAINT_EXTEND_REFLECT: திரும்பு CAIRO_EXTEND_REFLECT;
    இயல்பு: நிறுத்து;
    }

  திரும்பு CAIRO_EXTEND_PAD;
}

#ifdef CAIRO_HAS_PNG_FUNCTIONS
வகைவரை கட்டமைப்பு
{
  hb_blob_t *blob;
  unsigned முழு offset;
} hb_cairo_read_blob_data_t;

நிலையான cairo_status_t
hb_cairo_read_blob (வெறுமை *closure,
		    unsigned எழுத்து *data,
		    unsigned முழு length)
{
  hb_cairo_read_blob_data_t *r = (hb_cairo_read_blob_data_t *) closure;
  மாறிலி எழுத்து *d;
  unsigned முழு size;

  d = hb_blob_get_data (r->blob, &size);

  எனில் (r->offset + length > size)
    திரும்பு CAIRO_STATUS_READ_ERROR;

  hb_memcpy (data, d + r->offset, length);
  r->offset += length;

  திரும்பு CAIRO_STATUS_SUCCESS;
}
#endif

நிலையான மாறிலி cairo_user_data_சாவி_t *hb_cairo_surface_blob_user_data_சாவி = {0};

நிலையான வெறுமை
_hb_cairo_destroy_blob (வெறுமை *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

வெறுமை
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  எனில் (use_foreground)
  {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
    இரட்டை r, g, b, a;
    cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
    எனில் (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
      cairo_set_source_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    இல்லையெனில்
#endif
      cairo_set_source_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  இல்லையெனில்
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
			     HB_UNUSED தசம slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  எனில் (!extents) /* SVG currently. */
    திரும்பு பொய்;

  cairo_surface_t *surface = nullptr;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
  எனில் (format == HB_PAINT_IMAGE_FORMAT_PNG)
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
  இல்லையெனில்
#endif
  எனில் (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    unsigned data_size = hb_blob_get_length (blob);
    எனில் (data_size < width * height * 4)
      திரும்பு பொய்;

    unsigned எழுத்து *data;
#ifdef __BYTE_ORDER
    எனில் (__BYTE_ORDER == __BIG_ENDIAN)
    {
      data = (unsigned எழுத்து *) hb_blob_get_data_writable (blob, nullptr);
      எனில் (!data)
        திரும்பு பொய்;

      unsigned மொத்தம் = width * height * 4;
      க்கு (unsigned i = 0; i < மொத்தம்; i += 4)
      {
        unsigned எழுத்து b;
	b = data[i];
	data[i] = data[i+3];
	data[i+3] = b;
	b = data[i+1];
	data[i+1] = data[i+2];
	data[i+2] = b;
      }
    }
    இல்லையெனில்
#endif
      data = (unsigned எழுத்து *) hb_blob_get_data (blob, nullptr);

    surface = cairo_image_surface_create_for_data (data,
						   CAIRO_FORMAT_ARGB32,
						   width, height,
						   width * 4);

    cairo_surface_set_user_data (surface,
				 hb_cairo_surface_blob_user_data_சாவி,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  எனில் (!surface)
    திரும்பு பொய்;

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

  cairo_matrix_t matrix = {(இரட்டை) width, 0, 0, (இரட்டை) height, 0, 0};
  cairo_pattern_set_matrix (pattern, &matrix);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->width, extents->height);
  cairo_set_source (cr, pattern);

  cairo_paint (cr);

  cairo_pattern_destroy (pattern);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  திரும்பு உண்மை;
}

நிலையான தருக்க
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_வரி_t *color_வரி,
			   unsigned *மொத்தம்,
			   hb_color_stop_t **stops)
{
  unsigned len = hb_color_வரி_get_color_stops (color_வரி, 0, nullptr, nullptr);
  தருக்க allocated = பொய்;
  எனில் (unlikely (!len))
    திரும்பு பொய்;
  எனில் (len > *மொத்தம்)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, அளவு (hb_color_stop_t));
    எனில் (unlikely (!*stops))
      திரும்பு பொய்;
    allocated = உண்மை;
  }
  hb_color_வரி_get_color_stops (color_வரி, 0, &len, *stops);
  எனில் (unlikely (!len))
  {
    எனில் (allocated)
    {
      hb_free (*stops);
      *stops = nullptr;
    }
    திரும்பு பொய்;
  }
  க்கு (unsigned i = 0; i < len; i++)
    எனில் ((*stops)[i].is_foreground)
    {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
      இரட்டை r, g, b, a;
      cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
      எனில் (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      இல்லையெனில்
#endif
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *மொத்தம் = len;
  திரும்பு உண்மை;
}

வெறுமை
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_வரி_t *color_வரி,
				 தசம x0, தசம y0,
				 தசம x1, தசம y1,
				 தசம x2, தசம y2)
{
  cairo_t *cr = c->cr;

  unsigned முழு len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  தசம xx0, yy0, xx1, yy1;
  தசம xxx0, yyy0, xxx1, yyy1;
  தசம min, max;
  cairo_pattern_t *pattern;

  எனில் (unlikely (!_hb_cairo_get_color_stops (c, color_வரி, &len, &stops)))
    திரும்பு;
  hb_paint_normalize_color_வரி (stops, len, &min, &max);

  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  pattern = cairo_pattern_create_linear ((இரட்டை) xxx0, (இரட்டை) yyy0, (இரட்டை) xxx1, (இரட்டை) yyy1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_வரி_get_extend (color_வரி)));
  க்கு (unsigned முழு i = 0; i < len; i++)
    {
      இரட்டை r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (இரட்டை) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  எனில் (stops != stops_)
    hb_free (stops);
}

வெறுமை
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_வரி_t *color_வரி,
				 தசம x0, தசம y0, தசம r0,
				 தசம x1, தசம y1, தசம r1)
{
  cairo_t *cr = c->cr;

  unsigned முழு len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  தசம min, max;
  தசம xx0, yy0, xx1, yy1;
  தசம rr0, rr1;
  cairo_pattern_t *pattern;

  எனில் (unlikely (!_hb_cairo_get_color_stops (c, color_வரி, &len, &stops)))
    திரும்பு;
  hb_paint_normalize_color_வரி (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  pattern = cairo_pattern_create_radial ((இரட்டை) xx0, (இரட்டை) yy0, (இரட்டை) rr0, (இரட்டை) xx1, (இரட்டை) yy1, (இரட்டை) rr1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_வரி_get_extend (color_வரி)));

  க்கு (unsigned முழு i = 0; i < len; i++)
    {
      இரட்டை r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (இரட்டை) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  எனில் (stops != stops_)
    hb_free (stops);
}

வகைவரை கட்டமைப்பு {
  தசம x, y;
} hb_cairo_point_t;

நிலையான உள்வரி தசம
_hb_cairo_interpolate (தசம f0, தசம f1, தசம f)
{
  திரும்பு f0 + f * (f1 - f0);
}

நிலையான உள்வரி வெறுமை
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

நிலையான உள்வரி வெறுமை
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  எனில் (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

நிலையான வெறுமை
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, தசம k, hb_cairo_color_t *c)
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

நிலையான உள்வரி தசம
_hb_cairo_dot (hb_cairo_point_t p, hb_cairo_point_t q)
{
  திரும்பு p.x * q.x + p.y * q.y;
}

நிலையான உள்வரி hb_cairo_point_t
_hb_cairo_normalize (hb_cairo_point_t p)
{
  தசம len = sqrtf (_hb_cairo_dot (p, p));

  திரும்பு hb_cairo_point_t { p.x / len, p.y / len };
}

நிலையான உள்வரி hb_cairo_point_t
_hb_cairo_sum (hb_cairo_point_t p, hb_cairo_point_t q)
{
  திரும்பு hb_cairo_point_t { p.x + q.x, p.y + q.y };
}

நிலையான உள்வரி hb_cairo_point_t
_hb_cairo_difference (hb_cairo_point_t p, hb_cairo_point_t q)
{
  திரும்பு hb_cairo_point_t { p.x - q.x, p.y - q.y };
}

நிலையான உள்வரி hb_cairo_point_t
_hb_cairo_scale (hb_cairo_point_t p, தசம f)
{
  திரும்பு hb_cairo_point_t { p.x * f, p.y * f };
}

வகைவரை கட்டமைப்பு {
  hb_cairo_point_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

நிலையான வெறுமை
_hb_cairo_add_patch (cairo_pattern_t *pattern, hb_cairo_point_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_pattern_begin_patch (pattern);
  cairo_mesh_pattern_move_to (pattern, (இரட்டை) center->x, (இரட்டை) center->y);
  cairo_mesh_pattern_வரி_to (pattern, (இரட்டை) p->p0.x, (இரட்டை) p->p0.y);
  cairo_mesh_pattern_curve_to (pattern,
                               (இரட்டை) p->c0.x, (இரட்டை) p->c0.y,
                               (இரட்டை) p->c1.x, (இரட்டை) p->c1.y,
                               (இரட்டை) p->p1.x, (இரட்டை) p->p1.y);
  cairo_mesh_pattern_வரி_to (pattern, (இரட்டை) center->x, (இரட்டை) center->y);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 0,
                                            (இரட்டை) p->color0.r,
                                            (இரட்டை) p->color0.g,
                                            (இரட்டை) p->color0.b,
                                            (இரட்டை) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 1,
                                            (இரட்டை) p->color0.r,
                                            (இரட்டை) p->color0.g,
                                            (இரட்டை) p->color0.b,
                                            (இரட்டை) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 2,
                                            (இரட்டை) p->color1.r,
                                            (இரட்டை) p->color1.g,
                                            (இரட்டை) p->color1.b,
                                            (இரட்டை) p->color1.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 3,
                                            (இரட்டை) p->color1.r,
                                            (இரட்டை) p->color1.g,
                                            (இரட்டை) p->color1.b,
                                            (இரட்டை) p->color1.a);
  cairo_mesh_pattern_end_patch (pattern);
}

#define MAX_ANGLE (HB_PI / 8.f)

நிலையான வெறுமை
_hb_cairo_add_sweep_gradient_patches1 (தசம cx, தசம cy, தசம radius,
				       தசம a0, hb_cairo_color_t *c0,
				       தசம a1, hb_cairo_color_t *c1,
				       cairo_pattern_t *pattern)
{
  hb_cairo_point_t center = hb_cairo_point_t { cx, cy };
  முழு இலக்கம்_splits;
  hb_cairo_point_t p0;
  hb_cairo_color_t color0, color1;

  இலக்கம்_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_point_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  க்கு (முழு a = 0; a < இலக்கம்_splits; a++)
    {
      தசம k = (a + 1.) / இலக்கம்_splits;
      தசம angle1;
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

நிலையான உள்வரி hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  திரும்பு {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

கட்டமைப்பு _hb_cairo_sweep_patch_ctx {
  தசம cx, cy, radius;
  cairo_pattern_t *pattern;
};

நிலையான வெறுமை
_hb_cairo_sweep_emit_patch (தசம a0, hb_color_t c0,
			    தசம a1, hb_color_t c1,
			    வெறுமை *user_data)
{
  auto *ctx = (_hb_cairo_sweep_patch_ctx *) user_data;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  _hb_cairo_add_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->pattern);
}

நிலையான வெறுமை
_hb_cairo_add_sweep_gradient_patches (hb_color_stop_t *stops,
				      unsigned முழு n_stops,
				      cairo_extend_t extend,
				      தசம cx, தசம cy,
				      தசம radius,
				      தசம start_angle,
				      தசம end_angle,
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

வெறுமை
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_வரி_t *color_வரி,
				தசம cx, தசம cy,
				தசம start_angle,
				தசம end_angle)
{
  cairo_t *cr = c->cr;

  unsigned முழு len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  இரட்டை x1, y1, x2, y2;
  தசம max_x, max_y, radius;
  cairo_pattern_t *pattern;

  எனில் (unlikely (!_hb_cairo_get_color_stops (c, color_வரி, &len, &stops)))
    திரும்பு;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (மாறிலி hb_color_stop_t &a, மாறிலி hb_color_stop_t &b) {
      திரும்பு (a.offset > b.offset) - (a.offset < b.offset);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (தசம) hb_max ((x1 - (இரட்டை) cx) * (x1 - (இரட்டை) cx), (x2 - (இரட்டை) cx) * (x2 - (இரட்டை) cx));
  max_y = (தசம) hb_max ((y1 - (இரட்டை) cy) * (y1 - (இரட்டை) cy), (y2 - (இரட்டை) cy) * (y2 - (இரட்டை) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_வரி_get_extend (color_வரி));
  pattern = cairo_pattern_create_mesh ();

  _hb_cairo_add_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, start_angle, end_angle, pattern);

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  எனில் (stops != stops_)
    hb_free (stops);
}

#endif
