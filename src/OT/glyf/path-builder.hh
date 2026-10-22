#اگر_غیر_تعریف_شدہ OT_GLYF_راستہ_BUILDER_HH
#تعریف_کرو OT_GLYF_راستہ_BUILDER_HH


#شامل "../../hb.hh"


نامخلا OT {
نامخلا glyf_impl {


ساخت راستہ_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  ساخت optional_نقطہ_t
  {
    optional_نقطہ_t () {}
    optional_نقطہ_t (اعشاری x_, اعشاری y_) : has_ڈیٹا (سچ), x (x_), y (y_) {}
    عامل منطقی () ثابت { واپس has_ڈیٹا; }

    منطقی has_ڈیٹا = جھوٹ;
    اعشاری x;
    اعشاری y;

    optional_نقطہ_t mid (optional_نقطہ_t p)
    { واپس optional_نقطہ_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
  } پہلا_oncurve, پہلا_offcurve, پہلا_offcurve2, آخری_offcurve, آخری_offcurve2;

  راستہ_builder_t (hb_font_t *font_, hb_draw_session_t &draw_session_) :
    font (font_), draw_session (&draw_session_) {}

  /* based on https://github.com/RazrFalcon/ttf-parser/blob/4f32821/src/glyf.rs#L287
     See also:
     * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM01/Chap1.html
     * https://stackoverflow.com/a/20772557
     *
     * Cubic support added. */
  HB_ALWAYS_INLINE
  خالی consume_نقطہ (ثابت contour_نقطہ_t &نقطہ)
  {
    منطقی is_on_خم = نقطہ.flag & glyf_impl::SimpleGlyph::FLAG_ON_خم;
#اگر_تعریف_شدہ HB_NO_CUBIC_GLYF
    constexpr منطقی is_cubic = جھوٹ;
#ورنہ
    منطقی is_cubic = !is_on_خم && (نقطہ.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#اگر_ختم
    optional_نقطہ_t p (font->em_fscalef_x (نقطہ.x), font->em_fscalef_y (نقطہ.y));
    اگر (unlikely (!پہلا_oncurve))
    {
      اگر (is_on_خم)
      {
	پہلا_oncurve = p;
	draw_session->move_مقصد (p.x, p.y);
      }
      ورنہ
      {
	اگر (is_cubic && !پہلا_offcurve2)
	{
	  پہلا_offcurve2 = پہلا_offcurve;
	  پہلا_offcurve = p;
	}
	ورنہ اگر (پہلا_offcurve)
	{
	  optional_نقطہ_t mid = پہلا_offcurve.mid (p);
	  پہلا_oncurve = mid;
	  آخری_offcurve = p;
	  draw_session->move_مقصد (mid.x, mid.y);
	}
	ورنہ
	  پہلا_offcurve = p;
      }
    }
    ورنہ
    {
      اگر (آخری_offcurve)
      {
	اگر (is_on_خم)
	{
	  اگر (آخری_offcurve2)
	  {
	    draw_session->cubic_مقصد (آخری_offcurve2.x, آخری_offcurve2.y,
				    آخری_offcurve.x, آخری_offcurve.y,
				    p.x, p.y);
	    آخری_offcurve2 = optional_نقطہ_t ();
	  }
	  ورنہ
	    draw_session->quadratic_مقصد (آخری_offcurve.x, آخری_offcurve.y,
				       p.x, p.y);
	  آخری_offcurve = optional_نقطہ_t ();
	}
	ورنہ
	{
	  اگر (is_cubic && !آخری_offcurve2)
	  {
	    آخری_offcurve2 = آخری_offcurve;
	    آخری_offcurve = p;
	  }
	  ورنہ
	  {
	    optional_نقطہ_t mid = آخری_offcurve.mid (p);

	    اگر (is_cubic)
	    {
	      draw_session->cubic_مقصد (آخری_offcurve2.x, آخری_offcurve2.y,
				      آخری_offcurve.x, آخری_offcurve.y,
				      mid.x, mid.y);
	      آخری_offcurve2 = optional_نقطہ_t ();
	    }
	    ورنہ
	      draw_session->quadratic_مقصد (آخری_offcurve.x, آخری_offcurve.y,
					 mid.x, mid.y);
	    آخری_offcurve = p;
	  }
	}
      }
      ورنہ
      {
	اگر (is_on_خم)
	  draw_session->سطر_مقصد (p.x, p.y);
	ورنہ
	  آخری_offcurve = p;
      }
    }

  }

  خالی contour_انتہا ()
  {
    اگر (پہلا_offcurve && آخری_offcurve)
    {
      optional_نقطہ_t mid = آخری_offcurve.mid (پہلا_offcurve2 ?
						پہلا_offcurve2 :
						پہلا_offcurve);
      اگر (آخری_offcurve2)
	draw_session->cubic_مقصد (آخری_offcurve2.x, آخری_offcurve2.y,
				آخری_offcurve.x, آخری_offcurve.y,
				mid.x, mid.y);
      ورنہ
	draw_session->quadratic_مقصد (آخری_offcurve.x, آخری_offcurve.y,
				   mid.x, mid.y);
      آخری_offcurve = optional_نقطہ_t ();
    }
    /* now check the rest */

    اگر (پہلا_offcurve && پہلا_oncurve)
    {
      اگر (پہلا_offcurve2)
	draw_session->cubic_مقصد (پہلا_offcurve2.x, پہلا_offcurve2.y,
				پہلا_offcurve.x, پہلا_offcurve.y,
				پہلا_oncurve.x, پہلا_oncurve.y);
      ورنہ
	draw_session->quadratic_مقصد (پہلا_offcurve.x, پہلا_offcurve.y,
				   پہلا_oncurve.x, پہلا_oncurve.y);
    }
    ورنہ اگر (آخری_offcurve && پہلا_oncurve)
    {
      اگر (آخری_offcurve2)
	draw_session->cubic_مقصد (آخری_offcurve2.x, آخری_offcurve2.y,
				آخری_offcurve.x, آخری_offcurve.y,
				پہلا_oncurve.x, پہلا_oncurve.y);
      ورنہ
	draw_session->quadratic_مقصد (آخری_offcurve.x, آخری_offcurve.y,
				   پہلا_oncurve.x, پہلا_oncurve.y);
    }
    ورنہ اگر (پہلا_oncurve)
      draw_session->سطر_مقصد (پہلا_oncurve.x, پہلا_oncurve.y);
    ورنہ اگر (پہلا_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_نقطہ_t mid = پہلا_offcurve2.mid (پہلا_offcurve);
      draw_session->move_مقصد (mid.x, mid.y);
      draw_session->cubic_مقصد (پہلا_offcurve2.x, پہلا_offcurve2.y,
			      پہلا_offcurve.x, پہلا_offcurve.y,
			      mid.x, mid.y);
    }
    ورنہ اگر (پہلا_offcurve)
    {
      اعشاری x = پہلا_offcurve.x, y = پہلا_offcurve.y;
      draw_session->move_مقصد (x, y);
      draw_session->quadratic_مقصد (x, y, x, y);
    }

    /* Getting ready for the next contour */
    پہلا_oncurve = پہلا_offcurve = پہلا_offcurve2 = آخری_offcurve = آخری_offcurve2 = optional_نقطہ_t ();
    draw_session->close_راستہ ();
  }

  خالی points_انتہا () {}

  منطقی is_consuming_contour_points () { واپس سچ; }
  contour_نقطہ_t *get_phantoms_sink () { واپس خالی_اشاریہ; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#اگر_ختم /* OT_GLYF_PATH_BUILDER_HH */
