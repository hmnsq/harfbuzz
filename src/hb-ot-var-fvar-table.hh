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

#અન્તર્ભાવ "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#define HB_OT_TAG_fvar HB_TAG('f','v','a','r')


નામાકાશ OT {

સ્થિર તાર્કિક axis_coord_pinned_or_within_axis_range (અચલ hb_array_t<અચલ F16DOT16> coords,
                                                    unsigned axis_index,
                                                    Triple axis_limit)
{
  દશાંશ axis_coord = coords[axis_index].to_float ();
  જો (axis_limit.is_point ())
  {
    જો (axis_limit.minimum != axis_coord)
      પાછા ખોટું;
  }
  નહીંતર
  {
    જો (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      પાછા ખોટું;
  }
  પાછા સાચું;
}

સંરચના InstanceRecord
{
  friend સંરચના fvar;

  hb_array_t<અચલ F16DOT16> get_coordinates (unsigned પૂર્ણાંક axis_ગણતરી) અચલ
  { પાછા coordinatesZ.as_array (axis_ગણતરી); }

  તાર્કિક keep_instance (unsigned axis_ગણતરી,
                      અચલ hb_map_t *axes_index_tag_map,
                      અચલ hb_hashmap_t<hb_tag_t, Triple> *axes_location) અચલ
  {
    જો (axes_location->is_empty ()) પાછા સાચું;
    અચલ hb_array_t<અચલ F16DOT16> coords = get_coordinates (axis_ગણતરી);
    માટે (unsigned i = 0 ; i < axis_ગણતરી; i++)
    {
      uint32_t *axis_tag;
      જો (!axes_index_tag_map->has (i, &axis_tag))
        પાછા ખોટું;
      જો (!axes_location->has (*axis_tag))
        ચાલુ;

      Triple axis_limit = axes_location->get (*axis_tag);
      જો (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        પાછા ખોટું;
    }
    પાછા સાચું;
  }

  તાર્કિક subset (hb_subset_context_t *c,
               unsigned axis_ગણતરી,
               તાર્કિક has_postscript_nameid) અચલ
  {
    TRACE_SUBSET (આ);
    જો (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (ખોટું);
    જો (unlikely (!c->serializer->embed (flags))) return_trace (ખોટું);

    અચલ hb_array_t<અચલ F16DOT16> coords = get_coordinates (axis_ગણતરી);
    અચલ hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    માટે (unsigned i = 0 ; i < axis_ગણતરી; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      જો (!c->plan->axes_old_index_tag_map.has (i, &axis_tag)) return_trace (ખોટું);
      જો (axes_location->has (*axis_tag, &axis_limit))
      {
        જો (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (ખોટું);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        જો (axis_limit->is_point () && !c->plan->axes_index_map.has (i))
          ચાલુ;
      }

      જો (!c->serializer->embed (coords[i]))
        return_trace (ખોટું);
    }

    જો (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      જો (!c->serializer->embed (name_id))
        return_trace (ખોટું);
    }

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક axis_ગણતરી) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_ગણતરી));
  }

  સુરક્ષિત:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  UnsizedArrayOf<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  જાહેર:
  DEFINE_SIZE_UNBOUNDED (4);
};

સંરચના AxisRecord
{
  પૂર્ણાંક cmp (hb_tag_t key) અચલ { પાછા axisTag.cmp (key); }

  ગણના
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  શૂન્ય get_axis_deprecated (hb_ot_var_axis_t *info) અચલ
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_કિંમત, info->default_કિંમત, info->max_કિંમત);
  }
#endif

  શૂન્ય get_axis_info (unsigned axis_index, hb_ot_var_axis_info_t *info) અચલ
  {
    info->axis_index = axis_index;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (unsigned પૂર્ણાંક) flags;
    get_coordinates (info->min_કિંમત, info->default_કિંમત, info->max_કિંમત);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () અચલ { પાછા axisTag; }

  તાર્કિક is_hidden () અચલ { પાછા flags & AXIS_FLAG_HIDDEN; }

  દશાંશ normalize_axis_કિંમત (દશાંશ v) અચલ
  {
    દશાંશ min_કિંમત, default_કિંમત, max_કિંમત;
    get_coordinates (min_કિંમત, default_કિંમત, max_કિંમત);

    v = hb_clamp (v, min_કિંમત, max_કિંમત);

    જો (v == default_કિંમત)
      પાછા 0;
    નહીંતર જો (v < default_કિંમત)
      પાછા (v - default_કિંમત) / (default_કિંમત - min_કિંમત);
    નહીંતર
      પાછા (v - default_કિંમત) / (max_કિંમત - default_કિંમત);
  }

  hb_ot_name_id_t get_name_id () અચલ { પાછા axisNameID; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  શૂન્ય get_coordinates (દશાંશ &min, દશાંશ &default_, દશાંશ &max) અચલ
  {
    default_ = default_કિંમત_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_કિંમત_2.to_float ());
    max = hb_max (default_, max_કિંમત_2.to_float ());
  }

  દશાંશ get_default () અચલ
  {
    પાછા default_કિંમત_2.to_float ();
  }

  TripleDistances get_triple_distances () અચલ
  {
    દશાંશ min, default_, max;
    get_coordinates (min, default_, max);
    પાછા TripleDistances (min, default_, max);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    અચલ hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    જો (user_axes_location.has (axisTag, &axis_limit))
    {
      જો (c->plan->has_avar2 && axis_limit->is_point ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        out->min_કિંમત_2.set_float (axis_limit->મધ્ય);
        out->default_કિંમત_2.set_float (axis_limit->મધ્ય);
        out->max_કિંમત_2.set_float (axis_limit->મધ્ય);
        out->flags = out->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      નહીંતર
      {
        out->min_કિંમત_2.set_float (axis_limit->minimum);
        out->default_કિંમત_2.set_float (axis_limit->મધ્ય);
        out->max_કિંમત_2.set_float (axis_limit->maximum);
      }
    }
    return_trace (સાચું);
  }

  જાહેર:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  સુરક્ષિત:
  F16DOT16	min_કિંમત_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_કિંમત_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_કિંમત_2;	/* The maximum coordinate value for the axis. */
  જાહેર:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  જાહેર:
  DEFINE_SIZE_STATIC (20);
};

સંરચના fvar
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  તાર્કિક has_data () અચલ { પાછા version.to_int (); }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (આ) &&
		  hb_barrier () &&
		  axisSize == 20 && /* Assumed in our code. */
		  instanceSize >= axis_ગણતરી_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_ગણતરી, instanceSize));
  }

  unsigned પૂર્ણાંક get_axis_ગણતરી () અચલ { પાછા axis_ગણતરી_2; }

