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

#جے_بنا_تعریف HB_NO_OUTLINE

#ضمن "hb-outline.hh"

#ضمن "hb-machinery.hh"


فراغ hb_outline_t::replay (hb_draw_funcs_t *pen, فراغ *pen_انکڑے) ثابت
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  بنانشانی پہلا = 0;
  لكل (بنانشانی contour : contours)
  {
    خودکار it = points.as_array ().sub_array (پہلا, contour - پہلا);
    طالما (it)
    {
      hb_outline_نقطہ_t p1 = *it++;
      بدل (p1.type)
      {
	حالة hb_outline_نقطہ_t::type_t::MOVE_منزل:
	{
	  pen->move_منزل (pen_انکڑے, st,
			   p1.x, p1.y);
	}
	اكسر;
	حالة hb_outline_نقطہ_t::type_t::سطر_منزل_2:
	{
	  pen->سطر_منزل (pen_انکڑے, st,
			   p1.x, p1.y);
	}
	اكسر;
	حالة hb_outline_نقطہ_t::type_t::QUADRATIC_منزل:
	{
	  hb_outline_نقطہ_t p2 = *it++;
	  pen->quadratic_منزل (pen_انکڑے, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	اكسر;
	حالة hb_outline_نقطہ_t::type_t::CUBIC_منزل:
	{
	  hb_outline_نقطہ_t p2 = *it++;
	  hb_outline_نقطہ_t p3 = *it++;
	  pen->cubic_منزل (pen_انکڑے, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	اكسر;
      }
    }
    pen->close_رستہ (pen_انکڑے, st);
    پہلا = contour;
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
  بنانشانی پہلا = 0;
  لكل (بنانشانی contour : contours)
  {
    لكل (بنانشانی i = پہلا; i < contour; i++)
    {
      بنانشانی j = i + 1 < contour ? i + 1 : پہلا;

      خودکار &pi = points[i];
      خودکار &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    پہلا = contour;
  }
  أرجع a * .5f;
}

فراغ hb_outline_t::embolden (عائم x_strength, عائم y_strength,
			     عائم x_کھسکاؤ, عائم y_کھسکاؤ)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  إذا (!x_strength && !y_strength) أرجع;
  إذا (!points) أرجع;

  x_strength /= 2.f;
  y_strength /= 2.f;

  منطقي orientation_negative = control_area () < 0;

  نشانیوالا پہلا = 0;
  لكل (بنانشانی c = 0; c < contours.لمائی; c++)
  {
    hb_outline_سمتیہ_t وچ, باہرلا, anchor, کھسکاؤ;
    عائم l_وچ, l_باہرلا, l_anchor = 0, l, q, d;

    l_وچ = 0;
    نشانیوالا آخری = (صحيح) contours[c] - 1;

    /* pacify compiler */
    وچ.x = وچ.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    لكل ( نشانیوالا i = آخری, j = پہلا, k = -1;
	  j != i && i != k;
	  j = j < آخری ? j + 1 : پہلا )
    {
      إذا ( j != k )
      {
	باہرلا.x = points[j].x - points[i].x;
	باہرلا.y = points[j].y - points[i].y;
	l_باہرلا = باہرلا.normalize_len ();

	إذا ( l_باہرلا == 0 )
	  استمر;
      }
      وإلا
      {
	باہرلا   = anchor;
	l_باہرلا = l_anchor;
      }

      إذا ( l_وچ != 0 )
      {
	إذا ( k < 0 )
	{
	  k        = i;
	  anchor   = وچ;
	  l_anchor = l_وچ;
	}

	d = وچ.x * باہرلا.x + وچ.y * باہرلا.y;

	/* shift only if turn is less than ~160 degrees */
	إذا ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  کھسکاؤ.x = وچ.y + باہرلا.y;
	  کھسکاؤ.y = وچ.x + باہرلا.x;

	  إذا ( orientation_negative )
	    کھسکاؤ.x = -کھسکاؤ.x;
	  وإلا
	    کھسکاؤ.y = -کھسکاؤ.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = باہرلا.x * وچ.y - باہرلا.y * وچ.x;
	  إذا ( orientation_negative )
	    q = -q;

	  l = hb_min (l_وچ, l_باہرلا);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  إذا (x_strength * q <= l * d)
	    کھسکاؤ.x = کھسکاؤ.x * x_strength / d;
	  وإلا
	    کھسکاؤ.x = کھسکاؤ.x * l / q;


	  إذا (y_strength * q <= l * d)
	    کھسکاؤ.y = کھسکاؤ.y * y_strength / d;
	  وإلا
	    کھسکاؤ.y = کھسکاؤ.y * l / q;
	}
	وإلا
	  کھسکاؤ.x = کھسکاؤ.y = 0;

	لكل ( ;
	      i != j;
	      i = i < آخری ? i + 1 : پہلا )
	{
	  points[i].x += x_کھسکاؤ + کھسکاؤ.x;
	  points[i].y += y_کھسکاؤ + کھسکاؤ.y;
	}
      }
      وإلا
	i = j;

      وچ   = باہرلا;
      l_وچ = l_باہرلا;
    }

    پہلا = آخری + 1;
  }
}

ساكن فراغ
hb_outline_recording_pen_move_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
				  فراغ *انکڑے,
				  hb_draw_state_t *st,
				  عائم منزل_x, عائم منزل_y,
				  فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_outline_t *c = (hb_outline_t *) انکڑے;

  c->points.push (hb_outline_نقطہ_t {منزل_x, منزل_y, hb_outline_نقطہ_t::type_t::MOVE_منزل});
}

ساكن فراغ
hb_outline_recording_pen_سطر_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
				  فراغ *انکڑے,
				  hb_draw_state_t *st,
				  عائم منزل_x, عائم منزل_y,
				  فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_outline_t *c = (hb_outline_t *) انکڑے;

  c->points.push (hb_outline_نقطہ_t {منزل_x, منزل_y, hb_outline_نقطہ_t::type_t::سطر_منزل_2});
}

ساكن فراغ
hb_outline_recording_pen_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
				       فراغ *انکڑے,
				       hb_draw_state_t *st,
				       عائم control_x, عائم control_y,
				       عائم منزل_x, عائم منزل_y,
				       فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_outline_t *c = (hb_outline_t *) انکڑے;

  c->points.push (hb_outline_نقطہ_t {control_x, control_y, hb_outline_نقطہ_t::type_t::QUADRATIC_منزل});
  c->points.push (hb_outline_نقطہ_t {منزل_x, منزل_y, hb_outline_نقطہ_t::type_t::QUADRATIC_منزل});
}

