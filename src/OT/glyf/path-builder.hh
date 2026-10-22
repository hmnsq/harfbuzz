#ifndef OT_GLYF_PATH_BUILDER_HH
#define OT_GLYF_PATH_BUILDER_HH


#অন্তর্ভাৱ "../../hb.hh"


নামাকাশ OT {
নামাকাশ glyf_impl {


সংরচনা path_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  সংরচনা optional_point_t
  {
    optional_point_t () {}
    optional_point_t (ভিন্ন x_, ভিন্ন y_) : has_data (সঁচা), x (x_), y (y_) {}
    operator তার্কিক () ধ্রুৱ { ঘূৰাই has_data; }

    তার্কিক has_data = মিছা;
    ভিন্ন x;
    ভিন্ন y;

    optional_point_t mid (optional_point_t p)
    { ঘূৰাই optional_point_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
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
  শূন্য consume_point (ধ্রুৱ contour_point_t &point)
  {
    তার্কিক is_on_curve = point.flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE;
#ifdef HB_NO_CUBIC_GLYF
    constexpr তার্কিক is_cubic = মিছা;
#অন্যথা
    তার্কিক is_cubic = !is_on_curve && (point.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#endif
    optional_point_t p (font->em_fscalef_x (point.x), font->em_fscalef_y (point.y));
    যদি (unlikely (!first_oncurve))
    {
      যদি (is_on_curve)
      {
	first_oncurve = p;
	draw_session->move_to (p.x, p.y);
      }
      অন্যথা
      {
	যদি (is_cubic && !first_offcurve2)
	{
	  first_offcurve2 = first_offcurve;
	  first_offcurve = p;
	}
	অন্যথা যদি (first_offcurve)
	{
	  optional_point_t mid = first_offcurve.mid (p);
	  first_oncurve = mid;
	  last_offcurve = p;
	  draw_session->move_to (mid.x, mid.y);
	}
	অন্যথা
	  first_offcurve = p;
      }
    }
    অন্যথা
    {
      যদি (last_offcurve)
      {
	যদি (is_on_curve)
	{
	  যদি (last_offcurve2)
	  {
	    draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				    last_offcurve.x, last_offcurve.y,
				    p.x, p.y);
	    last_offcurve2 = optional_point_t ();
	  }
	  অন্যথা
	    draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				       p.x, p.y);
	  last_offcurve = optional_point_t ();
	}
	অন্যথা
	{
	  যদি (is_cubic && !last_offcurve2)
	  {
	    last_offcurve2 = last_offcurve;
	    last_offcurve = p;
	  }
	  অন্যথা
	  {
	    optional_point_t mid = last_offcurve.mid (p);

	    যদি (is_cubic)
	    {
	      draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				      last_offcurve.x, last_offcurve.y,
				      mid.x, mid.y);
	      last_offcurve2 = optional_point_t ();
	    }
	    অন্যথা
	      draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
					 mid.x, mid.y);
	    last_offcurve = p;
	  }
	}
      }
      অন্যথা
      {
	যদি (is_on_curve)
	  draw_session->সাৰি_to (p.x, p.y);
	অন্যথা
	  last_offcurve = p;
      }
    }

  }

  শূন্য contour_end ()
  {
    যদি (first_offcurve && last_offcurve)
    {
      optional_point_t mid = last_offcurve.mid (first_offcurve2 ?
						first_offcurve2 :
						first_offcurve);
      যদি (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				mid.x, mid.y);
      অন্যথা
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   mid.x, mid.y);
      last_offcurve = optional_point_t ();
    }
    /* now check the rest */

    যদি (first_offcurve && first_oncurve)
    {
      যদি (first_offcurve2)
	draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
				first_offcurve.x, first_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      অন্যথা
	draw_session->quadratic_to (first_offcurve.x, first_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    অন্যথা যদি (last_offcurve && first_oncurve)
    {
      যদি (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      অন্যথা
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    অন্যথা যদি (first_oncurve)
      draw_session->সাৰি_to (first_oncurve.x, first_oncurve.y);
    অন্যথা যদি (first_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_point_t mid = first_offcurve2.mid (first_offcurve);
      draw_session->move_to (mid.x, mid.y);
      draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
			      first_offcurve.x, first_offcurve.y,
			      mid.x, mid.y);
    }
    অন্যথা যদি (first_offcurve)
    {
      ভিন্ন x = first_offcurve.x, y = first_offcurve.y;
      draw_session->move_to (x, y);
      draw_session->quadratic_to (x, y, x, y);
    }

    /* Getting ready for the next contour */
    first_oncurve = first_offcurve = first_offcurve2 = last_offcurve = last_offcurve2 = optional_point_t ();
    draw_session->close_path ();
  }

  শূন্য points_end () {}

  তার্কিক is_consuming_contour_points () { ঘূৰাই সঁচা; }
  contour_point_t *get_phantoms_sink () { ঘূৰাই nullptr; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_PATH_BUILDER_HH */
