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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OUTLINE

#অন্তর্ভাব "hb-outline.hh"

#অন্তর্ভাব "hb-machinery.hh"


শূন্য hb_outline_t::replay (hb_draw_funcs_t *pen, শূন্য *pen_data) ধ্রুব
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  unsigned first = 0;
  প্রতি (unsigned contour : contours)
  {
    auto it = points.as_array ().sub_array (first, contour - first);
    যাবত্ (it)
    {
      hb_outline_point_t p1 = *it++;
      বিকল্প (p1.type)
      {
	পক্ষ hb_outline_point_t::type_t::MOVE_TO:
	{
	  pen->move_to (pen_data, st,
			   p1.x, p1.y);
	}
	ভঙ্গ;
	পক্ষ hb_outline_point_t::type_t::সারি_TO:
	{
	  pen->সারি_to (pen_data, st,
			   p1.x, p1.y);
	}
	ভঙ্গ;
	পক্ষ hb_outline_point_t::type_t::QUADRATIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  pen->quadratic_to (pen_data, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	ভঙ্গ;
	পক্ষ hb_outline_point_t::type_t::CUBIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  hb_outline_point_t p3 = *it++;
	  pen->cubic_to (pen_data, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	ভঙ্গ;
      }
    }
    pen->close_path (pen_data, st);
    first = contour;
  }
}

শূন্য hb_outline_t::translate (ভিন্ন dx, ভিন্ন dy)
{
  প্রতি (auto &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

শূন্য hb_outline_t::slant (ভিন্ন slant_xy)
{
  প্রতি (auto &p : points)
    p.x += slant_xy * p.y;
}

ভিন্ন hb_outline_t::control_area () ধ্রুব
{
  ভিন্ন a = 0;
  unsigned first = 0;
  প্রতি (unsigned contour : contours)
  {
    প্রতি (unsigned i = first; i < contour; i++)
    {
      unsigned j = i + 1 < contour ? i + 1 : first;

      auto &pi = points[i];
      auto &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    first = contour;
  }
  প্রত্যাগম a * .5f;
}

শূন্য hb_outline_t::embolden (ভিন্ন x_strength, ভিন্ন y_strength,
			     ভিন্ন x_shift, ভিন্ন y_shift)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  যদি (!x_strength && !y_strength) প্রত্যাগম;
  যদি (!points) প্রত্যাগম;

  x_strength /= 2.f;
  y_strength /= 2.f;

  তার্কিক orientation_negative = control_area () < 0;

  signed first = 0;
  প্রতি (unsigned c = 0; c < contours.length; c++)
  {
    hb_outline_vector_t in, out, anchor, shift;
    ভিন্ন l_in, l_out, l_anchor = 0, l, q, d;

    l_in = 0;
    signed last = (পূর্ণাঙ্ক) contours[c] - 1;

    /* pacify compiler */
    in.x = in.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    প্রতি ( signed i = last, j = first, k = -1;
	  j != i && i != k;
	  j = j < last ? j + 1 : first )
    {
      যদি ( j != k )
      {
	out.x = points[j].x - points[i].x;
	out.y = points[j].y - points[i].y;
	l_out = out.normalize_len ();

	যদি ( l_out == 0 )
	  অনুবর্ত;
      }
      অন্যথা
      {
	out   = anchor;
	l_out = l_anchor;
      }

      যদি ( l_in != 0 )
      {
	যদি ( k < 0 )
	{
	  k        = i;
	  anchor   = in;
	  l_anchor = l_in;
	}

	d = in.x * out.x + in.y * out.y;

	/* shift only if turn is less than ~160 degrees */
	যদি ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  shift.x = in.y + out.y;
	  shift.y = in.x + out.x;

	  যদি ( orientation_negative )
	    shift.x = -shift.x;
	  অন্যথা
	    shift.y = -shift.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = out.x * in.y - out.y * in.x;
	  যদি ( orientation_negative )
	    q = -q;

	  l = hb_min (l_in, l_out);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  যদি (x_strength * q <= l * d)
	    shift.x = shift.x * x_strength / d;
	  অন্যথা
	    shift.x = shift.x * l / q;


	  যদি (y_strength * q <= l * d)
	    shift.y = shift.y * y_strength / d;
	  অন্যথা
	    shift.y = shift.y * l / q;
	}
	অন্যথা
	  shift.x = shift.y = 0;

	প্রতি ( ;
	      i != j;
	      i = i < last ? i + 1 : first )
	{
	  points[i].x += x_shift + shift.x;
	  points[i].y += y_shift + shift.y;
	}
      }
      অন্যথা
	i = j;

      in   = out;
      l_in = l_out;
    }

    first = last + 1;
  }
}

স্থির শূন্য
hb_outline_recording_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  শূন্য *data,
				  hb_draw_state_t *st,
				  ভিন্ন to_x, ভিন্ন to_y,
				  শূন্য *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::MOVE_TO});
}

স্থির শূন্য
hb_outline_recording_pen_সারি_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  শূন্য *data,
				  hb_draw_state_t *st,
				  ভিন্ন to_x, ভিন্ন to_y,
				  শূন্য *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::সারি_TO});
}

স্থির শূন্য
hb_outline_recording_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				       শূন্য *data,
				       hb_draw_state_t *st,
				       ভিন্ন control_x, ভিন্ন control_y,
				       ভিন্ন to_x, ভিন্ন to_y,
				       শূন্য *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {control_x, control_y, hb_outline_point_t::type_t::QUADRATIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::QUADRATIC_TO});
}

স্থির শূন্য
hb_outline_recording_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				   শূন্য *data,
				   hb_draw_state_t *st,
				   ভিন্ন control1_x, ভিন্ন control1_y,
				   ভিন্ন control2_x, ভিন্ন control2_y,
				   ভিন্ন to_x, ভিন্ন to_y,
				   শূন্য *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {control1_x, control1_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {control2_x, control2_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::CUBIC_TO});
}

স্থির শূন্য
hb_outline_recording_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				     শূন্য *data,
				     hb_draw_state_t *st,
				     শূন্য *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->contours.push (c->points.length);
}

স্থির int64_t *
hb_outline_recording_pen_get_budget_remaining (hb_draw_funcs_t *dfuncs HB_UNUSED,
					       শূন্য *data,
					       শূন্য *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  প্রত্যাগম c->budget_remaining;
}

স্থির অন্তঃস্থ শূন্য free_static_outline_recording_pen_funcs ();

স্থির সংরচনা hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  স্থির hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_outline_recording_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_সারি_to_func (funcs, hb_outline_recording_pen_সারি_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_outline_recording_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_outline_recording_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_outline_recording_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_outline_recording_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    প্রত্যাগম funcs;
  }
} static_outline_recording_pen_funcs;

স্থির অন্তঃস্থ
শূন্য free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  প্রত্যাগম static_outline_recording_pen_funcs.get_unconst ();
}


#endif
