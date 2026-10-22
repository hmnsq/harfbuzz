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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-face.hh"

#अन्तर्भाव "hb-aat-layout-common.hh"
#अन्तर्भाव "hb-aat-layout-feat-table.hh"
#अन्तर्भाव "hb-cff-interp-common.hh"
#अन्तर्भाव "hb-ot-layout-common.hh"
#अन्तर्भाव "hb-ot-cmap-table.hh"
#अन्तर्भाव "OT/Color/COLR/COLR.hh"
#अन्तर्भाव "hb-ot-glyf-table.hh"
#अन्तर्भाव "hb-ot-head-table.hh"
#अन्तर्भाव "hb-ot-hmtx-table.hh"
#अन्तर्भाव "hb-ot-maxp-table.hh"

#ifndef HB_NO_VISIBILITY

#अन्तर्भाव "hb-ot-name-language-static.hh"

uint64_t स्थिरांक _hb_NullPool[(HB_NULL_POOL_SIZE + आकार (uint64_t) - 1) / आकार (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_SIZE + आकार (uint64_t) - 1) / आकार (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_BYTES (OT, Index) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (OT, RangeRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, ClipRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

स्थिरांक hb_codepoint_t minus_1 = -1;
स्थिर स्थिरांक अचिन्हित अक्षर static_endchar_str[] = {OpCode_endchar};
स्थिरांक अचिन्हित अक्षर *endchar_str = static_endchar_str;

/* hb_face_t */

स्थिर ओळीत अचिन्हित
load_अंक_glyphs_from_maxp (स्थिरांक hb_face_t *face)
{
  परत face->table.maxp->get_अंक_glyphs ();
}

अचिन्हित पूर्णांक
hb_face_t::load_अंक_glyphs () स्थिरांक
{
  अंक_glyphs = load_अंक_glyphs_from_maxp (हे);
  परत अंक_glyphs;
}

अचिन्हित पूर्णांक
hb_face_t::load_upem () स्थिरांक
{
  अचिन्हित पूर्णांक ret = table.head->get_upem ();
  upem = ret;
  परत ret;
}

#endif

#endif /* HB_STATIC_CC pacify */
#endif /* HB_STATIC_CC guard */
