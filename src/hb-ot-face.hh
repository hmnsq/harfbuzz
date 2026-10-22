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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_FACE_HH
#وصف_ڪريو HB_OT_FACE_HH

#ضمن "hb.hh"

#ضمن "hb-machinery.hh"


/*
 * hb_ot_face_t
 */

/* Declare tables. */
#وصف_ڪريو HB_OT_جدول(Namespace, Type) نطاق Namespace { بنية Type; }
#وصف_ڪريو HB_OT_ACCELERATOR(Namespace, Type) HB_OT_جدول (Namespace, Type##_accelerator_t)
#ضمن "hb-ot-face-table-list.hh"
#وصف_رد HB_OT_ACCELERATOR
#وصف_رد HB_OT_جدول

بنية hb_ot_face_t
{
  HB_اندروني فراغ init0 (hb_face_t *face);
  HB_اندروني فراغ fini ();

#وصف_ڪريو HB_OT_جدول_ترتيب(Namespace, Type) \
    HB_PASTE (ترتيب_2, HB_PASTE (Namespace, HB_PASTE (_, Type)))
  تعداد ترتيب_t
  {
    ترتيب_ZERO,
#وصف_ڪريو HB_OT_جدول(Namespace, Type) HB_OT_جدول_ترتيب (Namespace, Type),
#ضمن "hb-ot-face-table-list.hh"
#وصف_رد HB_OT_جدول
  };

  hb_face_t *face; /* MUST be JUST before the lazy loaders. */
#وصف_ڪريو HB_OT_جدول(Namespace, Type) \
  hb_جدول_lazy_loader_t<Namespace::Type, HB_OT_جدول_ترتيب (Namespace, Type)> Type;
#وصف_ڪريو HB_OT_ACCELERATOR(Namespace, Type) \
  hb_face_lazy_loader_t<Namespace::Type##_accelerator_t, HB_OT_جدول_ترتيب (Namespace, Type)> Type;
#ضمن "hb-ot-face-table-list.hh"
#وصف_رد HB_OT_ACCELERATOR
#وصف_رد HB_OT_جدول
};


#جيڪڏهن_پورو /* HB_OT_FACE_HH */
