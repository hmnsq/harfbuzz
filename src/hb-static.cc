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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-face.hh"

#அந்தர்பாவ "hb-aat-layout-common.hh"
#அந்தர்பாவ "hb-aat-layout-feat-table.hh"
#அந்தர்பாவ "hb-cff-interp-common.hh"
#அந்தர்பாவ "hb-ot-layout-common.hh"
#அந்தர்பாவ "hb-ot-cmap-table.hh"
#அந்தர்பாவ "OT/Color/COLR/COLR.hh"
#அந்தர்பாவ "hb-ot-glyf-table.hh"
#அந்தர்பாவ "hb-ot-head-table.hh"
#அந்தர்பாவ "hb-ot-hmtx-table.hh"
#அந்தர்பாவ "hb-ot-maxp-table.hh"

#ifndef HB_NO_VISIBILITY

#அந்தர்பாவ "hb-ot-name-language-static.hh"

uint64_t மாறிலி _hb_NullPool[(HB_NULL_POOL_SIZE + அளவு (uint64_t) - 1) / அளவு (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_SIZE + அளவு (uint64_t) - 1) / அளவு (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_BYTES (OT, Index) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (OT, RangeRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, ClipRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

மாறிலி hb_codepoint_t minus_1 = -1;
நிலையான மாறிலி unsigned எழுத்து static_endchar_str[] = {OpCode_endchar};
மாறிலி unsigned எழுத்து *endchar_str = static_endchar_str;

/* hb_face_t */

நிலையான உள்வரி unsigned
load_இலக்கம்_glyphs_from_maxp (மாறிலி hb_face_t *face)
{
  திரும்பு face->table.maxp->get_இலக்கம்_glyphs ();
}

unsigned முழு
hb_face_t::load_இலக்கம்_glyphs () மாறிலி
{
  இலக்கம்_glyphs = load_இலக்கம்_glyphs_from_maxp (இது);
  திரும்பு இலக்கம்_glyphs;
}

unsigned முழு
hb_face_t::load_upem () மாறிலி
{
  unsigned முழு ret = table.head->get_upem ();
  upem = ret;
  திரும்பு ret;
}

#endif

#endif /* HB_STATIC_CC pacify */
#endif /* HB_STATIC_CC guard */
