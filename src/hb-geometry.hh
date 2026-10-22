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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-algs.hh"

#অন্তর্ভাব <cmath>


template <typename Float = দশমিক>
কাঠামো hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (ধ্রুবক hb_glyph_extents_t &extents)
  {
    দ্বিগুণ x0 = (দ্বিগুণ) extents.x_bearing;
    দ্বিগুণ y0 = (দ্বিগুণ) extents.y_bearing;
    দ্বিগুণ x1 = x0 + (দ্বিগুণ) extents.width;
    দ্বিগুণ y1 = y0 + (দ্বিগুণ) extents.height;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  যৌক্তিক is_empty () ধ্রুবক { ফেরত xmin >= xmax || ymin >= ymax; }
  যৌক্তিক is_void () ধ্রুবক { ফেরত xmin > xmax; }

  শূন্য union_ (ধ্রুবক hb_extents_t &o)
  {
    যদি (o.is_empty ()) ফেরত;
    যদি (is_empty ())
    {
      *এই = o;
      ফেরত;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  শূন্য intersect (ধ্রুবক hb_extents_t &o)
  {
    যদি (o.is_empty () || is_empty ())
    {
      *এই = hb_extents_t {};
      ফেরত;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  শূন্য
  add_point (Float x, Float y)
  {
    যদি (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    অন্যথা
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t to_glyph_extents (যৌক্তিক xneg = মিথ্যা, যৌক্তিক yneg = মিথ্যা) ধ্রুবক
  {
    দ্বিগুণ x0 = round ((দ্বিগুণ) xmin);
    দ্বিগুণ y0 = round ((দ্বিগুণ) ymin);
    দ্বিগুণ x1 = round ((দ্বিগুণ) xmax);
    দ্বিগুণ y1 = round ((দ্বিগুণ) ymax);

    যদি (unlikely (!std::isfinite (x0) ||
		  !std::isfinite (y0) ||
		  !std::isfinite (x1) ||
		  !std::isfinite (y1)))
      ফেরত hb_glyph_extents_t {0, 0, 0, 0};

    ফেরত hb_glyph_extents_t {
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

  স্থির hb_position_t
  clamp_to_hb_position (দ্বিগুণ v)
  {
    ফেরত (hb_position_t) hb_clamp (v,
				     (দ্বিগুণ) hb_int_min (hb_position_t),
				     (দ্বিগুণ) hb_int_max (hb_position_t));
  }
};

template <typename Float = দশমিক>
কাঠামো hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  যৌক্তিক is_identity () ধ্রুবক
  {
    ফেরত xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  যৌক্তিক is_translation () ধ্রুবক
  {
    ফেরত xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  শূন্য multiply (ধ্রুবক hb_transform_t &o, যৌক্তিক before=মিথ্যা)
  {
    // Copied from cairo-matrix.c
    ধ্রুবক hb_transform_t &a = before ? o : *এই;
    ধ্রুবক hb_transform_t &b = before ? *এই : o;
    *এই = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  শূন্য transform_distance (Float &dx, Float &dy) ধ্রুবক
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  শূন্য transform_point (Float &x, Float &y) ধ্রুবক
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  শূন্য transform_extents (hb_extents_t<Float> &extents) ধ্রুবক
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
    জন্য (unsigned i = 0; i < 4; i++)
    {
      transform_point (quad_x[i], quad_y[i]);
      extents.add_point (quad_x[i], quad_y[i]);
    }
  }

  শূন্য transform (ধ্রুবক hb_transform_t &o, যৌক্তিক before=মিথ্যা) { multiply (o, before); }

  স্থির hb_transform_t translation (Float x, Float y)
  {
    ফেরত {1, 0, 0, 1, x, y};
  }
  শূন্য translate (Float x, Float y, যৌক্তিক before=মিথ্যা)
  {
    যদি (before)
    {
      x0 += x;
      y0 += y;
    }
    অন্যথা
    {
      যদি (x == 0 && y == 0)
	ফেরত;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  স্থির hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    ফেরত {scaleX, 0, 0, scaleY, 0, 0};
  }
  শূন্য scale (Float scaleX, Float scaleY)
  {
    যদি (scaleX == 1 && scaleY == 1)
      ফেরত;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  স্থির hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    ফেরত {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  শূন্য scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    যদি (scaleX == 1 && scaleY == 1)
      ফেরত;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  স্থির hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    ফেরত {c, s, -s, c, 0, 0};
  }
  শূন্য rotate (Float radians, যৌক্তিক before=মিথ্যা)
  {
    যদি (radians == 0)
      ফেরত;

    transform (rotation (radians), before);
  }

  স্থির hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    ফেরত {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  শূন্য rotate_around_center (Float radians, Float center_x, Float center_y, যৌক্তিক before=মিথ্যা)
  {
    যদি (radians == 0)
      ফেরত;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  স্থির hb_transform_t skewing (Float skewX, Float skewY)
  {
    ফেরত {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  শূন্য skew (Float skewX, Float skewY)
  {
    যদি (skewX == 0 && skewY == 0)
      ফেরত;

    transform (skewing (skewX, skewY));
  }
  স্থির hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    ফেরত {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  শূন্য skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    যদি (skewX == 0 && skewY == 0)
	    ফেরত;

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

template <typename Float = দশমিক>
কাঠামো hb_bounds_t
{
  গণনা status_t {
    UNBOUNDED,
    BOUNDED,
    EMPTY,
  };

  hb_bounds_t (status_t status = UNBOUNDED) : status (status) {}
  hb_bounds_t (ধ্রুবক hb_extents_t<Float> &extents) :
    status (extents.is_empty () ? EMPTY : BOUNDED), extents (extents) {}

  শূন্য union_ (ধ্রুবক hb_bounds_t &o)
  {
    যদি (o.status == UNBOUNDED)
      status = UNBOUNDED;
    অন্যথা যদি (o.status == BOUNDED)
    {
      যদি (status == EMPTY)
	*এই = o;
      অন্যথা যদি (status == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  শূন্য intersect (ধ্রুবক hb_bounds_t &o)
  {
    যদি (o.status == EMPTY)
      status = EMPTY;
    অন্যথা যদি (o.status == BOUNDED)
    {
      যদি (status == UNBOUNDED)
	*এই = o;
      অন্যথা যদি (status == BOUNDED)
      {
        extents.intersect (o.extents);
	যদি (extents.is_empty ())
	  status = EMPTY;
      }
    }
  }

  status_t status;
  hb_extents_t<Float> extents;
};

template <typename Float = দশমিক>
কাঠামো hb_transform_decomposed_t
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

  operator যৌক্তিক () ধ্রুবক
  {
    ফেরত translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> to_transform () ধ্রুবক
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    ফেরত t;
  }
};


#endif /* HB_GEOMETRY_HH */
