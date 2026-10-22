#ifndef OT_VAR_VARC_VARC_SUBSET_HH
#define OT_VAR_VARC_VARC_SUBSET_HH

#অন্তর্ভাব "VARC.hh"

নামাকাশ OT {

সংরচনা varc_subset_plan_t
{
  varc_subset_plan_t (ধ্রুব VARC &source_,
		      hb_subset_context_t *subset_context_,
		      ধ্রুব Coverage &source_coverage_,
		      ধ্রুব CFF2Index &source_records_,
		      ধ্রুব ConditionList &source_conditions_,
		      ধ্রুব CFF2Index &source_axis_indices_,
		      ধ্রুব MultiItemVariationStore &source_var_store_) :
    source (source_),
    subset_context (subset_context_),
    source_coverage (source_coverage_),
    source_records (source_records_),
    source_conditions (source_conditions_),
    source_axis_indices (source_axis_indices_),
    source_var_store (source_var_store_)
  {}

  তার্কিক collect_glyphs ()
  {
    ধ্রুব hb_set_t &glyphset = subset_context->plan->_glyphset_varced;
    প্রতি (auto _ : + hb_zip (source_coverage,
			    hb_range ((unsigned) source_records.গুনতি)))
    {
      hb_codepoint_t old_gid = _.first;
      unsigned index = _.second;
      যদি (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	যদি (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  প্রত্যাগম অসত্য;
	old_indices.push (index);
	new_gids.push (new_gid);
	data_size = hb_unsigned_add_saturate (data_size,
					      source_records[index].length);
      }
    }

    প্রত্যাগম !old_indices.in_error () && !new_gids.in_error () &&
	   data_size != UINT_MAX;
  }

  তার্কিক collect_indices ()
  {
    প্রতি (unsigned old_index : old_indices)
    {
      hb_ubytes_t remaining = source_records[old_index];
      যাবত্ (remaining)
      {
	VarComponent::record_t component;
	যদি (unlikely (!VarComponent::decompile_record (source, remaining,
						       nullptr, nullptr,
						       &component)))
	  প্রত্যাগম অসত্য;

	যদি (component.flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
	{
	  যদি (unlikely (component.condition_index >= source_conditions.get_গুনতি ()))
	    প্রত্যাগম অসত্য;
	  condition_indices.add (component.condition_index);
	}
	যদি (component.flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
	{
	  যদি (unlikely (component.axis_indices_index >= source_axis_indices.গুনতি))
	    প্রত্যাগম অসত্য;
	  axis_indices.add (component.axis_indices_index);
	}
	যদি ((component.flags & (unsigned) VarComponent::flags_t::AXIS_মানসমূহ_HAVE_VARIATION) &&
	    component.axis_মানসমূহ_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.axis_মানসমূহ_var_idx);
	যদি ((component.flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.transform_var_idx);

	remaining = remaining.sub_array (component.size);
      }
    }

    যদি (unlikely (condition_indices.in_error () || axis_indices.in_error () ||
		  var_indices.in_error ()))
      প্রত্যাগম অসত্য;

    প্রতি (hb_codepoint_t condition_index : condition_indices)
      যদি (unlikely (!source_conditions[condition_index].collect_var_indices (&var_indices)))
	প্রত্যাগম অসত্য;

    প্রত্যাগম সত্য;
  }

  তার্কিক create_maps ()
  {
    ধ্রুব auto *plan = subset_context->plan;
    যদি (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_index_map;
      প্রতি (auto _ : plan->axes_index_map)
	যদি (!plan->user_axes_location.has (plan->axes_old_index_tag_map.get (_.first)))
	  axes_index_map.set (_.first, _.second);
      যদি (unlikely (axes_index_map.in_error ())) প্রত্যাগম অসত্য;
    }
    condition_map.add_set (&condition_indices);
    axis_indices_map.add_set (&axis_indices);
    প্রত্যাগম !condition_map.in_error () && !axis_indices_map.in_error () &&
	   source_var_store.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  তার্কিক compile_records ()
  {
    যদি (unlikely (!records.alloc_exact (old_indices.length)))
      প্রত্যাগম অসত্য;

    তার্কিক retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    যদি (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      প্রতি (unsigned old_index : old_indices)
	records.push (source_records[old_index]);
    }
    অন্যথা
    {
      hb_vector_t<unsigned> record_offsets;
      record_data.alloc (data_size);
      যদি (unlikely (!record_offsets.alloc_exact (old_indices.length + 1)))
	প্রত্যাগম অসত্য;

      প্রতি (unsigned old_index : old_indices)
      {
	record_offsets.push (record_data.length);
	যদি (unlikely (!compile_record (source_records[old_index], retain_gids)))
	  প্রত্যাগম অসত্য;
      }
      record_offsets.push (record_data.length);

      যদি (unlikely (record_data.in_error () || record_offsets.in_error ()))
	প্রত্যাগম অসত্য;
      প্রতি (unsigned i = 0; i < old_indices.length; i++)
	records.push (hb_ubytes_t (record_data.arrayZ + record_offsets[i],
				  record_offsets[i + 1] - record_offsets[i]));
      data_size = record_data.length;
    }

    প্রত্যাগম !records.in_error ();
  }

  তার্কিক select_axis_indices ()
  {
    যদি (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_data.resize (
					  axis_indices_map.get_population ()))))
      প্রত্যাগম অসত্য;
    প্রতি (unsigned i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t bytes = source_axis_indices[axis_indices_map.backward (i)];
      যদি (axis_map)
      {
	hb_vector_t<পূর্ণাঙ্ক> indices;
	ধ্রুব HBUINT8 *p = (ধ্রুব HBUINT8 *) bytes.arrayZ;
	যদি (unlikely (!Tuple_মানসমূহ::decompile (p, indices, p + bytes.length, সত্য)))
	  প্রত্যাগম অসত্য;
	প্রতি (পূর্ণাঙ্ক &index : indices)
	{
	  যদি (unlikely (!axis_map->has (index))) প্রত্যাগম অসত্য;
	  index = axis_map->get (index);
	}
	auto &data = axis_indices_data[i];
	/* One control byte and at most four data bytes per value. */
	unsigned size = hb_unsigned_mul_saturate (indices.length, 5u);
	যদি (unlikely (size == UINT_MAX || !data.resize_dirty (size))) প্রত্যাগম অসত্য;
	data.shrink (Tuple_মানসমূহ::compile_unsafe (indices.as_array (), data.arrayZ));
	bytes = data.as_array ();
      }
      axis_indices_data_size = hb_unsigned_add_saturate (axis_indices_data_size,
							 bytes.length);
      selected_axis_indices.push (bytes);
    }

    প্রত্যাগম axis_indices_data_size != UINT_MAX &&
	   !selected_axis_indices.in_error ();
  }

  ধ্রুব VARC &source;
  hb_subset_context_t *subset_context;
  ধ্রুব Coverage &source_coverage;
  ধ্রুব CFF2Index &source_records;
  ধ্রুব ConditionList &source_conditions;
  ধ্রুব CFF2Index &source_axis_indices;
  ধ্রুব MultiItemVariationStore &source_var_store;

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

  hb_vector_t<unsigned বর্ণ> record_data;
  hb_vector_t<hb_ubytes_t> records;

  hb_vector_t<hb_ubytes_t> selected_axis_indices;
  hb_vector_t<hb_vector_t<unsigned বর্ণ>> axis_indices_data;
  unsigned axis_indices_data_size = 0;
  hb_map_t axes_index_map;
  ধ্রুব hb_map_t *axis_map = nullptr;

  গুপ্ত:
  তার্কিক auxiliary_indices_unchanged () ধ্রুব
  {
    প্রতি (hb_codepoint_t old_index : condition_indices)
      যদি (condition_map.get (old_index) != old_index)
	প্রত্যাগম অসত্য;
    প্রতি (hb_codepoint_t old_index : axis_indices)
      যদি (axis_indices_map.get (old_index) != old_index)
	প্রত্যাগম অসত্য;
    প্রতি (hb_codepoint_t old_index : var_indices)
      যদি (varidx_map.get (old_index) != old_index)
	প্রত্যাগম অসত্য;
    প্রত্যাগম সত্য;
  }

  তার্কিক append_varint (uint32_t value)
  {
    unsigned size = HBUINT32VAR::get_size (value);
    unsigned offset = record_data.length;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    যদি (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      প্রত্যাগম অসত্য;
    HBUINT32VAR::serialize_unsafe (record_data.arrayZ + offset, value);
    প্রত্যাগম সত্য;
  }

  তার্কিক append_bytes (hb_ubytes_t record,
		     unsigned component_size,
		     unsigned start,
		     unsigned end)
  {
    যদি (unlikely (start > end || end > component_size)) প্রত্যাগম অসত্য;
    unsigned offset = record_data.length;
    unsigned size = end - start;
    unsigned new_length = hb_unsigned_add_saturate (offset, size);
    যদি (unlikely (new_length == UINT_MAX ||
		  !record_data.resize_dirty (new_length)))
      প্রত্যাগম অসত্য;
    hb_memcpy (record_data.arrayZ + offset, record.arrayZ + start, size);
    প্রত্যাগম সত্য;
  }

  তার্কিক compile_record (hb_ubytes_t record, তার্কিক retain_gids)
  {
    hb_ubytes_t remaining = record;
    যাবত্ (remaining)
    {
      VarComponent::record_t component;
      যদি (unlikely (!VarComponent::decompile_record (source, remaining,
						     nullptr, nullptr,
						     &component) ||
		    !compile_component (remaining, component, retain_gids)))
	প্রত্যাগম অসত্য;
      remaining = remaining.sub_array (component.size);
    }
    প্রত্যাগম সত্য;
  }

  তার্কিক compile_component (hb_ubytes_t record,
			  ধ্রুব VarComponent::record_t &component,
			  তার্কিক retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    যদি (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      প্রত্যাগম অসত্য;

    uint32_t flags = component.flags;
    unsigned gid_size = component.gid_size;
    যদি (new_gid > 0xFFFFu)
    {
      flags |= (unsigned) VarComponent::flags_t::GID_IS_24BIT;
      gid_size = HBGlyphID24::static_size;
    }

    যদি (unlikely (!append_varint (flags))) প্রত্যাগম অসত্য;
    unsigned gid_offset = record_data.length;
    unsigned gid_end = hb_unsigned_add_saturate (gid_offset, gid_size);
    যদি (unlikely (gid_end == UINT_MAX ||
		  !record_data.resize_dirty (gid_end)))
      প্রত্যাগম অসত্য;
    unsigned বর্ণ *out = record_data.arrayZ + gid_offset;
    যদি (gid_size == HBGlyphID16::static_size)
      * (HBGlyphID16 *) out = new_gid;
    অন্যথা
      * (HBGlyphID24 *) out = new_gid;

    unsigned cursor = component.gid_offset + component.gid_size;
    যদি (flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
    {
      যদি (unlikely (!condition_map.has (component.condition_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.condition_offset) ||
		    !append_varint (condition_map.get (component.condition_index))))
	প্রত্যাগম অসত্য;
      cursor = component.condition_offset + component.condition_size;
    }
    যদি (flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
    {
      যদি (unlikely (!axis_indices_map.has (component.axis_indices_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_indices_offset) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_index))))
	প্রত্যাগম অসত্য;
      cursor = component.axis_indices_offset + component.axis_indices_size;
    }
    যদি (flags & (unsigned) VarComponent::flags_t::AXIS_মানসমূহ_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_মানসমূহ_var_idx;
      যদি (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_মানসমূহ_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	প্রত্যাগম অসত্য;
      cursor = component.axis_মানসমূহ_var_offset + component.axis_মানসমূহ_var_size;
    }
    যদি (flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      যদি (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.transform_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	প্রত্যাগম অসত্য;
      cursor = component.transform_var_offset + component.transform_var_size;
    }

    প্রত্যাগম append_bytes (record, component.size, cursor, component.size);
  }
};

তার্কিক
VARC::subset (hb_subset_context_t *c) ধ্রুব
{
  TRACE_SUBSET (এতত্);
  auto fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    প্রত্যাগম অসত্য;
  };

  ধ্রুব Coverage &source_coverage = এতত্+coverage;
  ধ্রুব CFF2Index &source_records = এতত্+glyphRecords;
  ধ্রুব ConditionList &source_conditions = এতত্+conditionList;
  ধ্রুব TupleList &source_axis_indices = এতত্+axisIndicesList;
  ধ্রুব CFF2Index &source_axis_indices_index = source_axis_indices;
  ধ্রুব MultiItemVariationStore &source_var_store = এতত্+varStore;
  varc_subset_plan_t subset_plan (*এতত্, c,
				  source_coverage,
				  source_records,
				  source_conditions,
				  source_axis_indices_index,
				  source_var_store);
  যদি (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  যদি (!subset_plan.old_indices)
    return_trace (অসত্য);
  যদি (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_records ()))
    return_trace (fail ());

  VARC *out = c->serializer->start_embed<VARC> ();
  যদি (unlikely (!out || !c->serializer->extend_min (out)))
    return_trace (অসত্য);
  out->version = version;

  যদি (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  যদি (unlikely (!out->coverage.serialize_serialize (c->serializer,
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
    return_trace (অসত্য);

  return_trace (সত্য);
}

} /* namespace OT */

#endif /* OT_VAR_VARC_VARC_SUBSET_HH */
