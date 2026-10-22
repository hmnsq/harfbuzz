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

#ಅನ್ತರ್ಭಾವ "hb-aat-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#define HB_AAT_TAG_opbd HB_TAG('o','p','b','d')


ನಾಮಾಕಾಶ AAT {

ಸಂರಚನಾ OpticalBounds
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  FWORD		ಎಡ_Side;
  FWORD		ತುದಿ_Side;
  FWORD		ಬಲ_Side;
  FWORD		bottomSide;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ opbdFormat0
{
  ತಾರ್ಕಿಕ get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಧ್ರುವ Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_ಮೌಲ್ಯ (glyph_id, font->face->get_ಅಂಕೆ_glyphs ());
    ಯದಿ (!bounds_offset) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಧ್ರುವ OpticalBounds &bounds = base+*bounds_offset;

    ಯದಿ (extents)
      *extents = {
	font->em_scale_x (bounds.ಎಡ_Side),
	font->em_scale_y (bounds.ತುದಿ_Side),
	font->em_scale_x (bounds.ಬಲ_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) && lookupTable.sanitize (c, base)));
  }

  ರಕ್ಷಿತ:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (2);
};

ಸಂರಚನಾ opbdFormat1
{
  ತಾರ್ಕಿಕ get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಧ್ರುವ Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_ಮೌಲ್ಯ (glyph_id, font->face->get_ಅಂಕೆ_glyphs ());
    ಯದಿ (!bounds_offset) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಧ್ರುವ OpticalBounds &bounds = base+*bounds_offset;

    hb_position_t left = 0, ತುದಿ_2 = 0, ಬಲ = 0, bottom = 0, ignore;
    ಯದಿ (font->get_glyph_contour_point (glyph_id, bounds.ಎಡ_Side, &left, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.ತುದಿ_Side, &ignore, &ತುದಿ_2) ||
	font->get_glyph_contour_point (glyph_id, bounds.ಬಲ_Side, &ಬಲ, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      ಯದಿ (extents)
	*extents = {left, ತುದಿ_2, ಬಲ, bottom};
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) && lookupTable.sanitize (c, base)));
  }

  ರಕ್ಷಿತ:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (2);
};

ಸಂರಚನಾ opbd
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_AAT_TAG_opbd;

  ತಾರ್ಕಿಕ get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_bounds (font, glyph_id, extents, ಏತತ್);
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_bounds (font, glyph_id, extents, ಏತತ್);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್) || version.major != 1))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); return_trace (u.format0.sanitize (c, ಏತತ್));
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c, ಏತತ್));
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  ರಕ್ಷಿತ:
  FixedVersion<>version;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  ಸಙ್ಘ {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (8);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
