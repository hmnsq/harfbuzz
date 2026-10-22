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

#ಅನ್ತರ್ಭಾವ "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#define HB_AAT_TAG_ankr HB_TAG('a','n','k','r')


ನಾಮಾಕಾಶ AAT {

using ನಾಮಾಕಾಶ OT;


ಸಂರಚನಾ Anchor
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಪ್ರಕಾರನಿರ್ದೇಶ Array32Of<Anchor> GlyphAnchors;

ಸಂರಚನಾ ankr
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_AAT_TAG_ankr;

  ಧ್ರುವ Anchor &get_anchor (hb_codepoint_t glyph_id,
			    unsigned ಪೂರ್ಣಾಙ್ಕ i,
			    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಧ್ರುವ NNOffset16To<GlyphAnchors> *offset = (ಏತತ್+lookupTable).get_ಮೌಲ್ಯ (glyph_id, ಅಂಕೆ_glyphs);
    ಯದಿ (!offset)
      ಪ್ರತ್ಯಾಗಮ Null (Anchor);
    ಧ್ರುವ GlyphAnchors &anchors = &(ಏತತ್+anchorData) + *offset;
    ಪ್ರತ್ಯಾಗಮ anchors[i];
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  version == 0 &&
			  c->check_range (ಏತತ್, anchorData) &&
			  lookupTable.sanitize (c, ಏತತ್, &(ಏತತ್+anchorData))));
  }

  ರಕ್ಷಿತ:
  HBUINT16	version;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32To<Lookup<NNOffset16To<GlyphAnchors>>>
		lookupTable;	/* Offset to the table's lookup table */
  NNOffset32To<HBUINT8>
		anchorData;	/* Offset to the glyph data table */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
