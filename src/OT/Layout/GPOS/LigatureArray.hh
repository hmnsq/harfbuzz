#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

HB_INTERNAL വരിയിൽ
ശൂന്യം collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        സ്ഥിരം Coverage &coverage,
                                        സ്ഥിരം ശൂന്യം *base,
                                        glyph_data_t offsets,
                                        unsigned എണ്ണം,
                                        unsigned class_എണ്ണം,
                                        സ്ഥിരം hb_map_t &klass_mapping)
{
  വേണ്ടി (സ്ഥിരം auto _ : hb_zip (coverage, hb_range (എണ്ണം)))
  {
    എങ്കിൽ (!c->glyph_set->has (_.first)) തുടരുക;
    hb_barrier ();
    unsigned offset = offsets[_.second];
    എങ്കിൽ (!offset) തുടരുക;

    സ്ഥിരം ശൂന്യം *matrix = &StructAtOffset<സ്ഥിരം HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    unsigned rows = *static_cast<സ്ഥിരം HBUINT16 *> (matrix);
    glyph_data_t anchors = offsets;
    anchors.data = (സ്ഥിരം അക്ഷരം *) matrix + HBUINT16::static_size;

    hb_sorted_vector_t<unsigned> indexes;
    collect_ligature_anchor_indexes (rows, class_എണ്ണം, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
template <typename Types>
ഘടന LigatureArray :
  Types::template ArrayOf<typename Types::template OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  തരനിർവചനം AnchorMatrix<Types, HBUINT16> LigatureAttach;
  തരനിർവചനം typename Types::template OffsetTo<LigatureAttach> LigatureAttachOffset;
  തരനിർവചനം typename Types::template ArrayOf<LigatureAttachOffset> Super;

  സ്ഥിരം LigatureAttach& operator [] (പൂർണ്ണസംഖ്യ i_) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i = (unsigned പൂർണ്ണസംഖ്യ) i_;
    എങ്കിൽ (unlikely (i >= ഇത്->len)) തിരികെ Null (LigatureAttach);
    hb_barrier ();
    തിരികെ ഇത്+ഇത്->arrayZ[i];
  }

  template <typename ...Ts>
  തർക്കം sanitize (hb_sanitize_context_t *c, Ts&&... ds) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (Super::sanitize (c, ഇത്, std::forward<Ts> (ds)...));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  സ്ഥിരം Coverage &coverage,
                                  unsigned class_എണ്ണം,
                                  സ്ഥിരം hb_map_t &klass_mapping) സ്ഥിരം
  {
    collect_ligature_variation_indices (c, coverage, ഇത്, ഇത്->arrayZ,
                                        ഇത്->len, class_എണ്ണം, klass_mapping);
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  തർക്കം subset (hb_subset_context_t *c,
               Iterator             coverage,
               unsigned             class_എണ്ണം,
               സ്ഥിരം hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out)))  return_trace (അസത്യം);

    തർക്കം ret = അസത്യം;
    വേണ്ടി (സ്ഥിരം auto _ : + hb_zip (coverage, *ഇത്)
                        | hb_filter (glyph_map, hb_first))
    {
      സ്ഥിരം LigatureAttach& src = (ഇത് + _.second);
      തർക്കം non_empty = + hb_range (src.rows * class_എണ്ണം)
                       | hb_filter ([=] (unsigned index) { തിരികെ klass_mapping->has (index % class_എണ്ണം); })
                       | hb_map ([&] (സ്ഥിരം unsigned index) { തിരികെ !src.offset_is_null (index / class_എണ്ണം, index % class_എണ്ണം, class_എണ്ണം); })
                       | hb_any;

      എങ്കിൽ (!non_empty) തുടരുക;

      auto *matrix = out->serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!matrix)) return_trace (അസത്യം);

      auto indexes =
          + hb_range (src.rows * class_എണ്ണം)
          | hb_filter ([=] (unsigned index) { തിരികെ klass_mapping->has (index % class_എണ്ണം); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       ഇത്,
				       src.rows,
				       indexes);

      hb_codepoint_t new_gid = glyph_map.get (_.first);
      new_coverage.push (new_gid);
    }
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_LIGATUREARRAY_HH */
