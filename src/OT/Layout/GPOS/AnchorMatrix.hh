#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_ଗଣନା,
                                     ଅଚଳ hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  ପାଇଁ (unsigned row = 0; row < rows; row++)
    ପାଇଁ (unsigned col = 0; col < class_ଗଣନା; col++)
      ଯଦି (klass_mapping.has (col))
        indexes.push (row * class_ଗଣନା + col);
}

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ଅଚଳ ଶୂନ୍ୟ *base,
                                      glyph_data_t offsets,
                                      hb_array_t<ଅଚଳ unsigned> indexes)
{
  ପାଇଁ (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    ଯଦି (offset)
      StructAtOffset<ଅଚଳ Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ collect_anchor_indexes (ଅଚଳ Coverage &coverage,
                             unsigned rows,
                             unsigned class_ଗଣନା,
                             ଅଚଳ hb_set_t &glyphset,
                             ଅଚଳ hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  ପାଇଁ (ଅଚଳ auto _ : hb_zip (coverage, hb_range (rows)))
    ଯଦି (glyphset.has (_.first))
      ପାଇଁ (unsigned col = 0; col < class_ଗଣନା; col++)
        ଯଦି (klass_mapping.has (col))
          indexes.push (_.second * class_ଗଣନା + col);
}

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ collect_subset_anchor_indexes (ଅଚଳ Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_ଗଣନା,
                                    glyph_data_t offsets,
                                    ଅଚଳ hb_set_t &glyphset,
                                    ଅଚଳ hb_map_t &glyph_map,
                                    ଅଚଳ hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  ପାଇଁ (ଅଚଳ auto _ : hb_zip (coverage, hb_range (rows)))
  {
    ଯଦି (!glyphset.has (_.first)) ଜାରି;
    unsigned row = _.second;
    ତାର୍କିକ non_empty = ମିଥ୍ୟା;
    ପାଇଁ (unsigned col = 0; col < class_ଗଣନା; col++)
      ଯଦି (klass_mapping.has (col) && offsets[row * class_ଗଣନା + col])
      {
        non_empty = ସତ୍ୟ;
        ଭାଙ୍ଗ;
      }
    ଯଦି (!non_empty) ଜାରି;

    new_coverage.push (glyph_map.get (_.first));
    ପାଇଁ (unsigned col = 0; col < class_ଗଣନା; col++)
      ଯଦି (klass_mapping.has (col))
        indexes.push (row * class_ଗଣନା + col);
  }
}

template <typename Types, typename Row_ଗଣନା=typename Types::HBUINT>
ସଂରଚନା AnchorMatrix
{
  Row_ଗଣନା      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (ଆକାର (Row_ଗଣନା), matrixZ);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!c->check_struct (ଏହି)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ଯଦି (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (ମିଥ୍ୟା);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = rows * cols;
    ଯଦି (!c->check_array (matrixZ.arrayZ, ଗଣନା_2)) return_trace (ମିଥ୍ୟା);

    ଯଦି (c->lazy_some_gpos)
      return_trace (ସତ୍ୟ);

    hb_barrier ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!matrixZ[i].sanitize (c, ଏହି)) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ଅଚଳ Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା row, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା col,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ତାର୍କିକ *found) ଅଚଳ
  {
    *found = ମିଥ୍ୟା;
    ଯଦି (unlikely (row >= rows || col >= cols)) ଫେରନ୍ତୁ Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    ଯଦି (unlikely (!offset.sanitize (&c->sanitizer, ଏହି))) ଫେରନ୍ତୁ Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    ଫେରନ୍ତୁ ଏହି+offset;
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ଅଚଳ unsigned> indexes) ଅଚଳ
  {
    collect_anchor_variation_indices (c, ଏହି, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ subset (hb_subset_context_t *c,
               unsigned             ଅଙ୍କ_rows,
               Iterator             index_iter) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    auto *out = c->serializer->start_embed (ଏହି);

    ଯଦି (!index_iter) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->serializer->extend_min (out)))  return_trace (ମିଥ୍ୟା);

    out->rows = ଅଙ୍କ_rows;
    ପାଇଁ (ଅଚଳ unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      ଯଦି (!offset) return_trace (ମିଥ୍ୟା);
      offset->serialize_subset (c, matrixZ[i], ଏହି);
    }

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ offset_is_null (unsigned row, unsigned col, unsigned ଅଙ୍କ_cols) ଅଚଳ
  {
    ଯଦି (unlikely (row >= rows || col >= ଅଙ୍କ_cols)) ଫେରନ୍ତୁ ସତ୍ୟ;
    auto &offset = matrixZ[row * ଅଙ୍କ_cols + col];
    ଫେରନ୍ତୁ offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
