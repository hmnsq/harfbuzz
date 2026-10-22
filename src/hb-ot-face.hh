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

#اگر_غیر_تعریف_شدہ HB_OT_FACE_HH
#تعریف_کرو HB_OT_FACE_HH

#شامل "hb.hh"

#شامل "hb-machinery.hh"


/*
 * hb_ot_face_t
 */

/* Declare tables. */
#تعریف_کرو HB_OT_جدول(Namespace, Type) نامخلا Namespace { ساخت Type; }
#تعریف_کرو HB_OT_ACCELERATOR(Namespace, Type) HB_OT_جدول (Namespace, Type##_accelerator_t)
#شامل "hb-ot-face-table-list.hh"
#تعریف_منسوخ HB_OT_ACCELERATOR
#تعریف_منسوخ HB_OT_جدول

ساخت hb_ot_face_t
{
  HB_اندرونی خالی init0 (hb_face_t *face);
  HB_اندرونی خالی fini ();

#تعریف_کرو HB_OT_جدول_ترتیب(Namespace, Type) \
    HB_PASTE (ترتیب_2, HB_PASTE (Namespace, HB_PASTE (_, Type)))
  شمار ترتیب_t
  {
    ترتیب_ZERO,
#تعریف_کرو HB_OT_جدول(Namespace, Type) HB_OT_جدول_ترتیب (Namespace, Type),
#شامل "hb-ot-face-table-list.hh"
#تعریف_منسوخ HB_OT_جدول
  };

  hb_face_t *face; /* MUST be JUST before the lazy loaders. */
#تعریف_کرو HB_OT_جدول(Namespace, Type) \
  hb_جدول_lazy_loader_t<Namespace::Type, HB_OT_جدول_ترتیب (Namespace, Type)> Type;
#تعریف_کرو HB_OT_ACCELERATOR(Namespace, Type) \
  hb_face_lazy_loader_t<Namespace::Type##_accelerator_t, HB_OT_جدول_ترتیب (Namespace, Type)> Type;
#شامل "hb-ot-face-table-list.hh"
#تعریف_منسوخ HB_OT_ACCELERATOR
#تعریف_منسوخ HB_OT_جدول
};


#اگر_ختم /* HB_OT_FACE_HH */
