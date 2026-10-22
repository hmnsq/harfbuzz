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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_CAIRO

#అన్తర్భావ "hb-cairo-utils.hh"
#అన్తర్భావ "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#నిర్వచించు PREALLOCATED_COLOR_STOPS 16

రకనిర్వచనం నిర్మాణం {
  దశాంశం r, g, b, a;
} hb_cairo_color_t;

స్థిర అంతఃవరుస cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  ఎంపిక (extend)
    {
    స్థితి HB_PAINT_EXTEND_PAD: తిరిగి CAIRO_EXTEND_PAD;
    స్థితి HB_PAINT_EXTEND_REPEAT: తిరిగి CAIRO_EXTEND_REPEAT;
    స్థితి HB_PAINT_EXTEND_REFLECT: తిరిగి CAIRO_EXTEND_REFLECT;
    అప్రమేయ: ఆపు;
    }

  తిరిగి CAIRO_EXTEND_PAD;
}

#నిర్వచితమైతే CAIRO_HAS_PNG_FUNCTIONS
రకనిర్వచనం నిర్మాణం
{
  hb_blob_t *blob;
  గుర్తులేని పూర్ణాంకం స్థానభ్రంశం;
} hb_cairo_read_blob_దత్తాంశం_t;

స్థిర cairo_స్థితి_t
hb_cairo_read_blob (శూన్య *సంవృతం,
		    గుర్తులేని అక్షరం *దత్తాంశం,
		    గుర్తులేని పూర్ణాంకం నిడివి)
{
  hb_cairo_read_blob_దత్తాంశం_t *r = (hb_cairo_read_blob_దత్తాంశం_t *) సంవృతం;
  స్థిరరాశి అక్షరం *d;
  గుర్తులేని పూర్ణాంకం కొలత;

  d = hb_blob_get_దత్తాంశం (r->blob, &కొలత);

  ఒకవేళ (r->స్థానభ్రంశం + నిడివి > కొలత)
    తిరిగి CAIRO_స్థితి_READ_ERROR;

  hb_memcpy (దత్తాంశం, d + r->స్థానభ్రంశం, నిడివి);
  r->స్థానభ్రంశం += నిడివి;

  తిరిగి CAIRO_స్థితి_SUCCESS;
}
#అయితే_ముగింపు

స్థిర స్థిరరాశి cairo_user_దత్తాంశం_కీలకం_t *hb_cairo_surface_blob_user_దత్తాంశం_కీలకం = {0};

