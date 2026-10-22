#ifndef OT_GLYF_PATH_BUILDER_HH
#define OT_GLYF_PATH_BUILDER_HH


#अन्तर्भाव "../../hb.hh"


नामाकाश OT {
नामाकाश glyf_impl {


संरचना path_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  संरचना optional_point_t
  {
    optional_point_t () {}
    optional_point_t (भिन्न x_, भिन्न y_) : has_data (सच्च), x (x_), y (y_) {}
    operator तार्किक () ध्रुव { प्रत्यागम has_data; }

    तार्किक has_data = मिच्छ;
    भिन्न x;
    भिन्न y;

    optional_point_t mid (optional_point_t p)
    { प्रत्यागम optional_point_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
  } first_oncurve, first_offcurve, first_offcurve2, last_offcurve, last_offcurve2;

  path_builder_t (hb_font_t *font_, hb_draw_session_t &draw_session_) :
    font (font_), draw_session (&draw_session_) {}

  /* based on https://github.com/RazrFalcon/ttf-parser/blob/4f32821/src/glyf.rs#L287
     See also:
     * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM01/Chap1.html
     * https://stackoverflow.com/a/20772557
     *
     * Cubic support added. */
  HB_ALWAYS_INLINE
  शून्य consume_point (ध्रुव contour_point_t &point)
  {
    तार्किक is_on_curve = point.flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE;
#ifdef HB_NO_CUBIC_GLYF
    constexpr तार्किक is_cubic = मिच्छ;
#अण्णहा
    तार्किक is_cubic = !is_on_curve && (point.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#endif
    optional_point_t p (font->em_fscalef_x (point.x), font->em_fscalef_y (point.y));
    जइ (unlikely (!first_oncurve))
    {
      जइ (is_on_curve)
      {
	first_oncurve = p;
	draw_session->move_to (p.x, p.y);
      }
      अण्णहा
      {
	जइ (is_cubic && !first_offcurve2)
	{
	  first_offcurve2 = first_offcurve;
	  first_offcurve = p;
	}
	अण्णहा जइ (first_offcurve)
	{
	  optional_point_t mid = first_offcurve.mid (p);
	  first_oncurve = mid;
	  last_offcurve = p;
	  draw_session->move_to (mid.x, mid.y);
	}
	अण्णहा
	  first_offcurve = p;
      }
    }
    अण्णहा
    {
      जइ (last_offcurve)
      {
	जइ (is_on_curve)
	{
	  जइ (last_offcurve2)
	  {
	    draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				    last_offcurve.x, last_offcurve.y,
				    p.x, p.y);
	    last_offcurve2 = optional_point_t ();
	  }
	  अण्णहा
	    draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				       p.x, p.y);
	  last_offcurve = optional_point_t ();
	}
	अण्णहा
	{
	  जइ (is_cubic && !last_offcurve2)
	  {
	    last_offcurve2 = last_offcurve;
	    last_offcurve = p;
	  }
	  अण्णहा
	  {
	    optional_point_t mid = last_offcurve.mid (p);

	    जइ (is_cubic)
	    {
	      draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				      last_offcurve.x, last_offcurve.y,
				      mid.x, mid.y);
	      last_offcurve2 = optional_point_t ();
	    }
	    अण्णहा
	      draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
					 mid.x, mid.y);
	    last_offcurve = p;
	  }
	}
      }
      अण्णहा
      {
	जइ (is_on_curve)
	  draw_session->पङ्क्तिः_to (p.x, p.y);
	अण्णहा
	  last_offcurve = p;
      }
    }

  }

  शून्य contour_end ()
  {
    जइ (first_offcurve && last_offcurve)
    {
      optional_point_t mid = last_offcurve.mid (first_offcurve2 ?
						first_offcurve2 :
						first_offcurve);
      जइ (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				mid.x, mid.y);
      अण्णहा
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   mid.x, mid.y);
      last_offcurve = optional_point_t ();
    }
    /* now check the rest */

    जइ (first_offcurve && first_oncurve)
    {
      जइ (first_offcurve2)
	draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
				first_offcurve.x, first_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      अण्णहा
	draw_session->quadratic_to (first_offcurve.x, first_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    अण्णहा जइ (last_offcurve && first_oncurve)
    {
      जइ (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      अण्णहा
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    अण्णहा जइ (first_oncurve)
      draw_session->पङ्क्तिः_to (first_oncurve.x, first_oncurve.y);
    अण्णहा जइ (first_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_point_t mid = first_offcurve2.mid (first_offcurve);
      draw_session->move_to (mid.x, mid.y);
      draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
			      first_offcurve.x, first_offcurve.y,
			      mid.x, mid.y);
    }
    अण्णहा जइ (first_offcurve)
    {
      भिन्न x = first_offcurve.x, y = first_offcurve.y;
      draw_session->move_to (x, y);
      draw_session->quadratic_to (x, y, x, y);
    }

    /* Getting ready for the next contour */
    first_oncurve = first_offcurve = first_offcurve2 = last_offcurve = last_offcurve2 = optional_point_t ();
    draw_session->close_path ();
  }

  शून्य points_end () {}

  तार्किक is_consuming_contour_points () { प्रत्यागम सच्च; }
  contour_point_t *get_phantoms_sink () { प्रत्यागम nullptr; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_PATH_BUILDER_HH */
