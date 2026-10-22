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

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ Common {

template <typename Types, typename மதிப்பு = HBUINT16>
கட்டமைப்பு RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  மதிப்பு                         value;          /*மதிப்பு*/

  DEFINE_SIZE_STATIC (2 * Types::size + மதிப்பு::static_size);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  முழு cmp (hb_codepoint_t g) மாறிலி
  { திரும்பு g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL நிலையான முழு cmp_range (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb) {
    மாறிலி RangeRecord *a = (மாறிலி RangeRecord *) pa;
    மாறிலி RangeRecord *b = (மாறிலி RangeRecord *) pb;
    எனில் (a->first < b->first) திரும்பு -1;
    எனில் (a->first > b->first) திரும்பு +1;
    எனில் (a->last < b->last) திரும்பு -1;
    எனில் (a->last > b->last) திரும்பு +1;
    எனில் (a->value < b->value) திரும்பு -1;
    எனில் (a->value > b->value) திரும்பு +1;
    திரும்பு 0;
  }

  unsigned get_population () மாறிலி
  {
    எனில் (unlikely (last < first)) திரும்பு 0;
    திரும்பு (last - first + 1);
  }

  தருக்க intersects (மாறிலி hb_set_t &glyphs) மாறிலி
  { திரும்பு glyphs.intersects (first, last); }

  template <typename set_t>
  தருக்க collect_coverage (set_t *glyphs) மாறிலி
  { திரும்பு glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
வெளி HB_INTERNAL மாறிலி unsigned எழுத்து _hb_Null_OT_RangeRecord[9];
template <typename Spec, typename மதிப்பு>
கட்டமைப்பு Null<OT::Layout::Common::RangeRecord<Spec, மதிப்பு>> {
  நிலையான OT::Layout::Common::RangeRecord<Spec, மதிப்பு> மாறிலி & get_null () {
    திரும்பு *reinterpret_cast<மாறிலி OT::Layout::Common::RangeRecord<Spec, மதிப்பு> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
