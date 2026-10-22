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
 * Google Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_VAR

#అన్తర్భావ "hb-ot-var.h"

#అన్తర్భావ "hb-ot-var-avar-table.hh"
#అన్తర్భావ "hb-ot-var-fvar-table.hh"
#అన్తర్భావ "hb-ot-var-mvar-table.hh"


/**
 * SECTION:hb-ot-var
 * @title: hb-ot-var
 * @short_description: OpenType Font Variations
 * @include: hb-ot.h
 *
 * Functions for fetching information about OpenType Variable Fonts.
 **/


/*
 * fvar/avar
 */


/**
 * hb_ot_var_has_data:
 * @face: The #hb_face_t to work on
 *
 * Tests whether a face includes any OpenType variation data in the `fvar` table.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 1.4.2
 **/
hb_bool_t
hb_ot_var_has_దత్తాంశం (hb_face_t *face)
{
  తిరిగి face->పట్టిక.fvar->has_దత్తాంశం ();
}

/**
 * hb_ot_var_get_axis_count:
 * @face: The #hb_face_t to work on
 *
 * Fetches the number of OpenType variation axes included in the face.
 *
 * Return value: the number of variation axes defined
 *
 * Since: 1.4.2
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_var_get_axis_లెక్క (hb_face_t *face)
{
  తిరిగి face->పట్టిక.fvar->get_axis_లెక్క ();
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
/**
 * hb_ot_var_get_axes:
 * @face: #hb_face_t to work upon
 * @start_offset: offset of the first lookup to retrieve
 * @axes_count: (inout) (optional): Input = the maximum number of variation axes to return;
 *                Output = the actual number of variation axes returned (may be zero)
 * @axes_array: (out caller-allocates) (array length=axes_count) (nullable): The array of variation axes found
 *
 * Fetches a list of all variation axes in the specified face. The list returned will begin
 * at the offset provided.
 *
 * Since: 1.4.2
 * Deprecated: 2.2.0: use hb_ot_var_get_axis_infos() instead
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_var_get_axes (hb_face_t        *face,
		    గుర్తులేని పూర్ణాంకం      ప్రారంభం_స్థానభ్రంశం,
		    గుర్తులేని పూర్ణాంకం     *axes_లెక్క /* IN/OUT */,
		    hb_ot_var_axis_t *axes_array /*బహిర్గతం*/)
{
  తిరిగి face->పట్టిక.fvar->get_axes_deprecated (ప్రారంభం_స్థానభ్రంశం, axes_లెక్క, axes_array);
}

/**
 * hb_ot_var_find_axis:
 * @face: #hb_face_t to work upon
 * @axis_tag: The #hb_tag_t of the variation axis to query
 * @axis_index: The index of the variation axis
 * @axis_info: (out): The #hb_ot_var_axis_info_t of the axis tag queried
 *
 * Fetches the variation-axis information corresponding to the specified axis tag
 * in the specified face.
 *
 * Since: 1.4.2
 * Deprecated: 2.2.0: use hb_ot_var_find_axis_info() instead
 **/
hb_bool_t
hb_ot_var_కనుగొను_axis (hb_face_t        *face,
		     hb_ట్యాగ్_t          axis_ట్యాగ్_2,
		     గుర్తులేని పూర్ణాంకం     *axis_సూచీ,
		     hb_ot_var_axis_t *axis_info)
{
  తిరిగి face->పట్టిక.fvar->కనుగొను_axis_deprecated (axis_ట్యాగ్_2, axis_సూచీ, axis_info);
}
#అయితే_ముగింపు

/**
 * hb_ot_var_get_axis_infos:
 * @face: #hb_face_t to work upon
 * @start_offset: offset of the first lookup to retrieve
 * @axes_count: (inout) (optional): Input = the maximum number of variation axes to return;
 *                Output = the actual number of variation axes returned (may be zero)
 * @axes_array: (out caller-allocates) (array length=axes_count) (nullable): The array of variation axes found
 *
 * Fetches a list of all variation axes in the specified face. The list returned will begin
 * at the offset provided.
 *
 * Return value: the number of variation axes in the face
 *
 * Since: 2.2.0
 **/
HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_var_get_axis_infos (hb_face_t             *face,
			  గుర్తులేని పూర్ణాంకం           ప్రారంభం_స్థానభ్రంశం,
			  గుర్తులేని పూర్ణాంకం          *axes_లెక్క /* IN/OUT */,
			  hb_ot_var_axis_info_t *axes_array /*బహిర్గతం*/)
{
  తిరిగి face->పట్టిక.fvar->get_axis_infos (ప్రారంభం_స్థానభ్రంశం, axes_లెక్క, axes_array);
}

/**
 * hb_ot_var_find_axis_info:
 * @face: #hb_face_t to work upon
 * @axis_tag: The #hb_tag_t of the variation axis to query
 * @axis_info: (out): The #hb_ot_var_axis_info_t of the axis tag queried
 *
 * Fetches the variation-axis information corresponding to the specified axis tag
 * in the specified face.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 2.2.0
 **/
HB_EXTERN hb_bool_t
hb_ot_var_కనుగొను_axis_info (hb_face_t             *face,
			  hb_ట్యాగ్_t               axis_ట్యాగ్_2,
			  hb_ot_var_axis_info_t *axis_info)
{
  తిరిగి face->పట్టిక.fvar->కనుగొను_axis_info (axis_ట్యాగ్_2, axis_info);
}


/*
 * Named instances.
 */

/**
 * hb_ot_var_get_named_instance_count:
 * @face: The #hb_face_t to work on
 *
 * Fetches the number of named instances included in the face.
 *
 * Return value: the number of named instances defined
 *
 * Since: 2.2.0
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_var_get_named_instance_లెక్క (hb_face_t *face)
{
  తిరిగి face->పట్టిక.fvar->get_instance_లెక్క ();
}

/**
 * hb_ot_var_named_instance_get_subfamily_name_id:
 * @face: The #hb_face_t to work on
 * @instance_index: The index of the named instance to query
 *
 * Fetches the `name` table Name ID that provides display names for
 * the "Subfamily name" defined for the given named instance in the face.
 *
 * Return value: the Name ID found for the Subfamily name
 *
 * Since: 2.2.0
 **/
hb_ot_name_id_t
hb_ot_var_named_instance_get_subfamily_name_id (hb_face_t   *face,
						గుర్తులేని పూర్ణాంకం instance_సూచీ)
{
  తిరిగి face->పట్టిక.fvar->get_instance_subfamily_name_id (instance_సూచీ);
}

/**
 * hb_ot_var_named_instance_get_postscript_name_id:
 * @face: The #hb_face_t to work on
 * @instance_index: The index of the named instance to query
 *
 * Fetches the `name` table Name ID that provides display names for
 * the "PostScript name" defined for the given named instance in the face.
 *
 * Return value: the Name ID found for the PostScript name
 *
 * Since: 2.2.0
 **/
hb_ot_name_id_t
hb_ot_var_named_instance_get_postscript_name_id (hb_face_t  *face,
						గుర్తులేని పూర్ణాంకం instance_సూచీ)
{
  తిరిగి face->పట్టిక.fvar->get_instance_postscript_name_id (instance_సూచీ);
}

/**
 * hb_ot_var_named_instance_get_design_coords:
 * @face: The #hb_face_t to work on
 * @instance_index: The index of the named instance to query
 * @coords_length: (inout) (optional): Input = the maximum number of coordinates to return;
 *                 Output = the actual number of coordinates returned (may be zero)
 * @coords: (out) (array length=coords_length) (nullable): The array of coordinates found for the query
 *
 * Fetches the design-space coordinates corresponding to the given
 * named instance in the face.
 *
 * Return value: the number of variation axes in the face
 *
 * Since: 2.2.0
 **/
