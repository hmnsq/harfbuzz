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

#அந்தர்பாவ "hb-aat-layout-common.hh"
#அந்தர்பாவ "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#define HB_AAT_TAG_opbd HB_TAG('o','p','b','d')


நாமாகாஶ AAT {

கட்டமைப்பு OpticalBounds
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  FWORD		இடது_Side;
  FWORD		உச்சி_Side;
  FWORD		வலது_Side;
  FWORD		bottomSide;
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு opbdFormat0
{
  தருக்க get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, மாறிலி வெறுமை *base) மாறிலி
  {
    மாறிலி Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_மதிப்பு (glyph_id, font->face->get_இலக்கம்_glyphs ());
    எனில் (!bounds_offset) திரும்பு பொய்;
    மாறிலி OpticalBounds &bounds = base+*bounds_offset;

    எனில் (extents)
      *extents = {
	font->em_scale_x (bounds.இடது_Side),
	font->em_scale_y (bounds.உச்சி_Side),
	font->em_scale_x (bounds.வலது_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    திரும்பு உண்மை;
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) && lookupTable.sanitize (c, base)));
  }

  பாதுகாக்கப்பட்ட:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  பொது:
  DEFINE_SIZE_MIN (2);
};

கட்டமைப்பு opbdFormat1
{
  தருக்க get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, மாறிலி வெறுமை *base) மாறிலி
  {
    மாறிலி Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_மதிப்பு (glyph_id, font->face->get_இலக்கம்_glyphs ());
    எனில் (!bounds_offset) திரும்பு பொய்;
    மாறிலி OpticalBounds &bounds = base+*bounds_offset;

    hb_position_t left = 0, உச்சி_2 = 0, வலது = 0, bottom = 0, ignore;
    எனில் (font->get_glyph_contour_point (glyph_id, bounds.இடது_Side, &left, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.உச்சி_Side, &ignore, &உச்சி_2) ||
	font->get_glyph_contour_point (glyph_id, bounds.வலது_Side, &வலது, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      எனில் (extents)
	*extents = {left, உச்சி_2, வலது, bottom};
      திரும்பு உண்மை;
    }
    திரும்பு பொய்;
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) && lookupTable.sanitize (c, base)));
  }

  பாதுகாக்கப்பட்ட:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  பொது:
  DEFINE_SIZE_MIN (2);
};

கட்டமைப்பு opbd
{
  நிலையான constexpr hb_tag_t tableTag = HB_AAT_TAG_opbd;

  தருக்க get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) மாறிலி
  {
    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_bounds (font, glyph_id, extents, இது);
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_bounds (font, glyph_id, extents, இது);
    இயல்பு:திரும்பு பொய்;
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது) || version.major != 1))
      return_trace (பொய்);
    hb_barrier ();

    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); return_trace (u.format0.sanitize (c, இது));
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c, இது));
    இயல்பு:return_trace (உண்மை);
    }
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  ஒன்றிப்பு {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  பொது:
  DEFINE_SIZE_MIN (8);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
