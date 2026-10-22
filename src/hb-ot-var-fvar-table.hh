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

#അന്തര്ഭാവ "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#define HB_OT_TAG_fvar HB_TAG('f','v','a','r')


നാമാകാശ OT {

സ്ഥിതം തർക്കം axis_coord_pinned_or_within_axis_range (സ്ഥിരം hb_array_t<സ്ഥിരം F16DOT16> coords,
                                                    unsigned axis_index,
                                                    Triple axis_limit)
{
  ദശാംശം axis_coord = coords[axis_index].to_float ();
  എങ്കിൽ (axis_limit.is_point ())
  {
    എങ്കിൽ (axis_limit.minimum != axis_coord)
      തിരികെ അസത്യം;
  }
  അല്ലെങ്കിൽ
  {
    എങ്കിൽ (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      തിരികെ അസത്യം;
  }
  തിരികെ സത്യം;
}

ഘടന InstanceRecord
{
  friend ഘടന fvar;

  hb_array_t<സ്ഥിരം F16DOT16> get_coordinates (unsigned പൂർണ്ണസംഖ്യ axis_എണ്ണം) സ്ഥിരം
  { തിരികെ coordinatesZ.as_array (axis_എണ്ണം); }

  തർക്കം keep_instance (unsigned axis_എണ്ണം,
                      സ്ഥിരം hb_map_t *axes_index_tag_map,
                      സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> *axes_location) സ്ഥിരം
  {
    എങ്കിൽ (axes_location->is_empty ()) തിരികെ സത്യം;
    സ്ഥിരം hb_array_t<സ്ഥിരം F16DOT16> coords = get_coordinates (axis_എണ്ണം);
    വേണ്ടി (unsigned i = 0 ; i < axis_എണ്ണം; i++)
    {
      uint32_t *axis_tag;
      എങ്കിൽ (!axes_index_tag_map->has (i, &axis_tag))
        തിരികെ അസത്യം;
      എങ്കിൽ (!axes_location->has (*axis_tag))
        തുടരുക;

      Triple axis_limit = axes_location->get (*axis_tag);
      എങ്കിൽ (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
  }

  തർക്കം subset (hb_subset_context_t *c,
               unsigned axis_എണ്ണം,
               തർക്കം has_postscript_nameid) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->serializer->embed (flags))) return_trace (അസത്യം);

    സ്ഥിരം hb_array_t<സ്ഥിരം F16DOT16> coords = get_coordinates (axis_എണ്ണം);
    സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    വേണ്ടി (unsigned i = 0 ; i < axis_എണ്ണം; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      എങ്കിൽ (!c->plan->axes_old_index_tag_map.has (i, &axis_tag)) return_trace (അസത്യം);
      എങ്കിൽ (axes_location->has (*axis_tag, &axis_limit))
      {
        എങ്കിൽ (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (അസത്യം);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        എങ്കിൽ (axis_limit->is_point () && !c->plan->axes_index_map.has (i))
          തുടരുക;
      }

      എങ്കിൽ (!c->serializer->embed (coords[i]))
        return_trace (അസത്യം);
    }

    എങ്കിൽ (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      എങ്കിൽ (!c->serializer->embed (name_id))
        return_trace (അസത്യം);
    }

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ axis_എണ്ണം) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_എണ്ണം));
  }

  സംരക്ഷിത:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  UnsizedArrayOf<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  പൊതു:
  DEFINE_SIZE_UNBOUNDED (4);
};

ഘടന AxisRecord
{
  പൂർണ്ണസംഖ്യ cmp (hb_tag_t key) സ്ഥിരം { തിരികെ axisTag.cmp (key); }

  എണ്ണൽ
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  ശൂന്യം get_axis_deprecated (hb_ot_var_axis_t *info) സ്ഥിരം
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_മൂല്യം, info->default_മൂല്യം, info->max_മൂല്യം);
  }
