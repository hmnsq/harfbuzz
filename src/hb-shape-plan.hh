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

#ifndef HB_SHAPE_PLAN_HH
#define HB_SHAPE_PLAN_HH

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-shaper.hh"
#অন্তর্ভাব "hb-ot-shape.hh"

সংরচনা hb_shape_plan_চাবি_t
{
  hb_segment_properties_t  props;

  ধ্রুব hb_feature_t      *user_features;
  unsigned পূর্ণাঙ্ক             অঙ্ক_user_features;

#ifndef HB_NO_OT_SHAPE
  hb_ot_shape_plan_চাবি_t   ot;
#endif

  hb_shape_func_t         *shaper_func;
  ধ্রুব বর্ণ              *shaper_name;

  HB_INTERNAL তার্কিক init (তার্কিক                           copy,
			 hb_face_t                     *face,
			 ধ্রুব hb_segment_properties_t *props,
			 ধ্রুব hb_feature_t            *user_features,
			 unsigned পূর্ণাঙ্ক                   অঙ্ক_user_features,
			 ধ্রুব পূর্ণাঙ্ক                     *coords,
			 unsigned পূর্ণাঙ্ক                   অঙ্ক_coords,
			 ধ্রুব বর্ণ * ধ্রুব            *shaper_list);

  HB_INTERNAL শূন্য fini ()
  {
    hb_free ((শূন্য *) user_features);
    user_features = nullptr;
#ifndef HB_NO_OT_SHAPE
    ot.fini ();
#endif
  }

  HB_INTERNAL তার্কিক user_features_match (ধ্রুব hb_shape_plan_চাবি_t *other);

  HB_INTERNAL তার্কিক equal (ধ্রুব hb_shape_plan_চাবি_t *other);
};

সংরচনা hb_shape_plan_t
{
  HB_INTERNAL ~hb_shape_plan_t ();
  hb_object_header_t header;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_চাবি_t key;
#ifndef HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#endif
#ifdef HAVE_HARFRUST
  hb_atomic_t<শূন্য *> harfrust_data;
#endif
};


#endif /* HB_SHAPE_PLAN_HH */
