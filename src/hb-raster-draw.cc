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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-raster-image.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-geometry.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"

#ਜੇ defined(__aarch64__) || defined(_M_ARM64)
#ਅਨ੍ਤਰ੍ਭਾਵ <arm_neon.h>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_NEON 1
#ਜਾਂਜੇ defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64)
#ਅਨ੍ਤਰ੍ਭਾਵ <emmintrin.h>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_SSE2 1
#ਜੇ_ਅੰਤ


/* Fixed-point precision for sub-pixel coordinates.
   8 bits = 24.8: 256 sub-pixel units per pixel. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_PIXEL_ਬਿੱਟ 8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_ONE_PIXEL  (1 << HB_RASTER_PIXEL_ਬਿੱਟ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_PIXEL_ਨਕਾਬ (HB_RASTER_ONE_PIXEL - 1)
/* Full-coverage alpha = 2 * ONE_PIXEL^2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_ਭਰਿਆ_COVERAGE (2 * HB_RASTER_ONE_PIXEL * HB_RASTER_ONE_PIXEL)
/* Flatness threshold for Bézier flattening: max deviation in pixels */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_FLAT_THRESH 0.25f


/* Normalized edge: yH > yL always */
ਬਣਤਰ hb_raster_ਕਿਨਾਰਾ_t
{
  int32_t xL, yL;   /* lower endpoint (fixed-point) */
  int32_t xH, yH;   /* upper endpoint (fixed-point) */
  int64_t ਢਲਾਨ;    /* dx/dy in 16.16 fixed point: ((int64_t)dx << 16) / dy */
  int32_t wind;     /* +1 or -1 */
};

/* hb_raster_draw_t — outline rasterizer */
ਬਣਤਰ hb_raster_draw_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  /* Configuration */
  hb_transform_t<>    transform         = {1, 0, 0, 1, 0, 0};
  ਦਸ਼ਮਲਵ               x_scale_ਗੁਣਕ    = 1.f;
  ਦਸ਼ਮਲਵ               y_scale_ਗੁਣਕ    = 1.f;
  hb_raster_extents_t fixed_extents     = {};
  ਤਾਰਕਿਕ                has_extents = ਝੂਠ;

  /* Visibility clip box for curve flattening (device pixels); set
     internally by raster-paint so invisible curves collapse to their
     chord.  See hb_raster_draw_set_clip_box(). */
  ਤਾਰਕਿਕ  has_clip_box = ਝੂਠ;
  ਦਸ਼ਮਲਵ clip_x0 = 0.f, clip_y0 = 0.f, clip_x1 = 0.f, clip_y1 = 0.f;

  /* Flattening clip resolved from the above (or from fixed extents),
     expanded by one pixel; kept in sync by hb_raster_draw_update_flatten_clip(). */
  ਤਾਰਕਿਕ  flatten_clip_ਸਰਗਰਮ = ਝੂਠ;
  ਦਸ਼ਮਲਵ flatten_clip_x0 = 0.f, flatten_clip_y0 = 0.f;
  ਦਸ਼ਮਲਵ flatten_clip_x1 = 0.f, flatten_clip_y1 = 0.f;

  /* Work budget for outline traversal and curve flattening.  A paint
     backend seeds this through the public draw-budget API before drawing
     and reads the remainder back afterwards, so a whole paint session
     shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_ਬਚਿਆ = HB_BUDGET_GLYPH;

  ਖਾਲੀ recharge_budget ()
  {
    budget_ਬਚਿਆ = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_ਬਚਿਆ ()
  { ਵਾਪਸ &budget_ਬਚਿਆ; }

  /* Accumulated geometry */
  int64_t ਕਿਨਾਰੇ_ਖੱਬਾ = HB_RASTER_MAX_DRAW_ਕਿਨਾਰੇ;
  hb_ਸਦਿਸ਼_t<hb_raster_ਕਿਨਾਰਾ_t> ਕਿਨਾਰੇ;

  /* Scratch — reused across render() calls */
  hb_ਸਦਿਸ਼_t<int32_t> ਪੰਕਤੀ_area;
  hb_ਸਦਿਸ਼_t<int16_t> ਪੰਕਤੀ_cover;
  hb_ਸਦਿਸ਼_t<int64_t> ਪੰਕਤੀ_area_wide;
  hb_ਸਦਿਸ਼_t<int64_t> ਪੰਕਤੀ_cover_wide;
  hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> ਕਿਨਾਰਾ_buckets;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਸਰਗਰਮ_ਕਿਨਾਰੇ;

  /* Recycled image for zero-malloc render */
  hb_raster_image_t *recycled_image = ਖਾਲੀ_ਸੂਚਕ;
};

