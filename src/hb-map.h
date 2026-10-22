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

#إذا !defined(HB_H_دننه) && !defined(HB_NO_SINGLE_سرلیک_ERROR)
#تېروتنه "Include <hb.h> instead."
#د_که_پای

#که_نه_تعریف_شوی HB_MAP_H
#تعریف_کړه HB_MAP_H

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
#تعریف_کړه HB_MAP_ارزښت_INVALID HB_CODEPOINT_INVALID

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
hb_map_get_تشه (فراغ);

HB_EXTERN hb_map_t *
hb_map_reference (hb_map_t *map);

HB_EXTERN فراغ
hb_map_destroy (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_سېټ_user_معلومات (hb_map_t           *map,
		      hb_user_معلومات_کلی_t *کلی_2,
		      فراغ *              معلومات,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN فراغ *
hb_map_get_user_معلومات (ثابت hb_map_t     *map,
		      hb_user_معلومات_کلی_t *کلی_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_map_ویش_successful (ثابت hb_map_t *map);

HB_EXTERN hb_map_t *
hb_map_copy (ثابت hb_map_t *map);

HB_EXTERN فراغ
hb_map_clear (hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_تشه (ثابت hb_map_t *map);

HB_EXTERN بینښه صحيح
hb_map_get_population (ثابت hb_map_t *map);

HB_EXTERN hb_bool_t
hb_map_is_equal (ثابت hb_map_t *map,
		 ثابت hb_map_t *other);

HB_EXTERN بینښه صحيح
hb_map_hash (ثابت hb_map_t *map);

HB_EXTERN فراغ
hb_map_سېټ (hb_map_t       *map,
	    hb_codepoint_t  کلی_2,
	    hb_codepoint_t  ارزښت_2);

HB_EXTERN hb_codepoint_t
hb_map_get (ثابت hb_map_t *map,
	    hb_codepoint_t  کلی_2);

HB_EXTERN فراغ
hb_map_del (hb_map_t       *map,
	    hb_codepoint_t  کلی_2);

HB_EXTERN hb_bool_t
hb_map_has (ثابت hb_map_t *map,
	    hb_codepoint_t  کلی_2);

HB_EXTERN فراغ
hb_map_update (hb_map_t *map,
	       ثابت hb_map_t *other);

/* Pass -1 in for idx to get started. */
HB_EXTERN hb_bool_t
hb_map_بل (ثابت hb_map_t *map,
	     صحيح *idx,
	     hb_codepoint_t *کلی_2,
	     hb_codepoint_t *ارزښت_2);

HB_EXTERN فراغ
hb_map_کلیګانې (ثابت hb_map_t *map,
	     hb_سېټ_t *کلیګانې);

HB_EXTERN فراغ
hb_map_ارزښتونه (ثابت hb_map_t *map,
	       hb_سېټ_t *ارزښتونه);

HB_پای_ټکی_DECLS

#د_که_پای /* HB_MAP_H */
