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

#నిర్వచితం_కాకపోతే HB_SHAPE_PLAN_HH
#నిర్వచించు HB_SHAPE_PLAN_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-shaper.hh"
#అన్తర్భావ "hb-ot-shape.hh"

నిర్మాణం hb_shape_plan_కీలకం_t
{
  hb_segment_properties_t  props;

  స్థిరరాశి hb_feature_t      *user_features;
  గుర్తులేని పూర్ణాంకం             అంకె_user_features;

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE
  hb_ot_shape_plan_కీలకం_t   ot;
#అయితే_ముగింపు

  hb_shape_func_t         *shaper_func;
  స్థిరరాశి అక్షరం              *shaper_name;

  HB_అంతర్గత తార్కిక init (తార్కిక                           copy,
			 hb_face_t                     *face,
			 స్థిరరాశి hb_segment_properties_t *props,
			 స్థిరరాశి hb_feature_t            *user_features,
			 గుర్తులేని పూర్ణాంకం                   అంకె_user_features,
			 స్థిరరాశి పూర్ణాంకం                     *coords,
			 గుర్తులేని పూర్ణాంకం                   అంకె_coords,
			 స్థిరరాశి అక్షరం * స్థిరరాశి            *shaper_list);

  HB_అంతర్గత శూన్య fini ()
  {
    hb_free ((శూన్య *) user_features);
    user_features = శూన్యసూచిక;
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE
    ot.fini ();
#అయితే_ముగింపు
  }

  HB_అంతర్గత తార్కిక user_features_సరిపోలిక (స్థిరరాశి hb_shape_plan_కీలకం_t *other);

  HB_అంతర్గత తార్కిక equal (స్థిరరాశి hb_shape_plan_కీలకం_t *other);
};

నిర్మాణం hb_shape_plan_t
{
  HB_అంతర్గత ~hb_shape_plan_t ();
  hb_object_శీర్షిక_t శీర్షిక;
  hb_face_t *face_unsafe; /* We don't carry a reference to face. */
  hb_shape_plan_కీలకం_t key;
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE
  hb_ot_shape_plan_t ot;
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_HARFRUST
  hb_atomic_t<శూన్య *> harfrust_దత్తాంశం;
#అయితే_ముగింపు
};


#అయితే_ముగింపు /* HB_SHAPE_PLAN_HH */
