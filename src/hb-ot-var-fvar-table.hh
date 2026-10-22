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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#define HB_OT_TAG_fvar HB_TAG('f','v','a','r')


ନାମାକାଶ OT {

ସ୍ଥିର ତାର୍କିକ axis_coord_pinned_or_within_axis_range (ଅଚଳ hb_array_t<ଅଚଳ F16DOT16> coords,
                                                    unsigned axis_index,
                                                    Triple axis_limit)
{
  ଦଶମିକ axis_coord = coords[axis_index].to_float ();
  ଯଦି (axis_limit.is_point ())
  {
    ଯଦି (axis_limit.minimum != axis_coord)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ଅନ୍ୟଥା
  {
    ଯଦି (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସଂରଚନା InstanceRecord
{
  friend ସଂରଚନା fvar;

  hb_array_t<ଅଚଳ F16DOT16> get_coordinates (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis_ଗଣନା) ଅଚଳ
  { ଫେରନ୍ତୁ coordinatesZ.as_array (axis_ଗଣନା); }

  ତାର୍କିକ keep_instance (unsigned axis_ଗଣନା,
                      ଅଚଳ hb_map_t *axes_index_tag_map,
                      ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> *axes_location) ଅଚଳ
  {
    ଯଦି (axes_location->is_empty ()) ଫେରନ୍ତୁ ସତ୍ୟ;
    ଅଚଳ hb_array_t<ଅଚଳ F16DOT16> coords = get_coordinates (axis_ଗଣନା);
    ପାଇଁ (unsigned i = 0 ; i < axis_ଗଣନା; i++)
    {
      uint32_t *axis_tag;
      ଯଦି (!axes_index_tag_map->has (i, &axis_tag))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (!axes_location->has (*axis_tag))
        ଜାରି;

      Triple axis_limit = axes_location->get (*axis_tag);
      ଯଦି (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               unsigned axis_ଗଣନା,
               ତାର୍କିକ has_postscript_nameid) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->serializer->embed (flags))) return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_array_t<ଅଚଳ F16DOT16> coords = get_coordinates (axis_ଗଣନା);
    ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    ପାଇଁ (unsigned i = 0 ; i < axis_ଗଣନା; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      ଯଦି (!c->plan->axes_old_index_tag_map.has (i, &axis_tag)) return_trace (ମିଥ୍ୟା);
      ଯଦି (axes_location->has (*axis_tag, &axis_limit))
      {
        ଯଦି (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (ମିଥ୍ୟା);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        ଯଦି (axis_limit->is_point () && !c->plan->axes_index_map.has (i))
          ଜାରି;
      }

      ଯଦି (!c->serializer->embed (coords[i]))
        return_trace (ମିଥ୍ୟା);
    }

    ଯଦି (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      ଯଦି (!c->serializer->embed (name_id))
        return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis_ଗଣନା) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_ଗଣନା));
  }

  ସୁରକ୍ଷିତ:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  UnsizedArrayOf<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNBOUNDED (4);
};

ସଂରଚନା AxisRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_tag_t key) ଅଚଳ { ଫେରନ୍ତୁ axisTag.cmp (key); }

  ଗଣନା
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  ଶୂନ୍ୟ get_axis_deprecated (hb_ot_var_axis_t *info) ଅଚଳ
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_ମୂଲ୍ୟ, info->default_ମୂଲ୍ୟ, info->max_ମୂଲ୍ୟ);
  }
