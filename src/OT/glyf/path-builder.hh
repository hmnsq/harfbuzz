#ifndef OT_GLYF_PATH_BUILDER_HH
#define OT_GLYF_PATH_BUILDER_HH


#شامل "../../hb.hh"


فضاینام OT {
فضاینام glyf_impl {


ساختار path_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  ساختار optional_point_t
  {
    optional_point_t () {}
    optional_point_t (اعشاری x_, اعشاری y_) : has_data (درست), x (x_), y (y_) {}
    operator منطقی () ثابت { بازگشت has_data; }

    منطقی has_data = نادرست;
    اعشاری x;
    اعشاری y;

    optional_point_t mid (optional_point_t p)
    { بازگشت optional_point_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
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
  تهی consume_point (ثابت contour_point_t &point)
  {
    منطقی is_on_curve = point.flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE;
#ifdef HB_NO_CUBIC_GLYF
    constexpr منطقی is_cubic = نادرست;
#وگرنه
    منطقی is_cubic = !is_on_curve && (point.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#endif
    optional_point_t p (font->em_fscalef_x (point.x), font->em_fscalef_y (point.y));
    اگر (unlikely (!first_oncurve))
    {
      اگر (is_on_curve)
      {
	first_oncurve = p;
	draw_session->move_to (p.x, p.y);
      }
      وگرنه
      {
	اگر (is_cubic && !first_offcurve2)
	{
	  first_offcurve2 = first_offcurve;
	  first_offcurve = p;
	}
	وگرنه اگر (first_offcurve)
	{
	  optional_point_t mid = first_offcurve.mid (p);
	  first_oncurve = mid;
	  last_offcurve = p;
	  draw_session->move_to (mid.x, mid.y);
	}
	وگرنه
	  first_offcurve = p;
      }
    }
    وگرنه
    {
      اگر (last_offcurve)
      {
	اگر (is_on_curve)
	{
	  اگر (last_offcurve2)
	  {
	    draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				    last_offcurve.x, last_offcurve.y,
				    p.x, p.y);
	    last_offcurve2 = optional_point_t ();
	  }
	  وگرنه
	    draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				       p.x, p.y);
	  last_offcurve = optional_point_t ();
	}
	وگرنه
	{
	  اگر (is_cubic && !last_offcurve2)
	  {
	    last_offcurve2 = last_offcurve;
	    last_offcurve = p;
	  }
	  وگرنه
	  {
	    optional_point_t mid = last_offcurve.mid (p);

	    اگر (is_cubic)
	    {
	      draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				      last_offcurve.x, last_offcurve.y,
				      mid.x, mid.y);
	      last_offcurve2 = optional_point_t ();
	    }
	    وگرنه
	      draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
					 mid.x, mid.y);
	    last_offcurve = p;
	  }
	}
      }
      وگرنه
      {
	اگر (is_on_curve)
	  draw_session->سطر_to (p.x, p.y);
	وگرنه
	  last_offcurve = p;
      }
    }

  }

  تهی contour_end ()
  {
    اگر (first_offcurve && last_offcurve)
    {
      optional_point_t mid = last_offcurve.mid (first_offcurve2 ?
						first_offcurve2 :
						first_offcurve);
      اگر (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				mid.x, mid.y);
      وگرنه
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   mid.x, mid.y);
      last_offcurve = optional_point_t ();
    }
    /* now check the rest */

    اگر (first_offcurve && first_oncurve)
    {
      اگر (first_offcurve2)
	draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
				first_offcurve.x, first_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      وگرنه
	draw_session->quadratic_to (first_offcurve.x, first_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    وگرنه اگر (last_offcurve && first_oncurve)
    {
      اگر (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      وگرنه
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    وگرنه اگر (first_oncurve)
      draw_session->سطر_to (first_oncurve.x, first_oncurve.y);
    وگرنه اگر (first_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_point_t mid = first_offcurve2.mid (first_offcurve);
      draw_session->move_to (mid.x, mid.y);
      draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
			      first_offcurve.x, first_offcurve.y,
			      mid.x, mid.y);
    }
    وگرنه اگر (first_offcurve)
    {
      اعشاری x = first_offcurve.x, y = first_offcurve.y;
      draw_session->move_to (x, y);
      draw_session->quadratic_to (x, y, x, y);
    }

    /* Getting ready for the next contour */
    first_oncurve = first_offcurve = first_offcurve2 = last_offcurve = last_offcurve2 = optional_point_t ();
    draw_session->close_path ();
  }

  تهی points_end () {}

  منطقی is_consuming_contour_points () { بازگشت درست; }
  contour_point_t *get_phantoms_sink () { بازگشت nullptr; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_PATH_BUILDER_HH */
