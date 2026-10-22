#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

HB_INTERNAL உள்வரி
வெறுமை collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_மொத்தம்,
                                     மாறிலி hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  க்கு (unsigned row = 0; row < rows; row++)
    க்கு (unsigned col = 0; col < class_மொத்தம்; col++)
      எனில் (klass_mapping.has (col))
        indexes.push (row * class_மொத்தம் + col);
}

HB_INTERNAL உள்வரி
வெறுமை collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      மாறிலி வெறுமை *base,
                                      glyph_data_t offsets,
                                      hb_array_t<மாறிலி unsigned> indexes)
{
  க்கு (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    எனில் (offset)
      StructAtOffset<மாறிலி Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL உள்வரி
வெறுமை collect_anchor_indexes (மாறிலி Coverage &coverage,
                             unsigned rows,
                             unsigned class_மொத்தம்,
                             மாறிலி hb_set_t &glyphset,
                             மாறிலி hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  க்கு (மாறிலி auto _ : hb_zip (coverage, hb_range (rows)))
    எனில் (glyphset.has (_.first))
      க்கு (unsigned col = 0; col < class_மொத்தம்; col++)
        எனில் (klass_mapping.has (col))
          indexes.push (_.second * class_மொத்தம் + col);
}

HB_INTERNAL உள்வரி
வெறுமை collect_subset_anchor_indexes (மாறிலி Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_மொத்தம்,
                                    glyph_data_t offsets,
                                    மாறிலி hb_set_t &glyphset,
                                    மாறிலி hb_map_t &glyph_map,
                                    மாறிலி hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  க்கு (மாறிலி auto _ : hb_zip (coverage, hb_range (rows)))
  {
    எனில் (!glyphset.has (_.first)) தொடர்;
    unsigned row = _.second;
    தருக்க non_empty = பொய்;
    க்கு (unsigned col = 0; col < class_மொத்தம்; col++)
      எனில் (klass_mapping.has (col) && offsets[row * class_மொத்தம் + col])
      {
        non_empty = உண்மை;
        நிறுத்து;
      }
    எனில் (!non_empty) தொடர்;

    new_coverage.push (glyph_map.get (_.first));
    க்கு (unsigned col = 0; col < class_மொத்தம்; col++)
      எனில் (klass_mapping.has (col))
        indexes.push (row * class_மொத்தம் + col);
  }
}

template <typename Types, typename Row_மொத்தம்=typename Types::HBUINT>
கட்டமைப்பு AnchorMatrix
{
  Row_மொத்தம்      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  பொது:
  DEFINE_SIZE_ARRAY (அளவு (Row_மொத்தம்), matrixZ);

  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு cols) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!c->check_struct (இது)) return_trace (பொய்);
    hb_barrier ();
    எனில் (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (பொய்);
    unsigned முழு மொத்தம் = rows * cols;
    எனில் (!c->check_array (matrixZ.arrayZ, மொத்தம்)) return_trace (பொய்);

    எனில் (c->lazy_some_gpos)
      return_trace (உண்மை);

    hb_barrier ();
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (!matrixZ[i].sanitize (c, இது)) return_trace (பொய்);
    return_trace (உண்மை);
  }

  மாறிலி Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned முழு row, unsigned முழு col,
			    unsigned முழு cols, தருக்க *found) மாறிலி
  {
    *found = பொய்;
    எனில் (unlikely (row >= rows || col >= cols)) திரும்பு Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    எனில் (unlikely (!offset.sanitize (&c->sanitizer, இது))) திரும்பு Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    திரும்பு இது+offset;
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<மாறிலி unsigned> indexes) மாறிலி
  {
    collect_anchor_variation_indices (c, இது, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  தருக்க subset (hb_subset_context_t *c,
               unsigned             இலக்கம்_rows,
               Iterator             index_iter) மாறிலி
  {
    TRACE_SUBSET (இது);

    auto *out = c->serializer->start_embed (இது);

    எனில் (!index_iter) return_trace (பொய்);
    எனில் (unlikely (!c->serializer->extend_min (out)))  return_trace (பொய்);

    out->rows = இலக்கம்_rows;
    க்கு (மாறிலி unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      எனில் (!offset) return_trace (பொய்);
      offset->serialize_subset (c, matrixZ[i], இது);
    }

    return_trace (உண்மை);
  }

  தருக்க offset_is_null (unsigned row, unsigned col, unsigned இலக்கம்_cols) மாறிலி
  {
    எனில் (unlikely (row >= rows || col >= இலக்கம்_cols)) திரும்பு உண்மை;
    auto &offset = matrixZ[row * இலக்கம்_cols + col];
    திரும்பு offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
