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

#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#define HB_AAT_TAG_opbd HB_TAG('o','p','b','d')


ନାମାକାଶ AAT {

ସଂରଚନା OpticalBounds
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  FWORD		ବାମ_Side;
  FWORD		ଶୀର୍ଷ_Side;
  FWORD		ଡାହାଣ_Side;
  FWORD		bottomSide;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା opbdFormat0
{
  ତାର୍କିକ get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଅଚଳ Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_ମୂଲ୍ୟ (glyph_id, font->face->get_ଅଙ୍କ_glyphs ());
    ଯଦି (!bounds_offset) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଅଚଳ OpticalBounds &bounds = base+*bounds_offset;

    ଯଦି (extents)
      *extents = {
	font->em_scale_x (bounds.ବାମ_Side),
	font->em_scale_y (bounds.ଶୀର୍ଷ_Side),
	font->em_scale_x (bounds.ଡାହାଣ_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) && lookupTable.sanitize (c, base)));
  }

  ସୁରକ୍ଷିତ:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (2);
};

ସଂରଚନା opbdFormat1
{
  ତାର୍କିକ get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଅଚଳ Offset16To<OpticalBounds> *bounds_offset = lookupTable.get_ମୂଲ୍ୟ (glyph_id, font->face->get_ଅଙ୍କ_glyphs ());
    ଯଦି (!bounds_offset) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଅଚଳ OpticalBounds &bounds = base+*bounds_offset;

    hb_position_t left = 0, ଶୀର୍ଷ_2 = 0, ଡାହାଣ = 0, bottom = 0, ignore;
    ଯଦି (font->get_glyph_contour_point (glyph_id, bounds.ବାମ_Side, &left, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.ଶୀର୍ଷ_Side, &ignore, &ଶୀର୍ଷ_2) ||
	font->get_glyph_contour_point (glyph_id, bounds.ଡାହାଣ_Side, &ଡାହାଣ, &ignore) ||
	font->get_glyph_contour_point (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      ଯଦି (extents)
	*extents = {left, ଶୀର୍ଷ_2, ଡାହାଣ, bottom};
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) && lookupTable.sanitize (c, base)));
  }

  ସୁରକ୍ଷିତ:
  Lookup<Offset16To<OpticalBounds>>
		lookupTable;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (2);
};

ସଂରଚନା opbd
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_AAT_TAG_opbd;

  ତାର୍କିକ get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) ଅଚଳ
  {
    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_bounds (font, glyph_id, extents, ଏହି);
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_bounds (font, glyph_id, extents, ଏହି);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି) || version.major != 1))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); return_trace (u.format0.sanitize (c, ଏହି));
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c, ଏହି));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>version;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  ସଂଘ {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (8);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
