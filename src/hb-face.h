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

#सचे !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#error "Include <hb.h> instead."
#endif

#ifndef HB_FACE_H
#define HB_FACE_H

#अन्तर्भाव "hb-common.h"
#अन्तर्भाव "hb-blob.h"
#अन्तर्भाव "hb-map.h"
#अन्तर्भाव "hb-set.h"

HB_BEGIN_DECLS


HB_EXTERN unsigned पूर्णाङ्क
hb_face_गणनम् (hb_blob_t *blob);


/*
 * hb_face_t
 */

/**
 * hb_face_t:
 *
 * Data type for holding font faces.
 *
 **/
प्रकारनिर्देश संरचना hb_face_t hb_face_t;

HB_EXTERN hb_face_t *
hb_face_create (hb_blob_t    *blob,
		unsigned पूर्णाङ्क  index);

HB_EXTERN hb_face_t *
hb_face_create_or_fail (hb_blob_t    *blob,
			unsigned पूर्णाङ्क  index);

HB_EXTERN hb_face_t *
hb_face_create_or_fail_using (hb_blob_t    *blob,
			      unsigned पूर्णाङ्क  index,
			      ध्रुव वर्ण   *loader_name);

HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail (ध्रुव वर्ण   *file_name,
				  unsigned पूर्णाङ्क  index);

HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail_using (ध्रुव वर्ण   *file_name,
					unsigned पूर्णाङ्क  index,
					ध्रुव वर्ण   *loader_name);

HB_EXTERN ध्रुव वर्ण **
hb_face_list_loaders (शून्य);


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

प्रकारनिर्देश hb_blob_t * (*hb_reference_table_func_t)  (hb_face_t *face, hb_tag_t tag, शून्य *user_data);

/* calls destroy() when not needing user_data anymore */
HB_EXTERN hb_face_t *
hb_face_create_for_tables (hb_reference_table_func_t  reference_table_func,
			   शून्य                      *user_data,
			   hb_destroy_func_t          destroy);

HB_EXTERN hb_face_t *
hb_face_get_empty (शून्य);

HB_EXTERN hb_face_t *
hb_face_reference (hb_face_t *face);

HB_EXTERN शून्य
hb_face_destroy (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_face_set_user_data (hb_face_t          *face,
		       hb_user_data_कुञ्चिका_t *कुञ्चिका_2,
		       शून्य *              data,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace);

HB_EXTERN शून्य *
hb_face_get_user_data (ध्रुव hb_face_t    *face,
		       hb_user_data_कुञ्चिका_t *कुञ्चिका_2);

HB_EXTERN शून्य
hb_face_make_immutable (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_face_is_immutable (hb_face_t *face);


HB_EXTERN hb_blob_t *
hb_face_reference_table (ध्रुव hb_face_t *face,
			 hb_tag_t tag);

HB_EXTERN hb_blob_t *
hb_face_reference_blob (hb_face_t *face);

HB_EXTERN शून्य
hb_face_set_index (hb_face_t    *face,
		   unsigned पूर्णाङ्क  index);

HB_EXTERN unsigned पूर्णाङ्क
hb_face_get_index (ध्रुव hb_face_t *face);

HB_EXTERN शून्य
hb_face_set_upem (hb_face_t    *face,
		  unsigned पूर्णाङ्क  upem);

HB_EXTERN unsigned पूर्णाङ्क
hb_face_get_upem (ध्रुव hb_face_t *face);

HB_EXTERN शून्य
hb_face_set_glyph_गणनम् (hb_face_t    *face,
			 unsigned पूर्णाङ्क  glyph_गणनम्);

HB_EXTERN unsigned पूर्णाङ्क
hb_face_get_glyph_गणनम् (ध्रुव hb_face_t *face);


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
प्रकारनिर्देश unsigned पूर्णाङ्क (*hb_get_table_tags_func_t) (ध्रुव hb_face_t *face,
						  unsigned पूर्णाङ्क  start_offset,
						  unsigned पूर्णाङ्क *table_गणनम्_2, /* IN/OUT */
						  hb_tag_t     *table_tags /* OUT */,
						  शून्य         *user_data);

HB_EXTERN शून्य
hb_face_set_get_table_tags_func (hb_face_t *face,
				 hb_get_table_tags_func_t func,
				 शून्य                    *user_data,
				 hb_destroy_func_t        destroy);

HB_EXTERN unsigned पूर्णाङ्क
hb_face_get_table_tags (ध्रुव hb_face_t *face,
			unsigned पूर्णाङ्क  start_offset,
			unsigned पूर्णाङ्क *table_गणनम्_2, /* IN/OUT */
			hb_tag_t     *table_tags /* OUT */);


/*
 * Character set.
 */

HB_EXTERN शून्य
hb_face_collect_unicodes (hb_face_t *face,
			  hb_set_t  *out);

HB_EXTERN शून्य
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_set_t  *unicodes);

HB_EXTERN शून्य
hb_face_collect_variation_selectors (hb_face_t *face,
				     hb_set_t  *out);

HB_EXTERN शून्य
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_set_t  *out);


/*
 * Builder face.
 */

HB_EXTERN hb_face_t *
hb_face_builder_create (शून्य);

HB_EXTERN hb_bool_t
hb_face_builder_add_table (hb_face_t *face,
			   hb_tag_t   tag,
			   hb_blob_t *blob);

HB_EXTERN शून्य
hb_face_builder_sort_tables (hb_face_t *face,
                             ध्रुव hb_tag_t  *tags);


HB_END_DECLS

#endif /* HB_FACE_H */
