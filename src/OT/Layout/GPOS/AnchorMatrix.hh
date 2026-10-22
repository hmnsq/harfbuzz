#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

HB_INTERNAL अन्तःस्थ
शून्य collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_गणनम्,
                                     ध्रुव hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  प्रति (unsigned row = 0; row < rows; row++)
    प्रति (unsigned col = 0; col < class_गणनम्; col++)
      जइ (klass_mapping.has (col))
        indexes.push (row * class_गणनम् + col);
}

HB_INTERNAL अन्तःस्थ
शून्य collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ध्रुव शून्य *base,
                                      glyph_data_t offsets,
                                      hb_array_t<ध्रुव unsigned> indexes)
{
  प्रति (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    जइ (offset)
      StructAtOffset<ध्रुव Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL अन्तःस्थ
शून्य collect_anchor_indexes (ध्रुव Coverage &coverage,
                             unsigned rows,
                             unsigned class_गणनम्,
                             ध्रुव hb_set_t &glyphset,
                             ध्रुव hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  प्रति (ध्रुव auto _ : hb_zip (coverage, hb_range (rows)))
    जइ (glyphset.has (_.first))
      प्रति (unsigned col = 0; col < class_गणनम्; col++)
        जइ (klass_mapping.has (col))
          indexes.push (_.second * class_गणनम् + col);
}

HB_INTERNAL अन्तःस्थ
शून्य collect_subset_anchor_indexes (ध्रुव Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_गणनम्,
                                    glyph_data_t offsets,
                                    ध्रुव hb_set_t &glyphset,
                                    ध्रुव hb_map_t &glyph_map,
                                    ध्रुव hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  प्रति (ध्रुव auto _ : hb_zip (coverage, hb_range (rows)))
  {
    जइ (!glyphset.has (_.first)) अनुवर्त;
    unsigned row = _.second;
    तार्किक non_empty = मिच्छ;
    प्रति (unsigned col = 0; col < class_गणनम्; col++)
      जइ (klass_mapping.has (col) && offsets[row * class_गणनम् + col])
      {
        non_empty = सच्च;
        भङ्ग;
      }
    जइ (!non_empty) अनुवर्त;

    new_coverage.push (glyph_map.get (_.first));
    प्रति (unsigned col = 0; col < class_गणनम्; col++)
      जइ (klass_mapping.has (col))
        indexes.push (row * class_गणनम् + col);
  }
}

template <typename Types, typename Row_गणनम्=typename Types::HBUINT>
संरचना AnchorMatrix
{
  Row_गणनम्      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (परिमाण (Row_गणनम्), matrixZ);

  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क cols) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (!c->check_struct (एतत्)) return_trace (मिच्छ);
    hb_barrier ();
    जइ (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (मिच्छ);
    unsigned पूर्णाङ्क गणनम् = rows * cols;
    जइ (!c->check_array (matrixZ.arrayZ, गणनम्)) return_trace (मिच्छ);

    जइ (c->lazy_some_gpos)
      return_trace (सच्च);

    hb_barrier ();
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      जइ (!matrixZ[i].sanitize (c, एतत्)) return_trace (मिच्छ);
    return_trace (सच्च);
  }

  ध्रुव Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned पूर्णाङ्क row, unsigned पूर्णाङ्क col,
			    unsigned पूर्णाङ्क cols, तार्किक *found) ध्रुव
  {
    *found = मिच्छ;
    जइ (unlikely (row >= rows || col >= cols)) प्रत्यागम Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    जइ (unlikely (!offset.sanitize (&c->sanitizer, एतत्))) प्रत्यागम Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    प्रत्यागम एतत्+offset;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ध्रुव unsigned> indexes) ध्रुव
  {
    collect_anchor_variation_indices (c, एतत्, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  तार्किक subset (hb_subset_context_t *c,
               unsigned             अङ्कः_rows,
               Iterator             index_iter) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    auto *out = c->serializer->start_embed (एतत्);

    जइ (!index_iter) return_trace (मिच्छ);
    जइ (unlikely (!c->serializer->extend_min (out)))  return_trace (मिच्छ);

    out->rows = अङ्कः_rows;
    प्रति (ध्रुव unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      जइ (!offset) return_trace (मिच्छ);
      offset->serialize_subset (c, matrixZ[i], एतत्);
    }

    return_trace (सच्च);
  }

  तार्किक offset_is_null (unsigned row, unsigned col, unsigned अङ्कः_cols) ध्रुव
  {
    जइ (unlikely (row >= rows || col >= अङ्कः_cols)) प्रत्यागम सच्च;
    auto &offset = matrixZ[row * अङ्कः_cols + col];
    प्रत्यागम offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
