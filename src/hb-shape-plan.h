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

#إذا !defined(HB_H_اندريون) && !defined(HB_NO_SINGLE_سرو_ERROR)
#غلطي "Include <hb.h> instead."
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_SHAPE_PLAN_H
#وصف_ڪريو HB_SHAPE_PLAN_H

#ضمن "hb-common.h"
#ضمن "hb-font.h"

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
تعريفنوع بنية hb_shape_plan_t hb_shape_plan_t;

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create (hb_face_t                     *face,
		      ثابت hb_segment_properties_t *props,
		      ثابت hb_feature_t            *user_features,
		      بنانشان صحيح                   انگ_user_features,
		      ثابت حرف * ثابت            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached (hb_face_t                     *face,
			     ثابت hb_segment_properties_t *props,
			     ثابت hb_feature_t            *user_features,
			     بنانشان صحيح                   انگ_user_features,
			     ثابت حرف * ثابت            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create2 (hb_face_t                     *face,
		       ثابت hb_segment_properties_t *props,
		       ثابت hb_feature_t            *user_features,
		       بنانشان صحيح                   انگ_user_features,
		       ثابت صحيح                     *coords,
		       بنانشان صحيح                   انگ_coords,
		       ثابت حرف * ثابت            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached2 (hb_face_t                     *face,
			      ثابت hb_segment_properties_t *props,
			      ثابت hb_feature_t            *user_features,
			      بنانشان صحيح                   انگ_user_features,
			      ثابت صحيح                     *coords,
			      بنانشان صحيح                   انگ_coords,
			      ثابت حرف * ثابت            *shaper_list);


HB_EXTERN hb_shape_plan_t *
hb_shape_plan_get_خالي_جايون (فراغ);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_reference (hb_shape_plan_t *shape_plan);

HB_EXTERN فراغ
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan);

HB_EXTERN hb_bool_t
hb_shape_plan_سيٽ_user_معلومات (hb_shape_plan_t    *shape_plan,
			     hb_user_معلومات_ڪنجي_t *ڪنجي_2,
			     فراغ *              معلومات,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace);

HB_EXTERN فراغ *
hb_shape_plan_get_user_معلومات (ثابت hb_shape_plan_t *shape_plan,
			     hb_user_معلومات_ڪنجي_t    *ڪنجي_2);


HB_EXTERN hb_bool_t
hb_shape_plan_execute (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_بفر_t        *بفر,
		       ثابت hb_feature_t *features,
		       بنانشان صحيح        انگ_features);

HB_EXTERN ثابت حرف *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan);


HB_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_SHAPE_PLAN_H */
