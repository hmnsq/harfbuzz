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

#অন্তর্ভাব "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#define HB_OT_TAG_fvar HB_TAG('f','v','a','r')


নামাকাশ OT {

স্থির যৌক্তিক axis_coord_pinned_or_within_axis_range (ধ্রুবক hb_array_t<ধ্রুবক F16DOT16> coords,
                                                    unsigned axis_সূচক,
                                                    Triple axis_limit)
{
  দশমিক axis_coord = coords[axis_সূচক].to_float ();
  যদি (axis_limit.is_point ())
  {
    যদি (axis_limit.minimum != axis_coord)
      ফেরত মিথ্যা;
  }
  অন্যথা
  {
    যদি (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      ফেরত মিথ্যা;
  }
  ফেরত সত্য;
}

কাঠামো InstanceRecord
{
  friend কাঠামো fvar;

  hb_array_t<ধ্রুবক F16DOT16> get_coordinates (unsigned পূর্ণসংখ্যা axis_গুনতি) ধ্রুবক
  { ফেরত coordinatesZ.as_array (axis_গুনতি); }

  যৌক্তিক keep_instance (unsigned axis_গুনতি,
                      ধ্রুবক hb_map_t *axes_সূচক_tag_map,
                      ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> *axes_location) ধ্রুবক
  {
    যদি (axes_location->is_empty ()) ফেরত সত্য;
    ধ্রুবক hb_array_t<ধ্রুবক F16DOT16> coords = get_coordinates (axis_গুনতি);
    জন্য (unsigned i = 0 ; i < axis_গুনতি; i++)
    {
      uint32_t *axis_tag;
      যদি (!axes_সূচক_tag_map->has (i, &axis_tag))
        ফেরত মিথ্যা;
      যদি (!axes_location->has (*axis_tag))
        চালু;

      Triple axis_limit = axes_location->get (*axis_tag);
      যদি (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        ফেরত মিথ্যা;
    }
    ফেরত সত্য;
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               unsigned axis_গুনতি,
               যৌক্তিক has_postscript_nameid) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (মিথ্যা);
    যদি (unlikely (!c->serializer->embed (flags))) return_trace (মিথ্যা);

    ধ্রুবক hb_array_t<ধ্রুবক F16DOT16> coords = get_coordinates (axis_গুনতি);
    ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    জন্য (unsigned i = 0 ; i < axis_গুনতি; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      যদি (!c->plan->axes_old_সূচক_tag_map.has (i, &axis_tag)) return_trace (মিথ্যা);
      যদি (axes_location->has (*axis_tag, &axis_limit))
      {
        যদি (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (মিথ্যা);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        যদি (axis_limit->is_point () && !c->plan->axes_সূচক_map.has (i))
          চালু;
      }

      যদি (!c->serializer->embed (coords[i]))
        return_trace (মিথ্যা);
    }

    যদি (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      যদি (!c->serializer->embed (name_id))
        return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা axis_গুনতি) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_গুনতি));
  }

  সুরক্ষিত:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  UnsizedArrayOf<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  সর্বজনীন:
  DEFINE_আকার_UNBOUNDED (4);
};

কাঠামো AxisRecord
{
  পূর্ণসংখ্যা cmp (hb_tag_t key) ধ্রুবক { ফেরত axisTag.cmp (key); }

  গণনা
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ifndef HB_DISABLE_DEPRECATED
  শূন্য get_axis_deprecated (hb_ot_var_axis_t *info) ধ্রুবক
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_মান, info->default_মান, info->max_মান);
  }
