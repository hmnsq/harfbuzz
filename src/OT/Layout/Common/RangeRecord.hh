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

ढाँचा <typename Types, typename मान = HBUINT16>
संरचना RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  मान                         value;          /*मान*/

  DEFINE_SIZE_STATIC (2 * Types::size + मान::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  पूर्णाङ्क cmp (hb_codepoint_t g) अचर
  { फर्काऊ g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL स्थिर पूर्णाङ्क cmp_range (अचर शून्य *pa, अचर शून्य *pb) {
    अचर RangeRecord *a = (अचर RangeRecord *) pa;
    अचर RangeRecord *b = (अचर RangeRecord *) pb;
    यदि (a->first < b->first) फर्काऊ -1;
    यदि (a->first > b->first) फर्काऊ +1;
    यदि (a->last < b->last) फर्काऊ -1;
    यदि (a->last > b->last) फर्काऊ +1;
    यदि (a->value < b->value) फर्काऊ -1;
    यदि (a->value > b->value) फर्काऊ +1;
    फर्काऊ 0;
  }

  अचिन्हित get_population () अचर
  {
    यदि (unlikely (last < first)) फर्काऊ 0;
    फर्काऊ (last - first + 1);
  }

  तार्किक intersects (अचर hb_set_t &glyphs) अचर
  { फर्काऊ glyphs.intersects (first, last); }

  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) अचर
  { फर्काऊ glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
बाह्य HB_INTERNAL अचर अचिन्हित वर्ण _hb_Null_OT_RangeRecord[9];
ढाँचा <typename Spec, typename मान>
संरचना Null<OT::Layout::Common::RangeRecord<Spec, मान>> {
  स्थिर OT::Layout::Common::RangeRecord<Spec, मान> अचर & get_null () {
    फर्काऊ *reinterpret_cast<अचर OT::Layout::Common::RangeRecord<Spec, मान> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
