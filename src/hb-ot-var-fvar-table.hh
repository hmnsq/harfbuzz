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

स्थिर तार्किक axis_coord_pinned_or_within_axis_range (स्थिरांक hb_array_t<स्थिरांक F16DOT16> coords,
                                                    अचिन्हित axis_index,
                                                    Triple axis_limit)
{
  दशांश axis_coord = coords[axis_index].to_float ();
  जर (axis_limit.is_point ())
  {
    जर (axis_limit.minimum != axis_coord)
      परत असत्य;
  }
  अन्यथा
  {
    जर (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      परत असत्य;
  }
  परत सत्य;
}

संरचना InstanceRecord
{
  मित्र संरचना fvar;

  hb_array_t<स्थिरांक F16DOT16> get_coordinates (अचिन्हित पूर्णांक axis_मोजणी) स्थिरांक
  { परत coordinatesZ.as_array (axis_मोजणी); }

  तार्किक keep_instance (अचिन्हित axis_मोजणी,
                      स्थिरांक hb_map_t *axes_index_tag_map,
                      स्थिरांक hb_hashmap_t<hb_tag_t, Triple> *axes_location) स्थिरांक
  {
    जर (axes_location->is_empty ()) परत सत्य;
    स्थिरांक hb_array_t<स्थिरांक F16DOT16> coords = get_coordinates (axis_मोजणी);
    साठी (अचिन्हित i = 0 ; i < axis_मोजणी; i++)
    {
      uint32_t *axis_tag;
      जर (!axes_index_tag_map->has (i, &axis_tag))
        परत असत्य;
      जर (!axes_location->has (*axis_tag))
        सुरूठेवा;

      Triple axis_limit = axes_location->get (*axis_tag);
      जर (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        परत असत्य;
    }
    परत सत्य;
  }

  तार्किक subset (hb_subset_context_t *c,
               अचिन्हित axis_मोजणी,
               तार्किक has_postscript_nameid) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (असत्य);
    जर (unlikely (!c->serializer->embed (flags))) return_trace (असत्य);

    स्थिरांक hb_array_t<स्थिरांक F16DOT16> coords = get_coordinates (axis_मोजणी);
    स्थिरांक hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    साठी (अचिन्हित i = 0 ; i < axis_मोजणी; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      जर (!c->plan->axes_old_index_tag_map.has (i, &axis_tag)) return_trace (असत्य);
      जर (axes_location->has (*axis_tag, &axis_limit))
      {
        जर (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (असत्य);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        जर (axis_limit->is_point () && !c->plan->axes_index_map.has (i))
          सुरूठेवा;
      }

      जर (!c->serializer->embed (coords[i]))
        return_trace (असत्य);
    }

    जर (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      जर (!c->serializer->embed (name_id))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक axis_मोजणी) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_मोजणी));
  }

  संरक्षित:
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
  पूर्णांक cmp (hb_tag_t key) स्थिरांक { परत axisTag.cmp (key); }

  गणना
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  शून्य get_axis_deprecated (hb_ot_var_axis_t *info) स्थिरांक
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_मूल्य, info->default_मूल्य, info->max_मूल्य);
  }
