/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
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
 *
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod, Garret Rieger
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_COMMON_RANGERECORD_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_COMMON_RANGERECORD_HH

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ Common {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types, ਕਿਸਮ_ਨਾਂ ਮੁੱਲ = HBUINT16>
ਬਣਤਰ Range_ਰਿਕਾਰਡ
{
  ਕਿਸਮ_ਨਾਂ Types::HBGlyphID     ਪਹਿਲਾ;          /* First GlyphID in the range */
  ਕਿਸਮ_ਨਾਂ Types::HBGlyphID     ਆਖਰੀ;           /* Last GlyphID in the range */
  ਮੁੱਲ                         value;          /*ਮੁੱਲ*/

  DEFINE_ਮਾਪ_STATIC (2 * Types::ਮਾਪ + ਮੁੱਲ::static_ਮਾਪ);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਪੂਰਨ cmp (hb_codepoint_t g) ਅਚਲ
  { ਵਾਪਸ g < ਪਹਿਲਾ ? -1 : g <= ਆਖਰੀ ? 0 : +1; }

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp_range (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb) {
    ਅਚਲ Range_ਰਿਕਾਰਡ *a = (ਅਚਲ Range_ਰਿਕਾਰਡ *) pa;
    ਅਚਲ Range_ਰਿਕਾਰਡ *b = (ਅਚਲ Range_ਰਿਕਾਰਡ *) pb;
    ਜੇ (a->ਪਹਿਲਾ < b->ਪਹਿਲਾ) ਵਾਪਸ -1;
    ਜੇ (a->ਪਹਿਲਾ > b->ਪਹਿਲਾ) ਵਾਪਸ +1;
    ਜੇ (a->ਆਖਰੀ < b->ਆਖਰੀ) ਵਾਪਸ -1;
    ਜੇ (a->ਆਖਰੀ > b->ਆਖਰੀ) ਵਾਪਸ +1;
    ਜੇ (a->value < b->value) ਵਾਪਸ -1;
    ਜੇ (a->value > b->value) ਵਾਪਸ +1;
    ਵਾਪਸ 0;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_population () ਅਚਲ
  {
    ਜੇ (unlikely (ਆਖਰੀ < ਪਹਿਲਾ)) ਵਾਪਸ 0;
    ਵਾਪਸ (ਆਖਰੀ - ਪਹਿਲਾ + 1);
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t &glyphs) ਅਚਲ
  { ਵਾਪਸ glyphs.intersects (ਪਹਿਲਾ, ਆਖਰੀ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਤਾਰਕਿਕ collect_coverage (ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ glyphs->ਜੋੜੋ_range (ਪਹਿਲਾ, ਆਖਰੀ); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ hb_Null_OT_Range_ਰਿਕਾਰਡ[9];
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Spec, ਕਿਸਮ_ਨਾਂ ਮੁੱਲ>
ਬਣਤਰ Null<OT::Layout::Common::Range_ਰਿਕਾਰਡ<Spec, ਮੁੱਲ>> {
  ਸਥਿਰ OT::Layout::Common::Range_ਰਿਕਾਰਡ<Spec, ਮੁੱਲ> ਅਚਲ & get_null () {
    ਵਾਪਸ *reinterpret_cast<ਅਚਲ OT::Layout::Common::Range_ਰਿਕਾਰਡ<Spec, ਮੁੱਲ> *> (hb_Null_OT_Range_ਰਿਕਾਰਡ);
  }
};


#ਜੇ_ਅੰਤ  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
