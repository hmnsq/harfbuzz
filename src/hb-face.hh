/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2011  Google, Inc.
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

#جيڪڏهن_وصف_نه_ٿيل HB_FACE_HH
#وصف_ڪريو HB_FACE_HH

#ضمن "hb.hh"

#ضمن "hb-shaper.hh"
#ضمن "hb-shape-plan.hh"
#ضمن "hb-ot-face.hh"


/*
 * hb_face_t
 */

#وصف_ڪريو HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_معلومات_INSTANTIATE_SHAPERS(shaper, face);
#ضمن "hb-shaper-list.hh"
#وصف_رد HB_SHAPER_IMPLEMENT

بنية hb_face_t
{
  hb_object_سرو_t سرو;

  بنانشان صحيح اشاريو_2;			/* Face index in a collection, zero-based. */
  بدلجندڙ hb_atomic_t<بنانشان> upem;	/* Units-per-EM. */
  بدلجندڙ hb_atomic_t<بنانشان> انگ_glyphs;/* Number of glyphs. */

  hb_reference_جدول_func_t  reference_جدول_func;
  فراغ                      *user_معلومات;
  hb_destroy_func_t          destroy;

  hb_get_جدول_ٽيگ_فهرست_func_t   get_جدول_ٽيگ_فهرست_func;
  فراغ                      *get_جدول_ٽيگ_فهرست_user_معلومات;
  hb_destroy_func_t          get_جدول_ٽيگ_فهرست_destroy;

  hb_shaper_object_dataset_t<hb_face_t> معلومات;/* Various shaper data. */
  hb_ot_face_t جدول;			/* All the face's tables. */

  /* Cache */
  بنية plan_ڳنڍ_t
  {
    hb_shape_plan_t *shape_plan;
    plan_ڳنڍ_t *next;
  };
#جيڪڏهن_وصف_نه_ٿيل HB_NO_SHAPER
  hb_atomic_t<plan_ڳنڍ_t *> shape_plans;
#جيڪڏهن_پورو

  hb_blob_t *reference_جدول (hb_ٽيگ_t ٽيگ_2) ثابت
  {
    hb_blob_t *blob;

    إذا (unlikely (!reference_جدول_func))
      أرجع hb_blob_get_خالي_جايون ();

    blob = reference_جدول_func (/*Oh, well.*/const_cast<hb_face_t *> (هذا), ٽيگ_2, user_معلومات);
    إذا (unlikely (!blob))
      أرجع hb_blob_get_خالي_جايون ();

    أرجع blob;
  }

  بنانشان صحيح get_upem () ثابت
  {
    بنانشان صحيح ret = upem;
    إذا (unlikely (!ret))
    {
      أرجع ڀريو_upem ();
    }
    أرجع ret;
  }

  بنانشان صحيح get_انگ_glyphs () ثابت
  {
    بنانشان صحيح ret = انگ_glyphs;
    إذا (unlikely (ret == UINT_MAX))
      أرجع ڀريو_انگ_glyphs ();
    أرجع ret;
  }

  خاص:
  HB_اندروني بنانشان صحيح ڀريو_upem () ثابت;
  HB_اندروني بنانشان صحيح ڀريو_انگ_glyphs () ثابت;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#جيڪڏهن_پورو /* HB_FACE_HH */
