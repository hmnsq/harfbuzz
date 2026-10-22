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
#నిర్వచితం_కాకపోతే HB_GEOMETRY_HH
#నిర్వచించు HB_GEOMETRY_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-algs.hh"

#అన్తర్భావ <cmath>


మూస <రకం_పేరు Float = దశాంశం>
నిర్మాణం hb_extents_t
{
  hb_extents_t () {}
  hb_extents_t (స్థిరరాశి hb_glyph_extents_t &extents)
  {
    ద్విగుణ x0 = (ద్విగుణ) extents.x_bearing;
    ద్విగుణ y0 = (ద్విగుణ) extents.y_bearing;
    ద్విగుణ x1 = x0 + (ద్విగుణ) extents.వెడల్పు;
    ద్విగుణ y1 = y0 + (ద్విగుణ) extents.ఎత్తు;

    xmin = (Float) hb_min (x0, x1);
    ymin = (Float) hb_min (y0, y1);
    xmax = (Float) hb_max (x0, x1);
    ymax = (Float) hb_max (y0, y1);
  }
  hb_extents_t (Float xmin, Float ymin, Float xmax, Float ymax) :
    xmin (xmin), ymin (ymin), xmax (xmax), ymax (ymax) {}

  తార్కిక is_empty () స్థిరరాశి { తిరిగి xmin >= xmax || ymin >= ymax; }
  తార్కిక is_void () స్థిరరాశి { తిరిగి xmin > xmax; }

  శూన్య union_ (స్థిరరాశి hb_extents_t &o)
  {
    ఒకవేళ (o.is_empty ()) తిరిగి;
    ఒకవేళ (is_empty ())
    {
      *ఇది = o;
      తిరిగి;
    }
    xmin = hb_min (xmin, o.xmin);
    ymin = hb_min (ymin, o.ymin);
    xmax = hb_max (xmax, o.xmax);
    ymax = hb_max (ymax, o.ymax);
  }

  శూన్య intersect (స్థిరరాశి hb_extents_t &o)
  {
    ఒకవేళ (o.is_empty () || is_empty ())
    {
      *ఇది = hb_extents_t {};
      తిరిగి;
    }
    xmin = hb_max (xmin, o.xmin);
    ymin = hb_max (ymin, o.ymin);
    xmax = hb_min (xmax, o.xmax);
    ymax = hb_min (ymax, o.ymax);
  }

  శూన్య
  కూడు_బిందువు (Float x, Float y)
  {
    ఒకవేళ (unlikely (is_void ()))
    {
      xmin = xmax = x;
      ymin = ymax = y;
    }
    లేకపోతే
    {
      xmin = hb_min (xmin, x);
      ymin = hb_min (ymin, y);
      xmax = hb_max (xmax, x);
      ymax = hb_max (ymax, y);
    }
  }

  hb_glyph_extents_t గమ్యం_glyph_extents (తార్కిక xneg = అబద్ధం, తార్కిక yneg = అబద్ధం) స్థిరరాశి
  {
    ద్విగుణ x0 = round ((ద్విగుణ) xmin);
    ద్విగుణ y0 = round ((ద్విగుణ) ymin);
    ద్విగుణ x1 = round ((ద్విగుణ) xmax);
    ద్విగుణ y1 = round ((ద్విగుణ) ymax);

    ఒకవేళ (unlikely (!std::isfinite (x0) ||
		  !std::isfinite (y0) ||
		  !std::isfinite (x1) ||
		  !std::isfinite (y1)))
      తిరిగి hb_glyph_extents_t {0, 0, 0, 0};

    తిరిగి hb_glyph_extents_t {
      clamp_గమ్యం_hb_position (xneg ? x1 : x0),
      clamp_గమ్యం_hb_position (yneg ? y0 : y1),
      clamp_గమ్యం_hb_position (xneg ? x0 - x1 : x1 - x0),
      clamp_గమ్యం_hb_position (yneg ? y1 - y0 : y0 - y1)
    };
  }

  Float xmin = 0;
  Float ymin = 0;
  Float xmax = -1;
  Float ymax = -1;

  స్థిర hb_position_t
  clamp_గమ్యం_hb_position (ద్విగుణ v)
  {
    తిరిగి (hb_position_t) hb_clamp (v,
				     (ద్విగుణ) hb_int_min (hb_position_t),
				     (ద్విగుణ) hb_int_max (hb_position_t));
  }
};

