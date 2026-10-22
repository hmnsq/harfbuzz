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

#إذا !defined(HB_H_وچ) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb.h> instead."
#جے_انت

#جے_بنا_تعریف HB_FACE_H
#تعریف_کرو HB_FACE_H

#ضمن "hb-common.h"
#ضمن "hb-blob.h"
#ضمن "hb-map.h"
#ضمن "hb-set.h"

HB_BEGIN_DECLS


HB_EXTERN بنانشانی صحيح
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
تعريفنوع بنية hb_face_t hb_face_t;

HB_EXTERN hb_face_t *
hb_face_create (hb_blob_t    *blob,
		بنانشانی صحيح  اشاریہ_2);

HB_EXTERN hb_face_t *
hb_face_create_or_fail (hb_blob_t    *blob,
			بنانشانی صحيح  اشاریہ_2);

HB_EXTERN hb_face_t *
hb_face_create_or_fail_using (hb_blob_t    *blob,
			      بنانشانی صحيح  اشاریہ_2,
			      ثابت حرف   *loader_name);

HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail (ثابت حرف   *file_name,
				  بنانشانی صحيح  اشاریہ_2);

HB_EXTERN hb_face_t *
hb_face_create_from_file_or_fail_using (ثابت حرف   *file_name,
					بنانشانی صحيح  اشاریہ_2,
					ثابت حرف   *loader_name);

HB_EXTERN ثابت حرف **
hb_face_list_loaders (فراغ);


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

تعريفنوع hb_blob_t * (*hb_reference_جدول_func_t)  (hb_face_t *face, hb_ٹیگ_t ٹیگ_2, فراغ *user_انکڑے);

/* calls destroy() when not needing user_data anymore */
HB_EXTERN hb_face_t *
hb_face_create_for_tables (hb_reference_جدول_func_t  reference_جدول_func,
			   فراغ                      *user_انکڑے,
			   hb_destroy_func_t          destroy);

HB_EXTERN hb_face_t *
hb_face_get_خالی_تھاواں (فراغ);

HB_EXTERN hb_face_t *
hb_face_reference (hb_face_t *face);

HB_EXTERN فراغ
hb_face_destroy (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_face_سیٹ_user_انکڑے (hb_face_t          *face,
		       hb_user_انکڑے_کلید_t *کلید_2,
		       فراغ *              انکڑے,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace);

HB_EXTERN فراغ *
hb_face_get_user_انکڑے (ثابت hb_face_t    *face,
		       hb_user_انکڑے_کلید_t *کلید_2);

HB_EXTERN فراغ
hb_face_make_immutable (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_face_is_immutable (hb_face_t *face);


HB_EXTERN hb_blob_t *
hb_face_reference_جدول (ثابت hb_face_t *face,
			 hb_ٹیگ_t ٹیگ_2);

HB_EXTERN hb_blob_t *
hb_face_reference_blob (hb_face_t *face);

HB_EXTERN فراغ
hb_face_سیٹ_اشاریہ (hb_face_t    *face,
		   بنانشانی صحيح  اشاریہ_2);

HB_EXTERN بنانشانی صحيح
hb_face_get_اشاریہ (ثابت hb_face_t *face);

HB_EXTERN فراغ
hb_face_سیٹ_upem (hb_face_t    *face,
		  بنانشانی صحيح  upem);

HB_EXTERN بنانشانی صحيح
hb_face_get_upem (ثابت hb_face_t *face);

HB_EXTERN فراغ
hb_face_سیٹ_glyph_گنتی (hb_face_t    *face,
			 بنانشانی صحيح  glyph_گنتی);

HB_EXTERN بنانشانی صحيح
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
تعريفنوع بنانشانی صحيح (*hb_get_جدول_ٹیگ_سوچی_func_t) (ثابت hb_face_t *face,
						  بنانشانی صحيح  شروع_وتھ,
						  بنانشانی صحيح *جدول_گنتی_2, /* IN/OUT */
						  hb_ٹیگ_t     *جدول_ٹیگ_سوچی /*باہرلا*/,
						  فراغ         *user_انکڑے);

HB_EXTERN فراغ
hb_face_سیٹ_get_جدول_ٹیگ_سوچی_func (hb_face_t *face,
				 hb_get_جدول_ٹیگ_سوچی_func_t func,
				 فراغ                    *user_انکڑے,
				 hb_destroy_func_t        destroy);

HB_EXTERN بنانشانی صحيح
hb_face_get_جدول_ٹیگ_سوچی (ثابت hb_face_t *face,
			بنانشانی صحيح  شروع_وتھ,
			بنانشانی صحيح *جدول_گنتی_2, /* IN/OUT */
			hb_ٹیگ_t     *جدول_ٹیگ_سوچی /*باہرلا*/);


/*
 * Character set.
 */

HB_EXTERN فراغ
hb_face_collect_unicodes (hb_face_t *face,
			  hb_سیٹ_t  *باہرلا);

HB_EXTERN فراغ
hb_face_collect_nominal_glyph_mapping (hb_face_t *face,
				       hb_map_t  *mapping,
				       hb_سیٹ_t  *unicodes);

HB_EXTERN فراغ
hb_face_collect_variation_selectors (hb_face_t *face,
				     hb_سیٹ_t  *باہرلا);

HB_EXTERN فراغ
hb_face_collect_variation_unicodes (hb_face_t *face,
				    hb_codepoint_t variation_selector,
				    hb_سیٹ_t  *باہرلا);


/*
 * Builder face.
 */

HB_EXTERN hb_face_t *
hb_face_builder_create (فراغ);

HB_EXTERN hb_bool_t
hb_face_builder_جوڑو_جدول (hb_face_t *face,
			   hb_ٹیگ_t   ٹیگ_2,
			   hb_blob_t *blob);

HB_EXTERN فراغ
hb_face_builder_ترتیب_دیو_tables (hb_face_t *face,
                             ثابت hb_ٹیگ_t  *ٹیگ_سوچی);


HB_سرا_DECLS

#جے_انت /* HB_FACE_H */
