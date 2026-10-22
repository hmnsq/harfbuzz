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

#অন্তর্ভাব "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#define HB_AAT_TAG_ankr HB_TAG('a','n','k','r')


নামাকাশ AAT {

using নামাকাশ OT;


কাঠামো Anchor
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

প্রকারলেখ Array32Of<Anchor> GlyphAnchors;

কাঠামো ankr
{
  স্থির constexpr hb_tag_t tableTag = HB_AAT_TAG_ankr;

  ধ্রুবক Anchor &get_anchor (hb_codepoint_t glyph_id,
			    unsigned পূর্ণসংখ্যা i,
			    unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    ধ্রুবক NNOffset16To<GlyphAnchors> *offset = (এই+lookupTable).get_মান (glyph_id, অঙ্ক_glyphs);
    যদি (!offset)
      ফেরত Null (Anchor);
    ধ্রুবক GlyphAnchors &anchors = &(এই+anchor_তথ্য) + *offset;
    ফেরত anchors[i];
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  version == 0 &&
			  c->check_range (এই, anchor_তথ্য) &&
			  lookupTable.sanitize (c, এই, &(এই+anchor_তথ্য))));
  }

  সুরক্ষিত:
  HBUINT16	version;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32To<Lookup<NNOffset16To<GlyphAnchors>>>
		lookupTable;	/* Offset to the table's lookup table */
  NNOffset32To<HBUINT8>
		anchor_তথ্য;	/* Offset to the glyph data table */

  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
