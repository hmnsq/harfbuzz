/*
 * Copyright © 2026  Behdad Esfahbod
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
 * Author(s): Behdad Esfahbod
 */

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-raster-image.hh"
#ଅନ୍ତର୍ଭାଵ "hb-geometry.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"

#ଯଦି defined(__aarch64__) || defined(_M_ARM64)
#ଅନ୍ତର୍ଭାଵ <arm_neon.h>
#define HB_RASTER_NEON 1
#ନଚେତଯଦି defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64)
#ଅନ୍ତର୍ଭାଵ <emmintrin.h>
#define HB_RASTER_SSE2 1
#endif


/* Fixed-point precision for sub-pixel coordinates.
   8 bits = 24.8: 256 sub-pixel units per pixel. */
#define HB_RASTER_PIXEL_BITS 8
#define HB_RASTER_ONE_PIXEL  (1 << HB_RASTER_PIXEL_BITS)
#define HB_RASTER_PIXEL_MASK (HB_RASTER_ONE_PIXEL - 1)
/* Full-coverage alpha = 2 * ONE_PIXEL^2 */
#define HB_RASTER_FULL_COVERAGE (2 * HB_RASTER_ONE_PIXEL * HB_RASTER_ONE_PIXEL)
/* Flatness threshold for Bézier flattening: max deviation in pixels */
#define HB_RASTER_FLAT_THRESH 0.25f


/* Normalized edge: yH > yL always */
ସଂରଚନା hb_raster_edge_t
{
  int32_t xL, yL;   /* lower endpoint (fixed-point) */
  int32_t xH, yH;   /* upper endpoint (fixed-point) */
  int64_t slope;    /* dx/dy in 16.16 fixed point: ((int64_t)dx << 16) / dy */
  int32_t wind;     /* +1 or -1 */
};

/* hb_raster_draw_t — outline rasterizer */
ସଂରଚନା hb_raster_draw_t
{
  hb_object_header_t header;

  /* Configuration */
  hb_transform_t<>    transform         = {1, 0, 0, 1, 0, 0};
  ଦଶମିକ               x_scale_factor    = 1.f;
  ଦଶମିକ               y_scale_factor    = 1.f;
  hb_raster_extents_t fixed_extents     = {};
  ତାର୍କିକ                has_extents = ମିଥ୍ୟା;

  /* Visibility clip box for curve flattening (device pixels); set
     internally by raster-paint so invisible curves collapse to their
     chord.  See hb_raster_draw_set_clip_box(). */
  ତାର୍କିକ  has_clip_box = ମିଥ୍ୟା;
  ଦଶମିକ clip_x0 = 0.f, clip_y0 = 0.f, clip_x1 = 0.f, clip_y1 = 0.f;

  /* Flattening clip resolved from the above (or from fixed extents),
     expanded by one pixel; kept in sync by hb_raster_draw_update_flatten_clip(). */
  ତାର୍କିକ  flatten_clip_active = ମିଥ୍ୟା;
  ଦଶମିକ flatten_clip_x0 = 0.f, flatten_clip_y0 = 0.f;
  ଦଶମିକ flatten_clip_x1 = 0.f, flatten_clip_y1 = 0.f;

  /* Work budget for outline traversal and curve flattening.  A paint
     backend seeds this through the public draw-budget API before drawing
     and reads the remainder back afterwards, so a whole paint session
     shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_remaining = HB_BUDGET_GLYPH;

  ଶୂନ୍ୟ recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_remaining ()
  { ଫେରନ୍ତୁ &budget_remaining; }

  /* Accumulated geometry */
  int64_t edges_ବାମ = HB_RASTER_MAX_DRAW_EDGES;
  hb_vector_t<hb_raster_edge_t> edges;

  /* Scratch — reused across render() calls */
  hb_vector_t<int32_t> row_area;
  hb_vector_t<int16_t> row_cover;
  hb_vector_t<int64_t> row_area_wide;
  hb_vector_t<int64_t> row_cover_wide;
  hb_vector_t<hb_vector_t<unsigned>> edge_buckets;
  hb_vector_t<unsigned> active_edges;

  /* Recycled image for zero-malloc render */
  hb_raster_image_t *recycled_image = nullptr;
};

ସ୍ଥିର HB_ALWAYS_INLINE ଶୂନ୍ୟ
hb_raster_draw_transform_point (ଅଚଳ hb_raster_draw_t *draw,
				ଦଶମିକ x, ଦଶମିକ y,
				ଦଶମିକ &tx, ଦଶମିକ &ty)
{
  tx = x; ty = y;
  draw->transform.transform_point (tx, ty);
  tx /= draw->x_scale_factor;
  ty /= draw->y_scale_factor;
}


/* hb_raster_draw_t */

/**
 * hb_raster_draw_create_or_fail:
 *
 * Creates a new rasterizer object.
 *
 * Return value: (transfer full):
 * A newly allocated #hb_raster_draw_t with a reference count of 1. The
 * initial reference count should be released with hb_raster_draw_destroy()
 * when you are done using the #hb_raster_draw_t, or `NULL` on
 * allocation failure.
 *
 * Since: 13.0.0
 **/
hb_raster_draw_t *
hb_raster_draw_create_or_fail (ଶୂନ୍ୟ)
{
  hb_raster_draw_t *draw = hb_object_create<hb_raster_draw_t> ();
  ଫେରନ୍ତୁ draw;
}

/**
 * hb_raster_draw_reference: (skip)
 * @draw: a rasterizer
 *
 * Increases the reference count on @draw by one.
 *
 * This prevents @draw from being destroyed until a matching
 * call to hb_raster_draw_destroy() is made.
 *
 * Return value: (transfer full):
 * The referenced #hb_raster_draw_t.
 *
 * Since: 13.0.0
 **/
hb_raster_draw_t *
hb_raster_draw_reference (hb_raster_draw_t *draw)
{
  ଫେରନ୍ତୁ hb_object_reference (draw);
}

/**
 * hb_raster_draw_destroy: (skip)
 * @draw: a rasterizer
 *
 * Decreases the reference count on @draw by one. When the
 * reference count reaches zero, the rasterizer is freed.
 *
 * Since: 13.0.0
 **/
ଶୂନ୍ୟ
hb_raster_draw_destroy (hb_raster_draw_t *draw)
{
  ଯଦି (!hb_object_should_destroy (draw))
    ଫେରନ୍ତୁ;

  hb_raster_image_destroy (draw->recycled_image);
  hb_object_actually_destroy (draw);
  hb_free (draw);
}

/**
 * hb_raster_draw_set_user_data: (skip)
 * @draw: a rasterizer
 * @key: the user-data key
 * @data: a pointer to the user data
 * @destroy: (nullable): a callback to call when @data is not needed anymore
 * @replace: whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified rasterizer.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 13.0.0
 **/
hb_bool_t
hb_raster_draw_set_user_data (hb_raster_draw_t   *draw,
			      hb_user_data_ଚାବି_t *key,
			      ଶୂନ୍ୟ               *data,
			      hb_destroy_func_t   destroy,
			      hb_bool_t           replace)
{
  ଫେରନ୍ତୁ hb_object_set_user_data (draw, key, data, destroy, replace);
}

/**
 * hb_raster_draw_get_user_data: (skip)
 * @draw: a rasterizer
 * @key: the user-data key
 *
 * Fetches the user-data associated with the specified key,
 * attached to the specified rasterizer.
 *
 * Return value: (transfer none):
 * A pointer to the user data
 *
 * Since: 13.0.0
 **/
ଶୂନ୍ୟ *
hb_raster_draw_get_user_data (ଅଚଳ hb_raster_draw_t   *draw,
			      hb_user_data_ଚାବି_t *key)
{
  ଫେରନ୍ତୁ hb_object_get_user_data (draw, key);
}

/**
 * hb_raster_draw_set_transform:
 * @draw: a rasterizer
 * @xx: xx component of the transform matrix
 * @yx: yx component of the transform matrix
 * @xy: xy component of the transform matrix
 * @yy: yy component of the transform matrix
 * @dx: x translation
 * @dy: y translation
 *
 * Sets a 2×3 affine transform applied to all incoming draw
 * coordinates before rasterization.  The default is the identity.
 *
 * Since: 13.0.0
 **/
ଶୂନ୍ୟ
hb_raster_draw_set_transform (hb_raster_draw_t *draw,
			      ଦଶମିକ xx, ଦଶମିକ yx,
			      ଦଶମିକ xy, ଦଶମିକ yy,
			      ଦଶମିକ dx, ଦଶମିକ dy)
{
  draw->transform = {xx, yx, xy, yy, dx, dy};
}

/**
 * hb_raster_draw_set_scale_factor:
 * @draw: a rasterizer
 * @x_scale_factor: x-axis minification factor
 * @y_scale_factor: y-axis minification factor
 *
 * Sets post-transform minification factors applied during rasterization.
 * Factors larger than 1 shrink the output in pixels. The default is 1.
 *
 * Since: 13.0.0
 **/
ଶୂନ୍ୟ
hb_raster_draw_set_scale_factor (hb_raster_draw_t *draw,
				 ଦଶମିକ x_scale_factor,
				 ଦଶମିକ y_scale_factor)
{
  draw->x_scale_factor = x_scale_factor > 0.f ? x_scale_factor : 1.f;
  draw->y_scale_factor = y_scale_factor > 0.f ? y_scale_factor : 1.f;
}

/**
 * hb_raster_draw_get_scale_factor:
 * @draw: a rasterizer
 * @x_scale_factor: (out) (nullable): x-axis minification factor
 * @y_scale_factor: (out) (nullable): y-axis minification factor
 *
 * Fetches the current post-transform minification factors.
 *
 * Since: 13.0.0
 **/
