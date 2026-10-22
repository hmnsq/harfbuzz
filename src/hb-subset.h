/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Rod Sheeter
 */

#اگر_غیر_تعریف_شدہ HB_SUBSET_H
#تعریف_کرو HB_SUBSET_H
#تعریف_کرو HB_SUBSET_H_اندر

#شامل "hb.h"
#شامل "hb-ot.h"
#شامل "hb-subset-serialize.h"
#شامل "hb-subset-depend.h"
#شامل "hb-subset-serialize.h"

#تعریف_منسوخ HB_SUBSET_H_اندر

HB_BEGIN_DECLS

/**
 * hb_subset_input_t:
 *
 * Things that change based on the input. Characters to keep, etc.
 */

قسمتعریف ساخت hb_subset_داخلہ_t hb_subset_داخلہ_t;

/**
 * hb_subset_plan_t:
 *
 * Contains information about how the subset operation will be executed.
 * Such as mappings from the old glyph ids to the new ones in the subset.
 */

قسمتعریف ساخت hb_subset_plan_t hb_subset_plan_t;

/**
 * hb_subset_flags_t:
 * @HB_SUBSET_FLAGS_DEFAULT: all flags at their default value of false.
 * @HB_SUBSET_FLAGS_NO_HINTING: If set hinting instructions will be dropped in
 * the produced subset. Otherwise hinting instructions will be retained.
 * @HB_SUBSET_FLAGS_RETAIN_GIDS: If set glyph indices will not be modified in
 * the produced subset. If glyphs are dropped their indices will be retained
 * as an empty glyph.
 * @HB_SUBSET_FLAGS_DESUBROUTINIZE: If set and subsetting a CFF font the
 * subsetter will attempt to remove subroutines from the CFF glyphs.
 * @HB_SUBSET_FLAGS_NAME_LEGACY: If set non-unicode name records will be
 * retained in the subset.
 * @HB_SUBSET_FLAGS_SET_OVERLAPS_FLAG:	If set the subsetter will set the
 * OVERLAP_SIMPLE flag on each simple glyph.
 * @HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED: If set the subsetter will not
 * drop unrecognized tables and instead pass them through untouched.
 * @HB_SUBSET_FLAGS_NOTDEF_OUTLINE: If set the notdef glyph outline will be
 * retained in the final subset.
 * @HB_SUBSET_FLAGS_GLYPH_NAMES: If set the PS glyph names will be retained
 * in the final subset.
 * @HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES: If set then the unicode ranges in
 * OS/2 will not be recalculated.
 * @HB_SUBSET_FLAGS_NO_LAYOUT_CLOSURE: If set do not perform glyph closure on layout
 * substitution rules (GSUB). Since: 7.2.0.
 * @HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS: If set perform IUP delta optimization on the
 * remaining gvar table's deltas. Since: 8.5.0
 * @HB_SUBSET_FLAGS_NO_BIDI_CLOSURE: If set do not pull mirrored versions of input
 * codepoints into the subset. Since: 11.1.0
 * @HB_SUBSET_FLAGS_IFTB_REQUIREMENTS: If set enforce requirements on the output subset
 * to allow it to be used with incremental font transfer IFTB patches. Primarily,
 * this forces all outline data to use long (32 bit) offsets. Since: EXPERIMENTAL
 * @HB_SUBSET_FLAGS_RETAIN_NUM_GLYPHS: If this flag is set along side
 * HB_SUBSET_FLAGS_RETAIN_GIDS then the number of glyphs in the font won't
 * be reduced as a result of subsetting. If necessary empty glyphs will be
 * included at the end of the font to keep the number of glyphs unchanged.
 * @HB_SUBSET_FLAGS_DOWNGRADE_CFF2: If set and instantiating a variable font
 * with all axes pinned, convert the output CFF2 table to CFF1. This enables
 * compatibility with older renderers that don't support CFF2. Since: 13.0.0
 * @HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET: If set and subsetting a CID-keyed CFF
 * font, the output CFF charset will use sequential identity CIDs (CID = new
 * GID) rather than preserving the original CIDs. Since: 14.3.0
 *
 * List of boolean properties that can be configured on the subset input.
 *
 * Since: 2.9.0
 **/
