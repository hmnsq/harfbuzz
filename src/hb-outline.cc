/*
 * Copyright © 2023  Behdad Esfahbod
 * Copyright © 1999  David Turner
 * Copyright © 2005  Werner Lemberg
 * Copyright © 2013-2015  Alexei Podtelezhnikov
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

#ضمن "hb.hh"

#اگر_تعریف_نشده HB_NO_OUTLINE

#ضمن "hb-outline.hh"

#ضمن "hb-machinery.hh"


فراغ hb_outline_t::replay (hb_draw_funcs_t *pen, فراغ *pen_داده) ثابت
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  بیعلامت اولی = 0;
  لكل (بیعلامت contour : contours)
  {
    خودکار it = points.as_array ().sub_array (اولی, contour - اولی);
    طالما (it)
    {
      hb_outline_نقطه_t p1 = *it++;
      بدل (p1.type)
      {
	حالة hb_outline_نقطه_t::type_t::MOVE_مقصد:
	{
	  pen->move_مقصد (pen_داده, st,
			   p1.x, p1.y);
	}
	اكسر;
	حالة hb_outline_نقطه_t::type_t::سطر_مقصد_2:
	{
	  pen->سطر_مقصد (pen_داده, st,
			   p1.x, p1.y);
	}
	اكسر;
	حالة hb_outline_نقطه_t::type_t::QUADRATIC_مقصد:
	{
	  hb_outline_نقطه_t p2 = *it++;
	  pen->quadratic_مقصد (pen_داده, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	اكسر;
	حالة hb_outline_نقطه_t::type_t::CUBIC_مقصد:
	{
	  hb_outline_نقطه_t p2 = *it++;
	  hb_outline_نقطه_t p3 = *it++;
	  pen->cubic_مقصد (pen_داده, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	اكسر;
      }
    }
    pen->close_مسیر (pen_داده, st);
    اولی = contour;
  }
}

فراغ hb_outline_t::translate (عائم dx, عائم dy)
{
  لكل (خودکار &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

فراغ hb_outline_t::slant (عائم slant_xy)
{
  لكل (خودکار &p : points)
    p.x += slant_xy * p.y;
}

عائم hb_outline_t::control_area () ثابت
{
  عائم a = 0;
  بیعلامت اولی = 0;
  لكل (بیعلامت contour : contours)
  {
    لكل (بیعلامت i = اولی; i < contour; i++)
    {
      بیعلامت j = i + 1 < contour ? i + 1 : اولی;

      خودکار &pi = points[i];
      خودکار &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    اولی = contour;
  }
  أرجع a * .5f;
}

فراغ hb_outline_t::embolden (عائم x_strength, عائم y_strength,
			     عائم x_لغزش, عائم y_لغزش)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  إذا (!x_strength && !y_strength) أرجع;
  إذا (!points) أرجع;

  x_strength /= 2.f;
  y_strength /= 2.f;

  منطقي orientation_negative = control_area () < 0;

  علامتدار اولی = 0;
  لكل (بیعلامت c = 0; c < contours.درازی; c++)
  {
    hb_outline_وکتور_t درون, بیرون, anchor, لغزش;
    عائم l_درون, l_بیرون, l_anchor = 0, l, q, d;

    l_درون = 0;
    علامتدار آخرین = (صحيح) contours[c] - 1;

    /* pacify compiler */
    درون.x = درون.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    لكل ( علامتدار i = آخرین, j = اولی, k = -1;
	  j != i && i != k;
	  j = j < آخرین ? j + 1 : اولی )
    {
      إذا ( j != k )
      {
	بیرون.x = points[j].x - points[i].x;
	بیرون.y = points[j].y - points[i].y;
	l_بیرون = بیرون.normalize_len ();

	إذا ( l_بیرون == 0 )
	  استمر;
      }
      وإلا
      {
	بیرون   = anchor;
	l_بیرون = l_anchor;
      }

      إذا ( l_درون != 0 )
      {
	إذا ( k < 0 )
	{
	  k        = i;
	  anchor   = درون;
	  l_anchor = l_درون;
	}

	d = درون.x * بیرون.x + درون.y * بیرون.y;

	/* shift only if turn is less than ~160 degrees */
	إذا ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  لغزش.x = درون.y + بیرون.y;
	  لغزش.y = درون.x + بیرون.x;

	  إذا ( orientation_negative )
	    لغزش.x = -لغزش.x;
	  وإلا
	    لغزش.y = -لغزش.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = بیرون.x * درون.y - بیرون.y * درون.x;
	  إذا ( orientation_negative )
	    q = -q;

	  l = hb_min (l_درون, l_بیرون);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  إذا (x_strength * q <= l * d)
	    لغزش.x = لغزش.x * x_strength / d;
	  وإلا
	    لغزش.x = لغزش.x * l / q;


	  إذا (y_strength * q <= l * d)
	    لغزش.y = لغزش.y * y_strength / d;
	  وإلا
	    لغزش.y = لغزش.y * l / q;
	}
	وإلا
	  لغزش.x = لغزش.y = 0;

	لكل ( ;
	      i != j;
	      i = i < آخرین ? i + 1 : اولی )
	{
	  points[i].x += x_لغزش + لغزش.x;
	  points[i].y += y_لغزش + لغزش.y;
	}
      }
      وإلا
	i = j;

      درون   = بیرون;
      l_درون = l_بیرون;
    }

    اولی = آخرین + 1;
  }
}

