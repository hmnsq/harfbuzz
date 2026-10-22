#ifndef OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#অন্তর্ভাব "AlternateSet.hh"
#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
কাঠামো AlternateSubstFormat1_2
{
  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::LOffset::static_আকার + Types::HBUINT::static_আকার, alternateSet);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (coverage.sanitize (c, এই) && alternateSet.sanitize (c, এই));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  { ফেরত (এই+coverage).intersects (glyphs); }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত মিথ্যা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (এই+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ধ্রুবক hb_pair_t<hb_codepoint_t, ধ্রুবক typename Types::template OffsetTo<AlternateSet<Types>>&> &_)
                {
                  ধ্রুবক AlternateSet<Types>& as = এই+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    + hb_zip (এই+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এই))
    | hb_apply ([c] (ধ্রুবক AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    যদি (unlikely (!(এই+coverage).collect_coverage (c->প্রবেশ))) ফেরত;
    + hb_zip (এই+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (এই))
    | hb_apply ([c] (ধ্রুবক AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  { ফেরত c->len == 1 && (এই+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  gid,
                        unsigned        start_offset,
                        unsigned       *alternate_গুনতি  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ধ্রুবক
  { ফেরত (এই+alternateSet[(এই+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_গুনতি, alternate_glyphs); }

  শূন্য
  collect_glyph_alternates (hb_map_t  *alternate_গুনতি /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ধ্রুবক
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (এই))
    | hb_zip (এই+coverage)
    | hb_apply ([&] (ধ্রুবক hb_pair_t<ধ্রুবক AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_গুনতি, alternate_glyphs);
		})
    ;
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (c->বাফার->cur().codepoint);
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    return_trace ((এই+alternateSet[সূচক_2]).apply (c));
  }

  template <typename GlyphID>
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ধ্রুবক unsigned পূর্ণসংখ্যা> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    যদি (unlikely (!alternateSet.serialize (c, glyphs.দৈর্ঘ্য))) return_trace (মিথ্যা);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < glyphs.দৈর্ঘ্য; i++)
    {
      unsigned পূর্ণসংখ্যা alternate_len = alternate_len_list[i];
      যদি (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (মিথ্যা);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এই+coverage, alternateSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->alternateSet, এই), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (যৌক্তিক (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
