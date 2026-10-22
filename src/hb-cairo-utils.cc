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

#شامل "hb.hh"

#ifdef HAVE_CAIRO

#شامل "hb-cairo-utils.hh"
#شامل "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#define PREALLOCATED_COLOR_STOPS 16

تعریفنوع ساختار {
  اعشاری r, g, b, a;
} hb_cairo_color_t;

ایستا درونخطی cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  انتخاب (extend)
    {
    حالت HB_PAINT_EXTEND_PAD: بازگشت CAIRO_EXTEND_PAD;
    حالت HB_PAINT_EXTEND_REPEAT: بازگشت CAIRO_EXTEND_REPEAT;
    حالت HB_PAINT_EXTEND_REFLECT: بازگشت CAIRO_EXTEND_REFLECT;
    پیشفرض: بشکن;
    }

  بازگشت CAIRO_EXTEND_PAD;
}

#ifdef CAIRO_HAS_PNG_FUNCTIONS
تعریفنوع ساختار
{
  hb_blob_t *blob;
  unsigned صحیح offset;
} hb_cairo_read_blob_data_t;

ایستا cairo_status_t
hb_cairo_read_blob (تهی *closure,
		    unsigned نویسه *data,
		    unsigned صحیح length)
{
  hb_cairo_read_blob_data_t *r = (hb_cairo_read_blob_data_t *) closure;
  ثابت نویسه *d;
  unsigned صحیح size;

  d = hb_blob_get_data (r->blob, &size);

  اگر (r->offset + length > size)
    بازگشت CAIRO_STATUS_READ_ERROR;

  hb_memcpy (data, d + r->offset, length);
  r->offset += length;

  بازگشت CAIRO_STATUS_SUCCESS;
}
#endif

ایستا ثابت cairo_user_data_کلید_t *hb_cairo_surface_blob_user_data_کلید = {0};

