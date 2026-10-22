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

#அந்தர்பாவ "hb-ot-face.hh"

#அந்தர்பாவ "hb-ot-cmap-table.hh"
#அந்தர்பாவ "hb-ot-glyf-table.hh"
#அந்தர்பாவ "hb-ot-cff1-table.hh"
#அந்தர்பாவ "hb-ot-cff2-table.hh"
#அந்தர்பாவ "hb-ot-hmtx-table.hh"
#அந்தர்பாவ "hb-ot-kern-table.hh"
#அந்தர்பாவ "hb-ot-meta-table.hh"
#அந்தர்பாவ "hb-ot-name-table.hh"
#அந்தர்பாவ "hb-ot-post-table.hh"
#அந்தர்பாவ "OT/Color/CBDT/CBDT.hh"
#அந்தர்பாவ "OT/Color/COLR/COLR.hh"
#அந்தர்பாவ "OT/Color/sbix/sbix.hh"
#அந்தர்பாவ "OT/Color/svg/svg.hh"
#அந்தர்பாவ "hb-ot-layout-gdef-table.hh"
#அந்தர்பாவ "hb-ot-layout-gsub-table.hh"
#அந்தர்பாவ "hb-ot-layout-gpos-table.hh"
#அந்தர்பாவ "hb-ot-var-varc-table.hh"
#அந்தர்பாவ "hb-aat-layout-kerx-table.hh"
#அந்தர்பாவ "hb-aat-layout-morx-table.hh"


வெறுமை hb_ot_face_t::init0 (hb_face_t *face)
{
  இது->face = face;
#define HB_OT_TABLE(Namespace, Type) Type.init0 ();
#அந்தர்பாவ "hb-ot-face-table-list.hh"
#undef HB_OT_TABLE
}
வெறுமை hb_ot_face_t::fini ()
{
#define HB_OT_TABLE(Namespace, Type) Type.fini ();
#அந்தர்பாவ "hb-ot-face-table-list.hh"
#undef HB_OT_TABLE
}
