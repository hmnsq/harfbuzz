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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_CAIRO

#অন্তর্ভাব "hb-cairo-utils.hh"
#অন্তর্ভাব "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#define PREALLOCATED_COLOR_STOPS 16

প্রকারলেখ কাঠামো {
  দশমিক r, g, b, a;
} hb_cairo_color_t;

স্থির অন্তঃরেখ cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  নির্বাচন (extend)
    {
    ক্ষেত্র HB_PAINT_EXTEND_PAD: ফেরত CAIRO_EXTEND_PAD;
    ক্ষেত্র HB_PAINT_EXTEND_REPEAT: ফেরত CAIRO_EXTEND_REPEAT;
    ক্ষেত্র HB_PAINT_EXTEND_REFLECT: ফেরত CAIRO_EXTEND_REFLECT;
    পূর্বনির্ধারিত: বিরতি;
    }

  ফেরত CAIRO_EXTEND_PAD;
}

#ifdef CAIRO_HAS_PNG_FUNCTIONS
প্রকারলেখ কাঠামো
{
  hb_blob_t *blob;
  unsigned পূর্ণসংখ্যা offset;
} hb_cairo_read_blob_তথ্য_t;

স্থির cairo_status_t
hb_cairo_read_blob (শূন্য *closure,
		    unsigned অক্ষর *তথ্য,
		    unsigned পূর্ণসংখ্যা দৈর্ঘ্য)
{
  hb_cairo_read_blob_তথ্য_t *r = (hb_cairo_read_blob_তথ্য_t *) closure;
  ধ্রুবক অক্ষর *d;
  unsigned পূর্ণসংখ্যা আকার_2;

  d = hb_blob_get_তথ্য (r->blob, &আকার_2);

  যদি (r->offset + দৈর্ঘ্য > আকার_2)
    ফেরত CAIRO_STATUS_READ_ERROR;

  hb_memcpy (তথ্য, d + r->offset, দৈর্ঘ্য);
  r->offset += দৈর্ঘ্য;

  ফেরত CAIRO_STATUS_SUCCESS;
}
#endif

স্থির ধ্রুবক cairo_user_তথ্য_চাবি_t *hb_cairo_surface_blob_user_তথ্য_চাবি = {0};

