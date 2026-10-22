#ifndef OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#அந்தர்பாவ "Common.hh"
#அந்தர்பாவ "Sequence.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

template <typename Types>
கட்டமைப்பு MultipleSubstFormat1_2
{
  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, sequence);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (coverage.sanitize (c, இது) && sequence.sanitize (c, இது));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  { திரும்பு (இது+coverage).intersects (glyphs); }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு உண்மை; }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    + hb_zip (இது+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (இது))
    | hb_apply ([c] (மாறிலி Sequence<Types> &_) { _.closure (c); })
    ;
  }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (இது+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (மாறிலி hb_pair_t<hb_codepoint_t, மாறிலி typename Types::template OffsetTo<Sequence<Types>>&> &_)
                {
                  மாறிலி Sequence<Types>& s = இது+_.second;
                  s.depend (c, _.first);
                })
    ;
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    எனில் (unlikely (!(இது+coverage).collect_coverage (c->உள்ளீடு))) திரும்பு;
    + hb_zip (இது+coverage, sequence)
    | hb_map (hb_second)
    | hb_map (hb_add (இது))
    | hb_apply ([c] (மாறிலி Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  { திரும்பு c->len == 1 && (இது+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    unsigned முழு index = (இது+coverage).get_coverage (c->buffer->cur().codepoint);
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    return_trace ((இது+sequence[index]).apply (c));
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  தருக்க serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (இது);
    auto sequences =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);

    எனில் (unlikely (!sequence.serialize (c, sequences.length))) return_trace (பொய்);

    க்கு (auto& pair : hb_zip (sequences, sequence))
    {
      எனில் (unlikely (!pair.second
		    .serialize_serialize (c, pair.first)))
        return_trace (பொய்);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (இது+coverage, sequence)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->sequence, இது), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (தருக்க (new_coverage));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
