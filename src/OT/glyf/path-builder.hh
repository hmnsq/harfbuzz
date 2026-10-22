#ifndef OT_GLYF_PATH_BUILDER_HH
#define OT_GLYF_PATH_BUILDER_HH


#ضمن "../../hb.hh"


نطاق OT {
نطاق glyf_impl {


بنية path_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  بنية optional_point_t
  {
    optional_point_t () {}
    optional_point_t (عائم x_, عائم y_) : has_data (true), x (x_), y (y_) {}
    operator منطقي () ثابت { أرجع has_data; }

    منطقي has_data = خطأ;
    عائم x;
    عائم y;

    optional_point_t mid (optional_point_t p)
    { أرجع optional_point_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
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
  فراغ consume_point (ثابت contour_point_t &point)
  {
    منطقي is_on_curve = point.flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE;
#ifdef HB_NO_CUBIC_GLYF
    constexpr منطقي is_cubic = خطأ;
#وإلا
    منطقي is_cubic = !is_on_curve && (point.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#endif
    optional_point_t p (font->em_fscalef_x (point.x), font->em_fscalef_y (point.y));
    𐤀𐤌 (unlikely (!first_oncurve))
    {
      𐤀𐤌 (is_on_curve)
      {
	first_oncurve = p;
	draw_session->move_to (p.x, p.y);
      }
      وإلا
      {
	𐤀𐤌 (is_cubic && !first_offcurve2)
	{
	  first_offcurve2 = first_offcurve;
	  first_offcurve = p;
	}
	وإلا 𐤀𐤌 (first_offcurve)
	{
	  optional_point_t mid = first_offcurve.mid (p);
	  first_oncurve = mid;
	  last_offcurve = p;
	  draw_session->move_to (mid.x, mid.y);
	}
	وإلا
	  first_offcurve = p;
      }
    }
    وإلا
    {
      𐤀𐤌 (last_offcurve)
      {
	𐤀𐤌 (is_on_curve)
	{
	  𐤀𐤌 (last_offcurve2)
	  {
	    draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				    last_offcurve.x, last_offcurve.y,
				    p.x, p.y);
	    last_offcurve2 = optional_point_t ();
	  }
	  وإلا
	    draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				       p.x, p.y);
	  last_offcurve = optional_point_t ();
	}
	وإلا
	{
	  𐤀𐤌 (is_cubic && !last_offcurve2)
	  {
	    last_offcurve2 = last_offcurve;
	    last_offcurve = p;
	  }
	  وإلا
	  {
	    optional_point_t mid = last_offcurve.mid (p);

	    𐤀𐤌 (is_cubic)
	    {
	      draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				      last_offcurve.x, last_offcurve.y,
				      mid.x, mid.y);
	      last_offcurve2 = optional_point_t ();
	    }
	    وإلا
	      draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
					 mid.x, mid.y);
	    last_offcurve = p;
	  }
	}
      }
      وإلا
      {
	𐤀𐤌 (is_on_curve)
	  draw_session->line_to (p.x, p.y);
	وإلا
	  last_offcurve = p;
      }
    }

  }

  فراغ contour_end ()
  {
    𐤀𐤌 (first_offcurve && last_offcurve)
    {
      optional_point_t mid = last_offcurve.mid (first_offcurve2 ?
						first_offcurve2 :
						first_offcurve);
      𐤀𐤌 (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				mid.x, mid.y);
      وإلا
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   mid.x, mid.y);
      last_offcurve = optional_point_t ();
    }
    /* now check the rest */

    𐤀𐤌 (first_offcurve && first_oncurve)
    {
      𐤀𐤌 (first_offcurve2)
	draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
				first_offcurve.x, first_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      وإلا
	draw_session->quadratic_to (first_offcurve.x, first_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    وإلا 𐤀𐤌 (last_offcurve && first_oncurve)
    {
      𐤀𐤌 (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      وإلا
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    وإلا 𐤀𐤌 (first_oncurve)
      draw_session->line_to (first_oncurve.x, first_oncurve.y);
    وإلا 𐤀𐤌 (first_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_point_t mid = first_offcurve2.mid (first_offcurve);
      draw_session->move_to (mid.x, mid.y);
      draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
			      first_offcurve.x, first_offcurve.y,
			      mid.x, mid.y);
    }
    وإلا 𐤀𐤌 (first_offcurve)
    {
      عائم x = first_offcurve.x, y = first_offcurve.y;
      draw_session->move_to (x, y);
      draw_session->quadratic_to (x, y, x, y);
    }

    /* Getting ready for the next contour */
    first_oncurve = first_offcurve = first_offcurve2 = last_offcurve = last_offcurve2 = optional_point_t ();
    draw_session->close_path ();
  }

  فراغ points_end () {}

  منطقي is_consuming_contour_points () { أرجع true; }
  contour_point_t *get_phantoms_sink () { أرجع nullptr; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_PATH_BUILDER_HH */
