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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-algs.hh"

#അന്തര്ഭാവ <cmath>


template <typename Float = ദശാംശം>
ഘടന hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (സ്ഥിരം hb_glyph_extents_t &extents)
  {
    ഇരട്ട x0 = (ഇരട്ട) extents.x_bearing;
    ഇരട്ട y0 = (ഇരട്ട) extents.y_bearing;
    ഇരട്ട x1 = x0 + (ഇരട്ട) extents.width;
    ഇരട്ട y1 = y0 + (ഇരട്ട) extents.height;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  തർക്കം is_empty () സ്ഥിരം { തിരികെ xmin >= xmax || ymin >= ymax; }
  തർക്കം is_void () സ്ഥിരം { തിരികെ xmin > xmax; }

  ശൂന്യം union_ (സ്ഥിരം hb_extents_t &o)
  {
    എങ്കിൽ (o.is_empty ()) തിരികെ;
    എങ്കിൽ (is_empty ())
    {
      *ഇത് = o;
      തിരികെ;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  ശൂന്യം intersect (സ്ഥിരം hb_extents_t &o)
  {
    എങ്കിൽ (o.is_empty () || is_empty ())
    {
      *ഇത് = hb_extents_t {};
      തിരികെ;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  ശൂന്യം
  add_point (Float x, Float y)
  {
    എങ്കിൽ (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    അല്ലെങ്കിൽ
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t to_glyph_extents (തർക്കം xneg = അസത്യം, തർക്കം yneg = അസത്യം) സ്ഥിരം
  {
    ഇരട്ട x0 = round ((ഇരട്ട) xmin);
    ഇരട്ട y0 = round ((ഇരട്ട) ymin);
    ഇരട്ട x1 = round ((ഇരട്ട) xmax);
    ഇരട്ട y1 = round ((ഇരട്ട) ymax);

    എങ്കിൽ (unlikely (!std::isfinite (x0) ||
		  !std::isfinite (y0) ||
		  !std::isfinite (x1) ||
		  !std::isfinite (y1)))
      തിരികെ hb_glyph_extents_t {0, 0, 0, 0};

    തിരികെ hb_glyph_extents_t {
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

  സ്ഥിതം hb_position_t
  clamp_to_hb_position (ഇരട്ട v)
  {
    തിരികെ (hb_position_t) hb_clamp (v,
				     (ഇരട്ട) hb_int_min (hb_position_t),
				     (ഇരട്ട) hb_int_max (hb_position_t));
  }
};

template <typename Float = ദശാംശം>
ഘടന hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  തർക്കം is_identity () സ്ഥിരം
  {
    തിരികെ xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  തർക്കം is_translation () സ്ഥിരം
  {
    തിരികെ xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  ശൂന്യം multiply (സ്ഥിരം hb_transform_t &o, തർക്കം before=അസത്യം)
  {
    // Copied from cairo-matrix.c
    സ്ഥിരം hb_transform_t &a = before ? o : *ഇത്;
    സ്ഥിരം hb_transform_t &b = before ? *ഇത് : o;
    *ഇത് = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  ശൂന്യം transform_distance (Float &dx, Float &dy) സ്ഥിരം
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  ശൂന്യം transform_point (Float &x, Float &y) സ്ഥിരം
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  ശൂന്യം transform_extents (hb_extents_t<Float> &extents) സ്ഥിരം
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
    വേണ്ടി (unsigned i = 0; i < 4; i++)
    {
      transform_point (quad_x[i], quad_y[i]);
      extents.add_point (quad_x[i], quad_y[i]);
    }
  }

  ശൂന്യം transform (സ്ഥിരം hb_transform_t &o, തർക്കം before=അസത്യം) { multiply (o, before); }

  സ്ഥിതം hb_transform_t translation (Float x, Float y)
  {
    തിരികെ {1, 0, 0, 1, x, y};
  }
  ശൂന്യം translate (Float x, Float y, തർക്കം before=അസത്യം)
  {
    എങ്കിൽ (before)
    {
      x0 += x;
      y0 += y;
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (x == 0 && y == 0)
	തിരികെ;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  സ്ഥിതം hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    തിരികെ {scaleX, 0, 0, scaleY, 0, 0};
  }
  ശൂന്യം scale (Float scaleX, Float scaleY)
  {
    എങ്കിൽ (scaleX == 1 && scaleY == 1)
      തിരികെ;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  സ്ഥിതം hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    തിരികെ {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  ശൂന്യം scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    എങ്കിൽ (scaleX == 1 && scaleY == 1)
      തിരികെ;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  സ്ഥിതം hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    തിരികെ {c, s, -s, c, 0, 0};
  }
  ശൂന്യം rotate (Float radians, തർക്കം before=അസത്യം)
  {
    എങ്കിൽ (radians == 0)
      തിരികെ;

    transform (rotation (radians), before);
  }

  സ്ഥിതം hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    തിരികെ {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  ശൂന്യം rotate_around_center (Float radians, Float center_x, Float center_y, തർക്കം before=അസത്യം)
  {
    എങ്കിൽ (radians == 0)
      തിരികെ;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  സ്ഥിതം hb_transform_t skewing (Float skewX, Float skewY)
  {
    തിരികെ {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  ശൂന്യം skew (Float skewX, Float skewY)
  {
    എങ്കിൽ (skewX == 0 && skewY == 0)
      തിരികെ;

    transform (skewing (skewX, skewY));
  }
  സ്ഥിതം hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    തിരികെ {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  ശൂന്യം skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    എങ്കിൽ (skewX == 0 && skewY == 0)
	    തിരികെ;

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

template <typename Float = ദശാംശം>
ഘടന hb_bounds_t
{
  എണ്ണൽ status_t {
    UNBOUNDED,
    BOUNDED,
    EMPTY,
  };

  hb_bounds_t (status_t status = UNBOUNDED) : status (status) {}
  hb_bounds_t (സ്ഥിരം hb_extents_t<Float> &extents) :
    status (extents.is_empty () ? EMPTY : BOUNDED), extents (extents) {}

  ശൂന്യം union_ (സ്ഥിരം hb_bounds_t &o)
  {
    എങ്കിൽ (o.status == UNBOUNDED)
      status = UNBOUNDED;
    അല്ലെങ്കിൽ എങ്കിൽ (o.status == BOUNDED)
    {
      എങ്കിൽ (status == EMPTY)
	*ഇത് = o;
      അല്ലെങ്കിൽ എങ്കിൽ (status == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  ശൂന്യം intersect (സ്ഥിരം hb_bounds_t &o)
  {
    എങ്കിൽ (o.status == EMPTY)
      status = EMPTY;
    അല്ലെങ്കിൽ എങ്കിൽ (o.status == BOUNDED)
    {
      എങ്കിൽ (status == UNBOUNDED)
	*ഇത് = o;
      അല്ലെങ്കിൽ എങ്കിൽ (status == BOUNDED)
      {
        extents.intersect (o.extents);
	എങ്കിൽ (extents.is_empty ())
	  status = EMPTY;
      }
    }
  }

  status_t status;
  hb_extents_t<Float> extents;
};

template <typename Float = ദശാംശം>
ഘടന hb_transform_decomposed_t
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

  operator തർക്കം () സ്ഥിരം
  {
    തിരികെ translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> to_transform () സ്ഥിരം
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    തിരികെ t;
  }
};


#endif /* HB_GEOMETRY_HH */
