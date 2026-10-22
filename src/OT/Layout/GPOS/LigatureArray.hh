#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ಧ್ರುವ Coverage &coverage,
                                        ಧ್ರುವ ಶೂನ್ಯ *base,
                                        glyph_data_t offsets,
                                        unsigned ಎಣಿಕೆ,
                                        unsigned class_ಎಣಿಕೆ,
                                        ಧ್ರುವ hb_map_t &klass_mapping)
{
  ಪ್ರತಿ (ಧ್ರುವ auto _ : hb_zip (coverage, hb_range (ಎಣಿಕೆ)))
  {
    ಯದಿ (!c->glyph_set->has (_.first)) ಅನುವರ್ತ;
    hb_barrier ();
    unsigned offset = offsets[_.second];
    ಯದಿ (!offset) ಅನುವರ್ತ;

    ಧ್ರುವ ಶೂನ್ಯ *matrix = &StructAtOffset<ಧ್ರುವ HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    unsigned rows = *static_cast<ಧ್ರುವ HBUINT16 *> (matrix);
    glyph_data_t anchors = offsets;
    anchors.data = (ಧ್ರುವ ವರ್ಣ *) matrix + HBUINT16::static_size;

    hb_sorted_vector_t<unsigned> indexes;
    collect_ligature_anchor_indexes (rows, class_ಎಣಿಕೆ, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
template <typename Types>
ಸಂರಚನಾ LigatureArray :
  Types::template ArrayOf<typename Types::template OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ AnchorMatrix<Types, HBUINT16> LigatureAttach;
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::template OffsetTo<LigatureAttach> LigatureAttachOffset;
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::template ArrayOf<LigatureAttachOffset> Super;

  ಧ್ರುವ LigatureAttach& operator [] (ಪೂರ್ಣಾಙ್ಕ i_) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = (unsigned ಪೂರ್ಣಾಙ್ಕ) i_;
    ಯದಿ (unlikely (i >= ಏತತ್->len)) ಪ್ರತ್ಯಾಗಮ Null (LigatureAttach);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+ಏತತ್->arrayZ[i];
  }

  template <typename ...Ts>
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (Super::sanitize (c, ಏತತ್, std::forward<Ts> (ds)...));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ಧ್ರುವ Coverage &coverage,
                                  unsigned class_ಎಣಿಕೆ,
                                  ಧ್ರುವ hb_map_t &klass_mapping) ಧ್ರುವ
  {
    collect_ligature_variation_indices (c, coverage, ಏತತ್, ಏತತ್->arrayZ,
                                        ಏತತ್->len, class_ಎಣಿಕೆ, klass_mapping);
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               Iterator             coverage,
               unsigned             class_ಎಣಿಕೆ,
               ಧ್ರುವ hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out)))  return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    ಪ್ರತಿ (ಧ್ರುವ auto _ : + hb_zip (coverage, *ಏತತ್)
                        | hb_filter (glyph_map, hb_first))
    {
      ಧ್ರುವ LigatureAttach& src = (ಏತತ್ + _.second);
      ತಾರ್ಕಿಕ non_empty = + hb_range (src.rows * class_ಎಣಿಕೆ)
                       | hb_filter ([=] (unsigned index) { ಪ್ರತ್ಯಾಗಮ klass_mapping->has (index % class_ಎಣಿಕೆ); })
                       | hb_map ([&] (ಧ್ರುವ unsigned index) { ಪ್ರತ್ಯಾಗಮ !src.offset_is_null (index / class_ಎಣಿಕೆ, index % class_ಎಣಿಕೆ, class_ಎಣಿಕೆ); })
                       | hb_any;

      ಯದಿ (!non_empty) ಅನುವರ್ತ;

      auto *matrix = out->serialize_append (c->serializer);
      ಯದಿ (unlikely (!matrix)) return_trace (ಅಸತ್ಯ);

      auto indexes =
          + hb_range (src.rows * class_ಎಣಿಕೆ)
          | hb_filter ([=] (unsigned index) { ಪ್ರತ್ಯಾಗಮ klass_mapping->has (index % class_ಎಣಿಕೆ); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       ಏತತ್,
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