స్థిర శూన్య
_hb_cairo_destroy_blob (శూన్య *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

శూన్య
hb_cairo_సమితి_ఆధారం_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  ఒకవేళ (use_foreground)
  {
#నిర్వచితమైతే HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_ఆధారం
    ద్విగుణ r, g, b, a;
    cairo_నమూనా_t *foreground = cairo_user_scaled_font_get_foreground_ఆధారం (c->scaled_font);
    ఒకవేళ (cairo_నమూనా_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_స్థితి_SUCCESS)
      cairo_సమితి_ఆధారం_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    లేకపోతే
#అయితే_ముగింపు
      cairo_సమితి_ఆధారం_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  లేకపోతే
    cairo_సమితి_ఆధారం_rgba (cr,
			   hb_color_get_red (color) / 255.,
			   hb_color_get_green (color) / 255.,
			   hb_color_get_blue (color) / 255.,
			   hb_color_get_alpha (color) / 255.);
}

hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     గుర్తులేని వెడల్పు,
			     గుర్తులేని ఎత్తు,
			     hb_ట్యాగ్_t format,
			     HB_వాడని దశాంశం slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  ఒకవేళ (!extents) /* SVG currently. */
    తిరిగి అబద్ధం;

  cairo_surface_t *surface = శూన్యసూచిక;

#నిర్వచితమైతే CAIRO_HAS_PNG_FUNCTIONS
  ఒకవేళ (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
    hb_cairo_read_blob_దత్తాంశం_t r;
    r.blob = blob;
    r.స్థానభ్రంశం = 0;
    surface = cairo_image_surface_create_from_png_ప్రవాహం (hb_cairo_read_blob, &r);

    /* For PNG, width,height can be unreliable, as is the case for NotoColorEmoji :(.
     * Just pull them out of the surface. */
    వెడల్పు = cairo_image_surface_get_వెడల్పు (surface);
    ఎత్తు = cairo_image_surface_get_వెడల్పు (surface);
  }
  లేకపోతే
#అయితే_ముగింపు
  ఒకవేళ (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    గుర్తులేని దత్తాంశం_కొలత = hb_blob_get_నిడివి (blob);
    ఒకవేళ (దత్తాంశం_కొలత < వెడల్పు * ఎత్తు * 4)
      తిరిగి అబద్ధం;

    గుర్తులేని అక్షరం *దత్తాంశం;
#నిర్వచితమైతే __BYTE_ORDER
    ఒకవేళ (__BYTE_ORDER == __BIG_ENDIAN)
    {
      దత్తాంశం = (గుర్తులేని అక్షరం *) hb_blob_get_దత్తాంశం_writable (blob, శూన్యసూచిక);
      ఒకవేళ (!దత్తాంశం)
        తిరిగి అబద్ధం;

      గుర్తులేని లెక్క = వెడల్పు * ఎత్తు * 4;
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i += 4)
      {
        గుర్తులేని అక్షరం b;
	b = దత్తాంశం[i];
	దత్తాంశం[i] = దత్తాంశం[i+3];
	దత్తాంశం[i+3] = b;
	b = దత్తాంశం[i+1];
	దత్తాంశం[i+1] = దత్తాంశం[i+2];
	దత్తాంశం[i+2] = b;
      }
    }
    లేకపోతే
#అయితే_ముగింపు
      దత్తాంశం = (గుర్తులేని అక్షరం *) hb_blob_get_దత్తాంశం (blob, శూన్యసూచిక);

    surface = cairo_image_surface_create_for_దత్తాంశం (దత్తాంశం,
						   CAIRO_FORMAT_ARGB32,
						   వెడల్పు, ఎత్తు,
						   వెడల్పు * 4);

    cairo_surface_సమితి_user_దత్తాంశం (surface,
				 hb_cairo_surface_blob_user_దత్తాంశం_కీలకం,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  ఒకవేళ (!surface)
    తిరిగి అబద్ధం;

  cairo_save (cr);
  /* this clip is here to work around recording surface limitations */
  cairo_rectangle (cr,
                   extents->x_bearing,
                   extents->y_bearing,
                   extents->వెడల్పు,
                   extents->ఎత్తు);
  cairo_clip (cr);

  cairo_నమూనా_t *నమూనా = cairo_నమూనా_create_for_surface (surface);
  cairo_నమూనా_సమితి_extend (నమూనా, CAIRO_EXTEND_PAD);

  cairo_మాతృక_t మాతృక = {(ద్విగుణ) వెడల్పు, 0, 0, (ద్విగుణ) ఎత్తు, 0, 0};
  cairo_నమూనా_సమితి_మాతృక (నమూనా, &మాతృక);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->వెడల్పు, extents->ఎత్తు);
  cairo_సమితి_ఆధారం (cr, నమూనా);

  cairo_paint (cr);

  cairo_నమూనా_destroy (నమూనా);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  తిరిగి నిజం;
}

స్థిర తార్కిక
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_పంక్తి_t *color_పంక్తి,
			   గుర్తులేని *లెక్క,
			   hb_color_stop_t **stops)
{
  గుర్తులేని len = hb_color_పంక్తి_get_color_stops (color_పంక్తి, 0, శూన్యసూచిక, శూన్యసూచిక);
  తార్కిక allocated = అబద్ధం;
  ఒకవేళ (unlikely (!len))
    తిరిగి అబద్ధం;
  ఒకవేళ (len > *లెక్క)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, పరిమాణం (hb_color_stop_t));
    ఒకవేళ (unlikely (!*stops))
      తిరిగి అబద్ధం;
    allocated = నిజం;
  }
  hb_color_పంక్తి_get_color_stops (color_పంక్తి, 0, &len, *stops);
  ఒకవేళ (unlikely (!len))
  {
    ఒకవేళ (allocated)
    {
      hb_free (*stops);
      *stops = శూన్యసూచిక;
    }
    తిరిగి అబద్ధం;
  }
  కొరకు (గుర్తులేని i = 0; i < len; i++)
    ఒకవేళ ((*stops)[i].is_foreground)
    {
#నిర్వచితమైతే HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_ఆధారం
      ద్విగుణ r, g, b, a;
      cairo_నమూనా_t *foreground = cairo_user_scaled_font_get_foreground_ఆధారం (c->scaled_font);
      ఒకవేళ (cairo_నమూనా_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_స్థితి_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      లేకపోతే
#అయితే_ముగింపు
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *లెక్క = len;
  తిరిగి నిజం;
}

శూన్య
hb_cairo_paint_రేఖీయ_gradient (hb_cairo_context_t *c,
				 hb_color_పంక్తి_t *color_పంక్తి,
				 దశాంశం x0, దశాంశం y0,
				 దశాంశం x1, దశాంశం y1,
				 దశాంశం x2, దశాంశం y2)
{
  cairo_t *cr = c->cr;

  గుర్తులేని పూర్ణాంకం len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  దశాంశం xx0, yy0, xx1, yy1;
  దశాంశం xxx0, yyy0, xxx1, yyy1;
  దశాంశం min, max;
  cairo_నమూనా_t *నమూనా;

  ఒకవేళ (unlikely (!_hb_cairo_get_color_stops (c, color_పంక్తి, &len, &stops)))
    తిరిగి;
  hb_paint_normalize_color_పంక్తి (stops, len, &min, &max);

  hb_paint_reduce_రేఖీయ_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  నమూనా = cairo_నమూనా_create_రేఖీయ ((ద్విగుణ) xxx0, (ద్విగుణ) yyy0, (ద్విగుణ) xxx1, (ద్విగుణ) yyy1);
  cairo_నమూనా_సమితి_extend (నమూనా, hb_cairo_extend (hb_color_పంక్తి_get_extend (color_పంక్తి)));
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < len; i++)
    {
      ద్విగుణ r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_నమూనా_కూడు_color_stop_rgba (నమూనా, (ద్విగుణ) stops[i].స్థానభ్రంశం, r, g, b, a);
    }

  cairo_సమితి_ఆధారం (cr, నమూనా);
  cairo_paint (cr);

  cairo_నమూనా_destroy (నమూనా);

  ఒకవేళ (stops != stops_)
    hb_free (stops);
}

