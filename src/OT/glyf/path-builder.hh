#ifndef OT_GLYF_PATH_BUILDER_HH
#define OT_GLYF_PATH_BUILDER_HH


#ଅନ୍ତର୍ଭାଵ "../../hb.hh"


ନାମାକାଶ OT {
ନାମାକାଶ glyf_impl {


ସଂରଚନା path_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  ସଂରଚନା optional_point_t
  {
    optional_point_t () {}
    optional_point_t (ଦଶମିକ x_, ଦଶମିକ y_) : has_data (ସତ୍ୟ), x (x_), y (y_) {}
    operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ has_data; }

    ତାର୍କିକ has_data = ମିଥ୍ୟା;
    ଦଶମିକ x;
    ଦଶମିକ y;

    optional_point_t mid (optional_point_t p)
    { ଫେରନ୍ତୁ optional_point_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
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
  ଶୂନ୍ୟ consume_point (ଅଚଳ contour_point_t &point)
  {
    ତାର୍କିକ is_on_curve = point.flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE;
#ifdef HB_NO_CUBIC_GLYF
    constexpr ତାର୍କିକ is_cubic = ମିଥ୍ୟା;
#ଅନ୍ୟଥା
    ତାର୍କିକ is_cubic = !is_on_curve && (point.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#endif
    optional_point_t p (font->em_fscalef_x (point.x), font->em_fscalef_y (point.y));
    ଯଦି (unlikely (!first_oncurve))
    {
      ଯଦି (is_on_curve)
      {
	first_oncurve = p;
	draw_session->move_to (p.x, p.y);
      }
      ଅନ୍ୟଥା
      {
	ଯଦି (is_cubic && !first_offcurve2)
	{
	  first_offcurve2 = first_offcurve;
	  first_offcurve = p;
	}
	ଅନ୍ୟଥା ଯଦି (first_offcurve)
	{
	  optional_point_t mid = first_offcurve.mid (p);
	  first_oncurve = mid;
	  last_offcurve = p;
	  draw_session->move_to (mid.x, mid.y);
	}
	ଅନ୍ୟଥା
	  first_offcurve = p;
      }
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (last_offcurve)
      {
	ଯଦି (is_on_curve)
	{
	  ଯଦି (last_offcurve2)
	  {
	    draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				    last_offcurve.x, last_offcurve.y,
				    p.x, p.y);
	    last_offcurve2 = optional_point_t ();
	  }
	  ଅନ୍ୟଥା
	    draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				       p.x, p.y);
	  last_offcurve = optional_point_t ();
	}
	ଅନ୍ୟଥା
	{
	  ଯଦି (is_cubic && !last_offcurve2)
	  {
	    last_offcurve2 = last_offcurve;
	    last_offcurve = p;
	  }
	  ଅନ୍ୟଥା
	  {
	    optional_point_t mid = last_offcurve.mid (p);

	    ଯଦି (is_cubic)
	    {
	      draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				      last_offcurve.x, last_offcurve.y,
				      mid.x, mid.y);
	      last_offcurve2 = optional_point_t ();
	    }
	    ଅନ୍ୟଥା
	      draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
					 mid.x, mid.y);
	    last_offcurve = p;
	  }
	}
      }
      ଅନ୍ୟଥା
      {
	ଯଦି (is_on_curve)
	  draw_session->ଧାଡ଼ି_to (p.x, p.y);
	ଅନ୍ୟଥା
	  last_offcurve = p;
      }
    }

  }

  ଶୂନ୍ୟ contour_end ()
  {
    ଯଦି (first_offcurve && last_offcurve)
    {
      optional_point_t mid = last_offcurve.mid (first_offcurve2 ?
						first_offcurve2 :
						first_offcurve);
      ଯଦି (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				mid.x, mid.y);
      ଅନ୍ୟଥା
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   mid.x, mid.y);
      last_offcurve = optional_point_t ();
    }
    /* now check the rest */

    ଯଦି (first_offcurve && first_oncurve)
    {
      ଯଦି (first_offcurve2)
	draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
				first_offcurve.x, first_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      ଅନ୍ୟଥା
	draw_session->quadratic_to (first_offcurve.x, first_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    ଅନ୍ୟଥା ଯଦି (last_offcurve && first_oncurve)
    {
      ଯଦି (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      ଅନ୍ୟଥା
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    ଅନ୍ୟଥା ଯଦି (first_oncurve)
      draw_session->ଧାଡ଼ି_to (first_oncurve.x, first_oncurve.y);
    ଅନ୍ୟଥା ଯଦି (first_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_point_t mid = first_offcurve2.mid (first_offcurve);
      draw_session->move_to (mid.x, mid.y);
      draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
			      first_offcurve.x, first_offcurve.y,
			      mid.x, mid.y);
    }
    ଅନ୍ୟଥା ଯଦି (first_offcurve)
    {
      ଦଶମିକ x = first_offcurve.x, y = first_offcurve.y;
      draw_session->move_to (x, y);
      draw_session->quadratic_to (x, y, x, y);
    }

    /* Getting ready for the next contour */
    first_oncurve = first_offcurve = first_offcurve2 = last_offcurve = last_offcurve2 = optional_point_t ();
    draw_session->close_path ();
  }

  ଶୂନ୍ୟ points_end () {}

  ତାର୍କିକ is_consuming_contour_points () { ଫେରନ୍ତୁ ସତ୍ୟ; }
  contour_point_t *get_phantoms_sink () { ଫେରନ୍ତୁ nullptr; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_PATH_BUILDER_HH */
