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

فضاینام OT {
فضاینام Layout {
فضاینام Common {

template <typename Types, typename مقدار = HBUINT16>
ساختار RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  مقدار                         value;          /*مقدار*/

  DEFINE_SIZE_STATIC (2 * Types::size + مقدار::static_size);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  صحیح cmp (hb_codepoint_t g) ثابت
  { بازگشت g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL ایستا صحیح cmp_range (ثابت تهی *pa, ثابت تهی *pb) {
    ثابت RangeRecord *a = (ثابت RangeRecord *) pa;
    ثابت RangeRecord *b = (ثابت RangeRecord *) pb;
    اگر (a->first < b->first) بازگشت -1;
    اگر (a->first > b->first) بازگشت +1;
    اگر (a->last < b->last) بازگشت -1;
    اگر (a->last > b->last) بازگشت +1;
    اگر (a->value < b->value) بازگشت -1;
    اگر (a->value > b->value) بازگشت +1;
    بازگشت 0;
  }

  unsigned get_population () ثابت
  {
    اگر (unlikely (last < first)) بازگشت 0;
    بازگشت (last - first + 1);
  }

  منطقی intersects (ثابت hb_set_t &glyphs) ثابت
  { بازگشت glyphs.intersects (first, last); }

  template <typename set_t>
  منطقی collect_coverage (set_t *glyphs) ثابت
  { بازگشت glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
بیرونی HB_INTERNAL ثابت unsigned نویسه _hb_Null_OT_RangeRecord[9];
template <typename Spec, typename مقدار>
ساختار Null<OT::Layout::Common::RangeRecord<Spec, مقدار>> {
  ایستا OT::Layout::Common::RangeRecord<Spec, مقدار> ثابت & get_null () {
    بازگشت *reinterpret_cast<ثابت OT::Layout::Common::RangeRecord<Spec, مقدار> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
