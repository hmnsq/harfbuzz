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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_FACE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_FACE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"


/*
 * hb_ot_face_t
 */

/* Declare tables. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਸਾਰਣੀ(Namespace, Type) ਨਾਮਾਕਾਸ਼ Namespace { ਬਣਤਰ Type; }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ACCELERATOR(Namespace, Type) HB_OT_ਸਾਰਣੀ (Namespace, Type##_accelerator_t)
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face-table-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_OT_ACCELERATOR
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_OT_ਸਾਰਣੀ

ਬਣਤਰ hb_ot_face_t
{
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ init0 (hb_face_t *face);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ fini ();

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਸਾਰਣੀ_ਕ੍ਰਮ(Namespace, Type) \
    HB_PASTE (ਕ੍ਰਮ_2, HB_PASTE (Namespace, HB_PASTE (_, Type)))
  ਗਿਣਤੀ ਕ੍ਰਮ_t
  {
    ਕ੍ਰਮ_ZERO,
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਸਾਰਣੀ(Namespace, Type) HB_OT_ਸਾਰਣੀ_ਕ੍ਰਮ (Namespace, Type),
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face-table-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_OT_ਸਾਰਣੀ
  };

  hb_face_t *face; /* MUST be JUST before the lazy loaders. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਸਾਰਣੀ(Namespace, Type) \
  hb_ਸਾਰਣੀ_lazy_loader_t<Namespace::Type, HB_OT_ਸਾਰਣੀ_ਕ੍ਰਮ (Namespace, Type)> Type;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ACCELERATOR(Namespace, Type) \
  hb_face_lazy_loader_t<Namespace::Type##_accelerator_t, HB_OT_ਸਾਰਣੀ_ਕ੍ਰਮ (Namespace, Type)> Type;
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face-table-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_OT_ACCELERATOR
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_OT_ਸਾਰਣੀ
};


#ਜੇ_ਅੰਤ /* HB_OT_FACE_HH */
