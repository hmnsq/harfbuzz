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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-instancer-solver.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-utf.hh"


hb_subset_ਨਿਵੇਸ਼_t::hb_subset_ਨਿਵੇਸ਼_t ()
{
  ਲਈ (ਸਵੈਚਾਲਿਤ& ਸੈੱਟ : sets_iter ())
    ਸੈੱਟ = hb::shared_ptr<hb_ਸੈੱਟ_t> (hb_ਸੈੱਟ_create ());

  ਜੇ (ਵਿੱਚ_error ())
    ਵਾਪਸ;

  flags = HB_SUBSET_FLAGS_DEFAULT;

  hb_ਸੈੱਟ_ਜੋੜੋ_range (sets.name_ids, 0, 6);
  hb_ਸੈੱਟ_ਜੋੜੋ (sets.name_languages, 0x0409);

  hb_ਟੈਗ_t default_drop_tables[] = {
    // Layout disabled by default
    HB_ਟੈਗ ('m', 'o', 'r', 'x'),
    HB_ਟੈਗ ('m', 'o', 'r', 't'),
    HB_ਟੈਗ ('k', 'e', 'r', 'x'),
    HB_ਟੈਗ ('k', 'e', 'r', 'n'),

    // Copied from fontTools:
    HB_ਟੈਗ ('J', 'S', 'T', 'F'),
    HB_ਟੈਗ ('D', 'S', 'I', 'G'),
    HB_ਟੈਗ ('E', 'B', 'D', 'T'),
    HB_ਟੈਗ ('E', 'B', 'L', 'C'),
    HB_ਟੈਗ ('E', 'B', 'S', 'C'),
    HB_ਟੈਗ ('S', 'V', 'G', ' '),
    HB_ਟੈਗ ('P', 'C', 'L', 'T'),
    HB_ਟੈਗ ('L', 'T', 'S', 'H'),
    // Graphite tables
    HB_ਟੈਗ ('F', 'e', 'a', 't'),
    HB_ਟੈਗ ('G', 'l', 'a', 't'),
    HB_ਟੈਗ ('G', 'l', 'o', 'c'),
    HB_ਟੈਗ ('S', 'i', 'l', 'f'),
    HB_ਟੈਗ ('S', 'i', 'l', 'l'),
  };
  sets.drop_tables->ਜੋੜੋ_array (default_drop_tables, ARRAY_ਲੰਮਾਈ (default_drop_tables));

  hb_ਟੈਗ_t default_no_subset_tables[] = {
    HB_ਟੈਗ ('g', 'a', 's', 'p'),
    HB_ਟੈਗ ('f', 'p', 'g', 'm'),
    HB_ਟੈਗ ('p', 'r', 'e', 'p'),
    HB_ਟੈਗ ('V', 'D', 'M', 'X'),
    HB_ਟੈਗ ('D', 'S', 'I', 'G'),
  };
  sets.no_subset_tables->ਜੋੜੋ_array (default_no_subset_tables,
					 ARRAY_ਲੰਮਾਈ (default_no_subset_tables));

  //copied from _layout_features_groups in fonttools
  hb_ਟੈਗ_t default_layout_features[] = {
    // default shaper
    // common
    HB_ਟੈਗ ('r', 'v', 'r', 'n'),
    HB_ਟੈਗ ('c', 'c', 'm', 'p'),
    HB_ਟੈਗ ('l', 'i', 'g', 'a'),
    HB_ਟੈਗ ('l', 'o', 'c', 'l'),
    HB_ਟੈਗ ('m', 'a', 'r', 'k'),
    HB_ਟੈਗ ('m', 'k', 'm', 'k'),
    HB_ਟੈਗ ('r', 'l', 'i', 'g'),

    //fractions
    HB_ਟੈਗ ('f', 'r', 'a', 'c'),
    HB_ਟੈਗ ('n', 'u', 'm', 'r'),
    HB_ਟੈਗ ('d', 'n', 'o', 'm'),

    //horizontal
    HB_ਟੈਗ ('c', 'a', 'l', 't'),
    HB_ਟੈਗ ('c', 'l', 'i', 'g'),
    HB_ਟੈਗ ('c', 'u', 'r', 's'),
    HB_ਟੈਗ ('k', 'e', 'r', 'n'),
    HB_ਟੈਗ ('r', 'c', 'l', 't'),

    //vertical
    HB_ਟੈਗ ('v', 'a', 'l', 't'),
    HB_ਟੈਗ ('v', 'e', 'r', 't'),
    HB_ਟੈਗ ('v', 'k', 'r', 'n'),
    HB_ਟੈਗ ('v', 'p', 'a', 'l'),
    HB_ਟੈਗ ('v', 'r', 't', '2'),

    //ltr
    HB_ਟੈਗ ('l', 't', 'r', 'a'),
    HB_ਟੈਗ ('l', 't', 'r', 'm'),

    //rtl
    HB_ਟੈਗ ('r', 't', 'l', 'a'),
    HB_ਟੈਗ ('r', 't', 'l', 'm'),

    //random
    HB_ਟੈਗ ('r', 'a', 'n', 'd'),

    //justify
    HB_ਟੈਗ ('j', 'a', 'l', 't'), // HarfBuzz doesn't use; others might

    //East Asian spacing
    HB_ਟੈਗ ('c', 'h', 'w', 's'),
    HB_ਟੈਗ ('v', 'c', 'h', 'w'),
    HB_ਟੈਗ ('h', 'a', 'l', 't'),
    HB_ਟੈਗ ('v', 'h', 'a', 'l'),
    HB_ਟੈਗ ('p', 'a', 'l', 't'),

    //ਨਿੱਜੀ
    HB_ਟੈਗ ('H', 'a', 'r', 'f'),
    HB_ਟੈਗ ('H', 'A', 'R', 'F'),
    HB_ਟੈਗ ('B', 'u', 'z', 'z'),
    HB_ਟੈਗ ('B', 'U', 'Z', 'Z'),

    //shapers

    //arabic
    HB_ਟੈਗ ('i', 'n', 'i', 't'),
    HB_ਟੈਗ ('m', 'e', 'd', 'i'),
    HB_ਟੈਗ ('f', 'i', 'n', 'a'),
    HB_ਟੈਗ ('i', 's', 'o', 'l'),
    HB_ਟੈਗ ('m', 'e', 'd', '2'),
    HB_ਟੈਗ ('f', 'i', 'n', '2'),
    HB_ਟੈਗ ('f', 'i', 'n', '3'),
    HB_ਟੈਗ ('c', 's', 'w', 'h'),
    HB_ਟੈਗ ('m', 's', 'e', 't'),
    HB_ਟੈਗ ('s', 't', 'c', 'h'),

    //hangul
    HB_ਟੈਗ ('l', 'j', 'm', 'o'),
    HB_ਟੈਗ ('v', 'j', 'm', 'o'),
    HB_ਟੈਗ ('t', 'j', 'm', 'o'),

    //tibetan
    HB_ਟੈਗ ('a', 'b', 'v', 's'),
    HB_ਟੈਗ ('b', 'l', 'w', 's'),
    HB_ਟੈਗ ('a', 'b', 'v', 'm'),
    HB_ਟੈਗ ('b', 'l', 'w', 'm'),

    //indic
    HB_ਟੈਗ ('n', 'u', 'k', 't'),
    HB_ਟੈਗ ('a', 'k', 'h', 'n'),
    HB_ਟੈਗ ('r', 'p', 'h', 'f'),
    HB_ਟੈਗ ('r', 'k', 'r', 'f'),
    HB_ਟੈਗ ('p', 'r', 'e', 'f'),
    HB_ਟੈਗ ('b', 'l', 'w', 'f'),
    HB_ਟੈਗ ('h', 'a', 'l', 'f'),
    HB_ਟੈਗ ('a', 'b', 'v', 'f'),
    HB_ਟੈਗ ('p', 's', 't', 'f'),
    HB_ਟੈਗ ('c', 'f', 'a', 'r'),
    HB_ਟੈਗ ('v', 'a', 't', 'u'),
    HB_ਟੈਗ ('c', 'j', 'c', 't'),
    HB_ਟੈਗ ('i', 'n', 'i', 't'),
    HB_ਟੈਗ ('p', 'r', 'e', 's'),
    HB_ਟੈਗ ('a', 'b', 'v', 's'),
    HB_ਟੈਗ ('b', 'l', 'w', 's'),
    HB_ਟੈਗ ('p', 's', 't', 's'),
    HB_ਟੈਗ ('h', 'a', 'l', 'n'),
    HB_ਟੈਗ ('d', 'i', 's', 't'),
    HB_ਟੈਗ ('a', 'b', 'v', 'm'),
    HB_ਟੈਗ ('b', 'l', 'w', 'm'),
  };

  sets.layout_features->ਜੋੜੋ_array (default_layout_features, ARRAY_ਲੰਮਾਈ (default_layout_features));

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
hb_subset_ਨਿਵੇਸ਼_t *
hb_subset_ਨਿਵੇਸ਼_create_or_fail (ਖਾਲੀ)
{
  hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼ = hb_object_create<hb_subset_ਨਿਵੇਸ਼_t>();

  ਜੇ (unlikely (!ਨਿਵੇਸ਼))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (ਨਿਵੇਸ਼->ਵਿੱਚ_error ())
  {
    hb_subset_ਨਿਵੇਸ਼_destroy (ਨਿਵੇਸ਼);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ ਨਿਵੇਸ਼;
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
hb_subset_ਨਿਵੇਸ਼_t *
hb_subset_ਨਿਵੇਸ਼_reference (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  ਵਾਪਸ hb_object_reference (ਨਿਵੇਸ਼);
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
ਖਾਲੀ
hb_subset_ਨਿਵੇਸ਼_destroy (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  ਜੇ (!hb_object_destroy (ਨਿਵੇਸ਼)) ਵਾਪਸ;

  hb_free (ਨਿਵੇਸ਼);
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
HB_EXTERN hb_ਸੈੱਟ_t *
hb_subset_ਨਿਵੇਸ਼_unicode_ਸੈੱਟ (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  ਵਾਪਸ ਨਿਵੇਸ਼->sets.unicodes;
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
HB_EXTERN hb_ਸੈੱਟ_t *
hb_subset_ਨਿਵੇਸ਼_glyph_ਸੈੱਟ (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  ਵਾਪਸ ਨਿਵੇਸ਼->sets.glyphs;
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
HB_EXTERN hb_ਸੈੱਟ_t *
hb_subset_ਨਿਵੇਸ਼_ਸੈੱਟ (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼, hb_subset_sets_t ਸੈੱਟ_type)
{
  ਵਾਪਸ ਨਿਵੇਸ਼->sets_iter () [ਸੈੱਟ_type];
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
hb_subset_ਨਿਵੇਸ਼_get_flags (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  ਵਾਪਸ (hb_subset_flags_t) ਨਿਵੇਸ਼->flags;
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
HB_EXTERN ਖਾਲੀ
hb_subset_ਨਿਵੇਸ਼_ਸੈੱਟ_flags (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ value)
{
  ਨਿਵੇਸ਼->flags = (hb_subset_flags_t) value;
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
hb_subset_ਨਿਵੇਸ਼_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_subset_ਨਿਵੇਸ਼_t  *ਨਿਵੇਸ਼,
			       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			       ਖਾਲੀ *		   ਅੰਕੜੇ,
			       hb_destroy_func_t   destroy,
			       hb_bool_t	   replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (ਨਿਵੇਸ਼, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_subset_ਨਿਵੇਸ਼_get_user_ਅੰਕੜੇ (ਅਚਲ hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼,
			       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t     *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (ਨਿਵੇਸ਼, key);
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
ਖਾਲੀ
hb_subset_ਨਿਵੇਸ਼_keep_everything (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  ਅਚਲ hb_subset_sets_t indices[] = {HB_SUBSET_SETS_UNICODE,
				      HB_SUBSET_SETS_GLYPH_ਸੂਚਕਾਂਕ,
				      HB_SUBSET_SETS_NAME_ID,
				      HB_SUBSET_SETS_NAME_LANG_ID,
				      HB_SUBSET_SETS_LAYOUT_FEATURE_ਟੈਗ,
				      HB_SUBSET_SETS_LAYOUT_SCRIPT_ਟੈਗ};

  ਲਈ (ਸਵੈਚਾਲਿਤ idx : hb_iter (indices))
  {
    hb_ਸੈੱਟ_t *ਸੈੱਟ = hb_subset_ਨਿਵੇਸ਼_ਸੈੱਟ (ਨਿਵੇਸ਼, idx);
    hb_ਸੈੱਟ_clear (ਸੈੱਟ);
    hb_ਸੈੱਟ_invert (ਸੈੱਟ);
  }

  // Don't drop any tables
  hb_ਸੈੱਟ_clear (hb_subset_ਨਿਵੇਸ਼_ਸੈੱਟ (ਨਿਵੇਸ਼, HB_SUBSET_SETS_DROP_ਸਾਰਣੀ_ਟੈਗ));

  hb_subset_ਨਿਵੇਸ਼_ਸੈੱਟ_flags (ਨਿਵੇਸ਼,
			     HB_SUBSET_FLAGS_NOTDEF_OUTLINE |
			     HB_SUBSET_FLAGS_GLYPH_ਨਾਂ |
			     HB_SUBSET_FLAGS_NAME_LEGACY |
			     HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES |
                             HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
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
hb_subset_ਨਿਵੇਸ਼_pin_all_axes_ਮੰਜ਼ਿਲ_default (hb_subset_ਨਿਵੇਸ਼_t  *ਨਿਵੇਸ਼,
                                         hb_face_t          *face)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ = hb_ot_var_get_axis_ਗਿਣਤੀ (face);
  ਜੇ (!axis_ਗਿਣਤੀ) ਵਾਪਸ ਝੂਠ;

  hb_ot_var_axis_info_t *axis_infos = (hb_ot_var_axis_info_t *) hb_calloc (axis_ਗਿਣਤੀ, ਆਕਾਰ (hb_ot_var_axis_info_t));
  ਜੇ (unlikely (!axis_infos)) ਵਾਪਸ ਝੂਠ;

  (ਖਾਲੀ) hb_ot_var_get_axis_infos (face, 0, &axis_ਗਿਣਤੀ, axis_infos);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ; i++)
  {
    hb_ਟੈਗ_t axis_ਟੈਗ_2 = axis_infos[i].ਟੈਗ_2;
    ਦੁੱਗਣਾ default_val = (ਦੁੱਗਣਾ) axis_infos[i].default_ਮੁੱਲ;
    ਜੇ (!ਨਿਵੇਸ਼->axes_location.ਸੈੱਟ (axis_ਟੈਗ_2, Triple (default_val, default_val, default_val)))
    {
      hb_free (axis_infos);
      ਵਾਪਸ ਝੂਠ;
    }
  }
  hb_free (axis_infos);
  ਵਾਪਸ ਸੱਚ;
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
hb_subset_ਨਿਵੇਸ਼_pin_axis_ਮੰਜ਼ਿਲ_default (hb_subset_ਨਿਵੇਸ਼_t  *ਨਿਵੇਸ਼,
                                     hb_face_t          *face,
                                     hb_ਟੈਗ_t            axis_ਟੈਗ_2)
{
  hb_ot_var_axis_info_t axis_info;
  ਜੇ (!hb_ot_var_ਲੱਭੋ_axis_info (face, axis_ਟੈਗ_2, &axis_info))
    ਵਾਪਸ ਝੂਠ;

  ਦੁੱਗਣਾ default_val = (ਦੁੱਗਣਾ) axis_info.default_ਮੁੱਲ;
  ਵਾਪਸ ਨਿਵੇਸ਼->axes_location.ਸੈੱਟ (axis_ਟੈਗ_2, Triple (default_val, default_val, default_val));
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
hb_subset_ਨਿਵੇਸ਼_pin_axis_location (hb_subset_ਨਿਵੇਸ਼_t  *ਨਿਵੇਸ਼,
                                   hb_face_t          *face,
                                   hb_ਟੈਗ_t            axis_ਟੈਗ_2,
                                   ਦਸ਼ਮਲਵ               axis_ਮੁੱਲ)
{
  hb_ot_var_axis_info_t axis_info;
  ਜੇ (!hb_ot_var_ਲੱਭੋ_axis_info (face, axis_ਟੈਗ_2, &axis_info))
    ਵਾਪਸ ਝੂਠ;

  ਦੁੱਗਣਾ val = hb_clamp((ਦੁੱਗਣਾ) axis_ਮੁੱਲ, (ਦੁੱਗਣਾ) axis_info.min_ਮੁੱਲ, (ਦੁੱਗਣਾ) axis_info.max_ਮੁੱਲ);
  ਵਾਪਸ ਨਿਵੇਸ਼->axes_location.ਸੈੱਟ (axis_ਟੈਗ_2, Triple (val, val, val));
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
hb_subset_ਨਿਵੇਸ਼_ਸੈੱਟ_axis_range (hb_subset_ਨਿਵੇਸ਼_t  *ਨਿਵੇਸ਼,
                                hb_face_t          *face,
                                hb_ਟੈਗ_t            axis_ਟੈਗ_2,
                                ਦਸ਼ਮਲਵ               axis_min_ਮੁੱਲ,
                                ਦਸ਼ਮਲਵ               axis_max_ਮੁੱਲ,
                                ਦਸ਼ਮਲਵ               axis_def_ਮੁੱਲ)
{
  hb_ot_var_axis_info_t axis_info;
  ਜੇ (!hb_ot_var_ਲੱਭੋ_axis_info (face, axis_ਟੈਗ_2, &axis_info))
    ਵਾਪਸ ਝੂਠ;

  ਦਸ਼ਮਲਵ min = !std::isnan(axis_min_ਮੁੱਲ) ? axis_min_ਮੁੱਲ : axis_info.min_ਮੁੱਲ;
  ਦਸ਼ਮਲਵ max = !std::isnan(axis_max_ਮੁੱਲ) ? axis_max_ਮੁੱਲ : axis_info.max_ਮੁੱਲ;
  ਦਸ਼ਮਲਵ def = !std::isnan(axis_def_ਮੁੱਲ) ? axis_def_ਮੁੱਲ : axis_info.default_ਮੁੱਲ;

  ਜੇ (min > max)
    ਵਾਪਸ ਝੂਠ;

  ਦਸ਼ਮਲਵ new_min_val = hb_clamp(min, axis_info.min_ਮੁੱਲ, axis_info.max_ਮੁੱਲ);
  ਦਸ਼ਮਲਵ new_max_val = hb_clamp(max, axis_info.min_ਮੁੱਲ, axis_info.max_ਮੁੱਲ);
  ਦਸ਼ਮਲਵ new_default_val = hb_clamp(def, new_min_val, new_max_val);
  ਵਾਪਸ ਨਿਵੇਸ਼->axes_location.ਸੈੱਟ (axis_ਟੈਗ_2, Triple ((ਦੁੱਗਣਾ) new_min_val, (ਦੁੱਗਣਾ) new_default_val, (ਦੁੱਗਣਾ) new_max_val));
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
hb_subset_ਨਿਵੇਸ਼_get_axis_range (hb_subset_ਨਿਵੇਸ਼_t  *ਨਿਵੇਸ਼,
				hb_ਟੈਗ_t            axis_ਟੈਗ_2,
				ਦਸ਼ਮਲਵ              *axis_min_ਮੁੱਲ,
				ਦਸ਼ਮਲਵ              *axis_max_ਮੁੱਲ,
				ਦਸ਼ਮਲਵ              *axis_def_ਮੁੱਲ)

{
  Triple* triple;
  ਜੇ (!ਨਿਵੇਸ਼->axes_location.has(axis_ਟੈਗ_2, &triple)) {
    ਵਾਪਸ ਝੂਠ;
  }

  *axis_min_ਮੁੱਲ = triple->minimum;
  *axis_def_ਮੁੱਲ = triple->ਵਿਚਕਾਰ;
  *axis_max_ਮੁੱਲ = triple->ਵੱਧ_ਤੋਂ_ਵੱਧ;
  ਵਾਪਸ ਸੱਚ;
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
hb_subset_axis_range_from_string (ਅਚਲ ਅੱਖਰ *str, ਪੂਰਨ len,
                                  ਦਸ਼ਮਲਵ *axis_min_ਮੁੱਲ,
                                  ਦਸ਼ਮਲਵ *axis_max_ਮੁੱਲ,
                                  ਦਸ਼ਮਲਵ *axis_def_ਮੁੱਲ)
{
  ਜੇ (len < 0)
    len = ਲੜੀ_ਲੰਬਾਈ (str);

  ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + len;
  ਅਚਲ ਅੱਖਰ* part = strpbrk (str, ":");
  ਜੇ (!part)
  {
    // Single value.
    ਜੇ (strcmp (str, "drop") == 0)
    {
      *axis_min_ਮੁੱਲ = NAN;
      *axis_def_ਮੁੱਲ = NAN;
      *axis_max_ਮੁੱਲ = NAN;
      ਵਾਪਸ ਸੱਚ;
    }

    ਦੁੱਗਣਾ v;
    ਜੇ (!hb_parse_double (&str, ਸਿਰਾ, &v)) ਵਾਪਸ ਝੂਠ;

    *axis_min_ਮੁੱਲ = v;
    *axis_def_ਮੁੱਲ = v;
    *axis_max_ਮੁੱਲ = v;
    ਵਾਪਸ ਸੱਚ;
  }

  ਦਸ਼ਮਲਵ values[3];
  ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
  ਲਈ (ਪੂਰਨ i = 0; i < 3; i++) {
    ਗਿਣਤੀ_2++;
    ਜੇ (!*str || part == str)
    {
      values[i] = NAN;

      ਜੇ (part == ਨਿਰਾ) ਰੋਕੋ;
      str = part + 1;
      part = strpbrk (str, ":");
      ਜਾਰੀ;
    }

    ਦੁੱਗਣਾ v;
    ਅਚਲ ਅੱਖਰ *piece_ਸਿਰਾ = part ? part : ਸਿਰਾ;
    ਜੇ (!hb_parse_double (&str, piece_ਸਿਰਾ, &v)) ਵਾਪਸ ਝੂਠ;
    values[i] = v;

    ਜੇ (part == ਨਿਰਾ) ਰੋਕੋ;
    str = part + 1;
    part = strpbrk (str, ":");
  }

  ਜੇ (ਗਿਣਤੀ_2 == 2)
  {
    *axis_min_ਮੁੱਲ = values[0];
    *axis_def_ਮੁੱਲ = NAN;
    *axis_max_ਮੁੱਲ = values[1];
    ਵਾਪਸ ਸੱਚ;
  }
  ਨਹੀਂਤਾਂ ਜੇ (ਗਿਣਤੀ_2 == 3)
  {
    *axis_min_ਮੁੱਲ = values[0];
    *axis_def_ਮੁੱਲ = values[1];
    *axis_max_ਮੁੱਲ = values[2];
    ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
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
HB_EXTERN ਖਾਲੀ
hb_subset_axis_range_ਮੰਜ਼ਿਲ_string (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼,
                                hb_ਟੈਗ_t axis_ਟੈਗ_2,
                                ਅੱਖਰ *buf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ)
{
  ਜੇ (unlikely (!ਮਾਪ)) ਵਾਪਸ;
  Triple* triple;
  ਜੇ (!ਨਿਵੇਸ਼->axes_location.has(axis_ਟੈਗ_2, &triple)) {
    ਵਾਪਸ;
  }

  ਅੱਖਰ s[128];
  ਬਿਨਾਨਿਸ਼ਾਨੀ len = 0;

  hb_locale_t clocale HB_ਅਣਵਰਤਿਆ;
  hb_locale_t oldlocale HB_ਅਣਵਰਤਿਆ;
  oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", ਨਿਰਾ));
  len += hb_max (0, snprintf (s, ARRAY_ਲੰਮਾਈ (s) - len, "%g", (ਦੁੱਗਣਾ) triple->minimum));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_ਲੰਮਾਈ (s) - len, "%g", (ਦੁੱਗਣਾ) triple->ਵਿਚਕਾਰ));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_ਲੰਮਾਈ (s) - len, "%g", (ਦੁੱਗਣਾ) triple->ਵੱਧ_ਤੋਂ_ਵੱਧ));
  (ਖਾਲੀ) hb_uselocale (((ਖਾਲੀ) freelocale (clocale), oldlocale));

  ਪ੍ਰਤਿਜ੍ਞਾ (len < ARRAY_ਲੰਮਾਈ (s));
  len = hb_min (len, ਮਾਪ - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
ਸਥਿਰ ਖਾਲੀ
_append_arg (hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &sb, ਅਚਲ ਅੱਖਰ *arg)
{
  ਜੇ (sb.ਵਿੱਚ_error ()) ਵਾਪਸ;
  ਜੇ (sb.ਲੰਮਾਈ)
    sb.push (' ');
  ਬਿਨਾਨਿਸ਼ਾਨੀ len = ਲੜੀ_ਲੰਬਾਈ (arg);
  sb.extend (hb_ਬਾਈਟਾਂ_t (arg, len));
}
ਸਥਿਰ ਖਾਲੀ
format_ਸੰਖਿਆ_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &ਬਾਹਰਲਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ = 10)
{
  ਪ੍ਰਤਿਜ੍ਞਾ (ਅਧਾਰ == 10 || ਅਧਾਰ == 16);
  ਜੇ (unlikely (ਅਧਾਰ != 10 && ਅਧਾਰ != 16)) ਵਾਪਸ;

  hb_codepoint_t ਪਹਿਲਾ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID, ਆਖਰੀ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
  ਤਾਰਕਿਕ ਪਹਿਲਾ_range = ਸੱਚ;
  ਜਦੋਂ (hb_ਸੈੱਟ_ਅਗਲਾ_range (ਸੈੱਟ, &ਪਹਿਲਾ, &ਆਖਰੀ))
  {
    ਜੇ (!ਪਹਿਲਾ_range)
      ਬਾਹਰਲਾ.push (',');
    ਪਹਿਲਾ_range = ਝੂਠ;

    ਅੱਖਰ buf[64];
    ਪੂਰਨ len;
    ਜੇ (ਪਹਿਲਾ == ਆਖਰੀ)
    {
      ਜੇ (ਅਧਾਰ == 16)
        len = snprintf (buf, ਆਕਾਰ (buf), "%X", ਪਹਿਲਾ);
      ਨਹੀਂਤਾਂ
        len = snprintf (buf, ਆਕਾਰ (buf), "%u", ਪਹਿਲਾ);
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (ਅਧਾਰ == 16)
        len = snprintf (buf, ਆਕਾਰ (buf), "%X-%X", ਪਹਿਲਾ, ਆਖਰੀ);
      ਨਹੀਂਤਾਂ
        len = snprintf (buf, ਆਕਾਰ (buf), "%u-%u", ਪਹਿਲਾ, ਆਖਰੀ);
    }
    ਜੇ (unlikely (len < 0 || (size_t) len >= ਆਕਾਰ (buf))) ਵਾਪਸ;
    ਬਾਹਰਲਾ.extend (hb_ਬਾਈਟਾਂ_t (buf, len));
  }
}

ਸਥਿਰ ਖਾਲੀ
format_ਟੈਗ (hb_ਟੈਗ_t ਟੈਗ_2, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &ਬਾਹਰਲਾ)
{
  ਅੱਖਰ buf[4];
  hb_ਟੈਗ_ਮੰਜ਼ਿਲ_string (ਟੈਗ_2, buf);

  ਬਿਨਾਨਿਸ਼ਾਨੀ len = 4;
  ਜਦੋਂ (len > 0 && buf[len - 1] == ' ')
    len--;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len; i++)
  {
    ਅੱਖਰ c = buf[i];
    ਜੇ (!((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '-' || c == '_'))
    {
      c = '_';
    }
    ਬਾਹਰਲਾ.push (c);
  }
}

ਸਥਿਰ ਖਾਲੀ
format_ਟੈਗ_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &ਬਾਹਰਲਾ)
{
  ਤਾਰਕਿਕ ਪਹਿਲਾ_ਟੈਗ = ਸੱਚ;
  ਲਈ (hb_codepoint_t ਟੈਗ_2 : *ਸੈੱਟ)
  {
    ਜੇ (!ਪਹਿਲਾ_ਟੈਗ)
      ਬਾਹਰਲਾ.push (',');
    ਪਹਿਲਾ_ਟੈਗ = ਝੂਠ;

    format_ਟੈਗ (ਟੈਗ_2, ਬਾਹਰਲਾ);
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
is_ਸੈੱਟ_default (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ, ਅਚਲ hb_ਸੈੱਟ_t *default_ਸੈੱਟ)
{
  ਵਾਪਸ hb_ਸੈੱਟ_is_inverted (ਸੈੱਟ) == hb_ਸੈੱਟ_is_inverted (default_ਸੈੱਟ) &&
         hb_ਸੈੱਟ_is_equal (ਸੈੱਟ, default_ਸੈੱਟ);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Formatter>
ਸਥਿਰ ਖਾਲੀ
format_ਸੈੱਟ_arg (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
                 ਅਚਲ ਅੱਖਰ *opt_name,
                 Formatter &&formatter,
                 hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &sb)
{
  ਜੇ (hb_ਸੈੱਟ_is_inverted (ਸੈੱਟ))
  {
    hb_ਸੈੱਟ_t *excluded = hb_ਸੈੱਟ_copy (ਸੈੱਟ);
    hb_ਸੈੱਟ_invert (excluded);

    hb_ਸਦਿਸ਼_t<ਅੱਖਰ> opt;
    opt.extend (hb_ਬਾਈਟਾਂ_t ("--", 2));
    opt.extend (hb_ਬਾਈਟਾਂ_t (opt_name, ਲੜੀ_ਲੰਬਾਈ (opt_name)));
    opt.extend (hb_ਬਾਈਟਾਂ_t ("=*", 2));
    opt.push ('\0');
    ਜੇ (opt.arrayZ) _append_arg (sb, opt.arrayZ);

    ਜੇ (!hb_ਸੈੱਟ_is_ਖਾਲੀ_ਥਾਵਾਂ (excluded))
    {
      hb_ਸਦਿਸ਼_t<ਅੱਖਰ> formatted;
      formatter (excluded, formatted);
      ਜੇ (formatted.ਲੰਮਾਈ)
      {
        hb_ਸਦਿਸ਼_t<ਅੱਖਰ> opt2;
        opt2.extend (hb_ਬਾਈਟਾਂ_t ("--", 2));
        opt2.extend (hb_ਬਾਈਟਾਂ_t (opt_name, ਲੜੀ_ਲੰਬਾਈ (opt_name)));
        opt2.extend (hb_ਬਾਈਟਾਂ_t ("-=", 2));
        opt2.extend (formatted.as_array ());
        opt2.push ('\0');
        ਜੇ (opt2.arrayZ) _append_arg (sb, opt2.arrayZ);
      }
    }
    hb_ਸੈੱਟ_destroy (excluded);
  }
  ਨਹੀਂਤਾਂ ਜੇ (hb_ਸੈੱਟ_is_ਖਾਲੀ_ਥਾਵਾਂ (ਸੈੱਟ))
  {
    hb_ਸਦਿਸ਼_t<ਅੱਖਰ> opt;
    opt.extend (hb_ਬਾਈਟਾਂ_t ("--", 2));
    opt.extend (hb_ਬਾਈਟਾਂ_t (opt_name, ਲੜੀ_ਲੰਬਾਈ (opt_name)));
    opt.extend (hb_ਬਾਈਟਾਂ_t ("-=*", 3));
    opt.push ('\0');
    ਜੇ (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  ਨਹੀਂਤਾਂ
  {
    hb_ਸਦਿਸ਼_t<ਅੱਖਰ> formatted;
    formatter (ਸੈੱਟ, formatted);
    ਜੇ (formatted.ਲੰਮਾਈ)
    {
      hb_ਸਦਿਸ਼_t<ਅੱਖਰ> opt;
      opt.extend (hb_ਬਾਈਟਾਂ_t ("--", 2));
      opt.extend (hb_ਬਾਈਟਾਂ_t (opt_name, ਲੜੀ_ਲੰਬਾਈ (opt_name)));
      opt.extend (hb_ਬਾਈਟਾਂ_t ("=", 1));
      opt.extend (formatted.as_array ());
      opt.push ('\0');
      ਜੇ (opt.arrayZ) _append_arg (sb, opt.arrayZ);
    }
  }
}

ਸਥਿਰ ਖਾਲੀ
format_ਸੰਖਿਆ_ਸੈੱਟ_arg (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
                        ਅਚਲ ਅੱਖਰ *opt_name,
                        hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &sb,
                        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ = 10)
{
  format_ਸੈੱਟ_arg (ਸੈੱਟ, opt_name, [ਅਧਾਰ] (ਅਚਲ hb_ਸੈੱਟ_t *s, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &ਬਾਹਰਲਾ) {
    format_ਸੰਖਿਆ_ਸੈੱਟ (s, ਬਾਹਰਲਾ, ਅਧਾਰ);
  }, sb);
}

ਸਥਿਰ ਖਾਲੀ
format_ਟੈਗ_ਸੈੱਟ_arg (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ,
                     ਅਚਲ ਅੱਖਰ *opt_name,
                     hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &sb)
{
  format_ਸੈੱਟ_arg (ਸੈੱਟ, opt_name, format_ਟੈਗ_ਸੈੱਟ, sb);
}

ਸਥਿਰ ਤਾਰਕਿਕ
_format_glyph_map_arg (ਅਚਲ hb_map_t &glyph_map,
                       hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &sb)
{
  ਜੇ (glyph_map.is_empty ())
    ਵਾਪਸ ਸੱਚ;

  hb_ਸੈੱਟ_t ਕੁੰਜੀਆਂ;
  hb_map_ਕੁੰਜੀਆਂ (&glyph_map, &ਕੁੰਜੀਆਂ);
  hb_ਸਦਿਸ਼_t<ਅੱਖਰ> ਜੋੜੇ;
  ਤਾਰਕਿਕ ਪਹਿਲਾ = ਸੱਚ;
  ਲਈ (hb_codepoint_t key : ਕੁੰਜੀਆਂ)
  {
    ਜੇ (!ਪਹਿਲਾ)
      ਜੋੜੇ.push (',');
    ਪਹਿਲਾ = ਝੂਠ;
    hb_codepoint_t val = hb_map_get (&glyph_map, key);
    ਅੱਖਰ buf[64];
    ਪੂਰਨ len = snprintf (buf, ਆਕਾਰ (buf), "%u:%u", key, val);
    ਜੇ (unlikely (len < 0 || (size_t) len >= ਆਕਾਰ (buf))) ਵਾਪਸ ਝੂਠ;
    ਜੋੜੇ.extend (hb_ਬਾਈਟਾਂ_t (buf, len));
  }
  ਜੇ (ਜੋੜੇ.ਲੰਮਾਈ)
  {
    hb_ਸਦਿਸ਼_t<ਅੱਖਰ> opt;
    opt.extend (hb_ਬਾਈਟਾਂ_t ("--gid-map=", 10));
    opt.extend (ਜੋੜੇ.as_array ());
    opt.push ('\0');
    ਜੇ (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
_format_axes_location_arg (ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> &axes_location,
                           hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &sb)
{
  ਜੇ (axes_location.is_empty ())
    ਵਾਪਸ ਸੱਚ;

  hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> axis_ਟੈਗ_ਸੂਚੀ;
  ਲਈ (ਸਵੈਚਾਲਿਤ ਟੈਗ_2 : axes_location.ਕੁੰਜੀਆਂ ())
    axis_ਟੈਗ_ਸੂਚੀ.push (ਟੈਗ_2);
  axis_ਟੈਗ_ਸੂਚੀ.qsort ([] (ਅਚਲ hb_ਟੈਗ_t &a, ਅਚਲ hb_ਟੈਗ_t &b) { ਵਾਪਸ a < b ? -1 : a > b ? 1 : 0; });

  hb_ਸਦਿਸ਼_t<ਅੱਖਰ> specs;
  ਤਾਰਕਿਕ ਪਹਿਲਾ = ਸੱਚ;
  ਲਈ (hb_ਟੈਗ_t ਟੈਗ_2 : axis_ਟੈਗ_ਸੂਚੀ)
  {
    Triple triple = axes_location.get (ਟੈਗ_2);
    ਜੇ (!ਪਹਿਲਾ)
      specs.push (',');
    ਪਹਿਲਾ = ਝੂਠ;

    hb_ਸਦਿਸ਼_t<ਅੱਖਰ> ਟੈਗ_buf;
    format_ਟੈਗ (ਟੈਗ_2, ਟੈਗ_buf);

    ਅੱਖਰ buf[128];
    ਪੂਰਨ len;
    ਜੇ (triple.minimum == triple.ਵਿਚਕਾਰ && triple.ਵਿਚਕਾਰ == triple.ਵੱਧ_ਤੋਂ_ਵੱਧ)
    {
      len = snprintf (buf, ਆਕਾਰ (buf), "%.*s=%g", (ਪੂਰਨ) ਟੈਗ_buf.ਲੰਮਾਈ, ਟੈਗ_buf.arrayZ, (ਦੁੱਗਣਾ) triple.ਵਿਚਕਾਰ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (std::isnan (triple.minimum) && std::isnan (triple.ਵਿਚਕਾਰ) && std::isnan (triple.ਵੱਧ_ਤੋਂ_ਵੱਧ))
    {
      len = snprintf (buf, ਆਕਾਰ (buf), "%.*s=drop", (ਪੂਰਨ) ਟੈਗ_buf.ਲੰਮਾਈ, ਟੈਗ_buf.arrayZ);
    }
    ਨਹੀਂਤਾਂ
    {
      len = snprintf (buf, ਆਕਾਰ (buf), "%.*s=%g:%g:%g", (ਪੂਰਨ) ਟੈਗ_buf.ਲੰਮਾਈ, ਟੈਗ_buf.arrayZ,
                      (ਦੁੱਗਣਾ) triple.minimum, (ਦੁੱਗਣਾ) triple.ਵਿਚਕਾਰ, (ਦੁੱਗਣਾ) triple.ਵੱਧ_ਤੋਂ_ਵੱਧ);
    }
    ਜੇ (unlikely (len < 0 || (size_t) len >= ਆਕਾਰ (buf))) ਵਾਪਸ ਝੂਠ;
    specs.extend (hb_ਬਾਈਟਾਂ_t (buf, len));
  }
  ਜੇ (specs.ਲੰਮਾਈ)
  {
    hb_ਸਦਿਸ਼_t<ਅੱਖਰ> opt;
    opt.extend (hb_ਬਾਈਟਾਂ_t ("--variations=", 13));
    opt.extend (specs.as_array ());
    opt.push ('\0');
    ਜੇ (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  ਵਾਪਸ ਸੱਚ;
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
hb_subset_ਨਿਵੇਸ਼_ਮੰਜ਼ਿਲ_string_or_fail (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  ਜੇ (unlikely (!ਨਿਵੇਸ਼ || ਨਿਵੇਸ਼->ਵਿੱਚ_error ()))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_ਸਦਿਸ਼_t<ਅੱਖਰ> sb;

  ਬਣਤਰ flag_option_t {
    hb_subset_flags_t flag;
    ਅੱਖਰ option[32];
  };
  ਸਥਿਰ ਅਚਲ flag_option_t flag_options[] = {
    { HB_SUBSET_FLAGS_NO_HINTING, "no-hinting" },
    { HB_SUBSET_FLAGS_RETAIN_GIDS, "retain-gids" },
    { HB_SUBSET_FLAGS_DESUBROUTINIZE, "desubroutinize" },
    { HB_SUBSET_FLAGS_NAME_LEGACY, "name-legacy" },
    { HB_SUBSET_FLAGS_ਸੈੱਟ_OVERLAPS_FLAG, "set-overlaps-flag" },
    { HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED, "passthrough-tables" },
    { HB_SUBSET_FLAGS_NOTDEF_OUTLINE, "notdef-outline" },
    { HB_SUBSET_FLAGS_GLYPH_ਨਾਂ, "glyph-names" },
    { HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES, "no-prune-unicode-ranges" },
    { HB_SUBSET_FLAGS_NO_LAYOUT_ਘੇਰਾ, "no-layout-closure" },
    { HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS, "optimize" },
    { HB_SUBSET_FLAGS_NO_BIDI_ਘੇਰਾ, "no-bidi-closure" },
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    { HB_SUBSET_FLAGS_IFTB_REQUIREMENTS, "iftb-requirements" },
    { HB_SUBSET_FLAGS_RETAIN_ਅੰਕ_GLYPHS, "retain-num-glyphs" },
#ਜੇ_ਅੰਤ
    { HB_SUBSET_FLAGS_DOWNGRADE_CFF2, "downgrade-cff2" },
  };

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
  static_assert (ਆਕਾਰ (flag_options) / ਆਕਾਰ (flag_options[0]) == 15,
                 "Check all flags in hb_subset_flags_t are handled here.");
#ਨਹੀਂਤਾਂ
  static_assert (ਆਕਾਰ (flag_options) / ਆਕਾਰ (flag_options[0]) == 13,
                 "Check all flags in hb_subset_flags_t are handled here.");
#ਜੇ_ਅੰਤ

  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &fo : flag_options)
  {
    ਜੇ (ਨਿਵੇਸ਼->flags & fo.flag)
    {
      ਅੱਖਰ buf[36];
      snprintf (buf, ਆਕਾਰ (buf), "--%s", fo.option);
      _append_arg (sb, buf);
    }
  }

  hb_subset_ਨਿਵੇਸ਼_t *default_ਨਿਵੇਸ਼ = hb_subset_ਨਿਵੇਸ਼_create_or_fail ();
  ਜੇ (unlikely (!default_ਨਿਵੇਸ਼))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  // Serialize all sets if they are not equal to their default setting:
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_ਸੰਖਿਆ_ਸੈੱਟ(ਖੇਤਰ, opt_name, ...) \
  ਜੇ (!is_ਸੈੱਟ_default (ਨਿਵੇਸ਼->sets.ਖੇਤਰ, default_ਨਿਵੇਸ਼->sets.ਖੇਤਰ)) \
    format_ਸੰਖਿਆ_ਸੈੱਟ_arg (ਨਿਵੇਸ਼->sets.ਖੇਤਰ, opt_name, sb, ##__VA_ARGS__)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_ਟੈਗ_ਸੈੱਟ(ਖੇਤਰ, opt_name) \
  ਜੇ (!is_ਸੈੱਟ_default (ਨਿਵੇਸ਼->sets.ਖੇਤਰ, default_ਨਿਵੇਸ਼->sets.ਖੇਤਰ)) \
    format_ਟੈਗ_ਸੈੱਟ_arg (ਨਿਵੇਸ਼->sets.ਖੇਤਰ, opt_name, sb)

  PROCESS_ਸੰਖਿਆ_ਸੈੱਟ (unicodes, "unicodes", 16);
  PROCESS_ਸੰਖਿਆ_ਸੈੱਟ (glyphs, "gids");
  PROCESS_ਸੰਖਿਆ_ਸੈੱਟ (name_ids, "name-IDs");
  PROCESS_ਸੰਖਿਆ_ਸੈੱਟ (name_languages, "name-languages");
  PROCESS_ਟੈਗ_ਸੈੱਟ (layout_features, "layout-features");
  PROCESS_ਟੈਗ_ਸੈੱਟ (layout_scripts, "layout-scripts");
  PROCESS_ਟੈਗ_ਸੈੱਟ (drop_tables, "drop-tables");
  PROCESS_ਟੈਗ_ਸੈੱਟ (no_subset_tables, "passthrough-tables");

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_ਸੰਖਿਆ_ਸੈੱਟ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_ਟੈਗ_ਸੈੱਟ

  hb_subset_ਨਿਵੇਸ਼_destroy (default_ਨਿਵੇਸ਼);

  static_assert (ਆਕਾਰ (ਨਿਵੇਸ਼->sets) / ਆਕਾਰ (hb_ਸੈੱਟ_t*) == 8,
                 "Check all sets in hb_subset_input_t::sets_t are handled here.");

  ਜੇ (unlikely (!_format_glyph_map_arg (ਨਿਵੇਸ਼->glyph_map, sb)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (unlikely (!_format_axes_location_arg (ਨਿਵੇਸ਼->axes_location, sb)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  sb.push ('\0');
  ਜੇ (unlikely (sb.ਵਿੱਚ_error ()))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ len = 0;
  ਅੱਖਰ *buf = sb.steal (&len);
  ਜੇ (unlikely (!buf))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ hb_blob_create_or_fail (buf, len, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE, buf, hb_free);
}
#ਜੇ_ਅੰਤ

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
hb_subset_preprocess (hb_face_t *ਸਰੋਤ)
{
  hb_subset_ਨਿਵੇਸ਼_t* ਨਿਵੇਸ਼ = hb_subset_ਨਿਵੇਸ਼_create_or_fail ();
  ਜੇ (!ਨਿਵੇਸ਼)
    ਵਾਪਸ hb_face_reference (ਸਰੋਤ);

  hb_subset_ਨਿਵੇਸ਼_keep_everything (ਨਿਵੇਸ਼);

  ਨਿਵੇਸ਼->attach_accelerator_ਅੰਕੜੇ = ਸੱਚ;

  // Always use long loca in the preprocessed version. This allows
  // us to store the glyph bytes unpadded which allows the future subset
  // operation to run faster by skipping the trim padding step.
  ਨਿਵੇਸ਼->force_long_loca = ਸੱਚ;

  hb_face_t* new_ਸਰੋਤ = hb_subset_or_fail (ਸਰੋਤ, ਨਿਵੇਸ਼);
  hb_subset_ਨਿਵੇਸ਼_destroy (ਨਿਵੇਸ਼);

  ਜੇ (!new_ਸਰੋਤ) {
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "Preprocessing failed due to subset failure.");
    ਵਾਪਸ hb_face_reference (ਸਰੋਤ);
  }

  ਵਾਪਸ new_ਸਰੋਤ;
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
hb_subset_ਨਿਵੇਸ਼_old_ਮੰਜ਼ਿਲ_new_glyph_mapping (hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  ਵਾਪਸ &ਨਿਵੇਸ਼->glyph_map;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
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
hb_subset_ਨਿਵੇਸ਼_override_name_ਸਾਰਣੀ (hb_subset_ਨਿਵੇਸ਼_t  *ਨਿਵੇਸ਼,
                                     hb_ot_name_id_t     name_id,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ            platform_id,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ            encoding_id,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ            language_id,
                                     ਅਚਲ ਅੱਖਰ         *name_str,
                                     ਪੂਰਨ                 str_len /* -1 means nul-terminated */)
{
  ਜੇ (!name_str)
  {
    str_len = 0;
  }
  ਨਹੀਂਤਾਂ ਜੇ (str_len == -1)
  {
      str_len = ਲੜੀ_ਲੰਬਾਈ (name_str);
  }

  hb_ਬਾਈਟਾਂ_t name_ਬਾਈਟਾਂ (ਖਾਲੀ_ਸੂਚਕ, 0);
  ਜੇ (str_len)
  {
    ਜੇ (platform_id == 1)
    {
      ਅਚਲ uint8_t *src = reinterpret_cast<ਅਚਲ uint8_t*> (name_str);
      ਅਚਲ uint8_t *src_ਸਿਰਾ = src + str_len;

      hb_codepoint_t unicode;
      ਅਚਲ hb_codepoint_t replacement = HB_ਬਫਰ_REPLACEMENT_CODEPOINT_DEFAULT;
      ਜਦੋਂ (src < src_ਸਿਰਾ)
      {
        src = hb_utf8_t::next (src, src_ਸਿਰਾ, &unicode, replacement);
        ਜੇ (unicode >= 0x0080u)
        {
          // Non-ascii character detected, ignored...
          ਵਾਪਸ ਝੂਠ;
        }
      }
    }
    ਅੱਖਰ *override_name = (ਅੱਖਰ *) hb_malloc (str_len);
    ਜੇ (unlikely (!override_name)) ਵਾਪਸ ਝੂਠ;

    hb_memcpy (override_name, name_str, str_len);
    name_ਬਾਈਟਾਂ = hb_ਬਾਈਟਾਂ_t (override_name, str_len);
  }
  ਨਿਵੇਸ਼->name_ਸਾਰਣੀ_overrides.ਸੈੱਟ (hb_ot_name_ਰਿਕਾਰਡ_ids_t (platform_id, encoding_id, language_id, name_id), name_ਬਾਈਟਾਂ);
  ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ
