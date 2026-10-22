#ifndef OT_VAR_VARC_VARC_SUBSET_HH
#define OT_VAR_VARC_VARC_SUBSET_HH

#अन्तर्भाव "VARC.hh"

नामाकाश OT {

संरचना varc_subset_plan_t
{
  varc_subset_plan_t (स्थिरांक VARC &source_,
		      hb_subset_context_t *subset_context_,
		      स्थिरांक Coverage &source_coverage_,
		      स्थिरांक CFF2Index &source_records_,
		      स्थिरांक ConditionList &source_conditions_,
		      स्थिरांक CFF2Index &source_axis_indices_,
		      स्थिरांक MultiItemVariationStore &source_var_store_) :
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
    स्थिरांक hb_set_t &glyphset = subset_context->plan->_glyphset_varced;
    साठी (स्वतः _ : + hb_zip (source_coverage,
			    hb_range ((अचिन्हित) source_records.मोजणी)))
    {
      hb_codepoint_t old_gid = _.first;
      अचिन्हित index = _.second;
      जर (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	जर (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  परत असत्य;
	old_indices.push (index);
	new_gids.push (new_gid);
	data_size = hb_unsigned_add_saturate (data_size,
					      source_records[index].length);
      }
    }

    परत !old_indices.in_error () && !new_gids.in_error () &&
	   data_size != अच_पूर्णांक_उच्च;
  }

  तार्किक collect_indices ()
  {
    साठी (अचिन्हित old_index : old_indices)
    {
      hb_ubytes_t remaining = source_records[old_index];
      पर्यंत (remaining)
      {
	VarComponent::record_t component;
	जर (unlikely (!VarComponent::decompile_record (source, remaining,
						       nullptr, nullptr,
						       &component)))
	  परत असत्य;

	जर (component.flags & (अचिन्हित) VarComponent::flags_t::HAVE_CONDITION)
	{
	  जर (unlikely (component.condition_index >= source_conditions.get_मोजणी ()))
	    परत असत्य;
	  condition_indices.add (component.condition_index);
	}
	जर (component.flags & (अचिन्हित) VarComponent::flags_t::HAVE_AXES)
	{
	  जर (unlikely (component.axis_indices_index >= source_axis_indices.मोजणी))
	    परत असत्य;
	  axis_indices.add (component.axis_indices_index);
	}
	जर ((component.flags & (अचिन्हित) VarComponent::flags_t::AXIS_मूल्ये_HAVE_VARIATION) &&
	    component.axis_मूल्ये_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.axis_मूल्ये_var_idx);
	जर ((component.flags & (अचिन्हित) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.transform_var_idx);

	remaining = remaining.sub_array (component.size);
      }
    }

    जर (unlikely (condition_indices.in_error () || axis_indices.in_error () ||
		  var_indices.in_error ()))
      परत असत्य;

    साठी (hb_codepoint_t condition_index : condition_indices)
      जर (unlikely (!source_conditions[condition_index].collect_var_indices (&var_indices)))
	परत असत्य;

    परत सत्य;
  }

  तार्किक create_maps ()
  {
    स्थिरांक स्वतः *plan = subset_context->plan;
    जर (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_index_map;
      साठी (स्वतः _ : plan->axes_index_map)
	जर (!plan->user_axes_location.has (plan->axes_old_index_tag_map.get (_.first)))
	  axes_index_map.set (_.first, _.second);
      जर (unlikely (axes_index_map.in_error ())) परत असत्य;
    }
    condition_map.add_set (&condition_indices);
    axis_indices_map.add_set (&axis_indices);
    परत !condition_map.in_error () && !axis_indices_map.in_error () &&
	   source_var_store.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  तार्किक compile_records ()
  {
    जर (unlikely (!records.alloc_exact (old_indices.length)))
      परत असत्य;

    तार्किक retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    जर (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      साठी (अचिन्हित old_index : old_indices)
	records.push (source_records[old_index]);
    }
    अन्यथा
    {
      hb_vector_t<अचिन्हित> record_offsets;
      record_data.alloc (data_size);
      जर (unlikely (!record_offsets.alloc_exact (old_indices.length + 1)))
	परत असत्य;

      साठी (अचिन्हित old_index : old_indices)
      {
	record_offsets.push (record_data.length);
	जर (unlikely (!compile_record (source_records[old_index], retain_gids)))
	  परत असत्य;
      }
      record_offsets.push (record_data.length);

      जर (unlikely (record_data.in_error () || record_offsets.in_error ()))
	परत असत्य;
      साठी (अचिन्हित i = 0; i < old_indices.length; i++)
	records.push (hb_ubytes_t (record_data.arrayZ + record_offsets[i],
				  record_offsets[i + 1] - record_offsets[i]));
      data_size = record_data.length;
    }

    परत !records.in_error ();
  }

  तार्किक select_axis_indices ()
  {
    जर (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_data.resize (
					  axis_indices_map.get_population ()))))
      परत असत्य;
    साठी (अचिन्हित i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t bytes = source_axis_indices[axis_indices_map.backward (i)];
      जर (axis_map)
      {
	hb_vector_t<पूर्णांक> indices;
	स्थिरांक HBUINT8 *p = (स्थिरांक HBUINT8 *) bytes.arrayZ;
	जर (unlikely (!Tuple_मूल्ये::decompile (p, indices, p + bytes.length, सत्य)))
	  परत असत्य;
	साठी (पूर्णांक &index : indices)
	{
	  जर (unlikely (!axis_map->has (index))) परत असत्य;
	  index = axis_map->get (index);
	}
	स्वतः &data = axis_indices_data[i];
	/* One control byte and at most four data bytes per value. */
	अचिन्हित size = hb_unsigned_mul_saturate (indices.length, 5u);
	जर (unlikely (size == अच_पूर्णांक_उच्च || !data.resize_dirty (size))) परत असत्य;
	data.shrink (Tuple_मूल्ये::compile_unsafe (indices.as_array (), data.arrayZ));
	bytes = data.as_array ();
      }
      axis_indices_data_size = hb_unsigned_add_saturate (axis_indices_data_size,
							 bytes.length);
      selected_axis_indices.push (bytes);
    }

    परत axis_indices_data_size != अच_पूर्णांक_उच्च &&
	   !selected_axis_indices.in_error ();
  }

  स्थिरांक VARC &source;
  hb_subset_context_t *subset_context;
  स्थिरांक Coverage &source_coverage;
  स्थिरांक CFF2Index &source_records;
  स्थिरांक ConditionList &source_conditions;
  स्थिरांक CFF2Index &source_axis_indices;
  स्थिरांक MultiItemVariationStore &source_var_store;

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

  hb_vector_t<अचिन्हित अक्षर> record_data;
  hb_vector_t<hb_ubytes_t> records;

  hb_vector_t<hb_ubytes_t> selected_axis_indices;
  hb_vector_t<hb_vector_t<अचिन्हित अक्षर>> axis_indices_data;
  अचिन्हित axis_indices_data_size = 0;
  hb_map_t axes_index_map;
  स्थिरांक hb_map_t *axis_map = nullptr;

  खाजगी:
  तार्किक auxiliary_indices_unchanged () स्थिरांक
  {
    साठी (hb_codepoint_t old_index : condition_indices)
      जर (condition_map.get (old_index) != old_index)
	परत असत्य;
    साठी (hb_codepoint_t old_index : axis_indices)
      जर (axis_indices_map.get (old_index) != old_index)
	परत असत्य;
    साठी (hb_codepoint_t old_index : var_indices)
      जर (varidx_map.get (old_index) != old_index)
	परत असत्य;
    परत सत्य;
  }

  तार्किक append_varint (uint32_t value)
  {
    अचिन्हित size = HBUINT32VAR::get_size (value);
    अचिन्हित offset = record_data.length;
    अचिन्हित new_length = hb_unsigned_add_saturate (offset, size);
    जर (unlikely (new_length == अच_पूर्णांक_उच्च ||
		  !record_data.resize_dirty (new_length)))
      परत असत्य;
    HBUINT32VAR::serialize_unsafe (record_data.arrayZ + offset, value);
    परत सत्य;
  }

  तार्किक append_bytes (hb_ubytes_t record,
		     अचिन्हित component_size,
		     अचिन्हित start,
		     अचिन्हित end)
  {
    जर (unlikely (start > end || end > component_size)) परत असत्य;
    अचिन्हित offset = record_data.length;
    अचिन्हित size = end - start;
    अचिन्हित new_length = hb_unsigned_add_saturate (offset, size);
    जर (unlikely (new_length == अच_पूर्णांक_उच्च ||
		  !record_data.resize_dirty (new_length)))
      परत असत्य;
    hb_memcpy (record_data.arrayZ + offset, record.arrayZ + start, size);
    परत सत्य;
  }

  तार्किक compile_record (hb_ubytes_t record, तार्किक retain_gids)
  {
    hb_ubytes_t remaining = record;
    पर्यंत (remaining)
    {
      VarComponent::record_t component;
      जर (unlikely (!VarComponent::decompile_record (source, remaining,
						     nullptr, nullptr,
						     &component) ||
		    !compile_component (remaining, component, retain_gids)))
	परत असत्य;
      remaining = remaining.sub_array (component.size);
    }
    परत सत्य;
  }

  तार्किक compile_component (hb_ubytes_t record,
			  स्थिरांक VarComponent::record_t &component,
			  तार्किक retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    जर (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      परत असत्य;

    uint32_t flags = component.flags;
    अचिन्हित gid_size = component.gid_size;
    जर (new_gid > 0xFFFFu)
    {
      flags |= (अचिन्हित) VarComponent::flags_t::GID_IS_24BIT;
      gid_size = HBGlyphID24::static_size;
    }

    जर (unlikely (!append_varint (flags))) परत असत्य;
    अचिन्हित gid_offset = record_data.length;
    अचिन्हित gid_end = hb_unsigned_add_saturate (gid_offset, gid_size);
    जर (unlikely (gid_end == अच_पूर्णांक_उच्च ||
		  !record_data.resize_dirty (gid_end)))
      परत असत्य;
    अचिन्हित अक्षर *out = record_data.arrayZ + gid_offset;
    जर (gid_size == HBGlyphID16::static_size)
      * (HBGlyphID16 *) out = new_gid;
    अन्यथा
      * (HBGlyphID24 *) out = new_gid;

    अचिन्हित cursor = component.gid_offset + component.gid_size;
    जर (flags & (अचिन्हित) VarComponent::flags_t::HAVE_CONDITION)
    {
      जर (unlikely (!condition_map.has (component.condition_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.condition_offset) ||
		    !append_varint (condition_map.get (component.condition_index))))
	परत असत्य;
      cursor = component.condition_offset + component.condition_size;
    }
    जर (flags & (अचिन्हित) VarComponent::flags_t::HAVE_AXES)
    {
      जर (unlikely (!axis_indices_map.has (component.axis_indices_index) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_indices_offset) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_index))))
	परत असत्य;
      cursor = component.axis_indices_offset + component.axis_indices_size;
    }
    जर (flags & (अचिन्हित) VarComponent::flags_t::AXIS_मूल्ये_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_मूल्ये_var_idx;
      जर (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.axis_मूल्ये_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	परत असत्य;
      cursor = component.axis_मूल्ये_var_offset + component.axis_मूल्ये_var_size;
    }
    जर (flags & (अचिन्हित) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      जर (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.size,
				   cursor, component.transform_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	परत असत्य;
      cursor = component.transform_var_offset + component.transform_var_size;
    }

    परत append_bytes (record, component.size, cursor, component.size);
  }
};

तार्किक
VARC::subset (hb_subset_context_t *c) स्थिरांक
{
  TRACE_SUBSET (हे);
  स्वतः fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    परत असत्य;
  };

  स्थिरांक Coverage &source_coverage = हे+coverage;
  स्थिरांक CFF2Index &source_records = हे+glyphRecords;
  स्थिरांक ConditionList &source_conditions = हे+conditionList;
  स्थिरांक TupleList &source_axis_indices = हे+axisIndicesList;
  स्थिरांक CFF2Index &source_axis_indices_index = source_axis_indices;
  स्थिरांक MultiItemVariationStore &source_var_store = हे+varStore;
  varc_subset_plan_t subset_plan (*हे, c,
				  source_coverage,
				  source_records,
				  source_conditions,
				  source_axis_indices_index,
				  source_var_store);
  जर (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  जर (!subset_plan.old_indices)
    return_trace (असत्य);
  जर (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_records ()))
    return_trace (fail ());

  VARC *out = c->serializer->start_embed<VARC> ();
  जर (unlikely (!out || !c->serializer->extend_min (out)))
    return_trace (असत्य);
  out->version = version;

  जर (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (fail ());

  जर (unlikely (!out->coverage.serialize_serialize (c->serializer,
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
