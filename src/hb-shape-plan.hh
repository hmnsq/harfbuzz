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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-shaper.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-shape.hh"

ಸಂರಚನಾ hb_shape_plan_ಕೀಲಿ_t
{
  hb_segment_properties_t  props;

  ಧ್ರುವ hb_feature_t      *user_features;
  unsigned ಪೂರ್ಣಾಙ್ಕ             ಅಂಕೆ_user_features;

#ifndef HB_NO_OT_SHAPE
  hb_ot_shape_plan_ಕೀಲಿ_t   ot;
#endif

  hb_shape_func_t         *shaper_func;
  ಧ್ರುವ ವರ್ಣ              *shaper_name;

  HB_INTERNAL ತಾರ್ಕಿಕ init (ತಾರ್ಕಿಕ                           copy,
			 hb_face_t                     *face,
			 ಧ್ರುವ hb_segment_properties_t *props,
			 ಧ್ರುವ hb_feature_t            *user_features,
			 unsigned ಪೂರ್ಣಾಙ್ಕ                   ಅಂಕೆ_user_features,
			 ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ                     *coords,
			 unsigned ಪೂರ್ಣಾಙ್ಕ                   ಅಂಕೆ_coords,
			 ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ            *shaper_list);

  HB_INTERNAL ಶೂನ್ಯ fini ()
  {
    hb_free ((ಶೂನ್ಯ *) user_features);
    user_features = nullptr;
#ifndef HB_NO_OT_SHAPE
    ot.fini ();
#endif
  }

  HB_INTERNAL ತಾರ್ಕಿಕ user_features_match (ಧ್ರುವ hb_shape_plan_ಕೀಲಿ_t *other);

  HB_INTERNAL ತಾರ್ಕಿಕ equal (ಧ್ರುವ hb_shape_plan_ಕೀಲಿ_t *other);
};

ಸಂರಚನಾ hb_shape_plan_t
{
  HB_INTERNAL ~hb_shape_plan_t ();
  hb_object_header_t header;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_ಕೀಲಿ_t key;
#ifndef HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#endif
#ifdef HAVE_HARFRUST
  hb_atomic_t<ಶೂನ್ಯ *> harfrust_data;
#endif
};


#endif /* HB_SHAPE_PLAN_HH */
