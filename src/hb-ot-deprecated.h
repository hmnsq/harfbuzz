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

#إذا !defined(HB_OT_H_اندريون) && !defined(HB_NO_SINGLE_سرو_ERROR)
#غلطي "Include <hb-ot.h> instead."
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_OT_DEPRECATED_H
#وصف_ڪريو HB_OT_DEPRECATED_H

#ضمن "hb.h"
#ضمن "hb-ot-name.h"


HB_BEGIN_DECLS

#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED


/* https://github.com/harfbuzz/harfbuzz/issues/1734 */
/**
 * HB_MATH_GLYPH_PART_FLAG_EXTENDER:
 *
 * Use #HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER instead.
 *
 * Deprecated: 2.5.1
 */
#وصف_ڪريو HB_MATH_GLYPH_PART_FLAG_EXTENDER HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER

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
#وصف_ڪريو HB_OT_MATH_SCRIPT HB_OT_ٽيگ_MATH_SCRIPT


/* Like hb_ot_layout_table_find_script, but takes zero-terminated array of scripts to test */
HB_DEPRECATED_FOR (hb_ot_layout_جدول_select_script)
HB_EXTERN hb_bool_t
hb_ot_layout_جدول_چونڊيو_script (hb_face_t      *face,
				  hb_ٽيگ_t        جدول_ٽيگ_2,
				  ثابت hb_ٽيگ_t *script_ٽيگ_فهرست,
				  بنانشان صحيح   *script_اشاريو,
				  hb_ٽيگ_t       *chosen_script);

HB_DEPRECATED_FOR (hb_ot_layout_script_select_language)
HB_EXTERN hb_bool_t
hb_ot_layout_script_ڳوليو_language (hb_face_t    *face,
				   hb_ٽيگ_t      جدول_ٽيگ_2,
				   بنانشان صحيح  script_اشاريو,
				   hb_ٽيگ_t      language_ٽيگ,
				   بنانشان صحيح *language_اشاريو);

HB_DEPRECATED_FOR (hb_ot_ٽيگ_فهرست_from_script_and_language)
HB_EXTERN فراغ
hb_ot_ٽيگ_فهرست_from_script (hb_script_t  script,
			hb_ٽيگ_t    *script_ٽيگ_1,
			hb_ٽيگ_t    *script_ٽيگ_2);

HB_DEPRECATED_FOR (hb_ot_ٽيگ_فهرست_from_script_and_language)
HB_EXTERN hb_ٽيگ_t
hb_ot_ٽيگ_from_language (hb_language_t language);


/**
 * HB_OT_VAR_NO_AXIS_INDEX:
 *
 * Do not use.
 *
 * Since: 1.4.2
 * Deprecated: 2.2.0
 */
#وصف_ڪريو HB_OT_VAR_NO_AXIS_اشاريو		0xFFFFFFFFu

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
  hb_ٽيگ_t ٽيگ_2;
  hb_ot_name_id_t name_id;
  عائم min_قدر;
  عائم default_قدر;
  عائم max_قدر;
} hb_ot_var_axis_t;

HB_DEPRECATED_FOR (hb_ot_var_get_axis_infos)
HB_EXTERN بنانشان صحيح
hb_ot_var_get_axes (hb_face_t        *face,
		    بنانشان صحيح      شروعات_وٿي,
		    بنانشان صحيح     *axes_ڳڻپ /* IN/OUT */,
		    hb_ot_var_axis_t *axes_array /*ٻاهريون*/);

HB_DEPRECATED_FOR (hb_ot_var_ڳوليو_axis_info)
HB_EXTERN hb_bool_t
hb_ot_var_ڳوليو_axis (hb_face_t        *face,
		     hb_ٽيگ_t          axis_ٽيگ_2,
		     بنانشان صحيح     *axis_اشاريو,
		     hb_ot_var_axis_t *axis_info);


#جيڪڏهن_پورو

HB_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_OT_DEPRECATED_H */
