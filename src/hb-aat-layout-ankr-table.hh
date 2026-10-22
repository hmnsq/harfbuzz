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

#अन्तर्भाव "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#define HB_AAT_TAG_ankr HB_TAG('a','n','k','r')


नामाकाश AAT {

using नामाकाश OT;


संरचना Anchor
{
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

प्रकारनिर्देश Array32Of<Anchor> GlyphAnchors;

संरचना ankr
{
  स्थिर constexpr hb_tag_t tableTag = HB_AAT_TAG_ankr;

  अचर Anchor &get_anchor (hb_codepoint_t glyph_id,
			    अचिन्हित पूर्णाङ्क i,
			    अचिन्हित पूर्णाङ्क अङ्क_glyphs) अचर
  {
    अचर NNOffset16To<GlyphAnchors> *offset = (एतत्+lookupTable).get_मान (glyph_id, अङ्क_glyphs);
    यदि (!offset)
      फर्काऊ Null (Anchor);
    अचर GlyphAnchors &anchors = &(एतत्+anchorData) + *offset;
    फर्काऊ anchors[i];
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  version == 0 &&
			  c->check_range (एतत्, anchorData) &&
			  lookupTable.sanitize (c, एतत्, &(एतत्+anchorData))));
  }

  रक्षित:
  HBUINT16	version;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32To<Lookup<NNOffset16To<GlyphAnchors>>>
		lookupTable;	/* Offset to the table's lookup table */
  NNOffset32To<HBUINT8>
		anchorData;	/* Offset to the glyph data table */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
