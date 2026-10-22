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

#ifndef HB_AAT_LAYOUT_OPBD_TABLE_HH
#define HB_AAT_LAYOUT_OPBD_TABLE_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#define HB_AAT_TAG_opbd HB_TAG('o','p','b','d')


نطاق AAT {

بنية OpticalBounds
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  FWORD		هغو_Side;
  FWORD		كولي_Side;
  FWORD		دام_Side;
  FWORD		bottomSide;
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية opbdFormat0
{
  منطقي get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ثابت فراغ *base) ثابت
  {
    ثابت Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_درج (glyph_id, font->face->get_لمب_glyphs ());
    إذا (!bounds_offset) أرجع خطأ;
    ثابت OpticalBounds &bounds = base+*bounds_offset;

    إذا (extents)
      *extents = {
	font->em_scale_x (bounds.هغو_Side),
	font->em_scale_y (bounds.كولي_Side),
	font->em_scale_x (bounds.دام_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    أرجع true;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && lookupTable.sanitize (c, base)));
  }

  محمي:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  عام:
  DEFINE_SIZE_MIN (2);
};

بنية opbdFormat1
{
  منطقي get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ثابت فراغ *base) ثابت
  {
    ثابت Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_درج (glyph_id, font->face->get_لمب_glyphs ());
    إذا (!bounds_offset) أرجع خطأ;
    ثابت OpticalBounds &bounds = base+*bounds_offset;

    hb_position_t left = 0, كولي_2 = 0, دام = 0, bottom = 0, ignore;
    إذا (font->get_glyph_contour_point (glyph_id, bounds.هغو_Side, &left, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.كولي_Side, &ignore, &كولي_2) ||
	font->get_glyph_contour_point (glyph_id, bounds.دام_Side, &دام, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      إذا (extents)
	*extents = {left, كولي_2, دام, bottom};
      أرجع true;
    }
    أرجع خطأ;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) && lookupTable.sanitize (c, base)));
  }

  محمي:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  عام:
  DEFINE_SIZE_MIN (2);
};

بنية opbd
{
  ساكن constexpr hb_tag_t tableTag = HB_AAT_TAG_opbd;

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
    إذا (unlikely (!c->check_struct (هذا) || version.major != 1))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, هذا));
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, هذا));
    افتراضي:return_trace (true);
    }
  }

  محمي:
  FixedVersion<>version;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  اتحاد {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  عام:
  DEFINE_SIZE_MIN (8);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
