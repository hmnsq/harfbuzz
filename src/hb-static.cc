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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_STATIC_CC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STATIC_CC
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_STATIC_CC /* Pacify -Wunused-macros. */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-face.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-feat-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-interp-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cmap-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/COLR/COLR.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-glyf-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-head-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-hmtx-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-maxp-table.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VISIBILITY

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-name-language-static.hh"

uint64_t ਅਚਲ _hb_NullPool[(HB_NULL_POOL_ਮਾਪ + ਆਕਾਰ (uint64_t) - 1) / ਆਕਾਰ (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_ਮਾਪ + ਆਕਾਰ (uint64_t) - 1) / ਆਕਾਰ (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, ਸੂਚਕਾਂਕ) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, Range_ਰਿਕਾਰਡ) = {0x01};
DEFINE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, Clip_ਰਿਕਾਰਡ) = {0x01};
DEFINE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, Cmap_Subtable_Long_ਸਮੂਹ) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_ਬਾਈਟਾਂ (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_ਬਾਈਟਾਂ (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

ਅਚਲ hb_codepoint_t minus_1 = -1;
ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ static_endchar_str[] = {Op_ਕੋਡ_endchar};
ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *endchar_str = static_endchar_str;

/* hb_face_t */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
ਲੱਦੋ_ਅੰਕ_glyphs_from_maxp (ਅਚਲ hb_face_t *face)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.maxp->get_ਅੰਕ_glyphs ();
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_t::ਲੱਦੋ_ਅੰਕ_glyphs () ਅਚਲ
{
  ਅੰਕ_glyphs = ਲੱਦੋ_ਅੰਕ_glyphs_from_maxp (ਇਹ);
  ਵਾਪਸ ਅੰਕ_glyphs;
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_t::ਲੱਦੋ_upem () ਅਚਲ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ret = ਸਾਰਣੀ.ਸਿਰ_2->get_upem ();
  upem = ret;
  ਵਾਪਸ ret;
}

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* HB_STATIC_CC pacify */
#ਜੇ_ਅੰਤ /* HB_STATIC_CC guard */
