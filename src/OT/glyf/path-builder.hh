#ifndef OT_GLYF_PATH_BUILDER_HH
#define OT_GLYF_PATH_BUILDER_HH


#include "../../hb.hh"


namespace OT {
namespace glyf_impl {


sa_nracanaa path_builder_t
{
  hb_font_t *font;
  hb_draw_session_t *draw_session;

  sa_nracanaa optional_point_t
  {
    optional_point_t () {}
    optional_point_t (float x_, float y_) : has_data (true), x (x_), y (y_) {}
    operator bool () const { ပြန်ပေး has_data; }

    bool has_data = false;
    float x;
    float y;

    optional_point_t mid (optional_point_t p)
    { ပြန်ပေး optional_point_t ((x + p.x) * 0.5f, (y + p.y) * 0.5f); }
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
  void consume_point (const contour_point_t &point)
  {
    bool is_on_curve = point.flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE;
#ifdef HB_NO_CUBIC_GLYF
    constexpr bool is_cubic = false;
#မဟုတ်ပါက
    bool is_cubic = !is_on_curve && (point.flag & glyf_impl::SimpleGlyph::FLAG_CUBIC);
#endif
    optional_point_t p (font->em_fscalef_x (point.x), font->em_fscalef_y (point.y));
    အကယ်၍ (unlikely (!first_oncurve))
    {
      အကယ်၍ (is_on_curve)
      {
	first_oncurve = p;
	draw_session->move_to (p.x, p.y);
      }
      မဟုတ်ပါက
      {
	အကယ်၍ (is_cubic && !first_offcurve2)
	{
	  first_offcurve2 = first_offcurve;
	  first_offcurve = p;
	}
	မဟုတ်ပါက အကယ်၍ (first_offcurve)
	{
	  optional_point_t mid = first_offcurve.mid (p);
	  first_oncurve = mid;
	  last_offcurve = p;
	  draw_session->move_to (mid.x, mid.y);
	}
	မဟုတ်ပါက
	  first_offcurve = p;
      }
    }
    မဟုတ်ပါက
    {
      အကယ်၍ (last_offcurve)
      {
	အကယ်၍ (is_on_curve)
	{
	  အကယ်၍ (last_offcurve2)
	  {
	    draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				    last_offcurve.x, last_offcurve.y,
				    p.x, p.y);
	    last_offcurve2 = optional_point_t ();
	  }
	  မဟုတ်ပါက
	    draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				       p.x, p.y);
	  last_offcurve = optional_point_t ();
	}
	မဟုတ်ပါက
	{
	  အကယ်၍ (is_cubic && !last_offcurve2)
	  {
	    last_offcurve2 = last_offcurve;
	    last_offcurve = p;
	  }
	  မဟုတ်ပါက
	  {
	    optional_point_t mid = last_offcurve.mid (p);

	    အကယ်၍ (is_cubic)
	    {
	      draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				      last_offcurve.x, last_offcurve.y,
				      mid.x, mid.y);
	      last_offcurve2 = optional_point_t ();
	    }
	    မဟုတ်ပါက
	      draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
					 mid.x, mid.y);
	    last_offcurve = p;
	  }
	}
      }
      မဟုတ်ပါက
      {
	အကယ်၍ (is_on_curve)
	  draw_session->line_to (p.x, p.y);
	မဟုတ်ပါက
	  last_offcurve = p;
      }
    }

  }

  void contour_end ()
  {
    အကယ်၍ (first_offcurve && last_offcurve)
    {
      optional_point_t mid = last_offcurve.mid (first_offcurve2 ?
						first_offcurve2 :
						first_offcurve);
      အကယ်၍ (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				mid.x, mid.y);
      မဟုတ်ပါက
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   mid.x, mid.y);
      last_offcurve = optional_point_t ();
    }
    /* now check the rest */

    အကယ်၍ (first_offcurve && first_oncurve)
    {
      အကယ်၍ (first_offcurve2)
	draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
				first_offcurve.x, first_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      မဟုတ်ပါက
	draw_session->quadratic_to (first_offcurve.x, first_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    မဟုတ်ပါက အကယ်၍ (last_offcurve && first_oncurve)
    {
      အကယ်၍ (last_offcurve2)
	draw_session->cubic_to (last_offcurve2.x, last_offcurve2.y,
				last_offcurve.x, last_offcurve.y,
				first_oncurve.x, first_oncurve.y);
      မဟုတ်ပါက
	draw_session->quadratic_to (last_offcurve.x, last_offcurve.y,
				   first_oncurve.x, first_oncurve.y);
    }
    မဟုတ်ပါက အကယ်၍ (first_oncurve)
      draw_session->line_to (first_oncurve.x, first_oncurve.y);
    မဟုတ်ပါက အကယ်၍ (first_offcurve2)
    {
      /* A contour with just two cubic controls closes at their midpoint. */
      optional_point_t mid = first_offcurve2.mid (first_offcurve);
      draw_session->move_to (mid.x, mid.y);
      draw_session->cubic_to (first_offcurve2.x, first_offcurve2.y,
			      first_offcurve.x, first_offcurve.y,
			      mid.x, mid.y);
    }
    မဟုတ်ပါက အကယ်၍ (first_offcurve)
    {
      float x = first_offcurve.x, y = first_offcurve.y;
      draw_session->move_to (x, y);
      draw_session->quadratic_to (x, y, x, y);
    }

    /* Getting ready for the next contour */
    first_oncurve = first_offcurve = first_offcurve2 = last_offcurve = last_offcurve2 = optional_point_t ();
    draw_session->close_path ();
  }

  void points_end () {}

  bool is_consuming_contour_points () { ပြန်ပေး true; }
  contour_point_t *get_phantoms_sink () { ပြန်ပေး nullptr; }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_PATH_BUILDER_HH */
