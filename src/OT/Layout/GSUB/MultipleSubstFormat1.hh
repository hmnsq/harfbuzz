#جے_بنا_تعریف OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#ضمن "Common.hh"
#ضمن "Sequence.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچا <قسم_ناں Types>
بنية مضرب_Subst_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::سانچا وتھ_منزل<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_ناپ_ARRAY (2 + Types::LOffset::static_ناپ + Types::HBUINT::static_ناپ, sequence);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && sequence.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (c->ماپا_سرگرم_glyphs (), hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.گھیرا (c); })
    ;
  }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (c->ماپا_سرگرم_glyphs (), hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت قسم_ناں Types::سانچا وتھ_منزل<Sequence<Types>>&> &_)
                {
                  ثابت Sequence<Types>& s = هذا+_.second;
                  s.depend (c, _.پہلا);
                })
    ;
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلہ))) أرجع;
    + hb_zip (هذا+coverage, sequence)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    return_trace ((هذا+sequence[اشاریہ_2]).apply (c));
  }

  سانچا<قسم_ناں Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    خودکار sequences =
      + it
      | hb_map (hb_دوجا)
      ;
    خودکار glyphs =
      + it
      | hb_map_retains_چھانٹی (hb_پہلا)
      ;
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!sequence.serialize (c, sequences.لمائی))) return_trace (خطأ);

    لكل (خودکار& pair : hb_zip (sequences, sequence))
    {
      إذا (unlikely (!pair.second
		    .serialize_serialize (c, pair.پہلا)))
        return_trace (خطأ);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_وتھ_array (c, باہرلا->sequence, هذا), hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    باہرلا->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }
};

}
}
}


#جے_انت /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
