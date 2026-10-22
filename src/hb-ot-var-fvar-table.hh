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

#அந்தர்பாவ "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#define HB_OT_TAG_fvar HB_TAG('f','v','a','r')


நாமாகாஶ OT {

நிலையான தருக்க axis_coord_pinned_or_within_axis_range (மாறிலி hb_array_t<மாறிலி F16DOT16> coords,
                                                    unsigned axis_index,
                                                    Triple axis_limit)
{
  தசம axis_coord = coords[axis_index].to_float ();
  எனில் (axis_limit.is_point ())
  {
    எனில் (axis_limit.minimum != axis_coord)
      திரும்பு பொய்;
  }
  இல்லையெனில்
  {
    எனில் (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      திரும்பு பொய்;
  }
  திரும்பு உண்மை;
}

கட்டமைப்பு InstanceRecord
{
  friend கட்டமைப்பு fvar;

  hb_array_t<மாறிலி F16DOT16> get_coordinates (unsigned முழு axis_மொத்தம்) மாறிலி
  { திரும்பு coordinatesZ.as_array (axis_மொத்தம்); }

  தருக்க keep_instance (unsigned axis_மொத்தம்,
                      மாறிலி hb_map_t *axes_index_tag_map,
                      மாறிலி hb_hashmap_t<hb_tag_t, Triple> *axes_location) மாறிலி
  {
    எனில் (axes_location->is_empty ()) திரும்பு உண்மை;
    மாறிலி hb_array_t<மாறிலி F16DOT16> coords = get_coordinates (axis_மொத்தம்);
    க்கு (unsigned i = 0 ; i < axis_மொத்தம்; i++)
    {
      uint32_t *axis_tag;
      எனில் (!axes_index_tag_map->has (i, &axis_tag))
        திரும்பு பொய்;
      எனில் (!axes_location->has (*axis_tag))
        தொடர்;

      Triple axis_limit = axes_location->get (*axis_tag);
      எனில் (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        திரும்பு பொய்;
    }
    திரும்பு உண்மை;
  }

  தருக்க subset (hb_subset_context_t *c,
               unsigned axis_மொத்தம்,
               தருக்க has_postscript_nameid) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (பொய்);
    எனில் (unlikely (!c->serializer->embed (flags))) return_trace (பொய்);

    மாறிலி hb_array_t<மாறிலி F16DOT16> coords = get_coordinates (axis_மொத்தம்);
    மாறிலி hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    க்கு (unsigned i = 0 ; i < axis_மொத்தம்; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      எனில் (!c->plan->axes_old_index_tag_map.has (i, &axis_tag)) return_trace (பொய்);
      எனில் (axes_location->has (*axis_tag, &axis_limit))
      {
        எனில் (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (பொய்);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        எனில் (axis_limit->is_point () && !c->plan->axes_index_map.has (i))
          தொடர்;
      }

      எனில் (!c->serializer->embed (coords[i]))
        return_trace (பொய்);
    }

    எனில் (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      எனில் (!c->serializer->embed (name_id))
        return_trace (பொய்);
    }

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு axis_மொத்தம்) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_மொத்தம்));
  }

  பாதுகாக்கப்பட்ட:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  UnsizedArrayOf<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  பொது:
  DEFINE_SIZE_UNBOUNDED (4);
};

கட்டமைப்பு AxisRecord
{
  முழு cmp (hb_tag_t key) மாறிலி { திரும்பு axisTag.cmp (key); }

  எண்ணிக்கை
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  வெறுமை get_axis_deprecated (hb_ot_var_axis_t *info) மாறிலி
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_மதிப்பு, info->default_மதிப்பு, info->max_மதிப்பு);
  }
