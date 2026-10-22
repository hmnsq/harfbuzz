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

#अन्तर्भाव "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#define HB_OT_TAG_fvar HB_TAG('f','v','a','r')


नामाकाश OT {

स्थिर तार्किक axis_coord_pinned_or_within_axis_range (ध्रुव hb_array_t<ध्रुव F16DOT16> coords,
                                                    unsigned axis_index,
                                                    Triple axis_limit)
{
  भिन्न axis_coord = coords[axis_index].to_float ();
  यदि (axis_limit.is_point ())
  {
    यदि (axis_limit.minimum != axis_coord)
      प्रत्यागम असत्य;
  }
  अन्यथा
  {
    यदि (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      प्रत्यागम असत्य;
  }
  प्रत्यागम सत्य;
}

संरचना InstanceRecord
{
  friend संरचना fvar;

  hb_array_t<ध्रुव F16DOT16> get_coordinates (unsigned पूर्णाङ्क axis_गिनती) ध्रुव
  { प्रत्यागम coordinatesZ.as_array (axis_गिनती); }

  तार्किक keep_instance (unsigned axis_गिनती,
                      ध्रुव hb_map_t *axes_index_tag_map,
                      ध्रुव hb_hashmap_t<hb_tag_t, Triple> *axes_location) ध्रुव
  {
    यदि (axes_location->is_empty ()) प्रत्यागम सत्य;
    ध्रुव hb_array_t<ध्रुव F16DOT16> coords = get_coordinates (axis_गिनती);
    प्रति (unsigned i = 0 ; i < axis_गिनती; i++)
    {
      uint32_t *axis_tag;
      यदि (!axes_index_tag_map->has (i, &axis_tag))
        प्रत्यागम असत्य;
      यदि (!axes_location->has (*axis_tag))
        अनुवर्त;

      Triple axis_limit = axes_location->get (*axis_tag);
      यदि (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        प्रत्यागम असत्य;
    }
    प्रत्यागम सत्य;
  }

  तार्किक subset (hb_subset_context_t *c,
               unsigned axis_गिनती,
               तार्किक has_postscript_nameid) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    यदि (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (असत्य);
    यदि (unlikely (!c->serializer->embed (flags))) return_trace (असत्य);

    ध्रुव hb_array_t<ध्रुव F16DOT16> coords = get_coordinates (axis_गिनती);
    ध्रुव hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    प्रति (unsigned i = 0 ; i < axis_गिनती; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      यदि (!c->plan->axes_old_index_tag_map.has (i, &axis_tag)) return_trace (असत्य);
      यदि (axes_location->has (*axis_tag, &axis_limit))
      {
        यदि (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (असत्य);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        यदि (axis_limit->is_point () && !c->plan->axes_index_map.has (i))
          अनुवर्त;
      }

      यदि (!c->serializer->embed (coords[i]))
        return_trace (असत्य);
    }

    यदि (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      यदि (!c->serializer->embed (name_id))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क axis_गिनती) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_गिनती));
  }

  रक्षित:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  UnsizedArrayOf<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  सार्वजनिक:
  DEFINE_SIZE_UNBOUNDED (4);
};

संरचना AxisRecord
{
  पूर्णाङ्क cmp (hb_tag_t key) ध्रुव { प्रत्यागम axisTag.cmp (key); }

  गणना
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  शून्य get_axis_deprecated (hb_ot_var_axis_t *info) ध्रुव
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_मूल्य, info->default_मूल्य, info->max_मूल्य);
  }
#endif

  शून्य get_axis_info (unsigned axis_index, hb_ot_var_axis_info_t *info) ध्रुव
  {
    info->axis_index = axis_index;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (unsigned पूर्णाङ्क) flags;
    get_coordinates (info->min_मूल्य, info->default_मूल्य, info->max_मूल्य);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () ध्रुव { प्रत्यागम axisTag; }

  तार्किक is_hidden () ध्रुव { प्रत्यागम flags & AXIS_FLAG_HIDDEN; }

  भिन्न normalize_axis_मूल्य (भिन्न v) ध्रुव
  {
    भिन्न min_मूल्य, default_मूल्य, max_मूल्य;
    get_coordinates (min_मूल्य, default_मूल्य, max_मूल्य);

    v = hb_clamp (v, min_मूल्य, max_मूल्य);

    यदि (v == default_मूल्य)
      प्रत्यागम 0;
    अन्यथा यदि (v < default_मूल्य)
      प्रत्यागम (v - default_मूल्य) / (default_मूल्य - min_मूल्य);
    अन्यथा
      प्रत्यागम (v - default_मूल्य) / (max_मूल्य - default_मूल्य);
  }

  hb_ot_name_id_t get_name_id () ध्रुव { प्रत्यागम axisNameID; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य get_coordinates (भिन्न &min, भिन्न &default_, भिन्न &max) ध्रुव
  {
    default_ = default_मूल्य_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_मूल्य_2.to_float ());
    max = hb_max (default_, max_मूल्य_2.to_float ());
  }

  भिन्न get_default () ध्रुव
  {
    प्रत्यागम default_मूल्य_2.to_float ();
  }

  TripleDistances get_triple_distances () ध्रुव
  {
    भिन्न min, default_, max;
    get_coordinates (min, default_, max);
    प्रत्यागम TripleDistances (min, default_, max);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    ध्रुव hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    यदि (user_axes_location.has (axisTag, &axis_limit))
    {
      यदि (c->plan->has_avar2 && axis_limit->is_point ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        out->min_मूल्य_2.set_float (axis_limit->मध्य);
        out->default_मूल्य_2.set_float (axis_limit->मध्य);
        out->max_मूल्य_2.set_float (axis_limit->मध्य);
        out->flags = out->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      अन्यथा
      {
        out->min_मूल्य_2.set_float (axis_limit->minimum);
        out->default_मूल्य_2.set_float (axis_limit->मध्य);
        out->max_मूल्य_2.set_float (axis_limit->maximum);
      }
    }
    return_trace (सत्य);
  }

  सार्वजनिक:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  रक्षित:
  F16DOT16	min_मूल्य_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_मूल्य_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_मूल्य_2;	/* The maximum coordinate value for the axis. */
  सार्वजनिक:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (20);
};

संरचना fvar
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  तार्किक has_data () ध्रुव { प्रत्यागम version.to_int (); }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (एतत्) &&
		  hb_barrier () &&
		  axisSize == 20 && /* Assumed in our code. */
		  instanceSize >= axis_गिनती_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_गिनती, instanceSize));
  }

  unsigned पूर्णाङ्क get_axis_गिनती () ध्रुव { प्रत्यागम axis_गिनती_2; }

