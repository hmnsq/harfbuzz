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

#شامل "hb-aat-layout-common.hh"
#شامل "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#define HB_AAT_TAG_opbd HB_TAG('o','p','b','d')


فضاینام AAT {

ساختار OpticalBounds
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  FWORD		چپ_Side;
  FWORD		بالاترین_Side;
  FWORD		راست_Side;
  FWORD		bottomSide;
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار opbdFormat0
{
  منطقی get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ثابت تهی *base) ثابت
  {
    ثابت Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_مقدار (glyph_id, font->face->get_رقم_glyphs ());
    اگر (!bounds_offset) بازگشت نادرست;
    ثابت OpticalBounds &bounds = base+*bounds_offset;

    اگر (extents)
      *extents = {
	font->em_scale_x (bounds.چپ_Side),
	font->em_scale_y (bounds.بالاترین_Side),
	font->em_scale_x (bounds.راست_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    بازگشت درست;
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) && lookupTable.sanitize (c, base)));
  }

  محافظتشده:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  عمومی:
  DEFINE_SIZE_MIN (2);
};

ساختار opbdFormat1
{
  منطقی get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ثابت تهی *base) ثابت
  {
    ثابت Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_مقدار (glyph_id, font->face->get_رقم_glyphs ());
    اگر (!bounds_offset) بازگشت نادرست;
    ثابت OpticalBounds &bounds = base+*bounds_offset;

    hb_position_t left = 0, بالاترین_2 = 0, راست = 0, bottom = 0, ignore;
    اگر (font->get_glyph_contour_point (glyph_id, bounds.چپ_Side, &left, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.بالاترین_Side, &ignore, &بالاترین_2) ||
	font->get_glyph_contour_point (glyph_id, bounds.راست_Side, &راست, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      اگر (extents)
	*extents = {left, بالاترین_2, راست, bottom};
      بازگشت درست;
    }
    بازگشت نادرست;
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) && lookupTable.sanitize (c, base)));
  }

  محافظتشده:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  عمومی:
  DEFINE_SIZE_MIN (2);
};

ساختار opbd
{
  ایستا constexpr hb_tag_t tableTag = HB_AAT_TAG_opbd;

  منطقی get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); بازگشت u.format0.get_bounds (font, glyph_id, extents, این);
    حالت 1: hb_barrier (); بازگشت u.format1.get_bounds (font, glyph_id, extents, این);
    پیشفرض:بازگشت نادرست;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این) || version.major != 1))
      return_trace (نادرست);
    hb_barrier ();

    انتخاب (format)
    {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c, این));
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c, این));
    پیشفرض:return_trace (درست);
    }
  }

  محافظتشده:
  FixedVersion<>version;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  اجتماع {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  عمومی:
  DEFINE_SIZE_MIN (8);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
