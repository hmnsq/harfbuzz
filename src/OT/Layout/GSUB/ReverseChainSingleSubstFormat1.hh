#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
কাঠামো ReverseChainSingleSubstFormat1_2
{
  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_MIN (6 + Types::LOffset::static_আকার + Types::HBUINT::static_আকার);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!(coverage.sanitize (c, এই) && backtrack.sanitize (c, এই)))
      return_trace (মিথ্যা);
    hb_barrier ();
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    যদি (!lookahead.sanitize (c, এই))
      return_trace (মিথ্যা);
    hb_barrier ();
    ধ্রুবক auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    যদি (!(এই+coverage).intersects (glyphs))
      ফেরত মিথ্যা;

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned পূর্ণসংখ্যা গুনতি;

    গুনতি = backtrack.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (!(এই+backtrack[i]).intersects (glyphs))
        ফেরত মিথ্যা;

    গুনতি = lookahead.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (!(এই+lookahead[i]).intersects (glyphs))
        ফেরত মিথ্যা;

    ফেরত সত্য;
  }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত মিথ্যা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক {
    // Filter by intersects and parent_active_glyphs like closure does
    যদি (!intersects (c->glyphs)) ফেরত;

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ধ্রুবক auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned পূর্ণসংখ্যা গুনতি = backtrack.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      hb_set_t back_glyphs;
      (এই+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      যদি (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      অন্যথা যদি (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_তথ্য->find_or_create_context_set (back_glyphs);
        যদি (unlikely (set_idx == HB_CODEPOINT_INVALID))
          ফেরত;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    গুনতি = lookahead.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      hb_set_t look_glyphs;
      (এই+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      যদি (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      অন্যথা যদি (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_তথ্য->find_or_create_context_set (look_glyphs);
        যদি (unlikely (set_idx == HB_CODEPOINT_INVALID))
          ফেরত;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    যদি (unlikely (context_glyphs.in_error ()))
    {
      c->depend_তথ্য->fail ();
      ফেরত;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_তথ্য->find_or_create_context_set (context_glyphs);
    যদি (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      ফেরত;

    hb_codepoint_t saved_context = c->depend_তথ্য->current_context_set_সূচক;
    c->depend_তথ্য->current_context_set_সূচক = context_set_idx;

    + hb_zip (এই+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ধ্রুবক hb_codepoint_pair_t &_) { c->depend_তথ্য->add_gsub_lookup (_.first, c->lookup_সূচক, _.second); })
    ;

    c->depend_তথ্য->current_context_set_সূচক = saved_context;
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    যদি (!intersects (c->glyphs)) ফেরত;

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ধ্রুবক auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (এই+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    যদি (unlikely (!(এই+coverage).collect_coverage (c->প্রবেশ))) ফেরত;

    unsigned পূর্ণসংখ্যা গুনতি;

    গুনতি = backtrack.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (unlikely (!(এই+backtrack[i]).collect_coverage (c->before))) ফেরত;

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    গুনতি = lookahead.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (unlikely (!(এই+lookahead[i]).collect_coverage (c->after))) ফেরত;

    ধ্রুবক auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    গুনতি = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  { ফেরত c->len == 1 && (এই+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (c->বাফার->cur ().codepoint);
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    যদি (unlikely (c->nesting_level_বাম != HB_MAX_NESTING_LEVEL))
      return_trace (মিথ্যা); /* No chaining to this type */

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ধ্রুবক auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    যদি (unlikely (সূচক_2 >= substitute.len)) return_trace (মিথ্যা);

    unsigned পূর্ণসংখ্যা start_সূচক = 0, end_সূচক = 0;
    যদি (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, এই,
                         &start_সূচক) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, এই,
                         c->বাফার->idx + 1, &end_সূচক))
    {
      c->বাফার->unsafe_to_break_from_outbuffer (start_সূচক, end_সূচক);

      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->বাফার->idx);
      }

      c->replace_glyph_inplace (substitute[সূচক_2]);

      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->বাফার->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (সত্য);
    }
    অন্যথা
    {
      c->বাফার->unsafe_to_concat_from_outbuffer (start_সূচক, end_সূচক);
      return_trace (মিথ্যা);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    যদি (unlikely (!out->serialize (c->serializer, 0u))) return_trace (মিথ্যা);

    জন্য (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      যদি (unlikely (!o) || !o->serialize_subset (c, offset, এই))
        return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  যৌক্তিক serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!c->serializer->embed (এই->format))) return_trace (মিথ্যা);
    যদি (unlikely (!c->serializer->embed (এই->coverage))) return_trace (মিথ্যা);

    যদি (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (মিথ্যা);
    যদি (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (মিথ্যা);

    auto *substitute_out = c->serializer->start_embed<typename Types::template ArrayOf<typename Types::HBGlyphID>> ();
    auto substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    auto glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    যদি (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (মিথ্যা);

    যদি (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ধ্রুবক auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (এই+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ধ্রুবক typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ফেরত hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (যৌক্তিক (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    যদি (unlikely (!out->serialize (c->serializer, 0u))) return_trace (মিথ্যা);

    জন্য (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      যদি (unlikely (!o) || !o->serialize_subset (c, offset, এই))
        return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
