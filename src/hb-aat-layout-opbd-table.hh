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

#অন্তর্ভাৱ "hb-aat-layout-common.hh"
#অন্তর্ভাৱ "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#define HB_AAT_TAG_opbd HB_TAG('o','p','b','d')


নামাকাশ AAT {

সংরচনা OpticalBounds
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  FWORD		বাম_Side;
  FWORD		শীৰ্ষ_Side;
  FWORD		ডান_Side;
  FWORD		bottomSide;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা opbdFormat0
{
  তার্কিক get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    ধ্রুৱ Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_মান (glyph_id, font->face->get_অঙ্ক_glyphs ());
    যদি (!bounds_offset) ঘূৰাই মিছা;
    ধ্রুৱ OpticalBounds &bounds = base+*bounds_offset;

    যদি (extents)
      *extents = {
	font->em_scale_x (bounds.বাম_Side),
	font->em_scale_y (bounds.শীৰ্ষ_Side),
	font->em_scale_x (bounds.ডান_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    ঘূৰাই সঁচা;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) && lookupTable.sanitize (c, base)));
  }

  রক্ষিত:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (2);
};

সংরচনা opbdFormat1
{
  তার্কিক get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    ধ্রুৱ Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_মান (glyph_id, font->face->get_অঙ্ক_glyphs ());
    যদি (!bounds_offset) ঘূৰাই মিছা;
    ধ্রুৱ OpticalBounds &bounds = base+*bounds_offset;

    hb_position_t left = 0, শীৰ্ষ_2 = 0, ডান = 0, bottom = 0, ignore;
    যদি (font->get_glyph_contour_point (glyph_id, bounds.বাম_Side, &left, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.শীৰ্ষ_Side, &ignore, &শীৰ্ষ_2) ||
	font->get_glyph_contour_point (glyph_id, bounds.ডান_Side, &ডান, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      যদি (extents)
	*extents = {left, শীৰ্ষ_2, ডান, bottom};
      ঘূৰাই সঁচা;
    }
    ঘূৰাই মিছা;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) && lookupTable.sanitize (c, base)));
  }

  রক্ষিত:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (2);
};

সংরচনা opbd
{
  স্থির constexpr hb_tag_t tableTag = HB_AAT_TAG_opbd;

  তার্কিক get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) ধ্রুৱ
  {
    ৱিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); ঘূৰাই u.format0.get_bounds (font, glyph_id, extents, এতত্);
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_bounds (font, glyph_id, extents, এতত্);
    স্ৱভাৱ:ঘূৰাই মিছা;
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্) || version.major != 1))
      return_trace (মিছা);
    hb_barrier ();

    ৱিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); return_trace (u.format0.sanitize (c, এতত্));
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c, এতত্));
    স্ৱভাৱ:return_trace (সঁচা);
    }
  }

  রক্ষিত:
  FixedVersion<>version;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  সঙ্ঘ {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (8);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
