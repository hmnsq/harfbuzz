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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OUTLINE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-outline.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"


ਖਾਲੀ hb_outline_t::replay (hb_draw_funcs_t *pen, ਖਾਲੀ *pen_ਅੰਕੜੇ) ਅਚਲ
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਹਿਲਾ = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ contour : contours)
  {
    ਸਵੈਚਾਲਿਤ it = points.as_array ().sub_array (ਪਹਿਲਾ, contour - ਪਹਿਲਾ);
    ਜਦੋਂ (it)
    {
      hb_outline_ਬਿੰਦੂ_t p1 = *it++;
      ਚੋਣ (p1.type)
      {
	ਹਾਲਤ hb_outline_ਬਿੰਦੂ_t::type_t::MOVE_ਮੰਜ਼ਿਲ:
	{
	  pen->move_ਮੰਜ਼ਿਲ (pen_ਅੰਕੜੇ, st,
			   p1.x, p1.y);
	}
	ਰੋਕੋ;
	ਹਾਲਤ hb_outline_ਬਿੰਦੂ_t::type_t::ਸਤਰ_ਮੰਜ਼ਿਲ_2:
	{
	  pen->ਸਤਰ_ਮੰਜ਼ਿਲ (pen_ਅੰਕੜੇ, st,
			   p1.x, p1.y);
	}
	ਰੋਕੋ;
	ਹਾਲਤ hb_outline_ਬਿੰਦੂ_t::type_t::QUADRATIC_ਮੰਜ਼ਿਲ:
	{
	  hb_outline_ਬਿੰਦੂ_t p2 = *it++;
	  pen->quadratic_ਮੰਜ਼ਿਲ (pen_ਅੰਕੜੇ, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	ਰੋਕੋ;
	ਹਾਲਤ hb_outline_ਬਿੰਦੂ_t::type_t::CUBIC_ਮੰਜ਼ਿਲ:
	{
	  hb_outline_ਬਿੰਦੂ_t p2 = *it++;
	  hb_outline_ਬਿੰਦੂ_t p3 = *it++;
	  pen->cubic_ਮੰਜ਼ਿਲ (pen_ਅੰਕੜੇ, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	ਰੋਕੋ;
      }
    }
    pen->close_ਰਸਤਾ (pen_ਅੰਕੜੇ, st);
    ਪਹਿਲਾ = contour;
  }
}

ਖਾਲੀ hb_outline_t::translate (ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy)
{
  ਲਈ (ਸਵੈਚਾਲਿਤ &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

ਖਾਲੀ hb_outline_t::slant (ਦਸ਼ਮਲਵ slant_xy)
{
  ਲਈ (ਸਵੈਚਾਲਿਤ &p : points)
    p.x += slant_xy * p.y;
}

ਦਸ਼ਮਲਵ hb_outline_t::control_area () ਅਚਲ
{
  ਦਸ਼ਮਲਵ a = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਹਿਲਾ = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ contour : contours)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਪਹਿਲਾ; i < contour; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ j = i + 1 < contour ? i + 1 : ਪਹਿਲਾ;

      ਸਵੈਚਾਲਿਤ &pi = points[i];
      ਸਵੈਚਾਲਿਤ &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    ਪਹਿਲਾ = contour;
  }
  ਵਾਪਸ a * .5f;
}

ਖਾਲੀ hb_outline_t::embolden (ਦਸ਼ਮਲਵ x_strength, ਦਸ਼ਮਲਵ y_strength,
			     ਦਸ਼ਮਲਵ x_ਖਿਸਕਾਓ, ਦਸ਼ਮਲਵ y_ਖਿਸਕਾਓ)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  ਜੇ (!x_strength && !y_strength) ਵਾਪਸ;
  ਜੇ (!points) ਵਾਪਸ;

  x_strength /= 2.f;
  y_strength /= 2.f;

  ਤਾਰਕਿਕ orientation_negative = control_area () < 0;

  ਨਿਸ਼ਾਨੀਵਾਲਾ ਪਹਿਲਾ = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ c = 0; c < contours.ਲੰਮਾਈ; c++)
  {
    hb_outline_ਸਦਿਸ਼_t ਵਿੱਚ, ਬਾਹਰਲਾ, anchor, ਖਿਸਕਾਓ;
    ਦਸ਼ਮਲਵ l_ਵਿੱਚ, l_ਬਾਹਰਲਾ, l_anchor = 0, l, q, d;

    l_ਵਿੱਚ = 0;
    ਨਿਸ਼ਾਨੀਵਾਲਾ ਆਖਰੀ = (ਪੂਰਨ) contours[c] - 1;

    /* pacify compiler */
    ਵਿੱਚ.x = ਵਿੱਚ.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    ਲਈ ( ਨਿਸ਼ਾਨੀਵਾਲਾ i = ਆਖਰੀ, j = ਪਹਿਲਾ, k = -1;
	  j != i && i != k;
	  j = j < ਆਖਰੀ ? j + 1 : ਪਹਿਲਾ )
    {
      ਜੇ ( j != k )
      {
	ਬਾਹਰਲਾ.x = points[j].x - points[i].x;
	ਬਾਹਰਲਾ.y = points[j].y - points[i].y;
	l_ਬਾਹਰਲਾ = ਬਾਹਰਲਾ.normalize_len ();

	ਜੇ ( l_ਬਾਹਰਲਾ == 0 )
	  ਜਾਰੀ;
      }
      ਨਹੀਂਤਾਂ
      {
	ਬਾਹਰਲਾ   = anchor;
	l_ਬਾਹਰਲਾ = l_anchor;
      }

      ਜੇ ( l_ਵਿੱਚ != 0 )
      {
	ਜੇ ( k < 0 )
	{
	  k        = i;
	  anchor   = ਵਿੱਚ;
	  l_anchor = l_ਵਿੱਚ;
	}

	d = ਵਿੱਚ.x * ਬਾਹਰਲਾ.x + ਵਿੱਚ.y * ਬਾਹਰਲਾ.y;

	/* shift only if turn is less than ~160 degrees */
	ਜੇ ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  ਖਿਸਕਾਓ.x = ਵਿੱਚ.y + ਬਾਹਰਲਾ.y;
	  ਖਿਸਕਾਓ.y = ਵਿੱਚ.x + ਬਾਹਰਲਾ.x;

	  ਜੇ ( orientation_negative )
	    ਖਿਸਕਾਓ.x = -ਖਿਸਕਾਓ.x;
	  ਨਹੀਂਤਾਂ
	    ਖਿਸਕਾਓ.y = -ਖਿਸਕਾਓ.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = ਬਾਹਰਲਾ.x * ਵਿੱਚ.y - ਬਾਹਰਲਾ.y * ਵਿੱਚ.x;
	  ਜੇ ( orientation_negative )
	    q = -q;

	  l = hb_min (l_ਵਿੱਚ, l_ਬਾਹਰਲਾ);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  ਜੇ (x_strength * q <= l * d)
	    ਖਿਸਕਾਓ.x = ਖਿਸਕਾਓ.x * x_strength / d;
	  ਨਹੀਂਤਾਂ
	    ਖਿਸਕਾਓ.x = ਖਿਸਕਾਓ.x * l / q;


	  ਜੇ (y_strength * q <= l * d)
	    ਖਿਸਕਾਓ.y = ਖਿਸਕਾਓ.y * y_strength / d;
	  ਨਹੀਂਤਾਂ
	    ਖਿਸਕਾਓ.y = ਖਿਸਕਾਓ.y * l / q;
	}
	ਨਹੀਂਤਾਂ
	  ਖਿਸਕਾਓ.x = ਖਿਸਕਾਓ.y = 0;

	ਲਈ ( ;
	      i != j;
	      i = i < ਆਖਰੀ ? i + 1 : ਪਹਿਲਾ )
	{
	  points[i].x += x_ਖਿਸਕਾਓ + ਖਿਸਕਾਓ.x;
	  points[i].y += y_ਖਿਸਕਾਓ + ਖਿਸਕਾਓ.y;
	}
      }
      ਨਹੀਂਤਾਂ
	i = j;

      ਵਿੱਚ   = ਬਾਹਰਲਾ;
      l_ਵਿੱਚ = l_ਬਾਹਰਲਾ;
    }

    ਪਹਿਲਾ = ਆਖਰੀ + 1;
  }
}

