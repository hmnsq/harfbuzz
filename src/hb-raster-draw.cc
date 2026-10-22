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

#شامل "hb.hh"

#شامل "hb-raster-image.hh"
#شامل "hb-geometry.hh"
#شامل "hb-machinery.hh"

#اگر defined(__aarch64__) || defined(_M_ARM64)
#شامل <arm_neon.h>
#تعریف_کرو HB_RASTER_NEON 1
#وگرنہ_اگر defined(__SSE2__) || defined(_M_X64) || defined(_M_AMD64)
#شامل <emmintrin.h>
#تعریف_کرو HB_RASTER_SSE2 1
#اگر_ختم


/* Fixed-point precision for sub-pixel coordinates.
   8 bits = 24.8: 256 sub-pixel units per pixel. */
#تعریف_کرو HB_RASTER_PIXEL_بٹ 8
#تعریف_کرو HB_RASTER_ONE_PIXEL  (1 << HB_RASTER_PIXEL_بٹ)
#تعریف_کرو HB_RASTER_PIXEL_نقاب (HB_RASTER_ONE_PIXEL - 1)
/* Full-coverage alpha = 2 * ONE_PIXEL^2 */
#تعریف_کرو HB_RASTER_بھرا_COVERAGE (2 * HB_RASTER_ONE_PIXEL * HB_RASTER_ONE_PIXEL)
/* Flatness threshold for Bézier flattening: max deviation in pixels */
#تعریف_کرو HB_RASTER_FLAT_THRESH 0.25f


/* Normalized edge: yH > yL always */
ساخت hb_raster_کنارہ_t
{
  int32_t xL, yL;   /* lower endpoint (fixed-point) */
  int32_t xH, yH;   /* upper endpoint (fixed-point) */
  int64_t ڈھلان;    /* dx/dy in 16.16 fixed point: ((int64_t)dx << 16) / dy */
  int32_t wind;     /* +1 or -1 */
};

/* hb_raster_draw_t — outline rasterizer */
ساخت hb_raster_draw_t
{
  hb_object_سرخی_t سرخی;

  /* Configuration */
  hb_transform_t<>    transform         = {1, 0, 0, 1, 0, 0};
  اعشاری               x_scale_ضربی_عدد    = 1.f;
  اعشاری               y_scale_ضربی_عدد    = 1.f;
  hb_raster_extents_t fixed_extents     = {};
  منطقی                has_extents = جھوٹ;

  /* Visibility clip box for curve flattening (device pixels); set
     internally by raster-paint so invisible curves collapse to their
     chord.  See hb_raster_draw_set_clip_box(). */
  منطقی  has_clip_box = جھوٹ;
  اعشاری clip_x0 = 0.f, clip_y0 = 0.f, clip_x1 = 0.f, clip_y1 = 0.f;

  /* Flattening clip resolved from the above (or from fixed extents),
     expanded by one pixel; kept in sync by hb_raster_draw_update_flatten_clip(). */
  منطقی  flatten_clip_فعال = جھوٹ;
  اعشاری flatten_clip_x0 = 0.f, flatten_clip_y0 = 0.f;
  اعشاری flatten_clip_x1 = 0.f, flatten_clip_y1 = 0.f;

  /* Work budget for outline traversal and curve flattening.  A paint
     backend seeds this through the public draw-budget API before drawing
     and reads the remainder back afterwards, so a whole paint session
     shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_بقایا = HB_BUDGET_GLYPH;

  خالی recharge_budget ()
  {
    budget_بقایا = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_بقایا ()
  { واپس &budget_بقایا; }

  /* Accumulated geometry */
  int64_t کنارے_بایاں = HB_RASTER_MAX_DRAW_کنارے;
  hb_سمتیہ_t<hb_raster_کنارہ_t> کنارے;

  /* Scratch — reused across render() calls */
  hb_سمتیہ_t<int32_t> صف_area;
  hb_سمتیہ_t<int16_t> صف_cover;
  hb_سمتیہ_t<int64_t> صف_area_wide;
  hb_سمتیہ_t<int64_t> صف_cover_wide;
  hb_سمتیہ_t<hb_سمتیہ_t<غیرعلامتی>> کنارہ_buckets;
  hb_سمتیہ_t<غیرعلامتی> فعال_کنارے;

  /* Recycled image for zero-malloc render */
  hb_raster_image_t *recycled_image = خالی_اشاریہ;
};