మూస <రకం_పేరు Float = దశాంశం>
నిర్మాణం hb_transform_t
{
  hb_transform_t () {}
  hb_transform_t (Float xx, Float yx,
		  Float xy, Float yy,
		  Float x0, Float y0) :
    xx (xx), yx (yx), xy (xy), yy (yy), x0 (x0), y0 (y0) {}

  తార్కిక is_తటస్థం () స్థిరరాశి
  {
    తిరిగి xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1 &&
	   x0 == 0 && y0 == 0;
  }
  తార్కిక is_translation () స్థిరరాశి
  {
    తిరిగి xx == 1 && yx == 0 &&
	   xy == 0 && yy == 1;
  }

  శూన్య multiply (స్థిరరాశి hb_transform_t &o, తార్కిక before=అబద్ధం)
  {
    // Copied from cairo-matrix.c
    స్థిరరాశి hb_transform_t &a = before ? o : *ఇది;
    స్థిరరాశి hb_transform_t &b = before ? *ఇది : o;
    *ఇది = {
      a.xx * b.xx + a.xy * b.yx,
      a.yx * b.xx + a.yy * b.yx,
      a.xx * b.xy + a.xy * b.yy,
      a.yx * b.xy + a.yy * b.yy,
      a.xx * b.x0 + a.xy * b.y0 + a.x0,
      a.yx * b.x0 + a.yy * b.y0 + a.y0
    };
  }

  HB_ALWAYS_INLINE
  శూన్య transform_దూరం (Float &dx, Float &dy) స్థిరరాశి
  {
    Float new_x = xx * dx + xy * dy;
    Float new_y = yx * dx + yy * dy;
    dx = new_x;
    dy = new_y;
  }

  HB_ALWAYS_INLINE
  శూన్య transform_బిందువు (Float &x, Float &y) స్థిరరాశి
  {
    Float new_x = x0 + xx * x + xy * y;
    Float new_y = y0 + yx * x + yy * y;
    x = new_x;
    y = new_y;
  }

  శూన్య transform_extents (hb_extents_t<Float> &extents) స్థిరరాశి
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
    కొరకు (గుర్తులేని i = 0; i < 4; i++)
    {
      transform_బిందువు (quad_x[i], quad_y[i]);
      extents.కూడు_బిందువు (quad_x[i], quad_y[i]);
    }
  }

  శూన్య transform (స్థిరరాశి hb_transform_t &o, తార్కిక before=అబద్ధం) { multiply (o, before); }

  స్థిర hb_transform_t translation (Float x, Float y)
  {
    తిరిగి {1, 0, 0, 1, x, y};
  }
  శూన్య translate (Float x, Float y, తార్కిక before=అబద్ధం)
  {
    ఒకవేళ (before)
    {
      x0 += x;
      y0 += y;
    }
    లేకపోతే
    {
      ఒకవేళ (x == 0 && y == 0)
	తిరిగి;

      x0 += xx * x + xy * y;
      y0 += yx * x + yy * y;
    }
  }

  స్థిర hb_transform_t scaling (Float scaleX, Float scaleY)
  {
    తిరిగి {scaleX, 0, 0, scaleY, 0, 0};
  }
  శూన్య scale (Float scaleX, Float scaleY)
  {
    ఒకవేళ (scaleX == 1 && scaleY == 1)
      తిరిగి;

    xx *= scaleX;
    yx *= scaleX;
    xy *= scaleY;
    yy *= scaleY;
  }
  స్థిర hb_transform_t scaling_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    తిరిగి {scaleX, 0, 0, scaleY,
	    center_x ? (1 - scaleX) * center_x : 0,
	    center_y ? (1 - scaleY) * center_y : 0};
  }
  శూన్య scale_around_center (Float scaleX, Float scaleY, Float center_x, Float center_y)
  {
    ఒకవేళ (scaleX == 1 && scaleY == 1)
      తిరిగి;

    transform (scaling_around_center (scaleX, scaleY, center_x, center_y));
  }

  స్థిర hb_transform_t rotation (Float radians)
  {
    // https://github.com/fonttools/fonttools/blob/f66ee05f71c8b57b5f519ee975e95edcd1466e14/Lib/fontTools/misc/transform.py#L240
    Float c;
    Float s;
    hb_sincos (radians, s, c);
    తిరిగి {c, s, -s, c, 0, 0};
  }
  శూన్య rotate (Float radians, తార్కిక before=అబద్ధం)
  {
    ఒకవేళ (radians == 0)
      తిరిగి;

    transform (rotation (radians), before);
  }

  స్థిర hb_transform_t rotation_around_center (Float radians, Float center_x, Float center_y)
  {
    Float s, c;
    hb_sincos (radians, s, c);
    తిరిగి {
      c, s, -s, c,
      (1 - c) * center_x + s * center_y,
      -s * center_x +  (1 - c) * center_y
    };
  }
  శూన్య rotate_around_center (Float radians, Float center_x, Float center_y, తార్కిక before=అబద్ధం)
  {
    ఒకవేళ (radians == 0)
      తిరిగి;

    transform (rotation_around_center (radians, center_x, center_y), before);
  }

  స్థిర hb_transform_t skewing (Float skewX, Float skewY)
  {
    తిరిగి {1, skewY ? tanf (skewY) : 0, skewX ? tanf (skewX) : 0, 1, 0, 0};
  }
  శూన్య skew (Float skewX, Float skewY)
  {
    ఒకవేళ (skewX == 0 && skewY == 0)
      తిరిగి;

    transform (skewing (skewX, skewY));
  }
  స్థిర hb_transform_t skewing_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    skewX = skewX ? tanf (skewX) : 0;
    skewY = skewY ? tanf (skewY) : 0;
    తిరిగి {
	    1, skewY, skewX, 1,
	    center_y ? -skewX * center_y : 0,
	    center_x ? -skewY * center_x : 0
    };
  }
  శూన్య skew_around_center (Float skewX, Float skewY, Float center_x, Float center_y)
  {
    ఒకవేళ (skewX == 0 && skewY == 0)
	    తిరిగి;

    transform (skewing_around_center (skewX, skewY, center_x, center_y));
  }

  Float xx = 1;
  Float yx = 0;
  Float xy = 0;
  Float yy = 1;
  Float x0 = 0;
  Float y0 = 0;
};

