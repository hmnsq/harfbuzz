/*
 * Copyright © 2015  Mozilla Foundation.
 * Copyright © 2015  Google, Inc.
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
 * Mozilla Author(s): Jonathan Kew
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_OT_SHAPER_ARABIC_HH
#నిర్వచించు HB_OT_SHAPER_ARABIC_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-ot-shaper.hh"


నిర్మాణం arabic_shape_plan_t;

HB_అంతర్గత శూన్య *
దత్తాంశం_create_arabic (స్థిరరాశి hb_ot_shape_plan_t *plan);

HB_అంతర్గత శూన్య
దత్తాంశం_destroy_arabic (శూన్య *దత్తాంశం);

HB_అంతర్గత శూన్య
అమరిక_masks_arabic_plan (స్థిరరాశి arabic_shape_plan_t *arabic_plan,
			 hb_బఫరు_t               *బఫరు,
			 hb_script_t                script);

#అయితే_ముగింపు /* HB_OT_SHAPER_ARABIC_HH */
