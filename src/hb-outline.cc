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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_OUTLINE

#अन्तर्भाव "hb-outline.hh"

#अन्तर्भाव "hb-machinery.hh"


शून्य hb_outline_t::replay (hb_draw_funcs_t *pen, शून्य *pen_data) ध्रुव
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  unsigned first = 0;
  अत्थाय (unsigned contour : contours)
  {
    auto it = points.as_array ().sub_array (first, contour - first);
    याव (it)
    {
      hb_outline_point_t p1 = *it++;
      विकल्प (p1.type)
      {
	पक्ष hb_outline_point_t::type_t::MOVE_TO:
	{
	  pen->move_to (pen_data, st,
			   p1.x, p1.y);
	}
	भङ्ग;
	पक्ष hb_outline_point_t::type_t::पङ्क्तिः_TO:
	{
	  pen->पङ्क्तिः_to (pen_data, st,
			   p1.x, p1.y);
	}
	भङ्ग;
	पक्ष hb_outline_point_t::type_t::QUADRATIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  pen->quadratic_to (pen_data, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	भङ्ग;
	पक्ष hb_outline_point_t::type_t::CUBIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  hb_outline_point_t p3 = *it++;
	  pen->cubic_to (pen_data, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	भङ्ग;
      }
    }
    pen->close_path (pen_data, st);
    first = contour;
  }
}

शून्य hb_outline_t::translate (भिन्न dx, भिन्न dy)
{
  अत्थाय (auto &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

शून्य hb_outline_t::slant (भिन्न slant_xy)
{
  अत्थाय (auto &p : points)
    p.x += slant_xy * p.y;
}

भिन्न hb_outline_t::control_area () ध्रुव
{
  भिन्न a = 0;
  unsigned first = 0;
  अत्थाय (unsigned contour : contours)
  {
    अत्थाय (unsigned i = first; i < contour; i++)
    {
      unsigned j = i + 1 < contour ? i + 1 : first;

      auto &pi = points[i];
      auto &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    first = contour;
  }
  पटिनिवत्त a * .5f;
}

शून्य hb_outline_t::embolden (भिन्न x_strength, भिन्न y_strength,
			     भिन्न x_shift, भिन्न y_shift)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  सचे (!x_strength && !y_strength) पटिनिवत्त;
  सचे (!points) पटिनिवत्त;

  x_strength /= 2.f;
  y_strength /= 2.f;

  तार्किक orientation_negative = control_area () < 0;

  signed first = 0;
  अत्थाय (unsigned c = 0; c < contours.length; c++)
  {
    hb_outline_vector_t in, out, anchor, shift;
    भिन्न l_in, l_out, l_anchor = 0, l, q, d;

    l_in = 0;
    signed last = (पूर्णाङ्क) contours[c] - 1;

    /* pacify compiler */
    in.x = in.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    अत्थाय ( signed i = last, j = first, k = -1;
	  j != i && i != k;
	  j = j < last ? j + 1 : first )
    {
      सचे ( j != k )
      {
	out.x = points[j].x - points[i].x;
	out.y = points[j].y - points[i].y;
	l_out = out.normalize_len ();

	सचे ( l_out == 0 )
	  अनुवर्त;
      }
      अञ्ञथा
      {
	out   = anchor;
	l_out = l_anchor;
      }

      सचे ( l_in != 0 )
      {
	सचे ( k < 0 )
	{
	  k        = i;
	  anchor   = in;
	  l_anchor = l_in;
	}

	d = in.x * out.x + in.y * out.y;

	/* shift only if turn is less than ~160 degrees */
	सचे ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  shift.x = in.y + out.y;
	  shift.y = in.x + out.x;

	  सचे ( orientation_negative )
	    shift.x = -shift.x;
	  अञ्ञथा
	    shift.y = -shift.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = out.x * in.y - out.y * in.x;
	  सचे ( orientation_negative )
	    q = -q;

	  l = hb_min (l_in, l_out);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  सचे (x_strength * q <= l * d)
	    shift.x = shift.x * x_strength / d;
	  अञ्ञथा
	    shift.x = shift.x * l / q;


	  सचे (y_strength * q <= l * d)
	    shift.y = shift.y * y_strength / d;
	  अञ्ञथा
	    shift.y = shift.y * l / q;
	}
	अञ्ञथा
	  shift.x = shift.y = 0;

	अत्थाय ( ;
	      i != j;
	      i = i < last ? i + 1 : first )
	{
	  points[i].x += x_shift + shift.x;
	  points[i].y += y_shift + shift.y;
	}
      }
      अञ्ञथा
	i = j;

      in   = out;
      l_in = l_out;
    }

    first = last + 1;
  }
}

स्थिर शून्य
hb_outline_recording_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  शून्य *data,
				  hb_draw_state_t *st,
				  भिन्न to_x, भिन्न to_y,
				  शून्य *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::MOVE_TO});
}

स्थिर शून्य
hb_outline_recording_pen_पङ्क्तिः_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  शून्य *data,
				  hb_draw_state_t *st,
				  भिन्न to_x, भिन्न to_y,
				  शून्य *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::पङ्क्तिः_TO});
}

स्थिर शून्य
hb_outline_recording_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				       शून्य *data,
				       hb_draw_state_t *st,
				       भिन्न control_x, भिन्न control_y,
				       भिन्न to_x, भिन्न to_y,
				       शून्य *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {control_x, control_y, hb_outline_point_t::type_t::QUADRATIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::QUADRATIC_TO});
}

स्थिर शून्य
hb_outline_recording_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				   शून्य *data,
				   hb_draw_state_t *st,
				   भिन्न control1_x, भिन्न control1_y,
				   भिन्न control2_x, भिन्न control2_y,
				   भिन्न to_x, भिन्न to_y,
				   शून्य *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {control1_x, control1_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {control2_x, control2_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::CUBIC_TO});
}

स्थिर शून्य
hb_outline_recording_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				     शून्य *data,
				     hb_draw_state_t *st,
				     शून्य *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->contours.push (c->points.length);
}

स्थिर int64_t *
hb_outline_recording_pen_get_budget_remaining (hb_draw_funcs_t *dfuncs HB_UNUSED,
					       शून्य *data,
					       शून्य *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  पटिनिवत्त c->budget_remaining;
}

स्थिर अन्तःस्थ शून्य free_static_outline_recording_pen_funcs ();

स्थिर संरचना hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  स्थिर hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_outline_recording_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_पङ्क्तिः_to_func (funcs, hb_outline_recording_pen_पङ्क्तिः_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_outline_recording_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_outline_recording_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_outline_recording_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_outline_recording_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    पटिनिवत्त funcs;
  }
} static_outline_recording_pen_funcs;

स्थिर अन्तःस्थ
शून्य free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  पटिनिवत्त static_outline_recording_pen_funcs.get_unconst ();
}


#endif
