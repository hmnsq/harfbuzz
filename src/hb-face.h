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

#اگر !defined(HB_H_اندر) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb.h> instead."
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_FACE_H
#تعریف_کرو HB_FACE_H

#شامل "hb-common.h"
#شامل "hb-blob.h"
#شامل "hb-map.h"
#شامل "hb-set.h"

HB_BEGIN_DECLS


HB_EXTERN غیرعلامتی صحیح
hb_face_گنتی (hb_blob_t *blob);


/*
 * hb_face_t
 */

/**
 * hb_face_t:
 *
 * Data type for holding font faces.
 *
 **/
قسمتعریف ساخت hb_face_t hb_face_t;

HB_EXTERN hb_face_t *
hb_face_create (hb_blob_t    *blob,
		غیرعلامتی صحیح  اشاریہ_2);

HB_EXTERN hb_face_t *
hb_face_create_or_fail (hb_blob_t    *blob,
			غیرعلامتی صحیح  اشاریہ_2);

HB_EXTERN hb_face_t *
hb_face_create_or_fail_using (hb_blob_t    *blob,
			      غیرعلامتی صحیح  اشاریہ_2,
			      ثابت حرف   *loader_name);

HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail (ثابت حرف   *file_name,
				  غیرعلامتی صحیح  اشاریہ_2);

HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail_using (ثابت حرف   *file_name,
					غیرعلامتی صحیح  اشاریہ_2,
					ثابت حرف   *loader_name);

HB_EXTERN ثابت حرف **
hb_face_list_loaders (خالی);


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

قسمتعریف hb_blob_t * (*hb_reference_جدول_func_t)  (hb_face_t *face, hb_لیبل_t لیبل_2, خالی *user_ڈیٹا);

/* calls destroy() when not needing user_data anymore */
HB_EXTERN hb_face_t *
hb_face_create_for_tables (hb_reference_جدول_func_t  reference_جدول_func,
			   خالی                      *user_ڈیٹا,
			   hb_destroy_func_t          destroy);

HB_EXTERN hb_face_t *
hb_face_get_خالی_خانے (خالی);

HB_EXTERN hb_face_t *
hb_face_reference (hb_face_t *face);

HB_EXTERN خالی
hb_face_destroy (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_face_سیٹ_user_ڈیٹا (hb_face_t          *face,
		       hb_user_ڈیٹا_کلید_t *کلید_2,
		       خالی *              ڈیٹا,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace);

HB_EXTERN خالی *
hb_face_get_user_ڈیٹا (ثابت hb_face_t    *face,
		       hb_user_ڈیٹا_کلید_t *کلید_2);

HB_EXTERN خالی
hb_face_make_immutable (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_face_is_immutable (hb_face_t *face);


HB_EXTERN hb_blob_t *
hb_face_reference_جدول (ثابت hb_face_t *face,
			 hb_لیبل_t لیبل_2);

HB_EXTERN hb_blob_t *
hb_face_reference_blob (hb_face_t *face);

HB_EXTERN خالی
hb_face_سیٹ_اشاریہ (hb_face_t    *face,
		   غیرعلامتی صحیح  اشاریہ_2);

HB_EXTERN غیرعلامتی صحیح
hb_face_get_اشاریہ (ثابت hb_face_t *face);

HB_EXTERN خالی
hb_face_سیٹ_upem (hb_face_t    *face,
		  غیرعلامتی صحیح  upem);

HB_EXTERN غیرعلامتی صحیح
hb_face_get_upem (ثابت hb_face_t *face);

HB_EXTERN خالی
hb_face_سیٹ_glyph_گنتی (hb_face_t    *face,
			 غیرعلامتی صحیح  glyph_گنتی);

HB_EXTERN غیرعلامتی صحیح
hb_face_get_glyph_گنتی (ثابت hb_face_t *face);


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
قسمتعریف غیرعلامتی صحیح (*hb_get_جدول_لیبل_فہرست_func_t) (ثابت hb_face_t *face,
						  غیرعلامتی صحیح  آغاز_انحراف,
						  غیرعلامتی صحیح *جدول_گنتی_2, /* IN/OUT */
						  hb_لیبل_t     *جدول_لیبل_فہرست /*برآمد*/,
						  خالی         *user_ڈیٹا);

HB_EXTERN خالی
hb_face_سیٹ_get_جدول_لیبل_فہرست_func (hb_face_t *face,
				 hb_get_جدول_لیبل_فہرست_func_t func,
				 خالی                    *user_ڈیٹا,
				 hb_destroy_func_t        destroy);

HB_EXTERN غیرعلامتی صحیح
hb_face_get_جدول_لیبل_فہرست (ثابت hb_face_t *face,
			غیرعلامتی صحیح  آغاز_انحراف,
			غیرعلامتی صحیح *جدول_گنتی_2, /* IN/OUT */
			hb_لیبل_t     *جدول_لیبل_فہرست /*برآمد*/);


/*
 * Character set.
 */

HB_EXTERN خالی
hb_face_collect_unicodes (hb_face_t *face,
			  hb_سیٹ_t  *برآمد);

HB_EXTERN خالی
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_سیٹ_t  *unicodes);

HB_EXTERN خالی
hb_face_collect_variation_selectors (hb_face_t *face,
				     hb_سیٹ_t  *برآمد);

HB_EXTERN خالی
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_سیٹ_t  *برآمد);


/*
 * Builder face.
 */

HB_EXTERN hb_face_t *
hb_face_builder_create (خالی);

HB_EXTERN hb_bool_t
hb_face_builder_جوڑو_جدول (hb_face_t *face,
			   hb_لیبل_t   لیبل_2,
			   hb_blob_t *blob);

HB_EXTERN خالی
hb_face_builder_ترتیب_دو_tables (hb_face_t *face,
                             ثابت hb_لیبل_t  *لیبل_فہرست);


HB_انتہا_DECLS

#اگر_ختم /* HB_FACE_H */
