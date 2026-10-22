/*
 * Copyright © 2009  Red Hat, Inc.
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

#ఒకవేళ !defined(HB_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_FACE_H
#నిర్వచించు HB_FACE_H

#అన్తర్భావ "hb-common.h"
#అన్తర్భావ "hb-blob.h"
#అన్తర్భావ "hb-map.h"
#అన్తర్భావ "hb-set.h"

HB_BEGIN_DECLS


HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_face_లెక్క (hb_blob_t *blob);


/*
 * hb_face_t
 */

/**
 * hb_face_t:
 *
 * Data type for holding font faces.
 *
 **/
రకనిర్వచనం నిర్మాణం hb_face_t hb_face_t;

HB_EXTERN hb_face_t *
hb_face_create (hb_blob_t    *blob,
		గుర్తులేని పూర్ణాంకం  సూచీ_2);

HB_EXTERN hb_face_t *
hb_face_create_or_fail (hb_blob_t    *blob,
			గుర్తులేని పూర్ణాంకం  సూచీ_2);

HB_EXTERN hb_face_t *
hb_face_create_or_fail_using (hb_blob_t    *blob,
			      గుర్తులేని పూర్ణాంకం  సూచీ_2,
			      స్థిరరాశి అక్షరం   *loader_name);

HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail (స్థిరరాశి అక్షరం   *file_name,
				  గుర్తులేని పూర్ణాంకం  సూచీ_2);

HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail_using (స్థిరరాశి అక్షరం   *file_name,
					గుర్తులేని పూర్ణాంకం  సూచీ_2,
					స్థిరరాశి అక్షరం   *loader_name);

HB_EXTERN స్థిరరాశి అక్షరం **
hb_face_list_loaders (శూన్య);


/**
 * hb_reference_table_func_t:
 * @face: an #hb_face_t to reference table for
 * @tag: the tag of the table to reference
 * @user_data: User data pointer passed by the caller
 *
 * Callback function for hb_face_create_for_tables(). The @tag is the tag of the
 * table to reference, and the special tag #HB_TAG_NONE is used to reference the
 * blob of the face itself. If referencing the face blob is not possible, it is
 * recommended to set hb_get_table_tags_func_t on the @face to allow
 * hb_face_reference_blob() to create a face blob out of individual table blobs.
 *
 * Return value: (transfer full): A pointer to the @tag table within @face or
 * `NULL` if the table is not found or cannot be referenced.
 *
 * Since: 0.9.2
 */

రకనిర్వచనం hb_blob_t * (*hb_reference_పట్టిక_func_t)  (hb_face_t *face, hb_ట్యాగ్_t ట్యాగ్_2, శూన్య *user_దత్తాంశం);

/* calls destroy() when not needing user_data anymore */
HB_EXTERN hb_face_t *
hb_face_create_for_tables (hb_reference_పట్టిక_func_t  reference_పట్టిక_func,
			   శూన్య                      *user_దత్తాంశం,
			   hb_destroy_func_t          destroy);

HB_EXTERN hb_face_t *
hb_face_get_ఖాళీలు (శూన్య);

HB_EXTERN hb_face_t *
hb_face_reference (hb_face_t *face);

HB_EXTERN శూన్య
hb_face_destroy (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_face_సమితి_user_దత్తాంశం (hb_face_t          *face,
		       hb_user_దత్తాంశం_కీలకం_t *కీలకం_2,
		       శూన్య *              దత్తాంశం,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace);

HB_EXTERN శూన్య *
hb_face_get_user_దత్తాంశం (స్థిరరాశి hb_face_t    *face,
		       hb_user_దత్తాంశం_కీలకం_t *కీలకం_2);

HB_EXTERN శూన్య
hb_face_make_immutable (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_face_is_immutable (hb_face_t *face);


HB_EXTERN hb_blob_t *
hb_face_reference_పట్టిక (స్థిరరాశి hb_face_t *face,
			 hb_ట్యాగ్_t ట్యాగ్_2);

HB_EXTERN hb_blob_t *
hb_face_reference_blob (hb_face_t *face);

HB_EXTERN శూన్య
hb_face_సమితి_సూచీ (hb_face_t    *face,
		   గుర్తులేని పూర్ణాంకం  సూచీ_2);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_face_get_సూచీ (స్థిరరాశి hb_face_t *face);

HB_EXTERN శూన్య
hb_face_సమితి_upem (hb_face_t    *face,
		  గుర్తులేని పూర్ణాంకం  upem);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_face_get_upem (స్థిరరాశి hb_face_t *face);

HB_EXTERN శూన్య
hb_face_సమితి_glyph_లెక్క (hb_face_t    *face,
			 గుర్తులేని పూర్ణాంకం  glyph_లెక్క);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_face_get_glyph_లెక్క (స్థిరరాశి hb_face_t *face);


/**
 * hb_get_table_tags_func_t:
 * @face: A face object
 * @start_offset: The index of first table tag to retrieve
 * @table_count: (inout): Input = the maximum number of table tags to return;
 *                Output = the actual number of table tags returned (may be zero)
 * @table_tags: (out) (array length=table_count) (nullable): The array of table tags found
 * @user_data: User data pointer passed by the caller
 *
 * Callback function for hb_face_get_table_tags().
 *
 * Return value: Total number of tables, or zero if it is not possible to list
 *
 * Since: 10.0.0
 */
రకనిర్వచనం గుర్తులేని పూర్ణాంకం (*hb_get_పట్టిక_ట్యాగులు_func_t) (స్థిరరాశి hb_face_t *face,
						  గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
						  గుర్తులేని పూర్ణాంకం *పట్టిక_లెక్క_2, /* IN/OUT */
						  hb_ట్యాగ్_t     *పట్టిక_ట్యాగులు /*బహిర్గతం*/,
						  శూన్య         *user_దత్తాంశం);

HB_EXTERN శూన్య
hb_face_సమితి_get_పట్టిక_ట్యాగులు_func (hb_face_t *face,
				 hb_get_పట్టిక_ట్యాగులు_func_t func,
				 శూన్య                    *user_దత్తాంశం,
				 hb_destroy_func_t        destroy);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_face_get_పట్టిక_ట్యాగులు (స్థిరరాశి hb_face_t *face,
			గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
			గుర్తులేని పూర్ణాంకం *పట్టిక_లెక్క_2, /* IN/OUT */
			hb_ట్యాగ్_t     *పట్టిక_ట్యాగులు /*బహిర్గతం*/);


/*
 * Character set.
 */

HB_EXTERN శూన్య
hb_face_collect_unicodes (hb_face_t *face,
			  hb_సమితి_t  *బహిర్గతం);

HB_EXTERN శూన్య
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_సమితి_t  *unicodes);

HB_EXTERN శూన్య
hb_face_collect_variation_selectors (hb_face_t *face,
				     hb_సమితి_t  *బహిర్గతం);

HB_EXTERN శూన్య
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_సమితి_t  *బహిర్గతం);


/*
 * Builder face.
 */

HB_EXTERN hb_face_t *
hb_face_builder_create (శూన్య);

HB_EXTERN hb_bool_t
hb_face_builder_కూడు_పట్టిక (hb_face_t *face,
			   hb_ట్యాగ్_t   ట్యాగ్_2,
			   hb_blob_t *blob);

HB_EXTERN శూన్య
hb_face_builder_క్రమీకరించు_tables (hb_face_t *face,
                             స్థిరరాశి hb_ట్యాగ్_t  *ట్యాగులు);


HB_చివర_DECLS

#అయితే_ముగింపు /* HB_FACE_H */
