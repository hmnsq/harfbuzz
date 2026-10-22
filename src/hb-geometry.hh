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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_GEOMETRY_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GEOMETRY_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-algs.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Float = ਦਸ਼ਮਲਵ>
ਬਣਤਰ hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (ਅਚਲ hb_glyph_extents_t &extents)
  {
    ਦੁੱਗਣਾ x0 = (ਦੁੱਗਣਾ) extents.x_bearing;
    ਦੁੱਗਣਾ y0 = (ਦੁੱਗਣਾ) extents.y_bearing;
    ਦੁੱਗਣਾ x1 = x0 + (ਦੁੱਗਣਾ) extents.ਚੌੜਾਈ;
    ਦੁੱਗਣਾ y1 = y0 + (ਦੁੱਗਣਾ) extents.ਉਚਾਈ;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  ਤਾਰਕਿਕ is_empty () ਅਚਲ { ਵਾਪਸ xmin >= xmax || ymin >= ymax; }
  ਤਾਰਕਿਕ is_void () ਅਚਲ { ਵਾਪਸ xmin > xmax; }

  ਖਾਲੀ union_ (ਅਚਲ hb_extents_t &o)
  {
    ਜੇ (o.is_empty ()) ਵਾਪਸ;
    ਜੇ (is_empty ())
    {
      *ਇਹ = o;
      ਵਾਪਸ;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  ਖਾਲੀ intersect (ਅਚਲ hb_extents_t &o)
  {
    ਜੇ (o.is_empty () || is_empty ())
    {
      *ਇਹ = hb_extents_t {};
      ਵਾਪਸ;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  ਖਾਲੀ
  ਜੋੜੋ_ਬਿੰਦੂ (Float x, Float y)
  {
    ਜੇ (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    ਨਹੀਂਤਾਂ
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t ਮੰਜ਼ਿਲ_glyph_extents (ਤਾਰਕਿਕ xneg = ਝੂਠ, ਤਾਰਕਿਕ yneg = ਝੂਠ) ਅਚਲ
  {
    ਦੁੱਗਣਾ x0 = round ((ਦੁੱਗਣਾ) xmin);
    ਦੁੱਗਣਾ y0 = round ((ਦੁੱਗਣਾ) ymin);
    ਦੁੱਗਣਾ x1 = round ((ਦੁੱਗਣਾ) xmax);
    ਦੁੱਗਣਾ y1 = round ((ਦੁੱਗਣਾ) ymax);

    ਜੇ (unlikely (!std::isfinite (x0) ||
		  !std::isfinite (y0) ||
		  !std::isfinite (x1) ||
		  !std::isfinite (y1)))
      ਵਾਪਸ hb_glyph_extents_t {0, 0, 0, 0};

    ਵਾਪਸ hb_glyph_extents_t {
      clamp_ਮੰਜ਼ਿਲ_hb_position (xneg ? x1 : x0),
      clamp_ਮੰਜ਼ਿਲ_hb_position (yneg ? y0 : y1),
      clamp_ਮੰਜ਼ਿਲ_hb_position (xneg ? x0 - x1 : x1 - x0),
      clamp_ਮੰਜ਼ਿਲ_hb_position (yneg ? y1 - y0 : y0 - y1)
    };
  }

  Float xmin = 0;
  Float ymin = 0;
  Float xmax = -1;
  Float ymax = -1;

  ਸਥਿਰ hb_position_t
  clamp_ਮੰਜ਼ਿਲ_hb_position (ਦੁੱਗਣਾ v)
  {
    ਵਾਪਸ (hb_position_t) hb_clamp (v,
				     (ਦੁੱਗਣਾ) hb_int_min (hb_position_t),
				     (ਦੁੱਗਣਾ) hb_int_max (hb_position_t));
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Float = ਦਸ਼ਮਲਵ>
ਬਣਤਰ hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  ਤਾਰਕਿਕ is_ਪਛਾਣ_ਤੱਤ () ਅਚਲ
  {
    ਵਾਪਸ xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  ਤਾਰਕਿਕ is_translation () ਅਚਲ
  {
    ਵਾਪਸ xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  ਖਾਲੀ multiply (ਅਚਲ hb_transform_t &o, ਤਾਰਕਿਕ before=ਝੂਠ)
  {
    // Copied from cairo-matrix.c
    ਅਚਲ hb_transform_t &a = before ? o : *ਇਹ;
    ਅਚਲ hb_transform_t &b = before ? *ਇਹ : o;
    *ਇਹ = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  ਖਾਲੀ transform_ਦੂਰੀ (Float &dx, Float &dy) ਅਚਲ
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  ਖਾਲੀ transform_ਬਿੰਦੂ (Float &x, Float &y) ਅਚਲ
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  ਖਾਲੀ transform_extents (hb_extents_t<Float> &extents) ਅਚਲ
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
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 4; i++)
    {
      transform_ਬਿੰਦੂ (quad_x[i], quad_y[i]);
      extents.ਜੋੜੋ_ਬਿੰਦੂ (quad_x[i], quad_y[i]);
    }
  }

  ਖਾਲੀ transform (ਅਚਲ hb_transform_t &o, ਤਾਰਕਿਕ before=ਝੂਠ) { multiply (o, before); }

  ਸਥਿਰ hb_transform_t translation (Float x, Float y)
  {
    ਵਾਪਸ {1, 0, 0, 1, x, y};
  }
  ਖਾਲੀ translate (Float x, Float y, ਤਾਰਕਿਕ before=ਝੂਠ)
  {
    ਜੇ (before)
    {
      x0 += x;
      y0 += y;
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (x == 0 && y == 0)
	ਵਾਪਸ;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  ਸਥਿਰ hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    ਵਾਪਸ {scaleX, 0, 0, scaleY, 0, 0};
  }
  ਖਾਲੀ scale (Float scaleX, Float scaleY)
  {
    ਜੇ (scaleX == 1 && scaleY == 1)
      ਵਾਪਸ;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  ਸਥਿਰ hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    ਵਾਪਸ {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  ਖਾਲੀ scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    ਜੇ (scaleX == 1 && scaleY == 1)
      ਵਾਪਸ;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  ਸਥਿਰ hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    ਵਾਪਸ {c, s, -s, c, 0, 0};
  }
  ਖਾਲੀ rotate (Float radians, ਤਾਰਕਿਕ before=ਝੂਠ)
  {
    ਜੇ (radians == 0)
      ਵਾਪਸ;

    transform (rotation (radians), before);
  }

  ਸਥਿਰ hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    ਵਾਪਸ {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  ਖਾਲੀ rotate_around_center (Float radians, Float center_x, Float center_y, ਤਾਰਕਿਕ before=ਝੂਠ)
  {
    ਜੇ (radians == 0)
      ਵਾਪਸ;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  ਸਥਿਰ hb_transform_t skewing (Float skewX, Float skewY)
  {
    ਵਾਪਸ {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  ਖਾਲੀ skew (Float skewX, Float skewY)
  {
    ਜੇ (skewX == 0 && skewY == 0)
      ਵਾਪਸ;

    transform (skewing (skewX, skewY));
  }
  ਸਥਿਰ hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    ਵਾਪਸ {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  ਖਾਲੀ skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    ਜੇ (skewX == 0 && skewY == 0)
	    ਵਾਪਸ;

    transform (skewing_around_center (skewX, skewY, center_x, center_y));
  }

  Float xx = 1;
  Float yx = 0;
  Float xy = 0;
  Float yy = 1;
  Float x0 = 0;
  Float y0 = 0;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_TRANSFORM_ਪਛਾਣ_ਤੱਤ {1, 0, 0, 1, 0, 0}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Float = ਦਸ਼ਮਲਵ>
ਬਣਤਰ hb_bounds_t
{
  ਗਿਣਤੀ ਸਥਿਤੀ_t {
    UNBOUNDED,
    BOUNDED,
    ਖਾਲੀ_ਥਾਵਾਂ,
  };

  hb_bounds_t (ਸਥਿਤੀ_t ਸਥਿਤੀ = UNBOUNDED) : ਸਥਿਤੀ (ਸਥਿਤੀ) {}
  hb_bounds_t (ਅਚਲ hb_extents_t<Float> &extents) :
    ਸਥਿਤੀ (extents.is_empty () ? ਖਾਲੀ_ਥਾਵਾਂ : BOUNDED), extents (extents) {}

  ਖਾਲੀ union_ (ਅਚਲ hb_bounds_t &o)
  {
    ਜੇ (o.ਸਥਿਤੀ == UNBOUNDED)
      ਸਥਿਤੀ = UNBOUNDED;
    ਨਹੀਂਤਾਂ ਜੇ (o.ਸਥਿਤੀ == BOUNDED)
    {
      ਜੇ (ਸਥਿਤੀ == ਖਾਲੀ_ਥਾਵਾਂ)
	*ਇਹ = o;
      ਨਹੀਂਤਾਂ ਜੇ (ਸਥਿਤੀ == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  ਖਾਲੀ intersect (ਅਚਲ hb_bounds_t &o)
  {
    ਜੇ (o.ਸਥਿਤੀ == ਖਾਲੀ_ਥਾਵਾਂ)
      ਸਥਿਤੀ = ਖਾਲੀ_ਥਾਵਾਂ;
    ਨਹੀਂਤਾਂ ਜੇ (o.ਸਥਿਤੀ == BOUNDED)
    {
      ਜੇ (ਸਥਿਤੀ == UNBOUNDED)
	*ਇਹ = o;
      ਨਹੀਂਤਾਂ ਜੇ (ਸਥਿਤੀ == BOUNDED)
      {
        extents.intersect (o.extents);
	ਜੇ (extents.is_empty ())
	  ਸਥਿਤੀ = ਖਾਲੀ_ਥਾਵਾਂ;
      }
    }
  }

  ਸਥਿਤੀ_t ਸਥਿਤੀ;
  hb_extents_t<Float> extents;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Float = ਦਸ਼ਮਲਵ>
ਬਣਤਰ hb_transform_decomposed_t
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

  ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ
  {
    ਵਾਪਸ translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> ਮੰਜ਼ਿਲ_transform () ਅਚਲ
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    ਵਾਪਸ t;
  }
};


#ਜੇ_ਅੰਤ /* HB_GEOMETRY_HH */
