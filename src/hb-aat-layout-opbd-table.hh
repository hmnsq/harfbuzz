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

#അന്തര്ഭാവ "hb-aat-layout-common.hh"
#അന്തര്ഭാവ "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#define HB_AAT_TAG_opbd HB_TAG('o','p','b','d')


നാമാകാശ AAT {

ഘടന OpticalBounds
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  FWORD		ഇടത്_Side;
  FWORD		മുകൾ_Side;
  FWORD		വലത്_Side;
  FWORD		bottomSide;
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന opbdFormat0
{
  തർക്കം get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    സ്ഥിരം Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_മൂല്യം (glyph_id, font->face->get_അക്കം_glyphs ());
    എങ്കിൽ (!bounds_offset) തിരികെ അസത്യം;
    സ്ഥിരം OpticalBounds &bounds = base+*bounds_offset;

    എങ്കിൽ (extents)
      *extents = {
	font->em_scale_x (bounds.ഇടത്_Side),
	font->em_scale_y (bounds.മുകൾ_Side),
	font->em_scale_x (bounds.വലത്_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    തിരികെ സത്യം;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) && lookupTable.sanitize (c, base)));
  }

  സംരക്ഷിത:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  പൊതു:
  DEFINE_SIZE_MIN (2);
};

ഘടന opbdFormat1
{
  തർക്കം get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    സ്ഥിരം Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_മൂല്യം (glyph_id, font->face->get_അക്കം_glyphs ());
    എങ്കിൽ (!bounds_offset) തിരികെ അസത്യം;
    സ്ഥിരം OpticalBounds &bounds = base+*bounds_offset;

    hb_position_t left = 0, മുകൾ_2 = 0, വലത് = 0, bottom = 0, ignore;
    എങ്കിൽ (font->get_glyph_contour_point (glyph_id, bounds.ഇടത്_Side, &left, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.മുകൾ_Side, &ignore, &മുകൾ_2) ||
	font->get_glyph_contour_point (glyph_id, bounds.വലത്_Side, &വലത്, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      എങ്കിൽ (extents)
	*extents = {left, മുകൾ_2, വലത്, bottom};
      തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) && lookupTable.sanitize (c, base)));
  }

  സംരക്ഷിത:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  പൊതു:
  DEFINE_SIZE_MIN (2);
};

ഘടന opbd
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_AAT_TAG_opbd;

  തർക്കം get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_bounds (font, glyph_id, extents, ഇത്);
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_bounds (font, glyph_id, extents, ഇത്);
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്) || version.major != 1))
      return_trace (അസത്യം);
    hb_barrier ();

    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); return_trace (u.format0.sanitize (c, ഇത്));
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c, ഇത്));
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  സംരക്ഷിത:
  FixedVersion<>version;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  സംഘം {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  പൊതു:
  DEFINE_SIZE_MIN (8);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
