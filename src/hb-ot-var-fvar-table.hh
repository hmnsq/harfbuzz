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

#شامل "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#define HB_OT_TAG_fvar HB_TAG('f','v','a','r')


فضاینام OT {

ایستا منطقی axis_coord_pinned_or_within_axis_range (ثابت hb_array_t<ثابت F16DOT16> coords,
                                                    unsigned axis_index,
                                                    Triple axis_limit)
{
  اعشاری axis_coord = coords[axis_index].to_float ();
  اگر (axis_limit.is_point ())
  {
    اگر (axis_limit.minimum != axis_coord)
      بازگشت نادرست;
  }
  وگرنه
  {
    اگر (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      بازگشت نادرست;
  }
  بازگشت درست;
}

ساختار InstanceRecord
{
  friend ساختار fvar;

  hb_array_t<ثابت F16DOT16> get_coordinates (unsigned صحیح axis_شمار) ثابت
  { بازگشت coordinatesZ.as_array (axis_شمار); }

  منطقی keep_instance (unsigned axis_شمار,
                      ثابت hb_map_t *axes_index_tag_map,
                      ثابت hb_hashmap_t<hb_tag_t, Triple> *axes_location) ثابت
  {
    اگر (axes_location->is_empty ()) بازگشت درست;
    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_شمار);
    برای (unsigned i = 0 ; i < axis_شمار; i++)
    {
      uint32_t *axis_tag;
      اگر (!axes_index_tag_map->has (i, &axis_tag))
        بازگشت نادرست;
      اگر (!axes_location->has (*axis_tag))
        ادامه;

      Triple axis_limit = axes_location->get (*axis_tag);
      اگر (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        بازگشت نادرست;
    }
    بازگشت درست;
  }

  منطقی subset (hb_subset_context_t *c,
               unsigned axis_شمار,
               منطقی has_postscript_nameid) ثابت
  {
    TRACE_SUBSET (این);
    اگر (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (نادرست);
    اگر (unlikely (!c->serializer->embed (flags))) return_trace (نادرست);

    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_شمار);
    ثابت hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    برای (unsigned i = 0 ; i < axis_شمار; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      اگر (!c->plan->axes_old_index_tag_map.has (i, &axis_tag)) return_trace (نادرست);
      اگر (axes_location->has (*axis_tag, &axis_limit))
      {
        اگر (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (نادرست);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        اگر (axis_limit->is_point () && !c->plan->axes_index_map.has (i))
          ادامه;
      }

      اگر (!c->serializer->embed (coords[i]))
        return_trace (نادرست);
    }

    اگر (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      اگر (!c->serializer->embed (name_id))
        return_trace (نادرست);
    }

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح axis_شمار) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_شمار));
  }

  محافظتشده:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  UnsizedArrayOf<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  عمومی:
  DEFINE_SIZE_UNBOUNDED (4);
};

ساختار AxisRecord
{
  صحیح cmp (hb_tag_t key) ثابت { بازگشت axisTag.cmp (key); }

  شمارش
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  تهی get_axis_deprecated (hb_ot_var_axis_t *info) ثابت
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_مقدار, info->default_مقدار, info->max_مقدار);
  }
