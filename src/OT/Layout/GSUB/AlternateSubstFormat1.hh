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
  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, alternateSet);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (coverage.sanitize (c, हे) && alternateSet.sanitize (c, हे));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  { परत (हे+coverage).intersects (glyphs); }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत असत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (हे+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (स्थिरांक hb_pair_t<hb_codepoint_t, स्थिरांक typename Types::ढाँचा OffsetTo<AlternateSet<Types>>&> &_)
                {
                  स्थिरांक AlternateSet<Types>& as = हे+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    + hb_zip (हे+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (हे))
    | hb_apply ([c] (स्थिरांक AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    जर (unlikely (!(हे+coverage).collect_coverage (c->आदान))) परत;
    + hb_zip (हे+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (हे))
    | hb_apply ([c] (स्थिरांक AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  { परत c->len == 1 && (हे+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  अचिन्हित
  get_glyph_alternates (hb_codepoint_t  gid,
                        अचिन्हित        start_offset,
                        अचिन्हित       *alternate_मोजणी  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) स्थिरांक
  { परत (हे+alternateSet[(हे+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_मोजणी, alternate_glyphs); }

  शून्य
  collect_glyph_alternates (hb_map_t  *alternate_मोजणी /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) स्थिरांक
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (हे))
    | hb_zip (हे+coverage)
    | hb_apply ([&] (स्थिरांक hb_pair_t<स्थिरांक AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_मोजणी, alternate_glyphs);
		})
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (c->buffer->cur().codepoint);
    जर (index == NOT_COVERED) return_trace (असत्य);

    return_trace ((हे+alternateSet[index]).apply (c));
  }

  ढाँचा <typename GlyphID>
  तार्किक serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<स्थिरांक अचिन्हित पूर्णांक> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    जर (unlikely (!alternateSet.serialize (c, glyphs.length))) return_trace (असत्य);
    साठी (अचिन्हित पूर्णांक i = 0; i < glyphs.length; i++)
    {
      अचिन्हित पूर्णांक alternate_len = alternate_len_list[i];
      जर (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (असत्य);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (हे+coverage, alternateSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->alternateSet, हे), hb_second)
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
