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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-shaper.hh"
#अन्तर्भाव "hb-ot-shape.hh"

संरचना hb_shape_plan_कुंजी_t
{
  hb_segment_properties_t  props;

  ध्रुव hb_feature_t      *user_features;
  unsigned पूर्णाङ्क             अंक_user_features;

#ifndef HB_NO_OT_SHAPE
  hb_ot_shape_plan_कुंजी_t   ot;
#endif

  hb_shape_func_t         *shaper_func;
  ध्रुव वर्ण              *shaper_name;

  HB_INTERNAL तार्किक init (तार्किक                           copy,
			 hb_face_t                     *face,
			 ध्रुव hb_segment_properties_t *props,
			 ध्रुव hb_feature_t            *user_features,
			 unsigned पूर्णाङ्क                   अंक_user_features,
			 ध्रुव पूर्णाङ्क                     *coords,
			 unsigned पूर्णाङ्क                   अंक_coords,
			 ध्रुव वर्ण * ध्रुव            *shaper_list);

  HB_INTERNAL शून्य fini ()
  {
    hb_free ((शून्य *) user_features);
    user_features = nullptr;
#ifndef HB_NO_OT_SHAPE
    ot.fini ();
#endif
  }

  HB_INTERNAL तार्किक user_features_match (ध्रुव hb_shape_plan_कुंजी_t *other);

  HB_INTERNAL तार्किक equal (ध्रुव hb_shape_plan_कुंजी_t *other);
};

संरचना hb_shape_plan_t
{
  HB_INTERNAL ~hb_shape_plan_t ();
  hb_object_header_t header;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_कुंजी_t key;
#ifndef HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#endif
#ifdef HAVE_HARFRUST
  hb_atomic_t<शून्य *> harfrust_data;
#endif
};


#endif /* HB_SHAPE_PLAN_HH */