#endif

  শূন্য get_axis_info (unsigned axis_সূচক, hb_ot_var_axis_info_t *info) ধ্রুবক
  {
    info->axis_সূচক = axis_সূচক;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (unsigned পূর্ণসংখ্যা) flags;
    get_coordinates (info->min_মান, info->default_মান, info->max_মান);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () ধ্রুবক { ফেরত axisTag; }

  যৌক্তিক is_hidden () ধ্রুবক { ফেরত flags & AXIS_FLAG_HIDDEN; }

  দশমিক normalize_axis_মান (দশমিক v) ধ্রুবক
  {
    দশমিক min_মান, default_মান, max_মান;
    get_coordinates (min_মান, default_মান, max_মান);

    v = hb_clamp (v, min_মান, max_মান);

    যদি (v == default_মান)
      ফেরত 0;
    অন্যথা যদি (v < default_মান)
      ফেরত (v - default_মান) / (default_মান - min_মান);
    অন্যথা
      ফেরত (v - default_মান) / (max_মান - default_মান);
  }

  hb_ot_name_id_t get_name_id () ধ্রুবক { ফেরত axisNameID; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য get_coordinates (দশমিক &min, দশমিক &default_, দশমিক &max) ধ্রুবক
  {
    default_ = default_মান_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_মান_2.to_float ());
    max = hb_max (default_, max_মান_2.to_float ());
  }

  দশমিক get_default () ধ্রুবক
  {
    ফেরত default_মান_2.to_float ();
  }

  TripleDistances get_triple_distances () ধ্রুবক
  {
    দশমিক min, default_, max;
    get_coordinates (min, default_, max);
    ফেরত TripleDistances (min, default_, max);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
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
    return_trace (সত্য);
  }

  সর্বজনীন:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  সুরক্ষিত:
  F16DOT16	min_মান_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_মান_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_মান_2;	/* The maximum coordinate value for the axis. */
  সর্বজনীন:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (20);
};

কাঠামো fvar
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.to_int (); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (এই) &&
		  hb_barrier () &&
		  axis_আকার == 20 && /* Assumed in our code. */
		  instance_আকার >= axis_গুনতি_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_গুনতি, instance_আকার));
  }

  unsigned পূর্ণসংখ্যা get_axis_গুনতি () ধ্রুবক { ফেরত axis_গুনতি_2; }