శూన్య
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_పంక్తి_t *color_పంక్తి,
				 దశాంశం x0, దశాంశం y0, దశాంశం r0,
				 దశాంశం x1, దశాంశం y1, దశాంశం r1)
{
  cairo_t *cr = c->cr;

  గుర్తులేని పూర్ణాంకం len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  దశాంశం min, max;
  దశాంశం xx0, yy0, xx1, yy1;
  దశాంశం rr0, rr1;
  cairo_నమూనా_t *నమూనా;

  ఒకవేళ (unlikely (!_hb_cairo_get_color_stops (c, color_పంక్తి, &len, &stops)))
    తిరిగి;
  hb_paint_normalize_color_పంక్తి (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  నమూనా = cairo_నమూనా_create_radial ((ద్విగుణ) xx0, (ద్విగుణ) yy0, (ద్విగుణ) rr0, (ద్విగుణ) xx1, (ద్విగుణ) yy1, (ద్విగుణ) rr1);
  cairo_నమూనా_సమితి_extend (నమూనా, hb_cairo_extend (hb_color_పంక్తి_get_extend (color_పంక్తి)));

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < len; i++)
    {
      ద్విగుణ r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_నమూనా_కూడు_color_stop_rgba (నమూనా, (ద్విగుణ) stops[i].స్థానభ్రంశం, r, g, b, a);
    }

  cairo_సమితి_ఆధారం (cr, నమూనా);
  cairo_paint (cr);

  cairo_నమూనా_destroy (నమూనా);

  ఒకవేళ (stops != stops_)
    hb_free (stops);
}

రకనిర్వచనం నిర్మాణం {
  దశాంశం x, y;
} hb_cairo_బిందువు_t;

స్థిర అంతఃవరుస దశాంశం
_hb_cairo_interpolate (దశాంశం f0, దశాంశం f1, దశాంశం f)
{
  తిరిగి f0 + f * (f1 - f0);
}

స్థిర అంతఃవరుస శూన్య
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

స్థిర అంతఃవరుస శూన్య
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  ఒకవేళ (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

స్థిర శూన్య
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, దశాంశం k, hb_cairo_color_t *c)
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

స్థిర అంతఃవరుస దశాంశం
_hb_cairo_dot (hb_cairo_బిందువు_t p, hb_cairo_బిందువు_t q)
{
  తిరిగి p.x * q.x + p.y * q.y;
}

స్థిర అంతఃవరుస hb_cairo_బిందువు_t
_hb_cairo_normalize (hb_cairo_బిందువు_t p)
{
  దశాంశం len = sqrtf (_hb_cairo_dot (p, p));

  తిరిగి hb_cairo_బిందువు_t { p.x / len, p.y / len };
}

స్థిర అంతఃవరుస hb_cairo_బిందువు_t
hb_cairo_కూడిక (hb_cairo_బిందువు_t p, hb_cairo_బిందువు_t q)
{
  తిరిగి hb_cairo_బిందువు_t { p.x + q.x, p.y + q.y };
}

