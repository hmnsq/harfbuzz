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

#அந்தர்பாவ "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#define HB_AAT_TAG_ankr HB_TAG('a','n','k','r')


நாமாகாஶ AAT {

using நாமாகாஶ OT;


கட்டமைப்பு Anchor
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  பொது:
  DEFINE_SIZE_STATIC (4);
};

வகைவரை Array32Of<Anchor> GlyphAnchors;

கட்டமைப்பு ankr
{
  நிலையான constexpr hb_tag_t tableTag = HB_AAT_TAG_ankr;

  மாறிலி Anchor &get_anchor (hb_codepoint_t glyph_id,
			    unsigned முழு i,
			    unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    மாறிலி NNOffset16To<GlyphAnchors> *offset = (இது+lookupTable).get_மதிப்பு (glyph_id, இலக்கம்_glyphs);
    எனில் (!offset)
      திரும்பு Null (Anchor);
    மாறிலி GlyphAnchors &anchors = &(இது+anchorData) + *offset;
    திரும்பு anchors[i];
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  version == 0 &&
			  c->check_range (இது, anchorData) &&
			  lookupTable.sanitize (c, இது, &(இது+anchorData))));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	version;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32To<Lookup<NNOffset16To<GlyphAnchors>>>
		lookupTable;	/* Offset to the table's lookup table */
  NNOffset32To<HBUINT8>
		anchorData;	/* Offset to the glyph data table */

  பொது:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
