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

#که_نه_تعریف_شوی OT_LAYOUT_COMMON_RANGERECORD_HH
#تعریف_کړه OT_LAYOUT_COMMON_RANGERECORD_HH

نطاق OT {
نطاق Layout {
نطاق Common {

کينډۍ <ډول_نوم Types, ډول_نوم ارزښت = HBUINT16>
بنية Range_ریکارډ
{
  ډول_نوم Types::HBGlyphID     لومړی;          /* First GlyphID in the range */
  ډول_نوم Types::HBGlyphID     وروستی;           /* Last GlyphID in the range */
  ارزښت                         value;          /*ارزښت*/

  DEFINE_اندازه_STATIC (2 * Types::اندازه + ارزښت::static_اندازه);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < لومړی ? -1 : g <= وروستی ? 0 : +1; }

  HB_دننی ساكن صحيح cmp_range (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت Range_ریکارډ *a = (ثابت Range_ریکارډ *) pa;
    ثابت Range_ریکارډ *b = (ثابت Range_ریکارډ *) pb;
    إذا (a->لومړی < b->لومړی) أرجع -1;
    إذا (a->لومړی > b->لومړی) أرجع +1;
    إذا (a->وروستی < b->وروستی) أرجع -1;
    إذا (a->وروستی > b->وروستی) أرجع +1;
    إذا (a->value < b->value) أرجع -1;
    إذا (a->value > b->value) أرجع +1;
    أرجع 0;
  }

  بینښه get_population () ثابت
  {
    إذا (unlikely (وروستی < لومړی)) أرجع 0;
    أرجع (وروستی - لومړی + 1);
  }

  منطقي intersects (ثابت hb_سېټ_t &glyphs) ثابت
  { أرجع glyphs.intersects (لومړی, وروستی); }

  کينډۍ <ډول_نوم سېټ_t>
  منطقي collect_coverage (سېټ_t *glyphs) ثابت
  { أرجع glyphs->جمع_کړه_range (لومړی, وروستی); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
خارجي HB_دننی ثابت بینښه حرف hb_Null_OT_Range_ریکارډ[9];
کينډۍ <ډول_نوم Spec, ډول_نوم ارزښت>
بنية Null<OT::Layout::Common::Range_ریکارډ<Spec, ارزښت>> {
  ساكن OT::Layout::Common::Range_ریکارډ<Spec, ارزښت> ثابت & get_null () {
    أرجع *reinterpret_cast<ثابت OT::Layout::Common::Range_ریکارډ<Spec, ارزښت> *> (hb_Null_OT_Range_ریکارډ);
  }
};


#د_که_پای  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