#endif

  ଶୂନ୍ୟ get_axis_info (unsigned axis_index, hb_ot_var_axis_info_t *info) ଅଚଳ
  {
    info->axis_index = axis_index;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) flags;
    get_coordinates (info->min_ମୂଲ୍ୟ, info->default_ମୂଲ୍ୟ, info->max_ମୂଲ୍ୟ);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () ଅଚଳ { ଫେରନ୍ତୁ axisTag; }

  ତାର୍କିକ is_hidden () ଅଚଳ { ଫେରନ୍ତୁ flags & AXIS_FLAG_HIDDEN; }

  ଦଶମିକ normalize_axis_ମୂଲ୍ୟ (ଦଶମିକ v) ଅଚଳ
  {
    ଦଶମିକ min_ମୂଲ୍ୟ, default_ମୂଲ୍ୟ, max_ମୂଲ୍ୟ;
    get_coordinates (min_ମୂଲ୍ୟ, default_ମୂଲ୍ୟ, max_ମୂଲ୍ୟ);

    v = hb_clamp (v, min_ମୂଲ୍ୟ, max_ମୂଲ୍ୟ);

    ଯଦି (v == default_ମୂଲ୍ୟ)
      ଫେରନ୍ତୁ 0;
    ଅନ୍ୟଥା ଯଦି (v < default_ମୂଲ୍ୟ)
      ଫେରନ୍ତୁ (v - default_ମୂଲ୍ୟ) / (default_ମୂଲ୍ୟ - min_ମୂଲ୍ୟ);
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ (v - default_ମୂଲ୍ୟ) / (max_ମୂଲ୍ୟ - default_ମୂଲ୍ୟ);
  }

  hb_ot_name_id_t get_name_id () ଅଚଳ { ଫେରନ୍ତୁ axisNameID; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ get_coordinates (ଦଶମିକ &min, ଦଶମିକ &default_, ଦଶମିକ &max) ଅଚଳ
  {
    default_ = default_ମୂଲ୍ୟ_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_ମୂଲ୍ୟ_2.to_float ());
    max = hb_max (default_, max_ମୂଲ୍ୟ_2.to_float ());
  }

  ଦଶମିକ get_default () ଅଚଳ
  {
    ଫେରନ୍ତୁ default_ମୂଲ୍ୟ_2.to_float ();
  }

  TripleDistances get_triple_distances () ଅଚଳ
  {
    ଦଶମିକ min, default_, max;
    get_coordinates (min, default_, max);
    ଫେରନ୍ତୁ TripleDistances (min, default_, max);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    ଯଦି (user_axes_location.has (axisTag, &axis_limit))
    {
      ଯଦି (c->plan->has_avar2 && axis_limit->is_point ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        out->min_ମୂଲ୍ୟ_2.set_float (axis_limit->ମଧ୍ୟ);
        out->default_ମୂଲ୍ୟ_2.set_float (axis_limit->ମଧ୍ୟ);
        out->max_ମୂଲ୍ୟ_2.set_float (axis_limit->ମଧ୍ୟ);
        out->flags = out->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      ଅନ୍ୟଥା
      {
        out->min_ମୂଲ୍ୟ_2.set_float (axis_limit->minimum);
        out->default_ମୂଲ୍ୟ_2.set_float (axis_limit->ମଧ୍ୟ);
        out->max_ମୂଲ୍ୟ_2.set_float (axis_limit->maximum);
      }
    }
    return_trace (ସତ୍ୟ);
  }

  ସର୍ବସାଧାରଣ:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  ସୁରକ୍ଷିତ:
  F16DOT16	min_ମୂଲ୍ୟ_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_ମୂଲ୍ୟ_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_ମୂଲ୍ୟ_2;	/* The maximum coordinate value for the axis. */
  ସର୍ବସାଧାରଣ:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (20);
};

ସଂରଚନା fvar
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.to_int (); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  axisSize == 20 && /* Assumed in our code. */
		  instanceSize >= axis_ଗଣନା_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_ଗଣନା, instanceSize));
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_axis_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ axis_ଗଣନା_2; }

