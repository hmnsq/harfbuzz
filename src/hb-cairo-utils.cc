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

#اگر_تعریف_شدہ HAVE_CAIRO

#شامل "hb-cairo-utils.hh"
#شامل "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#تعریف_کرو PREALLOCATED_COLOR_STOPS 16

قسمتعریف ساخت {
  اعشاری r, g, b, a;
} hb_cairo_color_t;

ساکن اندرونی cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  انتخاب (extend)
    {
    حالت HB_PAINT_EXTEND_PAD: واپس CAIRO_EXTEND_PAD;
    حالت HB_PAINT_EXTEND_REPEAT: واپس CAIRO_EXTEND_REPEAT;
    حالت HB_PAINT_EXTEND_REFLECT: واپس CAIRO_EXTEND_REFLECT;
    طےشدہ: توڑو;
    }

  واپس CAIRO_EXTEND_PAD;
}

#اگر_تعریف_شدہ CAIRO_HAS_PNG_FUNCTIONS
قسمتعریف ساخت
{
  hb_blob_t *blob;
  غیرعلامتی صحیح انحراف;
} hb_cairo_read_blob_ڈیٹا_t;

ساکن cairo_حالت_t
hb_cairo_read_blob (خالی *احاطہ,
		    غیرعلامتی حرف *ڈیٹا,
		    غیرعلامتی صحیح لمبائی)
{
  hb_cairo_read_blob_ڈیٹا_t *r = (hb_cairo_read_blob_ڈیٹا_t *) احاطہ;
  ثابت حرف *d;
  غیرعلامتی صحیح سائز;

  d = hb_blob_get_ڈیٹا (r->blob, &سائز);

  اگر (r->انحراف + لمبائی > سائز)
    واپس CAIRO_حالت_READ_ERROR;

  hb_memcpy (ڈیٹا, d + r->انحراف, لمبائی);
  r->انحراف += لمبائی;

  واپس CAIRO_حالت_SUCCESS;
}
#اگر_ختم

ساکن ثابت cairo_user_ڈیٹا_کلید_t *hb_cairo_surface_blob_user_ڈیٹا_کلید = {0};

