#ifndef OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#અન્તર્ભાવ "Common.hh"
#અન્તર્ભાવ "Sequence.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

template <typename Types>
સંરચના MultipleSubstFormat1_2
{
  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, sequence);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ) && sequence.sanitize (c, આ));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  { પાછા (આ+coverage).intersects (glyphs); }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા સાચું; }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    + hb_zip (આ+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_apply ([c] (અચલ Sequence<Types> &_) { _.closure (c); })
    ;
  }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (આ+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (અચલ hb_pair_t<hb_codepoint_t, અચલ typename Types::template OffsetTo<Sequence<Types>>&> &_)
                {
                  અચલ Sequence<Types>& s = આ+_.second;
                  s.depend (c, _.first);
                })
    ;
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    જો (unlikely (!(આ+coverage).collect_coverage (c->આગત))) પાછા;
    + hb_zip (આ+coverage, sequence)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_apply ([c] (અચલ Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  { પાછા c->len == 1 && (આ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (c->buffer->cur().codepoint);
    જો (index == NOT_COVERED) return_trace (ખોટું);

    return_trace ((આ+sequence[index]).apply (c));
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (આ);
    auto sequences =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);

    જો (unlikely (!sequence.serialize (c, sequences.length))) return_trace (ખોટું);

    માટે (auto& pair : hb_zip (sequences, sequence))
    {
      જો (unlikely (!pair.second
		    .serialize_serialize (c, pair.first)))
        return_trace (ખોટું);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (આ+coverage, sequence)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->sequence, આ), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (તાર્કિક (new_coverage));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
