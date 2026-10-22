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
#اگر_غیر_تعریف_شدہ HB_GEOMETRY_HH
#تعریف_کرو HB_GEOMETRY_HH

#شامل "hb.hh"

#شامل "hb-algs.hh"

#شامل <cmath>


سانچہ <قسم_نام Float = اعشاری>
ساخت hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (ثابت hb_glyph_extents_t &extents)
  {
    دوہرا x0 = (دوہرا) extents.x_bearing;
    دوہرا y0 = (دوہرا) extents.y_bearing;
    دوہرا x1 = x0 + (دوہرا) extents.چوڑائی;
    دوہرا y1 = y0 + (دوہرا) extents.اونچائی;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  منطقی is_empty () ثابت { واپس xmin >= xmax || ymin >= ymax; }
  منطقی is_void () ثابت { واپس xmin > xmax; }

  خالی union_ (ثابت hb_extents_t &o)
  {
    اگر (o.is_empty ()) واپس;
    اگر (is_empty ())
    {
      *یہ = o;
      واپس;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  خالی intersect (ثابت hb_extents_t &o)
  {
    اگر (o.is_empty () || is_empty ())
    {
      *یہ = hb_extents_t {};
      واپس;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  خالی
  جوڑو_نقطہ (Float x, Float y)
  {
    اگر (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    ورنہ
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t مقصد_glyph_extents (منطقی xneg = جھوٹ, منطقی yneg = جھوٹ) ثابت
  {
    دوہرا x0 = round ((دوہرا) xmin);
    دوہرا y0 = round ((دوہرا) ymin);
    دوہرا x1 = round ((دوہرا) xmax);
    دوہرا y1 = round ((دوہرا) ymax);

    اگر (unlikely (!std::isfinite (x0) ||
		  !std::isfinite (y0) ||
		  !std::isfinite (x1) ||
		  !std::isfinite (y1)))
      واپس hb_glyph_extents_t {0, 0, 0, 0};

    واپس hb_glyph_extents_t {
      clamp_مقصد_hb_position (xneg ? x1 : x0),
      clamp_مقصد_hb_position (yneg ? y0 : y1),
      clamp_مقصد_hb_position (xneg ? x0 - x1 : x1 - x0),
      clamp_مقصد_hb_position (yneg ? y1 - y0 : y0 - y1)
    };
  }

  Float xmin = 0;
  Float ymin = 0;
  Float xmax = -1;
  Float ymax = -1;

  ساکن hb_position_t
  clamp_مقصد_hb_position (دوہرا v)
  {
    واپس (hb_position_t) hb_clamp (v,
				     (دوہرا) hb_int_min (hb_position_t),
				     (دوہرا) hb_int_max (hb_position_t));
  }
};

سانچہ <قسم_نام Float = اعشاری>
ساخت hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  منطقی is_شناختی_عنصر () ثابت
  {
    واپس xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  منطقی is_translation () ثابت
  {
    واپس xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  خالی multiply (ثابت hb_transform_t &o, منطقی before=جھوٹ)
  {
    // Copied from cairo-matrix.c
    ثابت hb_transform_t &a = before ? o : *یہ;
    ثابت hb_transform_t &b = before ? *یہ : o;
    *یہ = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  خالی transform_فاصلہ (Float &dx, Float &dy) ثابت
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  خالی transform_نقطہ (Float &x, Float &y) ثابت
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  خالی transform_extents (hb_extents_t<Float> &extents) ثابت
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
    برائے (غیرعلامتی i = 0; i < 4; i++)
    {
      transform_نقطہ (quad_x[i], quad_y[i]);
      extents.جوڑو_نقطہ (quad_x[i], quad_y[i]);
    }
  }

  خالی transform (ثابت hb_transform_t &o, منطقی before=جھوٹ) { multiply (o, before); }

  ساکن hb_transform_t translation (Float x, Float y)
  {
    واپس {1, 0, 0, 1, x, y};
  }
  خالی translate (Float x, Float y, منطقی before=جھوٹ)
  {
    اگر (before)
    {
      x0 += x;
      y0 += y;
    }
    ورنہ
    {
      اگر (x == 0 && y == 0)
	واپس;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  ساکن hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    واپس {scaleX, 0, 0, scaleY, 0, 0};
  }
  خالی scale (Float scaleX, Float scaleY)
  {
    اگر (scaleX == 1 && scaleY == 1)
      واپس;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  ساکن hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    واپس {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  خالی scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    اگر (scaleX == 1 && scaleY == 1)
      واپس;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  ساکن hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    واپس {c, s, -s, c, 0, 0};
  }
  خالی rotate (Float radians, منطقی before=جھوٹ)
  {
    اگر (radians == 0)
      واپس;

    transform (rotation (radians), before);
  }

  ساکن hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    واپس {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  خالی rotate_around_center (Float radians, Float center_x, Float center_y, منطقی before=جھوٹ)
  {
    اگر (radians == 0)
      واپس;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  ساکن hb_transform_t skewing (Float skewX, Float skewY)
  {
    واپس {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  خالی skew (Float skewX, Float skewY)
  {
    اگر (skewX == 0 && skewY == 0)
      واپس;

    transform (skewing (skewX, skewY));
  }
  ساکن hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    واپس {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  خالی skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    اگر (skewX == 0 && skewY == 0)
	    واپس;

    transform (skewing_around_center (skewX, skewY, center_x, center_y));
  }

  Float xx = 1;
  Float yx = 0;
  Float xy = 0;
  Float yy = 1;
  Float x0 = 0;
  Float y0 = 0;
};

#تعریف_کرو HB_TRANSFORM_شناختی_عنصر {1, 0, 0, 1, 0, 0}

سانچہ <قسم_نام Float = اعشاری>
ساخت hb_bounds_t
{
  شمار حالت_t {
    UNBOUNDED,
    BOUNDED,
    خالی_خانے,
  };

  hb_bounds_t (حالت_t حالت_2 = UNBOUNDED) : حالت_2 (حالت_2) {}
  hb_bounds_t (ثابت hb_extents_t<Float> &extents) :
    حالت_2 (extents.is_empty () ? خالی_خانے : BOUNDED), extents (extents) {}

  خالی union_ (ثابت hb_bounds_t &o)
  {
    اگر (o.حالت_2 == UNBOUNDED)
      حالت_2 = UNBOUNDED;
    ورنہ اگر (o.حالت_2 == BOUNDED)
    {
      اگر (حالت_2 == خالی_خانے)
	*یہ = o;
      ورنہ اگر (حالت_2 == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  خالی intersect (ثابت hb_bounds_t &o)
  {
    اگر (o.حالت_2 == خالی_خانے)
      حالت_2 = خالی_خانے;
    ورنہ اگر (o.حالت_2 == BOUNDED)
    {
      اگر (حالت_2 == UNBOUNDED)
	*یہ = o;
      ورنہ اگر (حالت_2 == BOUNDED)
      {
        extents.intersect (o.extents);
	اگر (extents.is_empty ())
	  حالت_2 = خالی_خانے;
      }
    }
  }

  حالت_t حالت_2;
  hb_extents_t<Float> extents;
};

سانچہ <قسم_نام Float = اعشاری>
ساخت hb_transform_decomposed_t
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

  عامل منطقی () ثابت
  {
    واپس translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> مقصد_transform () ثابت
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    واپس t;
  }
};


#اگر_ختم /* HB_GEOMETRY_HH */
