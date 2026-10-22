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

#অন্তর্ভাৱ "hb-ot-face.hh"

#অন্তর্ভাৱ "hb-ot-cmap-table.hh"
#অন্তর্ভাৱ "hb-ot-glyf-table.hh"
#অন্তর্ভাৱ "hb-ot-cff1-table.hh"
#অন্তর্ভাৱ "hb-ot-cff2-table.hh"
#অন্তর্ভাৱ "hb-ot-hmtx-table.hh"
#অন্তর্ভাৱ "hb-ot-kern-table.hh"
#অন্তর্ভাৱ "hb-ot-meta-table.hh"
#অন্তর্ভাৱ "hb-ot-name-table.hh"
#অন্তর্ভাৱ "hb-ot-post-table.hh"
#অন্তর্ভাৱ "OT/Color/CBDT/CBDT.hh"
#অন্তর্ভাৱ "OT/Color/COLR/COLR.hh"
#অন্তর্ভাৱ "OT/Color/sbix/sbix.hh"
#অন্তর্ভাৱ "OT/Color/svg/svg.hh"
#অন্তর্ভাৱ "hb-ot-layout-gdef-table.hh"
#অন্তর্ভাৱ "hb-ot-layout-gsub-table.hh"
#অন্তর্ভাৱ "hb-ot-layout-gpos-table.hh"
#অন্তর্ভাৱ "hb-ot-var-varc-table.hh"
#অন্তর্ভাৱ "hb-aat-layout-kerx-table.hh"
#অন্তর্ভাৱ "hb-aat-layout-morx-table.hh"


শূন্য hb_ot_face_t::init0 (hb_face_t *face)
{
  এতত্->face = face;
#define HB_OT_TABLE(Namespace, Type) Type.init0 ();
#অন্তর্ভাৱ "hb-ot-face-table-list.hh"
#undef HB_OT_TABLE
}
শূন্য hb_ot_face_t::fini ()
{
#define HB_OT_TABLE(Namespace, Type) Type.fini ();
#অন্তর্ভাৱ "hb-ot-face-table-list.hh"
#undef HB_OT_TABLE
}
