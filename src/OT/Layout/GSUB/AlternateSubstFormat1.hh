#ifndef OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#അന്തര്ഭാവ "AlternateSet.hh"
#അന്തര്ഭാവ "Common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

template <typename Types>
ഘടന AlternateSubstFormat1_2
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, alternateSet);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്) && alternateSet.sanitize (c, ഇത്));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ (ഇത്+coverage).intersects (glyphs); }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ അസത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (ഇത്+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<hb_codepoint_t, സ്ഥിരം typename Types::template OffsetTo<AlternateSet<Types>>&> &_)
                {
                  സ്ഥിരം AlternateSet<Types>& as = ഇത്+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    + hb_zip (ഇത്+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([c] (സ്ഥിരം AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(ഇത്+coverage).collect_coverage (c->നിവേശം))) തിരികെ;
    + hb_zip (ഇത്+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([c] (സ്ഥിരം AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  { തിരികെ c->len == 1 && (ഇത്+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  gid,
                        unsigned        start_offset,
                        unsigned       *alternate_എണ്ണം  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) സ്ഥിരം
  { തിരികെ (ഇത്+alternateSet[(ഇത്+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_എണ്ണം, alternate_glyphs); }

  ശൂന്യം
  collect_glyph_alternates (hb_map_t  *alternate_എണ്ണം /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) സ്ഥിരം
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (ഇത്))
    | hb_zip (ഇത്+coverage)
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<സ്ഥിരം AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_എണ്ണം, alternate_glyphs);
		})
    ;
  }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (c->buffer->cur().codepoint);
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    return_trace ((ഇത്+alternateSet[index]).apply (c));
  }

  template <typename GlyphID>
  തർക്കം serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!alternateSet.serialize (c, glyphs.length))) return_trace (അസത്യം);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < glyphs.length; i++)
    {
      unsigned പൂർണ്ണസംഖ്യ alternate_len = alternate_len_list[i];
      എങ്കിൽ (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (അസത്യം);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (ഇത്+coverage, alternateSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->alternateSet, ഇത്), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (തർക്കം (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
