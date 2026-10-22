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

#جيڪڏهن_وصف_نه_ٿيل HB_AAT_LAYOUT_OPBD_جدول_HH
#وصف_ڪريو HB_AAT_LAYOUT_OPBD_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#وصف_ڪريو HB_AAT_ٽيگ_opbd HB_ٽيگ('o','p','b','d')


نطاق AAT {

بنية OpticalBounds
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  FWORD		کاٻو_Side;
  FWORD		چوٽي_Side;
  FWORD		ساڄو_Side;
  FWORD		bottomSide;
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية opbdFormat0
{
  منطقي get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ثابت فراغ *بنياد) ثابت
  {
    ثابت Offset16_منزل<OpticalBounds> *bounds_وٿي = lookup_جدول.get_قدر (glyph_id, font->face->get_انگ_glyphs ());
    إذا (!bounds_وٿي) أرجع خطأ;
    ثابت OpticalBounds &bounds = بنياد+*bounds_وٿي;

    إذا (extents)
      *extents = {
	font->em_scale_x (bounds.کاٻو_Side),
	font->em_scale_y (bounds.چوٽي_Side),
	font->em_scale_x (bounds.ساڄو_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    أرجع سچ;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && lookup_جدول.sanitize (c, بنياد)));
  }

  محمي:
  Lookup<Offset16_منزل<OpticalBounds>>
		lookup_جدول;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  عام:
  DEFINE_سائيز_MIN (2);
};

بنية opbdFormat1
{
  منطقي get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ثابت فراغ *بنياد) ثابت
  {
    ثابت Offset16_منزل<OpticalBounds> *bounds_وٿي = lookup_جدول.get_قدر (glyph_id, font->face->get_انگ_glyphs ());
    إذا (!bounds_وٿي) أرجع خطأ;
    ثابت OpticalBounds &bounds = بنياد+*bounds_وٿي;

    hb_position_t کاٻي = 0, چوٽي_2 = 0, ساڄي = 0, bottom = 0, ignore;
    إذا (font->get_glyph_contour_نقطو (glyph_id, bounds.کاٻو_Side, &کاٻي, &ignore) ||
	font->get_glyph_contour_نقطو (glyph_id, bounds.چوٽي_Side, &ignore, &چوٽي_2) ||
	font->get_glyph_contour_نقطو (glyph_id, bounds.ساڄو_Side, &ساڄي, &ignore) ||
	font->get_glyph_contour_نقطو (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      إذا (extents)
	*extents = {کاٻي, چوٽي_2, ساڄي, bottom};
      أرجع سچ;
    }
    أرجع خطأ;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && lookup_جدول.sanitize (c, بنياد)));
  }

  محمي:
  Lookup<Offset16_منزل<OpticalBounds>>
		lookup_جدول;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  عام:
  DEFINE_سائيز_MIN (2);
};

بنية opbd
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_AAT_ٽيگ_opbd;

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
    إذا (unlikely (!c->check_struct (هذا) || نسخو.major != 1))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, هذا));
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, هذا));
    افتراضي:return_trace (سچ);
    }
  }

  محمي:
  Fixed_نسخو<>نسخو;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  اتحاد {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  عام:
  DEFINE_سائيز_MIN (8);
};

} /* namespace AAT */


#جيڪڏهن_پورو /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
