#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

HB_INTERNAL અંતઃરેખ
શૂન્ય collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        અચલ Coverage &coverage,
                                        અચલ શૂન્ય *base,
                                        glyph_data_t offsets,
                                        unsigned ગણતરી,
                                        unsigned class_ગણતરી,
                                        અચલ hb_map_t &klass_mapping)
{
  માટે (અચલ auto _ : hb_zip (coverage, hb_range (ગણતરી)))
  {
    જો (!c->glyph_set->has (_.first)) ચાલુ;
    hb_barrier ();
    unsigned offset = offsets[_.second];
    જો (!offset) ચાલુ;

    અચલ શૂન્ય *matrix = &StructAtOffset<અચલ HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    unsigned rows = *static_cast<અચલ HBUINT16 *> (matrix);
    glyph_data_t anchors = offsets;
    anchors.data = (અચલ અક્ષર *) matrix + HBUINT16::static_size;

    hb_sorted_vector_t<unsigned> indexes;
    collect_ligature_anchor_indexes (rows, class_ગણતરી, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
template <typename Types>
સંરચના LigatureArray :
  Types::template ArrayOf<typename Types::template OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  પ્રકારવ્યાખ્યા AnchorMatrix<Types, HBUINT16> LigatureAttach;
  પ્રકારવ્યાખ્યા typename Types::template OffsetTo<LigatureAttach> LigatureAttachOffset;
  પ્રકારવ્યાખ્યા typename Types::template ArrayOf<LigatureAttachOffset> Super;

  અચલ LigatureAttach& operator [] (પૂર્ણાંક i_) અચલ
  {
    unsigned પૂર્ણાંક i = (unsigned પૂર્ણાંક) i_;
    જો (unlikely (i >= આ->len)) પાછા Null (LigatureAttach);
    hb_barrier ();
    પાછા આ+આ->arrayZ[i];
  }

  template <typename ...Ts>
  તાર્કિક sanitize (hb_sanitize_context_t *c, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (Super::sanitize (c, આ, std::forward<Ts> (ds)...));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  અચલ Coverage &coverage,
                                  unsigned class_ગણતરી,
                                  અચલ hb_map_t &klass_mapping) અચલ
  {
    collect_ligature_variation_indices (c, coverage, આ, આ->arrayZ,
                                        આ->len, class_ગણતરી, klass_mapping);
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક subset (hb_subset_context_t *c,
               Iterator             coverage,
               unsigned             class_ગણતરી,
               અચલ hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!c->serializer->extend_min (out)))  return_trace (ખોટું);

    તાર્કિક ret = ખોટું;
    માટે (અચલ auto _ : + hb_zip (coverage, *આ)
                        | hb_filter (glyph_map, hb_first))
    {
      અચલ LigatureAttach& src = (આ + _.second);
      તાર્કિક non_empty = + hb_range (src.rows * class_ગણતરી)
                       | hb_filter ([=] (unsigned index) { પાછા klass_mapping->has (index % class_ગણતરી); })
                       | hb_map ([&] (અચલ unsigned index) { પાછા !src.offset_is_null (index / class_ગણતરી, index % class_ગણતરી, class_ગણતરી); })
                       | hb_any;

      જો (!non_empty) ચાલુ;

      auto *matrix = out->serialize_append (c->serializer);
      જો (unlikely (!matrix)) return_trace (ખોટું);

      auto indexes =
          + hb_range (src.rows * class_ગણતરી)
          | hb_filter ([=] (unsigned index) { પાછા klass_mapping->has (index % class_ગણતરી); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       આ,
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
