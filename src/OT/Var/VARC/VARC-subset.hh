#अगर_अघोषित OT_VAR_VARC_VARC_SUBSET_HH
#घोषणा OT_VAR_VARC_VARC_SUBSET_HH

#समावेश "VARC.hh"

namespace OT {

काष्ठा varc_subset_plan_t
{
  varc_subset_plan_t (स्थिर VARC &source_,
		      hb_subset_context_t *subset_context_,
		      स्थिर Coverage &source_coverage_,
		      स्थिर CFF2_सूचकांक &source_records_,
		      स्थिर ConditionList &source_conditions_,
		      स्थिर CFF2_सूचकांक &source_axis_indices_,
		      स्थिर MultiItemVariationStore &source_var_store_) :
    source (source_),
    subset_context (subset_context_),
    source_coverage (source_coverage_),
    source_records (source_records_),
    source_conditions (source_conditions_),
    source_axis_indices (source_axis_indices_),
    source_var_store (source_var_store_)
  {}

  bool collect_glyphs ()
  {
    स्थिर hb_set_t &glyphset = subset_context->plan->_glyphset_varced;
    क्रम (स्वतः _ : + hb_zip (source_coverage,
			    hb_range ((अचिन्हित) source_records.गिनती)))
    {
      hb_codepoint_t old_gid = _.first;
      अचिन्हित सूचकांक_2 = _.second;
      अगर (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	अगर (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  वापस false;
	old_indices.push (सूचकांक_2);
	new_gids.push (new_gid);
	तथ्य_आकार = hb_unsigned_add_saturate (तथ्य_आकार,
					      source_records[सूचकांक_2].लंबाई);
      }
    }

    वापस !old_indices.in_error () && !new_gids.in_error () &&
	   तथ्य_आकार != अच_पूर्णांक_उच्च;
  }

  bool collect_indices ()
  {
    क्रम (अचिन्हित old_सूचकांक : old_indices)
    {
      hb_ubytes_t remaining = source_records[old_सूचकांक];
      जबतक (remaining)
      {
	VarComponent::record_t component;
	अगर (unlikely (!VarComponent::decompile_record (source, remaining,
						       nullptr, nullptr,
						       &component)))
	  वापस false;

	अगर (component.flags & (अचिन्हित) VarComponent::flags_t::HAVE_CONDITION)
	{
	  अगर (unlikely (component.condition_सूचकांक >= source_conditions.get_गिनती ()))
	    वापस false;
	  condition_indices.add (component.condition_सूचकांक);
	}
	अगर (component.flags & (अचिन्हित) VarComponent::flags_t::HAVE_AXES)
	{
	  अगर (unlikely (component.axis_indices_सूचकांक >= source_axis_indices.गिनती))
	    वापस false;
	  axis_indices.add (component.axis_indices_सूचकांक);
	}
	अगर ((component.flags & (अचिन्हित) VarComponent::flags_t::AXIS_मूल्यावली_HAVE_VARIATION) &&
	    component.axis_मूल्यावली_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.axis_मूल्यावली_var_idx);
	अगर ((component.flags & (अचिन्हित) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.transform_var_idx);

	remaining = remaining.sub_array (component.आकार);
      }
    }

    अगर (unlikely (condition_indices.in_error () || axis_indices.in_error () ||
		  var_indices.in_error ()))
      वापस false;

    क्रम (hb_codepoint_t condition_सूचकांक : condition_indices)
      अगर (unlikely (!source_conditions[condition_सूचकांक].collect_var_indices (&var_indices)))
	वापस false;

    वापस true;
  }

  bool create_maps ()
  {
    स्थिर स्वतः *plan = subset_context->plan;
    अगर (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_सूचकांक_map;
      क्रम (स्वतः _ : plan->axes_सूचकांक_map)
	अगर (!plan->user_axes_location.has (plan->axes_old_सूचकांक_tag_map.लो (_.first)))
	  axes_सूचकांक_map.set (_.first, _.second);
      अगर (unlikely (axes_सूचकांक_map.in_error ())) वापस false;
    }
    condition_map.add_set (&condition_indices);
    axis_indices_map.add_set (&axis_indices);
    वापस !condition_map.in_error () && !axis_indices_map.in_error () &&
	   source_var_store.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  bool compile_records ()
  {
    अगर (unlikely (!records.alloc_exact (old_indices.लंबाई)))
      वापस false;

    bool retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    अगर (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      क्रम (अचिन्हित old_सूचकांक : old_indices)
	records.push (source_records[old_सूचकांक]);
    }
    अन्यथा
    {
      hb_vector_t<अचिन्हित> record_offsets;
      record_तथ्य.alloc (तथ्य_आकार);
      अगर (unlikely (!record_offsets.alloc_exact (old_indices.लंबाई + 1)))
	वापस false;

      क्रम (अचिन्हित old_सूचकांक : old_indices)
      {
	record_offsets.push (record_तथ्य.लंबाई);
	अगर (unlikely (!compile_record (source_records[old_सूचकांक], retain_gids)))
	  वापस false;
      }
      record_offsets.push (record_तथ्य.लंबाई);

      अगर (unlikely (record_तथ्य.in_error () || record_offsets.in_error ()))
	वापस false;
      क्रम (अचिन्हित i = 0; i < old_indices.लंबाई; i++)
	records.push (hb_ubytes_t (record_तथ्य.arrayZ + record_offsets[i],
				  record_offsets[i + 1] - record_offsets[i]));
      तथ्य_आकार = record_तथ्य.लंबाई;
    }

    वापस !records.in_error ();
  }

  bool select_axis_indices ()
  {
    अगर (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_तथ्य.resize (
					  axis_indices_map.get_population ()))))
      वापस false;
    क्रम (अचिन्हित i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t bytes = source_axis_indices[axis_indices_map.backward (i)];
      अगर (axis_map)
      {
	hb_vector_t<पूर्णांक> indices;
	स्थिर HBUINT8 *p = (स्थिर HBUINT8 *) bytes.arrayZ;
	अगर (unlikely (!Tuple_मूल्यावली::decompile (p, indices, p + bytes.लंबाई, true)))
	  वापस false;
	क्रम (पूर्णांक &सूचकांक_2 : indices)
	{
	  अगर (unlikely (!axis_map->has (सूचकांक_2))) वापस false;
	  सूचकांक_2 = axis_map->लो (सूचकांक_2);
	}
	स्वतः &तथ्य = axis_indices_तथ्य[i];
	/* One control byte and at most four data bytes per value. */
	अचिन्हित आकार = hb_unsigned_mul_saturate (indices.लंबाई, 5u);
	अगर (unlikely (आकार == अच_पूर्णांक_उच्च || !तथ्य.resize_dirty (आकार))) वापस false;
	तथ्य.shrink (Tuple_मूल्यावली::compile_unsafe (indices.as_array (), तथ्य.arrayZ));
	bytes = तथ्य.as_array ();
      }
      axis_indices_तथ्य_आकार = hb_unsigned_add_saturate (axis_indices_तथ्य_आकार,
							 bytes.लंबाई);
      selected_axis_indices.push (bytes);
    }

    वापस axis_indices_तथ्य_आकार != अच_पूर्णांक_उच्च &&
	   !selected_axis_indices.in_error ();
  }

  स्थिर VARC &source;
  hb_subset_context_t *subset_context;
  स्थिर Coverage &source_coverage;
  स्थिर CFF2_सूचकांक &source_records;
  स्थिर ConditionList &source_conditions;
  स्थिर CFF2_सूचकांक &source_axis_indices;
  स्थिर MultiItemVariationStore &source_var_store;

  hb_vector_t<अचिन्हित> old_indices;
  hb_sorted_vector_t<hb_codepoint_t> new_gids;
  अचिन्हित तथ्य_आकार = 0;

  hb_set_t condition_indices;
  hb_set_t axis_indices;
  hb_set_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_vector_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_vector_t<अचिन्हित अक्षर> record_तथ्य;
  hb_vector_t<hb_ubytes_t> records;

  hb_vector_t<hb_ubytes_t> selected_axis_indices;
  hb_vector_t<hb_vector_t<अचिन्हित अक्षर>> axis_indices_तथ्य;
  अचिन्हित axis_indices_तथ्य_आकार = 0;
  hb_map_t axes_सूचकांक_map;
  स्थिर hb_map_t *axis_map = nullptr;

  निजी:
  bool auxiliary_indices_unchanged () स्थिर
  {
    क्रम (hb_codepoint_t old_सूचकांक : condition_indices)
      अगर (condition_map.लो (old_सूचकांक) != old_सूचकांक)
	वापस false;
    क्रम (hb_codepoint_t old_सूचकांक : axis_indices)
      अगर (axis_indices_map.लो (old_सूचकांक) != old_सूचकांक)
	वापस false;
    क्रम (hb_codepoint_t old_सूचकांक : var_indices)
      अगर (varidx_map.लो (old_सूचकांक) != old_सूचकांक)
	वापस false;
    वापस true;
  }

  bool append_varint (uint32_t value)
  {
    अचिन्हित आकार = HBUINT32VAR::get_आकार (value);
    अचिन्हित offset = record_तथ्य.लंबाई;
    अचिन्हित new_लंबाई = hb_unsigned_add_saturate (offset, आकार);
    अगर (unlikely (new_लंबाई == अच_पूर्णांक_उच्च ||
		  !record_तथ्य.resize_dirty (new_लंबाई)))
      वापस false;
    HBUINT32VAR::serialize_unsafe (record_तथ्य.arrayZ + offset, value);
    वापस true;
  }

  bool append_bytes (hb_ubytes_t record,
		     अचिन्हित component_आकार,
		     अचिन्हित start,
		     अचिन्हित end)
  {
    अगर (unlikely (start > end || end > component_आकार)) वापस false;
    अचिन्हित offset = record_तथ्य.लंबाई;
    अचिन्हित आकार = end - start;
    अचिन्हित new_लंबाई = hb_unsigned_add_saturate (offset, आकार);
    अगर (unlikely (new_लंबाई == अच_पूर्णांक_उच्च ||
		  !record_तथ्य.resize_dirty (new_लंबाई)))
      वापस false;
    hb_memcpy (record_तथ्य.arrayZ + offset, record.arrayZ + start, आकार);
    वापस true;
  }

  bool compile_record (hb_ubytes_t record, bool retain_gids)
  {
    hb_ubytes_t remaining = record;
    जबतक (remaining)
    {
      VarComponent::record_t component;
      अगर (unlikely (!VarComponent::decompile_record (source, remaining,
						     nullptr, nullptr,
						     &component) ||
		    !compile_component (remaining, component, retain_gids)))
	वापस false;
      remaining = remaining.sub_array (component.आकार);
    }
    वापस true;
  }

  bool compile_component (hb_ubytes_t record,
			  स्थिर VarComponent::record_t &component,
			  bool retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    अगर (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      वापस false;

    uint32_t flags = component.flags;
    अचिन्हित gid_आकार = component.gid_आकार;
    अगर (new_gid > 0xFFFFu)
    {
      flags |= (अचिन्हित) VarComponent::flags_t::GID_IS_24BIT;
      gid_आकार = HBGlyphID24::static_आकार;
    }

    अगर (unlikely (!append_varint (flags))) वापस false;
    अचिन्हित gid_offset = record_तथ्य.लंबाई;
    अचिन्हित gid_end = hb_unsigned_add_saturate (gid_offset, gid_आकार);
    अगर (unlikely (gid_end == अच_पूर्णांक_उच्च ||
		  !record_तथ्य.resize_dirty (gid_end)))
      वापस false;
    अचिन्हित अक्षर *out = record_तथ्य.arrayZ + gid_offset;
    अगर (gid_आकार == HBGlyphID16::static_आकार)
      * (HBGlyphID16 *) out = new_gid;
    अन्यथा
      * (HBGlyphID24 *) out = new_gid;

    अचिन्हित cursor = component.gid_offset + component.gid_आकार;
    अगर (flags & (अचिन्हित) VarComponent::flags_t::HAVE_CONDITION)
    {
      अगर (unlikely (!condition_map.has (component.condition_सूचकांक) ||
		    !append_bytes (record, component.आकार,
				   cursor, component.condition_offset) ||
		    !append_varint (condition_map.लो (component.condition_सूचकांक))))
	वापस false;
      cursor = component.condition_offset + component.condition_आकार;
    }
    अगर (flags & (अचिन्हित) VarComponent::flags_t::HAVE_AXES)
    {
      अगर (unlikely (!axis_indices_map.has (component.axis_indices_सूचकांक) ||
		    !append_bytes (record, component.आकार,
				   cursor, component.axis_indices_offset) ||
		    !append_varint (axis_indices_map.लो (component.axis_indices_सूचकांक))))
	वापस false;
      cursor = component.axis_indices_offset + component.axis_indices_आकार;
    }
    अगर (flags & (अचिन्हित) VarComponent::flags_t::AXIS_मूल्यावली_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_मूल्यावली_var_idx;
      अगर (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.आकार,
				   cursor, component.axis_मूल्यावली_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.लो (var_idx))))
	वापस false;
      cursor = component.axis_मूल्यावली_var_offset + component.axis_मूल्यावली_var_आकार;
    }
    अगर (flags & (अचिन्हित) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      अगर (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.आकार,
				   cursor, component.transform_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.लो (var_idx))))
	वापस false;
      cursor = component.transform_var_offset + component.transform_var_आकार;
    }