స్థిర అంతఃవరుస hb_cairo_బిందువు_t
_hb_cairo_difference (hb_cairo_బిందువు_t p, hb_cairo_బిందువు_t q)
{
  తిరిగి hb_cairo_బిందువు_t { p.x - q.x, p.y - q.y };
}

స్థిర అంతఃవరుస hb_cairo_బిందువు_t
_hb_cairo_scale (hb_cairo_బిందువు_t p, దశాంశం f)
{
  తిరిగి hb_cairo_బిందువు_t { p.x * f, p.y * f };
}

రకనిర్వచనం నిర్మాణం {
  hb_cairo_బిందువు_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

స్థిర శూన్య
hb_cairo_కూడు_patch (cairo_నమూనా_t *నమూనా, hb_cairo_బిందువు_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_నమూనా_begin_patch (నమూనా);
  cairo_mesh_నమూనా_move_గమ్యం (నమూనా, (ద్విగుణ) center->x, (ద్విగుణ) center->y);
  cairo_mesh_నమూనా_పంక్తి_గమ్యం (నమూనా, (ద్విగుణ) p->p0.x, (ద్విగుణ) p->p0.y);
  cairo_mesh_నమూనా_వక్రం_గమ్యం (నమూనా,
                               (ద్విగుణ) p->c0.x, (ద్విగుణ) p->c0.y,
                               (ద్విగుణ) p->c1.x, (ద్విగుణ) p->c1.y,
                               (ద్విగుణ) p->p1.x, (ద్విగుణ) p->p1.y);
  cairo_mesh_నమూనా_పంక్తి_గమ్యం (నమూనా, (ద్విగుణ) center->x, (ద్విగుణ) center->y);
  cairo_mesh_నమూనా_సమితి_మూల_color_rgba (నమూనా, 0,
                                            (ద్విగుణ) p->color0.r,
                                            (ద్విగుణ) p->color0.g,
                                            (ద్విగుణ) p->color0.b,
                                            (ద్విగుణ) p->color0.a);
  cairo_mesh_నమూనా_సమితి_మూల_color_rgba (నమూనా, 1,
                                            (ద్విగుణ) p->color0.r,
                                            (ద్విగుణ) p->color0.g,
                                            (ద్విగుణ) p->color0.b,
                                            (ద్విగుణ) p->color0.a);
  cairo_mesh_నమూనా_సమితి_మూల_color_rgba (నమూనా, 2,
                                            (ద్విగుణ) p->color1.r,
                                            (ద్విగుణ) p->color1.g,
                                            (ద్విగుణ) p->color1.b,
                                            (ద్విగుణ) p->color1.a);
  cairo_mesh_నమూనా_సమితి_మూల_color_rgba (నమూనా, 3,
                                            (ద్విగుణ) p->color1.r,
                                            (ద్విగుణ) p->color1.g,
                                            (ద్విగుణ) p->color1.b,
                                            (ద్విగుణ) p->color1.a);
  cairo_mesh_నమూనా_చివర_patch (నమూనా);
}

#నిర్వచించు MAX_ANGLE (HB_PI / 8.f)

స్థిర శూన్య
hb_cairo_కూడు_sweep_gradient_patches1 (దశాంశం cx, దశాంశం cy, దశాంశం radius,
				       దశాంశం a0, hb_cairo_color_t *c0,
				       దశాంశం a1, hb_cairo_color_t *c1,
				       cairo_నమూనా_t *నమూనా)
{
  hb_cairo_బిందువు_t center = hb_cairo_బిందువు_t { cx, cy };
  పూర్ణాంకం అంకె_splits;
  hb_cairo_బిందువు_t p0;
  hb_cairo_color_t color0, color1;

  అంకె_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_బిందువు_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  కొరకు (పూర్ణాంకం a = 0; a < అంకె_splits; a++)
    {
      దశాంశం k = (a + 1.) / అంకె_splits;
      దశాంశం angle1;
      hb_cairo_బిందువు_t p1;
      hb_cairo_బిందువు_t A, U;
      hb_cairo_బిందువు_t C0, C1;
      hb_cairo_patch_t patch;

      angle1 = _hb_cairo_interpolate (a0, a1, k);
      _hb_cairo_interpolate_colors (c0, c1, k, &color1);

      patch.color0 = color0;
      patch.color1 = color1;

      p1 = hb_cairo_బిందువు_t { cosf (angle1), sinf (angle1) };
      patch.p0 = hb_cairo_కూడిక (center, _hb_cairo_scale (p0, radius));
      patch.p1 = hb_cairo_కూడిక (center, _hb_cairo_scale (p1, radius));

      A = _hb_cairo_normalize (hb_cairo_కూడిక (p0, p1));
      U = hb_cairo_బిందువు_t { -A.y, A.x };
      C0 = hb_cairo_కూడిక (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p0, A), p0) / _hb_cairo_dot (U, p0)));
      C1 = hb_cairo_కూడిక (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p1, A), p1) / _hb_cairo_dot (U, p1)));

      patch.c0 = hb_cairo_కూడిక (center, _hb_cairo_scale (hb_cairo_కూడిక (C0, _hb_cairo_scale (_hb_cairo_difference (C0, p0), 0.33333f)), radius));
      patch.c1 = hb_cairo_కూడిక (center, _hb_cairo_scale (hb_cairo_కూడిక (C1, _hb_cairo_scale (_hb_cairo_difference (C1, p1), 0.33333f)), radius));

      hb_cairo_కూడు_patch (నమూనా, &center, &patch);

      p0 = p1;
      color0 = color1;
    }
}