#endif

  ശൂന്യം get_axis_info (unsigned axis_index, hb_ot_var_axis_info_t *info) സ്ഥിരം
  {
    info->axis_index = axis_index;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (unsigned പൂർണ്ണസംഖ്യ) flags;
    get_coordinates (info->min_മൂല്യം, info->default_മൂല്യം, info->max_മൂല്യം);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () സ്ഥിരം { തിരികെ axisTag; }

  തർക്കം is_hidden () സ്ഥിരം { തിരികെ flags & AXIS_FLAG_HIDDEN; }

  ദശാംശം normalize_axis_മൂല്യം (ദശാംശം v) സ്ഥിരം
  {
    ദശാംശം min_മൂല്യം, default_മൂല്യം, max_മൂല്യം;
    get_coordinates (min_മൂല്യം, default_മൂല്യം, max_മൂല്യം);

    v = hb_clamp (v, min_മൂല്യം, max_മൂല്യം);

    എങ്കിൽ (v == default_മൂല്യം)
      തിരികെ 0;
    അല്ലെങ്കിൽ എങ്കിൽ (v < default_മൂല്യം)
      തിരികെ (v - default_മൂല്യം) / (default_മൂല്യം - min_മൂല്യം);
    അല്ലെങ്കിൽ
      തിരികെ (v - default_മൂല്യം) / (max_മൂല്യം - default_മൂല്യം);
  }

  hb_ot_name_id_t get_name_id () സ്ഥിരം { തിരികെ axisNameID; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  ശൂന്യം get_coordinates (ദശാംശം &min, ദശാംശം &default_, ദശാംശം &max) സ്ഥിരം
  {
    default_ = default_മൂല്യം_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_മൂല്യം_2.to_float ());
    max = hb_max (default_, max_മൂല്യം_2.to_float ());
  }

  ദശാംശം get_default () സ്ഥിരം
  {
    തിരികെ default_മൂല്യം_2.to_float ();
  }

  TripleDistances get_triple_distances () സ്ഥിരം
  {
    ദശാംശം min, default_, max;
    get_coordinates (min, default_, max);
    തിരികെ TripleDistances (min, default_, max);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    എങ്കിൽ (user_axes_location.has (axisTag, &axis_limit))
    {
      എങ്കിൽ (c->plan->has_avar2 && axis_limit->is_point ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        out->min_മൂല്യം_2.set_float (axis_limit->നടുവ്);
        out->default_മൂല്യം_2.set_float (axis_limit->നടുവ്);
        out->max_മൂല്യം_2.set_float (axis_limit->നടുവ്);
        out->flags = out->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      അല്ലെങ്കിൽ
      {
        out->min_മൂല്യം_2.set_float (axis_limit->minimum);
        out->default_മൂല്യം_2.set_float (axis_limit->നടുവ്);
        out->max_മൂല്യം_2.set_float (axis_limit->maximum);
      }
    }
    return_trace (സത്യം);
  }

  പൊതു:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  സംരക്ഷിത:
  F16DOT16	min_മൂല്യം_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_മൂല്യം_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_മൂല്യം_2;	/* The maximum coordinate value for the axis. */
  പൊതു:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  പൊതു:
  DEFINE_SIZE_STATIC (20);
};

ഘടന fvar
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int (); }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  axisSize == 20 && /* Assumed in our code. */
		  instanceSize >= axis_എണ്ണം_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_എണ്ണം, instanceSize));
  }

  unsigned പൂർണ്ണസംഖ്യ get_axis_എണ്ണം () സ്ഥിരം { തിരികെ axis_എണ്ണം_2; }

