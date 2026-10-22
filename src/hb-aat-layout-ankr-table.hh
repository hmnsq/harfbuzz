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

#അന്തര്ഭാവ "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#define HB_AAT_TAG_ankr HB_TAG('a','n','k','r')


നാമാകാശ AAT {

using നാമാകാശ OT;


ഘടന Anchor
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

തരനിർവചനം Array32Of<Anchor> GlyphAnchors;

ഘടന ankr
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_AAT_TAG_ankr;

  സ്ഥിരം Anchor &get_anchor (hb_codepoint_t glyph_id,
			    unsigned പൂർണ്ണസംഖ്യ i,
			    unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    സ്ഥിരം NNOffset16To<GlyphAnchors> *offset = (ഇത്+lookupTable).get_മൂല്യം (glyph_id, അക്കം_glyphs);
    എങ്കിൽ (!offset)
      തിരികെ Null (Anchor);
    സ്ഥിരം GlyphAnchors &anchors = &(ഇത്+anchorData) + *offset;
    തിരികെ anchors[i];
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  version == 0 &&
			  c->check_range (ഇത്, anchorData) &&
			  lookupTable.sanitize (c, ഇത്, &(ഇത്+anchorData))));
  }

  സംരക്ഷിത:
  HBUINT16	version;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32To<Lookup<NNOffset16To<GlyphAnchors>>>
		lookupTable;	/* Offset to the table's lookup table */
  NNOffset32To<HBUINT8>
		anchorData;	/* Offset to the glyph data table */

  പൊതു:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
