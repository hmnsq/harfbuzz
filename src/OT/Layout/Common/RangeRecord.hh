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

#اگر_تعریف_نشده OT_LAYOUT_COMMON_RANGERECORD_HH
#تعریف_کن OT_LAYOUT_COMMON_RANGERECORD_HH

نطاق OT {
نطاق Layout {
نطاق Common {

قالب <نام_نوع Types, نام_نوع مقدار = HBUINT16>
بنية Range_رکورد
{
  نام_نوع Types::HBGlyphID     اولی;          /* First GlyphID in the range */
  نام_نوع Types::HBGlyphID     آخرین;           /* Last GlyphID in the range */
  مقدار                         value;          /*مقدار*/

  DEFINE_اندازه_STATIC (2 * Types::اندازه + مقدار::static_اندازه);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < اولی ? -1 : g <= آخرین ? 0 : +1; }

  HB_داخلی ساكن صحيح cmp_range (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت Range_رکورد *a = (ثابت Range_رکورد *) pa;
    ثابت Range_رکورد *b = (ثابت Range_رکورد *) pb;
    إذا (a->اولی < b->اولی) أرجع -1;
    إذا (a->اولی > b->اولی) أرجع +1;
    إذا (a->آخرین < b->آخرین) أرجع -1;
    إذا (a->آخرین > b->آخرین) أرجع +1;
    إذا (a->value < b->value) أرجع -1;
    إذا (a->value > b->value) أرجع +1;
    أرجع 0;
  }

  بیعلامت get_population () ثابت
  {
    إذا (unlikely (آخرین < اولی)) أرجع 0;
    أرجع (آخرین - اولی + 1);
  }

  منطقي intersects (ثابت hb_مجموعه_t &glyphs) ثابت
  { أرجع glyphs.intersects (اولی, آخرین); }

  قالب <نام_نوع مجموعه_t>
  منطقي collect_coverage (مجموعه_t *glyphs) ثابت
  { أرجع glyphs->جمع_کن_range (اولی, آخرین); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
خارجي HB_داخلی ثابت بیعلامت حرف hb_Null_OT_Range_رکورد[9];
قالب <نام_نوع Spec, نام_نوع مقدار>
بنية Null<OT::Layout::Common::Range_رکورد<Spec, مقدار>> {
  ساكن OT::Layout::Common::Range_رکورد<Spec, مقدار> ثابت & get_null () {
    أرجع *reinterpret_cast<ثابت OT::Layout::Common::Range_رکورد<Spec, مقدار> *> (hb_Null_OT_Range_رکورد);
  }
};


#پایان_اگر  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
