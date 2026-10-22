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

#اگر_غیر_تعریف_شدہ HB_AAT_LAYOUT_ANKR_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_ANKR_جدول_HH

#شامل "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#تعریف_کرو HB_AAT_لیبل_ankr HB_لیبل('a','n','k','r')


نامخلا AAT {

استعمال نامخلا OT;


ساخت Anchor
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  عوامی:
  DEFINE_سائز_STATIC (4);
};

قسمتعریف Array32_کا<Anchor> GlyphAnchors;

ساخت ankr
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_AAT_لیبل_ankr;

  ثابت Anchor &get_anchor (hb_codepoint_t glyph_id,
			    غیرعلامتی صحیح i,
			    غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    ثابت NNOffset16_مقصد<GlyphAnchors> *انحراف = (یہ+lookup_جدول).get_قدر (glyph_id, عدد_glyphs);
    اگر (!انحراف)
      واپس Null (Anchor);
    ثابت GlyphAnchors &anchors = &(یہ+anchor_ڈیٹا) + *انحراف;
    واپس anchors[i];
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  نسخہ == 0 &&
			  c->check_range (یہ, anchor_ڈیٹا) &&
			  lookup_جدول.sanitize (c, یہ, &(یہ+anchor_ڈیٹا))));
  }

  محفوظ:
  HBUINT16	نسخہ;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32_مقصد<Lookup<NNOffset16_مقصد<GlyphAnchors>>>
		lookup_جدول;	/* Offset to the table's lookup table */
  NNOffset32_مقصد<HBUINT8>
		anchor_ڈیٹا;	/* Offset to the glyph data table */

  عوامی:
  DEFINE_سائز_STATIC (12);
};

} /* namespace AAT */


#اگر_ختم /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
