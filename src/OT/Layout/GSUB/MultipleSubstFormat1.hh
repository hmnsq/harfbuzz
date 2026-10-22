#که_نه_تعریف_شوی OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#ضمن "Common.hh"
#ضمن "Sequence.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

کينډۍ <ډول_نوم Types>
بنية مضرب_Subst_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, sequence);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && sequence.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع ریښتیا; }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (c->پلار_فعال_glyphs (), hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.تړون (c); })
    ;
  }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (c->پلار_فعال_glyphs (), hb_لومړی)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت ډول_نوم Types::کينډۍ دننه_واټن_منزل<Sequence<Types>>&> &_)
                {
                  ثابت Sequence<Types>& s = هذا+_.second;
                  s.depend (c, _.لومړی);
                })
    ;
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->ننوت))) أرجع;
    + hb_zip (هذا+coverage, sequence)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    return_trace ((هذا+sequence[شاخص_2]).apply (c));
  }

  کينډۍ<ډول_نوم Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری sequences =
      + it
      | hb_map (hb_دوهم)
      ;
    خپلکاری glyphs =
      + it
      | hb_map_retains_ترتیبونه (hb_لومړی)
      ;
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!sequence.serialize (c, sequences.اوږدتیا))) return_trace (خطأ);

    لكل (خپلکاری& pair : hb_zip (sequences, sequence))
    {
      إذا (unlikely (!pair.second
		    .serialize_serialize (c, pair.لومړی)))
        return_trace (خطأ);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (subset_دننه_واټن_array (c, بهر->sequence, هذا), hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    بهر->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }
};

}
}
}


#د_که_پای /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
