#جے_بنا_تعریف OT_VAR_VARC_VARC_SUBSET_HH
#تعریف_کرو OT_VAR_VARC_VARC_SUBSET_HH

#ضمن "VARC.hh"

نطاق OT {

بنية varc_subset_plan_t
{
  varc_subset_plan_t (ثابت VARC &سروت_3,
		      hb_subset_context_t *subset_context_,
		      ثابت Coverage &سروت_coverage,
		      ثابت CFF2_اشاریہ &سروت_ریکارڈاں,
		      ثابت ConditionList &سروت_conditions,
		      ثابت CFF2_اشاریہ &سروت_axis_indices,
		      ثابت Multi_شے_Variation_Store &سروت_var_store) :
    سروت (سروت_3),
    subset_context (subset_context_),
    سروت_coverage_2 (سروت_coverage),
    سروت_ریکارڈاں_2 (سروت_ریکارڈاں),
    سروت_conditions_2 (سروت_conditions),
    سروت_axis_indices_2 (سروت_axis_indices),
    سروت_var_store_2 (سروت_var_store)
  {}

  منطقي collect_glyphs ()
  {
    ثابت hb_سیٹ_t &glyphset = subset_context->plan->_glyphset_varced;
    لكل (خودکار _ : + hb_zip (سروت_coverage_2,
			    hb_range ((بنانشانی) سروت_ریکارڈاں_2.گنتی)))
    {
      hb_codepoint_t old_gid = _.پہلا;
      بنانشانی اشاریہ_2 = _.second;
      إذا (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	إذا (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  أرجع خطأ;
	old_indices.push (اشاریہ_2);
	new_gids.push (new_gid);
	انکڑے_ناپ = hb_unsigned_جوڑو_saturate (انکڑے_ناپ,
					      سروت_ریکارڈاں_2[اشاریہ_2].لمائی);
      }
    }

    أرجع !old_indices.وچ_error () && !new_gids.وچ_error () &&
	   انکڑے_ناپ != UINT_MAX;
  }

  منطقي collect_indices ()
  {
    لكل (بنانشانی old_اشاریہ : old_indices)
    {
      hb_ubytes_t بچیا = سروت_ریکارڈاں_2[old_اشاریہ];
      طالما (بچیا)
      {
	VarComponent::ریکارڈ_t component;
	إذا (unlikely (!VarComponent::decompile_ریکارڈ (سروت, بچیا,
						       خالی_سوچک, خالی_سوچک,
						       &component)))
	  أرجع خطأ;

	إذا (component.flags & (بنانشانی) VarComponent::flags_t::HAVE_CONDITION)
	{
	  إذا (unlikely (component.condition_اشاریہ >= سروت_conditions_2.get_گنتی ()))
	    أرجع خطأ;
	  condition_indices.جوڑو (component.condition_اشاریہ);
	}
	إذا (component.flags & (بنانشانی) VarComponent::flags_t::HAVE_AXES)
	{
	  إذا (unlikely (component.axis_indices_اشاریہ >= سروت_axis_indices_2.گنتی))
	    أرجع خطأ;
	  axis_indices.جوڑو (component.axis_indices_اشاریہ);
	}
	إذا ((component.flags & (بنانشانی) VarComponent::flags_t::AXIS_اقدار_HAVE_VARIATION) &&
	    component.axis_اقدار_var_idx != VarIdx::NO_VARIATION)
	  var_indices.جوڑو (component.axis_اقدار_var_idx);
	إذا ((component.flags & (بنانشانی) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.جوڑو (component.transform_var_idx);

	بچیا = بچیا.sub_array (component.ناپ);
      }
    }

    إذا (unlikely (condition_indices.وچ_error () || axis_indices.وچ_error () ||
		  var_indices.وچ_error ()))
      أرجع خطأ;

    لكل (hb_codepoint_t condition_اشاریہ : condition_indices)
      إذا (unlikely (!سروت_conditions_2[condition_اشاریہ].collect_var_indices (&var_indices)))
	أرجع خطأ;

    أرجع سچ;
  }

  منطقي create_maps ()
  {
    ثابت خودکار *plan = subset_context->plan;
    إذا (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_اشاریہ_map;
      لكل (خودکار _ : plan->axes_اشاریہ_map)
	إذا (!plan->user_axes_location.has (plan->axes_old_اشاریہ_ٹیگ_map.get (_.پہلا)))
	  axes_اشاریہ_map.سیٹ (_.پہلا, _.second);
      إذا (unlikely (axes_اشاریہ_map.وچ_error ())) أرجع خطأ;
    }
    condition_map.جوڑو_سیٹ (&condition_indices);
    axis_indices_map.جوڑو_سیٹ (&axis_indices);
    أرجع !condition_map.وچ_error () && !axis_indices_map.وچ_error () &&
	   سروت_var_store_2.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  منطقي compile_ریکارڈاں ()
  {
    إذا (unlikely (!ریکارڈاں.alloc_exact (old_indices.لمائی)))
      أرجع خطأ;

    منطقي retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    إذا (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      لكل (بنانشانی old_اشاریہ : old_indices)
	ریکارڈاں.push (سروت_ریکارڈاں_2[old_اشاریہ]);
    }
    وإلا
    {
      hb_سمتیہ_t<بنانشانی> ریکارڈ_offsets;
      ریکارڈ_انکڑے.alloc (انکڑے_ناپ);
      إذا (unlikely (!ریکارڈ_offsets.alloc_exact (old_indices.لمائی + 1)))
	أرجع خطأ;

      لكل (بنانشانی old_اشاریہ : old_indices)
      {
	ریکارڈ_offsets.push (ریکارڈ_انکڑے.لمائی);
	إذا (unlikely (!compile_ریکارڈ (سروت_ریکارڈاں_2[old_اشاریہ], retain_gids)))
	  أرجع خطأ;
      }
      ریکارڈ_offsets.push (ریکارڈ_انکڑے.لمائی);

      إذا (unlikely (ریکارڈ_انکڑے.وچ_error () || ریکارڈ_offsets.وچ_error ()))
	أرجع خطأ;
      لكل (بنانشانی i = 0; i < old_indices.لمائی; i++)
	ریکارڈاں.push (hb_ubytes_t (ریکارڈ_انکڑے.arrayZ + ریکارڈ_offsets[i],
				  ریکارڈ_offsets[i + 1] - ریکارڈ_offsets[i]));
      انکڑے_ناپ = ریکارڈ_انکڑے.لمائی;
    }

    أرجع !ریکارڈاں.وچ_error ();
  }

  منطقي select_axis_indices ()
  {
    إذا (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_انکڑے.resize (
					  axis_indices_map.get_population ()))))
      أرجع خطأ;
    لكل (بنانشانی i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t بائٹاں = سروت_axis_indices_2[axis_indices_map.backward (i)];
      إذا (axis_map)
      {
	hb_سمتیہ_t<صحيح> indices;
	ثابت HBUINT8 *p = (ثابت HBUINT8 *) بائٹاں.arrayZ;
	إذا (unlikely (!Tuple_اقدار::decompile (p, indices, p + بائٹاں.لمائی, سچ)))
	  أرجع خطأ;
	لكل (صحيح &اشاریہ_2 : indices)
	{
	  إذا (unlikely (!axis_map->has (اشاریہ_2))) أرجع خطأ;
	  اشاریہ_2 = axis_map->get (اشاریہ_2);
	}
	خودکار &انکڑے = axis_indices_انکڑے[i];
	/* One control byte and at most four data bytes per value. */
	بنانشانی ناپ = hb_unsigned_mul_saturate (indices.لمائی, 5u);
	إذا (unlikely (ناپ == UINT_MAX || !انکڑے.resize_dirty (ناپ))) أرجع خطأ;
	انکڑے.shrink (Tuple_اقدار::compile_unsafe (indices.as_array (), انکڑے.arrayZ));
	بائٹاں = انکڑے.as_array ();
      }
      axis_indices_انکڑے_ناپ = hb_unsigned_جوڑو_saturate (axis_indices_انکڑے_ناپ,
							 بائٹاں.لمائی);
      selected_axis_indices.push (بائٹاں);
    }

    أرجع axis_indices_انکڑے_ناپ != UINT_MAX &&
	   !selected_axis_indices.وچ_error ();
  }

  ثابت VARC &سروت;
  hb_subset_context_t *subset_context;
  ثابت Coverage &سروت_coverage_2;
  ثابت CFF2_اشاریہ &سروت_ریکارڈاں_2;
  ثابت ConditionList &سروت_conditions_2;
  ثابت CFF2_اشاریہ &سروت_axis_indices_2;
  ثابت Multi_شے_Variation_Store &سروت_var_store_2;

  hb_سمتیہ_t<بنانشانی> old_indices;
  hb_sorted_سمتیہ_t<hb_codepoint_t> new_gids;
  بنانشانی انکڑے_ناپ = 0;

  hb_سیٹ_t condition_indices;
  hb_سیٹ_t axis_indices;
  hb_سیٹ_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_سمتیہ_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_سمتیہ_t<بنانشانی حرف> ریکارڈ_انکڑے;
  hb_سمتیہ_t<hb_ubytes_t> ریکارڈاں;

  hb_سمتیہ_t<hb_ubytes_t> selected_axis_indices;
  hb_سمتیہ_t<hb_سمتیہ_t<بنانشانی حرف>> axis_indices_انکڑے;
  بنانشانی axis_indices_انکڑے_ناپ = 0;
  hb_map_t axes_اشاریہ_map;
  ثابت hb_map_t *axis_map = خالی_سوچک;

  خاص:
  منطقي auxiliary_indices_unchanged () ثابت
  {
    لكل (hb_codepoint_t old_اشاریہ : condition_indices)
      إذا (condition_map.get (old_اشاریہ) != old_اشاریہ)
	أرجع خطأ;
    لكل (hb_codepoint_t old_اشاریہ : axis_indices)
      إذا (axis_indices_map.get (old_اشاریہ) != old_اشاریہ)
	أرجع خطأ;
    لكل (hb_codepoint_t old_اشاریہ : var_indices)
      إذا (varidx_map.get (old_اشاریہ) != old_اشاریہ)
	أرجع خطأ;
    أرجع سچ;
  }

  منطقي append_varint (uint32_t value)
  {
    بنانشانی ناپ = HBUINT32VAR::get_ناپ (value);
    بنانشانی وتھ = ریکارڈ_انکڑے.لمائی;
    بنانشانی new_لمائی = hb_unsigned_جوڑو_saturate (وتھ, ناپ);
    إذا (unlikely (new_لمائی == UINT_MAX ||
		  !ریکارڈ_انکڑے.resize_dirty (new_لمائی)))
      أرجع خطأ;
    HBUINT32VAR::serialize_unsafe (ریکارڈ_انکڑے.arrayZ + وتھ, value);
    أرجع سچ;
  }

  منطقي append_بائٹاں (hb_ubytes_t ریکارڈ,
		     بنانشانی component_ناپ,
		     بنانشانی شروع,
		     بنانشانی سرا)
  {
    إذا (unlikely (شروع > سرا || سرا > component_ناپ)) أرجع خطأ;
    بنانشانی وتھ = ریکارڈ_انکڑے.لمائی;
    بنانشانی ناپ = سرا - شروع;
    بنانشانی new_لمائی = hb_unsigned_جوڑو_saturate (وتھ, ناپ);
    إذا (unlikely (new_لمائی == UINT_MAX ||
		  !ریکارڈ_انکڑے.resize_dirty (new_لمائی)))
      أرجع خطأ;
    hb_memcpy (ریکارڈ_انکڑے.arrayZ + وتھ, ریکارڈ.arrayZ + شروع, ناپ);
    أرجع سچ;
  }

  منطقي compile_ریکارڈ (hb_ubytes_t ریکارڈ, منطقي retain_gids)
  {
    hb_ubytes_t بچیا = ریکارڈ;
    طالما (بچیا)
    {
      VarComponent::ریکارڈ_t component;
      إذا (unlikely (!VarComponent::decompile_ریکارڈ (سروت, بچیا,
						     خالی_سوچک, خالی_سوچک,
						     &component) ||
		    !compile_component (بچیا, component, retain_gids)))
	أرجع خطأ;
      بچیا = بچیا.sub_array (component.ناپ);
    }
    أرجع سچ;
  }

  منطقي compile_component (hb_ubytes_t ریکارڈ,
			  ثابت VarComponent::ریکارڈ_t &component,
			  منطقي retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    إذا (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      أرجع خطأ;

    uint32_t flags = component.flags;
    بنانشانی gid_ناپ = component.gid_ناپ;
    إذا (new_gid > 0xFFFFu)
    {
      flags |= (بنانشانی) VarComponent::flags_t::GID_IS_24_ثنائی_ہندسہ;
      gid_ناپ = HBGlyphID24::static_ناپ;
    }

    إذا (unlikely (!append_varint (flags))) أرجع خطأ;
    بنانشانی gid_وتھ = ریکارڈ_انکڑے.لمائی;
    بنانشانی gid_سرا = hb_unsigned_جوڑو_saturate (gid_وتھ, gid_ناپ);
    إذا (unlikely (gid_سرا == UINT_MAX ||
		  !ریکارڈ_انکڑے.resize_dirty (gid_سرا)))
      أرجع خطأ;
    بنانشانی حرف *باہرلا = ریکارڈ_انکڑے.arrayZ + gid_وتھ;
    إذا (gid_ناپ == HBGlyphID16::static_ناپ)
      * (HBGlyphID16 *) باہرلا = new_gid;
    وإلا
      * (HBGlyphID24 *) باہرلا = new_gid;

    بنانشانی cursor = component.gid_وتھ + component.gid_ناپ;
    إذا (flags & (بنانشانی) VarComponent::flags_t::HAVE_CONDITION)
    {
      إذا (unlikely (!condition_map.has (component.condition_اشاریہ) ||
		    !append_بائٹاں (ریکارڈ, component.ناپ,
				   cursor, component.condition_وتھ) ||
		    !append_varint (condition_map.get (component.condition_اشاریہ))))
	أرجع خطأ;
      cursor = component.condition_وتھ + component.condition_ناپ;
    }
    إذا (flags & (بنانشانی) VarComponent::flags_t::HAVE_AXES)
    {
      إذا (unlikely (!axis_indices_map.has (component.axis_indices_اشاریہ) ||
		    !append_بائٹاں (ریکارڈ, component.ناپ,
				   cursor, component.axis_indices_وتھ) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_اشاریہ))))
	أرجع خطأ;
      cursor = component.axis_indices_وتھ + component.axis_indices_ناپ;
    }
    إذا (flags & (بنانشانی) VarComponent::flags_t::AXIS_اقدار_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_اقدار_var_idx;
      إذا (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_بائٹاں (ریکارڈ, component.ناپ,
				   cursor, component.axis_اقدار_var_وتھ) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	أرجع خطأ;
      cursor = component.axis_اقدار_var_وتھ + component.axis_اقدار_var_ناپ;
    }
    إذا (flags & (بنانشانی) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      إذا (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_بائٹاں (ریکارڈ, component.ناپ,
				   cursor, component.transform_var_وتھ) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	أرجع خطأ;
      cursor = component.transform_var_وتھ + component.transform_var_ناپ;
    }

    أرجع append_بائٹاں (ریکارڈ, component.ناپ, cursor, component.ناپ);
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

  ثابت Coverage &سروت_coverage_2 = هذا+coverage;
  ثابت CFF2_اشاریہ &سروت_ریکارڈاں_2 = هذا+glyph_ریکارڈاں;
  ثابت ConditionList &سروت_conditions_2 = هذا+conditionList;
  ثابت TupleList &سروت_axis_indices_2 = هذا+axisIndicesList;
  ثابت CFF2_اشاریہ &سروت_axis_indices_اشاریہ = سروت_axis_indices_2;
  ثابت Multi_شے_Variation_Store &سروت_var_store_2 = هذا+varStore;
  varc_subset_plan_t subset_plan (*هذا, c,
				  سروت_coverage_2,
				  سروت_ریکارڈاں_2,
				  سروت_conditions_2,
				  سروت_axis_indices_اشاریہ,
				  سروت_var_store_2);
  إذا (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  إذا (!subset_plan.old_indices)
    return_trace (خطأ);
  إذا (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_ریکارڈاں ()))
    return_trace (fail ());

  VARC *باہرلا = c->serializer->شروع_embed<VARC> ();
  إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا)))
    return_trace (خطأ);
  باہرلا->نسخہ = نسخہ;

  إذا (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  إذا (unlikely (!باہرلا->coverage.serialize_serialize (c->serializer,
						     subset_plan.new_gids.iter ()) ||
		(subset_plan.condition_map.get_population () &&
		 !باہرلا->conditionList.serialize_serialize (c->serializer,
						       &سروت_conditions_2,
						       subset_plan.condition_map,
						       subset_plan.varidx_map,
						       subset_plan.axis_map)) ||
		(subset_plan.axis_indices_map.get_population () &&
		 !باہرلا->axisIndicesList.serialize_serialize (c->serializer,
							subset_plan.selected_axis_indices.iter (),
							&subset_plan.axis_indices_انکڑے_ناپ)) ||
		(subset_plan.var_indices.get_population () &&
		 !باہرلا->varStore.serialize_serialize (c->serializer,
						  &سروت_var_store_2,
						  subset_plan.var_inner_maps.as_array (),
						  subset_plan.axis_map)) ||
		!باہرلا->glyph_ریکارڈاں.serialize_serialize (c->serializer,
						 subset_plan.ریکارڈاں.iter (),
						 &subset_plan.انکڑے_ناپ)))
    return_trace (خطأ);

  return_trace (سچ);
}

} /* namespace OT */

#جے_انت /* OT_VAR_VARC_VARC_SUBSET_HH */
