#ifndef OT_VAR_VARC_VARC_SUBSET_HH
#define OT_VAR_VARC_VARC_SUBSET_HH

#অন্তর্ভাব "VARC.hh"

নামাকাশ OT {

কাঠামো varc_subset_plan_t
{
  varc_subset_plan_t (ধ্রুবক VARC &source_,
		      hb_subset_context_t *subset_context_,
		      ধ্রুবক Coverage &source_coverage_,
		      ধ্রুবক CFF2_সূচক &source_records_,
		      ধ্রুবক ConditionList &source_conditions_,
		      ধ্রুবক CFF2_সূচক &source_axis_indices_,
		      ধ্রুবক MultiItemVariationStore &source_var_store_) :
    source (source_),
    subset_context (subset_context_),
    source_coverage (source_coverage_),
    source_records (source_records_),
    source_conditions (source_conditions_),
    source_axis_indices (source_axis_indices_),
    source_var_store (source_var_store_)
  {}

  যৌক্তিক collect_glyphs ()
  {
    ধ্রুবক hb_set_t &glyphset = subset_context->plan->_glyphset_varced;
    জন্য (auto _ : + hb_zip (source_coverage,
			    hb_range ((unsigned) source_records.গুনতি)))
    {
      hb_codepoint_t old_gid = _.first;
      unsigned সূচক_2 = _.second;
      যদি (glyphset.has (old_gid))
      {
	hb_codepoint_t new_gid;
	যদি (unlikely (!subset_context->plan->new_gid_for_old_gid (old_gid,
								 &new_gid)))
	  ফেরত মিথ্যা;
	old_indices.push (সূচক_2);
	new_gids.push (new_gid);
	তথ্য_আকার = hb_unsigned_add_saturate (তথ্য_আকার,
					      source_records[সূচক_2].দৈর্ঘ্য);
      }
    }

    ফেরত !old_indices.in_error () && !new_gids.in_error () &&
	   তথ্য_আকার != UINT_MAX;
  }

  যৌক্তিক collect_indices ()
  {
    জন্য (unsigned old_সূচক : old_indices)
    {
      hb_ubytes_t remaining = source_records[old_সূচক];
      যখন (remaining)
      {
	VarComponent::record_t component;
	যদি (unlikely (!VarComponent::decompile_record (source, remaining,
						       nullptr, nullptr,
						       &component)))
	  ফেরত মিথ্যা;

	যদি (component.flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
	{
	  যদি (unlikely (component.condition_সূচক >= source_conditions.get_গুনতি ()))
	    ফেরত মিথ্যা;
	  condition_indices.add (component.condition_সূচক);
	}
	যদি (component.flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
	{
	  যদি (unlikely (component.axis_indices_সূচক >= source_axis_indices.গুনতি))
	    ফেরত মিথ্যা;
	  axis_indices.add (component.axis_indices_সূচক);
	}
	যদি ((component.flags & (unsigned) VarComponent::flags_t::AXIS_মানসমূহ_HAVE_VARIATION) &&
	    component.axis_মানসমূহ_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.axis_মানসমূহ_var_idx);
	যদি ((component.flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION) &&
	    component.transform_var_idx != VarIdx::NO_VARIATION)
	  var_indices.add (component.transform_var_idx);

	remaining = remaining.sub_array (component.আকার_2);
      }
    }

    যদি (unlikely (condition_indices.in_error () || axis_indices.in_error () ||
		  var_indices.in_error ()))
      ফেরত মিথ্যা;

    জন্য (hb_codepoint_t condition_সূচক : condition_indices)
      যদি (unlikely (!source_conditions[condition_সূচক].collect_var_indices (&var_indices)))
	ফেরত মিথ্যা;

    ফেরত সত্য;
  }

  যৌক্তিক create_maps ()
  {
    ধ্রুবক auto *plan = subset_context->plan;
    যদি (!plan->user_axes_location.is_empty ())
    {
      /* Instancing axes referenced by VARC is not supported. Keep this
       * restriction for avar2 as well; component coordinate overrides
       * and their variation data need special handling. */
      axis_map = &axes_সূচক_map;
      জন্য (auto _ : plan->axes_সূচক_map)
	যদি (!plan->user_axes_location.has (plan->axes_old_সূচক_tag_map.get (_.first)))
	  axes_সূচক_map.set (_.first, _.second);
      যদি (unlikely (axes_সূচক_map.in_error ())) ফেরত মিথ্যা;
    }
    condition_map.add_set (&condition_indices);
    axis_indices_map.add_set (&axis_indices);
    ফেরত !condition_map.in_error () && !axis_indices_map.in_error () &&
	   source_var_store.create_subset_plan (var_indices,
						&var_inner_maps,
						&varidx_map);
  }

  যৌক্তিক compile_records ()
  {
    যদি (unlikely (!records.alloc_exact (old_indices.দৈর্ঘ্য)))
      ফেরত মিথ্যা;

    যৌক্তিক retain_gids = subset_context->plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
    যদি (retain_gids && auxiliary_indices_unchanged ())
    {
      /* The records reference the same indices, so retain them byte-for-byte. */
      জন্য (unsigned old_সূচক : old_indices)
	records.push (source_records[old_সূচক]);
    }
    অন্যথা
    {
      hb_vector_t<unsigned> record_offsets;
      record_তথ্য.alloc (তথ্য_আকার);
      যদি (unlikely (!record_offsets.alloc_exact (old_indices.দৈর্ঘ্য + 1)))
	ফেরত মিথ্যা;

      জন্য (unsigned old_সূচক : old_indices)
      {
	record_offsets.push (record_তথ্য.দৈর্ঘ্য);
	যদি (unlikely (!compile_record (source_records[old_সূচক], retain_gids)))
	  ফেরত মিথ্যা;
      }
      record_offsets.push (record_তথ্য.দৈর্ঘ্য);

      যদি (unlikely (record_তথ্য.in_error () || record_offsets.in_error ()))
	ফেরত মিথ্যা;
      জন্য (unsigned i = 0; i < old_indices.দৈর্ঘ্য; i++)
	records.push (hb_ubytes_t (record_তথ্য.arrayZ + record_offsets[i],
				  record_offsets[i + 1] - record_offsets[i]));
      তথ্য_আকার = record_তথ্য.দৈর্ঘ্য;
    }

    ফেরত !records.in_error ();
  }

  যৌক্তিক select_axis_indices ()
  {
    যদি (unlikely (!selected_axis_indices.alloc_exact (
					  axis_indices_map.get_population ()) ||
		  (axis_map && !axis_indices_তথ্য.resize (
					  axis_indices_map.get_population ()))))
      ফেরত মিথ্যা;
    জন্য (unsigned i = 0; i < axis_indices_map.get_population (); i++)
    {
      hb_ubytes_t bytes = source_axis_indices[axis_indices_map.backward (i)];
      যদি (axis_map)
      {
	hb_vector_t<পূর্ণসংখ্যা> indices;
	ধ্রুবক HBUINT8 *p = (ধ্রুবক HBUINT8 *) bytes.arrayZ;
	যদি (unlikely (!Tuple_মানসমূহ::decompile (p, indices, p + bytes.দৈর্ঘ্য, সত্য)))
	  ফেরত মিথ্যা;
	জন্য (পূর্ণসংখ্যা &সূচক_2 : indices)
	{
	  যদি (unlikely (!axis_map->has (সূচক_2))) ফেরত মিথ্যা;
	  সূচক_2 = axis_map->get (সূচক_2);
	}
	auto &তথ্য = axis_indices_তথ্য[i];
	/* One control byte and at most four data bytes per value. */
	unsigned আকার_2 = hb_unsigned_mul_saturate (indices.দৈর্ঘ্য, 5u);
	যদি (unlikely (আকার_2 == UINT_MAX || !তথ্য.resize_dirty (আকার_2))) ফেরত মিথ্যা;
	তথ্য.shrink (Tuple_মানসমূহ::compile_unsafe (indices.as_array (), তথ্য.arrayZ));
	bytes = তথ্য.as_array ();
      }
      axis_indices_তথ্য_আকার = hb_unsigned_add_saturate (axis_indices_তথ্য_আকার,
							 bytes.দৈর্ঘ্য);
      selected_axis_indices.push (bytes);
    }

    ফেরত axis_indices_তথ্য_আকার != UINT_MAX &&
	   !selected_axis_indices.in_error ();
  }

  ধ্রুবক VARC &source;
  hb_subset_context_t *subset_context;
  ধ্রুবক Coverage &source_coverage;
  ধ্রুবক CFF2_সূচক &source_records;
  ধ্রুবক ConditionList &source_conditions;
  ধ্রুবক CFF2_সূচক &source_axis_indices;
  ধ্রুবক MultiItemVariationStore &source_var_store;

  hb_vector_t<unsigned> old_indices;
  hb_sorted_vector_t<hb_codepoint_t> new_gids;
  unsigned তথ্য_আকার = 0;

  hb_set_t condition_indices;
  hb_set_t axis_indices;
  hb_set_t var_indices;
  hb_inc_bimap_t condition_map;
  hb_inc_bimap_t axis_indices_map;
  hb_vector_t<hb_inc_bimap_t> var_inner_maps;
  hb_map_t varidx_map;

  hb_vector_t<unsigned অক্ষর> record_তথ্য;
  hb_vector_t<hb_ubytes_t> records;

  hb_vector_t<hb_ubytes_t> selected_axis_indices;
  hb_vector_t<hb_vector_t<unsigned অক্ষর>> axis_indices_তথ্য;
  unsigned axis_indices_তথ্য_আকার = 0;
  hb_map_t axes_সূচক_map;
  ধ্রুবক hb_map_t *axis_map = nullptr;

  ব্যক্তিগত:
  যৌক্তিক auxiliary_indices_unchanged () ধ্রুবক
  {
    জন্য (hb_codepoint_t old_সূচক : condition_indices)
      যদি (condition_map.get (old_সূচক) != old_সূচক)
	ফেরত মিথ্যা;
    জন্য (hb_codepoint_t old_সূচক : axis_indices)
      যদি (axis_indices_map.get (old_সূচক) != old_সূচক)
	ফেরত মিথ্যা;
    জন্য (hb_codepoint_t old_সূচক : var_indices)
      যদি (varidx_map.get (old_সূচক) != old_সূচক)
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  যৌক্তিক append_varint (uint32_t value)
  {
    unsigned আকার_2 = HBUINT32VAR::get_আকার (value);
    unsigned offset = record_তথ্য.দৈর্ঘ্য;
    unsigned new_দৈর্ঘ্য = hb_unsigned_add_saturate (offset, আকার_2);
    যদি (unlikely (new_দৈর্ঘ্য == UINT_MAX ||
		  !record_তথ্য.resize_dirty (new_দৈর্ঘ্য)))
      ফেরত মিথ্যা;
    HBUINT32VAR::serialize_unsafe (record_তথ্য.arrayZ + offset, value);
    ফেরত সত্য;
  }

  যৌক্তিক append_bytes (hb_ubytes_t record,
		     unsigned component_আকার,
		     unsigned start,
		     unsigned end)
  {
    যদি (unlikely (start > end || end > component_আকার)) ফেরত মিথ্যা;
    unsigned offset = record_তথ্য.দৈর্ঘ্য;
    unsigned আকার_2 = end - start;
    unsigned new_দৈর্ঘ্য = hb_unsigned_add_saturate (offset, আকার_2);
    যদি (unlikely (new_দৈর্ঘ্য == UINT_MAX ||
		  !record_তথ্য.resize_dirty (new_দৈর্ঘ্য)))
      ফেরত মিথ্যা;
    hb_memcpy (record_তথ্য.arrayZ + offset, record.arrayZ + start, আকার_2);
    ফেরত সত্য;
  }

  যৌক্তিক compile_record (hb_ubytes_t record, যৌক্তিক retain_gids)
  {
    hb_ubytes_t remaining = record;
    যখন (remaining)
    {
      VarComponent::record_t component;
      যদি (unlikely (!VarComponent::decompile_record (source, remaining,
						     nullptr, nullptr,
						     &component) ||
		    !compile_component (remaining, component, retain_gids)))
	ফেরত মিথ্যা;
      remaining = remaining.sub_array (component.আকার_2);
    }
    ফেরত সত্য;
  }

  যৌক্তিক compile_component (hb_ubytes_t record,
			  ধ্রুবক VarComponent::record_t &component,
			  যৌক্তিক retain_gids)
  {
    hb_codepoint_t new_gid = component.gid;
    যদি (unlikely ((!retain_gids &&
		   !subset_context->plan->new_gid_for_old_gid (component.gid,
								 &new_gid)) ||
		  new_gid > 0xFFFFFFu))
      ফেরত মিথ্যা;

    uint32_t flags = component.flags;
    unsigned gid_আকার = component.gid_আকার;
    যদি (new_gid > 0xFFFFu)
    {
      flags |= (unsigned) VarComponent::flags_t::GID_IS_24BIT;
      gid_আকার = HBGlyphID24::static_আকার;
    }

    যদি (unlikely (!append_varint (flags))) ফেরত মিথ্যা;
    unsigned gid_offset = record_তথ্য.দৈর্ঘ্য;
    unsigned gid_end = hb_unsigned_add_saturate (gid_offset, gid_আকার);
    যদি (unlikely (gid_end == UINT_MAX ||
		  !record_তথ্য.resize_dirty (gid_end)))
      ফেরত মিথ্যা;
    unsigned অক্ষর *out = record_তথ্য.arrayZ + gid_offset;
    যদি (gid_আকার == HBGlyphID16::static_আকার)
      * (HBGlyphID16 *) out = new_gid;
    অন্যথা
      * (HBGlyphID24 *) out = new_gid;

    unsigned cursor = component.gid_offset + component.gid_আকার;
    যদি (flags & (unsigned) VarComponent::flags_t::HAVE_CONDITION)
    {
      যদি (unlikely (!condition_map.has (component.condition_সূচক) ||
		    !append_bytes (record, component.আকার_2,
				   cursor, component.condition_offset) ||
		    !append_varint (condition_map.get (component.condition_সূচক))))
	ফেরত মিথ্যা;
      cursor = component.condition_offset + component.condition_আকার;
    }
    যদি (flags & (unsigned) VarComponent::flags_t::HAVE_AXES)
    {
      যদি (unlikely (!axis_indices_map.has (component.axis_indices_সূচক) ||
		    !append_bytes (record, component.আকার_2,
				   cursor, component.axis_indices_offset) ||
		    !append_varint (axis_indices_map.get (component.axis_indices_সূচক))))
	ফেরত মিথ্যা;
      cursor = component.axis_indices_offset + component.axis_indices_আকার;
    }
    যদি (flags & (unsigned) VarComponent::flags_t::AXIS_মানসমূহ_HAVE_VARIATION)
    {
      uint32_t var_idx = component.axis_মানসমূহ_var_idx;
      যদি (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.আকার_2,
				   cursor, component.axis_মানসমূহ_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	ফেরত মিথ্যা;
      cursor = component.axis_মানসমূহ_var_offset + component.axis_মানসমূহ_var_আকার;
    }
    যদি (flags & (unsigned) VarComponent::flags_t::TRANSFORM_HAS_VARIATION)
    {
      uint32_t var_idx = component.transform_var_idx;
      যদি (unlikely ((var_idx != VarIdx::NO_VARIATION && !varidx_map.has (var_idx)) ||
		    !append_bytes (record, component.আকার_2,
				   cursor, component.transform_var_offset) ||
		    !append_varint (var_idx == VarIdx::NO_VARIATION ?
				    var_idx : varidx_map.get (var_idx))))
	ফেরত মিথ্যা;
      cursor = component.transform_var_offset + component.transform_var_আকার;
    }

    ফেরত append_bytes (record, component.আকার_2, cursor, component.আকার_2);
  }
};

যৌক্তিক
VARC::subset (hb_subset_context_t *c) ধ্রুবক
{
  TRACE_SUBSET (এই);
  auto fail = [&] ()
  {
    c->serializer->err (HB_SERIALIZE_ERROR_OTHER);
    ফেরত মিথ্যা;
  };

  ধ্রুবক Coverage &source_coverage = এই+coverage;
  ধ্রুবক CFF2_সূচক &source_records = এই+glyphRecords;
  ধ্রুবক ConditionList &source_conditions = এই+conditionList;
  ধ্রুবক TupleList &source_axis_indices = এই+axisIndicesList;
  ধ্রুবক CFF2_সূচক &source_axis_indices_সূচক = source_axis_indices;
  ধ্রুবক MultiItemVariationStore &source_var_store = এই+varStore;
  varc_subset_plan_t subset_plan (*এই, c,
				  source_coverage,
				  source_records,
				  source_conditions,
				  source_axis_indices_সূচক,
				  source_var_store);
  যদি (unlikely (!subset_plan.collect_glyphs ()))
    return_trace (fail ());
  যদি (!subset_plan.old_indices)
    return_trace (মিথ্যা);
  যদি (unlikely (!subset_plan.collect_indices () ||
		!subset_plan.create_maps () ||
		!subset_plan.compile_records ()))
    return_trace (fail ());

  VARC *out = c->serializer->start_embed<VARC> ();
  যদি (unlikely (!out || !c->serializer->extend_min (out)))
    return_trace (মিথ্যা);
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
							&subset_plan.axis_indices_তথ্য_আকার)) ||
		(subset_plan.var_indices.get_population () &&
		 !out->varStore.serialize_serialize (c->serializer,
						  &source_var_store,
						  subset_plan.var_inner_maps.as_array (),
						  subset_plan.axis_map)) ||
		!out->glyphRecords.serialize_serialize (c->serializer,
						 subset_plan.records.iter (),
						 &subset_plan.তথ্য_আকার)))
    return_trace (মিথ্যা);

  return_trace (সত্য);
}

} /* namespace OT */

#endif /* OT_VAR_VARC_VARC_SUBSET_HH */
