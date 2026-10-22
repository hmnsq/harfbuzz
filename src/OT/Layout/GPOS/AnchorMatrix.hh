#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_INTERNAL مضمن
فراغ collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_count,
                                     ثابت hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  لكل (unsigned row = 0; row < rows; row++)
    لكل (unsigned col = 0; col < class_count; col++)
      ܐܢ (klass_mapping.has (col))
        indexes.push (row * class_count + col);
}

HB_INTERNAL مضمن
فراغ collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ثابت فراغ *base,
                                      glyph_data_t offsets,
                                      hb_array_t<ثابت unsigned> indexes)
{
  لكل (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    ܐܢ (offset)
      StructAtOffset<ثابت Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL مضمن
فراغ collect_anchor_indexes (ثابت Coverage &coverage,
                             unsigned rows,
                             unsigned class_count,
                             ثابت hb_set_t &glyphset,
                             ثابت hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  لكل (ثابت auto _ : hb_zip (coverage, hb_range (rows)))
    ܐܢ (glyphset.has (_.first))
      لكل (unsigned col = 0; col < class_count; col++)
        ܐܢ (klass_mapping.has (col))
          indexes.push (_.second * class_count + col);
}

HB_INTERNAL مضمن
فراغ collect_subset_anchor_indexes (ثابت Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_count,
                                    glyph_data_t offsets,
                                    ثابت hb_set_t &glyphset,
                                    ثابت hb_map_t &glyph_map,
                                    ثابت hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  لكل (ثابت auto _ : hb_zip (coverage, hb_range (rows)))
  {
    ܐܢ (!glyphset.has (_.first)) استمر;
    unsigned row = _.second;
    منطقي non_empty = ܕܓܠܐ;
    لكل (unsigned col = 0; col < class_count; col++)
      ܐܢ (klass_mapping.has (col) && offsets[row * class_count + col])
      {
        non_empty = ܫܪܪܐ;
        اكسر;
      }
    ܐܢ (!non_empty) استمر;

    new_coverage.push (glyph_map.get (_.first));
    لكل (unsigned col = 0; col < class_count; col++)
      ܐܢ (klass_mapping.has (col))
        indexes.push (row * class_count + col);
  }
}

template <typename Types, typename RowCount=typename Types::HBUINT>
بنية AnchorMatrix
{
  RowCount      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  عام:
  DEFINE_SIZE_ARRAY (حجم (RowCount), matrixZ);

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح cols) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (!c->check_struct (هذا)) return_trace (ܕܓܠܐ);
    hb_barrier ();
    ܐܢ (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (ܕܓܠܐ);
    unsigned صحيح count = rows * cols;
    ܐܢ (!c->check_array (matrixZ.arrayZ, count)) return_trace (ܕܓܠܐ);

    ܐܢ (c->lazy_some_gpos)
      return_trace (ܫܪܪܐ);

    hb_barrier ();
    لكل (unsigned صحيح i = 0; i < count; i++)
      ܐܢ (!matrixZ[i].sanitize (c, هذا)) return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }

  ثابت Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned صحيح row, unsigned صحيح col,
			    unsigned صحيح cols, منطقي *found) ثابت
  {
    *found = ܕܓܠܐ;
    ܐܢ (unlikely (row >= rows || col >= cols)) ܦܢܐ Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    ܐܢ (unlikely (!offset.sanitize (&c->sanitizer, هذا))) ܦܢܐ Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    ܦܢܐ هذا+offset;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ثابت unsigned> indexes) ثابت
  {
    collect_anchor_variation_indices (c, هذا, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               unsigned             num_rows,
               Iterator             index_iter) ثابت
  {
    TRACE_SUBSET (هذا);

    auto *out = c->serializer->start_embed (هذا);

    ܐܢ (!index_iter) return_trace (ܕܓܠܐ);
    ܐܢ (unlikely (!c->serializer->extend_min (out)))  return_trace (ܕܓܠܐ);

    out->rows = num_rows;
    لكل (ثابت unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      ܐܢ (!offset) return_trace (ܕܓܠܐ);
      offset->serialize_subset (c, matrixZ[i], هذا);
    }

    return_trace (ܫܪܪܐ);
  }

  منطقي offset_is_null (unsigned row, unsigned col, unsigned num_cols) ثابت
  {
    ܐܢ (unlikely (row >= rows || col >= num_cols)) ܦܢܐ ܫܪܪܐ;
    auto &offset = matrixZ[row * num_cols + col];
    ܦܢܐ offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
