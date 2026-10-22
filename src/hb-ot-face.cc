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

#અન્તર્ભાવ "hb-ot-face.hh"

#અન્તર્ભાવ "hb-ot-cmap-table.hh"
#અન્તર્ભાવ "hb-ot-glyf-table.hh"
#અન્તર્ભાવ "hb-ot-cff1-table.hh"
#અન્તર્ભાવ "hb-ot-cff2-table.hh"
#અન્તર્ભાવ "hb-ot-hmtx-table.hh"
#અન્તર્ભાવ "hb-ot-kern-table.hh"
#અન્તર્ભાવ "hb-ot-meta-table.hh"
#અન્તર્ભાવ "hb-ot-name-table.hh"
#અન્તર્ભાવ "hb-ot-post-table.hh"
#અન્તર્ભાવ "OT/Color/CBDT/CBDT.hh"
#અન્તર્ભાવ "OT/Color/COLR/COLR.hh"
#અન્તર્ભાવ "OT/Color/sbix/sbix.hh"
#અન્તર્ભાવ "OT/Color/svg/svg.hh"
#અન્તર્ભાવ "hb-ot-layout-gdef-table.hh"
#અન્તર્ભાવ "hb-ot-layout-gsub-table.hh"
#અન્તર્ભાવ "hb-ot-layout-gpos-table.hh"
#અન્તર્ભાવ "hb-ot-var-varc-table.hh"
#અન્તર્ભાવ "hb-aat-layout-kerx-table.hh"
#અન્તર્ભાવ "hb-aat-layout-morx-table.hh"


શૂન્ય hb_ot_face_t::init0 (hb_face_t *face)
{
  આ->face = face;
#define HB_OT_TABLE(Namespace, Type) Type.init0 ();
#અન્તર્ભાવ "hb-ot-face-table-list.hh"
#undef HB_OT_TABLE
}
શૂન્ય hb_ot_face_t::fini ()
{
#define HB_OT_TABLE(Namespace, Type) Type.fini ();
#અન્તર્ભાવ "hb-ot-face-table-list.hh"
#undef HB_OT_TABLE
}