#endif

  تهی get_axis_info (unsigned axis_index, hb_ot_var_axis_info_t *info) ثابت
  {
    info->axis_index = axis_index;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (unsigned صحیح) flags;
    get_coordinates (info->min_مقدار, info->default_مقدار, info->max_مقدار);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () ثابت { بازگشت axisTag; }

  منطقی is_hidden () ثابت { بازگشت flags & AXIS_FLAG_HIDDEN; }

  اعشاری normalize_axis_مقدار (اعشاری v) ثابت
  {
    اعشاری min_مقدار, default_مقدار, max_مقدار;
    get_coordinates (min_مقدار, default_مقدار, max_مقدار);

    v = hb_clamp (v, min_مقدار, max_مقدار);

    اگر (v == default_مقدار)
      بازگشت 0;
    وگرنه اگر (v < default_مقدار)
      بازگشت (v - default_مقدار) / (default_مقدار - min_مقدار);
    وگرنه
      بازگشت (v - default_مقدار) / (max_مقدار - default_مقدار);
  }

  hb_ot_name_id_t get_name_id () ثابت { بازگشت axisNameID; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  تهی get_coordinates (اعشاری &min, اعشاری &default_, اعشاری &max) ثابت
  {
    default_ = default_مقدار_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_مقدار_2.to_float ());
    max = hb_max (default_, max_مقدار_2.to_float ());
  }

  اعشاری get_default () ثابت
  {
    بازگشت default_مقدار_2.to_float ();
  }

  TripleDistances get_triple_distances () ثابت
  {
    اعشاری min, default_, max;
    get_coordinates (min, default_, max);
    بازگشت TripleDistances (min, default_, max);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    ثابت hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    اگر (user_axes_location.has (axisTag, &axis_limit))
    {
      اگر (c->plan->has_avar2 && axis_limit->is_point ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        out->min_مقدار_2.set_float (axis_limit->میانه);
        out->default_مقدار_2.set_float (axis_limit->میانه);
        out->max_مقدار_2.set_float (axis_limit->میانه);
        out->flags = out->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      وگرنه
      {
        out->min_مقدار_2.set_float (axis_limit->minimum);
        out->default_مقدار_2.set_float (axis_limit->میانه);
        out->max_مقدار_2.set_float (axis_limit->maximum);
      }
    }
    return_trace (درست);
  }

  عمومی:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  محافظتشده:
  F16DOT16	min_مقدار_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_مقدار_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_مقدار_2;	/* The maximum coordinate value for the axis. */
  عمومی:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  عمومی:
  DEFINE_SIZE_STATIC (20);
};

ساختار fvar
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  منطقی has_data () ثابت { بازگشت version.to_int (); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (این) &&
		  hb_barrier () &&
		  axisSize == 20 && /* Assumed in our code. */
		  instanceSize >= axis_شمار_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_شمار, instanceSize));
  }

  unsigned صحیح get_axis_شمار () ثابت { بازگشت axis_شمار_2; }

