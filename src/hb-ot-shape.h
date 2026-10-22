/*
 * Copyright © 2013  Red Hat, Inc.
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
 */

#ఒకవేళ !defined(HB_OT_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb-ot.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_OT_SHAPE_H
#నిర్వచించు HB_OT_SHAPE_H

#అన్తర్భావ "hb.h"

HB_BEGIN_DECLS

/* TODO port to shape-plan / set. */
HB_EXTERN శూన్య
hb_ot_shape_glyphs_సంవృతం (hb_font_t          *font,
			    hb_బఫరు_t        *బఫరు,
			    స్థిరరాశి hb_feature_t *features,
			    గుర్తులేని పూర్ణాంకం        అంకె_features,
			    hb_సమితి_t           *glyphs);

HB_EXTERN శూన్య
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_ట్యాగ్_t         పట్టిక_ట్యాగ్_2,
				  hb_సమితి_t        *lookup_indexes /*బహిర్గతం*/);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_shape_plan_get_feature_ట్యాగులు (hb_shape_plan_t *shape_plan,
				   గుర్తులేని పూర్ణాంకం     ప్రారంభం_స్థానభ్రంశం,
				   గుర్తులేని పూర్ణాంకం    *ట్యాగ్_లెక్క, /* IN/OUT */
				   hb_ట్యాగ్_t        *ట్యాగులు /*బహిర్గతం*/);

/**
 * HB_OT_SHAPE_BUFFER_FORMAT_SERIAL:
 *
 * The serial number of the current internal buffer format.
 *
 * The serial number will increase when internal #hb_glyph_info_t and
 * #hb_glyph_position_t members change their format.
 *
 * Since: 13.2.0
 */
#నిర్వచించు HB_OT_SHAPE_బఫరు_FORMAT_SERIAL 1

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_shape_get_బఫరు_format_serial (శూన్య);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_OT_SHAPE_H */
