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

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ Common {

template <typename Types, typename মান = HBUINT16>
সংরচনা RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  মান                         value;          /*মান*/

  DEFINE_SIZE_STATIC (2 * Types::size + মান::static_size);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  পূর্ণাঙ্ক cmp (hb_codepoint_t g) ধ্রুব
  { প্রত্যাগম g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL স্থির পূর্ণাঙ্ক cmp_range (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb) {
    ধ্রুব RangeRecord *a = (ধ্রুব RangeRecord *) pa;
    ধ্রুব RangeRecord *b = (ধ্রুব RangeRecord *) pb;
    যদি (a->first < b->first) প্রত্যাগম -1;
    যদি (a->first > b->first) প্রত্যাগম +1;
    যদি (a->last < b->last) প্রত্যাগম -1;
    যদি (a->last > b->last) প্রত্যাগম +1;
    যদি (a->value < b->value) প্রত্যাগম -1;
    যদি (a->value > b->value) প্রত্যাগম +1;
    প্রত্যাগম 0;
  }

  unsigned get_population () ধ্রুব
  {
    যদি (unlikely (last < first)) প্রত্যাগম 0;
    প্রত্যাগম (last - first + 1);
  }

  তার্কিক intersects (ধ্রুব hb_set_t &glyphs) ধ্রুব
  { প্রত্যাগম glyphs.intersects (first, last); }

  template <typename set_t>
  তার্কিক collect_coverage (set_t *glyphs) ধ্রুব
  { প্রত্যাগম glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
বাহ্য HB_INTERNAL ধ্রুব unsigned বর্ণ _hb_Null_OT_RangeRecord[9];
template <typename Spec, typename মান>
সংরচনা Null<OT::Layout::Common::RangeRecord<Spec, মান>> {
  স্থির OT::Layout::Common::RangeRecord<Spec, মান> ধ্রুব & get_null () {
    প্রত্যাগম *reinterpret_cast<ধ্রুব OT::Layout::Common::RangeRecord<Spec, মান> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
