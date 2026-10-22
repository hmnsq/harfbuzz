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

#اگر_تعریف_نشده HB_FACE_HH
#تعریف_کن HB_FACE_HH

#ضمن "hb.hh"

#ضمن "hb-shaper.hh"
#ضمن "hb-shape-plan.hh"
#ضمن "hb-ot-face.hh"


/*
 * hb_face_t
 */

#تعریف_کن HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_داده_INSTANTIATE_SHAPERS(shaper, face);
#ضمن "hb-shaper-list.hh"
#لغو_تعریف HB_SHAPER_IMPLEMENT

بنية hb_face_t
{
  hb_object_سرآیند_t سرآیند;

  بیعلامت صحيح شاخص_2;			/* Face index in a collection, zero-based. */
  تغییرپذیر hb_atomic_t<بیعلامت> upem;	/* Units-per-EM. */
  تغییرپذیر hb_atomic_t<بیعلامت> رقم_glyphs;/* Number of glyphs. */

  hb_reference_جدول_func_t  reference_جدول_func;
  فراغ                      *user_داده;
  hb_destroy_func_t          destroy;

  hb_get_جدول_برچسبها_func_t   get_جدول_برچسبها_func;
  فراغ                      *get_جدول_برچسبها_user_داده;
  hb_destroy_func_t          get_جدول_برچسبها_destroy;

  hb_shaper_object_dataset_t<hb_face_t> داده;/* Various shaper data. */
  hb_ot_face_t جدول;			/* All the face's tables. */

  /* Cache */
  بنية plan_گره_t
  {
    hb_shape_plan_t *shape_plan;
    plan_گره_t *next;
  };
#اگر_تعریف_نشده HB_NO_SHAPER
  hb_atomic_t<plan_گره_t *> shape_plans;
#پایان_اگر

  hb_blob_t *reference_جدول (hb_برچسب_t برچسب_2) ثابت
  {
    hb_blob_t *blob;

    إذا (unlikely (!reference_جدول_func))
      أرجع hb_blob_get_خالی ();

    blob = reference_جدول_func (/*Oh, well.*/const_cast<hb_face_t *> (هذا), برچسب_2, user_داده);
    إذا (unlikely (!blob))
      أرجع hb_blob_get_خالی ();

    أرجع blob;
  }

  بیعلامت صحيح get_upem () ثابت
  {
    بیعلامت صحيح ret = upem;
    إذا (unlikely (!ret))
    {
      أرجع بارگیری_upem ();
    }
    أرجع ret;
  }

  بیعلامت صحيح get_رقم_glyphs () ثابت
  {
    بیعلامت صحيح ret = رقم_glyphs;
    إذا (unlikely (ret == UINT_MAX))
      أرجع بارگیری_رقم_glyphs ();
    أرجع ret;
  }

  خاص:
  HB_داخلی بیعلامت صحيح بارگیری_upem () ثابت;
  HB_داخلی بیعلامت صحيح بارگیری_رقم_glyphs () ثابت;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#پایان_اگر /* HB_FACE_HH */