ଶୂନ୍ୟ
hb_raster_draw_get_scale_factor (ଅଚଳ hb_raster_draw_t *draw,
				 ଦଶମିକ *x_scale_factor,
				 ଦଶମିକ *y_scale_factor)
{
  ଯଦି (x_scale_factor) *x_scale_factor = draw->x_scale_factor;
  ଯଦି (y_scale_factor) *y_scale_factor = draw->y_scale_factor;
}

/**
 * hb_raster_draw_get_transform:
 * @draw: a rasterizer
 * @xx: (out) (nullable): xx component of the transform matrix
 * @yx: (out) (nullable): yx component of the transform matrix
 * @xy: (out) (nullable): xy component of the transform matrix
 * @yy: (out) (nullable): yy component of the transform matrix
 * @dx: (out) (nullable): x translation
 * @dy: (out) (nullable): y translation
 *
 * Fetches the current affine transform of the rasterizer.
 *
 * Since: 13.0.0
 **/
ଶୂନ୍ୟ
hb_raster_draw_get_transform (ଅଚଳ hb_raster_draw_t *draw,
			      ଦଶମିକ *xx, ଦଶମିକ *yx,
			      ଦଶମିକ *xy, ଦଶମିକ *yy,
			      ଦଶମିକ *dx, ଦଶମିକ *dy)
{
  ଯଦି (xx) *xx = draw->transform.xx;
  ଯଦି (yx) *yx = draw->transform.yx;
  ଯଦି (xy) *xy = draw->transform.xy;
  ଯଦି (yy) *yy = draw->transform.yy;
  ଯଦି (dx) *dx = draw->transform.x0;
  ଯଦି (dy) *dy = draw->transform.y0;
}

/* Recompute the resolved flattening clip box: the internal clip box
   when set, otherwise the fixed output extents when known.  Expanded
   by one pixel so fixed-point rounding at the boundary stays safe.
   Called whenever the clip box or extents change.  A curve whose
   control-point bounding box misses the clip box entirely is replaced
   by its chord: coverage inside the box then depends only on the
   curve's endpoints (intermediate scanline crossings cancel in pairs,
   and crossings right of the box never reach visible pixels), so the
   replacement is exact for rendering. */
ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_draw_update_flatten_clip (hb_raster_draw_t *draw)
{
  ଯଦି (draw->has_clip_box)
  {
    draw->flatten_clip_active = ସତ୍ୟ;
    draw->flatten_clip_x0 = draw->clip_x0 - 1.f;
    draw->flatten_clip_y0 = draw->clip_y0 - 1.f;
    draw->flatten_clip_x1 = draw->clip_x1 + 1.f;
    draw->flatten_clip_y1 = draw->clip_y1 + 1.f;
  }
  ଅନ୍ୟଥା ଯଦି (draw->has_extents)
  {
    draw->flatten_clip_active = ସତ୍ୟ;
    draw->flatten_clip_x0 = (ଦଶମିକ) draw->fixed_extents.x_origin - 1.f;
    draw->flatten_clip_y0 = (ଦଶମିକ) draw->fixed_extents.y_origin - 1.f;
    draw->flatten_clip_x1 = (ଦଶମିକ) draw->fixed_extents.x_origin + (ଦଶମିକ) draw->fixed_extents.width + 1.f;
    draw->flatten_clip_y1 = (ଦଶମିକ) draw->fixed_extents.y_origin + (ଦଶମିକ) draw->fixed_extents.height + 1.f;
  }
  ଅନ୍ୟଥା
    draw->flatten_clip_active = ମିଥ୍ୟା;
}

/**
 * hb_raster_draw_set_extents:
 * @draw: a rasterizer
 * @extents: the desired output extents
 *
 * Overrides the output image extents for the next render.  When set,
 * hb_raster_draw_render() uses the given extents instead of
 * auto-computing them from the accumulated geometry.
 *
 * Since: 13.0.0
 **/
ଶୂନ୍ୟ
hb_raster_draw_set_extents (hb_raster_draw_t          *draw,
			    ଅଚଳ hb_raster_extents_t *extents)
{
  draw->fixed_extents     = *extents;
  draw->has_extents = ସତ୍ୟ;
  hb_raster_draw_update_flatten_clip (draw);
}

/**
 * hb_raster_draw_get_extents:
 * @draw: a rasterizer
 * @extents: (out) (nullable): where to write current extents
 *
 * Gets currently configured output extents.
 *
 * Return value: `true` if extents are set, `false` otherwise.
 *
 * Since: 13.0.0
 **/
