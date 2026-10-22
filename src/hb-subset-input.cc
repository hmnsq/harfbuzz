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


hb_subset_داخلہ_t::hb_subset_داخلہ_t ()
{
  لكل (خودکار& سیٹ : sets_iter ())
    سیٹ = hb::shared_ptr<hb_سیٹ_t> (hb_سیٹ_create ());

  إذا (وچ_error ())
    أرجع;

  flags = HB_SUBSET_FLAGS_DEFAULT;

  hb_سیٹ_جوڑو_range (sets.name_ids, 0, 6);
  hb_سیٹ_جوڑو (sets.name_languages, 0x0409);

  hb_ٹیگ_t default_drop_tables[] = {
    // Layout disabled by default
    HB_ٹیگ ('m', 'o', 'r', 'x'),
    HB_ٹیگ ('m', 'o', 'r', 't'),
    HB_ٹیگ ('k', 'e', 'r', 'x'),
    HB_ٹیگ ('k', 'e', 'r', 'n'),

    // Copied from fontTools:
    HB_ٹیگ ('J', 'S', 'T', 'F'),
    HB_ٹیگ ('D', 'S', 'I', 'G'),
    HB_ٹیگ ('E', 'B', 'D', 'T'),
    HB_ٹیگ ('E', 'B', 'L', 'C'),
    HB_ٹیگ ('E', 'B', 'S', 'C'),
    HB_ٹیگ ('S', 'V', 'G', ' '),
    HB_ٹیگ ('P', 'C', 'L', 'T'),
    HB_ٹیگ ('L', 'T', 'S', 'H'),
    // Graphite tables
    HB_ٹیگ ('F', 'e', 'a', 't'),
    HB_ٹیگ ('G', 'l', 'a', 't'),
    HB_ٹیگ ('G', 'l', 'o', 'c'),
    HB_ٹیگ ('S', 'i', 'l', 'f'),
    HB_ٹیگ ('S', 'i', 'l', 'l'),
  };
  sets.drop_tables->جوڑو_array (default_drop_tables, ARRAY_لمائی (default_drop_tables));

  hb_ٹیگ_t default_no_subset_tables[] = {
    HB_ٹیگ ('g', 'a', 's', 'p'),
    HB_ٹیگ ('f', 'p', 'g', 'm'),
    HB_ٹیگ ('p', 'r', 'e', 'p'),
    HB_ٹیگ ('V', 'D', 'M', 'X'),
    HB_ٹیگ ('D', 'S', 'I', 'G'),
  };
  sets.no_subset_tables->جوڑو_array (default_no_subset_tables,
					 ARRAY_لمائی (default_no_subset_tables));

  //copied from _layout_features_groups in fonttools
  hb_ٹیگ_t default_layout_features[] = {
    // default shaper
    // common
    HB_ٹیگ ('r', 'v', 'r', 'n'),
    HB_ٹیگ ('c', 'c', 'm', 'p'),
    HB_ٹیگ ('l', 'i', 'g', 'a'),
    HB_ٹیگ ('l', 'o', 'c', 'l'),
    HB_ٹیگ ('m', 'a', 'r', 'k'),
    HB_ٹیگ ('m', 'k', 'm', 'k'),
    HB_ٹیگ ('r', 'l', 'i', 'g'),

    //fractions
    HB_ٹیگ ('f', 'r', 'a', 'c'),
    HB_ٹیگ ('n', 'u', 'm', 'r'),
    HB_ٹیگ ('d', 'n', 'o', 'm'),

    //horizontal
    HB_ٹیگ ('c', 'a', 'l', 't'),
    HB_ٹیگ ('c', 'l', 'i', 'g'),
    HB_ٹیگ ('c', 'u', 'r', 's'),
    HB_ٹیگ ('k', 'e', 'r', 'n'),
    HB_ٹیگ ('r', 'c', 'l', 't'),

    //vertical
    HB_ٹیگ ('v', 'a', 'l', 't'),
    HB_ٹیگ ('v', 'e', 'r', 't'),
    HB_ٹیگ ('v', 'k', 'r', 'n'),
    HB_ٹیگ ('v', 'p', 'a', 'l'),
    HB_ٹیگ ('v', 'r', 't', '2'),

    //ltr
    HB_ٹیگ ('l', 't', 'r', 'a'),
    HB_ٹیگ ('l', 't', 'r', 'm'),

    //rtl
    HB_ٹیگ ('r', 't', 'l', 'a'),
    HB_ٹیگ ('r', 't', 'l', 'm'),

    //random
    HB_ٹیگ ('r', 'a', 'n', 'd'),

    //justify
    HB_ٹیگ ('j', 'a', 'l', 't'), // HarfBuzz doesn't use; others might

    //East Asian spacing
    HB_ٹیگ ('c', 'h', 'w', 's'),
    HB_ٹیگ ('v', 'c', 'h', 'w'),
    HB_ٹیگ ('h', 'a', 'l', 't'),
    HB_ٹیگ ('v', 'h', 'a', 'l'),
    HB_ٹیگ ('p', 'a', 'l', 't'),

    //نجی
    HB_ٹیگ ('H', 'a', 'r', 'f'),
    HB_ٹیگ ('H', 'A', 'R', 'F'),
    HB_ٹیگ ('B', 'u', 'z', 'z'),
    HB_ٹیگ ('B', 'U', 'Z', 'Z'),

    //shapers

    //arabic
    HB_ٹیگ ('i', 'n', 'i', 't'),
    HB_ٹیگ ('m', 'e', 'd', 'i'),
    HB_ٹیگ ('f', 'i', 'n', 'a'),
    HB_ٹیگ ('i', 's', 'o', 'l'),
    HB_ٹیگ ('m', 'e', 'd', '2'),
    HB_ٹیگ ('f', 'i', 'n', '2'),
    HB_ٹیگ ('f', 'i', 'n', '3'),
    HB_ٹیگ ('c', 's', 'w', 'h'),
    HB_ٹیگ ('m', 's', 'e', 't'),
    HB_ٹیگ ('s', 't', 'c', 'h'),

    //hangul
    HB_ٹیگ ('l', 'j', 'm', 'o'),
    HB_ٹیگ ('v', 'j', 'm', 'o'),
    HB_ٹیگ ('t', 'j', 'm', 'o'),

    //tibetan
    HB_ٹیگ ('a', 'b', 'v', 's'),
    HB_ٹیگ ('b', 'l', 'w', 's'),
    HB_ٹیگ ('a', 'b', 'v', 'm'),
    HB_ٹیگ ('b', 'l', 'w', 'm'),

    //indic
    HB_ٹیگ ('n', 'u', 'k', 't'),
    HB_ٹیگ ('a', 'k', 'h', 'n'),
    HB_ٹیگ ('r', 'p', 'h', 'f'),
    HB_ٹیگ ('r', 'k', 'r', 'f'),
    HB_ٹیگ ('p', 'r', 'e', 'f'),
    HB_ٹیگ ('b', 'l', 'w', 'f'),
    HB_ٹیگ ('h', 'a', 'l', 'f'),
    HB_ٹیگ ('a', 'b', 'v', 'f'),
    HB_ٹیگ ('p', 's', 't', 'f'),
    HB_ٹیگ ('c', 'f', 'a', 'r'),
    HB_ٹیگ ('v', 'a', 't', 'u'),
    HB_ٹیگ ('c', 'j', 'c', 't'),
    HB_ٹیگ ('i', 'n', 'i', 't'),
    HB_ٹیگ ('p', 'r', 'e', 's'),
    HB_ٹیگ ('a', 'b', 'v', 's'),
    HB_ٹیگ ('b', 'l', 'w', 's'),
    HB_ٹیگ ('p', 's', 't', 's'),
    HB_ٹیگ ('h', 'a', 'l', 'n'),
    HB_ٹیگ ('d', 'i', 's', 't'),
    HB_ٹیگ ('a', 'b', 'v', 'm'),
    HB_ٹیگ ('b', 'l', 'w', 'm'),
  };

  sets.layout_features->جوڑو_array (default_layout_features, ARRAY_لمائی (default_layout_features));

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
hb_subset_داخلہ_t *
hb_subset_داخلہ_create_or_fail (فراغ)
{
  hb_subset_داخلہ_t *داخلہ = hb_object_create<hb_subset_داخلہ_t>();

  إذا (unlikely (!داخلہ))
    أرجع خالی_سوچک;

  إذا (داخلہ->وچ_error ())
  {
    hb_subset_داخلہ_destroy (داخلہ);
    أرجع خالی_سوچک;
  }

  أرجع داخلہ;
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
hb_subset_داخلہ_t *
hb_subset_داخلہ_reference (hb_subset_داخلہ_t *داخلہ)
{
  أرجع hb_object_reference (داخلہ);
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
hb_subset_داخلہ_destroy (hb_subset_داخلہ_t *داخلہ)
{
  إذا (!hb_object_destroy (داخلہ)) أرجع;

  hb_free (داخلہ);
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
HB_EXTERN hb_سیٹ_t *
hb_subset_داخلہ_unicode_سیٹ (hb_subset_داخلہ_t *داخلہ)
{
  أرجع داخلہ->sets.unicodes;
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
HB_EXTERN hb_سیٹ_t *
hb_subset_داخلہ_glyph_سیٹ (hb_subset_داخلہ_t *داخلہ)
{
  أرجع داخلہ->sets.glyphs;
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
HB_EXTERN hb_سیٹ_t *
hb_subset_داخلہ_سیٹ (hb_subset_داخلہ_t *داخلہ, hb_subset_sets_t سیٹ_type)
{
  أرجع داخلہ->sets_iter () [سیٹ_type];
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
hb_subset_داخلہ_get_flags (hb_subset_داخلہ_t *داخلہ)
{
  أرجع (hb_subset_flags_t) داخلہ->flags;
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
hb_subset_داخلہ_سیٹ_flags (hb_subset_داخلہ_t *داخلہ,
			   بنانشانی value)
{
  داخلہ->flags = (hb_subset_flags_t) value;
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
hb_subset_داخلہ_سیٹ_user_انکڑے (hb_subset_داخلہ_t  *داخلہ,
			       hb_user_انکڑے_کلید_t *key,
			       فراغ *		   انکڑے,
			       hb_destroy_func_t   destroy,
			       hb_bool_t	   replace)
{
  أرجع hb_object_سیٹ_user_انکڑے (داخلہ, key, انکڑے, destroy, replace);
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
hb_subset_داخلہ_get_user_انکڑے (ثابت hb_subset_داخلہ_t *داخلہ,
			       hb_user_انکڑے_کلید_t     *key)
{
  أرجع hb_object_get_user_انکڑے (داخلہ, key);
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
hb_subset_داخلہ_keep_everything (hb_subset_داخلہ_t *داخلہ)
{
  ثابت hb_subset_sets_t indices[] = {HB_SUBSET_SETS_UNICODE,
				      HB_SUBSET_SETS_GLYPH_اشاریہ,
				      HB_SUBSET_SETS_NAME_ID,
				      HB_SUBSET_SETS_NAME_LANG_ID,
				      HB_SUBSET_SETS_LAYOUT_FEATURE_ٹیگ,
				      HB_SUBSET_SETS_LAYOUT_SCRIPT_ٹیگ};

  لكل (خودکار idx : hb_iter (indices))
  {
    hb_سیٹ_t *سیٹ = hb_subset_داخلہ_سیٹ (داخلہ, idx);
    hb_سیٹ_clear (سیٹ);
    hb_سیٹ_invert (سیٹ);
  }

  // Don't drop any tables
  hb_سیٹ_clear (hb_subset_داخلہ_سیٹ (داخلہ, HB_SUBSET_SETS_DROP_جدول_ٹیگ));

  hb_subset_داخلہ_سیٹ_flags (داخلہ,
			     HB_SUBSET_FLAGS_NOTDEF_OUTLINE |
			     HB_SUBSET_FLAGS_GLYPH_ناں |
			     HB_SUBSET_FLAGS_NAME_LEGACY |
			     HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES |
                             HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED);
}

#جے_بنا_تعریف HB_NO_VAR
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
hb_subset_داخلہ_pin_all_axes_منزل_default (hb_subset_داخلہ_t  *داخلہ,
                                         hb_face_t          *face)
{
  بنانشانی axis_گنتی = hb_ot_var_get_axis_گنتی (face);
  إذا (!axis_گنتی) أرجع خطأ;

  hb_ot_var_axis_info_t *axis_infos = (hb_ot_var_axis_info_t *) hb_calloc (axis_گنتی, حجم (hb_ot_var_axis_info_t));
  إذا (unlikely (!axis_infos)) أرجع خطأ;

  (فراغ) hb_ot_var_get_axis_infos (face, 0, &axis_گنتی, axis_infos);

  لكل (بنانشانی i = 0; i < axis_گنتی; i++)
  {
    hb_ٹیگ_t axis_ٹیگ_2 = axis_infos[i].ٹیگ_2;
    مزدوج default_val = (مزدوج) axis_infos[i].default_قدر;
    إذا (!داخلہ->axes_location.سیٹ (axis_ٹیگ_2, Triple (default_val, default_val, default_val)))
    {
      hb_free (axis_infos);
      أرجع خطأ;
    }
  }
  hb_free (axis_infos);
  أرجع سچ;
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
hb_subset_داخلہ_pin_axis_منزل_default (hb_subset_داخلہ_t  *داخلہ,
                                     hb_face_t          *face,
                                     hb_ٹیگ_t            axis_ٹیگ_2)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_لبھو_axis_info (face, axis_ٹیگ_2, &axis_info))
    أرجع خطأ;

  مزدوج default_val = (مزدوج) axis_info.default_قدر;
  أرجع داخلہ->axes_location.سیٹ (axis_ٹیگ_2, Triple (default_val, default_val, default_val));
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
hb_subset_داخلہ_pin_axis_location (hb_subset_داخلہ_t  *داخلہ,
                                   hb_face_t          *face,
                                   hb_ٹیگ_t            axis_ٹیگ_2,
                                   عائم               axis_قدر)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_لبھو_axis_info (face, axis_ٹیگ_2, &axis_info))
    أرجع خطأ;

  مزدوج val = hb_clamp((مزدوج) axis_قدر, (مزدوج) axis_info.min_قدر, (مزدوج) axis_info.max_قدر);
  أرجع داخلہ->axes_location.سیٹ (axis_ٹیگ_2, Triple (val, val, val));
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
hb_subset_داخلہ_سیٹ_axis_range (hb_subset_داخلہ_t  *داخلہ,
                                hb_face_t          *face,
                                hb_ٹیگ_t            axis_ٹیگ_2,
                                عائم               axis_min_قدر,
                                عائم               axis_max_قدر,
                                عائم               axis_def_قدر)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_لبھو_axis_info (face, axis_ٹیگ_2, &axis_info))
    أرجع خطأ;

  عائم min = !std::isnan(axis_min_قدر) ? axis_min_قدر : axis_info.min_قدر;
  عائم max = !std::isnan(axis_max_قدر) ? axis_max_قدر : axis_info.max_قدر;
  عائم def = !std::isnan(axis_def_قدر) ? axis_def_قدر : axis_info.default_قدر;

  إذا (min > max)
    أرجع خطأ;

  عائم new_min_val = hb_clamp(min, axis_info.min_قدر, axis_info.max_قدر);
  عائم new_max_val = hb_clamp(max, axis_info.min_قدر, axis_info.max_قدر);
  عائم new_default_val = hb_clamp(def, new_min_val, new_max_val);
  أرجع داخلہ->axes_location.سیٹ (axis_ٹیگ_2, Triple ((مزدوج) new_min_val, (مزدوج) new_default_val, (مزدوج) new_max_val));
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
hb_subset_داخلہ_get_axis_range (hb_subset_داخلہ_t  *داخلہ,
				hb_ٹیگ_t            axis_ٹیگ_2,
				عائم              *axis_min_قدر,
				عائم              *axis_max_قدر,
				عائم              *axis_def_قدر)

{
  Triple* triple;
  إذا (!داخلہ->axes_location.has(axis_ٹیگ_2, &triple)) {
    أرجع خطأ;
  }

  *axis_min_قدر = triple->minimum;
  *axis_def_قدر = triple->درمیان;
  *axis_max_قدر = triple->ودھ_توں_ودھ;
  أرجع سچ;
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
                                  عائم *axis_min_قدر,
                                  عائم *axis_max_قدر,
                                  عائم *axis_def_قدر)
{
  إذا (len < 0)
    len = لڑی_لمبائی (str);

  ثابت حرف *سرا = str + len;
  ثابت حرف* part = strpbrk (str, ":");
  إذا (!part)
  {
    // Single value.
    إذا (strcmp (str, "drop") == 0)
    {
      *axis_min_قدر = NAN;
      *axis_def_قدر = NAN;
      *axis_max_قدر = NAN;
      أرجع سچ;
    }

    مزدوج v;
    إذا (!hb_parse_double (&str, سرا, &v)) أرجع خطأ;

    *axis_min_قدر = v;
    *axis_def_قدر = v;
    *axis_max_قدر = v;
    أرجع سچ;
  }

  عائم values[3];
  صحيح گنتی = 0;
  لكل (صحيح i = 0; i < 3; i++) {
    گنتی++;
    إذا (!*str || part == str)
    {
      values[i] = NAN;

      إذا (part == عدم) اكسر;
      str = part + 1;
      part = strpbrk (str, ":");
      استمر;
    }

    مزدوج v;
    ثابت حرف *piece_سرا = part ? part : سرا;
    إذا (!hb_parse_double (&str, piece_سرا, &v)) أرجع خطأ;
    values[i] = v;

    إذا (part == عدم) اكسر;
    str = part + 1;
    part = strpbrk (str, ":");
  }

  إذا (گنتی == 2)
  {
    *axis_min_قدر = values[0];
    *axis_def_قدر = NAN;
    *axis_max_قدر = values[1];
    أرجع سچ;
  }
  وإلا إذا (گنتی == 3)
  {
    *axis_min_قدر = values[0];
    *axis_def_قدر = values[1];
    *axis_max_قدر = values[2];
    أرجع سچ;
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
hb_subset_axis_range_منزل_string (hb_subset_داخلہ_t *داخلہ,
                                hb_ٹیگ_t axis_ٹیگ_2,
                                حرف *buf, بنانشانی ناپ)
{
  إذا (unlikely (!ناپ)) أرجع;
  Triple* triple;
  إذا (!داخلہ->axes_location.has(axis_ٹیگ_2, &triple)) {
    أرجع;
  }

  حرف s[128];
  بنانشانی len = 0;

  hb_locale_t clocale HB_بن_ورتیا;
  hb_locale_t oldlocale HB_بن_ورتیا;
  oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", عدم));
  len += hb_max (0, snprintf (s, ARRAY_لمائی (s) - len, "%g", (مزدوج) triple->minimum));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_لمائی (s) - len, "%g", (مزدوج) triple->درمیان));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_لمائی (s) - len, "%g", (مزدوج) triple->ودھ_توں_ودھ));
  (فراغ) hb_uselocale (((فراغ) freelocale (clocale), oldlocale));

  أكد (len < ARRAY_لمائی (s));
  len = hb_min (len, ناپ - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}
#جے_انت

#جے_تعریف_شدہ HB_EXPERIMENTAL_API
ساكن فراغ
_append_arg (hb_سمتیہ_t<حرف> &sb, ثابت حرف *arg)
{
  إذا (sb.وچ_error ()) أرجع;
  إذا (sb.لمائی)
    sb.push (' ');
  بنانشانی len = لڑی_لمبائی (arg);
  sb.extend (hb_بائٹاں_t (arg, len));
}
ساكن فراغ
format_نمبر_سیٹ (ثابت hb_سیٹ_t *سیٹ, hb_سمتیہ_t<حرف> &باہرلا, بنانشانی ادھار = 10)
{
  أكد (ادھار == 10 || ادھار == 16);
  إذا (unlikely (ادھار != 10 && ادھار != 16)) أرجع;

  hb_codepoint_t پہلا = HB_سیٹ_قدر_INVALID, آخری = HB_سیٹ_قدر_INVALID;
  منطقي پہلا_range = سچ;
  طالما (hb_سیٹ_اگلا_range (سیٹ, &پہلا, &آخری))
  {
    إذا (!پہلا_range)
      باہرلا.push (',');
    پہلا_range = خطأ;

    حرف buf[64];
    صحيح len;
    إذا (پہلا == آخری)
    {
      إذا (ادھار == 16)
        len = snprintf (buf, حجم (buf), "%X", پہلا);
      وإلا
        len = snprintf (buf, حجم (buf), "%u", پہلا);
    }
    وإلا
    {
      إذا (ادھار == 16)
        len = snprintf (buf, حجم (buf), "%X-%X", پہلا, آخری);
      وإلا
        len = snprintf (buf, حجم (buf), "%u-%u", پہلا, آخری);
    }
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع;
    باہرلا.extend (hb_بائٹاں_t (buf, len));
  }
}

ساكن فراغ
format_ٹیگ (hb_ٹیگ_t ٹیگ_2, hb_سمتیہ_t<حرف> &باہرلا)
{
  حرف buf[4];
  hb_ٹیگ_منزل_string (ٹیگ_2, buf);

  بنانشانی len = 4;
  طالما (len > 0 && buf[len - 1] == ' ')
    len--;

  لكل (بنانشانی i = 0; i < len; i++)
  {
    حرف c = buf[i];
    إذا (!((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '-' || c == '_'))
    {
      c = '_';
    }
    باہرلا.push (c);
  }
}

ساكن فراغ
format_ٹیگ_سیٹ (ثابت hb_سیٹ_t *سیٹ, hb_سمتیہ_t<حرف> &باہرلا)
{
  منطقي پہلا_ٹیگ = سچ;
  لكل (hb_codepoint_t ٹیگ_2 : *سیٹ)
  {
    إذا (!پہلا_ٹیگ)
      باہرلا.push (',');
    پہلا_ٹیگ = خطأ;

    format_ٹیگ (ٹیگ_2, باہرلا);
  }
}

ساكن منطقي
is_سیٹ_default (ثابت hb_سیٹ_t *سیٹ, ثابت hb_سیٹ_t *default_سیٹ)
{
  أرجع hb_سیٹ_is_inverted (سیٹ) == hb_سیٹ_is_inverted (default_سیٹ) &&
         hb_سیٹ_is_equal (سیٹ, default_سیٹ);
}

سانچا <قسم_ناں Formatter>
ساكن فراغ
format_سیٹ_arg (ثابت hb_سیٹ_t *سیٹ,
                 ثابت حرف *opt_name,
                 Formatter &&formatter,
                 hb_سمتیہ_t<حرف> &sb)
{
  إذا (hb_سیٹ_is_inverted (سیٹ))
  {
    hb_سیٹ_t *excluded = hb_سیٹ_copy (سیٹ);
    hb_سیٹ_invert (excluded);

    hb_سمتیہ_t<حرف> opt;
    opt.extend (hb_بائٹاں_t ("--", 2));
    opt.extend (hb_بائٹاں_t (opt_name, لڑی_لمبائی (opt_name)));
    opt.extend (hb_بائٹاں_t ("=*", 2));
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);

    إذا (!hb_سیٹ_is_خالی_تھاواں (excluded))
    {
      hb_سمتیہ_t<حرف> formatted;
      formatter (excluded, formatted);
      إذا (formatted.لمائی)
      {
        hb_سمتیہ_t<حرف> opt2;
        opt2.extend (hb_بائٹاں_t ("--", 2));
        opt2.extend (hb_بائٹاں_t (opt_name, لڑی_لمبائی (opt_name)));
        opt2.extend (hb_بائٹاں_t ("-=", 2));
        opt2.extend (formatted.as_array ());
        opt2.push ('\0');
        إذا (opt2.arrayZ) _append_arg (sb, opt2.arrayZ);
      }
    }
    hb_سیٹ_destroy (excluded);
  }
  وإلا إذا (hb_سیٹ_is_خالی_تھاواں (سیٹ))
  {
    hb_سمتیہ_t<حرف> opt;
    opt.extend (hb_بائٹاں_t ("--", 2));
    opt.extend (hb_بائٹاں_t (opt_name, لڑی_لمبائی (opt_name)));
    opt.extend (hb_بائٹاں_t ("-=*", 3));
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  وإلا
  {
    hb_سمتیہ_t<حرف> formatted;
    formatter (سیٹ, formatted);
    إذا (formatted.لمائی)
    {
      hb_سمتیہ_t<حرف> opt;
      opt.extend (hb_بائٹاں_t ("--", 2));
      opt.extend (hb_بائٹاں_t (opt_name, لڑی_لمبائی (opt_name)));
      opt.extend (hb_بائٹاں_t ("=", 1));
      opt.extend (formatted.as_array ());
      opt.push ('\0');
      إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
    }
  }
}

ساكن فراغ
format_نمبر_سیٹ_arg (ثابت hb_سیٹ_t *سیٹ,
                        ثابت حرف *opt_name,
                        hb_سمتیہ_t<حرف> &sb,
                        بنانشانی ادھار = 10)
{
  format_سیٹ_arg (سیٹ, opt_name, [ادھار] (ثابت hb_سیٹ_t *s, hb_سمتیہ_t<حرف> &باہرلا) {
    format_نمبر_سیٹ (s, باہرلا, ادھار);
  }, sb);
}

ساكن فراغ
format_ٹیگ_سیٹ_arg (ثابت hb_سیٹ_t *سیٹ,
                     ثابت حرف *opt_name,
                     hb_سمتیہ_t<حرف> &sb)
{
  format_سیٹ_arg (سیٹ, opt_name, format_ٹیگ_سیٹ, sb);
}

ساكن منطقي
_format_glyph_map_arg (ثابت hb_map_t &glyph_map,
                       hb_سمتیہ_t<حرف> &sb)
{
  إذا (glyph_map.is_empty ())
    أرجع سچ;

  hb_سیٹ_t کلیدیں;
  hb_map_کلیدیں (&glyph_map, &کلیدیں);
  hb_سمتیہ_t<حرف> جوڑے;
  منطقي پہلا = سچ;
  لكل (hb_codepoint_t key : کلیدیں)
  {
    إذا (!پہلا)
      جوڑے.push (',');
    پہلا = خطأ;
    hb_codepoint_t val = hb_map_get (&glyph_map, key);
    حرف buf[64];
    صحيح len = snprintf (buf, حجم (buf), "%u:%u", key, val);
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع خطأ;
    جوڑے.extend (hb_بائٹاں_t (buf, len));
  }
  إذا (جوڑے.لمائی)
  {
    hb_سمتیہ_t<حرف> opt;
    opt.extend (hb_بائٹاں_t ("--gid-map=", 10));
    opt.extend (جوڑے.as_array ());
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  أرجع سچ;
}

ساكن منطقي
_format_axes_location_arg (ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> &axes_location,
                           hb_سمتیہ_t<حرف> &sb)
{
  إذا (axes_location.is_empty ())
    أرجع سچ;

  hb_سمتیہ_t<hb_ٹیگ_t> axis_ٹیگ_سوچی;
  لكل (خودکار ٹیگ_2 : axes_location.کلیدیں ())
    axis_ٹیگ_سوچی.push (ٹیگ_2);
  axis_ٹیگ_سوچی.qsort ([] (ثابت hb_ٹیگ_t &a, ثابت hb_ٹیگ_t &b) { أرجع a < b ? -1 : a > b ? 1 : 0; });

  hb_سمتیہ_t<حرف> specs;
  منطقي پہلا = سچ;
  لكل (hb_ٹیگ_t ٹیگ_2 : axis_ٹیگ_سوچی)
  {
    Triple triple = axes_location.get (ٹیگ_2);
    إذا (!پہلا)
      specs.push (',');
    پہلا = خطأ;

    hb_سمتیہ_t<حرف> ٹیگ_buf;
    format_ٹیگ (ٹیگ_2, ٹیگ_buf);

    حرف buf[128];
    صحيح len;
    إذا (triple.minimum == triple.درمیان && triple.درمیان == triple.ودھ_توں_ودھ)
    {
      len = snprintf (buf, حجم (buf), "%.*s=%g", (صحيح) ٹیگ_buf.لمائی, ٹیگ_buf.arrayZ, (مزدوج) triple.درمیان);
    }
    وإلا إذا (std::isnan (triple.minimum) && std::isnan (triple.درمیان) && std::isnan (triple.ودھ_توں_ودھ))
    {
      len = snprintf (buf, حجم (buf), "%.*s=drop", (صحيح) ٹیگ_buf.لمائی, ٹیگ_buf.arrayZ);
    }
    وإلا
    {
      len = snprintf (buf, حجم (buf), "%.*s=%g:%g:%g", (صحيح) ٹیگ_buf.لمائی, ٹیگ_buf.arrayZ,
                      (مزدوج) triple.minimum, (مزدوج) triple.درمیان, (مزدوج) triple.ودھ_توں_ودھ);
    }
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع خطأ;
    specs.extend (hb_بائٹاں_t (buf, len));
  }
  إذا (specs.لمائی)
  {
    hb_سمتیہ_t<حرف> opt;
    opt.extend (hb_بائٹاں_t ("--variations=", 13));
    opt.extend (specs.as_array ());
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  أرجع سچ;
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
hb_subset_داخلہ_منزل_string_or_fail (hb_subset_داخلہ_t *داخلہ)
{
  إذا (unlikely (!داخلہ || داخلہ->وچ_error ()))
    أرجع خالی_سوچک;

  hb_سمتیہ_t<حرف> sb;

  بنية flag_option_t {
    hb_subset_flags_t flag;
    حرف option[32];
  };
  ساكن ثابت flag_option_t flag_options[] = {
    { HB_SUBSET_FLAGS_NO_HINTING, "no-hinting" },
    { HB_SUBSET_FLAGS_RETAIN_GIDS, "retain-gids" },
    { HB_SUBSET_FLAGS_DESUBROUTINIZE, "desubroutinize" },
    { HB_SUBSET_FLAGS_NAME_LEGACY, "name-legacy" },
    { HB_SUBSET_FLAGS_سیٹ_OVERLAPS_FLAG, "set-overlaps-flag" },
    { HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED, "passthrough-tables" },
    { HB_SUBSET_FLAGS_NOTDEF_OUTLINE, "notdef-outline" },
    { HB_SUBSET_FLAGS_GLYPH_ناں, "glyph-names" },
    { HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES, "no-prune-unicode-ranges" },
    { HB_SUBSET_FLAGS_NO_LAYOUT_گھیرا, "no-layout-closure" },
    { HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS, "optimize" },
    { HB_SUBSET_FLAGS_NO_BIDI_گھیرا, "no-bidi-closure" },
#جے_تعریف_شدہ HB_EXPERIMENTAL_API
    { HB_SUBSET_FLAGS_IFTB_REQUIREMENTS, "iftb-requirements" },
    { HB_SUBSET_FLAGS_RETAIN_عدد_GLYPHS, "retain-num-glyphs" },
#جے_انت
    { HB_SUBSET_FLAGS_DOWNGRADE_CFF2, "downgrade-cff2" },
  };

#جے_تعریف_شدہ HB_EXPERIMENTAL_API
  static_assert (حجم (flag_options) / حجم (flag_options[0]) == 15,
                 "Check all flags in hb_subset_flags_t are handled here.");
#وإلا
  static_assert (حجم (flag_options) / حجم (flag_options[0]) == 13,
                 "Check all flags in hb_subset_flags_t are handled here.");
#جے_انت

  لكل (ثابت خودکار &fo : flag_options)
  {
    إذا (داخلہ->flags & fo.flag)
    {
      حرف buf[36];
      snprintf (buf, حجم (buf), "--%s", fo.option);
      _append_arg (sb, buf);
    }
  }

  hb_subset_داخلہ_t *default_داخلہ = hb_subset_داخلہ_create_or_fail ();
  إذا (unlikely (!default_داخلہ))
    أرجع خالی_سوچک;

  // Serialize all sets if they are not equal to their default setting:
#تعریف_کرو PROCESS_نمبر_سیٹ(میدان, opt_name, ...) \
  إذا (!is_سیٹ_default (داخلہ->sets.میدان, default_داخلہ->sets.میدان)) \
    format_نمبر_سیٹ_arg (داخلہ->sets.میدان, opt_name, sb, ##__VA_ARGS__)

#تعریف_کرو PROCESS_ٹیگ_سیٹ(میدان, opt_name) \
  إذا (!is_سیٹ_default (داخلہ->sets.میدان, default_داخلہ->sets.میدان)) \
    format_ٹیگ_سیٹ_arg (داخلہ->sets.میدان, opt_name, sb)

  PROCESS_نمبر_سیٹ (unicodes, "unicodes", 16);
  PROCESS_نمبر_سیٹ (glyphs, "gids");
  PROCESS_نمبر_سیٹ (name_ids, "name-IDs");
  PROCESS_نمبر_سیٹ (name_languages, "name-languages");
  PROCESS_ٹیگ_سیٹ (layout_features, "layout-features");
  PROCESS_ٹیگ_سیٹ (layout_scripts, "layout-scripts");
  PROCESS_ٹیگ_سیٹ (drop_tables, "drop-tables");
  PROCESS_ٹیگ_سیٹ (no_subset_tables, "passthrough-tables");

#تعریف_رد PROCESS_نمبر_سیٹ
#تعریف_رد PROCESS_ٹیگ_سیٹ

  hb_subset_داخلہ_destroy (default_داخلہ);

  static_assert (حجم (داخلہ->sets) / حجم (hb_سیٹ_t*) == 8,
                 "Check all sets in hb_subset_input_t::sets_t are handled here.");

  إذا (unlikely (!_format_glyph_map_arg (داخلہ->glyph_map, sb)))
    أرجع خالی_سوچک;

  إذا (unlikely (!_format_axes_location_arg (داخلہ->axes_location, sb)))
    أرجع خالی_سوچک;

  sb.push ('\0');
  إذا (unlikely (sb.وچ_error ()))
    أرجع خالی_سوچک;

  بنانشانی len = 0;
  حرف *buf = sb.steal (&len);
  إذا (unlikely (!buf))
    أرجع خالی_سوچک;

  أرجع hb_blob_create_or_fail (buf, len, HB_حافظہ_منوال_WRITABLE, buf, hb_free);
}
#جے_انت

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
hb_subset_preprocess (hb_face_t *سروت)
{
  hb_subset_داخلہ_t* داخلہ = hb_subset_داخلہ_create_or_fail ();
  إذا (!داخلہ)
    أرجع hb_face_reference (سروت);

  hb_subset_داخلہ_keep_everything (داخلہ);

  داخلہ->attach_accelerator_انکڑے = سچ;

  // Always use long loca in the preprocessed version. This allows
  // us to store the glyph bytes unpadded which allows the future subset
  // operation to run faster by skipping the trim padding step.
  داخلہ->force_long_loca = سچ;

  hb_face_t* new_سروت = hb_subset_or_fail (سروت, داخلہ);
  hb_subset_داخلہ_destroy (داخلہ);

  إذا (!new_سروت) {
    DEBUG_پیغام (SUBSET, خالی_سوچک, "Preprocessing failed due to subset failure.");
    أرجع hb_face_reference (سروت);
  }

  أرجع new_سروت;
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
hb_subset_داخلہ_old_منزل_new_glyph_mapping (hb_subset_داخلہ_t *داخلہ)
{
  أرجع &داخلہ->glyph_map;
}

#جے_تعریف_شدہ HB_EXPERIMENTAL_API
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
hb_subset_داخلہ_override_name_جدول (hb_subset_داخلہ_t  *داخلہ,
                                     hb_ot_name_id_t     name_id,
                                     بنانشانی            platform_id,
                                     بنانشانی            encoding_id,
                                     بنانشانی            language_id,
                                     ثابت حرف         *name_str,
                                     صحيح                 str_len /* -1 means nul-terminated */)
{
  إذا (!name_str)
  {
    str_len = 0;
  }
  وإلا إذا (str_len == -1)
  {
      str_len = لڑی_لمبائی (name_str);
  }

  hb_بائٹاں_t name_بائٹاں (خالی_سوچک, 0);
  إذا (str_len)
  {
    إذا (platform_id == 1)
    {
      ثابت uint8_t *src = reinterpret_cast<ثابت uint8_t*> (name_str);
      ثابت uint8_t *src_سرا = src + str_len;

      hb_codepoint_t unicode;
      ثابت hb_codepoint_t replacement = HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT;
      طالما (src < src_سرا)
      {
        src = hb_utf8_t::next (src, src_سرا, &unicode, replacement);
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
    name_بائٹاں = hb_بائٹاں_t (override_name, str_len);
  }
  داخلہ->name_جدول_overrides.سیٹ (hb_ot_name_ریکارڈ_ids_t (platform_id, encoding_id, language_id, name_id), name_بائٹاں);
  أرجع سچ;
}
#جے_انت
