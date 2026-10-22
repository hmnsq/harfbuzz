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

#अन्तर्भाव "hb-subset-instancer-solver.hh"
#अन्तर्भाव "hb-subset.hh"
#अन्तर्भाव "hb-set.hh"
#अन्तर्भाव "hb-utf.hh"


hb_subset_निवेश_t::hb_subset_निवेश_t ()
{
  प्रति (स्वतः& set : sets_iter ())
    set = hb::shared_ptr<hb_set_t> (hb_set_create ());

  यदि (in_error ())
    प्रत्यागम;

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
hb_subset_निवेश_t *
hb_subset_निवेश_create_or_fail (शून्य)
{
  hb_subset_निवेश_t *निवेश = hb_object_create<hb_subset_निवेश_t>();

  यदि (unlikely (!निवेश))
    प्रत्यागम nullptr;

  यदि (निवेश->in_error ())
  {
    hb_subset_निवेश_destroy (निवेश);
    प्रत्यागम nullptr;
  }

  प्रत्यागम निवेश;
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
hb_subset_निवेश_t *
hb_subset_निवेश_reference (hb_subset_निवेश_t *निवेश)
{
  प्रत्यागम hb_object_reference (निवेश);
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
शून्य
hb_subset_निवेश_destroy (hb_subset_निवेश_t *निवेश)
{
  यदि (!hb_object_destroy (निवेश)) प्रत्यागम;

  hb_free (निवेश);
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
hb_subset_निवेश_unicode_set (hb_subset_निवेश_t *निवेश)
{
  प्रत्यागम निवेश->sets.unicodes;
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
hb_subset_निवेश_glyph_set (hb_subset_निवेश_t *निवेश)
{
  प्रत्यागम निवेश->sets.glyphs;
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
hb_subset_निवेश_set (hb_subset_निवेश_t *निवेश, hb_subset_sets_t set_type)
{
  प्रत्यागम निवेश->sets_iter () [set_type];
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
hb_subset_निवेश_get_flags (hb_subset_निवेश_t *निवेश)
{
  प्रत्यागम (hb_subset_flags_t) निवेश->flags;
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
HB_EXTERN शून्य
hb_subset_निवेश_set_flags (hb_subset_निवेश_t *निवेश,
			   अचिन्हित value)
{
  निवेश->flags = (hb_subset_flags_t) value;
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
hb_subset_निवेश_set_user_data (hb_subset_निवेश_t  *निवेश,
			       hb_user_data_कुंजी_t *key,
			       शून्य *		   data,
			       hb_destroy_func_t   destroy,
			       hb_bool_t	   replace)
{
  प्रत्यागम hb_object_set_user_data (निवेश, key, data, destroy, replace);
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
शून्य *
hb_subset_निवेश_get_user_data (ध्रुव hb_subset_निवेश_t *निवेश,
			       hb_user_data_कुंजी_t     *key)
{
  प्रत्यागम hb_object_get_user_data (निवेश, key);
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
शून्य
hb_subset_निवेश_keep_everything (hb_subset_निवेश_t *निवेश)
{
  ध्रुव hb_subset_sets_t indices[] = {HB_SUBSET_SETS_UNICODE,
				      HB_SUBSET_SETS_GLYPH_INDEX,
				      HB_SUBSET_SETS_NAME_ID,
				      HB_SUBSET_SETS_NAME_LANG_ID,
				      HB_SUBSET_SETS_LAYOUT_FEATURE_TAG,
				      HB_SUBSET_SETS_LAYOUT_SCRIPT_TAG};

  प्रति (स्वतः idx : hb_iter (indices))
  {
    hb_set_t *set = hb_subset_निवेश_set (निवेश, idx);
    hb_set_clear (set);
    hb_set_invert (set);
  }

  // Don't drop any tables
  hb_set_clear (hb_subset_निवेश_set (निवेश, HB_SUBSET_SETS_DROP_TABLE_TAG));

  hb_subset_निवेश_set_flags (निवेश,
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
hb_subset_निवेश_pin_all_axes_to_default (hb_subset_निवेश_t  *निवेश,
                                         hb_face_t          *face)
{
  अचिन्हित axis_गिनती = hb_ot_var_get_axis_गिनती (face);
  यदि (!axis_गिनती) प्रत्यागम असत्य;

  hb_ot_var_axis_info_t *axis_infos = (hb_ot_var_axis_info_t *) hb_calloc (axis_गिनती, परिमाण (hb_ot_var_axis_info_t));
  यदि (unlikely (!axis_infos)) प्रत्यागम असत्य;

  (शून्य) hb_ot_var_get_axis_infos (face, 0, &axis_गिनती, axis_infos);

  प्रति (अचिन्हित i = 0; i < axis_गिनती; i++)
  {
    hb_tag_t axis_tag = axis_infos[i].tag;
    द्विगुण default_val = (द्विगुण) axis_infos[i].default_मूल्य;
    यदि (!निवेश->axes_location.set (axis_tag, Triple (default_val, default_val, default_val)))
    {
      hb_free (axis_infos);
      प्रत्यागम असत्य;
    }
  }
  hb_free (axis_infos);
  प्रत्यागम सत्य;
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
hb_subset_निवेश_pin_axis_to_default (hb_subset_निवेश_t  *निवेश,
                                     hb_face_t          *face,
                                     hb_tag_t            axis_tag)
{
  hb_ot_var_axis_info_t axis_info;
  यदि (!hb_ot_var_find_axis_info (face, axis_tag, &axis_info))
    प्रत्यागम असत्य;

  द्विगुण default_val = (द्विगुण) axis_info.default_मूल्य;
  प्रत्यागम निवेश->axes_location.set (axis_tag, Triple (default_val, default_val, default_val));
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
hb_subset_निवेश_pin_axis_location (hb_subset_निवेश_t  *निवेश,
                                   hb_face_t          *face,
                                   hb_tag_t            axis_tag,
                                   भिन्न               axis_मूल्य)
{
  hb_ot_var_axis_info_t axis_info;
  यदि (!hb_ot_var_find_axis_info (face, axis_tag, &axis_info))
    प्रत्यागम असत्य;

  द्विगुण val = hb_clamp((द्विगुण) axis_मूल्य, (द्विगुण) axis_info.min_मूल्य, (द्विगुण) axis_info.max_मूल्य);
  प्रत्यागम निवेश->axes_location.set (axis_tag, Triple (val, val, val));
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
hb_subset_निवेश_set_axis_range (hb_subset_निवेश_t  *निवेश,
                                hb_face_t          *face,
                                hb_tag_t            axis_tag,
                                भिन्न               axis_min_मूल्य,
                                भिन्न               axis_max_मूल्य,
                                भिन्न               axis_def_मूल्य)
{
  hb_ot_var_axis_info_t axis_info;
  यदि (!hb_ot_var_find_axis_info (face, axis_tag, &axis_info))
    प्रत्यागम असत्य;

  भिन्न min = !std::isnan(axis_min_मूल्य) ? axis_min_मूल्य : axis_info.min_मूल्य;
  भिन्न max = !std::isnan(axis_max_मूल्य) ? axis_max_मूल्य : axis_info.max_मूल्य;
  भिन्न def = !std::isnan(axis_def_मूल्य) ? axis_def_मूल्य : axis_info.default_मूल्य;

  यदि (min > max)
    प्रत्यागम असत्य;

  भिन्न new_min_val = hb_clamp(min, axis_info.min_मूल्य, axis_info.max_मूल्य);
  भिन्न new_max_val = hb_clamp(max, axis_info.min_मूल्य, axis_info.max_मूल्य);
  भिन्न new_default_val = hb_clamp(def, new_min_val, new_max_val);
  प्रत्यागम निवेश->axes_location.set (axis_tag, Triple ((द्विगुण) new_min_val, (द्विगुण) new_default_val, (द्विगुण) new_max_val));
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
hb_subset_निवेश_get_axis_range (hb_subset_निवेश_t  *निवेश,
				hb_tag_t            axis_tag,
				भिन्न              *axis_min_मूल्य,
				भिन्न              *axis_max_मूल्य,
				भिन्न              *axis_def_मूल्य)

{
  Triple* triple;
  यदि (!निवेश->axes_location.has(axis_tag, &triple)) {
    प्रत्यागम असत्य;
  }

  *axis_min_मूल्य = triple->minimum;
  *axis_def_मूल्य = triple->मध्य;
  *axis_max_मूल्य = triple->maximum;
  प्रत्यागम सत्य;
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
hb_subset_axis_range_from_string (ध्रुव वर्ण *str, पूर्णाङ्क len,
                                  भिन्न *axis_min_मूल्य,
                                  भिन्न *axis_max_मूल्य,
                                  भिन्न *axis_def_मूल्य)
{
  यदि (len < 0)
    len = म_माप (str);

  ध्रुव वर्ण *end = str + len;
  ध्रुव वर्ण* part = strpbrk (str, ":");
  यदि (!part)
  {
    // Single value.
    यदि (म_भेद (str, "drop") == 0)
    {
      *axis_min_मूल्य = न_अंक;
      *axis_def_मूल्य = न_अंक;
      *axis_max_मूल्य = न_अंक;
      प्रत्यागम सत्य;
    }

    द्विगुण v;
    यदि (!hb_parse_double (&str, end, &v)) प्रत्यागम असत्य;

    *axis_min_मूल्य = v;
    *axis_def_मूल्य = v;
    *axis_max_मूल्य = v;
    प्रत्यागम सत्य;
  }

  भिन्न values[3];
  पूर्णाङ्क गिनती = 0;
  प्रति (पूर्णाङ्क i = 0; i < 3; i++) {
    गिनती++;
    यदि (!*str || part == str)
    {
      values[i] = न_अंक;

      यदि (part == रिक्त) भङ्ग;
      str = part + 1;
      part = strpbrk (str, ":");
      अनुवर्त;
    }

    द्विगुण v;
    ध्रुव वर्ण *piece_end = part ? part : end;
    यदि (!hb_parse_double (&str, piece_end, &v)) प्रत्यागम असत्य;
    values[i] = v;

    यदि (part == रिक्त) भङ्ग;
    str = part + 1;
    part = strpbrk (str, ":");
  }

  यदि (गिनती == 2)
  {
    *axis_min_मूल्य = values[0];
    *axis_def_मूल्य = न_अंक;
    *axis_max_मूल्य = values[1];
    प्रत्यागम सत्य;
  }
  अन्यथा यदि (गिनती == 3)
  {
    *axis_min_मूल्य = values[0];
    *axis_def_मूल्य = values[1];
    *axis_max_मूल्य = values[2];
    प्रत्यागम सत्य;
  }
  प्रत्यागम असत्य;
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
HB_EXTERN शून्य
hb_subset_axis_range_to_string (hb_subset_निवेश_t *निवेश,
                                hb_tag_t axis_tag,
                                वर्ण *buf, अचिन्हित size)
{
  यदि (unlikely (!size)) प्रत्यागम;
  Triple* triple;
  यदि (!निवेश->axes_location.has(axis_tag, &triple)) {
    प्रत्यागम;
  }

  वर्ण s[128];
  अचिन्हित len = 0;

  hb_locale_t clocale HB_UNUSED;
  hb_locale_t oldlocale HB_UNUSED;
  oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", रिक्त));
  len += hb_max (0, snprintf (s, ARRAY_LENGTH (s) - len, "%g", (द्विगुण) triple->minimum));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_LENGTH (s) - len, "%g", (द्विगुण) triple->मध्य));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_LENGTH (s) - len, "%g", (द्विगुण) triple->maximum));
  (शून्य) hb_uselocale (((शून्य) freelocale (clocale), oldlocale));

  प्रतिज्ञा (len < ARRAY_LENGTH (s));
  len = hb_min (len, size - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}
#endif

#ifdef HB_EXPERIMENTAL_API
स्थिर शून्य
_append_arg (hb_vector_t<वर्ण> &sb, ध्रुव वर्ण *arg)
{
  यदि (sb.in_error ()) प्रत्यागम;
  यदि (sb.length)
    sb.push (' ');
  अचिन्हित len = म_माप (arg);
  sb.extend (hb_bytes_t (arg, len));
}
स्थिर शून्य
format_संख्या_set (ध्रुव hb_set_t *set, hb_vector_t<वर्ण> &out, अचिन्हित base = 10)
{
  प्रतिज्ञा (base == 10 || base == 16);
  यदि (unlikely (base != 10 && base != 16)) प्रत्यागम;

  hb_codepoint_t first = HB_SET_मूल्य_INVALID, last = HB_SET_मूल्य_INVALID;
  तार्किक first_range = सत्य;
  यावत् (hb_set_next_range (set, &first, &last))
  {
    यदि (!first_range)
      out.push (',');
    first_range = असत्य;

    वर्ण buf[64];
    पूर्णाङ्क len;
    यदि (first == last)
    {
      यदि (base == 16)
        len = snprintf (buf, परिमाण (buf), "%X", first);
      अन्यथा
        len = snprintf (buf, परिमाण (buf), "%u", first);
    }
    अन्यथा
    {
      यदि (base == 16)
        len = snprintf (buf, परिमाण (buf), "%X-%X", first, last);
      अन्यथा
        len = snprintf (buf, परिमाण (buf), "%u-%u", first, last);
    }
    यदि (unlikely (len < 0 || (माप_प्रकार) len >= परिमाण (buf))) प्रत्यागम;
    out.extend (hb_bytes_t (buf, len));
  }
}

स्थिर शून्य
_format_tag (hb_tag_t tag, hb_vector_t<वर्ण> &out)
{
  वर्ण buf[4];
  hb_tag_to_string (tag, buf);

  अचिन्हित len = 4;
  यावत् (len > 0 && buf[len - 1] == ' ')
    len--;

  प्रति (अचिन्हित i = 0; i < len; i++)
  {
    वर्ण c = buf[i];
    यदि (!((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '-' || c == '_'))
    {
      c = '_';
    }
    out.push (c);
  }
}

स्थिर शून्य
_format_tag_set (ध्रुव hb_set_t *set, hb_vector_t<वर्ण> &out)
{
  तार्किक first_tag = सत्य;
  प्रति (hb_codepoint_t tag : *set)
  {
    यदि (!first_tag)
      out.push (',');
    first_tag = असत्य;

    _format_tag (tag, out);
  }
}

स्थिर तार्किक
_is_set_default (ध्रुव hb_set_t *set, ध्रुव hb_set_t *default_set)
{
  प्रत्यागम hb_set_is_inverted (set) == hb_set_is_inverted (default_set) &&
         hb_set_is_equal (set, default_set);
}

ढाँचा <typename Formatter>
स्थिर शून्य
_format_set_arg (ध्रुव hb_set_t *set,
                 ध्रुव वर्ण *opt_name,
                 Formatter &&formatter,
                 hb_vector_t<वर्ण> &sb)
{
  यदि (hb_set_is_inverted (set))
  {
    hb_set_t *excluded = hb_set_copy (set);
    hb_set_invert (excluded);

    hb_vector_t<वर्ण> opt;
    opt.extend (hb_bytes_t ("--", 2));
    opt.extend (hb_bytes_t (opt_name, म_माप (opt_name)));
    opt.extend (hb_bytes_t ("=*", 2));
    opt.push ('\0');
    यदि (opt.arrayZ) _append_arg (sb, opt.arrayZ);

    यदि (!hb_set_is_empty (excluded))
    {
      hb_vector_t<वर्ण> formatted;
      formatter (excluded, formatted);
      यदि (formatted.length)
      {
        hb_vector_t<वर्ण> opt2;
        opt2.extend (hb_bytes_t ("--", 2));
        opt2.extend (hb_bytes_t (opt_name, म_माप (opt_name)));
        opt2.extend (hb_bytes_t ("-=", 2));
        opt2.extend (formatted.as_array ());
        opt2.push ('\0');
        यदि (opt2.arrayZ) _append_arg (sb, opt2.arrayZ);
      }
    }
    hb_set_destroy (excluded);
  }
  अन्यथा यदि (hb_set_is_empty (set))
  {
    hb_vector_t<वर्ण> opt;
    opt.extend (hb_bytes_t ("--", 2));
    opt.extend (hb_bytes_t (opt_name, म_माप (opt_name)));
    opt.extend (hb_bytes_t ("-=*", 3));
    opt.push ('\0');
    यदि (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  अन्यथा
  {
    hb_vector_t<वर्ण> formatted;
    formatter (set, formatted);
    यदि (formatted.length)
    {
      hb_vector_t<वर्ण> opt;
      opt.extend (hb_bytes_t ("--", 2));
      opt.extend (hb_bytes_t (opt_name, म_माप (opt_name)));
      opt.extend (hb_bytes_t ("=", 1));
      opt.extend (formatted.as_array ());
      opt.push ('\0');
      यदि (opt.arrayZ) _append_arg (sb, opt.arrayZ);
    }
  }
}

स्थिर शून्य
format_संख्या_set_arg (ध्रुव hb_set_t *set,
                        ध्रुव वर्ण *opt_name,
                        hb_vector_t<वर्ण> &sb,
                        अचिन्हित base = 10)
{
  _format_set_arg (set, opt_name, [base] (ध्रुव hb_set_t *s, hb_vector_t<वर्ण> &out) {
    format_संख्या_set (s, out, base);
  }, sb);
}

स्थिर शून्य
_format_tag_set_arg (ध्रुव hb_set_t *set,
                     ध्रुव वर्ण *opt_name,
                     hb_vector_t<वर्ण> &sb)
{
  _format_set_arg (set, opt_name, _format_tag_set, sb);
}

स्थिर तार्किक
_format_glyph_map_arg (ध्रुव hb_map_t &glyph_map,
                       hb_vector_t<वर्ण> &sb)
{
  यदि (glyph_map.is_empty ())
    प्रत्यागम सत्य;

  hb_set_t कुंजियाँ;
  hb_map_कुंजियाँ (&glyph_map, &कुंजियाँ);
  hb_vector_t<वर्ण> pairs;
  तार्किक first = सत्य;
  प्रति (hb_codepoint_t key : कुंजियाँ)
  {
    यदि (!first)
      pairs.push (',');
    first = असत्य;
    hb_codepoint_t val = hb_map_get (&glyph_map, key);
    वर्ण buf[64];
    पूर्णाङ्क len = snprintf (buf, परिमाण (buf), "%u:%u", key, val);
    यदि (unlikely (len < 0 || (माप_प्रकार) len >= परिमाण (buf))) प्रत्यागम असत्य;
    pairs.extend (hb_bytes_t (buf, len));
  }
  यदि (pairs.length)
  {
    hb_vector_t<वर्ण> opt;
    opt.extend (hb_bytes_t ("--gid-map=", 10));
    opt.extend (pairs.as_array ());
    opt.push ('\0');
    यदि (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  प्रत्यागम सत्य;
}

स्थिर तार्किक
_format_axes_location_arg (ध्रुव hb_hashmap_t<hb_tag_t, Triple> &axes_location,
                           hb_vector_t<वर्ण> &sb)
{
  यदि (axes_location.is_empty ())
    प्रत्यागम सत्य;

  hb_vector_t<hb_tag_t> axis_tags;
  प्रति (स्वतः tag : axes_location.कुंजियाँ ())
    axis_tags.push (tag);
  axis_tags.क्विक ([] (ध्रुव hb_tag_t &a, ध्रुव hb_tag_t &b) { प्रत्यागम a < b ? -1 : a > b ? 1 : 0; });

  hb_vector_t<वर्ण> specs;
  तार्किक first = सत्य;
  प्रति (hb_tag_t tag : axis_tags)
  {
    Triple triple = axes_location.get (tag);
    यदि (!first)
      specs.push (',');
    first = असत्य;

    hb_vector_t<वर्ण> tag_buf;
    _format_tag (tag, tag_buf);

    वर्ण buf[128];
    पूर्णाङ्क len;
    यदि (triple.minimum == triple.मध्य && triple.मध्य == triple.maximum)
    {
      len = snprintf (buf, परिमाण (buf), "%.*s=%g", (पूर्णाङ्क) tag_buf.length, tag_buf.arrayZ, (द्विगुण) triple.मध्य);
    }
    अन्यथा यदि (std::isnan (triple.minimum) && std::isnan (triple.मध्य) && std::isnan (triple.maximum))
    {
      len = snprintf (buf, परिमाण (buf), "%.*s=drop", (पूर्णाङ्क) tag_buf.length, tag_buf.arrayZ);
    }
    अन्यथा
    {
      len = snprintf (buf, परिमाण (buf), "%.*s=%g:%g:%g", (पूर्णाङ्क) tag_buf.length, tag_buf.arrayZ,
                      (द्विगुण) triple.minimum, (द्विगुण) triple.मध्य, (द्विगुण) triple.maximum);
    }
    यदि (unlikely (len < 0 || (माप_प्रकार) len >= परिमाण (buf))) प्रत्यागम असत्य;
    specs.extend (hb_bytes_t (buf, len));
  }
  यदि (specs.length)
  {
    hb_vector_t<वर्ण> opt;
    opt.extend (hb_bytes_t ("--variations=", 13));
    opt.extend (specs.as_array ());
    opt.push ('\0');
    यदि (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  प्रत्यागम सत्य;
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
hb_subset_निवेश_to_string_or_fail (hb_subset_निवेश_t *निवेश)
{
  यदि (unlikely (!निवेश || निवेश->in_error ()))
    प्रत्यागम nullptr;

  hb_vector_t<वर्ण> sb;

  संरचना flag_option_t {
    hb_subset_flags_t flag;
    वर्ण option[32];
  };
  स्थिर ध्रुव flag_option_t flag_options[] = {
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
    { HB_SUBSET_FLAGS_RETAIN_अंक_GLYPHS, "retain-num-glyphs" },
#endif
    { HB_SUBSET_FLAGS_DOWNGRADE_CFF2, "downgrade-cff2" },
  };

#ifdef HB_EXPERIMENTAL_API
  static_assert (परिमाण (flag_options) / परिमाण (flag_options[0]) == 15,
                 "Check all flags in hb_subset_flags_t are handled here.");
#अन्यथा
  static_assert (परिमाण (flag_options) / परिमाण (flag_options[0]) == 13,
                 "Check all flags in hb_subset_flags_t are handled here.");
#endif

  प्रति (ध्रुव स्वतः &fo : flag_options)
  {
    यदि (निवेश->flags & fo.flag)
    {
      वर्ण buf[36];
      snprintf (buf, परिमाण (buf), "--%s", fo.option);
      _append_arg (sb, buf);
    }
  }

  hb_subset_निवेश_t *default_निवेश = hb_subset_निवेश_create_or_fail ();
  यदि (unlikely (!default_निवेश))
    प्रत्यागम nullptr;

  // Serialize all sets if they are not equal to their default setting:
#define PROCESS_संख्या_SET(field, opt_name, ...) \
  यदि (!_is_set_default (निवेश->sets.field, default_निवेश->sets.field)) \
    format_संख्या_set_arg (निवेश->sets.field, opt_name, sb, ##__VA_ARGS__)

#define PROCESS_TAG_SET(field, opt_name) \
  यदि (!_is_set_default (निवेश->sets.field, default_निवेश->sets.field)) \
    _format_tag_set_arg (निवेश->sets.field, opt_name, sb)

  PROCESS_संख्या_SET (unicodes, "unicodes", 16);
  PROCESS_संख्या_SET (glyphs, "gids");
  PROCESS_संख्या_SET (name_ids, "name-IDs");
  PROCESS_संख्या_SET (name_languages, "name-languages");
  PROCESS_TAG_SET (layout_features, "layout-features");
  PROCESS_TAG_SET (layout_scripts, "layout-scripts");
  PROCESS_TAG_SET (drop_tables, "drop-tables");
  PROCESS_TAG_SET (no_subset_tables, "passthrough-tables");

#undef PROCESS_संख्या_SET
#undef PROCESS_TAG_SET

  hb_subset_निवेश_destroy (default_निवेश);

  static_assert (परिमाण (निवेश->sets) / परिमाण (hb_set_t*) == 8,
                 "Check all sets in hb_subset_input_t::sets_t are handled here.");

  यदि (unlikely (!_format_glyph_map_arg (निवेश->glyph_map, sb)))
    प्रत्यागम nullptr;

  यदि (unlikely (!_format_axes_location_arg (निवेश->axes_location, sb)))
    प्रत्यागम nullptr;

  sb.push ('\0');
  यदि (unlikely (sb.in_error ()))
    प्रत्यागम nullptr;

  अचिन्हित len = 0;
  वर्ण *buf = sb.steal (&len);
  यदि (unlikely (!buf))
    प्रत्यागम nullptr;

  प्रत्यागम hb_blob_create_or_fail (buf, len, HB_MEMORY_MODE_WRITABLE, buf, hb_free);
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
  hb_subset_निवेश_t* निवेश = hb_subset_निवेश_create_or_fail ();
  यदि (!निवेश)
    प्रत्यागम hb_face_reference (source);

  hb_subset_निवेश_keep_everything (निवेश);

  निवेश->attach_accelerator_data = सत्य;

  // Always use long loca in the preprocessed version. This allows
  // us to store the glyph bytes unpadded which allows the future subset
  // operation to run faster by skipping the trim padding step.
  निवेश->force_long_loca = सत्य;

  hb_face_t* new_source = hb_subset_or_fail (source, निवेश);
  hb_subset_निवेश_destroy (निवेश);

  यदि (!new_source) {
    DEBUG_संदेश (SUBSET, nullptr, "Preprocessing failed due to subset failure.");
    प्रत्यागम hb_face_reference (source);
  }

  प्रत्यागम new_source;
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
hb_subset_निवेश_old_to_new_glyph_mapping (hb_subset_निवेश_t *निवेश)
{
  प्रत्यागम &निवेश->glyph_map;
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
hb_subset_निवेश_override_name_table (hb_subset_निवेश_t  *निवेश,
                                     hb_ot_name_id_t     name_id,
                                     अचिन्हित            platform_id,
                                     अचिन्हित            encoding_id,
                                     अचिन्हित            language_id,
                                     ध्रुव वर्ण         *name_str,
                                     पूर्णाङ्क                 str_len /* -1 means nul-terminated */)
{
  यदि (!name_str)
  {
    str_len = 0;
  }
  अन्यथा यदि (str_len == -1)
  {
      str_len = म_माप (name_str);
  }

  hb_bytes_t name_bytes (nullptr, 0);
  यदि (str_len)
  {
    यदि (platform_id == 1)
    {
      ध्रुव uint8_t *src = reinterpret_cast<ध्रुव uint8_t*> (name_str);
      ध्रुव uint8_t *src_end = src + str_len;

      hb_codepoint_t unicode;
      ध्रुव hb_codepoint_t replacement = HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT;
      यावत् (src < src_end)
      {
        src = hb_utf8_t::next (src, src_end, &unicode, replacement);
        यदि (unicode >= 0x0080u)
        {
          // Non-ascii character detected, ignored...
          प्रत्यागम असत्य;
        }
      }
    }
    वर्ण *override_name = (वर्ण *) hb_malloc (str_len);
    यदि (unlikely (!override_name)) प्रत्यागम असत्य;

    hb_memcpy (override_name, name_str, str_len);
    name_bytes = hb_bytes_t (override_name, str_len);
  }
  निवेश->name_table_overrides.set (hb_ot_name_record_ids_t (platform_id, encoding_id, language_id, name_id), name_bytes);
  प्रत्यागम सत्य;
}
#endif