স্থির শূন্য
_hb_cairo_destroy_blob (শূন্য *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

শূন্য
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  যদি (use_foreground)
  {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
    দ্বিগুণ r, g, b, a;
    cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
    যদি (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
      cairo_set_source_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    অন্যথা
#endif
      cairo_set_source_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  অন্যথা
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
			     HB_UNUSED দশমিক slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  যদি (!extents) /* SVG currently. */
    ফেরত মিথ্যা;

  cairo_surface_t *surface = nullptr;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
  যদি (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
    hb_cairo_read_blob_তথ্য_t r;
    r.blob = blob;
    r.offset = 0;
    surface = cairo_image_surface_create_from_png_stream (hb_cairo_read_blob, &r);

    /* For PNG, width,height can be unreliable, as is the case for NotoColorEmoji :(.
     * Just pull them out of the surface. */
    width = cairo_image_surface_get_width (surface);
    height = cairo_image_surface_get_width (surface);
  }
  অন্যথা
#endif
  যদি (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    unsigned তথ্য_আকার = hb_blob_get_দৈর্ঘ্য (blob);
    যদি (তথ্য_আকার < width * height * 4)
      ফেরত মিথ্যা;

    unsigned অক্ষর *তথ্য;
#ifdef __BYTE_ORDER
    যদি (__BYTE_ORDER == __BIG_ENDIAN)
    {
      তথ্য = (unsigned অক্ষর *) hb_blob_get_তথ্য_writable (blob, nullptr);
      যদি (!তথ্য)
        ফেরত মিথ্যা;

      unsigned গুনতি = width * height * 4;
      জন্য (unsigned i = 0; i < গুনতি; i += 4)
      {
        unsigned অক্ষর b;
	b = তথ্য[i];
	তথ্য[i] = তথ্য[i+3];
	তথ্য[i+3] = b;
	b = তথ্য[i+1];
	তথ্য[i+1] = তথ্য[i+2];
	তথ্য[i+2] = b;
      }
    }
    অন্যথা
#endif
      তথ্য = (unsigned অক্ষর *) hb_blob_get_তথ্য (blob, nullptr);

    surface = cairo_image_surface_create_for_তথ্য (তথ্য,
						   CAIRO_FORMAT_ARGB32,
						   width, height,
						   width * 4);

    cairo_surface_set_user_তথ্য (surface,
				 hb_cairo_surface_blob_user_তথ্য_চাবি,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  যদি (!surface)
    ফেরত মিথ্যা;

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

  cairo_matrix_t matrix = {(দ্বিগুণ) width, 0, 0, (দ্বিগুণ) height, 0, 0};
  cairo_pattern_set_matrix (pattern, &matrix);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->width, extents->height);
  cairo_set_source (cr, pattern);

  cairo_paint (cr);

  cairo_pattern_destroy (pattern);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  ফেরত সত্য;
}

স্থির যৌক্তিক
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_সারি_t *color_সারি,
			   unsigned *গুনতি,
			   hb_color_stop_t **stops)
{
  unsigned len = hb_color_সারি_get_color_stops (color_সারি, 0, nullptr, nullptr);
  যৌক্তিক allocated = মিথ্যা;
  যদি (unlikely (!len))
    ফেরত মিথ্যা;
  যদি (len > *গুনতি)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, আকার (hb_color_stop_t));
    যদি (unlikely (!*stops))
      ফেরত মিথ্যা;
    allocated = সত্য;
  }
  hb_color_সারি_get_color_stops (color_সারি, 0, &len, *stops);
  যদি (unlikely (!len))
  {
    যদি (allocated)
    {
      hb_free (*stops);
      *stops = nullptr;
    }
    ফেরত মিথ্যা;
  }
  জন্য (unsigned i = 0; i < len; i++)
    যদি ((*stops)[i].is_foreground)
    {
#ifdef HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
      দ্বিগুণ r, g, b, a;
      cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
      যদি (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      অন্যথা
#endif
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *গুনতি = len;
  ফেরত সত্য;
}

শূন্য
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_সারি_t *color_সারি,
				 দশমিক x0, দশমিক y0,
				 দশমিক x1, দশমিক y1,
				 দশমিক x2, দশমিক y2)
{
  cairo_t *cr = c->cr;

  unsigned পূর্ণসংখ্যা len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  দশমিক xx0, yy0, xx1, yy1;
  দশমিক xxx0, yyy0, xxx1, yyy1;
  দশমিক min, max;
  cairo_pattern_t *pattern;

  যদি (unlikely (!_hb_cairo_get_color_stops (c, color_সারি, &len, &stops)))
    ফেরত;
  hb_paint_normalize_color_সারি (stops, len, &min, &max);

  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  pattern = cairo_pattern_create_linear ((দ্বিগুণ) xxx0, (দ্বিগুণ) yyy0, (দ্বিগুণ) xxx1, (দ্বিগুণ) yyy1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_সারি_get_extend (color_সারি)));
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < len; i++)
    {
      দ্বিগুণ r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (দ্বিগুণ) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  যদি (stops != stops_)
    hb_free (stops);
}

শূন্য
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_সারি_t *color_সারি,
				 দশমিক x0, দশমিক y0, দশমিক r0,
				 দশমিক x1, দশমিক y1, দশমিক r1)
{
  cairo_t *cr = c->cr;

  unsigned পূর্ণসংখ্যা len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  দশমিক min, max;
  দশমিক xx0, yy0, xx1, yy1;
  দশমিক rr0, rr1;
  cairo_pattern_t *pattern;

  যদি (unlikely (!_hb_cairo_get_color_stops (c, color_সারি, &len, &stops)))
    ফেরত;
  hb_paint_normalize_color_সারি (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  pattern = cairo_pattern_create_radial ((দ্বিগুণ) xx0, (দ্বিগুণ) yy0, (দ্বিগুণ) rr0, (দ্বিগুণ) xx1, (দ্বিগুণ) yy1, (দ্বিগুণ) rr1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_সারি_get_extend (color_সারি)));

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < len; i++)
    {
      দ্বিগুণ r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (দ্বিগুণ) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  যদি (stops != stops_)
    hb_free (stops);
}

