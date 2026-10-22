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

#எனில் !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#error "Include <hb.h> instead."
#endif

#ifndef HB_MAP_H
#define HB_MAP_H

#அந்தர்பாவ "hb-common.h"
#அந்தர்பாவ "hb-set.h"

HB_BEGIN_DECLS


/**
 * HB_MAP_VALUE_INVALID:
 *
 * Unset #hb_map_t value.
 *
 * Since: 1.7.7
 */
#define HB_MAP_மதிப்பு_INVALID HB_CODEPOINT_INVALID

/**
 * hb_map_t:
 *
 * Data type for holding integer-to-integer hash maps.
 *
 **/
வகைவரை கட்டமைப்பு hb_map_t hb_map_t;


HB_EXTERN hb_map_t *
hb_map_create (வெறுமை);

HB_EXTERN hb_map_t *
hb_map_get_empty (வெறுமை);

HB_EXTERN hb_map_t *
hb_map_reference (hb_map_t *map);

HB_EXTERN வெறுமை
hb_map_destroy (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_set_user_data (hb_map_t           *map,
		      hb_user_data_சாவி_t *சாவி_2,
		      வெறுமை *              data,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN வெறுமை *
hb_map_get_user_data (மாறிலி hb_map_t     *map,
		      hb_user_data_சாவி_t *சாவி_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_map_allocation_successful (மாறிலி hb_map_t *map);

HB_EXTERN hb_map_t *
hb_map_copy (மாறிலி hb_map_t *map);

HB_EXTERN வெறுமை
hb_map_clear (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_empty (மாறிலி hb_map_t *map);

HB_EXTERN unsigned முழு
hb_map_get_population (மாறிலி hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_equal (மாறிலி hb_map_t *map,
		 மாறிலி hb_map_t *other);

HB_EXTERN unsigned முழு
hb_map_hash (மாறிலி hb_map_t *map);

HB_EXTERN வெறுமை
hb_map_set (hb_map_t       *map,
	    hb_codepoint_t  சாவி_2,
	    hb_codepoint_t  மதிப்பு_2);

HB_EXTERN hb_codepoint_t
hb_map_get (மாறிலி hb_map_t *map,
	    hb_codepoint_t  சாவி_2);

HB_EXTERN வெறுமை
hb_map_del (hb_map_t       *map,
	    hb_codepoint_t  சாவி_2);

HB_EXTERN hb_bool_t
hb_map_has (மாறிலி hb_map_t *map,
	    hb_codepoint_t  சாவி_2);

HB_EXTERN வெறுமை
hb_map_update (hb_map_t *map,
	       மாறிலி hb_map_t *other);

/* Pass -1 in for idx to get started. */
HB_EXTERN hb_bool_t
hb_map_next (மாறிலி hb_map_t *map,
	     முழு *idx,
	     hb_codepoint_t *சாவி_2,
	     hb_codepoint_t *மதிப்பு_2);

HB_EXTERN வெறுமை
hb_map_சாவிகள் (மாறிலி hb_map_t *map,
	     hb_set_t *சாவிகள்);

HB_EXTERN வெறுமை
hb_map_மதிப்புகள் (மாறிலி hb_map_t *map,
	       hb_set_t *மதிப்புகள்);

HB_END_DECLS

#endif /* HB_MAP_H */
