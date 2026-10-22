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

#اگر_غیر_تعریف_شدہ HB_AAT_LAYOUT_OPBD_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_OPBD_جدول_HH

#شامل "hb-aat-layout-common.hh"
#شامل "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#تعریف_کرو HB_AAT_لیبل_opbd HB_لیبل('o','p','b','d')


نامخلا AAT {

ساخت OpticalBounds
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  FWORD		بایاں_Side;
  FWORD		چوٹی_Side;
  FWORD		دایاں_Side;
  FWORD		bottomSide;
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت opbdFormat0
{
  منطقی get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ثابت خالی *بنیاد) ثابت
  {
    ثابت Offset16_مقصد<OpticalBounds> *bounds_انحراف = lookup_جدول.get_قدر (glyph_id, font->face->get_عدد_glyphs ());
    اگر (!bounds_انحراف) واپس جھوٹ;
    ثابت OpticalBounds &bounds = بنیاد+*bounds_انحراف;

    اگر (extents)
      *extents = {
	font->em_scale_x (bounds.بایاں_Side),
	font->em_scale_y (bounds.چوٹی_Side),
	font->em_scale_x (bounds.دایاں_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    واپس سچ;
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) && lookup_جدول.sanitize (c, بنیاد)));
  }

  محفوظ:
  Lookup<Offset16_مقصد<OpticalBounds>>
		lookup_جدول;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  عوامی:
  DEFINE_سائز_MIN (2);
};

ساخت opbdFormat1
{
  منطقی get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ثابت خالی *بنیاد) ثابت
  {
    ثابت Offset16_مقصد<OpticalBounds> *bounds_انحراف = lookup_جدول.get_قدر (glyph_id, font->face->get_عدد_glyphs ());
    اگر (!bounds_انحراف) واپس جھوٹ;
    ثابت OpticalBounds &bounds = بنیاد+*bounds_انحراف;

    hb_position_t بائیں = 0, چوٹی_2 = 0, دائیں = 0, bottom = 0, ignore;
    اگر (font->get_glyph_contour_نقطہ (glyph_id, bounds.بایاں_Side, &بائیں, &ignore) ||
	font->get_glyph_contour_نقطہ (glyph_id, bounds.چوٹی_Side, &ignore, &چوٹی_2) ||
	font->get_glyph_contour_نقطہ (glyph_id, bounds.دایاں_Side, &دائیں, &ignore) ||
	font->get_glyph_contour_نقطہ (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      اگر (extents)
	*extents = {بائیں, چوٹی_2, دائیں, bottom};
      واپس سچ;
    }
    واپس جھوٹ;
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) && lookup_جدول.sanitize (c, بنیاد)));
  }

  محفوظ:
  Lookup<Offset16_مقصد<OpticalBounds>>
		lookup_جدول;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  عوامی:
  DEFINE_سائز_MIN (2);
};

ساخت opbd
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_AAT_لیبل_opbd;

  منطقی get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); واپس u.format0.get_bounds (font, glyph_id, extents, یہ);
    حالت 1: hb_barrier (); واپس u.format1.get_bounds (font, glyph_id, extents, یہ);
    طےشدہ:واپس جھوٹ;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ) || نسخہ.major != 1))
      return_trace (جھوٹ);
    hb_barrier ();

    انتخاب (format)
    {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c, یہ));
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c, یہ));
    طےشدہ:return_trace (سچ);
    }
  }

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  اتحاد {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  عوامی:
  DEFINE_سائز_MIN (8);
};

} /* namespace AAT */


#اگر_ختم /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
