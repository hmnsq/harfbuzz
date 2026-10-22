#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

template <typename Types>
بنية ReverseChainSingleSubstFormat1_2
{
  محمي:
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
  عام:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(coverage.sanitize (c, هذا) && backtrack.sanitize (c, هذا)))
      return_trace (خطأ);
    hb_barrier ();
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    إذا (!lookahead.sanitize (c, هذا))
      return_trace (خطأ);
    hb_barrier ();
    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned صحيح کيراءن;

    کيراءن = backtrack.len;
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
      إذا (!(هذا+backtrack[i]).intersects (glyphs))
        أرجع خطأ;

    کيراءن = lookahead.len;
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
      إذا (!(هذا+lookahead[i]).intersects (glyphs))
        أرجع خطأ;

    أرجع true;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت {
    // Filter by intersects and parent_active_glyphs like closure does
    إذا (!intersects (c->glyphs)) أرجع;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned صحيح کيراءن = backtrack.len;
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
    {
      hb_set_t back_glyphs;
      (هذا+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      إذا (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      وإلا إذا (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        إذا (unlikely (set_idx == HB_CODEPOINT_INVALID))
          أرجع;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    کيراءن = lookahead.len;
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
    {
      hb_set_t look_glyphs;
      (هذا+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      إذا (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      وإلا إذا (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        إذا (unlikely (set_idx == HB_CODEPOINT_INVALID))
          أرجع;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    إذا (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      أرجع;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    إذا (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      أرجع;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (هذا+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs)) أرجع;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (هذا+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->ماسوقن))) أرجع;

    unsigned صحيح کيراءن;

    کيراءن = backtrack.len;
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
      إذا (unlikely (!(هذا+backtrack[i]).collect_coverage (c->before))) أرجع;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    کيراءن = lookahead.len;
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
      إذا (unlikely (!(هذا+lookahead[i]).collect_coverage (c->after))) أرجع;

    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    کيراءن = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    unsigned صحيح index = (هذا+coverage).get_coverage (c->buffer->cur ().codepoint);
    إذا (index == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (c->nesting_level_کيري != HB_MAX_NESTING_LEVEL))
      return_trace (خطأ); /* No chaining to this type */

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    إذا (unlikely (index >= substitute.len)) return_trace (خطأ);

    unsigned صحيح start_index = 0, end_index = 0;
    إذا (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, هذا,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, هذا,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (true);
    }
    وإلا
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (خطأ);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    إذا (unlikely (!out->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, offset, هذا))
        return_trace (خطأ);
    }

    return_trace (true);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  منطقي serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ثابت
  {
    TRACE_SERIALIZE (هذا);

    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!c->serializer->embed (هذا->format))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (هذا->coverage))) return_trace (خطأ);

    إذا (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (خطأ);
    إذا (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (خطأ);

    auto *substitute_out = c->serializer->start_embed<typename Types::template ArrayOf<typename Types::HBGlyphID>> ();
    auto substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    auto glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    إذا (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (خطأ);

    إذا (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (خطأ);
    return_trace (true);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (هذا+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ثابت typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { أرجع hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (منطقي (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    إذا (unlikely (!out->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, offset, هذا))
        return_trace (خطأ);
    }

    return_trace (true);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
