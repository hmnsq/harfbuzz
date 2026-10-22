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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cmap-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-glyf-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff1-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-hmtx-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-kern-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-meta-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-name-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-post-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/CBDT/CBDT.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/COLR/COLR.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/sbix/sbix.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/svg/svg.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gdef-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gsub-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gpos-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-varc-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-kerx-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-morx-table.hh"


ਖਾਲੀ hb_ot_face_t::init0 (hb_face_t *face)
{
  ਇਹ->face = face;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਸਾਰਣੀ(Namespace, Type) Type.init0 ();
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face-table-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_OT_ਸਾਰਣੀ
}
ਖਾਲੀ hb_ot_face_t::fini ()
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਸਾਰਣੀ(Namespace, Type) Type.fini ();
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face-table-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_OT_ਸਾਰਣੀ
}
