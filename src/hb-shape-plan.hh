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

#جيڪڏهن_وصف_نه_ٿيل HB_SHAPE_PLAN_HH
#وصف_ڪريو HB_SHAPE_PLAN_HH

#ضمن "hb.hh"
#ضمن "hb-shaper.hh"
#ضمن "hb-ot-shape.hh"

بنية hb_shape_plan_ڪنجي_t
{
  hb_segment_properties_t  props;

  ثابت hb_feature_t      *user_features;
  بنانشان صحيح             انگ_user_features;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE
  hb_ot_shape_plan_ڪنجي_t   ot;
#جيڪڏهن_پورو

  hb_shape_func_t         *shaper_func;
  ثابت حرف              *shaper_name;

  HB_اندروني منطقي init (منطقي                           copy,
			 hb_face_t                     *face,
			 ثابت hb_segment_properties_t *props,
			 ثابت hb_feature_t            *user_features,
			 بنانشان صحيح                   انگ_user_features,
			 ثابت صحيح                     *coords,
			 بنانشان صحيح                   انگ_coords,
			 ثابت حرف * ثابت            *shaper_list);

  HB_اندروني فراغ fini ()
  {
    hb_free ((فراغ *) user_features);
    user_features = خالي_اشارو;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE
    ot.fini ();
#جيڪڏهن_پورو
  }

  HB_اندروني منطقي user_features_ميلاپ (ثابت hb_shape_plan_ڪنجي_t *other);

  HB_اندروني منطقي equal (ثابت hb_shape_plan_ڪنجي_t *other);
};

بنية hb_shape_plan_t
{
  HB_اندروني ~hb_shape_plan_t ();
  hb_object_سرو_t سرو;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_ڪنجي_t key;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_HARFRUST
  hb_atomic_t<فراغ *> harfrust_معلومات;
#جيڪڏهن_پورو
};


#جيڪڏهن_پورو /* HB_SHAPE_PLAN_HH */
