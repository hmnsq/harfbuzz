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

#اگر_غیر_تعریف_شدہ HB_SHAPE_PLAN_HH
#تعریف_کرو HB_SHAPE_PLAN_HH

#شامل "hb.hh"
#شامل "hb-shaper.hh"
#شامل "hb-ot-shape.hh"

ساخت hb_shape_plan_کلید_t
{
  hb_segment_properties_t  props;

  ثابت hb_feature_t      *user_features;
  غیرعلامتی صحیح             عدد_user_features;

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE
  hb_ot_shape_plan_کلید_t   ot;
#اگر_ختم

  hb_shape_func_t         *shaper_func;
  ثابت حرف              *shaper_name;

  HB_اندرونی منطقی init (منطقی                           copy,
			 hb_face_t                     *face,
			 ثابت hb_segment_properties_t *props,
			 ثابت hb_feature_t            *user_features,
			 غیرعلامتی صحیح                   عدد_user_features,
			 ثابت صحیح                     *coords,
			 غیرعلامتی صحیح                   عدد_coords,
			 ثابت حرف * ثابت            *shaper_list);

  HB_اندرونی خالی fini ()
  {
    hb_free ((خالی *) user_features);
    user_features = خالی_اشاریہ;
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE
    ot.fini ();
#اگر_ختم
  }

  HB_اندرونی منطقی user_features_مطابقت (ثابت hb_shape_plan_کلید_t *other);

  HB_اندرونی منطقی equal (ثابت hb_shape_plan_کلید_t *other);
};

ساخت hb_shape_plan_t
{
  HB_اندرونی ~hb_shape_plan_t ();
  hb_object_سرخی_t سرخی;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_کلید_t key;
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#اگر_ختم
#اگر_تعریف_شدہ HAVE_HARFRUST
  hb_atomic_t<خالی *> harfrust_ڈیٹا;
#اگر_ختم
};


#اگر_ختم /* HB_SHAPE_PLAN_HH */
