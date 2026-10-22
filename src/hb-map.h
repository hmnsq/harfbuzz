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

#ಯದಿ !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#error "Include <hb.h> instead."
#endif

#ifndef HB_MAP_H
#define HB_MAP_H

#ಅನ್ತರ್ಭಾವ "hb-common.h"
#ಅನ್ತರ್ಭಾವ "hb-set.h"

HB_BEGIN_DECLS


/**
 * HB_MAP_VALUE_INVALID:
 *
 * Unset #hb_map_t value.
 *
 * Since: 1.7.7
 */
#define HB_MAP_ಮೌಲ್ಯ_INVALID HB_CODEPOINT_INVALID

/**
 * hb_map_t:
 *
 * Data type for holding integer-to-integer hash maps.
 *
 **/
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ hb_map_t hb_map_t;


HB_EXTERN hb_map_t *
hb_map_create (ಶೂನ್ಯ);

HB_EXTERN hb_map_t *
hb_map_get_empty (ಶೂನ್ಯ);

HB_EXTERN hb_map_t *
hb_map_reference (hb_map_t *map);

HB_EXTERN ಶೂನ್ಯ
hb_map_destroy (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_set_user_data (hb_map_t           *map,
		      hb_user_data_ಕೀಲಿ_t *ಕೀಲಿ_2,
		      ಶೂನ್ಯ *              data,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN ಶೂನ್ಯ *
hb_map_get_user_data (ಧ್ರುವ hb_map_t     *map,
		      hb_user_data_ಕೀಲಿ_t *ಕೀಲಿ_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_map_allocation_successful (ಧ್ರುವ hb_map_t *map);

HB_EXTERN hb_map_t *
hb_map_copy (ಧ್ರುವ hb_map_t *map);

HB_EXTERN ಶೂನ್ಯ
hb_map_clear (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_empty (ಧ್ರುವ hb_map_t *map);

HB_EXTERN unsigned ಪೂರ್ಣಾಙ್ಕ
hb_map_get_population (ಧ್ರುವ hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_equal (ಧ್ರುವ hb_map_t *map,
		 ಧ್ರುವ hb_map_t *other);

HB_EXTERN unsigned ಪೂರ್ಣಾಙ್ಕ
hb_map_hash (ಧ್ರುವ hb_map_t *map);

HB_EXTERN ಶೂನ್ಯ
hb_map_set (hb_map_t       *map,
	    hb_codepoint_t  ಕೀಲಿ_2,
	    hb_codepoint_t  ಮೌಲ್ಯ_2);

HB_EXTERN hb_codepoint_t
hb_map_get (ಧ್ರುವ hb_map_t *map,
	    hb_codepoint_t  ಕೀಲಿ_2);

HB_EXTERN ಶೂನ್ಯ
hb_map_del (hb_map_t       *map,
	    hb_codepoint_t  ಕೀಲಿ_2);

HB_EXTERN hb_bool_t
hb_map_has (ಧ್ರುವ hb_map_t *map,
	    hb_codepoint_t  ಕೀಲಿ_2);

HB_EXTERN ಶೂನ್ಯ
hb_map_update (hb_map_t *map,
	       ಧ್ರುವ hb_map_t *other);

/* Pass -1 in for idx to get started. */
HB_EXTERN hb_bool_t
hb_map_next (ಧ್ರುವ hb_map_t *map,
	     ಪೂರ್ಣಾಙ್ಕ *idx,
	     hb_codepoint_t *ಕೀಲಿ_2,
	     hb_codepoint_t *ಮೌಲ್ಯ_2);

HB_EXTERN ಶೂನ್ಯ
hb_map_ಕೀಲಿಗಳು (ಧ್ರುವ hb_map_t *map,
	     hb_set_t *ಕೀಲಿಗಳು);

HB_EXTERN ಶೂನ್ಯ
hb_map_ಮೌಲ್ಯಗಳು (ಧ್ರುವ hb_map_t *map,
	       hb_set_t *ಮೌಲ್ಯಗಳು);

HB_END_DECLS

#endif /* HB_MAP_H */
