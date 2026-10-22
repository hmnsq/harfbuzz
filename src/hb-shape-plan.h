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

#ఒకవేళ !defined(HB_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_SHAPE_PLAN_H
#నిర్వచించు HB_SHAPE_PLAN_H

#అన్తర్భావ "hb-common.h"
#అన్తర్భావ "hb-font.h"

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
రకనిర్వచనం నిర్మాణం hb_shape_plan_t hb_shape_plan_t;

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create (hb_face_t                     *face,
		      స్థిరరాశి hb_segment_properties_t *props,
		      స్థిరరాశి hb_feature_t            *user_features,
		      గుర్తులేని పూర్ణాంకం                   అంకె_user_features,
		      స్థిరరాశి అక్షరం * స్థిరరాశి            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached (hb_face_t                     *face,
			     స్థిరరాశి hb_segment_properties_t *props,
			     స్థిరరాశి hb_feature_t            *user_features,
			     గుర్తులేని పూర్ణాంకం                   అంకె_user_features,
			     స్థిరరాశి అక్షరం * స్థిరరాశి            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create2 (hb_face_t                     *face,
		       స్థిరరాశి hb_segment_properties_t *props,
		       స్థిరరాశి hb_feature_t            *user_features,
		       గుర్తులేని పూర్ణాంకం                   అంకె_user_features,
		       స్థిరరాశి పూర్ణాంకం                     *coords,
		       గుర్తులేని పూర్ణాంకం                   అంకె_coords,
		       స్థిరరాశి అక్షరం * స్థిరరాశి            *shaper_list);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_create_cached2 (hb_face_t                     *face,
			      స్థిరరాశి hb_segment_properties_t *props,
			      స్థిరరాశి hb_feature_t            *user_features,
			      గుర్తులేని పూర్ణాంకం                   అంకె_user_features,
			      స్థిరరాశి పూర్ణాంకం                     *coords,
			      గుర్తులేని పూర్ణాంకం                   అంకె_coords,
			      స్థిరరాశి అక్షరం * స్థిరరాశి            *shaper_list);


HB_EXTERN hb_shape_plan_t *
hb_shape_plan_get_ఖాళీలు (శూన్య);

HB_EXTERN hb_shape_plan_t *
hb_shape_plan_reference (hb_shape_plan_t *shape_plan);

HB_EXTERN శూన్య
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan);

HB_EXTERN hb_bool_t
hb_shape_plan_సమితి_user_దత్తాంశం (hb_shape_plan_t    *shape_plan,
			     hb_user_దత్తాంశం_కీలకం_t *కీలకం_2,
			     శూన్య *              దత్తాంశం,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace);

HB_EXTERN శూన్య *
hb_shape_plan_get_user_దత్తాంశం (స్థిరరాశి hb_shape_plan_t *shape_plan,
			     hb_user_దత్తాంశం_కీలకం_t    *కీలకం_2);


HB_EXTERN hb_bool_t
hb_shape_plan_execute (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_బఫరు_t        *బఫరు,
		       స్థిరరాశి hb_feature_t *features,
		       గుర్తులేని పూర్ణాంకం        అంకె_features);

HB_EXTERN స్థిరరాశి అక్షరం *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan);


HB_చివర_DECLS

#అయితే_ముగింపు /* HB_SHAPE_PLAN_H */
