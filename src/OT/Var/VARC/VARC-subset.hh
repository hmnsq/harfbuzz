#ifndef OT_VAR_VARC_VARC_SUBSET_HH
#define OT_VAR_VARC_VARC_SUBSET_HH

#अन्तर्भाव "VARC.hh"

नामाकाश OT {

संरचना varc_subset_plan_t
{
  varc_subset_plan_t (ध्रुव VARC &source_,
		      hb_subset_context_t *subset_context_,
		      ध्रुव Coverage &source_coverage_,
		      ध्रुव CFF2Index &source_records_,
		      ध्रुव ConditionList &source_conditions_,
		      ध्रुव CFF2Index &source_axis_indices_,
		      ध्रुव MultiItemVariationStore &source_var_store_) :
    source (source_),
    subset_context (subset_context_),
    source_coverage (source_coverage_),
    source_records (source_records_),
    source_conditions (source_conditions_),
    source_axis_indices (source_axis_indices_),
    source_var_store (source_var_store_)
  {}

  तार्किक collect_glyphs ()
  {
    ध्रुव hb_set_t &glyphset = subset_context->plan->_glyphset_varced;
    प्रति (auto _ : + hb_zip (source_coverage,
			    hb_range ((unsigned) source_records.गणनम्)))
    {
      hb_codepoint_t old_gid = _.first;
      unsigned index = _.second;
      जइ (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	जइ (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  प्रत्यागम मिच्छ;
	old_indices.push (index);
	new_gids.push (new_gid);
	data_size = hb_unsigned_add_saturate (data_size,
					      source_records[index].length);
      }
    }

    प्रत्यागम !old_indices.in_error () && !new_gids.in_error () &&
	   data_size != UINT_MAX;
  }

  तार्किक collect_indices ()
  {
    प्रति (unsigned old_index : old_indices)
    {
      hb_ubytes_t remaining = source_records[old_index];
      जाव (remaining)
      {
	VarComponent::record_t component;
	जइ (unlikely (!VarComponent::decompile_record (source, remaining,
						       nullptr, nullptr,
						       &component)))
	  प्रत्यागम मिच्छ;

	जइ (component.flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
	{
	  जइ (unlikely (component.condition_index >= source_conditions.get_गणनम् ()))
	    प्रत्यागम मिच्छ;
	  condition_indices.add (component.condition_index);
	}
	जइ (component.flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
	{
	  जइ (unlikely (component.axis_indices_index >= source_axis_indices.गणनम्))
	    प्रत्यागम मिच्छ;
	  axis_indices.add (component.axis_indices_index);
	}
	जइ ((component.flags & (unsigned) VarComponent::flags_t::AXIS_मानानि_HAVE_VARIATION) &&
	    component.axis_मानानि_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.axis_मानानि_var_idx);
	जइ ((component.flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.transform_var_idx);

	remaining = remaining.sub_array (component.size);
      }
    }

    जइ (unlikely (condition_indices.in_error () || axis_indices.in_error () ||
		  var_indices.in_error ()))
      प्रत्यागम मिच्छ;

    प्रति (hb_codepoint_t condition_index : condition_indices)
      जइ (unlikely (!source_conditions[condition_index].collect_var_indices (&var_indices)))
	प्रत्यागम मिच्छ;

    प्रत्यागम सच्च;
  }

  तार्किक create_maps ()
  {
    ध्रुव auto *plan = subset_context->plan;
    जइ (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_index_map;
      प्रति (auto _ : plan->axes_index_map)
	जइ (!plan->user_axes_location.has (plan->axes_old_index_tag_map.get (_.first)))
	  axes_index_map.set (_.first, _.second);
      जइ (unlikely (axes_index_map.in_error ())) प्रत्यागम मिच्छ;
    }
    condition_map.add_set (&condition_indices);
    axis_indices_map.add_set (&axis_indices);
    प्रत्यागम !condition_map.in_error () && !axis_indices_map.in_error () &&
	   source_var_store.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  तार्किक compile_records ()
  {
    जइ (unlikely (!records.alloc_exact (old_indices.length)))
      प्रत्यागम मिच्छ;

    तार्किक retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    जइ (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      प्रति (unsigned old_index : old_indices)
	records.push (source_records[old_index]);
    }
    अण्णहा
    {
      hb_vector_t<unsigned> record_offsets;
      record_data.alloc (data_size);
      जइ (unlikely (!record_offsets.alloc_exact (old_indices.length + 1)))
	प्रत्यागम मिच्छ;

      प्रति (unsigned old_index : old_indices)
      {
	record_offsets.push (record_data.length);
	जइ (unlikely (!compile_record (source_records[old_index], retain_gids)))
	  प्रत्यागम मिच्छ;
      }
      record_offsets.push (record_data.length);

      जइ (unlikely (record_data.in_error () || record_offsets.in_error ()))
	प्रत्यागम मिच्छ;
      प्रति (unsigned i = 0; i < old_indices.length; i++)
	records.push (hb_ubytes_t (record_data.arrayZ + record_offsets[i],
				  record_offsets[i + 1] - record_offsets[i]));
      data_size = record_data.length;
    }

    प्रत्यागम !records.in_error ();
  }

  तार्किक select_axis_indices ()
  {
    जइ (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_data.resize (
					  axis_indices_map.get_population ()))))
      प्रत्यागम मिच्छ;
    प्रति (unsigned i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t bytes = source_axis_indices[axis_indices_map.backward (i)];
      जइ (axis_map)
      {
	hb_vector_t<पूर्णाङ्क> indices;
	ध्रुव HBUINT8 *p = (ध्रुव HBUINT8 *) bytes.arrayZ;
	जइ (unlikely (!Tuple_मानानि::decompile (p, indices, p + bytes.length, सच्च)))
	  प्रत्यागम मिच्छ;
	प्रति (पूर्णाङ्क &index : indices)
	{
	  जइ (unlikely (!axis_map->has (index))) प्रत्यागम मिच्छ;
	  index = axis_map->get (index);
	}
	auto &data = axis_indices_data[i];
	/* One control byte and at most four data bytes per value. */
	unsigned size = hb_unsigned_mul_saturate (indices.length, 5u);
	जइ (unlikely (size == UINT_MAX || !data.resize_dirty (size))) प्रत्यागम मिच्छ;
	data.shrink (Tuple_मानानि::compile_unsafe (indices.as_array (), data.arrayZ));
	bytes = data.as_array ();
      }
      axis_indices_data_size = hb_unsigned_add_saturate (axis_indices_data_size,
							 bytes.length);
      selected_axis_indices.push (bytes);
    }

    प्रत्यागम axis_indices_data_size != UINT_MAX &&
	   !selected_axis_indices.in_error ();
  }

  ध्रुव VARC &source;
  hb_subset_context_t *subset_context;
  ध्रुव Coverage &source_coverage;
  ध्रुव CFF2Index &source_records;
  ध्रुव ConditionList &source_conditions;
  ध्रुव CFF2Index &source_axis_indices;
  ध्रुव MultiItemVariationStore &source_var_store;

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

  hb_vector_t<unsigned वर्ण> record_data;
  hb_vector_t<hb_ubytes_t> records;

  hb_vector_t<hb_ubytes_t> selected_axis_indices;
  hb_vector_t<hb_vector_t<unsigned वर्ण>> axis_indices_data;
  unsigned axis_indices_data_size = 0;
  hb_map_t axes_index_map;
  ध्रुव hb_map_t *axis_map = nullptr;

  गुप्त:
  तार्किक auxiliary_indices_unchanged () ध्रुव
  {
    प्रति (hb_codepoint_t old_index : condition_indices)
      जइ (condition_map.get (old_index) != old_index)
	प्रत्यागम मिच्छ;
    प्रति (hb_codepoint_t old_index : axis_indices)
      जइ (axis_indices_map.get (old_index) != old_index)
	प्रत्यागम मिच्छ;
    प्रति (hb_codepoint_t old_index : var_indices)
      जइ (varidx_map.get (old_index) != old_index)
	प्रत्यागम मिच्छ;
    प्रत्यागम सच्च;
  }

  तार्किक append_varint (uint32_t value)
  {
    unsigned size = HBUINT32VAR::get_size (value);
    unsigned offset = record_data.length;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    जइ (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      प्रत्यागम मिच्छ;
    HBUINT32VAR::serialize_unsafe (record_data.arrayZ + offset, value);
    प्रत्यागम सच्च;
  }

  तार्किक append_bytes (hb_ubytes_t record,
		     unsigned component_size,
		     unsigned start,
		     unsigned end)
  {
    जइ (unlikely (start > end || end > component_size)) प्रत्यागम मिच्छ;
    unsigned offset = record_data.length;
    unsigned size = end - start;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    जइ (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      प्रत्यागम मिच्छ;
    hb_memcpy (record_data.arrayZ + offset, record.arrayZ + start, size);
    प्रत्यागम सच्च;
  }

  तार्किक compile_record (hb_ubytes_t record, तार्किक retain_gids)
  {
    hb_ubytes_t remaining = record;
    जाव (remaining)
    {
      VarComponent::record_t component;
      जइ (unlikely (!VarComponent::decompile_record (source, remaining,
						     nullptr, nullptr,
						     &component) ||
		    !compile_component (remaining, component, retain_gids)))
	प्रत्यागम मिच्छ;
      remaining = remaining.sub_array (component.size);
    }
    प्रत्यागम सच्च;
  }

  तार्किक compile_component (hb_ubytes_t record,
			  ध्रुव VarComponent::record_t &component,
			  तार्किक retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    जइ (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      प्रत्यागम मिच्छ;

    uint32_t flags = component.flags;
    unsigned gid_size = component.gid_size;
    जइ (new_gid > 0xFFFFu)
    {
      flags |= (unsigned) VarComponent::flags_t::GID_IS_24BIT;
      gid_size = HBGlyphID24::static_size;
    }

    जइ (unlikely (!append_varint (flags))) प्रत्यागम मिच्छ;
    unsigned gid_offset = record_data.length;
    unsigned gid_end = hb_unsigned_add_saturate (gid_offset, gid_size);
    जइ (unlikely (gid_end == UINT_MAX ||
		  !record_data.resize_dirty (gid_end)))
      प्रत्यागम मिच्छ;
    unsigned वर्ण *out = record_data.arrayZ + gid_offset;
    जइ (gid_size == HBGlyphID16::static_size)
      * (HBGlyphID16 *) out = new_gid;
    अण्णहा
      * (HBGlyphID24 *) out = new_gid;

    unsigned cursor = component.gid_offset + component.gid_size;
    जइ (flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
    {
      जइ (unlikely (!condition_map.has (component.condition_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.condition_offset) ||
		    !append_varint (condition_map.get (component.condition_index))))
	प्रत्यागम मिच्छ;
      cursor = component.condition_offset + component.condition_size;
    }
    जइ (flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
    {
      जइ (unlikely (!axis_indices_map.has (component.axis_indices_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_indices_offset) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_index))))
	प्रत्यागम मिच्छ;
      cursor = component.axis_indices_offset + component.axis_indices_size;
    }
    जइ (flags & (unsigned) VarComponent::flags_t::AXIS_मानानि_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_मानानि_var_idx;
      जइ (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_मानानि_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	प्रत्यागम मिच्छ;
      cursor = component.axis_मानानि_var_offset + component.axis_मानानि_var_size;
    }
    जइ (flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      जइ (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.transform_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	प्रत्यागम मिच्छ;
      cursor = component.transform_var_offset + component.transform_var_size;
    }

    प्रत्यागम append_bytes (record, component.size, cursor, component.size);
  }
};

तार्किक
VARC::subset (hb_subset_context_t *c) ध्रुव
{
  TRACE_SUBSET (एतत्);
  auto fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    प्रत्यागम मिच्छ;
  };

  ध्रुव Coverage &source_coverage = एतत्+coverage;
  ध्रुव CFF2Index &source_records = एतत्+glyphRecords;
  ध्रुव ConditionList &source_conditions = एतत्+conditionList;
  ध्रुव TupleList &source_axis_indices = एतत्+axisIndicesList;
  ध्रुव CFF2Index &source_axis_indices_index = source_axis_indices;
  ध्रुव MultiItemVariationStore &source_var_store = एतत्+varStore;
  varc_subset_plan_t subset_plan (*एतत्, c,
				  source_coverage,
				  source_records,
				  source_conditions,
				  source_axis_indices_index,
				  source_var_store);
  जइ (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  जइ (!subset_plan.old_indices)
    return_trace (मिच्छ);
  जइ (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_records ()))
    return_trace (fail ());

  VARC *out = c->serializer->start_embed<VARC> ();
  जइ (unlikely (!out || !c->serializer->extend_min (out)))
    return_trace (मिच्छ);
  out->version = version;

  जइ (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  जइ (unlikely (!out->coverage.serialize_serialize (c->serializer,
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
    return_trace (मिच्छ);

  return_trace (सच्च);
}

} /* namespace OT */

#endif /* OT_VAR_VARC_VARC_SUBSET_HH */
