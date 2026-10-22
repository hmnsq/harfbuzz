#ifndef OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "Sequence.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

template <typename Types>
संरचना MultipleSubstFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, sequence);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && sequence.sanitize (c, एतत्));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम (एतत्+coverage).intersects (glyphs); }

  तार्किक may_have_non_1to1 () ध्रुव
  { प्रत्यागम सच्च; }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    + hb_zip (एतत्+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (ध्रुव Sequence<Types> &_) { _.closure (c); })
    ;
  }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (एतत्+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ध्रुव hb_pair_t<hb_codepoint_t, ध्रुव typename Types::template OffsetTo<Sequence<Types>>&> &_)
                {
                  ध्रुव Sequence<Types>& s = एतत्+_.second;
                  s.depend (c, _.first);
                })
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    जइ (unlikely (!(एतत्+coverage).collect_coverage (c->निवेशः))) प्रत्यागम;
    + hb_zip (एतत्+coverage, sequence)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (ध्रुव Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  { प्रत्यागम c->len == 1 && (एतत्+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->buffer->cur().codepoint);
    जइ (index == NOT_COVERED) return_trace (मिच्छ);

    return_trace ((एतत्+sequence[index]).apply (c));
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (एतत्);
    auto sequences =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    जइ (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छ);

    जइ (unlikely (!sequence.serialize (c, sequences.length))) return_trace (मिच्छ);

    प्रति (auto& pair : hb_zip (sequences, sequence))
    {
      जइ (unlikely (!pair.second
		    .serialize_serialize (c, pair.first)))
        return_trace (मिच्छ);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+coverage, sequence)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->sequence, एतत्), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (तार्किक (new_coverage));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
