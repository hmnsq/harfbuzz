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

#اگر_غیر_تعریف_شدہ HB_FACE_HH
#تعریف_کرو HB_FACE_HH

#شامل "hb.hh"

#شامل "hb-shaper.hh"
#شامل "hb-shape-plan.hh"
#شامل "hb-ot-face.hh"


/*
 * hb_face_t
 */

#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_ڈیٹا_INSTANTIATE_SHAPERS(shaper, face);
#شامل "hb-shaper-list.hh"
#تعریف_منسوخ HB_SHAPER_IMPLEMENT

ساخت hb_face_t
{
  hb_object_سرخی_t سرخی;

  غیرعلامتی صحیح اشاریہ_2;			/* Face index in a collection, zero-based. */
  تغیرپذیر hb_atomic_t<غیرعلامتی> upem;	/* Units-per-EM. */
  تغیرپذیر hb_atomic_t<غیرعلامتی> عدد_glyphs;/* Number of glyphs. */

  hb_reference_جدول_func_t  reference_جدول_func;
  خالی                      *user_ڈیٹا;
  hb_destroy_func_t          destroy;

  hb_get_جدول_لیبل_فہرست_func_t   get_جدول_لیبل_فہرست_func;
  خالی                      *get_جدول_لیبل_فہرست_user_ڈیٹا;
  hb_destroy_func_t          get_جدول_لیبل_فہرست_destroy;

  hb_shaper_object_dataset_t<hb_face_t> ڈیٹا;/* Various shaper data. */
  hb_ot_face_t جدول;			/* All the face's tables. */

  /* Cache */
  ساخت plan_گرہ_t
  {
    hb_shape_plan_t *shape_plan;
    plan_گرہ_t *next;
  };
#اگر_غیر_تعریف_شدہ HB_NO_SHAPER
  hb_atomic_t<plan_گرہ_t *> shape_plans;
#اگر_ختم

  hb_blob_t *reference_جدول (hb_لیبل_t لیبل_2) ثابت
  {
    hb_blob_t *blob;

    اگر (unlikely (!reference_جدول_func))
      واپس hb_blob_get_خالی_خانے ();

    blob = reference_جدول_func (/*Oh, well.*/const_cast<hb_face_t *> (یہ), لیبل_2, user_ڈیٹا);
    اگر (unlikely (!blob))
      واپس hb_blob_get_خالی_خانے ();

    واپس blob;
  }

  غیرعلامتی صحیح get_upem () ثابت
  {
    غیرعلامتی صحیح ret = upem;
    اگر (unlikely (!ret))
    {
      واپس بھرو_upem ();
    }
    واپس ret;
  }

  غیرعلامتی صحیح get_عدد_glyphs () ثابت
  {
    غیرعلامتی صحیح ret = عدد_glyphs;
    اگر (unlikely (ret == UINT_MAX))
      واپس بھرو_عدد_glyphs ();
    واپس ret;
  }

  نجی:
  HB_اندرونی غیرعلامتی صحیح بھرو_upem () ثابت;
  HB_اندرونی غیرعلامتی صحیح بھرو_عدد_glyphs () ثابت;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#اگر_ختم /* HB_FACE_HH */
