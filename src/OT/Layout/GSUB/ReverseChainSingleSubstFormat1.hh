#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

template <typename Types>
संरचना ReverseChainSingleSubstFormat1_2
{
  रक्षित:
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
  सार्वजनिक:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (!(coverage.sanitize (c, एतत्) && backtrack.sanitize (c, एतत्)))
      return_trace (मिच्छ);
    hb_barrier ();
    ध्रुव auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    जइ (!lookahead.sanitize (c, एतत्))
      return_trace (मिच्छ);
    hb_barrier ();
    ध्रुव auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    जइ (!(एतत्+coverage).intersects (glyphs))
      प्रत्यागम मिच्छ;

    ध्रुव auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned पूर्णाङ्क गणनम्;

    गणनम् = backtrack.len;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      जइ (!(एतत्+backtrack[i]).intersects (glyphs))
        प्रत्यागम मिच्छ;

    गणनम् = lookahead.len;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      जइ (!(एतत्+lookahead[i]).intersects (glyphs))
        प्रत्यागम मिच्छ;

    प्रत्यागम सच्च;
  }

  तार्किक may_have_non_1to1 () ध्रुव
  { प्रत्यागम मिच्छ; }

  शून्य depend (hb_depend_context_t *c) ध्रुव {
    // Filter by intersects and parent_active_glyphs like closure does
    जइ (!intersects (c->glyphs)) प्रत्यागम;

    ध्रुव auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ध्रुव auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned पूर्णाङ्क गणनम् = backtrack.len;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      hb_set_t back_glyphs;
      (एतत्+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      जइ (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      अण्णहा जइ (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        जइ (unlikely (set_idx == HB_CODEPOINT_INVALID))
          प्रत्यागम;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    गणनम् = lookahead.len;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      hb_set_t look_glyphs;
      (एतत्+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      जइ (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      अण्णहा जइ (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        जइ (unlikely (set_idx == HB_CODEPOINT_INVALID))
          प्रत्यागम;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    जइ (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      प्रत्यागम;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    जइ (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      प्रत्यागम;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (एतत्+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ध्रुव hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    जइ (!intersects (c->glyphs)) प्रत्यागम;

    ध्रुव auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ध्रुव auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (एतत्+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    जइ (unlikely (!(एतत्+coverage).collect_coverage (c->निवेशः))) प्रत्यागम;

    unsigned पूर्णाङ्क गणनम्;

    गणनम् = backtrack.len;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      जइ (unlikely (!(एतत्+backtrack[i]).collect_coverage (c->before))) प्रत्यागम;

    ध्रुव auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    गणनम् = lookahead.len;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      जइ (unlikely (!(एतत्+lookahead[i]).collect_coverage (c->after))) प्रत्यागम;

    ध्रुव auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    गणनम् = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  { प्रत्यागम c->len == 1 && (एतत्+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->buffer->cur ().codepoint);
    जइ (index == NOT_COVERED) return_trace (मिच्छ);

    जइ (unlikely (c->nesting_level_वामः != HB_MAX_NESTING_LEVEL))
      return_trace (मिच्छ); /* No chaining to this type */

    ध्रुव auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ध्रुव auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    जइ (unlikely (index >= substitute.len)) return_trace (मिच्छ);

    unsigned पूर्णाङ्क start_index = 0, end_index = 0;
    जइ (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, एतत्,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, एतत्,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (सच्च);
    }
    अण्णहा
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (मिच्छ);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    जइ (unlikely (!out->serialize (c->serializer, 0u))) return_trace (मिच्छ);

    प्रति (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      जइ (unlikely (!o) || !o->serialize_subset (c, offset, एतत्))
        return_trace (मिच्छ);
    }

    return_trace (सच्च);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  तार्किक serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    auto *out = c->serializer->start_embed (एतत्);
    जइ (unlikely (!c->serializer->embed (एतत्->format))) return_trace (मिच्छ);
    जइ (unlikely (!c->serializer->embed (एतत्->coverage))) return_trace (मिच्छ);

    जइ (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (मिच्छ);
    जइ (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (मिच्छ);

    auto *substitute_out = c->serializer->start_embed<typename Types::template ArrayOf<typename Types::HBGlyphID>> ();
    auto substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    auto glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    जइ (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (मिच्छ);

    जइ (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (मिच्छ);
    return_trace (सच्च);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    ध्रुव auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ध्रुव auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (एतत्+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ध्रुव typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { प्रत्यागम hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (तार्किक (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    जइ (unlikely (!out->serialize (c->serializer, 0u))) return_trace (मिच्छ);

    प्रति (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      जइ (unlikely (!o) || !o->serialize_subset (c, offset, एतत्))
        return_trace (मिच्छ);
    }

    return_trace (सच्च);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
