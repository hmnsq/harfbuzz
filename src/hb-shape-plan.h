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

#यदि !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#error "Include <hb.h> instead."
#endif

#ifndef HB_SHAPE_PLAN_H
#define HB_SHAPE_PLAN_H

#अन्तर्भाव "hb-common.h"
#अन्तर्भाव "hb-font.h"

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
प्रकारनिर्देश संरचना hb_shape_plan_t hb_shape_plan_t;

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create (hb_face_t                     *face,
		      अचर hb_segment_properties_t *props,
		      अचर hb_feature_t            *user_features,
		      अचिन्हित पूर्णाङ्क                   अङ्क_user_features,
		      अचर वर्ण * अचर            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached (hb_face_t                     *face,
			     अचर hb_segment_properties_t *props,
			     अचर hb_feature_t            *user_features,
			     अचिन्हित पूर्णाङ्क                   अङ्क_user_features,
			     अचर वर्ण * अचर            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create2 (hb_face_t                     *face,
		       अचर hb_segment_properties_t *props,
		       अचर hb_feature_t            *user_features,
		       अचिन्हित पूर्णाङ्क                   अङ्क_user_features,
		       अचर पूर्णाङ्क                     *coords,
		       अचिन्हित पूर्णाङ्क                   अङ्क_coords,
		       अचर वर्ण * अचर            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached2 (hb_face_t                     *face,
			      अचर hb_segment_properties_t *props,
			      अचर hb_feature_t            *user_features,
			      अचिन्हित पूर्णाङ्क                   अङ्क_user_features,
			      अचर पूर्णाङ्क                     *coords,
			      अचिन्हित पूर्णाङ्क                   अङ्क_coords,
			      अचर वर्ण * अचर            *shaper_list);


HB_EXTERN hb_shape_plan_t *
hb_shape_plan_get_empty (शून्य);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_reference (hb_shape_plan_t *shape_plan);

HB_EXTERN शून्य
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan);

HB_EXTERN hb_bool_t
hb_shape_plan_set_user_data (hb_shape_plan_t    *shape_plan,
			     hb_user_data_साँचो_t *साँचो_2,
			     शून्य *              data,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace);

HB_EXTERN शून्य *
hb_shape_plan_get_user_data (अचर hb_shape_plan_t *shape_plan,
			     hb_user_data_साँचो_t    *साँचो_2);


HB_EXTERN hb_bool_t
hb_shape_plan_execute (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_buffer_t        *buffer,
		       अचर hb_feature_t *features,
		       अचिन्हित पूर्णाङ्क        अङ्क_features);

HB_EXTERN अचर वर्ण *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan);


HB_END_DECLS

#endif /* HB_SHAPE_PLAN_H */
