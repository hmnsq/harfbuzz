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

#अन्तर्भाव "hb-aat-layout-common.hh"
#अन्तर्भाव "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#define HB_AAT_TAG_opbd HB_TAG('o','p','b','d')


नामाकाश AAT {

संरचना OpticalBounds
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  FWORD		बायाँ_Side;
  FWORD		शीर्ष_Side;
  FWORD		दायाँ_Side;
  FWORD		bottomSide;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना opbdFormat0
{
  तार्किक get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ध्रुव शून्य *base) ध्रुव
  {
    ध्रुव Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_मूल्य (glyph_id, font->face->get_अंक_glyphs ());
    यदि (!bounds_offset) प्रत्यागम असत्य;
    ध्रुव OpticalBounds &bounds = base+*bounds_offset;

    यदि (extents)
      *extents = {
	font->em_scale_x (bounds.बायाँ_Side),
	font->em_scale_y (bounds.शीर्ष_Side),
	font->em_scale_x (bounds.दायाँ_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    प्रत्यागम सत्य;
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) && lookupTable.sanitize (c, base)));
  }

  रक्षित:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  सार्वजनिक:
  DEFINE_SIZE_MIN (2);
};

संरचना opbdFormat1
{
  तार्किक get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ध्रुव शून्य *base) ध्रुव
  {
    ध्रुव Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_मूल्य (glyph_id, font->face->get_अंक_glyphs ());
    यदि (!bounds_offset) प्रत्यागम असत्य;
    ध्रुव OpticalBounds &bounds = base+*bounds_offset;

    hb_position_t left = 0, शीर्ष_2 = 0, दायाँ = 0, bottom = 0, ignore;
    यदि (font->get_glyph_contour_point (glyph_id, bounds.बायाँ_Side, &left, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.शीर्ष_Side, &ignore, &शीर्ष_2) ||
	font->get_glyph_contour_point (glyph_id, bounds.दायाँ_Side, &दायाँ, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      यदि (extents)
	*extents = {left, शीर्ष_2, दायाँ, bottom};
      प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) && lookupTable.sanitize (c, base)));
  }

  रक्षित:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  सार्वजनिक:
  DEFINE_SIZE_MIN (2);
};

संरचना opbd
{
  स्थिर constexpr hb_tag_t tableTag = HB_AAT_TAG_opbd;

  तार्किक get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) ध्रुव
  {
    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); प्रत्यागम u.format0.get_bounds (font, glyph_id, extents, एतत्);
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.get_bounds (font, glyph_id, extents, एतत्);
    स्वभाव:प्रत्यागम असत्य;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!c->check_struct (एतत्) || version.major != 1))
      return_trace (असत्य);
    hb_barrier ();

    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); return_trace (u.format0.sanitize (c, एतत्));
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c, एतत्));
    स्वभाव:return_trace (सत्य);
    }
  }

  रक्षित:
  FixedVersion<>version;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  सङ्घ {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
