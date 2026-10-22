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

#اگر !defined(HB_OT_H_اندر) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb-ot.h> instead."
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_OT_DEPRECATED_H
#تعریف_کرو HB_OT_DEPRECATED_H

#شامل "hb.h"
#شامل "hb-ot-name.h"


HB_BEGIN_DECLS

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED


/* https://github.com/harfbuzz/harfbuzz/issues/1734 */
/**
 * HB_MATH_GLYPH_PART_FLAG_EXTENDER:
 *
 * Use #HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER instead.
 *
 * Deprecated: 2.5.1
 */
#تعریف_کرو HB_MATH_GLYPH_PART_FLAG_EXTENDER HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER

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
#تعریف_کرو HB_OT_MATH_SCRIPT HB_OT_لیبل_MATH_SCRIPT


/* Like hb_ot_layout_table_find_script, but takes zero-terminated array of scripts to test */
HB_DEPRECATED_FOR (hb_ot_layout_جدول_select_script)
HB_EXTERN hb_bool_t
hb_ot_layout_جدول_انتخاب_script (hb_face_t      *face,
				  hb_لیبل_t        جدول_لیبل_2,
				  ثابت hb_لیبل_t *script_لیبل_فہرست,
				  غیرعلامتی صحیح   *script_اشاریہ,
				  hb_لیبل_t       *chosen_script);

HB_DEPRECATED_FOR (hb_ot_layout_script_select_language)
HB_EXTERN hb_bool_t
hb_ot_layout_script_ڈھونڈو_language (hb_face_t    *face,
				   hb_لیبل_t      جدول_لیبل_2,
				   غیرعلامتی صحیح  script_اشاریہ,
				   hb_لیبل_t      language_لیبل,
				   غیرعلامتی صحیح *language_اشاریہ);

HB_DEPRECATED_FOR (hb_ot_لیبل_فہرست_from_script_and_language)
HB_EXTERN خالی
hb_ot_لیبل_فہرست_from_script (hb_script_t  script,
			hb_لیبل_t    *script_لیبل_1,
			hb_لیبل_t    *script_لیبل_2);

HB_DEPRECATED_FOR (hb_ot_لیبل_فہرست_from_script_and_language)
HB_EXTERN hb_لیبل_t
hb_ot_لیبل_from_language (hb_language_t language);


/**
 * HB_OT_VAR_NO_AXIS_INDEX:
 *
 * Do not use.
 *
 * Since: 1.4.2
 * Deprecated: 2.2.0
 */
#تعریف_کرو HB_OT_VAR_NO_AXIS_اشاریہ		0xFFFFFFFFu

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
قسمتعریف ساخت hb_ot_var_axis_t {
  hb_لیبل_t لیبل_2;
  hb_ot_name_id_t name_id;
  اعشاری min_قدر;
  اعشاری default_قدر;
  اعشاری max_قدر;
} hb_ot_var_axis_t;

HB_DEPRECATED_FOR (hb_ot_var_get_axis_infos)
HB_EXTERN غیرعلامتی صحیح
hb_ot_var_get_axes (hb_face_t        *face,
		    غیرعلامتی صحیح      آغاز_انحراف,
		    غیرعلامتی صحیح     *axes_گنتی /* IN/OUT */,
		    hb_ot_var_axis_t *axes_array /*برآمد*/);

HB_DEPRECATED_FOR (hb_ot_var_ڈھونڈو_axis_info)
HB_EXTERN hb_bool_t
hb_ot_var_ڈھونڈو_axis (hb_face_t        *face,
		     hb_لیبل_t          axis_لیبل_2,
		     غیرعلامتی صحیح     *axis_اشاریہ,
		     hb_ot_var_axis_t *axis_info);


#اگر_ختم

HB_انتہا_DECLS

#اگر_ختم /* HB_OT_DEPRECATED_H */
