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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_OUTLINE

#அந்தர்பாவ "hb-outline.hh"

#அந்தர்பாவ "hb-machinery.hh"


வெறுமை hb_outline_t::replay (hb_draw_funcs_t *pen, வெறுமை *pen_data) மாறிலி
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  unsigned first = 0;
  க்கு (unsigned contour : contours)
  {
    auto it = points.as_array ().sub_array (first, contour - first);
    போது (it)
    {
      hb_outline_point_t p1 = *it++;
      தேர்வு (p1.type)
      {
	நிலை hb_outline_point_t::type_t::MOVE_TO:
	{
	  pen->move_to (pen_data, st,
			   p1.x, p1.y);
	}
	நிறுத்து;
	நிலை hb_outline_point_t::type_t::வரி_TO:
	{
	  pen->வரி_to (pen_data, st,
			   p1.x, p1.y);
	}
	நிறுத்து;
	நிலை hb_outline_point_t::type_t::QUADRATIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  pen->quadratic_to (pen_data, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	நிறுத்து;
	நிலை hb_outline_point_t::type_t::CUBIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  hb_outline_point_t p3 = *it++;
	  pen->cubic_to (pen_data, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	நிறுத்து;
      }
    }
    pen->close_path (pen_data, st);
    first = contour;
  }
}

வெறுமை hb_outline_t::translate (தசம dx, தசம dy)
{
  க்கு (auto &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

வெறுமை hb_outline_t::slant (தசம slant_xy)
{
  க்கு (auto &p : points)
    p.x += slant_xy * p.y;
}

தசம hb_outline_t::control_area () மாறிலி
{
  தசம a = 0;
  unsigned first = 0;
  க்கு (unsigned contour : contours)
  {
    க்கு (unsigned i = first; i < contour; i++)
    {
      unsigned j = i + 1 < contour ? i + 1 : first;

      auto &pi = points[i];
      auto &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    first = contour;
  }
  திரும்பு a * .5f;
}

வெறுமை hb_outline_t::embolden (தசம x_strength, தசம y_strength,
			     தசம x_shift, தசம y_shift)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  எனில் (!x_strength && !y_strength) திரும்பு;
  எனில் (!points) திரும்பு;

  x_strength /= 2.f;
  y_strength /= 2.f;

  தருக்க orientation_negative = control_area () < 0;

  signed first = 0;
  க்கு (unsigned c = 0; c < contours.length; c++)
  {
    hb_outline_vector_t in, out, anchor, shift;
    தசம l_in, l_out, l_anchor = 0, l, q, d;

    l_in = 0;
    signed last = (முழு) contours[c] - 1;

    /* pacify compiler */
    in.x = in.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    க்கு ( signed i = last, j = first, k = -1;
	  j != i && i != k;
	  j = j < last ? j + 1 : first )
    {
      எனில் ( j != k )
      {
	out.x = points[j].x - points[i].x;
	out.y = points[j].y - points[i].y;
	l_out = out.normalize_len ();

	எனில் ( l_out == 0 )
	  தொடர்;
      }
      இல்லையெனில்
      {
	out   = anchor;
	l_out = l_anchor;
      }

      எனில் ( l_in != 0 )
      {
	எனில் ( k < 0 )
	{
	  k        = i;
	  anchor   = in;
	  l_anchor = l_in;
	}

	d = in.x * out.x + in.y * out.y;

	/* shift only if turn is less than ~160 degrees */
	எனில் ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  shift.x = in.y + out.y;
	  shift.y = in.x + out.x;

	  எனில் ( orientation_negative )
	    shift.x = -shift.x;
	  இல்லையெனில்
	    shift.y = -shift.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = out.x * in.y - out.y * in.x;
	  எனில் ( orientation_negative )
	    q = -q;

	  l = hb_min (l_in, l_out);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  எனில் (x_strength * q <= l * d)
	    shift.x = shift.x * x_strength / d;
	  இல்லையெனில்
	    shift.x = shift.x * l / q;


	  எனில் (y_strength * q <= l * d)
	    shift.y = shift.y * y_strength / d;
	  இல்லையெனில்
	    shift.y = shift.y * l / q;
	}
	இல்லையெனில்
	  shift.x = shift.y = 0;

	க்கு ( ;
	      i != j;
	      i = i < last ? i + 1 : first )
	{
	  points[i].x += x_shift + shift.x;
	  points[i].y += y_shift + shift.y;
	}
      }
      இல்லையெனில்
	i = j;

      in   = out;
      l_in = l_out;
    }

    first = last + 1;
  }
}

நிலையான வெறுமை
hb_outline_recording_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  வெறுமை *data,
				  hb_draw_state_t *st,
				  தசம to_x, தசம to_y,
				  வெறுமை *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::MOVE_TO});
}

நிலையான வெறுமை
hb_outline_recording_pen_வரி_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  வெறுமை *data,
				  hb_draw_state_t *st,
				  தசம to_x, தசம to_y,
				  வெறுமை *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::வரி_TO});
}

நிலையான வெறுமை
hb_outline_recording_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				       வெறுமை *data,
				       hb_draw_state_t *st,
				       தசம control_x, தசம control_y,
				       தசம to_x, தசம to_y,
				       வெறுமை *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {control_x, control_y, hb_outline_point_t::type_t::QUADRATIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::QUADRATIC_TO});
}

நிலையான வெறுமை
hb_outline_recording_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				   வெறுமை *data,
				   hb_draw_state_t *st,
				   தசம control1_x, தசம control1_y,
				   தசம control2_x, தசம control2_y,
				   தசம to_x, தசம to_y,
				   வெறுமை *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {control1_x, control1_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {control2_x, control2_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::CUBIC_TO});
}

நிலையான வெறுமை
hb_outline_recording_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				     வெறுமை *data,
				     hb_draw_state_t *st,
				     வெறுமை *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->contours.push (c->points.length);
}

நிலையான int64_t *
hb_outline_recording_pen_get_budget_remaining (hb_draw_funcs_t *dfuncs HB_UNUSED,
					       வெறுமை *data,
					       வெறுமை *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  திரும்பு c->budget_remaining;
}

நிலையான உள்வரி வெறுமை free_static_outline_recording_pen_funcs ();

நிலையான கட்டமைப்பு hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  நிலையான hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_outline_recording_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_வரி_to_func (funcs, hb_outline_recording_pen_வரி_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_outline_recording_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_outline_recording_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_outline_recording_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_outline_recording_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    திரும்பு funcs;
  }
} static_outline_recording_pen_funcs;

நிலையான உள்வரி
வெறுமை free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  திரும்பு static_outline_recording_pen_funcs.get_unconst ();
}


#endif
