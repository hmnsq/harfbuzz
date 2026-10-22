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

#समावेश "hb.hh"

#अगर_घोषित HAVE_CAIRO

#समावेश "hb-cairo-utils.hh"
#समावेश "hb-paint.hh"

/* Some routines in this file were ported from BlackRenderer by Black Foundry.
 * Used by permission to relicense to HarfBuzz license.
 *
 * https://github.com/BlackFoundryCom/black-renderer
 */

#घोषणा PREALLOCATED_COLOR_STOPS 16

प्रकार काष्ठा {
  भग्न r, g, b, a;
} hb_cairo_color_t;

अटल अंतरभूत cairo_extend_t
hb_cairo_extend (hb_paint_extend_t extend)
{
  चयन (extend)
    {
    हाल HB_PAINT_EXTEND_PAD: वापस CAIRO_EXTEND_PAD;
    हाल HB_PAINT_EXTEND_REPEAT: वापस CAIRO_EXTEND_REPEAT;
    हाल HB_PAINT_EXTEND_REFLECT: वापस CAIRO_EXTEND_REFLECT;
    शेष: अवरोध;
    }

  वापस CAIRO_EXTEND_PAD;
}

#अगर_घोषित CAIRO_HAS_PNG_FUNCTIONS
प्रकार काष्ठा
{
  hb_blob_t *blob;
  अचिन्हित पूर्णांक offset;
} hb_cairo_read_blob_तथ्य_t;

अटल cairo_status_t
hb_cairo_read_blob (व्योम *closure,
		    अचिन्हित अक्षर *तथ्य,
		    अचिन्हित पूर्णांक लंबाई)
{
  hb_cairo_read_blob_तथ्य_t *r = (hb_cairo_read_blob_तथ्य_t *) closure;
  स्थिर अक्षर *d;
  अचिन्हित पूर्णांक आकार;

  d = hb_blob_get_तथ्य (r->blob, &आकार);

  अगर (r->offset + लंबाई > आकार)
    वापस CAIRO_STATUS_READ_ERROR;

  hb_memcpy (तथ्य, d + r->offset, लंबाई);
  r->offset += लंबाई;

  वापस CAIRO_STATUS_SUCCESS;
}
#पूर्ण_अगर

अटल स्थिर cairo_user_तथ्य_कुंजी_t *hb_cairo_surface_blob_user_तथ्य_कुंजी = {0};

