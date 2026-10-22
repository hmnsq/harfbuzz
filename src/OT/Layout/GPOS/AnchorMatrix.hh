#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

HB_INTERNAL অন্তঃস্থ
শূন্য collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_গুনতি,
                                     ধ্রুব hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  প্রতি (unsigned row = 0; row < rows; row++)
    প্রতি (unsigned col = 0; col < class_গুনতি; col++)
      যদি (klass_mapping.has (col))
        indexes.push (row * class_গুনতি + col);
}

HB_INTERNAL অন্তঃস্থ
শূন্য collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ধ্রুব শূন্য *base,
                                      glyph_data_t offsets,
                                      hb_array_t<ধ্রুব unsigned> indexes)
{
  প্রতি (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    যদি (offset)
      StructAtOffset<ধ্রুব Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL অন্তঃস্থ
শূন্য collect_anchor_indexes (ধ্রুব Coverage &coverage,
                             unsigned rows,
                             unsigned class_গুনতি,
                             ধ্রুব hb_set_t &glyphset,
                             ধ্রুব hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  প্রতি (ধ্রুব auto _ : hb_zip (coverage, hb_range (rows)))
    যদি (glyphset.has (_.first))
      প্রতি (unsigned col = 0; col < class_গুনতি; col++)
        যদি (klass_mapping.has (col))
          indexes.push (_.second * class_গুনতি + col);
}

HB_INTERNAL অন্তঃস্থ
শূন্য collect_subset_anchor_indexes (ধ্রুব Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_গুনতি,
                                    glyph_data_t offsets,
                                    ধ্রুব hb_set_t &glyphset,
                                    ধ্রুব hb_map_t &glyph_map,
                                    ধ্রুব hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  প্রতি (ধ্রুব auto _ : hb_zip (coverage, hb_range (rows)))
  {
    যদি (!glyphset.has (_.first)) অনুবর্ত;
    unsigned row = _.second;
    তার্কিক non_empty = অসত্য;
    প্রতি (unsigned col = 0; col < class_গুনতি; col++)
      যদি (klass_mapping.has (col) && offsets[row * class_গুনতি + col])
      {
        non_empty = সত্য;
        ভঙ্গ;
      }
    যদি (!non_empty) অনুবর্ত;

    new_coverage.push (glyph_map.get (_.first));
    প্রতি (unsigned col = 0; col < class_গুনতি; col++)
      যদি (klass_mapping.has (col))
        indexes.push (row * class_গুনতি + col);
  }
}

template <typename Types, typename Row_গুনতি=typename Types::HBUINT>
সংরচনা AnchorMatrix
{
  Row_গুনতি      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (পরিমাণ (Row_গুনতি), matrixZ);

  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক cols) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!c->check_struct (এতত্)) return_trace (অসত্য);
    hb_barrier ();
    যদি (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (অসত্য);
    unsigned পূর্ণাঙ্ক গুনতি = rows * cols;
    যদি (!c->check_array (matrixZ.arrayZ, গুনতি)) return_trace (অসত্য);

    যদি (c->lazy_some_gpos)
      return_trace (সত্য);

    hb_barrier ();
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (!matrixZ[i].sanitize (c, এতত্)) return_trace (অসত্য);
    return_trace (সত্য);
  }

  ধ্রুব Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned পূর্ণাঙ্ক row, unsigned পূর্ণাঙ্ক col,
			    unsigned পূর্ণাঙ্ক cols, তার্কিক *found) ধ্রুব
  {
    *found = অসত্য;
    যদি (unlikely (row >= rows || col >= cols)) প্রত্যাগম Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    যদি (unlikely (!offset.sanitize (&c->sanitizer, এতত্))) প্রত্যাগম Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    প্রত্যাগম এতত্+offset;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ধ্রুব unsigned> indexes) ধ্রুব
  {
    collect_anchor_variation_indices (c, এতত্, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  তার্কিক subset (hb_subset_context_t *c,
               unsigned             অঙ্ক_rows,
               Iterator             index_iter) ধ্রুব
  {
    TRACE_SUBSET (এতত্);

    auto *out = c->serializer->start_embed (এতত্);

    যদি (!index_iter) return_trace (অসত্য);
    যদি (unlikely (!c->serializer->extend_min (out)))  return_trace (অসত্য);

    out->rows = অঙ্ক_rows;
    প্রতি (ধ্রুব unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      যদি (!offset) return_trace (অসত্য);
      offset->serialize_subset (c, matrixZ[i], এতত্);
    }

    return_trace (সত্য);
  }

  তার্কিক offset_is_null (unsigned row, unsigned col, unsigned অঙ্ক_cols) ধ্রুব
  {
    যদি (unlikely (row >= rows || col >= অঙ্ক_cols)) প্রত্যাগম সত্য;
    auto &offset = matrixZ[row * অঙ্ক_cols + col];
    প্রত্যাগম offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
