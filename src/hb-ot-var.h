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

#ఒకవేళ !defined(HB_OT_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb-ot.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_OT_VAR_H
#నిర్వచించు HB_OT_VAR_H

#అన్తర్భావ "hb.h"

HB_BEGIN_DECLS

/**
 * HB_OT_TAG_VAR_AXIS_ITALIC:
 *
 * Registered tag for the roman/italic axis.
 */
#నిర్వచించు HB_OT_ట్యాగ్_VAR_AXIS_ITALIC	HB_ట్యాగ్('i','t','a','l')

/**
 * HB_OT_TAG_VAR_AXIS_OPTICAL_SIZE:
 *
 * Registered tag for the optical-size axis.
 * <note>Note: The optical-size axis supersedes the OpenType `size` feature.</note>
 */
#నిర్వచించు HB_OT_ట్యాగ్_VAR_AXIS_OPTICAL_కొలత	HB_ట్యాగ్('o','p','s','z')

/**
 * HB_OT_TAG_VAR_AXIS_SLANT:
 *
 * Registered tag for the slant axis
 */
#నిర్వచించు HB_OT_ట్యాగ్_VAR_AXIS_SLANT	HB_ట్యాగ్('s','l','n','t')

/**
 * HB_OT_TAG_VAR_AXIS_WIDTH:
 *
 * Registered tag for the width axis.
 */
#నిర్వచించు HB_OT_ట్యాగ్_VAR_AXIS_వెడల్పు	HB_ట్యాగ్('w','d','t','h')

/**
 * HB_OT_TAG_VAR_AXIS_WEIGHT:
 *
 * Registered tag for the weight axis.
 */
#నిర్వచించు HB_OT_ట్యాగ్_VAR_AXIS_బరువు	HB_ట్యాగ్('w','g','h','t')


/*
 * fvar / avar
 */

HB_EXTERN hb_bool_t
hb_ot_var_has_దత్తాంశం (hb_face_t *face);


/*
 * Variation axes.
 */


HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_var_get_axis_లెక్క (hb_face_t *face);

/**
 * hb_ot_var_axis_flags_t:
 * @HB_OT_VAR_AXIS_FLAG_HIDDEN: The axis should not be exposed directly in user interfaces.
 *
 * Flags for #hb_ot_var_axis_info_t.
 *
 * Since: 2.2.0
 */
రకనిర్వచనం గణన { /*< flags >*/
  HB_OT_VAR_AXIS_FLAG_HIDDEN	= 0x00000001u,

  /*< private >*/
  HB_OT_VAR_AXIS_FLAG_MAX_విలువ= HB_ట్యాగ్_MAX_SIGNED /*< skip >*/
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
రకనిర్వచనం నిర్మాణం hb_ot_var_axis_info_t {
  గుర్తులేని పూర్ణాంకం			axis_సూచీ;
  hb_ట్యాగ్_t			ట్యాగ్_2;
  hb_ot_name_id_t		name_id;
  hb_ot_var_axis_flags_t	flags;
  దశాంశం				min_విలువ;
  దశాంశం				default_విలువ;
  దశాంశం				max_విలువ;
  /*< private >*/
  గుర్తులేని పూర్ణాంకం			reserved;
} hb_ot_var_axis_info_t;

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_var_get_axis_infos (hb_face_t             *face,
			  గుర్తులేని పూర్ణాంకం           ప్రారంభం_స్థానభ్రంశం,
			  గుర్తులేని పూర్ణాంకం          *axes_లెక్క /* IN/OUT */,
			  hb_ot_var_axis_info_t *axes_array /*బహిర్గతం*/);

HB_EXTERN hb_bool_t
hb_ot_var_కనుగొను_axis_info (hb_face_t             *face,
			  hb_ట్యాగ్_t               axis_ట్యాగ్_2,
			  hb_ot_var_axis_info_t *axis_info);


/*
 * Named instances.
 */

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_var_get_named_instance_లెక్క (hb_face_t *face);

HB_EXTERN hb_ot_name_id_t
hb_ot_var_named_instance_get_subfamily_name_id (hb_face_t   *face,
						గుర్తులేని పూర్ణాంకం instance_సూచీ);

HB_EXTERN hb_ot_name_id_t
hb_ot_var_named_instance_get_postscript_name_id (hb_face_t  *face,
						గుర్తులేని పూర్ణాంకం instance_సూచీ);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_var_named_instance_get_design_coords (hb_face_t    *face,
					    గుర్తులేని పూర్ణాంకం  instance_సూచీ,
					    గుర్తులేని పూర్ణాంకం *coords_నిడివి, /* IN/OUT */
					    దశాంశం        *coords         /*బహిర్గతం*/);


/*
 * Conversions.
 */

HB_EXTERN శూన్య
hb_ot_var_normalize_variations (hb_face_t            *face,
				స్థిరరాశి hb_variation_t *variations, /*లోపల*/
				గుర్తులేని పూర్ణాంకం          variations_నిడివి,
				పూర్ణాంకం                  *coords, /*బహిర్గతం*/
				గుర్తులేని పూర్ణాంకం          coords_నిడివి);

HB_EXTERN శూన్య
hb_ot_var_normalize_coords (hb_face_t    *face,
			    గుర్తులేని పూర్ణాంకం coords_నిడివి,
			    స్థిరరాశి దశాంశం *design_coords, /*లోపల*/
			    పూర్ణాంకం *normalized_coords /*బహిర్గతం*/);


HB_చివర_DECLS

#అయితే_ముగింపు /* HB_OT_VAR_H */
