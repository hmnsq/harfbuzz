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

#अगर_अघोषित HB_SHAPE_PLAN_HH
#घोषणा HB_SHAPE_PLAN_HH

#समावेश "hb.hh"
#समावेश "hb-shaper.hh"
#समावेश "hb-ot-shape.hh"

काष्ठा hb_shape_plan_कुंजी_t
{
  hb_segment_properties_t  props;

  स्थिर hb_feature_t      *user_features;
  अचिन्हित पूर्णांक             अंक_user_features;

#अगर_अघोषित HB_NO_OT_SHAPE
  hb_ot_shape_plan_कुंजी_t   ot;
#पूर्ण_अगर

  hb_shape_func_t         *shaper_func;
  स्थिर अक्षर              *shaper_name;

  HB_INTERNAL bool init (bool                           copy,
			 hb_face_t                     *face,
			 स्थिर hb_segment_properties_t *props,
			 स्थिर hb_feature_t            *user_features,
			 अचिन्हित पूर्णांक                   अंक_user_features,
			 स्थिर पूर्णांक                     *coords,
			 अचिन्हित पूर्णांक                   अंक_coords,
			 स्थिर अक्षर * स्थिर            *shaper_list);

  HB_INTERNAL व्योम fini ()
  {
    hb_free ((व्योम *) user_features);
    user_features = nullptr;
#अगर_अघोषित HB_NO_OT_SHAPE
    ot.fini ();
#पूर्ण_अगर
  }

  HB_INTERNAL bool user_features_match (स्थिर hb_shape_plan_कुंजी_t *other);

  HB_INTERNAL bool equal (स्थिर hb_shape_plan_कुंजी_t *other);
};

काष्ठा hb_shape_plan_t
{
  HB_INTERNAL ~hb_shape_plan_t ();
  hb_object_header_t header;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_कुंजी_t key;
#अगर_अघोषित HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#पूर्ण_अगर
#अगर_घोषित HAVE_HARFRUST
  hb_atomic_t<व्योम *> harfrust_तथ्य;
#पूर्ण_अगर
};


#पूर्ण_अगर /* HB_SHAPE_PLAN_HH */