#endif

  शून्य get_axis_info (अचिन्हित axis_index, hb_ot_var_axis_info_t *info) स्थिरांक
  {
    info->axis_index = axis_index;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (अचिन्हित पूर्णांक) flags;
    get_coordinates (info->min_मूल्य, info->default_मूल्य, info->max_मूल्य);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () स्थिरांक { परत axisTag; }

  तार्किक is_hidden () स्थिरांक { परत flags & AXIS_FLAG_HIDDEN; }

  दशांश normalize_axis_मूल्य (दशांश v) स्थिरांक
  {
    दशांश min_मूल्य, default_मूल्य, max_मूल्य;
    get_coordinates (min_मूल्य, default_मूल्य, max_मूल्य);

    v = hb_clamp (v, min_मूल्य, max_मूल्य);

    जर (v == default_मूल्य)
      परत 0;
    अन्यथा जर (v < default_मूल्य)
      परत (v - default_मूल्य) / (default_मूल्य - min_मूल्य);
    अन्यथा
      परत (v - default_मूल्य) / (max_मूल्य - default_मूल्य);
  }

  hb_ot_name_id_t get_name_id () स्थिरांक { परत axisNameID; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  शून्य get_coordinates (दशांश &min, दशांश &default_, दशांश &max) स्थिरांक
  {
    default_ = default_मूल्य_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_मूल्य_2.to_float ());
    max = hb_max (default_, max_मूल्य_2.to_float ());
  }

  दशांश get_default () स्थिरांक
  {
    परत default_मूल्य_2.to_float ();
  }

  TripleDistances get_triple_distances () स्थिरांक
  {
    दशांश min, default_, max;
    get_coordinates (min, default_, max);
    परत TripleDistances (min, default_, max);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    स्थिरांक hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    जर (user_axes_location.has (axisTag, &axis_limit))
    {
      जर (c->plan->has_avar2 && axis_limit->is_point ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        out->min_मूल्य_2.set_float (axis_limit->मधला);
        out->default_मूल्य_2.set_float (axis_limit->मधला);
        out->max_मूल्य_2.set_float (axis_limit->मधला);
        out->flags = out->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      अन्यथा
      {
        out->min_मूल्य_2.set_float (axis_limit->minimum);
        out->default_मूल्य_2.set_float (axis_limit->मधला);
        out->max_मूल्य_2.set_float (axis_limit->maximum);
      }
    }
    return_trace (सत्य);
  }

  सार्वजनिक:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  संरक्षित:
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

  तार्किक has_data () स्थिरांक { परत version.to_int (); }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (हे) &&
		  hb_barrier () &&
		  axisSize == 20 && /* Assumed in our code. */
		  instanceSize >= axis_मोजणी_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_मोजणी, instanceSize));
  }

  अचिन्हित पूर्णांक get_axis_मोजणी () स्थिरांक { परत axis_मोजणी_2; }