ساکن خالی
_hb_cairo_destroy_blob (خالی *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

خالی
hb_cairo_سیٹ_ماخذ_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  اگر (use_foreground)
  {
#اگر_تعریف_شدہ HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_ماخذ
    دوہرا r, g, b, a;
    cairo_نمونہ_t *foreground = cairo_user_scaled_font_get_foreground_ماخذ (c->scaled_font);
    اگر (cairo_نمونہ_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_حالت_SUCCESS)
      cairo_سیٹ_ماخذ_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    ورنہ
#اگر_ختم
      cairo_سیٹ_ماخذ_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  ورنہ
    cairo_سیٹ_ماخذ_rgba (cr,
			   hb_color_get_red (color) / 255.,
			   hb_color_get_green (color) / 255.,
			   hb_color_get_blue (color) / 255.,
			   hb_color_get_alpha (color) / 255.);
}

hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     غیرعلامتی چوڑائی,
			     غیرعلامتی اونچائی,
			     hb_لیبل_t format,
			     HB_غیر_مستعمل اعشاری slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  اگر (!extents) /* SVG currently. */
    واپس جھوٹ;

  cairo_surface_t *surface = خالی_اشاریہ;

#اگر_تعریف_شدہ CAIRO_HAS_PNG_FUNCTIONS
  اگر (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
    hb_cairo_read_blob_ڈیٹا_t r;
    r.blob = blob;
    r.انحراف = 0;
    surface = cairo_image_surface_create_from_png_دھارا (hb_cairo_read_blob, &r);

    /* For PNG, width,height can be unreliable, as is the case for NotoColorEmoji :(.
     * Just pull them out of the surface. */
    چوڑائی = cairo_image_surface_get_چوڑائی (surface);
    اونچائی = cairo_image_surface_get_چوڑائی (surface);
  }
  ورنہ
#اگر_ختم
  اگر (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    غیرعلامتی ڈیٹا_سائز = hb_blob_get_لمبائی (blob);
    اگر (ڈیٹا_سائز < چوڑائی * اونچائی * 4)
      واپس جھوٹ;

    غیرعلامتی حرف *ڈیٹا;
#اگر_تعریف_شدہ __BYTE_ORDER
    اگر (__BYTE_ORDER == __BIG_ENDIAN)
    {
      ڈیٹا = (غیرعلامتی حرف *) hb_blob_get_ڈیٹا_writable (blob, خالی_اشاریہ);
      اگر (!ڈیٹا)
        واپس جھوٹ;

      غیرعلامتی گنتی = چوڑائی * اونچائی * 4;
      برائے (غیرعلامتی i = 0; i < گنتی; i += 4)
      {
        غیرعلامتی حرف b;
	b = ڈیٹا[i];
	ڈیٹا[i] = ڈیٹا[i+3];
	ڈیٹا[i+3] = b;
	b = ڈیٹا[i+1];
	ڈیٹا[i+1] = ڈیٹا[i+2];
	ڈیٹا[i+2] = b;
      }
    }
    ورنہ
#اگر_ختم
      ڈیٹا = (غیرعلامتی حرف *) hb_blob_get_ڈیٹا (blob, خالی_اشاریہ);

    surface = cairo_image_surface_create_for_ڈیٹا (ڈیٹا,
						   CAIRO_FORMAT_ARGB32,
						   چوڑائی, اونچائی,
						   چوڑائی * 4);

    cairo_surface_سیٹ_user_ڈیٹا (surface,
				 hb_cairo_surface_blob_user_ڈیٹا_کلید,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  اگر (!surface)
    واپس جھوٹ;

  cairo_save (cr);
  /* this clip is here to work around recording surface limitations */
  cairo_rectangle (cr,
                   extents->x_bearing,
                   extents->y_bearing,
                   extents->چوڑائی,
                   extents->اونچائی);
  cairo_clip (cr);

  cairo_نمونہ_t *نمونہ = cairo_نمونہ_create_for_surface (surface);
  cairo_نمونہ_سیٹ_extend (نمونہ, CAIRO_EXTEND_PAD);

  cairo_میٹرکس_t میٹرکس = {(دوہرا) چوڑائی, 0, 0, (دوہرا) اونچائی, 0, 0};
  cairo_نمونہ_سیٹ_میٹرکس (نمونہ, &میٹرکس);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->چوڑائی, extents->اونچائی);
  cairo_سیٹ_ماخذ (cr, نمونہ);

  cairo_paint (cr);

  cairo_نمونہ_destroy (نمونہ);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  واپس سچ;
}

ساکن منطقی
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_سطر_t *color_سطر,
			   غیرعلامتی *گنتی,
			   hb_color_stop_t **stops)
{
  غیرعلامتی len = hb_color_سطر_get_color_stops (color_سطر, 0, خالی_اشاریہ, خالی_اشاریہ);
  منطقی allocated = جھوٹ;
  اگر (unlikely (!len))
    واپس جھوٹ;
  اگر (len > *گنتی)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, حجم (hb_color_stop_t));
    اگر (unlikely (!*stops))
      واپس جھوٹ;
    allocated = سچ;
  }
  hb_color_سطر_get_color_stops (color_سطر, 0, &len, *stops);
  اگر (unlikely (!len))
  {
    اگر (allocated)
    {
      hb_free (*stops);
      *stops = خالی_اشاریہ;
    }
    واپس جھوٹ;
  }
  برائے (غیرعلامتی i = 0; i < len; i++)
    اگر ((*stops)[i].is_foreground)
    {
#اگر_تعریف_شدہ HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_ماخذ
      دوہرا r, g, b, a;
      cairo_نمونہ_t *foreground = cairo_user_scaled_font_get_foreground_ماخذ (c->scaled_font);
      اگر (cairo_نمونہ_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_حالت_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      ورنہ
#اگر_ختم
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *گنتی = len;
  واپس سچ;
}

خالی
hb_cairo_paint_خطی_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 اعشاری x0, اعشاری y0,
				 اعشاری x1, اعشاری y1,
				 اعشاری x2, اعشاری y2)
{
  cairo_t *cr = c->cr;

  غیرعلامتی صحیح len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  اعشاری xx0, yy0, xx1, yy1;
  اعشاری xxx0, yyy0, xxx1, yyy1;
  اعشاری min, max;
  cairo_نمونہ_t *نمونہ;

  اگر (unlikely (!_hb_cairo_get_color_stops (c, color_سطر, &len, &stops)))
    واپس;
  hb_paint_normalize_color_سطر (stops, len, &min, &max);

  hb_paint_reduce_خطی_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  نمونہ = cairo_نمونہ_create_خطی ((دوہرا) xxx0, (دوہرا) yyy0, (دوہرا) xxx1, (دوہرا) yyy1);
  cairo_نمونہ_سیٹ_extend (نمونہ, hb_cairo_extend (hb_color_سطر_get_extend (color_سطر)));
  برائے (غیرعلامتی صحیح i = 0; i < len; i++)
    {
      دوہرا r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_نمونہ_جوڑو_color_stop_rgba (نمونہ, (دوہرا) stops[i].انحراف, r, g, b, a);
    }

  cairo_سیٹ_ماخذ (cr, نمونہ);
  cairo_paint (cr);

  cairo_نمونہ_destroy (نمونہ);

  اگر (stops != stops_)
    hb_free (stops);
}

