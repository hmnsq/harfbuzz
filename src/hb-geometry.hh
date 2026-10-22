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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-algs.hh"

#அந்தர்பாவ <cmath>


template <typename Float = தசம>
கட்டமைப்பு hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (மாறிலி hb_glyph_extents_t &extents)
  {
    இரட்டை x0 = (இரட்டை) extents.x_bearing;
    இரட்டை y0 = (இரட்டை) extents.y_bearing;
    இரட்டை x1 = x0 + (இரட்டை) extents.width;
    இரட்டை y1 = y0 + (இரட்டை) extents.height;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  தருக்க is_empty () மாறிலி { திரும்பு xmin >= xmax || ymin >= ymax; }
  தருக்க is_void () மாறிலி { திரும்பு xmin > xmax; }

  வெறுமை union_ (மாறிலி hb_extents_t &o)
  {
    எனில் (o.is_empty ()) திரும்பு;
    எனில் (is_empty ())
    {
      *இது = o;
      திரும்பு;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  வெறுமை intersect (மாறிலி hb_extents_t &o)
  {
    எனில் (o.is_empty () || is_empty ())
    {
      *இது = hb_extents_t {};
      திரும்பு;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  வெறுமை
  add_point (Float x, Float y)
  {
    எனில் (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    இல்லையெனில்
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t to_glyph_extents (தருக்க xneg = பொய், தருக்க yneg = பொய்) மாறிலி
  {
    இரட்டை x0 = round ((இரட்டை) xmin);
    இரட்டை y0 = round ((இரட்டை) ymin);
    இரட்டை x1 = round ((இரட்டை) xmax);
    இரட்டை y1 = round ((இரட்டை) ymax);

    எனில் (unlikely (!std::isfinite (x0) ||
		  !std::isfinite (y0) ||
		  !std::isfinite (x1) ||
		  !std::isfinite (y1)))
      திரும்பு hb_glyph_extents_t {0, 0, 0, 0};

    திரும்பு hb_glyph_extents_t {
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

  நிலையான hb_position_t
  clamp_to_hb_position (இரட்டை v)
  {
    திரும்பு (hb_position_t) hb_clamp (v,
				     (இரட்டை) hb_int_min (hb_position_t),
				     (இரட்டை) hb_int_max (hb_position_t));
  }
};

template <typename Float = தசம>
கட்டமைப்பு hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  தருக்க is_identity () மாறிலி
  {
    திரும்பு xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  தருக்க is_translation () மாறிலி
  {
    திரும்பு xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  வெறுமை multiply (மாறிலி hb_transform_t &o, தருக்க before=பொய்)
  {
    // Copied from cairo-matrix.c
    மாறிலி hb_transform_t &a = before ? o : *இது;
    மாறிலி hb_transform_t &b = before ? *இது : o;
    *இது = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  வெறுமை transform_distance (Float &dx, Float &dy) மாறிலி
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  வெறுமை transform_point (Float &x, Float &y) மாறிலி
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  வெறுமை transform_extents (hb_extents_t<Float> &extents) மாறிலி
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
    க்கு (unsigned i = 0; i < 4; i++)
    {
      transform_point (quad_x[i], quad_y[i]);
      extents.add_point (quad_x[i], quad_y[i]);
    }
  }

  வெறுமை transform (மாறிலி hb_transform_t &o, தருக்க before=பொய்) { multiply (o, before); }

  நிலையான hb_transform_t translation (Float x, Float y)
  {
    திரும்பு {1, 0, 0, 1, x, y};
  }
  வெறுமை translate (Float x, Float y, தருக்க before=பொய்)
  {
    எனில் (before)
    {
      x0 += x;
      y0 += y;
    }
    இல்லையெனில்
    {
      எனில் (x == 0 && y == 0)
	திரும்பு;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  நிலையான hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    திரும்பு {scaleX, 0, 0, scaleY, 0, 0};
  }
  வெறுமை scale (Float scaleX, Float scaleY)
  {
    எனில் (scaleX == 1 && scaleY == 1)
      திரும்பு;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  நிலையான hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    திரும்பு {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  வெறுமை scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    எனில் (scaleX == 1 && scaleY == 1)
      திரும்பு;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  நிலையான hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    திரும்பு {c, s, -s, c, 0, 0};
  }
  வெறுமை rotate (Float radians, தருக்க before=பொய்)
  {
    எனில் (radians == 0)
      திரும்பு;

    transform (rotation (radians), before);
  }

  நிலையான hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    திரும்பு {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  வெறுமை rotate_around_center (Float radians, Float center_x, Float center_y, தருக்க before=பொய்)
  {
    எனில் (radians == 0)
      திரும்பு;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  நிலையான hb_transform_t skewing (Float skewX, Float skewY)
  {
    திரும்பு {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  வெறுமை skew (Float skewX, Float skewY)
  {
    எனில் (skewX == 0 && skewY == 0)
      திரும்பு;

    transform (skewing (skewX, skewY));
  }
  நிலையான hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    திரும்பு {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  வெறுமை skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    எனில் (skewX == 0 && skewY == 0)
	    திரும்பு;

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

template <typename Float = தசம>
கட்டமைப்பு hb_bounds_t
{
  எண்ணிக்கை status_t {
    UNBOUNDED,
    BOUNDED,
    EMPTY,
  };

  hb_bounds_t (status_t status = UNBOUNDED) : status (status) {}
  hb_bounds_t (மாறிலி hb_extents_t<Float> &extents) :
    status (extents.is_empty () ? EMPTY : BOUNDED), extents (extents) {}

  வெறுமை union_ (மாறிலி hb_bounds_t &o)
  {
    எனில் (o.status == UNBOUNDED)
      status = UNBOUNDED;
    இல்லையெனில் எனில் (o.status == BOUNDED)
    {
      எனில் (status == EMPTY)
	*இது = o;
      இல்லையெனில் எனில் (status == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  வெறுமை intersect (மாறிலி hb_bounds_t &o)
  {
    எனில் (o.status == EMPTY)
      status = EMPTY;
    இல்லையெனில் எனில் (o.status == BOUNDED)
    {
      எனில் (status == UNBOUNDED)
	*இது = o;
      இல்லையெனில் எனில் (status == BOUNDED)
      {
        extents.intersect (o.extents);
	எனில் (extents.is_empty ())
	  status = EMPTY;
      }
    }
  }

  status_t status;
  hb_extents_t<Float> extents;
};

template <typename Float = தசம>
கட்டமைப்பு hb_transform_decomposed_t
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

  operator தருக்க () மாறிலி
  {
    திரும்பு translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> to_transform () மாறிலி
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    திரும்பு t;
  }
};


#endif /* HB_GEOMETRY_HH */
