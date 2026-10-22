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

#ضمن "hb.hh"

#ضمن "hb-raster-image.hh"
#ضمن "hb-geometry.hh"
#ضمن "hb-machinery.hh"

#إذا defined(__aarch64__) || defined(_M_ARM64)
#ضمن <arm_neon.h>
#تعریف_کرو HB_RASTER_NEON 1
#وإلاإذا defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64)
#ضمن <emmintrin.h>
#تعریف_کرو HB_RASTER_SSE2 1
#جے_انت


/* Fixed-point precision for sub-pixel coordinates.
   8 bits = 24.8: 256 sub-pixel units per pixel. */
#تعریف_کرو HB_RASTER_PIXEL_بٹ 8
#تعریف_کرو HB_RASTER_ONE_PIXEL  (1 << HB_RASTER_PIXEL_بٹ)
#تعریف_کرو HB_RASTER_PIXEL_نقاب (HB_RASTER_ONE_PIXEL - 1)
/* Full-coverage alpha = 2 * ONE_PIXEL^2 */
#تعریف_کرو HB_RASTER_بھریا_COVERAGE (2 * HB_RASTER_ONE_PIXEL * HB_RASTER_ONE_PIXEL)
/* Flatness threshold for Bézier flattening: max deviation in pixels */
#تعریف_کرو HB_RASTER_FLAT_THRESH 0.25f


/* Normalized edge: yH > yL always */
بنية hb_raster_کنارا_t
{
  int32_t xL, yL;   /* lower endpoint (fixed-point) */
  int32_t xH, yH;   /* upper endpoint (fixed-point) */
  int64_t ڈھلوان;    /* dx/dy in 16.16 fixed point: ((int64_t)dx << 16) / dy */
  int32_t wind;     /* +1 or -1 */
};

/* hb_raster_draw_t — outline rasterizer */
بنية hb_raster_draw_t
{
  hb_object_سرخی_t سرخی;

  /* Configuration */
  hb_transform_t<>    transform         = {1, 0, 0, 1, 0, 0};
  عائم               x_scale_ضارب    = 1.f;
  عائم               y_scale_ضارب    = 1.f;
  hb_raster_extents_t fixed_extents     = {};
  منطقي                has_extents = خطأ;

  /* Visibility clip box for curve flattening (device pixels); set
     internally by raster-paint so invisible curves collapse to their
     chord.  See hb_raster_draw_set_clip_box(). */
  منطقي  has_clip_box = خطأ;
  عائم clip_x0 = 0.f, clip_y0 = 0.f, clip_x1 = 0.f, clip_y1 = 0.f;

  /* Flattening clip resolved from the above (or from fixed extents),
     expanded by one pixel; kept in sync by hb_raster_draw_update_flatten_clip(). */
  منطقي  flatten_clip_سرگرم = خطأ;
  عائم flatten_clip_x0 = 0.f, flatten_clip_y0 = 0.f;
  عائم flatten_clip_x1 = 0.f, flatten_clip_y1 = 0.f;

  /* Work budget for outline traversal and curve flattening.  A paint
     backend seeds this through the public draw-budget API before drawing
     and reads the remainder back afterwards, so a whole paint session
     shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_بچیا = HB_BUDGET_GLYPH;

  فراغ recharge_budget ()
  {
    budget_بچیا = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_بچیا ()
  { أرجع &budget_بچیا; }

  /* Accumulated geometry */
  int64_t کنارے_بایاں = HB_RASTER_MAX_DRAW_کنارے;
  hb_سمتیہ_t<hb_raster_کنارا_t> کنارے;

  /* Scratch — reused across render() calls */
  hb_سمتیہ_t<int32_t> صف_area;
  hb_سمتیہ_t<int16_t> صف_cover;
  hb_سمتیہ_t<int64_t> صف_area_wide;
  hb_سمتیہ_t<int64_t> صف_cover_wide;
  hb_سمتیہ_t<hb_سمتیہ_t<بنانشانی>> کنارا_buckets;
  hb_سمتیہ_t<بنانشانی> سرگرم_کنارے;

  /* Recycled image for zero-malloc render */
  hb_raster_image_t *recycled_image = خالی_سوچک;
};

