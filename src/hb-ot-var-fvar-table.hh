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

#अगर_अघोषित HB_OT_VAR_FVAR_TABLE_HH
#घोषणा HB_OT_VAR_FVAR_TABLE_HH

#समावेश "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#घोषणा HB_OT_TAG_fvar HB_TAG('f','v','a','r')


namespace OT {

अटल bool axis_coord_pinned_or_within_axis_range (स्थिर hb_array_t<स्थिर F16DOT16> coords,
                                                    अचिन्हित axis_सूचकांक,
                                                    Triple axis_limit)
{
  भग्न axis_coord = coords[axis_सूचकांक].to_float ();
  अगर (axis_limit.is_point ())
  {
    अगर (axis_limit.minimum != axis_coord)
      वापस false;
  }
  अन्यथा
  {
    अगर (axis_coord < axis_limit.minimum ||
        axis_coord > axis_limit.maximum)
      वापस false;
  }
  वापस true;
}

काष्ठा InstanceRecord
{
  मित्र काष्ठा fvar;

  hb_array_t<स्थिर F16DOT16> get_coordinates (अचिन्हित पूर्णांक axis_गिनती) स्थिर
  { वापस coordinatesZ.as_array (axis_गिनती); }

  bool keep_instance (अचिन्हित axis_गिनती,
                      स्थिर hb_map_t *axes_सूचकांक_tag_map,
                      स्थिर hb_hashmap_t<hb_tag_t, Triple> *axes_location) स्थिर
  {
    अगर (axes_location->is_empty ()) वापस true;
    स्थिर hb_array_t<स्थिर F16DOT16> coords = get_coordinates (axis_गिनती);
    क्रम (अचिन्हित i = 0 ; i < axis_गिनती; i++)
    {
      uint32_t *axis_tag;
      अगर (!axes_सूचकांक_tag_map->has (i, &axis_tag))
        वापस false;
      अगर (!axes_location->has (*axis_tag))
        जारी;

      Triple axis_limit = axes_location->लो (*axis_tag);
      अगर (!axis_coord_pinned_or_within_axis_range (coords, i, axis_limit))
        वापस false;
    }
    वापस true;
  }

  bool subset (hb_subset_context_t *c,
               अचिन्हित axis_गिनती,
               bool has_postscript_nameid) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (false);
    अगर (unlikely (!c->serializer->embed (flags))) return_trace (false);

    स्थिर hb_array_t<स्थिर F16DOT16> coords = get_coordinates (axis_गिनती);
    स्थिर hb_hashmap_t<hb_tag_t, Triple> *axes_location = &c->plan->user_axes_location;
    क्रम (अचिन्हित i = 0 ; i < axis_गिनती; i++)
    {
      uint32_t *axis_tag;
      Triple *axis_limit;
      // only keep instances whose coordinates == pinned axis location
      अगर (!c->plan->axes_old_सूचकांक_tag_map.has (i, &axis_tag)) return_trace (false);
      अगर (axes_location->has (*axis_tag, &axis_limit))
      {
        अगर (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_limit))
          return_trace (false);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        अगर (axis_limit->is_point () && !c->plan->axes_सूचकांक_map.has (i))
          जारी;
      }

      अगर (!c->serializer->embed (coords[i]))
        return_trace (false);
    }

    अगर (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      अगर (!c->serializer->embed (name_id))
        return_trace (false);
    }

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक axis_गिनती) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
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

  खुला:
  DEFINE_आकार_UNBOUNDED (4);
};

