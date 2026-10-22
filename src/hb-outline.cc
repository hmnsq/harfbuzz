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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OUTLINE

#ضمن "hb-outline.hh"

#ضمن "hb-machinery.hh"


فراغ hb_outline_t::replay (hb_draw_funcs_t *pen, فراغ *pen_معلومات) ثابت
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  بنانشان پهريون = 0;
  لكل (بنانشان contour : contours)
  {
    خودڪار it = points.as_array ().sub_array (پهريون, contour - پهريون);
    طالما (it)
    {
      hb_outline_نقطو_t p1 = *it++;
      بدل (p1.type)
      {
	حالة hb_outline_نقطو_t::type_t::MOVE_منزل:
	{
	  pen->move_منزل (pen_معلومات, st,
			   p1.x, p1.y);
	}
	اكسر;
	حالة hb_outline_نقطو_t::type_t::سٽ_منزل_2:
	{
	  pen->سٽ_منزل (pen_معلومات, st,
			   p1.x, p1.y);
	}
	اكسر;
	حالة hb_outline_نقطو_t::type_t::QUADRATIC_منزل:
	{
	  hb_outline_نقطو_t p2 = *it++;
	  pen->quadratic_منزل (pen_معلومات, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	اكسر;
	حالة hb_outline_نقطو_t::type_t::CUBIC_منزل:
	{
	  hb_outline_نقطو_t p2 = *it++;
	  hb_outline_نقطو_t p3 = *it++;
	  pen->cubic_منزل (pen_معلومات, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	اكسر;
      }
    }
    pen->close_رستو (pen_معلومات, st);
    پهريون = contour;
  }
}

فراغ hb_outline_t::translate (عائم dx, عائم dy)
{
  لكل (خودڪار &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

فراغ hb_outline_t::slant (عائم slant_xy)
{
  لكل (خودڪار &p : points)
    p.x += slant_xy * p.y;
}

عائم hb_outline_t::control_area () ثابت
{
  عائم a = 0;
  بنانشان پهريون = 0;
  لكل (بنانشان contour : contours)
  {
    لكل (بنانشان i = پهريون; i < contour; i++)
    {
      بنانشان j = i + 1 < contour ? i + 1 : پهريون;

      خودڪار &pi = points[i];
      خودڪار &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    پهريون = contour;
  }
  أرجع a * .5f;
}

فراغ hb_outline_t::embolden (عائم x_strength, عائم y_strength,
			     عائم x_سرڪايو, عائم y_سرڪايو)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  إذا (!x_strength && !y_strength) أرجع;
  إذا (!points) أرجع;

  x_strength /= 2.f;
  y_strength /= 2.f;

  منطقي orientation_negative = control_area () < 0;

  نشانوارو پهريون = 0;
  لكل (بنانشان c = 0; c < contours.ڊيگهائي; c++)
  {
    hb_outline_ويڪٽر_t اندريون, ٻاهريون, anchor, سرڪايو;
    عائم l_اندريون, l_ٻاهريون, l_anchor = 0, l, q, d;

    l_اندريون = 0;
    نشانوارو آخري = (صحيح) contours[c] - 1;

    /* pacify compiler */
    اندريون.x = اندريون.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    لكل ( نشانوارو i = آخري, j = پهريون, k = -1;
	  j != i && i != k;
	  j = j < آخري ? j + 1 : پهريون )
    {
      إذا ( j != k )
      {
	ٻاهريون.x = points[j].x - points[i].x;
	ٻاهريون.y = points[j].y - points[i].y;
	l_ٻاهريون = ٻاهريون.normalize_len ();

	إذا ( l_ٻاهريون == 0 )
	  استمر;
      }
      وإلا
      {
	ٻاهريون   = anchor;
	l_ٻاهريون = l_anchor;
      }

      إذا ( l_اندريون != 0 )
      {
	إذا ( k < 0 )
	{
	  k        = i;
	  anchor   = اندريون;
	  l_anchor = l_اندريون;
	}

	d = اندريون.x * ٻاهريون.x + اندريون.y * ٻاهريون.y;

	/* shift only if turn is less than ~160 degrees */
	إذا ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  سرڪايو.x = اندريون.y + ٻاهريون.y;
	  سرڪايو.y = اندريون.x + ٻاهريون.x;

	  إذا ( orientation_negative )
	    سرڪايو.x = -سرڪايو.x;
	  وإلا
	    سرڪايو.y = -سرڪايو.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = ٻاهريون.x * اندريون.y - ٻاهريون.y * اندريون.x;
	  إذا ( orientation_negative )
	    q = -q;

	  l = hb_min (l_اندريون, l_ٻاهريون);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  إذا (x_strength * q <= l * d)
	    سرڪايو.x = سرڪايو.x * x_strength / d;
	  وإلا
	    سرڪايو.x = سرڪايو.x * l / q;


	  إذا (y_strength * q <= l * d)
	    سرڪايو.y = سرڪايو.y * y_strength / d;
	  وإلا
	    سرڪايو.y = سرڪايو.y * l / q;
	}
	وإلا
	  سرڪايو.x = سرڪايو.y = 0;

	لكل ( ;
	      i != j;
	      i = i < آخري ? i + 1 : پهريون )
	{
	  points[i].x += x_سرڪايو + سرڪايو.x;
	  points[i].y += y_سرڪايو + سرڪايو.y;
	}
      }
      وإلا
	i = j;

      اندريون   = ٻاهريون;
      l_اندريون = l_ٻاهريون;
    }

    پهريون = آخري + 1;
  }
}

ساكن فراغ
hb_outline_recording_pen_move_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
				  فراغ *معلومات,
				  hb_draw_state_t *st,
				  عائم منزل_x, عائم منزل_y,
				  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  c->points.push (hb_outline_نقطو_t {منزل_x, منزل_y, hb_outline_نقطو_t::type_t::MOVE_منزل});
}

ساكن فراغ
hb_outline_recording_pen_سٽ_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
				  فراغ *معلومات,
				  hb_draw_state_t *st,
				  عائم منزل_x, عائم منزل_y,
				  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  c->points.push (hb_outline_نقطو_t {منزل_x, منزل_y, hb_outline_نقطو_t::type_t::سٽ_منزل_2});
}

ساكن فراغ
hb_outline_recording_pen_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
				       فراغ *معلومات,
				       hb_draw_state_t *st,
				       عائم control_x, عائم control_y,
				       عائم منزل_x, عائم منزل_y,
				       فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  c->points.push (hb_outline_نقطو_t {control_x, control_y, hb_outline_نقطو_t::type_t::QUADRATIC_منزل});
  c->points.push (hb_outline_نقطو_t {منزل_x, منزل_y, hb_outline_نقطو_t::type_t::QUADRATIC_منزل});
}

ساكن فراغ
hb_outline_recording_pen_cubic_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
				   فراغ *معلومات,
				   hb_draw_state_t *st,
				   عائم control1_x, عائم control1_y,
				   عائم control2_x, عائم control2_y,
				   عائم منزل_x, عائم منزل_y,
				   فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  c->points.push (hb_outline_نقطو_t {control1_x, control1_y, hb_outline_نقطو_t::type_t::CUBIC_منزل});
  c->points.push (hb_outline_نقطو_t {control2_x, control2_y, hb_outline_نقطو_t::type_t::CUBIC_منزل});
  c->points.push (hb_outline_نقطو_t {منزل_x, منزل_y, hb_outline_نقطو_t::type_t::CUBIC_منزل});
}

ساكن فراغ
hb_outline_recording_pen_close_رستو (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
				     فراغ *معلومات,
				     hb_draw_state_t *st,
				     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  c->contours.push (c->points.ڊيگهائي);
}

ساكن int64_t *
hb_outline_recording_pen_get_budget_بچيل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
					       فراغ *معلومات,
					       فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_outline_t *c = (hb_outline_t *) معلومات;

  أرجع c->budget_بچيل;
}

ساكن مضمن فراغ free_static_outline_recording_pen_funcs ();

ساكن بنية hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سيٽ_move_منزل_func (funcs, hb_outline_recording_pen_move_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سٽ_منزل_func (funcs, hb_outline_recording_pen_سٽ_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_quadratic_منزل_func (funcs, hb_outline_recording_pen_quadratic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_cubic_منزل_func (funcs, hb_outline_recording_pen_cubic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_close_رستو_func (funcs, hb_outline_recording_pen_close_رستو, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_outline_recording_pen_get_budget_بچيل, خالي_اشارو, خالي_اشارو);

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


#جيڪڏهن_پورو