hb_bool_t
hb_raster_draw_get_extents (ଅଚଳ hb_raster_draw_t    *draw,
			    hb_raster_extents_t *extents)
{
  ଯଦି (!draw->has_extents)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (extents)
    *extents = draw->fixed_extents;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

/**
 * hb_raster_draw_set_glyph_extents:
 * @draw: a rasterizer
 * @glyph_extents: glyph extents from hb_font_get_glyph_extents()
 *
 * Transforms @glyph_extents with the rasterizer's current transform and
 * sets the resulting pixel extents for the next render.
 *
 * This is equivalent to computing a transformed bounding box in pixel
 * space and calling hb_raster_draw_set_extents().
 *
 * The resulting dimensions are capped at 4096 pixels per side.
 *
 * Return value: `true` if transformed extents are non-empty and set;
 * `false` otherwise.
 *
 * Since: 13.0.0
 **/
hb_bool_t
hb_raster_draw_set_glyph_extents (hb_raster_draw_t         *draw,
				  ଅଚଳ hb_glyph_extents_t *glyph_extents)
{
  ଦଶମିକ x0 = (ଦଶମିକ) glyph_extents->x_bearing;
  ଦଶମିକ y0 = (ଦଶମିକ) glyph_extents->y_bearing;
  ଦଶମିକ x1 = (ଦଶମିକ) glyph_extents->x_bearing + glyph_extents->width;
  ଦଶମିକ y1 = (ଦଶମିକ) glyph_extents->y_bearing + glyph_extents->height;

  ଦଶମିକ xmin = hb_min (x0, x1);
  ଦଶମିକ xmax = hb_max (x0, x1);
  ଦଶମିକ ymin = hb_min (y0, y1);
  ଦଶମିକ ymax = hb_max (y0, y1);

  ଦଶମିକ px[4] = {xmin, xmin, xmax, xmax};
  ଦଶମିକ py[4] = {ymin, ymax, ymin, ymax};

  ଦଶମିକ tx, ty;
  hb_raster_draw_transform_point (draw, px[0], py[0], tx, ty);
  ଦଶମିକ tx_min = tx, tx_max = tx;
  ଦଶମିକ ty_min = ty, ty_max = ty;

  ପାଇଁ (unsigned i = 1; i < 4; i++)
  {
    hb_raster_draw_transform_point (draw, px[i], py[i], tx, ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  int32_t ex0 = hb_clamp_to<int32_t> (floorf (tx_min));
  int32_t ey0 = hb_clamp_to<int32_t> (floorf (ty_min));
  int32_t ex1 = hb_clamp_to<int32_t> (ceilf  (tx_max));
  int32_t ey1 = hb_clamp_to<int32_t> (ceilf  (ty_max));

  ଯଦି (ex1 <= ex0 || ey1 <= ey0)
  {
    draw->fixed_extents = {};
    draw->has_extents = ମିଥ୍ୟା;
    hb_raster_draw_update_flatten_clip (draw);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  draw->fixed_extents = {
    ex0, ey0,
    (unsigned) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (unsigned) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  draw->has_extents = ସତ୍ୟ;
  hb_raster_draw_update_flatten_clip (draw);
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

/**
 * hb_raster_draw_clear:
 * @draw: a rasterizer
 *
 * Discards accumulated geometry and extents so @draw can be reused
 * for another render.  User configuration (transform, scale factors)
 * is preserved.  Call hb_raster_draw_reset() to also reset user
 * configuration to defaults.
 *
 * Since: 14.2.0
 **/
ଶୂନ୍ୟ
hb_raster_draw_clear (hb_raster_draw_t *draw)
{
  draw->fixed_extents     = {};
  draw->has_extents = ମିଥ୍ୟା;
  draw->has_clip_box = ମିଥ୍ୟା;
  draw->flatten_clip_active = ମିଥ୍ୟା;
  draw->recharge_budget ();
  draw->edges_ବାମ = HB_RASTER_MAX_DRAW_EDGES;
  draw->edges.clear ();
  draw->active_edges.clear ();
}

/**
 * hb_raster_draw_reset:
 * @draw: a rasterizer
 *
 * Resets the rasterizer to its initial state, clearing all accumulated
 * geometry, the transform, and fixed extents.  The object can then be
 * reused for a new glyph.
 *
 * Since: 13.0.0
 **/
ଶୂନ୍ୟ
hb_raster_draw_reset (hb_raster_draw_t *draw)
{
  draw->transform         = {1, 0, 0, 1, 0, 0};
  draw->x_scale_factor    = 1.f;
  draw->y_scale_factor    = 1.f;
  draw->budget            = HB_BUDGET_DEFAULT;
  hb_raster_draw_clear (draw);
}

ଶୂନ୍ୟ
hb_raster_draw_set_clip_box (hb_raster_draw_t *draw,
			     ଦଶମିକ x0, ଦଶମିକ y0,
			     ଦଶମିକ x1, ଦଶମିକ y1)
{
  draw->has_clip_box = ସତ୍ୟ;
  draw->clip_x0 = x0;
  draw->clip_y0 = y0;
  draw->clip_x1 = x1;
  draw->clip_y1 = y1;
  hb_raster_draw_update_flatten_clip (draw);
}

int64_t
hb_raster_draw_get_pixel_work (ଅଚଳ hb_raster_draw_t *draw,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_rows,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_cols)
{
  int64_t work = 0;
  ପାଇଁ (ଅଚଳ auto &edge : draw->edges)
  {
    int64_t dx = (int64_t) edge.xH - edge.xL;
    ଯଦି (dx < 0) dx = -dx;
    work += 1 + hb_min (((int64_t) edge.yH - edge.yL) >> HB_RASTER_PIXEL_BITS,
			(int64_t) max_rows)
	      + hb_min (dx >> HB_RASTER_PIXEL_BITS,
			(int64_t) max_cols);
  }
  ଫେରନ୍ତୁ work;
}

/**
 * hb_raster_draw_recycle_image:
 * @draw: a rasterizer
 * @image: a raster image to recycle
 *
 * Recycles @image for reuse by a subsequent hb_raster_draw_render()
 * call, avoiding per-render memory allocation.  The caller transfers
 * ownership of @image to @draw and must not use it afterwards.
 *
 * If @draw already holds a recycled image, the previously recycled
 * image is destroyed.
 *
 * Since: 13.0.0
 **/
ଶୂନ୍ୟ
hb_raster_draw_recycle_image (hb_raster_draw_t  *draw,
			      hb_raster_image_t *image)
{
  hb_raster_image_destroy (draw->recycled_image);
  draw->recycled_image = image;
}


/*
 * Draw callbacks — flatten on the fly into hb_raster_edge_t
 */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
transform_point (ଅଚଳ hb_raster_draw_t *draw,
		 ଦଶମିକ  x,  ଦଶମିକ  y,
		 ଦଶମିକ &tx, ଦଶମିକ &ty)
{
  hb_raster_draw_transform_point (draw, x, y, tx, ty);
}

ସ୍ଥିର ଶୂନ୍ୟ
emit_segment (hb_raster_draw_t *draw,
	      ଦଶମିକ x0, ଦଶମିକ y0,
	      ଦଶମିକ x1, ଦଶମିକ y1)
{
  ଯଦି (unlikely (draw->edges_ବାମ <= 0))
    ଫେରନ୍ତୁ;

  int32_t X0 = hb_clamp_to<int32_t> (roundf (x0 * HB_RASTER_ONE_PIXEL));
  int32_t Y0 = hb_clamp_to<int32_t> (roundf (y0 * HB_RASTER_ONE_PIXEL));
  int32_t X1 = hb_clamp_to<int32_t> (roundf (x1 * HB_RASTER_ONE_PIXEL));
  int32_t Y1 = hb_clamp_to<int32_t> (roundf (y1 * HB_RASTER_ONE_PIXEL));

  ଯଦି (Y0 == Y1) ଫେରନ୍ତୁ; /* horizontal — skip */

  hb_raster_edge_t e;
  ଯଦି (Y0 < Y1) {
    e.xL = X0; e.yL = Y0; e.xH = X1; e.yH = Y1; e.wind = +1;
  } ଅନ୍ୟଥା {
    e.xL = X1; e.yL = Y1; e.xH = X0; e.yH = Y0; e.wind = -1;
  }
  e.slope = (((int64_t) e.xH - (int64_t) e.xL) * (int64_t) 65536) /
	    ((int64_t) e.yH - (int64_t) e.yL);

  ଯଦି (likely (draw->edges.ଠେଲ_or_fail (e)))
    draw->edges_ବାମ--;
  ଅନ୍ୟଥା
    draw->edges_ବାମ = 0;
}


/* Quadratic Bézier flattener — iterative de Casteljau at t=0.5. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
flatten_quadratic_recursive (hb_raster_draw_t *draw,
			     ଦଶମିକ x0, ଦଶମିକ y0,
			     ଦଶମିକ x1, ଦଶମିକ y1,
			     ଦଶମିକ x2, ଦଶମିକ y2,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = 0)
{
  ସଂରଚନା quad_ଗଣ୍ଠି_t
  {
    ଦଶମିକ x0, y0, x1, y1, x2, y2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;
    ତାର୍କିକ check_clip;
  };

  quad_ଗଣ୍ଠି_t ଗଦା[16];
  unsigned ଶୀର୍ଷ_2 = 0;

  ତାର୍କିକ check_clip = draw->flatten_clip_active;
  int64_t *work = draw->get_budget_remaining ();
  int64_t work_ବାମ = *work;

  ଯେତେବେଳେ (ସତ୍ୟ)
  {
    ତାର୍କିକ emit_chord = depth >= 16;

    /* Entirely outside the clip box: the chord is exact for coverage.
       Subdivided control points are convex combinations of their
       parent's, so once a node is entirely inside the box no
       descendant can leave it and the test is skipped below. */
    ଯଦି (!emit_chord && check_clip)
    {
      ଦଶମିକ bx0 = hb_min (x0, hb_min (x1, x2));
      ଦଶମିକ bx1 = hb_max (x0, hb_max (x1, x2));
      ଦଶମିକ by0 = hb_min (y0, hb_min (y1, y2));
      ଦଶମିକ by1 = hb_max (y0, hb_max (y1, y2));
      ଯଦି (bx1 < draw->flatten_clip_x0 || bx0 > draw->flatten_clip_x1 ||
	  by1 < draw->flatten_clip_y0 || by0 > draw->flatten_clip_y1)
	emit_chord = ସତ୍ୟ;
      ଅନ୍ୟଥା ଯଦି (bx0 >= draw->flatten_clip_x0 && bx1 <= draw->flatten_clip_x1 &&
	       by0 >= draw->flatten_clip_y0 && by1 <= draw->flatten_clip_y1)
	check_clip = ମିଥ୍ୟା;
    }

    ଯଦି (!emit_chord)
    {
      ତାର୍କିକ is_flat;
      ଯଦି (ମିଥ୍ୟା)
      {
	/* Old behavior: midpoint deviation from chord midpoint. */
	ଦଶମିକ mx = x0 * 0.25f + x1 * 0.5f + x2 * 0.25f;
	ଦଶମିକ my = y0 * 0.25f + y1 * 0.5f + y2 * 0.25f;
	ଦଶମିକ chord_mx = (x0 + x2) * 0.5f;
	ଦଶମିକ chord_my = (y0 + y2) * 0.5f;
	ଦଶମିକ dx = mx - chord_mx;
	ଦଶମିକ dy = my - chord_my;
	ସ୍ଥିର ଅଚଳ ଦଶମିକ flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;
	is_flat = (dx * dx + dy * dy) <= flat_thresh;
      }
      ଅନ୍ୟଥା
      {
	/* FreeType behavior: control-point deviation from chord center. */
	ଅଚଳ ଦଶମିକ flat_thresh = 0.25f;
	ଦଶମିକ dx = x0 + x2 - 2.f * x1;
	ଦଶମିକ dy = y0 + y2 - 2.f * y1;
	ଯଦି (dx < 0) dx = -dx;
	ଯଦି (dy < 0) dy = -dy;
	is_flat = dx <= flat_thresh && dy <= flat_thresh;
      }
      emit_chord = is_flat;
    }

    /* Charge one unit per subdivision; degrade to the chord when the
       session work budget is spent. */
    ଯଦି (!emit_chord)
    {
      ଯଦି (unlikely (work_ବାମ <= 0))
	emit_chord = ସତ୍ୟ;
      ଅନ୍ୟଥା
	work_ବାମ--;
    }

    ଯଦି (emit_chord)
    {
      emit_segment (draw, x0, y0, x2, y2);
      ଯଦି (!ଶୀର୍ଷ_2)
      {
	*work = work_ବାମ;
	ଫେରନ୍ତୁ;
      }
      ଅଚଳ quad_ଗଣ୍ଠି_t &n = ଗଦା[--ଶୀର୍ଷ_2];
      x0 = n.x0; y0 = n.y0;
      x1 = n.x1; y1 = n.y1;
      x2 = n.x2; y2 = n.y2;
      depth = n.depth;
      check_clip = n.check_clip;
      ଜାରି;
    }

    ଦଶମିକ x01 = (x0 + x1) * 0.5f, y01 = (y0 + y1) * 0.5f;
    ଦଶମିକ x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
    ଦଶମିକ xm  = (x01 + x12) * 0.5f, ym  = (y01 + y12) * 0.5f;

    /* Depth is capped at 16, so stack capacity 16 is sufficient. */
    ଗଦା[ଶୀର୍ଷ_2++] = {xm, ym, x12, y12, x2, y2, depth + 1, check_clip};
    x2 = xm; y2 = ym;
    x1 = x01; y1 = y01;
    depth++;
  }
}

/* Quadratic Bézier flattener using forward differencing.
   The error (midpoint deviation) shrinks exactly 4× per de Casteljau
   subdivision, so we compute the subdivision count upfront and iterate
   with constant-cost additions instead of recursive branching. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
flatten_quadratic_fd (hb_raster_draw_t *draw,
		      ଦଶମିକ x0, ଦଶମିକ y0,
		      ଦଶମିକ x1, ଦଶମିକ y1,
		      ଦଶମିକ x2, ଦଶମିକ y2)
{
  /* Deviation of curve midpoint from chord midpoint (squared). */
  ଦଶମିକ devx = (x0 - 2 * x1 + x2) * 0.25f;
  ଦଶମିକ devy = (y0 - 2 * y1 + y2) * 0.25f;
  ଦଶମିକ err2 = devx * devx + devy * devy;

  ସ୍ଥିର ଅଚଳ ଦଶମିକ flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;

  ଯଦି (err2 <= flat_thresh)
  {
    emit_segment (draw, x0, y0, x2, y2);
    ଫେରନ୍ତୁ;
  }

  /* err² shrinks 16× per subdivision level.  Find n such that
     err2 / 16^n <= flat_thresh, i.e. n = ceil(log₁₆(err2/flat_thresh)). */
  unsigned n = 1;
  {
    ଦଶମିକ ratio = err2 / flat_thresh;
    ଯେତେବେଳେ (ratio > 16.f) { ratio *= (1.f / 16.f); n++; }
    ଯଦି (n > 16) n = 16;
  }
  unsigned N = 1u << n;   /* number of line segments */
  ଦଶମିକ h = 1.f / N;

  /* Quadratic: B(t) = a·t² + b·t + c
     Forward differences with step h:
       d²f = 2·a·h²   (constant)
       df₀ = a·h² + b·h
       f₀  = c = P₀                     */
  ଦଶମିକ ax = x0 - 2 * x1 + x2;
  ଦଶମିକ ay = y0 - 2 * y1 + y2;
  ଦଶମିକ bx = 2 * (x1 - x0);
  ଦଶମିକ by = 2 * (y1 - y0);

  ଦଶମିକ d2fx = 2 * ax * h * h;
  ଦଶମିକ d2fy = 2 * ay * h * h;
  ଦଶମିକ dfx  = ax * h * h + bx * h;
  ଦଶମିକ dfy  = ay * h * h + by * h;
  ଦଶମିକ fx   = x0;
  ଦଶମିକ fy   = y0;

  ପାଇଁ (unsigned i = 1; i < N; i++)
  {
    ଦଶମିକ nx = fx + dfx;
    ଦଶମିକ ny = fy + dfy;
    emit_segment (draw, fx, fy, nx, ny);
    fx = nx;
    fy = ny;
    dfx += d2fx;
    dfy += d2fy;
  }
  /* Last segment uses exact endpoint to avoid drift. */
  emit_segment (draw, fx, fy, x2, y2);
}

ସ୍ଥିର ଶୂନ୍ୟ
flatten_quadratic (hb_raster_draw_t *draw,
		   ଦଶମିକ x0, ଦଶମିକ y0,
		   ଦଶମିକ x1, ଦଶମିକ y1,
		   ଦଶମିକ x2, ଦଶମିକ y2)
{
  ଯଦି (ମିଥ୍ୟା)
    flatten_quadratic_fd (draw, x0, y0, x1, y1, x2, y2);
  ଅନ୍ୟଥା
    flatten_quadratic_recursive (draw, x0, y0, x1, y1, x2, y2);
}

/* For cubic B(t), the max deviation from its chord on [0,1] is bounded by:
     max||B(t)-L(t)|| <= max_t ||B''(t)|| / 8.
   B''(t) is linear, so max norm is attained at t=0 or t=1:
     B''(0)=6*(P0-2P1+P2), B''(1)=6*(P1-2P2+P3). */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ
cubic_chord_error_bound2 (ଦଶମିକ x0, ଦଶମିକ y0,
			  ଦଶମିକ x1, ଦଶମିକ y1,
			  ଦଶମିକ x2, ଦଶମିକ y2,
			  ଦଶମିକ x3, ଦଶମିକ y3)
{
  ଦଶମିକ d20x = x0 - 2 * x1 + x2;
  ଦଶମିକ d20y = y0 - 2 * y1 + y2;
  ଦଶମିକ d21x = x1 - 2 * x2 + x3;
  ଦଶମିକ d21y = y1 - 2 * y2 + y3;
  ଦଶମିକ m0 = d20x * d20x + d20y * d20y;
  ଦଶମିକ m1 = d21x * d21x + d21y * d21y;
  ଦଶମିକ m = m0 > m1 ? m0 : m1;
  /* (max||B''||/8)^2 = (6/8)^2 * max||d2||^2 = (3/4)^2 * m. */
  ଫେରନ୍ତୁ m * (9.f / 16.f);
}

/* Cubic Bézier flattener — iterative de Casteljau at t=0.5. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
flatten_cubic_recursive (hb_raster_draw_t *draw,
			 ଦଶମିକ x0, ଦଶମିକ y0,
			 ଦଶମିକ x1, ଦଶମିକ y1,
			 ଦଶମିକ x2, ଦଶମିକ y2,
			 ଦଶମିକ x3, ଦଶମିକ y3,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = 0)
{
  ସଂରଚନା cubic_ଗଣ୍ଠି_t
  {
    ଦଶମିକ x0, y0, x1, y1, x2, y2, x3, y3;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;
    ତାର୍କିକ check_clip;
  };

  cubic_ଗଣ୍ଠି_t ଗଦା[16];
  unsigned ଶୀର୍ଷ_2 = 0;

  ତାର୍କିକ check_clip = draw->flatten_clip_active;
  int64_t *work = draw->get_budget_remaining ();
  int64_t work_ବାମ = *work;

  ଯେତେବେଳେ (ସତ୍ୟ)
  {
    ତାର୍କିକ emit_chord = depth >= 16;

    /* Entirely outside the clip box: the chord is exact for coverage.
       Subdivided control points are convex combinations of their
       parent's, so once a node is entirely inside the box no
       descendant can leave it and the test is skipped below. */
    ଯଦି (!emit_chord && check_clip)
    {
      ଦଶମିକ bx0 = hb_min (hb_min (x0, x1), hb_min (x2, x3));
      ଦଶମିକ bx1 = hb_max (hb_max (x0, x1), hb_max (x2, x3));
      ଦଶମିକ by0 = hb_min (hb_min (y0, y1), hb_min (y2, y3));
      ଦଶମିକ by1 = hb_max (hb_max (y0, y1), hb_max (y2, y3));
      ଯଦି (bx1 < draw->flatten_clip_x0 || bx0 > draw->flatten_clip_x1 ||
	  by1 < draw->flatten_clip_y0 || by0 > draw->flatten_clip_y1)
	emit_chord = ସତ୍ୟ;
      ଅନ୍ୟଥା ଯଦି (bx0 >= draw->flatten_clip_x0 && bx1 <= draw->flatten_clip_x1 &&
	       by0 >= draw->flatten_clip_y0 && by1 <= draw->flatten_clip_y1)
	check_clip = ମିଥ୍ୟା;
    }

    ଯଦି (!emit_chord)
    {
      ତାର୍କିକ is_flat;
      ଯଦି (ମିଥ୍ୟା)
      {
	/* Old behavior: curvature/chord-error bound. */
	ଦଶମିକ err2 = cubic_chord_error_bound2 (x0, y0, x1, y1, x2, y2, x3, y3);
	ସ୍ଥିର ଅଚଳ ଦଶମିକ flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;
	is_flat = err2 <= flat_thresh;
      }
      ଅନ୍ୟଥା
      {
	/* FreeType behavior: chord-trisection distance test. */
	ଅଚଳ ଦଶମିକ flat_thresh = 0.5f;

	ଦଶମିକ d10x = 2.f * x0 - 3.f * x1 + x3;
	ଦଶମିକ d10y = 2.f * y0 - 3.f * y1 + y3;
	ଦଶମିକ d20x = x0 - 3.f * x2 + 2.f * x3;
	ଦଶମିକ d20y = y0 - 3.f * y2 + 2.f * y3;

	ଯଦି (d10x < 0) d10x = -d10x;
	ଯଦି (d10y < 0) d10y = -d10y;
	ଯଦି (d20x < 0) d20x = -d20x;
	ଯଦି (d20y < 0) d20y = -d20y;

	is_flat = d10x <= flat_thresh &&
		  d10y <= flat_thresh &&
		  d20x <= flat_thresh &&
		  d20y <= flat_thresh;
      }
      emit_chord = is_flat;
    }

    /* Charge one unit per subdivision; degrade to the chord when the
       session work budget is spent. */
    ଯଦି (!emit_chord)
    {
      ଯଦି (unlikely (work_ବାମ <= 0))
	emit_chord = ସତ୍ୟ;
      ଅନ୍ୟଥା
	work_ବାମ--;
    }

    ଯଦି (emit_chord)
    {
      emit_segment (draw, x0, y0, x3, y3);
      ଯଦି (!ଶୀର୍ଷ_2)
      {
	*work = work_ବାମ;
	ଫେରନ୍ତୁ;
      }
      ଅଚଳ cubic_ଗଣ୍ଠି_t &n = ଗଦା[--ଶୀର୍ଷ_2];
      x0 = n.x0; y0 = n.y0;
      x1 = n.x1; y1 = n.y1;
      x2 = n.x2; y2 = n.y2;
      x3 = n.x3; y3 = n.y3;
      depth = n.depth;
      check_clip = n.check_clip;
      ଜାରି;
    }

    ଦଶମିକ x01  = (x0 + x1) * 0.5f, y01  = (y0 + y1) * 0.5f;
    ଦଶମିକ x12  = (x1 + x2) * 0.5f, y12  = (y1 + y2) * 0.5f;
    ଦଶମିକ x23  = (x2 + x3) * 0.5f, y23  = (y2 + y3) * 0.5f;
    ଦଶମିକ x012 = (x01 + x12) * 0.5f, y012 = (y01 + y12) * 0.5f;
    ଦଶମିକ x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
    ଦଶମିକ xm   = (x012 + x123) * 0.5f, ym   = (y012 + y123) * 0.5f;

    /* Depth is capped at 16, so stack capacity 16 is sufficient. */
    ଗଦା[ଶୀର୍ଷ_2++] = {xm, ym, x123, y123, x23, y23, x3, y3, depth + 1, check_clip};
    x3 = xm; y3 = ym;
    x2 = x012; y2 = y012;
    x1 = x01; y1 = y01;
    depth++;
  }
}

/* Cubic Bézier flattener using forward differencing.
   Use a curvature-based chord-error bound (max||B''||/8), then choose a
   uniform subdivision count n such that the bound drops below threshold.
   The cubic adds a constant third difference d³f = 6·a·h³. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
flatten_cubic_fd (hb_raster_draw_t *draw,
		  ଦଶମିକ x0, ଦଶମିକ y0,
		  ଦଶମିକ x1, ଦଶମିକ y1,
		  ଦଶମିକ x2, ଦଶମିକ y2,
		  ଦଶମିକ x3, ଦଶମିକ y3)
{
  ଦଶମିକ err2 = cubic_chord_error_bound2 (x0, y0, x1, y1, x2, y2, x3, y3);

  ସ୍ଥିର ଅଚଳ ଦଶମିକ flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;

  ଯଦି (err2 <= flat_thresh)
  {
    emit_segment (draw, x0, y0, x3, y3);
    ଫେରନ୍ତୁ;
  }

  /* The bound scales with h², so err² shrinks 16× per subdivision level. */
  unsigned n = 1;
  {
    ଦଶମିକ ratio = err2 / flat_thresh;
    ଯେତେବେଳେ (ratio > 16.f) { ratio *= (1.f / 16.f); n++; }
    ଯଦି (n > 16) n = 16;
  }
  unsigned N = 1u << n;
  ଦଶମିକ h = 1.f / N;

  /* Cubic: B(t) = a·t³ + b·t² + c·t + d
     a = -P₀ + 3P₁ - 3P₂ + P₃
     b =  3P₀ - 6P₁ + 3P₂
     c =  3(P₁ - P₀)
     d =  P₀
     Forward differences with step h:
       d³f  = 6·a·h³              (constant)
       d²f₀ = 6·a·h³ + 2·b·h²
       d¹f₀ = a·h³ + b·h² + c·h
       f₀   = d = P₀              */
  ଦଶମିକ ax = -x0 + 3*x1 - 3*x2 + x3;
  ଦଶମିକ ay = -y0 + 3*y1 - 3*y2 + y3;
  ଦଶମିକ bx =  3*x0 - 6*x1 + 3*x2;
  ଦଶମିକ by =  3*y0 - 6*y1 + 3*y2;
  ଦଶମିକ cx =  3*(x1 - x0);
  ଦଶମିକ cy =  3*(y1 - y0);

  ଦଶମିକ h2 = h * h, h3 = h2 * h;
  ଦଶମିକ d3fx = 6 * ax * h3;
  ଦଶମିକ d3fy = 6 * ay * h3;
  ଦଶମିକ d2fx = d3fx + 2 * bx * h2;
  ଦଶମିକ d2fy = d3fy + 2 * by * h2;
  ଦଶମିକ dfx  = ax * h3 + bx * h2 + cx * h;
  ଦଶମିକ dfy  = ay * h3 + by * h2 + cy * h;
  ଦଶମିକ fx   = x0;
  ଦଶମିକ fy   = y0;

  ପାଇଁ (unsigned i = 1; i < N; i++)
  {
    ଦଶମିକ nx = fx + dfx;
    ଦଶମିକ ny = fy + dfy;
    emit_segment (draw, fx, fy, nx, ny);
    fx = nx;  fy = ny;
    dfx += d2fx;  dfy += d2fy;
    d2fx += d3fx; d2fy += d3fy;
  }
  /* Last segment uses exact endpoint to avoid drift. */
  emit_segment (draw, fx, fy, x3, y3);
}

