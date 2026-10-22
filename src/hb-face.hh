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

#جے_بنا_تعریف HB_FACE_HH
#تعریف_کرو HB_FACE_HH

#ضمن "hb.hh"

#ضمن "hb-shaper.hh"
#ضمن "hb-shape-plan.hh"
#ضمن "hb-ot-face.hh"


/*
 * hb_face_t
 */

#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_انکڑے_INSTANTIATE_SHAPERS(shaper, face);
#ضمن "hb-shaper-list.hh"
#تعریف_رد HB_SHAPER_IMPLEMENT

بنية hb_face_t
{
  hb_object_سرخی_t سرخی;

  بنانشانی صحيح اشاریہ_2;			/* Face index in a collection, zero-based. */
  بدلنجوگ hb_atomic_t<بنانشانی> upem;	/* Units-per-EM. */
  بدلنجوگ hb_atomic_t<بنانشانی> عدد_glyphs;/* Number of glyphs. */

  hb_reference_جدول_func_t  reference_جدول_func;
  فراغ                      *user_انکڑے;
  hb_destroy_func_t          destroy;

  hb_get_جدول_ٹیگ_سوچی_func_t   get_جدول_ٹیگ_سوچی_func;
  فراغ                      *get_جدول_ٹیگ_سوچی_user_انکڑے;
  hb_destroy_func_t          get_جدول_ٹیگ_سوچی_destroy;

  hb_shaper_object_dataset_t<hb_face_t> انکڑے;/* Various shaper data. */
  hb_ot_face_t جدول;			/* All the face's tables. */

  /* Cache */
  بنية plan_گرہ_t
  {
    hb_shape_plan_t *shape_plan;
    plan_گرہ_t *next;
  };
#جے_بنا_تعریف HB_NO_SHAPER
  hb_atomic_t<plan_گرہ_t *> shape_plans;
#جے_انت

  hb_blob_t *reference_جدول (hb_ٹیگ_t ٹیگ_2) ثابت
  {
    hb_blob_t *blob;

    إذا (unlikely (!reference_جدول_func))
      أرجع hb_blob_get_خالی_تھاواں ();

    blob = reference_جدول_func (/*Oh, well.*/const_cast<hb_face_t *> (هذا), ٹیگ_2, user_انکڑے);
    إذا (unlikely (!blob))
      أرجع hb_blob_get_خالی_تھاواں ();

    أرجع blob;
  }

  بنانشانی صحيح get_upem () ثابت
  {
    بنانشانی صحيح ret = upem;
    إذا (unlikely (!ret))
    {
      أرجع لدو_upem ();
    }
    أرجع ret;
  }

  بنانشانی صحيح get_عدد_glyphs () ثابت
  {
    بنانشانی صحيح ret = عدد_glyphs;
    إذا (unlikely (ret == UINT_MAX))
      أرجع لدو_عدد_glyphs ();
    أرجع ret;
  }

  خاص:
  HB_اندرونی بنانشانی صحيح لدو_upem () ثابت;
  HB_اندرونی بنانشانی صحيح لدو_عدد_glyphs () ثابت;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#جے_انت /* HB_FACE_HH */
