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

#اگر !defined(HB_H_اندر) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb.h> instead."
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_MAP_H
#تعریف_کرو HB_MAP_H

#شامل "hb-common.h"
#شامل "hb-set.h"

HB_BEGIN_DECLS


/**
 * HB_MAP_VALUE_INVALID:
 *
 * Unset #hb_map_t value.
 *
 * Since: 1.7.7
 */
#تعریف_کرو HB_MAP_قدر_INVALID HB_CODEPOINT_INVALID

/**
 * hb_map_t:
 *
 * Data type for holding integer-to-integer hash maps.
 *
 **/
قسمتعریف ساخت hb_map_t hb_map_t;


HB_EXTERN hb_map_t *
hb_map_create (خالی);

HB_EXTERN hb_map_t *
hb_map_get_خالی_خانے (خالی);

HB_EXTERN hb_map_t *
hb_map_reference (hb_map_t *map);

HB_EXTERN خالی
hb_map_destroy (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_سیٹ_user_ڈیٹا (hb_map_t           *map,
		      hb_user_ڈیٹا_کلید_t *کلید_2,
		      خالی *              ڈیٹا,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN خالی *
hb_map_get_user_ڈیٹا (ثابت hb_map_t     *map,
		      hb_user_ڈیٹا_کلید_t *کلید_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_map_تخصیص_successful (ثابت hb_map_t *map);

HB_EXTERN hb_map_t *
hb_map_copy (ثابت hb_map_t *map);

HB_EXTERN خالی
hb_map_clear (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_خالی_خانے (ثابت hb_map_t *map);

HB_EXTERN غیرعلامتی صحیح
hb_map_get_population (ثابت hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_equal (ثابت hb_map_t *map,
		 ثابت hb_map_t *other);

HB_EXTERN غیرعلامتی صحیح
hb_map_hash (ثابت hb_map_t *map);

HB_EXTERN خالی
hb_map_سیٹ (hb_map_t       *map,
	    hb_codepoint_t  کلید_2,
	    hb_codepoint_t  قدر_2);

HB_EXTERN hb_codepoint_t
hb_map_get (ثابت hb_map_t *map,
	    hb_codepoint_t  کلید_2);

HB_EXTERN خالی
hb_map_del (hb_map_t       *map,
	    hb_codepoint_t  کلید_2);

HB_EXTERN hb_bool_t
hb_map_has (ثابت hb_map_t *map,
	    hb_codepoint_t  کلید_2);

HB_EXTERN خالی
hb_map_update (hb_map_t *map,
	       ثابت hb_map_t *other);

/* Pass -1 in for idx to get started. */
HB_EXTERN hb_bool_t
hb_map_اگلا (ثابت hb_map_t *map,
	     صحیح *idx,
	     hb_codepoint_t *کلید_2,
	     hb_codepoint_t *قدر_2);

HB_EXTERN خالی
hb_map_کلیدیں (ثابت hb_map_t *map,
	     hb_سیٹ_t *کلیدیں);

HB_EXTERN خالی
hb_map_اقدار (ثابت hb_map_t *map,
	       hb_سیٹ_t *اقدار);

HB_انتہا_DECLS

#اگر_ختم /* HB_MAP_H */
