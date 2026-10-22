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

#که_نه_تعریف_شوی HB_SHAPE_PLAN_HH
#تعریف_کړه HB_SHAPE_PLAN_HH

#ضمن "hb.hh"
#ضمن "hb-shaper.hh"
#ضمن "hb-ot-shape.hh"

بنية hb_shape_plan_کلی_t
{
  hb_segment_properties_t  props;

  ثابت hb_feature_t      *user_features;
  بینښه صحيح             عدد_user_features;

#که_نه_تعریف_شوی HB_NO_OT_SHAPE
  hb_ot_shape_plan_کلی_t   ot;
#د_که_پای

  hb_shape_func_t         *shaper_func;
  ثابت حرف              *shaper_name;

  HB_دننی منطقي init (منطقي                           copy,
			 hb_face_t                     *face,
			 ثابت hb_segment_properties_t *props,
			 ثابت hb_feature_t            *user_features,
			 بینښه صحيح                   عدد_user_features,
			 ثابت صحيح                     *coords,
			 بینښه صحيح                   عدد_coords,
			 ثابت حرف * ثابت            *shaper_list);

  HB_دننی فراغ fini ()
  {
    hb_free ((فراغ *) user_features);
    user_features = تش_اشاره;
#که_نه_تعریف_شوی HB_NO_OT_SHAPE
    ot.fini ();
#د_که_پای
  }

  HB_دننی منطقي user_features_سمون (ثابت hb_shape_plan_کلی_t *other);

  HB_دننی منطقي equal (ثابت hb_shape_plan_کلی_t *other);
};

بنية hb_shape_plan_t
{
  HB_دننی ~hb_shape_plan_t ();
  hb_object_سرلیک_t سرلیک;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_کلی_t key;
#که_نه_تعریف_شوی HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#د_که_پای
#که_تعریف_شوی HAVE_HARFRUST
  hb_atomic_t<فراغ *> harfrust_معلومات;
#د_که_پای
};


#د_که_پای /* HB_SHAPE_PLAN_HH */
