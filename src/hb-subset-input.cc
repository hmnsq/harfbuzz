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


hb_subset_داخلا_t::hb_subset_داخلا_t ()
{
  لكل (خودڪار& سيٽ : sets_iter ())
    سيٽ = hb::shared_ptr<hb_سيٽ_t> (hb_سيٽ_create ());

  إذا (اندريون_error ())
    أرجع;

  flags = HB_SUBSET_FLAGS_DEFAULT;

  hb_سيٽ_جوڙيو_range (sets.name_ids, 0, 6);
  hb_سيٽ_جوڙيو (sets.name_languages, 0x0409);

  hb_ٽيگ_t default_drop_tables[] = {
    // Layout disabled by default
    HB_ٽيگ ('m', 'o', 'r', 'x'),
    HB_ٽيگ ('m', 'o', 'r', 't'),
    HB_ٽيگ ('k', 'e', 'r', 'x'),
    HB_ٽيگ ('k', 'e', 'r', 'n'),

    // Copied from fontTools:
    HB_ٽيگ ('J', 'S', 'T', 'F'),
    HB_ٽيگ ('D', 'S', 'I', 'G'),
    HB_ٽيگ ('E', 'B', 'D', 'T'),
    HB_ٽيگ ('E', 'B', 'L', 'C'),
    HB_ٽيگ ('E', 'B', 'S', 'C'),
    HB_ٽيگ ('S', 'V', 'G', ' '),
    HB_ٽيگ ('P', 'C', 'L', 'T'),
    HB_ٽيگ ('L', 'T', 'S', 'H'),
    // Graphite tables
    HB_ٽيگ ('F', 'e', 'a', 't'),
    HB_ٽيگ ('G', 'l', 'a', 't'),
    HB_ٽيگ ('G', 'l', 'o', 'c'),
    HB_ٽيگ ('S', 'i', 'l', 'f'),
    HB_ٽيگ ('S', 'i', 'l', 'l'),
  };
  sets.drop_tables->جوڙيو_array (default_drop_tables, ARRAY_ڊيگهائي (default_drop_tables));

  hb_ٽيگ_t default_no_subset_tables[] = {
    HB_ٽيگ ('g', 'a', 's', 'p'),
    HB_ٽيگ ('f', 'p', 'g', 'm'),
    HB_ٽيگ ('p', 'r', 'e', 'p'),
    HB_ٽيگ ('V', 'D', 'M', 'X'),
    HB_ٽيگ ('D', 'S', 'I', 'G'),
  };
  sets.no_subset_tables->جوڙيو_array (default_no_subset_tables,
					 ARRAY_ڊيگهائي (default_no_subset_tables));

  //copied from _layout_features_groups in fonttools
  hb_ٽيگ_t default_layout_features[] = {
    // default shaper
    // common
    HB_ٽيگ ('r', 'v', 'r', 'n'),
    HB_ٽيگ ('c', 'c', 'm', 'p'),
    HB_ٽيگ ('l', 'i', 'g', 'a'),
    HB_ٽيگ ('l', 'o', 'c', 'l'),
    HB_ٽيگ ('m', 'a', 'r', 'k'),
    HB_ٽيگ ('m', 'k', 'm', 'k'),
    HB_ٽيگ ('r', 'l', 'i', 'g'),

    //fractions
    HB_ٽيگ ('f', 'r', 'a', 'c'),
    HB_ٽيگ ('n', 'u', 'm', 'r'),
    HB_ٽيگ ('d', 'n', 'o', 'm'),

    //horizontal
    HB_ٽيگ ('c', 'a', 'l', 't'),
    HB_ٽيگ ('c', 'l', 'i', 'g'),
    HB_ٽيگ ('c', 'u', 'r', 's'),
    HB_ٽيگ ('k', 'e', 'r', 'n'),
    HB_ٽيگ ('r', 'c', 'l', 't'),

    //vertical
    HB_ٽيگ ('v', 'a', 'l', 't'),
    HB_ٽيگ ('v', 'e', 'r', 't'),
    HB_ٽيگ ('v', 'k', 'r', 'n'),
    HB_ٽيگ ('v', 'p', 'a', 'l'),
    HB_ٽيگ ('v', 'r', 't', '2'),

    //ltr
    HB_ٽيگ ('l', 't', 'r', 'a'),
    HB_ٽيگ ('l', 't', 'r', 'm'),

    //rtl
    HB_ٽيگ ('r', 't', 'l', 'a'),
    HB_ٽيگ ('r', 't', 'l', 'm'),

    //random
    HB_ٽيگ ('r', 'a', 'n', 'd'),

    //justify
    HB_ٽيگ ('j', 'a', 'l', 't'), // HarfBuzz doesn't use; others might

    //East Asian spacing
    HB_ٽيگ ('c', 'h', 'w', 's'),
    HB_ٽيگ ('v', 'c', 'h', 'w'),
    HB_ٽيگ ('h', 'a', 'l', 't'),
    HB_ٽيگ ('v', 'h', 'a', 'l'),
    HB_ٽيگ ('p', 'a', 'l', 't'),

    //ذاتي
    HB_ٽيگ ('H', 'a', 'r', 'f'),
    HB_ٽيگ ('H', 'A', 'R', 'F'),
    HB_ٽيگ ('B', 'u', 'z', 'z'),
    HB_ٽيگ ('B', 'U', 'Z', 'Z'),

    //shapers

    //arabic
    HB_ٽيگ ('i', 'n', 'i', 't'),
    HB_ٽيگ ('m', 'e', 'd', 'i'),
    HB_ٽيگ ('f', 'i', 'n', 'a'),
    HB_ٽيگ ('i', 's', 'o', 'l'),
    HB_ٽيگ ('m', 'e', 'd', '2'),
    HB_ٽيگ ('f', 'i', 'n', '2'),
    HB_ٽيگ ('f', 'i', 'n', '3'),
    HB_ٽيگ ('c', 's', 'w', 'h'),
    HB_ٽيگ ('m', 's', 'e', 't'),
    HB_ٽيگ ('s', 't', 'c', 'h'),

    //hangul
    HB_ٽيگ ('l', 'j', 'm', 'o'),
    HB_ٽيگ ('v', 'j', 'm', 'o'),
    HB_ٽيگ ('t', 'j', 'm', 'o'),

    //tibetan
    HB_ٽيگ ('a', 'b', 'v', 's'),
    HB_ٽيگ ('b', 'l', 'w', 's'),
    HB_ٽيگ ('a', 'b', 'v', 'm'),
    HB_ٽيگ ('b', 'l', 'w', 'm'),

    //indic
    HB_ٽيگ ('n', 'u', 'k', 't'),
    HB_ٽيگ ('a', 'k', 'h', 'n'),
    HB_ٽيگ ('r', 'p', 'h', 'f'),
    HB_ٽيگ ('r', 'k', 'r', 'f'),
    HB_ٽيگ ('p', 'r', 'e', 'f'),
    HB_ٽيگ ('b', 'l', 'w', 'f'),
    HB_ٽيگ ('h', 'a', 'l', 'f'),
    HB_ٽيگ ('a', 'b', 'v', 'f'),
    HB_ٽيگ ('p', 's', 't', 'f'),
    HB_ٽيگ ('c', 'f', 'a', 'r'),
    HB_ٽيگ ('v', 'a', 't', 'u'),
    HB_ٽيگ ('c', 'j', 'c', 't'),
    HB_ٽيگ ('i', 'n', 'i', 't'),
    HB_ٽيگ ('p', 'r', 'e', 's'),
    HB_ٽيگ ('a', 'b', 'v', 's'),
    HB_ٽيگ ('b', 'l', 'w', 's'),
    HB_ٽيگ ('p', 's', 't', 's'),
    HB_ٽيگ ('h', 'a', 'l', 'n'),
    HB_ٽيگ ('d', 'i', 's', 't'),
    HB_ٽيگ ('a', 'b', 'v', 'm'),
    HB_ٽيگ ('b', 'l', 'w', 'm'),
  };

  sets.layout_features->جوڙيو_array (default_layout_features, ARRAY_ڊيگهائي (default_layout_features));

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
hb_subset_داخلا_t *
hb_subset_داخلا_create_or_fail (فراغ)
{
  hb_subset_داخلا_t *داخلا_2 = hb_object_create<hb_subset_داخلا_t>();

  إذا (unlikely (!داخلا_2))
    أرجع خالي_اشارو;

  إذا (داخلا_2->اندريون_error ())
  {
    hb_subset_داخلا_destroy (داخلا_2);
    أرجع خالي_اشارو;
  }

  أرجع داخلا_2;
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
hb_subset_داخلا_t *
hb_subset_داخلا_reference (hb_subset_داخلا_t *داخلا_2)
{
  أرجع hb_object_reference (داخلا_2);
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
hb_subset_داخلا_destroy (hb_subset_داخلا_t *داخلا_2)
{
  إذا (!hb_object_destroy (داخلا_2)) أرجع;

  hb_free (داخلا_2);
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
HB_EXTERN hb_سيٽ_t *
hb_subset_داخلا_unicode_سيٽ (hb_subset_داخلا_t *داخلا_2)
{
  أرجع داخلا_2->sets.unicodes;
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
HB_EXTERN hb_سيٽ_t *
hb_subset_داخلا_glyph_سيٽ (hb_subset_داخلا_t *داخلا_2)
{
  أرجع داخلا_2->sets.glyphs;
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
HB_EXTERN hb_سيٽ_t *
hb_subset_داخلا_سيٽ (hb_subset_داخلا_t *داخلا_2, hb_subset_sets_t سيٽ_type)
{
  أرجع داخلا_2->sets_iter () [سيٽ_type];
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
hb_subset_داخلا_get_flags (hb_subset_داخلا_t *داخلا_2)
{
  أرجع (hb_subset_flags_t) داخلا_2->flags;
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
hb_subset_داخلا_سيٽ_flags (hb_subset_داخلا_t *داخلا_2,
			   بنانشان value)
{
  داخلا_2->flags = (hb_subset_flags_t) value;
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
hb_subset_داخلا_سيٽ_user_معلومات (hb_subset_داخلا_t  *داخلا_2,
			       hb_user_معلومات_ڪنجي_t *key,
			       فراغ *		   معلومات,
			       hb_destroy_func_t   destroy,
			       hb_bool_t	   replace)
{
  أرجع hb_object_سيٽ_user_معلومات (داخلا_2, key, معلومات, destroy, replace);
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
hb_subset_داخلا_get_user_معلومات (ثابت hb_subset_داخلا_t *داخلا_2,
			       hb_user_معلومات_ڪنجي_t     *key)
{
  أرجع hb_object_get_user_معلومات (داخلا_2, key);
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
hb_subset_داخلا_keep_everything (hb_subset_داخلا_t *داخلا_2)
{
  ثابت hb_subset_sets_t indices[] = {HB_SUBSET_SETS_UNICODE,
				      HB_SUBSET_SETS_GLYPH_اشاريو,
				      HB_SUBSET_SETS_NAME_ID,
				      HB_SUBSET_SETS_NAME_LANG_ID,
				      HB_SUBSET_SETS_LAYOUT_FEATURE_ٽيگ,
				      HB_SUBSET_SETS_LAYOUT_SCRIPT_ٽيگ};

  لكل (خودڪار idx : hb_iter (indices))
  {
    hb_سيٽ_t *سيٽ = hb_subset_داخلا_سيٽ (داخلا_2, idx);
    hb_سيٽ_clear (سيٽ);
    hb_سيٽ_invert (سيٽ);
  }

  // Don't drop any tables
  hb_سيٽ_clear (hb_subset_داخلا_سيٽ (داخلا_2, HB_SUBSET_SETS_DROP_جدول_ٽيگ));

  hb_subset_داخلا_سيٽ_flags (داخلا_2,
			     HB_SUBSET_FLAGS_NOTDEF_OUTLINE |
			     HB_SUBSET_FLAGS_GLYPH_نالا |
			     HB_SUBSET_FLAGS_NAME_LEGACY |
			     HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES |
                             HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED);
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
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
hb_subset_داخلا_pin_all_axes_منزل_default (hb_subset_داخلا_t  *داخلا_2,
                                         hb_face_t          *face)
{
  بنانشان axis_ڳڻپ = hb_ot_var_get_axis_ڳڻپ (face);
  إذا (!axis_ڳڻپ) أرجع خطأ;

  hb_ot_var_axis_info_t *axis_infos = (hb_ot_var_axis_info_t *) hb_calloc (axis_ڳڻپ, حجم (hb_ot_var_axis_info_t));
  إذا (unlikely (!axis_infos)) أرجع خطأ;

  (فراغ) hb_ot_var_get_axis_infos (face, 0, &axis_ڳڻپ, axis_infos);

  لكل (بنانشان i = 0; i < axis_ڳڻپ; i++)
  {
    hb_ٽيگ_t axis_ٽيگ_2 = axis_infos[i].ٽيگ_2;
    مزدوج default_val = (مزدوج) axis_infos[i].default_قدر;
    إذا (!داخلا_2->axes_location.سيٽ (axis_ٽيگ_2, Triple (default_val, default_val, default_val)))
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
hb_subset_داخلا_pin_axis_منزل_default (hb_subset_داخلا_t  *داخلا_2,
                                     hb_face_t          *face,
                                     hb_ٽيگ_t            axis_ٽيگ_2)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_ڳوليو_axis_info (face, axis_ٽيگ_2, &axis_info))
    أرجع خطأ;

  مزدوج default_val = (مزدوج) axis_info.default_قدر;
  أرجع داخلا_2->axes_location.سيٽ (axis_ٽيگ_2, Triple (default_val, default_val, default_val));
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
hb_subset_داخلا_pin_axis_location (hb_subset_داخلا_t  *داخلا_2,
                                   hb_face_t          *face,
                                   hb_ٽيگ_t            axis_ٽيگ_2,
                                   عائم               axis_قدر)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_ڳوليو_axis_info (face, axis_ٽيگ_2, &axis_info))
    أرجع خطأ;

  مزدوج val = hb_clamp((مزدوج) axis_قدر, (مزدوج) axis_info.min_قدر, (مزدوج) axis_info.max_قدر);
  أرجع داخلا_2->axes_location.سيٽ (axis_ٽيگ_2, Triple (val, val, val));
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
hb_subset_داخلا_سيٽ_axis_range (hb_subset_داخلا_t  *داخلا_2,
                                hb_face_t          *face,
                                hb_ٽيگ_t            axis_ٽيگ_2,
                                عائم               axis_min_قدر,
                                عائم               axis_max_قدر,
                                عائم               axis_def_قدر)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_ڳوليو_axis_info (face, axis_ٽيگ_2, &axis_info))
    أرجع خطأ;

  عائم min = !std::isnan(axis_min_قدر) ? axis_min_قدر : axis_info.min_قدر;
  عائم max = !std::isnan(axis_max_قدر) ? axis_max_قدر : axis_info.max_قدر;
  عائم def = !std::isnan(axis_def_قدر) ? axis_def_قدر : axis_info.default_قدر;

  إذا (min > max)
    أرجع خطأ;

  عائم new_min_val = hb_clamp(min, axis_info.min_قدر, axis_info.max_قدر);
  عائم new_max_val = hb_clamp(max, axis_info.min_قدر, axis_info.max_قدر);
  عائم new_default_val = hb_clamp(def, new_min_val, new_max_val);
  أرجع داخلا_2->axes_location.سيٽ (axis_ٽيگ_2, Triple ((مزدوج) new_min_val, (مزدوج) new_default_val, (مزدوج) new_max_val));
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
hb_subset_داخلا_get_axis_range (hb_subset_داخلا_t  *داخلا_2,
				hb_ٽيگ_t            axis_ٽيگ_2,
				عائم              *axis_min_قدر,
				عائم              *axis_max_قدر,
				عائم              *axis_def_قدر)

{
  Triple* triple;
  إذا (!داخلا_2->axes_location.has(axis_ٽيگ_2, &triple)) {
    أرجع خطأ;
  }

  *axis_min_قدر = triple->minimum;
  *axis_def_قدر = triple->وچ;
  *axis_max_قدر = triple->وڌ_کان_وڌ;
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
    len = لڙي_ڊيگهه (str);

  ثابت حرف *پڇاڙي = str + len;
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
    إذا (!hb_parse_double (&str, پڇاڙي, &v)) أرجع خطأ;

    *axis_min_قدر = v;
    *axis_def_قدر = v;
    *axis_max_قدر = v;
    أرجع سچ;
  }

  عائم values[3];
  صحيح ڳڻپ = 0;
  لكل (صحيح i = 0; i < 3; i++) {
    ڳڻپ++;
    إذا (!*str || part == str)
    {
      values[i] = NAN;

      إذا (part == عدم) اكسر;
      str = part + 1;
      part = strpbrk (str, ":");
      استمر;
    }

    مزدوج v;
    ثابت حرف *piece_پڇاڙي = part ? part : پڇاڙي;
    إذا (!hb_parse_double (&str, piece_پڇاڙي, &v)) أرجع خطأ;
    values[i] = v;

    إذا (part == عدم) اكسر;
    str = part + 1;
    part = strpbrk (str, ":");
  }

  إذا (ڳڻپ == 2)
  {
    *axis_min_قدر = values[0];
    *axis_def_قدر = NAN;
    *axis_max_قدر = values[1];
    أرجع سچ;
  }
  وإلا إذا (ڳڻپ == 3)
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
hb_subset_axis_range_منزل_string (hb_subset_داخلا_t *داخلا_2,
                                hb_ٽيگ_t axis_ٽيگ_2,
                                حرف *buf, بنانشان سائيز)
{
  إذا (unlikely (!سائيز)) أرجع;
  Triple* triple;
  إذا (!داخلا_2->axes_location.has(axis_ٽيگ_2, &triple)) {
    أرجع;
  }

  حرف s[128];
  بنانشان len = 0;

  hb_locale_t clocale HB_اڻ_استعمال_ٿيل;
  hb_locale_t oldlocale HB_اڻ_استعمال_ٿيل;
  oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", عدم));
  len += hb_max (0, snprintf (s, ARRAY_ڊيگهائي (s) - len, "%g", (مزدوج) triple->minimum));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_ڊيگهائي (s) - len, "%g", (مزدوج) triple->وچ));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_ڊيگهائي (s) - len, "%g", (مزدوج) triple->وڌ_کان_وڌ));
  (فراغ) hb_uselocale (((فراغ) freelocale (clocale), oldlocale));

  أكد (len < ARRAY_ڊيگهائي (s));
  len = hb_min (len, سائيز - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
ساكن فراغ
_append_arg (hb_ويڪٽر_t<حرف> &sb, ثابت حرف *arg)
{
  إذا (sb.اندريون_error ()) أرجع;
  إذا (sb.ڊيگهائي)
    sb.push (' ');
  بنانشان len = لڙي_ڊيگهه (arg);
  sb.extend (hb_بائيٽون_t (arg, len));
}
ساكن فراغ
format_عدد_سيٽ (ثابت hb_سيٽ_t *سيٽ, hb_ويڪٽر_t<حرف> &ٻاهريون, بنانشان بنياد = 10)
{
  أكد (بنياد == 10 || بنياد == 16);
  إذا (unlikely (بنياد != 10 && بنياد != 16)) أرجع;

  hb_codepoint_t پهريون = HB_سيٽ_قدر_INVALID, آخري = HB_سيٽ_قدر_INVALID;
  منطقي پهريون_range = سچ;
  طالما (hb_سيٽ_ايندڙ_range (سيٽ, &پهريون, &آخري))
  {
    إذا (!پهريون_range)
      ٻاهريون.push (',');
    پهريون_range = خطأ;

    حرف buf[64];
    صحيح len;
    إذا (پهريون == آخري)
    {
      إذا (بنياد == 16)
        len = snprintf (buf, حجم (buf), "%X", پهريون);
      وإلا
        len = snprintf (buf, حجم (buf), "%u", پهريون);
    }
    وإلا
    {
      إذا (بنياد == 16)
        len = snprintf (buf, حجم (buf), "%X-%X", پهريون, آخري);
      وإلا
        len = snprintf (buf, حجم (buf), "%u-%u", پهريون, آخري);
    }
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع;
    ٻاهريون.extend (hb_بائيٽون_t (buf, len));
  }
}

ساكن فراغ
format_ٽيگ (hb_ٽيگ_t ٽيگ_2, hb_ويڪٽر_t<حرف> &ٻاهريون)
{
  حرف buf[4];
  hb_ٽيگ_منزل_string (ٽيگ_2, buf);

  بنانشان len = 4;
  طالما (len > 0 && buf[len - 1] == ' ')
    len--;

  لكل (بنانشان i = 0; i < len; i++)
  {
    حرف c = buf[i];
    إذا (!((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '-' || c == '_'))
    {
      c = '_';
    }
    ٻاهريون.push (c);
  }
}

ساكن فراغ
format_ٽيگ_سيٽ (ثابت hb_سيٽ_t *سيٽ, hb_ويڪٽر_t<حرف> &ٻاهريون)
{
  منطقي پهريون_ٽيگ = سچ;
  لكل (hb_codepoint_t ٽيگ_2 : *سيٽ)
  {
    إذا (!پهريون_ٽيگ)
      ٻاهريون.push (',');
    پهريون_ٽيگ = خطأ;

    format_ٽيگ (ٽيگ_2, ٻاهريون);
  }
}

ساكن منطقي
is_سيٽ_default (ثابت hb_سيٽ_t *سيٽ, ثابت hb_سيٽ_t *default_سيٽ)
{
  أرجع hb_سيٽ_is_inverted (سيٽ) == hb_سيٽ_is_inverted (default_سيٽ) &&
         hb_سيٽ_is_equal (سيٽ, default_سيٽ);
}

سانچو <قسم_نالو Formatter>
ساكن فراغ
format_سيٽ_arg (ثابت hb_سيٽ_t *سيٽ,
                 ثابت حرف *opt_name,
                 Formatter &&formatter,
                 hb_ويڪٽر_t<حرف> &sb)
{
  إذا (hb_سيٽ_is_inverted (سيٽ))
  {
    hb_سيٽ_t *excluded = hb_سيٽ_copy (سيٽ);
    hb_سيٽ_invert (excluded);

    hb_ويڪٽر_t<حرف> opt;
    opt.extend (hb_بائيٽون_t ("--", 2));
    opt.extend (hb_بائيٽون_t (opt_name, لڙي_ڊيگهه (opt_name)));
    opt.extend (hb_بائيٽون_t ("=*", 2));
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);

    إذا (!hb_سيٽ_is_خالي_جايون (excluded))
    {
      hb_ويڪٽر_t<حرف> formatted;
      formatter (excluded, formatted);
      إذا (formatted.ڊيگهائي)
      {
        hb_ويڪٽر_t<حرف> opt2;
        opt2.extend (hb_بائيٽون_t ("--", 2));
        opt2.extend (hb_بائيٽون_t (opt_name, لڙي_ڊيگهه (opt_name)));
        opt2.extend (hb_بائيٽون_t ("-=", 2));
        opt2.extend (formatted.as_array ());
        opt2.push ('\0');
        إذا (opt2.arrayZ) _append_arg (sb, opt2.arrayZ);
      }
    }
    hb_سيٽ_destroy (excluded);
  }
  وإلا إذا (hb_سيٽ_is_خالي_جايون (سيٽ))
  {
    hb_ويڪٽر_t<حرف> opt;
    opt.extend (hb_بائيٽون_t ("--", 2));
    opt.extend (hb_بائيٽون_t (opt_name, لڙي_ڊيگهه (opt_name)));
    opt.extend (hb_بائيٽون_t ("-=*", 3));
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  وإلا
  {
    hb_ويڪٽر_t<حرف> formatted;
    formatter (سيٽ, formatted);
    إذا (formatted.ڊيگهائي)
    {
      hb_ويڪٽر_t<حرف> opt;
      opt.extend (hb_بائيٽون_t ("--", 2));
      opt.extend (hb_بائيٽون_t (opt_name, لڙي_ڊيگهه (opt_name)));
      opt.extend (hb_بائيٽون_t ("=", 1));
      opt.extend (formatted.as_array ());
      opt.push ('\0');
      إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
    }
  }
}

ساكن فراغ
format_عدد_سيٽ_arg (ثابت hb_سيٽ_t *سيٽ,
                        ثابت حرف *opt_name,
                        hb_ويڪٽر_t<حرف> &sb,
                        بنانشان بنياد = 10)
{
  format_سيٽ_arg (سيٽ, opt_name, [بنياد] (ثابت hb_سيٽ_t *s, hb_ويڪٽر_t<حرف> &ٻاهريون) {
    format_عدد_سيٽ (s, ٻاهريون, بنياد);
  }, sb);
}

ساكن فراغ
format_ٽيگ_سيٽ_arg (ثابت hb_سيٽ_t *سيٽ,
                     ثابت حرف *opt_name,
                     hb_ويڪٽر_t<حرف> &sb)
{
  format_سيٽ_arg (سيٽ, opt_name, format_ٽيگ_سيٽ, sb);
}

ساكن منطقي
_format_glyph_map_arg (ثابت hb_map_t &glyph_map,
                       hb_ويڪٽر_t<حرف> &sb)
{
  إذا (glyph_map.is_empty ())
    أرجع سچ;

  hb_سيٽ_t ڪنجيون;
  hb_map_ڪنجيون (&glyph_map, &ڪنجيون);
  hb_ويڪٽر_t<حرف> جوڙا;
  منطقي پهريون = سچ;
  لكل (hb_codepoint_t key : ڪنجيون)
  {
    إذا (!پهريون)
      جوڙا.push (',');
    پهريون = خطأ;
    hb_codepoint_t val = hb_map_get (&glyph_map, key);
    حرف buf[64];
    صحيح len = snprintf (buf, حجم (buf), "%u:%u", key, val);
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع خطأ;
    جوڙا.extend (hb_بائيٽون_t (buf, len));
  }
  إذا (جوڙا.ڊيگهائي)
  {
    hb_ويڪٽر_t<حرف> opt;
    opt.extend (hb_بائيٽون_t ("--gid-map=", 10));
    opt.extend (جوڙا.as_array ());
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  أرجع سچ;
}

ساكن منطقي
_format_axes_location_arg (ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> &axes_location,
                           hb_ويڪٽر_t<حرف> &sb)
{
  إذا (axes_location.is_empty ())
    أرجع سچ;

  hb_ويڪٽر_t<hb_ٽيگ_t> axis_ٽيگ_فهرست;
  لكل (خودڪار ٽيگ_2 : axes_location.ڪنجيون ())
    axis_ٽيگ_فهرست.push (ٽيگ_2);
  axis_ٽيگ_فهرست.qsort ([] (ثابت hb_ٽيگ_t &a, ثابت hb_ٽيگ_t &b) { أرجع a < b ? -1 : a > b ? 1 : 0; });

  hb_ويڪٽر_t<حرف> specs;
  منطقي پهريون = سچ;
  لكل (hb_ٽيگ_t ٽيگ_2 : axis_ٽيگ_فهرست)
  {
    Triple triple = axes_location.get (ٽيگ_2);
    إذا (!پهريون)
      specs.push (',');
    پهريون = خطأ;

    hb_ويڪٽر_t<حرف> ٽيگ_buf;
    format_ٽيگ (ٽيگ_2, ٽيگ_buf);

    حرف buf[128];
    صحيح len;
    إذا (triple.minimum == triple.وچ && triple.وچ == triple.وڌ_کان_وڌ)
    {
      len = snprintf (buf, حجم (buf), "%.*s=%g", (صحيح) ٽيگ_buf.ڊيگهائي, ٽيگ_buf.arrayZ, (مزدوج) triple.وچ);
    }
    وإلا إذا (std::isnan (triple.minimum) && std::isnan (triple.وچ) && std::isnan (triple.وڌ_کان_وڌ))
    {
      len = snprintf (buf, حجم (buf), "%.*s=drop", (صحيح) ٽيگ_buf.ڊيگهائي, ٽيگ_buf.arrayZ);
    }
    وإلا
    {
      len = snprintf (buf, حجم (buf), "%.*s=%g:%g:%g", (صحيح) ٽيگ_buf.ڊيگهائي, ٽيگ_buf.arrayZ,
                      (مزدوج) triple.minimum, (مزدوج) triple.وچ, (مزدوج) triple.وڌ_کان_وڌ);
    }
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع خطأ;
    specs.extend (hb_بائيٽون_t (buf, len));
  }
  إذا (specs.ڊيگهائي)
  {
    hb_ويڪٽر_t<حرف> opt;
    opt.extend (hb_بائيٽون_t ("--variations=", 13));
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
hb_subset_داخلا_منزل_string_or_fail (hb_subset_داخلا_t *داخلا_2)
{
  إذا (unlikely (!داخلا_2 || داخلا_2->اندريون_error ()))
    أرجع خالي_اشارو;

  hb_ويڪٽر_t<حرف> sb;

  بنية flag_option_t {
    hb_subset_flags_t flag;
    حرف option[32];
  };
  ساكن ثابت flag_option_t flag_options[] = {
    { HB_SUBSET_FLAGS_NO_HINTING, "no-hinting" },
    { HB_SUBSET_FLAGS_RETAIN_GIDS, "retain-gids" },
    { HB_SUBSET_FLAGS_DESUBROUTINIZE, "desubroutinize" },
    { HB_SUBSET_FLAGS_NAME_LEGACY, "name-legacy" },
    { HB_SUBSET_FLAGS_سيٽ_OVERLAPS_FLAG, "set-overlaps-flag" },
    { HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED, "passthrough-tables" },
    { HB_SUBSET_FLAGS_NOTDEF_OUTLINE, "notdef-outline" },
    { HB_SUBSET_FLAGS_GLYPH_نالا, "glyph-names" },
    { HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES, "no-prune-unicode-ranges" },
    { HB_SUBSET_FLAGS_NO_LAYOUT_گهيرو, "no-layout-closure" },
    { HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS, "optimize" },
    { HB_SUBSET_FLAGS_NO_BIDI_گهيرو, "no-bidi-closure" },
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
    { HB_SUBSET_FLAGS_IFTB_REQUIREMENTS, "iftb-requirements" },
    { HB_SUBSET_FLAGS_RETAIN_انگ_GLYPHS, "retain-num-glyphs" },
#جيڪڏهن_پورو
    { HB_SUBSET_FLAGS_DOWNGRADE_CFF2, "downgrade-cff2" },
  };

#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
  static_assert (حجم (flag_options) / حجم (flag_options[0]) == 15,
                 "Check all flags in hb_subset_flags_t are handled here.");
#وإلا
  static_assert (حجم (flag_options) / حجم (flag_options[0]) == 13,
                 "Check all flags in hb_subset_flags_t are handled here.");
#جيڪڏهن_پورو

  لكل (ثابت خودڪار &fo : flag_options)
  {
    إذا (داخلا_2->flags & fo.flag)
    {
      حرف buf[36];
      snprintf (buf, حجم (buf), "--%s", fo.option);
      _append_arg (sb, buf);
    }
  }

  hb_subset_داخلا_t *default_داخلا = hb_subset_داخلا_create_or_fail ();
  إذا (unlikely (!default_داخلا))
    أرجع خالي_اشارو;

  // Serialize all sets if they are not equal to their default setting:
#وصف_ڪريو PROCESS_عدد_سيٽ(ميدان, opt_name, ...) \
  إذا (!is_سيٽ_default (داخلا_2->sets.ميدان, default_داخلا->sets.ميدان)) \
    format_عدد_سيٽ_arg (داخلا_2->sets.ميدان, opt_name, sb, ##__VA_ARGS__)

#وصف_ڪريو PROCESS_ٽيگ_سيٽ(ميدان, opt_name) \
  إذا (!is_سيٽ_default (داخلا_2->sets.ميدان, default_داخلا->sets.ميدان)) \
    format_ٽيگ_سيٽ_arg (داخلا_2->sets.ميدان, opt_name, sb)

  PROCESS_عدد_سيٽ (unicodes, "unicodes", 16);
  PROCESS_عدد_سيٽ (glyphs, "gids");
  PROCESS_عدد_سيٽ (name_ids, "name-IDs");
  PROCESS_عدد_سيٽ (name_languages, "name-languages");
  PROCESS_ٽيگ_سيٽ (layout_features, "layout-features");
  PROCESS_ٽيگ_سيٽ (layout_scripts, "layout-scripts");
  PROCESS_ٽيگ_سيٽ (drop_tables, "drop-tables");
  PROCESS_ٽيگ_سيٽ (no_subset_tables, "passthrough-tables");

#وصف_رد PROCESS_عدد_سيٽ
#وصف_رد PROCESS_ٽيگ_سيٽ

  hb_subset_داخلا_destroy (default_داخلا);

  static_assert (حجم (داخلا_2->sets) / حجم (hb_سيٽ_t*) == 8,
                 "Check all sets in hb_subset_input_t::sets_t are handled here.");

  إذا (unlikely (!_format_glyph_map_arg (داخلا_2->glyph_map, sb)))
    أرجع خالي_اشارو;

  إذا (unlikely (!_format_axes_location_arg (داخلا_2->axes_location, sb)))
    أرجع خالي_اشارو;

  sb.push ('\0');
  إذا (unlikely (sb.اندريون_error ()))
    أرجع خالي_اشارو;

  بنانشان len = 0;
  حرف *buf = sb.steal (&len);
  إذا (unlikely (!buf))
    أرجع خالي_اشارو;

  أرجع hb_blob_create_or_fail (buf, len, HB_يادگيري_گهڻائي_WRITABLE, buf, hb_free);
}
#جيڪڏهن_پورو

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
hb_subset_preprocess (hb_face_t *ذريعو)
{
  hb_subset_داخلا_t* داخلا_2 = hb_subset_داخلا_create_or_fail ();
  إذا (!داخلا_2)
    أرجع hb_face_reference (ذريعو);

  hb_subset_داخلا_keep_everything (داخلا_2);

  داخلا_2->attach_accelerator_معلومات = سچ;

  // Always use long loca in the preprocessed version. This allows
  // us to store the glyph bytes unpadded which allows the future subset
  // operation to run faster by skipping the trim padding step.
  داخلا_2->force_long_loca = سچ;

  hb_face_t* new_ذريعو = hb_subset_or_fail (ذريعو, داخلا_2);
  hb_subset_داخلا_destroy (داخلا_2);

  إذا (!new_ذريعو) {
    DEBUG_نياپو (SUBSET, خالي_اشارو, "Preprocessing failed due to subset failure.");
    أرجع hb_face_reference (ذريعو);
  }

  أرجع new_ذريعو;
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
hb_subset_داخلا_old_منزل_new_glyph_mapping (hb_subset_داخلا_t *داخلا_2)
{
  أرجع &داخلا_2->glyph_map;
}

#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
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
hb_subset_داخلا_override_name_جدول (hb_subset_داخلا_t  *داخلا_2,
                                     hb_ot_name_id_t     name_id,
                                     بنانشان            platform_id,
                                     بنانشان            encoding_id,
                                     بنانشان            language_id,
                                     ثابت حرف         *name_str,
                                     صحيح                 str_len /* -1 means nul-terminated */)
{
  إذا (!name_str)
  {
    str_len = 0;
  }
  وإلا إذا (str_len == -1)
  {
      str_len = لڙي_ڊيگهه (name_str);
  }

  hb_بائيٽون_t name_بائيٽون (خالي_اشارو, 0);
  إذا (str_len)
  {
    إذا (platform_id == 1)
    {
      ثابت uint8_t *src = reinterpret_cast<ثابت uint8_t*> (name_str);
      ثابت uint8_t *src_پڇاڙي = src + str_len;

      hb_codepoint_t unicode;
      ثابت hb_codepoint_t replacement = HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT;
      طالما (src < src_پڇاڙي)
      {
        src = hb_utf8_t::next (src, src_پڇاڙي, &unicode, replacement);
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
    name_بائيٽون = hb_بائيٽون_t (override_name, str_len);
  }
  داخلا_2->name_جدول_overrides.سيٽ (hb_ot_name_رڪارڊ_ids_t (platform_id, encoding_id, language_id, name_id), name_بائيٽون);
  أرجع سچ;
}
#جيڪڏهن_پورو
