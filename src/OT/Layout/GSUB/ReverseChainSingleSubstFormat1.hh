#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#અન્તર્ભાવ "Common.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

template <typename Types>
સંરચના ReverseChainSingleSubstFormat1_2
{
  સુરક્ષિત:
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
  જાહેર:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!(coverage.sanitize (c, આ) && backtrack.sanitize (c, આ)))
      return_trace (ખોટું);
    hb_barrier ();
    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    જો (!lookahead.sanitize (c, આ))
      return_trace (ખોટું);
    hb_barrier ();
    અચલ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    જો (!(આ+coverage).intersects (glyphs))
      પાછા ખોટું;

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned પૂર્ણાંક ગણતરી;

    ગણતરી = backtrack.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (!(આ+backtrack[i]).intersects (glyphs))
        પાછા ખોટું;

    ગણતરી = lookahead.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (!(આ+lookahead[i]).intersects (glyphs))
        પાછા ખોટું;

    પાછા સાચું;
  }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા ખોટું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ {
    // Filter by intersects and parent_active_glyphs like closure does
    જો (!intersects (c->glyphs)) પાછા;

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    અચલ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned પૂર્ણાંક ગણતરી = backtrack.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      hb_set_t back_glyphs;
      (આ+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      જો (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      નહીંતર જો (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        જો (unlikely (set_idx == HB_CODEPOINT_INVALID))
          પાછા;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    ગણતરી = lookahead.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      hb_set_t look_glyphs;
      (આ+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      જો (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      નહીંતર જો (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        જો (unlikely (set_idx == HB_CODEPOINT_INVALID))
          પાછા;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    જો (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      પાછા;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    જો (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      પાછા;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (આ+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (અચલ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    જો (!intersects (c->glyphs)) પાછા;

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    અચલ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (આ+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    જો (unlikely (!(આ+coverage).collect_coverage (c->આગત))) પાછા;

    unsigned પૂર્ણાંક ગણતરી;

    ગણતરી = backtrack.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (unlikely (!(આ+backtrack[i]).collect_coverage (c->before))) પાછા;

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ગણતરી = lookahead.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (unlikely (!(આ+lookahead[i]).collect_coverage (c->after))) પાછા;

    અચલ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    ગણતરી = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  { પાછા c->len == 1 && (આ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (c->buffer->cur ().codepoint);
    જો (index == NOT_COVERED) return_trace (ખોટું);

    જો (unlikely (c->nesting_level_ડાબું != HB_MAX_NESTING_LEVEL))
      return_trace (ખોટું); /* No chaining to this type */

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    અચલ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    જો (unlikely (index >= substitute.len)) return_trace (ખોટું);

    unsigned પૂર્ણાંક start_index = 0, end_index = 0;
    જો (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, આ,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, આ,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (સાચું);
    }
    નહીંતર
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (ખોટું);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    જો (unlikely (!out->serialize (c->serializer, 0u))) return_trace (ખોટું);

    માટે (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      જો (unlikely (!o) || !o->serialize_subset (c, offset, આ))
        return_trace (ખોટું);
    }

    return_trace (સાચું);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  તાર્કિક serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) અચલ
  {
    TRACE_SERIALIZE (આ);

    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!c->serializer->embed (આ->format))) return_trace (ખોટું);
    જો (unlikely (!c->serializer->embed (આ->coverage))) return_trace (ખોટું);

    જો (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (ખોટું);
    જો (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (ખોટું);

    auto *substitute_out = c->serializer->start_embed<typename Types::template ArrayOf<typename Types::HBGlyphID>> ();
    auto substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    auto glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    જો (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (ખોટું);

    જો (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (ખોટું);
    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    અચલ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    અચલ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (આ+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, અચલ typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { પાછા hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (તાર્કિક (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    જો (unlikely (!out->serialize (c->serializer, 0u))) return_trace (ખોટું);

    માટે (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      જો (unlikely (!o) || !o->serialize_subset (c, offset, આ))
        return_trace (ખોટું);
    }

    return_trace (સાચું);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
