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

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-raster-image.hh"
#అన్తర్భావ "hb-geometry.hh"
#అన్తర్భావ "hb-machinery.hh"

#ఒకవేళ defined(__aarch64__) || defined(_M_ARM64)
#అన్తర్భావ <arm_neon.h>
#నిర్వచించు HB_RASTER_NEON 1
#లేదావేళ defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64)
#అన్తర్భావ <emmintrin.h>
#నిర్వచించు HB_RASTER_SSE2 1
#అయితే_ముగింపు


/* Fixed-point precision for sub-pixel coordinates.
   8 bits = 24.8: 256 sub-pixel units per pixel. */
#నిర్వచించు HB_RASTER_PIXEL_బిట్లు 8
#నిర్వచించు HB_RASTER_ONE_PIXEL  (1 << HB_RASTER_PIXEL_బిట్లు)
#నిర్వచించు HB_RASTER_PIXEL_ముసుగు (HB_RASTER_ONE_PIXEL - 1)
/* Full-coverage alpha = 2 * ONE_PIXEL^2 */
#నిర్వచించు HB_RASTER_నిండిన_COVERAGE (2 * HB_RASTER_ONE_PIXEL * HB_RASTER_ONE_PIXEL)
/* Flatness threshold for Bézier flattening: max deviation in pixels */
#నిర్వచించు HB_RASTER_FLAT_THRESH 0.25f


/* Normalized edge: yH > yL always */
నిర్మాణం hb_raster_అంచు_t
{
  int32_t xL, yL;   /* lower endpoint (fixed-point) */
  int32_t xH, yH;   /* upper endpoint (fixed-point) */
  int64_t వాలు;    /* dx/dy in 16.16 fixed point: ((int64_t)dx << 16) / dy */
  int32_t wind;     /* +1 or -1 */
};

/* hb_raster_draw_t — outline rasterizer */
నిర్మాణం hb_raster_draw_t
{
  hb_object_శీర్షిక_t శీర్షిక;

  /* Configuration */
  hb_transform_t<>    transform         = {1, 0, 0, 1, 0, 0};
  దశాంశం               x_scale_గుణకం    = 1.f;
  దశాంశం               y_scale_గుణకం    = 1.f;
  hb_raster_extents_t fixed_extents     = {};
  తార్కిక                has_extents = అబద్ధం;

  /* Visibility clip box for curve flattening (device pixels); set
     internally by raster-paint so invisible curves collapse to their
     chord.  See hb_raster_draw_set_clip_box(). */
  తార్కిక  has_clip_box = అబద్ధం;
  దశాంశం clip_x0 = 0.f, clip_y0 = 0.f, clip_x1 = 0.f, clip_y1 = 0.f;

  /* Flattening clip resolved from the above (or from fixed extents),
     expanded by one pixel; kept in sync by hb_raster_draw_update_flatten_clip(). */
  తార్కిక  flatten_clip_చురుకైన = అబద్ధం;
  దశాంశం flatten_clip_x0 = 0.f, flatten_clip_y0 = 0.f;
  దశాంశం flatten_clip_x1 = 0.f, flatten_clip_y1 = 0.f;

  /* Work budget for outline traversal and curve flattening.  A paint
     backend seeds this through the public draw-budget API before drawing
     and reads the remainder back afterwards, so a whole paint session
     shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_మిగిలిన = HB_BUDGET_GLYPH;

  శూన్య recharge_budget ()
  {
    budget_మిగిలిన = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_మిగిలిన ()
  { తిరిగి &budget_మిగిలిన; }

  /* Accumulated geometry */
  int64_t అంచులు_ఎడమ = HB_RASTER_MAX_DRAW_అంచులు;
  hb_సదిశ_t<hb_raster_అంచు_t> అంచులు;

  /* Scratch — reused across render() calls */
  hb_సదిశ_t<int32_t> అడ్డువరుస_area;
  hb_సదిశ_t<int16_t> అడ్డువరుస_cover;
  hb_సదిశ_t<int64_t> అడ్డువరుస_area_wide;
  hb_సదిశ_t<int64_t> అడ్డువరుస_cover_wide;
  hb_సదిశ_t<hb_సదిశ_t<గుర్తులేని>> అంచు_buckets;
  hb_సదిశ_t<గుర్తులేని> చురుకైన_అంచులు;

  /* Recycled image for zero-malloc render */
  hb_raster_image_t *recycled_image = శూన్యసూచిక;
};

