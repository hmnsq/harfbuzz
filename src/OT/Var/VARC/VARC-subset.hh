#که_نه_تعریف_شوی OT_VAR_VARC_VARC_SUBSET_HH
#تعریف_کړه OT_VAR_VARC_VARC_SUBSET_HH

#ضمن "VARC.hh"

نطاق OT {

بنية varc_subset_plan_t
{
  varc_subset_plan_t (ثابت VARC &سرچینه_3,
		      hb_subset_context_t *subset_context_,
		      ثابت Coverage &سرچینه_coverage,
		      ثابت CFF2_شاخص &سرچینه_ریکارډونه,
		      ثابت ConditionList &سرچینه_conditions,
		      ثابت CFF2_شاخص &سرچینه_axis_indices,
		      ثابت Multi_توکی_Variation_Store &سرچینه_var_store) :
    سرچینه (سرچینه_3),
    subset_context (subset_context_),
    سرچینه_coverage_2 (سرچینه_coverage),
    سرچینه_ریکارډونه_2 (سرچینه_ریکارډونه),
    سرچینه_conditions_2 (سرچینه_conditions),
    سرچینه_axis_indices_2 (سرچینه_axis_indices),
    سرچینه_var_store_2 (سرچینه_var_store)
  {}

  منطقي collect_glyphs ()
  {
    ثابت hb_سېټ_t &glyphset = subset_context->plan->_glyphset_varced;
    لكل (خپلکاری _ : + hb_zip (سرچینه_coverage_2,
			    hb_range ((بینښه) سرچینه_ریکارډونه_2.شمېر)))
    {
      hb_codepoint_t old_gid = _.لومړی;
      بینښه شاخص_2 = _.second;
      إذا (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	إذا (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  أرجع خطأ;
	old_indices.push (شاخص_2);
	new_gids.push (new_gid);
	معلومات_اندازه = hb_unsigned_جمع_کړه_saturate (معلومات_اندازه,
					      سرچینه_ریکارډونه_2[شاخص_2].اوږدتیا);
      }
    }

    أرجع !old_indices.دننه_error () && !new_gids.دننه_error () &&
	   معلومات_اندازه != UINT_MAX;
  }

  منطقي collect_indices ()
  {
    لكل (بینښه old_شاخص : old_indices)
    {
      hb_ubytes_t پاتې_شوی = سرچینه_ریکارډونه_2[old_شاخص];
      طالما (پاتې_شوی)
      {
	VarComponent::ریکارډ_t component;
	إذا (unlikely (!VarComponent::decompile_ریکارډ (سرچینه, پاتې_شوی,
						       تش_اشاره, تش_اشاره,
						       &component)))
	  أرجع خطأ;

	إذا (component.flags & (بینښه) VarComponent::flags_t::HAVE_CONDITION)
	{
	  إذا (unlikely (component.condition_شاخص >= سرچینه_conditions_2.get_شمېر ()))
	    أرجع خطأ;
	  condition_indices.جمع_کړه (component.condition_شاخص);
	}
	إذا (component.flags & (بینښه) VarComponent::flags_t::HAVE_AXES)
	{
	  إذا (unlikely (component.axis_indices_شاخص >= سرچینه_axis_indices_2.شمېر))
	    أرجع خطأ;
	  axis_indices.جمع_کړه (component.axis_indices_شاخص);
	}
	إذا ((component.flags & (بینښه) VarComponent::flags_t::AXIS_ارزښتونه_HAVE_VARIATION) &&
	    component.axis_ارزښتونه_var_idx != VarIdx::NO_VARIATION)
	  var_indices.جمع_کړه (component.axis_ارزښتونه_var_idx);
	إذا ((component.flags & (بینښه) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.جمع_کړه (component.transform_var_idx);

	پاتې_شوی = پاتې_شوی.sub_array (component.اندازه);
      }
    }

    إذا (unlikely (condition_indices.دننه_error () || axis_indices.دننه_error () ||
		  var_indices.دننه_error ()))
      أرجع خطأ;

    لكل (hb_codepoint_t condition_شاخص : condition_indices)
      إذا (unlikely (!سرچینه_conditions_2[condition_شاخص].collect_var_indices (&var_indices)))
	أرجع خطأ;

    أرجع ریښتیا;
  }

  منطقي create_maps ()
  {
    ثابت خپلکاری *plan = subset_context->plan;
    إذا (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_شاخص_map;
      لكل (خپلکاری _ : plan->axes_شاخص_map)
	إذا (!plan->user_axes_location.has (plan->axes_old_شاخص_نښان_map.get (_.لومړی)))
	  axes_شاخص_map.سېټ (_.لومړی, _.second);
      إذا (unlikely (axes_شاخص_map.دننه_error ())) أرجع خطأ;
    }
    condition_map.جمع_کړه_سېټ (&condition_indices);
    axis_indices_map.جمع_کړه_سېټ (&axis_indices);
    أرجع !condition_map.دننه_error () && !axis_indices_map.دننه_error () &&
	   سرچینه_var_store_2.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  منطقي compile_ریکارډونه ()
  {
    إذا (unlikely (!ریکارډونه.alloc_exact (old_indices.اوږدتیا)))
      أرجع خطأ;

    منطقي retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    إذا (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      لكل (بینښه old_شاخص : old_indices)
	ریکارډونه.push (سرچینه_ریکارډونه_2[old_شاخص]);
    }
    وإلا
    {
      hb_ویکټور_t<بینښه> ریکارډ_offsets;
      ریکارډ_معلومات.alloc (معلومات_اندازه);
      إذا (unlikely (!ریکارډ_offsets.alloc_exact (old_indices.اوږدتیا + 1)))
	أرجع خطأ;

      لكل (بینښه old_شاخص : old_indices)
      {
	ریکارډ_offsets.push (ریکارډ_معلومات.اوږدتیا);
	إذا (unlikely (!compile_ریکارډ (سرچینه_ریکارډونه_2[old_شاخص], retain_gids)))
	  أرجع خطأ;
      }
      ریکارډ_offsets.push (ریکارډ_معلومات.اوږدتیا);

      إذا (unlikely (ریکارډ_معلومات.دننه_error () || ریکارډ_offsets.دننه_error ()))
	أرجع خطأ;
      لكل (بینښه i = 0; i < old_indices.اوږدتیا; i++)
	ریکارډونه.push (hb_ubytes_t (ریکارډ_معلومات.arrayZ + ریکارډ_offsets[i],
				  ریکارډ_offsets[i + 1] - ریکارډ_offsets[i]));
      معلومات_اندازه = ریکارډ_معلومات.اوږدتیا;
    }

    أرجع !ریکارډونه.دننه_error ();
  }

  منطقي select_axis_indices ()
  {
    إذا (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_معلومات.resize (
					  axis_indices_map.get_population ()))))
      أرجع خطأ;
    لكل (بینښه i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t بایټونه = سرچینه_axis_indices_2[axis_indices_map.backward (i)];
      إذا (axis_map)
      {
	hb_ویکټور_t<صحيح> indices;
	ثابت HBUINT8 *p = (ثابت HBUINT8 *) بایټونه.arrayZ;
	إذا (unlikely (!Tuple_ارزښتونه::decompile (p, indices, p + بایټونه.اوږدتیا, ریښتیا)))
	  أرجع خطأ;
	لكل (صحيح &شاخص_2 : indices)
	{
	  إذا (unlikely (!axis_map->has (شاخص_2))) أرجع خطأ;
	  شاخص_2 = axis_map->get (شاخص_2);
	}
	خپلکاری &معلومات = axis_indices_معلومات[i];
	/* One control byte and at most four data bytes per value. */
	بینښه اندازه = hb_unsigned_mul_saturate (indices.اوږدتیا, 5u);
	إذا (unlikely (اندازه == UINT_MAX || !معلومات.resize_dirty (اندازه))) أرجع خطأ;
	معلومات.shrink (Tuple_ارزښتونه::compile_unsafe (indices.as_array (), معلومات.arrayZ));
	بایټونه = معلومات.as_array ();
      }
      axis_indices_معلومات_اندازه = hb_unsigned_جمع_کړه_saturate (axis_indices_معلومات_اندازه,
							 بایټونه.اوږدتیا);
      selected_axis_indices.push (بایټونه);
    }

    أرجع axis_indices_معلومات_اندازه != UINT_MAX &&
	   !selected_axis_indices.دننه_error ();
  }

  ثابت VARC &سرچینه;
  hb_subset_context_t *subset_context;
  ثابت Coverage &سرچینه_coverage_2;
  ثابت CFF2_شاخص &سرچینه_ریکارډونه_2;
  ثابت ConditionList &سرچینه_conditions_2;
  ثابت CFF2_شاخص &سرچینه_axis_indices_2;
  ثابت Multi_توکی_Variation_Store &سرچینه_var_store_2;

  hb_ویکټور_t<بینښه> old_indices;
  hb_sorted_ویکټور_t<hb_codepoint_t> new_gids;
  بینښه معلومات_اندازه = 0;

  hb_سېټ_t condition_indices;
  hb_سېټ_t axis_indices;
  hb_سېټ_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_ویکټور_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_ویکټور_t<بینښه حرف> ریکارډ_معلومات;
  hb_ویکټور_t<hb_ubytes_t> ریکارډونه;

  hb_ویکټور_t<hb_ubytes_t> selected_axis_indices;
  hb_ویکټور_t<hb_ویکټور_t<بینښه حرف>> axis_indices_معلومات;
  بینښه axis_indices_معلومات_اندازه = 0;
  hb_map_t axes_شاخص_map;
  ثابت hb_map_t *axis_map = تش_اشاره;

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
    أرجع ریښتیا;
  }

  منطقي append_varint (uint32_t value)
  {
    بینښه اندازه = HBUINT32VAR::get_اندازه (value);
    بینښه دننه_واټن = ریکارډ_معلومات.اوږدتیا;
    بینښه new_اوږدتیا = hb_unsigned_جمع_کړه_saturate (دننه_واټن, اندازه);
    إذا (unlikely (new_اوږدتیا == UINT_MAX ||
		  !ریکارډ_معلومات.resize_dirty (new_اوږدتیا)))
      أرجع خطأ;
    HBUINT32VAR::serialize_unsafe (ریکارډ_معلومات.arrayZ + دننه_واټن, value);
    أرجع ریښتیا;
  }

  منطقي append_بایټونه (hb_ubytes_t ریکارډ,
		     بینښه component_اندازه,
		     بینښه پیل,
		     بینښه پای_ټکی)
  {
    إذا (unlikely (پیل > پای_ټکی || پای_ټکی > component_اندازه)) أرجع خطأ;
    بینښه دننه_واټن = ریکارډ_معلومات.اوږدتیا;
    بینښه اندازه = پای_ټکی - پیل;
    بینښه new_اوږدتیا = hb_unsigned_جمع_کړه_saturate (دننه_واټن, اندازه);
    إذا (unlikely (new_اوږدتیا == UINT_MAX ||
		  !ریکارډ_معلومات.resize_dirty (new_اوږدتیا)))
      أرجع خطأ;
    hb_memcpy (ریکارډ_معلومات.arrayZ + دننه_واټن, ریکارډ.arrayZ + پیل, اندازه);
    أرجع ریښتیا;
  }

  منطقي compile_ریکارډ (hb_ubytes_t ریکارډ, منطقي retain_gids)
  {
    hb_ubytes_t پاتې_شوی = ریکارډ;
    طالما (پاتې_شوی)
    {
      VarComponent::ریکارډ_t component;
      إذا (unlikely (!VarComponent::decompile_ریکارډ (سرچینه, پاتې_شوی,
						     تش_اشاره, تش_اشاره,
						     &component) ||
		    !compile_component (پاتې_شوی, component, retain_gids)))
	أرجع خطأ;
      پاتې_شوی = پاتې_شوی.sub_array (component.اندازه);
    }
    أرجع ریښتیا;
  }

  منطقي compile_component (hb_ubytes_t ریکارډ,
			  ثابت VarComponent::ریکارډ_t &component,
			  منطقي retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    إذا (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      أرجع خطأ;

    uint32_t flags = component.flags;
    بینښه gid_اندازه = component.gid_اندازه;
    إذا (new_gid > 0xFFFFu)
    {
      flags |= (بینښه) VarComponent::flags_t::GID_IS_24_بټ;
      gid_اندازه = HBGlyphID24::static_اندازه;
    }

    إذا (unlikely (!append_varint (flags))) أرجع خطأ;
    بینښه gid_دننه_واټن = ریکارډ_معلومات.اوږدتیا;
    بینښه gid_پای_ټکی = hb_unsigned_جمع_کړه_saturate (gid_دننه_واټن, gid_اندازه);
    إذا (unlikely (gid_پای_ټکی == UINT_MAX ||
		  !ریکارډ_معلومات.resize_dirty (gid_پای_ټکی)))
      أرجع خطأ;
    بینښه حرف *بهر = ریکارډ_معلومات.arrayZ + gid_دننه_واټن;
    إذا (gid_اندازه == HBGlyphID16::static_اندازه)
      * (HBGlyphID16 *) بهر = new_gid;
    وإلا
      * (HBGlyphID24 *) بهر = new_gid;

    بینښه cursor = component.gid_دننه_واټن + component.gid_اندازه;
    إذا (flags & (بینښه) VarComponent::flags_t::HAVE_CONDITION)
    {
      إذا (unlikely (!condition_map.has (component.condition_شاخص) ||
		    !append_بایټونه (ریکارډ, component.اندازه,
				   cursor, component.condition_دننه_واټن) ||
		    !append_varint (condition_map.get (component.condition_شاخص))))
	أرجع خطأ;
      cursor = component.condition_دننه_واټن + component.condition_اندازه;
    }
    إذا (flags & (بینښه) VarComponent::flags_t::HAVE_AXES)
    {
      إذا (unlikely (!axis_indices_map.has (component.axis_indices_شاخص) ||
		    !append_بایټونه (ریکارډ, component.اندازه,
				   cursor, component.axis_indices_دننه_واټن) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_شاخص))))
	أرجع خطأ;
      cursor = component.axis_indices_دننه_واټن + component.axis_indices_اندازه;
    }
    إذا (flags & (بینښه) VarComponent::flags_t::AXIS_ارزښتونه_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_ارزښتونه_var_idx;
      إذا (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_بایټونه (ریکارډ, component.اندازه,
				   cursor, component.axis_ارزښتونه_var_دننه_واټن) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	أرجع خطأ;
      cursor = component.axis_ارزښتونه_var_دننه_واټن + component.axis_ارزښتونه_var_اندازه;
    }
    إذا (flags & (بینښه) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      إذا (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_بایټونه (ریکارډ, component.اندازه,
				   cursor, component.transform_var_دننه_واټن) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	أرجع خطأ;
      cursor = component.transform_var_دننه_واټن + component.transform_var_اندازه;
    }

    أرجع append_بایټونه (ریکارډ, component.اندازه, cursor, component.اندازه);
  }
};

منطقي
VARC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);
  خپلکاری fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    أرجع خطأ;
  };

  ثابت Coverage &سرچینه_coverage_2 = هذا+coverage;
  ثابت CFF2_شاخص &سرچینه_ریکارډونه_2 = هذا+glyph_ریکارډونه;
  ثابت ConditionList &سرچینه_conditions_2 = هذا+conditionList;
  ثابت TupleList &سرچینه_axis_indices_2 = هذا+axisIndicesList;
  ثابت CFF2_شاخص &سرچینه_axis_indices_شاخص = سرچینه_axis_indices_2;
  ثابت Multi_توکی_Variation_Store &سرچینه_var_store_2 = هذا+varStore;
  varc_subset_plan_t subset_plan (*هذا, c,
				  سرچینه_coverage_2,
				  سرچینه_ریکارډونه_2,
				  سرچینه_conditions_2,
				  سرچینه_axis_indices_شاخص,
				  سرچینه_var_store_2);
  إذا (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  إذا (!subset_plan.old_indices)
    return_trace (خطأ);
  إذا (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_ریکارډونه ()))
    return_trace (fail ());

  VARC *بهر = c->serializer->پیل_embed<VARC> ();
  إذا (unlikely (!بهر || !c->serializer->extend_min (بهر)))
    return_trace (خطأ);
  بهر->نسخه = نسخه;

  إذا (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  إذا (unlikely (!بهر->coverage.serialize_serialize (c->serializer,
						     subset_plan.new_gids.iter ()) ||
		(subset_plan.condition_map.get_population () &&
		 !بهر->conditionList.serialize_serialize (c->serializer,
						       &سرچینه_conditions_2,
						       subset_plan.condition_map,
						       subset_plan.varidx_map,
						       subset_plan.axis_map)) ||
		(subset_plan.axis_indices_map.get_population () &&
		 !بهر->axisIndicesList.serialize_serialize (c->serializer,
							subset_plan.selected_axis_indices.iter (),
							&subset_plan.axis_indices_معلومات_اندازه)) ||
		(subset_plan.var_indices.get_population () &&
		 !بهر->varStore.serialize_serialize (c->serializer,
						  &سرچینه_var_store_2,
						  subset_plan.var_inner_maps.as_array (),
						  subset_plan.axis_map)) ||
		!بهر->glyph_ریکارډونه.serialize_serialize (c->serializer,
						 subset_plan.ریکارډونه.iter (),
						 &subset_plan.معلومات_اندازه)))
    return_trace (خطأ);

  return_trace (ریښتیا);
}

} /* namespace OT */

#د_که_پای /* OT_VAR_VARC_VARC_SUBSET_HH */
