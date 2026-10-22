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

#ଅନ୍ତର୍ଭାଵ "hb-ot-face.hh"

#ଅନ୍ତର୍ଭାଵ "hb-ot-cmap-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-glyf-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff1-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-hmtx-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-kern-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-meta-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-name-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-post-table.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/CBDT/CBDT.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/COLR/COLR.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/sbix/sbix.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/svg/svg.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gdef-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gsub-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gpos-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-varc-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-kerx-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-morx-table.hh"


ଶୂନ୍ୟ hb_ot_face_t::init0 (hb_face_t *face)
{
  ଏହି->face = face;
#define HB_OT_TABLE(Namespace, Type) Type.init0 ();
#ଅନ୍ତର୍ଭାଵ "hb-ot-face-table-list.hh"
#undef HB_OT_TABLE
}
ଶୂନ୍ୟ hb_ot_face_t::fini ()
{
#define HB_OT_TABLE(Namespace, Type) Type.fini ();
#ଅନ୍ତର୍ଭାଵ "hb-ot-face-table-list.hh"
#undef HB_OT_TABLE
}
