#ifndef OT_VAR_VARC_VARC_SUBSET_HH
#define OT_VAR_VARC_VARC_SUBSET_HH

#અન્તર્ભાવ "VARC.hh"

નામાકાશ OT {

સંરચના varc_subset_plan_t
{
  varc_subset_plan_t (અચલ VARC &source_,
		      hb_subset_context_t *subset_context_,
		      અચલ Coverage &source_coverage_,
		      અચલ CFF2Index &source_records_,
		      અચલ ConditionList &source_conditions_,
		      અચલ CFF2Index &source_axis_indices_,
		      અચલ MultiItemVariationStore &source_var_store_) :
    source (source_),
    subset_context (subset_context_),
    source_coverage (source_coverage_),
    source_records (source_records_),
    source_conditions (source_conditions_),
    source_axis_indices (source_axis_indices_),
    source_var_store (source_var_store_)
  {}

  તાર્કિક collect_glyphs ()
  {
    અચલ hb_set_t &glyphset = subset_context->plan->_glyphset_varced;
    માટે (auto _ : + hb_zip (source_coverage,
			    hb_range ((unsigned) source_records.ગણતરી)))
    {
      hb_codepoint_t old_gid = _.first;
      unsigned index = _.second;
      જો (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	જો (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  પાછા ખોટું;
	old_indices.push (index);
	new_gids.push (new_gid);
	data_size = hb_unsigned_add_saturate (data_size,
					      source_records[index].length);
      }
    }

    પાછા !old_indices.in_error () && !new_gids.in_error () &&
	   data_size != UINT_MAX;
  }

  તાર્કિક collect_indices ()
  {
    માટે (unsigned old_index : old_indices)
    {
      hb_ubytes_t remaining = source_records[old_index];
      જ્યારે (remaining)
      {
	VarComponent::record_t component;
	જો (unlikely (!VarComponent::decompile_record (source, remaining,
						       nullptr, nullptr,
						       &component)))
	  પાછા ખોટું;

	જો (component.flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
	{
	  જો (unlikely (component.condition_index >= source_conditions.get_ગણતરી ()))
	    પાછા ખોટું;
	  condition_indices.add (component.condition_index);
	}
	જો (component.flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
	{
	  જો (unlikely (component.axis_indices_index >= source_axis_indices.ગણતરી))
	    પાછા ખોટું;
	  axis_indices.add (component.axis_indices_index);
	}
	જો ((component.flags & (unsigned) VarComponent::flags_t::AXIS_કિંમતો_HAVE_VARIATION) &&
	    component.axis_કિંમતો_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.axis_કિંમતો_var_idx);
	જો ((component.flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.transform_var_idx);

	remaining = remaining.sub_array (component.size);
      }
    }

    જો (unlikely (condition_indices.in_error () || axis_indices.in_error () ||
		  var_indices.in_error ()))
      પાછા ખોટું;

    માટે (hb_codepoint_t condition_index : condition_indices)
      જો (unlikely (!source_conditions[condition_index].collect_var_indices (&var_indices)))
	પાછા ખોટું;

    પાછા સાચું;
  }

  તાર્કિક create_maps ()
  {
    અચલ auto *plan = subset_context->plan;
    જો (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_index_map;
      માટે (auto _ : plan->axes_index_map)
	જો (!plan->user_axes_location.has (plan->axes_old_index_tag_map.get (_.first)))
	  axes_index_map.set (_.first, _.second);
      જો (unlikely (axes_index_map.in_error ())) પાછા ખોટું;
    }
    condition_map.add_set (&condition_indices);
    axis_indices_map.add_set (&axis_indices);
    પાછા !condition_map.in_error () && !axis_indices_map.in_error () &&
	   source_var_store.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  તાર્કિક compile_records ()
  {
    જો (unlikely (!records.alloc_exact (old_indices.length)))
      પાછા ખોટું;

    તાર્કિક retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    જો (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      માટે (unsigned old_index : old_indices)
	records.push (source_records[old_index]);
    }
    નહીંતર
    {
      hb_vector_t<unsigned> record_offsets;
      record_data.alloc (data_size);
      જો (unlikely (!record_offsets.alloc_exact (old_indices.length + 1)))
	પાછા ખોટું;

      માટે (unsigned old_index : old_indices)
      {
	record_offsets.push (record_data.length);
	જો (unlikely (!compile_record (source_records[old_index], retain_gids)))
	  પાછા ખોટું;
      }
      record_offsets.push (record_data.length);

      જો (unlikely (record_data.in_error () || record_offsets.in_error ()))
	પાછા ખોટું;
      માટે (unsigned i = 0; i < old_indices.length; i++)
	records.push (hb_ubytes_t (record_data.arrayZ + record_offsets[i],
				  record_offsets[i + 1] - record_offsets[i]));
      data_size = record_data.length;
    }

    પાછા !records.in_error ();
  }

  તાર્કિક select_axis_indices ()
  {
    જો (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_data.resize (
					  axis_indices_map.get_population ()))))
      પાછા ખોટું;
    માટે (unsigned i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t bytes = source_axis_indices[axis_indices_map.backward (i)];
      જો (axis_map)
      {
	hb_vector_t<પૂર્ણાંક> indices;
	અચલ HBUINT8 *p = (અચલ HBUINT8 *) bytes.arrayZ;
	જો (unlikely (!Tuple_કિંમતો::decompile (p, indices, p + bytes.length, સાચું)))
	  પાછા ખોટું;
	માટે (પૂર્ણાંક &index : indices)
	{
	  જો (unlikely (!axis_map->has (index))) પાછા ખોટું;
	  index = axis_map->get (index);
	}
	auto &data = axis_indices_data[i];
	/* One control byte and at most four data bytes per value. */
	unsigned size = hb_unsigned_mul_saturate (indices.length, 5u);
	જો (unlikely (size == UINT_MAX || !data.resize_dirty (size))) પાછા ખોટું;
	data.shrink (Tuple_કિંમતો::compile_unsafe (indices.as_array (), data.arrayZ));
	bytes = data.as_array ();
      }
      axis_indices_data_size = hb_unsigned_add_saturate (axis_indices_data_size,
							 bytes.length);
      selected_axis_indices.push (bytes);
    }

    પાછા axis_indices_data_size != UINT_MAX &&
	   !selected_axis_indices.in_error ();
  }

  અચલ VARC &source;
  hb_subset_context_t *subset_context;
  અચલ Coverage &source_coverage;
  અચલ CFF2Index &source_records;
  અચલ ConditionList &source_conditions;
  અચલ CFF2Index &source_axis_indices;
  અચલ MultiItemVariationStore &source_var_store;

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

  hb_vector_t<unsigned અક્ષર> record_data;
  hb_vector_t<hb_ubytes_t> records;

  hb_vector_t<hb_ubytes_t> selected_axis_indices;
  hb_vector_t<hb_vector_t<unsigned અક્ષર>> axis_indices_data;
  unsigned axis_indices_data_size = 0;
  hb_map_t axes_index_map;
  અચલ hb_map_t *axis_map = nullptr;

  ખાનગી:
  તાર્કિક auxiliary_indices_unchanged () અચલ
  {
    માટે (hb_codepoint_t old_index : condition_indices)
      જો (condition_map.get (old_index) != old_index)
	પાછા ખોટું;
    માટે (hb_codepoint_t old_index : axis_indices)
      જો (axis_indices_map.get (old_index) != old_index)
	પાછા ખોટું;
    માટે (hb_codepoint_t old_index : var_indices)
      જો (varidx_map.get (old_index) != old_index)
	પાછા ખોટું;
    પાછા સાચું;
  }

  તાર્કિક append_varint (uint32_t value)
  {
    unsigned size = HBUINT32VAR::get_size (value);
    unsigned offset = record_data.length;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    જો (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      પાછા ખોટું;
    HBUINT32VAR::serialize_unsafe (record_data.arrayZ + offset, value);
    પાછા સાચું;
  }

  તાર્કિક append_bytes (hb_ubytes_t record,
		     unsigned component_size,
		     unsigned start,
		     unsigned end)
  {
    જો (unlikely (start > end || end > component_size)) પાછા ખોટું;
    unsigned offset = record_data.length;
    unsigned size = end - start;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    જો (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      પાછા ખોટું;
    hb_memcpy (record_data.arrayZ + offset, record.arrayZ + start, size);
    પાછા સાચું;
  }

  તાર્કિક compile_record (hb_ubytes_t record, તાર્કિક retain_gids)
  {
    hb_ubytes_t remaining = record;
    જ્યારે (remaining)
    {
      VarComponent::record_t component;
      જો (unlikely (!VarComponent::decompile_record (source, remaining,
						     nullptr, nullptr,
						     &component) ||
		    !compile_component (remaining, component, retain_gids)))
	પાછા ખોટું;
      remaining = remaining.sub_array (component.size);
    }
    પાછા સાચું;
  }

  તાર્કિક compile_component (hb_ubytes_t record,
			  અચલ VarComponent::record_t &component,
			  તાર્કિક retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    જો (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      પાછા ખોટું;

    uint32_t flags = component.flags;
    unsigned gid_size = component.gid_size;
    જો (new_gid > 0xFFFFu)
    {
      flags |= (unsigned) VarComponent::flags_t::GID_IS_24BIT;
      gid_size = HBGlyphID24::static_size;
    }

    જો (unlikely (!append_varint (flags))) પાછા ખોટું;
    unsigned gid_offset = record_data.length;
    unsigned gid_end = hb_unsigned_add_saturate (gid_offset, gid_size);
    જો (unlikely (gid_end == UINT_MAX ||
		  !record_data.resize_dirty (gid_end)))
      પાછા ખોટું;
    unsigned અક્ષર *out = record_data.arrayZ + gid_offset;
    જો (gid_size == HBGlyphID16::static_size)
      * (HBGlyphID16 *) out = new_gid;
    નહીંતર
      * (HBGlyphID24 *) out = new_gid;

    unsigned cursor = component.gid_offset + component.gid_size;
    જો (flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
    {
      જો (unlikely (!condition_map.has (component.condition_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.condition_offset) ||
		    !append_varint (condition_map.get (component.condition_index))))
	પાછા ખોટું;
      cursor = component.condition_offset + component.condition_size;
    }
    જો (flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
    {
      જો (unlikely (!axis_indices_map.has (component.axis_indices_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_indices_offset) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_index))))
	પાછા ખોટું;
      cursor = component.axis_indices_offset + component.axis_indices_size;
    }
    જો (flags & (unsigned) VarComponent::flags_t::AXIS_કિંમતો_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_કિંમતો_var_idx;
      જો (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_કિંમતો_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	પાછા ખોટું;
      cursor = component.axis_કિંમતો_var_offset + component.axis_કિંમતો_var_size;
    }
    જો (flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      જો (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.transform_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	પાછા ખોટું;
      cursor = component.transform_var_offset + component.transform_var_size;
    }

    પાછા append_bytes (record, component.size, cursor, component.size);
  }
};

તાર્કિક
VARC::subset (hb_subset_context_t *c) અચલ
{
  TRACE_SUBSET (આ);
  auto fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    પાછા ખોટું;
  };

  અચલ Coverage &source_coverage = આ+coverage;
  અચલ CFF2Index &source_records = આ+glyphRecords;
  અચલ ConditionList &source_conditions = આ+conditionList;
  અચલ TupleList &source_axis_indices = આ+axisIndicesList;
  અચલ CFF2Index &source_axis_indices_index = source_axis_indices;
  અચલ MultiItemVariationStore &source_var_store = આ+varStore;
  varc_subset_plan_t subset_plan (*આ, c,
				  source_coverage,
				  source_records,
				  source_conditions,
				  source_axis_indices_index,
				  source_var_store);
  જો (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  જો (!subset_plan.old_indices)
    return_trace (ખોટું);
  જો (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_records ()))
    return_trace (fail ());

  VARC *out = c->serializer->start_embed<VARC> ();
  જો (unlikely (!out || !c->serializer->extend_min (out)))
    return_trace (ખોટું);
  out->version = version;

  જો (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  જો (unlikely (!out->coverage.serialize_serialize (c->serializer,
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
    return_trace (ખોટું);

  return_trace (સાચું);
}

} /* namespace OT */

#endif /* OT_VAR_VARC_VARC_SUBSET_HH */