ساكن فراغ
hb_outline_recording_pen_move_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
				  فراغ *داده,
				  hb_draw_state_t *st,
				  عائم مقصد_x, عائم مقصد_y,
				  فراغ *user_داده HB_استفادهنشده)
{
  hb_outline_t *c = (hb_outline_t *) داده;

  c->points.push (hb_outline_نقطه_t {مقصد_x, مقصد_y, hb_outline_نقطه_t::type_t::MOVE_مقصد});
}

ساكن فراغ
hb_outline_recording_pen_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
				  فراغ *داده,
				  hb_draw_state_t *st,
				  عائم مقصد_x, عائم مقصد_y,
				  فراغ *user_داده HB_استفادهنشده)
{
  hb_outline_t *c = (hb_outline_t *) داده;

  c->points.push (hb_outline_نقطه_t {مقصد_x, مقصد_y, hb_outline_نقطه_t::type_t::سطر_مقصد_2});
}

ساكن فراغ
hb_outline_recording_pen_quadratic_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
				       فراغ *داده,
				       hb_draw_state_t *st,
				       عائم control_x, عائم control_y,
				       عائم مقصد_x, عائم مقصد_y,
				       فراغ *user_داده HB_استفادهنشده)
{
  hb_outline_t *c = (hb_outline_t *) داده;

  c->points.push (hb_outline_نقطه_t {control_x, control_y, hb_outline_نقطه_t::type_t::QUADRATIC_مقصد});
  c->points.push (hb_outline_نقطه_t {مقصد_x, مقصد_y, hb_outline_نقطه_t::type_t::QUADRATIC_مقصد});
}

ساكن فراغ
hb_outline_recording_pen_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
				   فراغ *داده,
				   hb_draw_state_t *st,
				   عائم control1_x, عائم control1_y,
				   عائم control2_x, عائم control2_y,
				   عائم مقصد_x, عائم مقصد_y,
				   فراغ *user_داده HB_استفادهنشده)
{
  hb_outline_t *c = (hb_outline_t *) داده;

  c->points.push (hb_outline_نقطه_t {control1_x, control1_y, hb_outline_نقطه_t::type_t::CUBIC_مقصد});
  c->points.push (hb_outline_نقطه_t {control2_x, control2_y, hb_outline_نقطه_t::type_t::CUBIC_مقصد});
  c->points.push (hb_outline_نقطه_t {مقصد_x, مقصد_y, hb_outline_نقطه_t::type_t::CUBIC_مقصد});
}

ساكن فراغ
hb_outline_recording_pen_close_مسیر (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
				     فراغ *داده,
				     hb_draw_state_t *st,
				     فراغ *user_داده HB_استفادهنشده)
{
  hb_outline_t *c = (hb_outline_t *) داده;

  c->contours.push (c->points.درازی);
}

ساكن int64_t *
hb_outline_recording_pen_get_budget_مانده (hb_draw_funcs_t *dfuncs HB_استفادهنشده,
					       فراغ *داده,
					       فراغ *user_داده HB_استفادهنشده)
{
  hb_outline_t *c = (hb_outline_t *) داده;

  أرجع c->budget_مانده;
}

ساكن مضمن فراغ free_static_outline_recording_pen_funcs ();

ساكن بنية hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_مجموعه_move_مقصد_func (funcs, hb_outline_recording_pen_move_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_سطر_مقصد_func (funcs, hb_outline_recording_pen_سطر_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_quadratic_مقصد_func (funcs, hb_outline_recording_pen_quadratic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_cubic_مقصد_func (funcs, hb_outline_recording_pen_cubic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_close_مسیر_func (funcs, hb_outline_recording_pen_close_مسیر, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_get_budget_مانده_func (funcs, hb_outline_recording_pen_get_budget_مانده, اشاره_تهی, اشاره_تهی);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    أرجع funcs;
  }
} static_outline_recording_pen_funcs;

ساكن مضمن
فراغ free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  أرجع static_outline_recording_pen_funcs.get_unconst ();
}


#پایان_اگر
