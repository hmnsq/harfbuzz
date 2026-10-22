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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-algs.hh"

#ଅନ୍ତର୍ଭାଵ <cmath>


template <typename Float = ଦଶମିକ>
ସଂରଚନା hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (ଅଚଳ hb_glyph_extents_t &extents)
  {
    ଦ୍ୱିଗୁଣ x0 = (ଦ୍ୱିଗୁଣ) extents.x_bearing;
    ଦ୍ୱିଗୁଣ y0 = (ଦ୍ୱିଗୁଣ) extents.y_bearing;
    ଦ୍ୱିଗୁଣ x1 = x0 + (ଦ୍ୱିଗୁଣ) extents.width;
    ଦ୍ୱିଗୁଣ y1 = y0 + (ଦ୍ୱିଗୁଣ) extents.height;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  ତାର୍କିକ is_empty () ଅଚଳ { ଫେରନ୍ତୁ xmin >= xmax || ymin >= ymax; }
  ତାର୍କିକ is_void () ଅଚଳ { ଫେରନ୍ତୁ xmin > xmax; }

  ଶୂନ୍ୟ union_ (ଅଚଳ hb_extents_t &o)
  {
    ଯଦି (o.is_empty ()) ଫେରନ୍ତୁ;
    ଯଦି (is_empty ())
    {
      *ଏହି = o;
      ଫେରନ୍ତୁ;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  ଶୂନ୍ୟ intersect (ଅଚଳ hb_extents_t &o)
  {
    ଯଦି (o.is_empty () || is_empty ())
    {
      *ଏହି = hb_extents_t {};
      ଫେରନ୍ତୁ;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  ଶୂନ୍ୟ
  add_point (Float x, Float y)
  {
    ଯଦି (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    ଅନ୍ୟଥା
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t to_glyph_extents (ତାର୍କିକ xneg = ମିଥ୍ୟା, ତାର୍କିକ yneg = ମିଥ୍ୟା) ଅଚଳ
  {
    ଦ୍ୱିଗୁଣ x0 = round ((ଦ୍ୱିଗୁଣ) xmin);
    ଦ୍ୱିଗୁଣ y0 = round ((ଦ୍ୱିଗୁଣ) ymin);
    ଦ୍ୱିଗୁଣ x1 = round ((ଦ୍ୱିଗୁଣ) xmax);
    ଦ୍ୱିଗୁଣ y1 = round ((ଦ୍ୱିଗୁଣ) ymax);

    ଯଦି (unlikely (!std::isfinite (x0) ||
		  !std::isfinite (y0) ||
		  !std::isfinite (x1) ||
		  !std::isfinite (y1)))
      ଫେରନ୍ତୁ hb_glyph_extents_t {0, 0, 0, 0};

    ଫେରନ୍ତୁ hb_glyph_extents_t {
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

  ସ୍ଥିର hb_position_t
  clamp_to_hb_position (ଦ୍ୱିଗୁଣ v)
  {
    ଫେରନ୍ତୁ (hb_position_t) hb_clamp (v,
				     (ଦ୍ୱିଗୁଣ) hb_int_min (hb_position_t),
				     (ଦ୍ୱିଗୁଣ) hb_int_max (hb_position_t));
  }
};

template <typename Float = ଦଶମିକ>
ସଂରଚନା hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  ତାର୍କିକ is_identity () ଅଚଳ
  {
    ଫେରନ୍ତୁ xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  ତାର୍କିକ is_translation () ଅଚଳ
  {
    ଫେରନ୍ତୁ xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  ଶୂନ୍ୟ multiply (ଅଚଳ hb_transform_t &o, ତାର୍କିକ before=ମିଥ୍ୟା)
  {
    // Copied from cairo-matrix.c
    ଅଚଳ hb_transform_t &a = before ? o : *ଏହି;
    ଅଚଳ hb_transform_t &b = before ? *ଏହି : o;
    *ଏହି = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ transform_distance (Float &dx, Float &dy) ଅଚଳ
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ transform_point (Float &x, Float &y) ଅଚଳ
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  ଶୂନ୍ୟ transform_extents (hb_extents_t<Float> &extents) ଅଚଳ
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
    ପାଇଁ (unsigned i = 0; i < 4; i++)
    {
      transform_point (quad_x[i], quad_y[i]);
      extents.add_point (quad_x[i], quad_y[i]);
    }
  }

  ଶୂନ୍ୟ transform (ଅଚଳ hb_transform_t &o, ତାର୍କିକ before=ମିଥ୍ୟା) { multiply (o, before); }

  ସ୍ଥିର hb_transform_t translation (Float x, Float y)
  {
    ଫେରନ୍ତୁ {1, 0, 0, 1, x, y};
  }
  ଶୂନ୍ୟ translate (Float x, Float y, ତାର୍କିକ before=ମିଥ୍ୟା)
  {
    ଯଦି (before)
    {
      x0 += x;
      y0 += y;
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (x == 0 && y == 0)
	ଫେରନ୍ତୁ;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  ସ୍ଥିର hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    ଫେରନ୍ତୁ {scaleX, 0, 0, scaleY, 0, 0};
  }
  ଶୂନ୍ୟ scale (Float scaleX, Float scaleY)
  {
    ଯଦି (scaleX == 1 && scaleY == 1)
      ଫେରନ୍ତୁ;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  ସ୍ଥିର hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    ଫେରନ୍ତୁ {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  ଶୂନ୍ୟ scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    ଯଦି (scaleX == 1 && scaleY == 1)
      ଫେରନ୍ତୁ;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  ସ୍ଥିର hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    ଫେରନ୍ତୁ {c, s, -s, c, 0, 0};
  }
  ଶୂନ୍ୟ rotate (Float radians, ତାର୍କିକ before=ମିଥ୍ୟା)
  {
    ଯଦି (radians == 0)
      ଫେରନ୍ତୁ;

    transform (rotation (radians), before);
  }

  ସ୍ଥିର hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    ଫେରନ୍ତୁ {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  ଶୂନ୍ୟ rotate_around_center (Float radians, Float center_x, Float center_y, ତାର୍କିକ before=ମିଥ୍ୟା)
  {
    ଯଦି (radians == 0)
      ଫେରନ୍ତୁ;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  ସ୍ଥିର hb_transform_t skewing (Float skewX, Float skewY)
  {
    ଫେରନ୍ତୁ {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  ଶୂନ୍ୟ skew (Float skewX, Float skewY)
  {
    ଯଦି (skewX == 0 && skewY == 0)
      ଫେରନ୍ତୁ;

    transform (skewing (skewX, skewY));
  }
  ସ୍ଥିର hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    ଫେରନ୍ତୁ {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  ଶୂନ୍ୟ skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    ଯଦି (skewX == 0 && skewY == 0)
	    ଫେରନ୍ତୁ;

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

template <typename Float = ଦଶମିକ>
ସଂରଚନା hb_bounds_t
{
  ଗଣନା status_t {
    UNBOUNDED,
    BOUNDED,
    EMPTY,
  };

  hb_bounds_t (status_t status = UNBOUNDED) : status (status) {}
  hb_bounds_t (ଅଚଳ hb_extents_t<Float> &extents) :
    status (extents.is_empty () ? EMPTY : BOUNDED), extents (extents) {}

  ଶୂନ୍ୟ union_ (ଅଚଳ hb_bounds_t &o)
  {
    ଯଦି (o.status == UNBOUNDED)
      status = UNBOUNDED;
    ଅନ୍ୟଥା ଯଦି (o.status == BOUNDED)
    {
      ଯଦି (status == EMPTY)
	*ଏହି = o;
      ଅନ୍ୟଥା ଯଦି (status == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  ଶୂନ୍ୟ intersect (ଅଚଳ hb_bounds_t &o)
  {
    ଯଦି (o.status == EMPTY)
      status = EMPTY;
    ଅନ୍ୟଥା ଯଦି (o.status == BOUNDED)
    {
      ଯଦି (status == UNBOUNDED)
	*ଏହି = o;
      ଅନ୍ୟଥା ଯଦି (status == BOUNDED)
      {
        extents.intersect (o.extents);
	ଯଦି (extents.is_empty ())
	  status = EMPTY;
      }
    }
  }

  status_t status;
  hb_extents_t<Float> extents;
};

template <typename Float = ଦଶମିକ>
ସଂରଚନା hb_transform_decomposed_t
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

  operator ତାର୍କିକ () ଅଚଳ
  {
    ଫେରନ୍ତୁ translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> to_transform () ଅଚଳ
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    ଫେରନ୍ତୁ t;
  }
};


#endif /* HB_GEOMETRY_HH */
