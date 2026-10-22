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

#إذا !defined(HB_H_درون) && !defined(HB_NO_SINGLE_سرآیند_ERROR)
#خطا "Include <hb.h> instead."
#پایان_اگر

#اگر_تعریف_نشده HB_مجموعه_H
#تعریف_کن HB_مجموعه_H

#ضمن "hb-common.h"

HB_BEGIN_DECLS


/**
 * HB_SET_VALUE_INVALID:
 *
 * Unset #hb_set_t value.
 *
 * Since: 0.9.21
 */
#تعریف_کن HB_مجموعه_مقدار_INVALID HB_CODEPOINT_INVALID

/**
 * hb_set_t:
 *
 * Data type for holding a set of integers. #hb_set_t's are
 * used to gather and contain glyph IDs, Unicode code
 * points, and various other collections of discrete 
 * values.
 *
 **/
تعريفنوع بنية hb_مجموعه_t hb_مجموعه_t;


HB_EXTERN hb_مجموعه_t *
hb_مجموعه_create (فراغ);

HB_EXTERN hb_مجموعه_t *
hb_مجموعه_get_خالی (فراغ);

HB_EXTERN hb_مجموعه_t *
hb_مجموعه_reference (hb_مجموعه_t *مجموعه);

HB_EXTERN فراغ
hb_مجموعه_destroy (hb_مجموعه_t *مجموعه);

HB_EXTERN hb_bool_t
hb_مجموعه_مجموعه_user_داده (hb_مجموعه_t           *مجموعه,
		      hb_user_داده_کلید_t *کلید_2,
		      فراغ *              داده,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN فراغ *
hb_مجموعه_get_user_داده (ثابت hb_مجموعه_t     *مجموعه,
		      hb_user_داده_کلید_t *کلید_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_مجموعه_تخصیص_successful (ثابت hb_مجموعه_t *مجموعه);

HB_EXTERN hb_مجموعه_t *
hb_مجموعه_copy (ثابت hb_مجموعه_t *مجموعه);

HB_EXTERN فراغ
hb_مجموعه_clear (hb_مجموعه_t *مجموعه);

HB_EXTERN hb_bool_t
hb_مجموعه_is_خالی (ثابت hb_مجموعه_t *مجموعه);

HB_EXTERN فراغ
hb_مجموعه_invert (hb_مجموعه_t *مجموعه);

HB_EXTERN hb_bool_t
hb_مجموعه_is_inverted (ثابت hb_مجموعه_t *مجموعه);

HB_EXTERN hb_bool_t
hb_مجموعه_has (ثابت hb_مجموعه_t *مجموعه,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_مجموعه_جمع_کن (hb_مجموعه_t       *مجموعه,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_مجموعه_جمع_کن_range (hb_مجموعه_t       *مجموعه,
		  hb_codepoint_t  اولی,
		  hb_codepoint_t  آخرین);

HB_EXTERN فراغ
hb_مجموعه_جمع_کن_sorted_array (hb_مجموعه_t             *مجموعه,
		         ثابت hb_codepoint_t *sorted_codepoints,
		         بیعلامت صحيح          رقم_codepoints);

HB_EXTERN فراغ
hb_مجموعه_del (hb_مجموعه_t       *مجموعه,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_مجموعه_del_range (hb_مجموعه_t       *مجموعه,
		  hb_codepoint_t  اولی,
		  hb_codepoint_t  آخرین);

HB_EXTERN hb_bool_t
hb_مجموعه_is_equal (ثابت hb_مجموعه_t *مجموعه,
		 ثابت hb_مجموعه_t *other);

HB_EXTERN hb_bool_t
hb_مجموعه_intersects (ثابت hb_مجموعه_t *مجموعه,
		   ثابت hb_مجموعه_t *other);

HB_EXTERN بیعلامت صحيح
hb_مجموعه_hash (ثابت hb_مجموعه_t *مجموعه);

HB_EXTERN hb_bool_t
hb_مجموعه_is_subset (ثابت hb_مجموعه_t *مجموعه,
		  ثابت hb_مجموعه_t *larger_مجموعه);

HB_EXTERN فراغ
hb_مجموعه_مجموعه (hb_مجموعه_t       *مجموعه,
	    ثابت hb_مجموعه_t *other);

HB_EXTERN فراغ
hb_مجموعه_union (hb_مجموعه_t       *مجموعه,
	      ثابت hb_مجموعه_t *other);

HB_EXTERN فراغ
hb_مجموعه_intersect (hb_مجموعه_t       *مجموعه,
		  ثابت hb_مجموعه_t *other);

HB_EXTERN فراغ
hb_مجموعه_تفریق (hb_مجموعه_t       *مجموعه,
		 ثابت hb_مجموعه_t *other);

HB_EXTERN فراغ
hb_مجموعه_متقارن_difference (hb_مجموعه_t       *مجموعه,
			     ثابت hb_مجموعه_t *other);

HB_EXTERN بیعلامت صحيح
hb_مجموعه_get_population (ثابت hb_مجموعه_t *مجموعه);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_مجموعه_get_min (ثابت hb_مجموعه_t *مجموعه);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_مجموعه_get_max (ثابت hb_مجموعه_t *مجموعه);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_مجموعه_بعدی (ثابت hb_مجموعه_t *مجموعه,
	     hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_مجموعه_قبلی (ثابت hb_مجموعه_t *مجموعه,
		 hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_مجموعه_بعدی_range (ثابت hb_مجموعه_t *مجموعه,
		   hb_codepoint_t *اولی,
		   hb_codepoint_t *آخرین);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_مجموعه_قبلی_range (ثابت hb_مجموعه_t *مجموعه,
		       hb_codepoint_t *اولی,
		       hb_codepoint_t *آخرین);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN بیعلامت صحيح
hb_مجموعه_بعدی_many (ثابت hb_مجموعه_t *مجموعه,
		  hb_codepoint_t  codepoint,
		  hb_codepoint_t *بیرون,
		  بیعلامت صحيح    اندازه);

HB_انتها_DECLS

#پایان_اگر /* HB_SET_H */
