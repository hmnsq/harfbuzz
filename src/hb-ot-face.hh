/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2012,2013  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#अगर_अघोषित HB_OT_FACE_HH
#घोषणा HB_OT_FACE_HH

#समावेश "hb.hh"

#समावेश "hb-machinery.hh"


/*
 * hb_ot_face_t
 */

/* Declare tables. */
#घोषणा HB_OT_TABLE(Namespace, Type) namespace Namespace { काष्ठा Type; }
#घोषणा HB_OT_ACCELERATOR(Namespace, Type) HB_OT_TABLE (Namespace, Type##_accelerator_t)
#समावेश "hb-ot-face-table-list.hh"
#अघोषित HB_OT_ACCELERATOR
#अघोषित HB_OT_TABLE

काष्ठा hb_ot_face_t
{
  HB_INTERNAL व्योम init0 (hb_face_t *face);
  HB_INTERNAL व्योम fini ();

#घोषणा HB_OT_TABLE_ORDER(Namespace, Type) \
    HB_PASTE (ORDER_, HB_PASTE (Namespace, HB_PASTE (_, Type)))
  क्रमागत order_t
  {
    ORDER_ZERO,
#घोषणा HB_OT_TABLE(Namespace, Type) HB_OT_TABLE_ORDER (Namespace, Type),
#समावेश "hb-ot-face-table-list.hh"
#अघोषित HB_OT_TABLE
  };

  hb_face_t *face; /* MUST be JUST before the lazy loaders. */
#घोषणा HB_OT_TABLE(Namespace, Type) \
  hb_table_lazy_loader_t<Namespace::Type, HB_OT_TABLE_ORDER (Namespace, Type)> Type;
#घोषणा HB_OT_ACCELERATOR(Namespace, Type) \
  hb_face_lazy_loader_t<Namespace::Type##_accelerator_t, HB_OT_TABLE_ORDER (Namespace, Type)> Type;
#समावेश "hb-ot-face-table-list.hh"
#अघोषित HB_OT_ACCELERATOR
#अघोषित HB_OT_TABLE
};


#पूर्ण_अगर /* HB_OT_FACE_HH */
