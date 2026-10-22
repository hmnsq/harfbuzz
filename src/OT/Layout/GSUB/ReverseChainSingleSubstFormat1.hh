#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

ढाँचा <typename Types>
संरचना ReverseChainSingleSubstFormat1_2
{
  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of table */
  Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>
                backtrack;              /* Array of coverage tables
                                         * in backtracking sequence, in glyph
                                         * sequence order */
  /* Format 2 widens the backtrack and lookahead coverage offsets but
   * keeps their counts 16-bit. */
  Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>
                lookaheadX;             /* Array of coverage tables
                                         * in lookahead sequence, in glyph
                                         * sequence order */
  typename Types::ढाँचा ArrayOf<typename Types::HBGlyphID>
                substituteX;            /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!(coverage.sanitize (c, हे) && backtrack.sanitize (c, हे)))
      return_trace (असत्य);
    hb_barrier ();
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    जर (!lookahead.sanitize (c, हे))
      return_trace (असत्य);
    hb_barrier ();
    स्थिरांक स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    जर (!(हे+coverage).intersects (glyphs))
      परत असत्य;

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    अचिन्हित पूर्णांक मोजणी;

    मोजणी = backtrack.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (!(हे+backtrack[i]).intersects (glyphs))
        परत असत्य;

    मोजणी = lookahead.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (!(हे+lookahead[i]).intersects (glyphs))
        परत असत्य;

    परत सत्य;
  }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत असत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक {
    // Filter by intersects and parent_active_glyphs like closure does
    जर (!intersects (c->glyphs)) परत;

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    स्थिरांक स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    अचिन्हित पूर्णांक मोजणी = backtrack.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      hb_set_t back_glyphs;
      (हे+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      जर (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      अन्यथा जर (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        जर (unlikely (set_idx == HB_CODEPOINT_INVALID))
          परत;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    मोजणी = lookahead.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      hb_set_t look_glyphs;
      (हे+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      जर (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      अन्यथा जर (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        जर (unlikely (set_idx == HB_CODEPOINT_INVALID))
          परत;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    जर (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      परत;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    जर (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      परत;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (हे+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (स्थिरांक hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    जर (!intersects (c->glyphs)) परत;

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    स्थिरांक स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (हे+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    जर (unlikely (!(हे+coverage).collect_coverage (c->आदान))) परत;

    अचिन्हित पूर्णांक मोजणी;

    मोजणी = backtrack.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (unlikely (!(हे+backtrack[i]).collect_coverage (c->before))) परत;

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    मोजणी = lookahead.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (unlikely (!(हे+lookahead[i]).collect_coverage (c->after))) परत;

    स्थिरांक स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);
    मोजणी = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  { परत c->len == 1 && (हे+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (c->buffer->cur ().codepoint);
    जर (index == NOT_COVERED) return_trace (असत्य);

    जर (unlikely (c->nesting_level_डावा != HB_MAX_NESTING_LEVEL))
      return_trace (असत्य); /* No chaining to this type */

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    स्थिरांक स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);

    जर (unlikely (index >= substitute.len)) return_trace (असत्य);

    अचिन्हित पूर्णांक start_index = 0, end_index = 0;
    जर (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, हे,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, हे,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (सत्य);
    }
    अन्यथा
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (असत्य);
    }
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->serializer->start_embed<Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>> ();
    जर (unlikely (!out->serialize (c->serializer, 0u))) return_trace (असत्य);

    साठी (स्वतः& offset : it) {
      स्वतः *o = out->serialize_append (c->serializer);
      जर (unlikely (!o) || !o->serialize_subset (c, offset, हे))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  ढाँचा<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  तार्किक serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!c->serializer->embed (हे->format))) return_trace (असत्य);
    जर (unlikely (!c->serializer->embed (हे->coverage))) return_trace (असत्य);

    जर (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (असत्य);
    जर (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (असत्य);

    स्वतः *substitute_out = c->serializer->start_embed<typename Types::ढाँचा ArrayOf<typename Types::HBGlyphID>> ();
    स्वतः substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    स्वतः glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    जर (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (असत्य);

    जर (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    स्थिरांक स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);

    स्वतः it =
    + hb_zip (हे+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, स्थिरांक typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { परत hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (तार्किक (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->serializer->start_embed<Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>> ();
    जर (unlikely (!out->serialize (c->serializer, 0u))) return_trace (असत्य);

    साठी (स्वतः& offset : it) {
      स्वतः *o = out->serialize_append (c->serializer);
      जर (unlikely (!o) || !o->serialize_subset (c, offset, हे))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
