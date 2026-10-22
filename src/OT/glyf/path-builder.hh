#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_GLYF_ਰਸਤਾ_BUILDER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYF_ਰਸਤਾ_BUILDER_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb.hh"


ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ glyf_impl {


ਬਣਤਰ ਰਸਤਾ_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  ਬਣਤਰ optional_ਬਿੰਦੂ_t
  {
    optional_ਬਿੰਦੂ_t () {}
    optional_ਬਿੰਦੂ_t (ਦਸ਼ਮਲਵ x_, ਦਸ਼ਮਲਵ y_) : has_ਅੰਕੜੇ (ਸੱਚ), x (x_), y (y_) {}
    ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ has_ਅੰਕੜੇ; }

    ਤਾਰਕਿਕ has_ਅੰਕੜੇ = ਝੂਠ;
    ਦਸ਼ਮਲਵ x;
    ਦਸ਼ਮਲਵ y;

    optional_ਬਿੰਦੂ_t mid (optional_ਬਿੰਦੂ_t p)
    { ਵਾਪਸ optional_ਬਿੰਦੂ_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
  } ਪਹਿਲਾ_oncurve, ਪਹਿਲਾ_offcurve, ਪਹਿਲਾ_offcurve2, ਆਖਰੀ_offcurve, ਆਖਰੀ_offcurve2;

  ਰਸਤਾ_builder_t (hb_font_t *font_, hb_draw_session_t &draw_session_) :
    font (font_), draw_session (&draw_session_) {}

  /* based on https://github.com/RazrFalcon/ttf-parser/blob/4f32821/src/glyf.rs#L287
     See also:
     * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM01/Chap1.html
     * https://stackoverflow.com/a/20772557
     *
     * Cubic support added. */
  HB_ALWAYS_INLINE
  ਖਾਲੀ consume_ਬਿੰਦੂ (ਅਚਲ contour_ਬਿੰਦੂ_t &ਬਿੰਦੂ)
  {
    ਤਾਰਕਿਕ is_on_ਵਕਰ = ਬਿੰਦੂ.flag & glyf_impl::SimpleGlyph::FLAG_ON_ਵਕਰ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_CUBIC_GLYF
    constexpr ਤਾਰਕਿਕ is_cubic = ਝੂਠ;
#ਨਹੀਂਤਾਂ
    ਤਾਰਕਿਕ is_cubic = !is_on_ਵਕਰ && (ਬਿੰਦੂ.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#ਜੇ_ਅੰਤ
    optional_ਬਿੰਦੂ_t p (font->em_fscalef_x (ਬਿੰਦੂ.x), font->em_fscalef_y (ਬਿੰਦੂ.y));
    ਜੇ (unlikely (!ਪਹਿਲਾ_oncurve))
    {
      ਜੇ (is_on_ਵਕਰ)
      {
	ਪਹਿਲਾ_oncurve = p;
	draw_session->move_ਮੰਜ਼ਿਲ (p.x, p.y);
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (is_cubic && !ਪਹਿਲਾ_offcurve2)
	{
	  ਪਹਿਲਾ_offcurve2 = ਪਹਿਲਾ_offcurve;
	  ਪਹਿਲਾ_offcurve = p;
	}
	ਨਹੀਂਤਾਂ ਜੇ (ਪਹਿਲਾ_offcurve)
	{
	  optional_ਬਿੰਦੂ_t mid = ਪਹਿਲਾ_offcurve.mid (p);
	  ਪਹਿਲਾ_oncurve = mid;
	  ਆਖਰੀ_offcurve = p;
	  draw_session->move_ਮੰਜ਼ਿਲ (mid.x, mid.y);
	}
	ਨਹੀਂਤਾਂ
	  ਪਹਿਲਾ_offcurve = p;
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (ਆਖਰੀ_offcurve)
      {
	ਜੇ (is_on_ਵਕਰ)
	{
	  ਜੇ (ਆਖਰੀ_offcurve2)
	  {
	    draw_session->cubic_ਮੰਜ਼ਿਲ (ਆਖਰੀ_offcurve2.x, ਆਖਰੀ_offcurve2.y,
				    ਆਖਰੀ_offcurve.x, ਆਖਰੀ_offcurve.y,
				    p.x, p.y);
	    ਆਖਰੀ_offcurve2 = optional_ਬਿੰਦੂ_t ();
	  }
	  ਨਹੀਂਤਾਂ
	    draw_session->quadratic_ਮੰਜ਼ਿਲ (ਆਖਰੀ_offcurve.x, ਆਖਰੀ_offcurve.y,
				       p.x, p.y);
	  ਆਖਰੀ_offcurve = optional_ਬਿੰਦੂ_t ();
	}
	ਨਹੀਂਤਾਂ
	{
	  ਜੇ (is_cubic && !ਆਖਰੀ_offcurve2)
	  {
	    ਆਖਰੀ_offcurve2 = ਆਖਰੀ_offcurve;
	    ਆਖਰੀ_offcurve = p;
	  }
	  ਨਹੀਂਤਾਂ
	  {
	    optional_ਬਿੰਦੂ_t mid = ਆਖਰੀ_offcurve.mid (p);

	    ਜੇ (is_cubic)
	    {
	      draw_session->cubic_ਮੰਜ਼ਿਲ (ਆਖਰੀ_offcurve2.x, ਆਖਰੀ_offcurve2.y,
				      ਆਖਰੀ_offcurve.x, ਆਖਰੀ_offcurve.y,
				      mid.x, mid.y);
	      ਆਖਰੀ_offcurve2 = optional_ਬਿੰਦੂ_t ();
	    }
	    ਨਹੀਂਤਾਂ
	      draw_session->quadratic_ਮੰਜ਼ਿਲ (ਆਖਰੀ_offcurve.x, ਆਖਰੀ_offcurve.y,
					 mid.x, mid.y);
	    ਆਖਰੀ_offcurve = p;
	  }
	}
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (is_on_ਵਕਰ)
	  draw_session->ਸਤਰ_ਮੰਜ਼ਿਲ (p.x, p.y);
	ਨਹੀਂਤਾਂ
	  ਆਖਰੀ_offcurve = p;
      }
    }

  }

  ਖਾਲੀ contour_ਸਿਰਾ ()
  {
    ਜੇ (ਪਹਿਲਾ_offcurve && ਆਖਰੀ_offcurve)
    {
      optional_ਬਿੰਦੂ_t mid = ਆਖਰੀ_offcurve.mid (ਪਹਿਲਾ_offcurve2 ?
						ਪਹਿਲਾ_offcurve2 :
						ਪਹਿਲਾ_offcurve);
      ਜੇ (ਆਖਰੀ_offcurve2)
	draw_session->cubic_ਮੰਜ਼ਿਲ (ਆਖਰੀ_offcurve2.x, ਆਖਰੀ_offcurve2.y,
				ਆਖਰੀ_offcurve.x, ਆਖਰੀ_offcurve.y,
				mid.x, mid.y);
      ਨਹੀਂਤਾਂ
	draw_session->quadratic_ਮੰਜ਼ਿਲ (ਆਖਰੀ_offcurve.x, ਆਖਰੀ_offcurve.y,
				   mid.x, mid.y);
      ਆਖਰੀ_offcurve = optional_ਬਿੰਦੂ_t ();
    }
    /* now check the rest */

    ਜੇ (ਪਹਿਲਾ_offcurve && ਪਹਿਲਾ_oncurve)
    {
      ਜੇ (ਪਹਿਲਾ_offcurve2)
	draw_session->cubic_ਮੰਜ਼ਿਲ (ਪਹਿਲਾ_offcurve2.x, ਪਹਿਲਾ_offcurve2.y,
				ਪਹਿਲਾ_offcurve.x, ਪਹਿਲਾ_offcurve.y,
				ਪਹਿਲਾ_oncurve.x, ਪਹਿਲਾ_oncurve.y);
      ਨਹੀਂਤਾਂ
	draw_session->quadratic_ਮੰਜ਼ਿਲ (ਪਹਿਲਾ_offcurve.x, ਪਹਿਲਾ_offcurve.y,
				   ਪਹਿਲਾ_oncurve.x, ਪਹਿਲਾ_oncurve.y);
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਆਖਰੀ_offcurve && ਪਹਿਲਾ_oncurve)
    {
      ਜੇ (ਆਖਰੀ_offcurve2)
	draw_session->cubic_ਮੰਜ਼ਿਲ (ਆਖਰੀ_offcurve2.x, ਆਖਰੀ_offcurve2.y,
				ਆਖਰੀ_offcurve.x, ਆਖਰੀ_offcurve.y,
				ਪਹਿਲਾ_oncurve.x, ਪਹਿਲਾ_oncurve.y);
      ਨਹੀਂਤਾਂ
	draw_session->quadratic_ਮੰਜ਼ਿਲ (ਆਖਰੀ_offcurve.x, ਆਖਰੀ_offcurve.y,
				   ਪਹਿਲਾ_oncurve.x, ਪਹਿਲਾ_oncurve.y);
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਪਹਿਲਾ_oncurve)
      draw_session->ਸਤਰ_ਮੰਜ਼ਿਲ (ਪਹਿਲਾ_oncurve.x, ਪਹਿਲਾ_oncurve.y);
    ਨਹੀਂਤਾਂ ਜੇ (ਪਹਿਲਾ_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_ਬਿੰਦੂ_t mid = ਪਹਿਲਾ_offcurve2.mid (ਪਹਿਲਾ_offcurve);
      draw_session->move_ਮੰਜ਼ਿਲ (mid.x, mid.y);
      draw_session->cubic_ਮੰਜ਼ਿਲ (ਪਹਿਲਾ_offcurve2.x, ਪਹਿਲਾ_offcurve2.y,
			      ਪਹਿਲਾ_offcurve.x, ਪਹਿਲਾ_offcurve.y,
			      mid.x, mid.y);
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਪਹਿਲਾ_offcurve)
    {
      ਦਸ਼ਮਲਵ x = ਪਹਿਲਾ_offcurve.x, y = ਪਹਿਲਾ_offcurve.y;
      draw_session->move_ਮੰਜ਼ਿਲ (x, y);
      draw_session->quadratic_ਮੰਜ਼ਿਲ (x, y, x, y);
    }

    /* Getting ready for the next contour */
    ਪਹਿਲਾ_oncurve = ਪਹਿਲਾ_offcurve = ਪਹਿਲਾ_offcurve2 = ਆਖਰੀ_offcurve = ਆਖਰੀ_offcurve2 = optional_ਬਿੰਦੂ_t ();
    draw_session->close_ਰਸਤਾ ();
  }

  ਖਾਲੀ points_ਸਿਰਾ () {}

  ਤਾਰਕਿਕ is_consuming_contour_points () { ਵਾਪਸ ਸੱਚ; }
  contour_ਬਿੰਦੂ_t *get_phantoms_sink () { ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_GLYF_PATH_BUILDER_HH */
