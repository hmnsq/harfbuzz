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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifndef HB_NO_OUTLINE

#ಅನ್ತರ್ಭಾವ "hb-outline.hh"

#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"


ಶೂನ್ಯ hb_outline_t::replay (hb_draw_funcs_t *pen, ಶೂನ್ಯ *pen_data) ಧ್ರುವ
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  unsigned first = 0;
  ಪ್ರತಿ (unsigned contour : contours)
  {
    auto it = points.as_array ().sub_array (first, contour - first);
    ಯಾವತ್ (it)
    {
      hb_outline_point_t p1 = *it++;
      ವಿಕಲ್ಪ (p1.type)
      {
	ಪಕ್ಷ hb_outline_point_t::type_t::MOVE_TO:
	{
	  pen->move_to (pen_data, st,
			   p1.x, p1.y);
	}
	ಭಙ್ಗ;
	ಪಕ್ಷ hb_outline_point_t::type_t::ಸಾಲು_TO:
	{
	  pen->ಸಾಲು_to (pen_data, st,
			   p1.x, p1.y);
	}
	ಭಙ್ಗ;
	ಪಕ್ಷ hb_outline_point_t::type_t::QUADRATIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  pen->quadratic_to (pen_data, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	ಭಙ್ಗ;
	ಪಕ್ಷ hb_outline_point_t::type_t::CUBIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  hb_outline_point_t p3 = *it++;
	  pen->cubic_to (pen_data, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	ಭಙ್ಗ;
      }
    }
    pen->close_path (pen_data, st);
    first = contour;
  }
}

ಶೂನ್ಯ hb_outline_t::translate (ಭಿನ್ನ dx, ಭಿನ್ನ dy)
{
  ಪ್ರತಿ (auto &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

ಶೂನ್ಯ hb_outline_t::slant (ಭಿನ್ನ slant_xy)
{
  ಪ್ರತಿ (auto &p : points)
    p.x += slant_xy * p.y;
}

ಭಿನ್ನ hb_outline_t::control_area () ಧ್ರುವ
{
  ಭಿನ್ನ a = 0;
  unsigned first = 0;
  ಪ್ರತಿ (unsigned contour : contours)
  {
    ಪ್ರತಿ (unsigned i = first; i < contour; i++)
    {
      unsigned j = i + 1 < contour ? i + 1 : first;

      auto &pi = points[i];
      auto &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    first = contour;
  }
  ಪ್ರತ್ಯಾಗಮ a * .5f;
}

ಶೂನ್ಯ hb_outline_t::embolden (ಭಿನ್ನ x_strength, ಭಿನ್ನ y_strength,
			     ಭಿನ್ನ x_shift, ಭಿನ್ನ y_shift)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  ಯದಿ (!x_strength && !y_strength) ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (!points) ಪ್ರತ್ಯಾಗಮ;

  x_strength /= 2.f;
  y_strength /= 2.f;

  ತಾರ್ಕಿಕ orientation_negative = control_area () < 0;

  signed first = 0;
  ಪ್ರತಿ (unsigned c = 0; c < contours.length; c++)
  {
    hb_outline_vector_t in, out, anchor, shift;
    ಭಿನ್ನ l_in, l_out, l_anchor = 0, l, q, d;

    l_in = 0;
    signed last = (ಪೂರ್ಣಾಙ್ಕ) contours[c] - 1;

    /* pacify compiler */
    in.x = in.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    ಪ್ರತಿ ( signed i = last, j = first, k = -1;
	  j != i && i != k;
	  j = j < last ? j + 1 : first )
    {
      ಯದಿ ( j != k )
      {
	out.x = points[j].x - points[i].x;
	out.y = points[j].y - points[i].y;
	l_out = out.normalize_len ();

	ಯದಿ ( l_out == 0 )
	  ಅನುವರ್ತ;
      }
      ಅನ್ಯಥಾ
      {
	out   = anchor;
	l_out = l_anchor;
      }

      ಯದಿ ( l_in != 0 )
      {
	ಯದಿ ( k < 0 )
	{
	  k        = i;
	  anchor   = in;
	  l_anchor = l_in;
	}

	d = in.x * out.x + in.y * out.y;

	/* shift only if turn is less than ~160 degrees */
	ಯದಿ ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  shift.x = in.y + out.y;
	  shift.y = in.x + out.x;

	  ಯದಿ ( orientation_negative )
	    shift.x = -shift.x;
	  ಅನ್ಯಥಾ
	    shift.y = -shift.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = out.x * in.y - out.y * in.x;
	  ಯದಿ ( orientation_negative )
	    q = -q;

	  l = hb_min (l_in, l_out);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  ಯದಿ (x_strength * q <= l * d)
	    shift.x = shift.x * x_strength / d;
	  ಅನ್ಯಥಾ
	    shift.x = shift.x * l / q;


	  ಯದಿ (y_strength * q <= l * d)
	    shift.y = shift.y * y_strength / d;
	  ಅನ್ಯಥಾ
	    shift.y = shift.y * l / q;
	}
	ಅನ್ಯಥಾ
	  shift.x = shift.y = 0;

	ಪ್ರತಿ ( ;
	      i != j;
	      i = i < last ? i + 1 : first )
	{
	  points[i].x += x_shift + shift.x;
	  points[i].y += y_shift + shift.y;
	}
      }
      ಅನ್ಯಥಾ
	i = j;

      in   = out;
      l_in = l_out;
    }

    first = last + 1;
  }
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_outline_recording_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  ಶೂನ್ಯ *data,
				  hb_draw_state_t *st,
				  ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
				  ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::MOVE_TO});
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_outline_recording_pen_ಸಾಲು_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  ಶೂನ್ಯ *data,
				  hb_draw_state_t *st,
				  ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
				  ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::ಸಾಲು_TO});
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_outline_recording_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				       ಶೂನ್ಯ *data,
				       hb_draw_state_t *st,
				       ಭಿನ್ನ control_x, ಭಿನ್ನ control_y,
				       ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
				       ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {control_x, control_y, hb_outline_point_t::type_t::QUADRATIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::QUADRATIC_TO});
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_outline_recording_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				   ಶೂನ್ಯ *data,
				   hb_draw_state_t *st,
				   ಭಿನ್ನ control1_x, ಭಿನ್ನ control1_y,
				   ಭಿನ್ನ control2_x, ಭಿನ್ನ control2_y,
				   ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
				   ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {control1_x, control1_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {control2_x, control2_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::CUBIC_TO});
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_outline_recording_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				     ಶೂನ್ಯ *data,
				     hb_draw_state_t *st,
				     ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->contours.push (c->points.length);
}

ಸ್ಥಿರ int64_t *
hb_outline_recording_pen_get_budget_remaining (hb_draw_funcs_t *dfuncs HB_UNUSED,
					       ಶೂನ್ಯ *data,
					       ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  ಪ್ರತ್ಯಾಗಮ c->budget_remaining;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_outline_recording_pen_funcs ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  ಸ್ಥಿರ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_outline_recording_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_ಸಾಲು_to_func (funcs, hb_outline_recording_pen_ಸಾಲು_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_outline_recording_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_outline_recording_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_outline_recording_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_outline_recording_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    ಪ್ರತ್ಯಾಗಮ funcs;
  }
} static_outline_recording_pen_funcs;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  ಪ್ರತ್ಯಾಗಮ static_outline_recording_pen_funcs.get_unconst ();
}


#endif
