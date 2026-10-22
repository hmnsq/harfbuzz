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
 * Google Author(s): Garret Rieger, Rod Sheeter, Behdad Esfahbod
 */

#ضمن "hb-subset-instancer-solver.hh"
#ضمن "hb-subset.hh"
#ضمن "hb-set.hh"
#ضمن "hb-utf.hh"


hb_subset_ننوت_t::hb_subset_ننوت_t ()
{
  لكل (خپلکاری& سېټ : sets_iter ())
    سېټ = hb::shared_ptr<hb_سېټ_t> (hb_سېټ_create ());

  إذا (دننه_error ())
    أرجع;

  flags = HB_SUBSET_FLAGS_DEFAULT;

  hb_سېټ_جمع_کړه_range (sets.name_ids, 0, 6);
  hb_سېټ_جمع_کړه (sets.name_languages, 0x0409);

  hb_نښان_t default_drop_tables[] = {
    // Layout disabled by default
    HB_نښان ('m', 'o', 'r', 'x'),
    HB_نښان ('m', 'o', 'r', 't'),
    HB_نښان ('k', 'e', 'r', 'x'),
    HB_نښان ('k', 'e', 'r', 'n'),

    // Copied from fontTools:
    HB_نښان ('J', 'S', 'T', 'F'),
    HB_نښان ('D', 'S', 'I', 'G'),
    HB_نښان ('E', 'B', 'D', 'T'),
    HB_نښان ('E', 'B', 'L', 'C'),
    HB_نښان ('E', 'B', 'S', 'C'),
    HB_نښان ('S', 'V', 'G', ' '),
    HB_نښان ('P', 'C', 'L', 'T'),
    HB_نښان ('L', 'T', 'S', 'H'),
    // Graphite tables
    HB_نښان ('F', 'e', 'a', 't'),
    HB_نښان ('G', 'l', 'a', 't'),
    HB_نښان ('G', 'l', 'o', 'c'),
    HB_نښان ('S', 'i', 'l', 'f'),
    HB_نښان ('S', 'i', 'l', 'l'),
  };
  sets.drop_tables->جمع_کړه_array (default_drop_tables, ARRAY_اوږدتیا (default_drop_tables));

  hb_نښان_t default_no_subset_tables[] = {
    HB_نښان ('g', 'a', 's', 'p'),
    HB_نښان ('f', 'p', 'g', 'm'),
    HB_نښان ('p', 'r', 'e', 'p'),
    HB_نښان ('V', 'D', 'M', 'X'),
    HB_نښان ('D', 'S', 'I', 'G'),
  };
  sets.no_subset_tables->جمع_کړه_array (default_no_subset_tables,
					 ARRAY_اوږدتیا (default_no_subset_tables));

  //copied from _layout_features_groups in fonttools
  hb_نښان_t default_layout_features[] = {
    // default shaper
    // common
    HB_نښان ('r', 'v', 'r', 'n'),
    HB_نښان ('c', 'c', 'm', 'p'),
    HB_نښان ('l', 'i', 'g', 'a'),
    HB_نښان ('l', 'o', 'c', 'l'),
    HB_نښان ('m', 'a', 'r', 'k'),
    HB_نښان ('m', 'k', 'm', 'k'),
    HB_نښان ('r', 'l', 'i', 'g'),

    //fractions
    HB_نښان ('f', 'r', 'a', 'c'),
    HB_نښان ('n', 'u', 'm', 'r'),
    HB_نښان ('d', 'n', 'o', 'm'),

    //horizontal
    HB_نښان ('c', 'a', 'l', 't'),
    HB_نښان ('c', 'l', 'i', 'g'),
    HB_نښان ('c', 'u', 'r', 's'),
    HB_نښان ('k', 'e', 'r', 'n'),
    HB_نښان ('r', 'c', 'l', 't'),

    //vertical
    HB_نښان ('v', 'a', 'l', 't'),
    HB_نښان ('v', 'e', 'r', 't'),
    HB_نښان ('v', 'k', 'r', 'n'),
    HB_نښان ('v', 'p', 'a', 'l'),
    HB_نښان ('v', 'r', 't', '2'),

    //ltr
    HB_نښان ('l', 't', 'r', 'a'),
    HB_نښان ('l', 't', 'r', 'm'),

    //rtl
    HB_نښان ('r', 't', 'l', 'a'),
    HB_نښان ('r', 't', 'l', 'm'),

    //random
    HB_نښان ('r', 'a', 'n', 'd'),

    //justify
    HB_نښان ('j', 'a', 'l', 't'), // HarfBuzz doesn't use; others might

    //East Asian spacing
    HB_نښان ('c', 'h', 'w', 's'),
    HB_نښان ('v', 'c', 'h', 'w'),
    HB_نښان ('h', 'a', 'l', 't'),
    HB_نښان ('v', 'h', 'a', 'l'),
    HB_نښان ('p', 'a', 'l', 't'),

    //شخصي
    HB_نښان ('H', 'a', 'r', 'f'),
    HB_نښان ('H', 'A', 'R', 'F'),
    HB_نښان ('B', 'u', 'z', 'z'),
    HB_نښان ('B', 'U', 'Z', 'Z'),

    //shapers

    //arabic
    HB_نښان ('i', 'n', 'i', 't'),
    HB_نښان ('m', 'e', 'd', 'i'),
    HB_نښان ('f', 'i', 'n', 'a'),
    HB_نښان ('i', 's', 'o', 'l'),
    HB_نښان ('m', 'e', 'd', '2'),
    HB_نښان ('f', 'i', 'n', '2'),
    HB_نښان ('f', 'i', 'n', '3'),
    HB_نښان ('c', 's', 'w', 'h'),
    HB_نښان ('m', 's', 'e', 't'),
    HB_نښان ('s', 't', 'c', 'h'),

    //hangul
    HB_نښان ('l', 'j', 'm', 'o'),
    HB_نښان ('v', 'j', 'm', 'o'),
    HB_نښان ('t', 'j', 'm', 'o'),

    //tibetan
    HB_نښان ('a', 'b', 'v', 's'),
    HB_نښان ('b', 'l', 'w', 's'),
    HB_نښان ('a', 'b', 'v', 'm'),
    HB_نښان ('b', 'l', 'w', 'm'),

    //indic
    HB_نښان ('n', 'u', 'k', 't'),
    HB_نښان ('a', 'k', 'h', 'n'),
    HB_نښان ('r', 'p', 'h', 'f'),
    HB_نښان ('r', 'k', 'r', 'f'),
    HB_نښان ('p', 'r', 'e', 'f'),
    HB_نښان ('b', 'l', 'w', 'f'),
    HB_نښان ('h', 'a', 'l', 'f'),
    HB_نښان ('a', 'b', 'v', 'f'),
    HB_نښان ('p', 's', 't', 'f'),
    HB_نښان ('c', 'f', 'a', 'r'),
    HB_نښان ('v', 'a', 't', 'u'),
    HB_نښان ('c', 'j', 'c', 't'),
    HB_نښان ('i', 'n', 'i', 't'),
    HB_نښان ('p', 'r', 'e', 's'),
    HB_نښان ('a', 'b', 'v', 's'),
    HB_نښان ('b', 'l', 'w', 's'),
    HB_نښان ('p', 's', 't', 's'),
    HB_نښان ('h', 'a', 'l', 'n'),
    HB_نښان ('d', 'i', 's', 't'),
    HB_نښان ('a', 'b', 'v', 'm'),
    HB_نښان ('b', 'l', 'w', 'm'),
  };

  sets.layout_features->جمع_کړه_array (default_layout_features, ARRAY_اوږدتیا (default_layout_features));

  sets.layout_scripts->invert (); // Default to all scripts.
}

