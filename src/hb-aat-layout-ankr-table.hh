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

#جے_بنا_تعریف HB_AAT_LAYOUT_ANKR_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_ANKR_جدول_HH

#ضمن "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#تعریف_کرو HB_AAT_ٹیگ_ankr HB_ٹیگ('a','n','k','r')


نطاق AAT {

ورتوں نطاق OT;


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
  DEFINE_ناپ_STATIC (4);
};

تعريفنوع Array32_دا<Anchor> GlyphAnchors;

بنية ankr
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_AAT_ٹیگ_ankr;

  ثابت Anchor &get_anchor (hb_codepoint_t glyph_id,
			    بنانشانی صحيح i,
			    بنانشانی صحيح عدد_glyphs) ثابت
  {
    ثابت NNOffset16_منزل<GlyphAnchors> *وتھ = (هذا+lookup_جدول).get_قدر (glyph_id, عدد_glyphs);
    إذا (!وتھ)
      أرجع Null (Anchor);
    ثابت GlyphAnchors &anchors = &(هذا+anchor_انکڑے) + *وتھ;
    أرجع anchors[i];
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخہ == 0 &&
			  c->check_range (هذا, anchor_انکڑے) &&
			  lookup_جدول.sanitize (c, هذا, &(هذا+anchor_انکڑے))));
  }

  محمي:
  HBUINT16	نسخہ;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32_منزل<Lookup<NNOffset16_منزل<GlyphAnchors>>>
		lookup_جدول;	/* Offset to the table's lookup table */
  NNOffset32_منزل<HBUINT8>
		anchor_انکڑے;	/* Offset to the glyph data table */

  عام:
  DEFINE_ناپ_STATIC (12);
};

} /* namespace AAT */


#جے_انت /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
