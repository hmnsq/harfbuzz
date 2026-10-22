#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#அந்தர்பாவ "Common.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

template <typename Types>
கட்டமைப்பு ReverseChainSingleSubstFormat1_2
{
  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!(coverage.sanitize (c, இது) && backtrack.sanitize (c, இது)))
      return_trace (பொய்);
    hb_barrier ();
    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    எனில் (!lookahead.sanitize (c, இது))
      return_trace (பொய்);
    hb_barrier ();
    மாறிலி auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    எனில் (!(இது+coverage).intersects (glyphs))
      திரும்பு பொய்;

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned முழு மொத்தம்;

    மொத்தம் = backtrack.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (!(இது+backtrack[i]).intersects (glyphs))
        திரும்பு பொய்;

    மொத்தம் = lookahead.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (!(இது+lookahead[i]).intersects (glyphs))
        திரும்பு பொய்;

    திரும்பு உண்மை;
  }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு பொய்; }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி {
    // Filter by intersects and parent_active_glyphs like closure does
    எனில் (!intersects (c->glyphs)) திரும்பு;

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    மாறிலி auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned முழு மொத்தம் = backtrack.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      hb_set_t back_glyphs;
      (இது+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      எனில் (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      இல்லையெனில் எனில் (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        எனில் (unlikely (set_idx == HB_CODEPOINT_INVALID))
          திரும்பு;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    மொத்தம் = lookahead.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      hb_set_t look_glyphs;
      (இது+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      எனில் (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      இல்லையெனில் எனில் (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        எனில் (unlikely (set_idx == HB_CODEPOINT_INVALID))
          திரும்பு;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    எனில் (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      திரும்பு;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    எனில் (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      திரும்பு;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (இது+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (மாறிலி hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    எனில் (!intersects (c->glyphs)) திரும்பு;

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    மாறிலி auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (இது+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    எனில் (unlikely (!(இது+coverage).collect_coverage (c->உள்ளீடு))) திரும்பு;

    unsigned முழு மொத்தம்;

    மொத்தம் = backtrack.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (unlikely (!(இது+backtrack[i]).collect_coverage (c->before))) திரும்பு;

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    மொத்தம் = lookahead.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (unlikely (!(இது+lookahead[i]).collect_coverage (c->after))) திரும்பு;

    மாறிலி auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    மொத்தம் = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  { திரும்பு c->len == 1 && (இது+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    unsigned முழு index = (இது+coverage).get_coverage (c->buffer->cur ().codepoint);
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    எனில் (unlikely (c->nesting_level_இடது != HB_MAX_NESTING_LEVEL))
      return_trace (பொய்); /* No chaining to this type */

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    மாறிலி auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    எனில் (unlikely (index >= substitute.len)) return_trace (பொய்);

    unsigned முழு start_index = 0, end_index = 0;
    எனில் (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, இது,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, இது,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (உண்மை);
    }
    இல்லையெனில்
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (பொய்);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  தருக்க serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    எனில் (unlikely (!out->serialize (c->serializer, 0u))) return_trace (பொய்);

    க்கு (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      எனில் (unlikely (!o) || !o->serialize_subset (c, offset, இது))
        return_trace (பொய்);
    }

    return_trace (உண்மை);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  தருக்க serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!c->serializer->embed (இது->format))) return_trace (பொய்);
    எனில் (unlikely (!c->serializer->embed (இது->coverage))) return_trace (பொய்);

    எனில் (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (பொய்);
    எனில் (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (பொய்);

    auto *substitute_out = c->serializer->start_embed<typename Types::template ArrayOf<typename Types::HBGlyphID>> ();
    auto substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    auto glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    எனில் (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (பொய்);

    எனில் (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (பொய்);
    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    மாறிலி auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    மாறிலி auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (இது+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, மாறிலி typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { திரும்பு hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (தருக்க (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  தருக்க serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    எனில் (unlikely (!out->serialize (c->serializer, 0u))) return_trace (பொய்);

    க்கு (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      எனில் (unlikely (!o) || !o->serialize_subset (c, offset, இது))
        return_trace (பொய்);
    }

    return_trace (உண்மை);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
