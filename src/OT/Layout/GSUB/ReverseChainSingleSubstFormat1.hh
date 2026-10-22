#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#অন্তর্ভাৱ "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
সংরচনা ReverseChainSingleSubstFormat1_2
{
  রক্ষিত:
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
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!(coverage.sanitize (c, এতত্) && backtrack.sanitize (c, এতত্)))
      return_trace (মিছা);
    hb_barrier ();
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    যদি (!lookahead.sanitize (c, এতত্))
      return_trace (মিছা);
    hb_barrier ();
    ধ্রুৱ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    যদি (!(এতত্+coverage).intersects (glyphs))
      ঘূৰাই মিছা;

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned পূর্ণাঙ্ক গুনতি;

    গুনতি = backtrack.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (!(এতত্+backtrack[i]).intersects (glyphs))
        ঘূৰাই মিছা;

    গুনতি = lookahead.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (!(এতত্+lookahead[i]).intersects (glyphs))
        ঘূৰাই মিছা;

    ঘূৰাই সঁচা;
  }

  তার্কিক may_have_non_1to1 () ধ্রুৱ
  { ঘূৰাই মিছা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুৱ {
    // Filter by intersects and parent_active_glyphs like closure does
    যদি (!intersects (c->glyphs)) ঘূৰাই;

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ধ্রুৱ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned পূর্ণাঙ্ক গুনতি = backtrack.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      hb_set_t back_glyphs;
      (এতত্+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      যদি (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      অন্যথা যদি (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        যদি (unlikely (set_idx == HB_CODEPOINT_INVALID))
          ঘূৰাই;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    গুনতি = lookahead.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      hb_set_t look_glyphs;
      (এতত্+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      যদি (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      অন্যথা যদি (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        যদি (unlikely (set_idx == HB_CODEPOINT_INVALID))
          ঘূৰাই;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    যদি (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      ঘূৰাই;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    যদি (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      ঘূৰাই;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (এতত্+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ধ্রুৱ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    যদি (!intersects (c->glyphs)) ঘূৰাই;

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ধ্রুৱ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (এতত্+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    যদি (unlikely (!(এতত্+coverage).collect_coverage (c->প্ৰবেশ))) ঘূৰাই;

    unsigned পূর্ণাঙ্ক গুনতি;

    গুনতি = backtrack.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (unlikely (!(এতত্+backtrack[i]).collect_coverage (c->before))) ঘূৰাই;

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    গুনতি = lookahead.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (unlikely (!(এতত্+lookahead[i]).collect_coverage (c->after))) ঘূৰাই;

    ধ্রুৱ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    গুনতি = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+coverage; }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  { ঘূৰাই c->len == 1 && (এতত্+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (c->buffer->cur ().codepoint);
    যদি (index == NOT_COVERED) return_trace (মিছা);

    যদি (unlikely (c->nesting_level_বাম != HB_MAX_NESTING_LEVEL))
      return_trace (মিছা); /* No chaining to this type */

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ধ্রুৱ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    যদি (unlikely (index >= substitute.len)) return_trace (মিছা);

    unsigned পূর্ণাঙ্ক start_index = 0, end_index = 0;
    যদি (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, এতত্,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, এতত্,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (সঁচা);
    }
    অন্যথা
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (মিছা);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  তার্কিক serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    যদি (unlikely (!out->serialize (c->serializer, 0u))) return_trace (মিছা);

    বাবে (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      যদি (unlikely (!o) || !o->serialize_subset (c, offset, এতত্))
        return_trace (মিছা);
    }

    return_trace (সঁচা);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  তার্কিক serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);

    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!c->serializer->embed (এতত্->format))) return_trace (মিছা);
    যদি (unlikely (!c->serializer->embed (এতত্->coverage))) return_trace (মিছা);

    যদি (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (মিছা);
    যদি (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (মিছা);

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
        return_trace (মিছা);

    যদি (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (মিছা);
    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুৱ hb_map_t &glyph_map = *c->plan->glyph_map;

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ধ্রুৱ auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (এতত্+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ধ্রুৱ typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ঘূৰাই hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (তার্কিক (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  তার্কিক serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    যদি (unlikely (!out->serialize (c->serializer, 0u))) return_trace (মিছা);

    বাবে (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      যদি (unlikely (!o) || !o->serialize_subset (c, offset, এতত্))
        return_trace (মিছা);
    }

    return_trace (সঁচা);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
