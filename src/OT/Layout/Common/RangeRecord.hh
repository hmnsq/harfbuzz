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

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ Common {

template <typename Types, typename ಮೌಲ್ಯ = HBUINT16>
ಸಂರಚನಾ RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  ಮೌಲ್ಯ                         value;          /*ಮೌಲ್ಯ*/

  DEFINE_SIZE_STATIC (2 * Types::size + ಮೌಲ್ಯ::static_size);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t g) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp_range (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb) {
    ಧ್ರುವ RangeRecord *a = (ಧ್ರುವ RangeRecord *) pa;
    ಧ್ರುವ RangeRecord *b = (ಧ್ರುವ RangeRecord *) pb;
    ಯದಿ (a->first < b->first) ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (a->first > b->first) ಪ್ರತ್ಯಾಗಮ +1;
    ಯದಿ (a->last < b->last) ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (a->last > b->last) ಪ್ರತ್ಯಾಗಮ +1;
    ಯದಿ (a->value < b->value) ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (a->value > b->value) ಪ್ರತ್ಯಾಗಮ +1;
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  unsigned get_population () ಧ್ರುವ
  {
    ಯದಿ (unlikely (last < first)) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ (last - first + 1);
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t &glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ glyphs.intersects (first, last); }

  template <typename set_t>
  ತಾರ್ಕಿಕ collect_coverage (set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
ಬಾಹ್ಯ HB_INTERNAL ಧ್ರುವ unsigned ವರ್ಣ _hb_Null_OT_RangeRecord[9];
template <typename Spec, typename ಮೌಲ್ಯ>
ಸಂರಚನಾ Null<OT::Layout::Common::RangeRecord<Spec, ಮೌಲ್ಯ>> {
  ಸ್ಥಿರ OT::Layout::Common::RangeRecord<Spec, ಮೌಲ್ಯ> ಧ್ರುವ & get_null () {
    ಪ್ರತ್ಯಾಗಮ *reinterpret_cast<ಧ್ರುವ OT::Layout::Common::RangeRecord<Spec, ಮೌಲ್ಯ> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