काष्ठा AxisRecord
{
  पूर्णांक cmp (hb_tag_t key) स्थिर { वापस axisTag.cmp (key); }

  क्रमागत
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#अगर_अघोषित HB_DISABLE_DEPRECATED
  व्योम get_axis_deprecated (hb_ot_var_axis_t *info) स्थिर
  {
    info->tag = axisTag;
    info->name_id = axisNameID;
    get_coordinates (info->min_मूल्य, info->default_मूल्य, info->max_मूल्य);
  }
#पूर्ण_अगर

  व्योम get_axis_info (अचिन्हित axis_सूचकांक, hb_ot_var_axis_info_t *info) स्थिर
  {
    info->axis_सूचकांक = axis_सूचकांक;
    info->tag = axisTag;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (अचिन्हित पूर्णांक) flags;
    get_coordinates (info->min_मूल्य, info->default_मूल्य, info->max_मूल्य);
    info->reserved = 0;
  }

  hb_tag_t get_axis_tag () स्थिर { वापस axisTag; }

  bool is_hidden () स्थिर { वापस flags & AXIS_FLAG_HIDDEN; }

  भग्न normalize_axis_मूल्य (भग्न v) स्थिर
  {
    भग्न min_मूल्य, default_मूल्य, max_मूल्य;
    get_coordinates (min_मूल्य, default_मूल्य, max_मूल्य);

    v = hb_clamp (v, min_मूल्य, max_मूल्य);

    अगर (v == default_मूल्य)
      वापस 0;
    अन्यथा अगर (v < default_मूल्य)
      वापस (v - default_मूल्य) / (default_मूल्य - min_मूल्य);
    अन्यथा
      वापस (v - default_मूल्य) / (max_मूल्य - default_मूल्य);
  }

  hb_ot_name_id_t get_name_id () स्थिर { वापस axisNameID; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम get_coordinates (भग्न &min, भग्न &default_, भग्न &max) स्थिर
  {
    default_ = default_मूल्य_2.to_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_मूल्य_2.to_float ());
    max = hb_max (default_, max_मूल्य_2.to_float ());
  }

  भग्न get_default () स्थिर
  {
    वापस default_मूल्य_2.to_float ();
  }

  TripleDistances get_triple_distances () स्थिर
  {
    भग्न min, default_, max;
    get_coordinates (min, default_, max);
    वापस TripleDistances (min, default_, max);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    स्थिर hb_hashmap_t<hb_tag_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_limit;
    अगर (user_axes_location.has (axisTag, &axis_limit))
    {
      अगर (c->plan->has_avar2 && axis_limit->is_point ())
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
    return_trace (true);
  }

  खुला:
  Tag		axisTag;	/* Tag identifying the design variation for the axis. */
  रक्षित:
  F16DOT16	min_मूल्य_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_मूल्य_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_मूल्य_2;	/* The maximum coordinate value for the axis. */
  खुला:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  खुला:
  DEFINE_आकार_STATIC (20);
};

काष्ठा fvar
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_fvar;

  bool has_तथ्य () स्थिर { वापस version.to_int (); }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (यह) &&
		  hb_barrier () &&
		  axis_आकार == 20 && /* Assumed in our code. */
		  instance_आकार >= axis_गिनती_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<InstanceRecord> (get_axes ()),
				  instance_गिनती, instance_आकार));
  }

  अचिन्हित पूर्णांक get_axis_गिनती () स्थिर { वापस axis_गिनती_2; }

