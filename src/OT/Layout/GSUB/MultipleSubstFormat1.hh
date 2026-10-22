#ifndef OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#شامل "Common.hh"
#شامل "Sequence.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

template <typename Types>
ساختار MultipleSubstFormat1_2
{
  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, sequence);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (coverage.sanitize (c, این) && sequence.sanitize (c, این));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  { بازگشت (این+coverage).intersects (glyphs); }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت درست; }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    + hb_zip (این+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (این))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.closure (c); })
    ;
  }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (این+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت typename Types::template OffsetTo<Sequence<Types>>&> &_)
                {
                  ثابت Sequence<Types>& s = این+_.second;
                  s.depend (c, _.first);
                })
    ;
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(این+coverage).collect_coverage (c->درون_داد))) بازگشت;
    + hb_zip (این+coverage, sequence)
    | hb_map (hb_second)
    | hb_map (hb_add (این))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  { بازگشت c->len == 1 && (این+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    unsigned صحیح index = (این+coverage).get_coverage (c->buffer->cur().codepoint);
    اگر (index == NOT_COVERED) return_trace (نادرست);

    return_trace ((این+sequence[index]).apply (c));
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (این);
    auto sequences =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);

    اگر (unlikely (!sequence.serialize (c, sequences.length))) return_trace (نادرست);

    برای (auto& pair : hb_zip (sequences, sequence))
    {
      اگر (unlikely (!pair.second
		    .serialize_serialize (c, pair.first)))
        return_trace (نادرست);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (این+coverage, sequence)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->sequence, این), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقی (new_coverage));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
