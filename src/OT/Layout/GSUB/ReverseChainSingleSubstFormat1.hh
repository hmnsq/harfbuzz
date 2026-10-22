#جے_بنا_تعریف OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچا <قسم_ناں Types>
بنية الٹ_Chain_Single_Subst_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of table */
  Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Coverage>>
                backtrack;              /* Array of coverage tables
                                         * in backtracking sequence, in glyph
                                         * sequence order */
  /* Format 2 widens the backtrack and lookahead coverage offsets but
   * keeps their counts 16-bit. */
  Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Coverage>>
                lookaheadX;             /* Array of coverage tables
                                         * in lookahead sequence, in glyph
                                         * sequence order */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::HBGlyphID>
                substituteX;            /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */
  عام:
  DEFINE_ناپ_MIN (6 + Types::LOffset::static_ناپ + Types::HBUINT::static_ناپ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(coverage.sanitize (c, هذا) && backtrack.sanitize (c, هذا)))
      return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    إذا (!lookahead.sanitize (c, هذا))
      return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    بنانشانی صحيح گنتی;

    گنتی = backtrack.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (!(هذا+backtrack[i]).intersects (glyphs))
        أرجع خطأ;

    گنتی = lookahead.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (!(هذا+lookahead[i]).intersects (glyphs))
        أرجع خطأ;

    أرجع سچ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت {
    // Filter by intersects and parent_active_glyphs like closure does
    إذا (!intersects (c->glyphs)) أرجع;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_سیٹ_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    بنانشانی صحيح گنتی = backtrack.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      hb_سیٹ_t پٹھ_glyphs;
      (هذا+backtrack[i]).intersect_سیٹ (*c->glyphs, پٹھ_glyphs);
      إذا (پٹھ_glyphs.get_population () == 1)
        context_glyphs.جوڑو (پٹھ_glyphs.get_min ());
      وإلا إذا (پٹھ_glyphs.get_population () > 1)
      {
        hb_codepoint_t سیٹ_idx = c->depend_انکڑے->لبھو_or_create_context_سیٹ (پٹھ_glyphs);
        إذا (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
          أرجع;
        context_glyphs.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    گنتی = lookahead.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      hb_سیٹ_t look_glyphs;
      (هذا+lookahead[i]).intersect_سیٹ (*c->glyphs, look_glyphs);
      إذا (look_glyphs.get_population () == 1)
        context_glyphs.جوڑو (look_glyphs.get_min ());
      وإلا إذا (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t سیٹ_idx = c->depend_انکڑے->لبھو_or_create_context_سیٹ (look_glyphs);
        إذا (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
          أرجع;
        context_glyphs.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    إذا (unlikely (context_glyphs.وچ_error ()))
    {
      c->depend_انکڑے->fail ();
      أرجع;
    }
    hb_codepoint_t context_سیٹ_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_انکڑے->لبھو_or_create_context_سیٹ (context_glyphs);
    إذا (unlikely (!context_glyphs.is_empty () &&
                  context_سیٹ_idx == HB_CODEPOINT_INVALID))
      أرجع;

    hb_codepoint_t saved_context = c->depend_انکڑے->current_context_سیٹ_اشاریہ;
    c->depend_انکڑے->current_context_سیٹ_اشاریہ = context_سیٹ_idx;

    + hb_zip (هذا+coverage, substitute)
    | hb_filter (c->ماپا_سرگرم_glyphs (), hb_پہلا)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_انکڑے->جوڑو_gsub_lookup (_.پہلا, c->lookup_اشاریہ, _.second); })
    ;

    c->depend_انکڑے->current_context_سیٹ_اشاریہ = saved_context;
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs)) أرجع;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (هذا+coverage, substitute)
    | hb_filter (c->ماپا_سرگرم_glyphs (), hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_sink (c->output)
    ;
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلہ))) أرجع;

    بنانشانی صحيح گنتی;

    گنتی = backtrack.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (unlikely (!(هذا+backtrack[i]).collect_coverage (c->before))) أرجع;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    گنتی = lookahead.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (unlikely (!(هذا+lookahead[i]).collect_coverage (c->after))) أرجع;

    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);
    گنتی = substitute.len;
    c->output->جوڑو_array (substitute.arrayZ, substitute.len);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (c->بفر->cur ().codepoint);
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (c->nesting_level_بایاں != HB_MAX_NESTING_LEVEL))
      return_trace (خطأ); /* No chaining to this type */

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    إذا (unlikely (اشاریہ_2 >= substitute.len)) return_trace (خطأ);

    بنانشانی صحيح شروع_اشاریہ = 0, سرا_اشاریہ = 0;
    إذا (میل_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         میل_coverage, هذا,
                         &شروع_اشاریہ) &&
        میل_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         میل_coverage, هذا,
                         c->بفر->idx + 1, &سرا_اشاریہ))
    {
      c->بفر->unsafe_منزل_break_from_outbuffer (شروع_اشاریہ, سرا_اشاریہ);

      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->سنیہا (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph_inplace (substitute[اشاریہ_2]);

      إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
      {
	c->بفر->سنیہا (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->بفر->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (سچ);
    }
    وإلا
    {
      c->بفر->unsafe_منزل_concat_from_outbuffer (شروع_اشاریہ, سرا_اشاریہ);
      return_trace (خطأ);
    }
  }

  سانچا<قسم_ناں Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_coverage_وتھ_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed<Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Coverage>>> ();
    إذا (unlikely (!باہرلا->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خودکار& وتھ : it) {
      خودکار *o = باہرلا->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, وتھ, هذا))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }

  سانچا<قسم_ناں Iterator, قسم_ناں BacktrackIterator, قسم_ناں LookaheadIterator,
           hb_requires (hb_is_sorted_سروت_دا (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  منطقي serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->embed (هذا->format))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (هذا->coverage))) return_trace (خطأ);

    إذا (!serialize_coverage_وتھ_array (c, backtrack_iter)) return_trace (خطأ);
    إذا (!serialize_lookahead_coverage_وتھ_array (c, lookahead_iter)) return_trace (خطأ);

    خودکار *substitute_باہرلا = c->serializer->شروع_embed<قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::HBGlyphID>> ();
    خودکار substitutes =
    + coverage_subst_iter
    | hb_map (hb_دوجا)
    ;

    خودکار glyphs =
    + coverage_subst_iter
    | hb_map_retains_چھانٹی (hb_پہلا)
    ;
    إذا (unlikely (! c->serializer->check_success (substitute_باہرلا->serialize (c->serializer, substitutes))))
        return_trace (خطأ);

    إذا (unlikely (!باہرلا->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    خودکار it =
    + hb_zip (هذا+coverage, substitute)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (glyphset, hb_دوجا)
    | hb_map_retains_چھانٹی ([&] (hb_pair_t<hb_codepoint_t, ثابت قسم_ناں Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { أرجع hb_pair (glyph_map[p.پہلا], glyph_map[p.second]); })
    ;

    return_trace (منطقي (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  سانچا<قسم_ناں Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_lookahead_coverage_وتھ_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed<Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Coverage>>> ();
    إذا (unlikely (!باہرلا->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خودکار& وتھ : it) {
      خودکار *o = باہرلا->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, وتھ, هذا))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }
};

}
}
}

#جے_انت  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
