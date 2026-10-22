#اگر_تعریف_نشده OT_VAR_VARC_VARC_SUBSET_HH
#تعریف_کن OT_VAR_VARC_VARC_SUBSET_HH

#ضمن "VARC.hh"

نطاق OT {

بنية varc_subset_plan_t
{
  varc_subset_plan_t (ثابت VARC &منبع_3,
		      hb_subset_context_t *subset_context_,
		      ثابت Coverage &منبع_coverage,
		      ثابت CFF2_شاخص &منبع_رکوردها,
		      ثابت ConditionList &منبع_conditions,
		      ثابت CFF2_شاخص &منبع_axis_indices,
		      ثابت Multi_قلم_Variation_Store &منبع_var_store) :
    منبع (منبع_3),
    subset_context (subset_context_),
    منبع_coverage_2 (منبع_coverage),
    منبع_رکوردها_2 (منبع_رکوردها),
    منبع_conditions_2 (منبع_conditions),
    منبع_axis_indices_2 (منبع_axis_indices),
    منبع_var_store_2 (منبع_var_store)
  {}

  منطقي collect_glyphs ()
  {
    ثابت hb_مجموعه_t &glyphset = subset_context->plan->_glyphset_varced;
    لكل (خودکار _ : + hb_zip (منبع_coverage_2,
			    hb_range ((بیعلامت) منبع_رکوردها_2.شمار)))
    {
      hb_codepoint_t old_gid = _.اولی;
      بیعلامت شاخص_2 = _.second;
      إذا (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	إذا (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  أرجع خطأ;
	old_indices.push (شاخص_2);
	new_gids.push (new_gid);
	داده_اندازه = hb_unsigned_جمع_کن_saturate (داده_اندازه,
					      منبع_رکوردها_2[شاخص_2].درازی);
      }
    }

    أرجع !old_indices.درون_error () && !new_gids.درون_error () &&
	   داده_اندازه != UINT_MAX;
  }

  منطقي collect_indices ()
  {
    لكل (بیعلامت old_شاخص : old_indices)
    {
      hb_ubytes_t مانده = منبع_رکوردها_2[old_شاخص];
      طالما (مانده)
      {
	VarComponent::رکورد_t component;
	إذا (unlikely (!VarComponent::decompile_رکورد (منبع, مانده,
						       اشاره_تهی, اشاره_تهی,
						       &component)))
	  أرجع خطأ;

	إذا (component.flags & (بیعلامت) VarComponent::flags_t::HAVE_CONDITION)
	{
	  إذا (unlikely (component.condition_شاخص >= منبع_conditions_2.get_شمار ()))
	    أرجع خطأ;
	  condition_indices.جمع_کن (component.condition_شاخص);
	}
	إذا (component.flags & (بیعلامت) VarComponent::flags_t::HAVE_AXES)
	{
	  إذا (unlikely (component.axis_indices_شاخص >= منبع_axis_indices_2.شمار))
	    أرجع خطأ;
	  axis_indices.جمع_کن (component.axis_indices_شاخص);
	}
	إذا ((component.flags & (بیعلامت) VarComponent::flags_t::AXIS_مقادیر_HAVE_VARIATION) &&
	    component.axis_مقادیر_var_idx != VarIdx::NO_VARIATION)
	  var_indices.جمع_کن (component.axis_مقادیر_var_idx);
	إذا ((component.flags & (بیعلامت) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.جمع_کن (component.transform_var_idx);

	مانده = مانده.sub_array (component.اندازه);
      }
    }

    إذا (unlikely (condition_indices.درون_error () || axis_indices.درون_error () ||
		  var_indices.درون_error ()))
      أرجع خطأ;

    لكل (hb_codepoint_t condition_شاخص : condition_indices)
      إذا (unlikely (!منبع_conditions_2[condition_شاخص].collect_var_indices (&var_indices)))
	أرجع خطأ;

    أرجع درست;
  }

  منطقي create_maps ()
  {
    ثابت خودکار *plan = subset_context->plan;
    إذا (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_شاخص_map;
      لكل (خودکار _ : plan->axes_شاخص_map)
	إذا (!plan->user_axes_location.has (plan->axes_old_شاخص_برچسب_map.get (_.اولی)))
	  axes_شاخص_map.مجموعه (_.اولی, _.second);
      إذا (unlikely (axes_شاخص_map.درون_error ())) أرجع خطأ;
    }
    condition_map.جمع_کن_مجموعه (&condition_indices);
    axis_indices_map.جمع_کن_مجموعه (&axis_indices);
    أرجع !condition_map.درون_error () && !axis_indices_map.درون_error () &&
	   منبع_var_store_2.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  منطقي compile_رکوردها ()
  {
    إذا (unlikely (!رکوردها.alloc_exact (old_indices.درازی)))
      أرجع خطأ;

    منطقي retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    إذا (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      لكل (بیعلامت old_شاخص : old_indices)
	رکوردها.push (منبع_رکوردها_2[old_شاخص]);
    }
    وإلا
    {
      hb_وکتور_t<بیعلامت> رکورد_offsets;
      رکورد_داده.alloc (داده_اندازه);
      إذا (unlikely (!رکورد_offsets.alloc_exact (old_indices.درازی + 1)))
	أرجع خطأ;

      لكل (بیعلامت old_شاخص : old_indices)
      {
	رکورد_offsets.push (رکورد_داده.درازی);
	إذا (unlikely (!compile_رکورد (منبع_رکوردها_2[old_شاخص], retain_gids)))
	  أرجع خطأ;
      }
      رکورد_offsets.push (رکورد_داده.درازی);

      إذا (unlikely (رکورد_داده.درون_error () || رکورد_offsets.درون_error ()))
	أرجع خطأ;
      لكل (بیعلامت i = 0; i < old_indices.درازی; i++)
	رکوردها.push (hb_ubytes_t (رکورد_داده.arrayZ + رکورد_offsets[i],
				  رکورد_offsets[i + 1] - رکورد_offsets[i]));
      داده_اندازه = رکورد_داده.درازی;
    }

    أرجع !رکوردها.درون_error ();
  }

  منطقي select_axis_indices ()
  {
    إذا (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_داده.resize (
					  axis_indices_map.get_population ()))))
      أرجع خطأ;
    لكل (بیعلامت i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t بایتها = منبع_axis_indices_2[axis_indices_map.backward (i)];
      إذا (axis_map)
      {
	hb_وکتور_t<صحيح> indices;
	ثابت HBUINT8 *p = (ثابت HBUINT8 *) بایتها.arrayZ;
	إذا (unlikely (!Tuple_مقادیر::decompile (p, indices, p + بایتها.درازی, درست)))
	  أرجع خطأ;
	لكل (صحيح &شاخص_2 : indices)
	{
	  إذا (unlikely (!axis_map->has (شاخص_2))) أرجع خطأ;
	  شاخص_2 = axis_map->get (شاخص_2);
	}
	خودکار &داده = axis_indices_داده[i];
	/* One control byte and at most four data bytes per value. */
	بیعلامت اندازه = hb_unsigned_mul_saturate (indices.درازی, 5u);
	إذا (unlikely (اندازه == UINT_MAX || !داده.resize_dirty (اندازه))) أرجع خطأ;
	داده.shrink (Tuple_مقادیر::compile_unsafe (indices.as_array (), داده.arrayZ));
	بایتها = داده.as_array ();
      }
      axis_indices_داده_اندازه = hb_unsigned_جمع_کن_saturate (axis_indices_داده_اندازه,
							 بایتها.درازی);
      selected_axis_indices.push (بایتها);
    }

    أرجع axis_indices_داده_اندازه != UINT_MAX &&
	   !selected_axis_indices.درون_error ();
  }

  ثابت VARC &منبع;
  hb_subset_context_t *subset_context;
  ثابت Coverage &منبع_coverage_2;
  ثابت CFF2_شاخص &منبع_رکوردها_2;
  ثابت ConditionList &منبع_conditions_2;
  ثابت CFF2_شاخص &منبع_axis_indices_2;
  ثابت Multi_قلم_Variation_Store &منبع_var_store_2;

  hb_وکتور_t<بیعلامت> old_indices;
  hb_sorted_وکتور_t<hb_codepoint_t> new_gids;
  بیعلامت داده_اندازه = 0;

  hb_مجموعه_t condition_indices;
  hb_مجموعه_t axis_indices;
  hb_مجموعه_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_وکتور_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_وکتور_t<بیعلامت حرف> رکورد_داده;
  hb_وکتور_t<hb_ubytes_t> رکوردها;

  hb_وکتور_t<hb_ubytes_t> selected_axis_indices;
  hb_وکتور_t<hb_وکتور_t<بیعلامت حرف>> axis_indices_داده;
  بیعلامت axis_indices_داده_اندازه = 0;
  hb_map_t axes_شاخص_map;
  ثابت hb_map_t *axis_map = اشاره_تهی;

  خاص:
  منطقي auxiliary_indices_unchanged () ثابت
  {
    لكل (hb_codepoint_t old_شاخص : condition_indices)
      إذا (condition_map.get (old_شاخص) != old_شاخص)
	أرجع خطأ;
    لكل (hb_codepoint_t old_شاخص : axis_indices)
      إذا (axis_indices_map.get (old_شاخص) != old_شاخص)
	أرجع خطأ;
    لكل (hb_codepoint_t old_شاخص : var_indices)
      إذا (varidx_map.get (old_شاخص) != old_شاخص)
	أرجع خطأ;
    أرجع درست;
  }

  منطقي append_varint (uint32_t value)
  {
    بیعلامت اندازه = HBUINT32VAR::get_اندازه (value);
    بیعلامت جابه_جایی = رکورد_داده.درازی;
    بیعلامت new_درازی = hb_unsigned_جمع_کن_saturate (جابه_جایی, اندازه);
    إذا (unlikely (new_درازی == UINT_MAX ||
		  !رکورد_داده.resize_dirty (new_درازی)))
      أرجع خطأ;
    HBUINT32VAR::serialize_unsafe (رکورد_داده.arrayZ + جابه_جایی, value);
    أرجع درست;
  }

  منطقي append_بایتها (hb_ubytes_t رکورد,
		     بیعلامت component_اندازه,
		     بیعلامت شروع,
		     بیعلامت انتها)
  {
    إذا (unlikely (شروع > انتها || انتها > component_اندازه)) أرجع خطأ;
    بیعلامت جابه_جایی = رکورد_داده.درازی;
    بیعلامت اندازه = انتها - شروع;
    بیعلامت new_درازی = hb_unsigned_جمع_کن_saturate (جابه_جایی, اندازه);
    إذا (unlikely (new_درازی == UINT_MAX ||
		  !رکورد_داده.resize_dirty (new_درازی)))
      أرجع خطأ;
    hb_memcpy (رکورد_داده.arrayZ + جابه_جایی, رکورد.arrayZ + شروع, اندازه);
    أرجع درست;
  }

  منطقي compile_رکورد (hb_ubytes_t رکورد, منطقي retain_gids)
  {
    hb_ubytes_t مانده = رکورد;
    طالما (مانده)
    {
      VarComponent::رکورد_t component;
      إذا (unlikely (!VarComponent::decompile_رکورد (منبع, مانده,
						     اشاره_تهی, اشاره_تهی,
						     &component) ||
		    !compile_component (مانده, component, retain_gids)))
	أرجع خطأ;
      مانده = مانده.sub_array (component.اندازه);
    }
    أرجع درست;
  }

  منطقي compile_component (hb_ubytes_t رکورد,
			  ثابت VarComponent::رکورد_t &component,
			  منطقي retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    إذا (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      أرجع خطأ;

    uint32_t flags = component.flags;
    بیعلامت gid_اندازه = component.gid_اندازه;
    إذا (new_gid > 0xFFFFu)
    {
      flags |= (بیعلامت) VarComponent::flags_t::GID_IS_24_بیت;
      gid_اندازه = HBGlyphID24::static_اندازه;
    }

    إذا (unlikely (!append_varint (flags))) أرجع خطأ;
    بیعلامت gid_جابه_جایی = رکورد_داده.درازی;
    بیعلامت gid_انتها = hb_unsigned_جمع_کن_saturate (gid_جابه_جایی, gid_اندازه);
    إذا (unlikely (gid_انتها == UINT_MAX ||
		  !رکورد_داده.resize_dirty (gid_انتها)))
      أرجع خطأ;
    بیعلامت حرف *بیرون = رکورد_داده.arrayZ + gid_جابه_جایی;
    إذا (gid_اندازه == HBGlyphID16::static_اندازه)
      * (HBGlyphID16 *) بیرون = new_gid;
    وإلا
      * (HBGlyphID24 *) بیرون = new_gid;

    بیعلامت cursor = component.gid_جابه_جایی + component.gid_اندازه;
    إذا (flags & (بیعلامت) VarComponent::flags_t::HAVE_CONDITION)
    {
      إذا (unlikely (!condition_map.has (component.condition_شاخص) ||
		    !append_بایتها (رکورد, component.اندازه,
				   cursor, component.condition_جابه_جایی) ||
		    !append_varint (condition_map.get (component.condition_شاخص))))
	أرجع خطأ;
      cursor = component.condition_جابه_جایی + component.condition_اندازه;
    }
    إذا (flags & (بیعلامت) VarComponent::flags_t::HAVE_AXES)
    {
      إذا (unlikely (!axis_indices_map.has (component.axis_indices_شاخص) ||
		    !append_بایتها (رکورد, component.اندازه,
				   cursor, component.axis_indices_جابه_جایی) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_شاخص))))
	أرجع خطأ;
      cursor = component.axis_indices_جابه_جایی + component.axis_indices_اندازه;
    }
    إذا (flags & (بیعلامت) VarComponent::flags_t::AXIS_مقادیر_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_مقادیر_var_idx;
      إذا (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_بایتها (رکورد, component.اندازه,
				   cursor, component.axis_مقادیر_var_جابه_جایی) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	أرجع خطأ;
      cursor = component.axis_مقادیر_var_جابه_جایی + component.axis_مقادیر_var_اندازه;
    }
    إذا (flags & (بیعلامت) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      إذا (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_بایتها (رکورد, component.اندازه,
				   cursor, component.transform_var_جابه_جایی) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	أرجع خطأ;
      cursor = component.transform_var_جابه_جایی + component.transform_var_اندازه;
    }

    أرجع append_بایتها (رکورد, component.اندازه, cursor, component.اندازه);
  }
};

منطقي
VARC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);
  خودکار fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    أرجع خطأ;
  };

  ثابت Coverage &منبع_coverage_2 = هذا+coverage;
  ثابت CFF2_شاخص &منبع_رکوردها_2 = هذا+glyph_رکوردها;
  ثابت ConditionList &منبع_conditions_2 = هذا+conditionList;
  ثابت TupleList &منبع_axis_indices_2 = هذا+axisIndicesList;
  ثابت CFF2_شاخص &منبع_axis_indices_شاخص = منبع_axis_indices_2;
  ثابت Multi_قلم_Variation_Store &منبع_var_store_2 = هذا+varStore;
  varc_subset_plan_t subset_plan (*هذا, c,
				  منبع_coverage_2,
				  منبع_رکوردها_2,
				  منبع_conditions_2,
				  منبع_axis_indices_شاخص,
				  منبع_var_store_2);
  إذا (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  إذا (!subset_plan.old_indices)
    return_trace (خطأ);
  إذا (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_رکوردها ()))
    return_trace (fail ());

  VARC *بیرون = c->serializer->شروع_embed<VARC> ();
  إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون)))
    return_trace (خطأ);
  بیرون->نسخه = نسخه;

  إذا (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  إذا (unlikely (!بیرون->coverage.serialize_serialize (c->serializer,
						     subset_plan.new_gids.iter ()) ||
		(subset_plan.condition_map.get_population () &&
		 !بیرون->conditionList.serialize_serialize (c->serializer,
						       &منبع_conditions_2,
						       subset_plan.condition_map,
						       subset_plan.varidx_map,
						       subset_plan.axis_map)) ||
		(subset_plan.axis_indices_map.get_population () &&
		 !بیرون->axisIndicesList.serialize_serialize (c->serializer,
							subset_plan.selected_axis_indices.iter (),
							&subset_plan.axis_indices_داده_اندازه)) ||
		(subset_plan.var_indices.get_population () &&
		 !بیرون->varStore.serialize_serialize (c->serializer,
						  &منبع_var_store_2,
						  subset_plan.var_inner_maps.as_array (),
						  subset_plan.axis_map)) ||
		!بیرون->glyph_رکوردها.serialize_serialize (c->serializer,
						 subset_plan.رکوردها.iter (),
						 &subset_plan.داده_اندازه)))
    return_trace (خطأ);

  return_trace (درست);
}

} /* namespace OT */

#پایان_اگر /* OT_VAR_VARC_VARC_SUBSET_HH */
