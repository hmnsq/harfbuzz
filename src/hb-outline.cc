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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OUTLINE

#شامل "hb-outline.hh"

#شامل "hb-machinery.hh"


خالی hb_outline_t::replay (hb_draw_funcs_t *pen, خالی *pen_ڈیٹا) ثابت
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  غیرعلامتی پہلا = 0;
  برائے (غیرعلامتی contour : contours)
  {
    خودکار it = points.as_array ().sub_array (پہلا, contour - پہلا);
    جب (it)
    {
      hb_outline_نقطہ_t p1 = *it++;
      انتخاب (p1.type)
      {
	حالت hb_outline_نقطہ_t::type_t::MOVE_مقصد:
	{
	  pen->move_مقصد (pen_ڈیٹا, st,
			   p1.x, p1.y);
	}
	توڑو;
	حالت hb_outline_نقطہ_t::type_t::سطر_مقصد_2:
	{
	  pen->سطر_مقصد (pen_ڈیٹا, st,
			   p1.x, p1.y);
	}
	توڑو;
	حالت hb_outline_نقطہ_t::type_t::QUADRATIC_مقصد:
	{
	  hb_outline_نقطہ_t p2 = *it++;
	  pen->quadratic_مقصد (pen_ڈیٹا, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	توڑو;
	حالت hb_outline_نقطہ_t::type_t::CUBIC_مقصد:
	{
	  hb_outline_نقطہ_t p2 = *it++;
	  hb_outline_نقطہ_t p3 = *it++;
	  pen->cubic_مقصد (pen_ڈیٹا, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	توڑو;
      }
    }
    pen->close_راستہ (pen_ڈیٹا, st);
    پہلا = contour;
  }
}

خالی hb_outline_t::translate (اعشاری dx, اعشاری dy)
{
  برائے (خودکار &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

خالی hb_outline_t::slant (اعشاری slant_xy)
{
  برائے (خودکار &p : points)
    p.x += slant_xy * p.y;
}

اعشاری hb_outline_t::control_area () ثابت
{
  اعشاری a = 0;
  غیرعلامتی پہلا = 0;
  برائے (غیرعلامتی contour : contours)
  {
    برائے (غیرعلامتی i = پہلا; i < contour; i++)
    {
      غیرعلامتی j = i + 1 < contour ? i + 1 : پہلا;

      خودکار &pi = points[i];
      خودکار &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    پہلا = contour;
  }
  واپس a * .5f;
}

خالی hb_outline_t::embolden (اعشاری x_strength, اعشاری y_strength,
			     اعشاری x_سرکاؤ, اعشاری y_سرکاؤ)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  اگر (!x_strength && !y_strength) واپس;
  اگر (!points) واپس;

  x_strength /= 2.f;
  y_strength /= 2.f;

  منطقی orientation_negative = control_area () < 0;

  علامتی پہلا = 0;
  برائے (غیرعلامتی c = 0; c < contours.لمبائی; c++)
  {
    hb_outline_سمتیہ_t اندر, برآمد, anchor, سرکاؤ;
    اعشاری l_اندر, l_برآمد, l_anchor = 0, l, q, d;

    l_اندر = 0;
    علامتی آخری = (صحیح) contours[c] - 1;

    /* pacify compiler */
    اندر.x = اندر.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    برائے ( علامتی i = آخری, j = پہلا, k = -1;
	  j != i && i != k;
	  j = j < آخری ? j + 1 : پہلا )
    {
      اگر ( j != k )
      {
	برآمد.x = points[j].x - points[i].x;
	برآمد.y = points[j].y - points[i].y;
	l_برآمد = برآمد.normalize_len ();

	اگر ( l_برآمد == 0 )
	  جاری;
      }
      ورنہ
      {
	برآمد   = anchor;
	l_برآمد = l_anchor;
      }

      اگر ( l_اندر != 0 )
      {
	اگر ( k < 0 )
	{
	  k        = i;
	  anchor   = اندر;
	  l_anchor = l_اندر;
	}

	d = اندر.x * برآمد.x + اندر.y * برآمد.y;

	/* shift only if turn is less than ~160 degrees */
	اگر ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  سرکاؤ.x = اندر.y + برآمد.y;
	  سرکاؤ.y = اندر.x + برآمد.x;

	  اگر ( orientation_negative )
	    سرکاؤ.x = -سرکاؤ.x;
	  ورنہ
	    سرکاؤ.y = -سرکاؤ.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = برآمد.x * اندر.y - برآمد.y * اندر.x;
	  اگر ( orientation_negative )
	    q = -q;

	  l = hb_min (l_اندر, l_برآمد);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  اگر (x_strength * q <= l * d)
	    سرکاؤ.x = سرکاؤ.x * x_strength / d;
	  ورنہ
	    سرکاؤ.x = سرکاؤ.x * l / q;


	  اگر (y_strength * q <= l * d)
	    سرکاؤ.y = سرکاؤ.y * y_strength / d;
	  ورنہ
	    سرکاؤ.y = سرکاؤ.y * l / q;
	}
	ورنہ
	  سرکاؤ.x = سرکاؤ.y = 0;

	برائے ( ;
	      i != j;
	      i = i < آخری ? i + 1 : پہلا )
	{
	  points[i].x += x_سرکاؤ + سرکاؤ.x;
	  points[i].y += y_سرکاؤ + سرکاؤ.y;
	}
      }
      ورنہ
	i = j;

      اندر   = برآمد;
      l_اندر = l_برآمد;
    }

    پہلا = آخری + 1;
  }
}

ساکن خالی
hb_outline_recording_pen_move_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
				  خالی *ڈیٹا,
				  hb_draw_state_t *st,
				  اعشاری مقصد_x, اعشاری مقصد_y,
				  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_outline_t *c = (hb_outline_t *) ڈیٹا;

  c->points.push (hb_outline_نقطہ_t {مقصد_x, مقصد_y, hb_outline_نقطہ_t::type_t::MOVE_مقصد});
}

ساکن خالی
hb_outline_recording_pen_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
				  خالی *ڈیٹا,
				  hb_draw_state_t *st,
				  اعشاری مقصد_x, اعشاری مقصد_y,
				  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_outline_t *c = (hb_outline_t *) ڈیٹا;

  c->points.push (hb_outline_نقطہ_t {مقصد_x, مقصد_y, hb_outline_نقطہ_t::type_t::سطر_مقصد_2});
}

ساکن خالی
hb_outline_recording_pen_quadratic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
				       خالی *ڈیٹا,
				       hb_draw_state_t *st,
				       اعشاری control_x, اعشاری control_y,
				       اعشاری مقصد_x, اعشاری مقصد_y,
				       خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_outline_t *c = (hb_outline_t *) ڈیٹا;

  c->points.push (hb_outline_نقطہ_t {control_x, control_y, hb_outline_نقطہ_t::type_t::QUADRATIC_مقصد});
  c->points.push (hb_outline_نقطہ_t {مقصد_x, مقصد_y, hb_outline_نقطہ_t::type_t::QUADRATIC_مقصد});
}

ساکن خالی
hb_outline_recording_pen_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
				   خالی *ڈیٹا,
				   hb_draw_state_t *st,
				   اعشاری control1_x, اعشاری control1_y,
				   اعشاری control2_x, اعشاری control2_y,
				   اعشاری مقصد_x, اعشاری مقصد_y,
				   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_outline_t *c = (hb_outline_t *) ڈیٹا;

  c->points.push (hb_outline_نقطہ_t {control1_x, control1_y, hb_outline_نقطہ_t::type_t::CUBIC_مقصد});
  c->points.push (hb_outline_نقطہ_t {control2_x, control2_y, hb_outline_نقطہ_t::type_t::CUBIC_مقصد});
  c->points.push (hb_outline_نقطہ_t {مقصد_x, مقصد_y, hb_outline_نقطہ_t::type_t::CUBIC_مقصد});
}

ساکن خالی
hb_outline_recording_pen_close_راستہ (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
				     خالی *ڈیٹا,
				     hb_draw_state_t *st,
				     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_outline_t *c = (hb_outline_t *) ڈیٹا;

  c->contours.push (c->points.لمبائی);
}

ساکن int64_t *
hb_outline_recording_pen_get_budget_بقایا (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
					       خالی *ڈیٹا,
					       خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_outline_t *c = (hb_outline_t *) ڈیٹا;

  واپس c->budget_بقایا;
}

ساکن اندرونی خالی free_static_outline_recording_pen_funcs ();

ساکن ساخت hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_مقصد_func (funcs, hb_outline_recording_pen_move_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func (funcs, hb_outline_recording_pen_سطر_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_quadratic_مقصد_func (funcs, hb_outline_recording_pen_quadratic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_cubic_مقصد_func (funcs, hb_outline_recording_pen_cubic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_close_راستہ_func (funcs, hb_outline_recording_pen_close_راستہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_outline_recording_pen_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    واپس funcs;
  }
} static_outline_recording_pen_funcs;

ساکن اندرونی
خالی free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  واپس static_outline_recording_pen_funcs.get_unconst ();
}


#اگر_ختم
