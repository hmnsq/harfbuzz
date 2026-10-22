#ifndef OT_VAR_VARC_VARC_SUBSET_HH
#define OT_VAR_VARC_VARC_SUBSET_HH

#ಅನ್ತರ್ಭಾವ "VARC.hh"

ನಾಮಾಕಾಶ OT {

ಸಂರಚನಾ varc_subset_plan_t
{
  varc_subset_plan_t (ಧ್ರುವ VARC &source_,
		      hb_subset_context_t *subset_context_,
		      ಧ್ರುವ Coverage &source_coverage_,
		      ಧ್ರುವ CFF2Index &source_records_,
		      ಧ್ರುವ ConditionList &source_conditions_,
		      ಧ್ರುವ CFF2Index &source_axis_indices_,
		      ಧ್ರುವ MultiItemVariationStore &source_var_store_) :
    source (source_),
    subset_context (subset_context_),
    source_coverage (source_coverage_),
    source_records (source_records_),
    source_conditions (source_conditions_),
    source_axis_indices (source_axis_indices_),
    source_var_store (source_var_store_)
  {}

  ತಾರ್ಕಿಕ collect_glyphs ()
  {
    ಧ್ರುವ hb_set_t &glyphset = subset_context->plan->_glyphset_varced;
    ಪ್ರತಿ (auto _ : + hb_zip (source_coverage,
			    hb_range ((unsigned) source_records.ಎಣಿಕೆ)))
    {
      hb_codepoint_t old_gid = _.first;
      unsigned index = _.second;
      ಯದಿ (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	ಯದಿ (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	old_indices.push (index);
	new_gids.push (new_gid);
	data_size = hb_unsigned_add_saturate (data_size,
					      source_records[index].length);
      }
    }

    ಪ್ರತ್ಯಾಗಮ !old_indices.in_error () && !new_gids.in_error () &&
	   data_size != UINT_MAX;
  }

  ತಾರ್ಕಿಕ collect_indices ()
  {
    ಪ್ರತಿ (unsigned old_index : old_indices)
    {
      hb_ubytes_t remaining = source_records[old_index];
      ಯಾವತ್ (remaining)
      {
	VarComponent::record_t component;
	ಯದಿ (unlikely (!VarComponent::decompile_record (source, remaining,
						       nullptr, nullptr,
						       &component)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	ಯದಿ (component.flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
	{
	  ಯದಿ (unlikely (component.condition_index >= source_conditions.get_ಎಣಿಕೆ ()))
	    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	  condition_indices.add (component.condition_index);
	}
	ಯದಿ (component.flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
	{
	  ಯದಿ (unlikely (component.axis_indices_index >= source_axis_indices.ಎಣಿಕೆ))
	    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	  axis_indices.add (component.axis_indices_index);
	}
	ಯದಿ ((component.flags & (unsigned) VarComponent::flags_t::AXIS_ಮೌಲ್ಯಗಳು_HAVE_VARIATION) &&
	    component.axis_ಮೌಲ್ಯಗಳು_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.axis_ಮೌಲ್ಯಗಳು_var_idx);
	ಯದಿ ((component.flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.transform_var_idx);

	remaining = remaining.sub_array (component.size);
      }
    }

    ಯದಿ (unlikely (condition_indices.in_error () || axis_indices.in_error () ||
		  var_indices.in_error ()))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (hb_codepoint_t condition_index : condition_indices)
      ಯದಿ (unlikely (!source_conditions[condition_index].collect_var_indices (&var_indices)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ create_maps ()
  {
    ಧ್ರುವ auto *plan = subset_context->plan;
    ಯದಿ (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_index_map;
      ಪ್ರತಿ (auto _ : plan->axes_index_map)
	ಯದಿ (!plan->user_axes_location.has (plan->axes_old_index_tag_map.get (_.first)))
	  axes_index_map.set (_.first, _.second);
      ಯದಿ (unlikely (axes_index_map.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    condition_map.add_set (&condition_indices);
    axis_indices_map.add_set (&axis_indices);
    ಪ್ರತ್ಯಾಗಮ !condition_map.in_error () && !axis_indices_map.in_error () &&
	   source_var_store.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  ತಾರ್ಕಿಕ compile_records ()
  {
    ಯದಿ (unlikely (!records.alloc_exact (old_indices.length)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ತಾರ್ಕಿಕ retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    ಯದಿ (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      ಪ್ರತಿ (unsigned old_index : old_indices)
	records.push (source_records[old_index]);
    }
    ಅನ್ಯಥಾ
    {
      hb_vector_t<unsigned> record_offsets;
      record_data.alloc (data_size);
      ಯದಿ (unlikely (!record_offsets.alloc_exact (old_indices.length + 1)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಪ್ರತಿ (unsigned old_index : old_indices)
      {
	record_offsets.push (record_data.length);
	ಯದಿ (unlikely (!compile_record (source_records[old_index], retain_gids)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      record_offsets.push (record_data.length);

      ಯದಿ (unlikely (record_data.in_error () || record_offsets.in_error ()))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತಿ (unsigned i = 0; i < old_indices.length; i++)
	records.push (hb_ubytes_t (record_data.arrayZ + record_offsets[i],
				  record_offsets[i + 1] - record_offsets[i]));
      data_size = record_data.length;
    }

    ಪ್ರತ್ಯಾಗಮ !records.in_error ();
  }

  ತಾರ್ಕಿಕ select_axis_indices ()
  {
    ಯದಿ (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_data.resize (
					  axis_indices_map.get_population ()))))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t bytes = source_axis_indices[axis_indices_map.backward (i)];
      ಯದಿ (axis_map)
      {
	hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> indices;
	ಧ್ರುವ HBUINT8 *p = (ಧ್ರುವ HBUINT8 *) bytes.arrayZ;
	ಯದಿ (unlikely (!Tuple_ಮೌಲ್ಯಗಳು::decompile (p, indices, p + bytes.length, ಸತ್ಯ)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ &index : indices)
	{
	  ಯದಿ (unlikely (!axis_map->has (index))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	  index = axis_map->get (index);
	}
	auto &data = axis_indices_data[i];
	/* One control byte and at most four data bytes per value. */
	unsigned size = hb_unsigned_mul_saturate (indices.length, 5u);
	ಯದಿ (unlikely (size == UINT_MAX || !data.resize_dirty (size))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	data.shrink (Tuple_ಮೌಲ್ಯಗಳು::compile_unsafe (indices.as_array (), data.arrayZ));
	bytes = data.as_array ();
      }
      axis_indices_data_size = hb_unsigned_add_saturate (axis_indices_data_size,
							 bytes.length);
      selected_axis_indices.push (bytes);
    }

    ಪ್ರತ್ಯಾಗಮ axis_indices_data_size != UINT_MAX &&
	   !selected_axis_indices.in_error ();
  }

  ಧ್ರುವ VARC &source;
  hb_subset_context_t *subset_context;
  ಧ್ರುವ Coverage &source_coverage;
  ಧ್ರುವ CFF2Index &source_records;
  ಧ್ರುವ ConditionList &source_conditions;
  ಧ್ರುವ CFF2Index &source_axis_indices;
  ಧ್ರುವ MultiItemVariationStore &source_var_store;

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

  hb_vector_t<unsigned ವರ್ಣ> record_data;
  hb_vector_t<hb_ubytes_t> records;

  hb_vector_t<hb_ubytes_t> selected_axis_indices;
  hb_vector_t<hb_vector_t<unsigned ವರ್ಣ>> axis_indices_data;
  unsigned axis_indices_data_size = 0;
  hb_map_t axes_index_map;
  ಧ್ರುವ hb_map_t *axis_map = nullptr;

  ಗುಪ್ತ:
  ತಾರ್ಕಿಕ auxiliary_indices_unchanged () ಧ್ರುವ
  {
    ಪ್ರತಿ (hb_codepoint_t old_index : condition_indices)
      ಯದಿ (condition_map.get (old_index) != old_index)
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (hb_codepoint_t old_index : axis_indices)
      ಯದಿ (axis_indices_map.get (old_index) != old_index)
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (hb_codepoint_t old_index : var_indices)
      ಯದಿ (varidx_map.get (old_index) != old_index)
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ append_varint (uint32_t value)
  {
    unsigned size = HBUINT32VAR::get_size (value);
    unsigned offset = record_data.length;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    ಯದಿ (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    HBUINT32VAR::serialize_unsafe (record_data.arrayZ + offset, value);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ append_bytes (hb_ubytes_t record,
		     unsigned component_size,
		     unsigned start,
		     unsigned end)
  {
    ಯದಿ (unlikely (start > end || end > component_size)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned offset = record_data.length;
    unsigned size = end - start;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    ಯದಿ (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_memcpy (record_data.arrayZ + offset, record.arrayZ + start, size);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ compile_record (hb_ubytes_t record, ತಾರ್ಕಿಕ retain_gids)
  {
    hb_ubytes_t remaining = record;
    ಯಾವತ್ (remaining)
    {
      VarComponent::record_t component;
      ಯದಿ (unlikely (!VarComponent::decompile_record (source, remaining,
						     nullptr, nullptr,
						     &component) ||
		    !compile_component (remaining, component, retain_gids)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      remaining = remaining.sub_array (component.size);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ compile_component (hb_ubytes_t record,
			  ಧ್ರುವ VarComponent::record_t &component,
			  ತಾರ್ಕಿಕ retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    ಯದಿ (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    uint32_t flags = component.flags;
    unsigned gid_size = component.gid_size;
    ಯದಿ (new_gid > 0xFFFFu)
    {
      flags |= (unsigned) VarComponent::flags_t::GID_IS_24BIT;
      gid_size = HBGlyphID24::static_size;
    }

    ಯದಿ (unlikely (!append_varint (flags))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned gid_offset = record_data.length;
    unsigned gid_end = hb_unsigned_add_saturate (gid_offset, gid_size);
    ಯದಿ (unlikely (gid_end == UINT_MAX ||
		  !record_data.resize_dirty (gid_end)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned ವರ್ಣ *out = record_data.arrayZ + gid_offset;
    ಯದಿ (gid_size == HBGlyphID16::static_size)
      * (HBGlyphID16 *) out = new_gid;
    ಅನ್ಯಥಾ
      * (HBGlyphID24 *) out = new_gid;

    unsigned cursor = component.gid_offset + component.gid_size;
    ಯದಿ (flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
    {
      ಯದಿ (unlikely (!condition_map.has (component.condition_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.condition_offset) ||
		    !append_varint (condition_map.get (component.condition_index))))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      cursor = component.condition_offset + component.condition_size;
    }
    ಯದಿ (flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
    {
      ಯದಿ (unlikely (!axis_indices_map.has (component.axis_indices_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_indices_offset) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_index))))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      cursor = component.axis_indices_offset + component.axis_indices_size;
    }
    ಯದಿ (flags & (unsigned) VarComponent::flags_t::AXIS_ಮೌಲ್ಯಗಳು_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_ಮೌಲ್ಯಗಳು_var_idx;
      ಯದಿ (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_ಮೌಲ್ಯಗಳು_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      cursor = component.axis_ಮೌಲ್ಯಗಳು_var_offset + component.axis_ಮೌಲ್ಯಗಳು_var_size;
    }
    ಯದಿ (flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      ಯದಿ (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.transform_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      cursor = component.transform_var_offset + component.transform_var_size;
    }

    ಪ್ರತ್ಯಾಗಮ append_bytes (record, component.size, cursor, component.size);
  }
};

ತಾರ್ಕಿಕ
VARC::subset (hb_subset_context_t *c) ಧ್ರುವ
{
  TRACE_SUBSET (ಏತತ್);
  auto fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  };

  ಧ್ರುವ Coverage &source_coverage = ಏತತ್+coverage;
  ಧ್ರುವ CFF2Index &source_records = ಏತತ್+glyphRecords;
  ಧ್ರುವ ConditionList &source_conditions = ಏತತ್+conditionList;
  ಧ್ರುವ TupleList &source_axis_indices = ಏತತ್+axisIndicesList;
  ಧ್ರುವ CFF2Index &source_axis_indices_index = source_axis_indices;
  ಧ್ರುವ MultiItemVariationStore &source_var_store = ಏತತ್+varStore;
  varc_subset_plan_t subset_plan (*ಏತತ್, c,
				  source_coverage,
				  source_records,
				  source_conditions,
				  source_axis_indices_index,
				  source_var_store);
  ಯದಿ (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  ಯದಿ (!subset_plan.old_indices)
    return_trace (ಅಸತ್ಯ);
  ಯದಿ (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_records ()))
    return_trace (fail ());

  VARC *out = c->serializer->start_embed<VARC> ();
  ಯದಿ (unlikely (!out || !c->serializer->extend_min (out)))
    return_trace (ಅಸತ್ಯ);
  out->version = version;

  ಯದಿ (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  ಯದಿ (unlikely (!out->coverage.serialize_serialize (c->serializer,
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
    return_trace (ಅಸತ್ಯ);

  return_trace (ಸತ್ಯ);
}

} /* namespace OT */

#endif /* OT_VAR_VARC_VARC_SUBSET_HH */
