#که_نه_تعریف_شوی OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

کينډۍ <ډول_نوم Types>
بنية سرچپه_Chain_Single_Subst_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of table */
  Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Coverage>>
                backtrack;              /* Array of coverage tables
                                         * in backtracking sequence, in glyph
                                         * sequence order */
  /* Format 2 widens the backtrack and lookahead coverage offsets but
   * keeps their counts 16-bit. */
  Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Coverage>>
                lookaheadX;             /* Array of coverage tables
                                         * in lookahead sequence, in glyph
                                         * sequence order */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::HBGlyphID>
                substituteX;            /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */
  عام:
  DEFINE_اندازه_MIN (6 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(coverage.sanitize (c, هذا) && backtrack.sanitize (c, هذا)))
      return_trace (خطأ);
    hb_barrier ();
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    إذا (!lookahead.sanitize (c, هذا))
      return_trace (خطأ);
    hb_barrier ();
    ثابت خپلکاری &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    بینښه صحيح شمېر;

    شمېر = backtrack.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (!(هذا+backtrack[i]).intersects (glyphs))
        أرجع خطأ;

    شمېر = lookahead.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (!(هذا+lookahead[i]).intersects (glyphs))
        أرجع خطأ;

    أرجع ریښتیا;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت {
    // Filter by intersects and parent_active_glyphs like closure does
    إذا (!intersects (c->glyphs)) أرجع;

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خپلکاری &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_سېټ_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    بینښه صحيح شمېر = backtrack.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      hb_سېټ_t شا_glyphs;
      (هذا+backtrack[i]).intersect_سېټ (*c->glyphs, شا_glyphs);
      إذا (شا_glyphs.get_population () == 1)
        context_glyphs.جمع_کړه (شا_glyphs.get_min ());
      وإلا إذا (شا_glyphs.get_population () > 1)
      {
        hb_codepoint_t سېټ_idx = c->depend_معلومات->پیدا_کړه_or_create_context_سېټ (شا_glyphs);
        إذا (unlikely (سېټ_idx == HB_CODEPOINT_INVALID))
          أرجع;
        context_glyphs.جمع_کړه (HB_DEPEND_CONTEXT_سېټ_FLAG | سېټ_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    شمېر = lookahead.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      hb_سېټ_t look_glyphs;
      (هذا+lookahead[i]).intersect_سېټ (*c->glyphs, look_glyphs);
      إذا (look_glyphs.get_population () == 1)
        context_glyphs.جمع_کړه (look_glyphs.get_min ());
      وإلا إذا (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t سېټ_idx = c->depend_معلومات->پیدا_کړه_or_create_context_سېټ (look_glyphs);
        إذا (unlikely (سېټ_idx == HB_CODEPOINT_INVALID))
          أرجع;
        context_glyphs.جمع_کړه (HB_DEPEND_CONTEXT_سېټ_FLAG | سېټ_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    إذا (unlikely (context_glyphs.دننه_error ()))
    {
      c->depend_معلومات->fail ();
      أرجع;
    }
    hb_codepoint_t context_سېټ_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_معلومات->پیدا_کړه_or_create_context_سېټ (context_glyphs);
    إذا (unlikely (!context_glyphs.is_empty () &&
                  context_سېټ_idx == HB_CODEPOINT_INVALID))
      أرجع;

    hb_codepoint_t saved_context = c->depend_معلومات->current_context_سېټ_شاخص;
    c->depend_معلومات->current_context_سېټ_شاخص = context_سېټ_idx;

    + hb_zip (هذا+coverage, substitute)
    | hb_filter (c->پلار_فعال_glyphs (), hb_لومړی)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_معلومات->جمع_کړه_gsub_lookup (_.لومړی, c->lookup_شاخص, _.second); })
    ;

    c->depend_معلومات->current_context_سېټ_شاخص = saved_context;
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs)) أرجع;

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خپلکاری &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (هذا+coverage, substitute)
    | hb_filter (c->پلار_فعال_glyphs (), hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_sink (c->output)
    ;
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->ننوت))) أرجع;

    بینښه صحيح شمېر;

    شمېر = backtrack.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (unlikely (!(هذا+backtrack[i]).collect_coverage (c->before))) أرجع;

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    شمېر = lookahead.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (unlikely (!(هذا+lookahead[i]).collect_coverage (c->after))) أرجع;

    ثابت خپلکاری &substitute = StructAfter<decltype (substituteX)> (lookahead);
    شمېر = substitute.len;
    c->output->جمع_کړه_array (substitute.arrayZ, substitute.len);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (c->بفر->cur ().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (c->nesting_level_کیڼ != HB_MAX_NESTING_LEVEL))
      return_trace (خطأ); /* No chaining to this type */

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خپلکاری &substitute = StructAfter<decltype (substituteX)> (lookahead);

    إذا (unlikely (شاخص_2 >= substitute.len)) return_trace (خطأ);

    بینښه صحيح پیل_شاخص = 0, پای_ټکی_شاخص = 0;
    إذا (سمون_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         سمون_coverage, هذا,
                         &پیل_شاخص) &&
        سمون_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         سمون_coverage, هذا,
                         c->بفر->idx + 1, &پای_ټکی_شاخص))
    {
      c->بفر->unsafe_منزل_break_from_outbuffer (پیل_شاخص, پای_ټکی_شاخص);

      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph_inplace (substitute[شاخص_2]);

      إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->بفر->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (ریښتیا);
    }
    وإلا
    {
      c->بفر->unsafe_منزل_concat_from_outbuffer (پیل_شاخص, پای_ټکی_شاخص);
      return_trace (خطأ);
    }
  }

  کينډۍ<ډول_نوم Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_coverage_دننه_واټن_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed<Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Coverage>>> ();
    إذا (unlikely (!بهر->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خپلکاری& دننه_واټن : it) {
      خپلکاری *o = بهر->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, دننه_واټن, هذا))
        return_trace (خطأ);
    }

    return_trace (ریښتیا);
  }

  کينډۍ<ډول_نوم Iterator, ډول_نوم BacktrackIterator, ډول_نوم LookaheadIterator,
           hb_requires (hb_is_sorted_سرچینه_د (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  منطقي serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->serializer->embed (هذا->format))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (هذا->coverage))) return_trace (خطأ);

    إذا (!serialize_coverage_دننه_واټن_array (c, backtrack_iter)) return_trace (خطأ);
    إذا (!serialize_lookahead_coverage_دننه_واټن_array (c, lookahead_iter)) return_trace (خطأ);

    خپلکاری *substitute_بهر = c->serializer->پیل_embed<ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::HBGlyphID>> ();
    خپلکاری substitutes =
    + coverage_subst_iter
    | hb_map (hb_دوهم)
    ;

    خپلکاری glyphs =
    + coverage_subst_iter
    | hb_map_retains_ترتیبونه (hb_لومړی)
    ;
    إذا (unlikely (! c->serializer->check_success (substitute_بهر->serialize (c->serializer, substitutes))))
        return_trace (خطأ);

    إذا (unlikely (!بهر->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (خطأ);
    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خپلکاری &substitute = StructAfter<decltype (substituteX)> (lookahead);

    خپلکاری it =
    + hb_zip (هذا+coverage, substitute)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (glyphset, hb_دوهم)
    | hb_map_retains_ترتیبونه ([&] (hb_pair_t<hb_codepoint_t, ثابت ډول_نوم Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { أرجع hb_pair (glyph_map[p.لومړی], glyph_map[p.second]); })
    ;

    return_trace (منطقي (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  کينډۍ<ډول_نوم Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_lookahead_coverage_دننه_واټن_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed<Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Coverage>>> ();
    إذا (unlikely (!بهر->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خپلکاری& دننه_واټن : it) {
      خپلکاری *o = بهر->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, دننه_واټن, هذا))
        return_trace (خطأ);
    }

    return_trace (ریښتیا);
  }
};

}
}
}

#د_که_پای  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
