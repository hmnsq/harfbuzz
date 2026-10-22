#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ਉਲਟ_Chain_Single_Subst_Format1_2
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of table */
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Coverage>>
                backtrack;              /* Array of coverage tables
                                         * in backtracking sequence, in glyph
                                         * sequence order */
  /* Format 2 widens the backtrack and lookahead coverage offsets but
   * keeps their counts 16-bit. */
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Coverage>>
                lookaheadX;             /* Array of coverage tables
                                         * in lookahead sequence, in glyph
                                         * sequence order */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBGlyphID>
                substituteX;            /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (6 + Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!(coverage.sanitize (c, ਇਹ) && backtrack.sanitize (c, ਇਹ)))
      return_trace (ਝੂਠ);
    hb_barrier ();
    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ਜੇ (!lookahead.sanitize (c, ਇਹ))
      return_trace (ਝੂਠ);
    hb_barrier ();
    ਅਚਲ ਸਵੈਚਾਲਿਤ &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਜੇ (!(ਇਹ+coverage).intersects (glyphs))
      ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2;

    ਗਿਣਤੀ_2 = backtrack.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!(ਇਹ+backtrack[i]).intersects (glyphs))
        ਵਾਪਸ ਝੂਠ;

    ਗਿਣਤੀ_2 = lookahead.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!(ਇਹ+lookahead[i]).intersects (glyphs))
        ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਝੂਠ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ {
    // Filter by intersects and parent_active_glyphs like closure does
    ਜੇ (!intersects (c->glyphs)) ਵਾਪਸ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_ਸੈੱਟ_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = backtrack.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_ਸੈੱਟ_t ਪਿੱਠ_glyphs;
      (ਇਹ+backtrack[i]).intersect_ਸੈੱਟ (*c->glyphs, ਪਿੱਠ_glyphs);
      ਜੇ (ਪਿੱਠ_glyphs.get_population () == 1)
        context_glyphs.ਜੋੜੋ (ਪਿੱਠ_glyphs.get_min ());
      ਨਹੀਂਤਾਂ ਜੇ (ਪਿੱਠ_glyphs.get_population () > 1)
      {
        hb_codepoint_t ਸੈੱਟ_idx = c->depend_ਅੰਕੜੇ->ਲੱਭੋ_or_create_context_ਸੈੱਟ (ਪਿੱਠ_glyphs);
        ਜੇ (unlikely (ਸੈੱਟ_idx == HB_CODEPOINT_INVALID))
          ਵਾਪਸ;
        context_glyphs.ਜੋੜੋ (HB_DEPEND_CONTEXT_ਸੈੱਟ_FLAG | ਸੈੱਟ_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    ਗਿਣਤੀ_2 = lookahead.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_ਸੈੱਟ_t look_glyphs;
      (ਇਹ+lookahead[i]).intersect_ਸੈੱਟ (*c->glyphs, look_glyphs);
      ਜੇ (look_glyphs.get_population () == 1)
        context_glyphs.ਜੋੜੋ (look_glyphs.get_min ());
      ਨਹੀਂਤਾਂ ਜੇ (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t ਸੈੱਟ_idx = c->depend_ਅੰਕੜੇ->ਲੱਭੋ_or_create_context_ਸੈੱਟ (look_glyphs);
        ਜੇ (unlikely (ਸੈੱਟ_idx == HB_CODEPOINT_INVALID))
          ਵਾਪਸ;
        context_glyphs.ਜੋੜੋ (HB_DEPEND_CONTEXT_ਸੈੱਟ_FLAG | ਸੈੱਟ_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    ਜੇ (unlikely (context_glyphs.ਵਿੱਚ_error ()))
    {
      c->depend_ਅੰਕੜੇ->fail ();
      ਵਾਪਸ;
    }
    hb_codepoint_t context_ਸੈੱਟ_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_ਅੰਕੜੇ->ਲੱਭੋ_or_create_context_ਸੈੱਟ (context_glyphs);
    ਜੇ (unlikely (!context_glyphs.is_empty () &&
                  context_ਸੈੱਟ_idx == HB_CODEPOINT_INVALID))
      ਵਾਪਸ;

    hb_codepoint_t saved_context = c->depend_ਅੰਕੜੇ->current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ;
    c->depend_ਅੰਕੜੇ->current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ = context_ਸੈੱਟ_idx;

    + hb_zip (ਇਹ+coverage, substitute)
    | hb_filter (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), hb_ਪਹਿਲਾ)
    | hb_apply ([&] (ਅਚਲ hb_codepoint_pair_t &_) { c->depend_ਅੰਕੜੇ->ਜੋੜੋ_gsub_lookup (_.ਪਹਿਲਾ, c->lookup_ਸੂਚਕਾਂਕ, _.second); })
    ;

    c->depend_ਅੰਕੜੇ->current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ = saved_context;
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    ਜੇ (!intersects (c->glyphs)) ਵਾਪਸ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (ਇਹ+coverage, substitute)
    | hb_filter (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_sink (c->output)
    ;
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2;

    ਗਿਣਤੀ_2 = backtrack.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (unlikely (!(ਇਹ+backtrack[i]).collect_coverage (c->before))) ਵਾਪਸ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ਗਿਣਤੀ_2 = lookahead.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (unlikely (!(ਇਹ+lookahead[i]).collect_coverage (c->after))) ਵਾਪਸ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &substitute = StructAfter<decltype (substituteX)> (lookahead);
    ਗਿਣਤੀ_2 = substitute.len;
    c->output->ਜੋੜੋ_array (substitute.arrayZ, substitute.len);
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  { ਵਾਪਸ c->len == 1 && (ਇਹ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (c->ਬਫਰ->cur ().codepoint);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਜੇ (unlikely (c->nesting_level_ਖੱਬਾ != HB_MAX_NESTING_LEVEL))
      return_trace (ਝੂਠ); /* No chaining to this type */

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &substitute = StructAfter<decltype (substituteX)> (lookahead);

    ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= substitute.len)) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ = 0, ਸਿਰਾ_ਸੂਚਕਾਂਕ = 0;
    ਜੇ (ਮੇਲ_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         ਮੇਲ_coverage, ਇਹ,
                         &ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ) &&
        ਮੇਲ_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         ਮੇਲ_coverage, ਇਹ,
                         c->ਬਫਰ->idx + 1, &ਸਿਰਾ_ਸੂਚਕਾਂਕ))
    {
      c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break_from_outbuffer (ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ, ਸਿਰਾ_ਸੂਚਕਾਂਕ);

      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->ਬਫਰ->idx);
      }

      c->replace_glyph_inplace (substitute[ਸੂਚਕਾਂਕ_2]);

      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->ਬਫਰ->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (ਸੱਚ);
    }
    ਨਹੀਂਤਾਂ
    {
      c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ, ਸਿਰਾ_ਸੂਚਕਾਂਕ);
      return_trace (ਝੂਠ);
    }
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize_coverage_ਵਿੱਥ_array (hb_subset_context_t *c, Iterator it) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed<Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Coverage>>> ();
    ਜੇ (unlikely (!ਬਾਹਰਲਾ->serialize (c->serializer, 0u))) return_trace (ਝੂਠ);

    ਲਈ (ਸਵੈਚਾਲਿਤ& ਵਿੱਥ : it) {
      ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->serialize_append (c->serializer);
      ਜੇ (unlikely (!o) || !o->serialize_subset (c, ਵਿੱਥ, ਇਹ))
        return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator, ਕਿਸਮ_ਨਾਂ BacktrackIterator, ਕਿਸਮ_ਨਾਂ LookaheadIterator,
           hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  ਤਾਰਕਿਕ serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->serializer->embed (ਇਹ->format))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->serializer->embed (ਇਹ->coverage))) return_trace (ਝੂਠ);

    ਜੇ (!serialize_coverage_ਵਿੱਥ_array (c, backtrack_iter)) return_trace (ਝੂਠ);
    ਜੇ (!serialize_lookahead_coverage_ਵਿੱਥ_array (c, lookahead_iter)) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ *substitute_ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBGlyphID>> ();
    ਸਵੈਚਾਲਿਤ substitutes =
    + coverage_subst_iter
    | hb_map (hb_ਦੂਜਾ)
    ;

    ਸਵੈਚਾਲਿਤ glyphs =
    + coverage_subst_iter
    | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
    ;
    ਜੇ (unlikely (! c->serializer->check_success (substitute_ਬਾਹਰਲਾ->serialize (c->serializer, substitutes))))
        return_trace (ਝੂਠ);

    ਜੇ (unlikely (!ਬਾਹਰਲਾ->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &substitute = StructAfter<decltype (substituteX)> (lookahead);

    ਸਵੈਚਾਲਿਤ it =
    + hb_zip (ਇਹ+coverage, substitute)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (glyphset, hb_ਦੂਜਾ)
    | hb_map_retains_ਛਾਂਟੀ ([&] (hb_pair_t<hb_codepoint_t, ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ਵਾਪਸ hb_pair (glyph_map[p.ਪਹਿਲਾ], glyph_map[p.second]); })
    ;

    return_trace (ਤਾਰਕਿਕ (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize_lookahead_coverage_ਵਿੱਥ_array (hb_subset_context_t *c, Iterator it) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed<Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Coverage>>> ();
    ਜੇ (unlikely (!ਬਾਹਰਲਾ->serialize (c->serializer, 0u))) return_trace (ਝੂਠ);

    ਲਈ (ਸਵੈਚਾਲਿਤ& ਵਿੱਥ : it) {
      ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->serialize_append (c->serializer);
      ਜੇ (unlikely (!o) || !o->serialize_subset (c, ਵਿੱਥ, ਇਹ))
        return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }
};

}
}
}

#ਜੇ_ਅੰਤ  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