ساکن HB_ALWAYS_INLINE خالی
hb_raster_draw_transform_نقطہ (ثابت hb_raster_draw_t *draw,
				اعشاری x, اعشاری y,
				اعشاری &tx, اعشاری &ty)
{
  tx = x; ty = y;
  draw->transform.transform_نقطہ (tx, ty);
  tx /= draw->x_scale_ضربی_عدد;
  ty /= draw->y_scale_ضربی_عدد;
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
hb_raster_draw_create_or_fail (خالی)
{
  hb_raster_draw_t *draw = hb_object_create<hb_raster_draw_t> ();
  واپس draw;
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
  واپس hb_object_reference (draw);
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
خالی
hb_raster_draw_destroy (hb_raster_draw_t *draw)
{
  اگر (!hb_object_should_destroy (draw))
    واپس;

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
hb_raster_draw_سیٹ_user_ڈیٹا (hb_raster_draw_t   *draw,
			      hb_user_ڈیٹا_کلید_t *key,
			      خالی               *ڈیٹا,
			      hb_destroy_func_t   destroy,
			      hb_bool_t           replace)
{
  واپس hb_object_سیٹ_user_ڈیٹا (draw, key, ڈیٹا, destroy, replace);
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
خالی *
hb_raster_draw_get_user_ڈیٹا (ثابت hb_raster_draw_t   *draw,
			      hb_user_ڈیٹا_کلید_t *key)
{
  واپس hb_object_get_user_ڈیٹا (draw, key);
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
خالی
hb_raster_draw_سیٹ_transform (hb_raster_draw_t *draw,
			      اعشاری xx, اعشاری yx,
			      اعشاری xy, اعشاری yy,
			      اعشاری dx, اعشاری dy)
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
خالی
hb_raster_draw_سیٹ_scale_ضربی_عدد (hb_raster_draw_t *draw,
				 اعشاری x_scale_ضربی_عدد,
				 اعشاری y_scale_ضربی_عدد)
{
  draw->x_scale_ضربی_عدد = x_scale_ضربی_عدد > 0.f ? x_scale_ضربی_عدد : 1.f;
  draw->y_scale_ضربی_عدد = y_scale_ضربی_عدد > 0.f ? y_scale_ضربی_عدد : 1.f;
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
خالی
hb_raster_draw_get_scale_ضربی_عدد (ثابت hb_raster_draw_t *draw,
				 اعشاری *x_scale_ضربی_عدد,
				 اعشاری *y_scale_ضربی_عدد)
{
  اگر (x_scale_ضربی_عدد) *x_scale_ضربی_عدد = draw->x_scale_ضربی_عدد;
  اگر (y_scale_ضربی_عدد) *y_scale_ضربی_عدد = draw->y_scale_ضربی_عدد;
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
خالی
hb_raster_draw_get_transform (ثابت hb_raster_draw_t *draw,
			      اعشاری *xx, اعشاری *yx,
			      اعشاری *xy, اعشاری *yy,
			      اعشاری *dx, اعشاری *dy)
{
  اگر (xx) *xx = draw->transform.xx;
  اگر (yx) *yx = draw->transform.yx;
  اگر (xy) *xy = draw->transform.xy;
  اگر (yy) *yy = draw->transform.yy;
  اگر (dx) *dx = draw->transform.x0;
  اگر (dy) *dy = draw->transform.y0;
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
ساکن خالی
hb_raster_draw_update_flatten_clip (hb_raster_draw_t *draw)
{
  اگر (draw->has_clip_box)
  {
    draw->flatten_clip_فعال = سچ;
    draw->flatten_clip_x0 = draw->clip_x0 - 1.f;
    draw->flatten_clip_y0 = draw->clip_y0 - 1.f;
    draw->flatten_clip_x1 = draw->clip_x1 + 1.f;
    draw->flatten_clip_y1 = draw->clip_y1 + 1.f;
  }
  ورنہ اگر (draw->has_extents)
  {
    draw->flatten_clip_فعال = سچ;
    draw->flatten_clip_x0 = (اعشاری) draw->fixed_extents.x_origin - 1.f;
    draw->flatten_clip_y0 = (اعشاری) draw->fixed_extents.y_origin - 1.f;
    draw->flatten_clip_x1 = (اعشاری) draw->fixed_extents.x_origin + (اعشاری) draw->fixed_extents.چوڑائی + 1.f;
    draw->flatten_clip_y1 = (اعشاری) draw->fixed_extents.y_origin + (اعشاری) draw->fixed_extents.اونچائی + 1.f;
  }
  ورنہ
    draw->flatten_clip_فعال = جھوٹ;
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
خالی
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
  اگر (!draw->has_extents)
    واپس جھوٹ;

  اگر (extents)
    *extents = draw->fixed_extents;
  واپس سچ;
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
  اعشاری x0 = (اعشاری) glyph_extents->x_bearing;
  اعشاری y0 = (اعشاری) glyph_extents->y_bearing;
  اعشاری x1 = (اعشاری) glyph_extents->x_bearing + glyph_extents->چوڑائی;
  اعشاری y1 = (اعشاری) glyph_extents->y_bearing + glyph_extents->اونچائی;

  اعشاری xmin = hb_min (x0, x1);
  اعشاری xmax = hb_max (x0, x1);
  اعشاری ymin = hb_min (y0, y1);
  اعشاری ymax = hb_max (y0, y1);

  اعشاری px[4] = {xmin, xmin, xmax, xmax};
  اعشاری py[4] = {ymin, ymax, ymin, ymax};

  اعشاری tx, ty;
  hb_raster_draw_transform_نقطہ (draw, px[0], py[0], tx, ty);
  اعشاری tx_min = tx, tx_max = tx;
  اعشاری ty_min = ty, ty_max = ty;

  برائے (غیرعلامتی i = 1; i < 4; i++)
  {
    hb_raster_draw_transform_نقطہ (draw, px[i], py[i], tx, ty);
    tx_min = hb_min (tx_min, tx);
    tx_max = hb_max (tx_max, tx);
    ty_min = hb_min (ty_min, ty);
    ty_max = hb_max (ty_max, ty);
  }

  int32_t ex0 = hb_clamp_مقصد<int32_t> (floorf (tx_min));
  int32_t ey0 = hb_clamp_مقصد<int32_t> (floorf (ty_min));
  int32_t ex1 = hb_clamp_مقصد<int32_t> (ceilf  (tx_max));
  int32_t ey1 = hb_clamp_مقصد<int32_t> (ceilf  (ty_max));

  اگر (ex1 <= ex0 || ey1 <= ey0)
  {
    draw->fixed_extents = {};
    draw->has_extents = جھوٹ;
    hb_raster_draw_update_flatten_clip (draw);
    واپس جھوٹ;
  }

  draw->fixed_extents = {
    ex0, ey0,
    (غیرعلامتی) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (غیرعلامتی) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  draw->has_extents = سچ;
  hb_raster_draw_update_flatten_clip (draw);
  واپس سچ;
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
خالی
hb_raster_draw_clear (hb_raster_draw_t *draw)
{
  draw->fixed_extents     = {};
  draw->has_extents = جھوٹ;
  draw->has_clip_box = جھوٹ;
  draw->flatten_clip_فعال = جھوٹ;
  draw->recharge_budget ();
  draw->کنارے_بایاں = HB_RASTER_MAX_DRAW_کنارے;
  draw->کنارے.clear ();
  draw->فعال_کنارے.clear ();
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
خالی
hb_raster_draw_reset (hb_raster_draw_t *draw)
{
  draw->transform         = {1, 0, 0, 1, 0, 0};
  draw->x_scale_ضربی_عدد    = 1.f;
  draw->y_scale_ضربی_عدد    = 1.f;
  draw->budget            = HB_BUDGET_DEFAULT;
  hb_raster_draw_clear (draw);
}

خالی
hb_raster_draw_سیٹ_clip_box (hb_raster_draw_t *draw,
			     اعشاری x0, اعشاری y0,
			     اعشاری x1, اعشاری y1)
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
			       غیرعلامتی صحیح max_صفیں,
			       غیرعلامتی صحیح max_ستون_تعداد)
{
  int64_t work = 0;
  برائے (ثابت خودکار &کنارہ : draw->کنارے)
  {
    int64_t dx = (int64_t) کنارہ.xH - کنارہ.xL;
    اگر (dx < 0) dx = -dx;
    work += 1 + hb_min (((int64_t) کنارہ.yH - کنارہ.yL) >> HB_RASTER_PIXEL_بٹ,
			(int64_t) max_صفیں)
	      + hb_min (dx >> HB_RASTER_PIXEL_بٹ,
			(int64_t) max_ستون_تعداد);
  }
  واپس work;
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
خالی
hb_raster_draw_recycle_image (hb_raster_draw_t  *draw,
			      hb_raster_image_t *image)
{
  hb_raster_image_destroy (draw->recycled_image);
  draw->recycled_image = image;
}


/*
 * Draw callbacks — flatten on the fly into hb_raster_edge_t
 */

ساکن اندرونی خالی
transform_نقطہ (ثابت hb_raster_draw_t *draw,
		 اعشاری  x,  اعشاری  y,
		 اعشاری &tx, اعشاری &ty)
{
  hb_raster_draw_transform_نقطہ (draw, x, y, tx, ty);
}

ساکن خالی
emit_segment (hb_raster_draw_t *draw,
	      اعشاری x0, اعشاری y0,
	      اعشاری x1, اعشاری y1)
{
  اگر (unlikely (draw->کنارے_بایاں <= 0))
    واپس;

  int32_t X0 = hb_clamp_مقصد<int32_t> (roundf (x0 * HB_RASTER_ONE_PIXEL));
  int32_t Y0 = hb_clamp_مقصد<int32_t> (roundf (y0 * HB_RASTER_ONE_PIXEL));
  int32_t X1 = hb_clamp_مقصد<int32_t> (roundf (x1 * HB_RASTER_ONE_PIXEL));
  int32_t Y1 = hb_clamp_مقصد<int32_t> (roundf (y1 * HB_RASTER_ONE_PIXEL));

  اگر (Y0 == Y1) واپس; /* horizontal — skip */

  hb_raster_کنارہ_t e;
  اگر (Y0 < Y1) {
    e.xL = X0; e.yL = Y0; e.xH = X1; e.yH = Y1; e.wind = +1;
  } ورنہ {
    e.xL = X1; e.yL = Y1; e.xH = X0; e.yH = Y0; e.wind = -1;
  }
  e.ڈھلان = (((int64_t) e.xH - (int64_t) e.xL) * (int64_t) 65536) /
	    ((int64_t) e.yH - (int64_t) e.yL);

  اگر (likely (draw->کنارے.ڈالو_or_fail (e)))
    draw->کنارے_بایاں--;
  ورنہ
    draw->کنارے_بایاں = 0;
}


/* Quadratic Bézier flattener — iterative de Casteljau at t=0.5. */
ساکن اندرونی خالی
flatten_quadratic_recursive (hb_raster_draw_t *draw,
			     اعشاری x0, اعشاری y0,
			     اعشاری x1, اعشاری y1,
			     اعشاری x2, اعشاری y2,
			     صحیح گہرائی = 0)
{
  ساخت quad_گرہ_t
  {
    اعشاری x0, y0, x1, y1, x2, y2;
    صحیح گہرائی;
    منطقی check_clip;
  };

  quad_گرہ_t ڈھیر[16];
  غیرعلامتی چوٹی_2 = 0;

  منطقی check_clip = draw->flatten_clip_فعال;
  int64_t *work = draw->get_budget_بقایا ();
  int64_t work_بایاں = *work;

  جب (سچ)
  {
    منطقی emit_chord = گہرائی >= 16;

    /* Entirely outside the clip box: the chord is exact for coverage.
       Subdivided control points are convex combinations of their
       parent's, so once a node is entirely inside the box no
       descendant can leave it and the test is skipped below. */
    اگر (!emit_chord && check_clip)
    {
      اعشاری bx0 = hb_min (x0, hb_min (x1, x2));
      اعشاری bx1 = hb_max (x0, hb_max (x1, x2));
      اعشاری by0 = hb_min (y0, hb_min (y1, y2));
      اعشاری by1 = hb_max (y0, hb_max (y1, y2));
      اگر (bx1 < draw->flatten_clip_x0 || bx0 > draw->flatten_clip_x1 ||
	  by1 < draw->flatten_clip_y0 || by0 > draw->flatten_clip_y1)
	emit_chord = سچ;
      ورنہ اگر (bx0 >= draw->flatten_clip_x0 && bx1 <= draw->flatten_clip_x1 &&
	       by0 >= draw->flatten_clip_y0 && by1 <= draw->flatten_clip_y1)
	check_clip = جھوٹ;
    }

    اگر (!emit_chord)
    {
      منطقی is_flat;
      اگر (جھوٹ)
      {
	/* Old behavior: midpoint deviation from chord midpoint. */
	اعشاری mx = x0 * 0.25f + x1 * 0.5f + x2 * 0.25f;
	اعشاری my = y0 * 0.25f + y1 * 0.5f + y2 * 0.25f;
	اعشاری chord_mx = (x0 + x2) * 0.5f;
	اعشاری chord_my = (y0 + y2) * 0.5f;
	اعشاری dx = mx - chord_mx;
	اعشاری dy = my - chord_my;
	ساکن ثابت اعشاری flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;
	is_flat = (dx * dx + dy * dy) <= flat_thresh;
      }
      ورنہ
      {
	/* FreeType behavior: control-point deviation from chord center. */
	ثابت اعشاری flat_thresh = 0.25f;
	اعشاری dx = x0 + x2 - 2.f * x1;
	اعشاری dy = y0 + y2 - 2.f * y1;
	اگر (dx < 0) dx = -dx;
	اگر (dy < 0) dy = -dy;
	is_flat = dx <= flat_thresh && dy <= flat_thresh;
      }
      emit_chord = is_flat;
    }

    /* Charge one unit per subdivision; degrade to the chord when the
       session work budget is spent. */
    اگر (!emit_chord)
    {
      اگر (unlikely (work_بایاں <= 0))
	emit_chord = سچ;
      ورنہ
	work_بایاں--;
    }

    اگر (emit_chord)
    {
      emit_segment (draw, x0, y0, x2, y2);
      اگر (!چوٹی_2)
      {
	*work = work_بایاں;
	واپس;
      }
      ثابت quad_گرہ_t &n = ڈھیر[--چوٹی_2];
      x0 = n.x0; y0 = n.y0;
      x1 = n.x1; y1 = n.y1;
      x2 = n.x2; y2 = n.y2;
      گہرائی = n.گہرائی;
      check_clip = n.check_clip;
      جاری;
    }

    اعشاری x01 = (x0 + x1) * 0.5f, y01 = (y0 + y1) * 0.5f;
    اعشاری x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
    اعشاری xm  = (x01 + x12) * 0.5f, ym  = (y01 + y12) * 0.5f;

    /* Depth is capped at 16, so stack capacity 16 is sufficient. */
    ڈھیر[چوٹی_2++] = {xm, ym, x12, y12, x2, y2, گہرائی + 1, check_clip};
    x2 = xm; y2 = ym;
    x1 = x01; y1 = y01;
    گہرائی++;
  }
}

/* Quadratic Bézier flattener using forward differencing.
   The error (midpoint deviation) shrinks exactly 4× per de Casteljau
   subdivision, so we compute the subdivision count upfront and iterate
   with constant-cost additions instead of recursive branching. */
ساکن اندرونی خالی
flatten_quadratic_fd (hb_raster_draw_t *draw,
		      اعشاری x0, اعشاری y0,
		      اعشاری x1, اعشاری y1,
		      اعشاری x2, اعشاری y2)
{
  /* Deviation of curve midpoint from chord midpoint (squared). */
  اعشاری devx = (x0 - 2 * x1 + x2) * 0.25f;
  اعشاری devy = (y0 - 2 * y1 + y2) * 0.25f;
  اعشاری err2 = devx * devx + devy * devy;

  ساکن ثابت اعشاری flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;

  اگر (err2 <= flat_thresh)
  {
    emit_segment (draw, x0, y0, x2, y2);
    واپس;
  }

  /* err² shrinks 16× per subdivision level.  Find n such that
     err2 / 16^n <= flat_thresh, i.e. n = ceil(log₁₆(err2/flat_thresh)). */
  غیرعلامتی n = 1;
  {
    اعشاری تناسب = err2 / flat_thresh;
    جب (تناسب > 16.f) { تناسب *= (1.f / 16.f); n++; }
    اگر (n > 16) n = 16;
  }
  غیرعلامتی N = 1u << n;   /* number of line segments */
  اعشاری h = 1.f / N;

  /* Quadratic: B(t) = a·t² + b·t + c
     Forward differences with step h:
       d²f = 2·a·h²   (constant)
       df₀ = a·h² + b·h
       f₀  = c = P₀                     */
  اعشاری ax = x0 - 2 * x1 + x2;
  اعشاری ay = y0 - 2 * y1 + y2;
  اعشاری bx = 2 * (x1 - x0);
  اعشاری by = 2 * (y1 - y0);

  اعشاری d2fx = 2 * ax * h * h;
  اعشاری d2fy = 2 * ay * h * h;
  اعشاری dfx  = ax * h * h + bx * h;
  اعشاری dfy  = ay * h * h + by * h;
  اعشاری fx   = x0;
  اعشاری fy   = y0;

  برائے (غیرعلامتی i = 1; i < N; i++)
  {
    اعشاری nx = fx + dfx;
    اعشاری ny = fy + dfy;
    emit_segment (draw, fx, fy, nx, ny);
    fx = nx;
    fy = ny;
    dfx += d2fx;
    dfy += d2fy;
  }
  /* Last segment uses exact endpoint to avoid drift. */
  emit_segment (draw, fx, fy, x2, y2);
}

ساکن خالی
flatten_quadratic (hb_raster_draw_t *draw,
		   اعشاری x0, اعشاری y0,
		   اعشاری x1, اعشاری y1,
		   اعشاری x2, اعشاری y2)
{
  اگر (جھوٹ)
    flatten_quadratic_fd (draw, x0, y0, x1, y1, x2, y2);
  ورنہ
    flatten_quadratic_recursive (draw, x0, y0, x1, y1, x2, y2);
}

/* For cubic B(t), the max deviation from its chord on [0,1] is bounded by:
     max||B(t)-L(t)|| <= max_t ||B''(t)|| / 8.
   B''(t) is linear, so max norm is attained at t=0 or t=1:
     B''(0)=6*(P0-2P1+P2), B''(1)=6*(P1-2P2+P3). */
ساکن اندرونی اعشاری
cubic_chord_error_bound2 (اعشاری x0, اعشاری y0,
			  اعشاری x1, اعشاری y1,
			  اعشاری x2, اعشاری y2,
			  اعشاری x3, اعشاری y3)
{
  اعشاری d20x = x0 - 2 * x1 + x2;
  اعشاری d20y = y0 - 2 * y1 + y2;
  اعشاری d21x = x1 - 2 * x2 + x3;
  اعشاری d21y = y1 - 2 * y2 + y3;
  اعشاری m0 = d20x * d20x + d20y * d20y;
  اعشاری m1 = d21x * d21x + d21y * d21y;
  اعشاری m = m0 > m1 ? m0 : m1;
  /* (max||B''||/8)^2 = (6/8)^2 * max||d2||^2 = (3/4)^2 * m. */
  واپس m * (9.f / 16.f);
}

/* Cubic Bézier flattener — iterative de Casteljau at t=0.5. */
ساکن اندرونی خالی
flatten_cubic_recursive (hb_raster_draw_t *draw,
			 اعشاری x0, اعشاری y0,
			 اعشاری x1, اعشاری y1,
			 اعشاری x2, اعشاری y2,
			 اعشاری x3, اعشاری y3,
			 صحیح گہرائی = 0)
{
  ساخت cubic_گرہ_t
  {
    اعشاری x0, y0, x1, y1, x2, y2, x3, y3;
    صحیح گہرائی;
    منطقی check_clip;
  };

  cubic_گرہ_t ڈھیر[16];
  غیرعلامتی چوٹی_2 = 0;

  منطقی check_clip = draw->flatten_clip_فعال;
  int64_t *work = draw->get_budget_بقایا ();
  int64_t work_بایاں = *work;

  جب (سچ)
  {
    منطقی emit_chord = گہرائی >= 16;

    /* Entirely outside the clip box: the chord is exact for coverage.
       Subdivided control points are convex combinations of their
       parent's, so once a node is entirely inside the box no
       descendant can leave it and the test is skipped below. */
    اگر (!emit_chord && check_clip)
    {
      اعشاری bx0 = hb_min (hb_min (x0, x1), hb_min (x2, x3));
      اعشاری bx1 = hb_max (hb_max (x0, x1), hb_max (x2, x3));
      اعشاری by0 = hb_min (hb_min (y0, y1), hb_min (y2, y3));
      اعشاری by1 = hb_max (hb_max (y0, y1), hb_max (y2, y3));
      اگر (bx1 < draw->flatten_clip_x0 || bx0 > draw->flatten_clip_x1 ||
	  by1 < draw->flatten_clip_y0 || by0 > draw->flatten_clip_y1)
	emit_chord = سچ;
      ورنہ اگر (bx0 >= draw->flatten_clip_x0 && bx1 <= draw->flatten_clip_x1 &&
	       by0 >= draw->flatten_clip_y0 && by1 <= draw->flatten_clip_y1)
	check_clip = جھوٹ;
    }

    اگر (!emit_chord)
    {
      منطقی is_flat;
      اگر (جھوٹ)
      {
	/* Old behavior: curvature/chord-error bound. */
	اعشاری err2 = cubic_chord_error_bound2 (x0, y0, x1, y1, x2, y2, x3, y3);
	ساکن ثابت اعشاری flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;
	is_flat = err2 <= flat_thresh;
      }
      ورنہ
      {
	/* FreeType behavior: chord-trisection distance test. */
	ثابت اعشاری flat_thresh = 0.5f;

	اعشاری d10x = 2.f * x0 - 3.f * x1 + x3;
	اعشاری d10y = 2.f * y0 - 3.f * y1 + y3;
	اعشاری d20x = x0 - 3.f * x2 + 2.f * x3;
	اعشاری d20y = y0 - 3.f * y2 + 2.f * y3;

	اگر (d10x < 0) d10x = -d10x;
	اگر (d10y < 0) d10y = -d10y;
	اگر (d20x < 0) d20x = -d20x;
	اگر (d20y < 0) d20y = -d20y;

	is_flat = d10x <= flat_thresh &&
		  d10y <= flat_thresh &&
		  d20x <= flat_thresh &&
		  d20y <= flat_thresh;
      }
      emit_chord = is_flat;
    }

    /* Charge one unit per subdivision; degrade to the chord when the
       session work budget is spent. */
    اگر (!emit_chord)
    {
      اگر (unlikely (work_بایاں <= 0))
	emit_chord = سچ;
      ورنہ
	work_بایاں--;
    }

    اگر (emit_chord)
    {
      emit_segment (draw, x0, y0, x3, y3);
      اگر (!چوٹی_2)
      {
	*work = work_بایاں;
	واپس;
      }
      ثابت cubic_گرہ_t &n = ڈھیر[--چوٹی_2];
      x0 = n.x0; y0 = n.y0;
      x1 = n.x1; y1 = n.y1;
      x2 = n.x2; y2 = n.y2;
      x3 = n.x3; y3 = n.y3;
      گہرائی = n.گہرائی;
      check_clip = n.check_clip;
      جاری;
    }

    اعشاری x01  = (x0 + x1) * 0.5f, y01  = (y0 + y1) * 0.5f;
    اعشاری x12  = (x1 + x2) * 0.5f, y12  = (y1 + y2) * 0.5f;
    اعشاری x23  = (x2 + x3) * 0.5f, y23  = (y2 + y3) * 0.5f;
    اعشاری x012 = (x01 + x12) * 0.5f, y012 = (y01 + y12) * 0.5f;
    اعشاری x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
    اعشاری xm   = (x012 + x123) * 0.5f, ym   = (y012 + y123) * 0.5f;

    /* Depth is capped at 16, so stack capacity 16 is sufficient. */
    ڈھیر[چوٹی_2++] = {xm, ym, x123, y123, x23, y23, x3, y3, گہرائی + 1, check_clip};
    x3 = xm; y3 = ym;
    x2 = x012; y2 = y012;
    x1 = x01; y1 = y01;
    گہرائی++;
  }
}

/* Cubic Bézier flattener using forward differencing.
   Use a curvature-based chord-error bound (max||B''||/8), then choose a
   uniform subdivision count n such that the bound drops below threshold.
   The cubic adds a constant third difference d³f = 6·a·h³. */
ساکن اندرونی خالی
flatten_cubic_fd (hb_raster_draw_t *draw,
		  اعشاری x0, اعشاری y0,
		  اعشاری x1, اعشاری y1,
		  اعشاری x2, اعشاری y2,
		  اعشاری x3, اعشاری y3)
{
  اعشاری err2 = cubic_chord_error_bound2 (x0, y0, x1, y1, x2, y2, x3, y3);

  ساکن ثابت اعشاری flat_thresh = HB_RASTER_FLAT_THRESH * HB_RASTER_FLAT_THRESH;

  اگر (err2 <= flat_thresh)
  {
    emit_segment (draw, x0, y0, x3, y3);
    واپس;
  }

  /* The bound scales with h², so err² shrinks 16× per subdivision level. */
  غیرعلامتی n = 1;
  {
    اعشاری تناسب = err2 / flat_thresh;
    جب (تناسب > 16.f) { تناسب *= (1.f / 16.f); n++; }
    اگر (n > 16) n = 16;
  }
  غیرعلامتی N = 1u << n;
  اعشاری h = 1.f / N;

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
  اعشاری ax = -x0 + 3*x1 - 3*x2 + x3;
  اعشاری ay = -y0 + 3*y1 - 3*y2 + y3;
  اعشاری bx =  3*x0 - 6*x1 + 3*x2;
  اعشاری by =  3*y0 - 6*y1 + 3*y2;
  اعشاری cx =  3*(x1 - x0);
  اعشاری cy =  3*(y1 - y0);

  اعشاری h2 = h * h, h3 = h2 * h;
  اعشاری d3fx = 6 * ax * h3;
  اعشاری d3fy = 6 * ay * h3;
  اعشاری d2fx = d3fx + 2 * bx * h2;
  اعشاری d2fy = d3fy + 2 * by * h2;
  اعشاری dfx  = ax * h3 + bx * h2 + cx * h;
  اعشاری dfy  = ay * h3 + by * h2 + cy * h;
  اعشاری fx   = x0;
  اعشاری fy   = y0;

  برائے (غیرعلامتی i = 1; i < N; i++)
  {
    اعشاری nx = fx + dfx;
    اعشاری ny = fy + dfy;
    emit_segment (draw, fx, fy, nx, ny);
    fx = nx;  fy = ny;
    dfx += d2fx;  dfy += d2fy;
    d2fx += d3fx; d2fy += d3fy;
  }
  /* Last segment uses exact endpoint to avoid drift. */
  emit_segment (draw, fx, fy, x3, y3);
}

ساکن خالی
flatten_cubic (hb_raster_draw_t *draw,
	       اعشاری x0, اعشاری y0,
	       اعشاری x1, اعشاری y1,
	       اعشاری x2, اعشاری y2,
	       اعشاری x3, اعشاری y3)
{
  اگر (جھوٹ)
    flatten_cubic_fd (draw, x0, y0, x1, y1, x2, y2, x3, y3);
  ورنہ
    flatten_cubic_recursive (draw, x0, y0, x1, y1, x2, y2, x3, y3);
}


/* Draw callback implementations */

ساکن خالی
hb_raster_move_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		   خالی *draw_ڈیٹا,
		   hb_draw_state_t *st HB_غیر_مستعمل,
		   اعشاری مقصد_x HB_غیر_مستعمل, اعشاری مقصد_y HB_غیر_مستعمل,
		   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  /* no-op: state tracked by hb_draw_state_t */
}

ساکن خالی
hb_raster_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		   خالی *draw_ڈیٹا,
		   hb_draw_state_t *st,
		   اعشاری مقصد_x, اعشاری مقصد_y,
		   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_ڈیٹا;
  اگر (unlikely (draw->کنارے_بایاں <= 0)) واپس;

  اعشاری tx0, ty0, tx1, ty1;
  transform_نقطہ (draw, st->current_x, st->current_y, tx0, ty0);
  transform_نقطہ (draw, مقصد_x,          مقصد_y,           tx1, ty1);
  emit_segment (draw, tx0, ty0, tx1, ty1);
}

ساکن خالی
hb_raster_quadratic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			خالی *draw_ڈیٹا,
			hb_draw_state_t *st,
			اعشاری control_x, اعشاری control_y,
			اعشاری مقصد_x, اعشاری مقصد_y,
			خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_ڈیٹا;
  اگر (unlikely (draw->کنارے_بایاں <= 0)) واپس;

  اعشاری tx0, ty0, tx1, ty1, tx2, ty2;
  transform_نقطہ (draw, st->current_x, st->current_y, tx0, ty0);
  transform_نقطہ (draw, control_x,     control_y,      tx1, ty1);
  transform_نقطہ (draw, مقصد_x,          مقصد_y,           tx2, ty2);
  flatten_quadratic (draw, tx0, ty0, tx1, ty1, tx2, ty2);
}

ساکن خالی
hb_raster_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		    خالی *draw_ڈیٹا,
		    hb_draw_state_t *st,
		    اعشاری control1_x, اعشاری control1_y,
		    اعشاری control2_x, اعشاری control2_y,
		    اعشاری مقصد_x, اعشاری مقصد_y,
		    خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_raster_draw_t *draw = (hb_raster_draw_t *) draw_ڈیٹا;
  اگر (unlikely (draw->کنارے_بایاں <= 0)) واپس;

  اعشاری tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3;
  transform_نقطہ (draw, st->current_x, st->current_y, tx0, ty0);
  transform_نقطہ (draw, control1_x,    control1_y,     tx1, ty1);
  transform_نقطہ (draw, control2_x,    control2_y,     tx2, ty2);
  transform_نقطہ (draw, مقصد_x,          مقصد_y,           tx3, ty3);
  flatten_cubic (draw, tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3);
}

ساکن خالی
hb_raster_close_راستہ (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		      خالی *draw_ڈیٹا HB_غیر_مستعمل,
		      hb_draw_state_t *st HB_غیر_مستعمل,
		      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  /* no-op: hb_draw_funcs_t already emits closing line_to before us */
}

ساکن hb_bool_t
hb_raster_draw_سیٹ_budget (hb_draw_funcs_t *, خالی *draw_ڈیٹا,
			   int64_t budget, خالی *)
{
  خودکار *draw = (hb_raster_draw_t *) draw_ڈیٹا;
  draw->budget = budget;
  draw->recharge_budget ();
  واپس سچ;
}

ساکن int64_t
hb_raster_draw_get_budget (hb_draw_funcs_t *, خالی *draw_ڈیٹا, خالی *)
{
  واپس ((hb_raster_draw_t *) draw_ڈیٹا)->budget;
}

ساکن int64_t *
hb_raster_draw_get_budget_بقایا (hb_draw_funcs_t *, خالی *draw_ڈیٹا, خالی *)
{
  واپس ((hb_raster_draw_t *) draw_ڈیٹا)->get_budget_بقایا ();
}


/* Lazy-loader singleton for draw funcs */

ساکن اندرونی خالی free_static_raster_draw_funcs ();

ساکن ساخت hb_raster_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_raster_draw_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_مقصد_func      (funcs, hb_raster_move_مقصد,      خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func      (funcs, hb_raster_سطر_مقصد,      خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_quadratic_مقصد_func (funcs, hb_raster_quadratic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_cubic_مقصد_func     (funcs, hb_raster_cubic_مقصد,     خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_close_راستہ_func   (funcs, hb_raster_close_راستہ,   خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سیٹ_budget_func (funcs, hb_raster_draw_سیٹ_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_func (funcs, hb_raster_draw_get_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_raster_draw_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_raster_draw_funcs);

    واپس funcs;
  }
} static_raster_draw_funcs;

ساکن اندرونی خالی
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
hb_raster_draw_get_funcs (ثابت hb_raster_draw_t *draw HB_غیر_مستعمل)
{
  واپس static_raster_draw_funcs.get_unconst ();
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
  واپس hb_font_draw_glyph_or_fail (font, glyph,
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
خالی
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
ساکن constexpr غیرعلامتی HB_RASTER_NARROW_MAX_فعال_کنارے =
  INT16_MAX / HB_RASTER_ONE_PIXEL;

/* Add one edge piece's area/cover into a single cell. */
سانچہ <قسم_نام Area, قسم_نام Cover>
ساکن HB_ALWAYS_INLINE خالی
cell_جوڑو (Area *area, Cover *cover, غیرعلامتی چوڑائی, صحیح ستون,
	  int32_t fx0, int32_t fy0, int32_t fx1, int32_t fy1, int32_t wind,
	  غیرعلامتی &x_min, غیرعلامتی &x_max)
{
  اگر (unlikely ((غیرعلامتی) ستون >= چوڑائی))
  {
    اگر (unlikely (ستون < 0))
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
    واپس;
  }
  int32_t dy = fy1 - fy0;
  area[ستون]  += (Area) (fx0 + fx1) * dy * wind;
  cover[ستون] += (Cover) (dy * wind);
  x_min = hb_min (x_min, (غیرعلامتی) ستون);
  x_max = hb_max (x_max, (غیرعلامتی) ستون);
}

/* Walk one edge through the pixel cells of a single pixel row,
   accumulating area/cover.  py is the integer pixel-row index. */
سانچہ <قسم_نام Area, قسم_نام Cover>
ساکن HB_ALWAYS_INLINE خالی
کنارہ_sweep_صف (Area                   *area,
		Cover                  *cover,
		غیرعلامتی                چوڑائی,
		صحیح                     x_org,
		int32_t                 y_چوٹی,
		ثابت hb_raster_کنارہ_t &کنارہ,
		غیرعلامتی               &x_min,
		غیرعلامتی               &x_max)
{
  /* Saturate: y_top can be within ONE_PIXEL of INT32_MAX when the surface
   * sits at the extreme of the fixed-point coordinate space.  Edges are
   * int32-clamped too, so no coverage exists beyond INT32_MAX anyway. */
  int32_t y_bot = (int32_t) hb_min ((int64_t) y_چوٹی + HB_RASTER_ONE_PIXEL, (int64_t) INT32_MAX);

  int32_t ey0 = hb_max (کنارہ.yL, y_چوٹی);
  int32_t ey1 = hb_min (کنارہ.yH, y_bot);
  اگر (ey0 >= ey1) واپس;

  /* X at clipped endpoints (fixed-point). Keep the interpolation in 64-bit
   * so extreme y values do not overflow before the slope multiply. */
  int64_t x0_64 = (int64_t) کنارہ.xL +
		  ((((int64_t) ey0 - (int64_t) کنارہ.yL) * کنارہ.ڈھلان) >> 16);
  int64_t x1_64 = (int64_t) کنارہ.xL +
		  ((((int64_t) ey1 - (int64_t) کنارہ.yL) * کنارہ.ڈھلان) >> 16);
  int32_t x0 = (int32_t) hb_clamp (x0_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);
  int32_t x1 = (int32_t) hb_clamp (x1_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);

  /* Fractional y within this pixel row [0, ONE_PIXEL] */
  int32_t fy0 = ey0 - y_چوٹی;
  int32_t fy1 = ey1 - y_چوٹی;

  int32_t cx0 = x0 >> HB_RASTER_PIXEL_بٹ;
  int32_t fx0 = x0 & HB_RASTER_PIXEL_نقاب;
  int32_t cx1 = x1 >> HB_RASTER_PIXEL_بٹ;
  int32_t fx1 = x1 & HB_RASTER_PIXEL_نقاب;
  int32_t wind = کنارہ.wind;

  /* Fast path: both endpoints in the same pixel column. */
  اگر (cx0 == cx1)
  {
    cell_جوڑو (area, cover, چوڑائی, cx0 - x_org, fx0, fy0, fx1, fy1, wind, x_min, x_max);
    واپس;
  }

  int64_t کل_dx = (int64_t) x1 - (int64_t) x0;
  int64_t کل_dy = (int64_t) fy1 - (int64_t) fy0;

  /* fy increment per pixel column (constant since x_b advances by ONE_PIXEL). */
  int32_t delta_fy = (int32_t) ((int64_t) HB_RASTER_ONE_PIXEL * کل_dy / کل_dx);

  اگر (کل_dx > 0)
  {
    /* Left-to-right edge. */
    اگر (likely ((غیرعلامتی) (cx0 - x_org) < چوڑائی && (غیرعلامتی) (cx1 - x_org) < چوڑائی))
    {
      /* Entirely inside the surface: unclipped walk. */
      int32_t x_b = (int32_t) hb_clamp (((int64_t) cx0 + 1) * HB_RASTER_ONE_PIXEL,
					(int64_t) INT32_MIN, (int64_t) INT32_MAX);
      int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * کل_dy) / کل_dx);
      cell_جوڑو (area, cover, چوڑائی, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);

      int32_t fy_پچھلا = fy_b;
      برائے (int32_t cx = cx0 + 1; cx < cx1; cx++)
      {
	fy_b = fy_پچھلا + delta_fy;
	cell_جوڑو (area, cover, چوڑائی, cx - x_org, 0, fy_پچھلا, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
	fy_پچھلا = fy_b;
      }

      cell_جوڑو (area, cover, چوڑائی, cx1 - x_org, 0, fy_پچھلا, fx1, fy1, wind, x_min, x_max);
      واپس;
    }

    /* Visible column window.  Columns left of it only contribute their
     * total cover to column 0 (see cell_add) and that total telescopes,
     * while columns right of it contribute nothing; so neither side is
     * walked cell by cell.  Skipping ahead in the fy accumulation is
     * exact: the increment is constant and the running sums are bounded,
     * so k steps equal one k·delta_fy jump. */
    int64_t ستون_min = (int64_t) x_org;
    int64_t ستون_max = (int64_t) x_org + (int64_t) چوڑائی - 1;

    اگر (unlikely (cx1 < ستون_min))
    {
      /* Entirely left of the surface. */
      cell_جوڑو (area, cover, چوڑائی, -1, 0, fy0, 0, fy1, wind, x_min, x_max);
      واپس;
    }
    اگر (unlikely (cx0 > ستون_max))
      واپس; /* Entirely right of the surface. */

    int32_t x_b = (int32_t) hb_clamp (((int64_t) cx0 + 1) * HB_RASTER_ONE_PIXEL,
				      (int64_t) INT32_MIN, (int64_t) INT32_MAX);
    int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * کل_dy) / کل_dx);

    int32_t cx = cx0 + 1;
    int32_t fy_پچھلا = fy_b;
    اگر (likely (cx0 >= ستون_min))
      cell_جوڑو (area, cover, چوڑائی, cx0 - x_org, fx0, fy0, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
    ورنہ
    {
      /* Fold the first cell and the mid columns left of the surface
       * into one column-0 cover update. */
      int32_t cx_skip = (int32_t) ستون_min; /* ≤ cx1, so it fits */
      fy_پچھلا = fy_b + (int32_t) ((int64_t) (cx_skip - cx) * delta_fy);
      cell_جوڑو (area, cover, چوڑائی, -1, 0, fy0, 0, fy_پچھلا, wind, x_min, x_max);
      cx = cx_skip;
    }

    int32_t cx_انتہا = (int32_t) hb_min ((int64_t) cx1, ستون_max + 1);
    برائے (; cx < cx_انتہا; cx++)
    {
      fy_b = fy_پچھلا + delta_fy;
      cell_جوڑو (area, cover, چوڑائی, cx - x_org, 0, fy_پچھلا, HB_RASTER_ONE_PIXEL, fy_b, wind, x_min, x_max);
      fy_پچھلا = fy_b;
    }

    اگر (likely (cx1 <= ستون_max))
      cell_جوڑو (area, cover, چوڑائی, cx1 - x_org, 0, fy_پچھلا, fx1, fy1, wind, x_min, x_max);
  }
  ورنہ
  {
    /* Right-to-left edge. */
    اگر (likely ((غیرعلامتی) (cx0 - x_org) < چوڑائی && (غیرعلامتی) (cx1 - x_org) < چوڑائی))
    {
      /* Entirely inside the surface: unclipped walk. */
      int32_t x_b = (int32_t) hb_clamp ((int64_t) cx0 * HB_RASTER_ONE_PIXEL,
					(int64_t) INT32_MIN, (int64_t) INT32_MAX);
      int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * کل_dy) / کل_dx);
      cell_جوڑو (area, cover, چوڑائی, cx0 - x_org, fx0, fy0, 0, fy_b, wind, x_min, x_max);

      int32_t fy_پچھلا = fy_b;
      برائے (int32_t cx = cx0 - 1; cx > cx1; cx--)
      {
	fy_b = fy_پچھلا - delta_fy;
	cell_جوڑو (area, cover, چوڑائی, cx - x_org, HB_RASTER_ONE_PIXEL, fy_پچھلا, 0, fy_b, wind, x_min, x_max);
	fy_پچھلا = fy_b;
      }

      cell_جوڑو (area, cover, چوڑائی, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_پچھلا, fx1, fy1, wind, x_min, x_max);
      واپس;
    }

    int64_t ستون_min = (int64_t) x_org;
    int64_t ستون_max = (int64_t) x_org + (int64_t) چوڑائی - 1;

    اگر (unlikely (cx0 < ستون_min))
    {
      /* Entirely left of the surface. */
      cell_جوڑو (area, cover, چوڑائی, -1, 0, fy0, 0, fy1, wind, x_min, x_max);
      واپس;
    }
    اگر (unlikely (cx1 > ستون_max))
      واپس; /* Entirely right of the surface. */

    int32_t x_b = (int32_t) hb_clamp ((int64_t) cx0 * HB_RASTER_ONE_PIXEL,
				      (int64_t) INT32_MIN, (int64_t) INT32_MAX);
    int32_t fy_b = fy0 + (int32_t) ((((int64_t) x_b - (int64_t) x0) * کل_dy) / کل_dx);
    cell_جوڑو (area, cover, چوڑائی, cx0 - x_org, fx0, fy0, 0, fy_b, wind, x_min, x_max);

    int32_t cx = cx0 - 1;
    int32_t fy_پچھلا = fy_b;
    اگر (unlikely (cx > ستون_max))
    {
      /* Mid columns right of the surface contribute nothing. */
      int32_t cx_skip = (int32_t) ستون_max; /* ≥ cx1, so it fits */
      fy_پچھلا = fy_b - (int32_t) ((int64_t) (cx - cx_skip) * delta_fy);
      cx = cx_skip;
    }

    int32_t cx_stop = (int32_t) hb_max ((int64_t) cx1, ستون_min - 1);
    برائے (; cx > cx_stop; cx--)
    {
      fy_b = fy_پچھلا - delta_fy;
      cell_جوڑو (area, cover, چوڑائی, cx - x_org, HB_RASTER_ONE_PIXEL, fy_پچھلا, 0, fy_b, wind, x_min, x_max);
      fy_پچھلا = fy_b;
    }

    اگر (likely (cx1 >= ستون_min))
      cell_جوڑو (area, cover, چوڑائی, cx1 - x_org, HB_RASTER_ONE_PIXEL, fy_پچھلا, fx1, fy1, wind, x_min, x_max);
    ورنہ
      /* Fold the mid columns left of the surface and the last cell
       * into one column-0 cover update. */
      cell_جوڑو (area, cover, چوڑائی, -1, 0, fy_پچھلا, 0, fy1, wind, x_min, x_max);
  }
}

/* Convert cover-delta + area to alpha bytes, then clear.
   Returns final cover accumulator over [x_min, x_max]. */
ساکن int32_t
sweep_صف_مقصد_alpha (uint8_t *__restrict صف_buf,
		    int32_t *__restrict area,
		    int16_t *__restrict cover,
		    غیرعلامتی x_min,
		    غیرعلامتی x_max)
{
  ثابت int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
  int32_t cover_accum = 0;
  غیرعلامتی x = x_min;

#اگر_تعریف_شدہ HB_RASTER_NEON
  int32x4_t clamp_v = vdupq_n_s32 (HB_RASTER_بھرا_COVERAGE);
  int32x4_t bias_v  = vdupq_n_s32 (HB_RASTER_بھرا_COVERAGE / 2);
  int32x4_t zero32  = vdupq_n_s32 (0);
  int16x8_t zero16  = vdupq_n_s16 (0);
  برائے (; x + 7 <= x_max; x += 8)
  {
    int32_t ctmp[8];
    برائے (غیرعلامتی i = 0; i < 8; i++)
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
#وگرنہ_اگر defined(HB_RASTER_SSE2)
  __m128i clamp_v = _mm_set1_epi32 (HB_RASTER_بھرا_COVERAGE);
  __m128i bias_v  = _mm_set1_epi32 (HB_RASTER_بھرا_COVERAGE / 2);
  __m128i zero_v  = _mm_setzero_si128 ();
  برائے (; x + 7 <= x_max; x += 8)
  {
    int32_t ctmp[8];
    برائے (غیرعلامتی i = 0; i < 8; i++)
    {
      cover_accum += cover[x + i];
      ctmp[i] = cover_accum * cover_scale;
    }

    __m128i c0  = _mm_loadu_si128 ((__m128i *) (خالی *) (ctmp + 0));
    __m128i c1  = _mm_loadu_si128 ((__m128i *) (خالی *) (ctmp + 4));
    __m128i a0  = _mm_loadu_si128 ((__m128i *) (خالی *) (area + x));
    __m128i a1  = _mm_loadu_si128 ((__m128i *) (خالی *) (area + x + 4));

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
    _mm_storel_epi64 ((__m128i *) (خالی *) (صف_buf + x), b);

    _mm_storeu_si128 ((__m128i *) (خالی *) (area + x),     zero_v);
    _mm_storeu_si128 ((__m128i *) (خالی *) (area + x + 4), zero_v);
    _mm_storeu_si128 ((__m128i *) (خالی *) (cover + x),    zero_v);
  }
#اگر_ختم

  برائے (; x <= x_max; x++)
  {
    cover_accum += cover[x];
    int32_t val   = cover_accum * cover_scale - area[x];
    int32_t alpha = val < 0 ? -val : val;
    اگر (alpha > HB_RASTER_بھرا_COVERAGE) alpha = HB_RASTER_بھرا_COVERAGE;
    صف_buf[x] = (uint8_t) (((غیرعلامتی) alpha * 255 + HB_RASTER_بھرا_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_بٹ + 1));
    area[x]  = 0;
    cover[x] = 0;
  }

  واپس cover_accum;
}

ساکن int64_t
sweep_صف_مقصد_alpha_wide (uint8_t *__restrict صف_buf,
			 int64_t *__restrict area,
			 int64_t *__restrict cover,
			 غیرعلامتی x_min,
			 غیرعلامتی x_max)
{
  ثابت int32_t cover_scale = 2 * HB_RASTER_ONE_PIXEL;
  int64_t cover_accum = 0;

  برائے (غیرعلامتی x = x_min; x <= x_max; x++)
  {
    cover_accum += cover[x];
    int64_t val = cover_accum * cover_scale - area[x];
    int64_t alpha = val < 0 ? -val : val;
    اگر (alpha > HB_RASTER_بھرا_COVERAGE) alpha = HB_RASTER_بھرا_COVERAGE;
    صف_buf[x] = (uint8_t) (((غیرعلامتی) alpha * 255 + HB_RASTER_بھرا_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_بٹ + 1));
    area[x] = 0;
    cover[x] = 0;
  }

  واپس cover_accum;
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

  اگر (draw->has_extents)
  {
    ext = draw->fixed_extents;
  }
  ورنہ
  {
    /* Auto-size from edge bounding box */
    اگر (draw->کنارے.لمبائی == 0)
    {
      /* No edges: produce 0×0 image */
      ext = { 0, 0, 0, 0, 0 };
    }
    ورنہ
    {
      int32_t xmin = draw->کنارے.arrayZ[0].xL, xmax = draw->کنارے.arrayZ[0].xL;
      int32_t ymin = draw->کنارے.arrayZ[0].yL, ymax = draw->کنارے.arrayZ[0].yH;

      برائے (ثابت خودکار &e : draw->کنارے)
      {
	xmin = hb_min (xmin, hb_min (e.xL, e.xH));
	xmax = hb_max (xmax, hb_max (e.xL, e.xH));
	ymin = hb_min (ymin, e.yL);
	ymax = hb_max (ymax, e.yH);
      }

      /* Convert fixed-point → pixels (floor for min, ceil for max).  Edge
	 coordinates are saturated to int32 range in emit_segment, so the
	 +MASK ceil step must be widened to avoid signed overflow. */
      صحیح x0 = xmin >> HB_RASTER_PIXEL_بٹ;
      صحیح y0 = ymin >> HB_RASTER_PIXEL_بٹ;
      صحیح x1 = (صحیح) (((int64_t) xmax + HB_RASTER_PIXEL_نقاب) >> HB_RASTER_PIXEL_بٹ);
      صحیح y1 = (صحیح) (((int64_t) ymax + HB_RASTER_PIXEL_نقاب) >> HB_RASTER_PIXEL_بٹ);

      ext.x_origin = x0;
      ext.y_origin = y0;
      ext.چوڑائی    = (غیرعلامتی) hb_min (hb_max (0, x1 - x0), HB_RASTER_MAX_AUTO_DIMENSION);
      ext.اونچائی   = (غیرعلامتی) hb_min (hb_max (0, y1 - y0), HB_RASTER_MAX_AUTO_DIMENSION);
      ext.stride   = 0; /* filled below */
    }
  }

  /* ── 2. Compute stride ─────────────────────────────────────────── */
  اگر (ext.stride == 0)
    ext.stride = (ext.چوڑائی + 3u) & ~3u;

  /* ── 3. Allocate or reuse image ─────────────────────────────────── */
  /* Reset one-shot state on every exit path. */
  HB_SCOPE_GUARD (hb_raster_draw_clear (draw));

  hb_unique_ptr_t<hb_raster_image_t> image;
  اگر (draw->recycled_image)
  {
    image = hb_unique_ptr_t<hb_raster_image_t> (draw->recycled_image);
    draw->recycled_image = خالی_اشاریہ;
  }
  ورنہ
  {
    image = hb_unique_ptr_t<hb_raster_image_t> (hb_raster_image_create_or_fail ());
    اگر (unlikely (!image)) واپس خالی_اشاریہ;
  }

  اگر (unlikely (!image->configure (HB_RASTER_FORMAT_A8, ext)))
    واپس خالی_اشاریہ;
  image->clear ();

  /* ── 4. Bucket edges by starting row and rasterize scanlines ──── */
  اگر (draw->کنارے.لمبائی && ext.چوڑائی && ext.اونچائی)
  {
    اگر (unlikely (!draw->صف_area.resize_dirty (ext.چوڑائی) ||
		  !draw->صف_cover.resize_dirty (ext.چوڑائی)))
      واپس خالی_اشاریہ;
    hb_memset (draw->صف_area.arrayZ,  0, ext.چوڑائی * حجم (int32_t));
    hb_memset (draw->صف_cover.arrayZ, 0, ext.چوڑائی * حجم (int16_t));

    /* Bucket edges by their starting pixel row.
       Only grow the outer vector; clear inner vectors without freeing. */
    غیرعلامتی old_buckets = draw->کنارہ_buckets.لمبائی;
    اگر (ext.اونچائی > old_buckets)
    {
      اگر (unlikely (!draw->کنارہ_buckets.resize (ext.اونچائی)))
	واپس خالی_اشاریہ;
    }
    برائے (غیرعلامتی i = 0; i < hb_min (ext.اونچائی, old_buckets); i++)
      draw->کنارہ_buckets.arrayZ[i].clear ();
    /* New buckets (if any) are already empty from resize's zero-init. */

    برائے (غیرعلامتی i = 0; i < draw->کنارے.لمبائی; i++)
    {
      صحیح صف = (draw->کنارے.arrayZ[i].yL >> HB_RASTER_PIXEL_بٹ) - ext.y_origin;
      اگر (صف < 0) صف = 0;
      اگر ((غیرعلامتی) صف >= ext.اونچائی) جاری;
      draw->کنارہ_buckets.arrayZ[صف].push (i);
    }

    /* Scanline loop with active edge list. */
    draw->فعال_کنارے.clear ();
    منطقی wide_initialized = جھوٹ;

    برائے (غیرعلامتی صف = 0; صف < ext.اونچائی; صف++)
    {
      int64_t y_چوٹی_64 = ((int64_t) ext.y_origin + (int64_t) صف) * HB_RASTER_ONE_PIXEL;
      int32_t y_چوٹی = (int32_t) hb_clamp (y_چوٹی_64, (int64_t) INT32_MIN, (int64_t) INT32_MAX);

      /* Add new edges from this row's bucket. */
      draw->فعال_کنارے.extend (draw->کنارہ_buckets.arrayZ[صف]);

      /* Process active edges and compact live ones in one linear pass. */
      غیرعلامتی x_min = ext.چوڑائی, x_max = 0;
      غیرعلامتی اكتب = 0;
      غیرعلامتی فعال_len = draw->فعال_کنارے.لمبائی;
      منطقی use_wide = فعال_len > HB_RASTER_NARROW_MAX_فعال_کنارے;
      اگر (unlikely (use_wide && !wide_initialized))
      {
	اگر (unlikely (!draw->صف_area_wide.resize_dirty (ext.چوڑائی) ||
		      !draw->صف_cover_wide.resize_dirty (ext.چوڑائی)))
	  واپس خالی_اشاریہ;
	hb_memset (draw->صف_area_wide.arrayZ,  0, ext.چوڑائی * حجم (int64_t));
	hb_memset (draw->صف_cover_wide.arrayZ, 0, ext.چوڑائی * حجم (int64_t));
	wide_initialized = سچ;
      }
      برائے (غیرعلامتی j = 0; j < فعال_len; j++)
      {
	غیرعلامتی کنارہ_idx = draw->فعال_کنارے.arrayZ[j];
	ثابت خودکار &e = draw->کنارے.arrayZ[کنارہ_idx];
	اگر (e.yH <= y_چوٹی)
	  جاری;

	اگر (unlikely (use_wide))
	  کنارہ_sweep_صف (draw->صف_area_wide.arrayZ, draw->صف_cover_wide.arrayZ,
			  ext.چوڑائی, ext.x_origin, y_چوٹی, e, x_min, x_max);
	ورنہ
	  کنارہ_sweep_صف (draw->صف_area.arrayZ, draw->صف_cover.arrayZ,
			  ext.چوڑائی, ext.x_origin, y_چوٹی, e, x_min, x_max);
	draw->فعال_کنارے.arrayZ[اكتب++] = کنارہ_idx;
      }
      draw->فعال_کنارے.resize (اكتب);

      اگر (x_min <= x_max)
      {
	int64_t cover_accum = use_wide ?
	  sweep_صف_مقصد_alpha_wide (image->بفر.arrayZ + صف * ext.stride,
				   draw->صف_area_wide.arrayZ, draw->صف_cover_wide.arrayZ,
				   x_min, x_max) :
	  sweep_صف_مقصد_alpha (image->بفر.arrayZ + صف * ext.stride,
			      draw->صف_area.arrayZ, draw->صف_cover.arrayZ,
			      x_min, x_max);

	/* If cover doesn't cancel, memset the constant-alpha tail. */
	اگر (cover_accum != 0)
	{
	  int64_t alpha = cover_accum * (2 * HB_RASTER_ONE_PIXEL);
	  alpha = alpha < 0 ? -alpha : alpha;
	  اگر (alpha > HB_RASTER_بھرا_COVERAGE) alpha = HB_RASTER_بھرا_COVERAGE;
	  uint8_t byte = (uint8_t) (((غیرعلامتی) alpha * 255 + HB_RASTER_بھرا_COVERAGE / 2) >> (2 * HB_RASTER_PIXEL_بٹ + 1));

	  uint8_t *صف_buf = image->بفر.arrayZ + صف * ext.stride;
	  hb_memset (صف_buf + x_max + 1, byte, ext.چوڑائی - 1 - x_max);
	}
      }
    }
  }

  واپس image.release ();
}
