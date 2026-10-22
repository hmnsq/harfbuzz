#جيڪڏهن_وصف_نه_ٿيل OT_GLYF_رستو_BUILDER_HH
#وصف_ڪريو OT_GLYF_رستو_BUILDER_HH


#ضمن "../../hb.hh"


نطاق OT {
نطاق glyf_impl {


بنية رستو_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  بنية optional_نقطو_t
  {
    optional_نقطو_t () {}
    optional_نقطو_t (عائم x_, عائم y_) : has_معلومات (سچ), x (x_), y (y_) {}
    عامل منطقي () ثابت { أرجع has_معلومات; }

    منطقي has_معلومات = خطأ;
    عائم x;
    عائم y;

    optional_نقطو_t mid (optional_نقطو_t p)
    { أرجع optional_نقطو_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
  } پهريون_oncurve, پهريون_offcurve, پهريون_offcurve2, آخري_offcurve, آخري_offcurve2;

  رستو_builder_t (hb_font_t *font_, hb_draw_session_t &draw_session_) :
    font (font_), draw_session (&draw_session_) {}

  /* based on https://github.com/RazrFalcon/ttf-parser/blob/4f32821/src/glyf.rs#L287
     See also:
     * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM01/Chap1.html
     * https://stackoverflow.com/a/20772557
     *
     * Cubic support added. */
  HB_ALWAYS_INLINE
  فراغ consume_نقطو (ثابت contour_نقطو_t &نقطو)
  {
    منطقي is_on_وڪر = نقطو.flag & glyf_impl::SimpleGlyph::FLAG_ON_وڪر;
#جيڪڏهن_وصف_ٿيل HB_NO_CUBIC_GLYF
    constexpr منطقي is_cubic = خطأ;
#وإلا
    منطقي is_cubic = !is_on_وڪر && (نقطو.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#جيڪڏهن_پورو
    optional_نقطو_t p (font->em_fscalef_x (نقطو.x), font->em_fscalef_y (نقطو.y));
    إذا (unlikely (!پهريون_oncurve))
    {
      إذا (is_on_وڪر)
      {
	پهريون_oncurve = p;
	draw_session->move_منزل (p.x, p.y);
      }
      وإلا
      {
	إذا (is_cubic && !پهريون_offcurve2)
	{
	  پهريون_offcurve2 = پهريون_offcurve;
	  پهريون_offcurve = p;
	}
	وإلا إذا (پهريون_offcurve)
	{
	  optional_نقطو_t mid = پهريون_offcurve.mid (p);
	  پهريون_oncurve = mid;
	  آخري_offcurve = p;
	  draw_session->move_منزل (mid.x, mid.y);
	}
	وإلا
	  پهريون_offcurve = p;
      }
    }
    وإلا
    {
      إذا (آخري_offcurve)
      {
	إذا (is_on_وڪر)
	{
	  إذا (آخري_offcurve2)
	  {
	    draw_session->cubic_منزل (آخري_offcurve2.x, آخري_offcurve2.y,
				    آخري_offcurve.x, آخري_offcurve.y,
				    p.x, p.y);
	    آخري_offcurve2 = optional_نقطو_t ();
	  }
	  وإلا
	    draw_session->quadratic_منزل (آخري_offcurve.x, آخري_offcurve.y,
				       p.x, p.y);
	  آخري_offcurve = optional_نقطو_t ();
	}
	وإلا
	{
	  إذا (is_cubic && !آخري_offcurve2)
	  {
	    آخري_offcurve2 = آخري_offcurve;
	    آخري_offcurve = p;
	  }
	  وإلا
	  {
	    optional_نقطو_t mid = آخري_offcurve.mid (p);

	    إذا (is_cubic)
	    {
	      draw_session->cubic_منزل (آخري_offcurve2.x, آخري_offcurve2.y,
				      آخري_offcurve.x, آخري_offcurve.y,
				      mid.x, mid.y);
	      آخري_offcurve2 = optional_نقطو_t ();
	    }
	    وإلا
	      draw_session->quadratic_منزل (آخري_offcurve.x, آخري_offcurve.y,
					 mid.x, mid.y);
	    آخري_offcurve = p;
	  }
	}
      }
      وإلا
      {
	إذا (is_on_وڪر)
	  draw_session->سٽ_منزل (p.x, p.y);
	وإلا
	  آخري_offcurve = p;
      }
    }

  }

  فراغ contour_پڇاڙي ()
  {
    إذا (پهريون_offcurve && آخري_offcurve)
    {
      optional_نقطو_t mid = آخري_offcurve.mid (پهريون_offcurve2 ?
						پهريون_offcurve2 :
						پهريون_offcurve);
      إذا (آخري_offcurve2)
	draw_session->cubic_منزل (آخري_offcurve2.x, آخري_offcurve2.y,
				آخري_offcurve.x, آخري_offcurve.y,
				mid.x, mid.y);
      وإلا
	draw_session->quadratic_منزل (آخري_offcurve.x, آخري_offcurve.y,
				   mid.x, mid.y);
      آخري_offcurve = optional_نقطو_t ();
    }
    /* now check the rest */

    إذا (پهريون_offcurve && پهريون_oncurve)
    {
      إذا (پهريون_offcurve2)
	draw_session->cubic_منزل (پهريون_offcurve2.x, پهريون_offcurve2.y,
				پهريون_offcurve.x, پهريون_offcurve.y,
				پهريون_oncurve.x, پهريون_oncurve.y);
      وإلا
	draw_session->quadratic_منزل (پهريون_offcurve.x, پهريون_offcurve.y,
				   پهريون_oncurve.x, پهريون_oncurve.y);
    }
    وإلا إذا (آخري_offcurve && پهريون_oncurve)
    {
      إذا (آخري_offcurve2)
	draw_session->cubic_منزل (آخري_offcurve2.x, آخري_offcurve2.y,
				آخري_offcurve.x, آخري_offcurve.y,
				پهريون_oncurve.x, پهريون_oncurve.y);
      وإلا
	draw_session->quadratic_منزل (آخري_offcurve.x, آخري_offcurve.y,
				   پهريون_oncurve.x, پهريون_oncurve.y);
    }
    وإلا إذا (پهريون_oncurve)
      draw_session->سٽ_منزل (پهريون_oncurve.x, پهريون_oncurve.y);
    وإلا إذا (پهريون_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_نقطو_t mid = پهريون_offcurve2.mid (پهريون_offcurve);
      draw_session->move_منزل (mid.x, mid.y);
      draw_session->cubic_منزل (پهريون_offcurve2.x, پهريون_offcurve2.y,
			      پهريون_offcurve.x, پهريون_offcurve.y,
			      mid.x, mid.y);
    }
    وإلا إذا (پهريون_offcurve)
    {
      عائم x = پهريون_offcurve.x, y = پهريون_offcurve.y;
      draw_session->move_منزل (x, y);
      draw_session->quadratic_منزل (x, y, x, y);
    }

    /* Getting ready for the next contour */
    پهريون_oncurve = پهريون_offcurve = پهريون_offcurve2 = آخري_offcurve = آخري_offcurve2 = optional_نقطو_t ();
    draw_session->close_رستو ();
  }

  فراغ points_پڇاڙي () {}

  منطقي is_consuming_contour_points () { أرجع سچ; }
  contour_نقطو_t *get_phantoms_sink () { أرجع خالي_اشارو; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#جيڪڏهن_پورو /* OT_GLYF_PATH_BUILDER_HH */
