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

#నిర్వచితం_కాకపోతే HB_OT_VAR_FVAR_పట్టిక_HH
#నిర్వచించు HB_OT_VAR_FVAR_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#నిర్వచించు HB_OT_ట్యాగ్_fvar HB_ట్యాగ్('f','v','a','r')


నామాకాశ OT {

స్థిర తార్కిక axis_coord_pinned_or_within_axis_range (స్థిరరాశి hb_array_t<స్థిరరాశి F16DOT16> coords,
                                                    గుర్తులేని axis_సూచీ,
                                                    Triple axis_హద్దు)
{
  దశాంశం axis_coord = coords[axis_సూచీ].గమ్యం_float ();
  ఒకవేళ (axis_హద్దు.is_బిందువు ())
  {
    ఒకవేళ (axis_హద్దు.minimum != axis_coord)
      తిరిగి అబద్ధం;
  }
  లేకపోతే
  {
    ఒకవేళ (axis_coord < axis_హద్దు.minimum ||
        axis_coord > axis_హద్దు.గరిష్ఠ)
      తిరిగి అబద్ధం;
  }
  తిరిగి నిజం;
}

నిర్మాణం Instance_రికార్డు
{
  మిత్ర నిర్మాణం fvar;

  hb_array_t<స్థిరరాశి F16DOT16> get_coordinates (గుర్తులేని పూర్ణాంకం axis_లెక్క) స్థిరరాశి
  { తిరిగి coordinatesZ.as_array (axis_లెక్క); }

  తార్కిక keep_instance (గుర్తులేని axis_లెక్క,
                      స్థిరరాశి hb_map_t *axes_సూచీ_ట్యాగ్_map,
                      స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *axes_location) స్థిరరాశి
  {
    ఒకవేళ (axes_location->is_empty ()) తిరిగి నిజం;
    స్థిరరాశి hb_array_t<స్థిరరాశి F16DOT16> coords = get_coordinates (axis_లెక్క);
    కొరకు (గుర్తులేని i = 0 ; i < axis_లెక్క; i++)
    {
      uint32_t *axis_ట్యాగ్_2;
      ఒకవేళ (!axes_సూచీ_ట్యాగ్_map->has (i, &axis_ట్యాగ్_2))
        తిరిగి అబద్ధం;
      ఒకవేళ (!axes_location->has (*axis_ట్యాగ్_2))
        కొనసాగు;

      Triple axis_హద్దు = axes_location->get (*axis_ట్యాగ్_2);
      ఒకవేళ (!axis_coord_pinned_or_within_axis_range (coords, i, axis_హద్దు))
        తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
  }

  తార్కిక subset (hb_subset_context_t *c,
               గుర్తులేని axis_లెక్క,
               తార్కిక has_postscript_nameid) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->serializer->embed (flags))) return_trace (అబద్ధం);

    స్థిరరాశి hb_array_t<స్థిరరాశి F16DOT16> coords = get_coordinates (axis_లెక్క);
    స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *axes_location = &c->plan->user_axes_location;
    కొరకు (గుర్తులేని i = 0 ; i < axis_లెక్క; i++)
    {
      uint32_t *axis_ట్యాగ్_2;
      Triple *axis_హద్దు;
      // only keep instances whose coordinates == pinned axis location
      ఒకవేళ (!c->plan->axes_old_సూచీ_ట్యాగ్_map.has (i, &axis_ట్యాగ్_2)) return_trace (అబద్ధం);
      ఒకవేళ (axes_location->has (*axis_ట్యాగ్_2, &axis_హద్దు))
      {
        ఒకవేళ (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_హద్దు))
          return_trace (అబద్ధం);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        ఒకవేళ (axis_హద్దు->is_బిందువు () && !c->plan->axes_సూచీ_map.has (i))
          కొనసాగు;
      }

      ఒకవేళ (!c->serializer->embed (coords[i]))
        return_trace (అబద్ధం);
    }

    ఒకవేళ (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      ఒకవేళ (!c->serializer->embed (name_id))
        return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం axis_లెక్క) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_లెక్క));
  }

  రక్షిత:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  Unsized_Array_యొక్క<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  బహిరంగ:
  DEFINE_కొలత_UNBOUNDED (4);
};

