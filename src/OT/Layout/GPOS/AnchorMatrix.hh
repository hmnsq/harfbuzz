#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_ಎಣಿಕೆ,
                                     ಧ್ರುವ hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  ಪ್ರತಿ (unsigned row = 0; row < rows; row++)
    ಪ್ರತಿ (unsigned col = 0; col < class_ಎಣಿಕೆ; col++)
      ಯದಿ (klass_mapping.has (col))
        indexes.push (row * class_ಎಣಿಕೆ + col);
}

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ಧ್ರುವ ಶೂನ್ಯ *base,
                                      glyph_data_t offsets,
                                      hb_array_t<ಧ್ರುವ unsigned> indexes)
{
  ಪ್ರತಿ (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    ಯದಿ (offset)
      StructAtOffset<ಧ್ರುವ Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ collect_anchor_indexes (ಧ್ರುವ Coverage &coverage,
                             unsigned rows,
                             unsigned class_ಎಣಿಕೆ,
                             ಧ್ರುವ hb_set_t &glyphset,
                             ಧ್ರುವ hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  ಪ್ರತಿ (ಧ್ರುವ auto _ : hb_zip (coverage, hb_range (rows)))
    ಯದಿ (glyphset.has (_.first))
      ಪ್ರತಿ (unsigned col = 0; col < class_ಎಣಿಕೆ; col++)
        ಯದಿ (klass_mapping.has (col))
          indexes.push (_.second * class_ಎಣಿಕೆ + col);
}

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ collect_subset_anchor_indexes (ಧ್ರುವ Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_ಎಣಿಕೆ,
                                    glyph_data_t offsets,
                                    ಧ್ರುವ hb_set_t &glyphset,
                                    ಧ್ರುವ hb_map_t &glyph_map,
                                    ಧ್ರುವ hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  ಪ್ರತಿ (ಧ್ರುವ auto _ : hb_zip (coverage, hb_range (rows)))
  {
    ಯದಿ (!glyphset.has (_.first)) ಅನುವರ್ತ;
    unsigned row = _.second;
    ತಾರ್ಕಿಕ non_empty = ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned col = 0; col < class_ಎಣಿಕೆ; col++)
      ಯದಿ (klass_mapping.has (col) && offsets[row * class_ಎಣಿಕೆ + col])
      {
        non_empty = ಸತ್ಯ;
        ಭಙ್ಗ;
      }
    ಯದಿ (!non_empty) ಅನುವರ್ತ;

    new_coverage.push (glyph_map.get (_.first));
    ಪ್ರತಿ (unsigned col = 0; col < class_ಎಣಿಕೆ; col++)
      ಯದಿ (klass_mapping.has (col))
        indexes.push (row * class_ಎಣಿಕೆ + col);
  }
}

template <typename Types, typename Row_ಎಣಿಕೆ=typename Types::HBUINT>
ಸಂರಚನಾ AnchorMatrix
{
  Row_ಎಣಿಕೆ      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (ಪರಿಮಾಣ (Row_ಎಣಿಕೆ), matrixZ);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ cols) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!c->check_struct (ಏತತ್)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ಯದಿ (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (ಅಸತ್ಯ);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = rows * cols;
    ಯದಿ (!c->check_array (matrixZ.arrayZ, ಎಣಿಕೆ)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (c->lazy_some_gpos)
      return_trace (ಸತ್ಯ);

    hb_barrier ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!matrixZ[i].sanitize (c, ಏತತ್)) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ಧ್ರುವ Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned ಪೂರ್ಣಾಙ್ಕ row, unsigned ಪೂರ್ಣಾಙ್ಕ col,
			    unsigned ಪೂರ್ಣಾಙ್ಕ cols, ತಾರ್ಕಿಕ *found) ಧ್ರುವ
  {
    *found = ಅಸತ್ಯ;
    ಯದಿ (unlikely (row >= rows || col >= cols)) ಪ್ರತ್ಯಾಗಮ Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    ಯದಿ (unlikely (!offset.sanitize (&c->sanitizer, ಏತತ್))) ಪ್ರತ್ಯಾಗಮ Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+offset;
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ಧ್ರುವ unsigned> indexes) ಧ್ರುವ
  {
    collect_anchor_variation_indices (c, ಏತತ್, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               unsigned             ಅಂಕೆ_rows,
               Iterator             index_iter) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    auto *out = c->serializer->start_embed (ಏತತ್);

    ಯದಿ (!index_iter) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->serializer->extend_min (out)))  return_trace (ಅಸತ್ಯ);

    out->rows = ಅಂಕೆ_rows;
    ಪ್ರತಿ (ಧ್ರುವ unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      ಯದಿ (!offset) return_trace (ಅಸತ್ಯ);
      offset->serialize_subset (c, matrixZ[i], ಏತತ್);
    }

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ offset_is_null (unsigned row, unsigned col, unsigned ಅಂಕೆ_cols) ಧ್ರುವ
  {
    ಯದಿ (unlikely (row >= rows || col >= ಅಂಕೆ_cols)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    auto &offset = matrixZ[row * ಅಂಕೆ_cols + col];
    ಪ್ರತ್ಯಾಗಮ offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
