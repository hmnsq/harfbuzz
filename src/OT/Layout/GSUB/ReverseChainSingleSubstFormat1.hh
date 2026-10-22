#اگر_تعریف_نشده OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#تعریف_کن OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

قالب <نام_نوع Types>
بنية معکوس_Chain_Single_Subst_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of table */
  Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<Coverage>>
                backtrack;              /* Array of coverage tables
                                         * in backtracking sequence, in glyph
                                         * sequence order */
  /* Format 2 widens the backtrack and lookahead coverage offsets but
   * keeps their counts 16-bit. */
  Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<Coverage>>
                lookaheadX;             /* Array of coverage tables
                                         * in lookahead sequence, in glyph
                                         * sequence order */
  نام_نوع Types::قالب Array_از<نام_نوع Types::HBGlyphID>
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
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    إذا (!lookahead.sanitize (c, هذا))
      return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    بیعلامت صحيح شمار;

    شمار = backtrack.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (!(هذا+backtrack[i]).intersects (glyphs))
        أرجع خطأ;

    شمار = lookahead.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (!(هذا+lookahead[i]).intersects (glyphs))
        أرجع خطأ;

    أرجع درست;
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
    hb_مجموعه_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    بیعلامت صحيح شمار = backtrack.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      hb_مجموعه_t پشت_glyphs;
      (هذا+backtrack[i]).intersect_مجموعه (*c->glyphs, پشت_glyphs);
      إذا (پشت_glyphs.get_population () == 1)
        context_glyphs.جمع_کن (پشت_glyphs.get_min ());
      وإلا إذا (پشت_glyphs.get_population () > 1)
      {
        hb_codepoint_t مجموعه_idx = c->depend_داده->پیدا_کن_or_create_context_مجموعه (پشت_glyphs);
        إذا (unlikely (مجموعه_idx == HB_CODEPOINT_INVALID))
          أرجع;
        context_glyphs.جمع_کن (HB_DEPEND_CONTEXT_مجموعه_FLAG | مجموعه_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    شمار = lookahead.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      hb_مجموعه_t look_glyphs;
      (هذا+lookahead[i]).intersect_مجموعه (*c->glyphs, look_glyphs);
      إذا (look_glyphs.get_population () == 1)
        context_glyphs.جمع_کن (look_glyphs.get_min ());
      وإلا إذا (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t مجموعه_idx = c->depend_داده->پیدا_کن_or_create_context_مجموعه (look_glyphs);
        إذا (unlikely (مجموعه_idx == HB_CODEPOINT_INVALID))
          أرجع;
        context_glyphs.جمع_کن (HB_DEPEND_CONTEXT_مجموعه_FLAG | مجموعه_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    إذا (unlikely (context_glyphs.درون_error ()))
    {
      c->depend_داده->fail ();
      أرجع;
    }
    hb_codepoint_t context_مجموعه_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_داده->پیدا_کن_or_create_context_مجموعه (context_glyphs);
    إذا (unlikely (!context_glyphs.is_empty () &&
                  context_مجموعه_idx == HB_CODEPOINT_INVALID))
      أرجع;

    hb_codepoint_t saved_context = c->depend_داده->current_context_مجموعه_شاخص;
    c->depend_داده->current_context_مجموعه_شاخص = context_مجموعه_idx;

    + hb_zip (هذا+coverage, substitute)
    | hb_filter (c->والد_فعال_glyphs (), hb_اولی)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_داده->جمع_کن_gsub_lookup (_.اولی, c->lookup_شاخص, _.second); })
    ;

    c->depend_داده->current_context_مجموعه_شاخص = saved_context;
  }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs)) أرجع;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (هذا+coverage, substitute)
    | hb_filter (c->والد_فعال_glyphs (), hb_اولی)
    | hb_map (hb_دومی)
    | hb_sink (c->output)
    ;
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->درون_داد))) أرجع;

    بیعلامت صحيح شمار;

    شمار = backtrack.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (unlikely (!(هذا+backtrack[i]).collect_coverage (c->before))) أرجع;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    شمار = lookahead.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (unlikely (!(هذا+lookahead[i]).collect_coverage (c->after))) أرجع;

    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);
    شمار = substitute.len;
    c->output->جمع_کن_array (substitute.arrayZ, substitute.len);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (c->میانگیر->cur ().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (c->nesting_level_چپ != HB_MAX_NESTING_LEVEL))
      return_trace (خطأ); /* No chaining to this type */

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    إذا (unlikely (شاخص_2 >= substitute.len)) return_trace (خطأ);

    بیعلامت صحيح شروع_شاخص = 0, انتها_شاخص = 0;
    إذا (تطابق_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         تطابق_coverage, هذا,
                         &شروع_شاخص) &&
        تطابق_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         تطابق_coverage, هذا,
                         c->میانگیر->idx + 1, &انتها_شاخص))
    {
      c->میانگیر->unsafe_مقصد_break_from_outbuffer (شروع_شاخص, انتها_شاخص);

      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->پیام (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->میانگیر->idx);
      }

      c->replace_glyph_inplace (substitute[شاخص_2]);

      إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
      {
	c->میانگیر->پیام (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->میانگیر->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (درست);
    }
    وإلا
    {
      c->میانگیر->unsafe_مقصد_concat_from_outbuffer (شروع_شاخص, انتها_شاخص);
      return_trace (خطأ);
    }
  }

  قالب<نام_نوع Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_coverage_جابه_جایی_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->serializer->شروع_embed<Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<Coverage>>> ();
    إذا (unlikely (!بیرون->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خودکار& جابه_جایی : it) {
      خودکار *o = بیرون->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, جابه_جایی, هذا))
        return_trace (خطأ);
    }

    return_trace (درست);
  }

  قالب<نام_نوع Iterator, نام_نوع BacktrackIterator, نام_نوع LookaheadIterator,
           hb_requires (hb_is_sorted_منبع_از (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  منطقي serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->embed (هذا->format))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (هذا->coverage))) return_trace (خطأ);

    إذا (!serialize_coverage_جابه_جایی_array (c, backtrack_iter)) return_trace (خطأ);
    إذا (!serialize_lookahead_coverage_جابه_جایی_array (c, lookahead_iter)) return_trace (خطأ);

    خودکار *substitute_بیرون = c->serializer->شروع_embed<نام_نوع Types::قالب Array_از<نام_نوع Types::HBGlyphID>> ();
    خودکار substitutes =
    + coverage_subst_iter
    | hb_map (hb_دومی)
    ;

    خودکار glyphs =
    + coverage_subst_iter
    | hb_map_retains_ترتیبدهی (hb_اولی)
    ;
    إذا (unlikely (! c->serializer->check_success (substitute_بیرون->serialize (c->serializer, substitutes))))
        return_trace (خطأ);

    إذا (unlikely (!بیرون->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (خطأ);
    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    خودکار it =
    + hb_zip (هذا+coverage, substitute)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (glyphset, hb_دومی)
    | hb_map_retains_ترتیبدهی ([&] (hb_pair_t<hb_codepoint_t, ثابت نام_نوع Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { أرجع hb_pair (glyph_map[p.اولی], glyph_map[p.second]); })
    ;

    return_trace (منطقي (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  قالب<نام_نوع Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_lookahead_coverage_جابه_جایی_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->serializer->شروع_embed<Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<Coverage>>> ();
    إذا (unlikely (!بیرون->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خودکار& جابه_جایی : it) {
      خودکار *o = بیرون->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, جابه_جایی, هذا))
        return_trace (خطأ);
    }

    return_trace (درست);
  }
};

}
}
}

#پایان_اگر  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
