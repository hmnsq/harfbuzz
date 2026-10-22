#ifndef OT_GLYF_PATH_BUILDER_HH
#define OT_GLYF_PATH_BUILDER_HH


#அந்தர்பாவ "../../hb.hh"


நாமாகாஶ OT {
நாமாகாஶ glyf_impl {


கட்டமைப்பு path_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  கட்டமைப்பு optional_point_t
  {
    optional_point_t () {}
    optional_point_t (தசம x_, தசம y_) : has_data (உண்மை), x (x_), y (y_) {}
    operator தருக்க () மாறிலி { திரும்பு has_data; }

    தருக்க has_data = பொய்;
    தசம x;
    தசம y;

    optional_point_t mid (optional_point_t p)
    { திரும்பு optional_point_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
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
  வெறுமை consume_point (மாறிலி contour_point_t &point)
  {
    தருக்க is_on_curve = point.flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE;
#ifdef HB_NO_CUBIC_GLYF
    constexpr தருக்க is_cubic = பொய்;
#இல்லையெனில்
    தருக்க is_cubic = !is_on_curve && (point.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#endif
    optional_point_t p (font->em_fscalef_x (point.x), font->em_fscalef_y (point.y));
    எனில் (unlikely (!first_oncurve))
    {
      எனில் (is_on_curve)
      {
	first_oncurve = p;
	draw_session->move_to (p.x, p.y);
      }
      இல்லையெனில்
      {
	எனில் (is_cubic && !first_offcurve2)
	{
	  first_offcurve2 = first_offcurve;
	  first_offcurve = p;
	}
	இல்லையெனில் எனில் (first_offcurve)
	{
	  optional_point_t mid = first_offcurve.mid (p);
	  first_oncurve = mid;
	  last_offcurve = p;
	  draw_session->move_to (mid.x, mid.y);
	}
	இல்லையெனில்
	  first_offcurve = p;
      }
    }
    இல்லையெனில்
    {
      எனில் (last_offcurve)
      {
	எனில் (is_on_curve)
	{
	  எனில் (last_offcurve2)
	  {
	    draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				    last_offcurve.x, last_offcurve.y,
				    p.x, p.y);
	    last_offcurve2 = optional_point_t ();
	  }
	  இல்லையெனில்
	    draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				       p.x, p.y);
	  last_offcurve = optional_point_t ();
	}
	இல்லையெனில்
	{
	  எனில் (is_cubic && !last_offcurve2)
	  {
	    last_offcurve2 = last_offcurve;
	    last_offcurve = p;
	  }
	  இல்லையெனில்
	  {
	    optional_point_t mid = last_offcurve.mid (p);

	    எனில் (is_cubic)
	    {
	      draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				      last_offcurve.x, last_offcurve.y,
				      mid.x, mid.y);
	      last_offcurve2 = optional_point_t ();
	    }
	    இல்லையெனில்
	      draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
					 mid.x, mid.y);
	    last_offcurve = p;
	  }
	}
      }
      இல்லையெனில்
      {
	எனில் (is_on_curve)
	  draw_session->வரி_to (p.x, p.y);
	இல்லையெனில்
	  last_offcurve = p;
      }
    }

  }

  வெறுமை contour_end ()
  {
    எனில் (first_offcurve && last_offcurve)
    {
      optional_point_t mid = last_offcurve.mid (first_offcurve2 ?
						first_offcurve2 :
						first_offcurve);
      எனில் (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				mid.x, mid.y);
      இல்லையெனில்
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   mid.x, mid.y);
      last_offcurve = optional_point_t ();
    }
    /* now check the rest */

    எனில் (first_offcurve && first_oncurve)
    {
      எனில் (first_offcurve2)
	draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
				first_offcurve.x, first_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      இல்லையெனில்
	draw_session->quadratic_to (first_offcurve.x, first_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    இல்லையெனில் எனில் (last_offcurve && first_oncurve)
    {
      எனில் (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      இல்லையெனில்
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    இல்லையெனில் எனில் (first_oncurve)
      draw_session->வரி_to (first_oncurve.x, first_oncurve.y);
    இல்லையெனில் எனில் (first_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_point_t mid = first_offcurve2.mid (first_offcurve);
      draw_session->move_to (mid.x, mid.y);
      draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
			      first_offcurve.x, first_offcurve.y,
			      mid.x, mid.y);
    }
    இல்லையெனில் எனில் (first_offcurve)
    {
      தசம x = first_offcurve.x, y = first_offcurve.y;
      draw_session->move_to (x, y);
      draw_session->quadratic_to (x, y, x, y);
    }

    /* Getting ready for the next contour */
    first_oncurve = first_offcurve = first_offcurve2 = last_offcurve = last_offcurve2 = optional_point_t ();
    draw_session->close_path ();
  }

  வெறுமை points_end () {}

  தருக்க is_consuming_contour_points () { திரும்பு உண்மை; }
  contour_point_t *get_phantoms_sink () { திரும்பு nullptr; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_PATH_BUILDER_HH */