#ifndef HB_DISABLE_DEPRECATED
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_axes_deprecated (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା      start_offset,
				    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା     *axes_ଗଣନା /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) ଅଚଳ
  {
    ଯଦି (axes_ଗଣନା && axes_array)
    {
      hb_array_t<ଅଚଳ AxisRecord> arr = get_axes ().sub_array (start_offset, axes_ଗଣନା);
      ପାଇଁ (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    ଫେରନ୍ତୁ axis_ଗଣନା_2;
  }
#endif

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_axis_infos (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା           start_offset,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା          *axes_ଗଣନା /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) ଅଚଳ
  {
    ଯଦି (axes_ଗଣନା && axes_array)
    {
      hb_array_t<ଅଚଳ AxisRecord> arr = get_axes ().sub_array (start_offset, axes_ଗଣନା);
      ପାଇଁ (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    ଫେରନ୍ତୁ axis_ଗଣନା_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  ତାର୍କିକ
  find_axis_deprecated (hb_tag_t tag, unsigned *axis_index, hb_ot_var_axis_t *info) ଅଚଳ
  {
    unsigned i;
    ଯଦି (!axis_index) axis_index = &i;
    *axis_index = HB_OT_VAR_NO_AXIS_INDEX;
    auto axes = get_axes ();
    ଫେରନ୍ତୁ axes.lfind (tag, axis_index) && ((ଶୂନ୍ୟ) axes[*axis_index].get_axis_deprecated (info), ସତ୍ୟ);
  }
#endif
  ତାର୍କିକ
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) ଅଚଳ
  {
    unsigned i;
    auto axes = get_axes ();
    ଫେରନ୍ତୁ axes.lfind (tag, &i) && ((ଶୂନ୍ୟ) axes[i].get_axis_info (i, info), ସତ୍ୟ);
  }

  ଦଶମିକ normalize_axis_ମୂଲ୍ୟ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis_index, ଦଶମିକ v) ଅଚଳ
  { ଫେରନ୍ତୁ get_axes ()[axis_index].normalize_axis_ମୂଲ୍ୟ (v); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_instance_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ instance_ଗଣନା; }

  hb_ot_name_id_t get_instance_subfamily_name_id (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା instance_index) ଅଚଳ
  {
    ଅଚଳ InstanceRecord *instance = get_instance (instance_index);
    ଯଦି (unlikely (!instance)) ଫେରନ୍ତୁ HB_OT_NAME_ID_INVALID;
    ଫେରନ୍ତୁ instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା instance_index) ଅଚଳ
  {
    ଅଚଳ InstanceRecord *instance = get_instance (instance_index);
    ଯଦି (unlikely (!instance)) ଫେରନ୍ତୁ HB_OT_NAME_ID_INVALID;
    ଯଦି (instanceSize >= axis_ଗଣନା_2 * 4 + 6)
      ଫେରନ୍ତୁ StructAfter<NameID> (instance->get_coordinates (axis_ଗଣନା_2));
    ଫେରନ୍ତୁ HB_OT_NAME_ID_INVALID;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_instance_coords (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  instance_index,
				    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords_length, /* IN/OUT */
				    ଦଶମିକ        *coords         /* OUT */) ଅଚଳ
  {
    ଅଚଳ InstanceRecord *instance = get_instance (instance_index);
    ଯଦି (unlikely (!instance))
    {
      ଯଦି (coords_length)
	*coords_length = 0;
      ଫେରନ୍ତୁ 0;
    }

    ଯଦି (coords_length && *coords_length && coords)
    {
      hb_array_t<ଅଚଳ F16DOT16> instanceCoords = instance->get_coordinates (axis_ଗଣନା_2)
							 .sub_array (0, coords_length);
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < instanceCoords.length; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    ଫେରନ୍ତୁ axis_ଗଣନା_2;
  }

  ଶୂନ୍ୟ collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_index_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) ଅଚଳ
  {
    ଯଦି (!has_data ()) ଫେରନ୍ତୁ;

    auto axis_records = get_axes ();
    ପାଇଁ (unsigned i = 0 ; i < (unsigned)axis_ଗଣନା_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      ଯଦି (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        ଜାରି;

      nameids->add (axis_records[i].get_name_id ());
    }

    ପାଇଁ (unsigned i = 0 ; i < (unsigned)instance_ଗଣନା; i++)
    {
      ଅଚଳ InstanceRecord *instance = get_instance (i);

      ଯଦି (!instance->keep_instance (axis_ଗଣନା_2, axes_old_index_tag_map, user_axes_location))
        ଜାରି;

      nameids->add (instance->subfamilyNameID);

      ଯଦି (instanceSize >= axis_ଗଣନା_2 * 4 + 6)
      {
        unsigned post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_ଗଣନା_2));
        ଯଦି (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    unsigned retained_axis_ଗଣନା = c->plan->axes_index_map.get_population ();
    ଯଦି (!retained_axis_ଗଣନା) //all axes are pinned
      return_trace (ମିଥ୍ୟା);

    fvar *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (!c->serializer->check_assign (out->axis_ଗଣନା_2, retained_axis_ଗଣନା, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ has_postscript_nameid = ମିଥ୍ୟା;
    ଯଦି (instanceSize >= axis_ଗଣନା_2 * 4 + 6)
      has_postscript_nameid = ସତ୍ୟ;

    ଯଦି (!c->serializer->check_assign (out->instanceSize, retained_axis_ଗଣନା * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);

    auto axes_records = get_axes ();
    ପାଇଁ (unsigned i = 0 ; i < (unsigned)axis_ଗଣନା_2; i++)
    {
      ଯଦି (!c->plan->axes_index_map.has (i)) ଜାରି;
      ଯଦି (unlikely (!axes_records[i].subset (c)))
        return_trace (ମିଥ୍ୟା);
    }

    ଯଦି (!c->serializer->check_assign (out->firstAxis, get_size (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);

    unsigned ଅଙ୍କ_retained_instances = 0;
    ପାଇଁ (unsigned i = 0 ; i < (unsigned)instance_ଗଣନା; i++)
    {
      ଅଚଳ InstanceRecord *instance = get_instance (i);
      auto snap = c->serializer->snapshot ();
      ଯଦି (!instance->subset (c, axis_ଗଣନା_2, has_postscript_nameid))
        c->serializer->revert (snap);
      ଅନ୍ୟଥା
        ଅଙ୍କ_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_ଗଣନା, ଅଙ୍କ_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ସର୍ବସାଧାରଣ:
  hb_array_t<ଅଚଳ AxisRecord> get_axes () ଅଚଳ
  { ଫେରନ୍ତୁ hb_array (&(ଏହି+firstAxis), axis_ଗଣନା_2); }

  ଅଚଳ InstanceRecord *get_instance (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  {
    ଯଦି (unlikely (i >= instance_ଗଣନା)) ଫେରନ୍ତୁ nullptr;
   ଫେରନ୍ତୁ &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instanceSize);
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_ଗଣନା_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axisSize;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_ଗଣନା;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instanceSize;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
