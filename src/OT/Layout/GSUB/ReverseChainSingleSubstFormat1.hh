#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#അന്തര്ഭാവ "Common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

template <typename Types>
ഘടന ReverseChainSingleSubstFormat1_2
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of table */
  Array16Of<typename Types::template OffsetTo<Coverage>>
                backtrack;              /* Array of coverage tables
                                         * in backtracking sequence, in glyph
                                         * sequence order */
  /* Format 2 widens the backtrack and lookahead coverage offsets but
   * keeps their counts 16-bit. */
  Array16Of<typename Types::template OffsetTo<Coverage>>
                lookaheadX;             /* Array of coverage tables
                                         * in lookahead sequence, in glyph
                                         * sequence order */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substituteX;            /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */
  പൊതു:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!(coverage.sanitize (c, ഇത്) && backtrack.sanitize (c, ഇത്)))
      return_trace (അസത്യം);
    hb_barrier ();
    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    എങ്കിൽ (!lookahead.sanitize (c, ഇത്))
      return_trace (അസത്യം);
    hb_barrier ();
    സ്ഥിരം auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    എങ്കിൽ (!(ഇത്+coverage).intersects (glyphs))
      തിരികെ അസത്യം;

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned പൂർണ്ണസംഖ്യ എണ്ണം;

    എണ്ണം = backtrack.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!(ഇത്+backtrack[i]).intersects (glyphs))
        തിരികെ അസത്യം;

    എണ്ണം = lookahead.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!(ഇത്+lookahead[i]).intersects (glyphs))
        തിരികെ അസത്യം;

    തിരികെ സത്യം;
  }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ അസത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം {
    // Filter by intersects and parent_active_glyphs like closure does
    എങ്കിൽ (!intersects (c->glyphs)) തിരികെ;

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    സ്ഥിരം auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = backtrack.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      hb_set_t back_glyphs;
      (ഇത്+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      എങ്കിൽ (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      അല്ലെങ്കിൽ എങ്കിൽ (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        എങ്കിൽ (unlikely (set_idx == HB_CODEPOINT_INVALID))
          തിരികെ;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    എണ്ണം = lookahead.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      hb_set_t look_glyphs;
      (ഇത്+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      എങ്കിൽ (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      അല്ലെങ്കിൽ എങ്കിൽ (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        എങ്കിൽ (unlikely (set_idx == HB_CODEPOINT_INVALID))
          തിരികെ;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    എങ്കിൽ (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      തിരികെ;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    എങ്കിൽ (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      തിരികെ;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (ഇത്+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (സ്ഥിരം hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!intersects (c->glyphs)) തിരികെ;

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    സ്ഥിരം auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (ഇത്+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(ഇത്+coverage).collect_coverage (c->നിവേശം))) തിരികെ;

    unsigned പൂർണ്ണസംഖ്യ എണ്ണം;

    എണ്ണം = backtrack.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (unlikely (!(ഇത്+backtrack[i]).collect_coverage (c->before))) തിരികെ;

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    എണ്ണം = lookahead.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (unlikely (!(ഇത്+lookahead[i]).collect_coverage (c->after))) തിരികെ;

    സ്ഥിരം auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    എണ്ണം = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  { തിരികെ c->len == 1 && (ഇത്+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (c->buffer->cur ().codepoint);
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    എങ്കിൽ (unlikely (c->nesting_level_ഇടത് != HB_MAX_NESTING_LEVEL))
      return_trace (അസത്യം); /* No chaining to this type */

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    സ്ഥിരം auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    എങ്കിൽ (unlikely (index >= substitute.len)) return_trace (അസത്യം);

    unsigned പൂർണ്ണസംഖ്യ start_index = 0, end_index = 0;
    എങ്കിൽ (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, ഇത്,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, ഇത്,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (സത്യം);
    }
    അല്ലെങ്കിൽ
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (അസത്യം);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  തർക്കം serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    എങ്കിൽ (unlikely (!out->serialize (c->serializer, 0u))) return_trace (അസത്യം);

    വേണ്ടി (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!o) || !o->serialize_subset (c, offset, ഇത്))
        return_trace (അസത്യം);
    }

    return_trace (സത്യം);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  തർക്കം serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->embed (ഇത്->format))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->serializer->embed (ഇത്->coverage))) return_trace (അസത്യം);

    എങ്കിൽ (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (അസത്യം);
    എങ്കിൽ (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (അസത്യം);

    auto *substitute_out = c->serializer->start_embed<typename Types::template ArrayOf<typename Types::HBGlyphID>> ();
    auto substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    auto glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    എങ്കിൽ (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    സ്ഥിരം auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    സ്ഥിരം auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (ഇത്+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, സ്ഥിരം typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { തിരികെ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (തർക്കം (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  തർക്കം serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    എങ്കിൽ (unlikely (!out->serialize (c->serializer, 0u))) return_trace (അസത്യം);

    വേണ്ടി (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!o) || !o->serialize_subset (c, offset, ഇത്))
        return_trace (അസത്യം);
    }

    return_trace (സത്യം);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
