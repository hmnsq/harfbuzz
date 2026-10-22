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

#ਜੇ !defined(HB_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
#ਗਲਤੀ "Include <hb.h> instead."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAP_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAP_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.h"

HB_BEGIN_DECLS


/**
 * HB_MAP_VALUE_INVALID:
 *
 * Unset #hb_map_t value.
 *
 * Since: 1.7.7
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAP_ਮੁੱਲ_INVALID HB_CODEPOINT_INVALID

/**
 * hb_map_t:
 *
 * Data type for holding integer-to-integer hash maps.
 *
 **/
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_map_t hb_map_t;


HB_EXTERN hb_map_t *
hb_map_create (ਖਾਲੀ);

HB_EXTERN hb_map_t *
hb_map_get_ਖਾਲੀ_ਥਾਵਾਂ (ਖਾਲੀ);

HB_EXTERN hb_map_t *
hb_map_reference (hb_map_t *map);

HB_EXTERN ਖਾਲੀ
hb_map_destroy (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_map_t           *map,
		      hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2,
		      ਖਾਲੀ *              ਅੰਕੜੇ,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN ਖਾਲੀ *
hb_map_get_user_ਅੰਕੜੇ (ਅਚਲ hb_map_t     *map,
		      hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_map_ਵੰਡ_successful (ਅਚਲ hb_map_t *map);

HB_EXTERN hb_map_t *
hb_map_copy (ਅਚਲ hb_map_t *map);

HB_EXTERN ਖਾਲੀ
hb_map_clear (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_ਖਾਲੀ_ਥਾਵਾਂ (ਅਚਲ hb_map_t *map);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_map_get_population (ਅਚਲ hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_equal (ਅਚਲ hb_map_t *map,
		 ਅਚਲ hb_map_t *other);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_map_hash (ਅਚਲ hb_map_t *map);

HB_EXTERN ਖਾਲੀ
hb_map_ਸੈੱਟ (hb_map_t       *map,
	    hb_codepoint_t  ਕੁੰਜੀ_2,
	    hb_codepoint_t  ਮੁੱਲ_2);

HB_EXTERN hb_codepoint_t
hb_map_get (ਅਚਲ hb_map_t *map,
	    hb_codepoint_t  ਕੁੰਜੀ_2);

HB_EXTERN ਖਾਲੀ
hb_map_del (hb_map_t       *map,
	    hb_codepoint_t  ਕੁੰਜੀ_2);

HB_EXTERN hb_bool_t
hb_map_has (ਅਚਲ hb_map_t *map,
	    hb_codepoint_t  ਕੁੰਜੀ_2);

HB_EXTERN ਖਾਲੀ
hb_map_update (hb_map_t *map,
	       ਅਚਲ hb_map_t *other);

/* Pass -1 in for idx to get started. */
HB_EXTERN hb_bool_t
hb_map_ਅਗਲਾ (ਅਚਲ hb_map_t *map,
	     ਪੂਰਨ *idx,
	     hb_codepoint_t *ਕੁੰਜੀ_2,
	     hb_codepoint_t *ਮੁੱਲ_2);

HB_EXTERN ਖਾਲੀ
hb_map_ਕੁੰਜੀਆਂ (ਅਚਲ hb_map_t *map,
	     hb_ਸੈੱਟ_t *ਕੁੰਜੀਆਂ);

HB_EXTERN ਖਾਲੀ
hb_map_ਮੁੱਲਾਂ (ਅਚਲ hb_map_t *map,
	       hb_ਸੈੱਟ_t *ਮੁੱਲਾਂ);

HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_MAP_H */
