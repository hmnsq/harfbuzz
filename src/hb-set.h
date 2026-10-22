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

#إذا !defined(HB_H_اندريون) && !defined(HB_NO_SINGLE_سرو_ERROR)
#غلطي "Include <hb.h> instead."
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_سيٽ_H
#وصف_ڪريو HB_سيٽ_H

#ضمن "hb-common.h"

HB_BEGIN_DECLS


/**
 * HB_SET_VALUE_INVALID:
 *
 * Unset #hb_set_t value.
 *
 * Since: 0.9.21
 */
#وصف_ڪريو HB_سيٽ_قدر_INVALID HB_CODEPOINT_INVALID

/**
 * hb_set_t:
 *
 * Data type for holding a set of integers. #hb_set_t's are
 * used to gather and contain glyph IDs, Unicode code
 * points, and various other collections of discrete 
 * values.
 *
 **/
تعريفنوع بنية hb_سيٽ_t hb_سيٽ_t;


HB_EXTERN hb_سيٽ_t *
hb_سيٽ_create (فراغ);

HB_EXTERN hb_سيٽ_t *
hb_سيٽ_get_خالي_جايون (فراغ);

HB_EXTERN hb_سيٽ_t *
hb_سيٽ_reference (hb_سيٽ_t *سيٽ);

HB_EXTERN فراغ
hb_سيٽ_destroy (hb_سيٽ_t *سيٽ);

HB_EXTERN hb_bool_t
hb_سيٽ_سيٽ_user_معلومات (hb_سيٽ_t           *سيٽ,
		      hb_user_معلومات_ڪنجي_t *ڪنجي_2,
		      فراغ *              معلومات,
		      hb_destroy_func_t   destroy,
		      hb_bool_t           replace);

HB_EXTERN فراغ *
hb_سيٽ_get_user_معلومات (ثابت hb_سيٽ_t     *سيٽ,
		      hb_user_معلومات_ڪنجي_t *ڪنجي_2);


/* Returns false if allocation has failed before */
HB_EXTERN hb_bool_t
hb_سيٽ_ورهاست_successful (ثابت hb_سيٽ_t *سيٽ);

HB_EXTERN hb_سيٽ_t *
hb_سيٽ_copy (ثابت hb_سيٽ_t *سيٽ);

HB_EXTERN فراغ
hb_سيٽ_clear (hb_سيٽ_t *سيٽ);

HB_EXTERN hb_bool_t
hb_سيٽ_is_خالي_جايون (ثابت hb_سيٽ_t *سيٽ);

HB_EXTERN فراغ
hb_سيٽ_invert (hb_سيٽ_t *سيٽ);

HB_EXTERN hb_bool_t
hb_سيٽ_is_inverted (ثابت hb_سيٽ_t *سيٽ);

HB_EXTERN hb_bool_t
hb_سيٽ_has (ثابت hb_سيٽ_t *سيٽ,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_سيٽ_جوڙيو (hb_سيٽ_t       *سيٽ,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_سيٽ_جوڙيو_range (hb_سيٽ_t       *سيٽ,
		  hb_codepoint_t  پهريون,
		  hb_codepoint_t  آخري);

HB_EXTERN فراغ
hb_سيٽ_جوڙيو_sorted_array (hb_سيٽ_t             *سيٽ,
		         ثابت hb_codepoint_t *sorted_codepoints,
		         بنانشان صحيح          انگ_codepoints);

HB_EXTERN فراغ
hb_سيٽ_del (hb_سيٽ_t       *سيٽ,
	    hb_codepoint_t  codepoint);

HB_EXTERN فراغ
hb_سيٽ_del_range (hb_سيٽ_t       *سيٽ,
		  hb_codepoint_t  پهريون,
		  hb_codepoint_t  آخري);

HB_EXTERN hb_bool_t
hb_سيٽ_is_equal (ثابت hb_سيٽ_t *سيٽ,
		 ثابت hb_سيٽ_t *other);

HB_EXTERN hb_bool_t
hb_سيٽ_intersects (ثابت hb_سيٽ_t *سيٽ,
		   ثابت hb_سيٽ_t *other);

HB_EXTERN بنانشان صحيح
hb_سيٽ_hash (ثابت hb_سيٽ_t *سيٽ);

HB_EXTERN hb_bool_t
hb_سيٽ_is_subset (ثابت hb_سيٽ_t *سيٽ,
		  ثابت hb_سيٽ_t *larger_سيٽ);

HB_EXTERN فراغ
hb_سيٽ_سيٽ (hb_سيٽ_t       *سيٽ,
	    ثابت hb_سيٽ_t *other);

HB_EXTERN فراغ
hb_سيٽ_union (hb_سيٽ_t       *سيٽ,
	      ثابت hb_سيٽ_t *other);

HB_EXTERN فراغ
hb_سيٽ_intersect (hb_سيٽ_t       *سيٽ,
		  ثابت hb_سيٽ_t *other);

HB_EXTERN فراغ
hb_سيٽ_ڪٽيو (hb_سيٽ_t       *سيٽ,
		 ثابت hb_سيٽ_t *other);

HB_EXTERN فراغ
hb_سيٽ_متناظر_difference (hb_سيٽ_t       *سيٽ,
			     ثابت hb_سيٽ_t *other);

HB_EXTERN بنانشان صحيح
hb_سيٽ_get_population (ثابت hb_سيٽ_t *سيٽ);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_سيٽ_get_min (ثابت hb_سيٽ_t *سيٽ);

/* Returns HB_SET_VALUE_INVALID if set empty. */
HB_EXTERN hb_codepoint_t
hb_سيٽ_get_max (ثابت hb_سيٽ_t *سيٽ);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_سيٽ_ايندڙ (ثابت hb_سيٽ_t *سيٽ,
	     hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN hb_bool_t
hb_سيٽ_اڳيون (ثابت hb_سيٽ_t *سيٽ,
		 hb_codepoint_t *codepoint);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_سيٽ_ايندڙ_range (ثابت hb_سيٽ_t *سيٽ,
		   hb_codepoint_t *پهريون,
		   hb_codepoint_t *آخري);

/* Pass HB_SET_VALUE_INVALID for first and last to get started. */
HB_EXTERN hb_bool_t
hb_سيٽ_اڳيون_range (ثابت hb_سيٽ_t *سيٽ,
		       hb_codepoint_t *پهريون,
		       hb_codepoint_t *آخري);

/* Pass HB_SET_VALUE_INVALID in to get started. */
HB_EXTERN بنانشان صحيح
hb_سيٽ_ايندڙ_many (ثابت hb_سيٽ_t *سيٽ,
		  hb_codepoint_t  codepoint,
		  hb_codepoint_t *ٻاهريون,
		  بنانشان صحيح    سائيز);

HB_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_SET_H */
