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

#جے_بنا_تعریف HB_STATIC_CC
#تعریف_کرو HB_STATIC_CC
#جے_تعریف_شدہ HB_STATIC_CC /* Pacify -Wunused-macros. */

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

#جے_بنا_تعریف HB_NO_VISIBILITY

#ضمن "hb-ot-name-language-static.hh"

uint64_t ثابت _hb_NullPool[(HB_NULL_POOL_ناپ + حجم (uint64_t) - 1) / حجم (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_ناپ + حجم (uint64_t) - 1) / حجم (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_بائٹاں (OT, اشاریہ) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_بائٹاں (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_بائٹاں (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_بائٹاں (OT, Range_ریکارڈ) = {0x01};
DEFINE_NULL_NAMESPACE_بائٹاں (OT, Clip_ریکارڈ) = {0x01};
DEFINE_NULL_NAMESPACE_بائٹاں (OT, Cmap_Subtable_Long_گروہ) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_بائٹاں (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_بائٹاں (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

ثابت hb_codepoint_t minus_1 = -1;
ساكن ثابت بنانشانی حرف static_endchar_str[] = {Op_کوڈ_endchar};
ثابت بنانشانی حرف *endchar_str = static_endchar_str;

/* hb_face_t */

ساكن مضمن بنانشانی
لدو_عدد_glyphs_from_maxp (ثابت hb_face_t *face)
{
  أرجع face->جدول.maxp->get_عدد_glyphs ();
}

بنانشانی صحيح
hb_face_t::لدو_عدد_glyphs () ثابت
{
  عدد_glyphs = لدو_عدد_glyphs_from_maxp (هذا);
  أرجع عدد_glyphs;
}

بنانشانی صحيح
hb_face_t::لدو_upem () ثابت
{
  بنانشانی صحيح ret = جدول.سر_2->get_upem ();
  upem = ret;
  أرجع ret;
}

#جے_انت

#جے_انت /* HB_STATIC_CC pacify */
#جے_انت /* HB_STATIC_CC guard */
