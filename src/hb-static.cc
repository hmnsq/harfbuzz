/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#اگر_تعریف_نشده HB_STATIC_CC
#تعریف_کن HB_STATIC_CC
#اگر_تعریف_شده HB_STATIC_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"

#ضمن "hb-open-type.hh"
#ضمن "hb-face.hh"

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-aat-layout-feat-table.hh"
#ضمن "hb-cff-interp-common.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-cmap-table.hh"
#ضمن "OT/Color/COLR/COLR.hh"
#ضمن "hb-ot-glyf-table.hh"
#ضمن "hb-ot-head-table.hh"
#ضمن "hb-ot-hmtx-table.hh"
#ضمن "hb-ot-maxp-table.hh"

#اگر_تعریف_نشده HB_NO_VISIBILITY

#ضمن "hb-ot-name-language-static.hh"

uint64_t ثابت _hb_NullPool[(HB_NULL_POOL_اندازه + حجم (uint64_t) - 1) / حجم (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_اندازه + حجم (uint64_t) - 1) / حجم (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_بایتها (OT, شاخص) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_بایتها (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_بایتها (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_بایتها (OT, Range_رکورد) = {0x01};
DEFINE_NULL_NAMESPACE_بایتها (OT, Clip_رکورد) = {0x01};
DEFINE_NULL_NAMESPACE_بایتها (OT, Cmap_Subtable_Long_گروه) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_بایتها (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_بایتها (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

ثابت hb_codepoint_t minus_1 = -1;
ساكن ثابت بیعلامت حرف static_endchar_str[] = {Op_کد_endchar};
ثابت بیعلامت حرف *endchar_str = static_endchar_str;

/* hb_face_t */

ساكن مضمن بیعلامت
بارگیری_رقم_glyphs_from_maxp (ثابت hb_face_t *face)
{
  أرجع face->جدول.maxp->get_رقم_glyphs ();
}

بیعلامت صحيح
hb_face_t::بارگیری_رقم_glyphs () ثابت
{
  رقم_glyphs = بارگیری_رقم_glyphs_from_maxp (هذا);
  أرجع رقم_glyphs;
}

بیعلامت صحيح
hb_face_t::بارگیری_upem () ثابت
{
  بیعلامت صحيح ret = جدول.سر_2->get_upem ();
  upem = ret;
  أرجع ret;
}

#پایان_اگر

#پایان_اگر /* HB_STATIC_CC pacify */
#پایان_اگر /* HB_STATIC_CC guard */
