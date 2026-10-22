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

#ਜੇ !defined(HB_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
#ਗਲਤੀ "Include <hb.h> instead."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_FACE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FACE_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-blob.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.h"

HB_BEGIN_DECLS


HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_ਗਿਣਤੀ (hb_blob_t *blob);


/*
 * hb_face_t
 */

/**
 * hb_face_t:
 *
 * Data type for holding font faces.
 *
 **/
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_face_t hb_face_t;

HB_EXTERN hb_face_t *
hb_face_create (hb_blob_t    *blob,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2);

HB_EXTERN hb_face_t *
hb_face_create_or_fail (hb_blob_t    *blob,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2);

HB_EXTERN hb_face_t *
hb_face_create_or_fail_using (hb_blob_t    *blob,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2,
			      ਅਚਲ ਅੱਖਰ   *loader_name);

HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail (ਅਚਲ ਅੱਖਰ   *file_name,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2);

HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail_using (ਅਚਲ ਅੱਖਰ   *file_name,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2,
					ਅਚਲ ਅੱਖਰ   *loader_name);

HB_EXTERN ਅਚਲ ਅੱਖਰ **
hb_face_list_loaders (ਖਾਲੀ);


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

ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_blob_t * (*hb_reference_ਸਾਰਣੀ_func_t)  (hb_face_t *face, hb_ਟੈਗ_t ਟੈਗ_2, ਖਾਲੀ *user_ਅੰਕੜੇ);

/* calls destroy() when not needing user_data anymore */
HB_EXTERN hb_face_t *
hb_face_create_for_tables (hb_reference_ਸਾਰਣੀ_func_t  reference_ਸਾਰਣੀ_func,
			   ਖਾਲੀ                      *user_ਅੰਕੜੇ,
			   hb_destroy_func_t          destroy);

HB_EXTERN hb_face_t *
hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ (ਖਾਲੀ);

HB_EXTERN hb_face_t *
hb_face_reference (hb_face_t *face);

HB_EXTERN ਖਾਲੀ
hb_face_destroy (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_face_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_face_t          *face,
		       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2,
		       ਖਾਲੀ *              ਅੰਕੜੇ,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace);

HB_EXTERN ਖਾਲੀ *
hb_face_get_user_ਅੰਕੜੇ (ਅਚਲ hb_face_t    *face,
		       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *ਕੁੰਜੀ_2);

HB_EXTERN ਖਾਲੀ
hb_face_make_immutable (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_face_is_immutable (hb_face_t *face);


HB_EXTERN hb_blob_t *
hb_face_reference_ਸਾਰਣੀ (ਅਚਲ hb_face_t *face,
			 hb_ਟੈਗ_t ਟੈਗ_2);

HB_EXTERN hb_blob_t *
hb_face_reference_blob (hb_face_t *face);

HB_EXTERN ਖਾਲੀ
hb_face_ਸੈੱਟ_ਸੂਚਕਾਂਕ (hb_face_t    *face,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_get_ਸੂਚਕਾਂਕ (ਅਚਲ hb_face_t *face);

HB_EXTERN ਖਾਲੀ
hb_face_ਸੈੱਟ_upem (hb_face_t    *face,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  upem);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_get_upem (ਅਚਲ hb_face_t *face);

HB_EXTERN ਖਾਲੀ
hb_face_ਸੈੱਟ_glyph_ਗਿਣਤੀ (hb_face_t    *face,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  glyph_ਗਿਣਤੀ);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_get_glyph_ਗਿਣਤੀ (ਅਚਲ hb_face_t *face);


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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ (*hb_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func_t) (ਅਚਲ hb_face_t *face,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਾਰਣੀ_ਗਿਣਤੀ_2, /* IN/OUT */
						  hb_ਟੈਗ_t     *ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/,
						  ਖਾਲੀ         *user_ਅੰਕੜੇ);

HB_EXTERN ਖਾਲੀ
hb_face_ਸੈੱਟ_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func (hb_face_t *face,
				 hb_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func_t func,
				 ਖਾਲੀ                    *user_ਅੰਕੜੇ,
				 hb_destroy_func_t        destroy);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_face_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (ਅਚਲ hb_face_t *face,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਾਰਣੀ_ਗਿਣਤੀ_2, /* IN/OUT */
			hb_ਟੈਗ_t     *ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/);


/*
 * Character set.
 */

HB_EXTERN ਖਾਲੀ
hb_face_collect_unicodes (hb_face_t *face,
			  hb_ਸੈੱਟ_t  *ਬਾਹਰਲਾ);

HB_EXTERN ਖਾਲੀ
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_ਸੈੱਟ_t  *unicodes);

HB_EXTERN ਖਾਲੀ
hb_face_collect_variation_selectors (hb_face_t *face,
				     hb_ਸੈੱਟ_t  *ਬਾਹਰਲਾ);

HB_EXTERN ਖਾਲੀ
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_ਸੈੱਟ_t  *ਬਾਹਰਲਾ);


/*
 * Builder face.
 */

HB_EXTERN hb_face_t *
hb_face_builder_create (ਖਾਲੀ);

HB_EXTERN hb_bool_t
hb_face_builder_ਜੋੜੋ_ਸਾਰਣੀ (hb_face_t *face,
			   hb_ਟੈਗ_t   ਟੈਗ_2,
			   hb_blob_t *blob);

HB_EXTERN ਖਾਲੀ
hb_face_builder_ਛਾਂਟੋ_tables (hb_face_t *face,
                             ਅਚਲ hb_ਟੈਗ_t  *ਟੈਗ_ਸੂਚੀ);


HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_FACE_H */
