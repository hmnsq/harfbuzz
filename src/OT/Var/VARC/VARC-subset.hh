#ifndef OT_VAR_VARC_VARC_SUBSET_HH
#define OT_VAR_VARC_VARC_SUBSET_HH

#अन्तर्भाव "VARC.hh"

नामाकाश OT {

संरचना varc_subset_plan_t
{
  varc_subset_plan_t (अचर VARC &source_,
		      hb_subset_context_t *subset_context_,
		      अचर Coverage &source_coverage_,
		      अचर CFF2Index &source_records_,
		      अचर ConditionList &source_conditions_,
		      अचर CFF2Index &source_axis_indices_,
		      अचर MultiItemVariationStore &source_var_store_) :
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
    अचर hb_set_t &glyphset = subset_context->plan->_glyphset_varced;
    लागि (स्वतः _ : + hb_zip (source_coverage,
			    hb_range ((अचिन्हित) source_records.गन्ती)))
    {
      hb_codepoint_t old_gid = _.first;
      अचिन्हित index = _.second;
      यदि (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	यदि (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  फर्काऊ असत्य;
	old_indices.push (index);
	new_gids.push (new_gid);
	data_size = hb_unsigned_add_saturate (data_size,
					      source_records[index].length);
      }
    }

    फर्काऊ !old_indices.in_error () && !new_gids.in_error () &&
	   data_size != अच_पूर्णांक_उच्च;
  }

  तार्किक collect_indices ()
  {
    लागि (अचिन्हित old_index : old_indices)
    {
      hb_ubytes_t remaining = source_records[old_index];
      जबसम्म (remaining)
      {
	VarComponent::record_t component;
	यदि (unlikely (!VarComponent::decompile_record (source, remaining,
						       nullptr, nullptr,
						       &component)))
	  फर्काऊ असत्य;

	यदि (component.flags & (अचिन्हित) VarComponent::flags_t::HAVE_CONDITION)
	{
	  यदि (unlikely (component.condition_index >= source_conditions.get_गन्ती ()))
	    फर्काऊ असत्य;
	  condition_indices.add (component.condition_index);
	}
	यदि (component.flags & (अचिन्हित) VarComponent::flags_t::HAVE_AXES)
	{
	  यदि (unlikely (component.axis_indices_index >= source_axis_indices.गन्ती))
	    फर्काऊ असत्य;
	  axis_indices.add (component.axis_indices_index);
	}
	यदि ((component.flags & (अचिन्हित) VarComponent::flags_t::AXIS_मानहरू_HAVE_VARIATION) &&
	    component.axis_मानहरू_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.axis_मानहरू_var_idx);
	यदि ((component.flags & (अचिन्हित) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.transform_var_idx);

	remaining = remaining.sub_array (component.size);
      }
    }

    यदि (unlikely (condition_indices.in_error () || axis_indices.in_error () ||
		  var_indices.in_error ()))
      फर्काऊ असत्य;

    लागि (hb_codepoint_t condition_index : condition_indices)
      यदि (unlikely (!source_conditions[condition_index].collect_var_indices (&var_indices)))
	फर्काऊ असत्य;

    फर्काऊ सत्य;
  }

  तार्किक create_maps ()
  {
    अचर स्वतः *plan = subset_context->plan;
    यदि (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_index_map;
      लागि (स्वतः _ : plan->axes_index_map)
	यदि (!plan->user_axes_location.has (plan->axes_old_index_tag_map.get (_.first)))
	  axes_index_map.set (_.first, _.second);
      यदि (unlikely (axes_index_map.in_error ())) फर्काऊ असत्य;
    }
    condition_map.add_set (&condition_indices);
    axis_indices_map.add_set (&axis_indices);
    फर्काऊ !condition_map.in_error () && !axis_indices_map.in_error () &&
	   source_var_store.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  तार्किक compile_records ()
  {
    यदि (unlikely (!records.alloc_exact (old_indices.length)))
      फर्काऊ असत्य;

    तार्किक retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    यदि (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      लागि (अचिन्हित old_index : old_indices)
	records.push (source_records[old_index]);
    }
    अन्यथा
    {
      hb_vector_t<अचिन्हित> record_offsets;
      record_data.alloc (data_size);
      यदि (unlikely (!record_offsets.alloc_exact (old_indices.length + 1)))
	फर्काऊ असत्य;

      लागि (अचिन्हित old_index : old_indices)
      {
	record_offsets.push (record_data.length);
	यदि (unlikely (!compile_record (source_records[old_index], retain_gids)))
	  फर्काऊ असत्य;
      }
      record_offsets.push (record_data.length);

      यदि (unlikely (record_data.in_error () || record_offsets.in_error ()))
	फर्काऊ असत्य;
      लागि (अचिन्हित i = 0; i < old_indices.length; i++)
	records.push (hb_ubytes_t (record_data.arrayZ + record_offsets[i],
				  record_offsets[i + 1] - record_offsets[i]));
      data_size = record_data.length;
    }

    फर्काऊ !records.in_error ();
  }

  तार्किक select_axis_indices ()
  {
    यदि (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_data.resize (
					  axis_indices_map.get_population ()))))
      फर्काऊ असत्य;
    लागि (अचिन्हित i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t bytes = source_axis_indices[axis_indices_map.backward (i)];
      यदि (axis_map)
      {
	hb_vector_t<पूर्णाङ्क> indices;
	अचर HBUINT8 *p = (अचर HBUINT8 *) bytes.arrayZ;
	यदि (unlikely (!Tuple_मानहरू::decompile (p, indices, p + bytes.length, सत्य)))
	  फर्काऊ असत्य;
	लागि (पूर्णाङ्क &index : indices)
	{
	  यदि (unlikely (!axis_map->has (index))) फर्काऊ असत्य;
	  index = axis_map->get (index);
	}
	स्वतः &data = axis_indices_data[i];
	/* One control byte and at most four data bytes per value. */
	अचिन्हित size = hb_unsigned_mul_saturate (indices.length, 5u);
	यदि (unlikely (size == अच_पूर्णांक_उच्च || !data.resize_dirty (size))) फर्काऊ असत्य;
	data.shrink (Tuple_मानहरू::compile_unsafe (indices.as_array (), data.arrayZ));
	bytes = data.as_array ();
      }
      axis_indices_data_size = hb_unsigned_add_saturate (axis_indices_data_size,
							 bytes.length);
      selected_axis_indices.push (bytes);
    }

    फर्काऊ axis_indices_data_size != अच_पूर्णांक_उच्च &&
	   !selected_axis_indices.in_error ();
  }

  अचर VARC &source;
  hb_subset_context_t *subset_context;
  अचर Coverage &source_coverage;
  अचर CFF2Index &source_records;
  अचर ConditionList &source_conditions;
  अचर CFF2Index &source_axis_indices;
  अचर MultiItemVariationStore &source_var_store;

  hb_vector_t<अचिन्हित> old_indices;
  hb_sorted_vector_t<hb_codepoint_t> new_gids;
  अचिन्हित data_size = 0;

  hb_set_t condition_indices;
  hb_set_t axis_indices;
  hb_set_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_vector_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_vector_t<अचिन्हित वर्ण> record_data;
  hb_vector_t<hb_ubytes_t> records;

  hb_vector_t<hb_ubytes_t> selected_axis_indices;
  hb_vector_t<hb_vector_t<अचिन्हित वर्ण>> axis_indices_data;
  अचिन्हित axis_indices_data_size = 0;
  hb_map_t axes_index_map;
  अचर hb_map_t *axis_map = nullptr;

  गुप्त:
  तार्किक auxiliary_indices_unchanged () अचर
  {
    लागि (hb_codepoint_t old_index : condition_indices)
      यदि (condition_map.get (old_index) != old_index)
	फर्काऊ असत्य;
    लागि (hb_codepoint_t old_index : axis_indices)
      यदि (axis_indices_map.get (old_index) != old_index)
	फर्काऊ असत्य;
    लागि (hb_codepoint_t old_index : var_indices)
      यदि (varidx_map.get (old_index) != old_index)
	फर्काऊ असत्य;
    फर्काऊ सत्य;
  }

  तार्किक append_varint (uint32_t value)
  {
    अचिन्हित size = HBUINT32VAR::get_size (value);
    अचिन्हित offset = record_data.length;
    अचिन्हित new_length = hb_unsigned_add_saturate (offset, size);
    यदि (unlikely (new_length == अच_पूर्णांक_उच्च ||
		  !record_data.resize_dirty (new_length)))
      फर्काऊ असत्य;
    HBUINT32VAR::serialize_unsafe (record_data.arrayZ + offset, value);
    फर्काऊ सत्य;
  }

  तार्किक append_bytes (hb_ubytes_t record,
		     अचिन्हित component_size,
		     अचिन्हित start,
		     अचिन्हित end)
  {
    यदि (unlikely (start > end || end > component_size)) फर्काऊ असत्य;
    अचिन्हित offset = record_data.length;
    अचिन्हित size = end - start;
    अचिन्हित new_length = hb_unsigned_add_saturate (offset, size);
    यदि (unlikely (new_length == अच_पूर्णांक_उच्च ||
		  !record_data.resize_dirty (new_length)))
      फर्काऊ असत्य;
    hb_memcpy (record_data.arrayZ + offset, record.arrayZ + start, size);
    फर्काऊ सत्य;
  }

  तार्किक compile_record (hb_ubytes_t record, तार्किक retain_gids)
  {
    hb_ubytes_t remaining = record;
    जबसम्म (remaining)
    {
      VarComponent::record_t component;
      यदि (unlikely (!VarComponent::decompile_record (source, remaining,
						     nullptr, nullptr,
						     &component) ||
		    !compile_component (remaining, component, retain_gids)))
	फर्काऊ असत्य;
      remaining = remaining.sub_array (component.size);
    }
    फर्काऊ सत्य;
  }

  तार्किक compile_component (hb_ubytes_t record,
			  अचर VarComponent::record_t &component,
			  तार्किक retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    यदि (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      फर्काऊ असत्य;

    uint32_t flags = component.flags;
    अचिन्हित gid_size = component.gid_size;
    यदि (new_gid > 0xFFFFu)
    {
      flags |= (अचिन्हित) VarComponent::flags_t::GID_IS_24BIT;
      gid_size = HBGlyphID24::static_size;
    }

    यदि (unlikely (!append_varint (flags))) फर्काऊ असत्य;
    अचिन्हित gid_offset = record_data.length;
    अचिन्हित gid_end = hb_unsigned_add_saturate (gid_offset, gid_size);
    यदि (unlikely (gid_end == अच_पूर्णांक_उच्च ||
		  !record_data.resize_dirty (gid_end)))
      फर्काऊ असत्य;
    अचिन्हित वर्ण *out = record_data.arrayZ + gid_offset;
    यदि (gid_size == HBGlyphID16::static_size)
      * (HBGlyphID16 *) out = new_gid;
    अन्यथा
      * (HBGlyphID24 *) out = new_gid;

    अचिन्हित cursor = component.gid_offset + component.gid_size;
    यदि (flags & (अचिन्हित) VarComponent::flags_t::HAVE_CONDITION)
    {
      यदि (unlikely (!condition_map.has (component.condition_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.condition_offset) ||
		    !append_varint (condition_map.get (component.condition_index))))
	फर्काऊ असत्य;
      cursor = component.condition_offset + component.condition_size;
    }
    यदि (flags & (अचिन्हित) VarComponent::flags_t::HAVE_AXES)
    {
      यदि (unlikely (!axis_indices_map.has (component.axis_indices_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_indices_offset) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_index))))
	फर्काऊ असत्य;
      cursor = component.axis_indices_offset + component.axis_indices_size;
    }
    यदि (flags & (अचिन्हित) VarComponent::flags_t::AXIS_मानहरू_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_मानहरू_var_idx;
      यदि (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_मानहरू_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	फर्काऊ असत्य;
      cursor = component.axis_मानहरू_var_offset + component.axis_मानहरू_var_size;
    }
    यदि (flags & (अचिन्हित) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      यदि (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.transform_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	फर्काऊ असत्य;
      cursor = component.transform_var_offset + component.transform_var_size;
    }

    फर्काऊ append_bytes (record, component.size, cursor, component.size);
  }
};

तार्किक
VARC::subset (hb_subset_context_t *c) अचर
{
  TRACE_SUBSET (एतत्);
  स्वतः fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    फर्काऊ असत्य;
  };

  अचर Coverage &source_coverage = एतत्+coverage;
  अचर CFF2Index &source_records = एतत्+glyphRecords;
  अचर ConditionList &source_conditions = एतत्+conditionList;
  अचर TupleList &source_axis_indices = एतत्+axisIndicesList;
  अचर CFF2Index &source_axis_indices_index = source_axis_indices;
  अचर MultiItemVariationStore &source_var_store = एतत्+varStore;
  varc_subset_plan_t subset_plan (*एतत्, c,
				  source_coverage,
				  source_records,
				  source_conditions,
				  source_axis_indices_index,
				  source_var_store);
  यदि (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  यदि (!subset_plan.old_indices)
    return_trace (असत्य);
  यदि (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_records ()))
    return_trace (fail ());

  VARC *out = c->serializer->start_embed<VARC> ();
  यदि (unlikely (!out || !c->serializer->extend_min (out)))
    return_trace (असत्य);
  out->version = version;

  यदि (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  यदि (unlikely (!out->coverage.serialize_serialize (c->serializer,
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
    return_trace (असत्य);

  return_trace (सत्य);
}

} /* namespace OT */

#endif /* OT_VAR_VARC_VARC_SUBSET_HH */
