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


hb_subset_درون_داد_t::hb_subset_درون_داد_t ()
{
  لكل (خودکار& مجموعه : sets_iter ())
    مجموعه = hb::shared_ptr<hb_مجموعه_t> (hb_مجموعه_create ());

  إذا (درون_error ())
    أرجع;

  flags = HB_SUBSET_FLAGS_DEFAULT;

  hb_مجموعه_جمع_کن_range (sets.name_ids, 0, 6);
  hb_مجموعه_جمع_کن (sets.name_languages, 0x0409);

  hb_برچسب_t default_drop_tables[] = {
    // Layout disabled by default
    HB_برچسب ('m', 'o', 'r', 'x'),
    HB_برچسب ('m', 'o', 'r', 't'),
    HB_برچسب ('k', 'e', 'r', 'x'),
    HB_برچسب ('k', 'e', 'r', 'n'),

    // Copied from fontTools:
    HB_برچسب ('J', 'S', 'T', 'F'),
    HB_برچسب ('D', 'S', 'I', 'G'),
    HB_برچسب ('E', 'B', 'D', 'T'),
    HB_برچسب ('E', 'B', 'L', 'C'),
    HB_برچسب ('E', 'B', 'S', 'C'),
    HB_برچسب ('S', 'V', 'G', ' '),
    HB_برچسب ('P', 'C', 'L', 'T'),
    HB_برچسب ('L', 'T', 'S', 'H'),
    // Graphite tables
    HB_برچسب ('F', 'e', 'a', 't'),
    HB_برچسب ('G', 'l', 'a', 't'),
    HB_برچسب ('G', 'l', 'o', 'c'),
    HB_برچسب ('S', 'i', 'l', 'f'),
    HB_برچسب ('S', 'i', 'l', 'l'),
  };
  sets.drop_tables->جمع_کن_array (default_drop_tables, ARRAY_درازی (default_drop_tables));

  hb_برچسب_t default_no_subset_tables[] = {
    HB_برچسب ('g', 'a', 's', 'p'),
    HB_برچسب ('f', 'p', 'g', 'm'),
    HB_برچسب ('p', 'r', 'e', 'p'),
    HB_برچسب ('V', 'D', 'M', 'X'),
    HB_برچسب ('D', 'S', 'I', 'G'),
  };
  sets.no_subset_tables->جمع_کن_array (default_no_subset_tables,
					 ARRAY_درازی (default_no_subset_tables));

  //copied from _layout_features_groups in fonttools
  hb_برچسب_t default_layout_features[] = {
    // default shaper
    // common
    HB_برچسب ('r', 'v', 'r', 'n'),
    HB_برچسب ('c', 'c', 'm', 'p'),
    HB_برچسب ('l', 'i', 'g', 'a'),
    HB_برچسب ('l', 'o', 'c', 'l'),
    HB_برچسب ('m', 'a', 'r', 'k'),
    HB_برچسب ('m', 'k', 'm', 'k'),
    HB_برچسب ('r', 'l', 'i', 'g'),

    //fractions
    HB_برچسب ('f', 'r', 'a', 'c'),
    HB_برچسب ('n', 'u', 'm', 'r'),
    HB_برچسب ('d', 'n', 'o', 'm'),

    //horizontal
    HB_برچسب ('c', 'a', 'l', 't'),
    HB_برچسب ('c', 'l', 'i', 'g'),
    HB_برچسب ('c', 'u', 'r', 's'),
    HB_برچسب ('k', 'e', 'r', 'n'),
    HB_برچسب ('r', 'c', 'l', 't'),

    //vertical
    HB_برچسب ('v', 'a', 'l', 't'),
    HB_برچسب ('v', 'e', 'r', 't'),
    HB_برچسب ('v', 'k', 'r', 'n'),
    HB_برچسب ('v', 'p', 'a', 'l'),
    HB_برچسب ('v', 'r', 't', '2'),

    //ltr
    HB_برچسب ('l', 't', 'r', 'a'),
    HB_برچسب ('l', 't', 'r', 'm'),

    //rtl
    HB_برچسب ('r', 't', 'l', 'a'),
    HB_برچسب ('r', 't', 'l', 'm'),

    //random
    HB_برچسب ('r', 'a', 'n', 'd'),

    //justify
    HB_برچسب ('j', 'a', 'l', 't'), // HarfBuzz doesn't use; others might

    //East Asian spacing
    HB_برچسب ('c', 'h', 'w', 's'),
    HB_برچسب ('v', 'c', 'h', 'w'),
    HB_برچسب ('h', 'a', 'l', 't'),
    HB_برچسب ('v', 'h', 'a', 'l'),
    HB_برچسب ('p', 'a', 'l', 't'),

    //خصوصی
    HB_برچسب ('H', 'a', 'r', 'f'),
    HB_برچسب ('H', 'A', 'R', 'F'),
    HB_برچسب ('B', 'u', 'z', 'z'),
    HB_برچسب ('B', 'U', 'Z', 'Z'),

    //shapers

    //arabic
    HB_برچسب ('i', 'n', 'i', 't'),
    HB_برچسب ('m', 'e', 'd', 'i'),
    HB_برچسب ('f', 'i', 'n', 'a'),
    HB_برچسب ('i', 's', 'o', 'l'),
    HB_برچسب ('m', 'e', 'd', '2'),
    HB_برچسب ('f', 'i', 'n', '2'),
    HB_برچسب ('f', 'i', 'n', '3'),
    HB_برچسب ('c', 's', 'w', 'h'),
    HB_برچسب ('m', 's', 'e', 't'),
    HB_برچسب ('s', 't', 'c', 'h'),

    //hangul
    HB_برچسب ('l', 'j', 'm', 'o'),
    HB_برچسب ('v', 'j', 'm', 'o'),
    HB_برچسب ('t', 'j', 'm', 'o'),

    //tibetan
    HB_برچسب ('a', 'b', 'v', 's'),
    HB_برچسب ('b', 'l', 'w', 's'),
    HB_برچسب ('a', 'b', 'v', 'm'),
    HB_برچسب ('b', 'l', 'w', 'm'),

    //indic
    HB_برچسب ('n', 'u', 'k', 't'),
    HB_برچسب ('a', 'k', 'h', 'n'),
    HB_برچسب ('r', 'p', 'h', 'f'),
    HB_برچسب ('r', 'k', 'r', 'f'),
    HB_برچسب ('p', 'r', 'e', 'f'),
    HB_برچسب ('b', 'l', 'w', 'f'),
    HB_برچسب ('h', 'a', 'l', 'f'),
    HB_برچسب ('a', 'b', 'v', 'f'),
    HB_برچسب ('p', 's', 't', 'f'),
    HB_برچسب ('c', 'f', 'a', 'r'),
    HB_برچسب ('v', 'a', 't', 'u'),
    HB_برچسب ('c', 'j', 'c', 't'),
    HB_برچسب ('i', 'n', 'i', 't'),
    HB_برچسب ('p', 'r', 'e', 's'),
    HB_برچسب ('a', 'b', 'v', 's'),
    HB_برچسب ('b', 'l', 'w', 's'),
    HB_برچسب ('p', 's', 't', 's'),
    HB_برچسب ('h', 'a', 'l', 'n'),
    HB_برچسب ('d', 'i', 's', 't'),
    HB_برچسب ('a', 'b', 'v', 'm'),
    HB_برچسب ('b', 'l', 'w', 'm'),
  };

  sets.layout_features->جمع_کن_array (default_layout_features, ARRAY_درازی (default_layout_features));

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
hb_subset_درون_داد_t *
hb_subset_درون_داد_create_or_fail (فراغ)
{
  hb_subset_درون_داد_t *درون_داد = hb_object_create<hb_subset_درون_داد_t>();

  إذا (unlikely (!درون_داد))
    أرجع اشاره_تهی;

  إذا (درون_داد->درون_error ())
  {
    hb_subset_درون_داد_destroy (درون_داد);
    أرجع اشاره_تهی;
  }

  أرجع درون_داد;
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
hb_subset_درون_داد_t *
hb_subset_درون_داد_reference (hb_subset_درون_داد_t *درون_داد)
{
  أرجع hb_object_reference (درون_داد);
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
hb_subset_درون_داد_destroy (hb_subset_درون_داد_t *درون_داد)
{
  إذا (!hb_object_destroy (درون_داد)) أرجع;

  hb_free (درون_داد);
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
HB_EXTERN hb_مجموعه_t *
hb_subset_درون_داد_unicode_مجموعه (hb_subset_درون_داد_t *درون_داد)
{
  أرجع درون_داد->sets.unicodes;
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
HB_EXTERN hb_مجموعه_t *
hb_subset_درون_داد_glyph_مجموعه (hb_subset_درون_داد_t *درون_داد)
{
  أرجع درون_داد->sets.glyphs;
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
HB_EXTERN hb_مجموعه_t *
hb_subset_درون_داد_مجموعه (hb_subset_درون_داد_t *درون_داد, hb_subset_sets_t مجموعه_type)
{
  أرجع درون_داد->sets_iter () [مجموعه_type];
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
hb_subset_درون_داد_get_flags (hb_subset_درون_داد_t *درون_داد)
{
  أرجع (hb_subset_flags_t) درون_داد->flags;
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
hb_subset_درون_داد_مجموعه_flags (hb_subset_درون_داد_t *درون_داد,
			   بیعلامت value)
{
  درون_داد->flags = (hb_subset_flags_t) value;
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
hb_subset_درون_داد_مجموعه_user_داده (hb_subset_درون_داد_t  *درون_داد,
			       hb_user_داده_کلید_t *key,
			       فراغ *		   داده,
			       hb_destroy_func_t   destroy,
			       hb_bool_t	   replace)
{
  أرجع hb_object_مجموعه_user_داده (درون_داد, key, داده, destroy, replace);
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
hb_subset_درون_داد_get_user_داده (ثابت hb_subset_درون_داد_t *درون_داد,
			       hb_user_داده_کلید_t     *key)
{
  أرجع hb_object_get_user_داده (درون_داد, key);
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
hb_subset_درون_داد_keep_everything (hb_subset_درون_داد_t *درون_داد)
{
  ثابت hb_subset_sets_t indices[] = {HB_SUBSET_SETS_UNICODE,
				      HB_SUBSET_SETS_GLYPH_شاخص,
				      HB_SUBSET_SETS_NAME_ID,
				      HB_SUBSET_SETS_NAME_LANG_ID,
				      HB_SUBSET_SETS_LAYOUT_FEATURE_برچسب,
				      HB_SUBSET_SETS_LAYOUT_SCRIPT_برچسب};

  لكل (خودکار idx : hb_iter (indices))
  {
    hb_مجموعه_t *مجموعه = hb_subset_درون_داد_مجموعه (درون_داد, idx);
    hb_مجموعه_clear (مجموعه);
    hb_مجموعه_invert (مجموعه);
  }

  // Don't drop any tables
  hb_مجموعه_clear (hb_subset_درون_داد_مجموعه (درون_داد, HB_SUBSET_SETS_DROP_جدول_برچسب));

  hb_subset_درون_داد_مجموعه_flags (درون_داد,
			     HB_SUBSET_FLAGS_NOTDEF_OUTLINE |
			     HB_SUBSET_FLAGS_GLYPH_نامها |
			     HB_SUBSET_FLAGS_NAME_LEGACY |
			     HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES |
                             HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED);
}

#اگر_تعریف_نشده HB_NO_VAR
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
hb_subset_درون_داد_pin_all_axes_مقصد_default (hb_subset_درون_داد_t  *درون_داد,
                                         hb_face_t          *face)
{
  بیعلامت axis_شمار = hb_ot_var_get_axis_شمار (face);
  إذا (!axis_شمار) أرجع خطأ;

  hb_ot_var_axis_info_t *axis_infos = (hb_ot_var_axis_info_t *) hb_calloc (axis_شمار, حجم (hb_ot_var_axis_info_t));
  إذا (unlikely (!axis_infos)) أرجع خطأ;

  (فراغ) hb_ot_var_get_axis_infos (face, 0, &axis_شمار, axis_infos);

  لكل (بیعلامت i = 0; i < axis_شمار; i++)
  {
    hb_برچسب_t axis_برچسب_2 = axis_infos[i].برچسب_2;
    مزدوج default_val = (مزدوج) axis_infos[i].default_مقدار;
    إذا (!درون_داد->axes_location.مجموعه (axis_برچسب_2, Triple (default_val, default_val, default_val)))
    {
      hb_free (axis_infos);
      أرجع خطأ;
    }
  }
  hb_free (axis_infos);
  أرجع درست;
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
hb_subset_درون_داد_pin_axis_مقصد_default (hb_subset_درون_داد_t  *درون_داد,
                                     hb_face_t          *face,
                                     hb_برچسب_t            axis_برچسب_2)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_پیدا_کن_axis_info (face, axis_برچسب_2, &axis_info))
    أرجع خطأ;

  مزدوج default_val = (مزدوج) axis_info.default_مقدار;
  أرجع درون_داد->axes_location.مجموعه (axis_برچسب_2, Triple (default_val, default_val, default_val));
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
hb_subset_درون_داد_pin_axis_location (hb_subset_درون_داد_t  *درون_داد,
                                   hb_face_t          *face,
                                   hb_برچسب_t            axis_برچسب_2,
                                   عائم               axis_مقدار)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_پیدا_کن_axis_info (face, axis_برچسب_2, &axis_info))
    أرجع خطأ;

  مزدوج val = hb_clamp((مزدوج) axis_مقدار, (مزدوج) axis_info.min_مقدار, (مزدوج) axis_info.max_مقدار);
  أرجع درون_داد->axes_location.مجموعه (axis_برچسب_2, Triple (val, val, val));
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
hb_subset_درون_داد_مجموعه_axis_range (hb_subset_درون_داد_t  *درون_داد,
                                hb_face_t          *face,
                                hb_برچسب_t            axis_برچسب_2,
                                عائم               axis_min_مقدار,
                                عائم               axis_max_مقدار,
                                عائم               axis_def_مقدار)
{
  hb_ot_var_axis_info_t axis_info;
  إذا (!hb_ot_var_پیدا_کن_axis_info (face, axis_برچسب_2, &axis_info))
    أرجع خطأ;

  عائم min = !std::isnan(axis_min_مقدار) ? axis_min_مقدار : axis_info.min_مقدار;
  عائم max = !std::isnan(axis_max_مقدار) ? axis_max_مقدار : axis_info.max_مقدار;
  عائم def = !std::isnan(axis_def_مقدار) ? axis_def_مقدار : axis_info.default_مقدار;

  إذا (min > max)
    أرجع خطأ;

  عائم new_min_val = hb_clamp(min, axis_info.min_مقدار, axis_info.max_مقدار);
  عائم new_max_val = hb_clamp(max, axis_info.min_مقدار, axis_info.max_مقدار);
  عائم new_default_val = hb_clamp(def, new_min_val, new_max_val);
  أرجع درون_داد->axes_location.مجموعه (axis_برچسب_2, Triple ((مزدوج) new_min_val, (مزدوج) new_default_val, (مزدوج) new_max_val));
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
hb_subset_درون_داد_get_axis_range (hb_subset_درون_داد_t  *درون_داد,
				hb_برچسب_t            axis_برچسب_2,
				عائم              *axis_min_مقدار,
				عائم              *axis_max_مقدار,
				عائم              *axis_def_مقدار)

{
  Triple* triple;
  إذا (!درون_داد->axes_location.has(axis_برچسب_2, &triple)) {
    أرجع خطأ;
  }

  *axis_min_مقدار = triple->minimum;
  *axis_def_مقدار = triple->میانه;
  *axis_max_مقدار = triple->بیشینه;
  أرجع درست;
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
                                  عائم *axis_min_مقدار,
                                  عائم *axis_max_مقدار,
                                  عائم *axis_def_مقدار)
{
  إذا (len < 0)
    len = طول_رشته (str);

  ثابت حرف *انتها = str + len;
  ثابت حرف* part = strpbrk (str, ":");
  إذا (!part)
  {
    // Single value.
    إذا (strcmp (str, "drop") == 0)
    {
      *axis_min_مقدار = NAN;
      *axis_def_مقدار = NAN;
      *axis_max_مقدار = NAN;
      أرجع درست;
    }

    مزدوج v;
    إذا (!hb_parse_double (&str, انتها, &v)) أرجع خطأ;

    *axis_min_مقدار = v;
    *axis_def_مقدار = v;
    *axis_max_مقدار = v;
    أرجع درست;
  }

  عائم values[3];
  صحيح شمار = 0;
  لكل (صحيح i = 0; i < 3; i++) {
    شمار++;
    إذا (!*str || part == str)
    {
      values[i] = NAN;

      إذا (part == عدم) اكسر;
      str = part + 1;
      part = strpbrk (str, ":");
      استمر;
    }

    مزدوج v;
    ثابت حرف *piece_انتها = part ? part : انتها;
    إذا (!hb_parse_double (&str, piece_انتها, &v)) أرجع خطأ;
    values[i] = v;

    إذا (part == عدم) اكسر;
    str = part + 1;
    part = strpbrk (str, ":");
  }

  إذا (شمار == 2)
  {
    *axis_min_مقدار = values[0];
    *axis_def_مقدار = NAN;
    *axis_max_مقدار = values[1];
    أرجع درست;
  }
  وإلا إذا (شمار == 3)
  {
    *axis_min_مقدار = values[0];
    *axis_def_مقدار = values[1];
    *axis_max_مقدار = values[2];
    أرجع درست;
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
hb_subset_axis_range_مقصد_string (hb_subset_درون_داد_t *درون_داد,
                                hb_برچسب_t axis_برچسب_2,
                                حرف *buf, بیعلامت اندازه)
{
  إذا (unlikely (!اندازه)) أرجع;
  Triple* triple;
  إذا (!درون_داد->axes_location.has(axis_برچسب_2, &triple)) {
    أرجع;
  }

  حرف s[128];
  بیعلامت len = 0;

  hb_locale_t clocale HB_استفادهنشده;
  hb_locale_t oldlocale HB_استفادهنشده;
  oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", عدم));
  len += hb_max (0, snprintf (s, ARRAY_درازی (s) - len, "%g", (مزدوج) triple->minimum));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_درازی (s) - len, "%g", (مزدوج) triple->میانه));
  s[len++] = ':';

  len += hb_max (0, snprintf (s + len, ARRAY_درازی (s) - len, "%g", (مزدوج) triple->بیشینه));
  (فراغ) hb_uselocale (((فراغ) freelocale (clocale), oldlocale));

  أكد (len < ARRAY_درازی (s));
  len = hb_min (len, اندازه - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}
#پایان_اگر

#اگر_تعریف_شده HB_EXPERIMENTAL_API
ساكن فراغ
_append_arg (hb_وکتور_t<حرف> &sb, ثابت حرف *arg)
{
  إذا (sb.درون_error ()) أرجع;
  إذا (sb.درازی)
    sb.push (' ');
  بیعلامت len = طول_رشته (arg);
  sb.extend (hb_بایتها_t (arg, len));
}
ساكن فراغ
format_عدد_مجموعه (ثابت hb_مجموعه_t *مجموعه, hb_وکتور_t<حرف> &بیرون, بیعلامت پایه = 10)
{
  أكد (پایه == 10 || پایه == 16);
  إذا (unlikely (پایه != 10 && پایه != 16)) أرجع;

  hb_codepoint_t اولی = HB_مجموعه_مقدار_INVALID, آخرین = HB_مجموعه_مقدار_INVALID;
  منطقي اولی_range = درست;
  طالما (hb_مجموعه_بعدی_range (مجموعه, &اولی, &آخرین))
  {
    إذا (!اولی_range)
      بیرون.push (',');
    اولی_range = خطأ;

    حرف buf[64];
    صحيح len;
    إذا (اولی == آخرین)
    {
      إذا (پایه == 16)
        len = snprintf (buf, حجم (buf), "%X", اولی);
      وإلا
        len = snprintf (buf, حجم (buf), "%u", اولی);
    }
    وإلا
    {
      إذا (پایه == 16)
        len = snprintf (buf, حجم (buf), "%X-%X", اولی, آخرین);
      وإلا
        len = snprintf (buf, حجم (buf), "%u-%u", اولی, آخرین);
    }
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع;
    بیرون.extend (hb_بایتها_t (buf, len));
  }
}

ساكن فراغ
format_برچسب (hb_برچسب_t برچسب_2, hb_وکتور_t<حرف> &بیرون)
{
  حرف buf[4];
  hb_برچسب_مقصد_string (برچسب_2, buf);

  بیعلامت len = 4;
  طالما (len > 0 && buf[len - 1] == ' ')
    len--;

  لكل (بیعلامت i = 0; i < len; i++)
  {
    حرف c = buf[i];
    إذا (!((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
          c == '-' || c == '_'))
    {
      c = '_';
    }
    بیرون.push (c);
  }
}

ساكن فراغ
format_برچسب_مجموعه (ثابت hb_مجموعه_t *مجموعه, hb_وکتور_t<حرف> &بیرون)
{
  منطقي اولی_برچسب = درست;
  لكل (hb_codepoint_t برچسب_2 : *مجموعه)
  {
    إذا (!اولی_برچسب)
      بیرون.push (',');
    اولی_برچسب = خطأ;

    format_برچسب (برچسب_2, بیرون);
  }
}

ساكن منطقي
is_مجموعه_default (ثابت hb_مجموعه_t *مجموعه, ثابت hb_مجموعه_t *default_مجموعه)
{
  أرجع hb_مجموعه_is_inverted (مجموعه) == hb_مجموعه_is_inverted (default_مجموعه) &&
         hb_مجموعه_is_equal (مجموعه, default_مجموعه);
}

قالب <نام_نوع Formatter>
ساكن فراغ
format_مجموعه_arg (ثابت hb_مجموعه_t *مجموعه,
                 ثابت حرف *opt_name,
                 Formatter &&formatter,
                 hb_وکتور_t<حرف> &sb)
{
  إذا (hb_مجموعه_is_inverted (مجموعه))
  {
    hb_مجموعه_t *excluded = hb_مجموعه_copy (مجموعه);
    hb_مجموعه_invert (excluded);

    hb_وکتور_t<حرف> opt;
    opt.extend (hb_بایتها_t ("--", 2));
    opt.extend (hb_بایتها_t (opt_name, طول_رشته (opt_name)));
    opt.extend (hb_بایتها_t ("=*", 2));
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);

    إذا (!hb_مجموعه_is_خالی (excluded))
    {
      hb_وکتور_t<حرف> formatted;
      formatter (excluded, formatted);
      إذا (formatted.درازی)
      {
        hb_وکتور_t<حرف> opt2;
        opt2.extend (hb_بایتها_t ("--", 2));
        opt2.extend (hb_بایتها_t (opt_name, طول_رشته (opt_name)));
        opt2.extend (hb_بایتها_t ("-=", 2));
        opt2.extend (formatted.as_array ());
        opt2.push ('\0');
        إذا (opt2.arrayZ) _append_arg (sb, opt2.arrayZ);
      }
    }
    hb_مجموعه_destroy (excluded);
  }
  وإلا إذا (hb_مجموعه_is_خالی (مجموعه))
  {
    hb_وکتور_t<حرف> opt;
    opt.extend (hb_بایتها_t ("--", 2));
    opt.extend (hb_بایتها_t (opt_name, طول_رشته (opt_name)));
    opt.extend (hb_بایتها_t ("-=*", 3));
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  وإلا
  {
    hb_وکتور_t<حرف> formatted;
    formatter (مجموعه, formatted);
    إذا (formatted.درازی)
    {
      hb_وکتور_t<حرف> opt;
      opt.extend (hb_بایتها_t ("--", 2));
      opt.extend (hb_بایتها_t (opt_name, طول_رشته (opt_name)));
      opt.extend (hb_بایتها_t ("=", 1));
      opt.extend (formatted.as_array ());
      opt.push ('\0');
      إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
    }
  }
}

ساكن فراغ
format_عدد_مجموعه_arg (ثابت hb_مجموعه_t *مجموعه,
                        ثابت حرف *opt_name,
                        hb_وکتور_t<حرف> &sb,
                        بیعلامت پایه = 10)
{
  format_مجموعه_arg (مجموعه, opt_name, [پایه] (ثابت hb_مجموعه_t *s, hb_وکتور_t<حرف> &بیرون) {
    format_عدد_مجموعه (s, بیرون, پایه);
  }, sb);
}

ساكن فراغ
format_برچسب_مجموعه_arg (ثابت hb_مجموعه_t *مجموعه,
                     ثابت حرف *opt_name,
                     hb_وکتور_t<حرف> &sb)
{
  format_مجموعه_arg (مجموعه, opt_name, format_برچسب_مجموعه, sb);
}

ساكن منطقي
_format_glyph_map_arg (ثابت hb_map_t &glyph_map,
                       hb_وکتور_t<حرف> &sb)
{
  إذا (glyph_map.is_empty ())
    أرجع درست;

  hb_مجموعه_t کلیدها;
  hb_map_کلیدها (&glyph_map, &کلیدها);
  hb_وکتور_t<حرف> جفتها;
  منطقي اولی = درست;
  لكل (hb_codepoint_t key : کلیدها)
  {
    إذا (!اولی)
      جفتها.push (',');
    اولی = خطأ;
    hb_codepoint_t val = hb_map_get (&glyph_map, key);
    حرف buf[64];
    صحيح len = snprintf (buf, حجم (buf), "%u:%u", key, val);
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع خطأ;
    جفتها.extend (hb_بایتها_t (buf, len));
  }
  إذا (جفتها.درازی)
  {
    hb_وکتور_t<حرف> opt;
    opt.extend (hb_بایتها_t ("--gid-map=", 10));
    opt.extend (جفتها.as_array ());
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  أرجع درست;
}

ساكن منطقي
_format_axes_location_arg (ثابت hb_hashmap_t<hb_برچسب_t, Triple> &axes_location,
                           hb_وکتور_t<حرف> &sb)
{
  إذا (axes_location.is_empty ())
    أرجع درست;

  hb_وکتور_t<hb_برچسب_t> axis_برچسبها;
  لكل (خودکار برچسب_2 : axes_location.کلیدها ())
    axis_برچسبها.push (برچسب_2);
  axis_برچسبها.qsort ([] (ثابت hb_برچسب_t &a, ثابت hb_برچسب_t &b) { أرجع a < b ? -1 : a > b ? 1 : 0; });

  hb_وکتور_t<حرف> specs;
  منطقي اولی = درست;
  لكل (hb_برچسب_t برچسب_2 : axis_برچسبها)
  {
    Triple triple = axes_location.get (برچسب_2);
    إذا (!اولی)
      specs.push (',');
    اولی = خطأ;

    hb_وکتور_t<حرف> برچسب_buf;
    format_برچسب (برچسب_2, برچسب_buf);

    حرف buf[128];
    صحيح len;
    إذا (triple.minimum == triple.میانه && triple.میانه == triple.بیشینه)
    {
      len = snprintf (buf, حجم (buf), "%.*s=%g", (صحيح) برچسب_buf.درازی, برچسب_buf.arrayZ, (مزدوج) triple.میانه);
    }
    وإلا إذا (std::isnan (triple.minimum) && std::isnan (triple.میانه) && std::isnan (triple.بیشینه))
    {
      len = snprintf (buf, حجم (buf), "%.*s=drop", (صحيح) برچسب_buf.درازی, برچسب_buf.arrayZ);
    }
    وإلا
    {
      len = snprintf (buf, حجم (buf), "%.*s=%g:%g:%g", (صحيح) برچسب_buf.درازی, برچسب_buf.arrayZ,
                      (مزدوج) triple.minimum, (مزدوج) triple.میانه, (مزدوج) triple.بیشینه);
    }
    إذا (unlikely (len < 0 || (size_t) len >= حجم (buf))) أرجع خطأ;
    specs.extend (hb_بایتها_t (buf, len));
  }
  إذا (specs.درازی)
  {
    hb_وکتور_t<حرف> opt;
    opt.extend (hb_بایتها_t ("--variations=", 13));
    opt.extend (specs.as_array ());
    opt.push ('\0');
    إذا (opt.arrayZ) _append_arg (sb, opt.arrayZ);
  }
  أرجع درست;
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
hb_subset_درون_داد_مقصد_string_or_fail (hb_subset_درون_داد_t *درون_داد)
{
  إذا (unlikely (!درون_داد || درون_داد->درون_error ()))
    أرجع اشاره_تهی;

  hb_وکتور_t<حرف> sb;

  بنية flag_option_t {
    hb_subset_flags_t flag;
    حرف option[32];
  };
  ساكن ثابت flag_option_t flag_options[] = {
    { HB_SUBSET_FLAGS_NO_HINTING, "no-hinting" },
    { HB_SUBSET_FLAGS_RETAIN_GIDS, "retain-gids" },
    { HB_SUBSET_FLAGS_DESUBROUTINIZE, "desubroutinize" },
    { HB_SUBSET_FLAGS_NAME_LEGACY, "name-legacy" },
    { HB_SUBSET_FLAGS_مجموعه_OVERLAPS_FLAG, "set-overlaps-flag" },
    { HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED, "passthrough-tables" },
    { HB_SUBSET_FLAGS_NOTDEF_OUTLINE, "notdef-outline" },
    { HB_SUBSET_FLAGS_GLYPH_نامها, "glyph-names" },
    { HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES, "no-prune-unicode-ranges" },
    { HB_SUBSET_FLAGS_NO_LAYOUT_بستگی, "no-layout-closure" },
    { HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS, "optimize" },
    { HB_SUBSET_FLAGS_NO_BIDI_بستگی, "no-bidi-closure" },
#اگر_تعریف_شده HB_EXPERIMENTAL_API
    { HB_SUBSET_FLAGS_IFTB_REQUIREMENTS, "iftb-requirements" },
    { HB_SUBSET_FLAGS_RETAIN_رقم_GLYPHS, "retain-num-glyphs" },
#پایان_اگر
    { HB_SUBSET_FLAGS_DOWNGRADE_CFF2, "downgrade-cff2" },
  };

#اگر_تعریف_شده HB_EXPERIMENTAL_API
  static_assert (حجم (flag_options) / حجم (flag_options[0]) == 15,
                 "Check all flags in hb_subset_flags_t are handled here.");
#وإلا
  static_assert (حجم (flag_options) / حجم (flag_options[0]) == 13,
                 "Check all flags in hb_subset_flags_t are handled here.");
#پایان_اگر

  لكل (ثابت خودکار &fo : flag_options)
  {
    إذا (درون_داد->flags & fo.flag)
    {
      حرف buf[36];
      snprintf (buf, حجم (buf), "--%s", fo.option);
      _append_arg (sb, buf);
    }
  }

  hb_subset_درون_داد_t *default_درون_داد = hb_subset_درون_داد_create_or_fail ();
  إذا (unlikely (!default_درون_داد))
    أرجع اشاره_تهی;

  // Serialize all sets if they are not equal to their default setting:
#تعریف_کن PROCESS_عدد_مجموعه(فیلد, opt_name, ...) \
  إذا (!is_مجموعه_default (درون_داد->sets.فیلد, default_درون_داد->sets.فیلد)) \
    format_عدد_مجموعه_arg (درون_داد->sets.فیلد, opt_name, sb, ##__VA_ARGS__)

#تعریف_کن PROCESS_برچسب_مجموعه(فیلد, opt_name) \
  إذا (!is_مجموعه_default (درون_داد->sets.فیلد, default_درون_داد->sets.فیلد)) \
    format_برچسب_مجموعه_arg (درون_داد->sets.فیلد, opt_name, sb)

  PROCESS_عدد_مجموعه (unicodes, "unicodes", 16);
  PROCESS_عدد_مجموعه (glyphs, "gids");
  PROCESS_عدد_مجموعه (name_ids, "name-IDs");
  PROCESS_عدد_مجموعه (name_languages, "name-languages");
  PROCESS_برچسب_مجموعه (layout_features, "layout-features");
  PROCESS_برچسب_مجموعه (layout_scripts, "layout-scripts");
  PROCESS_برچسب_مجموعه (drop_tables, "drop-tables");
  PROCESS_برچسب_مجموعه (no_subset_tables, "passthrough-tables");

#لغو_تعریف PROCESS_عدد_مجموعه
#لغو_تعریف PROCESS_برچسب_مجموعه

  hb_subset_درون_داد_destroy (default_درون_داد);

  static_assert (حجم (درون_داد->sets) / حجم (hb_مجموعه_t*) == 8,
                 "Check all sets in hb_subset_input_t::sets_t are handled here.");

  إذا (unlikely (!_format_glyph_map_arg (درون_داد->glyph_map, sb)))
    أرجع اشاره_تهی;

  إذا (unlikely (!_format_axes_location_arg (درون_داد->axes_location, sb)))
    أرجع اشاره_تهی;

  sb.push ('\0');
  إذا (unlikely (sb.درون_error ()))
    أرجع اشاره_تهی;

  بیعلامت len = 0;
  حرف *buf = sb.steal (&len);
  إذا (unlikely (!buf))
    أرجع اشاره_تهی;

  أرجع hb_blob_create_or_fail (buf, len, HB_حافظه_نما_WRITABLE, buf, hb_free);
}
#پایان_اگر

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
hb_subset_preprocess (hb_face_t *منبع)
{
  hb_subset_درون_داد_t* درون_داد = hb_subset_درون_داد_create_or_fail ();
  إذا (!درون_داد)
    أرجع hb_face_reference (منبع);

  hb_subset_درون_داد_keep_everything (درون_داد);

  درون_داد->attach_accelerator_داده = درست;

  // Always use long loca in the preprocessed version. This allows
  // us to store the glyph bytes unpadded which allows the future subset
  // operation to run faster by skipping the trim padding step.
  درون_داد->force_long_loca = درست;

  hb_face_t* new_منبع = hb_subset_or_fail (منبع, درون_داد);
  hb_subset_درون_داد_destroy (درون_داد);

  إذا (!new_منبع) {
    DEBUG_پیام (SUBSET, اشاره_تهی, "Preprocessing failed due to subset failure.");
    أرجع hb_face_reference (منبع);
  }

  أرجع new_منبع;
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
hb_subset_درون_داد_old_مقصد_new_glyph_mapping (hb_subset_درون_داد_t *درون_داد)
{
  أرجع &درون_داد->glyph_map;
}

#اگر_تعریف_شده HB_EXPERIMENTAL_API
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
hb_subset_درون_داد_override_name_جدول (hb_subset_درون_داد_t  *درون_داد,
                                     hb_ot_name_id_t     name_id,
                                     بیعلامت            platform_id,
                                     بیعلامت            encoding_id,
                                     بیعلامت            language_id,
                                     ثابت حرف         *name_str,
                                     صحيح                 str_len /* -1 means nul-terminated */)
{
  إذا (!name_str)
  {
    str_len = 0;
  }
  وإلا إذا (str_len == -1)
  {
      str_len = طول_رشته (name_str);
  }

  hb_بایتها_t name_بایتها (اشاره_تهی, 0);
  إذا (str_len)
  {
    إذا (platform_id == 1)
    {
      ثابت uint8_t *src = reinterpret_cast<ثابت uint8_t*> (name_str);
      ثابت uint8_t *src_انتها = src + str_len;

      hb_codepoint_t unicode;
      ثابت hb_codepoint_t replacement = HB_میانگیر_REPLACEMENT_CODEPOINT_DEFAULT;
      طالما (src < src_انتها)
      {
        src = hb_utf8_t::next (src, src_انتها, &unicode, replacement);
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
    name_بایتها = hb_بایتها_t (override_name, str_len);
  }
  درون_داد->name_جدول_overrides.مجموعه (hb_ot_name_رکورد_ids_t (platform_id, encoding_id, language_id, name_id), name_بایتها);
  أرجع درست;
}
#پایان_اگر
