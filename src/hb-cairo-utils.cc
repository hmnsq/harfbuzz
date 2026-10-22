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

#ضمن "hb.hh"

#اگر_تعریف_شده HAVE_CAIRO

#ضمن "hb-cairo-utils.hh"
#ضمن "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#تعریف_کن PREALLOCATED_COLOR_STOPS 16

تعريفنوع بنية {
  عائم r, g, b, a;
} hb_cairo_color_t;

ساكن مضمن cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  بدل (extend)
    {
    حالة HB_PAINT_EXTEND_PAD: أرجع CAIRO_EXTEND_PAD;
    حالة HB_PAINT_EXTEND_REPEAT: أرجع CAIRO_EXTEND_REPEAT;
    حالة HB_PAINT_EXTEND_REFLECT: أرجع CAIRO_EXTEND_REFLECT;
    افتراضي: اكسر;
    }

  أرجع CAIRO_EXTEND_PAD;
}

#اگر_تعریف_شده CAIRO_HAS_PNG_FUNCTIONS
تعريفنوع بنية
{
  hb_blob_t *blob;
  بیعلامت صحيح جابه_جایی;
} hb_cairo_read_blob_داده_t;

ساكن cairo_وضعیت_t
hb_cairo_read_blob (فراغ *بستگی,
		    بیعلامت حرف *داده,
		    بیعلامت صحيح درازی)
{
  hb_cairo_read_blob_داده_t *r = (hb_cairo_read_blob_داده_t *) بستگی;
  ثابت حرف *d;
  بیعلامت صحيح اندازه;

  d = hb_blob_get_داده (r->blob, &اندازه);

  إذا (r->جابه_جایی + درازی > اندازه)
    أرجع CAIRO_وضعیت_READ_ERROR;

  hb_memcpy (داده, d + r->جابه_جایی, درازی);
  r->جابه_جایی += درازی;

  أرجع CAIRO_وضعیت_SUCCESS;
}
#پایان_اگر

ساكن ثابت cairo_user_داده_کلید_t *hb_cairo_surface_blob_user_داده_کلید = {0};