/**
 * hb_subset_input_create_or_fail:
 *
 * Creates a new subset input object.
 *
 * Return value: (transfer full): New subset input, or `NULL` if failed. Destroy
 * with hb_subset_input_destroy().
 *
 * Since: 1.8.0
 **/
hb_subset_ننوت_t *
hb_subset_ننوت_create_or_fail (فراغ)
{
  hb_subset_ننوت_t *ننوت = hb_object_create<hb_subset_ننوت_t>();

  إذا (unlikely (!ننوت))
    أرجع تش_اشاره;

  إذا (ننوت->دننه_error ())
  {
    hb_subset_ننوت_destroy (ننوت);
    أرجع تش_اشاره;
  }

  أرجع ننوت;
}

/**
 * hb_subset_input_reference: (skip)
 * @input: a #hb_subset_input_t object.
 *
 * Increases the reference count on @input.
 *
 * Return value: @input.
 *
 * Since: 1.8.0
 **/
hb_subset_ننوت_t *
hb_subset_ننوت_reference (hb_subset_ننوت_t *ننوت)
{
  أرجع hb_object_reference (ننوت);
}

/**
 * hb_subset_input_destroy:
 * @input: a #hb_subset_input_t object.
 *
 * Decreases the reference count on @input, and if it reaches zero, destroys
 * @input, freeing all memory.
 *
 * Since: 1.8.0
 **/
فراغ
hb_subset_ننوت_destroy (hb_subset_ننوت_t *ننوت)
{
  إذا (!hb_object_destroy (ننوت)) أرجع;

  hb_free (ننوت);
}

/**
 * hb_subset_input_unicode_set:
 * @input: a #hb_subset_input_t object.
 *
 * Gets the set of Unicode code points to retain, the caller should modify the
 * set as needed.
 *
 * Return value: (transfer none): pointer to the #hb_set_t of Unicode code
 * points.
 *
 * Since: 1.8.0
 **/
HB_EXTERN hb_سېټ_t *
hb_subset_ننوت_unicode_سېټ (hb_subset_ننوت_t *ننوت)
{
  أرجع ننوت->sets.unicodes;
}

/**
 * hb_subset_input_glyph_set:
 * @input: a #hb_subset_input_t object.
 *
 * Gets the set of glyph IDs to retain, the caller should modify the set as
 * needed.
 *
 * Return value: (transfer none): pointer to the #hb_set_t of glyph IDs.
 *
 * Since: 1.8.0
 **/
HB_EXTERN hb_سېټ_t *
hb_subset_ننوت_glyph_سېټ (hb_subset_ننوت_t *ننوت)
{
  أرجع ننوت->sets.glyphs;
}

/**
 * hb_subset_input_set:
 * @input: a #hb_subset_input_t object.
 * @set_type: a #hb_subset_sets_t set type.
 *
 * Gets the set of the specified type.
 *
 * Return value: (transfer none): pointer to the #hb_set_t of the specified type.
 *
 * Since: 2.9.1
 **/
HB_EXTERN hb_سېټ_t *
hb_subset_ننوت_سېټ (hb_subset_ننوت_t *ننوت, hb_subset_sets_t سېټ_type)
{
  أرجع ننوت->sets_iter () [سېټ_type];
}

/**
 * hb_subset_input_get_flags:
 * @input: a #hb_subset_input_t object.
 *
 * Gets all of the subsetting flags in the input object.
 *
 * Return value: the subsetting flags bit field.
 *
 * Since: 2.9.0
 **/
HB_EXTERN hb_subset_flags_t
hb_subset_ننوت_get_flags (hb_subset_ننوت_t *ننوت)
{
  أرجع (hb_subset_flags_t) ننوت->flags;
}

/**
 * hb_subset_input_set_flags:
 * @input: a #hb_subset_input_t object.
 * @value: bit field of flags
 *
 * Sets all of the flags in the input object to the values specified by the bit
 * field.
 *
 * Since: 2.9.0
 **/
HB_EXTERN فراغ
hb_subset_ننوت_سېټ_flags (hb_subset_ننوت_t *ننوت,
			   بینښه value)
{
  ننوت->flags = (hb_subset_flags_t) value;
}

