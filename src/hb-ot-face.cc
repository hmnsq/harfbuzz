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

#ಅನ್ತರ್ಭಾವ "hb-ot-face.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-cmap-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-glyf-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-cff1-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-cff2-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-hmtx-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-kern-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-meta-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-name-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-post-table.hh"
#ಅನ್ತರ್ಭಾವ "OT/Color/CBDT/CBDT.hh"
#ಅನ್ತರ್ಭಾವ "OT/Color/COLR/COLR.hh"
#ಅನ್ತರ್ಭಾವ "OT/Color/sbix/sbix.hh"
#ಅನ್ತರ್ಭಾವ "OT/Color/svg/svg.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gdef-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gsub-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gpos-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-varc-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-layout-kerx-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-layout-morx-table.hh"


ಶೂನ್ಯ hb_ot_face_t::init0 (hb_face_t *face)
{
  ಏತತ್->face = face;
#define HB_OT_TABLE(Namespace, Type) Type.init0 ();
#ಅನ್ತರ್ಭಾವ "hb-ot-face-table-list.hh"
#undef HB_OT_TABLE
}
ಶೂನ್ಯ hb_ot_face_t::fini ()
{
#define HB_OT_TABLE(Namespace, Type) Type.fini ();
#ಅನ್ತರ್ಭಾವ "hb-ot-face-table-list.hh"
#undef HB_OT_TABLE
}