#endif

  வெறுமை get_axis_info (unsigned axis_index, hb_ot_var_axis_info_t *info) மாறிலி
  {
    info->axis_index = axis_index;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (unsigned முழு) flags;
    get_coordinates (info->min_மதிப்பு, info->default_மதிப்பு, info->max_மதிப்பு);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () மாறிலி { திரும்பு axisTag; }

  தருக்க is_hidden () மாறிலி { திரும்பு flags & AXIS_FLAG_HIDDEN; }

  தசம normalize_axis_மதிப்பு (தசம v) மாறிலி
  {
    தசம min_மதிப்பு, default_மதிப்பு, max_மதிப்பு;
    get_coordinates (min_மதிப்பு, default_மதிப்பு, max_மதிப்பு);

    v = hb_clamp (v, min_மதிப்பு, max_மதிப்பு);

    எனில் (v == default_மதிப்பு)
      திரும்பு 0;
    இல்லையெனில் எனில் (v < default_மதிப்பு)
      திரும்பு (v - default_மதிப்பு) / (default_மதிப்பு - min_மதிப்பு);
    இல்லையெனில்
      திரும்பு (v - default_மதிப்பு) / (max_மதிப்பு - default_மதிப்பு);
  }

  hb_ot_name_id_t get_name_id () மாறிலி { திரும்பு axisNameID; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  வெறுமை get_coordinates (தசம &min, தசம &default_, தசம &max) மாறிலி
  {
    default_ = default_மதிப்பு_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_மதிப்பு_2.to_float ());
    max = hb_max (default_, max_மதிப்பு_2.to_float ());
  }

  தசம get_default () மாறிலி
  {
    திரும்பு default_மதிப்பு_2.to_float ();
  }

  TripleDistances get_triple_distances () மாறிலி
  {
    தசம min, default_, max;
    get_coordinates (min, default_, max);
    திரும்பு TripleDistances (min, default_, max);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    மாறிலி hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    எனில் (user_axes_location.has (axisTag, &axis_limit))
    {
      எனில் (c->plan->has_avar2 && axis_limit->is_point ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        out->min_மதிப்பு_2.set_float (axis_limit->நடு);
        out->default_மதிப்பு_2.set_float (axis_limit->நடு);
        out->max_மதிப்பு_2.set_float (axis_limit->நடு);
        out->flags = out->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      இல்லையெனில்
      {
        out->min_மதிப்பு_2.set_float (axis_limit->minimum);
        out->default_மதிப்பு_2.set_float (axis_limit->நடு);
        out->max_மதிப்பு_2.set_float (axis_limit->maximum);
      }
    }
    return_trace (உண்மை);
  }

  பொது:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  பாதுகாக்கப்பட்ட:
  F16DOT16	min_மதிப்பு_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_மதிப்பு_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_மதிப்பு_2;	/* The maximum coordinate value for the axis. */
  பொது:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  பொது:
  DEFINE_SIZE_STATIC (20);
};

கட்டமைப்பு fvar
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  தருக்க has_data () மாறிலி { திரும்பு version.to_int (); }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (இது) &&
		  hb_barrier () &&
		  axisSize == 20 && /* Assumed in our code. */
		  instanceSize >= axis_மொத்தம்_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_மொத்தம், instanceSize));
  }

  unsigned முழு get_axis_மொத்தம் () மாறிலி { திரும்பு axis_மொத்தம்_2; }