/**
 * hb_subset_input_set_user_data: (skip)
 * @input: a #hb_subset_input_t object.
 * @key: The user-data key to set
 * @data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the given subset input object.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 2.9.0
 **/
hb_bool_t
hb_subset_ننوت_سېټ_user_معلومات (hb_subset_ننوت_t  *ننوت,
			       hb_user_معلومات_کلی_t *key,
			       فراغ *		   معلومات,
			       hb_destroy_func_t   destroy,
			       hb_bool_t	   replace)
{
  أرجع hb_object_سېټ_user_معلومات (ننوت, key, معلومات, destroy, replace);
}

/**
 * hb_subset_input_get_user_data: (skip)
 * @input: a #hb_subset_input_t object.
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified subset input object.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 2.9.0
 **/
فراغ *
hb_subset_ننوت_get_user_معلومات (ثابت hb_subset_ننوت_t *ننوت,
			       hb_user_معلومات_کلی_t     *key)
{
  أرجع hb_object_get_user_معلومات (ننوت, key);
}

/**
 * hb_subset_input_keep_everything:
 * @input: a #hb_subset_input_t object
 *
 * Configure input object to keep everything in the font face.
 * That is, all Unicodes, glyphs, names, layout items,
 * glyph names, etc.
 *
 * The input can be tailored afterwards by the caller.
 *
 * Since: 7.0.0
 */
فراغ
hb_subset_ننوت_keep_everything (hb_subset_ننوت_t *ننوت)
{
  ثابت hb_subset_sets_t indices[] = {HB_SUBSET_SETS_UNICODE,
				      HB_SUBSET_SETS_GLYPH_شاخص,
				      HB_SUBSET_SETS_NAME_ID,
				      HB_SUBSET_SETS_NAME_LANG_ID,
				      HB_SUBSET_SETS_LAYOUT_FEATURE_نښان,
				      HB_SUBSET_SETS_LAYOUT_SCRIPT_نښان};

  لكل (خپلکاری idx : hb_iter (indices))
  {
    hb_سېټ_t *سېټ = hb_subset_ننوت_سېټ (ننوت, idx);
    hb_سېټ_clear (سېټ);
    hb_سېټ_invert (سېټ);
  }

  // Don't drop any tables
  hb_سېټ_clear (hb_subset_ننوت_سېټ (ننوت, HB_SUBSET_SETS_DROP_جدول_نښان));

  hb_subset_ننوت_سېټ_flags (ننوت,
			     HB_SUBSET_FLAGS_NOTDEF_OUTLINE |
			     HB_SUBSET_FLAGS_GLYPH_نومونه |
			     HB_SUBSET_FLAGS_NAME_LEGACY |
			     HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES |
                             HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED);
}

#که_نه_تعریف_شوی HB_NO_VAR
/**
 * hb_subset_input_pin_all_axes_to_default: (skip)
 * @input: a #hb_subset_input_t object.
 * @face: a #hb_face_t object.
 *
 * Pin all axes to default locations in the given subset input object.
 *
 * The `CFF2` table, if present, will be de-subroutinized.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 8.3.1
 **/
HB_EXTERN hb_bool_t
hb_subset_ننوت_pin_all_axes_منزل_default (hb_subset_ننوت_t  *ننوت,
                                         hb_face_t          *face)
{
  بینښه axis_شمېر = hb_ot_var_get_axis_شمېر (face);
  إذا (!axis_شمېر) أرجع خطأ;

  hb_ot_var_axis_info_t *axis_infos = (hb_ot_var_axis_info_t *) hb_calloc (axis_شمېر, حجم (hb_ot_var_axis_info_t));
  إذا (unlikely (!axis_infos)) أرجع خطأ;

  (فراغ) hb_ot_var_get_axis_infos (face, 0, &axis_شمېر, axis_infos);

  لكل (بینښه i = 0; i < axis_شمېر; i++)
  {
    hb_نښان_t axis_نښان_2 = axis_infos[i].نښان_2;
    مزدوج default_val = (مزدوج) axis_infos[i].default_ارزښت;
    إذا (!ننوت->axes_location.سېټ (axis_نښان_2, Triple (default_val, default_val, default_val)))
    {
      hb_free (axis_infos);
      أرجع خطأ;
    }
  }
  hb_free (axis_infos);
  أرجع ریښتیا;
}

/**
 * hb_subset_input_pin_axis_to_default: (skip)
 * @input: a #hb_subset_input_t object.
 * @face: a #hb_face_t object.
 * @axis_tag: Tag of the axis to be pinned
 *
 * Pin an axis to its default location in the given subset input object.
 *
 * The `CFF2` table, if present, will be de-subroutinized.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 6.0.0
 **/
HB_EXTERN hb_bool_t
hb_subset_ننوت_pin_axis_منزل_default (hb_subset_ننوت_t  *ننوت,
                                     hb_face_t          *face,
                                     hb_نښان_t            axis_نښان_2)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_پیدا_کړه_axis_info (face, axis_نښان_2, &axis_info))
    أرجع خطأ;

  مزدوج default_val = (مزدوج) axis_info.default_ارزښت;
  أرجع ننوت->axes_location.سېټ (axis_نښان_2, Triple (default_val, default_val, default_val));
}

/**
 * hb_subset_input_pin_axis_location: (skip)
 * @input: a #hb_subset_input_t object.
 * @face: a #hb_face_t object.
 * @axis_tag: Tag of the axis to be pinned
 * @axis_value: Location on the axis to be pinned at
 *
 * Pin an axis to a fixed location in the given subset input object.
 *
 * The `CFF2` table, if present, will be de-subroutinized.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 6.0.0
 **/
