#अगर_अघोषित OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#घोषणा OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#समावेश "Common.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

ढाँचा <typename Types>
काष्ठा ReverseChainSingleSubstFormat1_2
{
  रक्षित:
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
  खुला:
  DEFINE_आकार_MIN (6 + Types::LOffset::static_आकार + Types::HBUINT::static_आकार);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!(coverage.sanitize (c, यह) && backtrack.sanitize (c, यह)))
      return_trace (false);
    hb_barrier ();
    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    अगर (!lookahead.sanitize (c, यह))
      return_trace (false);
    hb_barrier ();
    स्थिर स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    अगर (!(यह+coverage).intersects (glyphs))
      वापस false;

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    अचिन्हित पूर्णांक गिनती;

    गिनती = backtrack.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (!(यह+backtrack[i]).intersects (glyphs))
        वापस false;

    गिनती = lookahead.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (!(यह+lookahead[i]).intersects (glyphs))
        वापस false;

    वापस true;
  }

  bool may_have_non_1to1 () स्थिर
  { वापस false; }

  व्योम depend (hb_depend_context_t *c) स्थिर {
    // Filter by intersects and parent_active_glyphs like closure does
    अगर (!intersects (c->glyphs)) वापस;

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    स्थिर स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    अचिन्हित पूर्णांक गिनती = backtrack.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      hb_set_t back_glyphs;
      (यह+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      अगर (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      अन्यथा अगर (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_तथ्य->find_or_create_context_set (back_glyphs);
        अगर (unlikely (set_idx == HB_CODEPOINT_INVALID))
          वापस;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    गिनती = lookahead.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      hb_set_t look_glyphs;
      (यह+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      अगर (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      अन्यथा अगर (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_तथ्य->find_or_create_context_set (look_glyphs);
        अगर (unlikely (set_idx == HB_CODEPOINT_INVALID))
          वापस;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    अगर (unlikely (context_glyphs.in_error ()))
    {
      c->depend_तथ्य->हार ();
      वापस;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_तथ्य->find_or_create_context_set (context_glyphs);
    अगर (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      वापस;

    hb_codepoint_t saved_context = c->depend_तथ्य->current_context_set_सूचकांक;
    c->depend_तथ्य->current_context_set_सूचकांक = context_set_idx;

    + hb_zip (यह+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (स्थिर hb_codepoint_pair_t &_) { c->depend_तथ्य->add_gsub_lookup (_.first, c->lookup_सूचकांक, _.second); })
    ;

    c->depend_तथ्य->current_context_set_सूचकांक = saved_context;
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    अगर (!intersects (c->glyphs)) वापस;

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    स्थिर स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (यह+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    अगर (unlikely (!(यह+coverage).collect_coverage (c->निवेश))) वापस;

    अचिन्हित पूर्णांक गिनती;

    गिनती = backtrack.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (unlikely (!(यह+backtrack[i]).collect_coverage (c->before))) वापस;

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    गिनती = lookahead.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (unlikely (!(यह+lookahead[i]).collect_coverage (c->after))) वापस;

    स्थिर स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);
    गिनती = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  { वापस c->len == 1 && (यह+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (c->बफर->cur ().codepoint);
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    अगर (unlikely (c->nesting_level_बायाँ != HB_MAX_NESTING_LEVEL))
      return_trace (false); /* No chaining to this type */

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    स्थिर स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);

    अगर (unlikely (सूचकांक_2 >= substitute.len)) return_trace (false);

    अचिन्हित पूर्णांक start_सूचकांक = 0, end_सूचकांक = 0;
    अगर (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, यह,
                         &start_सूचकांक) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, यह,
                         c->बफर->idx + 1, &end_सूचकांक))
    {
      c->बफर->unsafe_to_break_from_outbuffer (start_सूचकांक, end_सूचकांक);

      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->बफर->idx);
      }

      c->replace_glyph_inplace (substitute[सूचकांक_2]);

      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->बफर->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (true);
    }
    अन्यथा
    {
      c->बफर->unsafe_to_concat_from_outbuffer (start_सूचकांक, end_सूचकांक);
      return_trace (false);
    }
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  bool serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->serializer->start_embed<Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>> ();
    अगर (unlikely (!out->serialize (c->serializer, 0u))) return_trace (false);

    क्रम (स्वतः& offset : it) {
      स्वतः *o = out->serialize_append (c->serializer);
      अगर (unlikely (!o) || !o->serialize_subset (c, offset, यह))
        return_trace (false);
    }

    return_trace (true);
  }

  ढाँचा<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  bool serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) स्थिर
  {
    TRACE_SERIALIZE (यह);

    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!c->serializer->embed (यह->format))) return_trace (false);
    अगर (unlikely (!c->serializer->embed (यह->coverage))) return_trace (false);

    अगर (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (false);
    अगर (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (false);

    स्वतः *substitute_out = c->serializer->start_embed<typename Types::ढाँचा ArrayOf<typename Types::HBGlyphID>> ();
    स्वतः substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    स्वतः glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    अगर (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (false);

    अगर (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (false);
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्थिर स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    स्थिर स्वतः &substitute = StructAfter<decltype (substituteX)> (lookahead);

    स्वतः it =
    + hb_zip (यह+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, स्थिर typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { वापस hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (bool (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  bool serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->serializer->start_embed<Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>> ();
    अगर (unlikely (!out->serialize (c->serializer, 0u))) return_trace (false);

    क्रम (स्वतः& offset : it) {
      स्वतः *o = out->serialize_append (c->serializer);
      अगर (unlikely (!o) || !o->serialize_subset (c, offset, यह))
        return_trace (false);
    }

    return_trace (true);
  }
};

}
}
}

#पूर्ण_अगर  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
