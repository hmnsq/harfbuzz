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

#که_نه_تعریف_شوی HB_FACE_HH
#تعریف_کړه HB_FACE_HH

#ضمن "hb.hh"

#ضمن "hb-shaper.hh"
#ضمن "hb-shape-plan.hh"
#ضمن "hb-ot-face.hh"


/*
 * hb_face_t
 */

#تعریف_کړه HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_معلومات_INSTANTIATE_SHAPERS(shaper, face);
#ضمن "hb-shaper-list.hh"
#تعریف_لغوه HB_SHAPER_IMPLEMENT

بنية hb_face_t
{
  hb_object_سرلیک_t سرلیک;

  بینښه صحيح شاخص_2;			/* Face index in a collection, zero-based. */
  بدلیدونکی hb_atomic_t<بینښه> upem;	/* Units-per-EM. */
  بدلیدونکی hb_atomic_t<بینښه> عدد_glyphs;/* Number of glyphs. */

  hb_reference_جدول_func_t  reference_جدول_func;
  فراغ                      *user_معلومات;
  hb_destroy_func_t          destroy;

  hb_get_جدول_نښانونه_func_t   get_جدول_نښانونه_func;
  فراغ                      *get_جدول_نښانونه_user_معلومات;
  hb_destroy_func_t          get_جدول_نښانونه_destroy;

  hb_shaper_object_dataset_t<hb_face_t> معلومات;/* Various shaper data. */
  hb_ot_face_t جدول;			/* All the face's tables. */

  /* Cache */
  بنية plan_غوټه_t
  {
    hb_shape_plan_t *shape_plan;
    plan_غوټه_t *next;
  };
#که_نه_تعریف_شوی HB_NO_SHAPER
  hb_atomic_t<plan_غوټه_t *> shape_plans;
#د_که_پای

  hb_blob_t *reference_جدول (hb_نښان_t نښان_2) ثابت
  {
    hb_blob_t *blob;

    إذا (unlikely (!reference_جدول_func))
      أرجع hb_blob_get_تشه ();

    blob = reference_جدول_func (/*Oh, well.*/const_cast<hb_face_t *> (هذا), نښان_2, user_معلومات);
    إذا (unlikely (!blob))
      أرجع hb_blob_get_تشه ();

    أرجع blob;
  }

  بینښه صحيح get_upem () ثابت
  {
    بینښه صحيح ret = upem;
    إذا (unlikely (!ret))
    {
      أرجع بارول_upem ();
    }
    أرجع ret;
  }

  بینښه صحيح get_عدد_glyphs () ثابت
  {
    بینښه صحيح ret = عدد_glyphs;
    إذا (unlikely (ret == UINT_MAX))
      أرجع بارول_عدد_glyphs ();
    أرجع ret;
  }

  خاص:
  HB_دننی بینښه صحيح بارول_upem () ثابت;
  HB_دننی بینښه صحيح بارول_عدد_glyphs () ثابت;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#د_که_پای /* HB_FACE_HH */