HB_EXTERN hb_bool_t
hb_subset_ننوت_pin_axis_location (hb_subset_ننوت_t  *ننوت,
                                   hb_face_t          *face,
                                   hb_نښان_t            axis_نښان_2,
                                   عائم               axis_ارزښت)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_پیدا_کړه_axis_info (face, axis_نښان_2, &axis_info))
    أرجع خطأ;

  مزدوج val = hb_clamp((مزدوج) axis_ارزښت, (مزدوج) axis_info.min_ارزښت, (مزدوج) axis_info.max_ارزښت);
  أرجع ننوت->axes_location.سېټ (axis_نښان_2, Triple (val, val, val));
}

/**
 * hb_subset_input_set_axis_range: (skip)
 * @input: a #hb_subset_input_t object.
 * @face: a #hb_face_t object.
 * @axis_tag: Tag of the axis
 * @axis_min_value: Minimum value of the axis variation range to set, if NaN the existing min will be used.
 * @axis_max_value: Maximum value of the axis variation range to set  if NaN the existing max will be used.
 * @axis_def_value: Default value of the axis variation range to set, if NaN the existing default will be used.
 *
 * Restricting the range of variation on an axis in the given subset input object.
 * New min/default/max values will be clamped if they're not within the fvar axis range.
 *
 * If the fvar axis default value is not within the new range, the new default
 * value will be changed to the new min or max value, whichever is closer to the fvar
 * axis default.
 *
 * Note: input min value can not be bigger than input max value. If the input
 * default value is not within the new min/max range, it'll be clamped.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 8.5.0
 **/
HB_EXTERN hb_bool_t
hb_subset_ننوت_سېټ_axis_range (hb_subset_ننوت_t  *ننوت,
                                hb_face_t          *face,
                                hb_نښان_t            axis_نښان_2,
                                عائم               axis_min_ارزښت,
                                عائم               axis_max_ارزښت,
                                عائم               axis_def_ارزښت)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_پیدا_کړه_axis_info (face, axis_نښان_2, &axis_info))
    أرجع خطأ;

  عائم min = !std::isnan(axis_min_ارزښت) ? axis_min_ارزښت : axis_info.min_ارزښت;
  عائم max = !std::isnan(axis_max_ارزښت) ? axis_max_ارزښت : axis_info.max_ارزښت;
  عائم def = !std::isnan(axis_def_ارزښت) ? axis_def_ارزښت : axis_info.default_ارزښت;

  إذا (min > max)
    أرجع خطأ;

  عائم new_min_val = hb_clamp(min, axis_info.min_ارزښت, axis_info.max_ارزښت);
  عائم new_max_val = hb_clamp(max, axis_info.min_ارزښت, axis_info.max_ارزښت);
  عائم new_default_val = hb_clamp(def, new_min_val, new_max_val);
  أرجع ننوت->axes_location.سېټ (axis_نښان_2, Triple ((مزدوج) new_min_val, (مزدوج) new_default_val, (مزدوج) new_max_val));
}

/**
 * hb_subset_input_get_axis_range: (skip)
 * @input: a #hb_subset_input_t object.
 * @axis_tag: Tag of the axis
 * @axis_min_value: Set to the previously configured minimum value of the axis variation range.
 * @axis_max_value: Set to the previously configured maximum value of the axis variation range.
 * @axis_def_value: Set to the previously configured default value of the axis variation range.
 *
 * Gets the axis range assigned by previous calls to hb_subset_input_set_axis_range.
 *
 * Return value: `true` if a range has been set for this axis tag, `false` otherwise.
 *
 * Since: 8.5.0
 **/
HB_EXTERN hb_bool_t
hb_subset_ننوت_get_axis_range (hb_subset_ننوت_t  *ننوت,
				hb_نښان_t            axis_نښان_2,
				عائم              *axis_min_ارزښت,
				عائم              *axis_max_ارزښت,
				عائم              *axis_def_ارزښت)

{
  Triple* triple;
  إذا (!ننوت->axes_location.has(axis_نښان_2, &triple)) {
    أرجع خطأ;
  }

  *axis_min_ارزښت = triple->minimum;
  *axis_def_ارزښت = triple->منځ;
  *axis_max_ارزښت = triple->اعظمي;
  أرجع ریښتیا;
}

/**
 * hb_subset_axis_range_from_string:
 * @str: a string to parse
 * @len: length of @str, or -1 if str is NULL terminated
 * @axis_min_value: (out): the axis min value to initialize with the parsed value
 * @axis_max_value: (out): the axis max value to initialize with the parsed value
 * @axis_def_value: (out): the axis default value to initialize with the parse
 * value
 *
 * Parses a string into a subset axis range(min, def, max).
 * Axis positions string is in the format of min:def:max or min:max
 * When parsing axis positions, empty values as meaning the existing value for that part
 * E.g: :300:500
 * Specifies min = existing, def = 300, max = 500
 * In the output axis_range, if a value should be set to it's default value,
 * then it will be set to NaN
 *
 * Return value:
 * `true` if @str is successfully parsed, `false` otherwise
 *
 * Since: 10.2.0
 */
HB_EXTERN hb_bool_t
hb_subset_axis_range_from_string (ثابت حرف *str, صحيح len,
                                  عائم *axis_min_ارزښت,
                                  عائم *axis_max_ارزښت,
                                  عائم *axis_def_ارزښت)
{
  إذا (len < 0)
    len = مزي_اوږدوالی (str);

  ثابت حرف *پای_ټکی = str + len;
  ثابت حرف* part = strpbrk (str, ":");
  إذا (!part)
  {
    // Single value.
    إذا (strcmp (str, "drop") == 0)
    {
      *axis_min_ارزښت = NAN;
      *axis_def_ارزښت = NAN;
      *axis_max_ارزښت = NAN;
      أرجع ریښتیا;
    }

    مزدوج v;
    إذا (!hb_parse_double (&str, پای_ټکی, &v)) أرجع خطأ;

    *axis_min_ارزښت = v;
    *axis_def_ارزښت = v;
    *axis_max_ارزښت = v;
    أرجع ریښتیا;
  }

  عائم values[3];
  صحيح شمېر = 0;
  لكل (صحيح i = 0; i < 3; i++) {
    شمېر++;
    إذا (!*str || part == str)
    {
      values[i] = NAN;

      إذا (part == عدم) اكسر;
      str = part + 1;
      part = strpbrk (str, ":");
      استمر;
    }

    مزدوج v;
    ثابت حرف *piece_پای_ټکی = part ? part : پای_ټکی;
    إذا (!hb_parse_double (&str, piece_پای_ټکی, &v)) أرجع خطأ;
    values[i] = v;

    إذا (part == عدم) اكسر;
    str = part + 1;
    part = strpbrk (str, ":");
  }

  إذا (شمېر == 2)
  {
    *axis_min_ارزښت = values[0];
    *axis_def_ارزښت = NAN;
    *axis_max_ارزښت = values[1];
    أرجع ریښتیا;
  }
  وإلا إذا (شمېر == 3)
  {
    *axis_min_ارزښت = values[0];
    *axis_def_ارزښت = values[1];
    *axis_max_ارزښت = values[2];
    أرجع ریښتیا;
  }
  أرجع خطأ;
}

