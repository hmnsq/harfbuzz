/*
 * Copyright © 2012,2018  Google, Inc.
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

#اگر_تعریف_نشده HB_SHAPE_PLAN_HH
#تعریف_کن HB_SHAPE_PLAN_HH

#ضمن "hb.hh"
#ضمن "hb-shaper.hh"
#ضمن "hb-ot-shape.hh"

بنية hb_shape_plan_کلید_t
{
  hb_segment_properties_t  props;

  ثابت hb_feature_t      *user_features;
  بیعلامت صحيح             رقم_user_features;

#اگر_تعریف_نشده HB_NO_OT_SHAPE
  hb_ot_shape_plan_کلید_t   ot;
#پایان_اگر

  hb_shape_func_t         *shaper_func;
  ثابت حرف              *shaper_name;

  HB_داخلی منطقي init (منطقي                           copy,
			 hb_face_t                     *face,
			 ثابت hb_segment_properties_t *props,
			 ثابت hb_feature_t            *user_features,
			 بیعلامت صحيح                   رقم_user_features,
			 ثابت صحيح                     *coords,
			 بیعلامت صحيح                   رقم_coords,
			 ثابت حرف * ثابت            *shaper_list);

  HB_داخلی فراغ fini ()
  {
    hb_free ((فراغ *) user_features);
    user_features = اشاره_تهی;
#اگر_تعریف_نشده HB_NO_OT_SHAPE
    ot.fini ();
#پایان_اگر
  }

  HB_داخلی منطقي user_features_تطابق (ثابت hb_shape_plan_کلید_t *other);

  HB_داخلی منطقي equal (ثابت hb_shape_plan_کلید_t *other);
};

بنية hb_shape_plan_t
{
  HB_داخلی ~hb_shape_plan_t ();
  hb_object_سرآیند_t سرآیند;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_کلید_t key;
#اگر_تعریف_نشده HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#پایان_اگر
#اگر_تعریف_شده HAVE_HARFRUST
  hb_atomic_t<فراغ *> harfrust_داده;
#پایان_اگر
};


#پایان_اگر /* HB_SHAPE_PLAN_HH */
