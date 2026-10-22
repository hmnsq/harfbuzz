#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#وصف_ڪريو OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچو <قسم_نالو Types>
بنية ابتو_Chain_Single_Subst_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of table */
  Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<Coverage>>
                backtrack;              /* Array of coverage tables
                                         * in backtracking sequence, in glyph
                                         * sequence order */
  /* Format 2 widens the backtrack and lookahead coverage offsets but
   * keeps their counts 16-bit. */
  Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<Coverage>>
                lookaheadX;             /* Array of coverage tables
                                         * in lookahead sequence, in glyph
                                         * sequence order */
  قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::HBGlyphID>
                substituteX;            /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */
  عام:
  DEFINE_سائيز_MIN (6 + Types::LOffset::static_سائيز + Types::HBUINT::static_سائيز);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(coverage.sanitize (c, هذا) && backtrack.sanitize (c, هذا)))
      return_trace (خطأ);
    hb_barrier ();
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    إذا (!lookahead.sanitize (c, هذا))
      return_trace (خطأ);
    hb_barrier ();
    ثابت خودڪار &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    بنانشان صحيح ڳڻپ;

    ڳڻپ = backtrack.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (!(هذا+backtrack[i]).intersects (glyphs))
        أرجع خطأ;

    ڳڻپ = lookahead.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (!(هذا+lookahead[i]).intersects (glyphs))
        أرجع خطأ;

    أرجع سچ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت {
    // Filter by intersects and parent_active_glyphs like closure does
    إذا (!intersects (c->glyphs)) أرجع;

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودڪار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_سيٽ_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    بنانشان صحيح ڳڻپ = backtrack.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      hb_سيٽ_t پٺ_glyphs;
      (هذا+backtrack[i]).intersect_سيٽ (*c->glyphs, پٺ_glyphs);
      إذا (پٺ_glyphs.get_population () == 1)
        context_glyphs.جوڙيو (پٺ_glyphs.get_min ());
      وإلا إذا (پٺ_glyphs.get_population () > 1)
      {
        hb_codepoint_t سيٽ_idx = c->depend_معلومات->ڳوليو_or_create_context_سيٽ (پٺ_glyphs);
        إذا (unlikely (سيٽ_idx == HB_CODEPOINT_INVALID))
          أرجع;
        context_glyphs.جوڙيو (HB_DEPEND_CONTEXT_سيٽ_FLAG | سيٽ_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    ڳڻپ = lookahead.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      hb_سيٽ_t look_glyphs;
      (هذا+lookahead[i]).intersect_سيٽ (*c->glyphs, look_glyphs);
      إذا (look_glyphs.get_population () == 1)
        context_glyphs.جوڙيو (look_glyphs.get_min ());
      وإلا إذا (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t سيٽ_idx = c->depend_معلومات->ڳوليو_or_create_context_سيٽ (look_glyphs);
        إذا (unlikely (سيٽ_idx == HB_CODEPOINT_INVALID))
          أرجع;
        context_glyphs.جوڙيو (HB_DEPEND_CONTEXT_سيٽ_FLAG | سيٽ_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    إذا (unlikely (context_glyphs.اندريون_error ()))
    {
      c->depend_معلومات->fail ();
      أرجع;
    }
    hb_codepoint_t context_سيٽ_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_معلومات->ڳوليو_or_create_context_سيٽ (context_glyphs);
    إذا (unlikely (!context_glyphs.is_empty () &&
                  context_سيٽ_idx == HB_CODEPOINT_INVALID))
      أرجع;

    hb_codepoint_t saved_context = c->depend_معلومات->current_context_سيٽ_اشاريو;
    c->depend_معلومات->current_context_سيٽ_اشاريو = context_سيٽ_idx;

    + hb_zip (هذا+coverage, substitute)
    | hb_filter (c->والد_سرگرم_glyphs (), hb_پهريون)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_معلومات->جوڙيو_gsub_lookup (_.پهريون, c->lookup_اشاريو, _.second); })
    ;

    c->depend_معلومات->current_context_سيٽ_اشاريو = saved_context;
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs)) أرجع;

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودڪار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (هذا+coverage, substitute)
    | hb_filter (c->والد_سرگرم_glyphs (), hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_sink (c->output)
    ;
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلا_2))) أرجع;

    بنانشان صحيح ڳڻپ;

    ڳڻپ = backtrack.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (unlikely (!(هذا+backtrack[i]).collect_coverage (c->before))) أرجع;

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ڳڻپ = lookahead.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (unlikely (!(هذا+lookahead[i]).collect_coverage (c->after))) أرجع;

    ثابت خودڪار &substitute = StructAfter<decltype (substituteX)> (lookahead);
    ڳڻپ = substitute.len;
    c->output->جوڙيو_array (substitute.arrayZ, substitute.len);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (c->بفر->cur ().codepoint);
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (c->nesting_level_کاٻو != HB_MAX_NESTING_LEVEL))
      return_trace (خطأ); /* No chaining to this type */

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودڪار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    إذا (unlikely (اشاريو_2 >= substitute.len)) return_trace (خطأ);

    بنانشان صحيح شروعات_اشاريو = 0, پڇاڙي_اشاريو = 0;
    إذا (ميلاپ_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         ميلاپ_coverage, هذا,
                         &شروعات_اشاريو) &&
        ميلاپ_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         ميلاپ_coverage, هذا,
                         c->بفر->idx + 1, &پڇاڙي_اشاريو))
    {
      c->بفر->unsafe_منزل_break_from_outbuffer (شروعات_اشاريو, پڇاڙي_اشاريو);

      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->پيغام (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph_inplace (substitute[اشاريو_2]);

      إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
      {
	c->بفر->پيغام (c->font,
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
      c->بفر->unsafe_منزل_concat_from_outbuffer (شروعات_اشاريو, پڇاڙي_اشاريو);
      return_trace (خطأ);
    }
  }

  سانچو<قسم_نالو Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_coverage_وٿي_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed<Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<Coverage>>> ();
    إذا (unlikely (!ٻاهريون->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خودڪار& وٿي : it) {
      خودڪار *o = ٻاهريون->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, وٿي, هذا))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }

  سانچو<قسم_نالو Iterator, قسم_نالو BacktrackIterator, قسم_نالو LookaheadIterator,
           hb_requires (hb_is_sorted_ذريعو_جو (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  منطقي serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!c->serializer->embed (هذا->format))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (هذا->coverage))) return_trace (خطأ);

    إذا (!serialize_coverage_وٿي_array (c, backtrack_iter)) return_trace (خطأ);
    إذا (!serialize_lookahead_coverage_وٿي_array (c, lookahead_iter)) return_trace (خطأ);

    خودڪار *substitute_ٻاهريون = c->serializer->شروعات_embed<قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::HBGlyphID>> ();
    خودڪار substitutes =
    + coverage_subst_iter
    | hb_map (hb_ٻيو)
    ;

    خودڪار glyphs =
    + coverage_subst_iter
    | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
    ;
    إذا (unlikely (! c->serializer->check_success (substitute_ٻاهريون->serialize (c->serializer, substitutes))))
        return_trace (خطأ);

    إذا (unlikely (!ٻاهريون->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودڪار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    خودڪار it =
    + hb_zip (هذا+coverage, substitute)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter (glyphset, hb_ٻيو)
    | hb_map_retains_ترتيب_ڏيڻ ([&] (hb_pair_t<hb_codepoint_t, ثابت قسم_نالو Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { أرجع hb_pair (glyph_map[p.پهريون], glyph_map[p.second]); })
    ;

    return_trace (منطقي (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  سانچو<قسم_نالو Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_lookahead_coverage_وٿي_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed<Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<Coverage>>> ();
    إذا (unlikely (!ٻاهريون->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خودڪار& وٿي : it) {
      خودڪار *o = ٻاهريون->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, وٿي, هذا))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }
};

}
}
}

#جيڪڏهن_پورو  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
