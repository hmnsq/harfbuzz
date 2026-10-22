#నిర్వచితం_కాకపోతే OT_GLYF_మార్గం_BUILDER_HH
#నిర్వచించు OT_GLYF_మార్గం_BUILDER_HH


#అన్తర్భావ "../../hb.hh"


నామాకాశ OT {
నామాకాశ glyf_impl {


నిర్మాణం మార్గం_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  నిర్మాణం optional_బిందువు_t
  {
    optional_బిందువు_t () {}
    optional_బిందువు_t (దశాంశం x_, దశాంశం y_) : has_దత్తాంశం (నిజం), x (x_), y (y_) {}
    క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి has_దత్తాంశం; }

    తార్కిక has_దత్తాంశం = అబద్ధం;
    దశాంశం x;
    దశాంశం y;

    optional_బిందువు_t mid (optional_బిందువు_t p)
    { తిరిగి optional_బిందువు_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
  } మొదటి_oncurve, మొదటి_offcurve, మొదటి_offcurve2, చివరి_offcurve, చివరి_offcurve2;

  మార్గం_builder_t (hb_font_t *font_, hb_draw_session_t &draw_session_) :
    font (font_), draw_session (&draw_session_) {}

  /* based on https://github.com/RazrFalcon/ttf-parser/blob/4f32821/src/glyf.rs#L287
     See also:
     * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM01/Chap1.html
     * https://stackoverflow.com/a/20772557
     *
     * Cubic support added. */
  HB_ALWAYS_INLINE
  శూన్య consume_బిందువు (స్థిరరాశి contour_బిందువు_t &బిందువు)
  {
    తార్కిక is_on_వక్రం = బిందువు.flag & glyf_impl::SimpleGlyph::FLAG_ON_వక్రం;
#నిర్వచితమైతే HB_NO_CUBIC_GLYF
    constexpr తార్కిక is_cubic = అబద్ధం;
#లేకపోతే
    తార్కిక is_cubic = !is_on_వక్రం && (బిందువు.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#అయితే_ముగింపు
    optional_బిందువు_t p (font->em_fscalef_x (బిందువు.x), font->em_fscalef_y (బిందువు.y));
    ఒకవేళ (unlikely (!మొదటి_oncurve))
    {
      ఒకవేళ (is_on_వక్రం)
      {
	మొదటి_oncurve = p;
	draw_session->move_గమ్యం (p.x, p.y);
      }
      లేకపోతే
      {
	ఒకవేళ (is_cubic && !మొదటి_offcurve2)
	{
	  మొదటి_offcurve2 = మొదటి_offcurve;
	  మొదటి_offcurve = p;
	}
	లేకపోతే ఒకవేళ (మొదటి_offcurve)
	{
	  optional_బిందువు_t mid = మొదటి_offcurve.mid (p);
	  మొదటి_oncurve = mid;
	  చివరి_offcurve = p;
	  draw_session->move_గమ్యం (mid.x, mid.y);
	}
	లేకపోతే
	  మొదటి_offcurve = p;
      }
    }
    లేకపోతే
    {
      ఒకవేళ (చివరి_offcurve)
      {
	ఒకవేళ (is_on_వక్రం)
	{
	  ఒకవేళ (చివరి_offcurve2)
	  {
	    draw_session->cubic_గమ్యం (చివరి_offcurve2.x, చివరి_offcurve2.y,
				    చివరి_offcurve.x, చివరి_offcurve.y,
				    p.x, p.y);
	    చివరి_offcurve2 = optional_బిందువు_t ();
	  }
	  లేకపోతే
	    draw_session->quadratic_గమ్యం (చివరి_offcurve.x, చివరి_offcurve.y,
				       p.x, p.y);
	  చివరి_offcurve = optional_బిందువు_t ();
	}
	లేకపోతే
	{
	  ఒకవేళ (is_cubic && !చివరి_offcurve2)
	  {
	    చివరి_offcurve2 = చివరి_offcurve;
	    చివరి_offcurve = p;
	  }
	  లేకపోతే
	  {
	    optional_బిందువు_t mid = చివరి_offcurve.mid (p);

	    ఒకవేళ (is_cubic)
	    {
	      draw_session->cubic_గమ్యం (చివరి_offcurve2.x, చివరి_offcurve2.y,
				      చివరి_offcurve.x, చివరి_offcurve.y,
				      mid.x, mid.y);
	      చివరి_offcurve2 = optional_బిందువు_t ();
	    }
	    లేకపోతే
	      draw_session->quadratic_గమ్యం (చివరి_offcurve.x, చివరి_offcurve.y,
					 mid.x, mid.y);
	    చివరి_offcurve = p;
	  }
	}
      }
      లేకపోతే
      {
	ఒకవేళ (is_on_వక్రం)
	  draw_session->పంక్తి_గమ్యం (p.x, p.y);
	లేకపోతే
	  చివరి_offcurve = p;
      }
    }

  }

  శూన్య contour_చివర ()
  {
    ఒకవేళ (మొదటి_offcurve && చివరి_offcurve)
    {
      optional_బిందువు_t mid = చివరి_offcurve.mid (మొదటి_offcurve2 ?
						మొదటి_offcurve2 :
						మొదటి_offcurve);
      ఒకవేళ (చివరి_offcurve2)
	draw_session->cubic_గమ్యం (చివరి_offcurve2.x, చివరి_offcurve2.y,
				చివరి_offcurve.x, చివరి_offcurve.y,
				mid.x, mid.y);
      లేకపోతే
	draw_session->quadratic_గమ్యం (చివరి_offcurve.x, చివరి_offcurve.y,
				   mid.x, mid.y);
      చివరి_offcurve = optional_బిందువు_t ();
    }
    /* now check the rest */

    ఒకవేళ (మొదటి_offcurve && మొదటి_oncurve)
    {
      ఒకవేళ (మొదటి_offcurve2)
	draw_session->cubic_గమ్యం (మొదటి_offcurve2.x, మొదటి_offcurve2.y,
				మొదటి_offcurve.x, మొదటి_offcurve.y,
				మొదటి_oncurve.x, మొదటి_oncurve.y);
      లేకపోతే
	draw_session->quadratic_గమ్యం (మొదటి_offcurve.x, మొదటి_offcurve.y,
				   మొదటి_oncurve.x, మొదటి_oncurve.y);
    }
    లేకపోతే ఒకవేళ (చివరి_offcurve && మొదటి_oncurve)
    {
      ఒకవేళ (చివరి_offcurve2)
	draw_session->cubic_గమ్యం (చివరి_offcurve2.x, చివరి_offcurve2.y,
				చివరి_offcurve.x, చివరి_offcurve.y,
				మొదటి_oncurve.x, మొదటి_oncurve.y);
      లేకపోతే
	draw_session->quadratic_గమ్యం (చివరి_offcurve.x, చివరి_offcurve.y,
				   మొదటి_oncurve.x, మొదటి_oncurve.y);
    }
    లేకపోతే ఒకవేళ (మొదటి_oncurve)
      draw_session->పంక్తి_గమ్యం (మొదటి_oncurve.x, మొదటి_oncurve.y);
    లేకపోతే ఒకవేళ (మొదటి_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_బిందువు_t mid = మొదటి_offcurve2.mid (మొదటి_offcurve);
      draw_session->move_గమ్యం (mid.x, mid.y);
      draw_session->cubic_గమ్యం (మొదటి_offcurve2.x, మొదటి_offcurve2.y,
			      మొదటి_offcurve.x, మొదటి_offcurve.y,
			      mid.x, mid.y);
    }
    లేకపోతే ఒకవేళ (మొదటి_offcurve)
    {
      దశాంశం x = మొదటి_offcurve.x, y = మొదటి_offcurve.y;
      draw_session->move_గమ్యం (x, y);
      draw_session->quadratic_గమ్యం (x, y, x, y);
    }

    /* Getting ready for the next contour */
    మొదటి_oncurve = మొదటి_offcurve = మొదటి_offcurve2 = చివరి_offcurve = చివరి_offcurve2 = optional_బిందువు_t ();
    draw_session->close_మార్గం ();
  }

  శూన్య points_చివర () {}

  తార్కిక is_consuming_contour_points () { తిరిగి నిజం; }
  contour_బిందువు_t *get_phantoms_sink () { తిరిగి శూన్యసూచిక; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#అయితే_ముగింపు /* OT_GLYF_PATH_BUILDER_HH */
