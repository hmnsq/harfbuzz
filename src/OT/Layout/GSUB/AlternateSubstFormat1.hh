#ifndef OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#અન્તર્ભાવ "AlternateSet.hh"
#અન્તર્ભાવ "Common.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

template <typename Types>
સંરચના AlternateSubstFormat1_2
{
  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, alternateSet);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ) && alternateSet.sanitize (c, આ));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  { પાછા (આ+coverage).intersects (glyphs); }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા ખોટું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (આ+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (અચલ hb_pair_t<hb_codepoint_t, અચલ typename Types::template OffsetTo<AlternateSet<Types>>&> &_)
                {
                  અચલ AlternateSet<Types>& as = આ+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    + hb_zip (આ+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_apply ([c] (અચલ AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    જો (unlikely (!(આ+coverage).collect_coverage (c->આગત))) પાછા;
    + hb_zip (આ+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_apply ([c] (અચલ AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  { પાછા c->len == 1 && (આ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  gid,
                        unsigned        start_offset,
                        unsigned       *alternate_ગણતરી  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) અચલ
  { પાછા (આ+alternateSet[(આ+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_ગણતરી, alternate_glyphs); }

  શૂન્ય
  collect_glyph_alternates (hb_map_t  *alternate_ગણતરી /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) અચલ
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (આ))
    | hb_zip (આ+coverage)
    | hb_apply ([&] (અચલ hb_pair_t<અચલ AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_ગણતરી, alternate_glyphs);
		})
    ;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (c->buffer->cur().codepoint);
    જો (index == NOT_COVERED) return_trace (ખોટું);

    return_trace ((આ+alternateSet[index]).apply (c));
  }

  template <typename GlyphID>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<અચલ unsigned પૂર્ણાંક> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    જો (unlikely (!alternateSet.serialize (c, glyphs.length))) return_trace (ખોટું);
    માટે (unsigned પૂર્ણાંક i = 0; i < glyphs.length; i++)
    {
      unsigned પૂર્ણાંક alternate_len = alternate_len_list[i];
      જો (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (ખોટું);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (આ+coverage, alternateSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->alternateSet, આ), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (તાર્કિક (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
