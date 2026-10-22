/*
 * Copyright © 2022 Behdad Esfahbod
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
 */
#ifndef HB_GEOMETRY_HH
#define HB_GEOMETRY_HH

#ضمن "hb.hh"

#ضمن "hb-algs.hh"

#ضمن <cmath>


template <typename Float = عائم>
بنية hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (ثابت hb_glyph_extents_t &extents)
  {
    مزدوج x0 = (مزدوج) extents.x_bearing;
    مزدوج y0 = (مزدوج) extents.y_bearing;
    مزدوج x1 = x0 + (مزدوج) extents.width;
    مزدوج y1 = y0 + (مزدوج) extents.height;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  منطقي is_empty () ثابت { החזר xmin >= xmax || ymin >= ymax; }
  منطقي is_void () ثابت { החזר xmin > xmax; }

  فراغ union_ (ثابت hb_extents_t &o)
  {
    אם (o.is_empty ()) החזר;
    אם (is_empty ())
    {
      *هذا = o;
      החזר;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  فراغ intersect (ثابت hb_extents_t &o)
  {
    אם (o.is_empty () || is_empty ())
    {
      *هذا = hb_extents_t {};
      החזר;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  فراغ
  add_point (Float x, Float y)
  {
    אם (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    אחרת
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t to_glyph_extents (منطقي xneg = שקר, منطقي yneg = שקר) ثابت
  {
    مزدوج x0 = round ((مزدوج) xmin);
    مزدوج y0 = round ((مزدوج) ymin);
    مزدوج x1 = round ((مزدوج) xmax);
    مزدوج y1 = round ((مزدوج) ymax);

    אם (unlikely (!std::isfinite (x0) ||
		  !std::isfinite (y0) ||
		  !std::isfinite (x1) ||
		  !std::isfinite (y1)))
      החזר hb_glyph_extents_t {0, 0, 0, 0};

    החזר hb_glyph_extents_t {
      clamp_to_hb_position (xneg ? x1 : x0),
      clamp_to_hb_position (yneg ? y0 : y1),
      clamp_to_hb_position (xneg ? x0 - x1 : x1 - x0),
      clamp_to_hb_position (yneg ? y1 - y0 : y0 - y1)
    };
  }

  Float xmin = 0;
  Float ymin = 0;
  Float xmax = -1;
  Float ymax = -1;

  ساكن hb_position_t
  clamp_to_hb_position (مزدوج v)
  {
    החזר (hb_position_t) hb_clamp (v,
				     (مزدوج) hb_int_min (hb_position_t),
				     (مزدوج) hb_int_max (hb_position_t));
  }
};

template <typename Float = عائم>
بنية hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  منطقي is_identity () ثابت
  {
    החזר xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  منطقي is_translation () ثابت
  {
    החזר xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  فراغ multiply (ثابت hb_transform_t &o, منطقي before=שקר)
  {
    // Copied from cairo-matrix.c
    ثابت hb_transform_t &a = before ? o : *هذا;
    ثابت hb_transform_t &b = before ? *هذا : o;
    *هذا = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  فراغ transform_distance (Float &dx, Float &dy) ثابت
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  فراغ transform_point (Float &x, Float &y) ثابت
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  فراغ transform_extents (hb_extents_t<Float> &extents) ثابت
  {
    Float quad_x[4], quad_y[4];

    quad_x[0] = extents.xmin;
    quad_y[0] = extents.ymin;
    quad_x[1] = extents.xmin;
    quad_y[1] = extents.ymax;
    quad_x[2] = extents.xmax;
    quad_y[2] = extents.ymin;
    quad_x[3] = extents.xmax;
    quad_y[3] = extents.ymax;

    extents = hb_extents_t<Float> {};
    עבור (unsigned i = 0; i < 4; i++)
    {
      transform_point (quad_x[i], quad_y[i]);
      extents.add_point (quad_x[i], quad_y[i]);
    }
  }

  فراغ transform (ثابت hb_transform_t &o, منطقي before=שקר) { multiply (o, before); }

  ساكن hb_transform_t translation (Float x, Float y)
  {
    החזר {1, 0, 0, 1, x, y};
  }
  فراغ translate (Float x, Float y, منطقي before=שקר)
  {
    אם (before)
    {
      x0 += x;
      y0 += y;
    }
    אחרת
    {
      אם (x == 0 && y == 0)
	החזר;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  ساكن hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    החזר {scaleX, 0, 0, scaleY, 0, 0};
  }
  فراغ scale (Float scaleX, Float scaleY)
  {
    אם (scaleX == 1 && scaleY == 1)
      החזר;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  ساكن hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    החזר {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  فراغ scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    אם (scaleX == 1 && scaleY == 1)
      החזר;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  ساكن hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    החזר {c, s, -s, c, 0, 0};
  }
  فراغ rotate (Float radians, منطقي before=שקר)
  {
    אם (radians == 0)
      החזר;

    transform (rotation (radians), before);
  }

  ساكن hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    החזר {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  فراغ rotate_around_center (Float radians, Float center_x, Float center_y, منطقي before=שקר)
  {
    אם (radians == 0)
      החזר;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  ساكن hb_transform_t skewing (Float skewX, Float skewY)
  {
    החזר {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  فراغ skew (Float skewX, Float skewY)
  {
    אם (skewX == 0 && skewY == 0)
      החזר;

    transform (skewing (skewX, skewY));
  }
  ساكن hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    החזר {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  فراغ skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    אם (skewX == 0 && skewY == 0)
	    החזר;

    transform (skewing_around_center (skewX, skewY, center_x, center_y));
  }

  Float xx = 1;
  Float yx = 0;
  Float xy = 0;
  Float yy = 1;
  Float x0 = 0;
  Float y0 = 0;
};

#define HB_TRANSFORM_IDENTITY {1, 0, 0, 1, 0, 0}

template <typename Float = عائم>
بنية hb_bounds_t
{
  تعداد status_t {
    UNBOUNDED,
    BOUNDED,
    EMPTY,
  };

  hb_bounds_t (status_t status = UNBOUNDED) : status (status) {}
  hb_bounds_t (ثابت hb_extents_t<Float> &extents) :
    status (extents.is_empty () ? EMPTY : BOUNDED), extents (extents) {}

  فراغ union_ (ثابت hb_bounds_t &o)
  {
    אם (o.status == UNBOUNDED)
      status = UNBOUNDED;
    אחרת אם (o.status == BOUNDED)
    {
      אם (status == EMPTY)
	*هذا = o;
      אחרת אם (status == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  فراغ intersect (ثابت hb_bounds_t &o)
  {
    אם (o.status == EMPTY)
      status = EMPTY;
    אחרת אם (o.status == BOUNDED)
    {
      אם (status == UNBOUNDED)
	*هذا = o;
      אחרת אם (status == BOUNDED)
      {
        extents.intersect (o.extents);
	אם (extents.is_empty ())
	  status = EMPTY;
      }
    }
  }

  status_t status;
  hb_extents_t<Float> extents;
};

template <typename Float = عائم>
بنية hb_transform_decomposed_t
{
  Float translateX = 0;
  Float translateY = 0;
  Float rotation = 0;  // in radians, counter-clockwise
  Float scaleX = 1;
  Float scaleY = 1;
  Float skewX = 0;  // in radians, counter-clockwise
  Float skewY = 0;  // in radians, counter-clockwise
  Float tCenterX = 0;
  Float tCenterY = 0;

  operator منطقي () ثابت
  {
    החזר translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> to_transform () ثابت
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    החזר t;
  }
};


#endif /* HB_GEOMETRY_HH */
