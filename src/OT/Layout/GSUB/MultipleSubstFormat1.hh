#ifndef OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "Sequence.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

template <typename Types>
ସଂରଚନା MultipleSubstFormat1_2
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, sequence);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (coverage.sanitize (c, ଏହି) && sequence.sanitize (c, ଏହି));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+coverage).intersects (glyphs); }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ସତ୍ୟ; }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    + hb_zip (ଏହି+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([c] (ଅଚଳ Sequence<Types> &_) { _.closure (c); })
    ;
  }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (ଏହି+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ଅଚଳ hb_pair_t<hb_codepoint_t, ଅଚଳ typename Types::template OffsetTo<Sequence<Types>>&> &_)
                {
                  ଅଚଳ Sequence<Types>& s = ଏହି+_.second;
                  s.depend (c, _.first);
                })
    ;
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    ଯଦି (unlikely (!(ଏହି+coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
    + hb_zip (ଏହି+coverage, sequence)
    | hb_map (hb_second)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([c] (ଅଚଳ Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ c->len == 1 && (ଏହି+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (c->buffer->cur().codepoint);
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    return_trace ((ଏହି+sequence[index]).apply (c));
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ଏହି);
    auto sequences =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!sequence.serialize (c, sequences.length))) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (auto& pair : hb_zip (sequences, sequence))
    {
      ଯଦି (unlikely (!pair.second
		    .serialize_serialize (c, pair.first)))
        return_trace (ମିଥ୍ୟା);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ଏହି+coverage, sequence)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->sequence, ଏହି), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ତାର୍କିକ (new_coverage));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
