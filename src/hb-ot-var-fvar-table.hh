/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OT_VAR_FVAR_TABLE_HH
#define HB_OT_VAR_FVAR_TABLE_HH

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#define HB_OT_TAG_fvar HB_TAG('f','v','a','r')


ನಾಮಾಕಾಶ OT {

ಸ್ಥಿರ ತಾರ್ಕಿಕ axis_coord_pinned_or_within_axis_range (ಧ್ರುವ hb_array_t<ಧ್ರುವ F16DOT16> coords,
                                                    unsigned axis_index,
                                                    Triple axis_limit)
{
  ಭಿನ್ನ axis_coord = coords[axis_index].to_float ();
  ಯದಿ (axis_limit.is_point ())
  {
    ಯದಿ (axis_limit.minimum != axis_coord)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ಅನ್ಯಥಾ
  {
    ಯದಿ (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸಂರಚನಾ InstanceRecord
{
  friend ಸಂರಚನಾ fvar;

  hb_array_t<ಧ್ರುವ F16DOT16> get_coordinates (unsigned ಪೂರ್ಣಾಙ್ಕ axis_ಎಣಿಕೆ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ coordinatesZ.as_array (axis_ಎಣಿಕೆ); }

  ತಾರ್ಕಿಕ keep_instance (unsigned axis_ಎಣಿಕೆ,
                      ಧ್ರುವ hb_map_t *axes_index_tag_map,
                      ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> *axes_location) ಧ್ರುವ
  {
    ಯದಿ (axes_location->is_empty ()) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಧ್ರುವ hb_array_t<ಧ್ರುವ F16DOT16> coords = get_coordinates (axis_ಎಣಿಕೆ);
    ಪ್ರತಿ (unsigned i = 0 ; i < axis_ಎಣಿಕೆ; i++)
    {
      uint32_t *axis_tag;
      ಯದಿ (!axes_index_tag_map->has (i, &axis_tag))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (!axes_location->has (*axis_tag))
        ಅನುವರ್ತ;

      Triple axis_limit = axes_location->get (*axis_tag);
      ಯದಿ (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               unsigned axis_ಎಣಿಕೆ,
               ತಾರ್ಕಿಕ has_postscript_nameid) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->serializer->embed (flags))) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_array_t<ಧ್ರುವ F16DOT16> coords = get_coordinates (axis_ಎಣಿಕೆ);
    ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    ಪ್ರತಿ (unsigned i = 0 ; i < axis_ಎಣಿಕೆ; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      ಯದಿ (!c->plan->axes_old_index_tag_map.has (i, &axis_tag)) return_trace (ಅಸತ್ಯ);
      ಯದಿ (axes_location->has (*axis_tag, &axis_limit))
      {
        ಯದಿ (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (ಅಸತ್ಯ);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        ಯದಿ (axis_limit->is_point () && !c->plan->axes_index_map.has (i))
          ಅನುವರ್ತ;
      }

      ಯದಿ (!c->serializer->embed (coords[i]))
        return_trace (ಅಸತ್ಯ);
    }

    ಯದಿ (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      ಯದಿ (!c->serializer->embed (name_id))
        return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ axis_ಎಣಿಕೆ) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_ಎಣಿಕೆ));
  }

  ರಕ್ಷಿತ:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  UnsizedArrayOf<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNBOUNDED (4);
};

ಸಂರಚನಾ AxisRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_tag_t key) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ axisTag.cmp (key); }

  ಗಣನಾ
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  ಶೂನ್ಯ get_axis_deprecated (hb_ot_var_axis_t *info) ಧ್ರುವ
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_ಮೌಲ್ಯ, info->default_ಮೌಲ್ಯ, info->max_ಮೌಲ್ಯ);
  }
