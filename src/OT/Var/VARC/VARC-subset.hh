#ifndef OT_VAR_VARC_VARC_SUBSET_HH
#define OT_VAR_VARC_VARC_SUBSET_HH

#ضمن "VARC.hh"

نطاق OT {

بنية varc_subset_plan_t
{
  varc_subset_plan_t (ثابت VARC &source_,
		      hb_subset_context_t *subset_context_,
		      ثابت Coverage &source_coverage_,
		      ثابت CFF2Index &source_records_,
		      ثابت ConditionList &source_conditions_,
		      ثابت CFF2Index &source_axis_indices_,
		      ثابت MultiItemVariationStore &source_var_store_) :
    source (source_),
    subset_context (subset_context_),
    source_coverage (source_coverage_),
    source_records (source_records_),
    source_conditions (source_conditions_),
    source_axis_indices (source_axis_indices_),
    source_var_store (source_var_store_)
  {}

  منطقي collect_glyphs ()
  {
    ثابت hb_set_t &glyphset = subset_context->plan->_glyphset_varced;
    עבור (auto _ : + hb_zip (source_coverage,
			    hb_range ((unsigned) source_records.count)))
    {
      hb_codepoint_t old_gid = _.first;
      unsigned index = _.second;
      אם (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	אם (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  החזר שקר;
	old_indices.push (index);
	new_gids.push (new_gid);
	data_size = hb_unsigned_add_saturate (data_size,
					      source_records[index].length);
      }
    }

    החזר !old_indices.in_error () && !new_gids.in_error () &&
	   data_size != UINT_MAX;
  }

  منطقي collect_indices ()
  {
    עבור (unsigned old_index : old_indices)
    {
      hb_ubytes_t remaining = source_records[old_index];
      כלעוד (remaining)
      {
	VarComponent::record_t component;
	אם (unlikely (!VarComponent::decompile_record (source, remaining,
						       nullptr, nullptr,
						       &component)))
	  החזר שקר;

	אם (component.flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
	{
	  אם (unlikely (component.condition_index >= source_conditions.get_count ()))
	    החזר שקר;
	  condition_indices.add (component.condition_index);
	}
	אם (component.flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
	{
	  אם (unlikely (component.axis_indices_index >= source_axis_indices.count))
	    החזר שקר;
	  axis_indices.add (component.axis_indices_index);
	}
	אם ((component.flags & (unsigned) VarComponent::flags_t::AXIS_VALUES_HAVE_VARIATION) &&
	    component.axis_values_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.axis_values_var_idx);
	אם ((component.flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.transform_var_idx);

	remaining = remaining.sub_array (component.size);
      }
    }

    אם (unlikely (condition_indices.in_error () || axis_indices.in_error () ||
		  var_indices.in_error ()))
      החזר שקר;

    עבור (hb_codepoint_t condition_index : condition_indices)
      אם (unlikely (!source_conditions[condition_index].collect_var_indices (&var_indices)))
	החזר שקר;

    החזר אמת;
  }

  منطقي create_maps ()
  {
    ثابت auto *plan = subset_context->plan;
    אם (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_index_map;
      עבור (auto _ : plan->axes_index_map)
	אם (!plan->user_axes_location.has (plan->axes_old_index_tag_map.get (_.first)))
	  axes_index_map.set (_.first, _.second);
      אם (unlikely (axes_index_map.in_error ())) החזר שקר;
    }
    condition_map.add_set (&condition_indices);
    axis_indices_map.add_set (&axis_indices);
    החזר !condition_map.in_error () && !axis_indices_map.in_error () &&
	   source_var_store.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  منطقي compile_records ()
  {
    אם (unlikely (!records.alloc_exact (old_indices.length)))
      החזר שקר;

    منطقي retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    אם (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      עבור (unsigned old_index : old_indices)
	records.push (source_records[old_index]);
    }
    אחרת
    {
      hb_vector_t<unsigned> record_offsets;
      record_data.alloc (data_size);
      אם (unlikely (!record_offsets.alloc_exact (old_indices.length + 1)))
	החזר שקר;

      עבור (unsigned old_index : old_indices)
      {
	record_offsets.push (record_data.length);
	אם (unlikely (!compile_record (source_records[old_index], retain_gids)))
	  החזר שקר;
      }
      record_offsets.push (record_data.length);

      אם (unlikely (record_data.in_error () || record_offsets.in_error ()))
	החזר שקר;
      עבור (unsigned i = 0; i < old_indices.length; i++)
	records.push (hb_ubytes_t (record_data.arrayZ + record_offsets[i],
				  record_offsets[i + 1] - record_offsets[i]));
      data_size = record_data.length;
    }

    החזר !records.in_error ();
  }

  منطقي select_axis_indices ()
  {
    אם (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_data.resize (
					  axis_indices_map.get_population ()))))
      החזר שקר;
    עבור (unsigned i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t bytes = source_axis_indices[axis_indices_map.backward (i)];
      אם (axis_map)
      {
	hb_vector_t<صحيح> indices;
	ثابت HBUINT8 *p = (ثابت HBUINT8 *) bytes.arrayZ;
	אם (unlikely (!TupleValues::decompile (p, indices, p + bytes.length, אמת)))
	  החזר שקר;
	עבור (صحيح &index : indices)
	{
	  אם (unlikely (!axis_map->has (index))) החזר שקר;
	  index = axis_map->get (index);
	}
	auto &data = axis_indices_data[i];
	/* One control byte and at most four data bytes per value. */
	unsigned size = hb_unsigned_mul_saturate (indices.length, 5u);
	אם (unlikely (size == UINT_MAX || !data.resize_dirty (size))) החזר שקר;
	data.shrink (TupleValues::compile_unsafe (indices.as_array (), data.arrayZ));
	bytes = data.as_array ();
      }
      axis_indices_data_size = hb_unsigned_add_saturate (axis_indices_data_size,
							 bytes.length);
      selected_axis_indices.push (bytes);
    }

    החזר axis_indices_data_size != UINT_MAX &&
	   !selected_axis_indices.in_error ();
  }

  ثابت VARC &source;
  hb_subset_context_t *subset_context;
  ثابت Coverage &source_coverage;
  ثابت CFF2Index &source_records;
  ثابت ConditionList &source_conditions;
  ثابت CFF2Index &source_axis_indices;
  ثابت MultiItemVariationStore &source_var_store;

  hb_vector_t<unsigned> old_indices;
  hb_sorted_vector_t<hb_codepoint_t> new_gids;
  unsigned data_size = 0;

  hb_set_t condition_indices;
  hb_set_t axis_indices;
  hb_set_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_vector_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_vector_t<unsigned حرف> record_data;
  hb_vector_t<hb_ubytes_t> records;

  hb_vector_t<hb_ubytes_t> selected_axis_indices;
  hb_vector_t<hb_vector_t<unsigned حرف>> axis_indices_data;
  unsigned axis_indices_data_size = 0;
  hb_map_t axes_index_map;
  ثابت hb_map_t *axis_map = nullptr;

  خاص:
  منطقي auxiliary_indices_unchanged () ثابت
  {
    עבור (hb_codepoint_t old_index : condition_indices)
      אם (condition_map.get (old_index) != old_index)
	החזר שקר;
    עבור (hb_codepoint_t old_index : axis_indices)
      אם (axis_indices_map.get (old_index) != old_index)
	החזר שקר;
    עבור (hb_codepoint_t old_index : var_indices)
      אם (varidx_map.get (old_index) != old_index)
	החזר שקר;
    החזר אמת;
  }

  منطقي append_varint (uint32_t value)
  {
    unsigned size = HBUINT32VAR::get_size (value);
    unsigned offset = record_data.length;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    אם (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      החזר שקר;
    HBUINT32VAR::serialize_unsafe (record_data.arrayZ + offset, value);
    החזר אמת;
  }

  منطقي append_bytes (hb_ubytes_t record,
		     unsigned component_size,
		     unsigned start,
		     unsigned end)
  {
    אם (unlikely (start > end || end > component_size)) החזר שקר;
    unsigned offset = record_data.length;
    unsigned size = end - start;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    אם (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      החזר שקר;
    hb_memcpy (record_data.arrayZ + offset, record.arrayZ + start, size);
    החזר אמת;
  }

  منطقي compile_record (hb_ubytes_t record, منطقي retain_gids)
  {
    hb_ubytes_t remaining = record;
    כלעוד (remaining)
    {
      VarComponent::record_t component;
      אם (unlikely (!VarComponent::decompile_record (source, remaining,
						     nullptr, nullptr,
						     &component) ||
		    !compile_component (remaining, component, retain_gids)))
	החזר שקר;
      remaining = remaining.sub_array (component.size);
    }
    החזר אמת;
  }

  منطقي compile_component (hb_ubytes_t record,
			  ثابت VarComponent::record_t &component,
			  منطقي retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    אם (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      החזר שקר;

    uint32_t flags = component.flags;
    unsigned gid_size = component.gid_size;
    אם (new_gid > 0xFFFFu)
    {
      flags |= (unsigned) VarComponent::flags_t::GID_IS_24BIT;
      gid_size = HBGlyphID24::static_size;
    }

    אם (unlikely (!append_varint (flags))) החזר שקר;
    unsigned gid_offset = record_data.length;
    unsigned gid_end = hb_unsigned_add_saturate (gid_offset, gid_size);
    אם (unlikely (gid_end == UINT_MAX ||
		  !record_data.resize_dirty (gid_end)))
      החזר שקר;
    unsigned حرف *out = record_data.arrayZ + gid_offset;
    אם (gid_size == HBGlyphID16::static_size)
      * (HBGlyphID16 *) out = new_gid;
    אחרת
      * (HBGlyphID24 *) out = new_gid;

    unsigned cursor = component.gid_offset + component.gid_size;
    אם (flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
    {
      אם (unlikely (!condition_map.has (component.condition_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.condition_offset) ||
		    !append_varint (condition_map.get (component.condition_index))))
	החזר שקר;
      cursor = component.condition_offset + component.condition_size;
    }
    אם (flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
    {
      אם (unlikely (!axis_indices_map.has (component.axis_indices_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_indices_offset) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_index))))
	החזר שקר;
      cursor = component.axis_indices_offset + component.axis_indices_size;
    }
    אם (flags & (unsigned) VarComponent::flags_t::AXIS_VALUES_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_values_var_idx;
      אם (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_values_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	החזר שקר;
      cursor = component.axis_values_var_offset + component.axis_values_var_size;
    }
    אם (flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      אם (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.transform_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	החזר שקר;
      cursor = component.transform_var_offset + component.transform_var_size;
    }

    החזר append_bytes (record, component.size, cursor, component.size);
  }
};

منطقي
VARC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);
  auto fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    החזר שקר;
  };

  ثابت Coverage &source_coverage = هذا+coverage;
  ثابت CFF2Index &source_records = هذا+glyphRecords;
  ثابت ConditionList &source_conditions = هذا+conditionList;
  ثابت TupleList &source_axis_indices = هذا+axisIndicesList;
  ثابت CFF2Index &source_axis_indices_index = source_axis_indices;
  ثابت MultiItemVariationStore &source_var_store = هذا+varStore;
  varc_subset_plan_t subset_plan (*هذا, c,
				  source_coverage,
				  source_records,
				  source_conditions,
				  source_axis_indices_index,
				  source_var_store);
  אם (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  אם (!subset_plan.old_indices)
    return_trace (שקר);
  אם (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_records ()))
    return_trace (fail ());

  VARC *out = c->serializer->start_embed<VARC> ();
  אם (unlikely (!out || !c->serializer->extend_min (out)))
    return_trace (שקר);
  out->version = version;

  אם (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  אם (unlikely (!out->coverage.serialize_serialize (c->serializer,
						     subset_plan.new_gids.iter ()) ||
		(subset_plan.condition_map.get_population () &&
		 !out->conditionList.serialize_serialize (c->serializer,
						       &source_conditions,
						       subset_plan.condition_map,
						       subset_plan.varidx_map,
						       subset_plan.axis_map)) ||
		(subset_plan.axis_indices_map.get_population () &&
		 !out->axisIndicesList.serialize_serialize (c->serializer,
							subset_plan.selected_axis_indices.iter (),
							&subset_plan.axis_indices_data_size)) ||
		(subset_plan.var_indices.get_population () &&
		 !out->varStore.serialize_serialize (c->serializer,
						  &source_var_store,
						  subset_plan.var_inner_maps.as_array (),
						  subset_plan.axis_map)) ||
		!out->glyphRecords.serialize_serialize (c->serializer,
						 subset_plan.records.iter (),
						 &subset_plan.data_size)))
    return_trace (שקר);

  return_trace (אמת);
}

} /* namespace OT */

#endif /* OT_VAR_VARC_VARC_SUBSET_HH */