#अगर_अघोषित HB_DISABLE_DEPRECATED
  अचिन्हित पूर्णांक get_axes_deprecated (अचिन्हित पूर्णांक      start_offset,
				    अचिन्हित पूर्णांक     *axes_गिनती /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /* OUT */) स्थिर
  {
    अगर (axes_गिनती && axes_array)
    {
      hb_array_t<स्थिर AxisRecord> arr = get_axes ().sub_array (start_offset, axes_गिनती);
      क्रम (अचिन्हित i = 0; i < arr.लंबाई; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    वापस axis_गिनती_2;
  }
#पूर्ण_अगर

  अचिन्हित पूर्णांक get_axis_infos (अचिन्हित पूर्णांक           start_offset,
			       अचिन्हित पूर्णांक          *axes_गिनती /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /* OUT */) स्थिर
  {
    अगर (axes_गिनती && axes_array)
    {
      hb_array_t<स्थिर AxisRecord> arr = get_axes ().sub_array (start_offset, axes_गिनती);
      क्रम (अचिन्हित i = 0; i < arr.लंबाई; ++i)
	arr[i].get_axis_info (start_offset + i, &axes_array[i]);
    }
    वापस axis_गिनती_2;
  }

#अगर_अघोषित HB_DISABLE_DEPRECATED
  bool
  find_axis_deprecated (hb_tag_t tag, अचिन्हित *axis_सूचकांक, hb_ot_var_axis_t *info) स्थिर
  {
    अचिन्हित i;
    अगर (!axis_सूचकांक) axis_सूचकांक = &i;
    *axis_सूचकांक = HB_OT_VAR_NO_AXIS_सूचकांक;
    स्वतः axes = get_axes ();
    वापस axes.lfind (tag, axis_सूचकांक) && ((व्योम) axes[*axis_सूचकांक].get_axis_deprecated (info), true);
  }
#पूर्ण_अगर
  bool
  find_axis_info (hb_tag_t tag, hb_ot_var_axis_info_t *info) स्थिर
  {
    अचिन्हित i;
    स्वतः axes = get_axes ();
    वापस axes.lfind (tag, &i) && ((व्योम) axes[i].get_axis_info (i, info), true);
  }

  भग्न normalize_axis_मूल्य (अचिन्हित पूर्णांक axis_सूचकांक, भग्न v) स्थिर
  { वापस get_axes ()[axis_सूचकांक].normalize_axis_मूल्य (v); }

  अचिन्हित पूर्णांक get_instance_गिनती () स्थिर { वापस instance_गिनती; }

  hb_ot_name_id_t get_instance_subfamily_name_id (अचिन्हित पूर्णांक instance_सूचकांक) स्थिर
  {
    स्थिर InstanceRecord *instance = get_instance (instance_सूचकांक);
    अगर (unlikely (!instance)) वापस HB_OT_NAME_ID_INVALID;
    वापस instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (अचिन्हित पूर्णांक instance_सूचकांक) स्थिर
  {
    स्थिर InstanceRecord *instance = get_instance (instance_सूचकांक);
    अगर (unlikely (!instance)) वापस HB_OT_NAME_ID_INVALID;
    अगर (instance_आकार >= axis_गिनती_2 * 4 + 6)
      वापस StructAfter<NameID> (instance->get_coordinates (axis_गिनती_2));
    वापस HB_OT_NAME_ID_INVALID;
  }

  अचिन्हित पूर्णांक get_instance_coords (अचिन्हित पूर्णांक  instance_सूचकांक,
				    अचिन्हित पूर्णांक *coords_लंबाई, /* IN/OUT */
				    भग्न        *coords         /* OUT */) स्थिर
  {
    स्थिर InstanceRecord *instance = get_instance (instance_सूचकांक);
    अगर (unlikely (!instance))
    {
      अगर (coords_लंबाई)
	*coords_लंबाई = 0;
      वापस 0;
    }

    अगर (coords_लंबाई && *coords_लंबाई && coords)
    {
      hb_array_t<स्थिर F16DOT16> instanceCoords = instance->get_coordinates (axis_गिनती_2)
							 .sub_array (0, coords_लंबाई);
      क्रम (अचिन्हित पूर्णांक i = 0; i < instanceCoords.लंबाई; i++)
	coords[i] = instanceCoords.arrayZ[i].to_float ();
    }
    वापस axis_गिनती_2;
  }

  व्योम collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_सूचकांक_tag_map,
			 hb_set_t *nameids  /* IN/OUT */) स्थिर
  {
    अगर (!has_तथ्य ()) वापस;

    स्वतः axis_records = get_axes ();
    क्रम (अचिन्हित i = 0 ; i < (अचिन्हित)axis_गिनती_2; i++)
    {
      hb_tag_t axis_tag = axis_records[i].get_axis_tag ();
      अगर (user_axes_location->has (axis_tag) &&
          user_axes_location->लो (axis_tag).is_point ())
        जारी;

      nameids->add (axis_records[i].get_name_id ());
    }

    क्रम (अचिन्हित i = 0 ; i < (अचिन्हित)instance_गिनती; i++)
    {
      स्थिर InstanceRecord *instance = get_instance (i);

      अगर (!instance->keep_instance (axis_गिनती_2, axes_old_सूचकांक_tag_map, user_axes_location))
        जारी;

      nameids->add (instance->subfamilyNameID);

      अगर (instance_आकार >= axis_गिनती_2 * 4 + 6)
      {
        अचिन्हित post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_गिनती_2));
        अगर (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->add (post_script_name_id);
      }
    }
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    अचिन्हित retained_axis_गिनती = c->plan->axes_सूचकांक_map.get_population ();
    अगर (!retained_axis_गिनती) //all axes are pinned
      return_trace (false);

    fvar *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (!c->serializer->check_assign (out->axis_गिनती_2, retained_axis_गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    bool has_postscript_nameid = false;
    अगर (instance_आकार >= axis_गिनती_2 * 4 + 6)
      has_postscript_nameid = true;

    अगर (!c->serializer->check_assign (out->instance_आकार, retained_axis_गिनती * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    स्वतः axes_records = get_axes ();
    क्रम (अचिन्हित i = 0 ; i < (अचिन्हित)axis_गिनती_2; i++)
    {
      अगर (!c->plan->axes_सूचकांक_map.has (i)) जारी;
      अगर (unlikely (!axes_records[i].subset (c)))
        return_trace (false);
    }

    अगर (!c->serializer->check_assign (out->firstAxis, get_आकार (), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    अचिन्हित अंक_retained_instances = 0;
    क्रम (अचिन्हित i = 0 ; i < (अचिन्हित)instance_गिनती; i++)
    {
      स्थिर InstanceRecord *instance = get_instance (i);
      स्वतः snap = c->serializer->snapshot ();
      अगर (!instance->subset (c, axis_गिनती_2, has_postscript_nameid))
        c->serializer->revert (snap);
      अन्यथा
        अंक_retained_instances++;
    }

    return_trace (c->serializer->check_assign (out->instance_गिनती, अंक_retained_instances, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  खुला:
  hb_array_t<स्थिर AxisRecord> get_axes () स्थिर
  { वापस hb_array (&(यह+firstAxis), axis_गिनती_2); }

  स्थिर InstanceRecord *get_instance (अचिन्हित पूर्णांक i) स्थिर
  {
    अगर (unlikely (i >= instance_गिनती)) वापस nullptr;
   वापस &StructAtOffset<InstanceRecord> (&StructAfter<InstanceRecord> (get_axes ()),
					   i * instance_आकार);
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
  HBUINT16	axis_आकार;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_गिनती;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instance_आकार;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  खुला:
  DEFINE_आकार_STATIC (16);
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_VAR_FVAR_TABLE_HH */