ساكن فراغ
_hb_cairo_destroy_blob (فراغ *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

فراغ
hb_cairo_مجموعه_منبع_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  إذا (use_foreground)
  {
#اگر_تعریف_شده HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_منبع
    مزدوج r, g, b, a;
    cairo_الگو_t *foreground = cairo_user_scaled_font_get_foreground_منبع (c->scaled_font);
    إذا (cairo_الگو_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_وضعیت_SUCCESS)
      cairo_مجموعه_منبع_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    وإلا
#پایان_اگر
      cairo_مجموعه_منبع_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  وإلا
    cairo_مجموعه_منبع_rgba (cr,
			   hb_color_get_red (color) / 255.,
			   hb_color_get_green (color) / 255.,
			   hb_color_get_blue (color) / 255.,
			   hb_color_get_alpha (color) / 255.);
}

hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     بیعلامت عرض,
			     بیعلامت ارتفاع,
			     hb_برچسب_t format,
			     HB_استفادهنشده عائم slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  إذا (!extents) /* SVG currently. */
    أرجع خطأ;

  cairo_surface_t *surface = اشاره_تهی;

#اگر_تعریف_شده CAIRO_HAS_PNG_FUNCTIONS
  إذا (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
    hb_cairo_read_blob_داده_t r;
    r.blob = blob;
    r.جابه_جایی = 0;
    surface = cairo_image_surface_create_from_png_جریان (hb_cairo_read_blob, &r);

    /* For PNG, width,height can be unreliable, as is the case for NotoColorEmoji :(.
     * Just pull them out of the surface. */
    عرض = cairo_image_surface_get_عرض (surface);
    ارتفاع = cairo_image_surface_get_عرض (surface);
  }
  وإلا
#پایان_اگر
  إذا (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    بیعلامت داده_اندازه = hb_blob_get_درازی (blob);
    إذا (داده_اندازه < عرض * ارتفاع * 4)
      أرجع خطأ;

    بیعلامت حرف *داده;
#اگر_تعریف_شده __BYTE_ORDER
    إذا (__BYTE_ORDER == __BIG_ENDIAN)
    {
      داده = (بیعلامت حرف *) hb_blob_get_داده_writable (blob, اشاره_تهی);
      إذا (!داده)
        أرجع خطأ;

      بیعلامت شمار = عرض * ارتفاع * 4;
      لكل (بیعلامت i = 0; i < شمار; i += 4)
      {
        بیعلامت حرف b;
	b = داده[i];
	داده[i] = داده[i+3];
	داده[i+3] = b;
	b = داده[i+1];
	داده[i+1] = داده[i+2];
	داده[i+2] = b;
      }
    }
    وإلا
#پایان_اگر
      داده = (بیعلامت حرف *) hb_blob_get_داده (blob, اشاره_تهی);

    surface = cairo_image_surface_create_for_داده (داده,
						   CAIRO_FORMAT_ARGB32,
						   عرض, ارتفاع,
						   عرض * 4);

    cairo_surface_مجموعه_user_داده (surface,
				 hb_cairo_surface_blob_user_داده_کلید,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  إذا (!surface)
    أرجع خطأ;

  cairo_save (cr);
  /* this clip is here to work around recording surface limitations */
  cairo_rectangle (cr,
                   extents->x_bearing,
                   extents->y_bearing,
                   extents->عرض,
                   extents->ارتفاع);
  cairo_clip (cr);

  cairo_الگو_t *الگو = cairo_الگو_create_for_surface (surface);
  cairo_الگو_مجموعه_extend (الگو, CAIRO_EXTEND_PAD);

  cairo_ماتریس_t ماتریس = {(مزدوج) عرض, 0, 0, (مزدوج) ارتفاع, 0, 0};
  cairo_الگو_مجموعه_ماتریس (الگو, &ماتریس);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->عرض, extents->ارتفاع);
  cairo_مجموعه_منبع (cr, الگو);

  cairo_paint (cr);

  cairo_الگو_destroy (الگو);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  أرجع درست;
}

ساكن منطقي
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_سطر_t *color_سطر,
			   بیعلامت *شمار,
			   hb_color_stop_t **stops)
{
  بیعلامت len = hb_color_سطر_get_color_stops (color_سطر, 0, اشاره_تهی, اشاره_تهی);
  منطقي allocated = خطأ;
  إذا (unlikely (!len))
    أرجع خطأ;
  إذا (len > *شمار)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, حجم (hb_color_stop_t));
    إذا (unlikely (!*stops))
      أرجع خطأ;
    allocated = درست;
  }
  hb_color_سطر_get_color_stops (color_سطر, 0, &len, *stops);
  إذا (unlikely (!len))
  {
    إذا (allocated)
    {
      hb_free (*stops);
      *stops = اشاره_تهی;
    }
    أرجع خطأ;
  }
  لكل (بیعلامت i = 0; i < len; i++)
    إذا ((*stops)[i].is_foreground)
    {
#اگر_تعریف_شده HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_منبع
      مزدوج r, g, b, a;
      cairo_الگو_t *foreground = cairo_user_scaled_font_get_foreground_منبع (c->scaled_font);
      إذا (cairo_الگو_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_وضعیت_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      وإلا
#پایان_اگر
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *شمار = len;
  أرجع درست;
}

فراغ
hb_cairo_paint_خطی_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 عائم x0, عائم y0,
				 عائم x1, عائم y1,
				 عائم x2, عائم y2)
{
  cairo_t *cr = c->cr;

  بیعلامت صحيح len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  عائم xx0, yy0, xx1, yy1;
  عائم xxx0, yyy0, xxx1, yyy1;
  عائم min, max;
  cairo_الگو_t *الگو;

  إذا (unlikely (!_hb_cairo_get_color_stops (c, color_سطر, &len, &stops)))
    أرجع;
  hb_paint_normalize_color_سطر (stops, len, &min, &max);

  hb_paint_reduce_خطی_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  الگو = cairo_الگو_create_خطی ((مزدوج) xxx0, (مزدوج) yyy0, (مزدوج) xxx1, (مزدوج) yyy1);
  cairo_الگو_مجموعه_extend (الگو, hb_cairo_extend (hb_color_سطر_get_extend (color_سطر)));
  لكل (بیعلامت صحيح i = 0; i < len; i++)
    {
      مزدوج r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_الگو_جمع_کن_color_stop_rgba (الگو, (مزدوج) stops[i].جابه_جایی, r, g, b, a);
    }

  cairo_مجموعه_منبع (cr, الگو);
  cairo_paint (cr);

  cairo_الگو_destroy (الگو);

  إذا (stops != stops_)
    hb_free (stops);
}

