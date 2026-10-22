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

#إذا !defined(HB_H_درون) && !defined(HB_NO_SINGLE_سرآیند_ERROR)
#خطا "Include <hb.h> instead."
#پایان_اگر

#اگر_تعریف_نشده HB_MAP_H
#تعریف_کن HB_MAP_H

#ضمن "hb-common.h"
#ضمن "hb-set.h"

HB_BEGIN_DECLS


/**
 * HB_MAP_VALUE_INVALID:
 *
 * Unset #hb_map_t value.
 *
 * Since: 1.7.7
 */
#تعریف_کن HB_MAP_مقدار_INVALID HB_CODEPOINT_INVALID

/**
 * hb_map_t:
 *
 * Data type for holding integer-to-integer hash maps.
 *
 **/
تعريفنوع بنية hb_map_t hb_map_t;


HB_EXTERN hb_map_t *
hb_map_create (فراغ);

HB_EXTERN hb_map_t *
hb_map_get_خالی (فراغ);

HB_EXTERN hb_map_t *
hb_map_reference (hb_map_t *map);

HB_EXTERN فراغ
hb_map_destroy (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_مجموعه_user_داده (hb_map_t           *map,
		      hb_user_داده_کلید_t *کلید_2,
		      فراغ *              داده,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN فراغ *
hb_map_get_user_داده (ثابت hb_map_t     *map,
		      hb_user_داده_کلید_t *کلید_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_map_تخصیص_successful (ثابت hb_map_t *map);

HB_EXTERN hb_map_t *
hb_map_copy (ثابت hb_map_t *map);

HB_EXTERN فراغ
hb_map_clear (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_خالی (ثابت hb_map_t *map);

HB_EXTERN بیعلامت صحيح
hb_map_get_population (ثابت hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_equal (ثابت hb_map_t *map,
		 ثابت hb_map_t *other);

HB_EXTERN بیعلامت صحيح
hb_map_hash (ثابت hb_map_t *map);

HB_EXTERN فراغ
hb_map_مجموعه (hb_map_t       *map,
	    hb_codepoint_t  کلید_2,
	    hb_codepoint_t  مقدار_2);

HB_EXTERN hb_codepoint_t
hb_map_get (ثابت hb_map_t *map,
	    hb_codepoint_t  کلید_2);

HB_EXTERN فراغ
hb_map_del (hb_map_t       *map,
	    hb_codepoint_t  کلید_2);

HB_EXTERN hb_bool_t
hb_map_has (ثابت hb_map_t *map,
	    hb_codepoint_t  کلید_2);

HB_EXTERN فراغ
hb_map_update (hb_map_t *map,
	       ثابت hb_map_t *other);

/* Pass -1 in for idx to get started. */
HB_EXTERN hb_bool_t
hb_map_بعدی (ثابت hb_map_t *map,
	     صحيح *idx,
	     hb_codepoint_t *کلید_2,
	     hb_codepoint_t *مقدار_2);

HB_EXTERN فراغ
hb_map_کلیدها (ثابت hb_map_t *map,
	     hb_مجموعه_t *کلیدها);

HB_EXTERN فراغ
hb_map_مقادیر (ثابت hb_map_t *map,
	       hb_مجموعه_t *مقادیر);

HB_انتها_DECLS

#پایان_اگر /* HB_MAP_H */