#ifndef HB_DISABLE_DEPRECATED
  अचिन्हित पूर्णांक get_axes_deprecated (अचिन्हित पूर्णांक      start_offset,
				    अचिन्हित पूर्णांक     *axes_मोजणी /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) स्थिरांक
  {
    जर (axes_मोजणी && axes_array)
    {
      hb_array_t<स्थिरांक AxisRecord> arr = get_axes ().sub_array (start_offset, axes_मोजणी);
      साठी (अचिन्हित i = 0; i < arr.length; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    परत axis_मोजणी_2;
  }
#endif

  अचिन्हित पूर्णांक get_axis_infos (अचिन्हित पूर्णांक           start_offset,
			       अचिन्हित पूर्णांक          *axes_मोजणी /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) स्थिरांक
  {
    जर (axes_मोजणी && axes_array)
    {
      hb_array_t<स्थिरांक AxisRecord> arr = get_axes ().sub_array (start_offset, axes_मोजणी);
      साठी (अचिन्हित i = 0; i < arr.length; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    परत axis_मोजणी_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  तार्किक
  find_axis_deprecated (hb_tag_t tag, अचिन्हित *axis_index, hb_ot_var_axis_t *info) स्थिरांक
  {
    अचिन्हित i;
    जर (!axis_index) axis_index = &i;
    *axis_index = HB_OT_VAR_NO_AXIS_INDEX;
    स्वतः axes = get_axes ();
    परत axes.lfind (tag, axis_index) && ((शून्य) axes[*axis_index].get_axis_deprecated (info), सत्य);
  }
#endif
  तार्किक
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) स्थिरांक
  {
    अचिन्हित i;
    स्वतः axes = get_axes ();
    परत axes.lfind (tag, &i) && ((शून्य) axes[i].get_axis_info (i, info), सत्य);
  }

  दशांश normalize_axis_मूल्य (अचिन्हित पूर्णांक axis_index, दशांश v) स्थिरांक
  { परत get_axes ()[axis_index].normalize_axis_मूल्य (v); }

  अचिन्हित पूर्णांक get_instance_मोजणी () स्थिरांक { परत instance_मोजणी; }

  hb_ot_name_id_t get_instance_subfamily_name_id (अचिन्हित पूर्णांक instance_index) स्थिरांक
  {
    स्थिरांक InstanceRecord *instance = get_instance (instance_index);
    जर (unlikely (!instance)) परत HB_OT_NAME_ID_INVALID;
    परत instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (अचिन्हित पूर्णांक instance_index) स्थिरांक
  {
    स्थिरांक InstanceRecord *instance = get_instance (instance_index);
    जर (unlikely (!instance)) परत HB_OT_NAME_ID_INVALID;
    जर (instanceSize >= axis_मोजणी_2 * 4 + 6)
      परत StructAfter<NameID> (instance->get_coordinates (axis_मोजणी_2));
    परत HB_OT_NAME_ID_INVALID;
  }

  अचिन्हित पूर्णांक get_instance_coords (अचिन्हित पूर्णांक  instance_index,
				    अचिन्हित पूर्णांक *coords_length, /* IN/OUT */
				    दशांश        *coords         /* OUT */) स्थिरांक
  {
    स्थिरांक InstanceRecord *instance = get_instance (instance_index);
    जर (unlikely (!instance))
    {
      जर (coords_length)
	*coords_length = 0;
      परत 0;
    }

    जर (coords_length && *coords_length && coords)
    {
      hb_array_t<स्थिरांक F16DOT16> instanceCoords = instance->get_coordinates (axis_मोजणी_2)
							 .sub_array (0, coords_length);
      साठी (अचिन्हित पूर्णांक i = 0; i < instanceCoords.length; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    परत axis_मोजणी_2;
  }

  शून्य collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_index_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) स्थिरांक
  {
    जर (!has_data ()) परत;

    स्वतः axis_records = get_axes ();
    साठी (अचिन्हित i = 0 ; i < (अचिन्हित)axis_मोजणी_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      जर (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        सुरूठेवा;

      nameids->add (axis_records[i].get_name_id ());
    }

    साठी (अचिन्हित i = 0 ; i < (अचिन्हित)instance_मोजणी; i++)
    {
      स्थिरांक InstanceRecord *instance = get_instance (i);

      जर (!instance->keep_instance (axis_मोजणी_2, axes_old_index_tag_map, user_axes_location))
        सुरूठेवा;

      nameids->add (instance->subfamilyNameID);

      जर (instanceSize >= axis_मोजणी_2 * 4 + 6)
      {
        अचिन्हित post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_मोजणी_2));
        जर (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    अचिन्हित retained_axis_मोजणी = c->plan->axes_index_map.get_population ();
    जर (!retained_axis_मोजणी) //all axes are pinned
      return_trace (असत्य);

    fvar *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (!c->serializer->check_assign (out->axis_मोजणी_2, retained_axis_मोजणी, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    तार्किक has_postscript_nameid = असत्य;
    जर (instanceSize >= axis_मोजणी_2 * 4 + 6)
      has_postscript_nameid = सत्य;

    जर (!c->serializer->check_assign (out->instanceSize, retained_axis_मोजणी * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    स्वतः axes_records = get_axes ();
    साठी (अचिन्हित i = 0 ; i < (अचिन्हित)axis_मोजणी_2; i++)
    {
      जर (!c->plan->axes_index_map.has (i)) सुरूठेवा;
      जर (unlikely (!axes_records[i].subset (c)))
        return_trace (असत्य);
    }

    जर (!c->serializer->check_assign (out->firstAxis, get_size (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    अचिन्हित अंक_retained_instances = 0;
    साठी (अचिन्हित i = 0 ; i < (अचिन्हित)instance_मोजणी; i++)
    {
      स्थिरांक InstanceRecord *instance = get_instance (i);
      स्वतः snap = c->serializer->snapshot ();
      जर (!instance->subset (c, axis_मोजणी_2, has_postscript_nameid))
        c->serializer->revert (snap);
      अन्यथा
        अंक_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_मोजणी, अंक_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  सार्वजनिक:
  hb_array_t<स्थिरांक AxisRecord> get_axes () स्थिरांक
  { परत hb_array (&(हे+firstAxis), axis_मोजणी_2); }

  स्थिरांक InstanceRecord *get_instance (अचिन्हित पूर्णांक i) स्थिरांक
  {
    जर (unlikely (i >= instance_मोजणी)) परत nullptr;
   परत &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instanceSize);
  }

  संरक्षित:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_मोजणी_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axisSize;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_मोजणी;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instanceSize;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