প্রকারলেখ কাঠামো {
  দশমিক x, y;
} hb_cairo_point_t;

স্থির অন্তঃরেখ দশমিক
_hb_cairo_interpolate (দশমিক f0, দশমিক f1, দশমিক f)
{
  ফেরত f0 + f * (f1 - f0);
}

স্থির অন্তঃরেখ শূন্য
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

স্থির অন্তঃরেখ শূন্য
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  যদি (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

স্থির শূন্য
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, দশমিক k, hb_cairo_color_t *c)
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

স্থির অন্তঃরেখ দশমিক
_hb_cairo_dot (hb_cairo_point_t p, hb_cairo_point_t q)
{
  ফেরত p.x * q.x + p.y * q.y;
}

স্থির অন্তঃরেখ hb_cairo_point_t
_hb_cairo_normalize (hb_cairo_point_t p)
{
  দশমিক len = sqrtf (_hb_cairo_dot (p, p));

  ফেরত hb_cairo_point_t { p.x / len, p.y / len };
}

স্থির অন্তঃরেখ hb_cairo_point_t
hb_cairo_যোগ (hb_cairo_point_t p, hb_cairo_point_t q)
{
  ফেরত hb_cairo_point_t { p.x + q.x, p.y + q.y };
}

স্থির অন্তঃরেখ hb_cairo_point_t
_hb_cairo_difference (hb_cairo_point_t p, hb_cairo_point_t q)
{
  ফেরত hb_cairo_point_t { p.x - q.x, p.y - q.y };
}

স্থির অন্তঃরেখ hb_cairo_point_t
_hb_cairo_scale (hb_cairo_point_t p, দশমিক f)
{
  ফেরত hb_cairo_point_t { p.x * f, p.y * f };
}

প্রকারলেখ কাঠামো {
  hb_cairo_point_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

স্থির শূন্য
_hb_cairo_add_patch (cairo_pattern_t *pattern, hb_cairo_point_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_pattern_begin_patch (pattern);
  cairo_mesh_pattern_move_to (pattern, (দ্বিগুণ) center->x, (দ্বিগুণ) center->y);
  cairo_mesh_pattern_সারি_to (pattern, (দ্বিগুণ) p->p0.x, (দ্বিগুণ) p->p0.y);
  cairo_mesh_pattern_curve_to (pattern,
                               (দ্বিগুণ) p->c0.x, (দ্বিগুণ) p->c0.y,
                               (দ্বিগুণ) p->c1.x, (দ্বিগুণ) p->c1.y,
                               (দ্বিগুণ) p->p1.x, (দ্বিগুণ) p->p1.y);
  cairo_mesh_pattern_সারি_to (pattern, (দ্বিগুণ) center->x, (দ্বিগুণ) center->y);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 0,
                                            (দ্বিগুণ) p->color0.r,
                                            (দ্বিগুণ) p->color0.g,
                                            (দ্বিগুণ) p->color0.b,
                                            (দ্বিগুণ) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 1,
                                            (দ্বিগুণ) p->color0.r,
                                            (দ্বিগুণ) p->color0.g,
                                            (দ্বিগুণ) p->color0.b,
                                            (দ্বিগুণ) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 2,
                                            (দ্বিগুণ) p->color1.r,
                                            (দ্বিগুণ) p->color1.g,
                                            (দ্বিগুণ) p->color1.b,
                                            (দ্বিগুণ) p->color1.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 3,
                                            (দ্বিগুণ) p->color1.r,
                                            (দ্বিগুণ) p->color1.g,
                                            (দ্বিগুণ) p->color1.b,
                                            (দ্বিগুণ) p->color1.a);
  cairo_mesh_pattern_end_patch (pattern);
}

#define MAX_ANGLE (HB_PI / 8.f)