ਸਥਿਰ ਖਾਲੀ
hb_outline_recording_pen_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
				  ਖਾਲੀ *ਅੰਕੜੇ,
				  hb_draw_state_t *st,
				  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
				  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_outline_t *c = (hb_outline_t *) ਅੰਕੜੇ;

  c->points.push (hb_outline_ਬਿੰਦੂ_t {ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y, hb_outline_ਬਿੰਦੂ_t::type_t::MOVE_ਮੰਜ਼ਿਲ});
}

ਸਥਿਰ ਖਾਲੀ
hb_outline_recording_pen_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
				  ਖਾਲੀ *ਅੰਕੜੇ,
				  hb_draw_state_t *st,
				  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
				  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_outline_t *c = (hb_outline_t *) ਅੰਕੜੇ;

  c->points.push (hb_outline_ਬਿੰਦੂ_t {ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y, hb_outline_ਬਿੰਦੂ_t::type_t::ਸਤਰ_ਮੰਜ਼ਿਲ_2});
}

ਸਥਿਰ ਖਾਲੀ
hb_outline_recording_pen_quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
				       ਖਾਲੀ *ਅੰਕੜੇ,
				       hb_draw_state_t *st,
				       ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
				       ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
				       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_outline_t *c = (hb_outline_t *) ਅੰਕੜੇ;

  c->points.push (hb_outline_ਬਿੰਦੂ_t {control_x, control_y, hb_outline_ਬਿੰਦੂ_t::type_t::QUADRATIC_ਮੰਜ਼ਿਲ});
  c->points.push (hb_outline_ਬਿੰਦੂ_t {ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y, hb_outline_ਬਿੰਦੂ_t::type_t::QUADRATIC_ਮੰਜ਼ਿਲ});
}

