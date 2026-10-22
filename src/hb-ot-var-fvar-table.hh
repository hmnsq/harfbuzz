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

#ضمن "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#define HB_OT_TAG_fvar HB_TAG('f','v','a','r')


نطاق OT {

ساكن منطقي axis_coord_pinned_or_within_axis_range (ثابت hb_array_t<ثابت F16DOT16> coords,
                                                    unsigned axis_index,
                                                    Triple axis_limit)
{
  عائم axis_coord = coords[axis_index].to_float ();
  إذا (axis_limit.is_point ())
  {
    إذا (axis_limit.minimum != axis_coord)
      إرجاع خطأ;
  }
  وإلا
  {
    إذا (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      إرجاع خطأ;
  }
  إرجاع true;
}

بنية InstanceRecord
{
  friend بنية fvar;

  hb_array_t<ثابت F16DOT16> get_coordinates (unsigned صحيح axis_تعداد) ثابت
  { إرجاع coordinatesZ.as_array (axis_تعداد); }

  منطقي keep_instance (unsigned axis_تعداد,
                      ثابت hb_map_t *axes_index_tag_map,
                      ثابت hb_hashmap_t<hb_tag_t, Triple> *axes_location) ثابت
  {
    إذا (axes_location->is_empty ()) إرجاع true;
    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_تعداد);
    لكل (unsigned i = 0 ; i < axis_تعداد; i++)
    {
      uint32_t *axis_tag;
      إذا (!axes_index_tag_map->has (i, &axis_tag))
        إرجاع خطأ;
      إذا (!axes_location->has (*axis_tag))
        استمر;

      Triple axis_limit = axes_location->get (*axis_tag);
      إذا (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        إرجاع خطأ;
    }
    إرجاع true;
  }

  منطقي subset (hb_subset_context_t *c,
               unsigned axis_تعداد,
               منطقي has_postscript_nameid) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (flags))) return_trace (خطأ);

    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_تعداد);
    ثابت hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    لكل (unsigned i = 0 ; i < axis_تعداد; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      إذا (!c->plan->axes_old_index_tag_map.has (i, &axis_tag)) return_trace (خطأ);
      إذا (axes_location->has (*axis_tag, &axis_limit))
      {
        إذا (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (خطأ);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        إذا (axis_limit->is_point () && !c->plan->axes_index_map.has (i))
          استمر;
      }

      إذا (!c->serializer->embed (coords[i]))
        return_trace (خطأ);
    }

    إذا (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      إذا (!c->serializer->embed (name_id))
        return_trace (خطأ);
    }

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح axis_تعداد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_تعداد));
  }

  محمي:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  UnsizedArrayOf<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  عام:
  DEFINE_SIZE_UNBOUNDED (4);
};

بنية AxisRecord
{
  صحيح cmp (hb_tag_t key) ثابت { إرجاع axisTag.cmp (key); }

  تعداد
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  فراغ get_axis_deprecated (hb_ot_var_axis_t *info) ثابت
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_قيمة, info->default_قيمة, info->max_قيمة);
  }
