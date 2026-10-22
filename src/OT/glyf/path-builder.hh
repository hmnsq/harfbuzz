#اگر_تعریف_نشده OT_GLYF_مسیر_BUILDER_HH
#تعریف_کن OT_GLYF_مسیر_BUILDER_HH


#ضمن "../../hb.hh"


نطاق OT {
نطاق glyf_impl {


بنية مسیر_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  بنية optional_نقطه_t
  {
    optional_نقطه_t () {}
    optional_نقطه_t (عائم x_, عائم y_) : has_داده (درست), x (x_), y (y_) {}
    عملگر منطقي () ثابت { أرجع has_داده; }

    منطقي has_داده = خطأ;
    عائم x;
    عائم y;

    optional_نقطه_t mid (optional_نقطه_t p)
    { أرجع optional_نقطه_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
  } اولی_oncurve, اولی_offcurve, اولی_offcurve2, آخرین_offcurve, آخرین_offcurve2;

  مسیر_builder_t (hb_font_t *font_, hb_draw_session_t &draw_session_) :
    font (font_), draw_session (&draw_session_) {}

  /* based on https://github.com/RazrFalcon/ttf-parser/blob/4f32821/src/glyf.rs#L287
     See also:
     * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM01/Chap1.html
     * https://stackoverflow.com/a/20772557
     *
     * Cubic support added. */
  HB_ALWAYS_INLINE
  فراغ consume_نقطه (ثابت contour_نقطه_t &نقطه)
  {
    منطقي is_on_منحنی = نقطه.flag & glyf_impl::SimpleGlyph::FLAG_ON_منحنی;
#اگر_تعریف_شده HB_NO_CUBIC_GLYF
    constexpr منطقي is_cubic = خطأ;
#وإلا
    منطقي is_cubic = !is_on_منحنی && (نقطه.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#پایان_اگر
    optional_نقطه_t p (font->em_fscalef_x (نقطه.x), font->em_fscalef_y (نقطه.y));
    إذا (unlikely (!اولی_oncurve))
    {
      إذا (is_on_منحنی)
      {
	اولی_oncurve = p;
	draw_session->move_مقصد (p.x, p.y);
      }
      وإلا
      {
	إذا (is_cubic && !اولی_offcurve2)
	{
	  اولی_offcurve2 = اولی_offcurve;
	  اولی_offcurve = p;
	}
	وإلا إذا (اولی_offcurve)
	{
	  optional_نقطه_t mid = اولی_offcurve.mid (p);
	  اولی_oncurve = mid;
	  آخرین_offcurve = p;
	  draw_session->move_مقصد (mid.x, mid.y);
	}
	وإلا
	  اولی_offcurve = p;
      }
    }
    وإلا
    {
      إذا (آخرین_offcurve)
      {
	إذا (is_on_منحنی)
	{
	  إذا (آخرین_offcurve2)
	  {
	    draw_session->cubic_مقصد (آخرین_offcurve2.x, آخرین_offcurve2.y,
				    آخرین_offcurve.x, آخرین_offcurve.y,
				    p.x, p.y);
	    آخرین_offcurve2 = optional_نقطه_t ();
	  }
	  وإلا
	    draw_session->quadratic_مقصد (آخرین_offcurve.x, آخرین_offcurve.y,
				       p.x, p.y);
	  آخرین_offcurve = optional_نقطه_t ();
	}
	وإلا
	{
	  إذا (is_cubic && !آخرین_offcurve2)
	  {
	    آخرین_offcurve2 = آخرین_offcurve;
	    آخرین_offcurve = p;
	  }
	  وإلا
	  {
	    optional_نقطه_t mid = آخرین_offcurve.mid (p);

	    إذا (is_cubic)
	    {
	      draw_session->cubic_مقصد (آخرین_offcurve2.x, آخرین_offcurve2.y,
				      آخرین_offcurve.x, آخرین_offcurve.y,
				      mid.x, mid.y);
	      آخرین_offcurve2 = optional_نقطه_t ();
	    }
	    وإلا
	      draw_session->quadratic_مقصد (آخرین_offcurve.x, آخرین_offcurve.y,
					 mid.x, mid.y);
	    آخرین_offcurve = p;
	  }
	}
      }
      وإلا
      {
	إذا (is_on_منحنی)
	  draw_session->سطر_مقصد (p.x, p.y);
	وإلا
	  آخرین_offcurve = p;
      }
    }

  }

  فراغ contour_انتها ()
  {
    إذا (اولی_offcurve && آخرین_offcurve)
    {
      optional_نقطه_t mid = آخرین_offcurve.mid (اولی_offcurve2 ?
						اولی_offcurve2 :
						اولی_offcurve);
      إذا (آخرین_offcurve2)
	draw_session->cubic_مقصد (آخرین_offcurve2.x, آخرین_offcurve2.y,
				آخرین_offcurve.x, آخرین_offcurve.y,
				mid.x, mid.y);
      وإلا
	draw_session->quadratic_مقصد (آخرین_offcurve.x, آخرین_offcurve.y,
				   mid.x, mid.y);
      آخرین_offcurve = optional_نقطه_t ();
    }
    /* now check the rest */

    إذا (اولی_offcurve && اولی_oncurve)
    {
      إذا (اولی_offcurve2)
	draw_session->cubic_مقصد (اولی_offcurve2.x, اولی_offcurve2.y,
				اولی_offcurve.x, اولی_offcurve.y,
				اولی_oncurve.x, اولی_oncurve.y);
      وإلا
	draw_session->quadratic_مقصد (اولی_offcurve.x, اولی_offcurve.y,
				   اولی_oncurve.x, اولی_oncurve.y);
    }
    وإلا إذا (آخرین_offcurve && اولی_oncurve)
    {
      إذا (آخرین_offcurve2)
	draw_session->cubic_مقصد (آخرین_offcurve2.x, آخرین_offcurve2.y,
				آخرین_offcurve.x, آخرین_offcurve.y,
				اولی_oncurve.x, اولی_oncurve.y);
      وإلا
	draw_session->quadratic_مقصد (آخرین_offcurve.x, آخرین_offcurve.y,
				   اولی_oncurve.x, اولی_oncurve.y);
    }
    وإلا إذا (اولی_oncurve)
      draw_session->سطر_مقصد (اولی_oncurve.x, اولی_oncurve.y);
    وإلا إذا (اولی_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_نقطه_t mid = اولی_offcurve2.mid (اولی_offcurve);
      draw_session->move_مقصد (mid.x, mid.y);
      draw_session->cubic_مقصد (اولی_offcurve2.x, اولی_offcurve2.y,
			      اولی_offcurve.x, اولی_offcurve.y,
			      mid.x, mid.y);
    }
    وإلا إذا (اولی_offcurve)
    {
      عائم x = اولی_offcurve.x, y = اولی_offcurve.y;
      draw_session->move_مقصد (x, y);
      draw_session->quadratic_مقصد (x, y, x, y);
    }

    /* Getting ready for the next contour */
    اولی_oncurve = اولی_offcurve = اولی_offcurve2 = آخرین_offcurve = آخرین_offcurve2 = optional_نقطه_t ();
    draw_session->close_مسیر ();
  }

  فراغ points_انتها () {}

  منطقي is_consuming_contour_points () { أرجع درست; }
  contour_نقطه_t *get_phantoms_sink () { أرجع اشاره_تهی; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#پایان_اگر /* OT_GLYF_PATH_BUILDER_HH */
