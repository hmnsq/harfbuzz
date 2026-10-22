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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LAYOUT_ANKR_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LAYOUT_ANKR_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-common.hh"

/*
 * ankr -- Anchor Point
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6ankr.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_ਟੈਗ_ankr HB_ਟੈਗ('a','n','k','r')


ਨਾਮਾਕਾਸ਼ AAT {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;


ਬਣਤਰ Anchor
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  FWORD		xCoordinate;
  FWORD		yCoordinate;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ Array32_ਦਾ<Anchor> GlyphAnchors;

ਬਣਤਰ ankr
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_AAT_ਟੈਗ_ankr;

  ਅਚਲ Anchor &get_anchor (hb_codepoint_t glyph_id,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਅਚਲ NNOffset16_ਮੰਜ਼ਿਲ<GlyphAnchors> *ਵਿੱਥ = (ਇਹ+lookup_ਸਾਰਣੀ).get_ਮੁੱਲ (glyph_id, ਅੰਕ_glyphs);
    ਜੇ (!ਵਿੱਥ)
      ਵਾਪਸ Null (Anchor);
    ਅਚਲ GlyphAnchors &anchors = &(ਇਹ+anchor_ਅੰਕੜੇ) + *ਵਿੱਥ;
    ਵਾਪਸ anchors[i];
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  ਸੰਸਕਰਣ == 0 &&
			  c->check_range (ਇਹ, anchor_ਅੰਕੜੇ) &&
			  lookup_ਸਾਰਣੀ.sanitize (c, ਇਹ, &(ਇਹ+anchor_ਅੰਕੜੇ))));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;	/* Version number (set to zero) */
  HBUINT16	flags;		/* Flags (currently unused; set to zero) */
  Offset32_ਮੰਜ਼ਿਲ<Lookup<NNOffset16_ਮੰਜ਼ਿਲ<GlyphAnchors>>>
		lookup_ਸਾਰਣੀ;	/* Offset to the table's lookup table */
  NNOffset32_ਮੰਜ਼ਿਲ<HBUINT8>
		anchor_ਅੰਕੜੇ;	/* Offset to the glyph data table */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};

} /* namespace AAT */


#ਜੇ_ਅੰਤ /* HB_AAT_LAYOUT_ANKR_TABLE_HH */
