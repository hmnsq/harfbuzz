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

#अगर_अघोषित HB_AAT_LAYOUT_ANKR_TABLE_HH
#घोषणा HB_AAT_LAYOUT_ANKR_TABLE_HH

#समावेश "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#घोषणा HB_AAT_TAG_ankr HB_TAG('a','n','k','r')


namespace AAT {

using namespace OT;


काष्ठा Anchor
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  खुला:
  DEFINE_आकार_STATIC (4);
};

प्रकार Array32Of<Anchor> GlyphAnchors;

काष्ठा ankr
{
  अटल constexpr hb_tag_t tableTag = HB_AAT_TAG_ankr;

  स्थिर Anchor &get_anchor (hb_codepoint_t glyph_id,
			    अचिन्हित पूर्णांक i,
			    अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    स्थिर NNOffset16To<GlyphAnchors> *offset = (यह+lookupTable).get_मूल्य (glyph_id, अंक_glyphs);
    अगर (!offset)
      वापस Null (Anchor);
    स्थिर GlyphAnchors &anchors = &(यह+anchor_तथ्य) + *offset;
    वापस anchors[i];
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  version == 0 &&
			  c->check_range (यह, anchor_तथ्य) &&
			  lookupTable.sanitize (c, यह, &(यह+anchor_तथ्य))));
  }

  रक्षित:
  HBUINT16	version;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32To<Lookup<NNOffset16To<GlyphAnchors>>>
		lookupTable;	/* Offset to the table's lookup table */
  NNOffset32To<HBUINT8>
		anchor_तथ्य;	/* Offset to the glyph data table */

  खुला:
  DEFINE_आकार_STATIC (12);
};

} /* namespace AAT */


#पूर्ण_अगर /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
