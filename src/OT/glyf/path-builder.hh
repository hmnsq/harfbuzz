#अगर_अघोषित OT_GLYF_PATH_BUILDER_HH
#घोषणा OT_GLYF_PATH_BUILDER_HH


#समावेश "../../hb.hh"


namespace OT {
namespace glyf_impl {


काष्ठा path_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  काष्ठा optional_point_t
  {
    optional_point_t () {}
    optional_point_t (भग्न x_, भग्न y_) : has_तथ्य (true), x (x_), y (y_) {}
    चालक bool () स्थिर { वापस has_तथ्य; }

    bool has_तथ्य = false;
    भग्न x;
    भग्न y;

    optional_point_t mid (optional_point_t p)
    { वापस optional_point_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
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
  व्योम consume_point (स्थिर contour_point_t &point)
  {
    bool is_on_curve = point.flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE;
#अगर_घोषित HB_NO_CUBIC_GLYF
    constexpr bool is_cubic = false;
#अन्यथा
    bool is_cubic = !is_on_curve && (point.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#पूर्ण_अगर
    optional_point_t p (font->em_fscalef_x (point.x), font->em_fscalef_y (point.y));
    अगर (unlikely (!first_oncurve))
    {
      अगर (is_on_curve)
      {
	first_oncurve = p;
	draw_session->move_to (p.x, p.y);
      }
      अन्यथा
      {
	अगर (is_cubic && !first_offcurve2)
	{
	  first_offcurve2 = first_offcurve;
	  first_offcurve = p;
	}
	अन्यथा अगर (first_offcurve)
	{
	  optional_point_t mid = first_offcurve.mid (p);
	  first_oncurve = mid;
	  last_offcurve = p;
	  draw_session->move_to (mid.x, mid.y);
	}
	अन्यथा
	  first_offcurve = p;
      }
    }
    अन्यथा
    {
      अगर (last_offcurve)
      {
	अगर (is_on_curve)
	{
	  अगर (last_offcurve2)
	  {
	    draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				    last_offcurve.x, last_offcurve.y,
				    p.x, p.y);
	    last_offcurve2 = optional_point_t ();
	  }
	  अन्यथा
	    draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				       p.x, p.y);
	  last_offcurve = optional_point_t ();
	}
	अन्यथा
	{
	  अगर (is_cubic && !last_offcurve2)
	  {
	    last_offcurve2 = last_offcurve;
	    last_offcurve = p;
	  }
	  अन्यथा
	  {
	    optional_point_t mid = last_offcurve.mid (p);

	    अगर (is_cubic)
	    {
	      draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				      last_offcurve.x, last_offcurve.y,
				      mid.x, mid.y);
	      last_offcurve2 = optional_point_t ();
	    }
	    अन्यथा
	      draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
					 mid.x, mid.y);
	    last_offcurve = p;
	  }
	}
      }
      अन्यथा
      {
	अगर (is_on_curve)
	  draw_session->पंक्ति_to (p.x, p.y);
	अन्यथा
	  last_offcurve = p;
      }
    }

  }

  व्योम contour_end ()
  {
    अगर (first_offcurve && last_offcurve)
    {
      optional_point_t mid = last_offcurve.mid (first_offcurve2 ?
						first_offcurve2 :
						first_offcurve);
      अगर (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				mid.x, mid.y);
      अन्यथा
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   mid.x, mid.y);
      last_offcurve = optional_point_t ();
    }
    /* now check the rest */

    अगर (first_offcurve && first_oncurve)
    {
      अगर (first_offcurve2)
	draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
				first_offcurve.x, first_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      अन्यथा
	draw_session->quadratic_to (first_offcurve.x, first_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    अन्यथा अगर (last_offcurve && first_oncurve)
    {
      अगर (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      अन्यथा
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    अन्यथा अगर (first_oncurve)
      draw_session->पंक्ति_to (first_oncurve.x, first_oncurve.y);
    अन्यथा अगर (first_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_point_t mid = first_offcurve2.mid (first_offcurve);
      draw_session->move_to (mid.x, mid.y);
      draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
			      first_offcurve.x, first_offcurve.y,
			      mid.x, mid.y);
    }
    अन्यथा अगर (first_offcurve)
    {
      भग्न x = first_offcurve.x, y = first_offcurve.y;
      draw_session->move_to (x, y);
      draw_session->quadratic_to (x, y, x, y);
    }

    /* Getting ready for the next contour */
    first_oncurve = first_offcurve = first_offcurve2 = last_offcurve = last_offcurve2 = optional_point_t ();
    draw_session->close_path ();
  }

  व्योम points_end () {}

  bool is_consuming_contour_points () { वापस true; }
  contour_point_t *get_phantoms_sink () { वापस nullptr; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#पूर्ण_अगर /* OT_GLYF_PATH_BUILDER_HH */