#ifndef HB_DISABLE_DEPRECATED
  unsigned पूर्णाङ्क get_axes_deprecated (unsigned पूर्णाङ्क      start_offset,
				    unsigned पूर्णाङ्क     *axes_गिनती /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) ध्रुव
  {
    यदि (axes_गिनती && axes_array)
    {
      hb_array_t<ध्रुव AxisRecord> arr = get_axes ().sub_array (start_offset, axes_गिनती);
      प्रति (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    प्रत्यागम axis_गिनती_2;
  }
#endif

  unsigned पूर्णाङ्क get_axis_infos (unsigned पूर्णाङ्क           start_offset,
			       unsigned पूर्णाङ्क          *axes_गिनती /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) ध्रुव
  {
    यदि (axes_गिनती && axes_array)
    {
      hb_array_t<ध्रुव AxisRecord> arr = get_axes ().sub_array (start_offset, axes_गिनती);
      प्रति (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    प्रत्यागम axis_गिनती_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  तार्किक
  find_axis_deprecated (hb_tag_t tag, unsigned *axis_index, hb_ot_var_axis_t *info) ध्रुव
  {
    unsigned i;
    यदि (!axis_index) axis_index = &i;
    *axis_index = HB_OT_VAR_NO_AXIS_INDEX;
    auto axes = get_axes ();
    प्रत्यागम axes.lfind (tag, axis_index) && ((शून्य) axes[*axis_index].get_axis_deprecated (info), सत्य);
  }
#endif
  तार्किक
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) ध्रुव
  {
    unsigned i;
    auto axes = get_axes ();
    प्रत्यागम axes.lfind (tag, &i) && ((शून्य) axes[i].get_axis_info (i, info), सत्य);
  }

  भिन्न normalize_axis_मूल्य (unsigned पूर्णाङ्क axis_index, भिन्न v) ध्रुव
  { प्रत्यागम get_axes ()[axis_index].normalize_axis_मूल्य (v); }

  unsigned पूर्णाङ्क get_instance_गिनती () ध्रुव { प्रत्यागम instance_गिनती; }

  hb_ot_name_id_t get_instance_subfamily_name_id (unsigned पूर्णाङ्क instance_index) ध्रुव
  {
    ध्रुव InstanceRecord *instance = get_instance (instance_index);
    यदि (unlikely (!instance)) प्रत्यागम HB_OT_NAME_ID_INVALID;
    प्रत्यागम instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (unsigned पूर्णाङ्क instance_index) ध्रुव
  {
    ध्रुव InstanceRecord *instance = get_instance (instance_index);
    यदि (unlikely (!instance)) प्रत्यागम HB_OT_NAME_ID_INVALID;
    यदि (instanceSize >= axis_गिनती_2 * 4 + 6)
      प्रत्यागम StructAfter<NameID> (instance->get_coordinates (axis_गिनती_2));
    प्रत्यागम HB_OT_NAME_ID_INVALID;
  }

  unsigned पूर्णाङ्क get_instance_coords (unsigned पूर्णाङ्क  instance_index,
				    unsigned पूर्णाङ्क *coords_length, /* IN/OUT */
				    भिन्न        *coords         /* OUT */) ध्रुव
  {
    ध्रुव InstanceRecord *instance = get_instance (instance_index);
    यदि (unlikely (!instance))
    {
      यदि (coords_length)
	*coords_length = 0;
      प्रत्यागम 0;
    }

    यदि (coords_length && *coords_length && coords)
    {
      hb_array_t<ध्रुव F16DOT16> instanceCoords = instance->get_coordinates (axis_गिनती_2)
							 .sub_array (0, coords_length);
      प्रति (unsigned पूर्णाङ्क i = 0; i < instanceCoords.length; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    प्रत्यागम axis_गिनती_2;
  }

  शून्य collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_index_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) ध्रुव
  {
    यदि (!has_data ()) प्रत्यागम;

    auto axis_records = get_axes ();
    प्रति (unsigned i = 0 ; i < (unsigned)axis_गिनती_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      यदि (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        अनुवर्त;

      nameids->add (axis_records[i].get_name_id ());
    }

    प्रति (unsigned i = 0 ; i < (unsigned)instance_गिनती; i++)
    {
      ध्रुव InstanceRecord *instance = get_instance (i);

      यदि (!instance->keep_instance (axis_गिनती_2, axes_old_index_tag_map, user_axes_location))
        अनुवर्त;

      nameids->add (instance->subfamilyNameID);

      यदि (instanceSize >= axis_गिनती_2 * 4 + 6)
      {
        unsigned post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_गिनती_2));
        यदि (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    unsigned retained_axis_गिनती = c->plan->axes_index_map.get_population ();
    यदि (!retained_axis_गिनती) //all axes are pinned
      return_trace (असत्य);

    fvar *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    यदि (!c->serializer->check_assign (out->axis_गिनती_2, retained_axis_गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    तार्किक has_postscript_nameid = असत्य;
    यदि (instanceSize >= axis_गिनती_2 * 4 + 6)
      has_postscript_nameid = सत्य;

    यदि (!c->serializer->check_assign (out->instanceSize, retained_axis_गिनती * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    auto axes_records = get_axes ();
    प्रति (unsigned i = 0 ; i < (unsigned)axis_गिनती_2; i++)
    {
      यदि (!c->plan->axes_index_map.has (i)) अनुवर्त;
      यदि (unlikely (!axes_records[i].subset (c)))
        return_trace (असत्य);
    }

    यदि (!c->serializer->check_assign (out->firstAxis, get_size (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    unsigned अंक_retained_instances = 0;
    प्रति (unsigned i = 0 ; i < (unsigned)instance_गिनती; i++)
    {
      ध्रुव InstanceRecord *instance = get_instance (i);
      auto snap = c->serializer->snapshot ();
      यदि (!instance->subset (c, axis_गिनती_2, has_postscript_nameid))
        c->serializer->revert (snap);
      अन्यथा
        अंक_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_गिनती, अंक_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  सार्वजनिक:
  hb_array_t<ध्रुव AxisRecord> get_axes () ध्रुव
  { प्रत्यागम hb_array (&(एतत्+firstAxis), axis_गिनती_2); }

  ध्रुव InstanceRecord *get_instance (unsigned पूर्णाङ्क i) ध्रुव
  {
    यदि (unlikely (i >= instance_गिनती)) प्रत्यागम nullptr;
   प्रत्यागम &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instanceSize);
  }

  रक्षित:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_गिनती_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axisSize;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_गिनती;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instanceSize;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