#endif

  ಶೂನ್ಯ get_axis_info (unsigned axis_index, hb_ot_var_axis_info_t *info) ಧ್ರುವ
  {
    info->axis_index = axis_index;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (unsigned ಪೂರ್ಣಾಙ್ಕ) flags;
    get_coordinates (info->min_ಮೌಲ್ಯ, info->default_ಮೌಲ್ಯ, info->max_ಮೌಲ್ಯ);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ axisTag; }

  ತಾರ್ಕಿಕ is_hidden () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ flags & AXIS_FLAG_HIDDEN; }

  ಭಿನ್ನ normalize_axis_ಮೌಲ್ಯ (ಭಿನ್ನ v) ಧ್ರುವ
  {
    ಭಿನ್ನ min_ಮೌಲ್ಯ, default_ಮೌಲ್ಯ, max_ಮೌಲ್ಯ;
    get_coordinates (min_ಮೌಲ್ಯ, default_ಮೌಲ್ಯ, max_ಮೌಲ್ಯ);

    v = hb_clamp (v, min_ಮೌಲ್ಯ, max_ಮೌಲ್ಯ);

    ಯದಿ (v == default_ಮೌಲ್ಯ)
      ಪ್ರತ್ಯಾಗಮ 0;
    ಅನ್ಯಥಾ ಯದಿ (v < default_ಮೌಲ್ಯ)
      ಪ್ರತ್ಯಾಗಮ (v - default_ಮೌಲ್ಯ) / (default_ಮೌಲ್ಯ - min_ಮೌಲ್ಯ);
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ (v - default_ಮೌಲ್ಯ) / (max_ಮೌಲ್ಯ - default_ಮೌಲ್ಯ);
  }

  hb_ot_name_id_t get_name_id () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ axisNameID; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ get_coordinates (ಭಿನ್ನ &min, ಭಿನ್ನ &default_, ಭಿನ್ನ &max) ಧ್ರುವ
  {
    default_ = default_ಮೌಲ್ಯ_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_ಮೌಲ್ಯ_2.to_float ());
    max = hb_max (default_, max_ಮೌಲ್ಯ_2.to_float ());
  }

  ಭಿನ್ನ get_default () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ default_ಮೌಲ್ಯ_2.to_float ();
  }

  TripleDistances get_triple_distances () ಧ್ರುವ
  {
    ಭಿನ್ನ min, default_, max;
    get_coordinates (min, default_, max);
    ಪ್ರತ್ಯಾಗಮ TripleDistances (min, default_, max);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    ಯದಿ (user_axes_location.has (axisTag, &axis_limit))
    {
      ಯದಿ (c->plan->has_avar2 && axis_limit->is_point ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        out->min_ಮೌಲ್ಯ_2.set_float (axis_limit->ಮಧ್ಯ);
        out->default_ಮೌಲ್ಯ_2.set_float (axis_limit->ಮಧ್ಯ);
        out->max_ಮೌಲ್ಯ_2.set_float (axis_limit->ಮಧ್ಯ);
        out->flags = out->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      ಅನ್ಯಥಾ
      {
        out->min_ಮೌಲ್ಯ_2.set_float (axis_limit->minimum);
        out->default_ಮೌಲ್ಯ_2.set_float (axis_limit->ಮಧ್ಯ);
        out->max_ಮೌಲ್ಯ_2.set_float (axis_limit->maximum);
      }
    }
    return_trace (ಸತ್ಯ);
  }

  ಸಾರ್ವಜನಿಕ:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  ರಕ್ಷಿತ:
  F16DOT16	min_ಮೌಲ್ಯ_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_ಮೌಲ್ಯ_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_ಮೌಲ್ಯ_2;	/* The maximum coordinate value for the axis. */
  ಸಾರ್ವಜನಿಕ:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (20);
};

ಸಂರಚನಾ fvar
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int (); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  axisSize == 20 && /* Assumed in our code. */
		  instanceSize >= axis_ಎಣಿಕೆ_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_ಎಣಿಕೆ, instanceSize));
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_axis_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ axis_ಎಣಿಕೆ_2; }

