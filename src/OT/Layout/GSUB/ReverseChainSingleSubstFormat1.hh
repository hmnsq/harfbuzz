#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#شامل "Common.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

template <typename Types>
ساختار ReverseChainSingleSubstFormat1_2
{
  محافظتشده:
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
  عمومی:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!(coverage.sanitize (c, این) && backtrack.sanitize (c, این)))
      return_trace (نادرست);
    hb_barrier ();
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    اگر (!lookahead.sanitize (c, این))
      return_trace (نادرست);
    hb_barrier ();
    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    اگر (!(این+coverage).intersects (glyphs))
      بازگشت نادرست;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned صحیح شمار;

    شمار = backtrack.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (!(این+backtrack[i]).intersects (glyphs))
        بازگشت نادرست;

    شمار = lookahead.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (!(این+lookahead[i]).intersects (glyphs))
        بازگشت نادرست;

    بازگشت درست;
  }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت نادرست; }

  تهی depend (hb_depend_context_t *c) ثابت {
    // Filter by intersects and parent_active_glyphs like closure does
    اگر (!intersects (c->glyphs)) بازگشت;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned صحیح شمار = backtrack.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      hb_set_t back_glyphs;
      (این+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      اگر (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      وگرنه اگر (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        اگر (unlikely (set_idx == HB_CODEPOINT_INVALID))
          بازگشت;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    شمار = lookahead.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      hb_set_t look_glyphs;
      (این+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      اگر (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      وگرنه اگر (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        اگر (unlikely (set_idx == HB_CODEPOINT_INVALID))
          بازگشت;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    اگر (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      بازگشت;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    اگر (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      بازگشت;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (این+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    اگر (!intersects (c->glyphs)) بازگشت;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (این+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(این+coverage).collect_coverage (c->درون_داد))) بازگشت;

    unsigned صحیح شمار;

    شمار = backtrack.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (unlikely (!(این+backtrack[i]).collect_coverage (c->before))) بازگشت;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    شمار = lookahead.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (unlikely (!(این+lookahead[i]).collect_coverage (c->after))) بازگشت;

    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    شمار = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  { بازگشت c->len == 1 && (این+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    unsigned صحیح index = (این+coverage).get_coverage (c->buffer->cur ().codepoint);
    اگر (index == NOT_COVERED) return_trace (نادرست);

    اگر (unlikely (c->nesting_level_چپ != HB_MAX_NESTING_LEVEL))
      return_trace (نادرست); /* No chaining to this type */

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    اگر (unlikely (index >= substitute.len)) return_trace (نادرست);

    unsigned صحیح start_index = 0, end_index = 0;
    اگر (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, این,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, این,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (درست);
    }
    وگرنه
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (نادرست);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    اگر (unlikely (!out->serialize (c->serializer, 0u))) return_trace (نادرست);

    برای (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      اگر (unlikely (!o) || !o->serialize_subset (c, offset, این))
        return_trace (نادرست);
    }

    return_trace (درست);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  منطقی serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ثابت
  {
    TRACE_SERIALIZE (این);

    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!c->serializer->embed (این->format))) return_trace (نادرست);
    اگر (unlikely (!c->serializer->embed (این->coverage))) return_trace (نادرست);

    اگر (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (نادرست);
    اگر (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (نادرست);

    auto *substitute_out = c->serializer->start_embed<typename Types::template ArrayOf<typename Types::HBGlyphID>> ();
    auto substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    auto glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    اگر (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (نادرست);

    اگر (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (نادرست);
    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (این+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ثابت typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { بازگشت hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (منطقی (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    اگر (unlikely (!out->serialize (c->serializer, 0u))) return_trace (نادرست);

    برای (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      اگر (unlikely (!o) || !o->serialize_subset (c, offset, این))
        return_trace (نادرست);
    }

    return_trace (درست);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
