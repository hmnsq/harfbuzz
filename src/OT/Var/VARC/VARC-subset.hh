#اگر_غیر_تعریف_شدہ OT_VAR_VARC_VARC_SUBSET_HH
#تعریف_کرو OT_VAR_VARC_VARC_SUBSET_HH

#شامل "VARC.hh"

نامخلا OT {

ساخت varc_subset_plan_t
{
  varc_subset_plan_t (ثابت VARC &ماخذ_3,
		      hb_subset_context_t *subset_context_,
		      ثابت Coverage &ماخذ_coverage,
		      ثابت CFF2_اشاریہ &ماخذ_اندراجات,
		      ثابت ConditionList &ماخذ_conditions,
		      ثابت CFF2_اشاریہ &ماخذ_axis_indices,
		      ثابت Multi_شے_Variation_Store &ماخذ_var_store) :
    ماخذ (ماخذ_3),
    subset_context (subset_context_),
    ماخذ_coverage_2 (ماخذ_coverage),
    ماخذ_اندراجات_2 (ماخذ_اندراجات),
    ماخذ_conditions_2 (ماخذ_conditions),
    ماخذ_axis_indices_2 (ماخذ_axis_indices),
    ماخذ_var_store_2 (ماخذ_var_store)
  {}

  منطقی collect_glyphs ()
  {
    ثابت hb_سیٹ_t &glyphset = subset_context->plan->_glyphset_varced;
    برائے (خودکار _ : + hb_zip (ماخذ_coverage_2,
			    hb_range ((غیرعلامتی) ماخذ_اندراجات_2.گنتی)))
    {
      hb_codepoint_t old_gid = _.پہلا;
      غیرعلامتی اشاریہ_2 = _.second;
      اگر (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	اگر (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  واپس جھوٹ;
	old_indices.push (اشاریہ_2);
	new_gids.push (new_gid);
	ڈیٹا_سائز = hb_unsigned_جوڑو_saturate (ڈیٹا_سائز,
					      ماخذ_اندراجات_2[اشاریہ_2].لمبائی);
      }
    }

    واپس !old_indices.اندر_error () && !new_gids.اندر_error () &&
	   ڈیٹا_سائز != UINT_MAX;
  }

  منطقی collect_indices ()
  {
    برائے (غیرعلامتی old_اشاریہ : old_indices)
    {
      hb_ubytes_t بقایا = ماخذ_اندراجات_2[old_اشاریہ];
      جب (بقایا)
      {
	VarComponent::ریکارڈ_t component;
	اگر (unlikely (!VarComponent::decompile_ریکارڈ (ماخذ, بقایا,
						       خالی_اشاریہ, خالی_اشاریہ,
						       &component)))
	  واپس جھوٹ;

	اگر (component.flags & (غیرعلامتی) VarComponent::flags_t::HAVE_CONDITION)
	{
	  اگر (unlikely (component.condition_اشاریہ >= ماخذ_conditions_2.get_گنتی ()))
	    واپس جھوٹ;
	  condition_indices.جوڑو (component.condition_اشاریہ);
	}
	اگر (component.flags & (غیرعلامتی) VarComponent::flags_t::HAVE_AXES)
	{
	  اگر (unlikely (component.axis_indices_اشاریہ >= ماخذ_axis_indices_2.گنتی))
	    واپس جھوٹ;
	  axis_indices.جوڑو (component.axis_indices_اشاریہ);
	}
	اگر ((component.flags & (غیرعلامتی) VarComponent::flags_t::AXIS_اقدار_HAVE_VARIATION) &&
	    component.axis_اقدار_var_idx != VarIdx::NO_VARIATION)
	  var_indices.جوڑو (component.axis_اقدار_var_idx);
	اگر ((component.flags & (غیرعلامتی) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.جوڑو (component.transform_var_idx);

	بقایا = بقایا.sub_array (component.سائز);
      }
    }

    اگر (unlikely (condition_indices.اندر_error () || axis_indices.اندر_error () ||
		  var_indices.اندر_error ()))
      واپس جھوٹ;

    برائے (hb_codepoint_t condition_اشاریہ : condition_indices)
      اگر (unlikely (!ماخذ_conditions_2[condition_اشاریہ].collect_var_indices (&var_indices)))
	واپس جھوٹ;

    واپس سچ;
  }

  منطقی create_maps ()
  {
    ثابت خودکار *plan = subset_context->plan;
    اگر (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_اشاریہ_map;
      برائے (خودکار _ : plan->axes_اشاریہ_map)
	اگر (!plan->user_axes_location.has (plan->axes_old_اشاریہ_لیبل_map.get (_.پہلا)))
	  axes_اشاریہ_map.سیٹ (_.پہلا, _.second);
      اگر (unlikely (axes_اشاریہ_map.اندر_error ())) واپس جھوٹ;
    }
    condition_map.جوڑو_سیٹ (&condition_indices);
    axis_indices_map.جوڑو_سیٹ (&axis_indices);
    واپس !condition_map.اندر_error () && !axis_indices_map.اندر_error () &&
	   ماخذ_var_store_2.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  منطقی compile_اندراجات ()
  {
    اگر (unlikely (!اندراجات.alloc_exact (old_indices.لمبائی)))
      واپس جھوٹ;

    منطقی retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    اگر (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      برائے (غیرعلامتی old_اشاریہ : old_indices)
	اندراجات.push (ماخذ_اندراجات_2[old_اشاریہ]);
    }
    ورنہ
    {
      hb_سمتیہ_t<غیرعلامتی> ریکارڈ_offsets;
      ریکارڈ_ڈیٹا.alloc (ڈیٹا_سائز);
      اگر (unlikely (!ریکارڈ_offsets.alloc_exact (old_indices.لمبائی + 1)))
	واپس جھوٹ;

      برائے (غیرعلامتی old_اشاریہ : old_indices)
      {
	ریکارڈ_offsets.push (ریکارڈ_ڈیٹا.لمبائی);
	اگر (unlikely (!compile_ریکارڈ (ماخذ_اندراجات_2[old_اشاریہ], retain_gids)))
	  واپس جھوٹ;
      }
      ریکارڈ_offsets.push (ریکارڈ_ڈیٹا.لمبائی);

      اگر (unlikely (ریکارڈ_ڈیٹا.اندر_error () || ریکارڈ_offsets.اندر_error ()))
	واپس جھوٹ;
      برائے (غیرعلامتی i = 0; i < old_indices.لمبائی; i++)
	اندراجات.push (hb_ubytes_t (ریکارڈ_ڈیٹا.arrayZ + ریکارڈ_offsets[i],
				  ریکارڈ_offsets[i + 1] - ریکارڈ_offsets[i]));
      ڈیٹا_سائز = ریکارڈ_ڈیٹا.لمبائی;
    }

    واپس !اندراجات.اندر_error ();
  }

  منطقی select_axis_indices ()
  {
    اگر (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_ڈیٹا.resize (
					  axis_indices_map.get_population ()))))
      واپس جھوٹ;
    برائے (غیرعلامتی i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t بائٹیں = ماخذ_axis_indices_2[axis_indices_map.backward (i)];
      اگر (axis_map)
      {
	hb_سمتیہ_t<صحیح> indices;
	ثابت HBUINT8 *p = (ثابت HBUINT8 *) بائٹیں.arrayZ;
	اگر (unlikely (!Tuple_اقدار::decompile (p, indices, p + بائٹیں.لمبائی, سچ)))
	  واپس جھوٹ;
	برائے (صحیح &اشاریہ_2 : indices)
	{
	  اگر (unlikely (!axis_map->has (اشاریہ_2))) واپس جھوٹ;
	  اشاریہ_2 = axis_map->get (اشاریہ_2);
	}
	خودکار &ڈیٹا = axis_indices_ڈیٹا[i];
	/* One control byte and at most four data bytes per value. */
	غیرعلامتی سائز = hb_unsigned_mul_saturate (indices.لمبائی, 5u);
	اگر (unlikely (سائز == UINT_MAX || !ڈیٹا.resize_dirty (سائز))) واپس جھوٹ;
	ڈیٹا.shrink (Tuple_اقدار::compile_unsafe (indices.as_array (), ڈیٹا.arrayZ));
	بائٹیں = ڈیٹا.as_array ();
      }
      axis_indices_ڈیٹا_سائز = hb_unsigned_جوڑو_saturate (axis_indices_ڈیٹا_سائز,
							 بائٹیں.لمبائی);
      selected_axis_indices.push (بائٹیں);
    }

    واپس axis_indices_ڈیٹا_سائز != UINT_MAX &&
	   !selected_axis_indices.اندر_error ();
  }

  ثابت VARC &ماخذ;
  hb_subset_context_t *subset_context;
  ثابت Coverage &ماخذ_coverage_2;
  ثابت CFF2_اشاریہ &ماخذ_اندراجات_2;
  ثابت ConditionList &ماخذ_conditions_2;
  ثابت CFF2_اشاریہ &ماخذ_axis_indices_2;
  ثابت Multi_شے_Variation_Store &ماخذ_var_store_2;

  hb_سمتیہ_t<غیرعلامتی> old_indices;
  hb_sorted_سمتیہ_t<hb_codepoint_t> new_gids;
  غیرعلامتی ڈیٹا_سائز = 0;

  hb_سیٹ_t condition_indices;
  hb_سیٹ_t axis_indices;
  hb_سیٹ_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_سمتیہ_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_سمتیہ_t<غیرعلامتی حرف> ریکارڈ_ڈیٹا;
  hb_سمتیہ_t<hb_ubytes_t> اندراجات;

  hb_سمتیہ_t<hb_ubytes_t> selected_axis_indices;
  hb_سمتیہ_t<hb_سمتیہ_t<غیرعلامتی حرف>> axis_indices_ڈیٹا;
  غیرعلامتی axis_indices_ڈیٹا_سائز = 0;
  hb_map_t axes_اشاریہ_map;
  ثابت hb_map_t *axis_map = خالی_اشاریہ;

  نجی:
  منطقی auxiliary_indices_unchanged () ثابت
  {
    برائے (hb_codepoint_t old_اشاریہ : condition_indices)
      اگر (condition_map.get (old_اشاریہ) != old_اشاریہ)
	واپس جھوٹ;
    برائے (hb_codepoint_t old_اشاریہ : axis_indices)
      اگر (axis_indices_map.get (old_اشاریہ) != old_اشاریہ)
	واپس جھوٹ;
    برائے (hb_codepoint_t old_اشاریہ : var_indices)
      اگر (varidx_map.get (old_اشاریہ) != old_اشاریہ)
	واپس جھوٹ;
    واپس سچ;
  }

  منطقی append_varint (uint32_t value)
  {
    غیرعلامتی سائز = HBUINT32VAR::get_سائز (value);
    غیرعلامتی انحراف = ریکارڈ_ڈیٹا.لمبائی;
    غیرعلامتی new_لمبائی = hb_unsigned_جوڑو_saturate (انحراف, سائز);
    اگر (unlikely (new_لمبائی == UINT_MAX ||
		  !ریکارڈ_ڈیٹا.resize_dirty (new_لمبائی)))
      واپس جھوٹ;
    HBUINT32VAR::serialize_unsafe (ریکارڈ_ڈیٹا.arrayZ + انحراف, value);
    واپس سچ;
  }

  منطقی append_بائٹیں (hb_ubytes_t ریکارڈ,
		     غیرعلامتی component_سائز,
		     غیرعلامتی آغاز,
		     غیرعلامتی انتہا)
  {
    اگر (unlikely (آغاز > انتہا || انتہا > component_سائز)) واپس جھوٹ;
    غیرعلامتی انحراف = ریکارڈ_ڈیٹا.لمبائی;
    غیرعلامتی سائز = انتہا - آغاز;
    غیرعلامتی new_لمبائی = hb_unsigned_جوڑو_saturate (انحراف, سائز);
    اگر (unlikely (new_لمبائی == UINT_MAX ||
		  !ریکارڈ_ڈیٹا.resize_dirty (new_لمبائی)))
      واپس جھوٹ;
    hb_memcpy (ریکارڈ_ڈیٹا.arrayZ + انحراف, ریکارڈ.arrayZ + آغاز, سائز);
    واپس سچ;
  }

  منطقی compile_ریکارڈ (hb_ubytes_t ریکارڈ, منطقی retain_gids)
  {
    hb_ubytes_t بقایا = ریکارڈ;
    جب (بقایا)
    {
      VarComponent::ریکارڈ_t component;
      اگر (unlikely (!VarComponent::decompile_ریکارڈ (ماخذ, بقایا,
						     خالی_اشاریہ, خالی_اشاریہ,
						     &component) ||
		    !compile_component (بقایا, component, retain_gids)))
	واپس جھوٹ;
      بقایا = بقایا.sub_array (component.سائز);
    }
    واپس سچ;
  }

  منطقی compile_component (hb_ubytes_t ریکارڈ,
			  ثابت VarComponent::ریکارڈ_t &component,
			  منطقی retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    اگر (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      واپس جھوٹ;

    uint32_t flags = component.flags;
    غیرعلامتی gid_سائز = component.gid_سائز;
    اگر (new_gid > 0xFFFFu)
    {
      flags |= (غیرعلامتی) VarComponent::flags_t::GID_IS_24_ثنائی_ہندسہ;
      gid_سائز = HBGlyphID24::static_سائز;
    }

    اگر (unlikely (!append_varint (flags))) واپس جھوٹ;
    غیرعلامتی gid_انحراف = ریکارڈ_ڈیٹا.لمبائی;
    غیرعلامتی gid_انتہا = hb_unsigned_جوڑو_saturate (gid_انحراف, gid_سائز);
    اگر (unlikely (gid_انتہا == UINT_MAX ||
		  !ریکارڈ_ڈیٹا.resize_dirty (gid_انتہا)))
      واپس جھوٹ;
    غیرعلامتی حرف *برآمد = ریکارڈ_ڈیٹا.arrayZ + gid_انحراف;
    اگر (gid_سائز == HBGlyphID16::static_سائز)
      * (HBGlyphID16 *) برآمد = new_gid;
    ورنہ
      * (HBGlyphID24 *) برآمد = new_gid;

    غیرعلامتی cursor = component.gid_انحراف + component.gid_سائز;
    اگر (flags & (غیرعلامتی) VarComponent::flags_t::HAVE_CONDITION)
    {
      اگر (unlikely (!condition_map.has (component.condition_اشاریہ) ||
		    !append_بائٹیں (ریکارڈ, component.سائز,
				   cursor, component.condition_انحراف) ||
		    !append_varint (condition_map.get (component.condition_اشاریہ))))
	واپس جھوٹ;
      cursor = component.condition_انحراف + component.condition_سائز;
    }
    اگر (flags & (غیرعلامتی) VarComponent::flags_t::HAVE_AXES)
    {
      اگر (unlikely (!axis_indices_map.has (component.axis_indices_اشاریہ) ||
		    !append_بائٹیں (ریکارڈ, component.سائز,
				   cursor, component.axis_indices_انحراف) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_اشاریہ))))
	واپس جھوٹ;
      cursor = component.axis_indices_انحراف + component.axis_indices_سائز;
    }
    اگر (flags & (غیرعلامتی) VarComponent::flags_t::AXIS_اقدار_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_اقدار_var_idx;
      اگر (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_بائٹیں (ریکارڈ, component.سائز,
				   cursor, component.axis_اقدار_var_انحراف) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	واپس جھوٹ;
      cursor = component.axis_اقدار_var_انحراف + component.axis_اقدار_var_سائز;
    }
    اگر (flags & (غیرعلامتی) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      اگر (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_بائٹیں (ریکارڈ, component.سائز,
				   cursor, component.transform_var_انحراف) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	واپس جھوٹ;
      cursor = component.transform_var_انحراف + component.transform_var_سائز;
    }

    واپس append_بائٹیں (ریکارڈ, component.سائز, cursor, component.سائز);
  }
};

منطقی
VARC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (یہ);
  خودکار fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    واپس جھوٹ;
  };

  ثابت Coverage &ماخذ_coverage_2 = یہ+coverage;
  ثابت CFF2_اشاریہ &ماخذ_اندراجات_2 = یہ+glyph_اندراجات;
  ثابت ConditionList &ماخذ_conditions_2 = یہ+conditionList;
  ثابت TupleList &ماخذ_axis_indices_2 = یہ+axisIndicesList;
  ثابت CFF2_اشاریہ &ماخذ_axis_indices_اشاریہ = ماخذ_axis_indices_2;
  ثابت Multi_شے_Variation_Store &ماخذ_var_store_2 = یہ+varStore;
  varc_subset_plan_t subset_plan (*یہ, c,
				  ماخذ_coverage_2,
				  ماخذ_اندراجات_2,
				  ماخذ_conditions_2,
				  ماخذ_axis_indices_اشاریہ,
				  ماخذ_var_store_2);
  اگر (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  اگر (!subset_plan.old_indices)
    return_trace (جھوٹ);
  اگر (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_اندراجات ()))
    return_trace (fail ());

  VARC *برآمد = c->serializer->آغاز_embed<VARC> ();
  اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد)))
    return_trace (جھوٹ);
  برآمد->نسخہ = نسخہ;

  اگر (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  اگر (unlikely (!برآمد->coverage.serialize_serialize (c->serializer,
						     subset_plan.new_gids.iter ()) ||
		(subset_plan.condition_map.get_population () &&
		 !برآمد->conditionList.serialize_serialize (c->serializer,
						       &ماخذ_conditions_2,
						       subset_plan.condition_map,
						       subset_plan.varidx_map,
						       subset_plan.axis_map)) ||
		(subset_plan.axis_indices_map.get_population () &&
		 !برآمد->axisIndicesList.serialize_serialize (c->serializer,
							subset_plan.selected_axis_indices.iter (),
							&subset_plan.axis_indices_ڈیٹا_سائز)) ||
		(subset_plan.var_indices.get_population () &&
		 !برآمد->varStore.serialize_serialize (c->serializer,
						  &ماخذ_var_store_2,
						  subset_plan.var_inner_maps.as_array (),
						  subset_plan.axis_map)) ||
		!برآمد->glyph_اندراجات.serialize_serialize (c->serializer,
						 subset_plan.اندراجات.iter (),
						 &subset_plan.ڈیٹا_سائز)))
    return_trace (جھوٹ);

  return_trace (سچ);
}

} /* namespace OT */

#اگر_ختم /* OT_VAR_VARC_VARC_SUBSET_HH */
