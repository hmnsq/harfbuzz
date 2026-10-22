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

#অন্তর্ভাৱ "hb-subset-instancer-solver.hh"
#অন্তর্ভাৱ "hb-subset.hh"
#অন্তর্ভাৱ "hb-set.hh"
#অন্তর্ভাৱ "hb-utf.hh"


hb_subset_প্ৰবেশ_t::hb_subset_প্ৰবেশ_t ()
{
  বাবে (auto& set : sets_iter ())
    set = hb::shared_ptr<hb_set_t> (hb_set_create ());

  যদি (in_error ())
    ঘূৰাই;

  flags = HB_SUBSET_FLAGS_DEFAULT;

  hb_set_add_range (sets.name_ids, 0, 6);
  hb_set_add (sets.name_languages, 0x0409);

  hb_tag_t default_drop_tables[] = {
    // Layout disabled by default
    HB_TAG ('m', 'o', 'r', 'x'),
    HB_TAG ('m', 'o', 'r', 't'),
    HB_TAG ('k', 'e', 'r', 'x'),
    HB_TAG ('k', 'e', 'r', 'n'),

    // Copied from fontTools:
    HB_TAG ('J', 'S', 'T', 'F'),
    HB_TAG ('D', 'S', 'I', 'G'),
    HB_TAG ('E', 'B', 'D', 'T'),
    HB_TAG ('E', 'B', 'L', 'C'),
    HB_TAG ('E', 'B', 'S', 'C'),
    HB_TAG ('S', 'V', 'G', ' '),
    HB_TAG ('P', 'C', 'L', 'T'),
    HB_TAG ('L', 'T', 'S', 'H'),
    // Graphite tables
    HB_TAG ('F', 'e', 'a', 't'),
    HB_TAG ('G', 'l', 'a', 't'),
    HB_TAG ('G', 'l', 'o', 'c'),
    HB_TAG ('S', 'i', 'l', 'f'),
    HB_TAG ('S', 'i', 'l', 'l'),
  };
  sets.drop_tables->add_array (default_drop_tables, ARRAY_LENGTH (default_drop_tables));

  hb_tag_t default_no_subset_tables[] = {
    HB_TAG ('g', 'a', 's', 'p'),
    HB_TAG ('f', 'p', 'g', 'm'),
    HB_TAG ('p', 'r', 'e', 'p'),
    HB_TAG ('V', 'D', 'M', 'X'),
    HB_TAG ('D', 'S', 'I', 'G'),
  };
  sets.no_subset_tables->add_array (default_no_subset_tables,
					 ARRAY_LENGTH (default_no_subset_tables));

  //copied from _layout_features_groups in fonttools
  hb_tag_t default_layout_features[] = {
    // default shaper
    // common
    HB_TAG ('r', 'v', 'r', 'n'),
    HB_TAG ('c', 'c', 'm', 'p'),
    HB_TAG ('l', 'i', 'g', 'a'),
    HB_TAG ('l', 'o', 'c', 'l'),
    HB_TAG ('m', 'a', 'r', 'k'),
    HB_TAG ('m', 'k', 'm', 'k'),
    HB_TAG ('r', 'l', 'i', 'g'),

    //fractions
    HB_TAG ('f', 'r', 'a', 'c'),
    HB_TAG ('n', 'u', 'm', 'r'),
    HB_TAG ('d', 'n', 'o', 'm'),

    //horizontal
    HB_TAG ('c', 'a', 'l', 't'),
    HB_TAG ('c', 'l', 'i', 'g'),
    HB_TAG ('c', 'u', 'r', 's'),
    HB_TAG ('k', 'e', 'r', 'n'),
    HB_TAG ('r', 'c', 'l', 't'),

    //vertical
    HB_TAG ('v', 'a', 'l', 't'),
    HB_TAG ('v', 'e', 'r', 't'),
    HB_TAG ('v', 'k', 'r', 'n'),
    HB_TAG ('v', 'p', 'a', 'l'),
    HB_TAG ('v', 'r', 't', '2'),

    //ltr
    HB_TAG ('l', 't', 'r', 'a'),
    HB_TAG ('l', 't', 'r', 'm'),

    //rtl
    HB_TAG ('r', 't', 'l', 'a'),
    HB_TAG ('r', 't', 'l', 'm'),

    //random
    HB_TAG ('r', 'a', 'n', 'd'),

    //justify
    HB_TAG ('j', 'a', 'l', 't'), // HarfBuzz doesn't use; others might

    //East Asian spacing
    HB_TAG ('c', 'h', 'w', 's'),
    HB_TAG ('v', 'c', 'h', 'w'),
    HB_TAG ('h', 'a', 'l', 't'),
    HB_TAG ('v', 'h', 'a', 'l'),
    HB_TAG ('p', 'a', 'l', 't'),

    //private
    HB_TAG ('H', 'a', 'r', 'f'),
    HB_TAG ('H', 'A', 'R', 'F'),
    HB_TAG ('B', 'u', 'z', 'z'),
    HB_TAG ('B', 'U', 'Z', 'Z'),

    //shapers

    //arabic
    HB_TAG ('i', 'n', 'i', 't'),
    HB_TAG ('m', 'e', 'd', 'i'),
    HB_TAG ('f', 'i', 'n', 'a'),
    HB_TAG ('i', 's', 'o', 'l'),
    HB_TAG ('m', 'e', 'd', '2'),
    HB_TAG ('f', 'i', 'n', '2'),
    HB_TAG ('f', 'i', 'n', '3'),
    HB_TAG ('c', 's', 'w', 'h'),
    HB_TAG ('m', 's', 'e', 't'),
    HB_TAG ('s', 't', 'c', 'h'),

    //hangul
    HB_TAG ('l', 'j', 'm', 'o'),
    HB_TAG ('v', 'j', 'm', 'o'),
    HB_TAG ('t', 'j', 'm', 'o'),

    //tibetan
    HB_TAG ('a', 'b', 'v', 's'),
    HB_TAG ('b', 'l', 'w', 's'),
    HB_TAG ('a', 'b', 'v', 'm'),
    HB_TAG ('b', 'l', 'w', 'm'),

    //indic
    HB_TAG ('n', 'u', 'k', 't'),
    HB_TAG ('a', 'k', 'h', 'n'),
    HB_TAG ('r', 'p', 'h', 'f'),
    HB_TAG ('r', 'k', 'r', 'f'),
    HB_TAG ('p', 'r', 'e', 'f'),
    HB_TAG ('b', 'l', 'w', 'f'),
    HB_TAG ('h', 'a', 'l', 'f'),
    HB_TAG ('a', 'b', 'v', 'f'),
    HB_TAG ('p', 's', 't', 'f'),
    HB_TAG ('c', 'f', 'a', 'r'),
    HB_TAG ('v', 'a', 't', 'u'),
    HB_TAG ('c', 'j', 'c', 't'),
    HB_TAG ('i', 'n', 'i', 't'),
    HB_TAG ('p', 'r', 'e', 's'),
    HB_TAG ('a', 'b', 'v', 's'),
    HB_TAG ('b', 'l', 'w', 's'),
    HB_TAG ('p', 's', 't', 's'),
    HB_TAG ('h', 'a', 'l', 'n'),
    HB_TAG ('d', 'i', 's', 't'),
    HB_TAG ('a', 'b', 'v', 'm'),
    HB_TAG ('b', 'l', 'w', 'm'),
  };

  sets.layout_features->add_array (default_layout_features, ARRAY_LENGTH (default_layout_features));

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
hb_subset_প্ৰবেশ_t *
hb_subset_প্ৰবেশ_create_or_fail (শূন্য)
{
  hb_subset_প্ৰবেশ_t *প্ৰবেশ = hb_object_create<hb_subset_প্ৰবেশ_t>();

  যদি (unlikely (!প্ৰবেশ))
    ঘূৰাই nullptr;

  যদি (প্ৰবেশ->in_error ())
  {
    hb_subset_প্ৰবেশ_destroy (প্ৰবেশ);
    ঘূৰাই nullptr;
  }

  ঘূৰাই প্ৰবেশ;
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
hb_subset_প্ৰবেশ_t *
hb_subset_প্ৰবেশ_reference (hb_subset_প্ৰবেশ_t *প্ৰবেশ)
{
  ঘূৰাই hb_object_reference (প্ৰবেশ);
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
শূন্য
hb_subset_প্ৰবেশ_destroy (hb_subset_প্ৰবেশ_t *প্ৰবেশ)
{
  যদি (!hb_object_destroy (প্ৰবেশ)) ঘূৰাই;

  hb_free (প্ৰবেশ);
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
HB_EXTERN hb_set_t *
hb_subset_প্ৰবেশ_unicode_set (hb_subset_প্ৰবেশ_t *প্ৰবেশ)
{
  ঘূৰাই প্ৰবেশ->sets.unicodes;
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
HB_EXTERN hb_set_t *
hb_subset_প্ৰবেশ_glyph_set (hb_subset_প্ৰবেশ_t *প্ৰবেশ)
{
  ঘূৰাই প্ৰবেশ->sets.glyphs;
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
HB_EXTERN hb_set_t *
hb_subset_প্ৰবেশ_set (hb_subset_প্ৰবেশ_t *প্ৰবেশ, hb_subset_sets_t set_type)
{
  ঘূৰাই প্ৰবেশ->sets_iter () [set_type];
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
hb_subset_প্ৰবেশ_get_flags (hb_subset_প্ৰবেশ_t *প্ৰবেশ)
{
  ঘূৰাই (hb_subset_flags_t) প্ৰবেশ->flags;
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
HB_EXTERN শূন্য
hb_subset_প্ৰবেশ_set_flags (hb_subset_প্ৰবেশ_t *প্ৰবেশ,
			   unsigned value)
{
  প্ৰবেশ->flags = (hb_subset_flags_t) value;
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
hb_subset_প্ৰবেশ_set_user_data (hb_subset_প্ৰবেশ_t  *প্ৰবেশ,
			       hb_user_data_চাবি_t *key,
			       শূন্য *		   data,
			       hb_destroy_func_t   destroy,
			       hb_bool_t	   replace)
{
  ঘূৰাই hb_object_set_user_data (প্ৰবেশ, key, data, destroy, replace);
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
শূন্য *
hb_subset_প্ৰবেশ_get_user_data (ধ্রুৱ hb_subset_প্ৰবেশ_t *প্ৰবেশ,
			       hb_user_data_চাবি_t     *key)
{
  ঘূৰাই hb_object_get_user_data (প্ৰবেশ, key);
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
শূন্য
hb_subset_প্ৰবেশ_keep_everything (hb_subset_প্ৰবেশ_t *প্ৰবেশ)
{
  ধ্রুৱ hb_subset_sets_t indices[] = {HB_SUBSET_SETS_UNICODE,
				      HB_SUBSET_SETS_GLYPH_INDEX,
				      HB_SUBSET_SETS_NAME_ID,
				      HB_SUBSET_SETS_NAME_LANG_ID,
				      HB_SUBSET_SETS_LAYOUT_FEATURE_TAG,
				      HB_SUBSET_SETS_LAYOUT_SCRIPT_TAG};

  বাবে (auto idx : hb_iter (indices))
  {
    hb_set_t *set = hb_subset_প্ৰবেশ_set (প্ৰবেশ, idx);
    hb_set_clear (set);
    hb_set_invert (set);
  }

  // Don't drop any tables
  hb_set_clear (hb_subset_প্ৰবেশ_set (প্ৰবেশ, HB_SUBSET_SETS_DROP_TABLE_TAG));

  hb_subset_প্ৰবেশ_set_flags (প্ৰবেশ,
			     HB_SUBSET_FLAGS_NOTDEF_OUTLINE |
			     HB_SUBSET_FLAGS_GLYPH_NAMES |
			     HB_SUBSET_FLAGS_NAME_LEGACY |
			     HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES |
                             HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED);
}

#ifndef HB_NO_VAR
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
hb_subset_প্ৰবেশ_pin_all_axes_to_default (hb_subset_প্ৰবেশ_t  *প্ৰবেশ,
                                         hb_face_t          *face)
{
  unsigned axis_গুনতি = hb_ot_var_get_axis_গুনতি (face);
  যদি (!axis_গুনতি) ঘূৰাই মিছা;

  hb_ot_var_axis_info_t *axis_infos = (hb_ot_var_axis_info_t *) hb_calloc (axis_গুনতি, পরিমাণ (hb_ot_var_axis_info_t));
  যদি (unlikely (!axis_infos)) ঘূৰাই মিছা;

  (শূন্য) hb_ot_var_get_axis_infos (face, 0, &axis_গুনতি, axis_infos);

  বাবে (unsigned i = 0; i < axis_গুনতি; i++)
  {
    hb_tag_t axis_tag = axis_infos[i].tag;
    দ্ৱিগুণ default_val = (দ্ৱিগুণ) axis_infos[i].default_মান;
    যদি (!প্ৰবেশ->axes_location.set (axis_tag, Triple (default_val, default_val, default_val)))
    {
      hb_free (axis_infos);
      ঘূৰাই মিছা;
    }
  }
  hb_free (axis_infos);
  ঘূৰাই সঁচা;
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
hb_subset_প্ৰবেশ_pin_axis_to_default (hb_subset_প্ৰবেশ_t  *প্ৰবেশ,
                                     hb_face_t          *face,
                                     hb_tag_t            axis_tag)
{
  hb_ot_var_axis_info_t axis_info;
  যদি (!hb_ot_var_find_axis_info (face, axis_tag, &axis_info))
    ঘূৰাই মিছা;

  দ্ৱিগুণ default_val = (দ্ৱিগুণ) axis_info.default_মান;
  ঘূৰাই প্ৰবেশ->axes_location.set (axis_tag, Triple (default_val, default_val, default_val));
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
hb_subset_প্ৰবেশ_pin_axis_location (hb_subset_প্ৰবেশ_t  *প্ৰবেশ,
                                   hb_face_t          *face,
                                   hb_tag_t            axis_tag,
                                   ভিন্ন               axis_মান)
{
  hb_ot_var_axis_info_t axis_info;
  যদি (!hb_ot_var_find_axis_info (face, axis_tag, &axis_info))
    ঘূৰাই মিছা;

  দ্ৱিগুণ val = hb_clamp((দ্ৱিগুণ) axis_মান, (দ্ৱিগুণ) axis_info.min_মান, (দ্ৱিগুণ) axis_info.max_মান);
  ঘূৰাই প্ৰবেশ->axes_location.set (axis_tag, Triple (val, val, val));
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
hb_subset_প্ৰবেশ_set_axis_range (hb_subset_প্ৰবেশ_t  *প্ৰবেশ,
                                hb_face_t          *face,
                                hb_tag_t            axis_tag,
                                ভিন্ন               axis_min_মান,
                                ভিন্ন               axis_max_মান,
                                ভিন্ন               axis_def_মান)
{
  hb_ot_var_axis_info_t axis_info;
  যদি (!hb_ot_var_find_axis_info (face, axis_tag, &axis_info))
    ঘূৰাই মিছা;

  ভিন্ন min = !std::isnan(axis_min_মান) ? axis_min_মান : axis_info.min_মান;
  ভিন্ন max = !std::isnan(axis_max_মান) ? axis_max_মান : axis_info.max_মান;
  ভিন্ন def = !std::isnan(axis_def_মান) ? axis_def_মান : axis_info.default_মান;

  যদি (min > max)
    ঘূৰাই মিছা;

  ভিন্ন new_min_val = hb_clamp(min, axis_info.min_মান, axis_info.max_মান);
  ভিন্ন new_max_val = hb_clamp(max, axis_info.min_মান, axis_info.max_মান);
  ভিন্ন new_default_val = hb_clamp(def, new_min_val, new_max_val);
  ঘূৰাই প্ৰবেশ->axes_location.set (axis_tag, Triple ((দ্ৱিগুণ) new_min_val, (দ্ৱিগুণ) new_default_val, (দ্ৱিগুণ) new_max_val));
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
hb_subset_প্ৰবেশ_get_axis_range (hb_subset_প্ৰবেশ_t  *প্ৰবেশ,
				hb_tag_t            axis_tag,
				ভিন্ন              *axis_min_মান,
				ভিন্ন              *axis_max_মান,
				ভিন্ন              *axis_def_মান)

{
  Triple* triple;
  যদি (!প্ৰবেশ->axes_location.has(axis_tag, &triple)) {
    ঘূৰাই মিছা;
  }

  *axis_min_মান = triple->minimum;
  *axis_def_মান = triple->মধ্য;
  *axis_max_মান = triple->maximum;
  ঘূৰাই সঁচা;
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
hb_subset_axis_range_from_string (ধ্রুৱ ৱর্ণ *str, পূর্ণাঙ্ক len,
                                  ভিন্ন *axis_min_মান,
                                  ভিন্ন *axis_max_মান,
                                  ভিন্ন *axis_def_মান)
{
  যদি (len < 0)
    len = strlen (str);

  ধ্রুৱ ৱর্ণ *end = str + len;
  ধ্রুৱ ৱর্ণ* part = strpbrk (str, ":");
  যদি (!part)
  {
    // Single value.
    যদি (strcmp (str, "drop") == 0)
    {
      *axis_min_মান = NAN;
      *axis_def_মান = NAN;
      *axis_max_মান = NAN;
      ঘূৰাই সঁচা;
    }

    দ্ৱিগুণ v;
    যদি (!hb_parse_double (&str, end, &v)) ঘূৰাই মিছা;

    *axis_min_মান = v;
    *axis_def_মান = v;
    *axis_max_মান = v;
    ঘূৰাই সঁচা;
  }

  ভিন্ন values[3];
  পূর্ণাঙ্ক গুনতি = 0;
  বাবে (পূর্ণাঙ্ক i = 0; i < 3; i++) {
    গুনতি++;
    যদি (!*str || part == str)
    {
      values[i] = NAN;

      যদি (part == রিক্ত) ভঙ্গ;
      str = part + 1;
      part = strpbrk (str, ":");
      অনুৱর্ত;
    }

    দ্ৱিগুণ v;
    ধ্রুৱ ৱর্ণ *piece_end = part ? part : end;
    যদি (!hb_parse_double (&str, piece_end, &v)) ঘূৰাই মিছা;
    values[i] = v;

    যদি (part == রিক্ত) ভঙ্গ;
    str = part + 1;
    part = strpbrk (str, ":");
  }

  যদি (গুনতি == 2)
  {
    *axis_min_মান = values[0];
    *axis_def_মান = NAN;
    *axis_max_মান = values[1];
    ঘূৰাই সঁচা;
  }
  অন্যথা যদি (গুনতি == 3)
  {
    *axis_min_মান = values[0];
    *axis_def_মান = values[1];
    *axis_max_মান = values[2];
    ঘূৰাই সঁচা;
  }
  ঘূৰাই মিছা;
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
HB_EXTERN শূন্য
hb_subset_axis_range_to_string (hb_subset_প্ৰবেশ_t *প্ৰবেশ,
                                hb_tag_t axis_tag,
                                ৱর্ণ *buf, unsigned size)
{
  যদি (unlikely (!size)) ঘূৰাই;
  Triple* triple;
  যদি (!প্ৰবেশ->axes_location.has(axis_tag, &triple)) {
    ঘূৰাই;
  }

  ৱর্ণ s[128];
  unsigned len = 0;

  hb_locale_t clocale HB_UNUSED;
  hb_locale_t oldlocale HB_UNUSED;
  oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", রিক্ত));
  len += hb_max (0, snprintf (s, ARRAY_LENGTH (s) - len, "%g", (দ্ৱিগুণ) triple->minimum));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_LENGTH (s) - len, "%g", (দ্ৱিগুণ) triple->মধ্য));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_LENGTH (s) - len, "%g", (দ্ৱিগুণ) triple->maximum));
  (শূন্য) hb_uselocale (((শূন্য) freelocale (clocale), oldlocale));

  প্রতিজ্ঞা (len < ARRAY_LENGTH (s));
  len = hb_min (len, size - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}
#endif

#ifdef HB_EXPERIMENTAL_API
স্থির শূন্য
_append_arg (hb_vector_t<ৱর্ণ> &sb, ধ্রুৱ ৱর্ণ *arg)
{
  যদি (sb.in_error ()) ঘূৰাই;
  যদি (sb.length)
    sb.push (' ');
  unsigned len = strlen (arg);
  sb.extend (hb_bytes_t (arg, len));
}
স্থির শূন্য
format_সংখ্যা_set (ধ্রুৱ hb_set_t *set, hb_vector_t<ৱর্ণ> &out, unsigned base = 10)
{
  প্রতিজ্ঞা (base == 10 || base == 16);
  যদি (unlikely (base != 10 && base != 16)) ঘূৰাই;

  hb_codepoint_t first = HB_SET_মান_INVALID, last = HB_SET_মান_INVALID;
  তার্কিক first_range = সঁচা;
  যেতিয়ালৈ (hb_set_next_range (set, &first, &last))
  {
    যদি (!first_range)
      out.push (',');
    first_range = মিছা;

    ৱর্ণ buf[64];
    পূর্ণাঙ্ক len;
    যদি (first == last)
    {
      যদি (base == 16)
        len = snprintf (buf, পরিমাণ (buf), "%X", first);
      অন্যথা
        len = snprintf (buf, পরিমাণ (buf), "%u", first);
    }
    অন্যথা
    {
      যদি (base == 16)
        len = snprintf (buf, পরিমাণ (buf), "%X-%X", first, last);
      অন্যথা
        len = snprintf (buf, পরিমাণ (buf), "%u-%u", first, last);
    }
    যদি (unlikely (len < 0 || (size_t) len >= পরিমাণ (buf))) ঘূৰাই;
    out.extend (hb_bytes_t (buf, len));
  }
}

স্থির শূন্য
_format_tag (hb_tag_t tag, hb_vector_t<ৱর্ণ> &out)
{
  ৱর্ণ buf[4];
  hb_tag_to_string (tag, buf);

  unsigned len = 4;
  যেতিয়ালৈ (len > 0 && buf[len - 1] == ' ')
    len--;

  বাবে (unsigned i = 0; i < len; i++)
  {
    ৱর্ণ c = buf[i];
    যদি (!((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '-' || c == '_'))
    {
      c = '_';
    }
    out.push (c);
  }
}

স্থির শূন্য
_format_tag_set (ধ্রুৱ hb_set_t *set, hb_vector_t<ৱর্ণ> &out)
{
  তার্কিক first_tag = সঁচা;
  বাবে (hb_codepoint_t tag : *set)
  {
    যদি (!first_tag)
      out.push (',');
    first_tag = মিছা;

    _format_tag (tag, out);
  }
}

স্থির তার্কিক
_is_set_default (ধ্রুৱ hb_set_t *set, ধ্রুৱ hb_set_t *default_set)
{
  ঘূৰাই hb_set_is_inverted (set) == hb_set_is_inverted (default_set) &&
         hb_set_is_equal (set, default_set);
}

template <typename Formatter>
স্থির শূন্য
_format_set_arg (ধ্রুৱ hb_set_t *set,
                 ধ্রুৱ ৱর্ণ *opt_name,
                 Formatter &&formatter,
                 hb_vector_t<ৱর্ণ> &sb)
{
  যদি (hb_set_is_inverted (set))
  {
    hb_set_t *excluded = hb_set_copy (set);
    hb_set_invert (excluded);

    hb_vector_t<ৱর্ণ> opt;
    opt.extend (hb_bytes_t ("--", 2));
    opt.extend (hb_bytes_t (opt_name, strlen (opt_name)));
    opt.extend (hb_bytes_t ("=*", 2));
    opt.push ('\0');
    যদি (opt.arrayZ) _append_arg (sb, opt.arrayZ);

    যদি (!hb_set_is_empty (excluded))
    {
      hb_vector_t<ৱর্ণ> formatted;
      formatter (excluded, formatted);
      যদি (formatted.length)
      {
        hb_vector_t<ৱর্ণ> opt2;
        opt2.extend (hb_bytes_t ("--", 2));
        opt2.extend (hb_bytes_t (opt_name, strlen (opt_name)));
        opt2.extend (hb_bytes_t ("-=", 2));
        opt2.extend (formatted.as_array ());
        opt2.push ('\0');
        যদি (opt2.arrayZ) _append_arg (sb, opt2.arrayZ);
      }
    }
    hb_set_destroy (excluded);
  }
  অন্যথা যদি (hb_set_is_empty (set))
  {
    hb_vector_t<ৱর্ণ> opt;
    opt.extend (hb_bytes_t ("--", 2));
    opt.extend (hb_bytes_t (opt_name, strlen (opt_name)));
    opt.extend (hb_bytes_t ("-=*", 3));
    opt.push ('\0');
    যদি (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  অন্যথা
  {
    hb_vector_t<ৱর্ণ> formatted;
    formatter (set, formatted);
    যদি (formatted.length)
    {
      hb_vector_t<ৱর্ণ> opt;
      opt.extend (hb_bytes_t ("--", 2));
      opt.extend (hb_bytes_t (opt_name, strlen (opt_name)));
      opt.extend (hb_bytes_t ("=", 1));
      opt.extend (formatted.as_array ());
      opt.push ('\0');
      যদি (opt.arrayZ) _append_arg (sb, opt.arrayZ);
    }
  }
}

স্থির শূন্য
format_সংখ্যা_set_arg (ধ্রুৱ hb_set_t *set,
                        ধ্রুৱ ৱর্ণ *opt_name,
                        hb_vector_t<ৱর্ণ> &sb,
                        unsigned base = 10)
{
  _format_set_arg (set, opt_name, [base] (ধ্রুৱ hb_set_t *s, hb_vector_t<ৱর্ণ> &out) {
    format_সংখ্যা_set (s, out, base);
  }, sb);
}

স্থির শূন্য
_format_tag_set_arg (ধ্রুৱ hb_set_t *set,
                     ধ্রুৱ ৱর্ণ *opt_name,
                     hb_vector_t<ৱর্ণ> &sb)
{
  _format_set_arg (set, opt_name, _format_tag_set, sb);
}

স্থির তার্কিক
_format_glyph_map_arg (ধ্রুৱ hb_map_t &glyph_map,
                       hb_vector_t<ৱর্ণ> &sb)
{
  যদি (glyph_map.is_empty ())
    ঘূৰাই সঁচা;

  hb_set_t চাবিগুলি;
  hb_map_চাবিগুলি (&glyph_map, &চাবিগুলি);
  hb_vector_t<ৱর্ণ> pairs;
  তার্কিক first = সঁচা;
  বাবে (hb_codepoint_t key : চাবিগুলি)
  {
    যদি (!first)
      pairs.push (',');
    first = মিছা;
    hb_codepoint_t val = hb_map_get (&glyph_map, key);
    ৱর্ণ buf[64];
    পূর্ণাঙ্ক len = snprintf (buf, পরিমাণ (buf), "%u:%u", key, val);
    যদি (unlikely (len < 0 || (size_t) len >= পরিমাণ (buf))) ঘূৰাই মিছা;
    pairs.extend (hb_bytes_t (buf, len));
  }
  যদি (pairs.length)
  {
    hb_vector_t<ৱর্ণ> opt;
    opt.extend (hb_bytes_t ("--gid-map=", 10));
    opt.extend (pairs.as_array ());
    opt.push ('\0');
    যদি (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  ঘূৰাই সঁচা;
}

স্থির তার্কিক
_format_axes_location_arg (ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> &axes_location,
                           hb_vector_t<ৱর্ণ> &sb)
{
  যদি (axes_location.is_empty ())
    ঘূৰাই সঁচা;

  hb_vector_t<hb_tag_t> axis_tags;
  বাবে (auto tag : axes_location.চাবিগুলি ())
    axis_tags.push (tag);
  axis_tags.qsort ([] (ধ্রুৱ hb_tag_t &a, ধ্রুৱ hb_tag_t &b) { ঘূৰাই a < b ? -1 : a > b ? 1 : 0; });

  hb_vector_t<ৱর্ণ> specs;
  তার্কিক first = সঁচা;
  বাবে (hb_tag_t tag : axis_tags)
  {
    Triple triple = axes_location.get (tag);
    যদি (!first)
      specs.push (',');
    first = মিছা;

    hb_vector_t<ৱর্ণ> tag_buf;
    _format_tag (tag, tag_buf);

    ৱর্ণ buf[128];
    পূর্ণাঙ্ক len;
    যদি (triple.minimum == triple.মধ্য && triple.মধ্য == triple.maximum)
    {
      len = snprintf (buf, পরিমাণ (buf), "%.*s=%g", (পূর্ণাঙ্ক) tag_buf.length, tag_buf.arrayZ, (দ্ৱিগুণ) triple.মধ্য);
    }
    অন্যথা যদি (std::isnan (triple.minimum) && std::isnan (triple.মধ্য) && std::isnan (triple.maximum))
    {
      len = snprintf (buf, পরিমাণ (buf), "%.*s=drop", (পূর্ণাঙ্ক) tag_buf.length, tag_buf.arrayZ);
    }
    অন্যথা
    {
      len = snprintf (buf, পরিমাণ (buf), "%.*s=%g:%g:%g", (পূর্ণাঙ্ক) tag_buf.length, tag_buf.arrayZ,
                      (দ্ৱিগুণ) triple.minimum, (দ্ৱিগুণ) triple.মধ্য, (দ্ৱিগুণ) triple.maximum);
    }
    যদি (unlikely (len < 0 || (size_t) len >= পরিমাণ (buf))) ঘূৰাই মিছা;
    specs.extend (hb_bytes_t (buf, len));
  }
  যদি (specs.length)
  {
    hb_vector_t<ৱর্ণ> opt;
    opt.extend (hb_bytes_t ("--variations=", 13));
    opt.extend (specs.as_array ());
    opt.push ('\0');
    যদি (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  ঘূৰাই সঁচা;
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
hb_subset_প্ৰবেশ_to_string_or_fail (hb_subset_প্ৰবেশ_t *প্ৰবেশ)
{
  যদি (unlikely (!প্ৰবেশ || প্ৰবেশ->in_error ()))
    ঘূৰাই nullptr;

  hb_vector_t<ৱর্ণ> sb;

  সংরচনা flag_option_t {
    hb_subset_flags_t flag;
    ৱর্ণ option[32];
  };
  স্থির ধ্রুৱ flag_option_t flag_options[] = {
    { HB_SUBSET_FLAGS_NO_HINTING, "no-hinting" },
    { HB_SUBSET_FLAGS_RETAIN_GIDS, "retain-gids" },
    { HB_SUBSET_FLAGS_DESUBROUTINIZE, "desubroutinize" },
    { HB_SUBSET_FLAGS_NAME_LEGACY, "name-legacy" },
    { HB_SUBSET_FLAGS_SET_OVERLAPS_FLAG, "set-overlaps-flag" },
    { HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED, "passthrough-tables" },
    { HB_SUBSET_FLAGS_NOTDEF_OUTLINE, "notdef-outline" },
    { HB_SUBSET_FLAGS_GLYPH_NAMES, "glyph-names" },
    { HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES, "no-prune-unicode-ranges" },
    { HB_SUBSET_FLAGS_NO_LAYOUT_CLOSURE, "no-layout-closure" },
    { HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS, "optimize" },
    { HB_SUBSET_FLAGS_NO_BIDI_CLOSURE, "no-bidi-closure" },
#ifdef HB_EXPERIMENTAL_API
    { HB_SUBSET_FLAGS_IFTB_REQUIREMENTS, "iftb-requirements" },
    { HB_SUBSET_FLAGS_RETAIN_অঙ্ক_GLYPHS, "retain-num-glyphs" },
#endif
    { HB_SUBSET_FLAGS_DOWNGRADE_CFF2, "downgrade-cff2" },
  };

#ifdef HB_EXPERIMENTAL_API
  static_assert (পরিমাণ (flag_options) / পরিমাণ (flag_options[0]) == 15,
                 "Check all flags in hb_subset_flags_t are handled here.");
#অন্যথা
  static_assert (পরিমাণ (flag_options) / পরিমাণ (flag_options[0]) == 13,
                 "Check all flags in hb_subset_flags_t are handled here.");
#endif

  বাবে (ধ্রুৱ auto &fo : flag_options)
  {
    যদি (প্ৰবেশ->flags & fo.flag)
    {
      ৱর্ণ buf[36];
      snprintf (buf, পরিমাণ (buf), "--%s", fo.option);
      _append_arg (sb, buf);
    }
  }

  hb_subset_প্ৰবেশ_t *default_প্ৰবেশ = hb_subset_প্ৰবেশ_create_or_fail ();
  যদি (unlikely (!default_প্ৰবেশ))
    ঘূৰাই nullptr;

  // Serialize all sets if they are not equal to their default setting:
#define PROCESS_সংখ্যা_SET(field, opt_name, ...) \
  যদি (!_is_set_default (প্ৰবেশ->sets.field, default_প্ৰবেশ->sets.field)) \
    format_সংখ্যা_set_arg (প্ৰবেশ->sets.field, opt_name, sb, ##__VA_ARGS__)

#define PROCESS_TAG_SET(field, opt_name) \
  যদি (!_is_set_default (প্ৰবেশ->sets.field, default_প্ৰবেশ->sets.field)) \
    _format_tag_set_arg (প্ৰবেশ->sets.field, opt_name, sb)

  PROCESS_সংখ্যা_SET (unicodes, "unicodes", 16);
  PROCESS_সংখ্যা_SET (glyphs, "gids");
  PROCESS_সংখ্যা_SET (name_ids, "name-IDs");
  PROCESS_সংখ্যা_SET (name_languages, "name-languages");
  PROCESS_TAG_SET (layout_features, "layout-features");
  PROCESS_TAG_SET (layout_scripts, "layout-scripts");
  PROCESS_TAG_SET (drop_tables, "drop-tables");
  PROCESS_TAG_SET (no_subset_tables, "passthrough-tables");

#undef PROCESS_সংখ্যা_SET
#undef PROCESS_TAG_SET

  hb_subset_প্ৰবেশ_destroy (default_প্ৰবেশ);

  static_assert (পরিমাণ (প্ৰবেশ->sets) / পরিমাণ (hb_set_t*) == 8,
                 "Check all sets in hb_subset_input_t::sets_t are handled here.");

  যদি (unlikely (!_format_glyph_map_arg (প্ৰবেশ->glyph_map, sb)))
    ঘূৰাই nullptr;

  যদি (unlikely (!_format_axes_location_arg (প্ৰবেশ->axes_location, sb)))
    ঘূৰাই nullptr;

  sb.push ('\0');
  যদি (unlikely (sb.in_error ()))
    ঘূৰাই nullptr;

  unsigned len = 0;
  ৱর্ণ *buf = sb.steal (&len);
  যদি (unlikely (!buf))
    ঘূৰাই nullptr;

  ঘূৰাই hb_blob_create_or_fail (buf, len, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
}
#endif

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
hb_subset_preprocess (hb_face_t *source)
{
  hb_subset_প্ৰবেশ_t* প্ৰবেশ = hb_subset_প্ৰবেশ_create_or_fail ();
  যদি (!প্ৰবেশ)
    ঘূৰাই hb_face_reference (source);

  hb_subset_প্ৰবেশ_keep_everything (প্ৰবেশ);

  প্ৰবেশ->attach_accelerator_data = সঁচা;

  // Always use long loca in the preprocessed version. This allows
  // us to store the glyph bytes unpadded which allows the future subset
  // operation to run faster by skipping the trim padding step.
  প্ৰবেশ->force_long_loca = সঁচা;

  hb_face_t* new_source = hb_subset_or_fail (source, প্ৰবেশ);
  hb_subset_প্ৰবেশ_destroy (প্ৰবেশ);

  যদি (!new_source) {
    DEBUG_বাৰ্তা (SUBSET, nullptr, "Preprocessing failed due to subset failure.");
    ঘূৰাই hb_face_reference (source);
  }

  ঘূৰাই new_source;
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
hb_subset_প্ৰবেশ_old_to_new_glyph_mapping (hb_subset_প্ৰবেশ_t *প্ৰবেশ)
{
  ঘূৰাই &প্ৰবেশ->glyph_map;
}

#ifdef HB_EXPERIMENTAL_API
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
hb_subset_প্ৰবেশ_override_name_table (hb_subset_প্ৰবেশ_t  *প্ৰবেশ,
                                     hb_ot_name_id_t     name_id,
                                     unsigned            platform_id,
                                     unsigned            encoding_id,
                                     unsigned            language_id,
                                     ধ্রুৱ ৱর্ণ         *name_str,
                                     পূর্ণাঙ্ক                 str_len /* -1 means nul-terminated */)
{
  যদি (!name_str)
  {
    str_len = 0;
  }
  অন্যথা যদি (str_len == -1)
  {
      str_len = strlen (name_str);
  }

  hb_bytes_t name_bytes (nullptr, 0);
  যদি (str_len)
  {
    যদি (platform_id == 1)
    {
      ধ্রুৱ uint8_t *src = reinterpret_cast<ধ্রুৱ uint8_t*> (name_str);
      ধ্রুৱ uint8_t *src_end = src + str_len;

      hb_codepoint_t unicode;
      ধ্রুৱ hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;
      যেতিয়ালৈ (src < src_end)
      {
        src = hb_utf8_t::next (src, src_end, &unicode, replacement);
        যদি (unicode >= 0x0080u)
        {
          // Non-ascii character detected, ignored...
          ঘূৰাই মিছা;
        }
      }
    }
    ৱর্ণ *override_name = (ৱর্ণ *) hb_malloc (str_len);
    যদি (unlikely (!override_name)) ঘূৰাই মিছা;

    hb_memcpy (override_name, name_str, str_len);
    name_bytes = hb_bytes_t (override_name, str_len);
  }
  প্ৰবেশ->name_table_overrides.set (hb_ot_name_record_ids_t (platform_id, encoding_id, language_id, name_id), name_bytes);
  ঘূৰাই সঁচা;
}
#endif