अटल व्योम
_hb_cairo_destroy_blob (व्योम *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

व्योम
_hb_cairo_set_source_color (hb_cairo_context_t *c,
			    hb_bool_t use_foreground,
			    hb_color_t color)
{
  cairo_t *cr = c->cr;

  अगर (use_foreground)
  {
#अगर_घोषित HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
    द्विगुन r, g, b, a;
    cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
    अगर (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
      cairo_set_source_rgba (cr, r, g, b, a * hb_color_get_alpha (color) / 255.);
    अन्यथा
#पूर्ण_अगर
      cairo_set_source_rgba (cr, 0, 0, 0, hb_color_get_alpha (color) / 255.);
  }
  अन्यथा
    cairo_set_source_rgba (cr,
			   hb_color_get_red (color) / 255.,
			   hb_color_get_green (color) / 255.,
			   hb_color_get_blue (color) / 255.,
			   hb_color_get_alpha (color) / 255.);
}

hb_bool_t
_hb_cairo_paint_glyph_image (hb_cairo_context_t *c,
			     hb_blob_t *blob,
			     अचिन्हित width,
			     अचिन्हित height,
			     hb_tag_t format,
			     HB_UNUSED भग्न slant_deprecated,
			     hb_glyph_extents_t *extents)
{
  cairo_t *cr = c->cr;

  अगर (!extents) /* SVG currently. */
    वापस false;

  cairo_surface_t *surface = nullptr;

#अगर_घोषित CAIRO_HAS_PNG_FUNCTIONS
  अगर (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
    hb_cairo_read_blob_तथ्य_t r;
    r.blob = blob;
    r.offset = 0;
    surface = cairo_image_surface_create_from_png_stream (hb_cairo_read_blob, &r);

    /* For PNG, width,height can be unreliable, as is the case for NotoColorEmoji :(.
     * Just pull them out of the surface. */
    width = cairo_image_surface_get_width (surface);
    height = cairo_image_surface_get_width (surface);
  }
  अन्यथा
#पूर्ण_अगर
  अगर (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    /* Byte-endian conversion. */
    अचिन्हित तथ्य_आकार = hb_blob_get_लंबाई (blob);
    अगर (तथ्य_आकार < width * height * 4)
      वापस false;

    अचिन्हित अक्षर *तथ्य;
#अगर_घोषित __BYTE_ORDER
    अगर (__BYTE_ORDER == __BIG_ENDIAN)
    {
      तथ्य = (अचिन्हित अक्षर *) hb_blob_get_तथ्य_writable (blob, nullptr);
      अगर (!तथ्य)
        वापस false;

      अचिन्हित गिनती = width * height * 4;
      क्रम (अचिन्हित i = 0; i < गिनती; i += 4)
      {
        अचिन्हित अक्षर b;
	b = तथ्य[i];
	तथ्य[i] = तथ्य[i+3];
	तथ्य[i+3] = b;
	b = तथ्य[i+1];
	तथ्य[i+1] = तथ्य[i+2];
	तथ्य[i+2] = b;
      }
    }
    अन्यथा
#पूर्ण_अगर
      तथ्य = (अचिन्हित अक्षर *) hb_blob_get_तथ्य (blob, nullptr);

    surface = cairo_image_surface_create_for_तथ्य (तथ्य,
						   CAIRO_FORMAT_ARGB32,
						   width, height,
						   width * 4);

    cairo_surface_set_user_तथ्य (surface,
				 hb_cairo_surface_blob_user_तथ्य_कुंजी,
				 hb_blob_reference (blob),
				 _hb_cairo_destroy_blob);
  }

  अगर (!surface)
    वापस false;

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

  cairo_matrix_t matrix = {(द्विगुन) width, 0, 0, (द्विगुन) height, 0, 0};
  cairo_pattern_set_matrix (pattern, &matrix);

  cairo_translate (cr, extents->x_bearing, extents->y_bearing);
  cairo_scale (cr, extents->width, extents->height);
  cairo_set_source (cr, pattern);

  cairo_paint (cr);

  cairo_pattern_destroy (pattern);
  cairo_surface_destroy (surface);

  cairo_restore (cr);

  वापस true;
}

अटल bool
_hb_cairo_get_color_stops (hb_cairo_context_t *c,
			   hb_color_पंक्ति_t *color_पंक्ति,
			   अचिन्हित *गिनती,
			   hb_color_stop_t **stops)
{
  अचिन्हित len = hb_color_पंक्ति_get_color_stops (color_पंक्ति, 0, nullptr, nullptr);
  bool allocated = false;
  अगर (unlikely (!len))
    वापस false;
  अगर (len > *गिनती)
  {
    *stops = (hb_color_stop_t *) hb_malloc2 (len, माप (hb_color_stop_t));
    अगर (unlikely (!*stops))
      वापस false;
    allocated = true;
  }
  hb_color_पंक्ति_get_color_stops (color_पंक्ति, 0, &len, *stops);
  अगर (unlikely (!len))
  {
    अगर (allocated)
    {
      hb_free (*stops);
      *stops = nullptr;
    }
    वापस false;
  }
  क्रम (अचिन्हित i = 0; i < len; i++)
    अगर ((*stops)[i].is_foreground)
    {
#अगर_घोषित HAVE_CAIRO_USER_SCALED_FONT_GET_FOREGROUND_SOURCE
      द्विगुन r, g, b, a;
      cairo_pattern_t *foreground = cairo_user_scaled_font_get_foreground_source (c->scaled_font);
      अगर (cairo_pattern_get_rgba (foreground, &r, &g, &b, &a) == CAIRO_STATUS_SUCCESS)
        (*stops)[i].color = HB_COLOR (round (b * 255.), round (g * 255.), round (r * 255.),
                                      round (a * hb_color_get_alpha ((*stops)[i].color)));
      अन्यथा
#पूर्ण_अगर
        (*stops)[i].color = HB_COLOR (0, 0, 0, hb_color_get_alpha ((*stops)[i].color));
    }

  *गिनती = len;
  वापस true;
}

व्योम
_hb_cairo_paint_linear_gradient (hb_cairo_context_t *c,
				 hb_color_पंक्ति_t *color_पंक्ति,
				 भग्न x0, भग्न y0,
				 भग्न x1, भग्न y1,
				 भग्न x2, भग्न y2)
{
  cairo_t *cr = c->cr;

  अचिन्हित पूर्णांक len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  भग्न xx0, yy0, xx1, yy1;
  भग्न xxx0, yyy0, xxx1, yyy1;
  भग्न min, max;
  cairo_pattern_t *pattern;

  अगर (unlikely (!_hb_cairo_get_color_stops (c, color_पंक्ति, &len, &stops)))
    वापस;
  hb_paint_normalize_color_पंक्ति (stops, len, &min, &max);

  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2, &xx0, &yy0, &xx1, &yy1);

  xxx0 = xx0 + min * (xx1 - xx0);
  yyy0 = yy0 + min * (yy1 - yy0);
  xxx1 = xx0 + max * (xx1 - xx0);
  yyy1 = yy0 + max * (yy1 - yy0);

  pattern = cairo_pattern_create_linear ((द्विगुन) xxx0, (द्विगुन) yyy0, (द्विगुन) xxx1, (द्विगुन) yyy1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_पंक्ति_get_extend (color_पंक्ति)));
  क्रम (अचिन्हित पूर्णांक i = 0; i < len; i++)
    {
      द्विगुन r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (द्विगुन) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  अगर (stops != stops_)
    hb_free (stops);
}

व्योम
_hb_cairo_paint_radial_gradient (hb_cairo_context_t *c,
				 hb_color_पंक्ति_t *color_पंक्ति,
				 भग्न x0, भग्न y0, भग्न r0,
				 भग्न x1, भग्न y1, भग्न r1)
{
  cairo_t *cr = c->cr;

  अचिन्हित पूर्णांक len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  भग्न min, max;
  भग्न xx0, yy0, xx1, yy1;
  भग्न rr0, rr1;
  cairo_pattern_t *pattern;

  अगर (unlikely (!_hb_cairo_get_color_stops (c, color_पंक्ति, &len, &stops)))
    वापस;
  hb_paint_normalize_color_पंक्ति (stops, len, &min, &max);

  xx0 = x0 + min * (x1 - x0);
  yy0 = y0 + min * (y1 - y0);
  xx1 = x0 + max * (x1 - x0);
  yy1 = y0 + max * (y1 - y0);
  rr0 = r0 + min * (r1 - r0);
  rr1 = r0 + max * (r1 - r0);

  pattern = cairo_pattern_create_radial ((द्विगुन) xx0, (द्विगुन) yy0, (द्विगुन) rr0, (द्विगुन) xx1, (द्विगुन) yy1, (द्विगुन) rr1);
  cairo_pattern_set_extend (pattern, hb_cairo_extend (hb_color_पंक्ति_get_extend (color_पंक्ति)));

  क्रम (अचिन्हित पूर्णांक i = 0; i < len; i++)
    {
      द्विगुन r, g, b, a;
      r = hb_color_get_red (stops[i].color) / 255.;
      g = hb_color_get_green (stops[i].color) / 255.;
      b = hb_color_get_blue (stops[i].color) / 255.;
      a = hb_color_get_alpha (stops[i].color) / 255.;
      cairo_pattern_add_color_stop_rgba (pattern, (द्विगुन) stops[i].offset, r, g, b, a);
    }

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  अगर (stops != stops_)
    hb_free (stops);
}

प्रकार काष्ठा {
  भग्न x, y;
} hb_cairo_point_t;

अटल अंतरभूत भग्न
_hb_cairo_interpolate (भग्न f0, भग्न f1, भग्न f)
{
  वापस f0 + f * (f1 - f0);
}

अटल अंतरभूत व्योम
_hb_cairo_premultiply (hb_cairo_color_t *c)
{
  c->r *= c->a;
  c->g *= c->a;
  c->b *= c->a;
}

अटल अंतरभूत व्योम
_hb_cairo_unpremultiply (hb_cairo_color_t *c)
{
  अगर (c->a != 0.f)
  {
     c->r /= c->a;
     c->g /= c->a;
     c->b /= c->a;
  }
}

अटल व्योम
_hb_cairo_interpolate_colors (hb_cairo_color_t *c0, hb_cairo_color_t *c1, भग्न k, hb_cairo_color_t *c)
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

अटल अंतरभूत भग्न
_hb_cairo_dot (hb_cairo_point_t p, hb_cairo_point_t q)
{
  वापस p.x * q.x + p.y * q.y;
}

अटल अंतरभूत hb_cairo_point_t
_hb_cairo_normalize (hb_cairo_point_t p)
{
  भग्न len = sqrtf (_hb_cairo_dot (p, p));

  वापस hb_cairo_point_t { p.x / len, p.y / len };
}

अटल अंतरभूत hb_cairo_point_t
hb_cairo_योग (hb_cairo_point_t p, hb_cairo_point_t q)
{
  वापस hb_cairo_point_t { p.x + q.x, p.y + q.y };
}

अटल अंतरभूत hb_cairo_point_t
_hb_cairo_difference (hb_cairo_point_t p, hb_cairo_point_t q)
{
  वापस hb_cairo_point_t { p.x - q.x, p.y - q.y };
}

अटल अंतरभूत hb_cairo_point_t
_hb_cairo_scale (hb_cairo_point_t p, भग्न f)
{
  वापस hb_cairo_point_t { p.x * f, p.y * f };
}

प्रकार काष्ठा {
  hb_cairo_point_t center, p0, c0, c1, p1;
  hb_cairo_color_t color0, color1;
} hb_cairo_patch_t;

अटल व्योम
_hb_cairo_add_patch (cairo_pattern_t *pattern, hb_cairo_point_t *center, hb_cairo_patch_t *p)
{
  cairo_mesh_pattern_begin_patch (pattern);
  cairo_mesh_pattern_move_to (pattern, (द्विगुन) center->x, (द्विगुन) center->y);
  cairo_mesh_pattern_पंक्ति_to (pattern, (द्विगुन) p->p0.x, (द्विगुन) p->p0.y);
  cairo_mesh_pattern_curve_to (pattern,
                               (द्विगुन) p->c0.x, (द्विगुन) p->c0.y,
                               (द्विगुन) p->c1.x, (द्विगुन) p->c1.y,
                               (द्विगुन) p->p1.x, (द्विगुन) p->p1.y);
  cairo_mesh_pattern_पंक्ति_to (pattern, (द्विगुन) center->x, (द्विगुन) center->y);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 0,
                                            (द्विगुन) p->color0.r,
                                            (द्विगुन) p->color0.g,
                                            (द्विगुन) p->color0.b,
                                            (द्विगुन) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 1,
                                            (द्विगुन) p->color0.r,
                                            (द्विगुन) p->color0.g,
                                            (द्विगुन) p->color0.b,
                                            (द्विगुन) p->color0.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 2,
                                            (द्विगुन) p->color1.r,
                                            (द्विगुन) p->color1.g,
                                            (द्विगुन) p->color1.b,
                                            (द्विगुन) p->color1.a);
  cairo_mesh_pattern_set_corner_color_rgba (pattern, 3,
                                            (द्विगुन) p->color1.r,
                                            (द्विगुन) p->color1.g,
                                            (द्विगुन) p->color1.b,
                                            (द्विगुन) p->color1.a);
  cairo_mesh_pattern_end_patch (pattern);
}

#घोषणा MAX_ANGLE (HB_PI / 8.f)

अटल व्योम
_hb_cairo_add_sweep_gradient_patches1 (भग्न cx, भग्न cy, भग्न radius,
				       भग्न a0, hb_cairo_color_t *c0,
				       भग्न a1, hb_cairo_color_t *c1,
				       cairo_pattern_t *pattern)
{
  hb_cairo_point_t center = hb_cairo_point_t { cx, cy };
  पूर्णांक अंक_splits;
  hb_cairo_point_t p0;
  hb_cairo_color_t color0, color1;

  अंक_splits = ceilf (fabsf (a1 - a0) / MAX_ANGLE);
  p0 = hb_cairo_point_t { cosf (a0), sinf (a0) };
  color0 = *c0;

  क्रम (पूर्णांक a = 0; a < अंक_splits; a++)
    {
      भग्न k = (a + 1.) / अंक_splits;
      भग्न angle1;
      hb_cairo_point_t p1;
      hb_cairo_point_t A, U;
      hb_cairo_point_t C0, C1;
      hb_cairo_patch_t patch;

      angle1 = _hb_cairo_interpolate (a0, a1, k);
      _hb_cairo_interpolate_colors (c0, c1, k, &color1);

      patch.color0 = color0;
      patch.color1 = color1;

      p1 = hb_cairo_point_t { cosf (angle1), sinf (angle1) };
      patch.p0 = hb_cairo_योग (center, _hb_cairo_scale (p0, radius));
      patch.p1 = hb_cairo_योग (center, _hb_cairo_scale (p1, radius));

      A = _hb_cairo_normalize (hb_cairo_योग (p0, p1));
      U = hb_cairo_point_t { -A.y, A.x };
      C0 = hb_cairo_योग (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p0, A), p0) / _hb_cairo_dot (U, p0)));
      C1 = hb_cairo_योग (A, _hb_cairo_scale (U, _hb_cairo_dot (_hb_cairo_difference (p1, A), p1) / _hb_cairo_dot (U, p1)));

      patch.c0 = hb_cairo_योग (center, _hb_cairo_scale (hb_cairo_योग (C0, _hb_cairo_scale (_hb_cairo_difference (C0, p0), 0.33333f)), radius));
      patch.c1 = hb_cairo_योग (center, _hb_cairo_scale (hb_cairo_योग (C1, _hb_cairo_scale (_hb_cairo_difference (C1, p1), 0.33333f)), radius));

      _hb_cairo_add_patch (pattern, &center, &patch);

      p0 = p1;
      color0 = color1;
    }
}