స్థిర అంతఃవరుస hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  తిరిగి {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

నిర్మాణం _hb_cairo_sweep_patch_ctx {
  దశాంశం cx, cy, radius;
  cairo_నమూనా_t *నమూనా;
};

స్థిర శూన్య
_hb_cairo_sweep_emit_patch (దశాంశం a0, hb_color_t c0,
			    దశాంశం a1, hb_color_t c1,
			    శూన్య *user_దత్తాంశం)
{
  స్వయంచాలక *ctx = (_hb_cairo_sweep_patch_ctx *) user_దత్తాంశం;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  hb_cairo_కూడు_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->నమూనా);
}

స్థిర శూన్య
hb_cairo_కూడు_sweep_gradient_patches (hb_color_stop_t *stops,
				      గుర్తులేని పూర్ణాంకం n_stops,
				      cairo_extend_t extend,
				      దశాంశం cx, దశాంశం cy,
				      దశాంశం radius,
				      దశాంశం ప్రారంభం_angle,
				      దశాంశం చివర_angle,
				      cairo_నమూనా_t *నమూనా)
{
  hb_paint_extend_t hb_extend =
    extend == CAIRO_EXTEND_REPEAT  ? HB_PAINT_EXTEND_REPEAT  :
    extend == CAIRO_EXTEND_REFLECT ? HB_PAINT_EXTEND_REFLECT :
				     HB_PAINT_EXTEND_PAD;

  _hb_cairo_sweep_patch_ctx ctx { cx, cy, radius, నమూనా };
  hb_paint_sweep_gradient_tiles (stops, n_stops, hb_extend,
				 ప్రారంభం_angle, చివర_angle,
				 _hb_cairo_sweep_emit_patch, &ctx);
}

శూన్య
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_పంక్తి_t *color_పంక్తి,
				దశాంశం cx, దశాంశం cy,
				దశాంశం ప్రారంభం_angle,
				దశాంశం చివర_angle)
{
  cairo_t *cr = c->cr;

  గుర్తులేని పూర్ణాంకం len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  ద్విగుణ x1, y1, x2, y2;
  దశాంశం max_x, max_y, radius;
  cairo_నమూనా_t *నమూనా;

  ఒకవేళ (unlikely (!_hb_cairo_get_color_stops (c, color_పంక్తి, &len, &stops)))
    తిరిగి;

  hb_array_t<hb_color_stop_t> (stops, len)
    .qsort ([] (స్థిరరాశి hb_color_stop_t &a, స్థిరరాశి hb_color_stop_t &b) {
      తిరిగి (a.స్థానభ్రంశం > b.స్థానభ్రంశం) - (a.స్థానభ్రంశం < b.స్థానభ్రంశం);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (దశాంశం) hb_max ((x1 - (ద్విగుణ) cx) * (x1 - (ద్విగుణ) cx), (x2 - (ద్విగుణ) cx) * (x2 - (ద్విగుణ) cx));
  max_y = (దశాంశం) hb_max ((y1 - (ద్విగుణ) cy) * (y1 - (ద్విగుణ) cy), (y2 - (ద్విగుణ) cy) * (y2 - (ద్విగుణ) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_పంక్తి_get_extend (color_పంక్తి));
  నమూనా = cairo_నమూనా_create_mesh ();

  hb_cairo_కూడు_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, ప్రారంభం_angle, చివర_angle, నమూనా);

  cairo_సమితి_ఆధారం (cr, నమూనా);
  cairo_paint (cr);

  cairo_నమూనా_destroy (నమూనా);

  ఒకవేళ (stops != stops_)
    hb_free (stops);
}

#అయితే_ముగింపు
