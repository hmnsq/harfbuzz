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

#که_نه_تعریف_شوی HB_NO_OUTLINE

#ضمن "hb-outline.hh"

#ضمن "hb-machinery.hh"


فراغ hb_outline_t::replay (hb_draw_funcs_t *pen, فراغ *pen_معلومات) ثابت
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  بینښه لومړی = 0;
  لكل (بینښه contour : contours)
  {
    خپلکاری it = points.as_array ().sub_array (لومړی, contour - لومړی);
    طالما (it)
    {
      hb_outline_ټکی_t p1 = *it++;
      بدل (p1.type)
      {
	حالة hb_outline_ټکی_t::type_t::MOVE_منزل:
	{
	  pen->move_منزل (pen_معلومات, st,
			   p1.x, p1.y);
	}
	اكسر;
	حالة hb_outline_ټکی_t::type_t::کرښه_منزل_2:
	{
	  pen->کرښه_منزل (pen_معلومات, st,
			   p1.x, p1.y);
	}
	اكسر;
	حالة hb_outline_ټکی_t::type_t::QUADRATIC_منزل:
	{
	  hb_outline_ټکی_t p2 = *it++;
	  pen->quadratic_منزل (pen_معلومات, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	اكسر;
	حالة hb_outline_ټکی_t::type_t::CUBIC_منزل:
	{
	  hb_outline_ټکی_t p2 = *it++;
	  hb_outline_ټکی_t p3 = *it++;
	  pen->cubic_منزل (pen_معلومات, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	اكسر;
      }
    }
    pen->close_لار (pen_معلومات, st);
    لومړی = contour;
  }
}

فراغ hb_outline_t::translate (عائم dx, عائم dy)
{
  لكل (خپلکاری &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

فراغ hb_outline_t::slant (عائم slant_xy)
{
  لكل (خپلکاری &p : points)
    p.x += slant_xy * p.y;
}

عائم hb_outline_t::control_area () ثابت
{
  عائم a = 0;
  بینښه لومړی = 0;
  لكل (بینښه contour : contours)
  {
    لكل (بینښه i = لومړی; i < contour; i++)
    {
      بینښه j = i + 1 < contour ? i + 1 : لومړی;

      خپلکاری &pi = points[i];
      خپلکاری &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    لومړی = contour;
  }
  أرجع a * .5f;
}

فراغ hb_outline_t::embolden (عائم x_strength, عائم y_strength,
			     عائم x_ښويدنه, عائم y_ښويدنه)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  إذا (!x_strength && !y_strength) أرجع;
  إذا (!points) أرجع;

  x_strength /= 2.f;
  y_strength /= 2.f;

  منطقي orientation_negative = control_area () < 0;

  نښهدار لومړی = 0;
  لكل (بینښه c = 0; c < contours.اوږدتیا; c++)
  {
    hb_outline_ویکټور_t دننه, بهر, anchor, ښويدنه;
    عائم l_دننه, l_بهر, l_anchor = 0, l, q, d;

    l_دننه = 0;
    نښهدار وروستی = (صحيح) contours[c] - 1;

    /* pacify compiler */
    دننه.x = دننه.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    لكل ( نښهدار i = وروستی, j = لومړی, k = -1;
	  j != i && i != k;
	  j = j < وروستی ? j + 1 : لومړی )
    {
      إذا ( j != k )
      {
	بهر.x = points[j].x - points[i].x;
	بهر.y = points[j].y - points[i].y;
	l_بهر = بهر.normalize_len ();

	إذا ( l_بهر == 0 )
	  استمر;
      }
      وإلا
      {
	بهر   = anchor;
	l_بهر = l_anchor;
      }

      إذا ( l_دننه != 0 )
      {
	إذا ( k < 0 )
	{
	  k        = i;
	  anchor   = دننه;
	  l_anchor = l_دننه;
	}

	d = دننه.x * بهر.x + دننه.y * بهر.y;

	/* shift only if turn is less than ~160 degrees */
	إذا ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  ښويدنه.x = دننه.y + بهر.y;
	  ښويدنه.y = دننه.x + بهر.x;

	  إذا ( orientation_negative )
	    ښويدنه.x = -ښويدنه.x;
	  وإلا
	    ښويدنه.y = -ښويدنه.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = بهر.x * دننه.y - بهر.y * دننه.x;
	  إذا ( orientation_negative )
	    q = -q;

	  l = hb_min (l_دننه, l_بهر);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  إذا (x_strength * q <= l * d)
	    ښويدنه.x = ښويدنه.x * x_strength / d;
	  وإلا
	    ښويدنه.x = ښويدنه.x * l / q;


	  إذا (y_strength * q <= l * d)
	    ښويدنه.y = ښويدنه.y * y_strength / d;
	  وإلا
	    ښويدنه.y = ښويدنه.y * l / q;
	}
	وإلا
	  ښويدنه.x = ښويدنه.y = 0;

	لكل ( ;
	      i != j;
	      i = i < وروستی ? i + 1 : لومړی )
	{
	  points[i].x += x_ښويدنه + ښويدنه.x;
	  points[i].y += y_ښويدنه + ښويدنه.y;
	}
      }
      وإلا
	i = j;

      دننه   = بهر;
      l_دننه = l_بهر;
    }

    لومړی = وروستی + 1;
  }
}

ساكن فراغ
hb_outline_recording_pen_move_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
				  فراغ *معلومات,
				  hb_draw_state_t *st,
				  عائم منزل_x, عائم منزل_y,
				  فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  c->points.push (hb_outline_ټکی_t {منزل_x, منزل_y, hb_outline_ټکی_t::type_t::MOVE_منزل});
}

ساكن فراغ
hb_outline_recording_pen_کرښه_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
				  فراغ *معلومات,
				  hb_draw_state_t *st,
				  عائم منزل_x, عائم منزل_y,
				  فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  c->points.push (hb_outline_ټکی_t {منزل_x, منزل_y, hb_outline_ټکی_t::type_t::کرښه_منزل_2});
}

ساكن فراغ
hb_outline_recording_pen_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
				       فراغ *معلومات,
				       hb_draw_state_t *st,
				       عائم control_x, عائم control_y,
				       عائم منزل_x, عائم منزل_y,
				       فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  c->points.push (hb_outline_ټکی_t {control_x, control_y, hb_outline_ټکی_t::type_t::QUADRATIC_منزل});
  c->points.push (hb_outline_ټکی_t {منزل_x, منزل_y, hb_outline_ټکی_t::type_t::QUADRATIC_منزل});
}

ساكن فراغ
hb_outline_recording_pen_cubic_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
				   فراغ *معلومات,
				   hb_draw_state_t *st,
				   عائم control1_x, عائم control1_y,
				   عائم control2_x, عائم control2_y,
				   عائم منزل_x, عائم منزل_y,
				   فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  c->points.push (hb_outline_ټکی_t {control1_x, control1_y, hb_outline_ټکی_t::type_t::CUBIC_منزل});
  c->points.push (hb_outline_ټکی_t {control2_x, control2_y, hb_outline_ټکی_t::type_t::CUBIC_منزل});
  c->points.push (hb_outline_ټکی_t {منزل_x, منزل_y, hb_outline_ټکی_t::type_t::CUBIC_منزل});
}

ساكن فراغ
hb_outline_recording_pen_close_لار (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
				     فراغ *معلومات,
				     hb_draw_state_t *st,
				     فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  c->contours.push (c->points.اوږدتیا);
}

ساكن int64_t *
hb_outline_recording_pen_get_budget_پاتې_شوی (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
					       فراغ *معلومات,
					       فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  أرجع c->budget_پاتې_شوی;
}

ساكن مضمن فراغ free_static_outline_recording_pen_funcs ();

ساكن بنية hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سېټ_move_منزل_func (funcs, hb_outline_recording_pen_move_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_کرښه_منزل_func (funcs, hb_outline_recording_pen_کرښه_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_quadratic_منزل_func (funcs, hb_outline_recording_pen_quadratic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_cubic_منزل_func (funcs, hb_outline_recording_pen_cubic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_close_لار_func (funcs, hb_outline_recording_pen_close_لار, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_get_budget_پاتې_شوی_func (funcs, hb_outline_recording_pen_get_budget_پاتې_شوی, تش_اشاره, تش_اشاره);

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


#د_که_پای