ਸਥਿਰ HB_ALWAYS_INLINE ਖਾਲੀ
hb_raster_draw_transform_ਬਿੰਦੂ (ਅਚਲ hb_raster_draw_t *draw,
				ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y,
				ਦਸ਼ਮਲਵ &tx, ਦਸ਼ਮਲਵ &ty)
{
  tx = x; ty = y;
  draw->transform.transform_ਬਿੰਦੂ (tx, ty);
  tx /= draw->x_scale_ਗੁਣਕ;
  ty /= draw->y_scale_ਗੁਣਕ;
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
hb_raster_draw_create_or_fail (ਖਾਲੀ)
{
  hb_raster_draw_t *draw = hb_object_create<hb_raster_draw_t> ();
  ਵਾਪਸ draw;
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
  ਵਾਪਸ hb_object_reference (draw);
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
ਖਾਲੀ
hb_raster_draw_destroy (hb_raster_draw_t *draw)
{
  ਜੇ (!hb_object_should_destroy (draw))
    ਵਾਪਸ;

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
hb_raster_draw_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_raster_draw_t   *draw,
			      hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			      ਖਾਲੀ               *ਅੰਕੜੇ,
			      hb_destroy_func_t   destroy,
			      hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (draw, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_raster_draw_get_user_ਅੰਕੜੇ (ਅਚਲ hb_raster_draw_t   *draw,
			      hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (draw, key);
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
ਖਾਲੀ
hb_raster_draw_ਸੈੱਟ_transform (hb_raster_draw_t *draw,
			      ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
			      ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
			      ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy)
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
ਖਾਲੀ
hb_raster_draw_ਸੈੱਟ_scale_ਗੁਣਕ (hb_raster_draw_t *draw,
				 ਦਸ਼ਮਲਵ x_scale_ਗੁਣਕ,
				 ਦਸ਼ਮਲਵ y_scale_ਗੁਣਕ)
{
  draw->x_scale_ਗੁਣਕ = x_scale_ਗੁਣਕ > 0.f ? x_scale_ਗੁਣਕ : 1.f;
  draw->y_scale_ਗੁਣਕ = y_scale_ਗੁਣਕ > 0.f ? y_scale_ਗੁਣਕ : 1.f;
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
ਖਾਲੀ
hb_raster_draw_get_scale_ਗੁਣਕ (ਅਚਲ hb_raster_draw_t *draw,
				 ਦਸ਼ਮਲਵ *x_scale_ਗੁਣਕ,
				 ਦਸ਼ਮਲਵ *y_scale_ਗੁਣਕ)
{
  ਜੇ (x_scale_ਗੁਣਕ) *x_scale_ਗੁਣਕ = draw->x_scale_ਗੁਣਕ;
  ਜੇ (y_scale_ਗੁਣਕ) *y_scale_ਗੁਣਕ = draw->y_scale_ਗੁਣਕ;
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
ਖਾਲੀ
hb_raster_draw_get_transform (ਅਚਲ hb_raster_draw_t *draw,
			      ਦਸ਼ਮਲਵ *xx, ਦਸ਼ਮਲਵ *yx,
			      ਦਸ਼ਮਲਵ *xy, ਦਸ਼ਮਲਵ *yy,
			      ਦਸ਼ਮਲਵ *dx, ਦਸ਼ਮਲਵ *dy)
{
  ਜੇ (xx) *xx = draw->transform.xx;
  ਜੇ (yx) *yx = draw->transform.yx;
  ਜੇ (xy) *xy = draw->transform.xy;
  ਜੇ (yy) *yy = draw->transform.yy;
  ਜੇ (dx) *dx = draw->transform.x0;
  ਜੇ (dy) *dy = draw->transform.y0;
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
ਸਥਿਰ ਖਾਲੀ
hb_raster_draw_update_flatten_clip (hb_raster_draw_t *draw)
{
  ਜੇ (draw->has_clip_box)
  {
    draw->flatten_clip_ਸਰਗਰਮ = ਸੱਚ;
    draw->flatten_clip_x0 = draw->clip_x0 - 1.f;
    draw->flatten_clip_y0 = draw->clip_y0 - 1.f;
    draw->flatten_clip_x1 = draw->clip_x1 + 1.f;
    draw->flatten_clip_y1 = draw->clip_y1 + 1.f;
  }
  ਨਹੀਂਤਾਂ ਜੇ (draw->has_extents)
  {
    draw->flatten_clip_ਸਰਗਰਮ = ਸੱਚ;
    draw->flatten_clip_x0 = (ਦਸ਼ਮਲਵ) draw->fixed_extents.x_origin - 1.f;
    draw->flatten_clip_y0 = (ਦਸ਼ਮਲਵ) draw->fixed_extents.y_origin - 1.f;
    draw->flatten_clip_x1 = (ਦਸ਼ਮਲਵ) draw->fixed_extents.x_origin + (ਦਸ਼ਮਲਵ) draw->fixed_extents.ਚੌੜਾਈ + 1.f;
    draw->flatten_clip_y1 = (ਦਸ਼ਮਲਵ) draw->fixed_extents.y_origin + (ਦਸ਼ਮਲਵ) draw->fixed_extents.ਉਚਾਈ + 1.f;
  }
  ਨਹੀਂਤਾਂ
    draw->flatten_clip_ਸਰਗਰਮ = ਝੂਠ;
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
ਖਾਲੀ
hb_raster_draw_ਸੈੱਟ_extents (hb_raster_draw_t          *draw,
			    ਅਚਲ hb_raster_extents_t *extents)
{
  draw->fixed_extents     = *extents;
  draw->has_extents = ਸੱਚ;
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
hb_raster_draw_get_extents (ਅਚਲ hb_raster_draw_t    *draw,
			    hb_raster_extents_t *extents)
{
  ਜੇ (!draw->has_extents)
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (extents)
    *extents = draw->fixed_extents;
  ਵਾਪਸ ਸੱਚ;
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
hb_raster_draw_ਸੈੱਟ_glyph_extents (hb_raster_draw_t         *draw,
				  ਅਚਲ hb_glyph_extents_t *glyph_extents)
{
  ਦਸ਼ਮਲਵ x0 = (ਦਸ਼ਮਲਵ) glyph_extents->x_bearing;
  ਦਸ਼ਮਲਵ y0 = (ਦਸ਼ਮਲਵ) glyph_extents->y_bearing;
  ਦਸ਼ਮਲਵ x1 = (ਦਸ਼ਮਲਵ) glyph_extents->x_bearing + glyph_extents->ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ y1 = (ਦਸ਼ਮਲਵ) glyph_extents->y_bearing + glyph_extents->ਉਚਾਈ;

  ਦਸ਼ਮਲਵ xmin = hb_min (x0, x1);
  ਦਸ਼ਮਲਵ xmax = hb_max (x0, x1);
  ਦਸ਼ਮਲਵ ymin = hb_min (y0, y1);
  ਦਸ਼ਮਲਵ ymax = hb_max (y0, y1);

  ਦਸ਼ਮਲਵ px[4] = {xmin, xmin, xmax, xmax};
  ਦਸ਼ਮਲਵ py[4] = {ymin, ymax, ymin, ymax};

  ਦਸ਼ਮਲਵ tx, ty;
  hb_raster_draw_transform_ਬਿੰਦੂ (draw, px[0], py[0], tx, ty);
  ਦਸ਼ਮਲਵ tx_min = tx, tx_max = tx;
  ਦਸ਼ਮਲਵ ty_min = ty, ty_max = ty;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < 4; i++)
  {
    hb_raster_draw_transform_ਬਿੰਦੂ (draw, px[i], py[i], tx, ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  int32_t ex0 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (floorf (tx_min));
  int32_t ey0 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (floorf (ty_min));
  int32_t ex1 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (ceilf  (tx_max));
  int32_t ey1 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (ceilf  (ty_max));

  ਜੇ (ex1 <= ex0 || ey1 <= ey0)
  {
    draw->fixed_extents = {};
    draw->has_extents = ਝੂਠ;
    hb_raster_draw_update_flatten_clip (draw);
    ਵਾਪਸ ਝੂਠ;
  }

  draw->fixed_extents = {
    ex0, ey0,
    (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  draw->has_extents = ਸੱਚ;
  hb_raster_draw_update_flatten_clip (draw);
  ਵਾਪਸ ਸੱਚ;
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
ਖਾਲੀ
hb_raster_draw_clear (hb_raster_draw_t *draw)
{
  draw->fixed_extents     = {};
  draw->has_extents = ਝੂਠ;
  draw->has_clip_box = ਝੂਠ;
  draw->flatten_clip_ਸਰਗਰਮ = ਝੂਠ;
  draw->recharge_budget ();
  draw->ਕਿਨਾਰੇ_ਖੱਬਾ = HB_RASTER_MAX_DRAW_ਕਿਨਾਰੇ;
  draw->ਕਿਨਾਰੇ.clear ();
  draw->ਸਰਗਰਮ_ਕਿਨਾਰੇ.clear ();
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
ਖਾਲੀ
hb_raster_draw_reset (hb_raster_draw_t *draw)
{
  draw->transform         = {1, 0, 0, 1, 0, 0};
  draw->x_scale_ਗੁਣਕ    = 1.f;
  draw->y_scale_ਗੁਣਕ    = 1.f;
  draw->budget            = HB_BUDGET_DEFAULT;
  hb_raster_draw_clear (draw);
}

ਖਾਲੀ
hb_raster_draw_ਸੈੱਟ_clip_box (hb_raster_draw_t *draw,
			     ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
			     ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1)
{
  draw->has_clip_box = ਸੱਚ;
  draw->clip_x0 = x0;
  draw->clip_y0 = y0;
  draw->clip_x1 = x1;
  draw->clip_y1 = y1;
  hb_raster_draw_update_flatten_clip (draw);
}

int64_t
hb_raster_draw_get_pixel_work (ਅਚਲ hb_raster_draw_t *draw,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_ਪੰਕਤੀਆਂ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_ਖਾਨੇ_ਗਿਣਤੀ)
{
  int64_t work = 0;
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਕਿਨਾਰਾ : draw->ਕਿਨਾਰੇ)
  {
    int64_t dx = (int64_t) ਕਿਨਾਰਾ.xH - ਕਿਨਾਰਾ.xL;
    ਜੇ (dx < 0) dx = -dx;
    work += 1 + hb_min (((int64_t) ਕਿਨਾਰਾ.yH - ਕਿਨਾਰਾ.yL) >> HB_RASTER_PIXEL_ਬਿੱਟ,
			(int64_t) max_ਪੰਕਤੀਆਂ)
	      + hb_min (dx >> HB_RASTER_PIXEL_ਬਿੱਟ,
			(int64_t) max_ਖਾਨੇ_ਗਿਣਤੀ);
  }
  ਵਾਪਸ work;
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
ਖਾਲੀ
hb_raster_draw_recycle_image (hb_raster_draw_t  *draw,
			      hb_raster_image_t *image)
{
  hb_raster_image_destroy (draw->recycled_image);
  draw->recycled_image = image;
}


/*
 * Draw callbacks — flatten on the fly into hb_raster_edge_t
 */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
transform_ਬਿੰਦੂ (ਅਚਲ hb_raster_draw_t *draw,
		 ਦਸ਼ਮਲਵ  x,  ਦਸ਼ਮਲਵ  y,
		 ਦਸ਼ਮਲਵ &tx, ਦਸ਼ਮਲਵ &ty)
{
  hb_raster_draw_transform_ਬਿੰਦੂ (draw, x, y, tx, ty);
}

ਸਥਿਰ ਖਾਲੀ
emit_segment (hb_raster_draw_t *draw,
	      ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
	      ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1)
{
  ਜੇ (unlikely (draw->ਕਿਨਾਰੇ_ਖੱਬਾ <= 0))
    ਵਾਪਸ;

  int32_t X0 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (roundf (x0 * HB_RASTER_ONE_PIXEL));
  int32_t Y0 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (roundf (y0 * HB_RASTER_ONE_PIXEL));
  int32_t X1 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (roundf (x1 * HB_RASTER_ONE_PIXEL));
  int32_t Y1 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (roundf (y1 * HB_RASTER_ONE_PIXEL));

  ਜੇ (Y0 == Y1) ਵਾਪਸ; /* horizontal — skip */

  hb_raster_ਕਿਨਾਰਾ_t e;
  ਜੇ (Y0 < Y1) {
    e.xL = X0; e.yL = Y0; e.xH = X1; e.yH = Y1; e.wind = +1;
  } ਨਹੀਂਤਾਂ {
    e.xL = X1; e.yL = Y1; e.xH = X0; e.yH = Y0; e.wind = -1;
  }
  e.ਢਲਾਨ = (((int64_t) e.xH - (int64_t) e.xL) * (int64_t) 65536) /
	    ((int64_t) e.yH - (int64_t) e.yL);

  ਜੇ (likely (draw->ਕਿਨਾਰੇ.ਧੱਕੋ_or_fail (e)))
    draw->ਕਿਨਾਰੇ_ਖੱਬਾ--;
  ਨਹੀਂਤਾਂ
    draw->ਕਿਨਾਰੇ_ਖੱਬਾ = 0;
}


/* Quadratic Bézier flattener — iterative de Casteljau at t=0.5. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
flatten_quadratic_recursive (hb_raster_draw_t *draw,
			     ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
			     ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
			     ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
			     ਪੂਰਨ ਡੂੰਘਾਈ = 0)
{
  ਬਣਤਰ quad_ਗੰਢ_t
  {
    ਦਸ਼ਮਲਵ x0, y0, x1, y1, x2, y2;
    ਪੂਰਨ ਡੂੰਘਾਈ;
    ਤਾਰਕਿਕ check_clip;
  };

  quad_ਗੰਢ_t ਢੇਰ[16];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਖਰ_2 = 0;

  ਤਾਰਕਿਕ check_clip = draw->flatten_clip_ਸਰਗਰਮ;
  int64_t *work = draw->get_budget_ਬਚਿਆ ();
  int64_t work_ਖੱਬਾ = *work;

  ਜਦੋਂ (ਸੱਚ)
  {
    ਤਾਰਕਿਕ emit_chord = ਡੂੰਘਾਈ >= 16;

    /* Entirely outside the clip box: the chord is exact for coverage.
       Subdivided control points are convex combinations of their
       parent's, so once a node is entirely inside the box no
       descendant can leave it and the test is skipped below. */
    ਜੇ (!emit_chord && check_clip)
    {
      ਦਸ਼ਮਲਵ bx0 = hb_min (x0, hb_min (x1, x2));
      ਦਸ਼ਮਲਵ bx1 = hb_max (x0, hb_max (x1, x2));
      ਦਸ਼ਮਲਵ by0 = hb_min (y0, hb_min (y1, y2));
      ਦਸ਼ਮਲਵ by1 = hb_max (y0, hb_max (y1, y2));
      ਜੇ (bx1 < draw->flatten_clip_x0 || bx0 > draw->flatten_clip_x1 ||
	  by1 < draw->flatten_clip_y0 || by0 > draw->flatten_clip_y1)
	emit_chord = ਸੱਚ;
      ਨਹੀਂਤਾਂ ਜੇ (bx0 >= draw->flatten_clip_x0 && bx1 <= draw->flatten_clip_x1 &&
	       by0 >= draw->flatten_clip_y0 && by1 <= draw->flatten_clip_y1)
	check_clip = ਝੂਠ;
    }

    ਜੇ (!emit_chord)
    {
      ਤਾਰਕਿਕ is_flat;
      ਜੇ (ਝੂਠ)
      {
	/* Old behavior: midpoint deviation from chord midpoint. */
	ਦਸ਼ਮਲਵ mx = x0 * 0.25f + x1 * 0.5f + x2 * 0.25f;
	ਦਸ਼ਮਲਵ my = y0 * 0.25f + y1 * 0.5f + y2 * 0.25f;
	ਦਸ਼ਮਲਵ chord_mx = (x0 + x2) * 0.5f;
	ਦਸ਼ਮਲਵ chord_my = (y0 + y2) * 0.5f;
	ਦਸ਼ਮਲਵ dx = mx - chord_mx;
	ਦਸ਼ਮਲਵ dy = my - chord_my;
	ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;
	is_flat = (dx * dx + dy * dy) <= flat_thresh;
      }
      ਨਹੀਂਤਾਂ
      {
	/* FreeType behavior: control-point deviation from chord center. */
	ਅਚਲ ਦਸ਼ਮਲਵ flat_thresh = 0.25f;
	ਦਸ਼ਮਲਵ dx = x0 + x2 - 2.f * x1;
	ਦਸ਼ਮਲਵ dy = y0 + y2 - 2.f * y1;
	ਜੇ (dx < 0) dx = -dx;
	ਜੇ (dy < 0) dy = -dy;
	is_flat = dx <= flat_thresh && dy <= flat_thresh;
      }
      emit_chord = is_flat;
    }

    /* Charge one unit per subdivision; degrade to the chord when the
       session work budget is spent. */
    ਜੇ (!emit_chord)
    {
      ਜੇ (unlikely (work_ਖੱਬਾ <= 0))
	emit_chord = ਸੱਚ;
      ਨਹੀਂਤਾਂ
	work_ਖੱਬਾ--;
    }

    ਜੇ (emit_chord)
    {
      emit_segment (draw, x0, y0, x2, y2);
      ਜੇ (!ਸਿਖਰ_2)
      {
	*work = work_ਖੱਬਾ;
	ਵਾਪਸ;
      }
      ਅਚਲ quad_ਗੰਢ_t &n = ਢੇਰ[--ਸਿਖਰ_2];
      x0 = n.x0; y0 = n.y0;
      x1 = n.x1; y1 = n.y1;
      x2 = n.x2; y2 = n.y2;
      ਡੂੰਘਾਈ = n.ਡੂੰਘਾਈ;
      check_clip = n.check_clip;
      ਜਾਰੀ;
    }

    ਦਸ਼ਮਲਵ x01 = (x0 + x1) * 0.5f, y01 = (y0 + y1) * 0.5f;
    ਦਸ਼ਮਲਵ x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
    ਦਸ਼ਮਲਵ xm  = (x01 + x12) * 0.5f, ym  = (y01 + y12) * 0.5f;

    /* Depth is capped at 16, so stack capacity 16 is sufficient. */
    ਢੇਰ[ਸਿਖਰ_2++] = {xm, ym, x12, y12, x2, y2, ਡੂੰਘਾਈ + 1, check_clip};
    x2 = xm; y2 = ym;
    x1 = x01; y1 = y01;
    ਡੂੰਘਾਈ++;
  }
}

/* Quadratic Bézier flattener using forward differencing.
   The error (midpoint deviation) shrinks exactly 4× per de Casteljau
   subdivision, so we compute the subdivision count upfront and iterate
   with constant-cost additions instead of recursive branching. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
flatten_quadratic_fd (hb_raster_draw_t *draw,
		      ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
		      ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
		      ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2)
{
  /* Deviation of curve midpoint from chord midpoint (squared). */
  ਦਸ਼ਮਲਵ devx = (x0 - 2 * x1 + x2) * 0.25f;
  ਦਸ਼ਮਲਵ devy = (y0 - 2 * y1 + y2) * 0.25f;
  ਦਸ਼ਮਲਵ err2 = devx * devx + devy * devy;

  ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;

  ਜੇ (err2 <= flat_thresh)
  {
    emit_segment (draw, x0, y0, x2, y2);
    ਵਾਪਸ;
  }

  /* err² shrinks 16× per subdivision level.  Find n such that
     err2 / 16^n <= flat_thresh, i.e. n = ceil(log₁₆(err2/flat_thresh)). */
  ਬਿਨਾਨਿਸ਼ਾਨੀ n = 1;
  {
    ਦਸ਼ਮਲਵ ਅਨੁਪਾਤ = err2 / flat_thresh;
    ਜਦੋਂ (ਅਨੁਪਾਤ > 16.f) { ਅਨੁਪਾਤ *= (1.f / 16.f); n++; }
    ਜੇ (n > 16) n = 16;
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ N = 1u << n;   /* number of line segments */
  ਦਸ਼ਮਲਵ h = 1.f / N;

  /* Quadratic: B(t) = a·t² + b·t + c
     Forward differences with step h:
       d²f = 2·a·h²   (constant)
       df₀ = a·h² + b·h
       f₀  = c = P₀                     */
  ਦਸ਼ਮਲਵ ax = x0 - 2 * x1 + x2;
  ਦਸ਼ਮਲਵ ay = y0 - 2 * y1 + y2;
  ਦਸ਼ਮਲਵ bx = 2 * (x1 - x0);
  ਦਸ਼ਮਲਵ by = 2 * (y1 - y0);

  ਦਸ਼ਮਲਵ d2fx = 2 * ax * h * h;
  ਦਸ਼ਮਲਵ d2fy = 2 * ay * h * h;
  ਦਸ਼ਮਲਵ dfx  = ax * h * h + bx * h;
  ਦਸ਼ਮਲਵ dfy  = ay * h * h + by * h;
  ਦਸ਼ਮਲਵ fx   = x0;
  ਦਸ਼ਮਲਵ fy   = y0;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < N; i++)
  {
    ਦਸ਼ਮਲਵ nx = fx + dfx;
    ਦਸ਼ਮਲਵ ny = fy + dfy;
    emit_segment (draw, fx, fy, nx, ny);
    fx = nx;
    fy = ny;
    dfx += d2fx;
    dfy += d2fy;
  }
  /* Last segment uses exact endpoint to avoid drift. */
  emit_segment (draw, fx, fy, x2, y2);
}

ਸਥਿਰ ਖਾਲੀ
flatten_quadratic (hb_raster_draw_t *draw,
		   ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
		   ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
		   ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2)
{
  ਜੇ (ਝੂਠ)
    flatten_quadratic_fd (draw, x0, y0, x1, y1, x2, y2);
  ਨਹੀਂਤਾਂ
    flatten_quadratic_recursive (draw, x0, y0, x1, y1, x2, y2);
}

/* For cubic B(t), the max deviation from its chord on [0,1] is bounded by:
     max||B(t)-L(t)|| <= max_t ||B''(t)|| / 8.
   B''(t) is linear, so max norm is attained at t=0 or t=1:
     B''(0)=6*(P0-2P1+P2), B''(1)=6*(P1-2P2+P3). */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
cubic_chord_error_bound2 (ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
			  ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
			  ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
			  ਦਸ਼ਮਲਵ x3, ਦਸ਼ਮਲਵ y3)
{
  ਦਸ਼ਮਲਵ d20x = x0 - 2 * x1 + x2;
  ਦਸ਼ਮਲਵ d20y = y0 - 2 * y1 + y2;
  ਦਸ਼ਮਲਵ d21x = x1 - 2 * x2 + x3;
  ਦਸ਼ਮਲਵ d21y = y1 - 2 * y2 + y3;
  ਦਸ਼ਮਲਵ m0 = d20x * d20x + d20y * d20y;
  ਦਸ਼ਮਲਵ m1 = d21x * d21x + d21y * d21y;
  ਦਸ਼ਮਲਵ m = m0 > m1 ? m0 : m1;
  /* (max||B''||/8)^2 = (6/8)^2 * max||d2||^2 = (3/4)^2 * m. */
  ਵਾਪਸ m * (9.f / 16.f);
}

/* Cubic Bézier flattener — iterative de Casteljau at t=0.5. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
flatten_cubic_recursive (hb_raster_draw_t *draw,
			 ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
			 ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
			 ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
			 ਦਸ਼ਮਲਵ x3, ਦਸ਼ਮਲਵ y3,
			 ਪੂਰਨ ਡੂੰਘਾਈ = 0)
{
  ਬਣਤਰ cubic_ਗੰਢ_t
  {
    ਦਸ਼ਮਲਵ x0, y0, x1, y1, x2, y2, x3, y3;
    ਪੂਰਨ ਡੂੰਘਾਈ;
    ਤਾਰਕਿਕ check_clip;
  };

  cubic_ਗੰਢ_t ਢੇਰ[16];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਖਰ_2 = 0;

  ਤਾਰਕਿਕ check_clip = draw->flatten_clip_ਸਰਗਰਮ;
  int64_t *work = draw->get_budget_ਬਚਿਆ ();
  int64_t work_ਖੱਬਾ = *work;

  ਜਦੋਂ (ਸੱਚ)
  {
    ਤਾਰਕਿਕ emit_chord = ਡੂੰਘਾਈ >= 16;

    /* Entirely outside the clip box: the chord is exact for coverage.
       Subdivided control points are convex combinations of their
       parent's, so once a node is entirely inside the box no
       descendant can leave it and the test is skipped below. */
    ਜੇ (!emit_chord && check_clip)
    {
      ਦਸ਼ਮਲਵ bx0 = hb_min (hb_min (x0, x1), hb_min (x2, x3));
      ਦਸ਼ਮਲਵ bx1 = hb_max (hb_max (x0, x1), hb_max (x2, x3));
      ਦਸ਼ਮਲਵ by0 = hb_min (hb_min (y0, y1), hb_min (y2, y3));
      ਦਸ਼ਮਲਵ by1 = hb_max (hb_max (y0, y1), hb_max (y2, y3));
      ਜੇ (bx1 < draw->flatten_clip_x0 || bx0 > draw->flatten_clip_x1 ||
	  by1 < draw->flatten_clip_y0 || by0 > draw->flatten_clip_y1)
	emit_chord = ਸੱਚ;
      ਨਹੀਂਤਾਂ ਜੇ (bx0 >= draw->flatten_clip_x0 && bx1 <= draw->flatten_clip_x1 &&
	       by0 >= draw->flatten_clip_y0 && by1 <= draw->flatten_clip_y1)
	check_clip = ਝੂਠ;
    }

    ਜੇ (!emit_chord)
    {
      ਤਾਰਕਿਕ is_flat;
      ਜੇ (ਝੂਠ)
      {
	/* Old behavior: curvature/chord-error bound. */
	ਦਸ਼ਮਲਵ err2 = cubic_chord_error_bound2 (x0, y0, x1, y1, x2, y2, x3, y3);
	ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;
	is_flat = err2 <= flat_thresh;
      }
      ਨਹੀਂਤਾਂ
      {
	/* FreeType behavior: chord-trisection distance test. */
	ਅਚਲ ਦਸ਼ਮਲਵ flat_thresh = 0.5f;

	ਦਸ਼ਮਲਵ d10x = 2.f * x0 - 3.f * x1 + x3;
	ਦਸ਼ਮਲਵ d10y = 2.f * y0 - 3.f * y1 + y3;
	ਦਸ਼ਮਲਵ d20x = x0 - 3.f * x2 + 2.f * x3;
	ਦਸ਼ਮਲਵ d20y = y0 - 3.f * y2 + 2.f * y3;

	ਜੇ (d10x < 0) d10x = -d10x;
	ਜੇ (d10y < 0) d10y = -d10y;
	ਜੇ (d20x < 0) d20x = -d20x;
	ਜੇ (d20y < 0) d20y = -d20y;

	is_flat = d10x <= flat_thresh &&
		  d10y <= flat_thresh &&
		  d20x <= flat_thresh &&
		  d20y <= flat_thresh;
      }
      emit_chord = is_flat;
    }

    /* Charge one unit per subdivision; degrade to the chord when the
       session work budget is spent. */
    ਜੇ (!emit_chord)
    {
      ਜੇ (unlikely (work_ਖੱਬਾ <= 0))
	emit_chord = ਸੱਚ;
      ਨਹੀਂਤਾਂ
	work_ਖੱਬਾ--;
    }

    ਜੇ (emit_chord)
    {
      emit_segment (draw, x0, y0, x3, y3);
      ਜੇ (!ਸਿਖਰ_2)
      {
	*work = work_ਖੱਬਾ;
	ਵਾਪਸ;
      }
      ਅਚਲ cubic_ਗੰਢ_t &n = ਢੇਰ[--ਸਿਖਰ_2];
      x0 = n.x0; y0 = n.y0;
      x1 = n.x1; y1 = n.y1;
      x2 = n.x2; y2 = n.y2;
      x3 = n.x3; y3 = n.y3;
      ਡੂੰਘਾਈ = n.ਡੂੰਘਾਈ;
      check_clip = n.check_clip;
      ਜਾਰੀ;
    }

    ਦਸ਼ਮਲਵ x01  = (x0 + x1) * 0.5f, y01  = (y0 + y1) * 0.5f;
    ਦਸ਼ਮਲਵ x12  = (x1 + x2) * 0.5f, y12  = (y1 + y2) * 0.5f;
    ਦਸ਼ਮਲਵ x23  = (x2 + x3) * 0.5f, y23  = (y2 + y3) * 0.5f;
    ਦਸ਼ਮਲਵ x012 = (x01 + x12) * 0.5f, y012 = (y01 + y12) * 0.5f;
    ਦਸ਼ਮਲਵ x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
    ਦਸ਼ਮਲਵ xm   = (x012 + x123) * 0.5f, ym   = (y012 + y123) * 0.5f;

    /* Depth is capped at 16, so stack capacity 16 is sufficient. */
    ਢੇਰ[ਸਿਖਰ_2++] = {xm, ym, x123, y123, x23, y23, x3, y3, ਡੂੰਘਾਈ + 1, check_clip};
    x3 = xm; y3 = ym;
    x2 = x012; y2 = y012;
    x1 = x01; y1 = y01;
    ਡੂੰਘਾਈ++;
  }
}

/* Cubic Bézier flattener using forward differencing.
   Use a curvature-based chord-error bound (max||B''||/8), then choose a
   uniform subdivision count n such that the bound drops below threshold.
   The cubic adds a constant third difference d³f = 6·a·h³. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
flatten_cubic_fd (hb_raster_draw_t *draw,
		  ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
		  ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
		  ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
		  ਦਸ਼ਮਲਵ x3, ਦਸ਼ਮਲਵ y3)
{
  ਦਸ਼ਮਲਵ err2 = cubic_chord_error_bound2 (x0, y0, x1, y1, x2, y2, x3, y3);

  ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;

  ਜੇ (err2 <= flat_thresh)
  {
    emit_segment (draw, x0, y0, x3, y3);
    ਵਾਪਸ;
  }

  /* The bound scales with h², so err² shrinks 16× per subdivision level. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ n = 1;
  {
    ਦਸ਼ਮਲਵ ਅਨੁਪਾਤ = err2 / flat_thresh;
    ਜਦੋਂ (ਅਨੁਪਾਤ > 16.f) { ਅਨੁਪਾਤ *= (1.f / 16.f); n++; }
    ਜੇ (n > 16) n = 16;
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ N = 1u << n;
  ਦਸ਼ਮਲਵ h = 1.f / N;

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
  ਦਸ਼ਮਲਵ ax = -x0 + 3*x1 - 3*x2 + x3;
  ਦਸ਼ਮਲਵ ay = -y0 + 3*y1 - 3*y2 + y3;
  ਦਸ਼ਮਲਵ bx =  3*x0 - 6*x1 + 3*x2;
  ਦਸ਼ਮਲਵ by =  3*y0 - 6*y1 + 3*y2;
  ਦਸ਼ਮਲਵ cx =  3*(x1 - x0);
  ਦਸ਼ਮਲਵ cy =  3*(y1 - y0);

  ਦਸ਼ਮਲਵ h2 = h * h, h3 = h2 * h;
  ਦਸ਼ਮਲਵ d3fx = 6 * ax * h3;
  ਦਸ਼ਮਲਵ d3fy = 6 * ay * h3;
  ਦਸ਼ਮਲਵ d2fx = d3fx + 2 * bx * h2;
  ਦਸ਼ਮਲਵ d2fy = d3fy + 2 * by * h2;
  ਦਸ਼ਮਲਵ dfx  = ax * h3 + bx * h2 + cx * h;
  ਦਸ਼ਮਲਵ dfy  = ay * h3 + by * h2 + cy * h;
  ਦਸ਼ਮਲਵ fx   = x0;
  ਦਸ਼ਮਲਵ fy   = y0;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < N; i++)
  {
    ਦਸ਼ਮਲਵ nx = fx + dfx;
    ਦਸ਼ਮਲਵ ny = fy + dfy;
    emit_segment (draw, fx, fy, nx, ny);
    fx = nx;  fy = ny;
    dfx += d2fx;  dfy += d2fy;
    d2fx += d3fx; d2fy += d3fy;
  }
  /* Last segment uses exact endpoint to avoid drift. */
  emit_segment (draw, fx, fy, x3, y3);
}

ਸਥਿਰ ਖਾਲੀ
flatten_cubic (hb_raster_draw_t *draw,
	       ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
	       ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
	       ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
	       ਦਸ਼ਮਲਵ x3, ਦਸ਼ਮਲਵ y3)
{
  ਜੇ (ਝੂਠ)
    flatten_cubic_fd (draw, x0, y0, x1, y1, x2, y2, x3, y3);
  ਨਹੀਂਤਾਂ
    flatten_cubic_recursive (draw, x0, y0, x1, y1, x2, y2, x3, y3);
}


/* Draw callback implementations */

ਸਥਿਰ ਖਾਲੀ
hb_raster_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		   ਖਾਲੀ *draw_ਅੰਕੜੇ,
		   hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		   ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y HB_ਅਣਵਰਤਿਆ,
		   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  /* no-op: state tracked by hb_draw_state_t */
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		   ਖਾਲੀ *draw_ਅੰਕੜੇ,
		   hb_draw_state_t *st,
		   ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
		   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (draw->ਕਿਨਾਰੇ_ਖੱਬਾ <= 0)) ਵਾਪਸ;

  ਦਸ਼ਮਲਵ tx0, ty0, tx1, ty1;
  transform_ਬਿੰਦੂ (draw, st->current_x, st->current_y, tx0, ty0);
  transform_ਬਿੰਦੂ (draw, ਮੰਜ਼ਿਲ_x,          ਮੰਜ਼ਿਲ_y,           tx1, ty1);
  emit_segment (draw, tx0, ty0, tx1, ty1);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			ਖਾਲੀ *draw_ਅੰਕੜੇ,
			hb_draw_state_t *st,
			ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
			ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (draw->ਕਿਨਾਰੇ_ਖੱਬਾ <= 0)) ਵਾਪਸ;

  ਦਸ਼ਮਲਵ tx0, ty0, tx1, ty1, tx2, ty2;
  transform_ਬਿੰਦੂ (draw, st->current_x, st->current_y, tx0, ty0);
  transform_ਬਿੰਦੂ (draw, control_x,     control_y,      tx1, ty1);
  transform_ਬਿੰਦੂ (draw, ਮੰਜ਼ਿਲ_x,          ਮੰਜ਼ਿਲ_y,           tx2, ty2);
  flatten_quadratic (draw, tx0, ty0, tx1, ty1, tx2, ty2);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		    ਖਾਲੀ *draw_ਅੰਕੜੇ,
		    hb_draw_state_t *st,
		    ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
		    ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
		    ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
		    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (draw->ਕਿਨਾਰੇ_ਖੱਬਾ <= 0)) ਵਾਪਸ;

  ਦਸ਼ਮਲਵ tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3;
  transform_ਬਿੰਦੂ (draw, st->current_x, st->current_y, tx0, ty0);
  transform_ਬਿੰਦੂ (draw, control1_x,    control1_y,     tx1, ty1);
  transform_ਬਿੰਦੂ (draw, control2_x,    control2_y,     tx2, ty2);
  transform_ਬਿੰਦੂ (draw, ਮੰਜ਼ਿਲ_x,          ਮੰਜ਼ਿਲ_y,           tx3, ty3);
  flatten_cubic (draw, tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_close_ਰਸਤਾ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		      ਖਾਲੀ *draw_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
		      hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  /* no-op: hb_draw_funcs_t already emits closing line_to before us */
}

ਸਥਿਰ hb_bool_t
hb_raster_draw_ਸੈੱਟ_budget (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			   int64_t budget, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *draw = (hb_raster_draw_t *) draw_ਅੰਕੜੇ;
  draw->budget = budget;
  draw->recharge_budget ();
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ int64_t
hb_raster_draw_get_budget (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_raster_draw_t *) draw_ਅੰਕੜੇ)->budget;
}

ਸਥਿਰ int64_t *
hb_raster_draw_get_budget_ਬਚਿਆ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_raster_draw_t *) draw_ਅੰਕੜੇ)->get_budget_ਬਚਿਆ ();
}


/* Lazy-loader singleton for draw funcs */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_raster_draw_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_raster_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_raster_draw_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func      (funcs, hb_raster_move_ਮੰਜ਼ਿਲ,      ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func      (funcs, hb_raster_ਸਤਰ_ਮੰਜ਼ਿਲ,      ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_quadratic_ਮੰਜ਼ਿਲ_func (funcs, hb_raster_quadratic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func     (funcs, hb_raster_cubic_ਮੰਜ਼ਿਲ,     ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func   (funcs, hb_raster_close_ਰਸਤਾ,   ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_raster_draw_ਸੈੱਟ_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_raster_draw_get_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_raster_draw_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_raster_draw_funcs);

    ਵਾਪਸ funcs;
  }
} static_raster_draw_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
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
hb_raster_draw_get_funcs (ਅਚਲ hb_raster_draw_t *draw HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ static_raster_draw_funcs.get_unconst ();
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
  ਵਾਪਸ hb_font_draw_glyph_or_fail (font, glyph,
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
ਖਾਲੀ
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
ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ HB_RASTER_NARROW_MAX_ਸਰਗਰਮ_ਕਿਨਾਰੇ =
  INT16_MAX / HB_RASTER_ONE_PIXEL;

/* Add one edge piece's area/cover into a single cell. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Area, ਕਿਸਮ_ਨਾਂ Cover>
ਸਥਿਰ HB_ALWAYS_INLINE ਖਾਲੀ
cell_ਜੋੜੋ (Area *area, Cover *cover, ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ, ਪੂਰਨ ਖਾਨਾ,
	  int32_t fx0, int32_t fy0, int32_t fx1, int32_t fy1, int32_t wind,
	  ਬਿਨਾਨਿਸ਼ਾਨੀ &x_min, ਬਿਨਾਨਿਸ਼ਾਨੀ &x_max)
{
  ਜੇ (unlikely ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਖਾਨਾ >= ਚੌੜਾਈ))
  {
    ਜੇ (unlikely (ਖਾਨਾ < 0))
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
    ਵਾਪਸ;
  }
  int32_t dy = fy1 - fy0;
  area[ਖਾਨਾ]  += (Area) (fx0 + fx1) * dy * wind;
  cover[ਖਾਨਾ] += (Cover) (dy * wind);
  x_min = hb_min (x_min, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਖਾਨਾ);
  x_max = hb_max (x_max, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਖਾਨਾ);
}

/* Walk one edge through the pixel cells of a single pixel row,
   accumulating area/cover.  py is the integer pixel-row index. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Area, ਕਿਸਮ_ਨਾਂ Cover>
ਸਥਿਰ HB_ALWAYS_INLINE ਖਾਲੀ
ਕਿਨਾਰਾ_sweep_ਪੰਕਤੀ (Area                   *area,
		Cover                  *cover,
		ਬਿਨਾਨਿਸ਼ਾਨੀ                ਚੌੜਾਈ,
		ਪੂਰਨ                     x_org,
		int32_t                 y_ਸਿਖਰ,
		ਅਚਲ hb_raster_ਕਿਨਾਰਾ_t &ਕਿਨਾਰਾ,
		ਬਿਨਾਨਿਸ਼ਾਨੀ               &x_min,
		ਬਿਨਾਨਿਸ਼ਾਨੀ               &x_max)
{
  /* Saturate: y_top can be within ONE_PIXEL of INT32_MAX when the surface
   * sits at the extreme of the fixed-point coordinate space.  Edges are
   * int32-clamped too, so no coverage exists beyond INT32_MAX anyway. */
  int32_t y_bot = (int32_t) hb_min ((int64_t) y_ਸਿਖਰ + HB_RASTER_ONE_PIXEL, (int64_t) INT32_MAX);

  int32_t ey0 = hb_max (ਕਿਨਾਰਾ.yL, y_ਸਿਖਰ);
  int32_t ey1 = hb_min (ਕਿਨਾਰਾ.yH, y_bot);
  ਜੇ (ey0 >= ey1) ਵਾਪਸ;

  /* X at clipped endpoints (fixed-point). Keep the interpolation in 64-bit
   * so extreme y values do not overflow before the slope multiply. */
  int64_t x0_64 = (int64_t) ਕਿਨਾਰਾ.xL +
		  ((((int64_t) ey0 - (int64_t) ਕਿਨਾਰਾ.yL) * ਕਿਨਾਰਾ.ਢਲਾਨ) >> 16);
  int64_t x1_64 = (int64_t) ਕਿਨਾਰਾ.xL +
		  ((((int64_t) ey1 - (int64_t) ਕਿਨਾਰਾ.yL) * ਕਿਨਾਰਾ.ਢਲਾਨ) >> 16);
  int32_t x0 = (int32_t) hb_clamp (x0_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);
  int32_t x1 = (int32_t) hb_clamp (x1_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);

  /* Fractional y within this pixel row [0, ONE_PIXEL] */
  int32_t fy0 = ey0 - y_ਸਿਖਰ;
  int32_t fy1 = ey1 - y_ਸਿਖਰ;

  int32_t cx0 = x0 >> HB_RASTER_PIXEL_ਬਿੱਟ;
  int32_t fx0 = x0 & HB_RASTER_PIXEL_ਨਕਾਬ;
  int32_t cx1 = x1 >> HB_RASTER_PIXEL_ਬਿੱਟ;
  int32_t fx1 = x1 & HB_RASTER_PIXEL_ਨਕਾਬ;
  int32_t wind = ਕਿਨਾਰਾ.wind;

  /* Fast path: both endpoints in the same pixel column. */
  ਜੇ (cx0 == cx1)
  {
    cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx0 - x_org, fx0, fy0, fx1, fy1, wind, x_min, x_max);
    ਵਾਪਸ;
  }

  int64_t ਕੁੱਲ_dx = (int64_t) x1 - (int64_t) x0;
  int64_t ਕੁੱਲ_dy = (int64_t) fy1 - (int64_t) fy0;

  /* fy increment per pixel column (constant since x_b advances by ONE_PIXEL). */
  int32_t delta_fy = (int32_t) ((int64_t) HB_RASTER_ONE_PIXEL * ਕੁੱਲ_dy / ਕੁੱਲ_dx);

  ਜੇ (ਕੁੱਲ_dx > 0)
  {
    /* Left-to-right edge. */
    ਜੇ (likely ((ਬਿਨਾਨਿਸ਼ਾਨੀ) (cx0 - x_org) < ਚੌੜਾਈ && (ਬਿਨਾਨਿਸ਼ਾਨੀ) (cx1 - x_org) < ਚੌੜਾਈ))
    {
      /* Entirely inside the surface: unclipped walk. */
      int32_t x_b = (int32_t) hb_clamp (((int64_t) cx0 + 1) * HB_RASTER_ONE_PIXEL,
					(int64_t) INT32_MIN, (int64_t) INT32_MAX);
      int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * ਕੁੱਲ_dy) / ਕੁੱਲ_dx);
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);

      int32_t fy_prev = fy_b;
      ਲਈ (int32_t cx = cx0 + 1; cx < cx1; cx++)
      {
	fy_b = fy_prev + delta_fy;
	cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx - x_org, 0, fy_prev, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
	fy_prev = fy_b;
      }

      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx1 - x_org, 0, fy_prev, fx1, fy1, wind, x_min, x_max);
      ਵਾਪਸ;
    }

    /* Visible column window.  Columns left of it only contribute their
     * total cover to column 0 (see cell_add) and that total telescopes,
     * while columns right of it contribute nothing; so neither side is
     * walked cell by cell.  Skipping ahead in the fy accumulation is
     * exact: the increment is constant and the running sums are bounded,
     * so k steps equal one k·delta_fy jump. */
    int64_t ਖਾਨਾ_min = (int64_t) x_org;
    int64_t ਖਾਨਾ_max = (int64_t) x_org + (int64_t) ਚੌੜਾਈ - 1;

    ਜੇ (unlikely (cx1 < ਖਾਨਾ_min))
    {
      /* Entirely left of the surface. */
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, -1, 0, fy0, 0, fy1, wind, x_min, x_max);
      ਵਾਪਸ;
    }
    ਜੇ (unlikely (cx0 > ਖਾਨਾ_max))
      ਵਾਪਸ; /* Entirely right of the surface. */

    int32_t x_b = (int32_t) hb_clamp (((int64_t) cx0 + 1) * HB_RASTER_ONE_PIXEL,
				      (int64_t) INT32_MIN, (int64_t) INT32_MAX);
    int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * ਕੁੱਲ_dy) / ਕੁੱਲ_dx);

    int32_t cx = cx0 + 1;
    int32_t fy_prev = fy_b;
    ਜੇ (likely (cx0 >= ਖਾਨਾ_min))
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
    ਨਹੀਂਤਾਂ
    {
      /* Fold the first cell and the mid columns left of the surface
       * into one column-0 cover update. */
      int32_t cx_skip = (int32_t) ਖਾਨਾ_min; /* ≤ cx1, so it fits */
      fy_prev = fy_b + (int32_t) ((int64_t) (cx_skip - cx) * delta_fy);
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, -1, 0, fy0, 0, fy_prev, wind, x_min, x_max);
      cx = cx_skip;
    }

    int32_t cx_ਸਿਰਾ = (int32_t) hb_min ((int64_t) cx1, ਖਾਨਾ_max + 1);
    ਲਈ (; cx < cx_ਸਿਰਾ; cx++)
    {
      fy_b = fy_prev + delta_fy;
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx - x_org, 0, fy_prev, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
      fy_prev = fy_b;
    }

    ਜੇ (likely (cx1 <= ਖਾਨਾ_max))
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx1 - x_org, 0, fy_prev, fx1, fy1, wind, x_min, x_max);
  }
  ਨਹੀਂਤਾਂ
  {
    /* Right-to-left edge. */
    ਜੇ (likely ((ਬਿਨਾਨਿਸ਼ਾਨੀ) (cx0 - x_org) < ਚੌੜਾਈ && (ਬਿਨਾਨਿਸ਼ਾਨੀ) (cx1 - x_org) < ਚੌੜਾਈ))
    {
      /* Entirely inside the surface: unclipped walk. */
      int32_t x_b = (int32_t) hb_clamp ((int64_t) cx0 * HB_RASTER_ONE_PIXEL,
					(int64_t) INT32_MIN, (int64_t) INT32_MAX);
      int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * ਕੁੱਲ_dy) / ਕੁੱਲ_dx);
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx0 - x_org, fx0, fy0, 0, fy_b, wind, x_min, x_max);

      int32_t fy_prev = fy_b;
      ਲਈ (int32_t cx = cx0 - 1; cx > cx1; cx--)
      {
	fy_b = fy_prev - delta_fy;
	cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx - x_org, HB_RASTER_ONE_PIXEL, fy_prev, 0, fy_b, wind, x_min, x_max);
	fy_prev = fy_b;
      }

      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_prev, fx1, fy1, wind, x_min, x_max);
      ਵਾਪਸ;
    }

    int64_t ਖਾਨਾ_min = (int64_t) x_org;
    int64_t ਖਾਨਾ_max = (int64_t) x_org + (int64_t) ਚੌੜਾਈ - 1;

    ਜੇ (unlikely (cx0 < ਖਾਨਾ_min))
    {
      /* Entirely left of the surface. */
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, -1, 0, fy0, 0, fy1, wind, x_min, x_max);
      ਵਾਪਸ;
    }
    ਜੇ (unlikely (cx1 > ਖਾਨਾ_max))
      ਵਾਪਸ; /* Entirely right of the surface. */

    int32_t x_b = (int32_t) hb_clamp ((int64_t) cx0 * HB_RASTER_ONE_PIXEL,
				      (int64_t) INT32_MIN, (int64_t) INT32_MAX);
    int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * ਕੁੱਲ_dy) / ਕੁੱਲ_dx);
    cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx0 - x_org, fx0, fy0, 0, fy_b, wind, x_min, x_max);

    int32_t cx = cx0 - 1;
    int32_t fy_prev = fy_b;
    ਜੇ (unlikely (cx > ਖਾਨਾ_max))
    {
      /* Mid columns right of the surface contribute nothing. */
      int32_t cx_skip = (int32_t) ਖਾਨਾ_max; /* ≥ cx1, so it fits */
      fy_prev = fy_b - (int32_t) ((int64_t) (cx - cx_skip) * delta_fy);
      cx = cx_skip;
    }

    int32_t cx_stop = (int32_t) hb_max ((int64_t) cx1, ਖਾਨਾ_min - 1);
    ਲਈ (; cx > cx_stop; cx--)
    {
      fy_b = fy_prev - delta_fy;
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx - x_org, HB_RASTER_ONE_PIXEL, fy_prev, 0, fy_b, wind, x_min, x_max);
      fy_prev = fy_b;
    }

    ਜੇ (likely (cx1 >= ਖਾਨਾ_min))
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_prev, fx1, fy1, wind, x_min, x_max);
    ਨਹੀਂਤਾਂ
      /* Fold the mid columns left of the surface and the last cell
       * into one column-0 cover update. */
      cell_ਜੋੜੋ (area, cover, ਚੌੜਾਈ, -1, 0, fy_prev, 0, fy1, wind, x_min, x_max);
  }
}