#ifndef HB_DISABLE_DEPRECATED
  unsigned পূর্ণসংখ্যা get_axes_deprecated (unsigned পূর্ণসংখ্যা      start_offset,
				    unsigned পূর্ণসংখ্যা     *axes_গুনতি /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) ধ্রুবক
  {
    যদি (axes_গুনতি && axes_array)
    {
      hb_array_t<ধ্রুবক AxisRecord> arr = get_axes ().sub_array (start_offset, axes_গুনতি);
      জন্য (unsigned i = 0; i < arr.দৈর্ঘ্য; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    ফেরত axis_গুনতি_2;
  }
#endif

  unsigned পূর্ণসংখ্যা get_axis_infos (unsigned পূর্ণসংখ্যা           start_offset,
			       unsigned পূর্ণসংখ্যা          *axes_গুনতি /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) ধ্রুবক
  {
    যদি (axes_গুনতি && axes_array)
    {
      hb_array_t<ধ্রুবক AxisRecord> arr = get_axes ().sub_array (start_offset, axes_গুনতি);
      জন্য (unsigned i = 0; i < arr.দৈর্ঘ্য; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    ফেরত axis_গুনতি_2;
  }

#ifndef HB_DISABLE_DEPRECATED
  যৌক্তিক
  find_axis_deprecated (hb_tag_t tag, unsigned *axis_সূচক, hb_ot_var_axis_t *info) ধ্রুবক
  {
    unsigned i;
    যদি (!axis_সূচক) axis_সূচক = &i;
    *axis_সূচক = HB_OT_VAR_NO_AXIS_সূচক;
    auto axes = get_axes ();
    ফেরত axes.lfind (tag, axis_সূচক) && ((শূন্য) axes[*axis_সূচক].get_axis_deprecated (info), সত্য);
  }
#endif
  যৌক্তিক
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) ধ্রুবক
  {
    unsigned i;
    auto axes = get_axes ();
    ফেরত axes.lfind (tag, &i) && ((শূন্য) axes[i].get_axis_info (i, info), সত্য);
  }

  দশমিক normalize_axis_মান (unsigned পূর্ণসংখ্যা axis_সূচক, দশমিক v) ধ্রুবক
  { ফেরত get_axes ()[axis_সূচক].normalize_axis_মান (v); }

  unsigned পূর্ণসংখ্যা get_instance_গুনতি () ধ্রুবক { ফেরত instance_গুনতি; }

  hb_ot_name_id_t get_instance_subfamily_name_id (unsigned পূর্ণসংখ্যা instance_সূচক) ধ্রুবক
  {
    ধ্রুবক InstanceRecord *instance = get_instance (instance_সূচক);
    যদি (unlikely (!instance)) ফেরত HB_OT_NAME_ID_INVALID;
    ফেরত instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (unsigned পূর্ণসংখ্যা instance_সূচক) ধ্রুবক
  {
    ধ্রুবক InstanceRecord *instance = get_instance (instance_সূচক);
    যদি (unlikely (!instance)) ফেরত HB_OT_NAME_ID_INVALID;
    যদি (instance_আকার >= axis_গুনতি_2 * 4 + 6)
      ফেরত StructAfter<NameID> (instance->get_coordinates (axis_গুনতি_2));
    ফেরত HB_OT_NAME_ID_INVALID;
  }

  unsigned পূর্ণসংখ্যা get_instance_coords (unsigned পূর্ণসংখ্যা  instance_সূচক,
				    unsigned পূর্ণসংখ্যা *coords_দৈর্ঘ্য, /* IN/OUT */
				    দশমিক        *coords         /* OUT */) ধ্রুবক
  {
    ধ্রুবক InstanceRecord *instance = get_instance (instance_সূচক);
    যদি (unlikely (!instance))
    {
      যদি (coords_দৈর্ঘ্য)
	*coords_দৈর্ঘ্য = 0;
      ফেরত 0;
    }

    যদি (coords_দৈর্ঘ্য && *coords_দৈর্ঘ্য && coords)
    {
      hb_array_t<ধ্রুবক F16DOT16> instanceCoords = instance->get_coordinates (axis_গুনতি_2)
							 .sub_array (0, coords_দৈর্ঘ্য);
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < instanceCoords.দৈর্ঘ্য; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    ফেরত axis_গুনতি_2;
  }

  শূন্য collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_সূচক_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) ধ্রুবক
  {
    যদি (!has_তথ্য ()) ফেরত;

    auto axis_records = get_axes ();
    জন্য (unsigned i = 0 ; i < (unsigned)axis_গুনতি_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      যদি (user_axes_location->has (axis_tag) &&
          user_axes_location->get (axis_tag).is_point ())
        চালু;

      nameids->add (axis_records[i].get_name_id ());
    }

    জন্য (unsigned i = 0 ; i < (unsigned)instance_গুনতি; i++)
    {
      ধ্রুবক InstanceRecord *instance = get_instance (i);

      যদি (!instance->keep_instance (axis_গুনতি_2, axes_old_সূচক_tag_map, user_axes_location))
        চালু;

      nameids->add (instance->subfamilyNameID);

      যদি (instance_আকার >= axis_গুনতি_2 * 4 + 6)
      {
        unsigned post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_গুনতি_2));
        যদি (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    unsigned retained_axis_গুনতি = c->plan->axes_সূচক_map.get_population ();
    যদি (!retained_axis_গুনতি) //all axes are pinned
      return_trace (মিথ্যা);

    fvar *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (!c->serializer->check_assign (out->axis_গুনতি_2, retained_axis_গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);

    যৌক্তিক has_postscript_nameid = মিথ্যা;
    যদি (instance_আকার >= axis_গুনতি_2 * 4 + 6)
      has_postscript_nameid = সত্য;

    যদি (!c->serializer->check_assign (out->instance_আকার, retained_axis_গুনতি * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);

    auto axes_records = get_axes ();
    জন্য (unsigned i = 0 ; i < (unsigned)axis_গুনতি_2; i++)
    {
      যদি (!c->plan->axes_সূচক_map.has (i)) চালু;
      যদি (unlikely (!axes_records[i].subset (c)))
        return_trace (মিথ্যা);
    }

    যদি (!c->serializer->check_assign (out->firstAxis, get_আকার (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);

    unsigned অঙ্ক_retained_instances = 0;
    জন্য (unsigned i = 0 ; i < (unsigned)instance_গুনতি; i++)
    {
      ধ্রুবক InstanceRecord *instance = get_instance (i);
      auto snap = c->serializer->snapshot ();
      যদি (!instance->subset (c, axis_গুনতি_2, has_postscript_nameid))
        c->serializer->revert (snap);
      অন্যথা
        অঙ্ক_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_গুনতি, অঙ্ক_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  সর্বজনীন:
  hb_array_t<ধ্রুবক AxisRecord> get_axes () ধ্রুবক
  { ফেরত hb_array (&(এই+firstAxis), axis_গুনতি_2); }

  ধ্রুবক InstanceRecord *get_instance (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  {
    যদি (unlikely (i >= instance_গুনতি)) ফেরত nullptr;
   ফেরত &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instance_আকার);
  }

  সুরক্ষিত:
  FixedVersion<>version;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16To<AxisRecord>
		firstAxis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_গুনতি_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axis_আকার;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_গুনতি;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instance_আকার;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (16);
};

} /* namespace OT */


#endif /* HB_OT_VAR_FVAR_TABLE_HH */
