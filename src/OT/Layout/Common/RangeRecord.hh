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

نطاق OT {
نطاق Layout {
نطاق Common {

template <typename Types, typename قيمة = HBUINT16>
بنية RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  قيمة                         value;          /*قيمة*/

  DEFINE_SIZE_STATIC (2 * Types::size + قيمة::static_size);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL ساكن صحيح cmp_range (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت RangeRecord *a = (ثابت RangeRecord *) pa;
    ثابت RangeRecord *b = (ثابت RangeRecord *) pb;
    إذا (a->first < b->first) أرجع -1;
    إذا (a->first > b->first) أرجع +1;
    إذا (a->last < b->last) أرجع -1;
    إذا (a->last > b->last) أرجع +1;
    إذا (a->value < b->value) أرجع -1;
    إذا (a->value > b->value) أرجع +1;
    أرجع 0;
  }

  unsigned get_population () ثابت
  {
    إذا (unlikely (last < first)) أرجع 0;
    أرجع (last - first + 1);
  }

  منطقي intersects (ثابت hb_set_t &glyphs) ثابت
  { أرجع glyphs.intersects (first, last); }

  template <typename set_t>
  منطقي collect_coverage (set_t *glyphs) ثابت
  { أرجع glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
خارجي HB_INTERNAL ثابت unsigned حرف _hb_Null_OT_RangeRecord[9];
template <typename Spec, typename قيمة>
بنية Null<OT::Layout::Common::RangeRecord<Spec, قيمة>> {
  ساكن OT::Layout::Common::RangeRecord<Spec, قيمة> ثابت & get_null () {
    أرجع *reinterpret_cast<ثابت OT::Layout::Common::RangeRecord<Spec, قيمة> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