قسمتعریف شمار { /*< flags >*/
  HB_SUBSET_FLAGS_DEFAULT =		     0x00000000u,
  HB_SUBSET_FLAGS_NO_HINTING =		     0x00000001u,
  HB_SUBSET_FLAGS_RETAIN_GIDS =		     0x00000002u,
  HB_SUBSET_FLAGS_DESUBROUTINIZE =	     0x00000004u,
  HB_SUBSET_FLAGS_NAME_LEGACY =		     0x00000008u,
  HB_SUBSET_FLAGS_سیٹ_OVERLAPS_FLAG =	     0x00000010u,
  HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED = 0x00000020u,
  HB_SUBSET_FLAGS_NOTDEF_OUTLINE =	     0x00000040u,
  HB_SUBSET_FLAGS_GLYPH_نام =		     0x00000080u,
  HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES =  0x00000100u,
  HB_SUBSET_FLAGS_NO_LAYOUT_احاطہ =        0x00000200u,
  HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS	  =  0x00000400u,
  HB_SUBSET_FLAGS_NO_BIDI_احاطہ         =  0x00000800u,
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
  HB_SUBSET_FLAGS_IFTB_REQUIREMENTS       =  0x00001000u,
  HB_SUBSET_FLAGS_RETAIN_عدد_GLYPHS  =  0x00002000u,
#اگر_ختم
  HB_SUBSET_FLAGS_DOWNGRADE_CFF2          =  0x00004000u,
  HB_SUBSET_FLAGS_CFF_شناختی_عنصر_CHARSET    =  0x00008000u,
} hb_subset_flags_t;

/**
 * hb_subset_sets_t:
 * @HB_SUBSET_SETS_GLYPH_INDEX: the set of glyph indexes to retain in the subset.
 * @HB_SUBSET_SETS_UNICODE: the set of unicode codepoints to retain in the subset.
 * @HB_SUBSET_SETS_NO_SUBSET_TABLE_TAG: the set of table tags which specifies tables that should not be
 * subsetted.
 * @HB_SUBSET_SETS_DROP_TABLE_TAG: the set of table tags which specifies tables which will be dropped
 * in the subset.
 * @HB_SUBSET_SETS_NAME_ID: the set of name ids that will be retained.
 * @HB_SUBSET_SETS_NAME_LANG_ID: the set of name lang ids that will be retained.
 * @HB_SUBSET_SETS_LAYOUT_FEATURE_TAG: the set of layout feature tags that will be retained
 * in the subset.
 * @HB_SUBSET_SETS_LAYOUT_SCRIPT_TAG: the set of layout script tags that will be retained
 * in the subset. Defaults to all tags. Since: 5.0.0
 *
 * List of sets that can be configured on the subset input.
 *
 * Since: 2.9.1
 **/
قسمتعریف شمار {
  HB_SUBSET_SETS_GLYPH_اشاریہ = 0,
  HB_SUBSET_SETS_UNICODE,
  HB_SUBSET_SETS_NO_SUBSET_جدول_لیبل,
  HB_SUBSET_SETS_DROP_جدول_لیبل,
  HB_SUBSET_SETS_NAME_ID,
  HB_SUBSET_SETS_NAME_LANG_ID,
  HB_SUBSET_SETS_LAYOUT_FEATURE_لیبل,
  HB_SUBSET_SETS_LAYOUT_SCRIPT_لیبل,
} hb_subset_sets_t;

HB_EXTERN hb_subset_داخلہ_t *
hb_subset_داخلہ_create_or_fail (خالی);

HB_EXTERN hb_subset_داخلہ_t *
hb_subset_داخلہ_reference (hb_subset_داخلہ_t *داخلہ);

HB_EXTERN خالی
hb_subset_داخلہ_destroy (hb_subset_داخلہ_t *داخلہ);

HB_EXTERN hb_bool_t
hb_subset_داخلہ_سیٹ_user_ڈیٹا (hb_subset_داخلہ_t  *داخلہ,
			       hb_user_ڈیٹا_کلید_t *کلید_2,
			       خالی *		   ڈیٹا,
			       hb_destroy_func_t   destroy,
			       hb_bool_t	   replace);

HB_EXTERN خالی *
hb_subset_داخلہ_get_user_ڈیٹا (ثابت hb_subset_داخلہ_t *داخلہ,
			       hb_user_ڈیٹا_کلید_t      *کلید_2);

HB_EXTERN خالی
hb_subset_داخلہ_keep_everything (hb_subset_داخلہ_t *داخلہ);

HB_EXTERN hb_سیٹ_t *
hb_subset_داخلہ_unicode_سیٹ (hb_subset_داخلہ_t *داخلہ);

HB_EXTERN hb_سیٹ_t *
hb_subset_داخلہ_glyph_سیٹ (hb_subset_داخلہ_t *داخلہ);

HB_EXTERN hb_سیٹ_t *
hb_subset_داخلہ_سیٹ (hb_subset_داخلہ_t *داخلہ, hb_subset_sets_t سیٹ_type);

HB_EXTERN hb_map_t*
hb_subset_داخلہ_old_مقصد_new_glyph_mapping (hb_subset_داخلہ_t *داخلہ);

HB_EXTERN hb_subset_flags_t
hb_subset_داخلہ_get_flags (hb_subset_داخلہ_t *داخلہ);

HB_EXTERN خالی
hb_subset_داخلہ_سیٹ_flags (hb_subset_داخلہ_t *داخلہ,
			   غیرعلامتی قدر_2);

HB_EXTERN hb_bool_t
hb_subset_داخلہ_pin_all_axes_مقصد_default (hb_subset_داخلہ_t  *داخلہ,
					 hb_face_t          *face);

