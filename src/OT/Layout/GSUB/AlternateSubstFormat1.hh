#ifndef OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#অন্তর্ভাৱ "AlternateSet.hh"
#অন্তর্ভাৱ "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
সংরচনা AlternateSubstFormat1_2
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, alternateSet);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্) && alternateSet.sanitize (c, এতত্));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  { ঘূৰাই (এতত্+coverage).intersects (glyphs); }

  তার্কিক may_have_non_1to1 () ধ্রুৱ
  { ঘূৰাই মিছা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুৱ
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (এতত্+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<hb_codepoint_t, ধ্রুৱ typename Types::template OffsetTo<AlternateSet<Types>>&> &_)
                {
                  ধ্রুৱ AlternateSet<Types>& as = এতত্+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    + hb_zip (এতত্+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_apply ([c] (ধ্রুৱ AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    যদি (unlikely (!(এতত্+coverage).collect_coverage (c->প্ৰবেশ))) ঘূৰাই;
    + hb_zip (এতত্+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_apply ([c] (ধ্রুৱ AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+coverage; }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  { ঘূৰাই c->len == 1 && (এতত্+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  gid,
                        unsigned        start_offset,
                        unsigned       *alternate_গুনতি  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ধ্রুৱ
  { ঘূৰাই (এতত্+alternateSet[(এতত্+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_গুনতি, alternate_glyphs); }

  শূন্য
  collect_glyph_alternates (hb_map_t  *alternate_গুনতি /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ধ্রুৱ
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (এতত্))
    | hb_zip (এতত্+coverage)
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<ধ্রুৱ AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_গুনতি, alternate_glyphs);
		})
    ;
  }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);

    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (c->buffer->cur().codepoint);
    যদি (index == NOT_COVERED) return_trace (মিছা);

    return_trace ((এতত্+alternateSet[index]).apply (c));
  }

  template <typename GlyphID>
  তার্কিক serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ধ্রুৱ unsigned পূর্ণাঙ্ক> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    যদি (unlikely (!alternateSet.serialize (c, glyphs.length))) return_trace (মিছা);
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < glyphs.length; i++)
    {
      unsigned পূর্ণাঙ্ক alternate_len = alternate_len_list[i];
      যদি (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (মিছা);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুৱ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এতত্+coverage, alternateSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->alternateSet, এতত্), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (তার্কিক (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