/* Convert cover-delta + area to alpha bytes, then clear.
   Returns final cover accumulator over [x_min, x_max]. */
ਸਥਿਰ int32_t
sweep_ਪੰਕਤੀ_ਮੰਜ਼ਿਲ_alpha (uint8_t *__restrict ਪੰਕਤੀ_buf,
		    int32_t *__restrict area,
		    int16_t *__restrict cover,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ x_min,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ x_max)
{
  ਅਚਲ int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
  int32_t cover_accum = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ x = x_min;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_RASTER_NEON
  int32x4_t clamp_v = vdupq_n_s32 (HB_RASTER_ਭਰਿਆ_COVERAGE);
  int32x4_t bias_v  = vdupq_n_s32 (HB_RASTER_ਭਰਿਆ_COVERAGE / 2);
  int32x4_t zero32  = vdupq_n_s32 (0);
  int16x8_t zero16  = vdupq_n_s16 (0);
  ਲਈ (; x + 7 <= x_max; x += 8)
  {
    int32_t ctmp[8];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 8; i++)
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

    int32x4_t r0 = vshrq_n_s32 (vmlaq_n_s32 (bias_v, v0, 255), 2 * HB_RASTER_PIXEL_ਬਿੱਟ + 1);
    int32x4_t r1 = vshrq_n_s32 (vmlaq_n_s32 (bias_v, v1, 255), 2 * HB_RASTER_PIXEL_ਬਿੱਟ + 1);

    int16x4_t h0 = vmovn_s32 (r0);
    int16x4_t h1 = vmovn_s32 (r1);
    int16x8_t h  = vcombine_s16 (h0, h1);
    uint8x8_t b  = vqmovun_s16 (h);
    vst1_u8 (ਪੰਕਤੀ_buf + x, b);

    vst1q_s32 (area + x,     zero32);
    vst1q_s32 (area + x + 4, zero32);
    vst1q_s16 (cover + x,    zero16);
  }
