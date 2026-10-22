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

#ਜੇ !defined(HB_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
#ਗਲਤੀ "Include <hb.h> instead."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SHAPE_PLAN_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPE_PLAN_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.h"

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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_shape_plan_t hb_shape_plan_t;

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create (hb_face_t                     *face,
		      ਅਚਲ hb_segment_properties_t *props,
		      ਅਚਲ hb_feature_t            *user_features,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_user_features,
		      ਅਚਲ ਅੱਖਰ * ਅਚਲ            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached (hb_face_t                     *face,
			     ਅਚਲ hb_segment_properties_t *props,
			     ਅਚਲ hb_feature_t            *user_features,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_user_features,
			     ਅਚਲ ਅੱਖਰ * ਅਚਲ            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create2 (hb_face_t                     *face,
		       ਅਚਲ hb_segment_properties_t *props,
		       ਅਚਲ hb_feature_t            *user_features,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_user_features,
		       ਅਚਲ ਪੂਰਨ                     *coords,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_coords,
		       ਅਚਲ ਅੱਖਰ * ਅਚਲ            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached2 (hb_face_t                     *face,
			      ਅਚਲ hb_segment_properties_t *props,
			      ਅਚਲ hb_feature_t            *user_features,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_user_features,
			      ਅਚਲ ਪੂਰਨ                     *coords,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_coords,
			      ਅਚਲ ਅੱਖਰ * ਅਚਲ            *shaper_list);


HB_EXTERN hb_shape_plan_t *
hb_shape_plan_get_ਖਾਲੀ_ਥਾਵਾਂ (ਖਾਲੀ);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_reference (hb_shape_plan_t *shape_plan);

HB_EXTERN ਖਾਲੀ
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan);

HB_EXTERN hb_bool_t
hb_shape_plan_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_shape_plan_t    *shape_plan,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2,
			     ਖਾਲੀ *              ਅੰਕੜੇ,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace);

HB_EXTERN ਖਾਲੀ *
hb_shape_plan_get_user_ਅੰਕੜੇ (ਅਚਲ hb_shape_plan_t *shape_plan,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t    *ਕੁੰਜੀ_2);


HB_EXTERN hb_bool_t
hb_shape_plan_execute (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_ਬਫਰ_t        *ਬਫਰ,
		       ਅਚਲ hb_feature_t *features,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features);

HB_EXTERN ਅਚਲ ਅੱਖਰ *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan);


HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_SHAPE_PLAN_H */