স্থির শূন্য
_hb_cairo_add_sweep_gradient_patches1 (দশমিক cx, দশমিক cy, দশমিক radius,
				       দশমিক a0, hb_cairo_color_t *c0,
				       দশমিক a1, hb_cairo_color_t *c1,
				       cairo_pattern_t *pattern)
{
  hb_cairo_point_t center = hb_cairo_point_t { cx, cy };
  পূর্ণসংখ্যা অঙ্ক_splits;
  hb_cairo_point_t p0;
  hb_cairo_color_t color0, color1;

  অঙ্ক_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_point_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  জন্য (পূর্ণসংখ্যা a = 0; a < অঙ্ক_splits; a++)
    {
      দশমিক k = (a + 1.) / অঙ্ক_splits;
      দশমিক angle1;
      hb_cairo_point_t p1;
      hb_cairo_point_t A, U;
      hb_cairo_point_t C0, C1;
      hb_cairo_patch_t patch;

      angle1 = _hb_cairo_interpolate (a0, a1, k);
      _hb_cairo_interpolate_colors (c0, c1, k, &color1);

      patch.color0 = color0;
      patch.color1 = color1;

      p1 = hb_cairo_point_t { cosf (angle1), sinf (angle1) };
      patch.p0 = hb_cairo_যোগ (center, _hb_cairo_scale (p0, radius));
      patch.p1 = hb_cairo_যোগ (center, _hb_cairo_scale (p1, radius));

      A = _hb_cairo_normalize (hb_cairo_যোগ (p0, p1));
      U = hb_cairo_point_t { -A.y, A.x };
      C0 = hb_cairo_যোগ (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p0, A), p0) / _hb_cairo_dot (U, p0)));
      C1 = hb_cairo_যোগ (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p1, A), p1) / _hb_cairo_dot (U, p1)));

      patch.c0 = hb_cairo_যোগ (center, _hb_cairo_scale (hb_cairo_যোগ (C0, _hb_cairo_scale (_hb_cairo_difference (C0, p0), 0.33333f)), radius));
      patch.c1 = hb_cairo_যোগ (center, _hb_cairo_scale (hb_cairo_যোগ (C1, _hb_cairo_scale (_hb_cairo_difference (C1, p1), 0.33333f)), radius));

      _hb_cairo_add_patch (pattern, &center, &patch);

      p0 = p1;
      color0 = color1;
    }
}

স্থির অন্তঃরেখ hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  ফেরত {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

কাঠামো _hb_cairo_sweep_patch_ctx {
  দশমিক cx, cy, radius;
  cairo_pattern_t *pattern;
};

স্থির শূন্য
_hb_cairo_sweep_emit_patch (দশমিক a0, hb_color_t c0,
			    দশমিক a1, hb_color_t c1,
			    শূন্য *user_তথ্য)
{
  auto *ctx = (_hb_cairo_sweep_patch_ctx *) user_তথ্য;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  _hb_cairo_add_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->pattern);
}

স্থির শূন্য
_hb_cairo_add_sweep_gradient_patches (hb_color_stop_t *stops,
				      unsigned পূর্ণসংখ্যা n_stops,
				      cairo_extend_t extend,
				      দশমিক cx, দশমিক cy,
				      দশমিক radius,
				      দশমিক start_angle,
				      দশমিক end_angle,
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

শূন্য
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_সারি_t *color_সারি,
				দশমিক cx, দশমিক cy,
				দশমিক start_angle,
				দশমিক end_angle)
{
  cairo_t *cr = c->cr;

  unsigned পূর্ণসংখ্যা len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  দ্বিগুণ x1, y1, x2, y2;
  দশমিক max_x, max_y, radius;
  cairo_pattern_t *pattern;

  যদি (unlikely (!_hb_cairo_get_color_stops (c, color_সারি, &len, &stops)))
    ফেরত;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (ধ্রুবক hb_color_stop_t &a, ধ্রুবক hb_color_stop_t &b) {
      ফেরত (a.offset > b.offset) - (a.offset < b.offset);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (দশমিক) hb_max ((x1 - (দ্বিগুণ) cx) * (x1 - (দ্বিগুণ) cx), (x2 - (দ্বিগুণ) cx) * (x2 - (দ্বিগুণ) cx));
  max_y = (দশমিক) hb_max ((y1 - (দ্বিগুণ) cy) * (y1 - (দ্বিগুণ) cy), (y2 - (দ্বিগুণ) cy) * (y2 - (দ্বিগুণ) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_সারি_get_extend (color_সারি));
  pattern = cairo_pattern_create_mesh ();

  _hb_cairo_add_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, start_angle, end_angle, pattern);

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  যদি (stops != stops_)
    hb_free (stops);
}

#endif