#ਜਾਂਜੇ defined(HB_RASTER_SSE2)
  __m128i clamp_v = _mm_set1_epi32 (HB_RASTER_ਭਰਿਆ_COVERAGE);
  __m128i bias_v  = _mm_set1_epi32 (HB_RASTER_ਭਰਿਆ_COVERAGE / 2);
  __m128i zero_v  = _mm_setzero_si128 ();
  ਲਈ (; x + 7 <= x_max; x += 8)
  {
    int32_t ctmp[8];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 8; i++)
    {
      cover_accum += cover[x + i];
      ctmp[i] = cover_accum * cover_scale;
    }

    __m128i c0  = _mm_loadu_si128 ((__m128i *) (ਖਾਲੀ *) (ctmp + 0));
    __m128i c1  = _mm_loadu_si128 ((__m128i *) (ਖਾਲੀ *) (ctmp + 4));
    __m128i a0  = _mm_loadu_si128 ((__m128i *) (ਖਾਲੀ *) (area + x));
    __m128i a1  = _mm_loadu_si128 ((__m128i *) (ਖਾਲੀ *) (area + x + 4));

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

    __m128i r0 = _mm_srai_epi32 (_mm_add_epi32 (_mm_sub_epi32 (_mm_slli_epi32 (v0, 8), v0), bias_v), 2 * HB_RASTER_PIXEL_ਬਿੱਟ + 1);
    __m128i r1 = _mm_srai_epi32 (_mm_add_epi32 (_mm_sub_epi32 (_mm_slli_epi32 (v1, 8), v1), bias_v), 2 * HB_RASTER_PIXEL_ਬਿੱਟ + 1);

    __m128i h = _mm_packs_epi32 (r0, r1);
    __m128i b = _mm_packus_epi16 (h, h);
    _mm_storel_epi64 ((__m128i *) (ਖਾਲੀ *) (ਪੰਕਤੀ_buf + x), b);

    _mm_storeu_si128 ((__m128i *) (ਖਾਲੀ *) (area + x),     zero_v);
    _mm_storeu_si128 ((__m128i *) (ਖਾਲੀ *) (area + x + 4), zero_v);
    _mm_storeu_si128 ((__m128i *) (ਖਾਲੀ *) (cover + x),    zero_v);
  }
