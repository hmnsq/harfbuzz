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

ढाँचा <typename Types, typename मूल्य = HBUINT16>
संरचना RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  मूल्य                         value;          /*मूल्य*/

  DEFINE_SIZE_STATIC (2 * Types::size + मूल्य::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  पूर्णांक cmp (hb_codepoint_t g) स्थिरांक
  { परत g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL स्थिर पूर्णांक cmp_range (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb) {
    स्थिरांक RangeRecord *a = (स्थिरांक RangeRecord *) pa;
    स्थिरांक RangeRecord *b = (स्थिरांक RangeRecord *) pb;
    जर (a->first < b->first) परत -1;
    जर (a->first > b->first) परत +1;
    जर (a->last < b->last) परत -1;
    जर (a->last > b->last) परत +1;
    जर (a->value < b->value) परत -1;
    जर (a->value > b->value) परत +1;
    परत 0;
  }

  अचिन्हित get_population () स्थिरांक
  {
    जर (unlikely (last < first)) परत 0;
    परत (last - first + 1);
  }

  तार्किक intersects (स्थिरांक hb_set_t &glyphs) स्थिरांक
  { परत glyphs.intersects (first, last); }

  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) स्थिरांक
  { परत glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
बाह्य HB_INTERNAL स्थिरांक अचिन्हित अक्षर _hb_Null_OT_RangeRecord[9];
ढाँचा <typename Spec, typename मूल्य>
संरचना Null<OT::Layout::Common::RangeRecord<Spec, मूल्य>> {
  स्थिर OT::Layout::Common::RangeRecord<Spec, मूल्य> स्थिरांक & get_null () {
    परत *reinterpret_cast<स्थिरांक OT::Layout::Common::RangeRecord<Spec, मूल्य> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