#ifndef HB_DISABLE_DEPRECATED
  unsigned صحیح get_axes_deprecated (unsigned صحیح      start_offset,
				    unsigned صحیح     *axes_شمار /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) ثابت
  {
    اگر (axes_شمار && axes_array)
    {
      hb_array_t<ثابت AxisRecord> arr = get_axes ().sub_array (start_offset, axes_شمار);
      برای (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    بازگشت axis_شمار_2;
  }
#endif

  unsigned صحیح get_axis_infos (unsigned صحیح           start_offset,
			       unsigned صحیح          *axes_شمار /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) ثابت
  {
    اگر (axes_شمار && axes_array)
    {
      hb_array_t<ثابت AxisRecord> arr = get_axes ().sub_array (start_offset, axes_شمار);
      برای (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    بازگشت axis_شمار_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  منطقی
  find_axis_deprecated (hb_tag_t tag, unsigned *axis_index, hb_ot_var_axis_t *info) ثابت
  {
    unsigned i;
    اگر (!axis_index) axis_index = &i;
    *axis_index = HB_OT_VAR_NO_AXIS_INDEX;
    auto axes = get_axes ();
    بازگشت axes.lfind (tag, axis_index) && ((تهی) axes[*axis_index].get_axis_deprecated (info), درست);
  }
#endif
  منطقی
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) ثابت
  {
    unsigned i;
    auto axes = get_axes ();
    بازگشت axes.lfind (tag, &i) && ((تهی) axes[i].get_axis_info (i, info), درست);
  }

  اعشاری normalize_axis_مقدار (unsigned صحیح axis_index, اعشاری v) ثابت
  { بازگشت get_axes ()[axis_index].normalize_axis_مقدار (v); }

  unsigned صحیح get_instance_شمار () ثابت { بازگشت instance_شمار; }

  hb_ot_name_id_t get_instance_subfamily_name_id (unsigned صحیح instance_index) ثابت
  {
    ثابت InstanceRecord *instance = get_instance (instance_index);
    اگر (unlikely (!instance)) بازگشت HB_OT_NAME_ID_INVALID;
    بازگشت instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (unsigned صحیح instance_index) ثابت
  {
    ثابت InstanceRecord *instance = get_instance (instance_index);
    اگر (unlikely (!instance)) بازگشت HB_OT_NAME_ID_INVALID;
    اگر (instanceSize >= axis_شمار_2 * 4 + 6)
      بازگشت StructAfter<NameID> (instance->get_coordinates (axis_شمار_2));
    بازگشت HB_OT_NAME_ID_INVALID;
  }

  unsigned صحیح get_instance_coords (unsigned صحیح  instance_index,
				    unsigned صحیح *coords_length, /* IN/OUT */
				    اعشاری        *coords         /* OUT */) ثابت
  {
    ثابت InstanceRecord *instance = get_instance (instance_index);
    اگر (unlikely (!instance))
    {
      اگر (coords_length)
	*coords_length = 0;
      بازگشت 0;
    }

    اگر (coords_length && *coords_length && coords)
    {
      hb_array_t<ثابت F16DOT16> instanceCoords = instance->get_coordinates (axis_شمار_2)
							 .sub_array (0, coords_length);
      برای (unsigned صحیح i = 0; i < instanceCoords.length; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    بازگشت axis_شمار_2;
  }

  تهی collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_index_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) ثابت
  {
    اگر (!has_data ()) بازگشت;

    auto axis_records = get_axes ();
    برای (unsigned i = 0 ; i < (unsigned)axis_شمار_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      اگر (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        ادامه;

      nameids->add (axis_records[i].get_name_id ());
    }

    برای (unsigned i = 0 ; i < (unsigned)instance_شمار; i++)
    {
      ثابت InstanceRecord *instance = get_instance (i);

      اگر (!instance->keep_instance (axis_شمار_2, axes_old_index_tag_map, user_axes_location))
        ادامه;

      nameids->add (instance->subfamilyNameID);

      اگر (instanceSize >= axis_شمار_2 * 4 + 6)
      {
        unsigned post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_شمار_2));
        اگر (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    unsigned retained_axis_شمار = c->plan->axes_index_map.get_population ();
    اگر (!retained_axis_شمار) //all axes are pinned
      return_trace (نادرست);

    fvar *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (!c->serializer->check_assign (out->axis_شمار_2, retained_axis_شمار, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);

    منطقی has_postscript_nameid = نادرست;
    اگر (instanceSize >= axis_شمار_2 * 4 + 6)
      has_postscript_nameid = درست;

    اگر (!c->serializer->check_assign (out->instanceSize, retained_axis_شمار * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);

    auto axes_records = get_axes ();
    برای (unsigned i = 0 ; i < (unsigned)axis_شمار_2; i++)
    {
      اگر (!c->plan->axes_index_map.has (i)) ادامه;
      اگر (unlikely (!axes_records[i].subset (c)))
        return_trace (نادرست);
    }

    اگر (!c->serializer->check_assign (out->firstAxis, get_size (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);

    unsigned رقم_retained_instances = 0;
    برای (unsigned i = 0 ; i < (unsigned)instance_شمار; i++)
    {
      ثابت InstanceRecord *instance = get_instance (i);
      auto snap = c->serializer->snapshot ();
      اگر (!instance->subset (c, axis_شمار_2, has_postscript_nameid))
        c->serializer->revert (snap);
      وگرنه
        رقم_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_شمار, رقم_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  عمومی:
  hb_array_t<ثابت AxisRecord> get_axes () ثابت
  { بازگشت hb_array (&(این+firstAxis), axis_شمار_2); }

  ثابت InstanceRecord *get_instance (unsigned صحیح i) ثابت
  {
    اگر (unlikely (i >= instance_شمار)) بازگشت nullptr;
   بازگشت &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instanceSize);
  }

  محافظتشده:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_شمار_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axisSize;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_شمار;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instanceSize;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  عمومی:
  DEFINE_SIZE_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