#ਜੇ_ਅੰਤ

  ਲਈ (; x <= x_max; x++)
  {
    cover_accum += cover[x];
    int32_t val   = cover_accum * cover_scale - area[x];
    int32_t alpha = val < 0 ? -val : val;
    ਜੇ (alpha > HB_RASTER_ਭਰਿਆ_COVERAGE) alpha = HB_RASTER_ਭਰਿਆ_COVERAGE;
    ਪੰਕਤੀ_buf[x] = (uint8_t) (((ਬਿਨਾਨਿਸ਼ਾਨੀ) alpha * 255 + HB_RASTER_ਭਰਿਆ_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_ਬਿੱਟ + 1));
    area[x]  = 0;
    cover[x] = 0;
  }

  ਵਾਪਸ cover_accum;
}

ਸਥਿਰ int64_t
sweep_ਪੰਕਤੀ_ਮੰਜ਼ਿਲ_alpha_wide (uint8_t *__restrict ਪੰਕਤੀ_buf,
			 int64_t *__restrict area,
			 int64_t *__restrict cover,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ x_min,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ x_max)
{
  ਅਚਲ int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
  int64_t cover_accum = 0;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = x_min; x <= x_max; x++)
  {
    cover_accum += cover[x];
    int64_t val = cover_accum * cover_scale - area[x];
    int64_t alpha = val < 0 ? -val : val;
    ਜੇ (alpha > HB_RASTER_ਭਰਿਆ_COVERAGE) alpha = HB_RASTER_ਭਰਿਆ_COVERAGE;
    ਪੰਕਤੀ_buf[x] = (uint8_t) (((ਬਿਨਾਨਿਸ਼ਾਨੀ) alpha * 255 + HB_RASTER_ਭਰਿਆ_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_ਬਿੱਟ + 1));
    area[x] = 0;
    cover[x] = 0;
  }

  ਵਾਪਸ cover_accum;
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

  ਜੇ (draw->has_extents)
  {
    ext = draw->fixed_extents;
  }
  ਨਹੀਂਤਾਂ
  {
    /* Auto-size from edge bounding box */
    ਜੇ (draw->ਕਿਨਾਰੇ.ਲੰਮਾਈ == 0)
    {
      /* No edges: produce 0×0 image */
      ext = { 0, 0, 0, 0, 0 };
    }
    ਨਹੀਂਤਾਂ
    {
      int32_t xmin = draw->ਕਿਨਾਰੇ.arrayZ[0].xL, xmax = draw->ਕਿਨਾਰੇ.arrayZ[0].xL;
      int32_t ymin = draw->ਕਿਨਾਰੇ.arrayZ[0].yL, ymax = draw->ਕਿਨਾਰੇ.arrayZ[0].yH;

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &e : draw->ਕਿਨਾਰੇ)
      {
	xmin = hb_min (xmin, hb_min (e.xL, e.xH));
	xmax = hb_max (xmax, hb_max (e.xL, e.xH));
	ymin = hb_min (ymin, e.yL);
	ymax = hb_max (ymax, e.yH);
      }

      /* Convert fixed-point → pixels (floor for min, ceil for max).  Edge
	 coordinates are saturated to int32 range in emit_segment, so the
	 +MASK ceil step must be widened to avoid signed overflow. */
      ਪੂਰਨ x0 = xmin >> HB_RASTER_PIXEL_ਬਿੱਟ;
      ਪੂਰਨ y0 = ymin >> HB_RASTER_PIXEL_ਬਿੱਟ;
      ਪੂਰਨ x1 = (ਪੂਰਨ) (((int64_t) xmax + HB_RASTER_PIXEL_ਨਕਾਬ) >> HB_RASTER_PIXEL_ਬਿੱਟ);
      ਪੂਰਨ y1 = (ਪੂਰਨ) (((int64_t) ymax + HB_RASTER_PIXEL_ਨਕਾਬ) >> HB_RASTER_PIXEL_ਬਿੱਟ);

      ext.x_origin = x0;
      ext.y_origin = y0;
      ext.ਚੌੜਾਈ    = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_min (hb_max (0, x1 - x0), HB_RASTER_MAX_AUTO_DIMENSION);
      ext.ਉਚਾਈ   = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_min (hb_max (0, y1 - y0), HB_RASTER_MAX_AUTO_DIMENSION);
      ext.stride   = 0; /* filled below */
    }
  }

  /* ── 2. Compute stride ─────────────────────────────────────────── */
  ਜੇ (ext.stride == 0)
    ext.stride = (ext.ਚੌੜਾਈ + 3u) & ~3u;

  /* ── 3. Allocate or reuse image ─────────────────────────────────── */
  /* Reset one-shot state on every exit path. */
  HB_SCOPE_GUARD (hb_raster_draw_clear (draw));

  hb_unique_ptr_t<hb_raster_image_t> image;
  ਜੇ (draw->recycled_image)
  {
    image = hb_unique_ptr_t<hb_raster_image_t> (draw->recycled_image);
    draw->recycled_image = ਖਾਲੀ_ਸੂਚਕ;
  }
  ਨਹੀਂਤਾਂ
  {
    image = hb_unique_ptr_t<hb_raster_image_t> (hb_raster_image_create_or_fail ());
    ਜੇ (unlikely (!image)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (unlikely (!image->configure (HB_RASTER_FORMAT_A8, ext)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  image->clear ();

  /* ── 4. Bucket edges by starting row and rasterize scanlines ──── */
  ਜੇ (draw->ਕਿਨਾਰੇ.ਲੰਮਾਈ && ext.ਚੌੜਾਈ && ext.ਉਚਾਈ)
  {
    ਜੇ (unlikely (!draw->ਪੰਕਤੀ_area.resize_dirty (ext.ਚੌੜਾਈ) ||
		  !draw->ਪੰਕਤੀ_cover.resize_dirty (ext.ਚੌੜਾਈ)))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    hb_memset (draw->ਪੰਕਤੀ_area.arrayZ,  0, ext.ਚੌੜਾਈ * ਆਕਾਰ (int32_t));
    hb_memset (draw->ਪੰਕਤੀ_cover.arrayZ, 0, ext.ਚੌੜਾਈ * ਆਕਾਰ (int16_t));

    /* Bucket edges by their starting pixel row.
       Only grow the outer vector; clear inner vectors without freeing. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ old_buckets = draw->ਕਿਨਾਰਾ_buckets.ਲੰਮਾਈ;
    ਜੇ (ext.ਉਚਾਈ > old_buckets)
    {
      ਜੇ (unlikely (!draw->ਕਿਨਾਰਾ_buckets.resize (ext.ਉਚਾਈ)))
	ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < hb_min (ext.ਉਚਾਈ, old_buckets); i++)
      draw->ਕਿਨਾਰਾ_buckets.arrayZ[i].clear ();
    /* New buckets (if any) are already empty from resize's zero-init. */

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < draw->ਕਿਨਾਰੇ.ਲੰਮਾਈ; i++)
    {
      ਪੂਰਨ ਪੰਕਤੀ = (draw->ਕਿਨਾਰੇ.arrayZ[i].yL >> HB_RASTER_PIXEL_ਬਿੱਟ) - ext.y_origin;
      ਜੇ (ਪੰਕਤੀ < 0) ਪੰਕਤੀ = 0;
      ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਪੰਕਤੀ >= ext.ਉਚਾਈ) ਜਾਰੀ;
      draw->ਕਿਨਾਰਾ_buckets.arrayZ[ਪੰਕਤੀ].push (i);
    }

    /* Scanline loop with active edge list. */
    draw->ਸਰਗਰਮ_ਕਿਨਾਰੇ.clear ();
    ਤਾਰਕਿਕ wide_initialized = ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ = 0; ਪੰਕਤੀ < ext.ਉਚਾਈ; ਪੰਕਤੀ++)
    {
      int64_t y_ਸਿਖਰ_64 = ((int64_t) ext.y_origin + (int64_t) ਪੰਕਤੀ) * HB_RASTER_ONE_PIXEL;
      int32_t y_ਸਿਖਰ = (int32_t) hb_clamp (y_ਸਿਖਰ_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);

      /* Add new edges from this row's bucket. */
      draw->ਸਰਗਰਮ_ਕਿਨਾਰੇ.extend (draw->ਕਿਨਾਰਾ_buckets.arrayZ[ਪੰਕਤੀ]);

      /* Process active edges and compact live ones in one linear pass. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ x_min = ext.ਚੌੜਾਈ, x_max = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਿਖ = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਰਗਰਮ_len = draw->ਸਰਗਰਮ_ਕਿਨਾਰੇ.ਲੰਮਾਈ;
      ਤਾਰਕਿਕ use_wide = ਸਰਗਰਮ_len > HB_RASTER_NARROW_MAX_ਸਰਗਰਮ_ਕਿਨਾਰੇ;
      ਜੇ (unlikely (use_wide && !wide_initialized))
      {
	ਜੇ (unlikely (!draw->ਪੰਕਤੀ_area_wide.resize_dirty (ext.ਚੌੜਾਈ) ||
		      !draw->ਪੰਕਤੀ_cover_wide.resize_dirty (ext.ਚੌੜਾਈ)))
	  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
	hb_memset (draw->ਪੰਕਤੀ_area_wide.arrayZ,  0, ext.ਚੌੜਾਈ * ਆਕਾਰ (int64_t));
	hb_memset (draw->ਪੰਕਤੀ_cover_wide.arrayZ, 0, ext.ਚੌੜਾਈ * ਆਕਾਰ (int64_t));
	wide_initialized = ਸੱਚ;
      }
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < ਸਰਗਰਮ_len; j++)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਕਿਨਾਰਾ_idx = draw->ਸਰਗਰਮ_ਕਿਨਾਰੇ.arrayZ[j];
	ਅਚਲ ਸਵੈਚਾਲਿਤ &e = draw->ਕਿਨਾਰੇ.arrayZ[ਕਿਨਾਰਾ_idx];
	ਜੇ (e.yH <= y_ਸਿਖਰ)
	  ਜਾਰੀ;

	ਜੇ (unlikely (use_wide))
	  ਕਿਨਾਰਾ_sweep_ਪੰਕਤੀ (draw->ਪੰਕਤੀ_area_wide.arrayZ, draw->ਪੰਕਤੀ_cover_wide.arrayZ,
			  ext.ਚੌੜਾਈ, ext.x_origin, y_ਸਿਖਰ, e, x_min, x_max);
	ਨਹੀਂਤਾਂ
	  ਕਿਨਾਰਾ_sweep_ਪੰਕਤੀ (draw->ਪੰਕਤੀ_area.arrayZ, draw->ਪੰਕਤੀ_cover.arrayZ,
			  ext.ਚੌੜਾਈ, ext.x_origin, y_ਸਿਖਰ, e, x_min, x_max);
	draw->ਸਰਗਰਮ_ਕਿਨਾਰੇ.arrayZ[ਲਿਖ++] = ਕਿਨਾਰਾ_idx;
      }
      draw->ਸਰਗਰਮ_ਕਿਨਾਰੇ.resize (ਲਿਖ);

      ਜੇ (x_min <= x_max)
      {
	int64_t cover_accum = use_wide ?
	  sweep_ਪੰਕਤੀ_ਮੰਜ਼ਿਲ_alpha_wide (image->ਬਫਰ.arrayZ + ਪੰਕਤੀ * ext.stride,
				   draw->ਪੰਕਤੀ_area_wide.arrayZ, draw->ਪੰਕਤੀ_cover_wide.arrayZ,
				   x_min, x_max) :
	  sweep_ਪੰਕਤੀ_ਮੰਜ਼ਿਲ_alpha (image->ਬਫਰ.arrayZ + ਪੰਕਤੀ * ext.stride,
			      draw->ਪੰਕਤੀ_area.arrayZ, draw->ਪੰਕਤੀ_cover.arrayZ,
			      x_min, x_max);

	/* If cover doesn't cancel, memset the constant-alpha tail. */
	ਜੇ (cover_accum != 0)
	{
	  int64_t alpha = cover_accum * (2 * HB_RASTER_ONE_PIXEL);
	  alpha = alpha < 0 ? -alpha : alpha;
	  ਜੇ (alpha > HB_RASTER_ਭਰਿਆ_COVERAGE) alpha = HB_RASTER_ਭਰਿਆ_COVERAGE;
	  uint8_t byte = (uint8_t) (((ਬਿਨਾਨਿਸ਼ਾਨੀ) alpha * 255 + HB_RASTER_ਭਰਿਆ_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_ਬਿੱਟ + 1));

	  uint8_t *ਪੰਕਤੀ_buf = image->ਬਫਰ.arrayZ + ਪੰਕਤੀ * ext.stride;
	  hb_memset (ਪੰਕਤੀ_buf + x_max + 1, byte, ext.ਚੌੜਾਈ - 1 - x_max);
	}
      }
    }
  }

  ਵਾਪਸ image.release ();
}