HB_EXTERN hb_bool_t
hb_subset_داخلہ_pin_axis_مقصد_default (hb_subset_داخلہ_t  *داخلہ,
				     hb_face_t          *face,
				     hb_لیبل_t            axis_لیبل_2);

HB_EXTERN hb_bool_t
hb_subset_داخلہ_pin_axis_location (hb_subset_داخلہ_t  *داخلہ,
				   hb_face_t          *face,
				   hb_لیبل_t            axis_لیبل_2,
				   اعشاری               axis_قدر);

HB_EXTERN hb_bool_t
hb_subset_داخلہ_get_axis_range (hb_subset_داخلہ_t  *داخلہ,
				hb_لیبل_t            axis_لیبل_2,
				اعشاری              *axis_min_قدر,
				اعشاری              *axis_max_قدر,
				اعشاری              *axis_def_قدر);

HB_EXTERN hb_bool_t
hb_subset_داخلہ_سیٹ_axis_range (hb_subset_داخلہ_t  *داخلہ,
				hb_face_t          *face,
				hb_لیبل_t            axis_لیبل_2,
				اعشاری               axis_min_قدر,
				اعشاری               axis_max_قدر,
				اعشاری               axis_def_قدر);

HB_EXTERN hb_bool_t
hb_subset_axis_range_from_string (ثابت حرف *str, صحیح len,
				  اعشاری *axis_min_قدر,
				  اعشاری *axis_max_قدر,
				  اعشاری *axis_def_قدر);

HB_EXTERN خالی
hb_subset_axis_range_مقصد_string (hb_subset_داخلہ_t *داخلہ,
				hb_لیبل_t axis_لیبل_2,
				حرف *buf,
				غیرعلامتی سائز);

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
HB_EXTERN hb_blob_t *
hb_subset_داخلہ_مقصد_string_or_fail (hb_subset_داخلہ_t *داخلہ);

HB_EXTERN hb_bool_t
hb_subset_داخلہ_override_name_جدول (hb_subset_داخلہ_t  *داخلہ,
				     hb_ot_name_id_t     name_id,
				     غیرعلامتی            platform_id,
				     غیرعلامتی            encoding_id,
				     غیرعلامتی            language_id,
				     ثابت حرف         *name_str,
				     صحیح                 str_len);


/*
* Raw outline data access
*/

HB_EXTERN hb_blob_t*
hb_subset_cff_get_charstring_ڈیٹا (hb_face_t* face, hb_codepoint_t glyph);

HB_EXTERN hb_blob_t*
hb_subset_cff_get_charstrings_اشاریہ (hb_face_t* face);

HB_EXTERN hb_blob_t*
hb_subset_cff2_get_charstring_ڈیٹا (hb_face_t* face, hb_codepoint_t glyph);

HB_EXTERN hb_blob_t*
hb_subset_cff2_get_charstrings_اشاریہ (hb_face_t* face);
#اگر_ختم

HB_EXTERN hb_face_t *
hb_subset_preprocess (hb_face_t *ماخذ);

HB_EXTERN hb_face_t *
hb_subset_or_fail (hb_face_t *ماخذ, ثابت hb_subset_داخلہ_t *داخلہ);

HB_EXTERN hb_face_t *
hb_subset_plan_execute_or_fail (hb_subset_plan_t *plan);

HB_EXTERN hb_subset_plan_t *
hb_subset_plan_create_or_fail (hb_face_t                 *face,
                               ثابت hb_subset_داخلہ_t   *داخلہ);

HB_EXTERN خالی
hb_subset_plan_destroy (hb_subset_plan_t *plan);

HB_EXTERN hb_map_t *
hb_subset_plan_old_مقصد_new_glyph_mapping (ثابت hb_subset_plan_t *plan);

HB_EXTERN hb_map_t *
hb_subset_plan_new_مقصد_old_glyph_mapping (ثابت hb_subset_plan_t *plan);

HB_EXTERN hb_map_t *
hb_subset_plan_unicode_مقصد_old_glyph_mapping (ثابت hb_subset_plan_t *plan);


HB_EXTERN hb_subset_plan_t *
hb_subset_plan_reference (hb_subset_plan_t *plan);

HB_EXTERN hb_bool_t
hb_subset_plan_سیٹ_user_ڈیٹا (hb_subset_plan_t   *plan,
                              hb_user_ڈیٹا_کلید_t *کلید_2,
                              خالی               *ڈیٹا,
                              hb_destroy_func_t   destroy,
                              hb_bool_t	          replace);

HB_EXTERN خالی *
hb_subset_plan_get_user_ڈیٹا (ثابت hb_subset_plan_t *plan,
                              hb_user_ڈیٹا_کلید_t     *کلید_2);


HB_انتہا_DECLS


#اگر defined(__cplusplus) && defined(HB_CPLUSPLUS_HH)
namespace hb {
HB_DEFINE_VTABLE (subset_داخلہ, nullptr);
HB_DEFINE_VTABLE (subset_plan,  nullptr);
} // namespace hb
#اگر_ختم

#اگر_ختم /* HB_SUBSET_H */