ସ୍ଥିର ଶୂନ୍ୟ
flatten_cubic (hb_raster_draw_t *draw,
	       ଦଶମିକ x0, ଦଶମିକ y0,
	       ଦଶମିକ x1, ଦଶମିକ y1,
	       ଦଶମିକ x2, ଦଶମିକ y2,
	       ଦଶମିକ x3, ଦଶମିକ y3)
{
  ଯଦି (ମିଥ୍ୟା)
    flatten_cubic_fd (draw, x0, y0, x1, y1, x2, y2, x3, y3);
  ଅନ୍ୟଥା
    flatten_cubic_recursive (draw, x0, y0, x1, y1, x2, y2, x3, y3);
}


/* Draw callback implementations */

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		   ଶୂନ୍ୟ *draw_data,
		   hb_draw_state_t *st HB_UNUSED,
		   ଦଶମିକ to_x HB_UNUSED, ଦଶମିକ to_y HB_UNUSED,
		   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  /* no-op: state tracked by hb_draw_state_t */
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_ଧାଡ଼ି_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		   ଶୂନ୍ୟ *draw_data,
		   hb_draw_state_t *st,
		   ଦଶମିକ to_x, ଦଶମିକ to_y,
		   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_data;
  ଯଦି (unlikely (draw->edges_ବାମ <= 0)) ଫେରନ୍ତୁ;

  ଦଶମିକ tx0, ty0, tx1, ty1;
  transform_point (draw, st->current_x, st->current_y, tx0, ty0);
  transform_point (draw, to_x,          to_y,           tx1, ty1);
  emit_segment (draw, tx0, ty0, tx1, ty1);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			ଶୂନ୍ୟ *draw_data,
			hb_draw_state_t *st,
			ଦଶମିକ control_x, ଦଶମିକ control_y,
			ଦଶମିକ to_x, ଦଶମିକ to_y,
			ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_data;
  ଯଦି (unlikely (draw->edges_ବାମ <= 0)) ଫେରନ୍ତୁ;

  ଦଶମିକ tx0, ty0, tx1, ty1, tx2, ty2;
  transform_point (draw, st->current_x, st->current_y, tx0, ty0);
  transform_point (draw, control_x,     control_y,      tx1, ty1);
  transform_point (draw, to_x,          to_y,           tx2, ty2);
  flatten_quadratic (draw, tx0, ty0, tx1, ty1, tx2, ty2);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		    ଶୂନ୍ୟ *draw_data,
		    hb_draw_state_t *st,
		    ଦଶମିକ control1_x, ଦଶମିକ control1_y,
		    ଦଶମିକ control2_x, ଦଶମିକ control2_y,
		    ଦଶମିକ to_x, ଦଶମିକ to_y,
		    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_data;
  ଯଦି (unlikely (draw->edges_ବାମ <= 0)) ଫେରନ୍ତୁ;

  ଦଶମିକ tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3;
  transform_point (draw, st->current_x, st->current_y, tx0, ty0);
  transform_point (draw, control1_x,    control1_y,     tx1, ty1);
  transform_point (draw, control2_x,    control2_y,     tx2, ty2);
  transform_point (draw, to_x,          to_y,           tx3, ty3);
  flatten_cubic (draw, tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
		      ଶୂନ୍ୟ *draw_data HB_UNUSED,
		      hb_draw_state_t *st HB_UNUSED,
		      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  /* no-op: hb_draw_funcs_t already emits closing line_to before us */
}

