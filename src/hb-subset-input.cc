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

#అన్తర్భావ "hb-subset-instancer-solver.hh"
#అన్తర్భావ "hb-subset.hh"
#అన్తర్భావ "hb-set.hh"
#అన్తర్భావ "hb-utf.hh"


hb_subset_ఆదానం_t::hb_subset_ఆదానం_t ()
{
  కొరకు (స్వయంచాలక& సమితి : sets_iter ())
    సమితి = hb::shared_ptr<hb_సమితి_t> (hb_సమితి_create ());

  ఒకవేళ (లోపల_error ())
    తిరిగి;

  flags = HB_SUBSET_FLAGS_DEFAULT;

  hb_సమితి_కూడు_range (sets.name_ids, 0, 6);
  hb_సమితి_కూడు (sets.name_languages, 0x0409);

  hb_ట్యాగ్_t default_drop_tables[] = {
    // Layout disabled by default
    HB_ట్యాగ్ ('m', 'o', 'r', 'x'),
    HB_ట్యాగ్ ('m', 'o', 'r', 't'),
    HB_ట్యాగ్ ('k', 'e', 'r', 'x'),
    HB_ట్యాగ్ ('k', 'e', 'r', 'n'),

    // Copied from fontTools:
    HB_ట్యాగ్ ('J', 'S', 'T', 'F'),
    HB_ట్యాగ్ ('D', 'S', 'I', 'G'),
    HB_ట్యాగ్ ('E', 'B', 'D', 'T'),
    HB_ట్యాగ్ ('E', 'B', 'L', 'C'),
    HB_ట్యాగ్ ('E', 'B', 'S', 'C'),
    HB_ట్యాగ్ ('S', 'V', 'G', ' '),
    HB_ట్యాగ్ ('P', 'C', 'L', 'T'),
    HB_ట్యాగ్ ('L', 'T', 'S', 'H'),
    // Graphite tables
    HB_ట్యాగ్ ('F', 'e', 'a', 't'),
    HB_ట్యాగ్ ('G', 'l', 'a', 't'),
    HB_ట్యాగ్ ('G', 'l', 'o', 'c'),
    HB_ట్యాగ్ ('S', 'i', 'l', 'f'),
    HB_ట్యాగ్ ('S', 'i', 'l', 'l'),
  };
  sets.drop_tables->కూడు_array (default_drop_tables, ARRAY_నిడివి (default_drop_tables));

  hb_ట్యాగ్_t default_no_subset_tables[] = {
    HB_ట్యాగ్ ('g', 'a', 's', 'p'),
    HB_ట్యాగ్ ('f', 'p', 'g', 'm'),
    HB_ట్యాగ్ ('p', 'r', 'e', 'p'),
    HB_ట్యాగ్ ('V', 'D', 'M', 'X'),
    HB_ట్యాగ్ ('D', 'S', 'I', 'G'),
  };
  sets.no_subset_tables->కూడు_array (default_no_subset_tables,
					 ARRAY_నిడివి (default_no_subset_tables));

  //copied from _layout_features_groups in fonttools
  hb_ట్యాగ్_t default_layout_features[] = {
    // default shaper
    // common
    HB_ట్యాగ్ ('r', 'v', 'r', 'n'),
    HB_ట్యాగ్ ('c', 'c', 'm', 'p'),
    HB_ట్యాగ్ ('l', 'i', 'g', 'a'),
    HB_ట్యాగ్ ('l', 'o', 'c', 'l'),
    HB_ట్యాగ్ ('m', 'a', 'r', 'k'),
    HB_ట్యాగ్ ('m', 'k', 'm', 'k'),
    HB_ట్యాగ్ ('r', 'l', 'i', 'g'),

    //fractions
    HB_ట్యాగ్ ('f', 'r', 'a', 'c'),
    HB_ట్యాగ్ ('n', 'u', 'm', 'r'),
    HB_ట్యాగ్ ('d', 'n', 'o', 'm'),

    //horizontal
    HB_ట్యాగ్ ('c', 'a', 'l', 't'),
    HB_ట్యాగ్ ('c', 'l', 'i', 'g'),
    HB_ట్యాగ్ ('c', 'u', 'r', 's'),
    HB_ట్యాగ్ ('k', 'e', 'r', 'n'),
    HB_ట్యాగ్ ('r', 'c', 'l', 't'),

    //vertical
    HB_ట్యాగ్ ('v', 'a', 'l', 't'),
    HB_ట్యాగ్ ('v', 'e', 'r', 't'),
    HB_ట్యాగ్ ('v', 'k', 'r', 'n'),
    HB_ట్యాగ్ ('v', 'p', 'a', 'l'),
    HB_ట్యాగ్ ('v', 'r', 't', '2'),

    //ltr
    HB_ట్యాగ్ ('l', 't', 'r', 'a'),
    HB_ట్యాగ్ ('l', 't', 'r', 'm'),

    //rtl
    HB_ట్యాగ్ ('r', 't', 'l', 'a'),
    HB_ట్యాగ్ ('r', 't', 'l', 'm'),

    //random
    HB_ట్యాగ్ ('r', 'a', 'n', 'd'),

    //justify
    HB_ట్యాగ్ ('j', 'a', 'l', 't'), // HarfBuzz doesn't use; others might

    //East Asian spacing
    HB_ట్యాగ్ ('c', 'h', 'w', 's'),
    HB_ట్యాగ్ ('v', 'c', 'h', 'w'),
    HB_ట్యాగ్ ('h', 'a', 'l', 't'),
    HB_ట్యాగ్ ('v', 'h', 'a', 'l'),
    HB_ట్యాగ్ ('p', 'a', 'l', 't'),

    //వ్యక్తిగత
    HB_ట్యాగ్ ('H', 'a', 'r', 'f'),
    HB_ట్యాగ్ ('H', 'A', 'R', 'F'),
    HB_ట్యాగ్ ('B', 'u', 'z', 'z'),
    HB_ట్యాగ్ ('B', 'U', 'Z', 'Z'),

    //shapers

    //arabic
    HB_ట్యాగ్ ('i', 'n', 'i', 't'),
    HB_ట్యాగ్ ('m', 'e', 'd', 'i'),
    HB_ట్యాగ్ ('f', 'i', 'n', 'a'),
    HB_ట్యాగ్ ('i', 's', 'o', 'l'),
    HB_ట్యాగ్ ('m', 'e', 'd', '2'),
    HB_ట్యాగ్ ('f', 'i', 'n', '2'),
    HB_ట్యాగ్ ('f', 'i', 'n', '3'),
    HB_ట్యాగ్ ('c', 's', 'w', 'h'),
    HB_ట్యాగ్ ('m', 's', 'e', 't'),
    HB_ట్యాగ్ ('s', 't', 'c', 'h'),

    //hangul
    HB_ట్యాగ్ ('l', 'j', 'm', 'o'),
    HB_ట్యాగ్ ('v', 'j', 'm', 'o'),
    HB_ట్యాగ్ ('t', 'j', 'm', 'o'),

    //tibetan
    HB_ట్యాగ్ ('a', 'b', 'v', 's'),
    HB_ట్యాగ్ ('b', 'l', 'w', 's'),
    HB_ట్యాగ్ ('a', 'b', 'v', 'm'),
    HB_ట్యాగ్ ('b', 'l', 'w', 'm'),

    //indic
    HB_ట్యాగ్ ('n', 'u', 'k', 't'),
    HB_ట్యాగ్ ('a', 'k', 'h', 'n'),
    HB_ట్యాగ్ ('r', 'p', 'h', 'f'),
    HB_ట్యాగ్ ('r', 'k', 'r', 'f'),
    HB_ట్యాగ్ ('p', 'r', 'e', 'f'),
    HB_ట్యాగ్ ('b', 'l', 'w', 'f'),
    HB_ట్యాగ్ ('h', 'a', 'l', 'f'),
    HB_ట్యాగ్ ('a', 'b', 'v', 'f'),
    HB_ట్యాగ్ ('p', 's', 't', 'f'),
    HB_ట్యాగ్ ('c', 'f', 'a', 'r'),
    HB_ట్యాగ్ ('v', 'a', 't', 'u'),
    HB_ట్యాగ్ ('c', 'j', 'c', 't'),
    HB_ట్యాగ్ ('i', 'n', 'i', 't'),
    HB_ట్యాగ్ ('p', 'r', 'e', 's'),
    HB_ట్యాగ్ ('a', 'b', 'v', 's'),
    HB_ట్యాగ్ ('b', 'l', 'w', 's'),
    HB_ట్యాగ్ ('p', 's', 't', 's'),
    HB_ట్యాగ్ ('h', 'a', 'l', 'n'),
    HB_ట్యాగ్ ('d', 'i', 's', 't'),
    HB_ట్యాగ్ ('a', 'b', 'v', 'm'),
    HB_ట్యాగ్ ('b', 'l', 'w', 'm'),
  };

  sets.layout_features->కూడు_array (default_layout_features, ARRAY_నిడివి (default_layout_features));

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
hb_subset_ఆదానం_t *
hb_subset_ఆదానం_create_or_fail (శూన్య)
{
  hb_subset_ఆదానం_t *ఆదానం = hb_object_create<hb_subset_ఆదానం_t>();

  ఒకవేళ (unlikely (!ఆదానం))
    తిరిగి శూన్యసూచిక;

  ఒకవేళ (ఆదానం->లోపల_error ())
  {
    hb_subset_ఆదానం_destroy (ఆదానం);
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి ఆదానం;
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
hb_subset_ఆదానం_t *
hb_subset_ఆదానం_reference (hb_subset_ఆదానం_t *ఆదానం)
{
  తిరిగి hb_object_reference (ఆదానం);
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
శూన్య
hb_subset_ఆదానం_destroy (hb_subset_ఆదానం_t *ఆదానం)
{
  ఒకవేళ (!hb_object_destroy (ఆదానం)) తిరిగి;

  hb_free (ఆదానం);
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
HB_EXTERN hb_సమితి_t *
hb_subset_ఆదానం_unicode_సమితి (hb_subset_ఆదానం_t *ఆదానం)
{
  తిరిగి ఆదానం->sets.unicodes;
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
HB_EXTERN hb_సమితి_t *
hb_subset_ఆదానం_glyph_సమితి (hb_subset_ఆదానం_t *ఆదానం)
{
  తిరిగి ఆదానం->sets.glyphs;
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
HB_EXTERN hb_సమితి_t *
hb_subset_ఆదానం_సమితి (hb_subset_ఆదానం_t *ఆదానం, hb_subset_sets_t సమితి_type)
{
  తిరిగి ఆదానం->sets_iter () [సమితి_type];
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
hb_subset_ఆదానం_get_flags (hb_subset_ఆదానం_t *ఆదానం)
{
  తిరిగి (hb_subset_flags_t) ఆదానం->flags;
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
HB_EXTERN శూన్య
hb_subset_ఆదానం_సమితి_flags (hb_subset_ఆదానం_t *ఆదానం,
			   గుర్తులేని value)
{
  ఆదానం->flags = (hb_subset_flags_t) value;
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
hb_subset_ఆదానం_సమితి_user_దత్తాంశం (hb_subset_ఆదానం_t  *ఆదానం,
			       hb_user_దత్తాంశం_కీలకం_t *key,
			       శూన్య *		   దత్తాంశం,
			       hb_destroy_func_t   destroy,
			       hb_bool_t	   replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (ఆదానం, key, దత్తాంశం, destroy, replace);
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
శూన్య *
hb_subset_ఆదానం_get_user_దత్తాంశం (స్థిరరాశి hb_subset_ఆదానం_t *ఆదానం,
			       hb_user_దత్తాంశం_కీలకం_t     *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (ఆదానం, key);
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
శూన్య
hb_subset_ఆదానం_keep_everything (hb_subset_ఆదానం_t *ఆదానం)
{
  స్థిరరాశి hb_subset_sets_t indices[] = {HB_SUBSET_SETS_UNICODE,
				      HB_SUBSET_SETS_GLYPH_సూచీ,
				      HB_SUBSET_SETS_NAME_ID,
				      HB_SUBSET_SETS_NAME_LANG_ID,
				      HB_SUBSET_SETS_LAYOUT_FEATURE_ట్యాగ్,
				      HB_SUBSET_SETS_LAYOUT_SCRIPT_ట్యాగ్};

  కొరకు (స్వయంచాలక idx : hb_iter (indices))
  {
    hb_సమితి_t *సమితి = hb_subset_ఆదానం_సమితి (ఆదానం, idx);
    hb_సమితి_clear (సమితి);
    hb_సమితి_invert (సమితి);
  }

  // Don't drop any tables
  hb_సమితి_clear (hb_subset_ఆదానం_సమితి (ఆదానం, HB_SUBSET_SETS_DROP_పట్టిక_ట్యాగ్));

  hb_subset_ఆదానం_సమితి_flags (ఆదానం,
			     HB_SUBSET_FLAGS_NOTDEF_OUTLINE |
			     HB_SUBSET_FLAGS_GLYPH_పేర్లు |
			     HB_SUBSET_FLAGS_NAME_LEGACY |
			     HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES |
                             HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED);
}

#నిర్వచితం_కాకపోతే HB_NO_VAR
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
hb_subset_ఆదానం_pin_all_axes_గమ్యం_default (hb_subset_ఆదానం_t  *ఆదానం,
                                         hb_face_t          *face)
{
  గుర్తులేని axis_లెక్క = hb_ot_var_get_axis_లెక్క (face);
  ఒకవేళ (!axis_లెక్క) తిరిగి అబద్ధం;

  hb_ot_var_axis_info_t *axis_infos = (hb_ot_var_axis_info_t *) hb_calloc (axis_లెక్క, పరిమాణం (hb_ot_var_axis_info_t));
  ఒకవేళ (unlikely (!axis_infos)) తిరిగి అబద్ధం;

  (శూన్య) hb_ot_var_get_axis_infos (face, 0, &axis_లెక్క, axis_infos);

  కొరకు (గుర్తులేని i = 0; i < axis_లెక్క; i++)
  {
    hb_ట్యాగ్_t axis_ట్యాగ్_2 = axis_infos[i].ట్యాగ్_2;
    ద్విగుణ default_val = (ద్విగుణ) axis_infos[i].default_విలువ;
    ఒకవేళ (!ఆదానం->axes_location.సమితి (axis_ట్యాగ్_2, Triple (default_val, default_val, default_val)))
    {
      hb_free (axis_infos);
      తిరిగి అబద్ధం;
    }
  }
  hb_free (axis_infos);
  తిరిగి నిజం;
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
hb_subset_ఆదానం_pin_axis_గమ్యం_default (hb_subset_ఆదానం_t  *ఆదానం,
                                     hb_face_t          *face,
                                     hb_ట్యాగ్_t            axis_ట్యాగ్_2)
{
  hb_ot_var_axis_info_t axis_info;
  ఒకవేళ (!hb_ot_var_కనుగొను_axis_info (face, axis_ట్యాగ్_2, &axis_info))
    తిరిగి అబద్ధం;

  ద్విగుణ default_val = (ద్విగుణ) axis_info.default_విలువ;
  తిరిగి ఆదానం->axes_location.సమితి (axis_ట్యాగ్_2, Triple (default_val, default_val, default_val));
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
hb_subset_ఆదానం_pin_axis_location (hb_subset_ఆదానం_t  *ఆదానం,
                                   hb_face_t          *face,
                                   hb_ట్యాగ్_t            axis_ట్యాగ్_2,
                                   దశాంశం               axis_విలువ)
{
  hb_ot_var_axis_info_t axis_info;
  ఒకవేళ (!hb_ot_var_కనుగొను_axis_info (face, axis_ట్యాగ్_2, &axis_info))
    తిరిగి అబద్ధం;

  ద్విగుణ val = hb_clamp((ద్విగుణ) axis_విలువ, (ద్విగుణ) axis_info.min_విలువ, (ద్విగుణ) axis_info.max_విలువ);
  తిరిగి ఆదానం->axes_location.సమితి (axis_ట్యాగ్_2, Triple (val, val, val));
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
hb_subset_ఆదానం_సమితి_axis_range (hb_subset_ఆదానం_t  *ఆదానం,
                                hb_face_t          *face,
                                hb_ట్యాగ్_t            axis_ట్యాగ్_2,
                                దశాంశం               axis_min_విలువ,
                                దశాంశం               axis_max_విలువ,
                                దశాంశం               axis_def_విలువ)
{
  hb_ot_var_axis_info_t axis_info;
  ఒకవేళ (!hb_ot_var_కనుగొను_axis_info (face, axis_ట్యాగ్_2, &axis_info))
    తిరిగి అబద్ధం;

  దశాంశం min = !std::isnan(axis_min_విలువ) ? axis_min_విలువ : axis_info.min_విలువ;
  దశాంశం max = !std::isnan(axis_max_విలువ) ? axis_max_విలువ : axis_info.max_విలువ;
  దశాంశం def = !std::isnan(axis_def_విలువ) ? axis_def_విలువ : axis_info.default_విలువ;

  ఒకవేళ (min > max)
    తిరిగి అబద్ధం;

  దశాంశం new_min_val = hb_clamp(min, axis_info.min_విలువ, axis_info.max_విలువ);
  దశాంశం new_max_val = hb_clamp(max, axis_info.min_విలువ, axis_info.max_విలువ);
  దశాంశం new_default_val = hb_clamp(def, new_min_val, new_max_val);
  తిరిగి ఆదానం->axes_location.సమితి (axis_ట్యాగ్_2, Triple ((ద్విగుణ) new_min_val, (ద్విగుణ) new_default_val, (ద్విగుణ) new_max_val));
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
hb_subset_ఆదానం_get_axis_range (hb_subset_ఆదానం_t  *ఆదానం,
				hb_ట్యాగ్_t            axis_ట్యాగ్_2,
				దశాంశం              *axis_min_విలువ,
				దశాంశం              *axis_max_విలువ,
				దశాంశం              *axis_def_విలువ)

{
  Triple* triple;
  ఒకవేళ (!ఆదానం->axes_location.has(axis_ట్యాగ్_2, &triple)) {
    తిరిగి అబద్ధం;
  }

  *axis_min_విలువ = triple->minimum;
  *axis_def_విలువ = triple->మధ్య;
  *axis_max_విలువ = triple->గరిష్ఠ;
  తిరిగి నిజం;
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
hb_subset_axis_range_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len,
                                  దశాంశం *axis_min_విలువ,
                                  దశాంశం *axis_max_విలువ,
                                  దశాంశం *axis_def_విలువ)
{
  ఒకవేళ (len < 0)
    len = అక్షరశ్రేణి_పొడవు (str);

  స్థిరరాశి అక్షరం *చివర = str + len;
  స్థిరరాశి అక్షరం* part = strpbrk (str, ":");
  ఒకవేళ (!part)
  {
    // Single value.
    ఒకవేళ (strcmp (str, "drop") == 0)
    {
      *axis_min_విలువ = NAN;
      *axis_def_విలువ = NAN;
      *axis_max_విలువ = NAN;
      తిరిగి నిజం;
    }

    ద్విగుణ v;
    ఒకవేళ (!hb_parse_double (&str, చివర, &v)) తిరిగి అబద్ధం;

    *axis_min_విలువ = v;
    *axis_def_విలువ = v;
    *axis_max_విలువ = v;
    తిరిగి నిజం;
  }

  దశాంశం values[3];
  పూర్ణాంకం లెక్క = 0;
  కొరకు (పూర్ణాంకం i = 0; i < 3; i++) {
    లెక్క++;
    ఒకవేళ (!*str || part == str)
    {
      values[i] = NAN;

      ఒకవేళ (part == ఖాళీ) ఆపు;
      str = part + 1;
      part = strpbrk (str, ":");
      కొనసాగు;
    }

    ద్విగుణ v;
    స్థిరరాశి అక్షరం *piece_చివర = part ? part : చివర;
    ఒకవేళ (!hb_parse_double (&str, piece_చివర, &v)) తిరిగి అబద్ధం;
    values[i] = v;

    ఒకవేళ (part == ఖాళీ) ఆపు;
    str = part + 1;
    part = strpbrk (str, ":");
  }

  ఒకవేళ (లెక్క == 2)
  {
    *axis_min_విలువ = values[0];
    *axis_def_విలువ = NAN;
    *axis_max_విలువ = values[1];
    తిరిగి నిజం;
  }
  లేకపోతే ఒకవేళ (లెక్క == 3)
  {
    *axis_min_విలువ = values[0];
    *axis_def_విలువ = values[1];
    *axis_max_విలువ = values[2];
    తిరిగి నిజం;
  }
  తిరిగి అబద్ధం;
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
HB_EXTERN శూన్య
hb_subset_axis_range_గమ్యం_string (hb_subset_ఆదానం_t *ఆదానం,
                                hb_ట్యాగ్_t axis_ట్యాగ్_2,
                                అక్షరం *buf, గుర్తులేని కొలత)
{
  ఒకవేళ (unlikely (!కొలత)) తిరిగి;
  Triple* triple;
  ఒకవేళ (!ఆదానం->axes_location.has(axis_ట్యాగ్_2, &triple)) {
    తిరిగి;
  }

  అక్షరం s[128];
  గుర్తులేని len = 0;

  hb_locale_t clocale HB_వాడని;
  hb_locale_t oldlocale HB_వాడని;
  oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", ఖాళీ));
  len += hb_max (0, snprintf (s, ARRAY_నిడివి (s) - len, "%g", (ద్విగుణ) triple->minimum));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_నిడివి (s) - len, "%g", (ద్విగుణ) triple->మధ్య));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_నిడివి (s) - len, "%g", (ద్విగుణ) triple->గరిష్ఠ));
  (శూన్య) hb_uselocale (((శూన్య) freelocale (clocale), oldlocale));

  ప్రతిజ్ఞా (len < ARRAY_నిడివి (s));
  len = hb_min (len, కొలత - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}
#అయితే_ముగింపు

#నిర్వచితమైతే HB_EXPERIMENTAL_API
స్థిర శూన్య
_append_arg (hb_సదిశ_t<అక్షరం> &sb, స్థిరరాశి అక్షరం *arg)
{
  ఒకవేళ (sb.లోపల_error ()) తిరిగి;
  ఒకవేళ (sb.నిడివి)
    sb.push (' ');
  గుర్తులేని len = అక్షరశ్రేణి_పొడవు (arg);
  sb.extend (hb_బైట్లు_t (arg, len));
}
స్థిర శూన్య
format_సంఖ్య_సమితి (స్థిరరాశి hb_సమితి_t *సమితి, hb_సదిశ_t<అక్షరం> &బహిర్గతం, గుర్తులేని పునాది = 10)
{
  ప్రతిజ్ఞా (పునాది == 10 || పునాది == 16);
  ఒకవేళ (unlikely (పునాది != 10 && పునాది != 16)) తిరిగి;

  hb_codepoint_t మొదటి = HB_సమితి_విలువ_INVALID, చివరి = HB_సమితి_విలువ_INVALID;
  తార్కిక మొదటి_range = నిజం;
  అయితే (hb_సమితి_తదుపరి_range (సమితి, &మొదటి, &చివరి))
  {
    ఒకవేళ (!మొదటి_range)
      బహిర్గతం.push (',');
    మొదటి_range = అబద్ధం;

    అక్షరం buf[64];
    పూర్ణాంకం len;
    ఒకవేళ (మొదటి == చివరి)
    {
      ఒకవేళ (పునాది == 16)
        len = snprintf (buf, పరిమాణం (buf), "%X", మొదటి);
      లేకపోతే
        len = snprintf (buf, పరిమాణం (buf), "%u", మొదటి);
    }
    లేకపోతే
    {
      ఒకవేళ (పునాది == 16)
        len = snprintf (buf, పరిమాణం (buf), "%X-%X", మొదటి, చివరి);
      లేకపోతే
        len = snprintf (buf, పరిమాణం (buf), "%u-%u", మొదటి, చివరి);
    }
    ఒకవేళ (unlikely (len < 0 || (size_t) len >= పరిమాణం (buf))) తిరిగి;
    బహిర్గతం.extend (hb_బైట్లు_t (buf, len));
  }
}

స్థిర శూన్య
format_ట్యాగ్ (hb_ట్యాగ్_t ట్యాగ్_2, hb_సదిశ_t<అక్షరం> &బహిర్గతం)
{
  అక్షరం buf[4];
  hb_ట్యాగ్_గమ్యం_string (ట్యాగ్_2, buf);

  గుర్తులేని len = 4;
  అయితే (len > 0 && buf[len - 1] == ' ')
    len--;

  కొరకు (గుర్తులేని i = 0; i < len; i++)
  {
    అక్షరం c = buf[i];
    ఒకవేళ (!((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '-' || c == '_'))
    {
      c = '_';
    }
    బహిర్గతం.push (c);
  }
}

స్థిర శూన్య
format_ట్యాగ్_సమితి (స్థిరరాశి hb_సమితి_t *సమితి, hb_సదిశ_t<అక్షరం> &బహిర్గతం)
{
  తార్కిక మొదటి_ట్యాగ్ = నిజం;
  కొరకు (hb_codepoint_t ట్యాగ్_2 : *సమితి)
  {
    ఒకవేళ (!మొదటి_ట్యాగ్)
      బహిర్గతం.push (',');
    మొదటి_ట్యాగ్ = అబద్ధం;

    format_ట్యాగ్ (ట్యాగ్_2, బహిర్గతం);
  }
}

స్థిర తార్కిక
is_సమితి_default (స్థిరరాశి hb_సమితి_t *సమితి, స్థిరరాశి hb_సమితి_t *default_సమితి)
{
  తిరిగి hb_సమితి_is_inverted (సమితి) == hb_సమితి_is_inverted (default_సమితి) &&
         hb_సమితి_is_equal (సమితి, default_సమితి);
}

మూస <రకం_పేరు Formatter>
స్థిర శూన్య
format_సమితి_arg (స్థిరరాశి hb_సమితి_t *సమితి,
                 స్థిరరాశి అక్షరం *opt_name,
                 Formatter &&formatter,
                 hb_సదిశ_t<అక్షరం> &sb)
{
  ఒకవేళ (hb_సమితి_is_inverted (సమితి))
  {
    hb_సమితి_t *excluded = hb_సమితి_copy (సమితి);
    hb_సమితి_invert (excluded);

    hb_సదిశ_t<అక్షరం> opt;
    opt.extend (hb_బైట్లు_t ("--", 2));
    opt.extend (hb_బైట్లు_t (opt_name, అక్షరశ్రేణి_పొడవు (opt_name)));
    opt.extend (hb_బైట్లు_t ("=*", 2));
    opt.push ('\0');
    ఒకవేళ (opt.arrayZ) _append_arg (sb, opt.arrayZ);

    ఒకవేళ (!hb_సమితి_is_ఖాళీలు (excluded))
    {
      hb_సదిశ_t<అక్షరం> formatted;
      formatter (excluded, formatted);
      ఒకవేళ (formatted.నిడివి)
      {
        hb_సదిశ_t<అక్షరం> opt2;
        opt2.extend (hb_బైట్లు_t ("--", 2));
        opt2.extend (hb_బైట్లు_t (opt_name, అక్షరశ్రేణి_పొడవు (opt_name)));
        opt2.extend (hb_బైట్లు_t ("-=", 2));
        opt2.extend (formatted.as_array ());
        opt2.push ('\0');
        ఒకవేళ (opt2.arrayZ) _append_arg (sb, opt2.arrayZ);
      }
    }
    hb_సమితి_destroy (excluded);
  }
  లేకపోతే ఒకవేళ (hb_సమితి_is_ఖాళీలు (సమితి))
  {
    hb_సదిశ_t<అక్షరం> opt;
    opt.extend (hb_బైట్లు_t ("--", 2));
    opt.extend (hb_బైట్లు_t (opt_name, అక్షరశ్రేణి_పొడవు (opt_name)));
    opt.extend (hb_బైట్లు_t ("-=*", 3));
    opt.push ('\0');
    ఒకవేళ (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  లేకపోతే
  {
    hb_సదిశ_t<అక్షరం> formatted;
    formatter (సమితి, formatted);
    ఒకవేళ (formatted.నిడివి)
    {
      hb_సదిశ_t<అక్షరం> opt;
      opt.extend (hb_బైట్లు_t ("--", 2));
      opt.extend (hb_బైట్లు_t (opt_name, అక్షరశ్రేణి_పొడవు (opt_name)));
      opt.extend (hb_బైట్లు_t ("=", 1));
      opt.extend (formatted.as_array ());
      opt.push ('\0');
      ఒకవేళ (opt.arrayZ) _append_arg (sb, opt.arrayZ);
    }
  }
}

స్థిర శూన్య
format_సంఖ్య_సమితి_arg (స్థిరరాశి hb_సమితి_t *సమితి,
                        స్థిరరాశి అక్షరం *opt_name,
                        hb_సదిశ_t<అక్షరం> &sb,
                        గుర్తులేని పునాది = 10)
{
  format_సమితి_arg (సమితి, opt_name, [పునాది] (స్థిరరాశి hb_సమితి_t *s, hb_సదిశ_t<అక్షరం> &బహిర్గతం) {
    format_సంఖ్య_సమితి (s, బహిర్గతం, పునాది);
  }, sb);
}

స్థిర శూన్య
format_ట్యాగ్_సమితి_arg (స్థిరరాశి hb_సమితి_t *సమితి,
                     స్థిరరాశి అక్షరం *opt_name,
                     hb_సదిశ_t<అక్షరం> &sb)
{
  format_సమితి_arg (సమితి, opt_name, format_ట్యాగ్_సమితి, sb);
}

స్థిర తార్కిక
_format_glyph_map_arg (స్థిరరాశి hb_map_t &glyph_map,
                       hb_సదిశ_t<అక్షరం> &sb)
{
  ఒకవేళ (glyph_map.is_empty ())
    తిరిగి నిజం;

  hb_సమితి_t కీలకాలు;
  hb_map_కీలకాలు (&glyph_map, &కీలకాలు);
  hb_సదిశ_t<అక్షరం> జతలు;
  తార్కిక మొదటి = నిజం;
  కొరకు (hb_codepoint_t key : కీలకాలు)
  {
    ఒకవేళ (!మొదటి)
      జతలు.push (',');
    మొదటి = అబద్ధం;
    hb_codepoint_t val = hb_map_get (&glyph_map, key);
    అక్షరం buf[64];
    పూర్ణాంకం len = snprintf (buf, పరిమాణం (buf), "%u:%u", key, val);
    ఒకవేళ (unlikely (len < 0 || (size_t) len >= పరిమాణం (buf))) తిరిగి అబద్ధం;
    జతలు.extend (hb_బైట్లు_t (buf, len));
  }
  ఒకవేళ (జతలు.నిడివి)
  {
    hb_సదిశ_t<అక్షరం> opt;
    opt.extend (hb_బైట్లు_t ("--gid-map=", 10));
    opt.extend (జతలు.as_array ());
    opt.push ('\0');
    ఒకవేళ (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  తిరిగి నిజం;
}

స్థిర తార్కిక
_format_axes_location_arg (స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> &axes_location,
                           hb_సదిశ_t<అక్షరం> &sb)
{
  ఒకవేళ (axes_location.is_empty ())
    తిరిగి నిజం;

  hb_సదిశ_t<hb_ట్యాగ్_t> axis_ట్యాగులు;
  కొరకు (స్వయంచాలక ట్యాగ్_2 : axes_location.కీలకాలు ())
    axis_ట్యాగులు.push (ట్యాగ్_2);
  axis_ట్యాగులు.qsort ([] (స్థిరరాశి hb_ట్యాగ్_t &a, స్థిరరాశి hb_ట్యాగ్_t &b) { తిరిగి a < b ? -1 : a > b ? 1 : 0; });

  hb_సదిశ_t<అక్షరం> specs;
  తార్కిక మొదటి = నిజం;
  కొరకు (hb_ట్యాగ్_t ట్యాగ్_2 : axis_ట్యాగులు)
  {
    Triple triple = axes_location.get (ట్యాగ్_2);
    ఒకవేళ (!మొదటి)
      specs.push (',');
    మొదటి = అబద్ధం;

    hb_సదిశ_t<అక్షరం> ట్యాగ్_buf;
    format_ట్యాగ్ (ట్యాగ్_2, ట్యాగ్_buf);

    అక్షరం buf[128];
    పూర్ణాంకం len;
    ఒకవేళ (triple.minimum == triple.మధ్య && triple.మధ్య == triple.గరిష్ఠ)
    {
      len = snprintf (buf, పరిమాణం (buf), "%.*s=%g", (పూర్ణాంకం) ట్యాగ్_buf.నిడివి, ట్యాగ్_buf.arrayZ, (ద్విగుణ) triple.మధ్య);
    }
    లేకపోతే ఒకవేళ (std::isnan (triple.minimum) && std::isnan (triple.మధ్య) && std::isnan (triple.గరిష్ఠ))
    {
      len = snprintf (buf, పరిమాణం (buf), "%.*s=drop", (పూర్ణాంకం) ట్యాగ్_buf.నిడివి, ట్యాగ్_buf.arrayZ);
    }
    లేకపోతే
    {
      len = snprintf (buf, పరిమాణం (buf), "%.*s=%g:%g:%g", (పూర్ణాంకం) ట్యాగ్_buf.నిడివి, ట్యాగ్_buf.arrayZ,
                      (ద్విగుణ) triple.minimum, (ద్విగుణ) triple.మధ్య, (ద్విగుణ) triple.గరిష్ఠ);
    }
    ఒకవేళ (unlikely (len < 0 || (size_t) len >= పరిమాణం (buf))) తిరిగి అబద్ధం;
    specs.extend (hb_బైట్లు_t (buf, len));
  }
  ఒకవేళ (specs.నిడివి)
  {
    hb_సదిశ_t<అక్షరం> opt;
    opt.extend (hb_బైట్లు_t ("--variations=", 13));
    opt.extend (specs.as_array ());
    opt.push ('\0');
    ఒకవేళ (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  తిరిగి నిజం;
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
hb_subset_ఆదానం_గమ్యం_string_or_fail (hb_subset_ఆదానం_t *ఆదానం)
{
  ఒకవేళ (unlikely (!ఆదానం || ఆదానం->లోపల_error ()))
    తిరిగి శూన్యసూచిక;

  hb_సదిశ_t<అక్షరం> sb;

  నిర్మాణం flag_option_t {
    hb_subset_flags_t flag;
    అక్షరం option[32];
  };
  స్థిర స్థిరరాశి flag_option_t flag_options[] = {
    { HB_SUBSET_FLAGS_NO_HINTING, "no-hinting" },
    { HB_SUBSET_FLAGS_RETAIN_GIDS, "retain-gids" },
    { HB_SUBSET_FLAGS_DESUBROUTINIZE, "desubroutinize" },
    { HB_SUBSET_FLAGS_NAME_LEGACY, "name-legacy" },
    { HB_SUBSET_FLAGS_సమితి_OVERLAPS_FLAG, "set-overlaps-flag" },
    { HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED, "passthrough-tables" },
    { HB_SUBSET_FLAGS_NOTDEF_OUTLINE, "notdef-outline" },
    { HB_SUBSET_FLAGS_GLYPH_పేర్లు, "glyph-names" },
    { HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES, "no-prune-unicode-ranges" },
    { HB_SUBSET_FLAGS_NO_LAYOUT_సంవృతం, "no-layout-closure" },
    { HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS, "optimize" },
    { HB_SUBSET_FLAGS_NO_BIDI_సంవృతం, "no-bidi-closure" },
#నిర్వచితమైతే HB_EXPERIMENTAL_API
    { HB_SUBSET_FLAGS_IFTB_REQUIREMENTS, "iftb-requirements" },
    { HB_SUBSET_FLAGS_RETAIN_అంకె_GLYPHS, "retain-num-glyphs" },
#అయితే_ముగింపు
    { HB_SUBSET_FLAGS_DOWNGRADE_CFF2, "downgrade-cff2" },
  };

#నిర్వచితమైతే HB_EXPERIMENTAL_API
  static_assert (పరిమాణం (flag_options) / పరిమాణం (flag_options[0]) == 15,
                 "Check all flags in hb_subset_flags_t are handled here.");
#లేకపోతే
  static_assert (పరిమాణం (flag_options) / పరిమాణం (flag_options[0]) == 13,
                 "Check all flags in hb_subset_flags_t are handled here.");
#అయితే_ముగింపు

  కొరకు (స్థిరరాశి స్వయంచాలక &fo : flag_options)
  {
    ఒకవేళ (ఆదానం->flags & fo.flag)
    {
      అక్షరం buf[36];
      snprintf (buf, పరిమాణం (buf), "--%s", fo.option);
      _append_arg (sb, buf);
    }
  }

  hb_subset_ఆదానం_t *default_ఆదానం = hb_subset_ఆదానం_create_or_fail ();
  ఒకవేళ (unlikely (!default_ఆదానం))
    తిరిగి శూన్యసూచిక;

  // Serialize all sets if they are not equal to their default setting:
#నిర్వచించు PROCESS_సంఖ్య_సమితి(క్షేత్రం, opt_name, ...) \
  ఒకవేళ (!is_సమితి_default (ఆదానం->sets.క్షేత్రం, default_ఆదానం->sets.క్షేత్రం)) \
    format_సంఖ్య_సమితి_arg (ఆదానం->sets.క్షేత్రం, opt_name, sb, ##__VA_ARGS__)

#నిర్వచించు PROCESS_ట్యాగ్_సమితి(క్షేత్రం, opt_name) \
  ఒకవేళ (!is_సమితి_default (ఆదానం->sets.క్షేత్రం, default_ఆదానం->sets.క్షేత్రం)) \
    format_ట్యాగ్_సమితి_arg (ఆదానం->sets.క్షేత్రం, opt_name, sb)

  PROCESS_సంఖ్య_సమితి (unicodes, "unicodes", 16);
  PROCESS_సంఖ్య_సమితి (glyphs, "gids");
  PROCESS_సంఖ్య_సమితి (name_ids, "name-IDs");
  PROCESS_సంఖ్య_సమితి (name_languages, "name-languages");
  PROCESS_ట్యాగ్_సమితి (layout_features, "layout-features");
  PROCESS_ట్యాగ్_సమితి (layout_scripts, "layout-scripts");
  PROCESS_ట్యాగ్_సమితి (drop_tables, "drop-tables");
  PROCESS_ట్యాగ్_సమితి (no_subset_tables, "passthrough-tables");

#నిర్వచనం_రద్దు PROCESS_సంఖ్య_సమితి
#నిర్వచనం_రద్దు PROCESS_ట్యాగ్_సమితి

  hb_subset_ఆదానం_destroy (default_ఆదానం);

  static_assert (పరిమాణం (ఆదానం->sets) / పరిమాణం (hb_సమితి_t*) == 8,
                 "Check all sets in hb_subset_input_t::sets_t are handled here.");

  ఒకవేళ (unlikely (!_format_glyph_map_arg (ఆదానం->glyph_map, sb)))
    తిరిగి శూన్యసూచిక;

  ఒకవేళ (unlikely (!_format_axes_location_arg (ఆదానం->axes_location, sb)))
    తిరిగి శూన్యసూచిక;

  sb.push ('\0');
  ఒకవేళ (unlikely (sb.లోపల_error ()))
    తిరిగి శూన్యసూచిక;

  గుర్తులేని len = 0;
  అక్షరం *buf = sb.steal (&len);
  ఒకవేళ (unlikely (!buf))
    తిరిగి శూన్యసూచిక;

  తిరిగి hb_blob_create_or_fail (buf, len, HB_జ్ఞాపకం_బహుళకం_WRITABLE, buf, hb_free);
}
#అయితే_ముగింపు

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
hb_subset_preprocess (hb_face_t *ఆధారం)
{
  hb_subset_ఆదానం_t* ఆదానం = hb_subset_ఆదానం_create_or_fail ();
  ఒకవేళ (!ఆదానం)
    తిరిగి hb_face_reference (ఆధారం);

  hb_subset_ఆదానం_keep_everything (ఆదానం);

  ఆదానం->attach_accelerator_దత్తాంశం = నిజం;

  // Always use long loca in the preprocessed version. This allows
  // us to store the glyph bytes unpadded which allows the future subset
  // operation to run faster by skipping the trim padding step.
  ఆదానం->force_long_loca = నిజం;

  hb_face_t* new_ఆధారం = hb_subset_or_fail (ఆధారం, ఆదానం);
  hb_subset_ఆదానం_destroy (ఆదానం);

  ఒకవేళ (!new_ఆధారం) {
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "Preprocessing failed due to subset failure.");
    తిరిగి hb_face_reference (ఆధారం);
  }

  తిరిగి new_ఆధారం;
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
hb_subset_ఆదానం_old_గమ్యం_new_glyph_mapping (hb_subset_ఆదానం_t *ఆదానం)
{
  తిరిగి &ఆదానం->glyph_map;
}

#నిర్వచితమైతే HB_EXPERIMENTAL_API
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
hb_subset_ఆదానం_override_name_పట్టిక (hb_subset_ఆదానం_t  *ఆదానం,
                                     hb_ot_name_id_t     name_id,
                                     గుర్తులేని            platform_id,
                                     గుర్తులేని            encoding_id,
                                     గుర్తులేని            language_id,
                                     స్థిరరాశి అక్షరం         *name_str,
                                     పూర్ణాంకం                 str_len /* -1 means nul-terminated */)
{
  ఒకవేళ (!name_str)
  {
    str_len = 0;
  }
  లేకపోతే ఒకవేళ (str_len == -1)
  {
      str_len = అక్షరశ్రేణి_పొడవు (name_str);
  }

  hb_బైట్లు_t name_బైట్లు (శూన్యసూచిక, 0);
  ఒకవేళ (str_len)
  {
    ఒకవేళ (platform_id == 1)
    {
      స్థిరరాశి uint8_t *src = reinterpret_cast<స్థిరరాశి uint8_t*> (name_str);
      స్థిరరాశి uint8_t *src_చివర = src + str_len;

      hb_codepoint_t unicode;
      స్థిరరాశి hb_codepoint_t replacement = HB_బఫరు_REPLACEMENT_CODEPOINT_DEFAULT;
      అయితే (src < src_చివర)
      {
        src = hb_utf8_t::next (src, src_చివర, &unicode, replacement);
        ఒకవేళ (unicode >= 0x0080u)
        {
          // Non-ascii character detected, ignored...
          తిరిగి అబద్ధం;
        }
      }
    }
    అక్షరం *override_name = (అక్షరం *) hb_malloc (str_len);
    ఒకవేళ (unlikely (!override_name)) తిరిగి అబద్ధం;

    hb_memcpy (override_name, name_str, str_len);
    name_బైట్లు = hb_బైట్లు_t (override_name, str_len);
  }
  ఆదానం->name_పట్టిక_overrides.సమితి (hb_ot_name_రికార్డు_ids_t (platform_id, encoding_id, language_id, name_id), name_బైట్లు);
  తిరిగి నిజం;
}
#అయితే_ముగింపు