अटल अंतरभूत hb_cairo_color_t
_hb_cairo_color_from_hb_color (hb_color_t c)
{
  वापस {
    hb_color_get_red   (c) / 255.f,
    hb_color_get_green (c) / 255.f,
    hb_color_get_blue  (c) / 255.f,
    hb_color_get_alpha (c) / 255.f,
  };
}

काष्ठा _hb_cairo_sweep_patch_ctx {
  भग्न cx, cy, radius;
  cairo_pattern_t *pattern;
};

अटल व्योम
_hb_cairo_sweep_emit_patch (भग्न a0, hb_color_t c0,
			    भग्न a1, hb_color_t c1,
			    व्योम *user_तथ्य)
{
  स्वतः *ctx = (_hb_cairo_sweep_patch_ctx *) user_तथ्य;
  hb_cairo_color_t cc0 = _hb_cairo_color_from_hb_color (c0);
  hb_cairo_color_t cc1 = _hb_cairo_color_from_hb_color (c1);
  _hb_cairo_add_sweep_gradient_patches1 (ctx->cx, ctx->cy, ctx->radius,
					 a0, &cc0, a1, &cc1,
					 ctx->pattern);
}

अटल व्योम
_hb_cairo_add_sweep_gradient_patches (hb_color_stop_t *stops,
				      अचिन्हित पूर्णांक n_stops,
				      cairo_extend_t extend,
				      भग्न cx, भग्न cy,
				      भग्न radius,
				      भग्न start_angle,
				      भग्न end_angle,
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

व्योम
_hb_cairo_paint_sweep_gradient (hb_cairo_context_t *c,
				hb_color_पंक्ति_t *color_पंक्ति,
				भग्न cx, भग्न cy,
				भग्न start_angle,
				भग्न end_angle)
{
  cairo_t *cr = c->cr;

  अचिन्हित पूर्णांक len = PREALLOCATED_COLOR_STOPS;
  hb_color_stop_t stops_[PREALLOCATED_COLOR_STOPS];
  hb_color_stop_t *stops = stops_;
  cairo_extend_t extend;
  द्विगुन x1, y1, x2, y2;
  भग्न max_x, max_y, radius;
  cairo_pattern_t *pattern;

  अगर (unlikely (!_hb_cairo_get_color_stops (c, color_पंक्ति, &len, &stops)))
    वापस;

  hb_array_t<hb_color_stop_t> (stops, len)
    .क्विक ([] (स्थिर hb_color_stop_t &a, स्थिर hb_color_stop_t &b) {
      वापस (a.offset > b.offset) - (a.offset < b.offset);
    });

  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  max_x = (भग्न) hb_max ((x1 - (द्विगुन) cx) * (x1 - (द्विगुन) cx), (x2 - (द्विगुन) cx) * (x2 - (द्विगुन) cx));
  max_y = (भग्न) hb_max ((y1 - (द्विगुन) cy) * (y1 - (द्विगुन) cy), (y2 - (द्विगुन) cy) * (y2 - (द्विगुन) cy));
  radius = sqrtf (max_x + max_y);

  extend = hb_cairo_extend (hb_color_पंक्ति_get_extend (color_पंक्ति));
  pattern = cairo_pattern_create_mesh ();

  _hb_cairo_add_sweep_gradient_patches (stops, len, extend, cx, cy,
					radius, start_angle, end_angle, pattern);

  cairo_set_source (cr, pattern);
  cairo_paint (cr);

  cairo_pattern_destroy (pattern);

  अगर (stops != stops_)
    hb_free (stops);
}

#पूर्ण_अगर
