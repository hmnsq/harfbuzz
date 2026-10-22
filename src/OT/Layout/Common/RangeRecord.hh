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

#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_COMMON_RANGERECORD_HH
#وصف_ڪريو OT_LAYOUT_COMMON_RANGERECORD_HH

نطاق OT {
نطاق Layout {
نطاق Common {

سانچو <قسم_نالو Types, قسم_نالو قدر = HBUINT16>
بنية Range_رڪارڊ
{
  قسم_نالو Types::HBGlyphID     پهريون;          /* First GlyphID in the range */
  قسم_نالو Types::HBGlyphID     آخري;           /* Last GlyphID in the range */
  قدر                         value;          /*قدر*/

  DEFINE_سائيز_STATIC (2 * Types::سائيز + قدر::static_سائيز);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < پهريون ? -1 : g <= آخري ? 0 : +1; }

  HB_اندروني ساكن صحيح cmp_range (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت Range_رڪارڊ *a = (ثابت Range_رڪارڊ *) pa;
    ثابت Range_رڪارڊ *b = (ثابت Range_رڪارڊ *) pb;
    إذا (a->پهريون < b->پهريون) أرجع -1;
    إذا (a->پهريون > b->پهريون) أرجع +1;
    إذا (a->آخري < b->آخري) أرجع -1;
    إذا (a->آخري > b->آخري) أرجع +1;
    إذا (a->value < b->value) أرجع -1;
    إذا (a->value > b->value) أرجع +1;
    أرجع 0;
  }

  بنانشان get_population () ثابت
  {
    إذا (unlikely (آخري < پهريون)) أرجع 0;
    أرجع (آخري - پهريون + 1);
  }

  منطقي intersects (ثابت hb_سيٽ_t &glyphs) ثابت
  { أرجع glyphs.intersects (پهريون, آخري); }

  سانچو <قسم_نالو سيٽ_t>
  منطقي collect_coverage (سيٽ_t *glyphs) ثابت
  { أرجع glyphs->جوڙيو_range (پهريون, آخري); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
خارجي HB_اندروني ثابت بنانشان حرف hb_Null_OT_Range_رڪارڊ[9];
سانچو <قسم_نالو Spec, قسم_نالو قدر>
بنية Null<OT::Layout::Common::Range_رڪارڊ<Spec, قدر>> {
  ساكن OT::Layout::Common::Range_رڪارڊ<Spec, قدر> ثابت & get_null () {
    أرجع *reinterpret_cast<ثابت OT::Layout::Common::Range_رڪارڊ<Spec, قدر> *> (hb_Null_OT_Range_رڪارڊ);
  }
};


#جيڪڏهن_پورو  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
