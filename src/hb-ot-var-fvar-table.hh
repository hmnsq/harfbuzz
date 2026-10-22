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

#অন্তর্ভাৱ "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#define HB_OT_TAG_fvar HB_TAG('f','v','a','r')


নামাকাশ OT {

স্থির তার্কিক axis_coord_pinned_or_within_axis_range (ধ্রুৱ hb_array_t<ধ্রুৱ F16DOT16> coords,
                                                    unsigned axis_index,
                                                    Triple axis_limit)
{
  ভিন্ন axis_coord = coords[axis_index].to_float ();
  যদি (axis_limit.is_point ())
  {
    যদি (axis_limit.minimum != axis_coord)
      ঘূৰাই মিছা;
  }
  অন্যথা
  {
    যদি (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      ঘূৰাই মিছা;
  }
  ঘূৰাই সঁচা;
}

সংরচনা InstanceRecord
{
  friend সংরচনা fvar;

  hb_array_t<ধ্রুৱ F16DOT16> get_coordinates (unsigned পূর্ণাঙ্ক axis_গুনতি) ধ্রুৱ
  { ঘূৰাই coordinatesZ.as_array (axis_গুনতি); }

  তার্কিক keep_instance (unsigned axis_গুনতি,
                      ধ্রুৱ hb_map_t *axes_index_tag_map,
                      ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> *axes_location) ধ্রুৱ
  {
    যদি (axes_location->is_empty ()) ঘূৰাই সঁচা;
    ধ্রুৱ hb_array_t<ধ্রুৱ F16DOT16> coords = get_coordinates (axis_গুনতি);
    বাবে (unsigned i = 0 ; i < axis_গুনতি; i++)
    {
      uint32_t *axis_tag;
      যদি (!axes_index_tag_map->has (i, &axis_tag))
        ঘূৰাই মিছা;
      যদি (!axes_location->has (*axis_tag))
        অনুৱর্ত;

      Triple axis_limit = axes_location->get (*axis_tag);
      যদি (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        ঘূৰাই মিছা;
    }
    ঘূৰাই সঁচা;
  }

  তার্কিক subset (hb_subset_context_t *c,
               unsigned axis_গুনতি,
               তার্কিক has_postscript_nameid) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (মিছা);
    যদি (unlikely (!c->serializer->embed (flags))) return_trace (মিছা);

    ধ্রুৱ hb_array_t<ধ্রুৱ F16DOT16> coords = get_coordinates (axis_গুনতি);
    ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    বাবে (unsigned i = 0 ; i < axis_গুনতি; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      যদি (!c->plan->axes_old_index_tag_map.has (i, &axis_tag)) return_trace (মিছা);
      যদি (axes_location->has (*axis_tag, &axis_limit))
      {
        যদি (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (মিছা);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        যদি (axis_limit->is_point () && !c->plan->axes_index_map.has (i))
          অনুৱর্ত;
      }

      যদি (!c->serializer->embed (coords[i]))
        return_trace (মিছা);
    }

    যদি (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      যদি (!c->serializer->embed (name_id))
        return_trace (মিছা);
    }

    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক axis_গুনতি) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_গুনতি));
  }

  রক্ষিত:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  UnsizedArrayOf<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  সার্ৱজনিক:
  DEFINE_SIZE_UNBOUNDED (4);
};

সংরচনা AxisRecord
{
  পূর্ণাঙ্ক cmp (hb_tag_t key) ধ্রুৱ { ঘূৰাই axisTag.cmp (key); }

  গণনা
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  শূন্য get_axis_deprecated (hb_ot_var_axis_t *info) ধ্রুৱ
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_মান, info->default_মান, info->max_মান);
  }