ساكن فراغ
hb_outline_recording_pen_cubic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
				   فراغ *انکڑے,
				   hb_draw_state_t *st,
				   عائم control1_x, عائم control1_y,
				   عائم control2_x, عائم control2_y,
				   عائم منزل_x, عائم منزل_y,
				   فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_outline_t *c = (hb_outline_t *) انکڑے;

  c->points.push (hb_outline_نقطہ_t {control1_x, control1_y, hb_outline_نقطہ_t::type_t::CUBIC_منزل});
  c->points.push (hb_outline_نقطہ_t {control2_x, control2_y, hb_outline_نقطہ_t::type_t::CUBIC_منزل});
  c->points.push (hb_outline_نقطہ_t {منزل_x, منزل_y, hb_outline_نقطہ_t::type_t::CUBIC_منزل});
}

ساكن فراغ
hb_outline_recording_pen_close_رستہ (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
				     فراغ *انکڑے,
				     hb_draw_state_t *st,
				     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_outline_t *c = (hb_outline_t *) انکڑے;

  c->contours.push (c->points.لمائی);
}

ساكن int64_t *
hb_outline_recording_pen_get_budget_بچیا (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
					       فراغ *انکڑے,
					       فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_outline_t *c = (hb_outline_t *) انکڑے;

  أرجع c->budget_بچیا;
}

ساكن مضمن فراغ free_static_outline_recording_pen_funcs ();

ساكن بنية hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_منزل_func (funcs, hb_outline_recording_pen_move_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_سطر_منزل_func (funcs, hb_outline_recording_pen_سطر_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_quadratic_منزل_func (funcs, hb_outline_recording_pen_quadratic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_cubic_منزل_func (funcs, hb_outline_recording_pen_cubic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_close_رستہ_func (funcs, hb_outline_recording_pen_close_رستہ, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_outline_recording_pen_get_budget_بچیا, خالی_سوچک, خالی_سوچک);

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


#جے_انت
