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

नामाकाश OT {
नामाकाश Layout {
नामाकाश Common {

template <typename Types, typename मानम् = HBUINT16>
संरचना RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  मानम्                         value;          /*मानम्*/

  DEFINE_SIZE_STATIC (2 * Types::size + मानम्::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  पूर्णाङ्क cmp (hb_codepoint_t g) ध्रुव
  { प्रत्यागम g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL स्थिर पूर्णाङ्क cmp_range (ध्रुव शून्य *pa, ध्रुव शून्य *pb) {
    ध्रुव RangeRecord *a = (ध्रुव RangeRecord *) pa;
    ध्रुव RangeRecord *b = (ध्रुव RangeRecord *) pb;
    जइ (a->first < b->first) प्रत्यागम -1;
    जइ (a->first > b->first) प्रत्यागम +1;
    जइ (a->last < b->last) प्रत्यागम -1;
    जइ (a->last > b->last) प्रत्यागम +1;
    जइ (a->value < b->value) प्रत्यागम -1;
    जइ (a->value > b->value) प्रत्यागम +1;
    प्रत्यागम 0;
  }

  unsigned get_population () ध्रुव
  {
    जइ (unlikely (last < first)) प्रत्यागम 0;
    प्रत्यागम (last - first + 1);
  }

  तार्किक intersects (ध्रुव hb_set_t &glyphs) ध्रुव
  { प्रत्यागम glyphs.intersects (first, last); }

  template <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) ध्रुव
  { प्रत्यागम glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
बाह्य HB_INTERNAL ध्रुव unsigned वर्ण _hb_Null_OT_RangeRecord[9];
template <typename Spec, typename मानम्>
संरचना Null<OT::Layout::Common::RangeRecord<Spec, मानम्>> {
  स्थिर OT::Layout::Common::RangeRecord<Spec, मानम्> ध्रुव & get_null () {
    प्रत्यागम *reinterpret_cast<ध्रुव OT::Layout::Common::RangeRecord<Spec, मानम्> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