ସ୍ଥିର hb_bool_t
hb_raster_draw_set_budget (hb_draw_funcs_t *, ଶୂନ୍ୟ *draw_data,
			   int64_t budget, ଶୂନ୍ୟ *)
{
  auto *draw = (hb_raster_draw_t *) draw_data;
  draw->budget = budget;
  draw->recharge_budget ();
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର int64_t
hb_raster_draw_get_budget (hb_draw_funcs_t *, ଶୂନ୍ୟ *draw_data, ଶୂନ୍ୟ *)
{
  ଫେରନ୍ତୁ ((hb_raster_draw_t *) draw_data)->budget;
}

ସ୍ଥିର int64_t *
hb_raster_draw_get_budget_remaining (hb_draw_funcs_t *, ଶୂନ୍ୟ *draw_data, ଶୂନ୍ୟ *)
{
  ଫେରନ୍ତୁ ((hb_raster_draw_t *) draw_data)->get_budget_remaining ();
}


/* Lazy-loader singleton for draw funcs */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_raster_draw_funcs ();

ସ୍ଥିର ସଂରଚନା hb_raster_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_raster_draw_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func      (funcs, hb_raster_move_to,      nullptr, nullptr);
    hb_draw_funcs_set_ଧାଡ଼ି_to_func      (funcs, hb_raster_ଧାଡ଼ି_to,      nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_raster_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func     (funcs, hb_raster_cubic_to,     nullptr, nullptr);
    hb_draw_funcs_set_close_path_func   (funcs, hb_raster_close_path,   nullptr, nullptr);
    hb_draw_funcs_set_set_budget_func (funcs, hb_raster_draw_set_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_func (funcs, hb_raster_draw_get_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_raster_draw_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_raster_draw_funcs);

    ଫେରନ୍ତୁ funcs;
  }
} static_raster_draw_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
free_static_raster_draw_funcs ()
{
  static_raster_draw_funcs.free_instance ();
}

/**
 * hb_raster_draw_get_funcs:
 * @draw: a rasterizer draw context.
 *
 * Fetches the #hb_draw_funcs_t that feeds outline data into
 * @draw.  Pass @draw as the @draw_data argument when calling
 * the draw functions.
 *
 * Return value: (transfer none):
 * The rasterizer draw functions
 *
 * Since: 14.2.0
 **/
hb_draw_funcs_t *
hb_raster_draw_get_funcs (ଅଚଳ hb_raster_draw_t *draw HB_UNUSED)
{
  ଫେରନ୍ତୁ static_raster_draw_funcs.get_unconst ();
}

/**
 * hb_raster_draw_glyph_or_fail:
 * @draw: a rasterizer
 * @font: font to draw from
 * @glyph: glyph ID to draw
 *
 * Convenience to draw one glyph.  Equivalent to:
 *
 * |[<!-- language="plain" -->
 * hb_font_draw_glyph_or_fail (font, glyph,
 *   hb_raster_draw_get_funcs (draw), draw);
 * ]|
 *
 * Return value: `true` if the glyph was drawn, `false` if the font has
 * no outlines for @glyph.
 *
 * Since: 14.2.0
 **/
hb_bool_t
hb_raster_draw_glyph_or_fail (hb_raster_draw_t *draw,
			      hb_font_t       *font,
			      hb_codepoint_t   glyph)
{
  ଫେରନ୍ତୁ hb_font_draw_glyph_or_fail (font, glyph,
				     hb_raster_draw_get_funcs (draw), draw);
}

/**
 * hb_raster_draw_glyph:
 * @draw: a rasterizer
 * @font: font to draw from
 * @glyph: glyph ID to draw
 *
 * Draws one glyph into @draw using the rasterizer's current
 * transform.  Equivalent to hb_raster_draw_glyph_or_fail() with the
 * return value ignored.
 *
 * Since: 14.2.0
 **/
ଶୂନ୍ୟ
hb_raster_draw_glyph (hb_raster_draw_t *draw,
		      hb_font_t       *font,
		      hb_codepoint_t   glyph)
{
  hb_raster_draw_glyph_or_fail (draw, font, glyph);
}


/*
 * Analytic coverage rasterizer
 *
 * For each line-segment edge and each pixel row it crosses, we compute
 * exact area/cover contributions per pixel cell.  A left-to-right sweep
 * then converts accumulated (area, cover) into alpha values.
 *
 * Coordinates are fixed-point.
 *
 *   cover[x] = Σ dy · wind        — signed vertical extent per cell
 *   area[x]  = Σ (fx₀+fx₁)·dy·wind — twice the signed trapezoidal area
 *
 * Sweep:
 *   cover_accum += cover[x]
 *   α = min(|cover_accum·(2·ONE_PIXEL) − area[x]|, FULL_COVERAGE)
 *       · 255 / FULL_COVERAGE
 */

/* Each edge contributes at most ONE_PIXEL to a cell's cover, so the
 * compact accumulators are exact up to this many active edges. */
ସ୍ଥିର constexpr unsigned HB_RASTER_NARROW_MAX_ACTIVE_EDGES =
  INT16_MAX / HB_RASTER_ONE_PIXEL;

/* Add one edge piece's area/cover into a single cell. */
template <typename Area, typename Cover>
ସ୍ଥିର HB_ALWAYS_INLINE ଶୂନ୍ୟ
cell_add (Area *area, Cover *cover, unsigned width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା col,
	  int32_t fx0, int32_t fy0, int32_t fx1, int32_t fy1, int32_t wind,
	  unsigned &x_min, unsigned &x_max)
{
  ଯଦି (unlikely ((unsigned) col >= width))
  {
    ଯଦି (unlikely (col < 0))
    {
      /* Edge is to the left of the surface.  The winding contribution
       * still carries into the visible region, so add the cover delta
       * to column 0.  Area is not added since the edge doesn't cross
       * column 0's cell. */
      int32_t dy = fy1 - fy0;
      cover[0] += (Cover) (dy * wind);
      x_min = hb_min (x_min, 0u);
      x_max = hb_max (x_max, 0u);
    }
    ଫେରନ୍ତୁ;
  }
  int32_t dy = fy1 - fy0;
  area[col]  += (Area) (fx0 + fx1) * dy * wind;
  cover[col] += (Cover) (dy * wind);
  x_min = hb_min (x_min, (unsigned) col);
  x_max = hb_max (x_max, (unsigned) col);
}

/* Walk one edge through the pixel cells of a single pixel row,
   accumulating area/cover.  py is the integer pixel-row index. */
template <typename Area, typename Cover>
ସ୍ଥିର HB_ALWAYS_INLINE ଶୂନ୍ୟ
edge_sweep_row (Area                   *area,
		Cover                  *cover,
		unsigned                width,
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା                     x_org,
		int32_t                 y_ଶୀର୍ଷ,
		ଅଚଳ hb_raster_edge_t &edge,
		unsigned               &x_min,
		unsigned               &x_max)
{
  /* Saturate: y_top can be within ONE_PIXEL of INT32_MAX when the surface
   * sits at the extreme of the fixed-point coordinate space.  Edges are
   * int32-clamped too, so no coverage exists beyond INT32_MAX anyway. */
  int32_t y_bot = (int32_t) hb_min ((int64_t) y_ଶୀର୍ଷ + HB_RASTER_ONE_PIXEL, (int64_t) INT32_MAX);

  int32_t ey0 = hb_max (edge.yL, y_ଶୀର୍ଷ);
  int32_t ey1 = hb_min (edge.yH, y_bot);
  ଯଦି (ey0 >= ey1) ଫେରନ୍ତୁ;

  /* X at clipped endpoints (fixed-point). Keep the interpolation in 64-bit
   * so extreme y values do not overflow before the slope multiply. */
  int64_t x0_64 = (int64_t) edge.xL +
		  ((((int64_t) ey0 - (int64_t) edge.yL) * edge.slope) >> 16);
  int64_t x1_64 = (int64_t) edge.xL +
		  ((((int64_t) ey1 - (int64_t) edge.yL) * edge.slope) >> 16);
  int32_t x0 = (int32_t) hb_clamp (x0_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);
  int32_t x1 = (int32_t) hb_clamp (x1_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);

  /* Fractional y within this pixel row [0, ONE_PIXEL] */
  int32_t fy0 = ey0 - y_ଶୀର୍ଷ;
  int32_t fy1 = ey1 - y_ଶୀର୍ଷ;

  int32_t cx0 = x0 >> HB_RASTER_PIXEL_BITS;
  int32_t fx0 = x0 & HB_RASTER_PIXEL_MASK;
  int32_t cx1 = x1 >> HB_RASTER_PIXEL_BITS;
  int32_t fx1 = x1 & HB_RASTER_PIXEL_MASK;
  int32_t wind = edge.wind;

  /* Fast path: both endpoints in the same pixel column. */
  ଯଦି (cx0 == cx1)
  {
    cell_add (area, cover, width, cx0 - x_org, fx0, fy0, fx1, fy1, wind, x_min, x_max);
    ଫେରନ୍ତୁ;
  }

  int64_t total_dx = (int64_t) x1 - (int64_t) x0;
  int64_t total_dy = (int64_t) fy1 - (int64_t) fy0;

  /* fy increment per pixel column (constant since x_b advances by ONE_PIXEL). */
  int32_t delta_fy = (int32_t) ((int64_t) HB_RASTER_ONE_PIXEL * total_dy / total_dx);

  ଯଦି (total_dx > 0)
  {
    /* Left-to-right edge. */
    ଯଦି (likely ((unsigned) (cx0 - x_org) < width && (unsigned) (cx1 - x_org) < width))
    {
      /* Entirely inside the surface: unclipped walk. */
      int32_t x_b = (int32_t) hb_clamp (((int64_t) cx0 + 1) * HB_RASTER_ONE_PIXEL,
					(int64_t) INT32_MIN, (int64_t) INT32_MAX);
      int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * total_dy) / total_dx);
      cell_add (area, cover, width, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);

      int32_t fy_prev = fy_b;
      ପାଇଁ (int32_t cx = cx0 + 1; cx < cx1; cx++)
      {
	fy_b = fy_prev + delta_fy;
	cell_add (area, cover, width, cx - x_org, 0, fy_prev, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
	fy_prev = fy_b;
      }

      cell_add (area, cover, width, cx1 - x_org, 0, fy_prev, fx1, fy1, wind, x_min, x_max);
      ଫେରନ୍ତୁ;
    }

    /* Visible column window.  Columns left of it only contribute their
     * total cover to column 0 (see cell_add) and that total telescopes,
     * while columns right of it contribute nothing; so neither side is
     * walked cell by cell.  Skipping ahead in the fy accumulation is
     * exact: the increment is constant and the running sums are bounded,
     * so k steps equal one k·delta_fy jump. */
    int64_t col_min = (int64_t) x_org;
    int64_t col_max = (int64_t) x_org + (int64_t) width - 1;

    ଯଦି (unlikely (cx1 < col_min))
    {
      /* Entirely left of the surface. */
      cell_add (area, cover, width, -1, 0, fy0, 0, fy1, wind, x_min, x_max);
      ଫେରନ୍ତୁ;
    }
    ଯଦି (unlikely (cx0 > col_max))
      ଫେରନ୍ତୁ; /* Entirely right of the surface. */

    int32_t x_b = (int32_t) hb_clamp (((int64_t) cx0 + 1) * HB_RASTER_ONE_PIXEL,
				      (int64_t) INT32_MIN, (int64_t) INT32_MAX);
    int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * total_dy) / total_dx);

    int32_t cx = cx0 + 1;
    int32_t fy_prev = fy_b;
    ଯଦି (likely (cx0 >= col_min))
      cell_add (area, cover, width, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
    ଅନ୍ୟଥା
    {
      /* Fold the first cell and the mid columns left of the surface
       * into one column-0 cover update. */
      int32_t cx_skip = (int32_t) col_min; /* ≤ cx1, so it fits */
      fy_prev = fy_b + (int32_t) ((int64_t) (cx_skip - cx) * delta_fy);
      cell_add (area, cover, width, -1, 0, fy0, 0, fy_prev, wind, x_min, x_max);
      cx = cx_skip;
    }

    int32_t cx_end = (int32_t) hb_min ((int64_t) cx1, col_max + 1);
    ପାଇଁ (; cx < cx_end; cx++)
    {
      fy_b = fy_prev + delta_fy;
      cell_add (area, cover, width, cx - x_org, 0, fy_prev, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
      fy_prev = fy_b;
    }

    ଯଦି (likely (cx1 <= col_max))
      cell_add (area, cover, width, cx1 - x_org, 0, fy_prev, fx1, fy1, wind, x_min, x_max);
  }
  ଅନ୍ୟଥା
  {
    /* Right-to-left edge. */
    ଯଦି (likely ((unsigned) (cx0 - x_org) < width && (unsigned) (cx1 - x_org) < width))
    {
      /* Entirely inside the surface: unclipped walk. */
      int32_t x_b = (int32_t) hb_clamp ((int64_t) cx0 * HB_RASTER_ONE_PIXEL,
					(int64_t) INT32_MIN, (int64_t) INT32_MAX);
      int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * total_dy) / total_dx);
      cell_add (area, cover, width, cx0 - x_org, fx0, fy0, 0, fy_b, wind, x_min, x_max);

      int32_t fy_prev = fy_b;
      ପାଇଁ (int32_t cx = cx0 - 1; cx > cx1; cx--)
      {
	fy_b = fy_prev - delta_fy;
	cell_add (area, cover, width, cx - x_org, HB_RASTER_ONE_PIXEL, fy_prev, 0, fy_b, wind, x_min, x_max);
	fy_prev = fy_b;
      }

      cell_add (area, cover, width, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_prev, fx1, fy1, wind, x_min, x_max);
      ଫେରନ୍ତୁ;
    }

    int64_t col_min = (int64_t) x_org;
    int64_t col_max = (int64_t) x_org + (int64_t) width - 1;

    ଯଦି (unlikely (cx0 < col_min))
    {
      /* Entirely left of the surface. */
      cell_add (area, cover, width, -1, 0, fy0, 0, fy1, wind, x_min, x_max);
      ଫେରନ୍ତୁ;
    }
    ଯଦି (unlikely (cx1 > col_max))
      ଫେରନ୍ତୁ; /* Entirely right of the surface. */

    int32_t x_b = (int32_t) hb_clamp ((int64_t) cx0 * HB_RASTER_ONE_PIXEL,
				      (int64_t) INT32_MIN, (int64_t) INT32_MAX);
    int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * total_dy) / total_dx);
    cell_add (area, cover, width, cx0 - x_org, fx0, fy0, 0, fy_b, wind, x_min, x_max);

    int32_t cx = cx0 - 1;
    int32_t fy_prev = fy_b;
    ଯଦି (unlikely (cx > col_max))
    {
      /* Mid columns right of the surface contribute nothing. */
      int32_t cx_skip = (int32_t) col_max; /* ≥ cx1, so it fits */
      fy_prev = fy_b - (int32_t) ((int64_t) (cx - cx_skip) * delta_fy);
      cx = cx_skip;
    }

    int32_t cx_stop = (int32_t) hb_max ((int64_t) cx1, col_min - 1);
    ପାଇଁ (; cx > cx_stop; cx--)
    {
      fy_b = fy_prev - delta_fy;
      cell_add (area, cover, width, cx - x_org, HB_RASTER_ONE_PIXEL, fy_prev, 0, fy_b, wind, x_min, x_max);
      fy_prev = fy_b;
    }

    ଯଦି (likely (cx1 >= col_min))
      cell_add (area, cover, width, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_prev, fx1, fy1, wind, x_min, x_max);
    ଅନ୍ୟଥା
      /* Fold the mid columns left of the surface and the last cell
       * into one column-0 cover update. */
      cell_add (area, cover, width, -1, 0, fy_prev, 0, fy1, wind, x_min, x_max);
  }
}