#ifndef HB_DISABLE_DEPRECATED
  unsigned ಪೂರ್ಣಾಙ್ಕ get_axes_deprecated (unsigned ಪೂರ್ಣಾಙ್ಕ      start_offset,
				    unsigned ಪೂರ್ಣಾಙ್ಕ     *axes_ಎಣಿಕೆ /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (axes_ಎಣಿಕೆ && axes_array)
    {
      hb_array_t<ಧ್ರುವ AxisRecord> arr = get_axes ().sub_array (start_offset, axes_ಎಣಿಕೆ);
      ಪ್ರತಿ (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    ಪ್ರತ್ಯಾಗಮ axis_ಎಣಿಕೆ_2;
  }
#endif

  unsigned ಪೂರ್ಣಾಙ್ಕ get_axis_infos (unsigned ಪೂರ್ಣಾಙ್ಕ           start_offset,
			       unsigned ಪೂರ್ಣಾಙ್ಕ          *axes_ಎಣಿಕೆ /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (axes_ಎಣಿಕೆ && axes_array)
    {
      hb_array_t<ಧ್ರುವ AxisRecord> arr = get_axes ().sub_array (start_offset, axes_ಎಣಿಕೆ);
      ಪ್ರತಿ (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    ಪ್ರತ್ಯಾಗಮ axis_ಎಣಿಕೆ_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  ತಾರ್ಕಿಕ
  find_axis_deprecated (hb_tag_t tag, unsigned *axis_index, hb_ot_var_axis_t *info) ಧ್ರುವ
  {
    unsigned i;
    ಯದಿ (!axis_index) axis_index = &i;
    *axis_index = HB_OT_VAR_NO_AXIS_INDEX;
    auto axes = get_axes ();
    ಪ್ರತ್ಯಾಗಮ axes.lfind (tag, axis_index) && ((ಶೂನ್ಯ) axes[*axis_index].get_axis_deprecated (info), ಸತ್ಯ);
  }
#endif
  ತಾರ್ಕಿಕ
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) ಧ್ರುವ
  {
    unsigned i;
    auto axes = get_axes ();
    ಪ್ರತ್ಯಾಗಮ axes.lfind (tag, &i) && ((ಶೂನ್ಯ) axes[i].get_axis_info (i, info), ಸತ್ಯ);
  }

  ಭಿನ್ನ normalize_axis_ಮೌಲ್ಯ (unsigned ಪೂರ್ಣಾಙ್ಕ axis_index, ಭಿನ್ನ v) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_axes ()[axis_index].normalize_axis_ಮೌಲ್ಯ (v); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_instance_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ instance_ಎಣಿಕೆ; }

  hb_ot_name_id_t get_instance_subfamily_name_id (unsigned ಪೂರ್ಣಾಙ್ಕ instance_index) ಧ್ರುವ
  {
    ಧ್ರುವ InstanceRecord *instance = get_instance (instance_index);
    ಯದಿ (unlikely (!instance)) ಪ್ರತ್ಯಾಗಮ HB_OT_NAME_ID_INVALID;
    ಪ್ರತ್ಯಾಗಮ instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (unsigned ಪೂರ್ಣಾಙ್ಕ instance_index) ಧ್ರುವ
  {
    ಧ್ರುವ InstanceRecord *instance = get_instance (instance_index);
    ಯದಿ (unlikely (!instance)) ಪ್ರತ್ಯಾಗಮ HB_OT_NAME_ID_INVALID;
    ಯದಿ (instanceSize >= axis_ಎಣಿಕೆ_2 * 4 + 6)
      ಪ್ರತ್ಯಾಗಮ StructAfter<NameID> (instance->get_coordinates (axis_ಎಣಿಕೆ_2));
    ಪ್ರತ್ಯಾಗಮ HB_OT_NAME_ID_INVALID;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_instance_coords (unsigned ಪೂರ್ಣಾಙ್ಕ  instance_index,
				    unsigned ಪೂರ್ಣಾಙ್ಕ *coords_length, /* IN/OUT */
				    ಭಿನ್ನ        *coords         /* OUT */) ಧ್ರುವ
  {
    ಧ್ರುವ InstanceRecord *instance = get_instance (instance_index);
    ಯದಿ (unlikely (!instance))
    {
      ಯದಿ (coords_length)
	*coords_length = 0;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಯದಿ (coords_length && *coords_length && coords)
    {
      hb_array_t<ಧ್ರುವ F16DOT16> instanceCoords = instance->get_coordinates (axis_ಎಣಿಕೆ_2)
							 .sub_array (0, coords_length);
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < instanceCoords.length; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    ಪ್ರತ್ಯಾಗಮ axis_ಎಣಿಕೆ_2;
  }

  ಶೂನ್ಯ collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_index_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) ಧ್ರುವ
  {
    ಯದಿ (!has_data ()) ಪ್ರತ್ಯಾಗಮ;

    auto axis_records = get_axes ();
    ಪ್ರತಿ (unsigned i = 0 ; i < (unsigned)axis_ಎಣಿಕೆ_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      ಯದಿ (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        ಅನುವರ್ತ;

      nameids->add (axis_records[i].get_name_id ());
    }

    ಪ್ರತಿ (unsigned i = 0 ; i < (unsigned)instance_ಎಣಿಕೆ; i++)
    {
      ಧ್ರುವ InstanceRecord *instance = get_instance (i);

      ಯದಿ (!instance->keep_instance (axis_ಎಣಿಕೆ_2, axes_old_index_tag_map, user_axes_location))
        ಅನುವರ್ತ;

      nameids->add (instance->subfamilyNameID);

      ಯದಿ (instanceSize >= axis_ಎಣಿಕೆ_2 * 4 + 6)
      {
        unsigned post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_ಎಣಿಕೆ_2));
        ಯದಿ (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    unsigned retained_axis_ಎಣಿಕೆ = c->plan->axes_index_map.get_population ();
    ಯದಿ (!retained_axis_ಎಣಿಕೆ) //all axes are pinned
      return_trace (ಅಸತ್ಯ);

    fvar *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (!c->serializer->check_assign (out->axis_ಎಣಿಕೆ_2, retained_axis_ಎಣಿಕೆ, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ has_postscript_nameid = ಅಸತ್ಯ;
    ಯದಿ (instanceSize >= axis_ಎಣಿಕೆ_2 * 4 + 6)
      has_postscript_nameid = ಸತ್ಯ;

    ಯದಿ (!c->serializer->check_assign (out->instanceSize, retained_axis_ಎಣಿಕೆ * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);

    auto axes_records = get_axes ();
    ಪ್ರತಿ (unsigned i = 0 ; i < (unsigned)axis_ಎಣಿಕೆ_2; i++)
    {
      ಯದಿ (!c->plan->axes_index_map.has (i)) ಅನುವರ್ತ;
      ಯದಿ (unlikely (!axes_records[i].subset (c)))
        return_trace (ಅಸತ್ಯ);
    }

    ಯದಿ (!c->serializer->check_assign (out->firstAxis, get_size (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);

    unsigned ಅಂಕೆ_retained_instances = 0;
    ಪ್ರತಿ (unsigned i = 0 ; i < (unsigned)instance_ಎಣಿಕೆ; i++)
    {
      ಧ್ರುವ InstanceRecord *instance = get_instance (i);
      auto snap = c->serializer->snapshot ();
      ಯದಿ (!instance->subset (c, axis_ಎಣಿಕೆ_2, has_postscript_nameid))
        c->serializer->revert (snap);
      ಅನ್ಯಥಾ
        ಅಂಕೆ_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_ಎಣಿಕೆ, ಅಂಕೆ_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ಸಾರ್ವಜನಿಕ:
  hb_array_t<ಧ್ರುವ AxisRecord> get_axes () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_array (&(ಏತತ್+firstAxis), axis_ಎಣಿಕೆ_2); }

  ಧ್ರುವ InstanceRecord *get_instance (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  {
    ಯದಿ (unlikely (i >= instance_ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ nullptr;
   ಪ್ರತ್ಯಾಗಮ &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instanceSize);
  }

  ರಕ್ಷಿತ:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_ಎಣಿಕೆ_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axisSize;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_ಎಣಿಕೆ;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instanceSize;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