ایستا تهی
_hb_cairo_destroy_blob (تهی *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

تهی
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  اگر (use_foreground)
  {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
    دوگانه r, g, b, a;
    cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
    اگر (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
      cairo_set_source_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    وگرنه
#endif
      cairo_set_source_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  وگرنه
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
			     HB_UNUSED اعشاری slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  اگر (!extents) /* SVG currently. */
    بازگشت نادرست;

  cairo_surface_t *surface = nullptr;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
  اگر (format == HB_PAINT_IMAGE_FORMAT_PNG)
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
  وگرنه
#endif
  اگر (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    unsigned data_size = hb_blob_get_length (blob);
    اگر (data_size < width * height * 4)
      بازگشت نادرست;

    unsigned نویسه *data;
#ifdef __BYTE_ORDER
    اگر (__BYTE_ORDER == __BIG_ENDIAN)
    {
      data = (unsigned نویسه *) hb_blob_get_data_writable (blob, nullptr);
      اگر (!data)
        بازگشت نادرست;

      unsigned شمار = width * height * 4;
      برای (unsigned i = 0; i < شمار; i += 4)
      {
        unsigned نویسه b;
	b = data[i];
	data[i] = data[i+3];
	data[i+3] = b;
	b = data[i+1];
	data[i+1] = data[i+2];
	data[i+2] = b;
      }
    }
    وگرنه
#endif
      data = (unsigned نویسه *) hb_blob_get_data (blob, nullptr);

    surface = cairo_image_surface_create_for_data (data,
						   CAIRO_FORMAT_ARGB32,
						   width, height,
						   width * 4);

    cairo_surface_set_user_data (surface,
				 hb_cairo_surface_blob_user_data_کلید,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  اگر (!surface)
    بازگشت نادرست;

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

  cairo_matrix_t matrix = {(دوگانه) width, 0, 0, (دوگانه) height, 0, 0};
  cairo_pattern_set_matrix (pattern, &matrix);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->width, extents->height);
  cairo_set_source (cr, pattern);

  cairo_paint (cr);

  cairo_pattern_destroy (pattern);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  بازگشت درست;
}

ایستا منطقی
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_سطر_t *color_سطر,
			   unsigned *شمار,
			   hb_color_stop_t **stops)
{
  unsigned len = hb_color_سطر_get_color_stops (color_سطر, 0, nullptr, nullptr);
  منطقی allocated = نادرست;
  اگر (unlikely (!len))
    بازگشت نادرست;
  اگر (len > *شمار)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, اندازه (hb_color_stop_t));
    اگر (unlikely (!*stops))
      بازگشت نادرست;
    allocated = درست;
  }
  hb_color_سطر_get_color_stops (color_سطر, 0, &len, *stops);
  اگر (unlikely (!len))
  {
    اگر (allocated)
    {
      hb_free (*stops);
      *stops = nullptr;
    }
    بازگشت نادرست;
  }
  برای (unsigned i = 0; i < len; i++)
    اگر ((*stops)[i].is_foreground)
    {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
      دوگانه r, g, b, a;
      cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
      اگر (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      وگرنه
#endif
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *شمار = len;
  بازگشت درست;
}

تهی
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 اعشاری x0, اعشاری y0,
				 اعشاری x1, اعشاری y1,
				 اعشاری x2, اعشاری y2)
{
  cairo_t *cr = c->cr;

  unsigned صحیح len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  اعشاری xx0, yy0, xx1, yy1;
  اعشاری xxx0, yyy0, xxx1, yyy1;
  اعشاری min, max;
  cairo_pattern_t *pattern;

  اگر (unlikely (!_hb_cairo_get_color_stops (c, color_سطر, &len, &stops)))
    بازگشت;
  hb_paint_normalize_color_سطر (stops, len, &min, &max);

  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  pattern = cairo_pattern_create_linear ((دوگانه) xxx0, (دوگانه) yyy0, (دوگانه) xxx1, (دوگانه) yyy1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_سطر_get_extend (color_سطر)));
  برای (unsigned صحیح i = 0; i < len; i++)
    {
      دوگانه r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (دوگانه) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  اگر (stops != stops_)
    hb_free (stops);
}

تهی
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 اعشاری x0, اعشاری y0, اعشاری r0,
				 اعشاری x1, اعشاری y1, اعشاری r1)
{
  cairo_t *cr = c->cr;

  unsigned صحیح len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  اعشاری min, max;
  اعشاری xx0, yy0, xx1, yy1;
  اعشاری rr0, rr1;
  cairo_pattern_t *pattern;

  اگر (unlikely (!_hb_cairo_get_color_stops (c, color_سطر, &len, &stops)))
    بازگشت;
  hb_paint_normalize_color_سطر (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  pattern = cairo_pattern_create_radial ((دوگانه) xx0, (دوگانه) yy0, (دوگانه) rr0, (دوگانه) xx1, (دوگانه) yy1, (دوگانه) rr1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_سطر_get_extend (color_سطر)));

  برای (unsigned صحیح i = 0; i < len; i++)
    {
      دوگانه r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (دوگانه) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  اگر (stops != stops_)
    hb_free (stops);
}

تعریفنوع ساختار {
  اعشاری x, y;
} hb_cairo_point_t;

ایستا درونخطی اعشاری
_hb_cairo_interpolate (اعشاری f0, اعشاری f1, اعشاری f)
{
  بازگشت f0 + f * (f1 - f0);
}

ایستا درونخطی تهی
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

ایستا درونخطی تهی
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  اگر (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

ایستا تهی
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, اعشاری k, hb_cairo_color_t *c)
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

ایستا درونخطی اعشاری
_hb_cairo_dot (hb_cairo_point_t p, hb_cairo_point_t q)
{
  بازگشت p.x * q.x + p.y * q.y;
}

ایستا درونخطی hb_cairo_point_t
_hb_cairo_normalize (hb_cairo_point_t p)
{
  اعشاری len = sqrtf (_hb_cairo_dot (p, p));

  بازگشت hb_cairo_point_t { p.x / len, p.y / len };
}

ایستا درونخطی hb_cairo_point_t
_hb_cairo_sum (hb_cairo_point_t p, hb_cairo_point_t q)
{
  بازگشت hb_cairo_point_t { p.x + q.x, p.y + q.y };
}

ایستا درونخطی hb_cairo_point_t
_hb_cairo_difference (hb_cairo_point_t p, hb_cairo_point_t q)
{
  بازگشت hb_cairo_point_t { p.x - q.x, p.y - q.y };
}

ایستا درونخطی hb_cairo_point_t
_hb_cairo_scale (hb_cairo_point_t p, اعشاری f)
{
  بازگشت hb_cairo_point_t { p.x * f, p.y * f };
}

تعریفنوع ساختار {
  hb_cairo_point_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

ایستا تهی
_hb_cairo_add_patch (cairo_pattern_t *pattern, hb_cairo_point_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_pattern_begin_patch (pattern);
  cairo_mesh_pattern_move_to (pattern, (دوگانه) center->x, (دوگانه) center->y);
  cairo_mesh_pattern_سطر_to (pattern, (دوگانه) p->p0.x, (دوگانه) p->p0.y);
  cairo_mesh_pattern_curve_to (pattern,
                               (دوگانه) p->c0.x, (دوگانه) p->c0.y,
                               (دوگانه) p->c1.x, (دوگانه) p->c1.y,
                               (دوگانه) p->p1.x, (دوگانه) p->p1.y);
  cairo_mesh_pattern_سطر_to (pattern, (دوگانه) center->x, (دوگانه) center->y);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 0,
                                            (دوگانه) p->color0.r,
                                            (دوگانه) p->color0.g,
                                            (دوگانه) p->color0.b,
                                            (دوگانه) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 1,
                                            (دوگانه) p->color0.r,
                                            (دوگانه) p->color0.g,
                                            (دوگانه) p->color0.b,
                                            (دوگانه) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 2,
                                            (دوگانه) p->color1.r,
                                            (دوگانه) p->color1.g,
                                            (دوگانه) p->color1.b,
                                            (دوگانه) p->color1.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 3,
                                            (دوگانه) p->color1.r,
                                            (دوگانه) p->color1.g,
                                            (دوگانه) p->color1.b,
                                            (دوگانه) p->color1.a);
  cairo_mesh_pattern_end_patch (pattern);
}

#define MAX_ANGLE (HB_PI / 8.f)

ایستا تهی
_hb_cairo_add_sweep_gradient_patches1 (اعشاری cx, اعشاری cy, اعشاری radius,
				       اعشاری a0, hb_cairo_color_t *c0,
				       اعشاری a1, hb_cairo_color_t *c1,
				       cairo_pattern_t *pattern)
{
  hb_cairo_point_t center = hb_cairo_point_t { cx, cy };
  صحیح رقم_splits;
  hb_cairo_point_t p0;
  hb_cairo_color_t color0, color1;

  رقم_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_point_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  برای (صحیح a = 0; a < رقم_splits; a++)
    {
      اعشاری k = (a + 1.) / رقم_splits;
      اعشاری angle1;
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

ایستا درونخطی hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  بازگشت {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

ساختار _hb_cairo_sweep_patch_ctx {
  اعشاری cx, cy, radius;
  cairo_pattern_t *pattern;
};

ایستا تهی
_hb_cairo_sweep_emit_patch (اعشاری a0, hb_color_t c0,
			    اعشاری a1, hb_color_t c1,
			    تهی *user_data)
{
  auto *ctx = (_hb_cairo_sweep_patch_ctx *) user_data;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  _hb_cairo_add_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->pattern);
}

ایستا تهی
_hb_cairo_add_sweep_gradient_patches (hb_color_stop_t *stops,
				      unsigned صحیح n_stops,
				      cairo_extend_t extend,
				      اعشاری cx, اعشاری cy,
				      اعشاری radius,
				      اعشاری start_angle,
				      اعشاری end_angle,
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

تهی
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_سطر_t *color_سطر,
				اعشاری cx, اعشاری cy,
				اعشاری start_angle,
				اعشاری end_angle)
{
  cairo_t *cr = c->cr;

  unsigned صحیح len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  دوگانه x1, y1, x2, y2;
  اعشاری max_x, max_y, radius;
  cairo_pattern_t *pattern;

  اگر (unlikely (!_hb_cairo_get_color_stops (c, color_سطر, &len, &stops)))
    بازگشت;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b) {
      بازگشت (a.offset > b.offset) - (a.offset < b.offset);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (اعشاری) hb_max ((x1 - (دوگانه) cx) * (x1 - (دوگانه) cx), (x2 - (دوگانه) cx) * (x2 - (دوگانه) cx));
  max_y = (اعشاری) hb_max ((y1 - (دوگانه) cy) * (y1 - (دوگانه) cy), (y2 - (دوگانه) cy) * (y2 - (دوگانه) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_سطر_get_extend (color_سطر));
  pattern = cairo_pattern_create_mesh ();

  _hb_cairo_add_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, start_angle, end_angle, pattern);

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  اگر (stops != stops_)
    hb_free (stops);
}

#endif
