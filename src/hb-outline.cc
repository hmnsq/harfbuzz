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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_OUTLINE

#അന്തര്ഭാവ "hb-outline.hh"

#അന്തര്ഭാവ "hb-machinery.hh"


ശൂന്യം hb_outline_t::replay (hb_draw_funcs_t *pen, ശൂന്യം *pen_data) സ്ഥിരം
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  unsigned first = 0;
  വേണ്ടി (unsigned contour : contours)
  {
    auto it = points.as_array ().sub_array (first, contour - first);
    ആകുമ്പോൾ (it)
    {
      hb_outline_point_t p1 = *it++;
      തിരഞ്ഞെടുപ്പ് (p1.type)
      {
	അവസ്ഥ hb_outline_point_t::type_t::MOVE_TO:
	{
	  pen->move_to (pen_data, st,
			   p1.x, p1.y);
	}
	നിർത്തുക;
	അവസ്ഥ hb_outline_point_t::type_t::വരി_TO:
	{
	  pen->വരി_to (pen_data, st,
			   p1.x, p1.y);
	}
	നിർത്തുക;
	അവസ്ഥ hb_outline_point_t::type_t::QUADRATIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  pen->quadratic_to (pen_data, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	നിർത്തുക;
	അവസ്ഥ hb_outline_point_t::type_t::CUBIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  hb_outline_point_t p3 = *it++;
	  pen->cubic_to (pen_data, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	നിർത്തുക;
      }
    }
    pen->close_path (pen_data, st);
    first = contour;
  }
}

ശൂന്യം hb_outline_t::translate (ദശാംശം dx, ദശാംശം dy)
{
  വേണ്ടി (auto &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

ശൂന്യം hb_outline_t::slant (ദശാംശം slant_xy)
{
  വേണ്ടി (auto &p : points)
    p.x += slant_xy * p.y;
}

ദശാംശം hb_outline_t::control_area () സ്ഥിരം
{
  ദശാംശം a = 0;
  unsigned first = 0;
  വേണ്ടി (unsigned contour : contours)
  {
    വേണ്ടി (unsigned i = first; i < contour; i++)
    {
      unsigned j = i + 1 < contour ? i + 1 : first;

      auto &pi = points[i];
      auto &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    first = contour;
  }
  തിരികെ a * .5f;
}

ശൂന്യം hb_outline_t::embolden (ദശാംശം x_strength, ദശാംശം y_strength,
			     ദശാംശം x_shift, ദശാംശം y_shift)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  എങ്കിൽ (!x_strength && !y_strength) തിരികെ;
  എങ്കിൽ (!points) തിരികെ;

  x_strength /= 2.f;
  y_strength /= 2.f;

  തർക്കം orientation_negative = control_area () < 0;

  signed first = 0;
  വേണ്ടി (unsigned c = 0; c < contours.length; c++)
  {
    hb_outline_vector_t in, out, anchor, shift;
    ദശാംശം l_in, l_out, l_anchor = 0, l, q, d;

    l_in = 0;
    signed last = (പൂർണ്ണസംഖ്യ) contours[c] - 1;

    /* pacify compiler */
    in.x = in.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    വേണ്ടി ( signed i = last, j = first, k = -1;
	  j != i && i != k;
	  j = j < last ? j + 1 : first )
    {
      എങ്കിൽ ( j != k )
      {
	out.x = points[j].x - points[i].x;
	out.y = points[j].y - points[i].y;
	l_out = out.normalize_len ();

	എങ്കിൽ ( l_out == 0 )
	  തുടരുക;
      }
      അല്ലെങ്കിൽ
      {
	out   = anchor;
	l_out = l_anchor;
      }

      എങ്കിൽ ( l_in != 0 )
      {
	എങ്കിൽ ( k < 0 )
	{
	  k        = i;
	  anchor   = in;
	  l_anchor = l_in;
	}

	d = in.x * out.x + in.y * out.y;

	/* shift only if turn is less than ~160 degrees */
	എങ്കിൽ ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  shift.x = in.y + out.y;
	  shift.y = in.x + out.x;

	  എങ്കിൽ ( orientation_negative )
	    shift.x = -shift.x;
	  അല്ലെങ്കിൽ
	    shift.y = -shift.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = out.x * in.y - out.y * in.x;
	  എങ്കിൽ ( orientation_negative )
	    q = -q;

	  l = hb_min (l_in, l_out);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  എങ്കിൽ (x_strength * q <= l * d)
	    shift.x = shift.x * x_strength / d;
	  അല്ലെങ്കിൽ
	    shift.x = shift.x * l / q;


	  എങ്കിൽ (y_strength * q <= l * d)
	    shift.y = shift.y * y_strength / d;
	  അല്ലെങ്കിൽ
	    shift.y = shift.y * l / q;
	}
	അല്ലെങ്കിൽ
	  shift.x = shift.y = 0;

	വേണ്ടി ( ;
	      i != j;
	      i = i < last ? i + 1 : first )
	{
	  points[i].x += x_shift + shift.x;
	  points[i].y += y_shift + shift.y;
	}
      }
      അല്ലെങ്കിൽ
	i = j;

      in   = out;
      l_in = l_out;
    }

    first = last + 1;
  }
}

സ്ഥിതം ശൂന്യം
hb_outline_recording_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  ശൂന്യം *data,
				  hb_draw_state_t *st,
				  ദശാംശം to_x, ദശാംശം to_y,
				  ശൂന്യം *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::MOVE_TO});
}

സ്ഥിതം ശൂന്യം
hb_outline_recording_pen_വരി_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  ശൂന്യം *data,
				  hb_draw_state_t *st,
				  ദശാംശം to_x, ദശാംശം to_y,
				  ശൂന്യം *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::വരി_TO});
}

സ്ഥിതം ശൂന്യം
hb_outline_recording_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				       ശൂന്യം *data,
				       hb_draw_state_t *st,
				       ദശാംശം control_x, ദശാംശം control_y,
				       ദശാംശം to_x, ദശാംശം to_y,
				       ശൂന്യം *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {control_x, control_y, hb_outline_point_t::type_t::QUADRATIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::QUADRATIC_TO});
}

സ്ഥിതം ശൂന്യം
hb_outline_recording_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				   ശൂന്യം *data,
				   hb_draw_state_t *st,
				   ദശാംശം control1_x, ദശാംശം control1_y,
				   ദശാംശം control2_x, ദശാംശം control2_y,
				   ദശാംശം to_x, ദശാംശം to_y,
				   ശൂന്യം *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {control1_x, control1_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {control2_x, control2_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::CUBIC_TO});
}

സ്ഥിതം ശൂന്യം
hb_outline_recording_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				     ശൂന്യം *data,
				     hb_draw_state_t *st,
				     ശൂന്യം *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->contours.push (c->points.length);
}

സ്ഥിതം int64_t *
hb_outline_recording_pen_get_budget_remaining (hb_draw_funcs_t *dfuncs HB_UNUSED,
					       ശൂന്യം *data,
					       ശൂന്യം *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  തിരികെ c->budget_remaining;
}

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_outline_recording_pen_funcs ();

സ്ഥിതം ഘടന hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_outline_recording_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_വരി_to_func (funcs, hb_outline_recording_pen_വരി_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_outline_recording_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_outline_recording_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_outline_recording_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_outline_recording_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    തിരികെ funcs;
  }
} static_outline_recording_pen_funcs;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  തിരികെ static_outline_recording_pen_funcs.get_unconst ();
}


#endif