గుర్తులేని పూర్ణాంకం
hb_ot_var_named_instance_get_design_coords (hb_face_t    *face,
					    గుర్తులేని పూర్ణాంకం  instance_సూచీ,
					    గుర్తులేని పూర్ణాంకం *coords_నిడివి, /* IN/OUT */
					    దశాంశం        *coords         /*బహిర్గతం*/)
{
  తిరిగి face->పట్టిక.fvar->get_instance_coords (instance_సూచీ, coords_నిడివి, coords);
}


/**
 * hb_ot_var_normalize_variations:
 * @face: The #hb_face_t to work on
 * @variations: The array of variations to normalize
 * @variations_length: The number of variations to normalize
 * @coords: (out) (array length=coords_length): The array of normalized coordinates
 * @coords_length: The length of the coordinate array
 *
 * Normalizes all of the coordinates in the given list of variation axes.
 *
 * Since: 1.4.2
 **/
శూన్య
hb_ot_var_normalize_variations (hb_face_t            *face,
				స్థిరరాశి hb_variation_t *variations, /*లోపల*/
				గుర్తులేని పూర్ణాంకం          variations_నిడివి,
				పూర్ణాంకం                  *coords, /*బహిర్గతం*/
				గుర్తులేని పూర్ణాంకం          coords_నిడివి)
{
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < coords_నిడివి; i++)
    coords[i] = 0;

  స్థిరరాశి OT::fvar &fvar = *face->పట్టిక.fvar;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < variations_నిడివి; i++)
  {
    hb_ot_var_axis_info_t info;
    ఒకవేళ (hb_ot_var_కనుగొను_axis_info (face, variations[i].ట్యాగ్_2, &info) &&
	info.axis_సూచీ < coords_నిడివి)
      coords[info.axis_సూచీ] = roundf (fvar.normalize_axis_విలువ (info.axis_సూచీ, variations[i].value) * 65536.0f);
  }

  face->పట్టిక.avar->map_coords_16_16 (coords, coords_నిడివి);

  // Round to 2.14
  కొరకు (గుర్తులేని i = 0; i < coords_నిడివి; i++)
    coords[i] = (coords[i] + 2) >> 2;
}

/**
 * hb_ot_var_normalize_coords:
 * @face: The #hb_face_t to work on
 * @coords_length: The length of the coordinate array
 * @design_coords: The design-space coordinates to normalize
 * @normalized_coords: (out): The normalized coordinates
 *
 * Normalizes the given design-space coordinates. The minimum and maximum
 * values for the axis are mapped to the interval [-1,1], with the default
 * axis value mapped to 0.
 *
 * The normalized values have 14 bits of fixed-point sub-integer precision as per
 * OpenType specification.
 *
 * Any additional scaling defined in the face's `avar` table is also
 * applied, as described at https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 *
 * Note: @coords_length must be the same as the number of axes in the face, as
 * for example returned by hb_ot_var_get_axis_count().
 * Otherwise, the behavior is undefined.
 *
 * Since: 1.4.2
 **/
శూన్య
hb_ot_var_normalize_coords (hb_face_t    *face,
			    గుర్తులేని పూర్ణాంకం coords_నిడివి,
			    స్థిరరాశి దశాంశం *design_coords, /*లోపల*/
			    పూర్ణాంకం *normalized_coords /*బహిర్గతం*/)
{
  స్థిరరాశి OT::fvar &fvar = *face->పట్టిక.fvar;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < coords_నిడివి; i++)
    normalized_coords[i] = roundf (fvar.normalize_axis_విలువ (i, design_coords[i]) * 65536.0f);

  face->పట్టిక.avar->map_coords_16_16 (normalized_coords, coords_నిడివి);

  // Round to 2.14
  కొరకు (గుర్తులేని i = 0; i < coords_నిడివి; i++)
    normalized_coords[i] = (normalized_coords[i] + 2) >> 2;
}


#అయితే_ముగింపు