خالی
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 اعشاری x0, اعشاری y0, اعشاری r0,
				 اعشاری x1, اعشاری y1, اعشاری r1)
{
  cairo_t *cr = c->cr;

  غیرعلامتی صحیح len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  اعشاری min, max;
  اعشاری xx0, yy0, xx1, yy1;
  اعشاری rr0, rr1;
  cairo_نمونہ_t *نمونہ;

  اگر (unlikely (!_hb_cairo_get_color_stops (c, color_سطر, &len, &stops)))
    واپس;
  hb_paint_normalize_color_سطر (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  نمونہ = cairo_نمونہ_create_radial ((دوہرا) xx0, (دوہرا) yy0, (دوہرا) rr0, (دوہرا) xx1, (دوہرا) yy1, (دوہرا) rr1);
  cairo_نمونہ_سیٹ_extend (نمونہ, hb_cairo_extend (hb_color_سطر_get_extend (color_سطر)));

  برائے (غیرعلامتی صحیح i = 0; i < len; i++)
    {
      دوہرا r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_نمونہ_جوڑو_color_stop_rgba (نمونہ, (دوہرا) stops[i].انحراف, r, g, b, a);
    }

  cairo_سیٹ_ماخذ (cr, نمونہ);
  cairo_paint (cr);

  cairo_نمونہ_destroy (نمونہ);

  اگر (stops != stops_)
    hb_free (stops);
}

قسمتعریف ساخت {
  اعشاری x, y;
} hb_cairo_نقطہ_t;

ساکن اندرونی اعشاری
_hb_cairo_interpolate (اعشاری f0, اعشاری f1, اعشاری f)
{
  واپس f0 + f * (f1 - f0);
}

ساکن اندرونی خالی
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

ساکن اندرونی خالی
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  اگر (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

ساکن خالی
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

ساکن اندرونی اعشاری
_hb_cairo_dot (hb_cairo_نقطہ_t p, hb_cairo_نقطہ_t q)
{
  واپس p.x * q.x + p.y * q.y;
}

ساکن اندرونی hb_cairo_نقطہ_t
_hb_cairo_normalize (hb_cairo_نقطہ_t p)
{
  اعشاری len = sqrtf (_hb_cairo_dot (p, p));

  واپس hb_cairo_نقطہ_t { p.x / len, p.y / len };
}

ساکن اندرونی hb_cairo_نقطہ_t
hb_cairo_مجموع (hb_cairo_نقطہ_t p, hb_cairo_نقطہ_t q)
{
  واپس hb_cairo_نقطہ_t { p.x + q.x, p.y + q.y };
}

ساکن اندرونی hb_cairo_نقطہ_t
_hb_cairo_difference (hb_cairo_نقطہ_t p, hb_cairo_نقطہ_t q)
{
  واپس hb_cairo_نقطہ_t { p.x - q.x, p.y - q.y };
}

ساکن اندرونی hb_cairo_نقطہ_t
_hb_cairo_scale (hb_cairo_نقطہ_t p, اعشاری f)
{
  واپس hb_cairo_نقطہ_t { p.x * f, p.y * f };
}

قسمتعریف ساخت {
  hb_cairo_نقطہ_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

ساکن خالی
hb_cairo_جوڑو_patch (cairo_نمونہ_t *نمونہ, hb_cairo_نقطہ_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_نمونہ_begin_patch (نمونہ);
  cairo_mesh_نمونہ_move_مقصد (نمونہ, (دوہرا) center->x, (دوہرا) center->y);
  cairo_mesh_نمونہ_سطر_مقصد (نمونہ, (دوہرا) p->p0.x, (دوہرا) p->p0.y);
  cairo_mesh_نمونہ_خم_مقصد (نمونہ,
                               (دوہرا) p->c0.x, (دوہرا) p->c0.y,
                               (دوہرا) p->c1.x, (دوہرا) p->c1.y,
                               (دوہرا) p->p1.x, (دوہرا) p->p1.y);
  cairo_mesh_نمونہ_سطر_مقصد (نمونہ, (دوہرا) center->x, (دوہرا) center->y);
  cairo_mesh_نمونہ_سیٹ_کونا_color_rgba (نمونہ, 0,
                                            (دوہرا) p->color0.r,
                                            (دوہرا) p->color0.g,
                                            (دوہرا) p->color0.b,
                                            (دوہرا) p->color0.a);
  cairo_mesh_نمونہ_سیٹ_کونا_color_rgba (نمونہ, 1,
                                            (دوہرا) p->color0.r,
                                            (دوہرا) p->color0.g,
                                            (دوہرا) p->color0.b,
                                            (دوہرا) p->color0.a);
  cairo_mesh_نمونہ_سیٹ_کونا_color_rgba (نمونہ, 2,
                                            (دوہرا) p->color1.r,
                                            (دوہرا) p->color1.g,
                                            (دوہرا) p->color1.b,
                                            (دوہرا) p->color1.a);
  cairo_mesh_نمونہ_سیٹ_کونا_color_rgba (نمونہ, 3,
                                            (دوہرا) p->color1.r,
                                            (دوہرا) p->color1.g,
                                            (دوہرا) p->color1.b,
                                            (دوہرا) p->color1.a);
  cairo_mesh_نمونہ_انتہا_patch (نمونہ);
}

#تعریف_کرو MAX_ANGLE (HB_PI / 8.f)

ساکن خالی
hb_cairo_جوڑو_sweep_gradient_patches1 (اعشاری cx, اعشاری cy, اعشاری radius,
				       اعشاری a0, hb_cairo_color_t *c0,
				       اعشاری a1, hb_cairo_color_t *c1,
				       cairo_نمونہ_t *نمونہ)
{
  hb_cairo_نقطہ_t center = hb_cairo_نقطہ_t { cx, cy };
  صحیح عدد_splits;
  hb_cairo_نقطہ_t p0;
  hb_cairo_color_t color0, color1;

  عدد_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_نقطہ_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  برائے (صحیح a = 0; a < عدد_splits; a++)
    {
      اعشاری k = (a + 1.) / عدد_splits;
      اعشاری angle1;
      hb_cairo_نقطہ_t p1;
      hb_cairo_نقطہ_t A, U;
      hb_cairo_نقطہ_t C0, C1;
      hb_cairo_patch_t patch;

      angle1 = _hb_cairo_interpolate (a0, a1, k);
      _hb_cairo_interpolate_colors (c0, c1, k, &color1);

      patch.color0 = color0;
      patch.color1 = color1;

      p1 = hb_cairo_نقطہ_t { cosf (angle1), sinf (angle1) };
      patch.p0 = hb_cairo_مجموع (center, _hb_cairo_scale (p0, radius));
      patch.p1 = hb_cairo_مجموع (center, _hb_cairo_scale (p1, radius));

      A = _hb_cairo_normalize (hb_cairo_مجموع (p0, p1));
      U = hb_cairo_نقطہ_t { -A.y, A.x };
      C0 = hb_cairo_مجموع (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p0, A), p0) / _hb_cairo_dot (U, p0)));
      C1 = hb_cairo_مجموع (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p1, A), p1) / _hb_cairo_dot (U, p1)));

      patch.c0 = hb_cairo_مجموع (center, _hb_cairo_scale (hb_cairo_مجموع (C0, _hb_cairo_scale (_hb_cairo_difference (C0, p0), 0.33333f)), radius));
      patch.c1 = hb_cairo_مجموع (center, _hb_cairo_scale (hb_cairo_مجموع (C1, _hb_cairo_scale (_hb_cairo_difference (C1, p1), 0.33333f)), radius));

      hb_cairo_جوڑو_patch (نمونہ, &center, &patch);

      p0 = p1;
      color0 = color1;
    }
}

