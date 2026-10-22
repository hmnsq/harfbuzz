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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-face.hh"

#ಅನ್ತರ್ಭಾವ "hb-aat-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-layout-feat-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-cff-interp-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-cmap-table.hh"
#ಅನ್ತರ್ಭಾವ "OT/Color/COLR/COLR.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-glyf-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-head-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-hmtx-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-maxp-table.hh"

#ifndef HB_NO_VISIBILITY

#ಅನ್ತರ್ಭಾವ "hb-ot-name-language-static.hh"

uint64_t ಧ್ರುವ _hb_NullPool[(HB_NULL_POOL_SIZE + ಪರಿಮಾಣ (uint64_t) - 1) / ಪರಿಮಾಣ (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_SIZE + ಪರಿಮಾಣ (uint64_t) - 1) / ಪರಿಮಾಣ (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_BYTES (OT, Index) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (OT, RangeRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, ClipRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

ಧ್ರುವ hb_codepoint_t minus_1 = -1;
ಸ್ಥಿರ ಧ್ರುವ unsigned ವರ್ಣ static_endchar_str[] = {OpCode_endchar};
ಧ್ರುವ unsigned ವರ್ಣ *endchar_str = static_endchar_str;

/* hb_face_t */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned
load_ಅಂಕೆ_glyphs_from_maxp (ಧ್ರುವ hb_face_t *face)
{
  ಪ್ರತ್ಯಾಗಮ face->table.maxp->get_ಅಂಕೆ_glyphs ();
}

unsigned ಪೂರ್ಣಾಙ್ಕ
hb_face_t::load_ಅಂಕೆ_glyphs () ಧ್ರುವ
{
  ಅಂಕೆ_glyphs = load_ಅಂಕೆ_glyphs_from_maxp (ಏತತ್);
  ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_glyphs;
}

unsigned ಪೂರ್ಣಾಙ್ಕ
hb_face_t::load_upem () ಧ್ರುವ
{
  unsigned ಪೂರ್ಣಾಙ್ಕ ret = table.head->get_upem ();
  upem = ret;
  ಪ್ರತ್ಯಾಗಮ ret;
}

#endif

#endif /* HB_STATIC_CC pacify */
#endif /* HB_STATIC_CC guard */
