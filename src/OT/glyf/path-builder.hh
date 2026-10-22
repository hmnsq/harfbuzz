#جے_بنا_تعریف OT_GLYF_رستہ_BUILDER_HH
#تعریف_کرو OT_GLYF_رستہ_BUILDER_HH


#ضمن "../../hb.hh"


نطاق OT {
نطاق glyf_impl {


بنية رستہ_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  بنية optional_نقطہ_t
  {
    optional_نقطہ_t () {}
    optional_نقطہ_t (عائم x_, عائم y_) : has_انکڑے (سچ), x (x_), y (y_) {}
    عامل منطقي () ثابت { أرجع has_انکڑے; }

    منطقي has_انکڑے = خطأ;
    عائم x;
    عائم y;

    optional_نقطہ_t mid (optional_نقطہ_t p)
    { أرجع optional_نقطہ_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
  } پہلا_oncurve, پہلا_offcurve, پہلا_offcurve2, آخری_offcurve, آخری_offcurve2;

  رستہ_builder_t (hb_font_t *font_, hb_draw_session_t &draw_session_) :
    font (font_), draw_session (&draw_session_) {}

  /* based on https://github.com/RazrFalcon/ttf-parser/blob/4f32821/src/glyf.rs#L287
     See also:
     * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM01/Chap1.html
     * https://stackoverflow.com/a/20772557
     *
     * Cubic support added. */
  HB_ALWAYS_INLINE
  فراغ consume_نقطہ (ثابت contour_نقطہ_t &نقطہ)
  {
    منطقي is_on_ونگ = نقطہ.flag & glyf_impl::SimpleGlyph::FLAG_ON_ونگ;
#جے_تعریف_شدہ HB_NO_CUBIC_GLYF
    constexpr منطقي is_cubic = خطأ;
#وإلا
    منطقي is_cubic = !is_on_ونگ && (نقطہ.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#جے_انت
    optional_نقطہ_t p (font->em_fscalef_x (نقطہ.x), font->em_fscalef_y (نقطہ.y));
    إذا (unlikely (!پہلا_oncurve))
    {
      إذا (is_on_ونگ)
      {
	پہلا_oncurve = p;
	draw_session->move_منزل (p.x, p.y);
      }
      وإلا
      {
	إذا (is_cubic && !پہلا_offcurve2)
	{
	  پہلا_offcurve2 = پہلا_offcurve;
	  پہلا_offcurve = p;
	}
	وإلا إذا (پہلا_offcurve)
	{
	  optional_نقطہ_t mid = پہلا_offcurve.mid (p);
	  پہلا_oncurve = mid;
	  آخری_offcurve = p;
	  draw_session->move_منزل (mid.x, mid.y);
	}
	وإلا
	  پہلا_offcurve = p;
      }
    }
    وإلا
    {
      إذا (آخری_offcurve)
      {
	إذا (is_on_ونگ)
	{
	  إذا (آخری_offcurve2)
	  {
	    draw_session->cubic_منزل (آخری_offcurve2.x, آخری_offcurve2.y,
				    آخری_offcurve.x, آخری_offcurve.y,
				    p.x, p.y);
	    آخری_offcurve2 = optional_نقطہ_t ();
	  }
	  وإلا
	    draw_session->quadratic_منزل (آخری_offcurve.x, آخری_offcurve.y,
				       p.x, p.y);
	  آخری_offcurve = optional_نقطہ_t ();
	}
	وإلا
	{
	  إذا (is_cubic && !آخری_offcurve2)
	  {
	    آخری_offcurve2 = آخری_offcurve;
	    آخری_offcurve = p;
	  }
	  وإلا
	  {
	    optional_نقطہ_t mid = آخری_offcurve.mid (p);

	    إذا (is_cubic)
	    {
	      draw_session->cubic_منزل (آخری_offcurve2.x, آخری_offcurve2.y,
				      آخری_offcurve.x, آخری_offcurve.y,
				      mid.x, mid.y);
	      آخری_offcurve2 = optional_نقطہ_t ();
	    }
	    وإلا
	      draw_session->quadratic_منزل (آخری_offcurve.x, آخری_offcurve.y,
					 mid.x, mid.y);
	    آخری_offcurve = p;
	  }
	}
      }
      وإلا
      {
	إذا (is_on_ونگ)
	  draw_session->سطر_منزل (p.x, p.y);
	وإلا
	  آخری_offcurve = p;
      }
    }

  }

  فراغ contour_سرا ()
  {
    إذا (پہلا_offcurve && آخری_offcurve)
    {
      optional_نقطہ_t mid = آخری_offcurve.mid (پہلا_offcurve2 ?
						پہلا_offcurve2 :
						پہلا_offcurve);
      إذا (آخری_offcurve2)
	draw_session->cubic_منزل (آخری_offcurve2.x, آخری_offcurve2.y,
				آخری_offcurve.x, آخری_offcurve.y,
				mid.x, mid.y);
      وإلا
	draw_session->quadratic_منزل (آخری_offcurve.x, آخری_offcurve.y,
				   mid.x, mid.y);
      آخری_offcurve = optional_نقطہ_t ();
    }
    /* now check the rest */

    إذا (پہلا_offcurve && پہلا_oncurve)
    {
      إذا (پہلا_offcurve2)
	draw_session->cubic_منزل (پہلا_offcurve2.x, پہلا_offcurve2.y,
				پہلا_offcurve.x, پہلا_offcurve.y,
				پہلا_oncurve.x, پہلا_oncurve.y);
      وإلا
	draw_session->quadratic_منزل (پہلا_offcurve.x, پہلا_offcurve.y,
				   پہلا_oncurve.x, پہلا_oncurve.y);
    }
    وإلا إذا (آخری_offcurve && پہلا_oncurve)
    {
      إذا (آخری_offcurve2)
	draw_session->cubic_منزل (آخری_offcurve2.x, آخری_offcurve2.y,
				آخری_offcurve.x, آخری_offcurve.y,
				پہلا_oncurve.x, پہلا_oncurve.y);
      وإلا
	draw_session->quadratic_منزل (آخری_offcurve.x, آخری_offcurve.y,
				   پہلا_oncurve.x, پہلا_oncurve.y);
    }
    وإلا إذا (پہلا_oncurve)
      draw_session->سطر_منزل (پہلا_oncurve.x, پہلا_oncurve.y);
    وإلا إذا (پہلا_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_نقطہ_t mid = پہلا_offcurve2.mid (پہلا_offcurve);
      draw_session->move_منزل (mid.x, mid.y);
      draw_session->cubic_منزل (پہلا_offcurve2.x, پہلا_offcurve2.y,
			      پہلا_offcurve.x, پہلا_offcurve.y,
			      mid.x, mid.y);
    }
    وإلا إذا (پہلا_offcurve)
    {
      عائم x = پہلا_offcurve.x, y = پہلا_offcurve.y;
      draw_session->move_منزل (x, y);
      draw_session->quadratic_منزل (x, y, x, y);
    }

    /* Getting ready for the next contour */
    پہلا_oncurve = پہلا_offcurve = پہلا_offcurve2 = آخری_offcurve = آخری_offcurve2 = optional_نقطہ_t ();
    draw_session->close_رستہ ();
  }

  فراغ points_سرا () {}

  منطقي is_consuming_contour_points () { أرجع سچ; }
  contour_نقطہ_t *get_phantoms_sink () { أرجع خالی_سوچک; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#جے_انت /* OT_GLYF_PATH_BUILDER_HH */
