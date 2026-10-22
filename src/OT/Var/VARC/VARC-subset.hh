#جيڪڏهن_وصف_نه_ٿيل OT_VAR_VARC_VARC_SUBSET_HH
#وصف_ڪريو OT_VAR_VARC_VARC_SUBSET_HH

#ضمن "VARC.hh"

نطاق OT {

بنية varc_subset_plan_t
{
  varc_subset_plan_t (ثابت VARC &ذريعو_3,
		      hb_subset_context_t *subset_context_,
		      ثابت Coverage &ذريعو_coverage,
		      ثابت CFF2_اشاريو &ذريعو_رڪارڊون,
		      ثابت ConditionList &ذريعو_conditions,
		      ثابت CFF2_اشاريو &ذريعو_axis_indices,
		      ثابت Multi_شي_Variation_Store &ذريعو_var_store) :
    ذريعو (ذريعو_3),
    subset_context (subset_context_),
    ذريعو_coverage_2 (ذريعو_coverage),
    ذريعو_رڪارڊون_2 (ذريعو_رڪارڊون),
    ذريعو_conditions_2 (ذريعو_conditions),
    ذريعو_axis_indices_2 (ذريعو_axis_indices),
    ذريعو_var_store_2 (ذريعو_var_store)
  {}

  منطقي collect_glyphs ()
  {
    ثابت hb_سيٽ_t &glyphset = subset_context->plan->_glyphset_varced;
    لكل (خودڪار _ : + hb_zip (ذريعو_coverage_2,
			    hb_range ((بنانشان) ذريعو_رڪارڊون_2.ڳڻپ)))
    {
      hb_codepoint_t old_gid = _.پهريون;
      بنانشان اشاريو_2 = _.second;
      إذا (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	إذا (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  أرجع خطأ;
	old_indices.push (اشاريو_2);
	new_gids.push (new_gid);
	معلومات_سائيز = hb_unsigned_جوڙيو_saturate (معلومات_سائيز,
					      ذريعو_رڪارڊون_2[اشاريو_2].ڊيگهائي);
      }
    }

    أرجع !old_indices.اندريون_error () && !new_gids.اندريون_error () &&
	   معلومات_سائيز != UINT_MAX;
  }

  منطقي collect_indices ()
  {
    لكل (بنانشان old_اشاريو : old_indices)
    {
      hb_ubytes_t بچيل = ذريعو_رڪارڊون_2[old_اشاريو];
      طالما (بچيل)
      {
	VarComponent::رڪارڊ_t component;
	إذا (unlikely (!VarComponent::decompile_رڪارڊ (ذريعو, بچيل,
						       خالي_اشارو, خالي_اشارو,
						       &component)))
	  أرجع خطأ;

	إذا (component.flags & (بنانشان) VarComponent::flags_t::HAVE_CONDITION)
	{
	  إذا (unlikely (component.condition_اشاريو >= ذريعو_conditions_2.get_ڳڻپ ()))
	    أرجع خطأ;
	  condition_indices.جوڙيو (component.condition_اشاريو);
	}
	إذا (component.flags & (بنانشان) VarComponent::flags_t::HAVE_AXES)
	{
	  إذا (unlikely (component.axis_indices_اشاريو >= ذريعو_axis_indices_2.ڳڻپ))
	    أرجع خطأ;
	  axis_indices.جوڙيو (component.axis_indices_اشاريو);
	}
	إذا ((component.flags & (بنانشان) VarComponent::flags_t::AXIS_قدرون_HAVE_VARIATION) &&
	    component.axis_قدرون_var_idx != VarIdx::NO_VARIATION)
	  var_indices.جوڙيو (component.axis_قدرون_var_idx);
	إذا ((component.flags & (بنانشان) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.جوڙيو (component.transform_var_idx);

	بچيل = بچيل.sub_array (component.سائيز);
      }
    }

    إذا (unlikely (condition_indices.اندريون_error () || axis_indices.اندريون_error () ||
		  var_indices.اندريون_error ()))
      أرجع خطأ;

    لكل (hb_codepoint_t condition_اشاريو : condition_indices)
      إذا (unlikely (!ذريعو_conditions_2[condition_اشاريو].collect_var_indices (&var_indices)))
	أرجع خطأ;

    أرجع سچ;
  }

  منطقي create_maps ()
  {
    ثابت خودڪار *plan = subset_context->plan;
    إذا (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_اشاريو_map;
      لكل (خودڪار _ : plan->axes_اشاريو_map)
	إذا (!plan->user_axes_location.has (plan->axes_old_اشاريو_ٽيگ_map.get (_.پهريون)))
	  axes_اشاريو_map.سيٽ (_.پهريون, _.second);
      إذا (unlikely (axes_اشاريو_map.اندريون_error ())) أرجع خطأ;
    }
    condition_map.جوڙيو_سيٽ (&condition_indices);
    axis_indices_map.جوڙيو_سيٽ (&axis_indices);
    أرجع !condition_map.اندريون_error () && !axis_indices_map.اندريون_error () &&
	   ذريعو_var_store_2.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  منطقي compile_رڪارڊون ()
  {
    إذا (unlikely (!رڪارڊون.alloc_exact (old_indices.ڊيگهائي)))
      أرجع خطأ;

    منطقي retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    إذا (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      لكل (بنانشان old_اشاريو : old_indices)
	رڪارڊون.push (ذريعو_رڪارڊون_2[old_اشاريو]);
    }
    وإلا
    {
      hb_ويڪٽر_t<بنانشان> رڪارڊ_offsets;
      رڪارڊ_معلومات.alloc (معلومات_سائيز);
      إذا (unlikely (!رڪارڊ_offsets.alloc_exact (old_indices.ڊيگهائي + 1)))
	أرجع خطأ;

      لكل (بنانشان old_اشاريو : old_indices)
      {
	رڪارڊ_offsets.push (رڪارڊ_معلومات.ڊيگهائي);
	إذا (unlikely (!compile_رڪارڊ (ذريعو_رڪارڊون_2[old_اشاريو], retain_gids)))
	  أرجع خطأ;
      }
      رڪارڊ_offsets.push (رڪارڊ_معلومات.ڊيگهائي);

      إذا (unlikely (رڪارڊ_معلومات.اندريون_error () || رڪارڊ_offsets.اندريون_error ()))
	أرجع خطأ;
      لكل (بنانشان i = 0; i < old_indices.ڊيگهائي; i++)
	رڪارڊون.push (hb_ubytes_t (رڪارڊ_معلومات.arrayZ + رڪارڊ_offsets[i],
				  رڪارڊ_offsets[i + 1] - رڪارڊ_offsets[i]));
      معلومات_سائيز = رڪارڊ_معلومات.ڊيگهائي;
    }

    أرجع !رڪارڊون.اندريون_error ();
  }

  منطقي select_axis_indices ()
  {
    إذا (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_معلومات.resize (
					  axis_indices_map.get_population ()))))
      أرجع خطأ;
    لكل (بنانشان i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t بائيٽون = ذريعو_axis_indices_2[axis_indices_map.backward (i)];
      إذا (axis_map)
      {
	hb_ويڪٽر_t<صحيح> indices;
	ثابت HBUINT8 *p = (ثابت HBUINT8 *) بائيٽون.arrayZ;
	إذا (unlikely (!Tuple_قدرون::decompile (p, indices, p + بائيٽون.ڊيگهائي, سچ)))
	  أرجع خطأ;
	لكل (صحيح &اشاريو_2 : indices)
	{
	  إذا (unlikely (!axis_map->has (اشاريو_2))) أرجع خطأ;
	  اشاريو_2 = axis_map->get (اشاريو_2);
	}
	خودڪار &معلومات = axis_indices_معلومات[i];
	/* One control byte and at most four data bytes per value. */
	بنانشان سائيز = hb_unsigned_mul_saturate (indices.ڊيگهائي, 5u);
	إذا (unlikely (سائيز == UINT_MAX || !معلومات.resize_dirty (سائيز))) أرجع خطأ;
	معلومات.shrink (Tuple_قدرون::compile_unsafe (indices.as_array (), معلومات.arrayZ));
	بائيٽون = معلومات.as_array ();
      }
      axis_indices_معلومات_سائيز = hb_unsigned_جوڙيو_saturate (axis_indices_معلومات_سائيز,
							 بائيٽون.ڊيگهائي);
      selected_axis_indices.push (بائيٽون);
    }

    أرجع axis_indices_معلومات_سائيز != UINT_MAX &&
	   !selected_axis_indices.اندريون_error ();
  }

  ثابت VARC &ذريعو;
  hb_subset_context_t *subset_context;
  ثابت Coverage &ذريعو_coverage_2;
  ثابت CFF2_اشاريو &ذريعو_رڪارڊون_2;
  ثابت ConditionList &ذريعو_conditions_2;
  ثابت CFF2_اشاريو &ذريعو_axis_indices_2;
  ثابت Multi_شي_Variation_Store &ذريعو_var_store_2;

  hb_ويڪٽر_t<بنانشان> old_indices;
  hb_sorted_ويڪٽر_t<hb_codepoint_t> new_gids;
  بنانشان معلومات_سائيز = 0;

  hb_سيٽ_t condition_indices;
  hb_سيٽ_t axis_indices;
  hb_سيٽ_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_ويڪٽر_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_ويڪٽر_t<بنانشان حرف> رڪارڊ_معلومات;
  hb_ويڪٽر_t<hb_ubytes_t> رڪارڊون;

  hb_ويڪٽر_t<hb_ubytes_t> selected_axis_indices;
  hb_ويڪٽر_t<hb_ويڪٽر_t<بنانشان حرف>> axis_indices_معلومات;
  بنانشان axis_indices_معلومات_سائيز = 0;
  hb_map_t axes_اشاريو_map;
  ثابت hb_map_t *axis_map = خالي_اشارو;

  خاص:
  منطقي auxiliary_indices_unchanged () ثابت
  {
    لكل (hb_codepoint_t old_اشاريو : condition_indices)
      إذا (condition_map.get (old_اشاريو) != old_اشاريو)
	أرجع خطأ;
    لكل (hb_codepoint_t old_اشاريو : axis_indices)
      إذا (axis_indices_map.get (old_اشاريو) != old_اشاريو)
	أرجع خطأ;
    لكل (hb_codepoint_t old_اشاريو : var_indices)
      إذا (varidx_map.get (old_اشاريو) != old_اشاريو)
	أرجع خطأ;
    أرجع سچ;
  }

  منطقي append_varint (uint32_t value)
  {
    بنانشان سائيز = HBUINT32VAR::get_سائيز (value);
    بنانشان وٿي = رڪارڊ_معلومات.ڊيگهائي;
    بنانشان new_ڊيگهائي = hb_unsigned_جوڙيو_saturate (وٿي, سائيز);
    إذا (unlikely (new_ڊيگهائي == UINT_MAX ||
		  !رڪارڊ_معلومات.resize_dirty (new_ڊيگهائي)))
      أرجع خطأ;
    HBUINT32VAR::serialize_unsafe (رڪارڊ_معلومات.arrayZ + وٿي, value);
    أرجع سچ;
  }

  منطقي append_بائيٽون (hb_ubytes_t رڪارڊ,
		     بنانشان component_سائيز,
		     بنانشان شروعات,
		     بنانشان پڇاڙي)
  {
    إذا (unlikely (شروعات > پڇاڙي || پڇاڙي > component_سائيز)) أرجع خطأ;
    بنانشان وٿي = رڪارڊ_معلومات.ڊيگهائي;
    بنانشان سائيز = پڇاڙي - شروعات;
    بنانشان new_ڊيگهائي = hb_unsigned_جوڙيو_saturate (وٿي, سائيز);
    إذا (unlikely (new_ڊيگهائي == UINT_MAX ||
		  !رڪارڊ_معلومات.resize_dirty (new_ڊيگهائي)))
      أرجع خطأ;
    hb_memcpy (رڪارڊ_معلومات.arrayZ + وٿي, رڪارڊ.arrayZ + شروعات, سائيز);
    أرجع سچ;
  }

  منطقي compile_رڪارڊ (hb_ubytes_t رڪارڊ, منطقي retain_gids)
  {
    hb_ubytes_t بچيل = رڪارڊ;
    طالما (بچيل)
    {
      VarComponent::رڪارڊ_t component;
      إذا (unlikely (!VarComponent::decompile_رڪارڊ (ذريعو, بچيل,
						     خالي_اشارو, خالي_اشارو,
						     &component) ||
		    !compile_component (بچيل, component, retain_gids)))
	أرجع خطأ;
      بچيل = بچيل.sub_array (component.سائيز);
    }
    أرجع سچ;
  }

  منطقي compile_component (hb_ubytes_t رڪارڊ,
			  ثابت VarComponent::رڪارڊ_t &component,
			  منطقي retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    إذا (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      أرجع خطأ;

    uint32_t flags = component.flags;
    بنانشان gid_سائيز = component.gid_سائيز;
    إذا (new_gid > 0xFFFFu)
    {
      flags |= (بنانشان) VarComponent::flags_t::GID_IS_24_ٻٽو_انگ;
      gid_سائيز = HBGlyphID24::static_سائيز;
    }

    إذا (unlikely (!append_varint (flags))) أرجع خطأ;
    بنانشان gid_وٿي = رڪارڊ_معلومات.ڊيگهائي;
    بنانشان gid_پڇاڙي = hb_unsigned_جوڙيو_saturate (gid_وٿي, gid_سائيز);
    إذا (unlikely (gid_پڇاڙي == UINT_MAX ||
		  !رڪارڊ_معلومات.resize_dirty (gid_پڇاڙي)))
      أرجع خطأ;
    بنانشان حرف *ٻاهريون = رڪارڊ_معلومات.arrayZ + gid_وٿي;
    إذا (gid_سائيز == HBGlyphID16::static_سائيز)
      * (HBGlyphID16 *) ٻاهريون = new_gid;
    وإلا
      * (HBGlyphID24 *) ٻاهريون = new_gid;

    بنانشان cursor = component.gid_وٿي + component.gid_سائيز;
    إذا (flags & (بنانشان) VarComponent::flags_t::HAVE_CONDITION)
    {
      إذا (unlikely (!condition_map.has (component.condition_اشاريو) ||
		    !append_بائيٽون (رڪارڊ, component.سائيز,
				   cursor, component.condition_وٿي) ||
		    !append_varint (condition_map.get (component.condition_اشاريو))))
	أرجع خطأ;
      cursor = component.condition_وٿي + component.condition_سائيز;
    }
    إذا (flags & (بنانشان) VarComponent::flags_t::HAVE_AXES)
    {
      إذا (unlikely (!axis_indices_map.has (component.axis_indices_اشاريو) ||
		    !append_بائيٽون (رڪارڊ, component.سائيز,
				   cursor, component.axis_indices_وٿي) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_اشاريو))))
	أرجع خطأ;
      cursor = component.axis_indices_وٿي + component.axis_indices_سائيز;
    }
    إذا (flags & (بنانشان) VarComponent::flags_t::AXIS_قدرون_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_قدرون_var_idx;
      إذا (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_بائيٽون (رڪارڊ, component.سائيز,
				   cursor, component.axis_قدرون_var_وٿي) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	أرجع خطأ;
      cursor = component.axis_قدرون_var_وٿي + component.axis_قدرون_var_سائيز;
    }
    إذا (flags & (بنانشان) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      إذا (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_بائيٽون (رڪارڊ, component.سائيز,
				   cursor, component.transform_var_وٿي) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	أرجع خطأ;
      cursor = component.transform_var_وٿي + component.transform_var_سائيز;
    }

    أرجع append_بائيٽون (رڪارڊ, component.سائيز, cursor, component.سائيز);
  }
};

منطقي
VARC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);
  خودڪار fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    أرجع خطأ;
  };

  ثابت Coverage &ذريعو_coverage_2 = هذا+coverage;
  ثابت CFF2_اشاريو &ذريعو_رڪارڊون_2 = هذا+glyph_رڪارڊون;
  ثابت ConditionList &ذريعو_conditions_2 = هذا+conditionList;
  ثابت TupleList &ذريعو_axis_indices_2 = هذا+axisIndicesList;
  ثابت CFF2_اشاريو &ذريعو_axis_indices_اشاريو = ذريعو_axis_indices_2;
  ثابت Multi_شي_Variation_Store &ذريعو_var_store_2 = هذا+varStore;
  varc_subset_plan_t subset_plan (*هذا, c,
				  ذريعو_coverage_2,
				  ذريعو_رڪارڊون_2,
				  ذريعو_conditions_2,
				  ذريعو_axis_indices_اشاريو,
				  ذريعو_var_store_2);
  إذا (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  إذا (!subset_plan.old_indices)
    return_trace (خطأ);
  إذا (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_رڪارڊون ()))
    return_trace (fail ());

  VARC *ٻاهريون = c->serializer->شروعات_embed<VARC> ();
  إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون)))
    return_trace (خطأ);
  ٻاهريون->نسخو = نسخو;

  إذا (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  إذا (unlikely (!ٻاهريون->coverage.serialize_serialize (c->serializer,
						     subset_plan.new_gids.iter ()) ||
		(subset_plan.condition_map.get_population () &&
		 !ٻاهريون->conditionList.serialize_serialize (c->serializer,
						       &ذريعو_conditions_2,
						       subset_plan.condition_map,
						       subset_plan.varidx_map,
						       subset_plan.axis_map)) ||
		(subset_plan.axis_indices_map.get_population () &&
		 !ٻاهريون->axisIndicesList.serialize_serialize (c->serializer,
							subset_plan.selected_axis_indices.iter (),
							&subset_plan.axis_indices_معلومات_سائيز)) ||
		(subset_plan.var_indices.get_population () &&
		 !ٻاهريون->varStore.serialize_serialize (c->serializer,
						  &ذريعو_var_store_2,
						  subset_plan.var_inner_maps.as_array (),
						  subset_plan.axis_map)) ||
		!ٻاهريون->glyph_رڪارڊون.serialize_serialize (c->serializer,
						 subset_plan.رڪارڊون.iter (),
						 &subset_plan.معلومات_سائيز)))
    return_trace (خطأ);

  return_trace (سچ);
}

} /* namespace OT */

#جيڪڏهن_پورو /* OT_VAR_VARC_VARC_SUBSET_HH */
