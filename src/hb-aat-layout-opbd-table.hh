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

#અન્તર્ભાવ "hb-aat-layout-common.hh"
#અન્તર્ભાવ "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#define HB_AAT_TAG_opbd HB_TAG('o','p','b','d')


નામાકાશ AAT {

સંરચના OpticalBounds
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  FWORD		ડાબું_Side;
  FWORD		ટોચ_Side;
  FWORD		જમણું_Side;
  FWORD		bottomSide;
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના opbdFormat0
{
  તાર્કિક get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, અચલ શૂન્ય *base) અચલ
  {
    અચલ Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_કિંમત (glyph_id, font->face->get_અંક_glyphs ());
    જો (!bounds_offset) પાછા ખોટું;
    અચલ OpticalBounds &bounds = base+*bounds_offset;

    જો (extents)
      *extents = {
	font->em_scale_x (bounds.ડાબું_Side),
	font->em_scale_y (bounds.ટોચ_Side),
	font->em_scale_x (bounds.જમણું_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    પાછા સાચું;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) && lookupTable.sanitize (c, base)));
  }

  સુરક્ષિત:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  જાહેર:
  DEFINE_SIZE_MIN (2);
};

સંરચના opbdFormat1
{
  તાર્કિક get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, અચલ શૂન્ય *base) અચલ
  {
    અચલ Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_કિંમત (glyph_id, font->face->get_અંક_glyphs ());
    જો (!bounds_offset) પાછા ખોટું;
    અચલ OpticalBounds &bounds = base+*bounds_offset;

    hb_position_t left = 0, ટોચ_2 = 0, જમણું = 0, bottom = 0, ignore;
    જો (font->get_glyph_contour_point (glyph_id, bounds.ડાબું_Side, &left, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.ટોચ_Side, &ignore, &ટોચ_2) ||
	font->get_glyph_contour_point (glyph_id, bounds.જમણું_Side, &જમણું, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      જો (extents)
	*extents = {left, ટોચ_2, જમણું, bottom};
      પાછા સાચું;
    }
    પાછા ખોટું;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) && lookupTable.sanitize (c, base)));
  }

  સુરક્ષિત:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  જાહેર:
  DEFINE_SIZE_MIN (2);
};

સંરચના opbd
{
  સ્થિર constexpr hb_tag_t tableTag = HB_AAT_TAG_opbd;

  તાર્કિક get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) અચલ
  {
    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_bounds (font, glyph_id, extents, આ);
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_bounds (font, glyph_id, extents, આ);
    મૂળભૂત:પાછા ખોટું;
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ) || version.major != 1))
      return_trace (ખોટું);
    hb_barrier ();

    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); return_trace (u.format0.sanitize (c, આ));
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c, આ));
    મૂળભૂત:return_trace (સાચું);
    }
  }

  સુરક્ષિત:
  FixedVersion<>version;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  સંઘ {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  જાહેર:
  DEFINE_SIZE_MIN (8);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