ਸਥਿਰ ਖਾਲੀ
hb_outline_recording_pen_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
				   ਖਾਲੀ *ਅੰਕੜੇ,
				   hb_draw_state_t *st,
				   ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
				   ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
				   ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
				   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_outline_t *c = (hb_outline_t *) ਅੰਕੜੇ;

  c->points.push (hb_outline_ਬਿੰਦੂ_t {control1_x, control1_y, hb_outline_ਬਿੰਦੂ_t::type_t::CUBIC_ਮੰਜ਼ਿਲ});
  c->points.push (hb_outline_ਬਿੰਦੂ_t {control2_x, control2_y, hb_outline_ਬਿੰਦੂ_t::type_t::CUBIC_ਮੰਜ਼ਿਲ});
  c->points.push (hb_outline_ਬਿੰਦੂ_t {ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y, hb_outline_ਬਿੰਦੂ_t::type_t::CUBIC_ਮੰਜ਼ਿਲ});
}

ਸਥਿਰ ਖਾਲੀ
hb_outline_recording_pen_close_ਰਸਤਾ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
				     ਖਾਲੀ *ਅੰਕੜੇ,
				     hb_draw_state_t *st,
				     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_outline_t *c = (hb_outline_t *) ਅੰਕੜੇ;

  c->contours.push (c->points.ਲੰਮਾਈ);
}

ਸਥਿਰ int64_t *
hb_outline_recording_pen_get_budget_ਬਚਿਆ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
					       ਖਾਲੀ *ਅੰਕੜੇ,
					       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_outline_t *c = (hb_outline_t *) ਅੰਕੜੇ;

  ਵਾਪਸ c->budget_ਬਚਿਆ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_outline_recording_pen_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func (funcs, hb_outline_recording_pen_move_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func (funcs, hb_outline_recording_pen_ਸਤਰ_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_quadratic_ਮੰਜ਼ਿਲ_func (funcs, hb_outline_recording_pen_quadratic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func (funcs, hb_outline_recording_pen_cubic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func (funcs, hb_outline_recording_pen_close_ਰਸਤਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_outline_recording_pen_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    ਵਾਪਸ funcs;
  }
} static_outline_recording_pen_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  ਵਾਪਸ static_outline_recording_pen_funcs.get_unconst ();
}


#ਜੇ_ਅੰਤ
