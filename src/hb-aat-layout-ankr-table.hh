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

#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#define HB_AAT_TAG_ankr HB_TAG('a','n','k','r')


ନାମାକାଶ AAT {

using ନାମାକାଶ OT;


ସଂରଚନା Anchor
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା Array32Of<Anchor> GlyphAnchors;

ସଂରଚନା ankr
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_AAT_TAG_ankr;

  ଅଚଳ Anchor &get_anchor (hb_codepoint_t glyph_id,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ଅଚଳ NNOffset16To<GlyphAnchors> *offset = (ଏହି+lookupTable).get_ମୂଲ୍ୟ (glyph_id, ଅଙ୍କ_glyphs);
    ଯଦି (!offset)
      ଫେରନ୍ତୁ Null (Anchor);
    ଅଚଳ GlyphAnchors &anchors = &(ଏହି+anchorData) + *offset;
    ଫେରନ୍ତୁ anchors[i];
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  version == 0 &&
			  c->check_range (ଏହି, anchorData) &&
			  lookupTable.sanitize (c, ଏହି, &(ଏହି+anchorData))));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	version;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32To<Lookup<NNOffset16To<GlyphAnchors>>>
		lookupTable;	/* Offset to the table's lookup table */
  NNOffset32To<HBUINT8>
		anchorData;	/* Offset to the glyph data table */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