/* Convert cover-delta + area to alpha bytes, then clear.
   Returns final cover accumulator over [x_min, x_max]. */
ସ୍ଥିର int32_t
sweep_row_to_alpha (uint8_t *__restrict row_buf,
		    int32_t *__restrict area,
		    int16_t *__restrict cover,
		    unsigned x_min,
		    unsigned x_max)
{
  ଅଚଳ int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
  int32_t cover_accum = 0;
  unsigned x = x_min;

#ifdef HB_RASTER_NEON
  int32x4_t clamp_v = vdupq_n_s32 (HB_RASTER_FULL_COVERAGE);
  int32x4_t bias_v  = vdupq_n_s32 (HB_RASTER_FULL_COVERAGE / 2);
  int32x4_t zero32  = vdupq_n_s32 (0);
  int16x8_t zero16  = vdupq_n_s16 (0);
  ପାଇଁ (; x + 7 <= x_max; x += 8)
  {
    int32_t ctmp[8];
    ପାଇଁ (unsigned i = 0; i < 8; i++)
    {
      cover_accum += cover[x + i];
      ctmp[i] = cover_accum * cover_scale;
    }

    int32x4_t c0  = vld1q_s32 (ctmp + 0);
    int32x4_t c1  = vld1q_s32 (ctmp + 4);
    int32x4_t a0  = vld1q_s32 (area + x);
    int32x4_t a1  = vld1q_s32 (area + x + 4);

    int32x4_t v0 = vabsq_s32 (vsubq_s32 (c0, a0));
    int32x4_t v1 = vabsq_s32 (vsubq_s32 (c1, a1));

    v0 = vminq_s32 (v0, clamp_v);
    v1 = vminq_s32 (v1, clamp_v);

    int32x4_t r0 = vshrq_n_s32 (vmlaq_n_s32 (bias_v, v0, 255), 2 * HB_RASTER_PIXEL_BITS + 1);
    int32x4_t r1 = vshrq_n_s32 (vmlaq_n_s32 (bias_v, v1, 255), 2 * HB_RASTER_PIXEL_BITS + 1);

    int16x4_t h0 = vmovn_s32 (r0);
    int16x4_t h1 = vmovn_s32 (r1);
    int16x8_t h  = vcombine_s16 (h0, h1);
    uint8x8_t b  = vqmovun_s16 (h);
    vst1_u8 (row_buf + x, b);

    vst1q_s32 (area + x,     zero32);
    vst1q_s32 (area + x + 4, zero32);
    vst1q_s16 (cover + x,    zero16);
  }
#ନଚେତଯଦି defined(HB_RASTER_SSE2)
  __m128i clamp_v = _mm_set1_epi32 (HB_RASTER_FULL_COVERAGE);
  __m128i bias_v  = _mm_set1_epi32 (HB_RASTER_FULL_COVERAGE / 2);
  __m128i zero_v  = _mm_setzero_si128 ();
  ପାଇଁ (; x + 7 <= x_max; x += 8)
  {
    int32_t ctmp[8];
    ପାଇଁ (unsigned i = 0; i < 8; i++)
    {
      cover_accum += cover[x + i];
      ctmp[i] = cover_accum * cover_scale;
    }

    __m128i c0  = _mm_loadu_si128 ((__m128i *) (ଶୂନ୍ୟ *) (ctmp + 0));
    __m128i c1  = _mm_loadu_si128 ((__m128i *) (ଶୂନ୍ୟ *) (ctmp + 4));
    __m128i a0  = _mm_loadu_si128 ((__m128i *) (ଶୂନ୍ୟ *) (area + x));
    __m128i a1  = _mm_loadu_si128 ((__m128i *) (ଶୂନ୍ୟ *) (area + x + 4));

    __m128i v0 = _mm_sub_epi32 (c0, a0);
    __m128i v1 = _mm_sub_epi32 (c1, a1);

    __m128i s0 = _mm_srai_epi32 (v0, 31);
    __m128i s1 = _mm_srai_epi32 (v1, 31);
    v0 = _mm_sub_epi32 (_mm_xor_si128 (v0, s0), s0);
    v1 = _mm_sub_epi32 (_mm_xor_si128 (v1, s1), s1);

    __m128i lt0 = _mm_cmplt_epi32 (v0, clamp_v);
    __m128i lt1 = _mm_cmplt_epi32 (v1, clamp_v);
    v0 = _mm_or_si128 (_mm_and_si128 (lt0, v0), _mm_andnot_si128 (lt0, clamp_v));
    v1 = _mm_or_si128 (_mm_and_si128 (lt1, v1), _mm_andnot_si128 (lt1, clamp_v));

    __m128i r0 = _mm_srai_epi32 (_mm_add_epi32 (_mm_sub_epi32 (_mm_slli_epi32 (v0, 8), v0), bias_v), 2 * HB_RASTER_PIXEL_BITS + 1);
    __m128i r1 = _mm_srai_epi32 (_mm_add_epi32 (_mm_sub_epi32 (_mm_slli_epi32 (v1, 8), v1), bias_v), 2 * HB_RASTER_PIXEL_BITS + 1);

    __m128i h = _mm_packs_epi32 (r0, r1);
    __m128i b = _mm_packus_epi16 (h, h);
    _mm_storel_epi64 ((__m128i *) (ଶୂନ୍ୟ *) (row_buf + x), b);

    _mm_storeu_si128 ((__m128i *) (ଶୂନ୍ୟ *) (area + x),     zero_v);
    _mm_storeu_si128 ((__m128i *) (ଶୂନ୍ୟ *) (area + x + 4), zero_v);
    _mm_storeu_si128 ((__m128i *) (ଶୂନ୍ୟ *) (cover + x),    zero_v);
  }
#endif

  ପାଇଁ (; x <= x_max; x++)
  {
    cover_accum += cover[x];
    int32_t val   = cover_accum * cover_scale - area[x];
    int32_t alpha = val < 0 ? -val : val;
    ଯଦି (alpha > HB_RASTER_FULL_COVERAGE) alpha = HB_RASTER_FULL_COVERAGE;
    row_buf[x] = (uint8_t) (((unsigned) alpha * 255 + HB_RASTER_FULL_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_BITS + 1));
    area[x]  = 0;
    cover[x] = 0;
  }

  ଫେରନ୍ତୁ cover_accum;
}