/**
 * hb_subset_axis_range_to_string:
 * @input: a #hb_subset_input_t object.
 * @axis_tag: an axis to convert
 * @buf: (array length=size) (out caller-allocates): output string
 * @size: the allocated size of @buf
 *
 * Converts an axis range into a `NULL`-terminated string in the format
 * understood by hb_subset_axis_range_from_string(). The client in responsible for
 * allocating big enough size for @buf, 128 bytes is more than enough.
 *
 * Since: 10.2.0
 */
HB_EXTERN فراغ
hb_subset_axis_range_منزل_string (hb_subset_ننوت_t *ننوت,
                                hb_نښان_t axis_نښان_2,
                                حرف *buf, بینښه اندازه)
{
  إذا (unlikely (!اندازه)) أرجع;
  Triple* triple;
  إذا (!ننوت->axes_location.has(axis_نښان_2, &triple)) {
    أرجع;
  }

  حرف s[128];
  بینښه len = 0;

  hb_locale_t clocale HB_ناکارول_شوی;
  hb_locale_t oldlocale HB_ناکارول_شوی;
  oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", عدم));
  len += hb_max (0, snprintf (s, ARRAY_اوږدتیا (s) - len, "%g", (مزدوج) triple->minimum));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_اوږدتیا (s) - len, "%g", (مزدوج) triple->منځ));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_اوږدتیا (s) - len, "%g", (مزدوج) triple->اعظمي));
  (فراغ) hb_uselocale (((فراغ) freelocale (clocale), oldlocale));

  أكد (len < ARRAY_اوږدتیا (s));
  len = hb_min (len, اندازه - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}
#د_که_پای

#که_تعریف_شوی HB_EXPERIMENTAL_API
ساكن فراغ
_append_arg (hb_ویکټور_t<حرف> &sb, ثابت حرف *arg)
{
  إذا (sb.دننه_error ()) أرجع;
  إذا (sb.اوږدتیا)
    sb.push (' ');
  بینښه len = مزي_اوږدوالی (arg);
  sb.extend (hb_بایټونه_t (arg, len));
}
ساكن فراغ
format_شمېره_سېټ (ثابت hb_سېټ_t *سېټ, hb_ویکټور_t<حرف> &بهر, بینښه بنسټ = 10)
{
  أكد (بنسټ == 10 || بنسټ == 16);
  إذا (unlikely (بنسټ != 10 && بنسټ != 16)) أرجع;

  hb_codepoint_t لومړی = HB_سېټ_ارزښت_INVALID, وروستی = HB_سېټ_ارزښت_INVALID;
  منطقي لومړی_range = ریښتیا;
  طالما (hb_سېټ_بل_range (سېټ, &لومړی, &وروستی))
  {
    إذا (!لومړی_range)
      بهر.push (',');
    لومړی_range = خطأ;

    حرف buf[64];
    صحيح len;
    إذا (لومړی == وروستی)
    {
      إذا (بنسټ == 16)
        len = snprintf (buf, حجم (buf), "%X", لومړی);
      وإلا
        len = snprintf (buf, حجم (buf), "%u", لومړی);
    }
    وإلا
    {
      إذا (بنسټ == 16)
        len = snprintf (buf, حجم (buf), "%X-%X", لومړی, وروستی);
      وإلا
        len = snprintf (buf, حجم (buf), "%u-%u", لومړی, وروستی);
    }
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع;
    بهر.extend (hb_بایټونه_t (buf, len));
  }
}

