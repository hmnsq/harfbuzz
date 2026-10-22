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

#समावेश "hb-ot-face.hh"

#समावेश "hb-ot-cmap-table.hh"
#समावेश "hb-ot-glyf-table.hh"
#समावेश "hb-ot-cff1-table.hh"
#समावेश "hb-ot-cff2-table.hh"
#समावेश "hb-ot-hmtx-table.hh"
#समावेश "hb-ot-kern-table.hh"
#समावेश "hb-ot-meta-table.hh"
#समावेश "hb-ot-name-table.hh"
#समावेश "hb-ot-post-table.hh"
#समावेश "OT/Color/CBDT/CBDT.hh"
#समावेश "OT/Color/COLR/COLR.hh"
#समावेश "OT/Color/sbix/sbix.hh"
#समावेश "OT/Color/svg/svg.hh"
#समावेश "hb-ot-layout-gdef-table.hh"
#समावेश "hb-ot-layout-gsub-table.hh"
#समावेश "hb-ot-layout-gpos-table.hh"
#समावेश "hb-ot-var-varc-table.hh"
#समावेश "hb-aat-layout-kerx-table.hh"
#समावेश "hb-aat-layout-morx-table.hh"


व्योम hb_ot_face_t::init0 (hb_face_t *face)
{
  यह->face = face;
#घोषणा HB_OT_TABLE(Namespace, Type) Type.init0 ();
#समावेश "hb-ot-face-table-list.hh"
#अघोषित HB_OT_TABLE
}
व्योम hb_ot_face_t::fini ()
{
#घोषणा HB_OT_TABLE(Namespace, Type) Type.fini ();
#समावेश "hb-ot-face-table-list.hh"
#अघोषित HB_OT_TABLE
}
