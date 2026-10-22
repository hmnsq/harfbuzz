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

#ଯଦି !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#error "Include <hb.h> instead."
#endif

#ifndef HB_SET_H
#define HB_SET_H

#ଅନ୍ତର୍ଭାଵ "hb-common.h"

HB_BEGIN_DECLS


/**
 * HB_SET_VALUE_INVALID:
 *
 * Unset #hb_set_t value.
 *
 * Since: 0.9.21
 */
#define HB_SET_ମୂଲ୍ୟ_INVALID HB_CODEPOINT_INVALID

/**
 * hb_set_t:
 *
 * Data type for holding a set of integers. #hb_set_t's are
 * used to gather and contain glyph IDs, Unicode code
 * points, and various other collections of discrete 
 * values.
 *
 **/
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା hb_set_t hb_set_t;


HB_EXTERN hb_set_t *
hb_set_create (ଶୂନ୍ୟ);

HB_EXTERN hb_set_t *
hb_set_get_empty (ଶୂନ୍ୟ);

HB_EXTERN hb_set_t *
hb_set_reference (hb_set_t *set);

HB_EXTERN ଶୂନ୍ୟ
hb_set_destroy (hb_set_t *set);

HB_EXTERN hb_bool_t
hb_set_set_user_data (hb_set_t           *set,
		      hb_user_data_ଚାବି_t *ଚାବି_2,
		      ଶୂନ୍ୟ *              data,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN ଶୂନ୍ୟ *
hb_set_get_user_data (ଅଚଳ hb_set_t     *set,
		      hb_user_data_ଚାବି_t *ଚାବି_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_set_allocation_successful (ଅଚଳ hb_set_t *set);

HB_EXTERN hb_set_t *
hb_set_copy (ଅଚଳ hb_set_t *set);

HB_EXTERN ଶୂନ୍ୟ
hb_set_clear (hb_set_t *set);

HB_EXTERN hb_bool_t
hb_set_is_empty (ଅଚଳ hb_set_t *set);

HB_EXTERN ଶୂନ୍ୟ
hb_set_invert (hb_set_t *set);

HB_EXTERN hb_bool_t
hb_set_is_inverted (ଅଚଳ hb_set_t *set);

HB_EXTERN hb_bool_t
hb_set_has (ଅଚଳ hb_set_t *set,
	    hb_codepoint_t  codepoint);

HB_EXTERN ଶୂନ୍ୟ
hb_set_add (hb_set_t       *set,
	    hb_codepoint_t  codepoint);

HB_EXTERN ଶୂନ୍ୟ
hb_set_add_range (hb_set_t       *set,
		  hb_codepoint_t  first,
		  hb_codepoint_t  last);

HB_EXTERN ଶୂନ୍ୟ
hb_set_add_sorted_array (hb_set_t             *set,
		         ଅଚଳ hb_codepoint_t *sorted_codepoints,
		         unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା          ଅଙ୍କ_codepoints);

HB_EXTERN ଶୂନ୍ୟ
hb_set_del (hb_set_t       *set,
	    hb_codepoint_t  codepoint);

HB_EXTERN ଶୂନ୍ୟ
hb_set_del_range (hb_set_t       *set,
		  hb_codepoint_t  first,
		  hb_codepoint_t  last);

HB_EXTERN hb_bool_t
hb_set_is_equal (ଅଚଳ hb_set_t *set,
		 ଅଚଳ hb_set_t *other);

HB_EXTERN hb_bool_t
hb_set_intersects (ଅଚଳ hb_set_t *set,
		   ଅଚଳ hb_set_t *other);

HB_EXTERN unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_set_hash (ଅଚଳ hb_set_t *set);

HB_EXTERN hb_bool_t
hb_set_is_subset (ଅଚଳ hb_set_t *set,
		  ଅଚଳ hb_set_t *larger_set);

HB_EXTERN ଶୂନ୍ୟ
hb_set_set (hb_set_t       *set,
	    ଅଚଳ hb_set_t *other);

HB_EXTERN ଶୂନ୍ୟ
hb_set_union (hb_set_t       *set,
	      ଅଚଳ hb_set_t *other);

HB_EXTERN ଶୂନ୍ୟ
hb_set_intersect (hb_set_t       *set,
		  ଅଚଳ hb_set_t *other);

HB_EXTERN ଶୂନ୍ୟ
hb_set_subtract (hb_set_t       *set,
		 ଅଚଳ hb_set_t *other);

HB_EXTERN ଶୂନ୍ୟ
hb_set_symmetric_difference (hb_set_t       *set,
			     ଅଚଳ hb_set_t *other);

HB_EXTERN unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_set_get_population (ଅଚଳ hb_set_t *set);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_set_get_min (ଅଚଳ hb_set_t *set);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_set_get_max (ଅଚଳ hb_set_t *set);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_set_next (ଅଚଳ hb_set_t *set,
	     hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_set_previous (ଅଚଳ hb_set_t *set,
		 hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_set_next_range (ଅଚଳ hb_set_t *set,
		   hb_codepoint_t *first,
		   hb_codepoint_t *last);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_set_previous_range (ଅଚଳ hb_set_t *set,
		       hb_codepoint_t *first,
		       hb_codepoint_t *last);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_set_next_many (ଅଚଳ hb_set_t *set,
		  hb_codepoint_t  codepoint,
		  hb_codepoint_t *out,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    size);

HB_END_DECLS

#endif /* HB_SET_H */
