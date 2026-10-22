/*
 * Copyright © 2009  Red Hat, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ఒకవేళ !defined(HB_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_SHAPE_H
#నిర్వచించు HB_SHAPE_H

#అన్తర్భావ "hb-common.h"
#అన్తర్భావ "hb-buffer.h"
#అన్తర్భావ "hb-font.h"

HB_BEGIN_DECLS


HB_EXTERN శూన్య
hb_shape (hb_font_t           *font,
	  hb_బఫరు_t         *బఫరు,
	  స్థిరరాశి hb_feature_t  *features,
	  గుర్తులేని పూర్ణాంకం         అంకె_features);

HB_EXTERN hb_bool_t
hb_shape_నిండిన (hb_font_t          *font,
	       hb_బఫరు_t        *బఫరు,
	       స్థిరరాశి hb_feature_t *features,
	       గుర్తులేని పూర్ణాంకం        అంకె_features,
	       స్థిరరాశి అక్షరం * స్థిరరాశి *shaper_list);

#నిర్వచితమైతే HB_EXPERIMENTAL_API
HB_EXTERN hb_bool_t
hb_shape_justify (hb_font_t          *font,
		  hb_బఫరు_t        *బఫరు,
		  స్థిరరాశి hb_feature_t *features,
		  గుర్తులేని పూర్ణాంకం        అంకె_features,
		  స్థిరరాశి అక్షరం * స్థిరరాశి *shaper_list,
		  దశాంశం               min_లక్ష్యం_advance,
		  దశాంశం               max_లక్ష్యం_advance,
		  దశాంశం              *advance, /* IN/OUT */
		  hb_ట్యాగ్_t           *var_ట్యాగ్, /*బహిర్గతం*/
		  దశాంశం              *var_విలువ /*బహిర్గతం*/);
#అయితే_ముగింపు

HB_EXTERN స్థిరరాశి అక్షరం **
hb_shape_list_shapers (శూన్య);


HB_చివర_DECLS

#అయితే_ముగింపు /* HB_SHAPE_H */
