#ifndef OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#ضمن "AlternateSet.hh"
#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

template <typename Types>
بنية AlternateSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, alternateSet);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && alternateSet.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (هذا+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت typename Types::template OffsetTo<AlternateSet<Types>>&> &_)
                {
                  ثابت AlternateSet<Types>& as = هذا+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_apply ([c] (ثابت AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    𐡄𐡍 (unlikely (!(هذا+coverage).collect_coverage (c->input))) أرجع;
    + hb_zip (هذا+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_apply ([c] (ثابت AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  gid,
                        unsigned        start_offset,
                        unsigned       *alternate_count  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  { أرجع (هذا+alternateSet[(هذا+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_count, alternate_glyphs); }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_count /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (هذا))
    | hb_zip (هذا+coverage)
    | hb_apply ([&] (ثابت hb_pair_t<ثابت AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_count, alternate_glyphs);
		})
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    unsigned صحيح index = (هذا+coverage).get_coverage (c->buffer->cur().codepoint);
    𐡄𐡍 (index == NOT_COVERED) return_trace (خطأ);

    return_trace ((هذا+alternateSet[index]).apply (c));
  }

  template <typename GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ثابت unsigned صحيح> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (هذا);
    𐡄𐡍 (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    𐡄𐡍 (unlikely (!alternateSet.serialize (c, glyphs.length))) return_trace (خطأ);
    لكل (unsigned صحيح i = 0; i < glyphs.length; i++)
    {
      unsigned صحيح alternate_len = alternate_len_list[i];
      𐡄𐡍 (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (خطأ);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    𐡄𐡍 (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, alternateSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->alternateSet, هذا), hb_second)
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

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