ସ୍ଥିର int64_t
sweep_row_to_alpha_wide (uint8_t *__restrict row_buf,
			 int64_t *__restrict area,
			 int64_t *__restrict cover,
			 unsigned x_min,
			 unsigned x_max)
{
  ଅଚଳ int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
  int64_t cover_accum = 0;

  ପାଇଁ (unsigned x = x_min; x <= x_max; x++)
  {
    cover_accum += cover[x];
    int64_t val = cover_accum * cover_scale - area[x];
    int64_t alpha = val < 0 ? -val : val;
    ଯଦି (alpha > HB_RASTER_FULL_COVERAGE) alpha = HB_RASTER_FULL_COVERAGE;
    row_buf[x] = (uint8_t) (((unsigned) alpha * 255 + HB_RASTER_FULL_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_BITS + 1));
    area[x] = 0;
    cover[x] = 0;
  }

  ଫେରନ୍ତୁ cover_accum;
}


/**
 * hb_raster_draw_render:
 * @draw: a rasterizer
 *
 * Rasterizes the accumulated outline geometry into a new
 * #hb_raster_image_t.  After rendering, the accumulated edges are
 * cleared so the rasterizer can be reused. Output format is always
 * @HB_RASTER_FORMAT_A8.
 *
 * Return value: (transfer full):
 * A rendered #hb_raster_image_t. Returns `NULL` on allocation/configuration
 * failure. If no geometry was accumulated, returns an empty image.
 *
 * Since: 13.0.0
 **/
