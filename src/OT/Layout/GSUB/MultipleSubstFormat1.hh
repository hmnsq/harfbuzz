#ifndef OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#ضمن "Common.hh"
#ضمن "Sequence.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

template <typename Types>
بنية MultipleSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, sequence);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && sequence.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع true; }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.closure (c); })
    ;
  }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت typename Types::template OffsetTo<Sequence<Types>>&> &_)
                {
                  ثابت Sequence<Types>& s = هذا+_.second;
                  s.depend (c, _.first);
                })
    ;
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->كىرگۈزۈش))) أرجع;
    + hb_zip (هذا+coverage, sequence)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_apply ([c] (ثابت Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    unsigned صحيح index = (هذا+coverage).get_coverage (c->buffer->cur().codepoint);
    إذا (index == NOT_COVERED) return_trace (خطأ);

    return_trace ((هذا+sequence[index]).apply (c));
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    auto sequences =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!sequence.serialize (c, sequences.length))) return_trace (خطأ);

    لكل (auto& pair : hb_zip (sequences, sequence))
    {
      إذا (unlikely (!pair.second
		    .serialize_serialize (c, pair.first)))
        return_trace (خطأ);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, sequence)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->sequence, هذا), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
