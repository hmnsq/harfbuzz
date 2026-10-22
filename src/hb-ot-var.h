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

#إذا !defined(HB_OT_H_دننه) && !defined(HB_NO_SINGLE_سرلیک_ERROR)
#تېروتنه "Include <hb-ot.h> instead."
#د_که_پای

#که_نه_تعریف_شوی HB_OT_VAR_H
#تعریف_کړه HB_OT_VAR_H

#ضمن "hb.h"

HB_BEGIN_DECLS

/**
 * HB_OT_TAG_VAR_AXIS_ITALIC:
 *
 * Registered tag for the roman/italic axis.
 */
#تعریف_کړه HB_OT_نښان_VAR_AXIS_ITALIC	HB_نښان('i','t','a','l')

/**
 * HB_OT_TAG_VAR_AXIS_OPTICAL_SIZE:
 *
 * Registered tag for the optical-size axis.
 * <note>Note: The optical-size axis supersedes the OpenType `size` feature.</note>
 */
#تعریف_کړه HB_OT_نښان_VAR_AXIS_OPTICAL_اندازه	HB_نښان('o','p','s','z')

/**
 * HB_OT_TAG_VAR_AXIS_SLANT:
 *
 * Registered tag for the slant axis
 */
#تعریف_کړه HB_OT_نښان_VAR_AXIS_SLANT	HB_نښان('s','l','n','t')

/**
 * HB_OT_TAG_VAR_AXIS_WIDTH:
 *
 * Registered tag for the width axis.
 */
#تعریف_کړه HB_OT_نښان_VAR_AXIS_پلنوالی	HB_نښان('w','d','t','h')

/**
 * HB_OT_TAG_VAR_AXIS_WEIGHT:
 *
 * Registered tag for the weight axis.
 */
#تعریف_کړه HB_OT_نښان_VAR_AXIS_وزن	HB_نښان('w','g','h','t')


/*
 * fvar / avar
 */

HB_EXTERN hb_bool_t
hb_ot_var_has_معلومات (hb_face_t *face);


/*
 * Variation axes.
 */


HB_EXTERN بینښه صحيح
hb_ot_var_get_axis_شمېر (hb_face_t *face);

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
  HB_OT_VAR_AXIS_FLAG_MAX_ارزښت= HB_نښان_MAX_SIGNED /*< skip >*/
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
  بینښه صحيح			axis_شاخص;
  hb_نښان_t			نښان_2;
  hb_ot_name_id_t		name_id;
  hb_ot_var_axis_flags_t	flags;
  عائم				min_ارزښت;
  عائم				default_ارزښت;
  عائم				max_ارزښت;
  /*< private >*/
  بینښه صحيح			reserved;
} hb_ot_var_axis_info_t;

HB_EXTERN بینښه صحيح
hb_ot_var_get_axis_infos (hb_face_t             *face,
			  بینښه صحيح           پیل_دننه_واټن,
			  بینښه صحيح          *axes_شمېر /* IN/OUT */,
			  hb_ot_var_axis_info_t *axes_array /*بهر*/);

HB_EXTERN hb_bool_t
hb_ot_var_پیدا_کړه_axis_info (hb_face_t             *face,
			  hb_نښان_t               axis_نښان_2,
			  hb_ot_var_axis_info_t *axis_info);


/*
 * Named instances.
 */

HB_EXTERN بینښه صحيح
hb_ot_var_get_named_instance_شمېر (hb_face_t *face);

HB_EXTERN hb_ot_name_id_t
hb_ot_var_named_instance_get_subfamily_name_id (hb_face_t   *face,
						بینښه صحيح instance_شاخص);

HB_EXTERN hb_ot_name_id_t
hb_ot_var_named_instance_get_postscript_name_id (hb_face_t  *face,
						بینښه صحيح instance_شاخص);

HB_EXTERN بینښه صحيح
hb_ot_var_named_instance_get_design_coords (hb_face_t    *face,
					    بینښه صحيح  instance_شاخص,
					    بینښه صحيح *coords_اوږدتیا, /* IN/OUT */
					    عائم        *coords         /*بهر*/);


/*
 * Conversions.
 */

HB_EXTERN فراغ
hb_ot_var_normalize_variations (hb_face_t            *face,
				ثابت hb_variation_t *variations, /*دننه*/
				بینښه صحيح          variations_اوږدتیا,
				صحيح                  *coords, /*بهر*/
				بینښه صحيح          coords_اوږدتیا);

HB_EXTERN فراغ
hb_ot_var_normalize_coords (hb_face_t    *face,
			    بینښه صحيح coords_اوږدتیا,
			    ثابت عائم *design_coords, /*دننه*/
			    صحيح *normalized_coords /*بهر*/);


HB_پای_ټکی_DECLS

#د_که_پای /* HB_OT_VAR_H */