#endif

  فراغ get_axis_info (unsigned axis_index, hb_ot_var_axis_info_t *info) ثابت
  {
    info->axis_index = axis_index;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (unsigned صحيح) flags;
    get_coordinates (info->min_قيمة, info->default_قيمة, info->max_قيمة);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () ثابت { إرجاع axisTag; }

  منطقي is_hidden () ثابت { إرجاع flags & AXIS_FLAG_HIDDEN; }

  عائم normalize_axis_قيمة (عائم v) ثابت
  {
    عائم min_قيمة, default_قيمة, max_قيمة;
    get_coordinates (min_قيمة, default_قيمة, max_قيمة);

    v = hb_clamp (v, min_قيمة, max_قيمة);

    إذا (v == default_قيمة)
      إرجاع 0;
    وإلا إذا (v < default_قيمة)
      إرجاع (v - default_قيمة) / (default_قيمة - min_قيمة);
    وإلا
      إرجاع (v - default_قيمة) / (max_قيمة - default_قيمة);
  }

  hb_ot_name_id_t get_name_id () ثابت { إرجاع axisNameID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_coordinates (عائم &min, عائم &default_, عائم &max) ثابت
  {
    default_ = default_قيمة_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_قيمة_2.to_float ());
    max = hb_max (default_, max_قيمة_2.to_float ());
  }

  عائم get_default () ثابت
  {
    إرجاع default_قيمة_2.to_float ();
  }

  TripleDistances get_triple_distances () ثابت
  {
    عائم min, default_, max;
    get_coordinates (min, default_, max);
    إرجاع TripleDistances (min, default_, max);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    ثابت hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    إذا (user_axes_location.has (axisTag, &axis_limit))
    {
      إذا (c->plan->has_avar2 && axis_limit->is_point ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        out->min_قيمة_2.set_float (axis_limit->وسط);
        out->default_قيمة_2.set_float (axis_limit->وسط);
        out->max_قيمة_2.set_float (axis_limit->وسط);
        out->flags = out->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      وإلا
      {
        out->min_قيمة_2.set_float (axis_limit->minimum);
        out->default_قيمة_2.set_float (axis_limit->وسط);
        out->max_قيمة_2.set_float (axis_limit->maximum);
      }
    }
    return_trace (true);
  }

  عام:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  محمي:
  F16DOT16	min_قيمة_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_قيمة_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_قيمة_2;	/* The maximum coordinate value for the axis. */
  عام:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  عام:
  DEFINE_SIZE_STATIC (20);
};

بنية fvar
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  منطقي has_data () ثابت { إرجاع version.to_int (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (هذا) &&
		  hb_barrier () &&
		  axisSize == 20 && /* Assumed in our code. */
		  instanceSize >= axis_تعداد_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_تعداد, instanceSize));
  }

  unsigned صحيح get_axis_تعداد () ثابت { إرجاع axis_تعداد_2; }

