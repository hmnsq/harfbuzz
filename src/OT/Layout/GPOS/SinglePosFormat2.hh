#ifndef OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#define OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
ସଂରଚନା single_pos_ମୂଲ୍ୟ_mapper_t
{
  ଅଚଳ hb_map_t &glyph_map;
  ଅଚଳ hb_array_t<ଅଚଳ ମୂଲ୍ୟ> &values;
  ଅଚଳ unsigned &sub_length;

  hb_pair_t<hb_codepoint_t, hb_array_t<ଅଚଳ ମୂଲ୍ୟ>>
  operator () (ଅଚଳ hb_pair_t<hb_codepoint_t, unsigned> &_) ଅଚଳ
  {
    ଫେରନ୍ତୁ hb_pair (glyph_map[_.first],
                    values.sub_array (_.second * sub_length, sub_length));
  }
};

template <typename Types>
ସଂରଚନା SinglePosFormat2_4 : ମୂଲ୍ୟ_Base
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ମୂଲ୍ୟ_Format   ମୂଲ୍ୟ_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  typename Types::HBUINT
                ମୂଲ୍ୟ_ଗଣନା;             /* Number of ValueRecords */
  ମୂଲ୍ୟ_Record   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + ମୂଲ୍ୟ_Format::static_size + Types::HBUINT::static_size, values);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
                  coverage.sanitize (c, ଏହି) &&
                  ମୂଲ୍ୟ_Format_2.sanitize_ମୂଲ୍ୟଗୁଡ଼ିକ (c, ଏହି, values, ମୂଲ୍ୟ_ଗଣନା));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+coverage).intersects (glyphs); }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}
  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ଯଦି (!ମୂଲ୍ୟ_Format_2.has_device ()) ଫେରନ୍ତୁ;

    auto it =
    + hb_zip (ଏହି+coverage, hb_range ((unsigned) ମୂଲ୍ୟ_ଗଣନା))
    | hb_filter (c->glyph_set, hb_first)
    ;

    ଯଦି (!it) ଫେରନ୍ତୁ;

    unsigned sub_length = ମୂଲ୍ୟ_Format_2.get_len ();
    ଅଚଳ hb_array_t<ଅଚଳ ମୂଲ୍ୟ> ମୂଲ୍ୟଗୁଡ଼ିକ_array = values.as_array (ମୂଲ୍ୟ_ଗଣନା * sub_length);

    ପାଇଁ (unsigned i : + it
                      | hb_map (hb_second))
      ମୂଲ୍ୟ_Format_2.collect_variation_indices (c, ଏହି, ମୂଲ୍ୟଗୁଡ଼ିକ_array.sub_array (i * sub_length, sub_length));

  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  { ଯଦି (unlikely (!(ଏହି+coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ; }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ମୂଲ୍ୟ_Format get_ମୂଲ୍ୟ_format () ଅଚଳ { ଫେରନ୍ତୁ ମୂଲ୍ୟ_Format_2; }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    hb_buffer_t *buffer = c->buffer;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage  (buffer->cur().codepoint);
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (index >= ମୂଲ୍ୟ_ଗଣନା)) return_trace (ମିଥ୍ୟା);

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioning glyph at %u",
			  c->buffer->idx);
    }

    ମୂଲ୍ୟ_Format_2.apply_ମୂଲ୍ୟ (c, ଏହି,
                             &values[index * ମୂଲ୍ୟ_Format_2.get_len ()],
                             buffer->cur_pos());

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "positioned glyph at %u",
			  c->buffer->idx);
    }

    buffer->idx++;
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ
  position_single (hb_font_t           *font,
		   hb_blob_t           *table_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage  (gid);
    ଯଦି (likely (index == NOT_COVERED)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (unlikely (index >= ମୂଲ୍ୟ_ଗଣନା)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ମୂଲ୍ୟ_Format_2.position_single (font, table_blob, direction, ଏହି,
                                 &values[index * ମୂଲ୍ୟ_Format_2.get_len ()], pos);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }


  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize (hb_serialize_context_t *c,
                  ଅଚଳ ମୂଲ୍ୟ_Base *src,
                  ମୂଲ୍ୟ_Format src_format,
                  Iterator it,
                  ମୂଲ୍ୟ_Format newFormat,
                  ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *layout_variation_idx_delta_map)
  {
    auto out = c->extend_min (ଏହି);
    ଯଦି (unlikely (!out)) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (!c->check_assign (ମୂଲ୍ୟ_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_OVERFLOW))) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (!c->check_assign (ମୂଲ୍ୟ_ଗଣନା, it.len (), HB_SERIALIZE_ERROR_ARRAY_OVERFLOW))) ଫେରନ୍ତୁ;

    + it
    | hb_map (hb_second)
    | hb_apply ([&] (hb_array_t<ଅଚଳ ମୂଲ୍ୟ> _)
    { src_format.copy_ମୂଲ୍ୟଗୁଡ଼ିକ (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned compute_effective_format (ଅଚଳ hb_face_t *face,
                                     Iterator it,
                                     ତାର୍କିକ is_instancing, ତାର୍କିକ strip_hints,
                                     ତାର୍କିକ has_gdef_varstore,
                                     ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *varidx_delta_map) ଅଚଳ
  {
    hb_blob_t* blob = hb_face_reference_table (face, HB_TAG ('f','v','a','r'));
    ତାର୍କିକ has_fvar = (blob != hb_blob_get_empty ());
    hb_blob_destroy (blob);

    unsigned new_format = 0;
    ଯଦି (is_instancing)
    {
      new_format = new_format | ମୂଲ୍ୟ_Format_2.get_effective_format (+ it | hb_map (hb_second), ମିଥ୍ୟା, ମିଥ୍ୟା, ଏହି, varidx_delta_map);
    }
    /* do not strip hints for VF */
    ଅନ୍ୟଥା ଯଦି (strip_hints)
    {
      ତାର୍କିକ strip = !has_fvar;
      ଯଦି (has_fvar && !has_gdef_varstore)
        strip = ସତ୍ୟ;
      new_format = new_format | ମୂଲ୍ୟ_Format_2.get_effective_format (+ it | hb_map (hb_second), strip, ସତ୍ୟ, ଏହି, nullptr);
    }
    ଅନ୍ୟଥା
      new_format = ମୂଲ୍ୟ_Format_2;

    ଫେରନ୍ତୁ new_format;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    unsigned sub_length = ମୂଲ୍ୟ_Format_2.get_len ();
    auto ମୂଲ୍ୟଗୁଡ଼ିକ_array = values.as_array ((unsigned) ମୂଲ୍ୟ_ଗଣନା * sub_length);

    auto it =
    + hb_zip (ଏହି+coverage, hb_range ((unsigned) ମୂଲ୍ୟ_ଗଣନା))
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting (single_pos_ମୂଲ୍ୟ_mapper_t {glyph_map,
                                                       ମୂଲ୍ୟଗୁଡ଼ିକ_array,
                                                       sub_length})
    ;

    unsigned new_format = compute_effective_format (c->plan->source, it,
                                                    ତାର୍କିକ (c->plan->normalized_coords),
                                                    ତାର୍କିକ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    ତାର୍କିକ ret = ତାର୍କିକ (it);
    SinglePos_serialize (c->serializer, ଏହି, ମୂଲ୍ୟ_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
