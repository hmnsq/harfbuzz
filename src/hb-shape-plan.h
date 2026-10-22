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

#اگر !defined(HB_H_اندر) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb.h> instead."
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_SHAPE_PLAN_H
#تعریف_کرو HB_SHAPE_PLAN_H

#شامل "hb-common.h"
#شامل "hb-font.h"

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
قسمتعریف ساخت hb_shape_plan_t hb_shape_plan_t;

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create (hb_face_t                     *face,
		      ثابت hb_segment_properties_t *props,
		      ثابت hb_feature_t            *user_features,
		      غیرعلامتی صحیح                   عدد_user_features,
		      ثابت حرف * ثابت            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached (hb_face_t                     *face,
			     ثابت hb_segment_properties_t *props,
			     ثابت hb_feature_t            *user_features,
			     غیرعلامتی صحیح                   عدد_user_features,
			     ثابت حرف * ثابت            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create2 (hb_face_t                     *face,
		       ثابت hb_segment_properties_t *props,
		       ثابت hb_feature_t            *user_features,
		       غیرعلامتی صحیح                   عدد_user_features,
		       ثابت صحیح                     *coords,
		       غیرعلامتی صحیح                   عدد_coords,
		       ثابت حرف * ثابت            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached2 (hb_face_t                     *face,
			      ثابت hb_segment_properties_t *props,
			      ثابت hb_feature_t            *user_features,
			      غیرعلامتی صحیح                   عدد_user_features,
			      ثابت صحیح                     *coords,
			      غیرعلامتی صحیح                   عدد_coords,
			      ثابت حرف * ثابت            *shaper_list);


HB_EXTERN hb_shape_plan_t *
hb_shape_plan_get_خالی_خانے (خالی);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_reference (hb_shape_plan_t *shape_plan);

HB_EXTERN خالی
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan);

HB_EXTERN hb_bool_t
hb_shape_plan_سیٹ_user_ڈیٹا (hb_shape_plan_t    *shape_plan,
			     hb_user_ڈیٹا_کلید_t *کلید_2,
			     خالی *              ڈیٹا,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace);

HB_EXTERN خالی *
hb_shape_plan_get_user_ڈیٹا (ثابت hb_shape_plan_t *shape_plan,
			     hb_user_ڈیٹا_کلید_t    *کلید_2);


HB_EXTERN hb_bool_t
hb_shape_plan_execute (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_بفر_t        *بفر,
		       ثابت hb_feature_t *features,
		       غیرعلامتی صحیح        عدد_features);

HB_EXTERN ثابت حرف *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan);


HB_انتہا_DECLS

#اگر_ختم /* HB_SHAPE_PLAN_H */
