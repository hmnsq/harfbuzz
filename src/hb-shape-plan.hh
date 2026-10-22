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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SHAPE_PLAN_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPE_PLAN_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape.hh"

ਬਣਤਰ hb_shape_plan_ਕੁੰਜੀ_t
{
  hb_segment_properties_t  props;

  ਅਚਲ hb_feature_t      *user_features;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ             ਅੰਕ_user_features;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE
  hb_ot_shape_plan_ਕੁੰਜੀ_t   ot;
#ਜੇ_ਅੰਤ

  hb_shape_func_t         *shaper_func;
  ਅਚਲ ਅੱਖਰ              *shaper_name;

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ init (ਤਾਰਕਿਕ                           copy,
			 hb_face_t                     *face,
			 ਅਚਲ hb_segment_properties_t *props,
			 ਅਚਲ hb_feature_t            *user_features,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_user_features,
			 ਅਚਲ ਪੂਰਨ                     *coords,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_coords,
			 ਅਚਲ ਅੱਖਰ * ਅਚਲ            *shaper_list);

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ fini ()
  {
    hb_free ((ਖਾਲੀ *) user_features);
    user_features = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE
    ot.fini ();
#ਜੇ_ਅੰਤ
  }

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ user_features_ਮੇਲ (ਅਚਲ hb_shape_plan_ਕੁੰਜੀ_t *other);

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ equal (ਅਚਲ hb_shape_plan_ਕੁੰਜੀ_t *other);
};

ਬਣਤਰ hb_shape_plan_t
{
  HB_ਅੰਦਰੂਨੀ ~hb_shape_plan_t ();
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_ਕੁੰਜੀ_t key;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_HARFRUST
  hb_atomic_t<ਖਾਲੀ *> harfrust_ਅੰਕੜੇ;
#ਜੇ_ਅੰਤ
};


#ਜੇ_ਅੰਤ /* HB_SHAPE_PLAN_HH */
