#ifndef OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "AlternateSet.hh"
#ಅನ್ತರ್ಭಾವ "Common.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

template <typename Types>
ಸಂರಚನಾ AlternateSubstFormat1_2
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, alternateSet);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್) && alternateSet.sanitize (c, ಏತತ್));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+coverage).intersects (glyphs); }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (ಏತತ್+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<hb_codepoint_t, ಧ್ರುವ typename Types::template OffsetTo<AlternateSet<Types>>&> &_)
                {
                  ಧ್ರುವ AlternateSet<Types>& as = ಏತತ್+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    + hb_zip (ಏತತ್+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([c] (ಧ್ರುವ AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
    + hb_zip (ಏತತ್+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([c] (ಧ್ರುವ AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ c->len == 1 && (ಏತತ್+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  gid,
                        unsigned        start_offset,
                        unsigned       *alternate_ಎಣಿಕೆ  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+alternateSet[(ಏತತ್+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_ಎಣಿಕೆ, alternate_glyphs); }

  ಶೂನ್ಯ
  collect_glyph_alternates (hb_map_t  *alternate_ಎಣಿಕೆ /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ಧ್ರುವ
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_zip (ಏತತ್+coverage)
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<ಧ್ರುವ AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_ಎಣಿಕೆ, alternate_glyphs);
		})
    ;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (c->buffer->cur().codepoint);
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    return_trace ((ಏತತ್+alternateSet[index]).apply (c));
  }

  template <typename GlyphID>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!alternateSet.serialize (c, glyphs.length))) return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < glyphs.length; i++)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ alternate_len = alternate_len_list[i];
      ಯದಿ (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (ಅಸತ್ಯ);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ಏತತ್+coverage, alternateSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->alternateSet, ಏತತ್), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ತಾರ್ಕಿಕ (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
