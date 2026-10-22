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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OUTLINE

#समावेश "hb-outline.hh"

#समावेश "hb-machinery.hh"


व्योम hb_outline_t::replay (hb_draw_funcs_t *pen, व्योम *pen_तथ्य) स्थिर
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  अचिन्हित first = 0;
  क्रम (अचिन्हित contour : contours)
  {
    स्वतः it = points.as_array ().sub_array (first, contour - first);
    जबतक (it)
    {
      hb_outline_point_t p1 = *it++;
      चयन (p1.type)
      {
	हाल hb_outline_point_t::type_t::MOVE_TO:
	{
	  pen->move_to (pen_तथ्य, st,
			   p1.x, p1.y);
	}
	अवरोध;
	हाल hb_outline_point_t::type_t::पंक्ति_TO:
	{
	  pen->पंक्ति_to (pen_तथ्य, st,
			   p1.x, p1.y);
	}
	अवरोध;
	हाल hb_outline_point_t::type_t::QUADRATIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  pen->quadratic_to (pen_तथ्य, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	अवरोध;
	हाल hb_outline_point_t::type_t::CUBIC_TO:
	{
	  hb_outline_point_t p2 = *it++;
	  hb_outline_point_t p3 = *it++;
	  pen->cubic_to (pen_तथ्य, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	अवरोध;
      }
    }
    pen->close_path (pen_तथ्य, st);
    first = contour;
  }
}

व्योम hb_outline_t::translate (भग्न dx, भग्न dy)
{
  क्रम (स्वतः &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

व्योम hb_outline_t::slant (भग्न slant_xy)
{
  क्रम (स्वतः &p : points)
    p.x += slant_xy * p.y;
}

भग्न hb_outline_t::control_area () स्थिर
{
  भग्न a = 0;
  अचिन्हित first = 0;
  क्रम (अचिन्हित contour : contours)
  {
    क्रम (अचिन्हित i = first; i < contour; i++)
    {
      अचिन्हित j = i + 1 < contour ? i + 1 : first;

      स्वतः &pi = points[i];
      स्वतः &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    first = contour;
  }
  वापस a * .5f;
}

व्योम hb_outline_t::embolden (भग्न x_strength, भग्न y_strength,
			     भग्न x_shift, भग्न y_shift)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  अगर (!x_strength && !y_strength) वापस;
  अगर (!points) वापस;

  x_strength /= 2.f;
  y_strength /= 2.f;

  bool orientation_negative = control_area () < 0;

  चिन्हित first = 0;
  क्रम (अचिन्हित c = 0; c < contours.लंबाई; c++)
  {
    hb_outline_vector_t in, out, anchor, shift;
    भग्न l_in, l_out, l_anchor = 0, l, q, d;

    l_in = 0;
    चिन्हित last = (पूर्णांक) contours[c] - 1;

    /* pacify compiler */
    in.x = in.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    क्रम ( चिन्हित i = last, j = first, k = -1;
	  j != i && i != k;
	  j = j < last ? j + 1 : first )
    {
      अगर ( j != k )
      {
	out.x = points[j].x - points[i].x;
	out.y = points[j].y - points[i].y;
	l_out = out.normalize_len ();

	अगर ( l_out == 0 )
	  जारी;
      }
      अन्यथा
      {
	out   = anchor;
	l_out = l_anchor;
      }

      अगर ( l_in != 0 )
      {
	अगर ( k < 0 )
	{
	  k        = i;
	  anchor   = in;
	  l_anchor = l_in;
	}

	d = in.x * out.x + in.y * out.y;

	/* shift only if turn is less than ~160 degrees */
	अगर ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  shift.x = in.y + out.y;
	  shift.y = in.x + out.x;

	  अगर ( orientation_negative )
	    shift.x = -shift.x;
	  अन्यथा
	    shift.y = -shift.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = out.x * in.y - out.y * in.x;
	  अगर ( orientation_negative )
	    q = -q;

	  l = hb_min (l_in, l_out);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  अगर (x_strength * q <= l * d)
	    shift.x = shift.x * x_strength / d;
	  अन्यथा
	    shift.x = shift.x * l / q;


	  अगर (y_strength * q <= l * d)
	    shift.y = shift.y * y_strength / d;
	  अन्यथा
	    shift.y = shift.y * l / q;
	}
	अन्यथा
	  shift.x = shift.y = 0;

	क्रम ( ;
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

अटल व्योम
hb_outline_recording_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  व्योम *तथ्य,
				  hb_draw_state_t *st,
				  भग्न to_x, भग्न to_y,
				  व्योम *user_तथ्य HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) तथ्य;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::MOVE_TO});
}

अटल व्योम
hb_outline_recording_pen_पंक्ति_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				  व्योम *तथ्य,
				  hb_draw_state_t *st,
				  भग्न to_x, भग्न to_y,
				  व्योम *user_तथ्य HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) तथ्य;

  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::पंक्ति_TO});
}

अटल व्योम
hb_outline_recording_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				       व्योम *तथ्य,
				       hb_draw_state_t *st,
				       भग्न control_x, भग्न control_y,
				       भग्न to_x, भग्न to_y,
				       व्योम *user_तथ्य HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) तथ्य;

  c->points.push (hb_outline_point_t {control_x, control_y, hb_outline_point_t::type_t::QUADRATIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::QUADRATIC_TO});
}

अटल व्योम
hb_outline_recording_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
				   व्योम *तथ्य,
				   hb_draw_state_t *st,
				   भग्न control1_x, भग्न control1_y,
				   भग्न control2_x, भग्न control2_y,
				   भग्न to_x, भग्न to_y,
				   व्योम *user_तथ्य HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) तथ्य;

  c->points.push (hb_outline_point_t {control1_x, control1_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {control2_x, control2_y, hb_outline_point_t::type_t::CUBIC_TO});
  c->points.push (hb_outline_point_t {to_x, to_y, hb_outline_point_t::type_t::CUBIC_TO});
}

अटल व्योम
hb_outline_recording_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
				     व्योम *तथ्य,
				     hb_draw_state_t *st,
				     व्योम *user_तथ्य HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) तथ्य;

  c->contours.push (c->points.लंबाई);
}

अटल int64_t *
hb_outline_recording_pen_get_budget_remaining (hb_draw_funcs_t *dfuncs HB_UNUSED,
					       व्योम *तथ्य,
					       व्योम *user_तथ्य HB_UNUSED)
{
  hb_outline_t *c = (hb_outline_t *) तथ्य;

  वापस c->budget_remaining;
}

अटल अंतरभूत व्योम free_static_outline_recording_pen_funcs ();

अटल काष्ठा hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  अटल hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_outline_recording_pen_move_to, nullptr, nullptr);
    hb_draw_funcs_set_पंक्ति_to_func (funcs, hb_outline_recording_pen_पंक्ति_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_outline_recording_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_outline_recording_pen_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_outline_recording_pen_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_outline_recording_pen_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    वापस funcs;
  }
} static_outline_recording_pen_funcs;

अटल अंतरभूत
व्योम free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  वापस static_outline_recording_pen_funcs.get_unconst ();
}


#पूर्ण_अगर
