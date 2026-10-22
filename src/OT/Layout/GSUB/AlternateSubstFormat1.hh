#ifndef OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#अन्तर्भाव "AlternateSet.hh"
#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

ढाँचा <typename Types>
संरचना AlternateSubstFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, alternateSet);

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && alternateSet.sanitize (c, एतत्));
  }

  तार्किक intersects (अचर hb_set_t *glyphs) अचर
  { फर्काऊ (एतत्+coverage).intersects (glyphs); }

  तार्किक may_have_non_1to1 () अचर
  { फर्काऊ असत्य; }

  शून्य depend (hb_depend_context_t *c) अचर
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (एतत्+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (अचर hb_pair_t<hb_codepoint_t, अचर typename Types::ढाँचा OffsetTo<AlternateSet<Types>>&> &_)
                {
                  अचर AlternateSet<Types>& as = एतत्+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  शून्य closure (hb_closure_context_t *c) अचर
  {
    + hb_zip (एतत्+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (अचर AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) अचर {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) अचर
  {
    यदि (unlikely (!(एतत्+coverage).collect_coverage (c->आगत))) फर्काऊ;
    + hb_zip (एतत्+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (अचर AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  अचर Coverage &get_coverage () अचर { फर्काऊ एतत्+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) अचर
  { फर्काऊ c->len == 1 && (एतत्+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  अचिन्हित
  get_glyph_alternates (hb_codepoint_t  gid,
                        अचिन्हित        start_offset,
                        अचिन्हित       *alternate_गन्ती  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) अचर
  { फर्काऊ (एतत्+alternateSet[(एतत्+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_गन्ती, alternate_glyphs); }

  शून्य
  collect_glyph_alternates (hb_map_t  *alternate_गन्ती /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) अचर
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (एतत्))
    | hb_zip (एतत्+coverage)
    | hb_apply ([&] (अचर hb_pair_t<अचर AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_गन्ती, alternate_glyphs);
		})
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c) अचर
  {
    TRACE_APPLY (एतत्);

    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->buffer->cur().codepoint);
    यदि (index == NOT_COVERED) return_trace (असत्य);

    return_trace ((एतत्+alternateSet[index]).apply (c));
  }

  ढाँचा <typename GlyphID>
  तार्किक serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<अचर अचिन्हित पूर्णाङ्क> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    यदि (unlikely (!alternateSet.serialize (c, glyphs.length))) return_trace (असत्य);
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < glyphs.length; i++)
    {
      अचिन्हित पूर्णाङ्क alternate_len = alternate_len_list[i];
      यदि (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (असत्य);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
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
