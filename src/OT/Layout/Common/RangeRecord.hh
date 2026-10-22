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

#اگر_غیر_تعریف_شدہ OT_LAYOUT_COMMON_RANGERECORD_HH
#تعریف_کرو OT_LAYOUT_COMMON_RANGERECORD_HH

نامخلا OT {
نامخلا Layout {
نامخلا Common {

سانچہ <قسم_نام Types, قسم_نام قدر = HBUINT16>
ساخت Range_ریکارڈ
{
  قسم_نام Types::HBGlyphID     پہلا;          /* First GlyphID in the range */
  قسم_نام Types::HBGlyphID     آخری;           /* Last GlyphID in the range */
  قدر                         value;          /*قدر*/

  DEFINE_سائز_STATIC (2 * Types::سائز + قدر::static_سائز);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  صحیح cmp (hb_codepoint_t g) ثابت
  { واپس g < پہلا ? -1 : g <= آخری ? 0 : +1; }

  HB_اندرونی ساکن صحیح cmp_range (ثابت خالی *pa, ثابت خالی *pb) {
    ثابت Range_ریکارڈ *a = (ثابت Range_ریکارڈ *) pa;
    ثابت Range_ریکارڈ *b = (ثابت Range_ریکارڈ *) pb;
    اگر (a->پہلا < b->پہلا) واپس -1;
    اگر (a->پہلا > b->پہلا) واپس +1;
    اگر (a->آخری < b->آخری) واپس -1;
    اگر (a->آخری > b->آخری) واپس +1;
    اگر (a->value < b->value) واپس -1;
    اگر (a->value > b->value) واپس +1;
    واپس 0;
  }

  غیرعلامتی get_population () ثابت
  {
    اگر (unlikely (آخری < پہلا)) واپس 0;
    واپس (آخری - پہلا + 1);
  }

  منطقی intersects (ثابت hb_سیٹ_t &glyphs) ثابت
  { واپس glyphs.intersects (پہلا, آخری); }

  سانچہ <قسم_نام سیٹ_t>
  منطقی collect_coverage (سیٹ_t *glyphs) ثابت
  { واپس glyphs->جوڑو_range (پہلا, آخری); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
خارجی HB_اندرونی ثابت غیرعلامتی حرف hb_Null_OT_Range_ریکارڈ[9];
سانچہ <قسم_نام Spec, قسم_نام قدر>
ساخت Null<OT::Layout::Common::Range_ریکارڈ<Spec, قدر>> {
  ساکن OT::Layout::Common::Range_ریکارڈ<Spec, قدر> ثابت & get_null () {
    واپس *reinterpret_cast<ثابت OT::Layout::Common::Range_ریکارڈ<Spec, قدر> *> (hb_Null_OT_Range_ریکارڈ);
  }
};


#اگر_ختم  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