hb_raster_image_t *
hb_raster_draw_render (hb_raster_draw_t *draw)
{
  /* ── 1. Compute result extents ─────────────────────────────────── */
  hb_raster_extents_t ext;

  ଯଦି (draw->has_extents)
  {
    ext = draw->fixed_extents;
  }
  ଅନ୍ୟଥା
  {
    /* Auto-size from edge bounding box */
    ଯଦି (draw->edges.length == 0)
    {
      /* No edges: produce 0×0 image */
      ext = { 0, 0, 0, 0, 0 };
    }
    ଅନ୍ୟଥା
    {
      int32_t xmin = draw->edges.arrayZ[0].xL, xmax = draw->edges.arrayZ[0].xL;
      int32_t ymin = draw->edges.arrayZ[0].yL, ymax = draw->edges.arrayZ[0].yH;

      ପାଇଁ (ଅଚଳ auto &e : draw->edges)
      {
	xmin = hb_min (xmin, hb_min (e.xL, e.xH));
	xmax = hb_max (xmax, hb_max (e.xL, e.xH));
	ymin = hb_min (ymin, e.yL);
	ymax = hb_max (ymax, e.yH);
      }

      /* Convert fixed-point → pixels (floor for min, ceil for max).  Edge
	 coordinates are saturated to int32 range in emit_segment, so the
	 +MASK ceil step must be widened to avoid signed overflow. */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା x0 = xmin >> HB_RASTER_PIXEL_BITS;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା y0 = ymin >> HB_RASTER_PIXEL_BITS;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା x1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (((int64_t) xmax + HB_RASTER_PIXEL_MASK) >> HB_RASTER_PIXEL_BITS);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା y1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (((int64_t) ymax + HB_RASTER_PIXEL_MASK) >> HB_RASTER_PIXEL_BITS);

      ext.x_origin = x0;
      ext.y_origin = y0;
      ext.width    = (unsigned) hb_min (hb_max (0, x1 - x0), HB_RASTER_MAX_AUTO_DIMENSION);
      ext.height   = (unsigned) hb_min (hb_max (0, y1 - y0), HB_RASTER_MAX_AUTO_DIMENSION);
      ext.stride   = 0; /* filled below */
    }
  }

  /* ── 2. Compute stride ─────────────────────────────────────────── */
  ଯଦି (ext.stride == 0)
    ext.stride = (ext.width + 3u) & ~3u;

  /* ── 3. Allocate or reuse image ─────────────────────────────────── */
  /* Reset one-shot state on every exit path. */
  HB_SCOPE_GUARD (hb_raster_draw_clear (draw));

  hb_unique_ptr_t<hb_raster_image_t> image;
  ଯଦି (draw->recycled_image)
  {
    image = hb_unique_ptr_t<hb_raster_image_t> (draw->recycled_image);
    draw->recycled_image = nullptr;
  }
  ଅନ୍ୟଥା
  {
    image = hb_unique_ptr_t<hb_raster_image_t> (hb_raster_image_create_or_fail ());
    ଯଦି (unlikely (!image)) ଫେରନ୍ତୁ nullptr;
  }

  ଯଦି (unlikely (!image->configure (HB_RASTER_FORMAT_A8, ext)))
    ଫେରନ୍ତୁ nullptr;
  image->clear ();

  /* ── 4. Bucket edges by starting row and rasterize scanlines ──── */
  ଯଦି (draw->edges.length && ext.width && ext.height)
  {
    ଯଦି (unlikely (!draw->row_area.resize_dirty (ext.width) ||
		  !draw->row_cover.resize_dirty (ext.width)))
      ଫେରନ୍ତୁ nullptr;
    hb_memset (draw->row_area.arrayZ,  0, ext.width * ଆକାର (int32_t));
    hb_memset (draw->row_cover.arrayZ, 0, ext.width * ଆକାର (int16_t));

    /* Bucket edges by their starting pixel row.
       Only grow the outer vector; clear inner vectors without freeing. */
    unsigned old_buckets = draw->edge_buckets.length;
    ଯଦି (ext.height > old_buckets)
    {
      ଯଦି (unlikely (!draw->edge_buckets.resize (ext.height)))
	ଫେରନ୍ତୁ nullptr;
    }
    ପାଇଁ (unsigned i = 0; i < hb_min (ext.height, old_buckets); i++)
      draw->edge_buckets.arrayZ[i].clear ();
    /* New buckets (if any) are already empty from resize's zero-init. */

    ପାଇଁ (unsigned i = 0; i < draw->edges.length; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା row = (draw->edges.arrayZ[i].yL >> HB_RASTER_PIXEL_BITS) - ext.y_origin;
      ଯଦି (row < 0) row = 0;
      ଯଦି ((unsigned) row >= ext.height) ଜାରି;
      draw->edge_buckets.arrayZ[row].push (i);
    }

    /* Scanline loop with active edge list. */
    draw->active_edges.clear ();
    ତାର୍କିକ wide_initialized = ମିଥ୍ୟା;

    ପାଇଁ (unsigned row = 0; row < ext.height; row++)
    {
      int64_t y_ଶୀର୍ଷ_64 = ((int64_t) ext.y_origin + (int64_t) row) * HB_RASTER_ONE_PIXEL;
      int32_t y_ଶୀର୍ଷ = (int32_t) hb_clamp (y_ଶୀର୍ଷ_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);

      /* Add new edges from this row's bucket. */
      draw->active_edges.extend (draw->edge_buckets.arrayZ[row]);

      /* Process active edges and compact live ones in one linear pass. */
      unsigned x_min = ext.width, x_max = 0;
      unsigned ଲିଖ = 0;
      unsigned active_len = draw->active_edges.length;
      ତାର୍କିକ use_wide = active_len > HB_RASTER_NARROW_MAX_ACTIVE_EDGES;
      ଯଦି (unlikely (use_wide && !wide_initialized))
      {
	ଯଦି (unlikely (!draw->row_area_wide.resize_dirty (ext.width) ||
		      !draw->row_cover_wide.resize_dirty (ext.width)))
	  ଫେରନ୍ତୁ nullptr;
	hb_memset (draw->row_area_wide.arrayZ,  0, ext.width * ଆକାର (int64_t));
	hb_memset (draw->row_cover_wide.arrayZ, 0, ext.width * ଆକାର (int64_t));
	wide_initialized = ସତ୍ୟ;
      }
      ପାଇଁ (unsigned j = 0; j < active_len; j++)
      {
	unsigned edge_idx = draw->active_edges.arrayZ[j];
	ଅଚଳ auto &e = draw->edges.arrayZ[edge_idx];
	ଯଦି (e.yH <= y_ଶୀର୍ଷ)
	  ଜାରି;

	ଯଦି (unlikely (use_wide))
	  edge_sweep_row (draw->row_area_wide.arrayZ, draw->row_cover_wide.arrayZ,
			  ext.width, ext.x_origin, y_ଶୀର୍ଷ, e, x_min, x_max);
	ଅନ୍ୟଥା
	  edge_sweep_row (draw->row_area.arrayZ, draw->row_cover.arrayZ,
			  ext.width, ext.x_origin, y_ଶୀର୍ଷ, e, x_min, x_max);
	draw->active_edges.arrayZ[ଲିଖ++] = edge_idx;
      }
      draw->active_edges.resize (ଲିଖ);

      ଯଦି (x_min <= x_max)
      {
	int64_t cover_accum = use_wide ?
	  sweep_row_to_alpha_wide (image->buffer.arrayZ + row * ext.stride,
				   draw->row_area_wide.arrayZ, draw->row_cover_wide.arrayZ,
				   x_min, x_max) :
	  sweep_row_to_alpha (image->buffer.arrayZ + row * ext.stride,
			      draw->row_area.arrayZ, draw->row_cover.arrayZ,
			      x_min, x_max);

	/* If cover doesn't cancel, memset the constant-alpha tail. */
	ଯଦି (cover_accum != 0)
	{
	  int64_t alpha = cover_accum * (2 * HB_RASTER_ONE_PIXEL);
	  alpha = alpha < 0 ? -alpha : alpha;
	  ଯଦି (alpha > HB_RASTER_FULL_COVERAGE) alpha = HB_RASTER_FULL_COVERAGE;
	  uint8_t byte = (uint8_t) (((unsigned) alpha * 255 + HB_RASTER_FULL_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_BITS + 1));

	  uint8_t *row_buf = image->buffer.arrayZ + row * ext.stride;
	  hb_memset (row_buf + x_max + 1, byte, ext.width - 1 - x_max);
	}
      }
    }
  }

  ଫେରନ୍ତୁ image.release ();
}
