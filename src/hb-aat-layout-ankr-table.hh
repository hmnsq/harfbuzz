/*
 * Copyright © 2018  Ebrahim Byagowi
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

#ifndef HB_AAT_LAYOUT_ANKR_TABLE_HH
#define HB_AAT_LAYOUT_ANKR_TABLE_HH

#অন্তর্ভাৱ "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#define HB_AAT_TAG_ankr HB_TAG('a','n','k','r')


নামাকাশ AAT {

using নামাকাশ OT;


সংরচনা Anchor
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্ৱজনিক:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4);
};

প্রকারনির্দেশ Array32Of<Anchor> GlyphAnchors;

সংরচনা ankr
{
  স্থির constexpr hb_tag_t tableTag = HB_AAT_TAG_ankr;

  ধ্রুৱ Anchor &get_anchor (hb_codepoint_t glyph_id,
			    unsigned পূর্ণাঙ্ক i,
			    unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    ধ্রুৱ NNOffset16To<GlyphAnchors> *offset = (এতত্+lookupTable).get_মান (glyph_id, অঙ্ক_glyphs);
    যদি (!offset)
      ঘূৰাই Null (Anchor);
    ধ্রুৱ GlyphAnchors &anchors = &(এতত্+anchorData) + *offset;
    ঘূৰাই anchors[i];
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  hb_barrier () &&
			  version == 0 &&
			  c->check_range (এতত্, anchorData) &&
			  lookupTable.sanitize (c, এতত্, &(এতত্+anchorData))));
  }

  রক্ষিত:
  HBUINT16	version;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32To<Lookup<NNOffset16To<GlyphAnchors>>>
		lookupTable;	/* Offset to the table's lookup table */
  NNOffset32To<HBUINT8>
		anchorData;	/* Offset to the glyph data table */

  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
