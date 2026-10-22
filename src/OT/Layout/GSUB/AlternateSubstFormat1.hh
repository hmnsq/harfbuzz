#ifndef OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#شامل "AlternateSet.hh"
#شامل "Common.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

template <typename Types>
ساختار AlternateSubstFormat1_2
{
  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, alternateSet);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (coverage.sanitize (c, این) && alternateSet.sanitize (c, این));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  { بازگشت (این+coverage).intersects (glyphs); }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت نادرست; }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (این+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت typename Types::template OffsetTo<AlternateSet<Types>>&> &_)
                {
                  ثابت AlternateSet<Types>& as = این+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    + hb_zip (این+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (این))
    | hb_apply ([c] (ثابت AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(این+coverage).collect_coverage (c->درون_داد))) بازگشت;
    + hb_zip (این+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (این))
    | hb_apply ([c] (ثابت AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  { بازگشت c->len == 1 && (این+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  gid,
                        unsigned        start_offset,
                        unsigned       *alternate_شمار  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  { بازگشت (این+alternateSet[(این+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_شمار, alternate_glyphs); }

  تهی
  collect_glyph_alternates (hb_map_t  *alternate_شمار /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (این))
    | hb_zip (این+coverage)
    | hb_apply ([&] (ثابت hb_pair_t<ثابت AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_شمار, alternate_glyphs);
		})
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    unsigned صحیح index = (این+coverage).get_coverage (c->buffer->cur().codepoint);
    اگر (index == NOT_COVERED) return_trace (نادرست);

    return_trace ((این+alternateSet[index]).apply (c));
  }

  template <typename GlyphID>
  منطقی serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ثابت unsigned صحیح> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    اگر (unlikely (!alternateSet.serialize (c, glyphs.length))) return_trace (نادرست);
    برای (unsigned صحیح i = 0; i < glyphs.length; i++)
    {
      unsigned صحیح alternate_len = alternate_len_list[i];
      اگر (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (نادرست);
      alternate_glyphs_list += alternate_len;
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
    + hb_zip (این+coverage, alternateSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->alternateSet, این), hb_second)
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

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