నిర్మాణం Axis_రికార్డు
{
  పూర్ణాంకం cmp (hb_ట్యాగ్_t key) స్థిరరాశి { తిరిగి axis_ట్యాగ్.cmp (key); }

  గణన
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
  శూన్య get_axis_deprecated (hb_ot_var_axis_t *info) స్థిరరాశి
  {
    info->ట్యాగ్_2 = axis_ట్యాగ్;
    info->name_id = axisNameID;
    get_coordinates (info->min_విలువ, info->default_విలువ, info->max_విలువ);
  }
#అయితే_ముగింపు

  శూన్య get_axis_info (గుర్తులేని axis_సూచీ, hb_ot_var_axis_info_t *info) స్థిరరాశి
  {
    info->axis_సూచీ = axis_సూచీ;
    info->ట్యాగ్_2 = axis_ట్యాగ్;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (గుర్తులేని పూర్ణాంకం) flags;
    get_coordinates (info->min_విలువ, info->default_విలువ, info->max_విలువ);
    info->reserved = 0;
  }

  hb_ట్యాగ్_t get_axis_ట్యాగ్ () స్థిరరాశి { తిరిగి axis_ట్యాగ్; }

  తార్కిక is_hidden () స్థిరరాశి { తిరిగి flags & AXIS_FLAG_HIDDEN; }

  దశాంశం normalize_axis_విలువ (దశాంశం v) స్థిరరాశి
  {
    దశాంశం min_విలువ, default_విలువ, max_విలువ;
    get_coordinates (min_విలువ, default_విలువ, max_విలువ);

    v = hb_clamp (v, min_విలువ, max_విలువ);

    ఒకవేళ (v == default_విలువ)
      తిరిగి 0;
    లేకపోతే ఒకవేళ (v < default_విలువ)
      తిరిగి (v - default_విలువ) / (default_విలువ - min_విలువ);
    లేకపోతే
      తిరిగి (v - default_విలువ) / (max_విలువ - default_విలువ);
  }

  hb_ot_name_id_t get_name_id () స్థిరరాశి { తిరిగి axisNameID; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య get_coordinates (దశాంశం &min, దశాంశం &default_, దశాంశం &max) స్థిరరాశి
  {
    default_ = default_విలువ_2.గమ్యం_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_విలువ_2.గమ్యం_float ());
    max = hb_max (default_, max_విలువ_2.గమ్యం_float ());
  }

  దశాంశం get_default () స్థిరరాశి
  {
    తిరిగి default_విలువ_2.గమ్యం_float ();
  }

  TripleDistances get_triple_distances () స్థిరరాశి
  {
    దశాంశం min, default_, max;
    get_coordinates (min, default_, max);
    తిరిగి TripleDistances (min, default_, max);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_హద్దు;
    ఒకవేళ (user_axes_location.has (axis_ట్యాగ్, &axis_హద్దు))
    {
      ఒకవేళ (c->plan->has_avar2 && axis_హద్దు->is_బిందువు ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        బహిర్గతం->min_విలువ_2.సమితి_float (axis_హద్దు->మధ్య);
        బహిర్గతం->default_విలువ_2.సమితి_float (axis_హద్దు->మధ్య);
        బహిర్గతం->max_విలువ_2.సమితి_float (axis_హద్దు->మధ్య);
        బహిర్గతం->flags = బహిర్గతం->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      లేకపోతే
      {
        బహిర్గతం->min_విలువ_2.సమితి_float (axis_హద్దు->minimum);
        బహిర్గతం->default_విలువ_2.సమితి_float (axis_హద్దు->మధ్య);
        బహిర్గతం->max_విలువ_2.సమితి_float (axis_హద్దు->గరిష్ఠ);
      }
    }
    return_trace (నిజం);
  }

  బహిరంగ:
  ట్యాగ్		axis_ట్యాగ్;	/* Tag identifying the design variation for the axis. */
  రక్షిత:
  F16DOT16	min_విలువ_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_విలువ_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_విలువ_2;	/* The maximum coordinate value for the axis. */
  బహిరంగ:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (20);
};

నిర్మాణం fvar
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_fvar;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int (); }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (సంస్కరణ.sanitize (c) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 1) &&
		  c->check_struct (ఇది) &&
		  hb_barrier () &&
		  axis_కొలత == 20 && /* Assumed in our code. */
		  instance_కొలత >= axis_లెక్క_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<Instance_రికార్డు> (get_axes ()),
				  instance_లెక్క, instance_కొలత));
  }

  గుర్తులేని పూర్ణాంకం get_axis_లెక్క () స్థిరరాశి { తిరిగి axis_లెక్క_2; }

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
  గుర్తులేని పూర్ణాంకం get_axes_deprecated (గుర్తులేని పూర్ణాంకం      ప్రారంభం_స్థానభ్రంశం,
				    గుర్తులేని పూర్ణాంకం     *axes_లెక్క /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (axes_లెక్క && axes_array)
    {
      hb_array_t<స్థిరరాశి Axis_రికార్డు> arr = get_axes ().sub_array (ప్రారంభం_స్థానభ్రంశం, axes_లెక్క);
      కొరకు (గుర్తులేని i = 0; i < arr.నిడివి; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    తిరిగి axis_లెక్క_2;
  }
#అయితే_ముగింపు

  గుర్తులేని పూర్ణాంకం get_axis_infos (గుర్తులేని పూర్ణాంకం           ప్రారంభం_స్థానభ్రంశం,
			       గుర్తులేని పూర్ణాంకం          *axes_లెక్క /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (axes_లెక్క && axes_array)
    {
      hb_array_t<స్థిరరాశి Axis_రికార్డు> arr = get_axes ().sub_array (ప్రారంభం_స్థానభ్రంశం, axes_లెక్క);
      కొరకు (గుర్తులేని i = 0; i < arr.నిడివి; ++i)
	arr[i].get_axis_info (ప్రారంభం_స్థానభ్రంశం + i, &axes_array[i]);
    }
    తిరిగి axis_లెక్క_2;
  }

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
  తార్కిక
  కనుగొను_axis_deprecated (hb_ట్యాగ్_t ట్యాగ్_2, గుర్తులేని *axis_సూచీ, hb_ot_var_axis_t *info) స్థిరరాశి
  {
    గుర్తులేని i;
    ఒకవేళ (!axis_సూచీ) axis_సూచీ = &i;
    *axis_సూచీ = HB_OT_VAR_NO_AXIS_సూచీ;
    స్వయంచాలక axes = get_axes ();
    తిరిగి axes.lfind (ట్యాగ్_2, axis_సూచీ) && ((శూన్య) axes[*axis_సూచీ].get_axis_deprecated (info), నిజం);
  }
#అయితే_ముగింపు
  తార్కిక
  కనుగొను_axis_info (hb_ట్యాగ్_t ట్యాగ్_2, hb_ot_var_axis_info_t *info) స్థిరరాశి
  {
    గుర్తులేని i;
    స్వయంచాలక axes = get_axes ();
    తిరిగి axes.lfind (ట్యాగ్_2, &i) && ((శూన్య) axes[i].get_axis_info (i, info), నిజం);
  }

  దశాంశం normalize_axis_విలువ (గుర్తులేని పూర్ణాంకం axis_సూచీ, దశాంశం v) స్థిరరాశి
  { తిరిగి get_axes ()[axis_సూచీ].normalize_axis_విలువ (v); }

  గుర్తులేని పూర్ణాంకం get_instance_లెక్క () స్థిరరాశి { తిరిగి instance_లెక్క; }

  hb_ot_name_id_t get_instance_subfamily_name_id (గుర్తులేని పూర్ణాంకం instance_సూచీ) స్థిరరాశి
  {
    స్థిరరాశి Instance_రికార్డు *instance = get_instance (instance_సూచీ);
    ఒకవేళ (unlikely (!instance)) తిరిగి HB_OT_NAME_ID_INVALID;
    తిరిగి instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (గుర్తులేని పూర్ణాంకం instance_సూచీ) స్థిరరాశి
  {
    స్థిరరాశి Instance_రికార్డు *instance = get_instance (instance_సూచీ);
    ఒకవేళ (unlikely (!instance)) తిరిగి HB_OT_NAME_ID_INVALID;
    ఒకవేళ (instance_కొలత >= axis_లెక్క_2 * 4 + 6)
      తిరిగి StructAfter<NameID> (instance->get_coordinates (axis_లెక్క_2));
    తిరిగి HB_OT_NAME_ID_INVALID;
  }

  గుర్తులేని పూర్ణాంకం get_instance_coords (గుర్తులేని పూర్ణాంకం  instance_సూచీ,
				    గుర్తులేని పూర్ణాంకం *coords_నిడివి, /* IN/OUT */
				    దశాంశం        *coords         /*బహిర్గతం*/) స్థిరరాశి
  {
    స్థిరరాశి Instance_రికార్డు *instance = get_instance (instance_సూచీ);
    ఒకవేళ (unlikely (!instance))
    {
      ఒకవేళ (coords_నిడివి)
	*coords_నిడివి = 0;
      తిరిగి 0;
    }

    ఒకవేళ (coords_నిడివి && *coords_నిడివి && coords)
    {
      hb_array_t<స్థిరరాశి F16DOT16> instanceCoords = instance->get_coordinates (axis_లెక్క_2)
							 .sub_array (0, coords_నిడివి);
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < instanceCoords.నిడివి; i++)
	coords[i] = instanceCoords.arrayZ[i].గమ్యం_float ();
    }
    తిరిగి axis_లెక్క_2;
  }

  శూన్య collect_name_ids (hb_hashmap_t<hb_ట్యాగ్_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_సూచీ_ట్యాగ్_map,
			 hb_సమితి_t *nameids  /* IN/OUT */) స్థిరరాశి
  {
    ఒకవేళ (!has_దత్తాంశం ()) తిరిగి;

    స్వయంచాలక axis_రికార్డులు = get_axes ();
    కొరకు (గుర్తులేని i = 0 ; i < (గుర్తులేని)axis_లెక్క_2; i++)
    {
      hb_ట్యాగ్_t axis_ట్యాగ్_2 = axis_రికార్డులు[i].get_axis_ట్యాగ్ ();
      ఒకవేళ (user_axes_location->has (axis_ట్యాగ్_2) &&
          user_axes_location->get (axis_ట్యాగ్_2).is_బిందువు ())
        కొనసాగు;

      nameids->కూడు (axis_రికార్డులు[i].get_name_id ());
    }

    కొరకు (గుర్తులేని i = 0 ; i < (గుర్తులేని)instance_లెక్క; i++)
    {
      స్థిరరాశి Instance_రికార్డు *instance = get_instance (i);

      ఒకవేళ (!instance->keep_instance (axis_లెక్క_2, axes_old_సూచీ_ట్యాగ్_map, user_axes_location))
        కొనసాగు;

      nameids->కూడు (instance->subfamilyNameID);

      ఒకవేళ (instance_కొలత >= axis_లెక్క_2 * 4 + 6)
      {
        గుర్తులేని post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_లెక్క_2));
        ఒకవేళ (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->కూడు (post_script_name_id);
      }
    }
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    గుర్తులేని retained_axis_లెక్క = c->plan->axes_సూచీ_map.get_population ();
    ఒకవేళ (!retained_axis_లెక్క) //all axes are pinned
      return_trace (అబద్ధం);

    fvar *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->axis_లెక్క_2, retained_axis_లెక్క, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);

    తార్కిక has_postscript_nameid = అబద్ధం;
    ఒకవేళ (instance_కొలత >= axis_లెక్క_2 * 4 + 6)
      has_postscript_nameid = నిజం;

    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->instance_కొలత, retained_axis_లెక్క * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);

    స్వయంచాలక axes_రికార్డులు = get_axes ();
    కొరకు (గుర్తులేని i = 0 ; i < (గుర్తులేని)axis_లెక్క_2; i++)
    {
      ఒకవేళ (!c->plan->axes_సూచీ_map.has (i)) కొనసాగు;
      ఒకవేళ (unlikely (!axes_రికార్డులు[i].subset (c)))
        return_trace (అబద్ధం);
    }

    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->మొదటి_Axis, get_కొలత (), HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);

    గుర్తులేని అంకె_retained_instances = 0;
    కొరకు (గుర్తులేని i = 0 ; i < (గుర్తులేని)instance_లెక్క; i++)
    {
      స్థిరరాశి Instance_రికార్డు *instance = get_instance (i);
      స్వయంచాలక snap = c->serializer->snapshot ();
      ఒకవేళ (!instance->subset (c, axis_లెక్క_2, has_postscript_nameid))
        c->serializer->revert (snap);
      లేకపోతే
        అంకె_retained_instances++;
    }

    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->instance_లెక్క, అంకె_retained_instances, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  బహిరంగ:
  hb_array_t<స్థిరరాశి Axis_రికార్డు> get_axes () స్థిరరాశి
  { తిరిగి hb_array (&(ఇది+మొదటి_Axis), axis_లెక్క_2); }

  స్థిరరాశి Instance_రికార్డు *get_instance (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  {
    ఒకవేళ (unlikely (i >= instance_లెక్క)) తిరిగి శూన్యసూచిక;
   తిరిగి &Struct_At_స్థానభ్రంశం<Instance_రికార్డు> (&StructAfter<Instance_రికార్డు> (get_axes ()),
					   i * instance_కొలత);
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16_గమ్యం<Axis_రికార్డు>
		మొదటి_Axis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_లెక్క_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axis_కొలత;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_లెక్క;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instance_కొలత;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (16);
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_VAR_FVAR_TABLE_HH */
