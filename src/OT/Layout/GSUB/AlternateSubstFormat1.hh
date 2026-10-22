#ifndef OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#अन्तर्भाव "AlternateSet.hh"
#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

template <typename Types>
संरचना AlternateSubstFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, alternateSet);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && alternateSet.sanitize (c, एतत्));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम (एतत्+coverage).intersects (glyphs); }

  तार्किक may_have_non_1to1 () ध्रुव
  { प्रत्यागम मिच्छ; }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (एतत्+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ध्रुव hb_pair_t<hb_codepoint_t, ध्रुव typename Types::template OffsetTo<AlternateSet<Types>>&> &_)
                {
                  ध्रुव AlternateSet<Types>& as = एतत्+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    + hb_zip (एतत्+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (ध्रुव AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    जइ (unlikely (!(एतत्+coverage).collect_coverage (c->निवेशः))) प्रत्यागम;
    + hb_zip (एतत्+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (ध्रुव AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  { प्रत्यागम c->len == 1 && (एतत्+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  gid,
                        unsigned        start_offset,
                        unsigned       *alternate_गणनम्  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ध्रुव
  { प्रत्यागम (एतत्+alternateSet[(एतत्+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_गणनम्, alternate_glyphs); }

  शून्य
  collect_glyph_alternates (hb_map_t  *alternate_गणनम् /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ध्रुव
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (एतत्))
    | hb_zip (एतत्+coverage)
    | hb_apply ([&] (ध्रुव hb_pair_t<ध्रुव AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_गणनम्, alternate_glyphs);
		})
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->buffer->cur().codepoint);
    जइ (index == NOT_COVERED) return_trace (मिच्छ);

    return_trace ((एतत्+alternateSet[index]).apply (c));
  }

  template <typename GlyphID>
  तार्किक serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ध्रुव unsigned पूर्णाङ्क> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (एतत्);
    जइ (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छ);
    जइ (unlikely (!alternateSet.serialize (c, glyphs.length))) return_trace (मिच्छ);
    प्रति (unsigned पूर्णाङ्क i = 0; i < glyphs.length; i++)
    {
      unsigned पूर्णाङ्क alternate_len = alternate_len_list[i];
      जइ (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (मिच्छ);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+coverage, alternateSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->alternateSet, एतत्), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (तार्किक (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
