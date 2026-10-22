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

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ Common {

template <typename Types, typename മൂല്യം = HBUINT16>
ഘടന RangeRecord
{
  typename Types::HBGlyphID     first;          /* First GlyphID in the range */
  typename Types::HBGlyphID     last;           /* Last GlyphID in the range */
  മൂല്യം                         value;          /*മൂല്യം*/

  DEFINE_SIZE_STATIC (2 * Types::size + മൂല്യം::static_size);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൂർണ്ണസംഖ്യ cmp (hb_codepoint_t g) സ്ഥിരം
  { തിരികെ g < first ? -1 : g <= last ? 0 : +1; }

  HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp_range (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb) {
    സ്ഥിരം RangeRecord *a = (സ്ഥിരം RangeRecord *) pa;
    സ്ഥിരം RangeRecord *b = (സ്ഥിരം RangeRecord *) pb;
    എങ്കിൽ (a->first < b->first) തിരികെ -1;
    എങ്കിൽ (a->first > b->first) തിരികെ +1;
    എങ്കിൽ (a->last < b->last) തിരികെ -1;
    എങ്കിൽ (a->last > b->last) തിരികെ +1;
    എങ്കിൽ (a->value < b->value) തിരികെ -1;
    എങ്കിൽ (a->value > b->value) തിരികെ +1;
    തിരികെ 0;
  }

  unsigned get_population () സ്ഥിരം
  {
    എങ്കിൽ (unlikely (last < first)) തിരികെ 0;
    തിരികെ (last - first + 1);
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t &glyphs) സ്ഥിരം
  { തിരികെ glyphs.intersects (first, last); }

  template <typename set_t>
  തർക്കം collect_coverage (set_t *glyphs) സ്ഥിരം
  { തിരികെ glyphs->add_range (first, last); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
ബാഹ്യ HB_INTERNAL സ്ഥിരം unsigned അക്ഷരം _hb_Null_OT_RangeRecord[9];
template <typename Spec, typename മൂല്യം>
ഘടന Null<OT::Layout::Common::RangeRecord<Spec, മൂല്യം>> {
  സ്ഥിതം OT::Layout::Common::RangeRecord<Spec, മൂല്യം> സ്ഥിരം & get_null () {
    തിരികെ *reinterpret_cast<സ്ഥിരം OT::Layout::Common::RangeRecord<Spec, മൂല്യം> *> (_hb_Null_OT_RangeRecord);
  }
};


#endif  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
