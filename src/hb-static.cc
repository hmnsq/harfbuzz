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

#شامل "hb.hh"

#شامل "hb-open-type.hh"
#شامل "hb-face.hh"

#شامل "hb-aat-layout-common.hh"
#شامل "hb-aat-layout-feat-table.hh"
#شامل "hb-cff-interp-common.hh"
#شامل "hb-ot-layout-common.hh"
#شامل "hb-ot-cmap-table.hh"
#شامل "OT/Color/COLR/COLR.hh"
#شامل "hb-ot-glyf-table.hh"
#شامل "hb-ot-head-table.hh"
#شامل "hb-ot-hmtx-table.hh"
#شامل "hb-ot-maxp-table.hh"

#ifndef HB_NO_VISIBILITY

#شامل "hb-ot-name-language-static.hh"

uint64_t ثابت _hb_NullPool[(HB_NULL_POOL_SIZE + اندازه (uint64_t) - 1) / اندازه (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_SIZE + اندازه (uint64_t) - 1) / اندازه (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_BYTES (OT, Index) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (OT, RangeRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, ClipRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

ثابت hb_codepoint_t minus_1 = -1;
ایستا ثابت unsigned نویسه static_endchar_str[] = {OpCode_endchar};
ثابت unsigned نویسه *endchar_str = static_endchar_str;

/* hb_face_t */

ایستا درونخطی unsigned
load_رقم_glyphs_from_maxp (ثابت hb_face_t *face)
{
  بازگشت face->table.maxp->get_رقم_glyphs ();
}

unsigned صحیح
hb_face_t::load_رقم_glyphs () ثابت
{
  رقم_glyphs = load_رقم_glyphs_from_maxp (این);
  بازگشت رقم_glyphs;
}

unsigned صحیح
hb_face_t::load_upem () ثابت
{
  unsigned صحیح ret = table.head->get_upem ();
  upem = ret;
  بازگشت ret;
}

#endif

#endif /* HB_STATIC_CC pacify */
#endif /* HB_STATIC_CC guard */
