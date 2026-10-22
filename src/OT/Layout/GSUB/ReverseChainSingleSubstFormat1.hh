#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

template <typename Types>
ସଂରଚନା ReverseChainSingleSubstFormat1_2
{
  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!(coverage.sanitize (c, ଏହି) && backtrack.sanitize (c, ଏହି)))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ଯଦି (!lookahead.sanitize (c, ଏହି))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଅଚଳ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଯଦି (!(ଏହି+coverage).intersects (glyphs))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;

    ଗଣନା_2 = backtrack.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!(ଏହି+backtrack[i]).intersects (glyphs))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଗଣନା_2 = lookahead.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!(ଏହି+lookahead[i]).intersects (glyphs))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ମିଥ୍ୟା; }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ {
    // Filter by intersects and parent_active_glyphs like closure does
    ଯଦି (!intersects (c->glyphs)) ଫେରନ୍ତୁ;

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ଅଚଳ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = backtrack.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      hb_set_t back_glyphs;
      (ଏହି+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      ଯଦି (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      ଅନ୍ୟଥା ଯଦି (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        ଯଦି (unlikely (set_idx == HB_CODEPOINT_INVALID))
          ଫେରନ୍ତୁ;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    ଗଣନା_2 = lookahead.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      hb_set_t look_glyphs;
      (ଏହି+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      ଯଦି (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      ଅନ୍ୟଥା ଯଦି (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        ଯଦି (unlikely (set_idx == HB_CODEPOINT_INVALID))
          ଫେରନ୍ତୁ;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    ଯଦି (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      ଫେରନ୍ତୁ;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    ଯଦି (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      ଫେରନ୍ତୁ;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (ଏହି+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ଅଚଳ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    ଯଦି (!intersects (c->glyphs)) ଫେରନ୍ତୁ;

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ଅଚଳ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (ଏହି+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    ଯଦି (unlikely (!(ଏହି+coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;

    ଗଣନା_2 = backtrack.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (unlikely (!(ଏହି+backtrack[i]).collect_coverage (c->before))) ଫେରନ୍ତୁ;

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ଗଣନା_2 = lookahead.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (unlikely (!(ଏହି+lookahead[i]).collect_coverage (c->after))) ଫେରନ୍ତୁ;

    ଅଚଳ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    ଗଣନା_2 = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ c->len == 1 && (ଏହି+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (c->buffer->cur ().codepoint);
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (c->nesting_level_ବାମ != HB_MAX_NESTING_LEVEL))
      return_trace (ମିଥ୍ୟା); /* No chaining to this type */

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ଅଚଳ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    ଯଦି (unlikely (index >= substitute.len)) return_trace (ମିଥ୍ୟା);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_index = 0, end_index = 0;
    ଯଦି (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, ଏହି,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, ଏହି,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (ସତ୍ୟ);
    }
    ଅନ୍ୟଥା
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (ମିଥ୍ୟା);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    ଯଦି (unlikely (!out->serialize (c->serializer, 0u))) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      ଯଦି (unlikely (!o) || !o->serialize_subset (c, offset, ଏହି))
        return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  ତାର୍କିକ serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->serializer->embed (ଏହି->format))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->serializer->embed (ଏହି->coverage))) return_trace (ମିଥ୍ୟା);

    ଯଦି (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (ମିଥ୍ୟା);
    ଯଦି (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (ମିଥ୍ୟା);

    auto *substitute_out = c->serializer->start_embed<typename Types::template ArrayOf<typename Types::HBGlyphID>> ();
    auto substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    auto glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    ଯଦି (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    ଅଚଳ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ଅଚଳ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (ଏହି+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ଅଚଳ typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ଫେରନ୍ତୁ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (ତାର୍କିକ (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    ଯଦି (unlikely (!out->serialize (c->serializer, 0u))) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      ଯଦି (unlikely (!o) || !o->serialize_subset (c, offset, ଏହି))
        return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
