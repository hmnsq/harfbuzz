#నిర్వచితం_కాకపోతే OT_VAR_VARC_VARC_SUBSET_HH
#నిర్వచించు OT_VAR_VARC_VARC_SUBSET_HH

#అన్తర్భావ "VARC.hh"

నామాకాశ OT {

నిర్మాణం varc_subset_plan_t
{
  varc_subset_plan_t (స్థిరరాశి VARC &ఆధారం_3,
		      hb_subset_context_t *subset_context_,
		      స్థిరరాశి Coverage &ఆధారం_coverage,
		      స్థిరరాశి CFF2_సూచీ &ఆధారం_రికార్డులు,
		      స్థిరరాశి ConditionList &ఆధారం_conditions,
		      స్థిరరాశి CFF2_సూచీ &ఆధారం_axis_indices,
		      స్థిరరాశి Multi_అంశం_Variation_Store &ఆధారం_var_store) :
    ఆధారం (ఆధారం_3),
    subset_context (subset_context_),
    ఆధారం_coverage_2 (ఆధారం_coverage),
    ఆధారం_రికార్డులు_2 (ఆధారం_రికార్డులు),
    ఆధారం_conditions_2 (ఆధారం_conditions),
    ఆధారం_axis_indices_2 (ఆధారం_axis_indices),
    ఆధారం_var_store_2 (ఆధారం_var_store)
  {}

  తార్కిక collect_glyphs ()
  {
    స్థిరరాశి hb_సమితి_t &glyphset = subset_context->plan->_glyphset_varced;
    కొరకు (స్వయంచాలక _ : + hb_zip (ఆధారం_coverage_2,
			    hb_range ((గుర్తులేని) ఆధారం_రికార్డులు_2.లెక్క)))
    {
      hb_codepoint_t old_gid = _.మొదటి;
      గుర్తులేని సూచీ_2 = _.second;
      ఒకవేళ (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	ఒకవేళ (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  తిరిగి అబద్ధం;
	old_indices.push (సూచీ_2);
	new_gids.push (new_gid);
	దత్తాంశం_కొలత = hb_unsigned_కూడు_saturate (దత్తాంశం_కొలత,
					      ఆధారం_రికార్డులు_2[సూచీ_2].నిడివి);
      }
    }

    తిరిగి !old_indices.లోపల_error () && !new_gids.లోపల_error () &&
	   దత్తాంశం_కొలత != UINT_MAX;
  }

  తార్కిక collect_indices ()
  {
    కొరకు (గుర్తులేని old_సూచీ : old_indices)
    {
      hb_ubytes_t మిగిలిన = ఆధారం_రికార్డులు_2[old_సూచీ];
      అయితే (మిగిలిన)
      {
	VarComponent::రికార్డు_t component;
	ఒకవేళ (unlikely (!VarComponent::decompile_రికార్డు (ఆధారం, మిగిలిన,
						       శూన్యసూచిక, శూన్యసూచిక,
						       &component)))
	  తిరిగి అబద్ధం;

	ఒకవేళ (component.flags & (గుర్తులేని) VarComponent::flags_t::HAVE_CONDITION)
	{
	  ఒకవేళ (unlikely (component.condition_సూచీ >= ఆధారం_conditions_2.get_లెక్క ()))
	    తిరిగి అబద్ధం;
	  condition_indices.కూడు (component.condition_సూచీ);
	}
	ఒకవేళ (component.flags & (గుర్తులేని) VarComponent::flags_t::HAVE_AXES)
	{
	  ఒకవేళ (unlikely (component.axis_indices_సూచీ >= ఆధారం_axis_indices_2.లెక్క))
	    తిరిగి అబద్ధం;
	  axis_indices.కూడు (component.axis_indices_సూచీ);
	}
	ఒకవేళ ((component.flags & (గుర్తులేని) VarComponent::flags_t::AXIS_విలువలు_HAVE_VARIATION) &&
	    component.axis_విలువలు_var_idx != VarIdx::NO_VARIATION)
	  var_indices.కూడు (component.axis_విలువలు_var_idx);
	ఒకవేళ ((component.flags & (గుర్తులేని) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.కూడు (component.transform_var_idx);

	మిగిలిన = మిగిలిన.sub_array (component.కొలత);
      }
    }

    ఒకవేళ (unlikely (condition_indices.లోపల_error () || axis_indices.లోపల_error () ||
		  var_indices.లోపల_error ()))
      తిరిగి అబద్ధం;

    కొరకు (hb_codepoint_t condition_సూచీ : condition_indices)
      ఒకవేళ (unlikely (!ఆధారం_conditions_2[condition_సూచీ].collect_var_indices (&var_indices)))
	తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }

  తార్కిక create_maps ()
  {
    స్థిరరాశి స్వయంచాలక *plan = subset_context->plan;
    ఒకవేళ (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_సూచీ_map;
      కొరకు (స్వయంచాలక _ : plan->axes_సూచీ_map)
	ఒకవేళ (!plan->user_axes_location.has (plan->axes_old_సూచీ_ట్యాగ్_map.get (_.మొదటి)))
	  axes_సూచీ_map.సమితి (_.మొదటి, _.second);
      ఒకవేళ (unlikely (axes_సూచీ_map.లోపల_error ())) తిరిగి అబద్ధం;
    }
    condition_map.కూడు_సమితి (&condition_indices);
    axis_indices_map.కూడు_సమితి (&axis_indices);
    తిరిగి !condition_map.లోపల_error () && !axis_indices_map.లోపల_error () &&
	   ఆధారం_var_store_2.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  తార్కిక compile_రికార్డులు ()
  {
    ఒకవేళ (unlikely (!రికార్డులు.alloc_exact (old_indices.నిడివి)))
      తిరిగి అబద్ధం;

    తార్కిక retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    ఒకవేళ (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      కొరకు (గుర్తులేని old_సూచీ : old_indices)
	రికార్డులు.push (ఆధారం_రికార్డులు_2[old_సూచీ]);
    }
    లేకపోతే
    {
      hb_సదిశ_t<గుర్తులేని> రికార్డు_offsets;
      రికార్డు_దత్తాంశం.alloc (దత్తాంశం_కొలత);
      ఒకవేళ (unlikely (!రికార్డు_offsets.alloc_exact (old_indices.నిడివి + 1)))
	తిరిగి అబద్ధం;

      కొరకు (గుర్తులేని old_సూచీ : old_indices)
      {
	రికార్డు_offsets.push (రికార్డు_దత్తాంశం.నిడివి);
	ఒకవేళ (unlikely (!compile_రికార్డు (ఆధారం_రికార్డులు_2[old_సూచీ], retain_gids)))
	  తిరిగి అబద్ధం;
      }
      రికార్డు_offsets.push (రికార్డు_దత్తాంశం.నిడివి);

      ఒకవేళ (unlikely (రికార్డు_దత్తాంశం.లోపల_error () || రికార్డు_offsets.లోపల_error ()))
	తిరిగి అబద్ధం;
      కొరకు (గుర్తులేని i = 0; i < old_indices.నిడివి; i++)
	రికార్డులు.push (hb_ubytes_t (రికార్డు_దత్తాంశం.arrayZ + రికార్డు_offsets[i],
				  రికార్డు_offsets[i + 1] - రికార్డు_offsets[i]));
      దత్తాంశం_కొలత = రికార్డు_దత్తాంశం.నిడివి;
    }

    తిరిగి !రికార్డులు.లోపల_error ();
  }

  తార్కిక select_axis_indices ()
  {
    ఒకవేళ (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_దత్తాంశం.resize (
					  axis_indices_map.get_population ()))))
      తిరిగి అబద్ధం;
    కొరకు (గుర్తులేని i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t బైట్లు = ఆధారం_axis_indices_2[axis_indices_map.backward (i)];
      ఒకవేళ (axis_map)
      {
	hb_సదిశ_t<పూర్ణాంకం> indices;
	స్థిరరాశి HBUINT8 *p = (స్థిరరాశి HBUINT8 *) బైట్లు.arrayZ;
	ఒకవేళ (unlikely (!Tuple_విలువలు::decompile (p, indices, p + బైట్లు.నిడివి, నిజం)))
	  తిరిగి అబద్ధం;
	కొరకు (పూర్ణాంకం &సూచీ_2 : indices)
	{
	  ఒకవేళ (unlikely (!axis_map->has (సూచీ_2))) తిరిగి అబద్ధం;
	  సూచీ_2 = axis_map->get (సూచీ_2);
	}
	స్వయంచాలక &దత్తాంశం = axis_indices_దత్తాంశం[i];
	/* One control byte and at most four data bytes per value. */
	గుర్తులేని కొలత = hb_unsigned_mul_saturate (indices.నిడివి, 5u);
	ఒకవేళ (unlikely (కొలత == UINT_MAX || !దత్తాంశం.resize_dirty (కొలత))) తిరిగి అబద్ధం;
	దత్తాంశం.shrink (Tuple_విలువలు::compile_unsafe (indices.as_array (), దత్తాంశం.arrayZ));
	బైట్లు = దత్తాంశం.as_array ();
      }
      axis_indices_దత్తాంశం_కొలత = hb_unsigned_కూడు_saturate (axis_indices_దత్తాంశం_కొలత,
							 బైట్లు.నిడివి);
      selected_axis_indices.push (బైట్లు);
    }

    తిరిగి axis_indices_దత్తాంశం_కొలత != UINT_MAX &&
	   !selected_axis_indices.లోపల_error ();
  }

  స్థిరరాశి VARC &ఆధారం;
  hb_subset_context_t *subset_context;
  స్థిరరాశి Coverage &ఆధారం_coverage_2;
  స్థిరరాశి CFF2_సూచీ &ఆధారం_రికార్డులు_2;
  స్థిరరాశి ConditionList &ఆధారం_conditions_2;
  స్థిరరాశి CFF2_సూచీ &ఆధారం_axis_indices_2;
  స్థిరరాశి Multi_అంశం_Variation_Store &ఆధారం_var_store_2;

  hb_సదిశ_t<గుర్తులేని> old_indices;
  hb_sorted_సదిశ_t<hb_codepoint_t> new_gids;
  గుర్తులేని దత్తాంశం_కొలత = 0;

  hb_సమితి_t condition_indices;
  hb_సమితి_t axis_indices;
  hb_సమితి_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_సదిశ_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_సదిశ_t<గుర్తులేని అక్షరం> రికార్డు_దత్తాంశం;
  hb_సదిశ_t<hb_ubytes_t> రికార్డులు;

  hb_సదిశ_t<hb_ubytes_t> selected_axis_indices;
  hb_సదిశ_t<hb_సదిశ_t<గుర్తులేని అక్షరం>> axis_indices_దత్తాంశం;
  గుర్తులేని axis_indices_దత్తాంశం_కొలత = 0;
  hb_map_t axes_సూచీ_map;
  స్థిరరాశి hb_map_t *axis_map = శూన్యసూచిక;

  అంతరంగ:
  తార్కిక auxiliary_indices_unchanged () స్థిరరాశి
  {
    కొరకు (hb_codepoint_t old_సూచీ : condition_indices)
      ఒకవేళ (condition_map.get (old_సూచీ) != old_సూచీ)
	తిరిగి అబద్ధం;
    కొరకు (hb_codepoint_t old_సూచీ : axis_indices)
      ఒకవేళ (axis_indices_map.get (old_సూచీ) != old_సూచీ)
	తిరిగి అబద్ధం;
    కొరకు (hb_codepoint_t old_సూచీ : var_indices)
      ఒకవేళ (varidx_map.get (old_సూచీ) != old_సూచీ)
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  తార్కిక append_varint (uint32_t value)
  {
    గుర్తులేని కొలత = HBUINT32VAR::get_కొలత (value);
    గుర్తులేని స్థానభ్రంశం = రికార్డు_దత్తాంశం.నిడివి;
    గుర్తులేని new_నిడివి = hb_unsigned_కూడు_saturate (స్థానభ్రంశం, కొలత);
    ఒకవేళ (unlikely (new_నిడివి == UINT_MAX ||
		  !రికార్డు_దత్తాంశం.resize_dirty (new_నిడివి)))
      తిరిగి అబద్ధం;
    HBUINT32VAR::serialize_unsafe (రికార్డు_దత్తాంశం.arrayZ + స్థానభ్రంశం, value);
    తిరిగి నిజం;
  }

  తార్కిక append_బైట్లు (hb_ubytes_t రికార్డు,
		     గుర్తులేని component_కొలత,
		     గుర్తులేని ప్రారంభం,
		     గుర్తులేని చివర)
  {
    ఒకవేళ (unlikely (ప్రారంభం > చివర || చివర > component_కొలత)) తిరిగి అబద్ధం;
    గుర్తులేని స్థానభ్రంశం = రికార్డు_దత్తాంశం.నిడివి;
    గుర్తులేని కొలత = చివర - ప్రారంభం;
    గుర్తులేని new_నిడివి = hb_unsigned_కూడు_saturate (స్థానభ్రంశం, కొలత);
    ఒకవేళ (unlikely (new_నిడివి == UINT_MAX ||
		  !రికార్డు_దత్తాంశం.resize_dirty (new_నిడివి)))
      తిరిగి అబద్ధం;
    hb_memcpy (రికార్డు_దత్తాంశం.arrayZ + స్థానభ్రంశం, రికార్డు.arrayZ + ప్రారంభం, కొలత);
    తిరిగి నిజం;
  }

  తార్కిక compile_రికార్డు (hb_ubytes_t రికార్డు, తార్కిక retain_gids)
  {
    hb_ubytes_t మిగిలిన = రికార్డు;
    అయితే (మిగిలిన)
    {
      VarComponent::రికార్డు_t component;
      ఒకవేళ (unlikely (!VarComponent::decompile_రికార్డు (ఆధారం, మిగిలిన,
						     శూన్యసూచిక, శూన్యసూచిక,
						     &component) ||
		    !compile_component (మిగిలిన, component, retain_gids)))
	తిరిగి అబద్ధం;
      మిగిలిన = మిగిలిన.sub_array (component.కొలత);
    }
    తిరిగి నిజం;
  }

  తార్కిక compile_component (hb_ubytes_t రికార్డు,
			  స్థిరరాశి VarComponent::రికార్డు_t &component,
			  తార్కిక retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    ఒకవేళ (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      తిరిగి అబద్ధం;

    uint32_t flags = component.flags;
    గుర్తులేని gid_కొలత = component.gid_కొలత;
    ఒకవేళ (new_gid > 0xFFFFu)
    {
      flags |= (గుర్తులేని) VarComponent::flags_t::GID_IS_24_బిట్టు;
      gid_కొలత = HBGlyphID24::static_కొలత;
    }

    ఒకవేళ (unlikely (!append_varint (flags))) తిరిగి అబద్ధం;
    గుర్తులేని gid_స్థానభ్రంశం = రికార్డు_దత్తాంశం.నిడివి;
    గుర్తులేని gid_చివర = hb_unsigned_కూడు_saturate (gid_స్థానభ్రంశం, gid_కొలత);
    ఒకవేళ (unlikely (gid_చివర == UINT_MAX ||
		  !రికార్డు_దత్తాంశం.resize_dirty (gid_చివర)))
      తిరిగి అబద్ధం;
    గుర్తులేని అక్షరం *బహిర్గతం = రికార్డు_దత్తాంశం.arrayZ + gid_స్థానభ్రంశం;
    ఒకవేళ (gid_కొలత == HBGlyphID16::static_కొలత)
      * (HBGlyphID16 *) బహిర్గతం = new_gid;
    లేకపోతే
      * (HBGlyphID24 *) బహిర్గతం = new_gid;

    గుర్తులేని cursor = component.gid_స్థానభ్రంశం + component.gid_కొలత;
    ఒకవేళ (flags & (గుర్తులేని) VarComponent::flags_t::HAVE_CONDITION)
    {
      ఒకవేళ (unlikely (!condition_map.has (component.condition_సూచీ) ||
		    !append_బైట్లు (రికార్డు, component.కొలత,
				   cursor, component.condition_స్థానభ్రంశం) ||
		    !append_varint (condition_map.get (component.condition_సూచీ))))
	తిరిగి అబద్ధం;
      cursor = component.condition_స్థానభ్రంశం + component.condition_కొలత;
    }
    ఒకవేళ (flags & (గుర్తులేని) VarComponent::flags_t::HAVE_AXES)
    {
      ఒకవేళ (unlikely (!axis_indices_map.has (component.axis_indices_సూచీ) ||
		    !append_బైట్లు (రికార్డు, component.కొలత,
				   cursor, component.axis_indices_స్థానభ్రంశం) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_సూచీ))))
	తిరిగి అబద్ధం;
      cursor = component.axis_indices_స్థానభ్రంశం + component.axis_indices_కొలత;
    }
    ఒకవేళ (flags & (గుర్తులేని) VarComponent::flags_t::AXIS_విలువలు_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_విలువలు_var_idx;
      ఒకవేళ (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_బైట్లు (రికార్డు, component.కొలత,
				   cursor, component.axis_విలువలు_var_స్థానభ్రంశం) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	తిరిగి అబద్ధం;
      cursor = component.axis_విలువలు_var_స్థానభ్రంశం + component.axis_విలువలు_var_కొలత;
    }
    ఒకవేళ (flags & (గుర్తులేని) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      ఒకవేళ (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_బైట్లు (రికార్డు, component.కొలత,
				   cursor, component.transform_var_స్థానభ్రంశం) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	తిరిగి అబద్ధం;
      cursor = component.transform_var_స్థానభ్రంశం + component.transform_var_కొలత;
    }

    తిరిగి append_బైట్లు (రికార్డు, component.కొలత, cursor, component.కొలత);
  }
};

తార్కిక
VARC::subset (hb_subset_context_t *c) స్థిరరాశి
{
  TRACE_SUBSET (ఇది);
  స్వయంచాలక fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    తిరిగి అబద్ధం;
  };

  స్థిరరాశి Coverage &ఆధారం_coverage_2 = ఇది+coverage;
  స్థిరరాశి CFF2_సూచీ &ఆధారం_రికార్డులు_2 = ఇది+glyph_రికార్డులు;
  స్థిరరాశి ConditionList &ఆధారం_conditions_2 = ఇది+conditionList;
  స్థిరరాశి TupleList &ఆధారం_axis_indices_2 = ఇది+axisIndicesList;
  స్థిరరాశి CFF2_సూచీ &ఆధారం_axis_indices_సూచీ = ఆధారం_axis_indices_2;
  స్థిరరాశి Multi_అంశం_Variation_Store &ఆధారం_var_store_2 = ఇది+varStore;
  varc_subset_plan_t subset_plan (*ఇది, c,
				  ఆధారం_coverage_2,
				  ఆధారం_రికార్డులు_2,
				  ఆధారం_conditions_2,
				  ఆధారం_axis_indices_సూచీ,
				  ఆధారం_var_store_2);
  ఒకవేళ (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  ఒకవేళ (!subset_plan.old_indices)
    return_trace (అబద్ధం);
  ఒకవేళ (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_రికార్డులు ()))
    return_trace (fail ());

  VARC *బహిర్గతం = c->serializer->ప్రారంభం_embed<VARC> ();
  ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం)))
    return_trace (అబద్ధం);
  బహిర్గతం->సంస్కరణ = సంస్కరణ;

  ఒకవేళ (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  ఒకవేళ (unlikely (!బహిర్గతం->coverage.serialize_serialize (c->serializer,
						     subset_plan.new_gids.iter ()) ||
		(subset_plan.condition_map.get_population () &&
		 !బహిర్గతం->conditionList.serialize_serialize (c->serializer,
						       &ఆధారం_conditions_2,
						       subset_plan.condition_map,
						       subset_plan.varidx_map,
						       subset_plan.axis_map)) ||
		(subset_plan.axis_indices_map.get_population () &&
		 !బహిర్గతం->axisIndicesList.serialize_serialize (c->serializer,
							subset_plan.selected_axis_indices.iter (),
							&subset_plan.axis_indices_దత్తాంశం_కొలత)) ||
		(subset_plan.var_indices.get_population () &&
		 !బహిర్గతం->varStore.serialize_serialize (c->serializer,
						  &ఆధారం_var_store_2,
						  subset_plan.var_inner_maps.as_array (),
						  subset_plan.axis_map)) ||
		!బహిర్గతం->glyph_రికార్డులు.serialize_serialize (c->serializer,
						 subset_plan.రికార్డులు.iter (),
						 &subset_plan.దత్తాంశం_కొలత)))
    return_trace (అబద్ధం);

  return_trace (నిజం);
}

} /* namespace OT */

#అయితే_ముగింపు /* OT_VAR_VARC_VARC_SUBSET_HH */
