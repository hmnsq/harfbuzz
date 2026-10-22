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

#ਜੇ !defined(HB_OT_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
#ਗਲਤੀ "Include <hb-ot.h> instead."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_VAR_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VAR_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"

HB_BEGIN_DECLS

/**
 * HB_OT_TAG_VAR_AXIS_ITALIC:
 *
 * Registered tag for the roman/italic axis.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_VAR_AXIS_ITALIC	HB_ਟੈਗ('i','t','a','l')

/**
 * HB_OT_TAG_VAR_AXIS_OPTICAL_SIZE:
 *
 * Registered tag for the optical-size axis.
 * <note>Note: The optical-size axis supersedes the OpenType `size` feature.</note>
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_VAR_AXIS_OPTICAL_ਮਾਪ	HB_ਟੈਗ('o','p','s','z')

/**
 * HB_OT_TAG_VAR_AXIS_SLANT:
 *
 * Registered tag for the slant axis
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_VAR_AXIS_SLANT	HB_ਟੈਗ('s','l','n','t')

/**
 * HB_OT_TAG_VAR_AXIS_WIDTH:
 *
 * Registered tag for the width axis.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_VAR_AXIS_ਚੌੜਾਈ	HB_ਟੈਗ('w','d','t','h')

/**
 * HB_OT_TAG_VAR_AXIS_WEIGHT:
 *
 * Registered tag for the weight axis.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_VAR_AXIS_ਭਾਰ	HB_ਟੈਗ('w','g','h','t')


/*
 * fvar / avar
 */

HB_EXTERN hb_bool_t
hb_ot_var_has_ਅੰਕੜੇ (hb_face_t *face);


/*
 * Variation axes.
 */


HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_var_get_axis_ਗਿਣਤੀ (hb_face_t *face);

/**
 * hb_ot_var_axis_flags_t:
 * @HB_OT_VAR_AXIS_FLAG_HIDDEN: The axis should not be exposed directly in user interfaces.
 *
 * Flags for #hb_ot_var_axis_info_t.
 *
 * Since: 2.2.0
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ { /*< flags >*/
  HB_OT_VAR_AXIS_FLAG_HIDDEN	= 0x00000001u,

  /*< private >*/
  HB_OT_VAR_AXIS_FLAG_MAX_ਮੁੱਲ= HB_ਟੈਗ_MAX_SIGNED /*< skip >*/
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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_ot_var_axis_info_t {
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ			axis_ਸੂਚਕਾਂਕ;
  hb_ਟੈਗ_t			ਟੈਗ_2;
  hb_ot_name_id_t		name_id;
  hb_ot_var_axis_flags_t	flags;
  ਦਸ਼ਮਲਵ				min_ਮੁੱਲ;
  ਦਸ਼ਮਲਵ				default_ਮੁੱਲ;
  ਦਸ਼ਮਲਵ				max_ਮੁੱਲ;
  /*< private >*/
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ			reserved;
} hb_ot_var_axis_info_t;

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_var_get_axis_infos (hb_face_t             *face,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ           ਸ਼ੁਰੂ_ਵਿੱਥ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          *axes_ਗਿਣਤੀ /* IN/OUT */,
			  hb_ot_var_axis_info_t *axes_array /*ਬਾਹਰਲਾ*/);

HB_EXTERN hb_bool_t
hb_ot_var_ਲੱਭੋ_axis_info (hb_face_t             *face,
			  hb_ਟੈਗ_t               axis_ਟੈਗ_2,
			  hb_ot_var_axis_info_t *axis_info);


/*
 * Named instances.
 */

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_var_get_named_instance_ਗਿਣਤੀ (hb_face_t *face);

HB_EXTERN hb_ot_name_id_t
hb_ot_var_named_instance_get_subfamily_name_id (hb_face_t   *face,
						ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instance_ਸੂਚਕਾਂਕ);

HB_EXTERN hb_ot_name_id_t
hb_ot_var_named_instance_get_postscript_name_id (hb_face_t  *face,
						ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instance_ਸੂਚਕਾਂਕ);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_var_named_instance_get_design_coords (hb_face_t    *face,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  instance_ਸੂਚਕਾਂਕ,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *coords_ਲੰਮਾਈ, /* IN/OUT */
					    ਦਸ਼ਮਲਵ        *coords         /*ਬਾਹਰਲਾ*/);


/*
 * Conversions.
 */

HB_EXTERN ਖਾਲੀ
hb_ot_var_normalize_variations (hb_face_t            *face,
				ਅਚਲ hb_variation_t *variations, /*ਵਿੱਚ*/
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          variations_ਲੰਮਾਈ,
				ਪੂਰਨ                  *coords, /*ਬਾਹਰਲਾ*/
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          coords_ਲੰਮਾਈ);

HB_EXTERN ਖਾਲੀ
hb_ot_var_normalize_coords (hb_face_t    *face,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coords_ਲੰਮਾਈ,
			    ਅਚਲ ਦਸ਼ਮਲਵ *design_coords, /*ਵਿੱਚ*/
			    ਪੂਰਨ *normalized_coords /*ਬਾਹਰਲਾ*/);


HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_OT_VAR_H */
