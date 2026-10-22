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

#నిర్వచితం_కాకపోతే HB_సమితి_H
#నిర్వచించు HB_సమితి_H

#అన్తర్భావ "hb-common.h"

HB_BEGIN_DECLS


/**
 * HB_SET_VALUE_INVALID:
 *
 * Unset #hb_set_t value.
 *
 * Since: 0.9.21
 */
#నిర్వచించు HB_సమితి_విలువ_INVALID HB_CODEPOINT_INVALID

/**
 * hb_set_t:
 *
 * Data type for holding a set of integers. #hb_set_t's are
 * used to gather and contain glyph IDs, Unicode code
 * points, and various other collections of discrete 
 * values.
 *
 **/
రకనిర్వచనం నిర్మాణం hb_సమితి_t hb_సమితి_t;


HB_EXTERN hb_సమితి_t *
hb_సమితి_create (శూన్య);

HB_EXTERN hb_సమితి_t *
hb_సమితి_get_ఖాళీలు (శూన్య);

HB_EXTERN hb_సమితి_t *
hb_సమితి_reference (hb_సమితి_t *సమితి);

HB_EXTERN శూన్య
hb_సమితి_destroy (hb_సమితి_t *సమితి);

HB_EXTERN hb_bool_t
hb_సమితి_సమితి_user_దత్తాంశం (hb_సమితి_t           *సమితి,
		      hb_user_దత్తాంశం_కీలకం_t *కీలకం_2,
		      శూన్య *              దత్తాంశం,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN శూన్య *
hb_సమితి_get_user_దత్తాంశం (స్థిరరాశి hb_సమితి_t     *సమితి,
		      hb_user_దత్తాంశం_కీలకం_t *కీలకం_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_సమితి_కేటాయింపు_successful (స్థిరరాశి hb_సమితి_t *సమితి);

HB_EXTERN hb_సమితి_t *
hb_సమితి_copy (స్థిరరాశి hb_సమితి_t *సమితి);

HB_EXTERN శూన్య
hb_సమితి_clear (hb_సమితి_t *సమితి);

HB_EXTERN hb_bool_t
hb_సమితి_is_ఖాళీలు (స్థిరరాశి hb_సమితి_t *సమితి);

HB_EXTERN శూన్య
hb_సమితి_invert (hb_సమితి_t *సమితి);

HB_EXTERN hb_bool_t
hb_సమితి_is_inverted (స్థిరరాశి hb_సమితి_t *సమితి);

HB_EXTERN hb_bool_t
hb_సమితి_has (స్థిరరాశి hb_సమితి_t *సమితి,
	    hb_codepoint_t  codepoint);

HB_EXTERN శూన్య
hb_సమితి_కూడు (hb_సమితి_t       *సమితి,
	    hb_codepoint_t  codepoint);

HB_EXTERN శూన్య
hb_సమితి_కూడు_range (hb_సమితి_t       *సమితి,
		  hb_codepoint_t  మొదటి,
		  hb_codepoint_t  చివరి);

HB_EXTERN శూన్య
hb_సమితి_కూడు_sorted_array (hb_సమితి_t             *సమితి,
		         స్థిరరాశి hb_codepoint_t *sorted_codepoints,
		         గుర్తులేని పూర్ణాంకం          అంకె_codepoints);

HB_EXTERN శూన్య
hb_సమితి_del (hb_సమితి_t       *సమితి,
	    hb_codepoint_t  codepoint);

HB_EXTERN శూన్య
hb_సమితి_del_range (hb_సమితి_t       *సమితి,
		  hb_codepoint_t  మొదటి,
		  hb_codepoint_t  చివరి);

HB_EXTERN hb_bool_t
hb_సమితి_is_equal (స్థిరరాశి hb_సమితి_t *సమితి,
		 స్థిరరాశి hb_సమితి_t *other);

HB_EXTERN hb_bool_t
hb_సమితి_intersects (స్థిరరాశి hb_సమితి_t *సమితి,
		   స్థిరరాశి hb_సమితి_t *other);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_సమితి_hash (స్థిరరాశి hb_సమితి_t *సమితి);

HB_EXTERN hb_bool_t
hb_సమితి_is_subset (స్థిరరాశి hb_సమితి_t *సమితి,
		  స్థిరరాశి hb_సమితి_t *larger_సమితి);

HB_EXTERN శూన్య
hb_సమితి_సమితి (hb_సమితి_t       *సమితి,
	    స్థిరరాశి hb_సమితి_t *other);

HB_EXTERN శూన్య
hb_సమితి_union (hb_సమితి_t       *సమితి,
	      స్థిరరాశి hb_సమితి_t *other);

HB_EXTERN శూన్య
hb_సమితి_intersect (hb_సమితి_t       *సమితి,
		  స్థిరరాశి hb_సమితి_t *other);

HB_EXTERN శూన్య
hb_సమితి_వ్యవకలనం (hb_సమితి_t       *సమితి,
		 స్థిరరాశి hb_సమితి_t *other);

HB_EXTERN శూన్య
hb_సమితి_సౌష్ఠవ_difference (hb_సమితి_t       *సమితి,
			     స్థిరరాశి hb_సమితి_t *other);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_సమితి_get_population (స్థిరరాశి hb_సమితి_t *సమితి);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_సమితి_get_min (స్థిరరాశి hb_సమితి_t *సమితి);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_సమితి_get_max (స్థిరరాశి hb_సమితి_t *సమితి);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_సమితి_తదుపరి (స్థిరరాశి hb_సమితి_t *సమితి,
	     hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_సమితి_మునుపటి (స్థిరరాశి hb_సమితి_t *సమితి,
		 hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_సమితి_తదుపరి_range (స్థిరరాశి hb_సమితి_t *సమితి,
		   hb_codepoint_t *మొదటి,
		   hb_codepoint_t *చివరి);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_సమితి_మునుపటి_range (స్థిరరాశి hb_సమితి_t *సమితి,
		       hb_codepoint_t *మొదటి,
		       hb_codepoint_t *చివరి);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_సమితి_తదుపరి_many (స్థిరరాశి hb_సమితి_t *సమితి,
		  hb_codepoint_t  codepoint,
		  hb_codepoint_t *బహిర్గతం,
		  గుర్తులేని పూర్ణాంకం    కొలత);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_SET_H */
