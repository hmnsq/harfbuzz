#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#அந்தர்பாவ "Common.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
கட்டமைப்பு single_pos_மதிப்பு_mapper_t
{
  மாறிலி hb_map_t &glyph_map;
  மாறிலி hb_array_t<மாறிலி மதிப்பு> &values;
  மாறிலி unsigned &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<மாறிலி மதிப்பு>>
  operator () (மாறிலி hb_pair_t<hb_codepoint_t, unsigned> &_) மாறிலி
  {
    திரும்பு hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

template <typename Types>
கட்டமைப்பு SinglePosFormat2_4 : மதிப்பு_Base
{
  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  மதிப்பு_Format   மதிப்பு_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                மதிப்பு_மொத்தம்;             /* Number of ValueRecords */
  மதிப்பு_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + மதிப்பு_Format::static_size + Types::HBUINT::static_size, values);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
                  coverage.sanitize (c, இது) &&
                  மதிப்பு_Format_2.sanitize_மதிப்புகள் (c, இது, values, மதிப்பு_மொத்தம்));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  { திரும்பு (இது+coverage).intersects (glyphs); }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}
  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    எனில் (!மதிப்பு_Format_2.has_device ()) திரும்பு;

    auto it =
    + hb_zip (இது+coverage, hb_range ((unsigned) மதிப்பு_மொத்தம்))
    | hb_filter (c->glyph_set, hb_first)
    ;

    எனில் (!it) திரும்பு;

    unsigned sub_length = மதிப்பு_Format_2.get_len ();
    மாறிலி hb_array_t<மாறிலி மதிப்பு> மதிப்புகள்_array = values.as_array (மதிப்பு_மொத்தம் * sub_length);

    க்கு (unsigned i : + it
                      | hb_map (hb_second))
      மதிப்பு_Format_2.collect_variation_indices (c, இது, மதிப்புகள்_array.sub_array (i * sub_length, sub_length));

  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  { எனில் (unlikely (!(இது+coverage).collect_coverage (c->உள்ளீடு))) திரும்பு; }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  மதிப்பு_Format get_மதிப்பு_format () மாறிலி { திரும்பு மதிப்பு_Format_2; }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    hb_buffer_t *buffer = c->buffer;
    unsigned முழு index = (இது+coverage).get_coverage  (buffer->cur().codepoint);
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    எனில் (unlikely (index >= மதிப்பு_மொத்தம்)) return_trace (பொய்);

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    மதிப்பு_Format_2.apply_மதிப்பு (c, இது,
                             &values[index * மதிப்பு_Format_2.get_len ()],
                             buffer->cur_pos());

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (உண்மை);
  }

  தருக்க
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) மாறிலி
  {
    unsigned முழு index = (இது+coverage).get_coverage  (gid);
    எனில் (likely (index == NOT_COVERED)) திரும்பு பொய்;
    எனில் (unlikely (index >= மதிப்பு_மொத்தம்)) திரும்பு பொய்;

    மதிப்பு_Format_2.position_single (font, table_blob, direction, இது,
                                 &values[index * மதிப்பு_Format_2.get_len ()], pos);
    திரும்பு உண்மை;
  }


  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize (hb_serialize_context_t *c,
                  மாறிலி மதிப்பு_Base *src,
                  மதிப்பு_Format src_format,
                  Iterator it,
                  மதிப்பு_Format newFormat,
                  மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map)
  {
    auto out = c->extend_min (இது);
    எனில் (unlikely (!out)) திரும்பு;
    எனில் (unlikely (!c->check_assign (மதிப்பு_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) திரும்பு;
    எனில் (unlikely (!c->check_assign (மதிப்பு_மொத்தம், it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) திரும்பு;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<மாறிலி மதிப்பு> _)
    { src_format.copy_மதிப்புகள் (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned compute_effective_format (மாறிலி hb_face_t *face,
                                     Iterator it,
                                     தருக்க is_instancing, தருக்க strip_hints,
                                     தருக்க has_gdef_varstore,
                                     மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *varidx_delta_map) மாறிலி
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    தருக்க has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    unsigned new_format = 0;
    எனில் (is_instancing)
    {
      new_format = new_format | மதிப்பு_Format_2.get_effective_format (+ it | hb_map (hb_second), பொய், பொய், இது, varidx_delta_map);
    }
    /* do not strip hints for VF */
    இல்லையெனில் எனில் (strip_hints)
    {
      தருக்க strip = !has_fvar;
      எனில் (has_fvar && !has_gdef_varstore)
        strip = உண்மை;
      new_format = new_format | மதிப்பு_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, உண்மை, இது, nullptr);
    }
    இல்லையெனில்
      new_format = மதிப்பு_Format_2;

    திரும்பு new_format;
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_length = மதிப்பு_Format_2.get_len ();
    auto மதிப்புகள்_array = values.as_array ((unsigned) மதிப்பு_மொத்தம் * sub_length);

    auto it =
    + hb_zip (இது+coverage, hb_range ((unsigned) மதிப்பு_மொத்தம்))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_மதிப்பு_mapper_t {glyph_map,
                                                       மதிப்புகள்_array,
                                                       sub_length})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    தருக்க (c->plan->normalized_coords),
                                                    தருக்க (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    தருக்க ret = தருக்க (it);
    SinglePos_serialize (c->serializer, இது, மதிப்பு_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
