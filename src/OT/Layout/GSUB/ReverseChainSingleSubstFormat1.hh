#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

template <typename Types>
ಸಂರಚನಾ ReverseChainSingleSubstFormat1_2
{
  ರಕ್ಷಿತ:
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!(coverage.sanitize (c, ಏತತ್) && backtrack.sanitize (c, ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ಯದಿ (!lookahead.sanitize (c, ಏತತ್))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಧ್ರುವ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಯದಿ (!(ಏತತ್+coverage).intersects (glyphs))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ;

    ಎಣಿಕೆ = backtrack.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!(ಏತತ್+backtrack[i]).intersects (glyphs))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಎಣಿಕೆ = lookahead.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!(ಏತತ್+lookahead[i]).intersects (glyphs))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ {
    // Filter by intersects and parent_active_glyphs like closure does
    ಯದಿ (!intersects (c->glyphs)) ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ಧ್ರುವ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = backtrack.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      hb_set_t back_glyphs;
      (ಏತತ್+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      ಯದಿ (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      ಅನ್ಯಥಾ ಯದಿ (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        ಯದಿ (unlikely (set_idx == HB_CODEPOINT_INVALID))
          ಪ್ರತ್ಯಾಗಮ;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    ಎಣಿಕೆ = lookahead.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      hb_set_t look_glyphs;
      (ಏತತ್+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      ಯದಿ (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      ಅನ್ಯಥಾ ಯದಿ (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        ಯದಿ (unlikely (set_idx == HB_CODEPOINT_INVALID))
          ಪ್ರತ್ಯಾಗಮ;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    ಯದಿ (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      ಪ್ರತ್ಯಾಗಮ;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    ಯದಿ (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      ಪ್ರತ್ಯಾಗಮ;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (ಏತತ್+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ಧ್ರುವ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!intersects (c->glyphs)) ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ಧ್ರುವ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (ಏತತ್+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;

    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ;

    ಎಣಿಕೆ = backtrack.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (unlikely (!(ಏತತ್+backtrack[i]).collect_coverage (c->before))) ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ಎಣಿಕೆ = lookahead.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (unlikely (!(ಏತತ್+lookahead[i]).collect_coverage (c->after))) ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    ಎಣಿಕೆ = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ c->len == 1 && (ಏತತ್+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (c->buffer->cur ().codepoint);
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (c->nesting_level_ಎಡ != HB_MAX_NESTING_LEVEL))
      return_trace (ಅಸತ್ಯ); /* No chaining to this type */

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ಧ್ರುವ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    ಯದಿ (unlikely (index >= substitute.len)) return_trace (ಅಸತ್ಯ);

    unsigned ಪೂರ್ಣಾಙ್ಕ start_index = 0, end_index = 0;
    ಯದಿ (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, ಏತತ್,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, ಏತತ್,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (ಸತ್ಯ);
    }
    ಅನ್ಯಥಾ
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (ಅಸತ್ಯ);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    ಯದಿ (unlikely (!out->serialize (c->serializer, 0u))) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      ಯದಿ (unlikely (!o) || !o->serialize_subset (c, offset, ಏತತ್))
        return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  ತಾರ್ಕಿಕ serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->embed (ಏತತ್->format))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->serializer->embed (ಏತತ್->coverage))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (ಅಸತ್ಯ);

    auto *substitute_out = c->serializer->start_embed<typename Types::template ArrayOf<typename Types::HBGlyphID>> ();
    auto substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    auto glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    ಯದಿ (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    ಧ್ರುವ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ಧ್ರುವ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (ಏತತ್+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ಧ್ರುವ typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ಪ್ರತ್ಯಾಗಮ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (ತಾರ್ಕಿಕ (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    ಯದಿ (unlikely (!out->serialize (c->serializer, 0u))) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      ಯದಿ (unlikely (!o) || !o->serialize_subset (c, offset, ಏತತ್))
        return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
