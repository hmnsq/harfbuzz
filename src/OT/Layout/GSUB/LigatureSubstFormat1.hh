#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "LigatureSet.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

ढाँचा <typename Types>
संरचना LigatureSubstFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ligatureSet);

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && ligatureSet.sanitize (c, एतत्));
  }

  तार्किक intersects (अचर hb_set_t *glyphs) अचर
  {
    फर्काऊ
    + hb_zip (एतत्+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([एतत्, glyphs] (अचर typename Types::ढाँचा OffsetTo<LigatureSet<Types>> &_)
              { फर्काऊ (एतत्+_).intersects (glyphs); })
    | hb_any
    ;
  }

  तार्किक may_have_non_1to1 () अचर
  { फर्काऊ सत्य; }

  शून्य depend (hb_depend_context_t *c) अचर
  {
    + hb_zip (एतत्+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (अचर hb_pair_t<hb_codepoint_t, अचर typename Types::ढाँचा OffsetTo<LigatureSet<Types>>&> &_)
                {
                  अचर LigatureSet<Types>& ls = एतत्+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  शून्य closure (hb_closure_context_t *c) अचर
  {
    + hb_zip (एतत्+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (अचर LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) अचर {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) अचर
  {
    यदि (unlikely (!(एतत्+coverage).collect_coverage (c->आगत))) फर्काऊ;

    + hb_zip (एतत्+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (अचर LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) अचर
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (एतत्))
    | hb_apply ([digest] (अचर LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  अचर Coverage &get_coverage () अचर { फर्काऊ एतत्+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) अचर
  {
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->glyphs[0]);
    यदि (likely (index == NOT_COVERED)) फर्काऊ असत्य;

    अचर स्वतः &lig_set = एतत्+ligatureSet[index];
    फर्काऊ lig_set.would_apply (c);
  }

  संरचना external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  शून्य *external_cache_create () अचर
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (परिमाण (external_cache_t));
    यदि (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (एतत्))
      | hb_apply ([cache] (अचर LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    फर्काऊ cache;
  }

  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) अचर
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचर hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचर hb_set_digest_t *seconds = nullptr;
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (buffer->cur().codepoint);
#endif
    यदि (index == NOT_COVERED) return_trace (असत्य);

    अचर स्वतः &lig_set = एतत्+ligatureSet[index];
    return_trace (lig_set.apply (c, seconds));
  }

  ढाँचा <typename GlyphID>
  तार्किक serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<अचर अचिन्हित पूर्णाङ्क> ligature_per_first_glyph_गन्ती_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<अचर अचिन्हित पूर्णाङ्क> component_गन्ती_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    यदि (unlikely (!ligatureSet.serialize (c, first_glyphs.length))) return_trace (असत्य);
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < first_glyphs.length; i++)
    {
      अचिन्हित पूर्णाङ्क ligature_गन्ती = ligature_per_first_glyph_गन्ती_list[i];
      यदि (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_गन्ती),
                                              component_गन्ती_list.sub_array (0, ligature_गन्ती),
                                              component_list))) return_trace (असत्य);
      ligatures_list += ligature_गन्ती;
      component_गन्ती_list += ligature_गन्ती;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (एतत्+coverage, hb_iter (ligatureSet) | hb_map (hb_add (एतत्)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (अचर LigatureSet<Types>& _) {
      फर्काऊ _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    यदि (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->झिक_discard ();
      return_trace (असत्य);
    }

    अचिन्हित coverage_idx = c->serializer->झिक_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (एतत्+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, एतत्, coverage_idx))
    ;

    return_trace (तार्किक (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