فراغ
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_سطر_t *color_سطر,
				 عائم x0, عائم y0, عائم r0,
				 عائم x1, عائم y1, عائم r1)
{
  cairo_t *cr = c->cr;

  بیعلامت صحيح len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  عائم min, max;
  عائم xx0, yy0, xx1, yy1;
  عائم rr0, rr1;
  cairo_الگو_t *الگو;

  إذا (unlikely (!_hb_cairo_get_color_stops (c, color_سطر, &len, &stops)))
    أرجع;
  hb_paint_normalize_color_سطر (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  الگو = cairo_الگو_create_radial ((مزدوج) xx0, (مزدوج) yy0, (مزدوج) rr0, (مزدوج) xx1, (مزدوج) yy1, (مزدوج) rr1);
  cairo_الگو_مجموعه_extend (الگو, hb_cairo_extend (hb_color_سطر_get_extend (color_سطر)));

  لكل (بیعلامت صحيح i = 0; i < len; i++)
    {
      مزدوج r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_الگو_جمع_کن_color_stop_rgba (الگو, (مزدوج) stops[i].جابه_جایی, r, g, b, a);
    }

  cairo_مجموعه_منبع (cr, الگو);
  cairo_paint (cr);

  cairo_الگو_destroy (الگو);

  إذا (stops != stops_)
    hb_free (stops);
}

تعريفنوع بنية {
  عائم x, y;
} hb_cairo_نقطه_t;

ساكن مضمن عائم
_hb_cairo_interpolate (عائم f0, عائم f1, عائم f)
{
  أرجع f0 + f * (f1 - f0);
}

ساكن مضمن فراغ
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

ساكن مضمن فراغ
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  إذا (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

ساكن فراغ
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, عائم k, hb_cairo_color_t *c)
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

ساكن مضمن عائم
_hb_cairo_dot (hb_cairo_نقطه_t p, hb_cairo_نقطه_t q)
{
  أرجع p.x * q.x + p.y * q.y;
}

ساكن مضمن hb_cairo_نقطه_t
_hb_cairo_normalize (hb_cairo_نقطه_t p)
{
  عائم len = sqrtf (_hb_cairo_dot (p, p));

  أرجع hb_cairo_نقطه_t { p.x / len, p.y / len };
}

ساكن مضمن hb_cairo_نقطه_t
hb_cairo_جمع (hb_cairo_نقطه_t p, hb_cairo_نقطه_t q)
{
  أرجع hb_cairo_نقطه_t { p.x + q.x, p.y + q.y };
}

ساكن مضمن hb_cairo_نقطه_t
_hb_cairo_difference (hb_cairo_نقطه_t p, hb_cairo_نقطه_t q)
{
  أرجع hb_cairo_نقطه_t { p.x - q.x, p.y - q.y };
}

ساكن مضمن hb_cairo_نقطه_t
_hb_cairo_scale (hb_cairo_نقطه_t p, عائم f)
{
  أرجع hb_cairo_نقطه_t { p.x * f, p.y * f };
}

تعريفنوع بنية {
  hb_cairo_نقطه_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

ساكن فراغ
hb_cairo_جمع_کن_patch (cairo_الگو_t *الگو, hb_cairo_نقطه_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_الگو_begin_patch (الگو);
  cairo_mesh_الگو_move_مقصد (الگو, (مزدوج) center->x, (مزدوج) center->y);
  cairo_mesh_الگو_سطر_مقصد (الگو, (مزدوج) p->p0.x, (مزدوج) p->p0.y);
  cairo_mesh_الگو_منحنی_مقصد (الگو,
                               (مزدوج) p->c0.x, (مزدوج) p->c0.y,
                               (مزدوج) p->c1.x, (مزدوج) p->c1.y,
                               (مزدوج) p->p1.x, (مزدوج) p->p1.y);
  cairo_mesh_الگو_سطر_مقصد (الگو, (مزدوج) center->x, (مزدوج) center->y);
  cairo_mesh_الگو_مجموعه_گوشه_color_rgba (الگو, 0,
                                            (مزدوج) p->color0.r,
                                            (مزدوج) p->color0.g,
                                            (مزدوج) p->color0.b,
                                            (مزدوج) p->color0.a);
  cairo_mesh_الگو_مجموعه_گوشه_color_rgba (الگو, 1,
                                            (مزدوج) p->color0.r,
                                            (مزدوج) p->color0.g,
                                            (مزدوج) p->color0.b,
                                            (مزدوج) p->color0.a);
  cairo_mesh_الگو_مجموعه_گوشه_color_rgba (الگو, 2,
                                            (مزدوج) p->color1.r,
                                            (مزدوج) p->color1.g,
                                            (مزدوج) p->color1.b,
                                            (مزدوج) p->color1.a);
  cairo_mesh_الگو_مجموعه_گوشه_color_rgba (الگو, 3,
                                            (مزدوج) p->color1.r,
                                            (مزدوج) p->color1.g,
                                            (مزدوج) p->color1.b,
                                            (مزدوج) p->color1.a);
  cairo_mesh_الگو_انتها_patch (الگو);
}

#تعریف_کن MAX_ANGLE (HB_PI / 8.f)

ساكن فراغ
hb_cairo_جمع_کن_sweep_gradient_patches1 (عائم cx, عائم cy, عائم radius,
				       عائم a0, hb_cairo_color_t *c0,
				       عائم a1, hb_cairo_color_t *c1,
				       cairo_الگو_t *الگو)
{
  hb_cairo_نقطه_t center = hb_cairo_نقطه_t { cx, cy };
  صحيح رقم_splits;
  hb_cairo_نقطه_t p0;
  hb_cairo_color_t color0, color1;

  رقم_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_نقطه_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  لكل (صحيح a = 0; a < رقم_splits; a++)
    {
      عائم k = (a + 1.) / رقم_splits;
      عائم angle1;
      hb_cairo_نقطه_t p1;
      hb_cairo_نقطه_t A, U;
      hb_cairo_نقطه_t C0, C1;
      hb_cairo_patch_t patch;

      angle1 = _hb_cairo_interpolate (a0, a1, k);
      _hb_cairo_interpolate_colors (c0, c1, k, &color1);

      patch.color0 = color0;
      patch.color1 = color1;

      p1 = hb_cairo_نقطه_t { cosf (angle1), sinf (angle1) };
      patch.p0 = hb_cairo_جمع (center, _hb_cairo_scale (p0, radius));
      patch.p1 = hb_cairo_جمع (center, _hb_cairo_scale (p1, radius));

      A = _hb_cairo_normalize (hb_cairo_جمع (p0, p1));
      U = hb_cairo_نقطه_t { -A.y, A.x };
      C0 = hb_cairo_جمع (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p0, A), p0) / _hb_cairo_dot (U, p0)));
      C1 = hb_cairo_جمع (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p1, A), p1) / _hb_cairo_dot (U, p1)));

      patch.c0 = hb_cairo_جمع (center, _hb_cairo_scale (hb_cairo_جمع (C0, _hb_cairo_scale (_hb_cairo_difference (C0, p0), 0.33333f)), radius));
      patch.c1 = hb_cairo_جمع (center, _hb_cairo_scale (hb_cairo_جمع (C1, _hb_cairo_scale (_hb_cairo_difference (C1, p1), 0.33333f)), radius));

      hb_cairo_جمع_کن_patch (الگو, &center, &patch);

      p0 = p1;
      color0 = color1;
    }
}

