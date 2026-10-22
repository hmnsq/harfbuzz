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


शून्य hb_outline_t::replay (hb_draw_funcs_t *pen, शून्य *pen_data) स्थिरांक
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  अचिन्हित first = 0;
  साठी (अचिन्हित contour : contours)
  {
    स्वतः it = points.as_array ().sub_array (first, contour - first);
    पर्यंत (it)
    {
      hb_outline_point_t p1 = *it++;
      निवड (p1.type)
      {
	स्थिती hb_outline_point_t::type_t::MOVE_TO:
	{
	  pen->move_to (pen_data, st,
			   p1.x, p1.y);
	}
	थांबा;
	स्थिती hb_outline_point_t::type_t::ओळ_TO:
	{
	  pen->ओळ_to (pen_data, st,
			   p1.x, p1.y);
	}
	थांबा;
	स्थिती hb_outline_point_t::type_t::QUADRATIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  pen->quadratic_to (pen_data, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	थांबा;
	स्थिती hb_outline_point_t::type_t::CUBIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  hb_outline_point_t p3 = *it++;
	  pen->cubic_to (pen_data, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	थांबा;
      }
    }
    pen->close_path (pen_data, st);
    first = contour;
  }
}

शून्य hb_outline_t::translate (दशांश dx, दशांश dy)
{
  साठी (स्वतः &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

शून्य hb_outline_t::slant (दशांश slant_xy)
{
  साठी (स्वतः &p : points)
    p.x += slant_xy * p.y;
}

दशांश hb_outline_t::control_area () स्थिरांक
{
  दशांश a = 0;
  अचिन्हित first = 0;
  साठी (अचिन्हित contour : contours)
  {
    साठी (अचिन्हित i = first; i < contour; i++)
    {
      अचिन्हित j = i + 1 < contour ? i + 1 : first;

      स्वतः &pi = points[i];
      स्वतः &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    first = contour;
  }
  परत a * .5f;
}

शून्य hb_outline_t::embolden (दशांश x_strength, दशांश y_strength,
			     दशांश x_shift, दशांश y_shift)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  जर (!x_strength && !y_strength) परत;
  जर (!points) परत;

  x_strength /= 2.f;
  y_strength /= 2.f;

  तार्किक orientation_negative = control_area () < 0;

  चिन्हित first = 0;
  साठी (अचिन्हित c = 0; c < contours.length; c++)
  {
    hb_outline_vector_t in, out, anchor, shift;
    दशांश l_in, l_out, l_anchor = 0, l, q, d;

    l_in = 0;
    चिन्हित last = (पूर्णांक) contours[c] - 1;

    /* pacify compiler */
    in.x = in.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    साठी ( चिन्हित i = last, j = first, k = -1;
	  j != i && i != k;
	  j = j < last ? j + 1 : first )
    {
      जर ( j != k )
      {
	out.x = points[j].x - points[i].x;
	out.y = points[j].y - points[i].y;
	l_out = out.normalize_len ();

	जर ( l_out == 0 )
	  सुरूठेवा;
      }
      अन्यथा
      {
	out   = anchor;
	l_out = l_anchor;
      }

      जर ( l_in != 0 )
      {
	जर ( k < 0 )
	{
	  k        = i;
	  anchor   = in;
	  l_anchor = l_in;
	}

	d = in.x * out.x + in.y * out.y;

	/* shift only if turn is less than ~160 degrees */
	जर ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  shift.x = in.y + out.y;
	  shift.y = in.x + out.x;

	  जर ( orientation_negative )
	    shift.x = -shift.x;
	  अन्यथा
	    shift.y = -shift.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = out.x * in.y - out.y * in.x;
	  जर ( orientation_negative )
	    q = -q;

	  l = hb_min (l_in, l_out);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  जर (x_strength * q <= l * d)
	    shift.x = shift.x * x_strength / d;
	  अन्यथा
	    shift.x = shift.x * l / q;


	  जर (y_strength * q <= l * d)
	    shift.y = shift.y * y_strength / d;
	  अन्यथा
	    shift.y = shift.y * l / q;
	}
	अन्यथा
	  shift.x = shift.y = 0;

	साठी ( ;
	      i != j;
	      i = i < last ? i + 1 : first )
	{
	  points[i].x += x_shift + shift.x;
	  points[i].y += y_shift + shift.y;
	}
      }
      अन्यथा
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
				  दशांश to_x, दशांश to_y,
				  शून्य *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::MOVE_TO});
}

स्थिर शून्य
hb_outline_recording_pen_ओळ_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  शून्य *data,
				  hb_draw_state_t *st,
				  दशांश to_x, दशांश to_y,
				  शून्य *user_data HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) data;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::ओळ_TO});
}

स्थिर शून्य
hb_outline_recording_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				       शून्य *data,
				       hb_draw_state_t *st,
				       दशांश control_x, दशांश control_y,
				       दशांश to_x, दशांश to_y,
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
				   दशांश control1_x, दशांश control1_y,
				   दशांश control2_x, दशांश control2_y,
				   दशांश to_x, दशांश to_y,
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

  परत c->budget_remaining;
}

स्थिर ओळीत शून्य free_static_outline_recording_pen_funcs ();

स्थिर संरचना hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  स्थिर hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_outline_recording_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_ओळ_to_func (funcs, hb_outline_recording_pen_ओळ_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_outline_recording_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_outline_recording_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_outline_recording_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_outline_recording_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    परत funcs;
  }
} static_outline_recording_pen_funcs;

स्थिर ओळीत
शून्य free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  परत static_outline_recording_pen_funcs.get_unconst ();
}


#endif
