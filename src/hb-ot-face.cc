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

#అన్తర్భావ "hb-ot-face.hh"

#అన్తర్భావ "hb-ot-cmap-table.hh"
#అన్తర్భావ "hb-ot-glyf-table.hh"
#అన్తర్భావ "hb-ot-cff1-table.hh"
#అన్తర్భావ "hb-ot-cff2-table.hh"
#అన్తర్భావ "hb-ot-hmtx-table.hh"
#అన్తర్భావ "hb-ot-kern-table.hh"
#అన్తర్భావ "hb-ot-meta-table.hh"
#అన్తర్భావ "hb-ot-name-table.hh"
#అన్తర్భావ "hb-ot-post-table.hh"
#అన్తర్భావ "OT/Color/CBDT/CBDT.hh"
#అన్తర్భావ "OT/Color/COLR/COLR.hh"
#అన్తర్భావ "OT/Color/sbix/sbix.hh"
#అన్తర్భావ "OT/Color/svg/svg.hh"
#అన్తర్భావ "hb-ot-layout-gdef-table.hh"
#అన్తర్భావ "hb-ot-layout-gsub-table.hh"
#అన్తర్భావ "hb-ot-layout-gpos-table.hh"
#అన్తర్భావ "hb-ot-var-varc-table.hh"
#అన్తర్భావ "hb-aat-layout-kerx-table.hh"
#అన్తర్భావ "hb-aat-layout-morx-table.hh"


శూన్య hb_ot_face_t::init0 (hb_face_t *face)
{
  ఇది->face = face;
#నిర్వచించు HB_OT_పట్టిక(Namespace, Type) Type.init0 ();
#అన్తర్భావ "hb-ot-face-table-list.hh"
#నిర్వచనం_రద్దు HB_OT_పట్టిక
}
శూన్య hb_ot_face_t::fini ()
{
#నిర్వచించు HB_OT_పట్టిక(Namespace, Type) Type.fini ();
#అన్తర్భావ "hb-ot-face-table-list.hh"
#నిర్వచనం_రద్దు HB_OT_పట్టిక
}