#ifndef HB_DISABLE_DEPRECATED
  unsigned પૂર્ણાંક get_axes_deprecated (unsigned પૂર્ણાંક      start_offset,
				    unsigned પૂર્ણાંક     *axes_ગણતરી /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) અચલ
  {
    જો (axes_ગણતરી && axes_array)
    {
      hb_array_t<અચલ AxisRecord> arr = get_axes ().sub_array (start_offset, axes_ગણતરી);
      માટે (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    પાછા axis_ગણતરી_2;
  }
#endif

  unsigned પૂર્ણાંક get_axis_infos (unsigned પૂર્ણાંક           start_offset,
			       unsigned પૂર્ણાંક          *axes_ગણતરી /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) અચલ
  {
    જો (axes_ગણતરી && axes_array)
    {
      hb_array_t<અચલ AxisRecord> arr = get_axes ().sub_array (start_offset, axes_ગણતરી);
      માટે (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    પાછા axis_ગણતરી_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  તાર્કિક
  find_axis_deprecated (hb_tag_t tag, unsigned *axis_index, hb_ot_var_axis_t *info) અચલ
  {
    unsigned i;
    જો (!axis_index) axis_index = &i;
    *axis_index = HB_OT_VAR_NO_AXIS_INDEX;
    auto axes = get_axes ();
    પાછા axes.lfind (tag, axis_index) && ((શૂન્ય) axes[*axis_index].get_axis_deprecated (info), સાચું);
  }
#endif
  તાર્કિક
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) અચલ
  {
    unsigned i;
    auto axes = get_axes ();
    પાછા axes.lfind (tag, &i) && ((શૂન્ય) axes[i].get_axis_info (i, info), સાચું);
  }

  દશાંશ normalize_axis_કિંમત (unsigned પૂર્ણાંક axis_index, દશાંશ v) અચલ
  { પાછા get_axes ()[axis_index].normalize_axis_કિંમત (v); }

  unsigned પૂર્ણાંક get_instance_ગણતરી () અચલ { પાછા instance_ગણતરી; }

  hb_ot_name_id_t get_instance_subfamily_name_id (unsigned પૂર્ણાંક instance_index) અચલ
  {
    અચલ InstanceRecord *instance = get_instance (instance_index);
    જો (unlikely (!instance)) પાછા HB_OT_NAME_ID_INVALID;
    પાછા instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (unsigned પૂર્ણાંક instance_index) અચલ
  {
    અચલ InstanceRecord *instance = get_instance (instance_index);
    જો (unlikely (!instance)) પાછા HB_OT_NAME_ID_INVALID;
    જો (instanceSize >= axis_ગણતરી_2 * 4 + 6)
      પાછા StructAfter<NameID> (instance->get_coordinates (axis_ગણતરી_2));
    પાછા HB_OT_NAME_ID_INVALID;
  }

  unsigned પૂર્ણાંક get_instance_coords (unsigned પૂર્ણાંક  instance_index,
				    unsigned પૂર્ણાંક *coords_length, /* IN/OUT */
				    દશાંશ        *coords         /* OUT */) અચલ
  {
    અચલ InstanceRecord *instance = get_instance (instance_index);
    જો (unlikely (!instance))
    {
      જો (coords_length)
	*coords_length = 0;
      પાછા 0;
    }

    જો (coords_length && *coords_length && coords)
    {
      hb_array_t<અચલ F16DOT16> instanceCoords = instance->get_coordinates (axis_ગણતરી_2)
							 .sub_array (0, coords_length);
      માટે (unsigned પૂર્ણાંક i = 0; i < instanceCoords.length; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    પાછા axis_ગણતરી_2;
  }

  શૂન્ય collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_index_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) અચલ
  {
    જો (!has_data ()) પાછા;

    auto axis_records = get_axes ();
    માટે (unsigned i = 0 ; i < (unsigned)axis_ગણતરી_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      જો (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        ચાલુ;

      nameids->add (axis_records[i].get_name_id ());
    }

    માટે (unsigned i = 0 ; i < (unsigned)instance_ગણતરી; i++)
    {
      અચલ InstanceRecord *instance = get_instance (i);

      જો (!instance->keep_instance (axis_ગણતરી_2, axes_old_index_tag_map, user_axes_location))
        ચાલુ;

      nameids->add (instance->subfamilyNameID);

      જો (instanceSize >= axis_ગણતરી_2 * 4 + 6)
      {
        unsigned post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_ગણતરી_2));
        જો (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    unsigned retained_axis_ગણતરી = c->plan->axes_index_map.get_population ();
    જો (!retained_axis_ગણતરી) //all axes are pinned
      return_trace (ખોટું);

    fvar *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (!c->serializer->check_assign (out->axis_ગણતરી_2, retained_axis_ગણતરી, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);

    તાર્કિક has_postscript_nameid = ખોટું;
    જો (instanceSize >= axis_ગણતરી_2 * 4 + 6)
      has_postscript_nameid = સાચું;

    જો (!c->serializer->check_assign (out->instanceSize, retained_axis_ગણતરી * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);

    auto axes_records = get_axes ();
    માટે (unsigned i = 0 ; i < (unsigned)axis_ગણતરી_2; i++)
    {
      જો (!c->plan->axes_index_map.has (i)) ચાલુ;
      જો (unlikely (!axes_records[i].subset (c)))
        return_trace (ખોટું);
    }

    જો (!c->serializer->check_assign (out->firstAxis, get_size (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);

    unsigned અંક_retained_instances = 0;
    માટે (unsigned i = 0 ; i < (unsigned)instance_ગણતરી; i++)
    {
      અચલ InstanceRecord *instance = get_instance (i);
      auto snap = c->serializer->snapshot ();
      જો (!instance->subset (c, axis_ગણતરી_2, has_postscript_nameid))
        c->serializer->revert (snap);
      નહીંતર
        અંક_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_ગણતરી, અંક_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  જાહેર:
  hb_array_t<અચલ AxisRecord> get_axes () અચલ
  { પાછા hb_array (&(આ+firstAxis), axis_ગણતરી_2); }

  અચલ InstanceRecord *get_instance (unsigned પૂર્ણાંક i) અચલ
  {
    જો (unlikely (i >= instance_ગણતરી)) પાછા nullptr;
   પાછા &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instanceSize);
  }

  સુરક્ષિત:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_ગણતરી_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axisSize;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_ગણતરી;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instanceSize;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  જાહેર:
  DEFINE_SIZE_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
