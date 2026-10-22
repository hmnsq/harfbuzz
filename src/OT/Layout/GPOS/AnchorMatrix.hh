#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

HB_INTERNAL অন্তঃরেখ
শূন্য collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_গুনতি,
                                     ধ্রুবক hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  জন্য (unsigned row = 0; row < rows; row++)
    জন্য (unsigned col = 0; col < class_গুনতি; col++)
      যদি (klass_mapping.has (col))
        indexes.push (row * class_গুনতি + col);
}

HB_INTERNAL অন্তঃরেখ
শূন্য collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ধ্রুবক শূন্য *base,
                                      glyph_তথ্য_t offsets,
                                      hb_array_t<ধ্রুবক unsigned> indexes)
{
  জন্য (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    যদি (offset)
      StructAtOffset<ধ্রুবক Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL অন্তঃরেখ
শূন্য collect_anchor_indexes (ধ্রুবক Coverage &coverage,
                             unsigned rows,
                             unsigned class_গুনতি,
                             ধ্রুবক hb_set_t &glyphset,
                             ধ্রুবক hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  জন্য (ধ্রুবক auto _ : hb_zip (coverage, hb_range (rows)))
    যদি (glyphset.has (_.first))
      জন্য (unsigned col = 0; col < class_গুনতি; col++)
        যদি (klass_mapping.has (col))
          indexes.push (_.second * class_গুনতি + col);
}

HB_INTERNAL অন্তঃরেখ
শূন্য collect_subset_anchor_indexes (ধ্রুবক Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_গুনতি,
                                    glyph_তথ্য_t offsets,
                                    ধ্রুবক hb_set_t &glyphset,
                                    ধ্রুবক hb_map_t &glyph_map,
                                    ধ্রুবক hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  জন্য (ধ্রুবক auto _ : hb_zip (coverage, hb_range (rows)))
  {
    যদি (!glyphset.has (_.first)) চালু;
    unsigned row = _.second;
    যৌক্তিক non_empty = মিথ্যা;
    জন্য (unsigned col = 0; col < class_গুনতি; col++)
      যদি (klass_mapping.has (col) && offsets[row * class_গুনতি + col])
      {
        non_empty = সত্য;
        বিরতি;
      }
    যদি (!non_empty) চালু;

    new_coverage.push (glyph_map.get (_.first));
    জন্য (unsigned col = 0; col < class_গুনতি; col++)
      যদি (klass_mapping.has (col))
        indexes.push (row * class_গুনতি + col);
  }
}

template <typename Types, typename Row_গুনতি=typename Types::HBUINT>
কাঠামো AnchorMatrix
{
  Row_গুনতি      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (আকার (Row_গুনতি), matrixZ);

  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা cols) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!c->check_struct (এই)) return_trace (মিথ্যা);
    hb_barrier ();
    যদি (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (মিথ্যা);
    unsigned পূর্ণসংখ্যা গুনতি = rows * cols;
    যদি (!c->check_array (matrixZ.arrayZ, গুনতি)) return_trace (মিথ্যা);

    যদি (c->lazy_some_gpos)
      return_trace (সত্য);

    hb_barrier ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (!matrixZ[i].sanitize (c, এই)) return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  ধ্রুবক Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned পূর্ণসংখ্যা row, unsigned পূর্ণসংখ্যা col,
			    unsigned পূর্ণসংখ্যা cols, যৌক্তিক *found) ধ্রুবক
  {
    *found = মিথ্যা;
    যদি (unlikely (row >= rows || col >= cols)) ফেরত Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    যদি (unlikely (!offset.sanitize (&c->sanitizer, এই))) ফেরত Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    ফেরত এই+offset;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ধ্রুবক unsigned> indexes) ধ্রুবক
  {
    collect_anchor_variation_indices (c, এই, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক subset (hb_subset_context_t *c,
               unsigned             অঙ্ক_rows,
               Iterator             সূচক_iter) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    auto *out = c->serializer->start_embed (এই);

    যদি (!সূচক_iter) return_trace (মিথ্যা);
    যদি (unlikely (!c->serializer->extend_min (out)))  return_trace (মিথ্যা);

    out->rows = অঙ্ক_rows;
    জন্য (ধ্রুবক unsigned i : সূচক_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      যদি (!offset) return_trace (মিথ্যা);
      offset->serialize_subset (c, matrixZ[i], এই);
    }

    return_trace (সত্য);
  }

  যৌক্তিক offset_is_null (unsigned row, unsigned col, unsigned অঙ্ক_cols) ধ্রুবক
  {
    যদি (unlikely (row >= rows || col >= অঙ্ক_cols)) ফেরত সত্য;
    auto &offset = matrixZ[row * অঙ্ক_cols + col];
    ফেরত offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
