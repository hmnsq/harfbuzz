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
  अत्थाय (unsigned row = 0; row < rows; row++)
    अत्थाय (unsigned col = 0; col < class_गणनम्; col++)
      सचे (klass_mapping.has (col))
        indexes.push (row * class_गणनम् + col);
}

HB_INTERNAL अन्तःस्थ
शून्य collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ध्रुव शून्य *base,
                                      glyph_data_t offsets,
                                      hb_array_t<ध्रुव unsigned> indexes)
{
  अत्थाय (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    सचे (offset)
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
  अत्थाय (ध्रुव auto _ : hb_zip (coverage, hb_range (rows)))
    सचे (glyphset.has (_.first))
      अत्थाय (unsigned col = 0; col < class_गणनम्; col++)
        सचे (klass_mapping.has (col))
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
  अत्थाय (ध्रुव auto _ : hb_zip (coverage, hb_range (rows)))
  {
    सचे (!glyphset.has (_.first)) अनुवर्त;
    unsigned row = _.second;
    तार्किक non_empty = मिच्छा;
    अत्थाय (unsigned col = 0; col < class_गणनम्; col++)
      सचे (klass_mapping.has (col) && offsets[row * class_गणनम् + col])
      {
        non_empty = सच्च;
        भङ्ग;
      }
    सचे (!non_empty) अनुवर्त;

    new_coverage.push (glyph_map.get (_.first));
    अत्थाय (unsigned col = 0; col < class_गणनम्; col++)
      सचे (klass_mapping.has (col))
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
    सचे (!c->check_struct (एतत्)) return_trace (मिच्छा);
    hb_barrier ();
    सचे (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (मिच्छा);
    unsigned पूर्णाङ्क गणनम् = rows * cols;
    सचे (!c->check_array (matrixZ.arrayZ, गणनम्)) return_trace (मिच्छा);

    सचे (c->lazy_some_gpos)
      return_trace (सच्च);

    hb_barrier ();
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (!matrixZ[i].sanitize (c, एतत्)) return_trace (मिच्छा);
    return_trace (सच्च);
  }

  ध्रुव Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned पूर्णाङ्क row, unsigned पूर्णाङ्क col,
			    unsigned पूर्णाङ्क cols, तार्किक *found) ध्रुव
  {
    *found = मिच्छा;
    सचे (unlikely (row >= rows || col >= cols)) पटिनिवत्त Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    सचे (unlikely (!offset.sanitize (&c->sanitizer, एतत्))) पटिनिवत्त Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    पटिनिवत्त एतत्+offset;
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

    सचे (!index_iter) return_trace (मिच्छा);
    सचे (unlikely (!c->serializer->extend_min (out)))  return_trace (मिच्छा);

    out->rows = अङ्कः_rows;
    अत्थाय (ध्रुव unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      सचे (!offset) return_trace (मिच्छा);
      offset->serialize_subset (c, matrixZ[i], एतत्);
    }

    return_trace (सच्च);
  }

  तार्किक offset_is_null (unsigned row, unsigned col, unsigned अङ्कः_cols) ध्रुव
  {
    सचे (unlikely (row >= rows || col >= अङ्कः_cols)) पटिनिवत्त सच्च;
    auto &offset = matrixZ[row * अङ्कः_cols + col];
    पटिनिवत्त offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