#endif

  শূন্য get_axis_info (unsigned axis_index, hb_ot_var_axis_info_t *info) ধ্রুৱ
  {
    info->axis_index = axis_index;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (unsigned পূর্ণাঙ্ক) flags;
    get_coordinates (info->min_মান, info->default_মান, info->max_মান);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () ধ্রুৱ { ঘূৰাই axisTag; }

  তার্কিক is_hidden () ধ্রুৱ { ঘূৰাই flags & AXIS_FLAG_HIDDEN; }

  ভিন্ন normalize_axis_মান (ভিন্ন v) ধ্রুৱ
  {
    ভিন্ন min_মান, default_মান, max_মান;
    get_coordinates (min_মান, default_মান, max_মান);

    v = hb_clamp (v, min_মান, max_মান);

    যদি (v == default_মান)
      ঘূৰাই 0;
    অন্যথা যদি (v < default_মান)
      ঘূৰাই (v - default_মান) / (default_মান - min_মান);
    অন্যথা
      ঘূৰাই (v - default_মান) / (max_মান - default_মান);
  }

  hb_ot_name_id_t get_name_id () ধ্রুৱ { ঘূৰাই axisNameID; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  শূন্য get_coordinates (ভিন্ন &min, ভিন্ন &default_, ভিন্ন &max) ধ্রুৱ
  {
    default_ = default_মান_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_মান_2.to_float ());
    max = hb_max (default_, max_মান_2.to_float ());
  }

  ভিন্ন get_default () ধ্রুৱ
  {
    ঘূৰাই default_মান_2.to_float ();
  }

  TripleDistances get_triple_distances () ধ্রুৱ
  {
    ভিন্ন min, default_, max;
    get_coordinates (min, default_, max);
    ঘূৰাই TripleDistances (min, default_, max);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    যদি (user_axes_location.has (axisTag, &axis_limit))
    {
      যদি (c->plan->has_avar2 && axis_limit->is_point ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        out->min_মান_2.set_float (axis_limit->মধ্য);
        out->default_মান_2.set_float (axis_limit->মধ্য);
        out->max_মান_2.set_float (axis_limit->মধ্য);
        out->flags = out->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      অন্যথা
      {
        out->min_মান_2.set_float (axis_limit->minimum);
        out->default_মান_2.set_float (axis_limit->মধ্য);
        out->max_মান_2.set_float (axis_limit->maximum);
      }
    }
    return_trace (সঁচা);
  }

  সার্ৱজনিক:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  রক্ষিত:
  F16DOT16	min_মান_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_মান_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_মান_2;	/* The maximum coordinate value for the axis. */
  সার্ৱজনিক:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (20);
};

সংরচনা fvar
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই version.to_int (); }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (এতত্) &&
		  hb_barrier () &&
		  axisSize == 20 && /* Assumed in our code. */
		  instanceSize >= axis_গুনতি_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_গুনতি, instanceSize));
  }

  unsigned পূর্ণাঙ্ক get_axis_গুনতি () ধ্রুৱ { ঘূৰাই axis_গুনতি_2; }

