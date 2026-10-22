/*
 * Copyright © 2017  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 */

#إذا !defined(HB_OT_H_درون) && !defined(HB_NO_SINGLE_سرآیند_ERROR)
#خطا "Include <hb-ot.h> instead."
#پایان_اگر

#اگر_تعریف_نشده HB_OT_VAR_H
#تعریف_کن HB_OT_VAR_H

#ضمن "hb.h"

HB_BEGIN_DECLS

/**
 * HB_OT_TAG_VAR_AXIS_ITALIC:
 *
 * Registered tag for the roman/italic axis.
 */
#تعریف_کن HB_OT_برچسب_VAR_AXIS_ITALIC	HB_برچسب('i','t','a','l')

/**
 * HB_OT_TAG_VAR_AXIS_OPTICAL_SIZE:
 *
 * Registered tag for the optical-size axis.
 * <note>Note: The optical-size axis supersedes the OpenType `size` feature.</note>
 */
#تعریف_کن HB_OT_برچسب_VAR_AXIS_OPTICAL_اندازه	HB_برچسب('o','p','s','z')

/**
 * HB_OT_TAG_VAR_AXIS_SLANT:
 *
 * Registered tag for the slant axis
 */
#تعریف_کن HB_OT_برچسب_VAR_AXIS_SLANT	HB_برچسب('s','l','n','t')

/**
 * HB_OT_TAG_VAR_AXIS_WIDTH:
 *
 * Registered tag for the width axis.
 */
#تعریف_کن HB_OT_برچسب_VAR_AXIS_عرض	HB_برچسب('w','d','t','h')

/**
 * HB_OT_TAG_VAR_AXIS_WEIGHT:
 *
 * Registered tag for the weight axis.
 */
#تعریف_کن HB_OT_برچسب_VAR_AXIS_وزن	HB_برچسب('w','g','h','t')


/*
 * fvar / avar
 */

HB_EXTERN hb_bool_t
hb_ot_var_has_داده (hb_face_t *face);


/*
 * Variation axes.
 */


HB_EXTERN بیعلامت صحيح
hb_ot_var_get_axis_شمار (hb_face_t *face);

/**
 * hb_ot_var_axis_flags_t:
 * @HB_OT_VAR_AXIS_FLAG_HIDDEN: The axis should not be exposed directly in user interfaces.
 *
 * Flags for #hb_ot_var_axis_info_t.
 *
 * Since: 2.2.0
 */
تعريفنوع تعداد { /*< flags >*/
  HB_OT_VAR_AXIS_FLAG_HIDDEN	= 0x00000001u,

  /*< private >*/
  HB_OT_VAR_AXIS_FLAG_MAX_مقدار= HB_برچسب_MAX_SIGNED /*< skip >*/
} hb_ot_var_axis_flags_t;

/**
 * hb_ot_var_axis_info_t:
 * @axis_index: Index of the axis in the variation-axis array
 * @tag: The #hb_tag_t tag identifying the design variation of the axis
 * @name_id: The `name` table Name ID that provides display names for the axis
 * @flags: The #hb_ot_var_axis_flags_t flags for the axis
 * @min_value: The minimum value on the variation axis that the font covers
 * @default_value: The position on the variation axis corresponding to the font's defaults
 * @max_value: The maximum value on the variation axis that the font covers
 * 
 * Data type for holding variation-axis values.
 *
 * The minimum, default, and maximum values are in un-normalized, user scales.
 *
 * <note>Note: at present, the only flag defined for @flags is
 * #HB_OT_VAR_AXIS_FLAG_HIDDEN.</note>
 *
 * Since: 2.2.0
 */
تعريفنوع بنية hb_ot_var_axis_info_t {
  بیعلامت صحيح			axis_شاخص;
  hb_برچسب_t			برچسب_2;
  hb_ot_name_id_t		name_id;
  hb_ot_var_axis_flags_t	flags;
  عائم				min_مقدار;
  عائم				default_مقدار;
  عائم				max_مقدار;
  /*< private >*/
  بیعلامت صحيح			reserved;
} hb_ot_var_axis_info_t;

HB_EXTERN بیعلامت صحيح
hb_ot_var_get_axis_infos (hb_face_t             *face,
			  بیعلامت صحيح           شروع_جابه_جایی,
			  بیعلامت صحيح          *axes_شمار /* IN/OUT */,
			  hb_ot_var_axis_info_t *axes_array /*بیرون*/);

HB_EXTERN hb_bool_t
hb_ot_var_پیدا_کن_axis_info (hb_face_t             *face,
			  hb_برچسب_t               axis_برچسب_2,
			  hb_ot_var_axis_info_t *axis_info);


/*
 * Named instances.
 */

HB_EXTERN بیعلامت صحيح
hb_ot_var_get_named_instance_شمار (hb_face_t *face);

HB_EXTERN hb_ot_name_id_t
hb_ot_var_named_instance_get_subfamily_name_id (hb_face_t   *face,
						بیعلامت صحيح instance_شاخص);

HB_EXTERN hb_ot_name_id_t
hb_ot_var_named_instance_get_postscript_name_id (hb_face_t  *face,
						بیعلامت صحيح instance_شاخص);

HB_EXTERN بیعلامت صحيح
hb_ot_var_named_instance_get_design_coords (hb_face_t    *face,
					    بیعلامت صحيح  instance_شاخص,
					    بیعلامت صحيح *coords_درازی, /* IN/OUT */
					    عائم        *coords         /*بیرون*/);


/*
 * Conversions.
 */

HB_EXTERN فراغ
hb_ot_var_normalize_variations (hb_face_t            *face,
				ثابت hb_variation_t *variations, /*درون*/
				بیعلامت صحيح          variations_درازی,
				صحيح                  *coords, /*بیرون*/
				بیعلامت صحيح          coords_درازی);

HB_EXTERN فراغ
hb_ot_var_normalize_coords (hb_face_t    *face,
			    بیعلامت صحيح coords_درازی,
			    ثابت عائم *design_coords, /*درون*/
			    صحيح *normalized_coords /*بیرون*/);


HB_انتها_DECLS

#پایان_اگر /* HB_OT_VAR_H */
