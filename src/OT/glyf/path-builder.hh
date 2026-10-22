#که_نه_تعریف_شوی OT_GLYF_لار_BUILDER_HH
#تعریف_کړه OT_GLYF_لار_BUILDER_HH


#ضمن "../../hb.hh"


نطاق OT {
نطاق glyf_impl {


بنية لار_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  بنية optional_ټکی_t
  {
    optional_ټکی_t () {}
    optional_ټکی_t (عائم x_, عائم y_) : has_معلومات (ریښتیا), x (x_), y (y_) {}
    چلوونکی منطقي () ثابت { أرجع has_معلومات; }

    منطقي has_معلومات = خطأ;
    عائم x;
    عائم y;

    optional_ټکی_t mid (optional_ټکی_t p)
    { أرجع optional_ټکی_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
  } لومړی_oncurve, لومړی_offcurve, لومړی_offcurve2, وروستی_offcurve, وروستی_offcurve2;

  لار_builder_t (hb_font_t *font_, hb_draw_session_t &draw_session_) :
    font (font_), draw_session (&draw_session_) {}

  /* based on https://github.com/RazrFalcon/ttf-parser/blob/4f32821/src/glyf.rs#L287
     See also:
     * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM01/Chap1.html
     * https://stackoverflow.com/a/20772557
     *
     * Cubic support added. */
  HB_ALWAYS_INLINE
  فراغ consume_ټکی (ثابت contour_ټکی_t &ټکی)
  {
    منطقي is_on_منحني = ټکی.flag & glyf_impl::SimpleGlyph::FLAG_ON_منحني;
#که_تعریف_شوی HB_NO_CUBIC_GLYF
    constexpr منطقي is_cubic = خطأ;
#وإلا
    منطقي is_cubic = !is_on_منحني && (ټکی.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#د_که_پای
    optional_ټکی_t p (font->em_fscalef_x (ټکی.x), font->em_fscalef_y (ټکی.y));
    إذا (unlikely (!لومړی_oncurve))
    {
      إذا (is_on_منحني)
      {
	لومړی_oncurve = p;
	draw_session->move_منزل (p.x, p.y);
      }
      وإلا
      {
	إذا (is_cubic && !لومړی_offcurve2)
	{
	  لومړی_offcurve2 = لومړی_offcurve;
	  لومړی_offcurve = p;
	}
	وإلا إذا (لومړی_offcurve)
	{
	  optional_ټکی_t mid = لومړی_offcurve.mid (p);
	  لومړی_oncurve = mid;
	  وروستی_offcurve = p;
	  draw_session->move_منزل (mid.x, mid.y);
	}
	وإلا
	  لومړی_offcurve = p;
      }
    }
    وإلا
    {
      إذا (وروستی_offcurve)
      {
	إذا (is_on_منحني)
	{
	  إذا (وروستی_offcurve2)
	  {
	    draw_session->cubic_منزل (وروستی_offcurve2.x, وروستی_offcurve2.y,
				    وروستی_offcurve.x, وروستی_offcurve.y,
				    p.x, p.y);
	    وروستی_offcurve2 = optional_ټکی_t ();
	  }
	  وإلا
	    draw_session->quadratic_منزل (وروستی_offcurve.x, وروستی_offcurve.y,
				       p.x, p.y);
	  وروستی_offcurve = optional_ټکی_t ();
	}
	وإلا
	{
	  إذا (is_cubic && !وروستی_offcurve2)
	  {
	    وروستی_offcurve2 = وروستی_offcurve;
	    وروستی_offcurve = p;
	  }
	  وإلا
	  {
	    optional_ټکی_t mid = وروستی_offcurve.mid (p);

	    إذا (is_cubic)
	    {
	      draw_session->cubic_منزل (وروستی_offcurve2.x, وروستی_offcurve2.y,
				      وروستی_offcurve.x, وروستی_offcurve.y,
				      mid.x, mid.y);
	      وروستی_offcurve2 = optional_ټکی_t ();
	    }
	    وإلا
	      draw_session->quadratic_منزل (وروستی_offcurve.x, وروستی_offcurve.y,
					 mid.x, mid.y);
	    وروستی_offcurve = p;
	  }
	}
      }
      وإلا
      {
	إذا (is_on_منحني)
	  draw_session->کرښه_منزل (p.x, p.y);
	وإلا
	  وروستی_offcurve = p;
      }
    }

  }

  فراغ contour_پای_ټکی ()
  {
    إذا (لومړی_offcurve && وروستی_offcurve)
    {
      optional_ټکی_t mid = وروستی_offcurve.mid (لومړی_offcurve2 ?
						لومړی_offcurve2 :
						لومړی_offcurve);
      إذا (وروستی_offcurve2)
	draw_session->cubic_منزل (وروستی_offcurve2.x, وروستی_offcurve2.y,
				وروستی_offcurve.x, وروستی_offcurve.y,
				mid.x, mid.y);
      وإلا
	draw_session->quadratic_منزل (وروستی_offcurve.x, وروستی_offcurve.y,
				   mid.x, mid.y);
      وروستی_offcurve = optional_ټکی_t ();
    }
    /* now check the rest */

    إذا (لومړی_offcurve && لومړی_oncurve)
    {
      إذا (لومړی_offcurve2)
	draw_session->cubic_منزل (لومړی_offcurve2.x, لومړی_offcurve2.y,
				لومړی_offcurve.x, لومړی_offcurve.y,
				لومړی_oncurve.x, لومړی_oncurve.y);
      وإلا
	draw_session->quadratic_منزل (لومړی_offcurve.x, لومړی_offcurve.y,
				   لومړی_oncurve.x, لومړی_oncurve.y);
    }
    وإلا إذا (وروستی_offcurve && لومړی_oncurve)
    {
      إذا (وروستی_offcurve2)
	draw_session->cubic_منزل (وروستی_offcurve2.x, وروستی_offcurve2.y,
				وروستی_offcurve.x, وروستی_offcurve.y,
				لومړی_oncurve.x, لومړی_oncurve.y);
      وإلا
	draw_session->quadratic_منزل (وروستی_offcurve.x, وروستی_offcurve.y,
				   لومړی_oncurve.x, لومړی_oncurve.y);
    }
    وإلا إذا (لومړی_oncurve)
      draw_session->کرښه_منزل (لومړی_oncurve.x, لومړی_oncurve.y);
    وإلا إذا (لومړی_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_ټکی_t mid = لومړی_offcurve2.mid (لومړی_offcurve);
      draw_session->move_منزل (mid.x, mid.y);
      draw_session->cubic_منزل (لومړی_offcurve2.x, لومړی_offcurve2.y,
			      لومړی_offcurve.x, لومړی_offcurve.y,
			      mid.x, mid.y);
    }
    وإلا إذا (لومړی_offcurve)
    {
      عائم x = لومړی_offcurve.x, y = لومړی_offcurve.y;
      draw_session->move_منزل (x, y);
      draw_session->quadratic_منزل (x, y, x, y);
    }

    /* Getting ready for the next contour */
    لومړی_oncurve = لومړی_offcurve = لومړی_offcurve2 = وروستی_offcurve = وروستی_offcurve2 = optional_ټکی_t ();
    draw_session->close_لار ();
  }

  فراغ points_پای_ټکی () {}

  منطقي is_consuming_contour_points () { أرجع ریښتیا; }
  contour_ټکی_t *get_phantoms_sink () { أرجع تش_اشاره; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#د_که_پای /* OT_GLYF_PATH_BUILDER_HH */