ساکن اندرونی hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  واپس {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

ساخت _hb_cairo_sweep_patch_ctx {
  اعشاری cx, cy, radius;
  cairo_نمونہ_t *نمونہ;
};

ساکن خالی
_hb_cairo_sweep_emit_patch (اعشاری a0, hb_color_t c0,
			    اعشاری a1, hb_color_t c1,
			    خالی *user_ڈیٹا)
{
  خودکار *ctx = (_hb_cairo_sweep_patch_ctx *) user_ڈیٹا;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  hb_cairo_جوڑو_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->نمونہ);
}

ساکن خالی
hb_cairo_جوڑو_sweep_gradient_patches (hb_color_stop_t *stops,
				      غیرعلامتی صحیح n_stops,
				      cairo_extend_t extend,
				      اعشاری cx, اعشاری cy,
				      اعشاری radius,
				      اعشاری آغاز_angle,
				      اعشاری انتہا_angle,
				      cairo_نمونہ_t *نمونہ)
{
  hb_paint_extend_t hb_extend =
    extend == CAIRO_EXTEND_REPEAT  ? HB_PAINT_EXTEND_REPEAT  :
    extend == CAIRO_EXTEND_REFLECT ? HB_PAINT_EXTEND_REFLECT :
				     HB_PAINT_EXTEND_PAD;

  _hb_cairo_sweep_patch_ctx ctx { cx, cy, radius, نمونہ };
  hb_paint_sweep_gradient_tiles (stops, n_stops, hb_extend,
				 آغاز_angle, انتہا_angle,
				 _hb_cairo_sweep_emit_patch, &ctx);
}

