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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-algs.hh"

#અન્તર્ભાવ <cmath>


template <typename Float = દશાંશ>
સંરચના hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (અચલ hb_glyph_extents_t &extents)
  {
    દ્વિગુણ x0 = (દ્વિગુણ) extents.x_bearing;
    દ્વિગુણ y0 = (દ્વિગુણ) extents.y_bearing;
    દ્વિગુણ x1 = x0 + (દ્વિગુણ) extents.width;
    દ્વિગુણ y1 = y0 + (દ્વિગુણ) extents.height;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  તાર્કિક is_empty () અચલ { પાછા xmin >= xmax || ymin >= ymax; }
  તાર્કિક is_void () અચલ { પાછા xmin > xmax; }

  શૂન્ય union_ (અચલ hb_extents_t &o)
  {
    જો (o.is_empty ()) પાછા;
    જો (is_empty ())
    {
      *આ = o;
      પાછા;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  શૂન્ય intersect (અચલ hb_extents_t &o)
  {
    જો (o.is_empty () || is_empty ())
    {
      *આ = hb_extents_t {};
      પાછા;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  શૂન્ય
  add_point (Float x, Float y)
  {
    જો (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    નહીંતર
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t to_glyph_extents (તાર્કિક xneg = ખોટું, તાર્કિક yneg = ખોટું) અચલ
  {
    દ્વિગુણ x0 = round ((દ્વિગુણ) xmin);
    દ્વિગુણ y0 = round ((દ્વિગુણ) ymin);
    દ્વિગુણ x1 = round ((દ્વિગુણ) xmax);
    દ્વિગુણ y1 = round ((દ્વિગુણ) ymax);

    જો (unlikely (!std::isfinite (x0) ||
		  !std::isfinite (y0) ||
		  !std::isfinite (x1) ||
		  !std::isfinite (y1)))
      પાછા hb_glyph_extents_t {0, 0, 0, 0};

    પાછા hb_glyph_extents_t {
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

  સ્થિર hb_position_t
  clamp_to_hb_position (દ્વિગુણ v)
  {
    પાછા (hb_position_t) hb_clamp (v,
				     (દ્વિગુણ) hb_int_min (hb_position_t),
				     (દ્વિગુણ) hb_int_max (hb_position_t));
  }
};

template <typename Float = દશાંશ>
સંરચના hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  તાર્કિક is_identity () અચલ
  {
    પાછા xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  તાર્કિક is_translation () અચલ
  {
    પાછા xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  શૂન્ય multiply (અચલ hb_transform_t &o, તાર્કિક before=ખોટું)
  {
    // Copied from cairo-matrix.c
    અચલ hb_transform_t &a = before ? o : *આ;
    અચલ hb_transform_t &b = before ? *આ : o;
    *આ = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  શૂન્ય transform_distance (Float &dx, Float &dy) અચલ
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  શૂન્ય transform_point (Float &x, Float &y) અચલ
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  શૂન્ય transform_extents (hb_extents_t<Float> &extents) અચલ
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
    માટે (unsigned i = 0; i < 4; i++)
    {
      transform_point (quad_x[i], quad_y[i]);
      extents.add_point (quad_x[i], quad_y[i]);
    }
  }

  શૂન્ય transform (અચલ hb_transform_t &o, તાર્કિક before=ખોટું) { multiply (o, before); }

  સ્થિર hb_transform_t translation (Float x, Float y)
  {
    પાછા {1, 0, 0, 1, x, y};
  }
  શૂન્ય translate (Float x, Float y, તાર્કિક before=ખોટું)
  {
    જો (before)
    {
      x0 += x;
      y0 += y;
    }
    નહીંતર
    {
      જો (x == 0 && y == 0)
	પાછા;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  સ્થિર hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    પાછા {scaleX, 0, 0, scaleY, 0, 0};
  }
  શૂન્ય scale (Float scaleX, Float scaleY)
  {
    જો (scaleX == 1 && scaleY == 1)
      પાછા;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  સ્થિર hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    પાછા {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  શૂન્ય scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    જો (scaleX == 1 && scaleY == 1)
      પાછા;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  સ્થિર hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    પાછા {c, s, -s, c, 0, 0};
  }
  શૂન્ય rotate (Float radians, તાર્કિક before=ખોટું)
  {
    જો (radians == 0)
      પાછા;

    transform (rotation (radians), before);
  }

  સ્થિર hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    પાછા {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  શૂન્ય rotate_around_center (Float radians, Float center_x, Float center_y, તાર્કિક before=ખોટું)
  {
    જો (radians == 0)
      પાછા;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  સ્થિર hb_transform_t skewing (Float skewX, Float skewY)
  {
    પાછા {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  શૂન્ય skew (Float skewX, Float skewY)
  {
    જો (skewX == 0 && skewY == 0)
      પાછા;

    transform (skewing (skewX, skewY));
  }
  સ્થિર hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    પાછા {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  શૂન્ય skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    જો (skewX == 0 && skewY == 0)
	    પાછા;

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

template <typename Float = દશાંશ>
સંરચના hb_bounds_t
{
  ગણના status_t {
    UNBOUNDED,
    BOUNDED,
    EMPTY,
  };

  hb_bounds_t (status_t status = UNBOUNDED) : status (status) {}
  hb_bounds_t (અચલ hb_extents_t<Float> &extents) :
    status (extents.is_empty () ? EMPTY : BOUNDED), extents (extents) {}

  શૂન્ય union_ (અચલ hb_bounds_t &o)
  {
    જો (o.status == UNBOUNDED)
      status = UNBOUNDED;
    નહીંતર જો (o.status == BOUNDED)
    {
      જો (status == EMPTY)
	*આ = o;
      નહીંતર જો (status == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  શૂન્ય intersect (અચલ hb_bounds_t &o)
  {
    જો (o.status == EMPTY)
      status = EMPTY;
    નહીંતર જો (o.status == BOUNDED)
    {
      જો (status == UNBOUNDED)
	*આ = o;
      નહીંતર જો (status == BOUNDED)
      {
        extents.intersect (o.extents);
	જો (extents.is_empty ())
	  status = EMPTY;
      }
    }
  }

  status_t status;
  hb_extents_t<Float> extents;
};

template <typename Float = દશાંશ>
સંરચના hb_transform_decomposed_t
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

  operator તાર્કિક () અચલ
  {
    પાછા translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> to_transform () અચલ
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    પાછા t;
  }
};


#endif /* HB_GEOMETRY_HH */
