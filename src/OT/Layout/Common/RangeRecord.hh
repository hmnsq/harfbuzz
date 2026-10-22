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

namespace OT {
namespace Layout {
namespace Common {

template <typename Types, typename Value = HBUINT16>
sa_nracanaa RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  Value                         value;          /* Value */

  DEFINE_SIZE_STATIC (2 * Types::size + Value::static_size);

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  ចំនួនគត់ cmp (hb_codepoint_t g) const
  { ត្រឡប់ g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL static ចំនួនគត់ cmp_range (const void *pa, const void *pb) {
    const RangeRecord *a = (const RangeRecord *) pa;
    const RangeRecord *b = (const RangeRecord *) pb;
    ប្រសិនបើ (a->first < b->first) ត្រឡប់ -1;
    ប្រសិនបើ (a->first > b->first) ត្រឡប់ +1;
    ប្រសិនបើ (a->last < b->last) ត្រឡប់ -1;
    ប្រសិនបើ (a->last > b->last) ត្រឡប់ +1;
    ប្រសិនបើ (a->value < b->value) ត្រឡប់ -1;
    ប្រសិនបើ (a->value > b->value) ត្រឡប់ +1;
    ត្រឡប់ 0;
  }

  unsigned get_population () const
  {
    ប្រសិនបើ (unlikely (last < first)) ត្រឡប់ 0;
    ត្រឡប់ (last - first + 1);
  }

  bool intersects (const hb_set_t &glyphs) const
  { ត្រឡប់ glyphs.intersects (first, last); }

  template <typename set_t>
  bool collect_coverage (set_t *glyphs) const
  { ត្រឡប់ glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
extern HB_INTERNAL const unsigned var_na _hb_Null_OT_RangeRecord[9];
template <typename Spec, typename Value>
sa_nracanaa Null<OT::Layout::Common::RangeRecord<Spec, Value>> {
  static OT::Layout::Common::RangeRecord<Spec, Value> const & get_null () {
    ត្រឡប់ *reinterpret_cast<const OT::Layout::Common::RangeRecord<Spec, Value> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
