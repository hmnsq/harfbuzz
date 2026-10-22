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

#شامل "hb-subset-instancer-solver.hh"
#شامل "hb-subset.hh"
#شامل "hb-set.hh"
#شامل "hb-utf.hh"


hb_subset_داخلہ_t::hb_subset_داخلہ_t ()
{
  برائے (خودکار& سیٹ : sets_iter ())
    سیٹ = hb::shared_ptr<hb_سیٹ_t> (hb_سیٹ_create ());

  اگر (اندر_error ())
    واپس;

  flags = HB_SUBSET_FLAGS_DEFAULT;

  hb_سیٹ_جوڑو_range (sets.name_ids, 0, 6);
  hb_سیٹ_جوڑو (sets.name_languages, 0x0409);

  hb_لیبل_t default_drop_tables[] = {
    // Layout disabled by default
    HB_لیبل ('m', 'o', 'r', 'x'),
    HB_لیبل ('m', 'o', 'r', 't'),
    HB_لیبل ('k', 'e', 'r', 'x'),
    HB_لیبل ('k', 'e', 'r', 'n'),

    // Copied from fontTools:
    HB_لیبل ('J', 'S', 'T', 'F'),
    HB_لیبل ('D', 'S', 'I', 'G'),
    HB_لیبل ('E', 'B', 'D', 'T'),
    HB_لیبل ('E', 'B', 'L', 'C'),
    HB_لیبل ('E', 'B', 'S', 'C'),
    HB_لیبل ('S', 'V', 'G', ' '),
    HB_لیبل ('P', 'C', 'L', 'T'),
    HB_لیبل ('L', 'T', 'S', 'H'),
    // Graphite tables
    HB_لیبل ('F', 'e', 'a', 't'),
    HB_لیبل ('G', 'l', 'a', 't'),
    HB_لیبل ('G', 'l', 'o', 'c'),
    HB_لیبل ('S', 'i', 'l', 'f'),
    HB_لیبل ('S', 'i', 'l', 'l'),
  };
  sets.drop_tables->جوڑو_array (default_drop_tables, ARRAY_لمبائی (default_drop_tables));

  hb_لیبل_t default_no_subset_tables[] = {
    HB_لیبل ('g', 'a', 's', 'p'),
    HB_لیبل ('f', 'p', 'g', 'm'),
    HB_لیبل ('p', 'r', 'e', 'p'),
    HB_لیبل ('V', 'D', 'M', 'X'),
    HB_لیبل ('D', 'S', 'I', 'G'),
  };
  sets.no_subset_tables->جوڑو_array (default_no_subset_tables,
					 ARRAY_لمبائی (default_no_subset_tables));

  //copied from _layout_features_groups in fonttools
  hb_لیبل_t default_layout_features[] = {
    // default shaper
    // common
    HB_لیبل ('r', 'v', 'r', 'n'),
    HB_لیبل ('c', 'c', 'm', 'p'),
    HB_لیبل ('l', 'i', 'g', 'a'),
    HB_لیبل ('l', 'o', 'c', 'l'),
    HB_لیبل ('m', 'a', 'r', 'k'),
    HB_لیبل ('m', 'k', 'm', 'k'),
    HB_لیبل ('r', 'l', 'i', 'g'),

    //fractions
    HB_لیبل ('f', 'r', 'a', 'c'),
    HB_لیبل ('n', 'u', 'm', 'r'),
    HB_لیبل ('d', 'n', 'o', 'm'),

    //horizontal
    HB_لیبل ('c', 'a', 'l', 't'),
    HB_لیبل ('c', 'l', 'i', 'g'),
    HB_لیبل ('c', 'u', 'r', 's'),
    HB_لیبل ('k', 'e', 'r', 'n'),
    HB_لیبل ('r', 'c', 'l', 't'),

    //vertical
    HB_لیبل ('v', 'a', 'l', 't'),
    HB_لیبل ('v', 'e', 'r', 't'),
    HB_لیبل ('v', 'k', 'r', 'n'),
    HB_لیبل ('v', 'p', 'a', 'l'),
    HB_لیبل ('v', 'r', 't', '2'),

    //ltr
    HB_لیبل ('l', 't', 'r', 'a'),
    HB_لیبل ('l', 't', 'r', 'm'),

    //rtl
    HB_لیبل ('r', 't', 'l', 'a'),
    HB_لیبل ('r', 't', 'l', 'm'),

    //random
    HB_لیبل ('r', 'a', 'n', 'd'),

    //justify
    HB_لیبل ('j', 'a', 'l', 't'), // HarfBuzz doesn't use; others might

    //East Asian spacing
    HB_لیبل ('c', 'h', 'w', 's'),
    HB_لیبل ('v', 'c', 'h', 'w'),
    HB_لیبل ('h', 'a', 'l', 't'),
    HB_لیبل ('v', 'h', 'a', 'l'),
    HB_لیبل ('p', 'a', 'l', 't'),

    //نجی
    HB_لیبل ('H', 'a', 'r', 'f'),
    HB_لیبل ('H', 'A', 'R', 'F'),
    HB_لیبل ('B', 'u', 'z', 'z'),
    HB_لیبل ('B', 'U', 'Z', 'Z'),

    //shapers

    //arabic
    HB_لیبل ('i', 'n', 'i', 't'),
    HB_لیبل ('m', 'e', 'd', 'i'),
    HB_لیبل ('f', 'i', 'n', 'a'),
    HB_لیبل ('i', 's', 'o', 'l'),
    HB_لیبل ('m', 'e', 'd', '2'),
    HB_لیبل ('f', 'i', 'n', '2'),
    HB_لیبل ('f', 'i', 'n', '3'),
    HB_لیبل ('c', 's', 'w', 'h'),
    HB_لیبل ('m', 's', 'e', 't'),
    HB_لیبل ('s', 't', 'c', 'h'),

    //hangul
    HB_لیبل ('l', 'j', 'm', 'o'),
    HB_لیبل ('v', 'j', 'm', 'o'),
    HB_لیبل ('t', 'j', 'm', 'o'),

    //tibetan
    HB_لیبل ('a', 'b', 'v', 's'),
    HB_لیبل ('b', 'l', 'w', 's'),
    HB_لیبل ('a', 'b', 'v', 'm'),
    HB_لیبل ('b', 'l', 'w', 'm'),

    //indic
    HB_لیبل ('n', 'u', 'k', 't'),
    HB_لیبل ('a', 'k', 'h', 'n'),
    HB_لیبل ('r', 'p', 'h', 'f'),
    HB_لیبل ('r', 'k', 'r', 'f'),
    HB_لیبل ('p', 'r', 'e', 'f'),
    HB_لیبل ('b', 'l', 'w', 'f'),
    HB_لیبل ('h', 'a', 'l', 'f'),
    HB_لیبل ('a', 'b', 'v', 'f'),
    HB_لیبل ('p', 's', 't', 'f'),
    HB_لیبل ('c', 'f', 'a', 'r'),
    HB_لیبل ('v', 'a', 't', 'u'),
    HB_لیبل ('c', 'j', 'c', 't'),
    HB_لیبل ('i', 'n', 'i', 't'),
    HB_لیبل ('p', 'r', 'e', 's'),
    HB_لیبل ('a', 'b', 'v', 's'),
    HB_لیبل ('b', 'l', 'w', 's'),
    HB_لیبل ('p', 's', 't', 's'),
    HB_لیبل ('h', 'a', 'l', 'n'),
    HB_لیبل ('d', 'i', 's', 't'),
    HB_لیبل ('a', 'b', 'v', 'm'),
    HB_لیبل ('b', 'l', 'w', 'm'),
  };

  sets.layout_features->جوڑو_array (default_layout_features, ARRAY_لمبائی (default_layout_features));

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
hb_subset_داخلہ_create_or_fail (خالی)
{
  hb_subset_داخلہ_t *داخلہ = hb_object_create<hb_subset_داخلہ_t>();

  اگر (unlikely (!داخلہ))
    واپس خالی_اشاریہ;

  اگر (داخلہ->اندر_error ())
  {
    hb_subset_داخلہ_destroy (داخلہ);
    واپس خالی_اشاریہ;
  }

  واپس داخلہ;
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
  واپس hb_object_reference (داخلہ);
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
خالی
hb_subset_داخلہ_destroy (hb_subset_داخلہ_t *داخلہ)
{
  اگر (!hb_object_destroy (داخلہ)) واپس;

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
  واپس داخلہ->sets.unicodes;
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
  واپس داخلہ->sets.glyphs;
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
  واپس داخلہ->sets_iter () [سیٹ_type];
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
  واپس (hb_subset_flags_t) داخلہ->flags;
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
HB_EXTERN خالی
hb_subset_داخلہ_سیٹ_flags (hb_subset_داخلہ_t *داخلہ,
			   غیرعلامتی value)
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
hb_subset_داخلہ_سیٹ_user_ڈیٹا (hb_subset_داخلہ_t  *داخلہ,
			       hb_user_ڈیٹا_کلید_t *key,
			       خالی *		   ڈیٹا,
			       hb_destroy_func_t   destroy,
			       hb_bool_t	   replace)
{
  واپس hb_object_سیٹ_user_ڈیٹا (داخلہ, key, ڈیٹا, destroy, replace);
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
خالی *
hb_subset_داخلہ_get_user_ڈیٹا (ثابت hb_subset_داخلہ_t *داخلہ,
			       hb_user_ڈیٹا_کلید_t     *key)
{
  واپس hb_object_get_user_ڈیٹا (داخلہ, key);
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
خالی
hb_subset_داخلہ_keep_everything (hb_subset_داخلہ_t *داخلہ)
{
  ثابت hb_subset_sets_t indices[] = {HB_SUBSET_SETS_UNICODE,
				      HB_SUBSET_SETS_GLYPH_اشاریہ,
				      HB_SUBSET_SETS_NAME_ID,
				      HB_SUBSET_SETS_NAME_LANG_ID,
				      HB_SUBSET_SETS_LAYOUT_FEATURE_لیبل,
				      HB_SUBSET_SETS_LAYOUT_SCRIPT_لیبل};

  برائے (خودکار idx : hb_iter (indices))
  {
    hb_سیٹ_t *سیٹ = hb_subset_داخلہ_سیٹ (داخلہ, idx);
    hb_سیٹ_clear (سیٹ);
    hb_سیٹ_invert (سیٹ);
  }

  // Don't drop any tables
  hb_سیٹ_clear (hb_subset_داخلہ_سیٹ (داخلہ, HB_SUBSET_SETS_DROP_جدول_لیبل));

  hb_subset_داخلہ_سیٹ_flags (داخلہ,
			     HB_SUBSET_FLAGS_NOTDEF_OUTLINE |
			     HB_SUBSET_FLAGS_GLYPH_نام |
			     HB_SUBSET_FLAGS_NAME_LEGACY |
			     HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES |
                             HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED);
}

#اگر_غیر_تعریف_شدہ HB_NO_VAR
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
hb_subset_داخلہ_pin_all_axes_مقصد_default (hb_subset_داخلہ_t  *داخلہ,
                                         hb_face_t          *face)
{
  غیرعلامتی axis_گنتی = hb_ot_var_get_axis_گنتی (face);
  اگر (!axis_گنتی) واپس جھوٹ;

  hb_ot_var_axis_info_t *axis_infos = (hb_ot_var_axis_info_t *) hb_calloc (axis_گنتی, حجم (hb_ot_var_axis_info_t));
  اگر (unlikely (!axis_infos)) واپس جھوٹ;

  (خالی) hb_ot_var_get_axis_infos (face, 0, &axis_گنتی, axis_infos);

  برائے (غیرعلامتی i = 0; i < axis_گنتی; i++)
  {
    hb_لیبل_t axis_لیبل_2 = axis_infos[i].لیبل_2;
    دوہرا default_val = (دوہرا) axis_infos[i].default_قدر;
    اگر (!داخلہ->axes_location.سیٹ (axis_لیبل_2, Triple (default_val, default_val, default_val)))
    {
      hb_free (axis_infos);
      واپس جھوٹ;
    }
  }
  hb_free (axis_infos);
  واپس سچ;
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
hb_subset_داخلہ_pin_axis_مقصد_default (hb_subset_داخلہ_t  *داخلہ,
                                     hb_face_t          *face,
                                     hb_لیبل_t            axis_لیبل_2)
{
  hb_ot_var_axis_info_t axis_info;
  اگر (!hb_ot_var_ڈھونڈو_axis_info (face, axis_لیبل_2, &axis_info))
    واپس جھوٹ;

  دوہرا default_val = (دوہرا) axis_info.default_قدر;
  واپس داخلہ->axes_location.سیٹ (axis_لیبل_2, Triple (default_val, default_val, default_val));
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
                                   hb_لیبل_t            axis_لیبل_2,
                                   اعشاری               axis_قدر)
{
  hb_ot_var_axis_info_t axis_info;
  اگر (!hb_ot_var_ڈھونڈو_axis_info (face, axis_لیبل_2, &axis_info))
    واپس جھوٹ;

  دوہرا val = hb_clamp((دوہرا) axis_قدر, (دوہرا) axis_info.min_قدر, (دوہرا) axis_info.max_قدر);
  واپس داخلہ->axes_location.سیٹ (axis_لیبل_2, Triple (val, val, val));
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
                                hb_لیبل_t            axis_لیبل_2,
                                اعشاری               axis_min_قدر,
                                اعشاری               axis_max_قدر,
                                اعشاری               axis_def_قدر)
{
  hb_ot_var_axis_info_t axis_info;
  اگر (!hb_ot_var_ڈھونڈو_axis_info (face, axis_لیبل_2, &axis_info))
    واپس جھوٹ;

  اعشاری min = !std::isnan(axis_min_قدر) ? axis_min_قدر : axis_info.min_قدر;
  اعشاری max = !std::isnan(axis_max_قدر) ? axis_max_قدر : axis_info.max_قدر;
  اعشاری def = !std::isnan(axis_def_قدر) ? axis_def_قدر : axis_info.default_قدر;

  اگر (min > max)
    واپس جھوٹ;

  اعشاری new_min_val = hb_clamp(min, axis_info.min_قدر, axis_info.max_قدر);
  اعشاری new_max_val = hb_clamp(max, axis_info.min_قدر, axis_info.max_قدر);
  اعشاری new_default_val = hb_clamp(def, new_min_val, new_max_val);
  واپس داخلہ->axes_location.سیٹ (axis_لیبل_2, Triple ((دوہرا) new_min_val, (دوہرا) new_default_val, (دوہرا) new_max_val));
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
				hb_لیبل_t            axis_لیبل_2,
				اعشاری              *axis_min_قدر,
				اعشاری              *axis_max_قدر,
				اعشاری              *axis_def_قدر)

{
  Triple* triple;
  اگر (!داخلہ->axes_location.has(axis_لیبل_2, &triple)) {
    واپس جھوٹ;
  }

  *axis_min_قدر = triple->minimum;
  *axis_def_قدر = triple->درمیان;
  *axis_max_قدر = triple->زیادہ_سے_زیادہ;
  واپس سچ;
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
hb_subset_axis_range_from_string (ثابت حرف *str, صحیح len,
                                  اعشاری *axis_min_قدر,
                                  اعشاری *axis_max_قدر,
                                  اعشاری *axis_def_قدر)
{
  اگر (len < 0)
    len = لڑی_لمبائی (str);

  ثابت حرف *انتہا = str + len;
  ثابت حرف* part = strpbrk (str, ":");
  اگر (!part)
  {
    // Single value.
    اگر (strcmp (str, "drop") == 0)
    {
      *axis_min_قدر = NAN;
      *axis_def_قدر = NAN;
      *axis_max_قدر = NAN;
      واپس سچ;
    }

    دوہرا v;
    اگر (!hb_parse_double (&str, انتہا, &v)) واپس جھوٹ;

    *axis_min_قدر = v;
    *axis_def_قدر = v;
    *axis_max_قدر = v;
    واپس سچ;
  }

  اعشاری values[3];
  صحیح گنتی = 0;
  برائے (صحیح i = 0; i < 3; i++) {
    گنتی++;
    اگر (!*str || part == str)
    {
      values[i] = NAN;

      اگر (part == تہی) توڑو;
      str = part + 1;
      part = strpbrk (str, ":");
      جاری;
    }

    دوہرا v;
    ثابت حرف *piece_انتہا = part ? part : انتہا;
    اگر (!hb_parse_double (&str, piece_انتہا, &v)) واپس جھوٹ;
    values[i] = v;

    اگر (part == تہی) توڑو;
    str = part + 1;
    part = strpbrk (str, ":");
  }

  اگر (گنتی == 2)
  {
    *axis_min_قدر = values[0];
    *axis_def_قدر = NAN;
    *axis_max_قدر = values[1];
    واپس سچ;
  }
  ورنہ اگر (گنتی == 3)
  {
    *axis_min_قدر = values[0];
    *axis_def_قدر = values[1];
    *axis_max_قدر = values[2];
    واپس سچ;
  }
  واپس جھوٹ;
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
HB_EXTERN خالی
hb_subset_axis_range_مقصد_string (hb_subset_داخلہ_t *داخلہ,
                                hb_لیبل_t axis_لیبل_2,
                                حرف *buf, غیرعلامتی سائز)
{
  اگر (unlikely (!سائز)) واپس;
  Triple* triple;
  اگر (!داخلہ->axes_location.has(axis_لیبل_2, &triple)) {
    واپس;
  }

  حرف s[128];
  غیرعلامتی len = 0;

  hb_locale_t clocale HB_غیر_مستعمل;
  hb_locale_t oldlocale HB_غیر_مستعمل;
  oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", تہی));
  len += hb_max (0, snprintf (s, ARRAY_لمبائی (s) - len, "%g", (دوہرا) triple->minimum));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_لمبائی (s) - len, "%g", (دوہرا) triple->درمیان));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_لمبائی (s) - len, "%g", (دوہرا) triple->زیادہ_سے_زیادہ));
  (خالی) hb_uselocale (((خالی) freelocale (clocale), oldlocale));

  أكد (len < ARRAY_لمبائی (s));
  len = hb_min (len, سائز - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}
#اگر_ختم

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
ساکن خالی
_append_arg (hb_سمتیہ_t<حرف> &sb, ثابت حرف *arg)
{
  اگر (sb.اندر_error ()) واپس;
  اگر (sb.لمبائی)
    sb.push (' ');
  غیرعلامتی len = لڑی_لمبائی (arg);
  sb.extend (hb_بائٹیں_t (arg, len));
}
ساکن خالی
format_نمبر_سیٹ (ثابت hb_سیٹ_t *سیٹ, hb_سمتیہ_t<حرف> &برآمد, غیرعلامتی بنیاد = 10)
{
  أكد (بنیاد == 10 || بنیاد == 16);
  اگر (unlikely (بنیاد != 10 && بنیاد != 16)) واپس;

  hb_codepoint_t پہلا = HB_سیٹ_قدر_INVALID, آخری = HB_سیٹ_قدر_INVALID;
  منطقی پہلا_range = سچ;
  جب (hb_سیٹ_اگلا_range (سیٹ, &پہلا, &آخری))
  {
    اگر (!پہلا_range)
      برآمد.push (',');
    پہلا_range = جھوٹ;

    حرف buf[64];
    صحیح len;
    اگر (پہلا == آخری)
    {
      اگر (بنیاد == 16)
        len = snprintf (buf, حجم (buf), "%X", پہلا);
      ورنہ
        len = snprintf (buf, حجم (buf), "%u", پہلا);
    }
    ورنہ
    {
      اگر (بنیاد == 16)
        len = snprintf (buf, حجم (buf), "%X-%X", پہلا, آخری);
      ورنہ
        len = snprintf (buf, حجم (buf), "%u-%u", پہلا, آخری);
    }
    اگر (unlikely (len < 0 || (size_t) len >= حجم (buf))) واپس;
    برآمد.extend (hb_بائٹیں_t (buf, len));
  }
}

ساکن خالی
format_لیبل (hb_لیبل_t لیبل_2, hb_سمتیہ_t<حرف> &برآمد)
{
  حرف buf[4];
  hb_لیبل_مقصد_string (لیبل_2, buf);

  غیرعلامتی len = 4;
  جب (len > 0 && buf[len - 1] == ' ')
    len--;

  برائے (غیرعلامتی i = 0; i < len; i++)
  {
    حرف c = buf[i];
    اگر (!((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '-' || c == '_'))
    {
      c = '_';
    }
    برآمد.push (c);
  }
}

ساکن خالی
format_لیبل_سیٹ (ثابت hb_سیٹ_t *سیٹ, hb_سمتیہ_t<حرف> &برآمد)
{
  منطقی پہلا_لیبل = سچ;
  برائے (hb_codepoint_t لیبل_2 : *سیٹ)
  {
    اگر (!پہلا_لیبل)
      برآمد.push (',');
    پہلا_لیبل = جھوٹ;

    format_لیبل (لیبل_2, برآمد);
  }
}

ساکن منطقی
is_سیٹ_default (ثابت hb_سیٹ_t *سیٹ, ثابت hb_سیٹ_t *default_سیٹ)
{
  واپس hb_سیٹ_is_inverted (سیٹ) == hb_سیٹ_is_inverted (default_سیٹ) &&
         hb_سیٹ_is_equal (سیٹ, default_سیٹ);
}

سانچہ <قسم_نام Formatter>
ساکن خالی
format_سیٹ_arg (ثابت hb_سیٹ_t *سیٹ,
                 ثابت حرف *opt_name,
                 Formatter &&formatter,
                 hb_سمتیہ_t<حرف> &sb)
{
  اگر (hb_سیٹ_is_inverted (سیٹ))
  {
    hb_سیٹ_t *excluded = hb_سیٹ_copy (سیٹ);
    hb_سیٹ_invert (excluded);

    hb_سمتیہ_t<حرف> opt;
    opt.extend (hb_بائٹیں_t ("--", 2));
    opt.extend (hb_بائٹیں_t (opt_name, لڑی_لمبائی (opt_name)));
    opt.extend (hb_بائٹیں_t ("=*", 2));
    opt.push ('\0');
    اگر (opt.arrayZ) _append_arg (sb, opt.arrayZ);

    اگر (!hb_سیٹ_is_خالی_خانے (excluded))
    {
      hb_سمتیہ_t<حرف> formatted;
      formatter (excluded, formatted);
      اگر (formatted.لمبائی)
      {
        hb_سمتیہ_t<حرف> opt2;
        opt2.extend (hb_بائٹیں_t ("--", 2));
        opt2.extend (hb_بائٹیں_t (opt_name, لڑی_لمبائی (opt_name)));
        opt2.extend (hb_بائٹیں_t ("-=", 2));
        opt2.extend (formatted.as_array ());
        opt2.push ('\0');
        اگر (opt2.arrayZ) _append_arg (sb, opt2.arrayZ);
      }
    }
    hb_سیٹ_destroy (excluded);
  }
  ورنہ اگر (hb_سیٹ_is_خالی_خانے (سیٹ))
  {
    hb_سمتیہ_t<حرف> opt;
    opt.extend (hb_بائٹیں_t ("--", 2));
    opt.extend (hb_بائٹیں_t (opt_name, لڑی_لمبائی (opt_name)));
    opt.extend (hb_بائٹیں_t ("-=*", 3));
    opt.push ('\0');
    اگر (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  ورنہ
  {
    hb_سمتیہ_t<حرف> formatted;
    formatter (سیٹ, formatted);
    اگر (formatted.لمبائی)
    {
      hb_سمتیہ_t<حرف> opt;
      opt.extend (hb_بائٹیں_t ("--", 2));
      opt.extend (hb_بائٹیں_t (opt_name, لڑی_لمبائی (opt_name)));
      opt.extend (hb_بائٹیں_t ("=", 1));
      opt.extend (formatted.as_array ());
      opt.push ('\0');
      اگر (opt.arrayZ) _append_arg (sb, opt.arrayZ);
    }
  }
}

ساکن خالی
format_نمبر_سیٹ_arg (ثابت hb_سیٹ_t *سیٹ,
                        ثابت حرف *opt_name,
                        hb_سمتیہ_t<حرف> &sb,
                        غیرعلامتی بنیاد = 10)
{
  format_سیٹ_arg (سیٹ, opt_name, [بنیاد] (ثابت hb_سیٹ_t *s, hb_سمتیہ_t<حرف> &برآمد) {
    format_نمبر_سیٹ (s, برآمد, بنیاد);
  }, sb);
}

ساکن خالی
format_لیبل_سیٹ_arg (ثابت hb_سیٹ_t *سیٹ,
                     ثابت حرف *opt_name,
                     hb_سمتیہ_t<حرف> &sb)
{
  format_سیٹ_arg (سیٹ, opt_name, format_لیبل_سیٹ, sb);
}

ساکن منطقی
_format_glyph_map_arg (ثابت hb_map_t &glyph_map,
                       hb_سمتیہ_t<حرف> &sb)
{
  اگر (glyph_map.is_empty ())
    واپس سچ;

  hb_سیٹ_t کلیدیں;
  hb_map_کلیدیں (&glyph_map, &کلیدیں);
  hb_سمتیہ_t<حرف> جوڑے;
  منطقی پہلا = سچ;
  برائے (hb_codepoint_t key : کلیدیں)
  {
    اگر (!پہلا)
      جوڑے.push (',');
    پہلا = جھوٹ;
    hb_codepoint_t val = hb_map_get (&glyph_map, key);
    حرف buf[64];
    صحیح len = snprintf (buf, حجم (buf), "%u:%u", key, val);
    اگر (unlikely (len < 0 || (size_t) len >= حجم (buf))) واپس جھوٹ;
    جوڑے.extend (hb_بائٹیں_t (buf, len));
  }
  اگر (جوڑے.لمبائی)
  {
    hb_سمتیہ_t<حرف> opt;
    opt.extend (hb_بائٹیں_t ("--gid-map=", 10));
    opt.extend (جوڑے.as_array ());
    opt.push ('\0');
    اگر (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  واپس سچ;
}

ساکن منطقی
_format_axes_location_arg (ثابت hb_hashmap_t<hb_لیبل_t, Triple> &axes_location,
                           hb_سمتیہ_t<حرف> &sb)
{
  اگر (axes_location.is_empty ())
    واپس سچ;

  hb_سمتیہ_t<hb_لیبل_t> axis_لیبل_فہرست;
  برائے (خودکار لیبل_2 : axes_location.کلیدیں ())
    axis_لیبل_فہرست.push (لیبل_2);
  axis_لیبل_فہرست.qsort ([] (ثابت hb_لیبل_t &a, ثابت hb_لیبل_t &b) { واپس a < b ? -1 : a > b ? 1 : 0; });

  hb_سمتیہ_t<حرف> specs;
  منطقی پہلا = سچ;
  برائے (hb_لیبل_t لیبل_2 : axis_لیبل_فہرست)
  {
    Triple triple = axes_location.get (لیبل_2);
    اگر (!پہلا)
      specs.push (',');
    پہلا = جھوٹ;

    hb_سمتیہ_t<حرف> لیبل_buf;
    format_لیبل (لیبل_2, لیبل_buf);

    حرف buf[128];
    صحیح len;
    اگر (triple.minimum == triple.درمیان && triple.درمیان == triple.زیادہ_سے_زیادہ)
    {
      len = snprintf (buf, حجم (buf), "%.*s=%g", (صحیح) لیبل_buf.لمبائی, لیبل_buf.arrayZ, (دوہرا) triple.درمیان);
    }
    ورنہ اگر (std::isnan (triple.minimum) && std::isnan (triple.درمیان) && std::isnan (triple.زیادہ_سے_زیادہ))
    {
      len = snprintf (buf, حجم (buf), "%.*s=drop", (صحیح) لیبل_buf.لمبائی, لیبل_buf.arrayZ);
    }
    ورنہ
    {
      len = snprintf (buf, حجم (buf), "%.*s=%g:%g:%g", (صحیح) لیبل_buf.لمبائی, لیبل_buf.arrayZ,
                      (دوہرا) triple.minimum, (دوہرا) triple.درمیان, (دوہرا) triple.زیادہ_سے_زیادہ);
    }
    اگر (unlikely (len < 0 || (size_t) len >= حجم (buf))) واپس جھوٹ;
    specs.extend (hb_بائٹیں_t (buf, len));
  }
  اگر (specs.لمبائی)
  {
    hb_سمتیہ_t<حرف> opt;
    opt.extend (hb_بائٹیں_t ("--variations=", 13));
    opt.extend (specs.as_array ());
    opt.push ('\0');
    اگر (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  واپس سچ;
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
hb_subset_داخلہ_مقصد_string_or_fail (hb_subset_داخلہ_t *داخلہ)
{
  اگر (unlikely (!داخلہ || داخلہ->اندر_error ()))
    واپس خالی_اشاریہ;

  hb_سمتیہ_t<حرف> sb;

  ساخت flag_option_t {
    hb_subset_flags_t flag;
    حرف option[32];
  };
  ساکن ثابت flag_option_t flag_options[] = {
    { HB_SUBSET_FLAGS_NO_HINTING, "no-hinting" },
    { HB_SUBSET_FLAGS_RETAIN_GIDS, "retain-gids" },
    { HB_SUBSET_FLAGS_DESUBROUTINIZE, "desubroutinize" },
    { HB_SUBSET_FLAGS_NAME_LEGACY, "name-legacy" },
    { HB_SUBSET_FLAGS_سیٹ_OVERLAPS_FLAG, "set-overlaps-flag" },
    { HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED, "passthrough-tables" },
    { HB_SUBSET_FLAGS_NOTDEF_OUTLINE, "notdef-outline" },
    { HB_SUBSET_FLAGS_GLYPH_نام, "glyph-names" },
    { HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES, "no-prune-unicode-ranges" },
    { HB_SUBSET_FLAGS_NO_LAYOUT_احاطہ, "no-layout-closure" },
    { HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS, "optimize" },
    { HB_SUBSET_FLAGS_NO_BIDI_احاطہ, "no-bidi-closure" },
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    { HB_SUBSET_FLAGS_IFTB_REQUIREMENTS, "iftb-requirements" },
    { HB_SUBSET_FLAGS_RETAIN_عدد_GLYPHS, "retain-num-glyphs" },
#اگر_ختم
    { HB_SUBSET_FLAGS_DOWNGRADE_CFF2, "downgrade-cff2" },
  };

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
  static_assert (حجم (flag_options) / حجم (flag_options[0]) == 15,
                 "Check all flags in hb_subset_flags_t are handled here.");
#ورنہ
  static_assert (حجم (flag_options) / حجم (flag_options[0]) == 13,
                 "Check all flags in hb_subset_flags_t are handled here.");
#اگر_ختم

  برائے (ثابت خودکار &fo : flag_options)
  {
    اگر (داخلہ->flags & fo.flag)
    {
      حرف buf[36];
      snprintf (buf, حجم (buf), "--%s", fo.option);
      _append_arg (sb, buf);
    }
  }

  hb_subset_داخلہ_t *default_داخلہ = hb_subset_داخلہ_create_or_fail ();
  اگر (unlikely (!default_داخلہ))
    واپس خالی_اشاریہ;

  // Serialize all sets if they are not equal to their default setting:
#تعریف_کرو PROCESS_نمبر_سیٹ(میدان, opt_name, ...) \
  اگر (!is_سیٹ_default (داخلہ->sets.میدان, default_داخلہ->sets.میدان)) \
    format_نمبر_سیٹ_arg (داخلہ->sets.میدان, opt_name, sb, ##__VA_ARGS__)

#تعریف_کرو PROCESS_لیبل_سیٹ(میدان, opt_name) \
  اگر (!is_سیٹ_default (داخلہ->sets.میدان, default_داخلہ->sets.میدان)) \
    format_لیبل_سیٹ_arg (داخلہ->sets.میدان, opt_name, sb)

  PROCESS_نمبر_سیٹ (unicodes, "unicodes", 16);
  PROCESS_نمبر_سیٹ (glyphs, "gids");
  PROCESS_نمبر_سیٹ (name_ids, "name-IDs");
  PROCESS_نمبر_سیٹ (name_languages, "name-languages");
  PROCESS_لیبل_سیٹ (layout_features, "layout-features");
  PROCESS_لیبل_سیٹ (layout_scripts, "layout-scripts");
  PROCESS_لیبل_سیٹ (drop_tables, "drop-tables");
  PROCESS_لیبل_سیٹ (no_subset_tables, "passthrough-tables");

#تعریف_منسوخ PROCESS_نمبر_سیٹ
#تعریف_منسوخ PROCESS_لیبل_سیٹ

  hb_subset_داخلہ_destroy (default_داخلہ);

  static_assert (حجم (داخلہ->sets) / حجم (hb_سیٹ_t*) == 8,
                 "Check all sets in hb_subset_input_t::sets_t are handled here.");

  اگر (unlikely (!_format_glyph_map_arg (داخلہ->glyph_map, sb)))
    واپس خالی_اشاریہ;

  اگر (unlikely (!_format_axes_location_arg (داخلہ->axes_location, sb)))
    واپس خالی_اشاریہ;

  sb.push ('\0');
  اگر (unlikely (sb.اندر_error ()))
    واپس خالی_اشاریہ;

  غیرعلامتی len = 0;
  حرف *buf = sb.steal (&len);
  اگر (unlikely (!buf))
    واپس خالی_اشاریہ;

  واپس hb_blob_create_or_fail (buf, len, HB_حافظہ_منوال_WRITABLE, buf, hb_free);
}
#اگر_ختم

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
hb_subset_preprocess (hb_face_t *ماخذ)
{
  hb_subset_داخلہ_t* داخلہ = hb_subset_داخلہ_create_or_fail ();
  اگر (!داخلہ)
    واپس hb_face_reference (ماخذ);

  hb_subset_داخلہ_keep_everything (داخلہ);

  داخلہ->attach_accelerator_ڈیٹا = سچ;

  // Always use long loca in the preprocessed version. This allows
  // us to store the glyph bytes unpadded which allows the future subset
  // operation to run faster by skipping the trim padding step.
  داخلہ->force_long_loca = سچ;

  hb_face_t* new_ماخذ = hb_subset_or_fail (ماخذ, داخلہ);
  hb_subset_داخلہ_destroy (داخلہ);

  اگر (!new_ماخذ) {
    DEBUG_پیغام (SUBSET, خالی_اشاریہ, "Preprocessing failed due to subset failure.");
    واپس hb_face_reference (ماخذ);
  }

  واپس new_ماخذ;
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
hb_subset_داخلہ_old_مقصد_new_glyph_mapping (hb_subset_داخلہ_t *داخلہ)
{
  واپس &داخلہ->glyph_map;
}

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
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
                                     غیرعلامتی            platform_id,
                                     غیرعلامتی            encoding_id,
                                     غیرعلامتی            language_id,
                                     ثابت حرف         *name_str,
                                     صحیح                 str_len /* -1 means nul-terminated */)
{
  اگر (!name_str)
  {
    str_len = 0;
  }
  ورنہ اگر (str_len == -1)
  {
      str_len = لڑی_لمبائی (name_str);
  }

  hb_بائٹیں_t name_بائٹیں (خالی_اشاریہ, 0);
  اگر (str_len)
  {
    اگر (platform_id == 1)
    {
      ثابت uint8_t *src = reinterpret_cast<ثابت uint8_t*> (name_str);
      ثابت uint8_t *src_انتہا = src + str_len;

      hb_codepoint_t unicode;
      ثابت hb_codepoint_t replacement = HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT;
      جب (src < src_انتہا)
      {
        src = hb_utf8_t::next (src, src_انتہا, &unicode, replacement);
        اگر (unicode >= 0x0080u)
        {
          // Non-ascii character detected, ignored...
          واپس جھوٹ;
        }
      }
    }
    حرف *override_name = (حرف *) hb_malloc (str_len);
    اگر (unlikely (!override_name)) واپس جھوٹ;

    hb_memcpy (override_name, name_str, str_len);
    name_بائٹیں = hb_بائٹیں_t (override_name, str_len);
  }
  داخلہ->name_جدول_overrides.سیٹ (hb_ot_name_ریکارڈ_ids_t (platform_id, encoding_id, language_id, name_id), name_بائٹیں);
  واپس سچ;
}
#اگر_ختم