    वापस append_bytes (record, component.आकार, cursor, component.आकार);
  }
};

bool
VARC::subset (hb_subset_context_t *c) स्थिर
{
  TRACE_SUBSET (यह);
  स्वतः हार = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    वापस false;
  };

  स्थिर Coverage &source_coverage = यह+coverage;
  स्थिर CFF2_सूचकांक &source_records = यह+glyphRecords;
  स्थिर ConditionList &source_conditions = यह+conditionList;
  स्थिर TupleList &source_axis_indices = यह+axisIndicesList;
  स्थिर CFF2_सूचकांक &source_axis_indices_सूचकांक = source_axis_indices;
  स्थिर MultiItemVariationStore &source_var_store = यह+varStore;
  varc_subset_plan_t subset_plan (*यह, c,
				  source_coverage,
				  source_records,
				  source_conditions,
				  source_axis_indices_सूचकांक,
				  source_var_store);
  अगर (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (हार ());
  अगर (!subset_plan.old_indices)
    return_trace (false);
  अगर (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_records ()))
    return_trace (हार ());

  VARC *out = c->serializer->start_embed<VARC> ();
  अगर (unlikely (!out || !c->serializer->extend_min (out)))
    return_trace (false);
  out->version = version;

  अगर (unlikely (!subset_plan.select_axis_indices ()))
    return_trace (हार ());

  अगर (unlikely (!out->coverage.serialize_serialize (c->serializer,
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
							&subset_plan.axis_indices_तथ्य_आकार)) ||
		(subset_plan.var_indices.get_population () &&
		 !out->varStore.serialize_serialize (c->serializer,
						  &source_var_store,
						  subset_plan.var_inner_maps.as_array (),
						  subset_plan.axis_map)) ||
		!out->glyphRecords.serialize_serialize (c->serializer,
						 subset_plan.records.iter (),
						 &subset_plan.तथ्य_आकार)))
    return_trace (false);

  return_trace (true);
}

} /* namespace OT */

#पूर्ण_अगर /* OT_VAR_VARC_VARC_SUBSET_HH */
