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

#إذا !defined(HB_H_وچ) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb.h> instead."
#جے_انت

#جے_بنا_تعریف HB_سیٹ_H
#تعریف_کرو HB_سیٹ_H

#ضمن "hb-common.h"

HB_BEGIN_DECLS


/**
 * HB_SET_VALUE_INVALID:
 *
 * Unset #hb_set_t value.
 *
 * Since: 0.9.21
 */
#تعریف_کرو HB_سیٹ_قدر_INVALID HB_CODEPOINT_INVALID

/**
 * hb_set_t:
 *
 * Data type for holding a set of integers. #hb_set_t's are
 * used to gather and contain glyph IDs, Unicode code
 * points, and various other collections of discrete 
 * values.
 *
 **/
تعريفنوع بنية hb_سیٹ_t hb_سیٹ_t;


HB_EXTERN hb_سیٹ_t *
hb_سیٹ_create (فراغ);

HB_EXTERN hb_سیٹ_t *
hb_سیٹ_get_خالی_تھاواں (فراغ);

HB_EXTERN hb_سیٹ_t *
hb_سیٹ_reference (hb_سیٹ_t *سیٹ);

HB_EXTERN فراغ
hb_سیٹ_destroy (hb_سیٹ_t *سیٹ);

HB_EXTERN hb_bool_t
hb_سیٹ_سیٹ_user_انکڑے (hb_سیٹ_t           *سیٹ,
		      hb_user_انکڑے_کلید_t *کلید_2,
		      فراغ *              انکڑے,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN فراغ *
hb_سیٹ_get_user_انکڑے (ثابت hb_سیٹ_t     *سیٹ,
		      hb_user_انکڑے_کلید_t *کلید_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_سیٹ_ونڈ_successful (ثابت hb_سیٹ_t *سیٹ);

HB_EXTERN hb_سیٹ_t *
hb_سیٹ_copy (ثابت hb_سیٹ_t *سیٹ);

HB_EXTERN فراغ
hb_سیٹ_clear (hb_سیٹ_t *سیٹ);

HB_EXTERN hb_bool_t
hb_سیٹ_is_خالی_تھاواں (ثابت hb_سیٹ_t *سیٹ);

HB_EXTERN فراغ
hb_سیٹ_invert (hb_سیٹ_t *سیٹ);

HB_EXTERN hb_bool_t
hb_سیٹ_is_inverted (ثابت hb_سیٹ_t *سیٹ);

HB_EXTERN hb_bool_t
hb_سیٹ_has (ثابت hb_سیٹ_t *سیٹ,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_سیٹ_جوڑو (hb_سیٹ_t       *سیٹ,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_سیٹ_جوڑو_range (hb_سیٹ_t       *سیٹ,
		  hb_codepoint_t  پہلا,
		  hb_codepoint_t  آخری);

HB_EXTERN فراغ
hb_سیٹ_جوڑو_sorted_array (hb_سیٹ_t             *سیٹ,
		         ثابت hb_codepoint_t *sorted_codepoints,
		         بنانشانی صحيح          عدد_codepoints);

HB_EXTERN فراغ
hb_سیٹ_del (hb_سیٹ_t       *سیٹ,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_سیٹ_del_range (hb_سیٹ_t       *سیٹ,
		  hb_codepoint_t  پہلا,
		  hb_codepoint_t  آخری);

HB_EXTERN hb_bool_t
hb_سیٹ_is_equal (ثابت hb_سیٹ_t *سیٹ,
		 ثابت hb_سیٹ_t *other);

HB_EXTERN hb_bool_t
hb_سیٹ_intersects (ثابت hb_سیٹ_t *سیٹ,
		   ثابت hb_سیٹ_t *other);

HB_EXTERN بنانشانی صحيح
hb_سیٹ_hash (ثابت hb_سیٹ_t *سیٹ);

HB_EXTERN hb_bool_t
hb_سیٹ_is_subset (ثابت hb_سیٹ_t *سیٹ,
		  ثابت hb_سیٹ_t *larger_سیٹ);

HB_EXTERN فراغ
hb_سیٹ_سیٹ (hb_سیٹ_t       *سیٹ,
	    ثابت hb_سیٹ_t *other);

HB_EXTERN فراغ
hb_سیٹ_union (hb_سیٹ_t       *سیٹ,
	      ثابت hb_سیٹ_t *other);

HB_EXTERN فراغ
hb_سیٹ_intersect (hb_سیٹ_t       *سیٹ,
		  ثابت hb_سیٹ_t *other);

HB_EXTERN فراغ
hb_سیٹ_گھٹاؤ (hb_سیٹ_t       *سیٹ,
		 ثابت hb_سیٹ_t *other);

HB_EXTERN فراغ
hb_سیٹ_متشاکل_difference (hb_سیٹ_t       *سیٹ,
			     ثابت hb_سیٹ_t *other);

HB_EXTERN بنانشانی صحيح
hb_سیٹ_get_population (ثابت hb_سیٹ_t *سیٹ);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_سیٹ_get_min (ثابت hb_سیٹ_t *سیٹ);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_سیٹ_get_max (ثابت hb_سیٹ_t *سیٹ);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_سیٹ_اگلا (ثابت hb_سیٹ_t *سیٹ,
	     hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_سیٹ_پچھلا (ثابت hb_سیٹ_t *سیٹ,
		 hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_سیٹ_اگلا_range (ثابت hb_سیٹ_t *سیٹ,
		   hb_codepoint_t *پہلا,
		   hb_codepoint_t *آخری);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_سیٹ_پچھلا_range (ثابت hb_سیٹ_t *سیٹ,
		       hb_codepoint_t *پہلا,
		       hb_codepoint_t *آخری);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN بنانشانی صحيح
hb_سیٹ_اگلا_many (ثابت hb_سیٹ_t *سیٹ,
		  hb_codepoint_t  codepoint,
		  hb_codepoint_t *باہرلا,
		  بنانشانی صحيح    ناپ);

HB_سرا_DECLS

#جے_انت /* HB_SET_H */