ساكن HB_ALWAYS_INLINE فراغ
hb_raster_draw_transform_نقطہ (ثابت hb_raster_draw_t *draw,
				عائم x, عائم y,
				عائم &tx, عائم &ty)
{
  tx = x; ty = y;
  draw->transform.transform_نقطہ (tx, ty);
  tx /= draw->x_scale_ضارب;
  ty /= draw->y_scale_ضارب;
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
hb_raster_draw_create_or_fail (فراغ)
{
  hb_raster_draw_t *draw = hb_object_create<hb_raster_draw_t> ();
  أرجع draw;
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
  أرجع hb_object_reference (draw);
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
فراغ
hb_raster_draw_destroy (hb_raster_draw_t *draw)
{
  إذا (!hb_object_should_destroy (draw))
    أرجع;

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
hb_raster_draw_سیٹ_user_انکڑے (hb_raster_draw_t   *draw,
			      hb_user_انکڑے_کلید_t *key,
			      فراغ               *انکڑے,
			      hb_destroy_func_t   destroy,
			      hb_bool_t           replace)
{
  أرجع hb_object_سیٹ_user_انکڑے (draw, key, انکڑے, destroy, replace);
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
فراغ *
hb_raster_draw_get_user_انکڑے (ثابت hb_raster_draw_t   *draw,
			      hb_user_انکڑے_کلید_t *key)
{
  أرجع hb_object_get_user_انکڑے (draw, key);
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
فراغ
hb_raster_draw_سیٹ_transform (hb_raster_draw_t *draw,
			      عائم xx, عائم yx,
			      عائم xy, عائم yy,
			      عائم dx, عائم dy)
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
فراغ
hb_raster_draw_سیٹ_scale_ضارب (hb_raster_draw_t *draw,
				 عائم x_scale_ضارب,
				 عائم y_scale_ضارب)
{
  draw->x_scale_ضارب = x_scale_ضارب > 0.f ? x_scale_ضارب : 1.f;
  draw->y_scale_ضارب = y_scale_ضارب > 0.f ? y_scale_ضارب : 1.f;
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
فراغ
hb_raster_draw_get_scale_ضارب (ثابت hb_raster_draw_t *draw,
				 عائم *x_scale_ضارب,
				 عائم *y_scale_ضارب)
{
  إذا (x_scale_ضارب) *x_scale_ضارب = draw->x_scale_ضارب;
  إذا (y_scale_ضارب) *y_scale_ضارب = draw->y_scale_ضارب;
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
فراغ
hb_raster_draw_get_transform (ثابت hb_raster_draw_t *draw,
			      عائم *xx, عائم *yx,
			      عائم *xy, عائم *yy,
			      عائم *dx, عائم *dy)
{
  إذا (xx) *xx = draw->transform.xx;
  إذا (yx) *yx = draw->transform.yx;
  إذا (xy) *xy = draw->transform.xy;
  إذا (yy) *yy = draw->transform.yy;
  إذا (dx) *dx = draw->transform.x0;
  إذا (dy) *dy = draw->transform.y0;
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
ساكن فراغ
hb_raster_draw_update_flatten_clip (hb_raster_draw_t *draw)
{
  إذا (draw->has_clip_box)
  {
    draw->flatten_clip_سرگرم = سچ;
    draw->flatten_clip_x0 = draw->clip_x0 - 1.f;
    draw->flatten_clip_y0 = draw->clip_y0 - 1.f;
    draw->flatten_clip_x1 = draw->clip_x1 + 1.f;
    draw->flatten_clip_y1 = draw->clip_y1 + 1.f;
  }
  وإلا إذا (draw->has_extents)
  {
    draw->flatten_clip_سرگرم = سچ;
    draw->flatten_clip_x0 = (عائم) draw->fixed_extents.x_origin - 1.f;
    draw->flatten_clip_y0 = (عائم) draw->fixed_extents.y_origin - 1.f;
    draw->flatten_clip_x1 = (عائم) draw->fixed_extents.x_origin + (عائم) draw->fixed_extents.چوڑائی + 1.f;
    draw->flatten_clip_y1 = (عائم) draw->fixed_extents.y_origin + (عائم) draw->fixed_extents.اچائی + 1.f;
  }
  وإلا
    draw->flatten_clip_سرگرم = خطأ;
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
فراغ
hb_raster_draw_سیٹ_extents (hb_raster_draw_t          *draw,
			    ثابت hb_raster_extents_t *extents)
{
  draw->fixed_extents     = *extents;
  draw->has_extents = سچ;
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
hb_raster_draw_get_extents (ثابت hb_raster_draw_t    *draw,
			    hb_raster_extents_t *extents)
{
  إذا (!draw->has_extents)
    أرجع خطأ;

  إذا (extents)
    *extents = draw->fixed_extents;
  أرجع سچ;
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
hb_raster_draw_سیٹ_glyph_extents (hb_raster_draw_t         *draw,
				  ثابت hb_glyph_extents_t *glyph_extents)
{
  عائم x0 = (عائم) glyph_extents->x_bearing;
  عائم y0 = (عائم) glyph_extents->y_bearing;
  عائم x1 = (عائم) glyph_extents->x_bearing + glyph_extents->چوڑائی;
  عائم y1 = (عائم) glyph_extents->y_bearing + glyph_extents->اچائی;

  عائم xmin = hb_min (x0, x1);
  عائم xmax = hb_max (x0, x1);
  عائم ymin = hb_min (y0, y1);
  عائم ymax = hb_max (y0, y1);

  عائم px[4] = {xmin, xmin, xmax, xmax};
  عائم py[4] = {ymin, ymax, ymin, ymax};

  عائم tx, ty;
  hb_raster_draw_transform_نقطہ (draw, px[0], py[0], tx, ty);
  عائم tx_min = tx, tx_max = tx;
  عائم ty_min = ty, ty_max = ty;

  لكل (بنانشانی i = 1; i < 4; i++)
  {
    hb_raster_draw_transform_نقطہ (draw, px[i], py[i], tx, ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  int32_t ex0 = hb_clamp_منزل<int32_t> (floorf (tx_min));
  int32_t ey0 = hb_clamp_منزل<int32_t> (floorf (ty_min));
  int32_t ex1 = hb_clamp_منزل<int32_t> (ceilf  (tx_max));
  int32_t ey1 = hb_clamp_منزل<int32_t> (ceilf  (ty_max));

  إذا (ex1 <= ex0 || ey1 <= ey0)
  {
    draw->fixed_extents = {};
    draw->has_extents = خطأ;
    hb_raster_draw_update_flatten_clip (draw);
    أرجع خطأ;
  }

  draw->fixed_extents = {
    ex0, ey0,
    (بنانشانی) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (بنانشانی) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  draw->has_extents = سچ;
  hb_raster_draw_update_flatten_clip (draw);
  أرجع سچ;
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
فراغ
hb_raster_draw_clear (hb_raster_draw_t *draw)
{
  draw->fixed_extents     = {};
  draw->has_extents = خطأ;
  draw->has_clip_box = خطأ;
  draw->flatten_clip_سرگرم = خطأ;
  draw->recharge_budget ();
  draw->کنارے_بایاں = HB_RASTER_MAX_DRAW_کنارے;
  draw->کنارے.clear ();
  draw->سرگرم_کنارے.clear ();
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
فراغ
hb_raster_draw_reset (hb_raster_draw_t *draw)
{
  draw->transform         = {1, 0, 0, 1, 0, 0};
  draw->x_scale_ضارب    = 1.f;
  draw->y_scale_ضارب    = 1.f;
  draw->budget            = HB_BUDGET_DEFAULT;
  hb_raster_draw_clear (draw);
}

فراغ
hb_raster_draw_سیٹ_clip_box (hb_raster_draw_t *draw,
			     عائم x0, عائم y0,
			     عائم x1, عائم y1)
{
  draw->has_clip_box = سچ;
  draw->clip_x0 = x0;
  draw->clip_y0 = y0;
  draw->clip_x1 = x1;
  draw->clip_y1 = y1;
  hb_raster_draw_update_flatten_clip (draw);
}

int64_t
hb_raster_draw_get_pixel_work (ثابت hb_raster_draw_t *draw,
			       بنانشانی صحيح max_صفاں,
			       بنانشانی صحيح max_خانے_گنتی)
{
  int64_t work = 0;
  لكل (ثابت خودکار &کنارا : draw->کنارے)
  {
    int64_t dx = (int64_t) کنارا.xH - کنارا.xL;
    إذا (dx < 0) dx = -dx;
    work += 1 + hb_min (((int64_t) کنارا.yH - کنارا.yL) >> HB_RASTER_PIXEL_بٹ,
			(int64_t) max_صفاں)
	      + hb_min (dx >> HB_RASTER_PIXEL_بٹ,
			(int64_t) max_خانے_گنتی);
  }
  أرجع work;
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
فراغ
hb_raster_draw_recycle_image (hb_raster_draw_t  *draw,
			      hb_raster_image_t *image)
{
  hb_raster_image_destroy (draw->recycled_image);
  draw->recycled_image = image;
}


/*
 * Draw callbacks — flatten on the fly into hb_raster_edge_t
 */

ساكن مضمن فراغ
transform_نقطہ (ثابت hb_raster_draw_t *draw,
		 عائم  x,  عائم  y,
		 عائم &tx, عائم &ty)
{
  hb_raster_draw_transform_نقطہ (draw, x, y, tx, ty);
}

ساكن فراغ
emit_segment (hb_raster_draw_t *draw,
	      عائم x0, عائم y0,
	      عائم x1, عائم y1)
{
  إذا (unlikely (draw->کنارے_بایاں <= 0))
    أرجع;

  int32_t X0 = hb_clamp_منزل<int32_t> (roundf (x0 * HB_RASTER_ONE_PIXEL));
  int32_t Y0 = hb_clamp_منزل<int32_t> (roundf (y0 * HB_RASTER_ONE_PIXEL));
  int32_t X1 = hb_clamp_منزل<int32_t> (roundf (x1 * HB_RASTER_ONE_PIXEL));
  int32_t Y1 = hb_clamp_منزل<int32_t> (roundf (y1 * HB_RASTER_ONE_PIXEL));

  إذا (Y0 == Y1) أرجع; /* horizontal — skip */

  hb_raster_کنارا_t e;
  إذا (Y0 < Y1) {
    e.xL = X0; e.yL = Y0; e.xH = X1; e.yH = Y1; e.wind = +1;
  } وإلا {
    e.xL = X1; e.yL = Y1; e.xH = X0; e.yH = Y0; e.wind = -1;
  }
  e.ڈھلوان = (((int64_t) e.xH - (int64_t) e.xL) * (int64_t) 65536) /
	    ((int64_t) e.yH - (int64_t) e.yL);

  إذا (likely (draw->کنارے.ڈالو_or_fail (e)))
    draw->کنارے_بایاں--;
  وإلا
    draw->کنارے_بایاں = 0;
}


/* Quadratic Bézier flattener — iterative de Casteljau at t=0.5. */
ساكن مضمن فراغ
flatten_quadratic_recursive (hb_raster_draw_t *draw,
			     عائم x0, عائم y0,
			     عائم x1, عائم y1,
			     عائم x2, عائم y2,
			     صحيح ڈونگھائی = 0)
{
  بنية quad_گرہ_t
  {
    عائم x0, y0, x1, y1, x2, y2;
    صحيح ڈونگھائی;
    منطقي check_clip;
  };

  quad_گرہ_t ڈھیر[16];
  بنانشانی چوٹی_2 = 0;

  منطقي check_clip = draw->flatten_clip_سرگرم;
  int64_t *work = draw->get_budget_بچیا ();
  int64_t work_بایاں = *work;

  طالما (سچ)
  {
    منطقي emit_chord = ڈونگھائی >= 16;

    /* Entirely outside the clip box: the chord is exact for coverage.
       Subdivided control points are convex combinations of their
       parent's, so once a node is entirely inside the box no
       descendant can leave it and the test is skipped below. */
    إذا (!emit_chord && check_clip)
    {
      عائم bx0 = hb_min (x0, hb_min (x1, x2));
      عائم bx1 = hb_max (x0, hb_max (x1, x2));
      عائم by0 = hb_min (y0, hb_min (y1, y2));
      عائم by1 = hb_max (y0, hb_max (y1, y2));
      إذا (bx1 < draw->flatten_clip_x0 || bx0 > draw->flatten_clip_x1 ||
	  by1 < draw->flatten_clip_y0 || by0 > draw->flatten_clip_y1)
	emit_chord = سچ;
      وإلا إذا (bx0 >= draw->flatten_clip_x0 && bx1 <= draw->flatten_clip_x1 &&
	       by0 >= draw->flatten_clip_y0 && by1 <= draw->flatten_clip_y1)
	check_clip = خطأ;
    }

    إذا (!emit_chord)
    {
      منطقي is_flat;
      إذا (خطأ)
      {
	/* Old behavior: midpoint deviation from chord midpoint. */
	عائم mx = x0 * 0.25f + x1 * 0.5f + x2 * 0.25f;
	عائم my = y0 * 0.25f + y1 * 0.5f + y2 * 0.25f;
	عائم chord_mx = (x0 + x2) * 0.5f;
	عائم chord_my = (y0 + y2) * 0.5f;
	عائم dx = mx - chord_mx;
	عائم dy = my - chord_my;
	ساكن ثابت عائم flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;
	is_flat = (dx * dx + dy * dy) <= flat_thresh;
      }
      وإلا
      {
	/* FreeType behavior: control-point deviation from chord center. */
	ثابت عائم flat_thresh = 0.25f;
	عائم dx = x0 + x2 - 2.f * x1;
	عائم dy = y0 + y2 - 2.f * y1;
	إذا (dx < 0) dx = -dx;
	إذا (dy < 0) dy = -dy;
	is_flat = dx <= flat_thresh && dy <= flat_thresh;
      }
      emit_chord = is_flat;
    }

    /* Charge one unit per subdivision; degrade to the chord when the
       session work budget is spent. */
    إذا (!emit_chord)
    {
      إذا (unlikely (work_بایاں <= 0))
	emit_chord = سچ;
      وإلا
	work_بایاں--;
    }

    إذا (emit_chord)
    {
      emit_segment (draw, x0, y0, x2, y2);
      إذا (!چوٹی_2)
      {
	*work = work_بایاں;
	أرجع;
      }
      ثابت quad_گرہ_t &n = ڈھیر[--چوٹی_2];
      x0 = n.x0; y0 = n.y0;
      x1 = n.x1; y1 = n.y1;
      x2 = n.x2; y2 = n.y2;
      ڈونگھائی = n.ڈونگھائی;
      check_clip = n.check_clip;
      استمر;
    }

    عائم x01 = (x0 + x1) * 0.5f, y01 = (y0 + y1) * 0.5f;
    عائم x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
    عائم xm  = (x01 + x12) * 0.5f, ym  = (y01 + y12) * 0.5f;

    /* Depth is capped at 16, so stack capacity 16 is sufficient. */
    ڈھیر[چوٹی_2++] = {xm, ym, x12, y12, x2, y2, ڈونگھائی + 1, check_clip};
    x2 = xm; y2 = ym;
    x1 = x01; y1 = y01;
    ڈونگھائی++;
  }
}

/* Quadratic Bézier flattener using forward differencing.
   The error (midpoint deviation) shrinks exactly 4× per de Casteljau
   subdivision, so we compute the subdivision count upfront and iterate
   with constant-cost additions instead of recursive branching. */
ساكن مضمن فراغ
flatten_quadratic_fd (hb_raster_draw_t *draw,
		      عائم x0, عائم y0,
		      عائم x1, عائم y1,
		      عائم x2, عائم y2)
{
  /* Deviation of curve midpoint from chord midpoint (squared). */
  عائم devx = (x0 - 2 * x1 + x2) * 0.25f;
  عائم devy = (y0 - 2 * y1 + y2) * 0.25f;
  عائم err2 = devx * devx + devy * devy;

  ساكن ثابت عائم flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;

  إذا (err2 <= flat_thresh)
  {
    emit_segment (draw, x0, y0, x2, y2);
    أرجع;
  }

  /* err² shrinks 16× per subdivision level.  Find n such that
     err2 / 16^n <= flat_thresh, i.e. n = ceil(log₁₆(err2/flat_thresh)). */
  بنانشانی n = 1;
  {
    عائم تناسب = err2 / flat_thresh;
    طالما (تناسب > 16.f) { تناسب *= (1.f / 16.f); n++; }
    إذا (n > 16) n = 16;
  }
  بنانشانی N = 1u << n;   /* number of line segments */
  عائم h = 1.f / N;

  /* Quadratic: B(t) = a·t² + b·t + c
     Forward differences with step h:
       d²f = 2·a·h²   (constant)
       df₀ = a·h² + b·h
       f₀  = c = P₀                     */
  عائم ax = x0 - 2 * x1 + x2;
  عائم ay = y0 - 2 * y1 + y2;
  عائم bx = 2 * (x1 - x0);
  عائم by = 2 * (y1 - y0);

  عائم d2fx = 2 * ax * h * h;
  عائم d2fy = 2 * ay * h * h;
  عائم dfx  = ax * h * h + bx * h;
  عائم dfy  = ay * h * h + by * h;
  عائم fx   = x0;
  عائم fy   = y0;

  لكل (بنانشانی i = 1; i < N; i++)
  {
    عائم nx = fx + dfx;
    عائم ny = fy + dfy;
    emit_segment (draw, fx, fy, nx, ny);
    fx = nx;
    fy = ny;
    dfx += d2fx;
    dfy += d2fy;
  }
  /* Last segment uses exact endpoint to avoid drift. */
  emit_segment (draw, fx, fy, x2, y2);
}

ساكن فراغ
flatten_quadratic (hb_raster_draw_t *draw,
		   عائم x0, عائم y0,
		   عائم x1, عائم y1,
		   عائم x2, عائم y2)
{
  إذا (خطأ)
    flatten_quadratic_fd (draw, x0, y0, x1, y1, x2, y2);
  وإلا
    flatten_quadratic_recursive (draw, x0, y0, x1, y1, x2, y2);
}

/* For cubic B(t), the max deviation from its chord on [0,1] is bounded by:
     max||B(t)-L(t)|| <= max_t ||B''(t)|| / 8.
   B''(t) is linear, so max norm is attained at t=0 or t=1:
     B''(0)=6*(P0-2P1+P2), B''(1)=6*(P1-2P2+P3). */
ساكن مضمن عائم
cubic_chord_error_bound2 (عائم x0, عائم y0,
			  عائم x1, عائم y1,
			  عائم x2, عائم y2,
			  عائم x3, عائم y3)
{
  عائم d20x = x0 - 2 * x1 + x2;
  عائم d20y = y0 - 2 * y1 + y2;
  عائم d21x = x1 - 2 * x2 + x3;
  عائم d21y = y1 - 2 * y2 + y3;
  عائم m0 = d20x * d20x + d20y * d20y;
  عائم m1 = d21x * d21x + d21y * d21y;
  عائم m = m0 > m1 ? m0 : m1;
  /* (max||B''||/8)^2 = (6/8)^2 * max||d2||^2 = (3/4)^2 * m. */
  أرجع m * (9.f / 16.f);
}

/* Cubic Bézier flattener — iterative de Casteljau at t=0.5. */
ساكن مضمن فراغ
flatten_cubic_recursive (hb_raster_draw_t *draw,
			 عائم x0, عائم y0,
			 عائم x1, عائم y1,
			 عائم x2, عائم y2,
			 عائم x3, عائم y3,
			 صحيح ڈونگھائی = 0)
{
  بنية cubic_گرہ_t
  {
    عائم x0, y0, x1, y1, x2, y2, x3, y3;
    صحيح ڈونگھائی;
    منطقي check_clip;
  };

  cubic_گرہ_t ڈھیر[16];
  بنانشانی چوٹی_2 = 0;

  منطقي check_clip = draw->flatten_clip_سرگرم;
  int64_t *work = draw->get_budget_بچیا ();
  int64_t work_بایاں = *work;

  طالما (سچ)
  {
    منطقي emit_chord = ڈونگھائی >= 16;

    /* Entirely outside the clip box: the chord is exact for coverage.
       Subdivided control points are convex combinations of their
       parent's, so once a node is entirely inside the box no
       descendant can leave it and the test is skipped below. */
    إذا (!emit_chord && check_clip)
    {
      عائم bx0 = hb_min (hb_min (x0, x1), hb_min (x2, x3));
      عائم bx1 = hb_max (hb_max (x0, x1), hb_max (x2, x3));
      عائم by0 = hb_min (hb_min (y0, y1), hb_min (y2, y3));
      عائم by1 = hb_max (hb_max (y0, y1), hb_max (y2, y3));
      إذا (bx1 < draw->flatten_clip_x0 || bx0 > draw->flatten_clip_x1 ||
	  by1 < draw->flatten_clip_y0 || by0 > draw->flatten_clip_y1)
	emit_chord = سچ;
      وإلا إذا (bx0 >= draw->flatten_clip_x0 && bx1 <= draw->flatten_clip_x1 &&
	       by0 >= draw->flatten_clip_y0 && by1 <= draw->flatten_clip_y1)
	check_clip = خطأ;
    }

    إذا (!emit_chord)
    {
      منطقي is_flat;
      إذا (خطأ)
      {
	/* Old behavior: curvature/chord-error bound. */
	عائم err2 = cubic_chord_error_bound2 (x0, y0, x1, y1, x2, y2, x3, y3);
	ساكن ثابت عائم flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;
	is_flat = err2 <= flat_thresh;
      }
      وإلا
      {
	/* FreeType behavior: chord-trisection distance test. */
	ثابت عائم flat_thresh = 0.5f;

	عائم d10x = 2.f * x0 - 3.f * x1 + x3;
	عائم d10y = 2.f * y0 - 3.f * y1 + y3;
	عائم d20x = x0 - 3.f * x2 + 2.f * x3;
	عائم d20y = y0 - 3.f * y2 + 2.f * y3;

	إذا (d10x < 0) d10x = -d10x;
	إذا (d10y < 0) d10y = -d10y;
	إذا (d20x < 0) d20x = -d20x;
	إذا (d20y < 0) d20y = -d20y;

	is_flat = d10x <= flat_thresh &&
		  d10y <= flat_thresh &&
		  d20x <= flat_thresh &&
		  d20y <= flat_thresh;
      }
      emit_chord = is_flat;
    }

    /* Charge one unit per subdivision; degrade to the chord when the
       session work budget is spent. */
    إذا (!emit_chord)
    {
      إذا (unlikely (work_بایاں <= 0))
	emit_chord = سچ;
      وإلا
	work_بایاں--;
    }

    إذا (emit_chord)
    {
      emit_segment (draw, x0, y0, x3, y3);
      إذا (!چوٹی_2)
      {
	*work = work_بایاں;
	أرجع;
      }
      ثابت cubic_گرہ_t &n = ڈھیر[--چوٹی_2];
      x0 = n.x0; y0 = n.y0;
      x1 = n.x1; y1 = n.y1;
      x2 = n.x2; y2 = n.y2;
      x3 = n.x3; y3 = n.y3;
      ڈونگھائی = n.ڈونگھائی;
      check_clip = n.check_clip;
      استمر;
    }

    عائم x01  = (x0 + x1) * 0.5f, y01  = (y0 + y1) * 0.5f;
    عائم x12  = (x1 + x2) * 0.5f, y12  = (y1 + y2) * 0.5f;
    عائم x23  = (x2 + x3) * 0.5f, y23  = (y2 + y3) * 0.5f;
    عائم x012 = (x01 + x12) * 0.5f, y012 = (y01 + y12) * 0.5f;
    عائم x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
    عائم xm   = (x012 + x123) * 0.5f, ym   = (y012 + y123) * 0.5f;

    /* Depth is capped at 16, so stack capacity 16 is sufficient. */
    ڈھیر[چوٹی_2++] = {xm, ym, x123, y123, x23, y23, x3, y3, ڈونگھائی + 1, check_clip};
    x3 = xm; y3 = ym;
    x2 = x012; y2 = y012;
    x1 = x01; y1 = y01;
    ڈونگھائی++;
  }
}

/* Cubic Bézier flattener using forward differencing.
   Use a curvature-based chord-error bound (max||B''||/8), then choose a
   uniform subdivision count n such that the bound drops below threshold.
   The cubic adds a constant third difference d³f = 6·a·h³. */
ساكن مضمن فراغ
flatten_cubic_fd (hb_raster_draw_t *draw,
		  عائم x0, عائم y0,
		  عائم x1, عائم y1,
		  عائم x2, عائم y2,
		  عائم x3, عائم y3)
{
  عائم err2 = cubic_chord_error_bound2 (x0, y0, x1, y1, x2, y2, x3, y3);

  ساكن ثابت عائم flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;

  إذا (err2 <= flat_thresh)
  {
    emit_segment (draw, x0, y0, x3, y3);
    أرجع;
  }

  /* The bound scales with h², so err² shrinks 16× per subdivision level. */
  بنانشانی n = 1;
  {
    عائم تناسب = err2 / flat_thresh;
    طالما (تناسب > 16.f) { تناسب *= (1.f / 16.f); n++; }
    إذا (n > 16) n = 16;
  }
  بنانشانی N = 1u << n;
  عائم h = 1.f / N;

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
  عائم ax = -x0 + 3*x1 - 3*x2 + x3;
  عائم ay = -y0 + 3*y1 - 3*y2 + y3;
  عائم bx =  3*x0 - 6*x1 + 3*x2;
  عائم by =  3*y0 - 6*y1 + 3*y2;
  عائم cx =  3*(x1 - x0);
  عائم cy =  3*(y1 - y0);

  عائم h2 = h * h, h3 = h2 * h;
  عائم d3fx = 6 * ax * h3;
  عائم d3fy = 6 * ay * h3;
  عائم d2fx = d3fx + 2 * bx * h2;
  عائم d2fy = d3fy + 2 * by * h2;
  عائم dfx  = ax * h3 + bx * h2 + cx * h;
  عائم dfy  = ay * h3 + by * h2 + cy * h;
  عائم fx   = x0;
  عائم fy   = y0;

  لكل (بنانشانی i = 1; i < N; i++)
  {
    عائم nx = fx + dfx;
    عائم ny = fy + dfy;
    emit_segment (draw, fx, fy, nx, ny);
    fx = nx;  fy = ny;
    dfx += d2fx;  dfy += d2fy;
    d2fx += d3fx; d2fy += d3fy;
  }
  /* Last segment uses exact endpoint to avoid drift. */
  emit_segment (draw, fx, fy, x3, y3);
}

ساكن فراغ
flatten_cubic (hb_raster_draw_t *draw,
	       عائم x0, عائم y0,
	       عائم x1, عائم y1,
	       عائم x2, عائم y2,
	       عائم x3, عائم y3)
{
  إذا (خطأ)
    flatten_cubic_fd (draw, x0, y0, x1, y1, x2, y2, x3, y3);
  وإلا
    flatten_cubic_recursive (draw, x0, y0, x1, y1, x2, y2, x3, y3);
}


/* Draw callback implementations */

ساكن فراغ
hb_raster_move_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
		   فراغ *draw_انکڑے,
		   hb_draw_state_t *st HB_بن_ورتیا,
		   عائم منزل_x HB_بن_ورتیا, عائم منزل_y HB_بن_ورتیا,
		   فراغ *user_انکڑے HB_بن_ورتیا)
{
  /* no-op: state tracked by hb_draw_state_t */
}

ساكن فراغ
hb_raster_سطر_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
		   فراغ *draw_انکڑے,
		   hb_draw_state_t *st,
		   عائم منزل_x, عائم منزل_y,
		   فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_انکڑے;
  إذا (unlikely (draw->کنارے_بایاں <= 0)) أرجع;

  عائم tx0, ty0, tx1, ty1;
  transform_نقطہ (draw, st->current_x, st->current_y, tx0, ty0);
  transform_نقطہ (draw, منزل_x,          منزل_y,           tx1, ty1);
  emit_segment (draw, tx0, ty0, tx1, ty1);
}

ساكن فراغ
hb_raster_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			فراغ *draw_انکڑے,
			hb_draw_state_t *st,
			عائم control_x, عائم control_y,
			عائم منزل_x, عائم منزل_y,
			فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_انکڑے;
  إذا (unlikely (draw->کنارے_بایاں <= 0)) أرجع;

  عائم tx0, ty0, tx1, ty1, tx2, ty2;
  transform_نقطہ (draw, st->current_x, st->current_y, tx0, ty0);
  transform_نقطہ (draw, control_x,     control_y,      tx1, ty1);
  transform_نقطہ (draw, منزل_x,          منزل_y,           tx2, ty2);
  flatten_quadratic (draw, tx0, ty0, tx1, ty1, tx2, ty2);
}

ساكن فراغ
hb_raster_cubic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
		    فراغ *draw_انکڑے,
		    hb_draw_state_t *st,
		    عائم control1_x, عائم control1_y,
		    عائم control2_x, عائم control2_y,
		    عائم منزل_x, عائم منزل_y,
		    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_انکڑے;
  إذا (unlikely (draw->کنارے_بایاں <= 0)) أرجع;

  عائم tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3;
  transform_نقطہ (draw, st->current_x, st->current_y, tx0, ty0);
  transform_نقطہ (draw, control1_x,    control1_y,     tx1, ty1);
  transform_نقطہ (draw, control2_x,    control2_y,     tx2, ty2);
  transform_نقطہ (draw, منزل_x,          منزل_y,           tx3, ty3);
  flatten_cubic (draw, tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3);
}

ساكن فراغ
hb_raster_close_رستہ (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
		      فراغ *draw_انکڑے HB_بن_ورتیا,
		      hb_draw_state_t *st HB_بن_ورتیا,
		      فراغ *user_انکڑے HB_بن_ورتیا)
{
  /* no-op: hb_draw_funcs_t already emits closing line_to before us */
}

ساكن hb_bool_t
hb_raster_draw_سیٹ_budget (hb_draw_funcs_t *, فراغ *draw_انکڑے,
			   int64_t budget, فراغ *)
{
  خودکار *draw = (hb_raster_draw_t *) draw_انکڑے;
  draw->budget = budget;
  draw->recharge_budget ();
  أرجع سچ;
}

ساكن int64_t
hb_raster_draw_get_budget (hb_draw_funcs_t *, فراغ *draw_انکڑے, فراغ *)
{
  أرجع ((hb_raster_draw_t *) draw_انکڑے)->budget;
}

ساكن int64_t *
hb_raster_draw_get_budget_بچیا (hb_draw_funcs_t *, فراغ *draw_انکڑے, فراغ *)
{
  أرجع ((hb_raster_draw_t *) draw_انکڑے)->get_budget_بچیا ();
}


/* Lazy-loader singleton for draw funcs */

ساكن مضمن فراغ free_static_raster_draw_funcs ();

ساكن بنية hb_raster_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_raster_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_منزل_func      (funcs, hb_raster_move_منزل,      خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_سطر_منزل_func      (funcs, hb_raster_سطر_منزل,      خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_quadratic_منزل_func (funcs, hb_raster_quadratic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_cubic_منزل_func     (funcs, hb_raster_cubic_منزل,     خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_close_رستہ_func   (funcs, hb_raster_close_رستہ,   خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_سیٹ_budget_func (funcs, hb_raster_draw_سیٹ_budget, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_get_budget_func (funcs, hb_raster_draw_get_budget, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_raster_draw_get_budget_بچیا, خالی_سوچک, خالی_سوچک);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_raster_draw_funcs);

    أرجع funcs;
  }
} static_raster_draw_funcs;

ساكن مضمن فراغ
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
hb_raster_draw_get_funcs (ثابت hb_raster_draw_t *draw HB_بن_ورتیا)
{
  أرجع static_raster_draw_funcs.get_unconst ();
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
  أرجع hb_font_draw_glyph_or_fail (font, glyph,
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
فراغ
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
ساكن constexpr بنانشانی HB_RASTER_NARROW_MAX_سرگرم_کنارے =
  INT16_MAX / HB_RASTER_ONE_PIXEL;

/* Add one edge piece's area/cover into a single cell. */
سانچا <قسم_ناں Area, قسم_ناں Cover>
ساكن HB_ALWAYS_INLINE فراغ
cell_جوڑو (Area *area, Cover *cover, بنانشانی چوڑائی, صحيح خانہ,
	  int32_t fx0, int32_t fy0, int32_t fx1, int32_t fy1, int32_t wind,
	  بنانشانی &x_min, بنانشانی &x_max)
{
  إذا (unlikely ((بنانشانی) خانہ >= چوڑائی))
  {
    إذا (unlikely (خانہ < 0))
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
    أرجع;
  }
  int32_t dy = fy1 - fy0;
  area[خانہ]  += (Area) (fx0 + fx1) * dy * wind;
  cover[خانہ] += (Cover) (dy * wind);
  x_min = hb_min (x_min, (بنانشانی) خانہ);
  x_max = hb_max (x_max, (بنانشانی) خانہ);
}

/* Walk one edge through the pixel cells of a single pixel row,
   accumulating area/cover.  py is the integer pixel-row index. */
سانچا <قسم_ناں Area, قسم_ناں Cover>
ساكن HB_ALWAYS_INLINE فراغ
کنارا_sweep_صف (Area                   *area,
		Cover                  *cover,
		بنانشانی                چوڑائی,
		صحيح                     x_org,
		int32_t                 y_چوٹی,
		ثابت hb_raster_کنارا_t &کنارا,
		بنانشانی               &x_min,
		بنانشانی               &x_max)
{
  /* Saturate: y_top can be within ONE_PIXEL of INT32_MAX when the surface
   * sits at the extreme of the fixed-point coordinate space.  Edges are
   * int32-clamped too, so no coverage exists beyond INT32_MAX anyway. */
  int32_t y_bot = (int32_t) hb_min ((int64_t) y_چوٹی + HB_RASTER_ONE_PIXEL, (int64_t) INT32_MAX);

  int32_t ey0 = hb_max (کنارا.yL, y_چوٹی);
  int32_t ey1 = hb_min (کنارا.yH, y_bot);
  إذا (ey0 >= ey1) أرجع;

  /* X at clipped endpoints (fixed-point). Keep the interpolation in 64-bit
   * so extreme y values do not overflow before the slope multiply. */
  int64_t x0_64 = (int64_t) کنارا.xL +
		  ((((int64_t) ey0 - (int64_t) کنارا.yL) * کنارا.ڈھلوان) >> 16);
  int64_t x1_64 = (int64_t) کنارا.xL +
		  ((((int64_t) ey1 - (int64_t) کنارا.yL) * کنارا.ڈھلوان) >> 16);
  int32_t x0 = (int32_t) hb_clamp (x0_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);
  int32_t x1 = (int32_t) hb_clamp (x1_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);

  /* Fractional y within this pixel row [0, ONE_PIXEL] */
  int32_t fy0 = ey0 - y_چوٹی;
  int32_t fy1 = ey1 - y_چوٹی;

  int32_t cx0 = x0 >> HB_RASTER_PIXEL_بٹ;
  int32_t fx0 = x0 & HB_RASTER_PIXEL_نقاب;
  int32_t cx1 = x1 >> HB_RASTER_PIXEL_بٹ;
  int32_t fx1 = x1 & HB_RASTER_PIXEL_نقاب;
  int32_t wind = کنارا.wind;

  /* Fast path: both endpoints in the same pixel column. */
  إذا (cx0 == cx1)
  {
    cell_جوڑو (area, cover, چوڑائی, cx0 - x_org, fx0, fy0, fx1, fy1, wind, x_min, x_max);
    أرجع;
  }

  int64_t کل_dx = (int64_t) x1 - (int64_t) x0;
  int64_t کل_dy = (int64_t) fy1 - (int64_t) fy0;

  /* fy increment per pixel column (constant since x_b advances by ONE_PIXEL). */
  int32_t delta_fy = (int32_t) ((int64_t) HB_RASTER_ONE_PIXEL * کل_dy / کل_dx);

  إذا (کل_dx > 0)
  {
    /* Left-to-right edge. */
    إذا (likely ((بنانشانی) (cx0 - x_org) < چوڑائی && (بنانشانی) (cx1 - x_org) < چوڑائی))
    {
      /* Entirely inside the surface: unclipped walk. */
      int32_t x_b = (int32_t) hb_clamp (((int64_t) cx0 + 1) * HB_RASTER_ONE_PIXEL,
					(int64_t) INT32_MIN, (int64_t) INT32_MAX);
      int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * کل_dy) / کل_dx);
      cell_جوڑو (area, cover, چوڑائی, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);

      int32_t fy_prev = fy_b;
      لكل (int32_t cx = cx0 + 1; cx < cx1; cx++)
      {
	fy_b = fy_prev + delta_fy;
	cell_جوڑو (area, cover, چوڑائی, cx - x_org, 0, fy_prev, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
	fy_prev = fy_b;
      }

      cell_جوڑو (area, cover, چوڑائی, cx1 - x_org, 0, fy_prev, fx1, fy1, wind, x_min, x_max);
      أرجع;
    }

    /* Visible column window.  Columns left of it only contribute their
     * total cover to column 0 (see cell_add) and that total telescopes,
     * while columns right of it contribute nothing; so neither side is
     * walked cell by cell.  Skipping ahead in the fy accumulation is
     * exact: the increment is constant and the running sums are bounded,
     * so k steps equal one k·delta_fy jump. */
    int64_t خانہ_min = (int64_t) x_org;
    int64_t خانہ_max = (int64_t) x_org + (int64_t) چوڑائی - 1;

    إذا (unlikely (cx1 < خانہ_min))
    {
      /* Entirely left of the surface. */
      cell_جوڑو (area, cover, چوڑائی, -1, 0, fy0, 0, fy1, wind, x_min, x_max);
      أرجع;
    }
    إذا (unlikely (cx0 > خانہ_max))
      أرجع; /* Entirely right of the surface. */

    int32_t x_b = (int32_t) hb_clamp (((int64_t) cx0 + 1) * HB_RASTER_ONE_PIXEL,
				      (int64_t) INT32_MIN, (int64_t) INT32_MAX);
    int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * کل_dy) / کل_dx);

    int32_t cx = cx0 + 1;
    int32_t fy_prev = fy_b;
    إذا (likely (cx0 >= خانہ_min))
      cell_جوڑو (area, cover, چوڑائی, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
    وإلا
    {
      /* Fold the first cell and the mid columns left of the surface
       * into one column-0 cover update. */
      int32_t cx_skip = (int32_t) خانہ_min; /* ≤ cx1, so it fits */
      fy_prev = fy_b + (int32_t) ((int64_t) (cx_skip - cx) * delta_fy);
      cell_جوڑو (area, cover, چوڑائی, -1, 0, fy0, 0, fy_prev, wind, x_min, x_max);
      cx = cx_skip;
    }

    int32_t cx_سرا = (int32_t) hb_min ((int64_t) cx1, خانہ_max + 1);
    لكل (; cx < cx_سرا; cx++)
    {
      fy_b = fy_prev + delta_fy;
      cell_جوڑو (area, cover, چوڑائی, cx - x_org, 0, fy_prev, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
      fy_prev = fy_b;
    }

    إذا (likely (cx1 <= خانہ_max))
      cell_جوڑو (area, cover, چوڑائی, cx1 - x_org, 0, fy_prev, fx1, fy1, wind, x_min, x_max);
  }
  وإلا
  {
    /* Right-to-left edge. */
    إذا (likely ((بنانشانی) (cx0 - x_org) < چوڑائی && (بنانشانی) (cx1 - x_org) < چوڑائی))
    {
      /* Entirely inside the surface: unclipped walk. */
      int32_t x_b = (int32_t) hb_clamp ((int64_t) cx0 * HB_RASTER_ONE_PIXEL,
					(int64_t) INT32_MIN, (int64_t) INT32_MAX);
      int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * کل_dy) / کل_dx);
      cell_جوڑو (area, cover, چوڑائی, cx0 - x_org, fx0, fy0, 0, fy_b, wind, x_min, x_max);

      int32_t fy_prev = fy_b;
      لكل (int32_t cx = cx0 - 1; cx > cx1; cx--)
      {
	fy_b = fy_prev - delta_fy;
	cell_جوڑو (area, cover, چوڑائی, cx - x_org, HB_RASTER_ONE_PIXEL, fy_prev, 0, fy_b, wind, x_min, x_max);
	fy_prev = fy_b;
      }

      cell_جوڑو (area, cover, چوڑائی, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_prev, fx1, fy1, wind, x_min, x_max);
      أرجع;
    }

    int64_t خانہ_min = (int64_t) x_org;
    int64_t خانہ_max = (int64_t) x_org + (int64_t) چوڑائی - 1;

    إذا (unlikely (cx0 < خانہ_min))
    {
      /* Entirely left of the surface. */
      cell_جوڑو (area, cover, چوڑائی, -1, 0, fy0, 0, fy1, wind, x_min, x_max);
      أرجع;
    }
    إذا (unlikely (cx1 > خانہ_max))
      أرجع; /* Entirely right of the surface. */

    int32_t x_b = (int32_t) hb_clamp ((int64_t) cx0 * HB_RASTER_ONE_PIXEL,
				      (int64_t) INT32_MIN, (int64_t) INT32_MAX);
    int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * کل_dy) / کل_dx);
    cell_جوڑو (area, cover, چوڑائی, cx0 - x_org, fx0, fy0, 0, fy_b, wind, x_min, x_max);

    int32_t cx = cx0 - 1;
    int32_t fy_prev = fy_b;
    إذا (unlikely (cx > خانہ_max))
    {
      /* Mid columns right of the surface contribute nothing. */
      int32_t cx_skip = (int32_t) خانہ_max; /* ≥ cx1, so it fits */
      fy_prev = fy_b - (int32_t) ((int64_t) (cx - cx_skip) * delta_fy);
      cx = cx_skip;
    }

    int32_t cx_stop = (int32_t) hb_max ((int64_t) cx1, خانہ_min - 1);
    لكل (; cx > cx_stop; cx--)
    {
      fy_b = fy_prev - delta_fy;
      cell_جوڑو (area, cover, چوڑائی, cx - x_org, HB_RASTER_ONE_PIXEL, fy_prev, 0, fy_b, wind, x_min, x_max);
      fy_prev = fy_b;
    }

    إذا (likely (cx1 >= خانہ_min))
      cell_جوڑو (area, cover, چوڑائی, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_prev, fx1, fy1, wind, x_min, x_max);
    وإلا
      /* Fold the mid columns left of the surface and the last cell
       * into one column-0 cover update. */
      cell_جوڑو (area, cover, چوڑائی, -1, 0, fy_prev, 0, fy1, wind, x_min, x_max);
  }
}

/* Convert cover-delta + area to alpha bytes, then clear.
   Returns final cover accumulator over [x_min, x_max]. */
ساكن int32_t
sweep_صف_منزل_alpha (uint8_t *__restrict صف_buf,
		    int32_t *__restrict area,
		    int16_t *__restrict cover,
		    بنانشانی x_min,
		    بنانشانی x_max)
{
  ثابت int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
  int32_t cover_accum = 0;
  بنانشانی x = x_min;

#جے_تعریف_شدہ HB_RASTER_NEON
  int32x4_t clamp_v = vdupq_n_s32 (HB_RASTER_بھریا_COVERAGE);
  int32x4_t bias_v  = vdupq_n_s32 (HB_RASTER_بھریا_COVERAGE / 2);
  int32x4_t zero32  = vdupq_n_s32 (0);
  int16x8_t zero16  = vdupq_n_s16 (0);
  لكل (; x + 7 <= x_max; x += 8)
  {
    int32_t ctmp[8];
    لكل (بنانشانی i = 0; i < 8; i++)
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

    int32x4_t r0 = vshrq_n_s32 (vmlaq_n_s32 (bias_v, v0, 255), 2 * HB_RASTER_PIXEL_بٹ + 1);
    int32x4_t r1 = vshrq_n_s32 (vmlaq_n_s32 (bias_v, v1, 255), 2 * HB_RASTER_PIXEL_بٹ + 1);

    int16x4_t h0 = vmovn_s32 (r0);
    int16x4_t h1 = vmovn_s32 (r1);
    int16x8_t h  = vcombine_s16 (h0, h1);
    uint8x8_t b  = vqmovun_s16 (h);
    vst1_u8 (صف_buf + x, b);

    vst1q_s32 (area + x,     zero32);
    vst1q_s32 (area + x + 4, zero32);
    vst1q_s16 (cover + x,    zero16);
  }
#وإلاإذا defined(HB_RASTER_SSE2)
  __m128i clamp_v = _mm_set1_epi32 (HB_RASTER_بھریا_COVERAGE);
  __m128i bias_v  = _mm_set1_epi32 (HB_RASTER_بھریا_COVERAGE / 2);
  __m128i zero_v  = _mm_setzero_si128 ();
  لكل (; x + 7 <= x_max; x += 8)
  {
    int32_t ctmp[8];
    لكل (بنانشانی i = 0; i < 8; i++)
    {
      cover_accum += cover[x + i];
      ctmp[i] = cover_accum * cover_scale;
    }

    __m128i c0  = _mm_loadu_si128 ((__m128i *) (فراغ *) (ctmp + 0));
    __m128i c1  = _mm_loadu_si128 ((__m128i *) (فراغ *) (ctmp + 4));
    __m128i a0  = _mm_loadu_si128 ((__m128i *) (فراغ *) (area + x));
    __m128i a1  = _mm_loadu_si128 ((__m128i *) (فراغ *) (area + x + 4));

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

    __m128i r0 = _mm_srai_epi32 (_mm_add_epi32 (_mm_sub_epi32 (_mm_slli_epi32 (v0, 8), v0), bias_v), 2 * HB_RASTER_PIXEL_بٹ + 1);
    __m128i r1 = _mm_srai_epi32 (_mm_add_epi32 (_mm_sub_epi32 (_mm_slli_epi32 (v1, 8), v1), bias_v), 2 * HB_RASTER_PIXEL_بٹ + 1);

    __m128i h = _mm_packs_epi32 (r0, r1);
    __m128i b = _mm_packus_epi16 (h, h);
    _mm_storel_epi64 ((__m128i *) (فراغ *) (صف_buf + x), b);

    _mm_storeu_si128 ((__m128i *) (فراغ *) (area + x),     zero_v);
    _mm_storeu_si128 ((__m128i *) (فراغ *) (area + x + 4), zero_v);
    _mm_storeu_si128 ((__m128i *) (فراغ *) (cover + x),    zero_v);
  }
#جے_انت

  لكل (; x <= x_max; x++)
  {
    cover_accum += cover[x];
    int32_t val   = cover_accum * cover_scale - area[x];
    int32_t alpha = val < 0 ? -val : val;
    إذا (alpha > HB_RASTER_بھریا_COVERAGE) alpha = HB_RASTER_بھریا_COVERAGE;
    صف_buf[x] = (uint8_t) (((بنانشانی) alpha * 255 + HB_RASTER_بھریا_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_بٹ + 1));
    area[x]  = 0;
    cover[x] = 0;
  }

  أرجع cover_accum;
}

ساكن int64_t
sweep_صف_منزل_alpha_wide (uint8_t *__restrict صف_buf,
			 int64_t *__restrict area,
			 int64_t *__restrict cover,
			 بنانشانی x_min,
			 بنانشانی x_max)
{
  ثابت int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
  int64_t cover_accum = 0;

  لكل (بنانشانی x = x_min; x <= x_max; x++)
  {
    cover_accum += cover[x];
    int64_t val = cover_accum * cover_scale - area[x];
    int64_t alpha = val < 0 ? -val : val;
    إذا (alpha > HB_RASTER_بھریا_COVERAGE) alpha = HB_RASTER_بھریا_COVERAGE;
    صف_buf[x] = (uint8_t) (((بنانشانی) alpha * 255 + HB_RASTER_بھریا_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_بٹ + 1));
    area[x] = 0;
    cover[x] = 0;
  }

  أرجع cover_accum;
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

  إذا (draw->has_extents)
  {
    ext = draw->fixed_extents;
  }
  وإلا
  {
    /* Auto-size from edge bounding box */
    إذا (draw->کنارے.لمائی == 0)
    {
      /* No edges: produce 0×0 image */
      ext = { 0, 0, 0, 0, 0 };
    }
    وإلا
    {
      int32_t xmin = draw->کنارے.arrayZ[0].xL, xmax = draw->کنارے.arrayZ[0].xL;
      int32_t ymin = draw->کنارے.arrayZ[0].yL, ymax = draw->کنارے.arrayZ[0].yH;

      لكل (ثابت خودکار &e : draw->کنارے)
      {
	xmin = hb_min (xmin, hb_min (e.xL, e.xH));
	xmax = hb_max (xmax, hb_max (e.xL, e.xH));
	ymin = hb_min (ymin, e.yL);
	ymax = hb_max (ymax, e.yH);
      }

      /* Convert fixed-point → pixels (floor for min, ceil for max).  Edge
	 coordinates are saturated to int32 range in emit_segment, so the
	 +MASK ceil step must be widened to avoid signed overflow. */
      صحيح x0 = xmin >> HB_RASTER_PIXEL_بٹ;
      صحيح y0 = ymin >> HB_RASTER_PIXEL_بٹ;
      صحيح x1 = (صحيح) (((int64_t) xmax + HB_RASTER_PIXEL_نقاب) >> HB_RASTER_PIXEL_بٹ);
      صحيح y1 = (صحيح) (((int64_t) ymax + HB_RASTER_PIXEL_نقاب) >> HB_RASTER_PIXEL_بٹ);

      ext.x_origin = x0;
      ext.y_origin = y0;
      ext.چوڑائی    = (بنانشانی) hb_min (hb_max (0, x1 - x0), HB_RASTER_MAX_AUTO_DIMENSION);
      ext.اچائی   = (بنانشانی) hb_min (hb_max (0, y1 - y0), HB_RASTER_MAX_AUTO_DIMENSION);
      ext.stride   = 0; /* filled below */
    }
  }

  /* ── 2. Compute stride ─────────────────────────────────────────── */
  إذا (ext.stride == 0)
    ext.stride = (ext.چوڑائی + 3u) & ~3u;

  /* ── 3. Allocate or reuse image ─────────────────────────────────── */
  /* Reset one-shot state on every exit path. */
  HB_SCOPE_GUARD (hb_raster_draw_clear (draw));

  hb_unique_ptr_t<hb_raster_image_t> image;
  إذا (draw->recycled_image)
  {
    image = hb_unique_ptr_t<hb_raster_image_t> (draw->recycled_image);
    draw->recycled_image = خالی_سوچک;
  }
  وإلا
  {
    image = hb_unique_ptr_t<hb_raster_image_t> (hb_raster_image_create_or_fail ());
    إذا (unlikely (!image)) أرجع خالی_سوچک;
  }

  إذا (unlikely (!image->configure (HB_RASTER_FORMAT_A8, ext)))
    أرجع خالی_سوچک;
  image->clear ();

  /* ── 4. Bucket edges by starting row and rasterize scanlines ──── */
  إذا (draw->کنارے.لمائی && ext.چوڑائی && ext.اچائی)
  {
    إذا (unlikely (!draw->صف_area.resize_dirty (ext.چوڑائی) ||
		  !draw->صف_cover.resize_dirty (ext.چوڑائی)))
      أرجع خالی_سوچک;
    hb_memset (draw->صف_area.arrayZ,  0, ext.چوڑائی * حجم (int32_t));
    hb_memset (draw->صف_cover.arrayZ, 0, ext.چوڑائی * حجم (int16_t));

    /* Bucket edges by their starting pixel row.
       Only grow the outer vector; clear inner vectors without freeing. */
    بنانشانی old_buckets = draw->کنارا_buckets.لمائی;
    إذا (ext.اچائی > old_buckets)
    {
      إذا (unlikely (!draw->کنارا_buckets.resize (ext.اچائی)))
	أرجع خالی_سوچک;
    }
    لكل (بنانشانی i = 0; i < hb_min (ext.اچائی, old_buckets); i++)
      draw->کنارا_buckets.arrayZ[i].clear ();
    /* New buckets (if any) are already empty from resize's zero-init. */

    لكل (بنانشانی i = 0; i < draw->کنارے.لمائی; i++)
    {
      صحيح صف = (draw->کنارے.arrayZ[i].yL >> HB_RASTER_PIXEL_بٹ) - ext.y_origin;
      إذا (صف < 0) صف = 0;
      إذا ((بنانشانی) صف >= ext.اچائی) استمر;
      draw->کنارا_buckets.arrayZ[صف].push (i);
    }

    /* Scanline loop with active edge list. */
    draw->سرگرم_کنارے.clear ();
    منطقي wide_initialized = خطأ;

    لكل (بنانشانی صف = 0; صف < ext.اچائی; صف++)
    {
      int64_t y_چوٹی_64 = ((int64_t) ext.y_origin + (int64_t) صف) * HB_RASTER_ONE_PIXEL;
      int32_t y_چوٹی = (int32_t) hb_clamp (y_چوٹی_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);

      /* Add new edges from this row's bucket. */
      draw->سرگرم_کنارے.extend (draw->کنارا_buckets.arrayZ[صف]);

      /* Process active edges and compact live ones in one linear pass. */
      بنانشانی x_min = ext.چوڑائی, x_max = 0;
      بنانشانی اكتب = 0;
      بنانشانی سرگرم_len = draw->سرگرم_کنارے.لمائی;
      منطقي use_wide = سرگرم_len > HB_RASTER_NARROW_MAX_سرگرم_کنارے;
      إذا (unlikely (use_wide && !wide_initialized))
      {
	إذا (unlikely (!draw->صف_area_wide.resize_dirty (ext.چوڑائی) ||
		      !draw->صف_cover_wide.resize_dirty (ext.چوڑائی)))
	  أرجع خالی_سوچک;
	hb_memset (draw->صف_area_wide.arrayZ,  0, ext.چوڑائی * حجم (int64_t));
	hb_memset (draw->صف_cover_wide.arrayZ, 0, ext.چوڑائی * حجم (int64_t));
	wide_initialized = سچ;
      }
      لكل (بنانشانی j = 0; j < سرگرم_len; j++)
      {
	بنانشانی کنارا_idx = draw->سرگرم_کنارے.arrayZ[j];
	ثابت خودکار &e = draw->کنارے.arrayZ[کنارا_idx];
	إذا (e.yH <= y_چوٹی)
	  استمر;

	إذا (unlikely (use_wide))
	  کنارا_sweep_صف (draw->صف_area_wide.arrayZ, draw->صف_cover_wide.arrayZ,
			  ext.چوڑائی, ext.x_origin, y_چوٹی, e, x_min, x_max);
	وإلا
	  کنارا_sweep_صف (draw->صف_area.arrayZ, draw->صف_cover.arrayZ,
			  ext.چوڑائی, ext.x_origin, y_چوٹی, e, x_min, x_max);
	draw->سرگرم_کنارے.arrayZ[اكتب++] = کنارا_idx;
      }
      draw->سرگرم_کنارے.resize (اكتب);

      إذا (x_min <= x_max)
      {
	int64_t cover_accum = use_wide ?
	  sweep_صف_منزل_alpha_wide (image->بفر.arrayZ + صف * ext.stride,
				   draw->صف_area_wide.arrayZ, draw->صف_cover_wide.arrayZ,
				   x_min, x_max) :
	  sweep_صف_منزل_alpha (image->بفر.arrayZ + صف * ext.stride,
			      draw->صف_area.arrayZ, draw->صف_cover.arrayZ,
			      x_min, x_max);

	/* If cover doesn't cancel, memset the constant-alpha tail. */
	إذا (cover_accum != 0)
	{
	  int64_t alpha = cover_accum * (2 * HB_RASTER_ONE_PIXEL);
	  alpha = alpha < 0 ? -alpha : alpha;
	  إذا (alpha > HB_RASTER_بھریا_COVERAGE) alpha = HB_RASTER_بھریا_COVERAGE;
	  uint8_t byte = (uint8_t) (((بنانشانی) alpha * 255 + HB_RASTER_بھریا_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_بٹ + 1));

	  uint8_t *صف_buf = image->بفر.arrayZ + صف * ext.stride;
	  hb_memset (صف_buf + x_max + 1, byte, ext.چوڑائی - 1 - x_max);
	}
      }
    }
  }

  أرجع image.release ();
}
