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

#إذا !defined(HB_H_دننه) && !defined(HB_NO_SINGLE_سرلیک_ERROR)
#تېروتنه "Include <hb.h> instead."
#د_که_پای

#که_نه_تعریف_شوی HB_سېټ_H
#تعریف_کړه HB_سېټ_H

#ضمن "hb-common.h"

HB_BEGIN_DECLS


/**
 * HB_SET_VALUE_INVALID:
 *
 * Unset #hb_set_t value.
 *
 * Since: 0.9.21
 */
#تعریف_کړه HB_سېټ_ارزښت_INVALID HB_CODEPOINT_INVALID

/**
 * hb_set_t:
 *
 * Data type for holding a set of integers. #hb_set_t's are
 * used to gather and contain glyph IDs, Unicode code
 * points, and various other collections of discrete 
 * values.
 *
 **/
تعريفنوع بنية hb_سېټ_t hb_سېټ_t;


HB_EXTERN hb_سېټ_t *
hb_سېټ_create (فراغ);

HB_EXTERN hb_سېټ_t *
hb_سېټ_get_تشه (فراغ);

HB_EXTERN hb_سېټ_t *
hb_سېټ_reference (hb_سېټ_t *سېټ);

HB_EXTERN فراغ
hb_سېټ_destroy (hb_سېټ_t *سېټ);

HB_EXTERN hb_bool_t
hb_سېټ_سېټ_user_معلومات (hb_سېټ_t           *سېټ,
		      hb_user_معلومات_کلی_t *کلی_2,
		      فراغ *              معلومات,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN فراغ *
hb_سېټ_get_user_معلومات (ثابت hb_سېټ_t     *سېټ,
		      hb_user_معلومات_کلی_t *کلی_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_سېټ_ویش_successful (ثابت hb_سېټ_t *سېټ);

HB_EXTERN hb_سېټ_t *
hb_سېټ_copy (ثابت hb_سېټ_t *سېټ);

HB_EXTERN فراغ
hb_سېټ_clear (hb_سېټ_t *سېټ);

HB_EXTERN hb_bool_t
hb_سېټ_is_تشه (ثابت hb_سېټ_t *سېټ);

HB_EXTERN فراغ
hb_سېټ_invert (hb_سېټ_t *سېټ);

HB_EXTERN hb_bool_t
hb_سېټ_is_inverted (ثابت hb_سېټ_t *سېټ);

HB_EXTERN hb_bool_t
hb_سېټ_has (ثابت hb_سېټ_t *سېټ,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_سېټ_جمع_کړه (hb_سېټ_t       *سېټ,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_سېټ_جمع_کړه_range (hb_سېټ_t       *سېټ,
		  hb_codepoint_t  لومړی,
		  hb_codepoint_t  وروستی);

HB_EXTERN فراغ
hb_سېټ_جمع_کړه_sorted_array (hb_سېټ_t             *سېټ,
		         ثابت hb_codepoint_t *sorted_codepoints,
		         بینښه صحيح          عدد_codepoints);

HB_EXTERN فراغ
hb_سېټ_del (hb_سېټ_t       *سېټ,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_سېټ_del_range (hb_سېټ_t       *سېټ,
		  hb_codepoint_t  لومړی,
		  hb_codepoint_t  وروستی);

HB_EXTERN hb_bool_t
hb_سېټ_is_equal (ثابت hb_سېټ_t *سېټ,
		 ثابت hb_سېټ_t *other);

HB_EXTERN hb_bool_t
hb_سېټ_intersects (ثابت hb_سېټ_t *سېټ,
		   ثابت hb_سېټ_t *other);

HB_EXTERN بینښه صحيح
hb_سېټ_hash (ثابت hb_سېټ_t *سېټ);

HB_EXTERN hb_bool_t
hb_سېټ_is_subset (ثابت hb_سېټ_t *سېټ,
		  ثابت hb_سېټ_t *larger_سېټ);

HB_EXTERN فراغ
hb_سېټ_سېټ (hb_سېټ_t       *سېټ,
	    ثابت hb_سېټ_t *other);

HB_EXTERN فراغ
hb_سېټ_union (hb_سېټ_t       *سېټ,
	      ثابت hb_سېټ_t *other);

HB_EXTERN فراغ
hb_سېټ_intersect (hb_سېټ_t       *سېټ,
		  ثابت hb_سېټ_t *other);

HB_EXTERN فراغ
hb_سېټ_کمول (hb_سېټ_t       *سېټ,
		 ثابت hb_سېټ_t *other);

HB_EXTERN فراغ
hb_سېټ_متناظر_difference (hb_سېټ_t       *سېټ,
			     ثابت hb_سېټ_t *other);

HB_EXTERN بینښه صحيح
hb_سېټ_get_population (ثابت hb_سېټ_t *سېټ);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_سېټ_get_min (ثابت hb_سېټ_t *سېټ);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_سېټ_get_max (ثابت hb_سېټ_t *سېټ);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_سېټ_بل (ثابت hb_سېټ_t *سېټ,
	     hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_سېټ_پخوانی (ثابت hb_سېټ_t *سېټ,
		 hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_سېټ_بل_range (ثابت hb_سېټ_t *سېټ,
		   hb_codepoint_t *لومړی,
		   hb_codepoint_t *وروستی);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_سېټ_پخوانی_range (ثابت hb_سېټ_t *سېټ,
		       hb_codepoint_t *لومړی,
		       hb_codepoint_t *وروستی);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN بینښه صحيح
hb_سېټ_بل_many (ثابت hb_سېټ_t *سېټ,
		  hb_codepoint_t  codepoint,
		  hb_codepoint_t *بهر,
		  بینښه صحيح    اندازه);

HB_پای_ټکی_DECLS

#د_که_پای /* HB_SET_H */
