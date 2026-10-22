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

#अगर_अघोषित HB_AAT_LAYOUT_OPBD_TABLE_HH
#घोषणा HB_AAT_LAYOUT_OPBD_TABLE_HH

#समावेश "hb-aat-layout-common.hh"
#समावेश "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#घोषणा HB_AAT_TAG_opbd HB_TAG('o','p','b','d')


namespace AAT {

काष्ठा OpticalBounds
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  FWORD		बायाँ_Side;
  FWORD		शीर्ष_Side;
  FWORD		दायाँ_Side;
  FWORD		bottomSide;
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा opbdFormat0
{
  bool get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, स्थिर व्योम *base) स्थिर
  {
    स्थिर Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_मूल्य (glyph_id, font->face->get_अंक_glyphs ());
    अगर (!bounds_offset) वापस false;
    स्थिर OpticalBounds &bounds = base+*bounds_offset;

    अगर (extents)
      *extents = {
	font->em_scale_x (bounds.बायाँ_Side),
	font->em_scale_y (bounds.शीर्ष_Side),
	font->em_scale_x (bounds.दायाँ_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    वापस true;
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) && lookupTable.sanitize (c, base)));
  }

  रक्षित:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  खुला:
  DEFINE_आकार_MIN (2);
};

काष्ठा opbdFormat1
{
  bool get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, स्थिर व्योम *base) स्थिर
  {
    स्थिर Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_मूल्य (glyph_id, font->face->get_अंक_glyphs ());
    अगर (!bounds_offset) वापस false;
    स्थिर OpticalBounds &bounds = base+*bounds_offset;

    hb_position_t बाँए = 0, शीर्ष_4 = 0, दाँए = 0, bottom = 0, ignore;
    अगर (font->get_glyph_contour_point (glyph_id, bounds.बायाँ_Side, &बाँए, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.शीर्ष_Side, &ignore, &शीर्ष_4) ||
	font->get_glyph_contour_point (glyph_id, bounds.दायाँ_Side, &दाँए, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      अगर (extents)
	*extents = {बाँए, शीर्ष_4, दाँए, bottom};
      वापस true;
    }
    वापस false;
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) && lookupTable.sanitize (c, base)));
  }

  रक्षित:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  खुला:
  DEFINE_आकार_MIN (2);
};

काष्ठा opbd
{
  अटल constexpr hb_tag_t tableTag = HB_AAT_TAG_opbd;

  bool get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) स्थिर
  {
    चयन (format)
    {
    हाल 0: hb_barrier (); वापस u.format0.get_bounds (font, glyph_id, extents, यह);
    हाल 1: hb_barrier (); वापस u.format1.get_bounds (font, glyph_id, extents, यह);
    शेष:वापस false;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह) || version.major != 1))
      return_trace (false);
    hb_barrier ();

    चयन (format)
    {
    हाल 0: hb_barrier (); return_trace (u.format0.sanitize (c, यह));
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c, यह));
    शेष:return_trace (true);
    }
  }

  रक्षित:
  FixedVersion<>version;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  जोड़ {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  खुला:
  DEFINE_आकार_MIN (8);
};

} /* namespace AAT */


#पूर्ण_अगर /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