#ifndef HB_DISABLE_DEPRECATED
  unsigned முழு get_axes_deprecated (unsigned முழு      start_offset,
				    unsigned முழு     *axes_மொத்தம் /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) மாறிலி
  {
    எனில் (axes_மொத்தம் && axes_array)
    {
      hb_array_t<மாறிலி AxisRecord> arr = get_axes ().sub_array (start_offset, axes_மொத்தம்);
      க்கு (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    திரும்பு axis_மொத்தம்_2;
  }
#endif

  unsigned முழு get_axis_infos (unsigned முழு           start_offset,
			       unsigned முழு          *axes_மொத்தம் /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) மாறிலி
  {
    எனில் (axes_மொத்தம் && axes_array)
    {
      hb_array_t<மாறிலி AxisRecord> arr = get_axes ().sub_array (start_offset, axes_மொத்தம்);
      க்கு (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    திரும்பு axis_மொத்தம்_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  தருக்க
  find_axis_deprecated (hb_tag_t tag, unsigned *axis_index, hb_ot_var_axis_t *info) மாறிலி
  {
    unsigned i;
    எனில் (!axis_index) axis_index = &i;
    *axis_index = HB_OT_VAR_NO_AXIS_INDEX;
    auto axes = get_axes ();
    திரும்பு axes.lfind (tag, axis_index) && ((வெறுமை) axes[*axis_index].get_axis_deprecated (info), உண்மை);
  }
#endif
  தருக்க
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) மாறிலி
  {
    unsigned i;
    auto axes = get_axes ();
    திரும்பு axes.lfind (tag, &i) && ((வெறுமை) axes[i].get_axis_info (i, info), உண்மை);
  }

  தசம normalize_axis_மதிப்பு (unsigned முழு axis_index, தசம v) மாறிலி
  { திரும்பு get_axes ()[axis_index].normalize_axis_மதிப்பு (v); }

  unsigned முழு get_instance_மொத்தம் () மாறிலி { திரும்பு instance_மொத்தம்; }

  hb_ot_name_id_t get_instance_subfamily_name_id (unsigned முழு instance_index) மாறிலி
  {
    மாறிலி InstanceRecord *instance = get_instance (instance_index);
    எனில் (unlikely (!instance)) திரும்பு HB_OT_NAME_ID_INVALID;
    திரும்பு instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (unsigned முழு instance_index) மாறிலி
  {
    மாறிலி InstanceRecord *instance = get_instance (instance_index);
    எனில் (unlikely (!instance)) திரும்பு HB_OT_NAME_ID_INVALID;
    எனில் (instanceSize >= axis_மொத்தம்_2 * 4 + 6)
      திரும்பு StructAfter<NameID> (instance->get_coordinates (axis_மொத்தம்_2));
    திரும்பு HB_OT_NAME_ID_INVALID;
  }

  unsigned முழு get_instance_coords (unsigned முழு  instance_index,
				    unsigned முழு *coords_length, /* IN/OUT */
				    தசம        *coords         /* OUT */) மாறிலி
  {
    மாறிலி InstanceRecord *instance = get_instance (instance_index);
    எனில் (unlikely (!instance))
    {
      எனில் (coords_length)
	*coords_length = 0;
      திரும்பு 0;
    }

    எனில் (coords_length && *coords_length && coords)
    {
      hb_array_t<மாறிலி F16DOT16> instanceCoords = instance->get_coordinates (axis_மொத்தம்_2)
							 .sub_array (0, coords_length);
      க்கு (unsigned முழு i = 0; i < instanceCoords.length; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    திரும்பு axis_மொத்தம்_2;
  }

  வெறுமை collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_index_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) மாறிலி
  {
    எனில் (!has_data ()) திரும்பு;

    auto axis_records = get_axes ();
    க்கு (unsigned i = 0 ; i < (unsigned)axis_மொத்தம்_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      எனில் (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        தொடர்;

      nameids->add (axis_records[i].get_name_id ());
    }

    க்கு (unsigned i = 0 ; i < (unsigned)instance_மொத்தம்; i++)
    {
      மாறிலி InstanceRecord *instance = get_instance (i);

      எனில் (!instance->keep_instance (axis_மொத்தம்_2, axes_old_index_tag_map, user_axes_location))
        தொடர்;

      nameids->add (instance->subfamilyNameID);

      எனில் (instanceSize >= axis_மொத்தம்_2 * 4 + 6)
      {
        unsigned post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_மொத்தம்_2));
        எனில் (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    unsigned retained_axis_மொத்தம் = c->plan->axes_index_map.get_population ();
    எனில் (!retained_axis_மொத்தம்) //all axes are pinned
      return_trace (பொய்);

    fvar *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (!c->serializer->check_assign (out->axis_மொத்தம்_2, retained_axis_மொத்தம், HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);

    தருக்க has_postscript_nameid = பொய்;
    எனில் (instanceSize >= axis_மொத்தம்_2 * 4 + 6)
      has_postscript_nameid = உண்மை;

    எனில் (!c->serializer->check_assign (out->instanceSize, retained_axis_மொத்தம் * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);

    auto axes_records = get_axes ();
    க்கு (unsigned i = 0 ; i < (unsigned)axis_மொத்தம்_2; i++)
    {
      எனில் (!c->plan->axes_index_map.has (i)) தொடர்;
      எனில் (unlikely (!axes_records[i].subset (c)))
        return_trace (பொய்);
    }

    எனில் (!c->serializer->check_assign (out->firstAxis, get_size (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);

    unsigned இலக்கம்_retained_instances = 0;
    க்கு (unsigned i = 0 ; i < (unsigned)instance_மொத்தம்; i++)
    {
      மாறிலி InstanceRecord *instance = get_instance (i);
      auto snap = c->serializer->snapshot ();
      எனில் (!instance->subset (c, axis_மொத்தம்_2, has_postscript_nameid))
        c->serializer->revert (snap);
      இல்லையெனில்
        இலக்கம்_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_மொத்தம், இலக்கம்_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  பொது:
  hb_array_t<மாறிலி AxisRecord> get_axes () மாறிலி
  { திரும்பு hb_array (&(இது+firstAxis), axis_மொத்தம்_2); }

  மாறிலி InstanceRecord *get_instance (unsigned முழு i) மாறிலி
  {
    எனில் (unlikely (i >= instance_மொத்தம்)) திரும்பு nullptr;
   திரும்பு &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instanceSize);
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_மொத்தம்_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axisSize;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_மொத்தம்;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instanceSize;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  பொது:
  DEFINE_SIZE_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
