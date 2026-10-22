#ifndef OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"
#ಅನ್ತರ್ಭಾವ "Sequence.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

template <typename Types>
ಸಂರಚನಾ MultipleSubstFormat1_2
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, sequence);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್) && sequence.sanitize (c, ಏತತ್));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+coverage).intersects (glyphs); }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    + hb_zip (ಏತತ್+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([c] (ಧ್ರುವ Sequence<Types> &_) { _.closure (c); })
    ;
  }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (ಏತತ್+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<hb_codepoint_t, ಧ್ರುವ typename Types::template OffsetTo<Sequence<Types>>&> &_)
                {
                  ಧ್ರುವ Sequence<Types>& s = ಏತತ್+_.second;
                  s.depend (c, _.first);
                })
    ;
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
    + hb_zip (ಏತತ್+coverage, sequence)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([c] (ಧ್ರುವ Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ c->len == 1 && (ಏತತ್+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (c->buffer->cur().codepoint);
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    return_trace ((ಏತತ್+sequence[index]).apply (c));
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto sequences =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!sequence.serialize (c, sequences.length))) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (auto& pair : hb_zip (sequences, sequence))
    {
      ಯದಿ (unlikely (!pair.second
		    .serialize_serialize (c, pair.first)))
        return_trace (ಅಸತ್ಯ);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ಏತತ್+coverage, sequence)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->sequence, ಏತತ್), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ತಾರ್ಕಿಕ (new_coverage));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
