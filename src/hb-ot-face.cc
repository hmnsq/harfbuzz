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

#ضمن "hb-ot-face.hh"

#ضمن "hb-ot-cmap-table.hh"
#ضمن "hb-ot-glyf-table.hh"
#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-ot-hmtx-table.hh"
#ضمن "hb-ot-kern-table.hh"
#ضمن "hb-ot-meta-table.hh"
#ضمن "hb-ot-name-table.hh"
#ضمن "hb-ot-post-table.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"
#ضمن "OT/Color/COLR/COLR.hh"
#ضمن "OT/Color/sbix/sbix.hh"
#ضمن "OT/Color/svg/svg.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsub-table.hh"
#ضمن "hb-ot-layout-gpos-table.hh"
#ضمن "hb-ot-var-varc-table.hh"
#ضمن "hb-aat-layout-kerx-table.hh"
#ضمن "hb-aat-layout-morx-table.hh"


فراغ hb_ot_face_t::init0 (hb_face_t *face)
{
  هذا->face = face;
#تعریف_کرو HB_OT_جدول(Namespace, Type) Type.init0 ();
#ضمن "hb-ot-face-table-list.hh"
#تعریف_رد HB_OT_جدول
}
فراغ hb_ot_face_t::fini ()
{
#تعریف_کرو HB_OT_جدول(Namespace, Type) Type.fini ();
#ضمن "hb-ot-face-table-list.hh"
#تعریف_رد HB_OT_جدول
}
