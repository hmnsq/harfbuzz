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

#ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
#define OT_LAYOUT_COMMON_RANGERECORD_HH

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ Common {

template <typename Types, typename ମୂଲ୍ୟ = HBUINT16>
ସଂରଚନା RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  ମୂଲ୍ୟ                         value;          /*ମୂଲ୍ୟ*/

  DEFINE_SIZE_STATIC (2 * Types::size + ମୂଲ୍ୟ::static_size);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t g) ଅଚଳ
  { ଫେରନ୍ତୁ g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp_range (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb) {
    ଅଚଳ RangeRecord *a = (ଅଚଳ RangeRecord *) pa;
    ଅଚଳ RangeRecord *b = (ଅଚଳ RangeRecord *) pb;
    ଯଦି (a->first < b->first) ଫେରନ୍ତୁ -1;
    ଯଦି (a->first > b->first) ଫେରନ୍ତୁ +1;
    ଯଦି (a->last < b->last) ଫେରନ୍ତୁ -1;
    ଯଦି (a->last > b->last) ଫେରନ୍ତୁ +1;
    ଯଦି (a->value < b->value) ଫେରନ୍ତୁ -1;
    ଯଦି (a->value > b->value) ଫେରନ୍ତୁ +1;
    ଫେରନ୍ତୁ 0;
  }

  unsigned get_population () ଅଚଳ
  {
    ଯଦି (unlikely (last < first)) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ (last - first + 1);
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t &glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ glyphs.intersects (first, last); }

  template <typename set_t>
  ତାର୍କିକ collect_coverage (set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
ବାହ୍ୟ HB_INTERNAL ଅଚଳ unsigned ଅକ୍ଷର _hb_Null_OT_RangeRecord[9];
template <typename Spec, typename ମୂଲ୍ୟ>
ସଂରଚନା Null<OT::Layout::Common::RangeRecord<Spec, ମୂଲ୍ୟ>> {
  ସ୍ଥିର OT::Layout::Common::RangeRecord<Spec, ମୂଲ୍ୟ> ଅଚଳ & get_null () {
    ଫେରନ୍ତୁ *reinterpret_cast<ଅଚଳ OT::Layout::Common::RangeRecord<Spec, ମୂଲ୍ୟ> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
