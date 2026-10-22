#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#شامل "Common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

سانچہ <قسم_نام Types>
ساخت الٹ_Chain_Single_Subst_Format1_2
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of table */
  Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Coverage>>
                backtrack;              /* Array of coverage tables
                                         * in backtracking sequence, in glyph
                                         * sequence order */
  /* Format 2 widens the backtrack and lookahead coverage offsets but
   * keeps their counts 16-bit. */
  Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Coverage>>
                lookaheadX;             /* Array of coverage tables
                                         * in lookahead sequence, in glyph
                                         * sequence order */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::HBGlyphID>
                substituteX;            /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */
  عوامی:
  DEFINE_سائز_MIN (6 + Types::LOffset::static_سائز + Types::HBUINT::static_سائز);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!(coverage.sanitize (c, یہ) && backtrack.sanitize (c, یہ)))
      return_trace (جھوٹ);
    hb_barrier ();
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    اگر (!lookahead.sanitize (c, یہ))
      return_trace (جھوٹ);
    hb_barrier ();
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    اگر (!(یہ+coverage).intersects (glyphs))
      واپس جھوٹ;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    غیرعلامتی صحیح گنتی;

    گنتی = backtrack.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (!(یہ+backtrack[i]).intersects (glyphs))
        واپس جھوٹ;

    گنتی = lookahead.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (!(یہ+lookahead[i]).intersects (glyphs))
        واپس جھوٹ;

    واپس سچ;
  }

  منطقی may_have_non_1to1 () ثابت
  { واپس جھوٹ; }

  خالی depend (hb_depend_context_t *c) ثابت {
    // Filter by intersects and parent_active_glyphs like closure does
    اگر (!intersects (c->glyphs)) واپس;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_سیٹ_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    غیرعلامتی صحیح گنتی = backtrack.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      hb_سیٹ_t عقب_glyphs;
      (یہ+backtrack[i]).intersect_سیٹ (*c->glyphs, عقب_glyphs);
      اگر (عقب_glyphs.get_population () == 1)
        context_glyphs.جوڑو (عقب_glyphs.get_min ());
      ورنہ اگر (عقب_glyphs.get_population () > 1)
      {
        hb_codepoint_t سیٹ_idx = c->depend_ڈیٹا->ڈھونڈو_or_create_context_سیٹ (عقب_glyphs);
        اگر (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
          واپس;
        context_glyphs.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    گنتی = lookahead.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      hb_سیٹ_t look_glyphs;
      (یہ+lookahead[i]).intersect_سیٹ (*c->glyphs, look_glyphs);
      اگر (look_glyphs.get_population () == 1)
        context_glyphs.جوڑو (look_glyphs.get_min ());
      ورنہ اگر (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t سیٹ_idx = c->depend_ڈیٹا->ڈھونڈو_or_create_context_سیٹ (look_glyphs);
        اگر (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
          واپس;
        context_glyphs.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    اگر (unlikely (context_glyphs.اندر_error ()))
    {
      c->depend_ڈیٹا->fail ();
      واپس;
    }
    hb_codepoint_t context_سیٹ_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_ڈیٹا->ڈھونڈو_or_create_context_سیٹ (context_glyphs);
    اگر (unlikely (!context_glyphs.is_empty () &&
                  context_سیٹ_idx == HB_CODEPOINT_INVALID))
      واپس;

    hb_codepoint_t saved_context = c->depend_ڈیٹا->current_context_سیٹ_اشاریہ;
    c->depend_ڈیٹا->current_context_سیٹ_اشاریہ = context_سیٹ_idx;

    + hb_zip (یہ+coverage, substitute)
    | hb_filter (c->والد_فعال_glyphs (), hb_پہلا)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_ڈیٹا->جوڑو_gsub_lookup (_.پہلا, c->lookup_اشاریہ, _.second); })
    ;

    c->depend_ڈیٹا->current_context_سیٹ_اشاریہ = saved_context;
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    اگر (!intersects (c->glyphs)) واپس;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (یہ+coverage, substitute)
    | hb_filter (c->والد_فعال_glyphs (), hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_sink (c->output)
    ;
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(یہ+coverage).collect_coverage (c->داخلہ))) واپس;

    غیرعلامتی صحیح گنتی;

    گنتی = backtrack.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (unlikely (!(یہ+backtrack[i]).collect_coverage (c->before))) واپس;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    گنتی = lookahead.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (unlikely (!(یہ+lookahead[i]).collect_coverage (c->after))) واپس;

    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);
    گنتی = substitute.len;
    c->output->جوڑو_array (substitute.arrayZ, substitute.len);
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  { واپس c->len == 1 && (یہ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (c->بفر->cur ().codepoint);
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    اگر (unlikely (c->nesting_level_بایاں != HB_MAX_NESTING_LEVEL))
      return_trace (جھوٹ); /* No chaining to this type */

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    اگر (unlikely (اشاریہ_2 >= substitute.len)) return_trace (جھوٹ);

    غیرعلامتی صحیح آغاز_اشاریہ = 0, انتہا_اشاریہ = 0;
    اگر (مطابقت_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         مطابقت_coverage, یہ,
                         &آغاز_اشاریہ) &&
        مطابقت_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         مطابقت_coverage, یہ,
                         c->بفر->idx + 1, &انتہا_اشاریہ))
    {
      c->بفر->unsafe_مقصد_break_from_outbuffer (آغاز_اشاریہ, انتہا_اشاریہ);

      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->بفر->idx);
      }

      c->replace_glyph_inplace (substitute[اشاریہ_2]);

      اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
      {
	c->بفر->پیغام (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->بفر->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (سچ);
    }
    ورنہ
    {
      c->بفر->unsafe_مقصد_concat_from_outbuffer (آغاز_اشاریہ, انتہا_اشاریہ);
      return_trace (جھوٹ);
    }
  }

  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_coverage_انحراف_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed<Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Coverage>>> ();
    اگر (unlikely (!برآمد->serialize (c->serializer, 0u))) return_trace (جھوٹ);

    برائے (خودکار& انحراف : it) {
      خودکار *o = برآمد->serialize_append (c->serializer);
      اگر (unlikely (!o) || !o->serialize_subset (c, انحراف, یہ))
        return_trace (جھوٹ);
    }

    return_trace (سچ);
  }

  سانچہ<قسم_نام Iterator, قسم_نام BacktrackIterator, قسم_نام LookaheadIterator,
           hb_requires (hb_is_sorted_ماخذ_کا (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  منطقی serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) ثابت
  {
    TRACE_SERIALIZE (یہ);

    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->serializer->embed (یہ->format))) return_trace (جھوٹ);
    اگر (unlikely (!c->serializer->embed (یہ->coverage))) return_trace (جھوٹ);

    اگر (!serialize_coverage_انحراف_array (c, backtrack_iter)) return_trace (جھوٹ);
    اگر (!serialize_lookahead_coverage_انحراف_array (c, lookahead_iter)) return_trace (جھوٹ);

    خودکار *substitute_برآمد = c->serializer->آغاز_embed<قسم_نام Types::سانچہ Array_کا<قسم_نام Types::HBGlyphID>> ();
    خودکار substitutes =
    + coverage_subst_iter
    | hb_map (hb_دوسرا)
    ;

    خودکار glyphs =
    + coverage_subst_iter
    | hb_map_retains_ترتیب_کاری (hb_پہلا)
    ;
    اگر (unlikely (! c->serializer->check_success (substitute_برآمد->serialize (c->serializer, substitutes))))
        return_trace (جھوٹ);

    اگر (unlikely (!برآمد->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (جھوٹ);
    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ثابت خودکار &substitute = StructAfter<decltype (substituteX)> (lookahead);

    خودکار it =
    + hb_zip (یہ+coverage, substitute)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (glyphset, hb_دوسرا)
    | hb_map_retains_ترتیب_کاری ([&] (hb_pair_t<hb_codepoint_t, ثابت قسم_نام Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { واپس hb_pair (glyph_map[p.پہلا], glyph_map[p.second]); })
    ;

    return_trace (منطقی (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_lookahead_coverage_انحراف_array (hb_subset_context_t *c, Iterator it) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed<Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Coverage>>> ();
    اگر (unlikely (!برآمد->serialize (c->serializer, 0u))) return_trace (جھوٹ);

    برائے (خودکار& انحراف : it) {
      خودکار *o = برآمد->serialize_append (c->serializer);
      اگر (unlikely (!o) || !o->serialize_subset (c, انحراف, یہ))
        return_trace (جھوٹ);
    }

    return_trace (سچ);
  }
};

}
}
}

#اگر_ختم  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