#నిర్వచించు HB_TRANSFORM_తటస్థం {1, 0, 0, 1, 0, 0}

మూస <రకం_పేరు Float = దశాంశం>
నిర్మాణం hb_bounds_t
{
  గణన స్థితి_t {
    UNBOUNDED,
    BOUNDED,
    ఖాళీలు,
  };

  hb_bounds_t (స్థితి_t స్థితి_2 = UNBOUNDED) : స్థితి_2 (స్థితి_2) {}
  hb_bounds_t (స్థిరరాశి hb_extents_t<Float> &extents) :
    స్థితి_2 (extents.is_empty () ? ఖాళీలు : BOUNDED), extents (extents) {}

  శూన్య union_ (స్థిరరాశి hb_bounds_t &o)
  {
    ఒకవేళ (o.స్థితి_2 == UNBOUNDED)
      స్థితి_2 = UNBOUNDED;
    లేకపోతే ఒకవేళ (o.స్థితి_2 == BOUNDED)
    {
      ఒకవేళ (స్థితి_2 == ఖాళీలు)
	*ఇది = o;
      లేకపోతే ఒకవేళ (స్థితి_2 == BOUNDED)
        extents.union_ (o.extents);
    }
  }

  శూన్య intersect (స్థిరరాశి hb_bounds_t &o)
  {
    ఒకవేళ (o.స్థితి_2 == ఖాళీలు)
      స్థితి_2 = ఖాళీలు;
    లేకపోతే ఒకవేళ (o.స్థితి_2 == BOUNDED)
    {
      ఒకవేళ (స్థితి_2 == UNBOUNDED)
	*ఇది = o;
      లేకపోతే ఒకవేళ (స్థితి_2 == BOUNDED)
      {
        extents.intersect (o.extents);
	ఒకవేళ (extents.is_empty ())
	  స్థితి_2 = ఖాళీలు;
      }
    }
  }

  స్థితి_t స్థితి_2;
  hb_extents_t<Float> extents;
};

మూస <రకం_పేరు Float = దశాంశం>
నిర్మాణం hb_transform_decomposed_t
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

  క్రియాకారకం తార్కిక () స్థిరరాశి
  {
    తిరిగి translateX || translateY ||
	   rotation ||
	   scaleX != 1 || scaleY != 1 ||
	   skewX || skewY ||
	   tCenterX || tCenterY;
  }

  hb_transform_t<Float> గమ్యం_transform () స్థిరరాశి
  {
    hb_transform_t<Float> t;
    t.translate (translateX + tCenterX, translateY + tCenterY);
    t.rotate (rotation);
    t.scale (scaleX, scaleY);
    t.skew (-skewX, skewY);
    t.translate (-tCenterX, -tCenterY);
    తిరిగి t;
  }
};


#అయితే_ముగింపు /* HB_GEOMETRY_HH */