ساكن فراغ
format_نښان (hb_نښان_t نښان_2, hb_ویکټور_t<حرف> &بهر)
{
  حرف buf[4];
  hb_نښان_منزل_string (نښان_2, buf);

  بینښه len = 4;
  طالما (len > 0 && buf[len - 1] == ' ')
    len--;

  لكل (بینښه i = 0; i < len; i++)
  {
    حرف c = buf[i];
    إذا (!((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '-' || c == '_'))
    {
      c = '_';
    }
    بهر.push (c);
  }
}

ساكن فراغ
format_نښان_سېټ (ثابت hb_سېټ_t *سېټ, hb_ویکټور_t<حرف> &بهر)
{
  منطقي لومړی_نښان = ریښتیا;
  لكل (hb_codepoint_t نښان_2 : *سېټ)
  {
    إذا (!لومړی_نښان)
      بهر.push (',');
    لومړی_نښان = خطأ;

    format_نښان (نښان_2, بهر);
  }
}

ساكن منطقي
is_سېټ_default (ثابت hb_سېټ_t *سېټ, ثابت hb_سېټ_t *default_سېټ)
{
  أرجع hb_سېټ_is_inverted (سېټ) == hb_سېټ_is_inverted (default_سېټ) &&
         hb_سېټ_is_equal (سېټ, default_سېټ);
}

کينډۍ <ډول_نوم Formatter>
ساكن فراغ
format_سېټ_arg (ثابت hb_سېټ_t *سېټ,
                 ثابت حرف *opt_name,
                 Formatter &&formatter,
                 hb_ویکټور_t<حرف> &sb)
{
  إذا (hb_سېټ_is_inverted (سېټ))
  {
    hb_سېټ_t *excluded = hb_سېټ_copy (سېټ);
    hb_سېټ_invert (excluded);

    hb_ویکټور_t<حرف> opt;
    opt.extend (hb_بایټونه_t ("--", 2));
    opt.extend (hb_بایټونه_t (opt_name, مزي_اوږدوالی (opt_name)));
    opt.extend (hb_بایټونه_t ("=*", 2));
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);

    إذا (!hb_سېټ_is_تشه (excluded))
    {
      hb_ویکټور_t<حرف> formatted;
      formatter (excluded, formatted);
      إذا (formatted.اوږدتیا)
      {
        hb_ویکټور_t<حرف> opt2;
        opt2.extend (hb_بایټونه_t ("--", 2));
        opt2.extend (hb_بایټونه_t (opt_name, مزي_اوږدوالی (opt_name)));
        opt2.extend (hb_بایټونه_t ("-=", 2));
        opt2.extend (formatted.as_array ());
        opt2.push ('\0');
        إذا (opt2.arrayZ) _append_arg (sb, opt2.arrayZ);
      }
    }
    hb_سېټ_destroy (excluded);
  }
  وإلا إذا (hb_سېټ_is_تشه (سېټ))
  {
    hb_ویکټور_t<حرف> opt;
    opt.extend (hb_بایټونه_t ("--", 2));
    opt.extend (hb_بایټونه_t (opt_name, مزي_اوږدوالی (opt_name)));
    opt.extend (hb_بایټونه_t ("-=*", 3));
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  وإلا
  {
    hb_ویکټور_t<حرف> formatted;
    formatter (سېټ, formatted);
    إذا (formatted.اوږدتیا)
    {
      hb_ویکټور_t<حرف> opt;
      opt.extend (hb_بایټونه_t ("--", 2));
      opt.extend (hb_بایټونه_t (opt_name, مزي_اوږدوالی (opt_name)));
      opt.extend (hb_بایټونه_t ("=", 1));
      opt.extend (formatted.as_array ());
      opt.push ('\0');
      إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
    }
  }
}

ساكن فراغ
format_شمېره_سېټ_arg (ثابت hb_سېټ_t *سېټ,
                        ثابت حرف *opt_name,
                        hb_ویکټور_t<حرف> &sb,
                        بینښه بنسټ = 10)
{
  format_سېټ_arg (سېټ, opt_name, [بنسټ] (ثابت hb_سېټ_t *s, hb_ویکټور_t<حرف> &بهر) {
    format_شمېره_سېټ (s, بهر, بنسټ);
  }, sb);
}

ساكن فراغ
format_نښان_سېټ_arg (ثابت hb_سېټ_t *سېټ,
                     ثابت حرف *opt_name,
                     hb_ویکټور_t<حرف> &sb)
{
  format_سېټ_arg (سېټ, opt_name, format_نښان_سېټ, sb);
}

ساكن منطقي
_format_glyph_map_arg (ثابت hb_map_t &glyph_map,
                       hb_ویکټور_t<حرف> &sb)
{
  إذا (glyph_map.is_empty ())
    أرجع ریښتیا;

  hb_سېټ_t کلیګانې;
  hb_map_کلیګانې (&glyph_map, &کلیګانې);
  hb_ویکټور_t<حرف> جوړې;
  منطقي لومړی = ریښتیا;
  لكل (hb_codepoint_t key : کلیګانې)
  {
    إذا (!لومړی)
      جوړې.push (',');
    لومړی = خطأ;
    hb_codepoint_t val = hb_map_get (&glyph_map, key);
    حرف buf[64];
    صحيح len = snprintf (buf, حجم (buf), "%u:%u", key, val);
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع خطأ;
    جوړې.extend (hb_بایټونه_t (buf, len));
  }
  إذا (جوړې.اوږدتیا)
  {
    hb_ویکټور_t<حرف> opt;
    opt.extend (hb_بایټونه_t ("--gid-map=", 10));
    opt.extend (جوړې.as_array ());
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  أرجع ریښتیا;
}

ساكن منطقي
_format_axes_location_arg (ثابت hb_hashmap_t<hb_نښان_t, Triple> &axes_location,
                           hb_ویکټور_t<حرف> &sb)
{
  إذا (axes_location.is_empty ())
    أرجع ریښتیا;

  hb_ویکټور_t<hb_نښان_t> axis_نښانونه;
  لكل (خپلکاری نښان_2 : axes_location.کلیګانې ())
    axis_نښانونه.push (نښان_2);
  axis_نښانونه.qsort ([] (ثابت hb_نښان_t &a, ثابت hb_نښان_t &b) { أرجع a < b ? -1 : a > b ? 1 : 0; });

  hb_ویکټور_t<حرف> specs;
  منطقي لومړی = ریښتیا;
  لكل (hb_نښان_t نښان_2 : axis_نښانونه)
  {
    Triple triple = axes_location.get (نښان_2);
    إذا (!لومړی)
      specs.push (',');
    لومړی = خطأ;

    hb_ویکټور_t<حرف> نښان_buf;
    format_نښان (نښان_2, نښان_buf);

    حرف buf[128];
    صحيح len;
    إذا (triple.minimum == triple.منځ && triple.منځ == triple.اعظمي)
    {
      len = snprintf (buf, حجم (buf), "%.*s=%g", (صحيح) نښان_buf.اوږدتیا, نښان_buf.arrayZ, (مزدوج) triple.منځ);
    }
    وإلا إذا (std::isnan (triple.minimum) && std::isnan (triple.منځ) && std::isnan (triple.اعظمي))
    {
      len = snprintf (buf, حجم (buf), "%.*s=drop", (صحيح) نښان_buf.اوږدتیا, نښان_buf.arrayZ);
    }
    وإلا
    {
      len = snprintf (buf, حجم (buf), "%.*s=%g:%g:%g", (صحيح) نښان_buf.اوږدتیا, نښان_buf.arrayZ,
                      (مزدوج) triple.minimum, (مزدوج) triple.منځ, (مزدوج) triple.اعظمي);
    }
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع خطأ;
    specs.extend (hb_بایټونه_t (buf, len));
  }
  إذا (specs.اوږدتیا)
  {
    hb_ویکټور_t<حرف> opt;
    opt.extend (hb_بایټونه_t ("--variations=", 13));
    opt.extend (specs.as_array ());
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  أرجع ریښتیا;
}

/**
 * hb_subset_input_to_string_or_fail:
 * @input: a #hb_subset_input_t object.
 *
 * Produces a command line string representation of the given subset input
 * suitable for use with the `hb-subset` command line tool.
 *
 * Return value: (transfer full): A new #hb_blob_t containing the command line
 * string, or `NULL` if failed. Destroy with hb_blob_destroy().
 *
 * XSince: EXPERIMENTAL
 **/
hb_blob_t *
hb_subset_ننوت_منزل_string_or_fail (hb_subset_ننوت_t *ننوت)
{
  إذا (unlikely (!ننوت || ننوت->دننه_error ()))
    أرجع تش_اشاره;

  hb_ویکټور_t<حرف> sb;

  بنية flag_option_t {
    hb_subset_flags_t flag;
    حرف option[32];
  };
  ساكن ثابت flag_option_t flag_options[] = {
    { HB_SUBSET_FLAGS_NO_HINTING, "no-hinting" },
    { HB_SUBSET_FLAGS_RETAIN_GIDS, "retain-gids" },
    { HB_SUBSET_FLAGS_DESUBROUTINIZE, "desubroutinize" },
    { HB_SUBSET_FLAGS_NAME_LEGACY, "name-legacy" },
    { HB_SUBSET_FLAGS_سېټ_OVERLAPS_FLAG, "set-overlaps-flag" },
    { HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED, "passthrough-tables" },
    { HB_SUBSET_FLAGS_NOTDEF_OUTLINE, "notdef-outline" },
    { HB_SUBSET_FLAGS_GLYPH_نومونه, "glyph-names" },
    { HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES, "no-prune-unicode-ranges" },
    { HB_SUBSET_FLAGS_NO_LAYOUT_تړون, "no-layout-closure" },
    { HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS, "optimize" },
    { HB_SUBSET_FLAGS_NO_BIDI_تړون, "no-bidi-closure" },
#که_تعریف_شوی HB_EXPERIMENTAL_API
    { HB_SUBSET_FLAGS_IFTB_REQUIREMENTS, "iftb-requirements" },
    { HB_SUBSET_FLAGS_RETAIN_عدد_GLYPHS, "retain-num-glyphs" },
#د_که_پای
    { HB_SUBSET_FLAGS_DOWNGRADE_CFF2, "downgrade-cff2" },
  };

#که_تعریف_شوی HB_EXPERIMENTAL_API
  static_assert (حجم (flag_options) / حجم (flag_options[0]) == 15,
                 "Check all flags in hb_subset_flags_t are handled here.");
#وإلا
  static_assert (حجم (flag_options) / حجم (flag_options[0]) == 13,
                 "Check all flags in hb_subset_flags_t are handled here.");
#د_که_پای

  لكل (ثابت خپلکاری &fo : flag_options)
  {
    إذا (ننوت->flags & fo.flag)
    {
      حرف buf[36];
      snprintf (buf, حجم (buf), "--%s", fo.option);
      _append_arg (sb, buf);
    }
  }

  hb_subset_ننوت_t *default_ننوت = hb_subset_ننوت_create_or_fail ();
  إذا (unlikely (!default_ننوت))
    أرجع تش_اشاره;

  // Serialize all sets if they are not equal to their default setting:
#تعریف_کړه PROCESS_شمېره_سېټ(ډګر, opt_name, ...) \
  إذا (!is_سېټ_default (ننوت->sets.ډګر, default_ننوت->sets.ډګر)) \
    format_شمېره_سېټ_arg (ننوت->sets.ډګر, opt_name, sb, ##__VA_ARGS__)

#تعریف_کړه PROCESS_نښان_سېټ(ډګر, opt_name) \
  إذا (!is_سېټ_default (ننوت->sets.ډګر, default_ننوت->sets.ډګر)) \
    format_نښان_سېټ_arg (ننوت->sets.ډګر, opt_name, sb)

  PROCESS_شمېره_سېټ (unicodes, "unicodes", 16);
  PROCESS_شمېره_سېټ (glyphs, "gids");
  PROCESS_شمېره_سېټ (name_ids, "name-IDs");
  PROCESS_شمېره_سېټ (name_languages, "name-languages");
  PROCESS_نښان_سېټ (layout_features, "layout-features");
  PROCESS_نښان_سېټ (layout_scripts, "layout-scripts");
  PROCESS_نښان_سېټ (drop_tables, "drop-tables");
  PROCESS_نښان_سېټ (no_subset_tables, "passthrough-tables");

#تعریف_لغوه PROCESS_شمېره_سېټ
#تعریف_لغوه PROCESS_نښان_سېټ

  hb_subset_ننوت_destroy (default_ننوت);

  static_assert (حجم (ننوت->sets) / حجم (hb_سېټ_t*) == 8,
                 "Check all sets in hb_subset_input_t::sets_t are handled here.");

  إذا (unlikely (!_format_glyph_map_arg (ننوت->glyph_map, sb)))
    أرجع تش_اشاره;

  إذا (unlikely (!_format_axes_location_arg (ننوت->axes_location, sb)))
    أرجع تش_اشاره;

  sb.push ('\0');
  إذا (unlikely (sb.دننه_error ()))
    أرجع تش_اشاره;

  بینښه len = 0;
  حرف *buf = sb.steal (&len);
  إذا (unlikely (!buf))
    أرجع تش_اشاره;

  أرجع hb_blob_create_or_fail (buf, len, HB_حافظه_ډېرپېښ_WRITABLE, buf, hb_free);
}
#د_که_پای

/**
 * hb_subset_preprocess:
 * @source: a #hb_face_t object.
 *
 * Preprocesses the face and attaches data that will be needed by the
 * subsetter. Future subsetting operations can then use the precomputed data
 * to speed up the subsetting operation.
 *
 * See [subset-preprocessing](https://github.com/harfbuzz/harfbuzz/blob/main/docs/subset-preprocessing.md)
 * for more information.
 *
 * Note: the preprocessed face may contain sub-blobs that reference the memory
 * backing the source #hb_face_t. Therefore in the case that this memory is not
 * owned by the source face you will need to ensure that memory lives
 * as long as the returned #hb_face_t.
 *
 * Returns: a new #hb_face_t.
 *
 * Since: 6.0.0
 **/

HB_EXTERN hb_face_t *
hb_subset_preprocess (hb_face_t *سرچینه)
{
  hb_subset_ننوت_t* ننوت = hb_subset_ننوت_create_or_fail ();
  إذا (!ننوت)
    أرجع hb_face_reference (سرچینه);

  hb_subset_ننوت_keep_everything (ننوت);

  ننوت->attach_accelerator_معلومات = ریښتیا;

  // Always use long loca in the preprocessed version. This allows
  // us to store the glyph bytes unpadded which allows the future subset
  // operation to run faster by skipping the trim padding step.
  ننوت->force_long_loca = ریښتیا;

  hb_face_t* new_سرچینه = hb_subset_or_fail (سرچینه, ننوت);
  hb_subset_ننوت_destroy (ننوت);

  إذا (!new_سرچینه) {
    DEBUG_پیغام (SUBSET, تش_اشاره, "Preprocessing failed due to subset failure.");
    أرجع hb_face_reference (سرچینه);
  }

  أرجع new_سرچینه;
}

/**
 * hb_subset_input_old_to_new_glyph_mapping:
 * @input: a #hb_subset_input_t object.
 *
 * Returns a map which can be used to provide an explicit mapping from old to new glyph
 * id's in the produced subset. The caller should populate the map as desired.
 * If this map is left empty then glyph ids will be automatically mapped to new
 * values by the subsetter. If populated, the mapping must be unique. That
 * is no two original glyph ids can be mapped to the same new id.
 * Additionally, if a mapping is provided then the retain gids option cannot
 * be enabled.
 *
 * Any glyphs that are retained in the subset which are not specified
 * in this mapping will be assigned glyph ids after the highest glyph
 * id in the mapping.
 *
 * Note: this will accept and apply non-monotonic mappings, however this
 * may result in unsorted Coverage tables. Such fonts may not work for all
 * use cases (for example ots will reject unsorted coverage tables). So it's
 * recommended, if possible, to supply a monotonic mapping.
 *
 * Return value: (transfer none): pointer to the #hb_map_t of the custom glyphs ID map.
 *
 * Since: 7.3.0
 **/
HB_EXTERN hb_map_t*
hb_subset_ننوت_old_منزل_new_glyph_mapping (hb_subset_ننوت_t *ننوت)
{
  أرجع &ننوت->glyph_map;
}

#که_تعریف_شوی HB_EXPERIMENTAL_API
/**
 * hb_subset_input_override_name_table:
 * @input: a #hb_subset_input_t object.
 * @name_id: name_id of a nameRecord
 * @platform_id: platform ID of a nameRecord
 * @encoding_id: encoding ID of a nameRecord
 * @language_id: language ID of a nameRecord
 * @name_str: pointer to name string new value or null to indicate should remove
 * @str_len: the size of @name_str, or -1 if it is `NULL`-terminated
 *
 * Override the name string of the NameRecord identified by name_id,
 * platform_id, encoding_id and language_id. If a record with that name_id
 * doesn't exist, create it and insert to the name table.
 *
 * Note: for mac platform, we only support name_str with all ascii characters,
 * name_str with non-ascii characters will be ignored.
 *
 * XSince: EXPERIMENTAL
 **/
HB_EXTERN hb_bool_t
hb_subset_ننوت_override_name_جدول (hb_subset_ننوت_t  *ننوت,
                                     hb_ot_name_id_t     name_id,
                                     بینښه            platform_id,
                                     بینښه            encoding_id,
                                     بینښه            language_id,
                                     ثابت حرف         *name_str,
                                     صحيح                 str_len /* -1 means nul-terminated */)
{
  إذا (!name_str)
  {
    str_len = 0;
  }
  وإلا إذا (str_len == -1)
  {
      str_len = مزي_اوږدوالی (name_str);
  }

  hb_بایټونه_t name_بایټونه (تش_اشاره, 0);
  إذا (str_len)
  {
    إذا (platform_id == 1)
    {
      ثابت uint8_t *src = reinterpret_cast<ثابت uint8_t*> (name_str);
      ثابت uint8_t *src_پای_ټکی = src + str_len;

      hb_codepoint_t unicode;
      ثابت hb_codepoint_t replacement = HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT;
      طالما (src < src_پای_ټکی)
      {
        src = hb_utf8_t::next (src, src_پای_ټکی, &unicode, replacement);
        إذا (unicode >= 0x0080u)
        {
          // Non-ascii character detected, ignored...
          أرجع خطأ;
        }
      }
    }
    حرف *override_name = (حرف *) hb_malloc (str_len);
    إذا (unlikely (!override_name)) أرجع خطأ;

    hb_memcpy (override_name, name_str, str_len);
    name_بایټونه = hb_بایټونه_t (override_name, str_len);
  }
  ننوت->name_جدول_overrides.سېټ (hb_ot_name_ریکارډ_ids_t (platform_id, encoding_id, language_id, name_id), name_بایټونه);
  أرجع ریښتیا;
}
#د_که_پای
