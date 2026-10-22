#ifndef OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#অন্তর্ভাব "Common.hh"
#অন্তর্ভাব "Sequence.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
সংরচনা MultipleSubstFormat1_2
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, sequence);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্) && sequence.sanitize (c, এতত্));
  }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs) ধ্রুব
  { প্রত্যাগম (এতত্+coverage).intersects (glyphs); }

  তার্কিক may_have_non_1to1 () ধ্রুব
  { প্রত্যাগম সত্য; }

  শূন্য closure (hb_closure_context_t *c) ধ্রুব
  {
    + hb_zip (এতত্+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_apply ([c] (ধ্রুব Sequence<Types> &_) { _.closure (c); })
    ;
  }

  শূন্য depend (hb_depend_context_t *c) ধ্রুব
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (এতত্+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ধ্রুব hb_pair_t<hb_codepoint_t, ধ্রুব typename Types::template OffsetTo<Sequence<Types>>&> &_)
                {
                  ধ্রুব Sequence<Types>& s = এতত্+_.second;
                  s.depend (c, _.first);
                })
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুব {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুব
  {
    যদি (unlikely (!(এতত্+coverage).collect_coverage (c->প্রবেশ))) প্রত্যাগম;
    + hb_zip (এতত্+coverage, sequence)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_apply ([c] (ধ্রুব Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ধ্রুব Coverage &get_coverage () ধ্রুব { প্রত্যাগম এতত্+coverage; }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুব
  { প্রত্যাগম c->len == 1 && (এতত্+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (c->buffer->cur().codepoint);
    যদি (index == NOT_COVERED) return_trace (অসত্য);

    return_trace ((এতত্+sequence[index]).apply (c));
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (এতত্);
    auto sequences =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (অসত্য);

    যদি (unlikely (!sequence.serialize (c, sequences.length))) return_trace (অসত্য);

    প্রতি (auto& pair : hb_zip (sequences, sequence))
    {
      যদি (unlikely (!pair.second
		    .serialize_serialize (c, pair.first)))
        return_trace (অসত্য);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এতত্+coverage, sequence)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->sequence, এতত্), hb_second)
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


#endif /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
