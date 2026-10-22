/*
 * Copyright © 2019  Ebrahim Byagowi
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#اگر_تعریف_نشده HB_AAT_LAYOUT_OPBD_جدول_HH
#تعریف_کن HB_AAT_LAYOUT_OPBD_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#تعریف_کن HB_AAT_برچسب_opbd HB_برچسب('o','p','b','d')


نطاق AAT {

بنية OpticalBounds
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  FWORD		چپ_Side;
  FWORD		بالاترین_Side;
  FWORD		راست_Side;
  FWORD		bottomSide;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية opbdFormat0
{
  منطقي get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ثابت فراغ *پایه) ثابت
  {
    ثابت Offset16_مقصد<OpticalBounds> *bounds_جابه_جایی = lookup_جدول.get_مقدار (glyph_id, font->face->get_رقم_glyphs ());
    إذا (!bounds_جابه_جایی) أرجع خطأ;
    ثابت OpticalBounds &bounds = پایه+*bounds_جابه_جایی;

    إذا (extents)
      *extents = {
	font->em_scale_x (bounds.چپ_Side),
	font->em_scale_y (bounds.بالاترین_Side),
	font->em_scale_x (bounds.راست_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    أرجع درست;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && lookup_جدول.sanitize (c, پایه)));
  }

  محمي:
  Lookup<Offset16_مقصد<OpticalBounds>>
		lookup_جدول;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  عام:
  DEFINE_اندازه_MIN (2);
};

بنية opbdFormat1
{
  منطقي get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ثابت فراغ *پایه) ثابت
  {
    ثابت Offset16_مقصد<OpticalBounds> *bounds_جابه_جایی = lookup_جدول.get_مقدار (glyph_id, font->face->get_رقم_glyphs ());
    إذا (!bounds_جابه_جایی) أرجع خطأ;
    ثابت OpticalBounds &bounds = پایه+*bounds_جابه_جایی;

    hb_position_t چپ = 0, بالاترین_2 = 0, راست = 0, bottom = 0, ignore;
    إذا (font->get_glyph_contour_نقطه (glyph_id, bounds.چپ_Side, &چپ, &ignore) ||
	font->get_glyph_contour_نقطه (glyph_id, bounds.بالاترین_Side, &ignore, &بالاترین_2) ||
	font->get_glyph_contour_نقطه (glyph_id, bounds.راست_Side, &راست, &ignore) ||
	font->get_glyph_contour_نقطه (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      إذا (extents)
	*extents = {چپ, بالاترین_2, راست, bottom};
      أرجع درست;
    }
    أرجع خطأ;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && lookup_جدول.sanitize (c, پایه)));
  }

  محمي:
  Lookup<Offset16_مقصد<OpticalBounds>>
		lookup_جدول;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  عام:
  DEFINE_اندازه_MIN (2);
};

بنية opbd
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_AAT_برچسب_opbd;

  منطقي get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_bounds (font, glyph_id, extents, هذا);
    حالة 1: hb_barrier (); أرجع u.format1.get_bounds (font, glyph_id, extents, هذا);
    افتراضي:أرجع خطأ;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا) || نسخه.major != 1))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, هذا));
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, هذا));
    افتراضي:return_trace (درست);
    }
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  اتحاد {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  عام:
  DEFINE_اندازه_MIN (8);
};

} /* namespace AAT */


#پایان_اگر /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
