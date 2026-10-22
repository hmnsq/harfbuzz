#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_VAR_VARC_VARC_SUBSET_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_VAR_VARC_VARC_SUBSET_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "VARC.hh"

ਨਾਮਾਕਾਸ਼ OT {

ਬਣਤਰ varc_subset_plan_t
{
  varc_subset_plan_t (ਅਚਲ VARC &ਸਰੋਤ_3,
		      hb_subset_context_t *subset_context_,
		      ਅਚਲ Coverage &ਸਰੋਤ_coverage,
		      ਅਚਲ CFF2_ਸੂਚਕਾਂਕ &ਸਰੋਤ_ਰਿਕਾਰਡਾਂ,
		      ਅਚਲ ConditionList &ਸਰੋਤ_conditions,
		      ਅਚਲ CFF2_ਸੂਚਕਾਂਕ &ਸਰੋਤ_axis_indices,
		      ਅਚਲ Multi_ਵਸਤੂ_Variation_Store &ਸਰੋਤ_var_store) :
    ਸਰੋਤ (ਸਰੋਤ_3),
    subset_context (subset_context_),
    ਸਰੋਤ_coverage_2 (ਸਰੋਤ_coverage),
    ਸਰੋਤ_ਰਿਕਾਰਡਾਂ_2 (ਸਰੋਤ_ਰਿਕਾਰਡਾਂ),
    ਸਰੋਤ_conditions_2 (ਸਰੋਤ_conditions),
    ਸਰੋਤ_axis_indices_2 (ਸਰੋਤ_axis_indices),
    ਸਰੋਤ_var_store_2 (ਸਰੋਤ_var_store)
  {}

  ਤਾਰਕਿਕ collect_glyphs ()
  {
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = subset_context->plan->_glyphset_varced;
    ਲਈ (ਸਵੈਚਾਲਿਤ _ : + hb_zip (ਸਰੋਤ_coverage_2,
			    hb_range ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਸਰੋਤ_ਰਿਕਾਰਡਾਂ_2.ਗਿਣਤੀ_2)))
    {
      hb_codepoint_t old_gid = _.ਪਹਿਲਾ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2 = _.second;
      ਜੇ (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	ਜੇ (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  ਵਾਪਸ ਝੂਠ;
	old_indices.push (ਸੂਚਕਾਂਕ_2);
	new_gids.push (new_gid);
	ਅੰਕੜੇ_ਮਾਪ = hb_unsigned_ਜੋੜੋ_saturate (ਅੰਕੜੇ_ਮਾਪ,
					      ਸਰੋਤ_ਰਿਕਾਰਡਾਂ_2[ਸੂਚਕਾਂਕ_2].ਲੰਮਾਈ);
      }
    }

    ਵਾਪਸ !old_indices.ਵਿੱਚ_error () && !new_gids.ਵਿੱਚ_error () &&
	   ਅੰਕੜੇ_ਮਾਪ != UINT_MAX;
  }

  ਤਾਰਕਿਕ collect_indices ()
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਸੂਚਕਾਂਕ : old_indices)
    {
      hb_ubytes_t ਬਚਿਆ = ਸਰੋਤ_ਰਿਕਾਰਡਾਂ_2[old_ਸੂਚਕਾਂਕ];
      ਜਦੋਂ (ਬਚਿਆ)
      {
	VarComponent::ਰਿਕਾਰਡ_t component;
	ਜੇ (unlikely (!VarComponent::decompile_ਰਿਕਾਰਡ (ਸਰੋਤ, ਬਚਿਆ,
						       ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ,
						       &component)))
	  ਵਾਪਸ ਝੂਠ;

	ਜੇ (component.flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) VarComponent::flags_t::HAVE_CONDITION)
	{
	  ਜੇ (unlikely (component.condition_ਸੂਚਕਾਂਕ >= ਸਰੋਤ_conditions_2.get_ਗਿਣਤੀ ()))
	    ਵਾਪਸ ਝੂਠ;
	  condition_indices.ਜੋੜੋ (component.condition_ਸੂਚਕਾਂਕ);
	}
	ਜੇ (component.flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) VarComponent::flags_t::HAVE_AXES)
	{
	  ਜੇ (unlikely (component.axis_indices_ਸੂਚਕਾਂਕ >= ਸਰੋਤ_axis_indices_2.ਗਿਣਤੀ_2))
	    ਵਾਪਸ ਝੂਠ;
	  axis_indices.ਜੋੜੋ (component.axis_indices_ਸੂਚਕਾਂਕ);
	}
	ਜੇ ((component.flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) VarComponent::flags_t::AXIS_ਮੁੱਲਾਂ_HAVE_VARIATION) &&
	    component.axis_ਮੁੱਲਾਂ_var_idx != VarIdx::NO_VARIATION)
	  var_indices.ਜੋੜੋ (component.axis_ਮੁੱਲਾਂ_var_idx);
	ਜੇ ((component.flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.ਜੋੜੋ (component.transform_var_idx);

	ਬਚਿਆ = ਬਚਿਆ.sub_array (component.ਮਾਪ);
      }
    }

    ਜੇ (unlikely (condition_indices.ਵਿੱਚ_error () || axis_indices.ਵਿੱਚ_error () ||
		  var_indices.ਵਿੱਚ_error ()))
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (hb_codepoint_t condition_ਸੂਚਕਾਂਕ : condition_indices)
      ਜੇ (unlikely (!ਸਰੋਤ_conditions_2[condition_ਸੂਚਕਾਂਕ].collect_var_indices (&var_indices)))
	ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ create_maps ()
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ *plan = subset_context->plan;
    ਜੇ (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_ਸੂਚਕਾਂਕ_map;
      ਲਈ (ਸਵੈਚਾਲਿਤ _ : plan->axes_ਸੂਚਕਾਂਕ_map)
	ਜੇ (!plan->user_axes_location.has (plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.get (_.ਪਹਿਲਾ)))
	  axes_ਸੂਚਕਾਂਕ_map.ਸੈੱਟ (_.ਪਹਿਲਾ, _.second);
      ਜੇ (unlikely (axes_ਸੂਚਕਾਂਕ_map.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
    }
    condition_map.ਜੋੜੋ_ਸੈੱਟ (&condition_indices);
    axis_indices_map.ਜੋੜੋ_ਸੈੱਟ (&axis_indices);
    ਵਾਪਸ !condition_map.ਵਿੱਚ_error () && !axis_indices_map.ਵਿੱਚ_error () &&
	   ਸਰੋਤ_var_store_2.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  ਤਾਰਕਿਕ compile_ਰਿਕਾਰਡਾਂ ()
  {
    ਜੇ (unlikely (!ਰਿਕਾਰਡਾਂ.alloc_exact (old_indices.ਲੰਮਾਈ)))
      ਵਾਪਸ ਝੂਠ;

    ਤਾਰਕਿਕ retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    ਜੇ (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਸੂਚਕਾਂਕ : old_indices)
	ਰਿਕਾਰਡਾਂ.push (ਸਰੋਤ_ਰਿਕਾਰਡਾਂ_2[old_ਸੂਚਕਾਂਕ]);
    }
    ਨਹੀਂਤਾਂ
    {
      hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਰਿਕਾਰਡ_offsets;
      ਰਿਕਾਰਡ_ਅੰਕੜੇ.alloc (ਅੰਕੜੇ_ਮਾਪ);
      ਜੇ (unlikely (!ਰਿਕਾਰਡ_offsets.alloc_exact (old_indices.ਲੰਮਾਈ + 1)))
	ਵਾਪਸ ਝੂਠ;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਸੂਚਕਾਂਕ : old_indices)
      {
	ਰਿਕਾਰਡ_offsets.push (ਰਿਕਾਰਡ_ਅੰਕੜੇ.ਲੰਮਾਈ);
	ਜੇ (unlikely (!compile_ਰਿਕਾਰਡ (ਸਰੋਤ_ਰਿਕਾਰਡਾਂ_2[old_ਸੂਚਕਾਂਕ], retain_gids)))
	  ਵਾਪਸ ਝੂਠ;
      }
      ਰਿਕਾਰਡ_offsets.push (ਰਿਕਾਰਡ_ਅੰਕੜੇ.ਲੰਮਾਈ);

      ਜੇ (unlikely (ਰਿਕਾਰਡ_ਅੰਕੜੇ.ਵਿੱਚ_error () || ਰਿਕਾਰਡ_offsets.ਵਿੱਚ_error ()))
	ਵਾਪਸ ਝੂਠ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < old_indices.ਲੰਮਾਈ; i++)
	ਰਿਕਾਰਡਾਂ.push (hb_ubytes_t (ਰਿਕਾਰਡ_ਅੰਕੜੇ.arrayZ + ਰਿਕਾਰਡ_offsets[i],
				  ਰਿਕਾਰਡ_offsets[i + 1] - ਰਿਕਾਰਡ_offsets[i]));
      ਅੰਕੜੇ_ਮਾਪ = ਰਿਕਾਰਡ_ਅੰਕੜੇ.ਲੰਮਾਈ;
    }

    ਵਾਪਸ !ਰਿਕਾਰਡਾਂ.ਵਿੱਚ_error ();
  }

  ਤਾਰਕਿਕ select_axis_indices ()
  {
    ਜੇ (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_ਅੰਕੜੇ.resize (
					  axis_indices_map.get_population ()))))
      ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t ਬਾਈਟਾਂ = ਸਰੋਤ_axis_indices_2[axis_indices_map.backward (i)];
      ਜੇ (axis_map)
      {
	hb_ਸਦਿਸ਼_t<ਪੂਰਨ> indices;
	ਅਚਲ HBUINT8 *p = (ਅਚਲ HBUINT8 *) ਬਾਈਟਾਂ.arrayZ;
	ਜੇ (unlikely (!Tuple_ਮੁੱਲਾਂ::decompile (p, indices, p + ਬਾਈਟਾਂ.ਲੰਮਾਈ, ਸੱਚ)))
	  ਵਾਪਸ ਝੂਠ;
	ਲਈ (ਪੂਰਨ &ਸੂਚਕਾਂਕ_2 : indices)
	{
	  ਜੇ (unlikely (!axis_map->has (ਸੂਚਕਾਂਕ_2))) ਵਾਪਸ ਝੂਠ;
	  ਸੂਚਕਾਂਕ_2 = axis_map->get (ਸੂਚਕਾਂਕ_2);
	}
	ਸਵੈਚਾਲਿਤ &ਅੰਕੜੇ = axis_indices_ਅੰਕੜੇ[i];
	/* One control byte and at most four data bytes per value. */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = hb_unsigned_mul_saturate (indices.ਲੰਮਾਈ, 5u);
	ਜੇ (unlikely (ਮਾਪ == UINT_MAX || !ਅੰਕੜੇ.resize_dirty (ਮਾਪ))) ਵਾਪਸ ਝੂਠ;
	ਅੰਕੜੇ.shrink (Tuple_ਮੁੱਲਾਂ::compile_unsafe (indices.as_array (), ਅੰਕੜੇ.arrayZ));
	ਬਾਈਟਾਂ = ਅੰਕੜੇ.as_array ();
      }
      axis_indices_ਅੰਕੜੇ_ਮਾਪ = hb_unsigned_ਜੋੜੋ_saturate (axis_indices_ਅੰਕੜੇ_ਮਾਪ,
							 ਬਾਈਟਾਂ.ਲੰਮਾਈ);
      selected_axis_indices.push (ਬਾਈਟਾਂ);
    }

    ਵਾਪਸ axis_indices_ਅੰਕੜੇ_ਮਾਪ != UINT_MAX &&
	   !selected_axis_indices.ਵਿੱਚ_error ();
  }

  ਅਚਲ VARC &ਸਰੋਤ;
  hb_subset_context_t *subset_context;
  ਅਚਲ Coverage &ਸਰੋਤ_coverage_2;
  ਅਚਲ CFF2_ਸੂਚਕਾਂਕ &ਸਰੋਤ_ਰਿਕਾਰਡਾਂ_2;
  ਅਚਲ ConditionList &ਸਰੋਤ_conditions_2;
  ਅਚਲ CFF2_ਸੂਚਕਾਂਕ &ਸਰੋਤ_axis_indices_2;
  ਅਚਲ Multi_ਵਸਤੂ_Variation_Store &ਸਰੋਤ_var_store_2;

  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> old_indices;
  hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_gids;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ = 0;

  hb_ਸੈੱਟ_t condition_indices;
  hb_ਸੈੱਟ_t axis_indices;
  hb_ਸੈੱਟ_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_ਸਦਿਸ਼_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> ਰਿਕਾਰਡ_ਅੰਕੜੇ;
  hb_ਸਦਿਸ਼_t<hb_ubytes_t> ਰਿਕਾਰਡਾਂ;

  hb_ਸਦਿਸ਼_t<hb_ubytes_t> selected_axis_indices;
  hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>> axis_indices_ਅੰਕੜੇ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ axis_indices_ਅੰਕੜੇ_ਮਾਪ = 0;
  hb_map_t axes_ਸੂਚਕਾਂਕ_map;
  ਅਚਲ hb_map_t *axis_map = ਖਾਲੀ_ਸੂਚਕ;

  ਨਿਜੀ:
  ਤਾਰਕਿਕ auxiliary_indices_unchanged () ਅਚਲ
  {
    ਲਈ (hb_codepoint_t old_ਸੂਚਕਾਂਕ : condition_indices)
      ਜੇ (condition_map.get (old_ਸੂਚਕਾਂਕ) != old_ਸੂਚਕਾਂਕ)
	ਵਾਪਸ ਝੂਠ;
    ਲਈ (hb_codepoint_t old_ਸੂਚਕਾਂਕ : axis_indices)
      ਜੇ (axis_indices_map.get (old_ਸੂਚਕਾਂਕ) != old_ਸੂਚਕਾਂਕ)
	ਵਾਪਸ ਝੂਠ;
    ਲਈ (hb_codepoint_t old_ਸੂਚਕਾਂਕ : var_indices)
      ਜੇ (varidx_map.get (old_ਸੂਚਕਾਂਕ) != old_ਸੂਚਕਾਂਕ)
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ append_varint (uint32_t value)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = HBUINT32VAR::get_ਮਾਪ (value);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = ਰਿਕਾਰਡ_ਅੰਕੜੇ.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਲੰਮਾਈ = hb_unsigned_ਜੋੜੋ_saturate (ਵਿੱਥ, ਮਾਪ);
    ਜੇ (unlikely (new_ਲੰਮਾਈ == UINT_MAX ||
		  !ਰਿਕਾਰਡ_ਅੰਕੜੇ.resize_dirty (new_ਲੰਮਾਈ)))
      ਵਾਪਸ ਝੂਠ;
    HBUINT32VAR::serialize_unsafe (ਰਿਕਾਰਡ_ਅੰਕੜੇ.arrayZ + ਵਿੱਥ, value);
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ append_ਬਾਈਟਾਂ (hb_ubytes_t ਰਿਕਾਰਡ,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ component_ਮਾਪ,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ)
  {
    ਜੇ (unlikely (ਸ਼ੁਰੂ > ਸਿਰਾ || ਸਿਰਾ > component_ਮਾਪ)) ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = ਰਿਕਾਰਡ_ਅੰਕੜੇ.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = ਸਿਰਾ - ਸ਼ੁਰੂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਲੰਮਾਈ = hb_unsigned_ਜੋੜੋ_saturate (ਵਿੱਥ, ਮਾਪ);
    ਜੇ (unlikely (new_ਲੰਮਾਈ == UINT_MAX ||
		  !ਰਿਕਾਰਡ_ਅੰਕੜੇ.resize_dirty (new_ਲੰਮਾਈ)))
      ਵਾਪਸ ਝੂਠ;
    hb_memcpy (ਰਿਕਾਰਡ_ਅੰਕੜੇ.arrayZ + ਵਿੱਥ, ਰਿਕਾਰਡ.arrayZ + ਸ਼ੁਰੂ, ਮਾਪ);
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ compile_ਰਿਕਾਰਡ (hb_ubytes_t ਰਿਕਾਰਡ, ਤਾਰਕਿਕ retain_gids)
  {
    hb_ubytes_t ਬਚਿਆ = ਰਿਕਾਰਡ;
    ਜਦੋਂ (ਬਚਿਆ)
    {
      VarComponent::ਰਿਕਾਰਡ_t component;
      ਜੇ (unlikely (!VarComponent::decompile_ਰਿਕਾਰਡ (ਸਰੋਤ, ਬਚਿਆ,
						     ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ,
						     &component) ||
		    !compile_component (ਬਚਿਆ, component, retain_gids)))
	ਵਾਪਸ ਝੂਠ;
      ਬਚਿਆ = ਬਚਿਆ.sub_array (component.ਮਾਪ);
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ compile_component (hb_ubytes_t ਰਿਕਾਰਡ,
			  ਅਚਲ VarComponent::ਰਿਕਾਰਡ_t &component,
			  ਤਾਰਕਿਕ retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    ਜੇ (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      ਵਾਪਸ ਝੂਠ;

    uint32_t flags = component.flags;
    ਬਿਨਾਨਿਸ਼ਾਨੀ gid_ਮਾਪ = component.gid_ਮਾਪ;
    ਜੇ (new_gid > 0xFFFFu)
    {
      flags |= (ਬਿਨਾਨਿਸ਼ਾਨੀ) VarComponent::flags_t::GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ;
      gid_ਮਾਪ = HBGlyphID24::static_ਮਾਪ;
    }

    ਜੇ (unlikely (!append_varint (flags))) ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ gid_ਵਿੱਥ = ਰਿਕਾਰਡ_ਅੰਕੜੇ.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ gid_ਸਿਰਾ = hb_unsigned_ਜੋੜੋ_saturate (gid_ਵਿੱਥ, gid_ਮਾਪ);
    ਜੇ (unlikely (gid_ਸਿਰਾ == UINT_MAX ||
		  !ਰਿਕਾਰਡ_ਅੰਕੜੇ.resize_dirty (gid_ਸਿਰਾ)))
      ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਬਾਹਰਲਾ = ਰਿਕਾਰਡ_ਅੰਕੜੇ.arrayZ + gid_ਵਿੱਥ;
    ਜੇ (gid_ਮਾਪ == HBGlyphID16::static_ਮਾਪ)
      * (HBGlyphID16 *) ਬਾਹਰਲਾ = new_gid;
    ਨਹੀਂਤਾਂ
      * (HBGlyphID24 *) ਬਾਹਰਲਾ = new_gid;

    ਬਿਨਾਨਿਸ਼ਾਨੀ cursor = component.gid_ਵਿੱਥ + component.gid_ਮਾਪ;
    ਜੇ (flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) VarComponent::flags_t::HAVE_CONDITION)
    {
      ਜੇ (unlikely (!condition_map.has (component.condition_ਸੂਚਕਾਂਕ) ||
		    !append_ਬਾਈਟਾਂ (ਰਿਕਾਰਡ, component.ਮਾਪ,
				   cursor, component.condition_ਵਿੱਥ) ||
		    !append_varint (condition_map.get (component.condition_ਸੂਚਕਾਂਕ))))
	ਵਾਪਸ ਝੂਠ;
      cursor = component.condition_ਵਿੱਥ + component.condition_ਮਾਪ;
    }
    ਜੇ (flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) VarComponent::flags_t::HAVE_AXES)
    {
      ਜੇ (unlikely (!axis_indices_map.has (component.axis_indices_ਸੂਚਕਾਂਕ) ||
		    !append_ਬਾਈਟਾਂ (ਰਿਕਾਰਡ, component.ਮਾਪ,
				   cursor, component.axis_indices_ਵਿੱਥ) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_ਸੂਚਕਾਂਕ))))
	ਵਾਪਸ ਝੂਠ;
      cursor = component.axis_indices_ਵਿੱਥ + component.axis_indices_ਮਾਪ;
    }
    ਜੇ (flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) VarComponent::flags_t::AXIS_ਮੁੱਲਾਂ_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_ਮੁੱਲਾਂ_var_idx;
      ਜੇ (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_ਬਾਈਟਾਂ (ਰਿਕਾਰਡ, component.ਮਾਪ,
				   cursor, component.axis_ਮੁੱਲਾਂ_var_ਵਿੱਥ) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	ਵਾਪਸ ਝੂਠ;
      cursor = component.axis_ਮੁੱਲਾਂ_var_ਵਿੱਥ + component.axis_ਮੁੱਲਾਂ_var_ਮਾਪ;
    }
    ਜੇ (flags & (ਬਿਨਾਨਿਸ਼ਾਨੀ) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      ਜੇ (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_ਬਾਈਟਾਂ (ਰਿਕਾਰਡ, component.ਮਾਪ,
				   cursor, component.transform_var_ਵਿੱਥ) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	ਵਾਪਸ ਝੂਠ;
      cursor = component.transform_var_ਵਿੱਥ + component.transform_var_ਮਾਪ;
    }

    ਵਾਪਸ append_ਬਾਈਟਾਂ (ਰਿਕਾਰਡ, component.ਮਾਪ, cursor, component.ਮਾਪ);
  }
};

ਤਾਰਕਿਕ
VARC::subset (hb_subset_context_t *c) ਅਚਲ
{
  TRACE_SUBSET (ਇਹ);
  ਸਵੈਚਾਲਿਤ fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    ਵਾਪਸ ਝੂਠ;
  };

  ਅਚਲ Coverage &ਸਰੋਤ_coverage_2 = ਇਹ+coverage;
  ਅਚਲ CFF2_ਸੂਚਕਾਂਕ &ਸਰੋਤ_ਰਿਕਾਰਡਾਂ_2 = ਇਹ+glyph_ਰਿਕਾਰਡਾਂ;
  ਅਚਲ ConditionList &ਸਰੋਤ_conditions_2 = ਇਹ+conditionList;
  ਅਚਲ TupleList &ਸਰੋਤ_axis_indices_2 = ਇਹ+axisIndicesList;
  ਅਚਲ CFF2_ਸੂਚਕਾਂਕ &ਸਰੋਤ_axis_indices_ਸੂਚਕਾਂਕ = ਸਰੋਤ_axis_indices_2;
  ਅਚਲ Multi_ਵਸਤੂ_Variation_Store &ਸਰੋਤ_var_store_2 = ਇਹ+varStore;
  varc_subset_plan_t subset_plan (*ਇਹ, c,
				  ਸਰੋਤ_coverage_2,
				  ਸਰੋਤ_ਰਿਕਾਰਡਾਂ_2,
				  ਸਰੋਤ_conditions_2,
				  ਸਰੋਤ_axis_indices_ਸੂਚਕਾਂਕ,
				  ਸਰੋਤ_var_store_2);
  ਜੇ (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  ਜੇ (!subset_plan.old_indices)
    return_trace (ਝੂਠ);
  ਜੇ (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_ਰਿਕਾਰਡਾਂ ()))
    return_trace (fail ());

  VARC *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed<VARC> ();
  ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ)))
    return_trace (ਝੂਠ);
  ਬਾਹਰਲਾ->ਸੰਸਕਰਣ = ਸੰਸਕਰਣ;

  ਜੇ (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  ਜੇ (unlikely (!ਬਾਹਰਲਾ->coverage.serialize_serialize (c->serializer,
						     subset_plan.new_gids.iter ()) ||
		(subset_plan.condition_map.get_population () &&
		 !ਬਾਹਰਲਾ->conditionList.serialize_serialize (c->serializer,
						       &ਸਰੋਤ_conditions_2,
						       subset_plan.condition_map,
						       subset_plan.varidx_map,
						       subset_plan.axis_map)) ||
		(subset_plan.axis_indices_map.get_population () &&
		 !ਬਾਹਰਲਾ->axisIndicesList.serialize_serialize (c->serializer,
							subset_plan.selected_axis_indices.iter (),
							&subset_plan.axis_indices_ਅੰਕੜੇ_ਮਾਪ)) ||
		(subset_plan.var_indices.get_population () &&
		 !ਬਾਹਰਲਾ->varStore.serialize_serialize (c->serializer,
						  &ਸਰੋਤ_var_store_2,
						  subset_plan.var_inner_maps.as_array (),
						  subset_plan.axis_map)) ||
		!ਬਾਹਰਲਾ->glyph_ਰਿਕਾਰਡਾਂ.serialize_serialize (c->serializer,
						 subset_plan.ਰਿਕਾਰਡਾਂ.iter (),
						 &subset_plan.ਅੰਕੜੇ_ਮਾਪ)))
    return_trace (ਝੂਠ);

  return_trace (ਸੱਚ);
}

} /* namespace OT */

#ਜੇ_ਅੰਤ /* OT_VAR_VARC_VARC_SUBSET_HH */
