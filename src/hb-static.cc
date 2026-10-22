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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-face.hh"

#અન્તર્ભાવ "hb-aat-layout-common.hh"
#અન્તર્ભાવ "hb-aat-layout-feat-table.hh"
#અન્તર્ભાવ "hb-cff-interp-common.hh"
#અન્તર્ભાવ "hb-ot-layout-common.hh"
#અન્તર્ભાવ "hb-ot-cmap-table.hh"
#અન્તર્ભાવ "OT/Color/COLR/COLR.hh"
#અન્તર્ભાવ "hb-ot-glyf-table.hh"
#અન્તર્ભાવ "hb-ot-head-table.hh"
#અન્તર્ભાવ "hb-ot-hmtx-table.hh"
#અન્તર્ભાવ "hb-ot-maxp-table.hh"

#ifndef HB_NO_VISIBILITY

#અન્તર્ભાવ "hb-ot-name-language-static.hh"

uint64_t અચલ _hb_NullPool[(HB_NULL_POOL_SIZE + કદ (uint64_t) - 1) / કદ (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_SIZE + કદ (uint64_t) - 1) / કદ (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_BYTES (OT, Index) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (OT, RangeRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, ClipRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

અચલ hb_codepoint_t minus_1 = -1;
સ્થિર અચલ unsigned અક્ષર static_endchar_str[] = {OpCode_endchar};
અચલ unsigned અક્ષર *endchar_str = static_endchar_str;

/* hb_face_t */

સ્થિર અંતઃરેખ unsigned
load_અંક_glyphs_from_maxp (અચલ hb_face_t *face)
{
  પાછા face->table.maxp->get_અંક_glyphs ();
}

unsigned પૂર્ણાંક
hb_face_t::load_અંક_glyphs () અચલ
{
  અંક_glyphs = load_અંક_glyphs_from_maxp (આ);
  પાછા અંક_glyphs;
}

unsigned પૂર્ણાંક
hb_face_t::load_upem () અચલ
{
  unsigned પૂર્ણાંક ret = table.head->get_upem ();
  upem = ret;
  પાછા ret;
}

#endif

#endif /* HB_STATIC_CC pacify */
#endif /* HB_STATIC_CC guard */
