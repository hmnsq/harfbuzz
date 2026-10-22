#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
بنية single_pos_value_mapper_t
{
  ثابت hb_map_t &glyph_map;
  ثابت hb_array_t<ثابت Value> &values;
  ثابت unsigned &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<ثابت Value>>
  operator () (ثابت hb_pair_t<hb_codepoint_t, unsigned> &_) ثابت
  {
    ܦܢܐ hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

template <typename Types>
بنية SinglePosFormat2_4 : ValueBase
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ValueFormat   valueFormat;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                valueCount;             /* Number of ValueRecords */
  ValueRecord   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + ValueFormat::static_size + Types::HBUINT::static_size, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  coverage.sanitize (c, هذا) &&
                  valueFormat.sanitize_values (c, هذا, values, valueCount));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  { ܦܢܐ (هذا+coverage).intersects (glyphs); }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    ܐܢ (!valueFormat.has_device ()) ܦܢܐ;

    auto it =
    + hb_zip (هذا+coverage, hb_range ((unsigned) valueCount))
    | hb_filter (c->glyph_set, hb_first)
    ;

    ܐܢ (!it) ܦܢܐ;

    unsigned sub_length = valueFormat.get_len ();
    ثابت hb_array_t<ثابت Value> values_array = values.as_array (valueCount * sub_length);

    لكل (unsigned i : + it
                      | hb_map (hb_second))
      valueFormat.collect_variation_indices (c, هذا, values_array.sub_array (i * sub_length, sub_length));

  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { ܐܢ (unlikely (!(هذا+coverage).collect_coverage (c->input))) ܦܢܐ; }

  ثابت Coverage &get_coverage () ثابت { ܦܢܐ هذا+coverage; }

  ValueFormat get_value_format () ثابت { ܦܢܐ valueFormat; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحيح index = (هذا+coverage).get_coverage  (buffer->cur().codepoint);
    ܐܢ (index == NOT_COVERED) return_trace (ܕܓܠܐ);

    ܐܢ (unlikely (index >= valueCount)) return_trace (ܕܓܠܐ);

    ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    valueFormat.apply_value (c, هذا,
                             &values[index * valueFormat.get_len ()],
                             buffer->cur_pos());

    ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (ܫܪܪܐ);
  }

  منطقي
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ثابت
  {
    unsigned صحيح index = (هذا+coverage).get_coverage  (gid);
    ܐܢ (likely (index == NOT_COVERED)) ܦܢܐ ܕܓܠܐ;
    ܐܢ (unlikely (index >= valueCount)) ܦܢܐ ܕܓܠܐ;

    valueFormat.position_single (font, table_blob, direction, هذا,
                                 &values[index * valueFormat.get_len ()], pos);
    ܦܢܐ ܫܪܪܐ;
  }


  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت ValueBase *src,
                  ValueFormat src_format,
                  Iterator it,
                  ValueFormat newFormat,
                  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map)
  {
    auto out = c->extend_min (هذا);
    ܐܢ (unlikely (!out)) ܦܢܐ;
    ܐܢ (unlikely (!c->check_assign (valueFormat, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) ܦܢܐ;
    ܐܢ (unlikely (!c->check_assign (valueCount, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) ܦܢܐ;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<ثابت Value> _)
    { src_format.copy_values (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned compute_effective_format (ثابت hb_face_t *face,
                                     Iterator it,
                                     منطقي is_instancing, منطقي strip_hints,
                                     منطقي has_gdef_varstore,
                                     ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    منطقي has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    unsigned new_format = 0;
    ܐܢ (is_instancing)
    {
      new_format = new_format | valueFormat.get_effective_format (+ it | hb_map (hb_second), ܕܓܠܐ, ܕܓܠܐ, هذا, varidx_delta_map);
    }
    /* do not strip hints for VF */
    ܐܚܪܢܐ ܐܢ (strip_hints)
    {
      منطقي strip = !has_fvar;
      ܐܢ (has_fvar && !has_gdef_varstore)
        strip = ܫܪܪܐ;
      new_format = new_format | valueFormat.get_effective_format (+ it | hb_map (hb_second), strip, ܫܪܪܐ, هذا, nullptr);
    }
    ܐܚܪܢܐ
      new_format = valueFormat;

    ܦܢܐ new_format;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_length = valueFormat.get_len ();
    auto values_array = values.as_array ((unsigned) valueCount * sub_length);

    auto it =
    + hb_zip (هذا+coverage, hb_range ((unsigned) valueCount))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_value_mapper_t {glyph_map,
                                                       values_array,
                                                       sub_length})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    منطقي (c->plan->normalized_coords),
                                                    منطقي (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    منطقي ret = منطقي (it);
    SinglePos_serialize (c->serializer, هذا, valueFormat, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