ساكن مضمن hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  أرجع {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

بنية _hb_cairo_sweep_patch_ctx {
  عائم cx, cy, radius;
  cairo_الگو_t *الگو;
};

ساكن فراغ
_hb_cairo_sweep_emit_patch (عائم a0, hb_color_t c0,
			    عائم a1, hb_color_t c1,
			    فراغ *user_داده)
{
  خودکار *ctx = (_hb_cairo_sweep_patch_ctx *) user_داده;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  hb_cairo_جمع_کن_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->الگو);
}

ساكن فراغ
hb_cairo_جمع_کن_sweep_gradient_patches (hb_color_stop_t *stops,
				      بیعلامت صحيح n_stops,
				      cairo_extend_t extend,
				      عائم cx, عائم cy,
				      عائم radius,
				      عائم شروع_angle,
				      عائم انتها_angle,
				      cairo_الگو_t *الگو)
{
  hb_paint_extend_t hb_extend =
    extend == CAIRO_EXTEND_REPEAT  ? HB_PAINT_EXTEND_REPEAT  :
    extend == CAIRO_EXTEND_REFLECT ? HB_PAINT_EXTEND_REFLECT :
				     HB_PAINT_EXTEND_PAD;

  _hb_cairo_sweep_patch_ctx ctx { cx, cy, radius, الگو };
  hb_paint_sweep_gradient_tiles (stops, n_stops, hb_extend,
				 شروع_angle, انتها_angle,
				 _hb_cairo_sweep_emit_patch, &ctx);
}

فراغ
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_سطر_t *color_سطر,
				عائم cx, عائم cy,
				عائم شروع_angle,
				عائم انتها_angle)
{
  cairo_t *cr = c->cr;

  بیعلامت صحيح len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  مزدوج x1, y1, x2, y2;
  عائم max_x, max_y, radius;
  cairo_الگو_t *الگو;

  إذا (unlikely (!_hb_cairo_get_color_stops (c, color_سطر, &len, &stops)))
    أرجع;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b) {
      أرجع (a.جابه_جایی > b.جابه_جایی) - (a.جابه_جایی < b.جابه_جایی);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (عائم) hb_max ((x1 - (مزدوج) cx) * (x1 - (مزدوج) cx), (x2 - (مزدوج) cx) * (x2 - (مزدوج) cx));
  max_y = (عائم) hb_max ((y1 - (مزدوج) cy) * (y1 - (مزدوج) cy), (y2 - (مزدوج) cy) * (y2 - (مزدوج) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_سطر_get_extend (color_سطر));
  الگو = cairo_الگو_create_mesh ();

  hb_cairo_جمع_کن_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, شروع_angle, انتها_angle, الگو);

  cairo_مجموعه_منبع (cr, الگو);
  cairo_paint (cr);

  cairo_الگو_destroy (الگو);

  إذا (stops != stops_)
    hb_free (stops);
}

#پایان_اگر
