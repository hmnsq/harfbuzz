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

#إذا !defined(HB_OT_H_دننه) && !defined(HB_NO_SINGLE_سرلیک_ERROR)
#تېروتنه "Include <hb-ot.h> instead."
#د_که_پای

#که_نه_تعریف_شوی HB_OT_DEPRECATED_H
#تعریف_کړه HB_OT_DEPRECATED_H

#ضمن "hb.h"
#ضمن "hb-ot-name.h"


HB_BEGIN_DECLS

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED


/* https://github.com/harfbuzz/harfbuzz/issues/1734 */
/**
 * HB_MATH_GLYPH_PART_FLAG_EXTENDER:
 *
 * Use #HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER instead.
 *
 * Deprecated: 2.5.1
 */
#تعریف_کړه HB_MATH_GLYPH_PART_FLAG_EXTENDER HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER

/* https://github.com/harfbuzz/harfbuzz/pull/3417 */
/**
 * HB_OT_MATH_SCRIPT:
 *
 * Use #HB_SCRIPT_MATH or #HB_OT_TAG_MATH_SCRIPT instead.
 *
 * <note>Previous versions of this documentation recommended passing
 * #HB_OT_MATH_SCRIPT to hb_buffer_set_script() to enable math shaping, but this
 * usage is no longer supported. Use #HB_SCRIPT_MATH instead.</note>
 *
 * Since: 1.3.3
 * Deprecated: 3.4.0
 */
#تعریف_کړه HB_OT_MATH_SCRIPT HB_OT_نښان_MATH_SCRIPT


/* Like hb_ot_layout_table_find_script, but takes zero-terminated array of scripts to test */
HB_DEPRECATED_FOR (hb_ot_layout_جدول_select_script)
HB_EXTERN hb_bool_t
hb_ot_layout_جدول_ټاکنه_script (hb_face_t      *face,
				  hb_نښان_t        جدول_نښان_2,
				  ثابت hb_نښان_t *script_نښانونه,
				  بینښه صحيح   *script_شاخص,
				  hb_نښان_t       *chosen_script);

HB_DEPRECATED_FOR (hb_ot_layout_script_select_language)
HB_EXTERN hb_bool_t
hb_ot_layout_script_پیدا_کړه_language (hb_face_t    *face,
				   hb_نښان_t      جدول_نښان_2,
				   بینښه صحيح  script_شاخص,
				   hb_نښان_t      language_نښان,
				   بینښه صحيح *language_شاخص);

HB_DEPRECATED_FOR (hb_ot_نښانونه_from_script_and_language)
HB_EXTERN فراغ
hb_ot_نښانونه_from_script (hb_script_t  script,
			hb_نښان_t    *script_نښان_1,
			hb_نښان_t    *script_نښان_2);

HB_DEPRECATED_FOR (hb_ot_نښانونه_from_script_and_language)
HB_EXTERN hb_نښان_t
hb_ot_نښان_from_language (hb_language_t language);


/**
 * HB_OT_VAR_NO_AXIS_INDEX:
 *
 * Do not use.
 *
 * Since: 1.4.2
 * Deprecated: 2.2.0
 */
#تعریف_کړه HB_OT_VAR_NO_AXIS_شاخص		0xFFFFFFFFu

/**
 * hb_ot_var_axis_t:
 * @tag: axis tag
 * @name_id: axis name identifier
 * @min_value: minimum value of the axis
 * @default_value: default value of the axis
 * @max_value: maximum value of the axis
 *
 * Use #hb_ot_var_axis_info_t instead.
 *
 * Since: 1.4.2
 * Deprecated: 2.2.0
 */
تعريفنوع بنية hb_ot_var_axis_t {
  hb_نښان_t نښان_2;
  hb_ot_name_id_t name_id;
  عائم min_ارزښت;
  عائم default_ارزښت;
  عائم max_ارزښت;
} hb_ot_var_axis_t;

HB_DEPRECATED_FOR (hb_ot_var_get_axis_infos)
HB_EXTERN بینښه صحيح
hb_ot_var_get_axes (hb_face_t        *face,
		    بینښه صحيح      پیل_دننه_واټن,
		    بینښه صحيح     *axes_شمېر /* IN/OUT */,
		    hb_ot_var_axis_t *axes_array /*بهر*/);

HB_DEPRECATED_FOR (hb_ot_var_پیدا_کړه_axis_info)
HB_EXTERN hb_bool_t
hb_ot_var_پیدا_کړه_axis (hb_face_t        *face,
		     hb_نښان_t          axis_نښان_2,
		     بینښه صحيح     *axis_شاخص,
		     hb_ot_var_axis_t *axis_info);


#د_که_پای

HB_پای_ټکی_DECLS

#د_که_پای /* HB_OT_DEPRECATED_H */
