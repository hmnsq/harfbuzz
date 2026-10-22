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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-algs.hh"

#ಅನ್ತರ್ಭಾವ <cmath>


template <typename Float = ಭಿನ್ನ>
ಸಂರಚನಾ hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (ಧ್ರುವ hb_glyph_extents_t &extents)
  {
    ದ್ವಿಗುಣ x0 = (ದ್ವಿಗುಣ) extents.x_bearing;
    ದ್ವಿಗುಣ y0 = (ದ್ವಿಗುಣ) extents.y_bearing;
    ದ್ವಿಗುಣ x1 = x0 + (ದ್ವಿಗುಣ) extents.width;
    ದ್ವಿಗುಣ y1 = y0 + (ದ್ವಿಗುಣ) extents.height;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  ತಾರ್ಕಿಕ is_empty () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ xmin >= xmax || ymin >= ymax; }
  ತಾರ್ಕಿಕ is_void () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ xmin > xmax; }

  ಶೂನ್ಯ union_ (ಧ್ರುವ hb_extents_t &o)
  {
    ಯದಿ (o.is_empty ()) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (is_empty ())
    {
      *ಏತತ್ = o;
      ಪ್ರತ್ಯಾಗಮ;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  ಶೂನ್ಯ intersect (ಧ್ರುವ hb_extents_t &o)
  {
    ಯದಿ (o.is_empty () || is_empty ())
    {
      *ಏತತ್ = hb_extents_t {};
      ಪ್ರತ್ಯಾಗಮ;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  ಶೂನ್ಯ
  add_point (Float x, Float y)
  {
    ಯದಿ (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    ಅನ್ಯಥಾ
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t to_glyph_extents (ತಾರ್ಕಿಕ xneg = ಅಸತ್ಯ, ತಾರ್ಕಿಕ yneg = ಅಸತ್ಯ) ಧ್ರುವ
  {
    ದ್ವಿಗುಣ x0 = round ((ದ್ವಿಗುಣ) xmin);
    ದ್ವಿಗುಣ y0 = round ((ದ್ವಿಗುಣ) ymin);
    ದ್ವಿಗುಣ x1 = round ((ದ್ವಿಗುಣ) xmax);
    ದ್ವಿಗುಣ y1 = round ((ದ್ವಿಗುಣ) ymax);

    ಯದಿ (unlikely (!std::isfinite (x0) ||
		  !std::isfinite (y0) ||
		  !std::isfinite (x1) ||
		  !std::isfinite (y1)))
      ಪ್ರತ್ಯಾಗಮ hb_glyph_extents_t {0, 0, 0, 0};

    ಪ್ರತ್ಯಾಗಮ hb_glyph_extents_t {
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

  ಸ್ಥಿರ hb_position_t
  clamp_to_hb_position (ದ್ವಿಗುಣ v)
  {
    ಪ್ರತ್ಯಾಗಮ (hb_position_t) hb_clamp (v,
				     (ದ್ವಿಗುಣ) hb_int_min (hb_position_t),
				     (ದ್ವಿಗುಣ) hb_int_max (hb_position_t));
  }
};

template <typename Float = ಭಿನ್ನ>
ಸಂರಚನಾ hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  ತಾರ್ಕಿಕ is_identity () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  ತಾರ್ಕಿಕ is_translation () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  ಶೂನ್ಯ multiply (ಧ್ರುವ hb_transform_t &o, ತಾರ್ಕಿಕ before=ಅಸತ್ಯ)
  {
    // Copied from cairo-matrix.c
    ಧ್ರುವ hb_transform_t &a = before ? o : *ಏತತ್;
    ಧ್ರುವ hb_transform_t &b = before ? *ಏತತ್ : o;
    *ಏತತ್ = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  ಶೂನ್ಯ transform_distance (Float &dx, Float &dy) ಧ್ರುವ
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  ಶೂನ್ಯ transform_point (Float &x, Float &y) ಧ್ರುವ
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  ಶೂನ್ಯ transform_extents (hb_extents_t<Float> &extents) ಧ್ರುವ
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
    ಪ್ರತಿ (unsigned i = 0; i < 4; i++)
    {
      transform_point (quad_x[i], quad_y[i]);
      extents.add_point (quad_x[i], quad_y[i]);
    }
  }

  ಶೂನ್ಯ transform (ಧ್ರುವ hb_transform_t &o, ತಾರ್ಕಿಕ before=ಅಸತ್ಯ) { multiply (o, before); }

  ಸ್ಥಿರ hb_transform_t translation (Float x, Float y)
  {
    ಪ್ರತ್ಯಾಗಮ {1, 0, 0, 1, x, y};
  }
  ಶೂನ್ಯ translate (Float x, Float y, ತಾರ್ಕಿಕ before=ಅಸತ್ಯ)
  {
    ಯದಿ (before)
    {
      x0 += x;
      y0 += y;
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (x == 0 && y == 0)
	ಪ್ರತ್ಯಾಗಮ;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  ಸ್ಥಿರ hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    ಪ್ರತ್ಯಾಗಮ {scaleX, 0, 0, scaleY, 0, 0};
  }
  ಶೂನ್ಯ scale (Float scaleX, Float scaleY)
  {
    ಯದಿ (scaleX == 1 && scaleY == 1)
      ಪ್ರತ್ಯಾಗಮ;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  ಸ್ಥಿರ hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    ಪ್ರತ್ಯಾಗಮ {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  ಶೂನ್ಯ scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    ಯದಿ (scaleX == 1 && scaleY == 1)
      ಪ್ರತ್ಯಾಗಮ;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  ಸ್ಥಿರ hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    ಪ್ರತ್ಯಾಗಮ {c, s, -s, c, 0, 0};
  }
  ಶೂನ್ಯ rotate (Float radians, ತಾರ್ಕಿಕ before=ಅಸತ್ಯ)
  {
    ಯದಿ (radians == 0)
      ಪ್ರತ್ಯಾಗಮ;

    transform (rotation (radians), before);
  }

  ಸ್ಥಿರ hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    ಪ್ರತ್ಯಾಗಮ {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  ಶೂನ್ಯ rotate_around_center (Float radians, Float center_x, Float center_y, ತಾರ್ಕಿಕ before=ಅಸತ್ಯ)
  {
    ಯದಿ (radians == 0)
      ಪ್ರತ್ಯಾಗಮ;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  ಸ್ಥಿರ hb_transform_t skewing (Float skewX, Float skewY)
  {
    ಪ್ರತ್ಯಾಗಮ {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  ಶೂನ್ಯ skew (Float skewX, Float skewY)
  {
    ಯದಿ (skewX == 0 && skewY == 0)
      ಪ್ರತ್ಯಾಗಮ;

    transform (skewing (skewX, skewY));
  }
  ಸ್ಥಿರ hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    ಪ್ರತ್ಯಾಗಮ {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  ಶೂನ್ಯ skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    ಯದಿ (skewX == 0 && skewY == 0)
	    ಪ್ರತ್ಯಾಗಮ;

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

template <typename Float = ಭಿನ್ನ>
ಸಂರಚನಾ hb_bounds_t
{
  ಗಣನಾ status_t {
    UNBOUNDED,
    BOUNDED,
    EMPTY,
  };

  hb_bounds_t (status_t status = UNBOUNDED) : status (status) {}
  hb_bounds_t (ಧ್ರುವ hb_extents_t<Float> &extents) :
    status (extents.is_empty () ? EMPTY : BOUNDED), extents (extents) {}

  ಶೂನ್ಯ union_ (ಧ್ರುವ hb_bounds_t &o)
  {
    ಯದಿ (o.status == UNBOUNDED)
      status = UNBOUNDED;
    ಅನ್ಯಥಾ ಯದಿ (o.status == BOUNDED)
    {
      ಯದಿ (status == EMPTY)
	*ಏತತ್ = o;
      ಅನ್ಯಥಾ ಯದಿ (status == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  ಶೂನ್ಯ intersect (ಧ್ರುವ hb_bounds_t &o)
  {
    ಯದಿ (o.status == EMPTY)
      status = EMPTY;
    ಅನ್ಯಥಾ ಯದಿ (o.status == BOUNDED)
    {
      ಯದಿ (status == UNBOUNDED)
	*ಏತತ್ = o;
      ಅನ್ಯಥಾ ಯದಿ (status == BOUNDED)
      {
        extents.intersect (o.extents);
	ಯದಿ (extents.is_empty ())
	  status = EMPTY;
      }
    }
  }

  status_t status;
  hb_extents_t<Float> extents;
};

template <typename Float = ಭಿನ್ನ>
ಸಂರಚನಾ hb_transform_decomposed_t
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

  operator ತಾರ್ಕಿಕ () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> to_transform () ಧ್ರುವ
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    ಪ್ರತ್ಯಾಗಮ t;
  }
};


#endif /* HB_GEOMETRY_HH */
