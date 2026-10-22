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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸੈੱਟ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸੈੱਟ_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-common.h"

HB_BEGIN_DECLS


/**
 * HB_SET_VALUE_INVALID:
 *
 * Unset #hb_set_t value.
 *
 * Since: 0.9.21
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸੈੱਟ_ਮੁੱਲ_INVALID HB_CODEPOINT_INVALID

/**
 * hb_set_t:
 *
 * Data type for holding a set of integers. #hb_set_t's are
 * used to gather and contain glyph IDs, Unicode code
 * points, and various other collections of discrete 
 * values.
 *
 **/
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_ਸੈੱਟ_t hb_ਸੈੱਟ_t;


HB_EXTERN hb_ਸੈੱਟ_t *
hb_ਸੈੱਟ_create (ਖਾਲੀ);

HB_EXTERN hb_ਸੈੱਟ_t *
hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ (ਖਾਲੀ);

HB_EXTERN hb_ਸੈੱਟ_t *
hb_ਸੈੱਟ_reference (hb_ਸੈੱਟ_t *ਸੈੱਟ);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_destroy (hb_ਸੈੱਟ_t *ਸੈੱਟ);

HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_ਸੈੱਟ_t           *ਸੈੱਟ,
		      hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2,
		      ਖਾਲੀ *              ਅੰਕੜੇ,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN ਖਾਲੀ *
hb_ਸੈੱਟ_get_user_ਅੰਕੜੇ (ਅਚਲ hb_ਸੈੱਟ_t     *ਸੈੱਟ,
		      hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_ਵੰਡ_successful (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ);

HB_EXTERN hb_ਸੈੱਟ_t *
hb_ਸੈੱਟ_copy (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_clear (hb_ਸੈੱਟ_t *ਸੈੱਟ);

HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_is_ਖਾਲੀ_ਥਾਵਾਂ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_invert (hb_ਸੈੱਟ_t *ਸੈੱਟ);

HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_is_inverted (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ);

HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_has (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
	    hb_codepoint_t  codepoint);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_ਜੋੜੋ (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
	    hb_codepoint_t  codepoint);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_ਜੋੜੋ_range (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
		  hb_codepoint_t  ਪਹਿਲਾ,
		  hb_codepoint_t  ਆਖਰੀ);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_ਜੋੜੋ_sorted_array (hb_ਸੈੱਟ_t             *ਸੈੱਟ,
		         ਅਚਲ hb_codepoint_t *sorted_codepoints,
		         ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          ਅੰਕ_codepoints);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_del (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
	    hb_codepoint_t  codepoint);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_del_range (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
		  hb_codepoint_t  ਪਹਿਲਾ,
		  hb_codepoint_t  ਆਖਰੀ);

HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_is_equal (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		 ਅਚਲ hb_ਸੈੱਟ_t *other);

HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_intersects (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		   ਅਚਲ hb_ਸੈੱਟ_t *other);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਸੈੱਟ_hash (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ);

HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_is_subset (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		  ਅਚਲ hb_ਸੈੱਟ_t *larger_ਸੈੱਟ);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_ਸੈੱਟ (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
	    ਅਚਲ hb_ਸੈੱਟ_t *other);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_union (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
	      ਅਚਲ hb_ਸੈੱਟ_t *other);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_intersect (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
		  ਅਚਲ hb_ਸੈੱਟ_t *other);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_ਘਟਾਓ (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
		 ਅਚਲ hb_ਸੈੱਟ_t *other);

HB_EXTERN ਖਾਲੀ
hb_ਸੈੱਟ_ਸਮਮਿਤ_difference (hb_ਸੈੱਟ_t       *ਸੈੱਟ,
			     ਅਚਲ hb_ਸੈੱਟ_t *other);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਸੈੱਟ_get_population (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_ਸੈੱਟ_get_min (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_ਸੈੱਟ_get_max (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_ਅਗਲਾ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
	     hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_ਪਿਛਲਾ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		 hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_ਅਗਲਾ_range (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		   hb_codepoint_t *ਪਹਿਲਾ,
		   hb_codepoint_t *ਆਖਰੀ);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_ਸੈੱਟ_ਪਿਛਲਾ_range (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		       hb_codepoint_t *ਪਹਿਲਾ,
		       hb_codepoint_t *ਆਖਰੀ);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਸੈੱਟ_ਅਗਲਾ_many (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
		  hb_codepoint_t  codepoint,
		  hb_codepoint_t *ਬਾਹਰਲਾ,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ);

HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_SET_H */
