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

#که_نه_تعریف_شوی HB_AAT_LAYOUT_ANKR_جدول_HH
#تعریف_کړه HB_AAT_LAYOUT_ANKR_جدول_HH

#ضمن "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#تعریف_کړه HB_AAT_نښان_ankr HB_نښان('a','n','k','r')


نطاق AAT {

کارول نطاق OT;


بنية Anchor
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  عام:
  DEFINE_اندازه_STATIC (4);
};

تعريفنوع Array32_د<Anchor> GlyphAnchors;

بنية ankr
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_AAT_نښان_ankr;

  ثابت Anchor &get_anchor (hb_codepoint_t glyph_id,
			    بینښه صحيح i,
			    بینښه صحيح عدد_glyphs) ثابت
  {
    ثابت NNOffset16_منزل<GlyphAnchors> *دننه_واټن = (هذا+lookup_جدول).get_ارزښت (glyph_id, عدد_glyphs);
    إذا (!دننه_واټن)
      أرجع Null (Anchor);
    ثابت GlyphAnchors &anchors = &(هذا+anchor_معلومات) + *دننه_واټن;
    أرجع anchors[i];
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه == 0 &&
			  c->check_range (هذا, anchor_معلومات) &&
			  lookup_جدول.sanitize (c, هذا, &(هذا+anchor_معلومات))));
  }

  محمي:
  HBUINT16	نسخه;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32_منزل<Lookup<NNOffset16_منزل<GlyphAnchors>>>
		lookup_جدول;	/* Offset to the table's lookup table */
  NNOffset32_منزل<HBUINT8>
		anchor_معلومات;	/* Offset to the glyph data table */

  عام:
  DEFINE_اندازه_STATIC (12);
};

} /* namespace AAT */


#د_که_پای /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
