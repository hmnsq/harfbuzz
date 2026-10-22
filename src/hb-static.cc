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

#నిర్వచితం_కాకపోతే HB_STATIC_CC
#నిర్వచించు HB_STATIC_CC
#నిర్వచితమైతే HB_STATIC_CC /* Pacify -Wunused-macros. */

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-face.hh"

#అన్తర్భావ "hb-aat-layout-common.hh"
#అన్తర్భావ "hb-aat-layout-feat-table.hh"
#అన్తర్భావ "hb-cff-interp-common.hh"
#అన్తర్భావ "hb-ot-layout-common.hh"
#అన్తర్భావ "hb-ot-cmap-table.hh"
#అన్తర్భావ "OT/Color/COLR/COLR.hh"
#అన్తర్భావ "hb-ot-glyf-table.hh"
#అన్తర్భావ "hb-ot-head-table.hh"
#అన్తర్భావ "hb-ot-hmtx-table.hh"
#అన్తర్భావ "hb-ot-maxp-table.hh"

#నిర్వచితం_కాకపోతే HB_NO_VISIBILITY

#అన్తర్భావ "hb-ot-name-language-static.hh"

uint64_t స్థిరరాశి _hb_NullPool[(HB_NULL_POOL_కొలత + పరిమాణం (uint64_t) - 1) / పరిమాణం (uint64_t)] = {};
/*thread_local*/ uint64_t _hb_CrapPool[(HB_NULL_POOL_కొలత + పరిమాణం (uint64_t) - 1) / పరిమాణం (uint64_t)] = {};

DEFINE_NULL_NAMESPACE_బైట్లు (OT, సూచీ) =  {0xFF,0xFF};
DEFINE_NULL_NAMESPACE_బైట్లు (OT, VarIdx) =  {0xFF,0xFF,0xFF,0xFF};
DEFINE_NULL_NAMESPACE_బైట్లు (OT, LangSys) = {0x00,0x00, 0xFF,0xFF, 0x00,0x00};
DEFINE_NULL_NAMESPACE_బైట్లు (OT, Range_రికార్డు) = {0x01};
DEFINE_NULL_NAMESPACE_బైట్లు (OT, Clip_రికార్డు) = {0x01};
DEFINE_NULL_NAMESPACE_బైట్లు (OT, Cmap_Subtable_Long_సమూహం) = {0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};
DEFINE_NULL_NAMESPACE_బైట్లు (AAT, SettingName) = {0xFF,0xFF, 0xFF,0xFF};
DEFINE_NULL_NAMESPACE_బైట్లు (AAT, Lookup) = {0xFF,0xFF};


/* hb_map_t */

స్థిరరాశి hb_codepoint_t minus_1 = -1;
స్థిర స్థిరరాశి గుర్తులేని అక్షరం static_endchar_str[] = {Op_సంకేతం_endchar};
స్థిరరాశి గుర్తులేని అక్షరం *endchar_str = static_endchar_str;

/* hb_face_t */

స్థిర అంతఃవరుస గుర్తులేని
ఎక్కించు_అంకె_glyphs_from_maxp (స్థిరరాశి hb_face_t *face)
{
  తిరిగి face->పట్టిక.maxp->get_అంకె_glyphs ();
}

గుర్తులేని పూర్ణాంకం
hb_face_t::ఎక్కించు_అంకె_glyphs () స్థిరరాశి
{
  అంకె_glyphs = ఎక్కించు_అంకె_glyphs_from_maxp (ఇది);
  తిరిగి అంకె_glyphs;
}

గుర్తులేని పూర్ణాంకం
hb_face_t::ఎక్కించు_upem () స్థిరరాశి
{
  గుర్తులేని పూర్ణాంకం ret = పట్టిక.తల_2->get_upem ();
  upem = ret;
  తిరిగి ret;
}

#అయితే_ముగింపు

#అయితే_ముగింపు /* HB_STATIC_CC pacify */
#అయితే_ముగింపు /* HB_STATIC_CC guard */
