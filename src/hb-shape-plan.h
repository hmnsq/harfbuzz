/*
 * Copyright © 2012  Google, Inc.
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

#अगर !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#त्रुटि "Include <hb.h> instead."
#पूर्ण_अगर

#अगर_अघोषित HB_SHAPE_PLAN_H
#घोषणा HB_SHAPE_PLAN_H

#समावेश "hb-common.h"
#समावेश "hb-font.h"

HB_BEGIN_DECLS

/**
 * hb_shape_plan_t:
 *
 * Data type for holding a shaping plan. 
 *
 * Shape plans contain information about how HarfBuzz will shape a
 * particular text segment, based on the segment's properties and the
 * capabilities in the font face in use.
 *
 * Shape plans can be queried about how shaping will perform, given a set
 * of specific input parameters (script, language, direction, features,
 * etc.).
 *
 **/
प्रकार काष्ठा hb_shape_plan_t hb_shape_plan_t;

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create (hb_face_t                     *face,
		      स्थिर hb_segment_properties_t *props,
		      स्थिर hb_feature_t            *user_features,
		      अचिन्हित पूर्णांक                   अंक_user_features,
		      स्थिर अक्षर * स्थिर            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached (hb_face_t                     *face,
			     स्थिर hb_segment_properties_t *props,
			     स्थिर hb_feature_t            *user_features,
			     अचिन्हित पूर्णांक                   अंक_user_features,
			     स्थिर अक्षर * स्थिर            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create2 (hb_face_t                     *face,
		       स्थिर hb_segment_properties_t *props,
		       स्थिर hb_feature_t            *user_features,
		       अचिन्हित पूर्णांक                   अंक_user_features,
		       स्थिर पूर्णांक                     *coords,
		       अचिन्हित पूर्णांक                   अंक_coords,
		       स्थिर अक्षर * स्थिर            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached2 (hb_face_t                     *face,
			      स्थिर hb_segment_properties_t *props,
			      स्थिर hb_feature_t            *user_features,
			      अचिन्हित पूर्णांक                   अंक_user_features,
			      स्थिर पूर्णांक                     *coords,
			      अचिन्हित पूर्णांक                   अंक_coords,
			      स्थिर अक्षर * स्थिर            *shaper_list);


HB_EXTERN hb_shape_plan_t *
hb_shape_plan_get_empty (व्योम);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_reference (hb_shape_plan_t *shape_plan);

HB_EXTERN व्योम
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan);

HB_EXTERN hb_bool_t
hb_shape_plan_set_user_तथ्य (hb_shape_plan_t    *shape_plan,
			     hb_user_तथ्य_कुंजी_t *कुंजी_2,
			     व्योम *              तथ्य,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace);

HB_EXTERN व्योम *
hb_shape_plan_get_user_तथ्य (स्थिर hb_shape_plan_t *shape_plan,
			     hb_user_तथ्य_कुंजी_t    *कुंजी_2);


HB_EXTERN hb_bool_t
hb_shape_plan_execute (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_बफर_t        *बफर,
		       स्थिर hb_feature_t *features,
		       अचिन्हित पूर्णांक        अंक_features);

HB_EXTERN स्थिर अक्षर *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan);


HB_END_DECLS

#पूर्ण_अगर /* HB_SHAPE_PLAN_H */
