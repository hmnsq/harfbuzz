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

#जर !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#error "Include <hb.h> instead."
#endif

#ifndef HB_MAP_H
#define HB_MAP_H

#अन्तर्भाव "hb-common.h"
#अन्तर्भाव "hb-set.h"

HB_BEGIN_DECLS


/**
 * HB_MAP_VALUE_INVALID:
 *
 * Unset #hb_map_t value.
 *
 * Since: 1.7.7
 */
#define HB_MAP_मूल्य_INVALID HB_CODEPOINT_INVALID

/**
 * hb_map_t:
 *
 * Data type for holding integer-to-integer hash maps.
 *
 **/
प्रकारव्याख्या संरचना hb_map_t hb_map_t;


HB_EXTERN hb_map_t *
hb_map_create (शून्य);

HB_EXTERN hb_map_t *
hb_map_get_empty (शून्य);

HB_EXTERN hb_map_t *
hb_map_reference (hb_map_t *map);

HB_EXTERN शून्य
hb_map_destroy (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_set_user_data (hb_map_t           *map,
		      hb_user_data_किल्ली_t *किल्ली_2,
		      शून्य *              data,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN शून्य *
hb_map_get_user_data (स्थिरांक hb_map_t     *map,
		      hb_user_data_किल्ली_t *किल्ली_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_map_allocation_successful (स्थिरांक hb_map_t *map);

HB_EXTERN hb_map_t *
hb_map_copy (स्थिरांक hb_map_t *map);

HB_EXTERN शून्य
hb_map_clear (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_empty (स्थिरांक hb_map_t *map);

HB_EXTERN अचिन्हित पूर्णांक
hb_map_get_population (स्थिरांक hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_equal (स्थिरांक hb_map_t *map,
		 स्थिरांक hb_map_t *other);

HB_EXTERN अचिन्हित पूर्णांक
hb_map_hash (स्थिरांक hb_map_t *map);

HB_EXTERN शून्य
hb_map_set (hb_map_t       *map,
	    hb_codepoint_t  किल्ली_2,
	    hb_codepoint_t  मूल्य_2);

HB_EXTERN hb_codepoint_t
hb_map_get (स्थिरांक hb_map_t *map,
	    hb_codepoint_t  किल्ली_2);

HB_EXTERN शून्य
hb_map_del (hb_map_t       *map,
	    hb_codepoint_t  किल्ली_2);

HB_EXTERN hb_bool_t
hb_map_has (स्थिरांक hb_map_t *map,
	    hb_codepoint_t  किल्ली_2);

HB_EXTERN शून्य
hb_map_update (hb_map_t *map,
	       स्थिरांक hb_map_t *other);

/* Pass -1 in for idx to get started. */
HB_EXTERN hb_bool_t
hb_map_next (स्थिरांक hb_map_t *map,
	     पूर्णांक *idx,
	     hb_codepoint_t *किल्ली_2,
	     hb_codepoint_t *मूल्य_2);

HB_EXTERN शून्य
hb_map_किल्ल्या (स्थिरांक hb_map_t *map,
	     hb_set_t *किल्ल्या);

HB_EXTERN शून्य
hb_map_मूल्ये (स्थिरांक hb_map_t *map,
	       hb_set_t *मूल्ये);

HB_END_DECLS

#endif /* HB_MAP_H */