#ifndef HB_DISABLE_DEPRECATED
  unsigned صحيح get_axes_deprecated (unsigned صحيح      start_offset,
				    unsigned صحيح     *axes_تعداد /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) ثابت
  {
    إذا (axes_تعداد && axes_array)
    {
      hb_array_t<ثابت AxisRecord> arr = get_axes ().sub_array (start_offset, axes_تعداد);
      لكل (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    إرجاع axis_تعداد_2;
  }
#endif

  unsigned صحيح get_axis_infos (unsigned صحيح           start_offset,
			       unsigned صحيح          *axes_تعداد /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) ثابت
  {
    إذا (axes_تعداد && axes_array)
    {
      hb_array_t<ثابت AxisRecord> arr = get_axes ().sub_array (start_offset, axes_تعداد);
      لكل (unsigned i = 0; i < arr.length; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    إرجاع axis_تعداد_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  منطقي
  find_axis_deprecated (hb_tag_t tag, unsigned *axis_index, hb_ot_var_axis_t *info) ثابت
  {
    unsigned i;
    إذا (!axis_index) axis_index = &i;
    *axis_index = HB_OT_VAR_NO_AXIS_INDEX;
    auto axes = get_axes ();
    إرجاع axes.lfind (tag, axis_index) && ((فراغ) axes[*axis_index].get_axis_deprecated (info), true);
  }
#endif
  منطقي
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) ثابت
  {
    unsigned i;
    auto axes = get_axes ();
    إرجاع axes.lfind (tag, &i) && ((فراغ) axes[i].get_axis_info (i, info), true);
  }

  عائم normalize_axis_قيمة (unsigned صحيح axis_index, عائم v) ثابت
  { إرجاع get_axes ()[axis_index].normalize_axis_قيمة (v); }

  unsigned صحيح get_instance_تعداد () ثابت { إرجاع instance_تعداد; }

  hb_ot_name_id_t get_instance_subfamily_name_id (unsigned صحيح instance_index) ثابت
  {
    ثابت InstanceRecord *instance = get_instance (instance_index);
    إذا (unlikely (!instance)) إرجاع HB_OT_NAME_ID_INVALID;
    إرجاع instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (unsigned صحيح instance_index) ثابت
  {
    ثابت InstanceRecord *instance = get_instance (instance_index);
    إذا (unlikely (!instance)) إرجاع HB_OT_NAME_ID_INVALID;
    إذا (instanceSize >= axis_تعداد_2 * 4 + 6)
      إرجاع StructAfter<NameID> (instance->get_coordinates (axis_تعداد_2));
    إرجاع HB_OT_NAME_ID_INVALID;
  }

  unsigned صحيح get_instance_coords (unsigned صحيح  instance_index,
				    unsigned صحيح *coords_length, /* IN/OUT */
				    عائم        *coords         /* OUT */) ثابت
  {
    ثابت InstanceRecord *instance = get_instance (instance_index);
    إذا (unlikely (!instance))
    {
      إذا (coords_length)
	*coords_length = 0;
      إرجاع 0;
    }

    إذا (coords_length && *coords_length && coords)
    {
      hb_array_t<ثابت F16DOT16> instanceCoords = instance->get_coordinates (axis_تعداد_2)
							 .sub_array (0, coords_length);
      لكل (unsigned صحيح i = 0; i < instanceCoords.length; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    إرجاع axis_تعداد_2;
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_index_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) ثابت
  {
    إذا (!has_data ()) إرجاع;

    auto axis_records = get_axes ();
    لكل (unsigned i = 0 ; i < (unsigned)axis_تعداد_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      إذا (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        استمر;

      nameids->add (axis_records[i].get_name_id ());
    }

    لكل (unsigned i = 0 ; i < (unsigned)instance_تعداد; i++)
    {
      ثابت InstanceRecord *instance = get_instance (i);

      إذا (!instance->keep_instance (axis_تعداد_2, axes_old_index_tag_map, user_axes_location))
        استمر;

      nameids->add (instance->subfamilyNameID);

      إذا (instanceSize >= axis_تعداد_2 * 4 + 6)
      {
        unsigned post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_تعداد_2));
        إذا (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    unsigned retained_axis_تعداد = c->plan->axes_index_map.get_population ();
    إذا (!retained_axis_تعداد) //all axes are pinned
      return_trace (خطأ);

    fvar *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (!c->serializer->check_assign (out->axis_تعداد_2, retained_axis_تعداد, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (خطأ);

    منطقي has_postscript_nameid = خطأ;
    إذا (instanceSize >= axis_تعداد_2 * 4 + 6)
      has_postscript_nameid = true;

    إذا (!c->serializer->check_assign (out->instanceSize, retained_axis_تعداد * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (خطأ);

    auto axes_records = get_axes ();
    لكل (unsigned i = 0 ; i < (unsigned)axis_تعداد_2; i++)
    {
      إذا (!c->plan->axes_index_map.has (i)) استمر;
      إذا (unlikely (!axes_records[i].subset (c)))
        return_trace (خطأ);
    }

    إذا (!c->serializer->check_assign (out->firstAxis, get_size (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (خطأ);

    unsigned رقم_retained_instances = 0;
    لكل (unsigned i = 0 ; i < (unsigned)instance_تعداد; i++)
    {
      ثابت InstanceRecord *instance = get_instance (i);
      auto snap = c->serializer->snapshot ();
      إذا (!instance->subset (c, axis_تعداد_2, has_postscript_nameid))
        c->serializer->revert (snap);
      وإلا
        رقم_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_تعداد, رقم_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  عام:
  hb_array_t<ثابت AxisRecord> get_axes () ثابت
  { إرجاع hb_array (&(هذا+firstAxis), axis_تعداد_2); }

  ثابت InstanceRecord *get_instance (unsigned صحيح i) ثابت
  {
    إذا (unlikely (i >= instance_تعداد)) إرجاع nullptr;
   إرجاع &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instanceSize);
  }

  محمي:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_تعداد_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axisSize;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_تعداد;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instanceSize;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  عام:
  DEFINE_SIZE_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
