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

#ਜੇ !defined(HB_OT_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
#ਗਲਤੀ "Include <hb-ot.h> instead."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_DEPRECATED_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_DEPRECATED_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-name.h"


HB_BEGIN_DECLS

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED


/* https://github.com/harfbuzz/harfbuzz/issues/1734 */
/**
 * HB_MATH_GLYPH_PART_FLAG_EXTENDER:
 *
 * Use #HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER instead.
 *
 * Deprecated: 2.5.1
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MATH_GLYPH_PART_FLAG_EXTENDER HB_OT_MATH_GLYPH_PART_FLAG_EXTENDER

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_MATH_SCRIPT HB_OT_ਟੈਗ_MATH_SCRIPT


/* Like hb_ot_layout_table_find_script, but takes zero-terminated array of scripts to test */
HB_DEPRECATED_FOR (hb_ot_layout_ਸਾਰਣੀ_select_script)
HB_EXTERN hb_bool_t
hb_ot_layout_ਸਾਰਣੀ_ਚੁਣੋ_script (hb_face_t      *face,
				  hb_ਟੈਗ_t        ਸਾਰਣੀ_ਟੈਗ_2,
				  ਅਚਲ hb_ਟੈਗ_t *script_ਟੈਗ_ਸੂਚੀ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *script_ਸੂਚਕਾਂਕ,
				  hb_ਟੈਗ_t       *chosen_script);

HB_DEPRECATED_FOR (hb_ot_layout_script_select_language)
HB_EXTERN hb_bool_t
hb_ot_layout_script_ਲੱਭੋ_language (hb_face_t    *face,
				   hb_ਟੈਗ_t      ਸਾਰਣੀ_ਟੈਗ_2,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  script_ਸੂਚਕਾਂਕ,
				   hb_ਟੈਗ_t      language_ਟੈਗ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *language_ਸੂਚਕਾਂਕ);

HB_DEPRECATED_FOR (hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language)
HB_EXTERN ਖਾਲੀ
hb_ot_ਟੈਗ_ਸੂਚੀ_from_script (hb_script_t  script,
			hb_ਟੈਗ_t    *script_ਟੈਗ_1,
			hb_ਟੈਗ_t    *script_ਟੈਗ_2);

HB_DEPRECATED_FOR (hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language)
HB_EXTERN hb_ਟੈਗ_t
hb_ot_ਟੈਗ_from_language (hb_language_t language);


/**
 * HB_OT_VAR_NO_AXIS_INDEX:
 *
 * Do not use.
 *
 * Since: 1.4.2
 * Deprecated: 2.2.0
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VAR_NO_AXIS_ਸੂਚਕਾਂਕ		0xFFFFFFFFu

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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_ot_var_axis_t {
  hb_ਟੈਗ_t ਟੈਗ_2;
  hb_ot_name_id_t name_id;
  ਦਸ਼ਮਲਵ min_ਮੁੱਲ;
  ਦਸ਼ਮਲਵ default_ਮੁੱਲ;
  ਦਸ਼ਮਲਵ max_ਮੁੱਲ;
} hb_ot_var_axis_t;

HB_DEPRECATED_FOR (hb_ot_var_get_axis_infos)
HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_var_get_axes (hb_face_t        *face,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ      ਸ਼ੁਰੂ_ਵਿੱਥ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     *axes_ਗਿਣਤੀ /* IN/OUT */,
		    hb_ot_var_axis_t *axes_array /*ਬਾਹਰਲਾ*/);

HB_DEPRECATED_FOR (hb_ot_var_ਲੱਭੋ_axis_info)
HB_EXTERN hb_bool_t
hb_ot_var_ਲੱਭੋ_axis (hb_face_t        *face,
		     hb_ਟੈਗ_t          axis_ਟੈਗ_2,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     *axis_ਸੂਚਕਾਂਕ,
		     hb_ot_var_axis_t *axis_info);


#ਜੇ_ਅੰਤ

HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_OT_DEPRECATED_H */
