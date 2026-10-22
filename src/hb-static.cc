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

#ifndef HB_STATIC_CC
#define HB_STATIC_CC
#ifdef HB_STATIC_CC /* Pacify -Wunused-macros. */

#অন্তর্ভাৱ "hb.hh"

#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-face.hh"

#অন্তর্ভাৱ "hb-aat-layout-common.hh"
#অন্তর্ভাৱ "hb-aat-layout-feat-table.hh"
#অন্তর্ভাৱ "hb-cff-interp-common.hh"
#অন্তর্ভাৱ "hb-ot-layout-common.hh"
#অন্তর্ভাৱ "hb-ot-cmap-table.hh"
#অন্তর্ভাৱ "OT/Color/COLR/COLR.hh"
#অন্তর্ভাৱ "hb-ot-glyf-table.hh"
#অন্তর্ভাৱ "hb-ot-head-table.hh"
#অন্তর্ভাৱ "hb-ot-hmtx-table.hh"
#অন্তর্ভাৱ "hb-ot-maxp-table.hh"

#ifndef HB_NO_VISIBILITY

#অন্তর্ভাৱ "hb-ot-name-language-static.hh"

uint64_t ধ্রুৱ _hb_NullPool[(HB_NULL_POOL_SIZE + পরিমাণ (uint64_t) - 1) / পরিমাণ (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_SIZE + পরিমাণ (uint64_t) - 1) / পরিমাণ (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_BYTES (OT, Index) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (OT, RangeRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, ClipRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

ধ্রুৱ hb_codepoint_t minus_1 = -1;
স্থির ধ্রুৱ unsigned ৱর্ণ static_endchar_str[] = {OpCode_endchar};
ধ্রুৱ unsigned ৱর্ণ *endchar_str = static_endchar_str;

/* hb_face_t */

স্থির অন্তঃস্থ unsigned
load_অঙ্ক_glyphs_from_maxp (ধ্রুৱ hb_face_t *face)
{
  ঘূৰাই face->table.maxp->get_অঙ্ক_glyphs ();
}

unsigned পূর্ণাঙ্ক
hb_face_t::load_অঙ্ক_glyphs () ধ্রুৱ
{
  অঙ্ক_glyphs = load_অঙ্ক_glyphs_from_maxp (এতত্);
  ঘূৰাই অঙ্ক_glyphs;
}

unsigned পূর্ণাঙ্ক
hb_face_t::load_upem () ধ্রুৱ
{
  unsigned পূর্ণাঙ্ক ret = table.head->get_upem ();
  upem = ret;
  ঘূৰাই ret;
}

#endif

#endif /* HB_STATIC_CC pacify */
#endif /* HB_STATIC_CC guard */