స్థిర HB_ALWAYS_INLINE శూన్య
hb_raster_draw_transform_బిందువు (స్థిరరాశి hb_raster_draw_t *draw,
				దశాంశం x, దశాంశం y,
				దశాంశం &tx, దశాంశం &ty)
{
  tx = x; ty = y;
  draw->transform.transform_బిందువు (tx, ty);
  tx /= draw->x_scale_గుణకం;
  ty /= draw->y_scale_గుణకం;
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
hb_raster_draw_create_or_fail (శూన్య)
{
  hb_raster_draw_t *draw = hb_object_create<hb_raster_draw_t> ();
  తిరిగి draw;
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
  తిరిగి hb_object_reference (draw);
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
శూన్య
hb_raster_draw_destroy (hb_raster_draw_t *draw)
{
  ఒకవేళ (!hb_object_should_destroy (draw))
    తిరిగి;

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
hb_raster_draw_సమితి_user_దత్తాంశం (hb_raster_draw_t   *draw,
			      hb_user_దత్తాంశం_కీలకం_t *key,
			      శూన్య               *దత్తాంశం,
			      hb_destroy_func_t   destroy,
			      hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (draw, key, దత్తాంశం, destroy, replace);
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
శూన్య *
hb_raster_draw_get_user_దత్తాంశం (స్థిరరాశి hb_raster_draw_t   *draw,
			      hb_user_దత్తాంశం_కీలకం_t *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (draw, key);
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
శూన్య
hb_raster_draw_సమితి_transform (hb_raster_draw_t *draw,
			      దశాంశం xx, దశాంశం yx,
			      దశాంశం xy, దశాంశం yy,
			      దశాంశం dx, దశాంశం dy)
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
శూన్య
hb_raster_draw_సమితి_scale_గుణకం (hb_raster_draw_t *draw,
				 దశాంశం x_scale_గుణకం,
				 దశాంశం y_scale_గుణకం)
{
  draw->x_scale_గుణకం = x_scale_గుణకం > 0.f ? x_scale_గుణకం : 1.f;
  draw->y_scale_గుణకం = y_scale_గుణకం > 0.f ? y_scale_గుణకం : 1.f;
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
శూన్య
hb_raster_draw_get_scale_గుణకం (స్థిరరాశి hb_raster_draw_t *draw,
				 దశాంశం *x_scale_గుణకం,
				 దశాంశం *y_scale_గుణకం)
{
  ఒకవేళ (x_scale_గుణకం) *x_scale_గుణకం = draw->x_scale_గుణకం;
  ఒకవేళ (y_scale_గుణకం) *y_scale_గుణకం = draw->y_scale_గుణకం;
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
శూన్య
hb_raster_draw_get_transform (స్థిరరాశి hb_raster_draw_t *draw,
			      దశాంశం *xx, దశాంశం *yx,
			      దశాంశం *xy, దశాంశం *yy,
			      దశాంశం *dx, దశాంశం *dy)
{
  ఒకవేళ (xx) *xx = draw->transform.xx;
  ఒకవేళ (yx) *yx = draw->transform.yx;
  ఒకవేళ (xy) *xy = draw->transform.xy;
  ఒకవేళ (yy) *yy = draw->transform.yy;
  ఒకవేళ (dx) *dx = draw->transform.x0;
  ఒకవేళ (dy) *dy = draw->transform.y0;
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
స్థిర శూన్య
hb_raster_draw_update_flatten_clip (hb_raster_draw_t *draw)
{
  ఒకవేళ (draw->has_clip_box)
  {
    draw->flatten_clip_చురుకైన = నిజం;
    draw->flatten_clip_x0 = draw->clip_x0 - 1.f;
    draw->flatten_clip_y0 = draw->clip_y0 - 1.f;
    draw->flatten_clip_x1 = draw->clip_x1 + 1.f;
    draw->flatten_clip_y1 = draw->clip_y1 + 1.f;
  }
  లేకపోతే ఒకవేళ (draw->has_extents)
  {
    draw->flatten_clip_చురుకైన = నిజం;
    draw->flatten_clip_x0 = (దశాంశం) draw->fixed_extents.x_origin - 1.f;
    draw->flatten_clip_y0 = (దశాంశం) draw->fixed_extents.y_origin - 1.f;
    draw->flatten_clip_x1 = (దశాంశం) draw->fixed_extents.x_origin + (దశాంశం) draw->fixed_extents.వెడల్పు + 1.f;
    draw->flatten_clip_y1 = (దశాంశం) draw->fixed_extents.y_origin + (దశాంశం) draw->fixed_extents.ఎత్తు + 1.f;
  }
  లేకపోతే
    draw->flatten_clip_చురుకైన = అబద్ధం;
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
శూన్య
hb_raster_draw_సమితి_extents (hb_raster_draw_t          *draw,
			    స్థిరరాశి hb_raster_extents_t *extents)
{
  draw->fixed_extents     = *extents;
  draw->has_extents = నిజం;
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
hb_raster_draw_get_extents (స్థిరరాశి hb_raster_draw_t    *draw,
			    hb_raster_extents_t *extents)
{
  ఒకవేళ (!draw->has_extents)
    తిరిగి అబద్ధం;

  ఒకవేళ (extents)
    *extents = draw->fixed_extents;
  తిరిగి నిజం;
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
hb_raster_draw_సమితి_glyph_extents (hb_raster_draw_t         *draw,
				  స్థిరరాశి hb_glyph_extents_t *glyph_extents)
{
  దశాంశం x0 = (దశాంశం) glyph_extents->x_bearing;
  దశాంశం y0 = (దశాంశం) glyph_extents->y_bearing;
  దశాంశం x1 = (దశాంశం) glyph_extents->x_bearing + glyph_extents->వెడల్పు;
  దశాంశం y1 = (దశాంశం) glyph_extents->y_bearing + glyph_extents->ఎత్తు;

  దశాంశం xmin = hb_min (x0, x1);
  దశాంశం xmax = hb_max (x0, x1);
  దశాంశం ymin = hb_min (y0, y1);
  దశాంశం ymax = hb_max (y0, y1);

  దశాంశం px[4] = {xmin, xmin, xmax, xmax};
  దశాంశం py[4] = {ymin, ymax, ymin, ymax};

  దశాంశం tx, ty;
  hb_raster_draw_transform_బిందువు (draw, px[0], py[0], tx, ty);
  దశాంశం tx_min = tx, tx_max = tx;
  దశాంశం ty_min = ty, ty_max = ty;

  కొరకు (గుర్తులేని i = 1; i < 4; i++)
  {
    hb_raster_draw_transform_బిందువు (draw, px[i], py[i], tx, ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  int32_t ex0 = hb_clamp_గమ్యం<int32_t> (floorf (tx_min));
  int32_t ey0 = hb_clamp_గమ్యం<int32_t> (floorf (ty_min));
  int32_t ex1 = hb_clamp_గమ్యం<int32_t> (ceilf  (tx_max));
  int32_t ey1 = hb_clamp_గమ్యం<int32_t> (ceilf  (ty_max));

  ఒకవేళ (ex1 <= ex0 || ey1 <= ey0)
  {
    draw->fixed_extents = {};
    draw->has_extents = అబద్ధం;
    hb_raster_draw_update_flatten_clip (draw);
    తిరిగి అబద్ధం;
  }

  draw->fixed_extents = {
    ex0, ey0,
    (గుర్తులేని) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (గుర్తులేని) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  draw->has_extents = నిజం;
  hb_raster_draw_update_flatten_clip (draw);
  తిరిగి నిజం;
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
శూన్య
hb_raster_draw_clear (hb_raster_draw_t *draw)
{
  draw->fixed_extents     = {};
  draw->has_extents = అబద్ధం;
  draw->has_clip_box = అబద్ధం;
  draw->flatten_clip_చురుకైన = అబద్ధం;
  draw->recharge_budget ();
  draw->అంచులు_ఎడమ = HB_RASTER_MAX_DRAW_అంచులు;
  draw->అంచులు.clear ();
  draw->చురుకైన_అంచులు.clear ();
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
శూన్య
hb_raster_draw_reset (hb_raster_draw_t *draw)
{
  draw->transform         = {1, 0, 0, 1, 0, 0};
  draw->x_scale_గుణకం    = 1.f;
  draw->y_scale_గుణకం    = 1.f;
  draw->budget            = HB_BUDGET_DEFAULT;
  hb_raster_draw_clear (draw);
}

శూన్య
hb_raster_draw_సమితి_clip_box (hb_raster_draw_t *draw,
			     దశాంశం x0, దశాంశం y0,
			     దశాంశం x1, దశాంశం y1)
{
  draw->has_clip_box = నిజం;
  draw->clip_x0 = x0;
  draw->clip_y0 = y0;
  draw->clip_x1 = x1;
  draw->clip_y1 = y1;
  hb_raster_draw_update_flatten_clip (draw);
}

int64_t
hb_raster_draw_get_pixel_work (స్థిరరాశి hb_raster_draw_t *draw,
			       గుర్తులేని పూర్ణాంకం max_అడ్డువరుసలు,
			       గుర్తులేని పూర్ణాంకం max_నిలువుల_సంఖ్య)
{
  int64_t work = 0;
  కొరకు (స్థిరరాశి స్వయంచాలక &అంచు : draw->అంచులు)
  {
    int64_t dx = (int64_t) అంచు.xH - అంచు.xL;
    ఒకవేళ (dx < 0) dx = -dx;
    work += 1 + hb_min (((int64_t) అంచు.yH - అంచు.yL) >> HB_RASTER_PIXEL_బిట్లు,
			(int64_t) max_అడ్డువరుసలు)
	      + hb_min (dx >> HB_RASTER_PIXEL_బిట్లు,
			(int64_t) max_నిలువుల_సంఖ్య);
  }
  తిరిగి work;
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
శూన్య
hb_raster_draw_recycle_image (hb_raster_draw_t  *draw,
			      hb_raster_image_t *image)
{
  hb_raster_image_destroy (draw->recycled_image);
  draw->recycled_image = image;
}


/*
 * Draw callbacks — flatten on the fly into hb_raster_edge_t
 */

స్థిర అంతఃవరుస శూన్య
transform_బిందువు (స్థిరరాశి hb_raster_draw_t *draw,
		 దశాంశం  x,  దశాంశం  y,
		 దశాంశం &tx, దశాంశం &ty)
{
  hb_raster_draw_transform_బిందువు (draw, x, y, tx, ty);
}

స్థిర శూన్య
emit_segment (hb_raster_draw_t *draw,
	      దశాంశం x0, దశాంశం y0,
	      దశాంశం x1, దశాంశం y1)
{
  ఒకవేళ (unlikely (draw->అంచులు_ఎడమ <= 0))
    తిరిగి;

  int32_t X0 = hb_clamp_గమ్యం<int32_t> (roundf (x0 * HB_RASTER_ONE_PIXEL));
  int32_t Y0 = hb_clamp_గమ్యం<int32_t> (roundf (y0 * HB_RASTER_ONE_PIXEL));
  int32_t X1 = hb_clamp_గమ్యం<int32_t> (roundf (x1 * HB_RASTER_ONE_PIXEL));
  int32_t Y1 = hb_clamp_గమ్యం<int32_t> (roundf (y1 * HB_RASTER_ONE_PIXEL));

  ఒకవేళ (Y0 == Y1) తిరిగి; /* horizontal — skip */

  hb_raster_అంచు_t e;
  ఒకవేళ (Y0 < Y1) {
    e.xL = X0; e.yL = Y0; e.xH = X1; e.yH = Y1; e.wind = +1;
  } లేకపోతే {
    e.xL = X1; e.yL = Y1; e.xH = X0; e.yH = Y0; e.wind = -1;
  }
  e.వాలు = (((int64_t) e.xH - (int64_t) e.xL) * (int64_t) 65536) /
	    ((int64_t) e.yH - (int64_t) e.yL);

  ఒకవేళ (likely (draw->అంచులు.నెట్టు_or_fail (e)))
    draw->అంచులు_ఎడమ--;
  లేకపోతే
    draw->అంచులు_ఎడమ = 0;
}


/* Quadratic Bézier flattener — iterative de Casteljau at t=0.5. */
స్థిర అంతఃవరుస శూన్య
flatten_quadratic_recursive (hb_raster_draw_t *draw,
			     దశాంశం x0, దశాంశం y0,
			     దశాంశం x1, దశాంశం y1,
			     దశాంశం x2, దశాంశం y2,
			     పూర్ణాంకం లోతు = 0)
{
  నిర్మాణం quad_కణుపు_t
  {
    దశాంశం x0, y0, x1, y1, x2, y2;
    పూర్ణాంకం లోతు;
    తార్కిక check_clip;
  };

  quad_కణుపు_t దొంతర[16];
  గుర్తులేని శిఖరం_2 = 0;

  తార్కిక check_clip = draw->flatten_clip_చురుకైన;
  int64_t *work = draw->get_budget_మిగిలిన ();
  int64_t work_ఎడమ = *work;

  అయితే (నిజం)
  {
    తార్కిక emit_chord = లోతు >= 16;

    /* Entirely outside the clip box: the chord is exact for coverage.
       Subdivided control points are convex combinations of their
       parent's, so once a node is entirely inside the box no
       descendant can leave it and the test is skipped below. */
    ఒకవేళ (!emit_chord && check_clip)
    {
      దశాంశం bx0 = hb_min (x0, hb_min (x1, x2));
      దశాంశం bx1 = hb_max (x0, hb_max (x1, x2));
      దశాంశం by0 = hb_min (y0, hb_min (y1, y2));
      దశాంశం by1 = hb_max (y0, hb_max (y1, y2));
      ఒకవేళ (bx1 < draw->flatten_clip_x0 || bx0 > draw->flatten_clip_x1 ||
	  by1 < draw->flatten_clip_y0 || by0 > draw->flatten_clip_y1)
	emit_chord = నిజం;
      లేకపోతే ఒకవేళ (bx0 >= draw->flatten_clip_x0 && bx1 <= draw->flatten_clip_x1 &&
	       by0 >= draw->flatten_clip_y0 && by1 <= draw->flatten_clip_y1)
	check_clip = అబద్ధం;
    }

    ఒకవేళ (!emit_chord)
    {
      తార్కిక is_flat;
      ఒకవేళ (అబద్ధం)
      {
	/* Old behavior: midpoint deviation from chord midpoint. */
	దశాంశం mx = x0 * 0.25f + x1 * 0.5f + x2 * 0.25f;
	దశాంశం my = y0 * 0.25f + y1 * 0.5f + y2 * 0.25f;
	దశాంశం chord_mx = (x0 + x2) * 0.5f;
	దశాంశం chord_my = (y0 + y2) * 0.5f;
	దశాంశం dx = mx - chord_mx;
	దశాంశం dy = my - chord_my;
	స్థిర స్థిరరాశి దశాంశం flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;
	is_flat = (dx * dx + dy * dy) <= flat_thresh;
      }
      లేకపోతే
      {
	/* FreeType behavior: control-point deviation from chord center. */
	స్థిరరాశి దశాంశం flat_thresh = 0.25f;
	దశాంశం dx = x0 + x2 - 2.f * x1;
	దశాంశం dy = y0 + y2 - 2.f * y1;
	ఒకవేళ (dx < 0) dx = -dx;
	ఒకవేళ (dy < 0) dy = -dy;
	is_flat = dx <= flat_thresh && dy <= flat_thresh;
      }
      emit_chord = is_flat;
    }

    /* Charge one unit per subdivision; degrade to the chord when the
       session work budget is spent. */
    ఒకవేళ (!emit_chord)
    {
      ఒకవేళ (unlikely (work_ఎడమ <= 0))
	emit_chord = నిజం;
      లేకపోతే
	work_ఎడమ--;
    }

    ఒకవేళ (emit_chord)
    {
      emit_segment (draw, x0, y0, x2, y2);
      ఒకవేళ (!శిఖరం_2)
      {
	*work = work_ఎడమ;
	తిరిగి;
      }
      స్థిరరాశి quad_కణుపు_t &n = దొంతర[--శిఖరం_2];
      x0 = n.x0; y0 = n.y0;
      x1 = n.x1; y1 = n.y1;
      x2 = n.x2; y2 = n.y2;
      లోతు = n.లోతు;
      check_clip = n.check_clip;
      కొనసాగు;
    }

    దశాంశం x01 = (x0 + x1) * 0.5f, y01 = (y0 + y1) * 0.5f;
    దశాంశం x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
    దశాంశం xm  = (x01 + x12) * 0.5f, ym  = (y01 + y12) * 0.5f;

    /* Depth is capped at 16, so stack capacity 16 is sufficient. */
    దొంతర[శిఖరం_2++] = {xm, ym, x12, y12, x2, y2, లోతు + 1, check_clip};
    x2 = xm; y2 = ym;
    x1 = x01; y1 = y01;
    లోతు++;
  }
}

/* Quadratic Bézier flattener using forward differencing.
   The error (midpoint deviation) shrinks exactly 4× per de Casteljau
   subdivision, so we compute the subdivision count upfront and iterate
   with constant-cost additions instead of recursive branching. */
స్థిర అంతఃవరుస శూన్య
flatten_quadratic_fd (hb_raster_draw_t *draw,
		      దశాంశం x0, దశాంశం y0,
		      దశాంశం x1, దశాంశం y1,
		      దశాంశం x2, దశాంశం y2)
{
  /* Deviation of curve midpoint from chord midpoint (squared). */
  దశాంశం devx = (x0 - 2 * x1 + x2) * 0.25f;
  దశాంశం devy = (y0 - 2 * y1 + y2) * 0.25f;
  దశాంశం err2 = devx * devx + devy * devy;

  స్థిర స్థిరరాశి దశాంశం flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;

  ఒకవేళ (err2 <= flat_thresh)
  {
    emit_segment (draw, x0, y0, x2, y2);
    తిరిగి;
  }

  /* err² shrinks 16× per subdivision level.  Find n such that
     err2 / 16^n <= flat_thresh, i.e. n = ceil(log₁₆(err2/flat_thresh)). */
  గుర్తులేని n = 1;
  {
    దశాంశం నిష్పత్తి = err2 / flat_thresh;
    అయితే (నిష్పత్తి > 16.f) { నిష్పత్తి *= (1.f / 16.f); n++; }
    ఒకవేళ (n > 16) n = 16;
  }
  గుర్తులేని N = 1u << n;   /* number of line segments */
  దశాంశం h = 1.f / N;

  /* Quadratic: B(t) = a·t² + b·t + c
     Forward differences with step h:
       d²f = 2·a·h²   (constant)
       df₀ = a·h² + b·h
       f₀  = c = P₀                     */
  దశాంశం ax = x0 - 2 * x1 + x2;
  దశాంశం ay = y0 - 2 * y1 + y2;
  దశాంశం bx = 2 * (x1 - x0);
  దశాంశం by = 2 * (y1 - y0);

  దశాంశం d2fx = 2 * ax * h * h;
  దశాంశం d2fy = 2 * ay * h * h;
  దశాంశం dfx  = ax * h * h + bx * h;
  దశాంశం dfy  = ay * h * h + by * h;
  దశాంశం fx   = x0;
  దశాంశం fy   = y0;

  కొరకు (గుర్తులేని i = 1; i < N; i++)
  {
    దశాంశం nx = fx + dfx;
    దశాంశం ny = fy + dfy;
    emit_segment (draw, fx, fy, nx, ny);
    fx = nx;
    fy = ny;
    dfx += d2fx;
    dfy += d2fy;
  }
  /* Last segment uses exact endpoint to avoid drift. */
  emit_segment (draw, fx, fy, x2, y2);
}

స్థిర శూన్య
flatten_quadratic (hb_raster_draw_t *draw,
		   దశాంశం x0, దశాంశం y0,
		   దశాంశం x1, దశాంశం y1,
		   దశాంశం x2, దశాంశం y2)
{
  ఒకవేళ (అబద్ధం)
    flatten_quadratic_fd (draw, x0, y0, x1, y1, x2, y2);
  లేకపోతే
    flatten_quadratic_recursive (draw, x0, y0, x1, y1, x2, y2);
}

/* For cubic B(t), the max deviation from its chord on [0,1] is bounded by:
     max||B(t)-L(t)|| <= max_t ||B''(t)|| / 8.
   B''(t) is linear, so max norm is attained at t=0 or t=1:
     B''(0)=6*(P0-2P1+P2), B''(1)=6*(P1-2P2+P3). */
స్థిర అంతఃవరుస దశాంశం
cubic_chord_error_bound2 (దశాంశం x0, దశాంశం y0,
			  దశాంశం x1, దశాంశం y1,
			  దశాంశం x2, దశాంశం y2,
			  దశాంశం x3, దశాంశం y3)
{
  దశాంశం d20x = x0 - 2 * x1 + x2;
  దశాంశం d20y = y0 - 2 * y1 + y2;
  దశాంశం d21x = x1 - 2 * x2 + x3;
  దశాంశం d21y = y1 - 2 * y2 + y3;
  దశాంశం m0 = d20x * d20x + d20y * d20y;
  దశాంశం m1 = d21x * d21x + d21y * d21y;
  దశాంశం m = m0 > m1 ? m0 : m1;
  /* (max||B''||/8)^2 = (6/8)^2 * max||d2||^2 = (3/4)^2 * m. */
  తిరిగి m * (9.f / 16.f);
}

/* Cubic Bézier flattener — iterative de Casteljau at t=0.5. */
స్థిర అంతఃవరుస శూన్య
flatten_cubic_recursive (hb_raster_draw_t *draw,
			 దశాంశం x0, దశాంశం y0,
			 దశాంశం x1, దశాంశం y1,
			 దశాంశం x2, దశాంశం y2,
			 దశాంశం x3, దశాంశం y3,
			 పూర్ణాంకం లోతు = 0)
{
  నిర్మాణం cubic_కణుపు_t
  {
    దశాంశం x0, y0, x1, y1, x2, y2, x3, y3;
    పూర్ణాంకం లోతు;
    తార్కిక check_clip;
  };

  cubic_కణుపు_t దొంతర[16];
  గుర్తులేని శిఖరం_2 = 0;

  తార్కిక check_clip = draw->flatten_clip_చురుకైన;
  int64_t *work = draw->get_budget_మిగిలిన ();
  int64_t work_ఎడమ = *work;

  అయితే (నిజం)
  {
    తార్కిక emit_chord = లోతు >= 16;

    /* Entirely outside the clip box: the chord is exact for coverage.
       Subdivided control points are convex combinations of their
       parent's, so once a node is entirely inside the box no
       descendant can leave it and the test is skipped below. */
    ఒకవేళ (!emit_chord && check_clip)
    {
      దశాంశం bx0 = hb_min (hb_min (x0, x1), hb_min (x2, x3));
      దశాంశం bx1 = hb_max (hb_max (x0, x1), hb_max (x2, x3));
      దశాంశం by0 = hb_min (hb_min (y0, y1), hb_min (y2, y3));
      దశాంశం by1 = hb_max (hb_max (y0, y1), hb_max (y2, y3));
      ఒకవేళ (bx1 < draw->flatten_clip_x0 || bx0 > draw->flatten_clip_x1 ||
	  by1 < draw->flatten_clip_y0 || by0 > draw->flatten_clip_y1)
	emit_chord = నిజం;
      లేకపోతే ఒకవేళ (bx0 >= draw->flatten_clip_x0 && bx1 <= draw->flatten_clip_x1 &&
	       by0 >= draw->flatten_clip_y0 && by1 <= draw->flatten_clip_y1)
	check_clip = అబద్ధం;
    }

    ఒకవేళ (!emit_chord)
    {
      తార్కిక is_flat;
      ఒకవేళ (అబద్ధం)
      {
	/* Old behavior: curvature/chord-error bound. */
	దశాంశం err2 = cubic_chord_error_bound2 (x0, y0, x1, y1, x2, y2, x3, y3);
	స్థిర స్థిరరాశి దశాంశం flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;
	is_flat = err2 <= flat_thresh;
      }
      లేకపోతే
      {
	/* FreeType behavior: chord-trisection distance test. */
	స్థిరరాశి దశాంశం flat_thresh = 0.5f;

	దశాంశం d10x = 2.f * x0 - 3.f * x1 + x3;
	దశాంశం d10y = 2.f * y0 - 3.f * y1 + y3;
	దశాంశం d20x = x0 - 3.f * x2 + 2.f * x3;
	దశాంశం d20y = y0 - 3.f * y2 + 2.f * y3;

	ఒకవేళ (d10x < 0) d10x = -d10x;
	ఒకవేళ (d10y < 0) d10y = -d10y;
	ఒకవేళ (d20x < 0) d20x = -d20x;
	ఒకవేళ (d20y < 0) d20y = -d20y;

	is_flat = d10x <= flat_thresh &&
		  d10y <= flat_thresh &&
		  d20x <= flat_thresh &&
		  d20y <= flat_thresh;
      }
      emit_chord = is_flat;
    }

    /* Charge one unit per subdivision; degrade to the chord when the
       session work budget is spent. */
    ఒకవేళ (!emit_chord)
    {
      ఒకవేళ (unlikely (work_ఎడమ <= 0))
	emit_chord = నిజం;
      లేకపోతే
	work_ఎడమ--;
    }

    ఒకవేళ (emit_chord)
    {
      emit_segment (draw, x0, y0, x3, y3);
      ఒకవేళ (!శిఖరం_2)
      {
	*work = work_ఎడమ;
	తిరిగి;
      }
      స్థిరరాశి cubic_కణుపు_t &n = దొంతర[--శిఖరం_2];
      x0 = n.x0; y0 = n.y0;
      x1 = n.x1; y1 = n.y1;
      x2 = n.x2; y2 = n.y2;
      x3 = n.x3; y3 = n.y3;
      లోతు = n.లోతు;
      check_clip = n.check_clip;
      కొనసాగు;
    }

    దశాంశం x01  = (x0 + x1) * 0.5f, y01  = (y0 + y1) * 0.5f;
    దశాంశం x12  = (x1 + x2) * 0.5f, y12  = (y1 + y2) * 0.5f;
    దశాంశం x23  = (x2 + x3) * 0.5f, y23  = (y2 + y3) * 0.5f;
    దశాంశం x012 = (x01 + x12) * 0.5f, y012 = (y01 + y12) * 0.5f;
    దశాంశం x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
    దశాంశం xm   = (x012 + x123) * 0.5f, ym   = (y012 + y123) * 0.5f;

    /* Depth is capped at 16, so stack capacity 16 is sufficient. */
    దొంతర[శిఖరం_2++] = {xm, ym, x123, y123, x23, y23, x3, y3, లోతు + 1, check_clip};
    x3 = xm; y3 = ym;
    x2 = x012; y2 = y012;
    x1 = x01; y1 = y01;
    లోతు++;
  }
}

/* Cubic Bézier flattener using forward differencing.
   Use a curvature-based chord-error bound (max||B''||/8), then choose a
   uniform subdivision count n such that the bound drops below threshold.
   The cubic adds a constant third difference d³f = 6·a·h³. */
స్థిర అంతఃవరుస శూన్య
flatten_cubic_fd (hb_raster_draw_t *draw,
		  దశాంశం x0, దశాంశం y0,
		  దశాంశం x1, దశాంశం y1,
		  దశాంశం x2, దశాంశం y2,
		  దశాంశం x3, దశాంశం y3)
{
  దశాంశం err2 = cubic_chord_error_bound2 (x0, y0, x1, y1, x2, y2, x3, y3);

  స్థిర స్థిరరాశి దశాంశం flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;

  ఒకవేళ (err2 <= flat_thresh)
  {
    emit_segment (draw, x0, y0, x3, y3);
    తిరిగి;
  }

  /* The bound scales with h², so err² shrinks 16× per subdivision level. */
  గుర్తులేని n = 1;
  {
    దశాంశం నిష్పత్తి = err2 / flat_thresh;
    అయితే (నిష్పత్తి > 16.f) { నిష్పత్తి *= (1.f / 16.f); n++; }
    ఒకవేళ (n > 16) n = 16;
  }
  గుర్తులేని N = 1u << n;
  దశాంశం h = 1.f / N;

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
  దశాంశం ax = -x0 + 3*x1 - 3*x2 + x3;
  దశాంశం ay = -y0 + 3*y1 - 3*y2 + y3;
  దశాంశం bx =  3*x0 - 6*x1 + 3*x2;
  దశాంశం by =  3*y0 - 6*y1 + 3*y2;
  దశాంశం cx =  3*(x1 - x0);
  దశాంశం cy =  3*(y1 - y0);

  దశాంశం h2 = h * h, h3 = h2 * h;
  దశాంశం d3fx = 6 * ax * h3;
  దశాంశం d3fy = 6 * ay * h3;
  దశాంశం d2fx = d3fx + 2 * bx * h2;
  దశాంశం d2fy = d3fy + 2 * by * h2;
  దశాంశం dfx  = ax * h3 + bx * h2 + cx * h;
  దశాంశం dfy  = ay * h3 + by * h2 + cy * h;
  దశాంశం fx   = x0;
  దశాంశం fy   = y0;

  కొరకు (గుర్తులేని i = 1; i < N; i++)
  {
    దశాంశం nx = fx + dfx;
    దశాంశం ny = fy + dfy;
    emit_segment (draw, fx, fy, nx, ny);
    fx = nx;  fy = ny;
    dfx += d2fx;  dfy += d2fy;
    d2fx += d3fx; d2fy += d3fy;
  }
  /* Last segment uses exact endpoint to avoid drift. */
  emit_segment (draw, fx, fy, x3, y3);
}

స్థిర శూన్య
flatten_cubic (hb_raster_draw_t *draw,
	       దశాంశం x0, దశాంశం y0,
	       దశాంశం x1, దశాంశం y1,
	       దశాంశం x2, దశాంశం y2,
	       దశాంశం x3, దశాంశం y3)
{
  ఒకవేళ (అబద్ధం)
    flatten_cubic_fd (draw, x0, y0, x1, y1, x2, y2, x3, y3);
  లేకపోతే
    flatten_cubic_recursive (draw, x0, y0, x1, y1, x2, y2, x3, y3);
}


/* Draw callback implementations */

స్థిర శూన్య
hb_raster_move_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
		   శూన్య *draw_దత్తాంశం,
		   hb_draw_state_t *st HB_వాడని,
		   దశాంశం గమ్యం_x HB_వాడని, దశాంశం గమ్యం_y HB_వాడని,
		   శూన్య *user_దత్తాంశం HB_వాడని)
{
  /* no-op: state tracked by hb_draw_state_t */
}

స్థిర శూన్య
hb_raster_పంక్తి_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
		   శూన్య *draw_దత్తాంశం,
		   hb_draw_state_t *st,
		   దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
		   శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (draw->అంచులు_ఎడమ <= 0)) తిరిగి;

  దశాంశం tx0, ty0, tx1, ty1;
  transform_బిందువు (draw, st->current_x, st->current_y, tx0, ty0);
  transform_బిందువు (draw, గమ్యం_x,          గమ్యం_y,           tx1, ty1);
  emit_segment (draw, tx0, ty0, tx1, ty1);
}

స్థిర శూన్య
hb_raster_quadratic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
			శూన్య *draw_దత్తాంశం,
			hb_draw_state_t *st,
			దశాంశం control_x, దశాంశం control_y,
			దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (draw->అంచులు_ఎడమ <= 0)) తిరిగి;

  దశాంశం tx0, ty0, tx1, ty1, tx2, ty2;
  transform_బిందువు (draw, st->current_x, st->current_y, tx0, ty0);
  transform_బిందువు (draw, control_x,     control_y,      tx1, ty1);
  transform_బిందువు (draw, గమ్యం_x,          గమ్యం_y,           tx2, ty2);
  flatten_quadratic (draw, tx0, ty0, tx1, ty1, tx2, ty2);
}

స్థిర శూన్య
hb_raster_cubic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
		    శూన్య *draw_దత్తాంశం,
		    hb_draw_state_t *st,
		    దశాంశం control1_x, దశాంశం control1_y,
		    దశాంశం control2_x, దశాంశం control2_y,
		    దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
		    శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (draw->అంచులు_ఎడమ <= 0)) తిరిగి;

  దశాంశం tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3;
  transform_బిందువు (draw, st->current_x, st->current_y, tx0, ty0);
  transform_బిందువు (draw, control1_x,    control1_y,     tx1, ty1);
  transform_బిందువు (draw, control2_x,    control2_y,     tx2, ty2);
  transform_బిందువు (draw, గమ్యం_x,          గమ్యం_y,           tx3, ty3);
  flatten_cubic (draw, tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3);
}

స్థిర శూన్య
hb_raster_close_మార్గం (hb_draw_funcs_t *dfuncs HB_వాడని,
		      శూన్య *draw_దత్తాంశం HB_వాడని,
		      hb_draw_state_t *st HB_వాడని,
		      శూన్య *user_దత్తాంశం HB_వాడని)
{
  /* no-op: hb_draw_funcs_t already emits closing line_to before us */
}

స్థిర hb_bool_t
hb_raster_draw_సమితి_budget (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం,
			   int64_t budget, శూన్య *)
{
  స్వయంచాలక *draw = (hb_raster_draw_t *) draw_దత్తాంశం;
  draw->budget = budget;
  draw->recharge_budget ();
  తిరిగి నిజం;
}

స్థిర int64_t
hb_raster_draw_get_budget (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_raster_draw_t *) draw_దత్తాంశం)->budget;
}

స్థిర int64_t *
hb_raster_draw_get_budget_మిగిలిన (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_raster_draw_t *) draw_దత్తాంశం)->get_budget_మిగిలిన ();
}


/* Lazy-loader singleton for draw funcs */

స్థిర అంతఃవరుస శూన్య free_static_raster_draw_funcs ();

స్థిర నిర్మాణం hb_raster_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_raster_draw_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_సమితి_move_గమ్యం_func      (funcs, hb_raster_move_గమ్యం,      శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func      (funcs, hb_raster_పంక్తి_గమ్యం,      శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_quadratic_గమ్యం_func (funcs, hb_raster_quadratic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_cubic_గమ్యం_func     (funcs, hb_raster_cubic_గమ్యం,     శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_close_మార్గం_func   (funcs, hb_raster_close_మార్గం,   శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_సమితి_budget_func (funcs, hb_raster_draw_సమితి_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_func (funcs, hb_raster_draw_get_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_raster_draw_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_raster_draw_funcs);

    తిరిగి funcs;
  }
} static_raster_draw_funcs;

స్థిర అంతఃవరుస శూన్య
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
hb_raster_draw_get_funcs (స్థిరరాశి hb_raster_draw_t *draw HB_వాడని)
{
  తిరిగి static_raster_draw_funcs.get_unconst ();
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
  తిరిగి hb_font_draw_glyph_or_fail (font, glyph,
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
శూన్య
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
స్థిర constexpr గుర్తులేని HB_RASTER_NARROW_MAX_చురుకైన_అంచులు =
  INT16_MAX / HB_RASTER_ONE_PIXEL;

/* Add one edge piece's area/cover into a single cell. */
మూస <రకం_పేరు Area, రకం_పేరు Cover>
స్థిర HB_ALWAYS_INLINE శూన్య
cell_కూడు (Area *area, Cover *cover, గుర్తులేని వెడల్పు, పూర్ణాంకం నిలువు,
	  int32_t fx0, int32_t fy0, int32_t fx1, int32_t fy1, int32_t wind,
	  గుర్తులేని &x_min, గుర్తులేని &x_max)
{
  ఒకవేళ (unlikely ((గుర్తులేని) నిలువు >= వెడల్పు))
  {
    ఒకవేళ (unlikely (నిలువు < 0))
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
    తిరిగి;
  }
  int32_t dy = fy1 - fy0;
  area[నిలువు]  += (Area) (fx0 + fx1) * dy * wind;
  cover[నిలువు] += (Cover) (dy * wind);
  x_min = hb_min (x_min, (గుర్తులేని) నిలువు);
  x_max = hb_max (x_max, (గుర్తులేని) నిలువు);
}

/* Walk one edge through the pixel cells of a single pixel row,
   accumulating area/cover.  py is the integer pixel-row index. */
మూస <రకం_పేరు Area, రకం_పేరు Cover>
స్థిర HB_ALWAYS_INLINE శూన్య
అంచు_sweep_అడ్డువరుస (Area                   *area,
		Cover                  *cover,
		గుర్తులేని                వెడల్పు,
		పూర్ణాంకం                     x_org,
		int32_t                 y_శిఖరం,
		స్థిరరాశి hb_raster_అంచు_t &అంచు,
		గుర్తులేని               &x_min,
		గుర్తులేని               &x_max)
{
  /* Saturate: y_top can be within ONE_PIXEL of INT32_MAX when the surface
   * sits at the extreme of the fixed-point coordinate space.  Edges are
   * int32-clamped too, so no coverage exists beyond INT32_MAX anyway. */
  int32_t y_bot = (int32_t) hb_min ((int64_t) y_శిఖరం + HB_RASTER_ONE_PIXEL, (int64_t) INT32_MAX);

  int32_t ey0 = hb_max (అంచు.yL, y_శిఖరం);
  int32_t ey1 = hb_min (అంచు.yH, y_bot);
  ఒకవేళ (ey0 >= ey1) తిరిగి;

  /* X at clipped endpoints (fixed-point). Keep the interpolation in 64-bit
   * so extreme y values do not overflow before the slope multiply. */
  int64_t x0_64 = (int64_t) అంచు.xL +
		  ((((int64_t) ey0 - (int64_t) అంచు.yL) * అంచు.వాలు) >> 16);
  int64_t x1_64 = (int64_t) అంచు.xL +
		  ((((int64_t) ey1 - (int64_t) అంచు.yL) * అంచు.వాలు) >> 16);
  int32_t x0 = (int32_t) hb_clamp (x0_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);
  int32_t x1 = (int32_t) hb_clamp (x1_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);

  /* Fractional y within this pixel row [0, ONE_PIXEL] */
  int32_t fy0 = ey0 - y_శిఖరం;
  int32_t fy1 = ey1 - y_శిఖరం;

  int32_t cx0 = x0 >> HB_RASTER_PIXEL_బిట్లు;
  int32_t fx0 = x0 & HB_RASTER_PIXEL_ముసుగు;
  int32_t cx1 = x1 >> HB_RASTER_PIXEL_బిట్లు;
  int32_t fx1 = x1 & HB_RASTER_PIXEL_ముసుగు;
  int32_t wind = అంచు.wind;

  /* Fast path: both endpoints in the same pixel column. */
  ఒకవేళ (cx0 == cx1)
  {
    cell_కూడు (area, cover, వెడల్పు, cx0 - x_org, fx0, fy0, fx1, fy1, wind, x_min, x_max);
    తిరిగి;
  }

  int64_t మొత్తం_dx = (int64_t) x1 - (int64_t) x0;
  int64_t మొత్తం_dy = (int64_t) fy1 - (int64_t) fy0;

  /* fy increment per pixel column (constant since x_b advances by ONE_PIXEL). */
  int32_t delta_fy = (int32_t) ((int64_t) HB_RASTER_ONE_PIXEL * మొత్తం_dy / మొత్తం_dx);

  ఒకవేళ (మొత్తం_dx > 0)
  {
    /* Left-to-right edge. */
    ఒకవేళ (likely ((గుర్తులేని) (cx0 - x_org) < వెడల్పు && (గుర్తులేని) (cx1 - x_org) < వెడల్పు))
    {
      /* Entirely inside the surface: unclipped walk. */
      int32_t x_b = (int32_t) hb_clamp (((int64_t) cx0 + 1) * HB_RASTER_ONE_PIXEL,
					(int64_t) INT32_MIN, (int64_t) INT32_MAX);
      int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * మొత్తం_dy) / మొత్తం_dx);
      cell_కూడు (area, cover, వెడల్పు, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);

      int32_t fy_prev = fy_b;
      కొరకు (int32_t cx = cx0 + 1; cx < cx1; cx++)
      {
	fy_b = fy_prev + delta_fy;
	cell_కూడు (area, cover, వెడల్పు, cx - x_org, 0, fy_prev, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
	fy_prev = fy_b;
      }

      cell_కూడు (area, cover, వెడల్పు, cx1 - x_org, 0, fy_prev, fx1, fy1, wind, x_min, x_max);
      తిరిగి;
    }

    /* Visible column window.  Columns left of it only contribute their
     * total cover to column 0 (see cell_add) and that total telescopes,
     * while columns right of it contribute nothing; so neither side is
     * walked cell by cell.  Skipping ahead in the fy accumulation is
     * exact: the increment is constant and the running sums are bounded,
     * so k steps equal one k·delta_fy jump. */
    int64_t నిలువు_min = (int64_t) x_org;
    int64_t నిలువు_max = (int64_t) x_org + (int64_t) వెడల్పు - 1;

    ఒకవేళ (unlikely (cx1 < నిలువు_min))
    {
      /* Entirely left of the surface. */
      cell_కూడు (area, cover, వెడల్పు, -1, 0, fy0, 0, fy1, wind, x_min, x_max);
      తిరిగి;
    }
    ఒకవేళ (unlikely (cx0 > నిలువు_max))
      తిరిగి; /* Entirely right of the surface. */

    int32_t x_b = (int32_t) hb_clamp (((int64_t) cx0 + 1) * HB_RASTER_ONE_PIXEL,
				      (int64_t) INT32_MIN, (int64_t) INT32_MAX);
    int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * మొత్తం_dy) / మొత్తం_dx);

    int32_t cx = cx0 + 1;
    int32_t fy_prev = fy_b;
    ఒకవేళ (likely (cx0 >= నిలువు_min))
      cell_కూడు (area, cover, వెడల్పు, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
    లేకపోతే
    {
      /* Fold the first cell and the mid columns left of the surface
       * into one column-0 cover update. */
      int32_t cx_skip = (int32_t) నిలువు_min; /* ≤ cx1, so it fits */
      fy_prev = fy_b + (int32_t) ((int64_t) (cx_skip - cx) * delta_fy);
      cell_కూడు (area, cover, వెడల్పు, -1, 0, fy0, 0, fy_prev, wind, x_min, x_max);
      cx = cx_skip;
    }

    int32_t cx_చివర = (int32_t) hb_min ((int64_t) cx1, నిలువు_max + 1);
    కొరకు (; cx < cx_చివర; cx++)
    {
      fy_b = fy_prev + delta_fy;
      cell_కూడు (area, cover, వెడల్పు, cx - x_org, 0, fy_prev, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
      fy_prev = fy_b;
    }

    ఒకవేళ (likely (cx1 <= నిలువు_max))
      cell_కూడు (area, cover, వెడల్పు, cx1 - x_org, 0, fy_prev, fx1, fy1, wind, x_min, x_max);
  }
  లేకపోతే
  {
    /* Right-to-left edge. */
    ఒకవేళ (likely ((గుర్తులేని) (cx0 - x_org) < వెడల్పు && (గుర్తులేని) (cx1 - x_org) < వెడల్పు))
    {
      /* Entirely inside the surface: unclipped walk. */
      int32_t x_b = (int32_t) hb_clamp ((int64_t) cx0 * HB_RASTER_ONE_PIXEL,
					(int64_t) INT32_MIN, (int64_t) INT32_MAX);
      int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * మొత్తం_dy) / మొత్తం_dx);
      cell_కూడు (area, cover, వెడల్పు, cx0 - x_org, fx0, fy0, 0, fy_b, wind, x_min, x_max);

      int32_t fy_prev = fy_b;
      కొరకు (int32_t cx = cx0 - 1; cx > cx1; cx--)
      {
	fy_b = fy_prev - delta_fy;
	cell_కూడు (area, cover, వెడల్పు, cx - x_org, HB_RASTER_ONE_PIXEL, fy_prev, 0, fy_b, wind, x_min, x_max);
	fy_prev = fy_b;
      }

      cell_కూడు (area, cover, వెడల్పు, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_prev, fx1, fy1, wind, x_min, x_max);
      తిరిగి;
    }

    int64_t నిలువు_min = (int64_t) x_org;
    int64_t నిలువు_max = (int64_t) x_org + (int64_t) వెడల్పు - 1;

    ఒకవేళ (unlikely (cx0 < నిలువు_min))
    {
      /* Entirely left of the surface. */
      cell_కూడు (area, cover, వెడల్పు, -1, 0, fy0, 0, fy1, wind, x_min, x_max);
      తిరిగి;
    }
    ఒకవేళ (unlikely (cx1 > నిలువు_max))
      తిరిగి; /* Entirely right of the surface. */

    int32_t x_b = (int32_t) hb_clamp ((int64_t) cx0 * HB_RASTER_ONE_PIXEL,
				      (int64_t) INT32_MIN, (int64_t) INT32_MAX);
    int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * మొత్తం_dy) / మొత్తం_dx);
    cell_కూడు (area, cover, వెడల్పు, cx0 - x_org, fx0, fy0, 0, fy_b, wind, x_min, x_max);

    int32_t cx = cx0 - 1;
    int32_t fy_prev = fy_b;
    ఒకవేళ (unlikely (cx > నిలువు_max))
    {
      /* Mid columns right of the surface contribute nothing. */
      int32_t cx_skip = (int32_t) నిలువు_max; /* ≥ cx1, so it fits */
      fy_prev = fy_b - (int32_t) ((int64_t) (cx - cx_skip) * delta_fy);
      cx = cx_skip;
    }

    int32_t cx_stop = (int32_t) hb_max ((int64_t) cx1, నిలువు_min - 1);
    కొరకు (; cx > cx_stop; cx--)
    {
      fy_b = fy_prev - delta_fy;
      cell_కూడు (area, cover, వెడల్పు, cx - x_org, HB_RASTER_ONE_PIXEL, fy_prev, 0, fy_b, wind, x_min, x_max);
      fy_prev = fy_b;
    }

    ఒకవేళ (likely (cx1 >= నిలువు_min))
      cell_కూడు (area, cover, వెడల్పు, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_prev, fx1, fy1, wind, x_min, x_max);
    లేకపోతే
      /* Fold the mid columns left of the surface and the last cell
       * into one column-0 cover update. */
      cell_కూడు (area, cover, వెడల్పు, -1, 0, fy_prev, 0, fy1, wind, x_min, x_max);
  }
}

/* Convert cover-delta + area to alpha bytes, then clear.
   Returns final cover accumulator over [x_min, x_max]. */
స్థిర int32_t
sweep_అడ్డువరుస_గమ్యం_alpha (uint8_t *__restrict అడ్డువరుస_buf,
		    int32_t *__restrict area,
		    int16_t *__restrict cover,
		    గుర్తులేని x_min,
		    గుర్తులేని x_max)
{
  స్థిరరాశి int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
  int32_t cover_accum = 0;
  గుర్తులేని x = x_min;

#నిర్వచితమైతే HB_RASTER_NEON
  int32x4_t clamp_v = vdupq_n_s32 (HB_RASTER_నిండిన_COVERAGE);
  int32x4_t bias_v  = vdupq_n_s32 (HB_RASTER_నిండిన_COVERAGE / 2);
  int32x4_t zero32  = vdupq_n_s32 (0);
  int16x8_t zero16  = vdupq_n_s16 (0);
  కొరకు (; x + 7 <= x_max; x += 8)
  {
    int32_t ctmp[8];
    కొరకు (గుర్తులేని i = 0; i < 8; i++)
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

    int32x4_t r0 = vshrq_n_s32 (vmlaq_n_s32 (bias_v, v0, 255), 2 * HB_RASTER_PIXEL_బిట్లు + 1);
    int32x4_t r1 = vshrq_n_s32 (vmlaq_n_s32 (bias_v, v1, 255), 2 * HB_RASTER_PIXEL_బిట్లు + 1);

    int16x4_t h0 = vmovn_s32 (r0);
    int16x4_t h1 = vmovn_s32 (r1);
    int16x8_t h  = vcombine_s16 (h0, h1);
    uint8x8_t b  = vqmovun_s16 (h);
    vst1_u8 (అడ్డువరుస_buf + x, b);

    vst1q_s32 (area + x,     zero32);
    vst1q_s32 (area + x + 4, zero32);
    vst1q_s16 (cover + x,    zero16);
  }
#లేదావేళ defined(HB_RASTER_SSE2)
  __m128i clamp_v = _mm_set1_epi32 (HB_RASTER_నిండిన_COVERAGE);
  __m128i bias_v  = _mm_set1_epi32 (HB_RASTER_నిండిన_COVERAGE / 2);
  __m128i zero_v  = _mm_setzero_si128 ();
  కొరకు (; x + 7 <= x_max; x += 8)
  {
    int32_t ctmp[8];
    కొరకు (గుర్తులేని i = 0; i < 8; i++)
    {
      cover_accum += cover[x + i];
      ctmp[i] = cover_accum * cover_scale;
    }

    __m128i c0  = _mm_loadu_si128 ((__m128i *) (శూన్య *) (ctmp + 0));
    __m128i c1  = _mm_loadu_si128 ((__m128i *) (శూన్య *) (ctmp + 4));
    __m128i a0  = _mm_loadu_si128 ((__m128i *) (శూన్య *) (area + x));
    __m128i a1  = _mm_loadu_si128 ((__m128i *) (శూన్య *) (area + x + 4));

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

    __m128i r0 = _mm_srai_epi32 (_mm_add_epi32 (_mm_sub_epi32 (_mm_slli_epi32 (v0, 8), v0), bias_v), 2 * HB_RASTER_PIXEL_బిట్లు + 1);
    __m128i r1 = _mm_srai_epi32 (_mm_add_epi32 (_mm_sub_epi32 (_mm_slli_epi32 (v1, 8), v1), bias_v), 2 * HB_RASTER_PIXEL_బిట్లు + 1);

    __m128i h = _mm_packs_epi32 (r0, r1);
    __m128i b = _mm_packus_epi16 (h, h);
    _mm_storel_epi64 ((__m128i *) (శూన్య *) (అడ్డువరుస_buf + x), b);

    _mm_storeu_si128 ((__m128i *) (శూన్య *) (area + x),     zero_v);
    _mm_storeu_si128 ((__m128i *) (శూన్య *) (area + x + 4), zero_v);
    _mm_storeu_si128 ((__m128i *) (శూన్య *) (cover + x),    zero_v);
  }
#అయితే_ముగింపు

  కొరకు (; x <= x_max; x++)
  {
    cover_accum += cover[x];
    int32_t val   = cover_accum * cover_scale - area[x];
    int32_t alpha = val < 0 ? -val : val;
    ఒకవేళ (alpha > HB_RASTER_నిండిన_COVERAGE) alpha = HB_RASTER_నిండిన_COVERAGE;
    అడ్డువరుస_buf[x] = (uint8_t) (((గుర్తులేని) alpha * 255 + HB_RASTER_నిండిన_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_బిట్లు + 1));
    area[x]  = 0;
    cover[x] = 0;
  }

  తిరిగి cover_accum;
}

స్థిర int64_t
sweep_అడ్డువరుస_గమ్యం_alpha_wide (uint8_t *__restrict అడ్డువరుస_buf,
			 int64_t *__restrict area,
			 int64_t *__restrict cover,
			 గుర్తులేని x_min,
			 గుర్తులేని x_max)
{
  స్థిరరాశి int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
  int64_t cover_accum = 0;

  కొరకు (గుర్తులేని x = x_min; x <= x_max; x++)
  {
    cover_accum += cover[x];
    int64_t val = cover_accum * cover_scale - area[x];
    int64_t alpha = val < 0 ? -val : val;
    ఒకవేళ (alpha > HB_RASTER_నిండిన_COVERAGE) alpha = HB_RASTER_నిండిన_COVERAGE;
    అడ్డువరుస_buf[x] = (uint8_t) (((గుర్తులేని) alpha * 255 + HB_RASTER_నిండిన_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_బిట్లు + 1));
    area[x] = 0;
    cover[x] = 0;
  }

  తిరిగి cover_accum;
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

  ఒకవేళ (draw->has_extents)
  {
    ext = draw->fixed_extents;
  }
  లేకపోతే
  {
    /* Auto-size from edge bounding box */
    ఒకవేళ (draw->అంచులు.నిడివి == 0)
    {
      /* No edges: produce 0×0 image */
      ext = { 0, 0, 0, 0, 0 };
    }
    లేకపోతే
    {
      int32_t xmin = draw->అంచులు.arrayZ[0].xL, xmax = draw->అంచులు.arrayZ[0].xL;
      int32_t ymin = draw->అంచులు.arrayZ[0].yL, ymax = draw->అంచులు.arrayZ[0].yH;

      కొరకు (స్థిరరాశి స్వయంచాలక &e : draw->అంచులు)
      {
	xmin = hb_min (xmin, hb_min (e.xL, e.xH));
	xmax = hb_max (xmax, hb_max (e.xL, e.xH));
	ymin = hb_min (ymin, e.yL);
	ymax = hb_max (ymax, e.yH);
      }

      /* Convert fixed-point → pixels (floor for min, ceil for max).  Edge
	 coordinates are saturated to int32 range in emit_segment, so the
	 +MASK ceil step must be widened to avoid signed overflow. */
      పూర్ణాంకం x0 = xmin >> HB_RASTER_PIXEL_బిట్లు;
      పూర్ణాంకం y0 = ymin >> HB_RASTER_PIXEL_బిట్లు;
      పూర్ణాంకం x1 = (పూర్ణాంకం) (((int64_t) xmax + HB_RASTER_PIXEL_ముసుగు) >> HB_RASTER_PIXEL_బిట్లు);
      పూర్ణాంకం y1 = (పూర్ణాంకం) (((int64_t) ymax + HB_RASTER_PIXEL_ముసుగు) >> HB_RASTER_PIXEL_బిట్లు);

      ext.x_origin = x0;
      ext.y_origin = y0;
      ext.వెడల్పు    = (గుర్తులేని) hb_min (hb_max (0, x1 - x0), HB_RASTER_MAX_AUTO_DIMENSION);
      ext.ఎత్తు   = (గుర్తులేని) hb_min (hb_max (0, y1 - y0), HB_RASTER_MAX_AUTO_DIMENSION);
      ext.stride   = 0; /* filled below */
    }
  }

  /* ── 2. Compute stride ─────────────────────────────────────────── */
  ఒకవేళ (ext.stride == 0)
    ext.stride = (ext.వెడల్పు + 3u) & ~3u;

  /* ── 3. Allocate or reuse image ─────────────────────────────────── */
  /* Reset one-shot state on every exit path. */
  HB_SCOPE_GUARD (hb_raster_draw_clear (draw));

  hb_unique_ptr_t<hb_raster_image_t> image;
  ఒకవేళ (draw->recycled_image)
  {
    image = hb_unique_ptr_t<hb_raster_image_t> (draw->recycled_image);
    draw->recycled_image = శూన్యసూచిక;
  }
  లేకపోతే
  {
    image = hb_unique_ptr_t<hb_raster_image_t> (hb_raster_image_create_or_fail ());
    ఒకవేళ (unlikely (!image)) తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (unlikely (!image->configure (HB_RASTER_FORMAT_A8, ext)))
    తిరిగి శూన్యసూచిక;
  image->clear ();

  /* ── 4. Bucket edges by starting row and rasterize scanlines ──── */
  ఒకవేళ (draw->అంచులు.నిడివి && ext.వెడల్పు && ext.ఎత్తు)
  {
    ఒకవేళ (unlikely (!draw->అడ్డువరుస_area.resize_dirty (ext.వెడల్పు) ||
		  !draw->అడ్డువరుస_cover.resize_dirty (ext.వెడల్పు)))
      తిరిగి శూన్యసూచిక;
    hb_memset (draw->అడ్డువరుస_area.arrayZ,  0, ext.వెడల్పు * పరిమాణం (int32_t));
    hb_memset (draw->అడ్డువరుస_cover.arrayZ, 0, ext.వెడల్పు * పరిమాణం (int16_t));

    /* Bucket edges by their starting pixel row.
       Only grow the outer vector; clear inner vectors without freeing. */
    గుర్తులేని old_buckets = draw->అంచు_buckets.నిడివి;
    ఒకవేళ (ext.ఎత్తు > old_buckets)
    {
      ఒకవేళ (unlikely (!draw->అంచు_buckets.resize (ext.ఎత్తు)))
	తిరిగి శూన్యసూచిక;
    }
    కొరకు (గుర్తులేని i = 0; i < hb_min (ext.ఎత్తు, old_buckets); i++)
      draw->అంచు_buckets.arrayZ[i].clear ();
    /* New buckets (if any) are already empty from resize's zero-init. */

    కొరకు (గుర్తులేని i = 0; i < draw->అంచులు.నిడివి; i++)
    {
      పూర్ణాంకం అడ్డువరుస = (draw->అంచులు.arrayZ[i].yL >> HB_RASTER_PIXEL_బిట్లు) - ext.y_origin;
      ఒకవేళ (అడ్డువరుస < 0) అడ్డువరుస = 0;
      ఒకవేళ ((గుర్తులేని) అడ్డువరుస >= ext.ఎత్తు) కొనసాగు;
      draw->అంచు_buckets.arrayZ[అడ్డువరుస].push (i);
    }

    /* Scanline loop with active edge list. */
    draw->చురుకైన_అంచులు.clear ();
    తార్కిక wide_initialized = అబద్ధం;

    కొరకు (గుర్తులేని అడ్డువరుస = 0; అడ్డువరుస < ext.ఎత్తు; అడ్డువరుస++)
    {
      int64_t y_శిఖరం_64 = ((int64_t) ext.y_origin + (int64_t) అడ్డువరుస) * HB_RASTER_ONE_PIXEL;
      int32_t y_శిఖరం = (int32_t) hb_clamp (y_శిఖరం_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);

      /* Add new edges from this row's bucket. */
      draw->చురుకైన_అంచులు.extend (draw->అంచు_buckets.arrayZ[అడ్డువరుస]);

      /* Process active edges and compact live ones in one linear pass. */
      గుర్తులేని x_min = ext.వెడల్పు, x_max = 0;
      గుర్తులేని లిఖ = 0;
      గుర్తులేని చురుకైన_len = draw->చురుకైన_అంచులు.నిడివి;
      తార్కిక use_wide = చురుకైన_len > HB_RASTER_NARROW_MAX_చురుకైన_అంచులు;
      ఒకవేళ (unlikely (use_wide && !wide_initialized))
      {
	ఒకవేళ (unlikely (!draw->అడ్డువరుస_area_wide.resize_dirty (ext.వెడల్పు) ||
		      !draw->అడ్డువరుస_cover_wide.resize_dirty (ext.వెడల్పు)))
	  తిరిగి శూన్యసూచిక;
	hb_memset (draw->అడ్డువరుస_area_wide.arrayZ,  0, ext.వెడల్పు * పరిమాణం (int64_t));
	hb_memset (draw->అడ్డువరుస_cover_wide.arrayZ, 0, ext.వెడల్పు * పరిమాణం (int64_t));
	wide_initialized = నిజం;
      }
      కొరకు (గుర్తులేని j = 0; j < చురుకైన_len; j++)
      {
	గుర్తులేని అంచు_idx = draw->చురుకైన_అంచులు.arrayZ[j];
	స్థిరరాశి స్వయంచాలక &e = draw->అంచులు.arrayZ[అంచు_idx];
	ఒకవేళ (e.yH <= y_శిఖరం)
	  కొనసాగు;

	ఒకవేళ (unlikely (use_wide))
	  అంచు_sweep_అడ్డువరుస (draw->అడ్డువరుస_area_wide.arrayZ, draw->అడ్డువరుస_cover_wide.arrayZ,
			  ext.వెడల్పు, ext.x_origin, y_శిఖరం, e, x_min, x_max);
	లేకపోతే
	  అంచు_sweep_అడ్డువరుస (draw->అడ్డువరుస_area.arrayZ, draw->అడ్డువరుస_cover.arrayZ,
			  ext.వెడల్పు, ext.x_origin, y_శిఖరం, e, x_min, x_max);
	draw->చురుకైన_అంచులు.arrayZ[లిఖ++] = అంచు_idx;
      }
      draw->చురుకైన_అంచులు.resize (లిఖ);

      ఒకవేళ (x_min <= x_max)
      {
	int64_t cover_accum = use_wide ?
	  sweep_అడ్డువరుస_గమ్యం_alpha_wide (image->బఫరు.arrayZ + అడ్డువరుస * ext.stride,
				   draw->అడ్డువరుస_area_wide.arrayZ, draw->అడ్డువరుస_cover_wide.arrayZ,
				   x_min, x_max) :
	  sweep_అడ్డువరుస_గమ్యం_alpha (image->బఫరు.arrayZ + అడ్డువరుస * ext.stride,
			      draw->అడ్డువరుస_area.arrayZ, draw->అడ్డువరుస_cover.arrayZ,
			      x_min, x_max);

	/* If cover doesn't cancel, memset the constant-alpha tail. */
	ఒకవేళ (cover_accum != 0)
	{
	  int64_t alpha = cover_accum * (2 * HB_RASTER_ONE_PIXEL);
	  alpha = alpha < 0 ? -alpha : alpha;
	  ఒకవేళ (alpha > HB_RASTER_నిండిన_COVERAGE) alpha = HB_RASTER_నిండిన_COVERAGE;
	  uint8_t byte = (uint8_t) (((గుర్తులేని) alpha * 255 + HB_RASTER_నిండిన_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_బిట్లు + 1));

	  uint8_t *అడ్డువరుస_buf = image->బఫరు.arrayZ + అడ్డువరుస * ext.stride;
	  hb_memset (అడ్డువరుస_buf + x_max + 1, byte, ext.వెడల్పు - 1 - x_max);
	}
      }
    }
  }

  తిరిగి image.release ();
}