خالی
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_سطر_t *color_سطر,
				اعشاری cx, اعشاری cy,
				اعشاری آغاز_angle,
				اعشاری انتہا_angle)
{
  cairo_t *cr = c->cr;

  غیرعلامتی صحیح len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  دوہرا x1, y1, x2, y2;
  اعشاری max_x, max_y, radius;
  cairo_نمونہ_t *نمونہ;

  اگر (unlikely (!_hb_cairo_get_color_stops (c, color_سطر, &len, &stops)))
    واپس;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b) {
      واپس (a.انحراف > b.انحراف) - (a.انحراف < b.انحراف);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (اعشاری) hb_max ((x1 - (دوہرا) cx) * (x1 - (دوہرا) cx), (x2 - (دوہرا) cx) * (x2 - (دوہرا) cx));
  max_y = (اعشاری) hb_max ((y1 - (دوہرا) cy) * (y1 - (دوہرا) cy), (y2 - (دوہرا) cy) * (y2 - (دوہرا) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_سطر_get_extend (color_سطر));
  نمونہ = cairo_نمونہ_create_mesh ();

  hb_cairo_جوڑو_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, آغاز_angle, انتہا_angle, نمونہ);

  cairo_سیٹ_ماخذ (cr, نمونہ);
  cairo_paint (cr);

  cairo_نمونہ_destroy (نمونہ);

  اگر (stops != stops_)
    hb_free (stops);
}

#اگر_ختم