#ifndef HB_DISABLE_DEPRECATED
  unsigned പൂർണ്ണസംഖ്യ get_axes_deprecated (unsigned പൂർണ്ണസംഖ്യ      start_offset,
				    unsigned പൂർണ്ണസംഖ്യ     *axes_എണ്ണം /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (axes_എണ്ണം && axes_array)
    {
      hb_array_t<സ്ഥിരം AxisRecord> arr = get_axes ().sub_array (start_offset, axes_എണ്ണം);
      വേണ്ടി (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    തിരികെ axis_എണ്ണം_2;
  }
#endif

  unsigned പൂർണ്ണസംഖ്യ get_axis_infos (unsigned പൂർണ്ണസംഖ്യ           start_offset,
			       unsigned പൂർണ്ണസംഖ്യ          *axes_എണ്ണം /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (axes_എണ്ണം && axes_array)
    {
      hb_array_t<സ്ഥിരം AxisRecord> arr = get_axes ().sub_array (start_offset, axes_എണ്ണം);
      വേണ്ടി (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    തിരികെ axis_എണ്ണം_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  തർക്കം
  find_axis_deprecated (hb_tag_t tag, unsigned *axis_index, hb_ot_var_axis_t *info) സ്ഥിരം
  {
    unsigned i;
    എങ്കിൽ (!axis_index) axis_index = &i;
    *axis_index = HB_OT_VAR_NO_AXIS_INDEX;
    auto axes = get_axes ();
    തിരികെ axes.lfind (tag, axis_index) && ((ശൂന്യം) axes[*axis_index].get_axis_deprecated (info), സത്യം);
  }
#endif
  തർക്കം
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) സ്ഥിരം
  {
    unsigned i;
    auto axes = get_axes ();
    തിരികെ axes.lfind (tag, &i) && ((ശൂന്യം) axes[i].get_axis_info (i, info), സത്യം);
  }

  ദശാംശം normalize_axis_മൂല്യം (unsigned പൂർണ്ണസംഖ്യ axis_index, ദശാംശം v) സ്ഥിരം
  { തിരികെ get_axes ()[axis_index].normalize_axis_മൂല്യം (v); }

  unsigned പൂർണ്ണസംഖ്യ get_instance_എണ്ണം () സ്ഥിരം { തിരികെ instance_എണ്ണം; }

  hb_ot_name_id_t get_instance_subfamily_name_id (unsigned പൂർണ്ണസംഖ്യ instance_index) സ്ഥിരം
  {
    സ്ഥിരം InstanceRecord *instance = get_instance (instance_index);
    എങ്കിൽ (unlikely (!instance)) തിരികെ HB_OT_NAME_ID_INVALID;
    തിരികെ instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (unsigned പൂർണ്ണസംഖ്യ instance_index) സ്ഥിരം
  {
    സ്ഥിരം InstanceRecord *instance = get_instance (instance_index);
    എങ്കിൽ (unlikely (!instance)) തിരികെ HB_OT_NAME_ID_INVALID;
    എങ്കിൽ (instanceSize >= axis_എണ്ണം_2 * 4 + 6)
      തിരികെ StructAfter<NameID> (instance->get_coordinates (axis_എണ്ണം_2));
    തിരികെ HB_OT_NAME_ID_INVALID;
  }

  unsigned പൂർണ്ണസംഖ്യ get_instance_coords (unsigned പൂർണ്ണസംഖ്യ  instance_index,
				    unsigned പൂർണ്ണസംഖ്യ *coords_length, /* IN/OUT */
				    ദശാംശം        *coords         /* OUT */) സ്ഥിരം
  {
    സ്ഥിരം InstanceRecord *instance = get_instance (instance_index);
    എങ്കിൽ (unlikely (!instance))
    {
      എങ്കിൽ (coords_length)
	*coords_length = 0;
      തിരികെ 0;
    }

    എങ്കിൽ (coords_length && *coords_length && coords)
    {
      hb_array_t<സ്ഥിരം F16DOT16> instanceCoords = instance->get_coordinates (axis_എണ്ണം_2)
							 .sub_array (0, coords_length);
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < instanceCoords.length; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    തിരികെ axis_എണ്ണം_2;
  }

  ശൂന്യം collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_index_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) സ്ഥിരം
  {
    എങ്കിൽ (!has_data ()) തിരികെ;

    auto axis_records = get_axes ();
    വേണ്ടി (unsigned i = 0 ; i < (unsigned)axis_എണ്ണം_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      എങ്കിൽ (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        തുടരുക;

      nameids->add (axis_records[i].get_name_id ());
    }

    വേണ്ടി (unsigned i = 0 ; i < (unsigned)instance_എണ്ണം; i++)
    {
      സ്ഥിരം InstanceRecord *instance = get_instance (i);

      എങ്കിൽ (!instance->keep_instance (axis_എണ്ണം_2, axes_old_index_tag_map, user_axes_location))
        തുടരുക;

      nameids->add (instance->subfamilyNameID);

      എങ്കിൽ (instanceSize >= axis_എണ്ണം_2 * 4 + 6)
      {
        unsigned post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_എണ്ണം_2));
        എങ്കിൽ (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    unsigned retained_axis_എണ്ണം = c->plan->axes_index_map.get_population ();
    എങ്കിൽ (!retained_axis_എണ്ണം) //all axes are pinned
      return_trace (അസത്യം);

    fvar *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    എങ്കിൽ (!c->serializer->check_assign (out->axis_എണ്ണം_2, retained_axis_എണ്ണം, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (അസത്യം);

    തർക്കം has_postscript_nameid = അസത്യം;
    എങ്കിൽ (instanceSize >= axis_എണ്ണം_2 * 4 + 6)
      has_postscript_nameid = സത്യം;

    എങ്കിൽ (!c->serializer->check_assign (out->instanceSize, retained_axis_എണ്ണം * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (അസത്യം);

    auto axes_records = get_axes ();
    വേണ്ടി (unsigned i = 0 ; i < (unsigned)axis_എണ്ണം_2; i++)
    {
      എങ്കിൽ (!c->plan->axes_index_map.has (i)) തുടരുക;
      എങ്കിൽ (unlikely (!axes_records[i].subset (c)))
        return_trace (അസത്യം);
    }

    എങ്കിൽ (!c->serializer->check_assign (out->firstAxis, get_size (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (അസത്യം);

    unsigned അക്കം_retained_instances = 0;
    വേണ്ടി (unsigned i = 0 ; i < (unsigned)instance_എണ്ണം; i++)
    {
      സ്ഥിരം InstanceRecord *instance = get_instance (i);
      auto snap = c->serializer->snapshot ();
      എങ്കിൽ (!instance->subset (c, axis_എണ്ണം_2, has_postscript_nameid))
        c->serializer->revert (snap);
      അല്ലെങ്കിൽ
        അക്കം_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_എണ്ണം, അക്കം_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  പൊതു:
  hb_array_t<സ്ഥിരം AxisRecord> get_axes () സ്ഥിരം
  { തിരികെ hb_array (&(ഇത്+firstAxis), axis_എണ്ണം_2); }

  സ്ഥിരം InstanceRecord *get_instance (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (i >= instance_എണ്ണം)) തിരികെ nullptr;
   തിരികെ &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instanceSize);
  }

  സംരക്ഷിത:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_എണ്ണം_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axisSize;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_എണ്ണം;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instanceSize;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  പൊതു:
  DEFINE_SIZE_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
