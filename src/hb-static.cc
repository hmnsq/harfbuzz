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

#अगर_अघोषित HB_STATIC_CC
#घोषणा HB_STATIC_CC
#अगर_घोषित HB_STATIC_CC /* Pacify -Wunused-macros. */

#समावेश "hb.hh"

#समावेश "hb-open-type.hh"
#समावेश "hb-face.hh"

#समावेश "hb-aat-layout-common.hh"
#समावेश "hb-aat-layout-feat-table.hh"
#समावेश "hb-cff-interp-common.hh"
#समावेश "hb-ot-layout-common.hh"
#समावेश "hb-ot-cmap-table.hh"
#समावेश "OT/Color/COLR/COLR.hh"
#समावेश "hb-ot-glyf-table.hh"
#समावेश "hb-ot-head-table.hh"
#समावेश "hb-ot-hmtx-table.hh"
#समावेश "hb-ot-maxp-table.hh"

#अगर_अघोषित HB_NO_VISIBILITY

#समावेश "hb-ot-name-language-static.hh"

uint64_t स्थिर _hb_NullPool[(HB_NULL_POOL_आकार + माप (uint64_t) - 1) / माप (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_आकार + माप (uint64_t) - 1) / माप (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_BYTES (OT, सूचकांक) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (OT, RangeRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, ClipRecord) = {0x01};
DEFINE_NULL_NAMESPACE_BYTES (OT, CmapSubtableLongGroup) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_BYTES (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_BYTES (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

स्थिर hb_codepoint_t minus_1 = -1;
अटल स्थिर अचिन्हित अक्षर static_endchar_str[] = {OpCode_endchar};
स्थिर अचिन्हित अक्षर *endchar_str = static_endchar_str;

/* hb_face_t */

अटल अंतरभूत अचिन्हित
load_अंक_glyphs_from_maxp (स्थिर hb_face_t *face)
{
  वापस face->table.maxp->get_अंक_glyphs ();
}

अचिन्हित पूर्णांक
hb_face_t::load_अंक_glyphs () स्थिर
{
  अंक_glyphs = load_अंक_glyphs_from_maxp (यह);
  वापस अंक_glyphs;
}

अचिन्हित पूर्णांक
hb_face_t::load_upem () स्थिर
{
  अचिन्हित पूर्णांक ret = table.शीर्ष_2->get_upem ();
  upem = ret;
  वापस ret;
}

#पूर्ण_अगर

#पूर्ण_अगर /* HB_STATIC_CC pacify */
#पूर्ण_अगर /* HB_STATIC_CC guard */
