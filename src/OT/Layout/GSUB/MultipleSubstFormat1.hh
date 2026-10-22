#ifndef OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#അന്തര്ഭാവ "Common.hh"
#അന്തര്ഭാവ "Sequence.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

template <typename Types>
ഘടന MultipleSubstFormat1_2
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, sequence);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്) && sequence.sanitize (c, ഇത്));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ (ഇത്+coverage).intersects (glyphs); }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ സത്യം; }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    + hb_zip (ഇത്+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([c] (സ്ഥിരം Sequence<Types> &_) { _.closure (c); })
    ;
  }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (ഇത്+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<hb_codepoint_t, സ്ഥിരം typename Types::template OffsetTo<Sequence<Types>>&> &_)
                {
                  സ്ഥിരം Sequence<Types>& s = ഇത്+_.second;
                  s.depend (c, _.first);
                })
    ;
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(ഇത്+coverage).collect_coverage (c->നിവേശം))) തിരികെ;
    + hb_zip (ഇത്+coverage, sequence)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([c] (സ്ഥിരം Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  { തിരികെ c->len == 1 && (ഇത്+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (c->buffer->cur().codepoint);
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    return_trace ((ഇത്+sequence[index]).apply (c));
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  തർക്കം serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ഇത്);
    auto sequences =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!sequence.serialize (c, sequences.length))) return_trace (അസത്യം);

    വേണ്ടി (auto& pair : hb_zip (sequences, sequence))
    {
      എങ്കിൽ (unlikely (!pair.second
		    .serialize_serialize (c, pair.first)))
        return_trace (അസത്യം);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ഇത്+coverage, sequence)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->sequence, ഇത്), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (തർക്കം (new_coverage));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
