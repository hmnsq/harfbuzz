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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-face.hh"

#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-feat-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cff-interp-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cmap-table.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/COLR/COLR.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-glyf-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-head-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-hmtx-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-maxp-table.hh"

#ifndef HB_NO_VISIBILITY

#ଅନ୍ତର୍ଭାଵ "hb-ot-name-language-static.hh"

uint64_t ଅଚଳ _hb_NullPool[(HB_NULL_POOL_SIZE + ଆକାର (uint64_t) - 1) / ଆକାର (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_SIZE + ଆକାର (uint64_t) - 1) / ଆକାର (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_BYTES (OT, Index) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (OT, RangeRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, ClipRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

ଅଚଳ hb_codepoint_t minus_1 = -1;
ସ୍ଥିର ଅଚଳ unsigned ଅକ୍ଷର static_endchar_str[] = {OpCode_endchar};
ଅଚଳ unsigned ଅକ୍ଷର *endchar_str = static_endchar_str;

/* hb_face_t */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned
load_ଅଙ୍କ_glyphs_from_maxp (ଅଚଳ hb_face_t *face)
{
  ଫେରନ୍ତୁ face->table.maxp->get_ଅଙ୍କ_glyphs ();
}

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_face_t::load_ଅଙ୍କ_glyphs () ଅଚଳ
{
  ଅଙ୍କ_glyphs = load_ଅଙ୍କ_glyphs_from_maxp (ଏହି);
  ଫେରନ୍ତୁ ଅଙ୍କ_glyphs;
}

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_face_t::load_upem () ଅଚଳ
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = table.head->get_upem ();
  upem = ret;
  ଫେରନ୍ତୁ ret;
}

#endif

#endif /* HB_STATIC_CC pacify */
#endif /* HB_STATIC_CC guard */
