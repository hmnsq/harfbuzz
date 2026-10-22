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
#अगर_अघोषित HB_GEOMETRY_HH
#घोषणा HB_GEOMETRY_HH

#समावेश "hb.hh"

#समावेश "hb-algs.hh"

#समावेश <cmath>


ढाँचा <typename Float = भग्न>
काष्ठा hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (स्थिर hb_glyph_extents_t &extents)
  {
    द्विगुन x0 = (द्विगुन) extents.x_bearing;
    द्विगुन y0 = (द्विगुन) extents.y_bearing;
    द्विगुन x1 = x0 + (द्विगुन) extents.width;
    द्विगुन y1 = y0 + (द्विगुन) extents.height;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  bool is_empty () स्थिर { वापस xmin >= xmax || ymin >= ymax; }
  bool is_void () स्थिर { वापस xmin > xmax; }

  व्योम union_ (स्थिर hb_extents_t &o)
  {
    अगर (o.is_empty ()) वापस;
    अगर (is_empty ())
    {
      *यह = o;
      वापस;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  व्योम intersect (स्थिर hb_extents_t &o)
  {
    अगर (o.is_empty () || is_empty ())
    {
      *यह = hb_extents_t {};
      वापस;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  व्योम
  add_point (Float x, Float y)
  {
    अगर (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    अन्यथा
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t to_glyph_extents (bool xneg = false, bool yneg = false) स्थिर
  {
    द्विगुन x0 = round ((द्विगुन) xmin);
    द्विगुन y0 = round ((द्विगुन) ymin);
    द्विगुन x1 = round ((द्विगुन) xmax);
    द्विगुन y1 = round ((द्विगुन) ymax);

    अगर (unlikely (!std::है_सीमित (x0) ||
		  !std::है_सीमित (y0) ||
		  !std::है_सीमित (x1) ||
		  !std::है_सीमित (y1)))
      वापस hb_glyph_extents_t {0, 0, 0, 0};

    वापस hb_glyph_extents_t {
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

  अटल hb_position_t
  clamp_to_hb_position (द्विगुन v)
  {
    वापस (hb_position_t) hb_clamp (v,
				     (द्विगुन) hb_int_min (hb_position_t),
				     (द्विगुन) hb_int_max (hb_position_t));
  }
};

ढाँचा <typename Float = भग्न>
काष्ठा hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  bool is_identity () स्थिर
  {
    वापस xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  bool is_translation () स्थिर
  {
    वापस xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  व्योम multiply (स्थिर hb_transform_t &o, bool before=false)
  {
    // Copied from cairo-matrix.c
    स्थिर hb_transform_t &a = before ? o : *यह;
    स्थिर hb_transform_t &b = before ? *यह : o;
    *यह = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  व्योम transform_distance (Float &dx, Float &dy) स्थिर
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  व्योम transform_point (Float &x, Float &y) स्थिर
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  व्योम transform_extents (hb_extents_t<Float> &extents) स्थिर
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
    क्रम (अचिन्हित i = 0; i < 4; i++)
    {
      transform_point (quad_x[i], quad_y[i]);
      extents.add_point (quad_x[i], quad_y[i]);
    }
  }

  व्योम transform (स्थिर hb_transform_t &o, bool before=false) { multiply (o, before); }

  अटल hb_transform_t translation (Float x, Float y)
  {
    वापस {1, 0, 0, 1, x, y};
  }
  व्योम translate (Float x, Float y, bool before=false)
  {
    अगर (before)
    {
      x0 += x;
      y0 += y;
    }
    अन्यथा
    {
      अगर (x == 0 && y == 0)
	वापस;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  अटल hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    वापस {scaleX, 0, 0, scaleY, 0, 0};
  }
  व्योम scale (Float scaleX, Float scaleY)
  {
    अगर (scaleX == 1 && scaleY == 1)
      वापस;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  अटल hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    वापस {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  व्योम scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    अगर (scaleX == 1 && scaleY == 1)
      वापस;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  अटल hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    वापस {c, s, -s, c, 0, 0};
  }
  व्योम rotate (Float radians, bool before=false)
  {
    अगर (radians == 0)
      वापस;

    transform (rotation (radians), before);
  }

  अटल hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    वापस {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  व्योम rotate_around_center (Float radians, Float center_x, Float center_y, bool before=false)
  {
    अगर (radians == 0)
      वापस;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  अटल hb_transform_t skewing (Float skewX, Float skewY)
  {
    वापस {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  व्योम skew (Float skewX, Float skewY)
  {
    अगर (skewX == 0 && skewY == 0)
      वापस;

    transform (skewing (skewX, skewY));
  }
  अटल hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    वापस {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  व्योम skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    अगर (skewX == 0 && skewY == 0)
	    वापस;

    transform (skewing_around_center (skewX, skewY, center_x, center_y));
  }

  Float xx = 1;
  Float yx = 0;
  Float xy = 0;
  Float yy = 1;
  Float x0 = 0;
  Float y0 = 0;
};

#घोषणा HB_TRANSFORM_IDENTITY {1, 0, 0, 1, 0, 0}

ढाँचा <typename Float = भग्न>
काष्ठा hb_bounds_t
{
  क्रमागत status_t {
    UNBOUNDED,
    BOUNDED,
    EMPTY,
  };

  hb_bounds_t (status_t status = UNBOUNDED) : status (status) {}
  hb_bounds_t (स्थिर hb_extents_t<Float> &extents) :
    status (extents.is_empty () ? EMPTY : BOUNDED), extents (extents) {}

  व्योम union_ (स्थिर hb_bounds_t &o)
  {
    अगर (o.status == UNBOUNDED)
      status = UNBOUNDED;
    अन्यथा अगर (o.status == BOUNDED)
    {
      अगर (status == EMPTY)
	*यह = o;
      अन्यथा अगर (status == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  व्योम intersect (स्थिर hb_bounds_t &o)
  {
    अगर (o.status == EMPTY)
      status = EMPTY;
    अन्यथा अगर (o.status == BOUNDED)
    {
      अगर (status == UNBOUNDED)
	*यह = o;
      अन्यथा अगर (status == BOUNDED)
      {
        extents.intersect (o.extents);
	अगर (extents.is_empty ())
	  status = EMPTY;
      }
    }
  }

  status_t status;
  hb_extents_t<Float> extents;
};

ढाँचा <typename Float = भग्न>
काष्ठा hb_transform_decomposed_t
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

  चालक bool () स्थिर
  {
    वापस translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> to_transform () स्थिर
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    वापस t;
  }
};


#पूर्ण_अगर /* HB_GEOMETRY_HH */
