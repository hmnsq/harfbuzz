/*
 * Copyright © 2018  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_MAP_H
#నిర్వచించు HB_MAP_H

#అన్తర్భావ "hb-common.h"
#అన్తర్భావ "hb-set.h"

HB_BEGIN_DECLS


/**
 * HB_MAP_VALUE_INVALID:
 *
 * Unset #hb_map_t value.
 *
 * Since: 1.7.7
 */
#నిర్వచించు HB_MAP_విలువ_INVALID HB_CODEPOINT_INVALID

/**
 * hb_map_t:
 *
 * Data type for holding integer-to-integer hash maps.
 *
 **/
రకనిర్వచనం నిర్మాణం hb_map_t hb_map_t;


HB_EXTERN hb_map_t *
hb_map_create (శూన్య);

HB_EXTERN hb_map_t *
hb_map_get_ఖాళీలు (శూన్య);

HB_EXTERN hb_map_t *
hb_map_reference (hb_map_t *map);

HB_EXTERN శూన్య
hb_map_destroy (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_సమితి_user_దత్తాంశం (hb_map_t           *map,
		      hb_user_దత్తాంశం_కీలకం_t *కీలకం_2,
		      శూన్య *              దత్తాంశం,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN శూన్య *
hb_map_get_user_దత్తాంశం (స్థిరరాశి hb_map_t     *map,
		      hb_user_దత్తాంశం_కీలకం_t *కీలకం_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_map_కేటాయింపు_successful (స్థిరరాశి hb_map_t *map);

HB_EXTERN hb_map_t *
hb_map_copy (స్థిరరాశి hb_map_t *map);

HB_EXTERN శూన్య
hb_map_clear (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_ఖాళీలు (స్థిరరాశి hb_map_t *map);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_map_get_population (స్థిరరాశి hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_equal (స్థిరరాశి hb_map_t *map,
		 స్థిరరాశి hb_map_t *other);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_map_hash (స్థిరరాశి hb_map_t *map);

HB_EXTERN శూన్య
hb_map_సమితి (hb_map_t       *map,
	    hb_codepoint_t  కీలకం_2,
	    hb_codepoint_t  విలువ_2);

HB_EXTERN hb_codepoint_t
hb_map_get (స్థిరరాశి hb_map_t *map,
	    hb_codepoint_t  కీలకం_2);

HB_EXTERN శూన్య
hb_map_del (hb_map_t       *map,
	    hb_codepoint_t  కీలకం_2);

HB_EXTERN hb_bool_t
hb_map_has (స్థిరరాశి hb_map_t *map,
	    hb_codepoint_t  కీలకం_2);

HB_EXTERN శూన్య
hb_map_update (hb_map_t *map,
	       స్థిరరాశి hb_map_t *other);

/* Pass -1 in for idx to get started. */
HB_EXTERN hb_bool_t
hb_map_తదుపరి (స్థిరరాశి hb_map_t *map,
	     పూర్ణాంకం *idx,
	     hb_codepoint_t *కీలకం_2,
	     hb_codepoint_t *విలువ_2);

HB_EXTERN శూన్య
hb_map_కీలకాలు (స్థిరరాశి hb_map_t *map,
	     hb_సమితి_t *కీలకాలు);

HB_EXTERN శూన్య
hb_map_విలువలు (స్థిరరాశి hb_map_t *map,
	       hb_సమితి_t *విలువలు);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_MAP_H */
