#ifndef OT_VAR_VARC_VARC_SUBSET_HH
#define OT_VAR_VARC_VARC_SUBSET_HH

#அந்தர்பாவ "VARC.hh"

நாமாகாஶ OT {

கட்டமைப்பு varc_subset_plan_t
{
  varc_subset_plan_t (மாறிலி VARC &source_,
		      hb_subset_context_t *subset_context_,
		      மாறிலி Coverage &source_coverage_,
		      மாறிலி CFF2Index &source_records_,
		      மாறிலி ConditionList &source_conditions_,
		      மாறிலி CFF2Index &source_axis_indices_,
		      மாறிலி MultiItemVariationStore &source_var_store_) :
    source (source_),
    subset_context (subset_context_),
    source_coverage (source_coverage_),
    source_records (source_records_),
    source_conditions (source_conditions_),
    source_axis_indices (source_axis_indices_),
    source_var_store (source_var_store_)
  {}

  தருக்க collect_glyphs ()
  {
    மாறிலி hb_set_t &glyphset = subset_context->plan->_glyphset_varced;
    க்கு (auto _ : + hb_zip (source_coverage,
			    hb_range ((unsigned) source_records.மொத்தம்)))
    {
      hb_codepoint_t old_gid = _.first;
      unsigned index = _.second;
      எனில் (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	எனில் (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  திரும்பு பொய்;
	old_indices.push (index);
	new_gids.push (new_gid);
	data_size = hb_unsigned_add_saturate (data_size,
					      source_records[index].length);
      }
    }

    திரும்பு !old_indices.in_error () && !new_gids.in_error () &&
	   data_size != UINT_MAX;
  }

  தருக்க collect_indices ()
  {
    க்கு (unsigned old_index : old_indices)
    {
      hb_ubytes_t remaining = source_records[old_index];
      போது (remaining)
      {
	VarComponent::record_t component;
	எனில் (unlikely (!VarComponent::decompile_record (source, remaining,
						       nullptr, nullptr,
						       &component)))
	  திரும்பு பொய்;

	எனில் (component.flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
	{
	  எனில் (unlikely (component.condition_index >= source_conditions.get_மொத்தம் ()))
	    திரும்பு பொய்;
	  condition_indices.add (component.condition_index);
	}
	எனில் (component.flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
	{
	  எனில் (unlikely (component.axis_indices_index >= source_axis_indices.மொத்தம்))
	    திரும்பு பொய்;
	  axis_indices.add (component.axis_indices_index);
	}
	எனில் ((component.flags & (unsigned) VarComponent::flags_t::AXIS_மதிப்புகள்_HAVE_VARIATION) &&
	    component.axis_மதிப்புகள்_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.axis_மதிப்புகள்_var_idx);
	எனில் ((component.flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.transform_var_idx);

	remaining = remaining.sub_array (component.size);
      }
    }

    எனில் (unlikely (condition_indices.in_error () || axis_indices.in_error () ||
		  var_indices.in_error ()))
      திரும்பு பொய்;

    க்கு (hb_codepoint_t condition_index : condition_indices)
      எனில் (unlikely (!source_conditions[condition_index].collect_var_indices (&var_indices)))
	திரும்பு பொய்;

    திரும்பு உண்மை;
  }

  தருக்க create_maps ()
  {
    மாறிலி auto *plan = subset_context->plan;
    எனில் (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_index_map;
      க்கு (auto _ : plan->axes_index_map)
	எனில் (!plan->user_axes_location.has (plan->axes_old_index_tag_map.get (_.first)))
	  axes_index_map.set (_.first, _.second);
      எனில் (unlikely (axes_index_map.in_error ())) திரும்பு பொய்;
    }
    condition_map.add_set (&condition_indices);
    axis_indices_map.add_set (&axis_indices);
    திரும்பு !condition_map.in_error () && !axis_indices_map.in_error () &&
	   source_var_store.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  தருக்க compile_records ()
  {
    எனில் (unlikely (!records.alloc_exact (old_indices.length)))
      திரும்பு பொய்;

    தருக்க retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    எனில் (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      க்கு (unsigned old_index : old_indices)
	records.push (source_records[old_index]);
    }
    இல்லையெனில்
    {
      hb_vector_t<unsigned> record_offsets;
      record_data.alloc (data_size);
      எனில் (unlikely (!record_offsets.alloc_exact (old_indices.length + 1)))
	திரும்பு பொய்;

      க்கு (unsigned old_index : old_indices)
      {
	record_offsets.push (record_data.length);
	எனில் (unlikely (!compile_record (source_records[old_index], retain_gids)))
	  திரும்பு பொய்;
      }
      record_offsets.push (record_data.length);

      எனில் (unlikely (record_data.in_error () || record_offsets.in_error ()))
	திரும்பு பொய்;
      க்கு (unsigned i = 0; i < old_indices.length; i++)
	records.push (hb_ubytes_t (record_data.arrayZ + record_offsets[i],
				  record_offsets[i + 1] - record_offsets[i]));
      data_size = record_data.length;
    }

    திரும்பு !records.in_error ();
  }

  தருக்க select_axis_indices ()
  {
    எனில் (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_data.resize (
					  axis_indices_map.get_population ()))))
      திரும்பு பொய்;
    க்கு (unsigned i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t bytes = source_axis_indices[axis_indices_map.backward (i)];
      எனில் (axis_map)
      {
	hb_vector_t<முழு> indices;
	மாறிலி HBUINT8 *p = (மாறிலி HBUINT8 *) bytes.arrayZ;
	எனில் (unlikely (!Tuple_மதிப்புகள்::decompile (p, indices, p + bytes.length, உண்மை)))
	  திரும்பு பொய்;
	க்கு (முழு &index : indices)
	{
	  எனில் (unlikely (!axis_map->has (index))) திரும்பு பொய்;
	  index = axis_map->get (index);
	}
	auto &data = axis_indices_data[i];
	/* One control byte and at most four data bytes per value. */
	unsigned size = hb_unsigned_mul_saturate (indices.length, 5u);
	எனில் (unlikely (size == UINT_MAX || !data.resize_dirty (size))) திரும்பு பொய்;
	data.shrink (Tuple_மதிப்புகள்::compile_unsafe (indices.as_array (), data.arrayZ));
	bytes = data.as_array ();
      }
      axis_indices_data_size = hb_unsigned_add_saturate (axis_indices_data_size,
							 bytes.length);
      selected_axis_indices.push (bytes);
    }

    திரும்பு axis_indices_data_size != UINT_MAX &&
	   !selected_axis_indices.in_error ();
  }

  மாறிலி VARC &source;
  hb_subset_context_t *subset_context;
  மாறிலி Coverage &source_coverage;
  மாறிலி CFF2Index &source_records;
  மாறிலி ConditionList &source_conditions;
  மாறிலி CFF2Index &source_axis_indices;
  மாறிலி MultiItemVariationStore &source_var_store;

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

  hb_vector_t<unsigned எழுத்து> record_data;
  hb_vector_t<hb_ubytes_t> records;

  hb_vector_t<hb_ubytes_t> selected_axis_indices;
  hb_vector_t<hb_vector_t<unsigned எழுத்து>> axis_indices_data;
  unsigned axis_indices_data_size = 0;
  hb_map_t axes_index_map;
  மாறிலி hb_map_t *axis_map = nullptr;

  தனிப்பட்ட:
  தருக்க auxiliary_indices_unchanged () மாறிலி
  {
    க்கு (hb_codepoint_t old_index : condition_indices)
      எனில் (condition_map.get (old_index) != old_index)
	திரும்பு பொய்;
    க்கு (hb_codepoint_t old_index : axis_indices)
      எனில் (axis_indices_map.get (old_index) != old_index)
	திரும்பு பொய்;
    க்கு (hb_codepoint_t old_index : var_indices)
      எனில் (varidx_map.get (old_index) != old_index)
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  தருக்க append_varint (uint32_t value)
  {
    unsigned size = HBUINT32VAR::get_size (value);
    unsigned offset = record_data.length;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    எனில் (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      திரும்பு பொய்;
    HBUINT32VAR::serialize_unsafe (record_data.arrayZ + offset, value);
    திரும்பு உண்மை;
  }

  தருக்க append_bytes (hb_ubytes_t record,
		     unsigned component_size,
		     unsigned start,
		     unsigned end)
  {
    எனில் (unlikely (start > end || end > component_size)) திரும்பு பொய்;
    unsigned offset = record_data.length;
    unsigned size = end - start;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    எனில் (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      திரும்பு பொய்;
    hb_memcpy (record_data.arrayZ + offset, record.arrayZ + start, size);
    திரும்பு உண்மை;
  }

  தருக்க compile_record (hb_ubytes_t record, தருக்க retain_gids)
  {
    hb_ubytes_t remaining = record;
    போது (remaining)
    {
      VarComponent::record_t component;
      எனில் (unlikely (!VarComponent::decompile_record (source, remaining,
						     nullptr, nullptr,
						     &component) ||
		    !compile_component (remaining, component, retain_gids)))
	திரும்பு பொய்;
      remaining = remaining.sub_array (component.size);
    }
    திரும்பு உண்மை;
  }

  தருக்க compile_component (hb_ubytes_t record,
			  மாறிலி VarComponent::record_t &component,
			  தருக்க retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    எனில் (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      திரும்பு பொய்;

    uint32_t flags = component.flags;
    unsigned gid_size = component.gid_size;
    எனில் (new_gid > 0xFFFFu)
    {
      flags |= (unsigned) VarComponent::flags_t::GID_IS_24BIT;
      gid_size = HBGlyphID24::static_size;
    }

    எனில் (unlikely (!append_varint (flags))) திரும்பு பொய்;
    unsigned gid_offset = record_data.length;
    unsigned gid_end = hb_unsigned_add_saturate (gid_offset, gid_size);
    எனில் (unlikely (gid_end == UINT_MAX ||
		  !record_data.resize_dirty (gid_end)))
      திரும்பு பொய்;
    unsigned எழுத்து *out = record_data.arrayZ + gid_offset;
    எனில் (gid_size == HBGlyphID16::static_size)
      * (HBGlyphID16 *) out = new_gid;
    இல்லையெனில்
      * (HBGlyphID24 *) out = new_gid;

    unsigned cursor = component.gid_offset + component.gid_size;
    எனில் (flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
    {
      எனில் (unlikely (!condition_map.has (component.condition_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.condition_offset) ||
		    !append_varint (condition_map.get (component.condition_index))))
	திரும்பு பொய்;
      cursor = component.condition_offset + component.condition_size;
    }
    எனில் (flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
    {
      எனில் (unlikely (!axis_indices_map.has (component.axis_indices_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_indices_offset) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_index))))
	திரும்பு பொய்;
      cursor = component.axis_indices_offset + component.axis_indices_size;
    }
    எனில் (flags & (unsigned) VarComponent::flags_t::AXIS_மதிப்புகள்_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_மதிப்புகள்_var_idx;
      எனில் (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_மதிப்புகள்_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	திரும்பு பொய்;
      cursor = component.axis_மதிப்புகள்_var_offset + component.axis_மதிப்புகள்_var_size;
    }
    எனில் (flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      எனில் (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.transform_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	திரும்பு பொய்;
      cursor = component.transform_var_offset + component.transform_var_size;
    }

    திரும்பு append_bytes (record, component.size, cursor, component.size);
  }
};

தருக்க
VARC::subset (hb_subset_context_t *c) மாறிலி
{
  TRACE_SUBSET (இது);
  auto fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    திரும்பு பொய்;
  };

  மாறிலி Coverage &source_coverage = இது+coverage;
  மாறிலி CFF2Index &source_records = இது+glyphRecords;
  மாறிலி ConditionList &source_conditions = இது+conditionList;
  மாறிலி TupleList &source_axis_indices = இது+axisIndicesList;
  மாறிலி CFF2Index &source_axis_indices_index = source_axis_indices;
  மாறிலி MultiItemVariationStore &source_var_store = இது+varStore;
  varc_subset_plan_t subset_plan (*இது, c,
				  source_coverage,
				  source_records,
				  source_conditions,
				  source_axis_indices_index,
				  source_var_store);
  எனில் (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  எனில் (!subset_plan.old_indices)
    return_trace (பொய்);
  எனில் (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_records ()))
    return_trace (fail ());

  VARC *out = c->serializer->start_embed<VARC> ();
  எனில் (unlikely (!out || !c->serializer->extend_min (out)))
    return_trace (பொய்);
  out->version = version;

  எனில் (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  எனில் (unlikely (!out->coverage.serialize_serialize (c->serializer,
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
    return_trace (பொய்);

  return_trace (உண்மை);
}

} /* namespace OT */

#endif /* OT_VAR_VARC_VARC_SUBSET_HH */