#ifndef HB_DISABLE_DEPRECATED
  unsigned পূর্ণাঙ্ক get_axes_deprecated (unsigned পূর্ণাঙ্ক      start_offset,
				    unsigned পূর্ণাঙ্ক     *axes_গুনতি /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) ধ্রুৱ
  {
    যদি (axes_গুনতি && axes_array)
    {
      hb_array_t<ধ্রুৱ AxisRecord> arr = get_axes ().sub_array (start_offset, axes_গুনতি);
      বাবে (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    ঘূৰাই axis_গুনতি_2;
  }
#endif

  unsigned পূর্ণাঙ্ক get_axis_infos (unsigned পূর্ণাঙ্ক           start_offset,
			       unsigned পূর্ণাঙ্ক          *axes_গুনতি /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) ধ্রুৱ
  {
    যদি (axes_গুনতি && axes_array)
    {
      hb_array_t<ধ্রুৱ AxisRecord> arr = get_axes ().sub_array (start_offset, axes_গুনতি);
      বাবে (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    ঘূৰাই axis_গুনতি_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  তার্কিক
  find_axis_deprecated (hb_tag_t tag, unsigned *axis_index, hb_ot_var_axis_t *info) ধ্রুৱ
  {
    unsigned i;
    যদি (!axis_index) axis_index = &i;
    *axis_index = HB_OT_VAR_NO_AXIS_INDEX;
    auto axes = get_axes ();
    ঘূৰাই axes.lfind (tag, axis_index) && ((শূন্য) axes[*axis_index].get_axis_deprecated (info), সঁচা);
  }
#endif
  তার্কিক
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) ধ্রুৱ
  {
    unsigned i;
    auto axes = get_axes ();
    ঘূৰাই axes.lfind (tag, &i) && ((শূন্য) axes[i].get_axis_info (i, info), সঁচা);
  }

  ভিন্ন normalize_axis_মান (unsigned পূর্ণাঙ্ক axis_index, ভিন্ন v) ধ্রুৱ
  { ঘূৰাই get_axes ()[axis_index].normalize_axis_মান (v); }

  unsigned পূর্ণাঙ্ক get_instance_গুনতি () ধ্রুৱ { ঘূৰাই instance_গুনতি; }

  hb_ot_name_id_t get_instance_subfamily_name_id (unsigned পূর্ণাঙ্ক instance_index) ধ্রুৱ
  {
    ধ্রুৱ InstanceRecord *instance = get_instance (instance_index);
    যদি (unlikely (!instance)) ঘূৰাই HB_OT_NAME_ID_INVALID;
    ঘূৰাই instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (unsigned পূর্ণাঙ্ক instance_index) ধ্রুৱ
  {
    ধ্রুৱ InstanceRecord *instance = get_instance (instance_index);
    যদি (unlikely (!instance)) ঘূৰাই HB_OT_NAME_ID_INVALID;
    যদি (instanceSize >= axis_গুনতি_2 * 4 + 6)
      ঘূৰাই StructAfter<NameID> (instance->get_coordinates (axis_গুনতি_2));
    ঘূৰাই HB_OT_NAME_ID_INVALID;
  }

  unsigned পূর্ণাঙ্ক get_instance_coords (unsigned পূর্ণাঙ্ক  instance_index,
				    unsigned পূর্ণাঙ্ক *coords_length, /* IN/OUT */
				    ভিন্ন        *coords         /* OUT */) ধ্রুৱ
  {
    ধ্রুৱ InstanceRecord *instance = get_instance (instance_index);
    যদি (unlikely (!instance))
    {
      যদি (coords_length)
	*coords_length = 0;
      ঘূৰাই 0;
    }

    যদি (coords_length && *coords_length && coords)
    {
      hb_array_t<ধ্রুৱ F16DOT16> instanceCoords = instance->get_coordinates (axis_গুনতি_2)
							 .sub_array (0, coords_length);
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < instanceCoords.length; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    ঘূৰাই axis_গুনতি_2;
  }

  শূন্য collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_index_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) ধ্রুৱ
  {
    যদি (!has_data ()) ঘূৰাই;

    auto axis_records = get_axes ();
    বাবে (unsigned i = 0 ; i < (unsigned)axis_গুনতি_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      যদি (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        অনুৱর্ত;

      nameids->add (axis_records[i].get_name_id ());
    }

    বাবে (unsigned i = 0 ; i < (unsigned)instance_গুনতি; i++)
    {
      ধ্রুৱ InstanceRecord *instance = get_instance (i);

      যদি (!instance->keep_instance (axis_গুনতি_2, axes_old_index_tag_map, user_axes_location))
        অনুৱর্ত;

      nameids->add (instance->subfamilyNameID);

      যদি (instanceSize >= axis_গুনতি_2 * 4 + 6)
      {
        unsigned post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_গুনতি_2));
        যদি (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    unsigned retained_axis_গুনতি = c->plan->axes_index_map.get_population ();
    যদি (!retained_axis_গুনতি) //all axes are pinned
      return_trace (মিছা);

    fvar *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (!c->serializer->check_assign (out->axis_গুনতি_2, retained_axis_গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিছা);

    তার্কিক has_postscript_nameid = মিছা;
    যদি (instanceSize >= axis_গুনতি_2 * 4 + 6)
      has_postscript_nameid = সঁচা;

    যদি (!c->serializer->check_assign (out->instanceSize, retained_axis_গুনতি * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিছা);

    auto axes_records = get_axes ();
    বাবে (unsigned i = 0 ; i < (unsigned)axis_গুনতি_2; i++)
    {
      যদি (!c->plan->axes_index_map.has (i)) অনুৱর্ত;
      যদি (unlikely (!axes_records[i].subset (c)))
        return_trace (মিছা);
    }

    যদি (!c->serializer->check_assign (out->firstAxis, get_size (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিছা);

    unsigned অঙ্ক_retained_instances = 0;
    বাবে (unsigned i = 0 ; i < (unsigned)instance_গুনতি; i++)
    {
      ধ্রুৱ InstanceRecord *instance = get_instance (i);
      auto snap = c->serializer->snapshot ();
      যদি (!instance->subset (c, axis_গুনতি_2, has_postscript_nameid))
        c->serializer->revert (snap);
      অন্যথা
        অঙ্ক_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_গুনতি, অঙ্ক_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  সার্ৱজনিক:
  hb_array_t<ধ্রুৱ AxisRecord> get_axes () ধ্রুৱ
  { ঘূৰাই hb_array (&(এতত্+firstAxis), axis_গুনতি_2); }

  ধ্রুৱ InstanceRecord *get_instance (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  {
    যদি (unlikely (i >= instance_গুনতি)) ঘূৰাই nullptr;
   ঘূৰাই &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instanceSize);
  }

  রক্ষিত:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_গুনতি_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axisSize;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_গুনতি;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instanceSize;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
