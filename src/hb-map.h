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

#إذا !defined(HB_H_اندريون) && !defined(HB_NO_SINGLE_سرو_ERROR)
#غلطي "Include <hb.h> instead."
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_MAP_H
#وصف_ڪريو HB_MAP_H

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
#وصف_ڪريو HB_MAP_قدر_INVALID HB_CODEPOINT_INVALID

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
hb_map_get_خالي_جايون (فراغ);

HB_EXTERN hb_map_t *
hb_map_reference (hb_map_t *map);

HB_EXTERN فراغ
hb_map_destroy (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_سيٽ_user_معلومات (hb_map_t           *map,
		      hb_user_معلومات_ڪنجي_t *ڪنجي_2,
		      فراغ *              معلومات,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN فراغ *
hb_map_get_user_معلومات (ثابت hb_map_t     *map,
		      hb_user_معلومات_ڪنجي_t *ڪنجي_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_map_ورهاست_successful (ثابت hb_map_t *map);

HB_EXTERN hb_map_t *
hb_map_copy (ثابت hb_map_t *map);

HB_EXTERN فراغ
hb_map_clear (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_خالي_جايون (ثابت hb_map_t *map);

HB_EXTERN بنانشان صحيح
hb_map_get_population (ثابت hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_equal (ثابت hb_map_t *map,
		 ثابت hb_map_t *other);

HB_EXTERN بنانشان صحيح
hb_map_hash (ثابت hb_map_t *map);

HB_EXTERN فراغ
hb_map_سيٽ (hb_map_t       *map,
	    hb_codepoint_t  ڪنجي_2,
	    hb_codepoint_t  قدر_2);

HB_EXTERN hb_codepoint_t
hb_map_get (ثابت hb_map_t *map,
	    hb_codepoint_t  ڪنجي_2);

HB_EXTERN فراغ
hb_map_del (hb_map_t       *map,
	    hb_codepoint_t  ڪنجي_2);

HB_EXTERN hb_bool_t
hb_map_has (ثابت hb_map_t *map,
	    hb_codepoint_t  ڪنجي_2);

HB_EXTERN فراغ
hb_map_update (hb_map_t *map,
	       ثابت hb_map_t *other);

/* Pass -1 in for idx to get started. */
HB_EXTERN hb_bool_t
hb_map_ايندڙ (ثابت hb_map_t *map,
	     صحيح *idx,
	     hb_codepoint_t *ڪنجي_2,
	     hb_codepoint_t *قدر_2);

HB_EXTERN فراغ
hb_map_ڪنجيون (ثابت hb_map_t *map,
	     hb_سيٽ_t *ڪنجيون);

HB_EXTERN فراغ
hb_map_قدرون (ثابت hb_map_t *map,
	       hb_سيٽ_t *قدرون);

HB_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_MAP_H */
