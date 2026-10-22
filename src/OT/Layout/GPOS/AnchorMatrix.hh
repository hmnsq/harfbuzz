#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

HB_INTERNAL অন্তঃস্থ
শূন্য collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_গুনতি,
                                     ধ্রুৱ hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  বাবে (unsigned row = 0; row < rows; row++)
    বাবে (unsigned col = 0; col < class_গুনতি; col++)
      যদি (klass_mapping.has (col))
        indexes.push (row * class_গুনতি + col);
}

HB_INTERNAL অন্তঃস্থ
শূন্য collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ধ্রুৱ শূন্য *base,
                                      glyph_data_t offsets,
                                      hb_array_t<ধ্রুৱ unsigned> indexes)
{
  বাবে (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    যদি (offset)
      StructAtOffset<ধ্রুৱ Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL অন্তঃস্থ
শূন্য collect_anchor_indexes (ধ্রুৱ Coverage &coverage,
                             unsigned rows,
                             unsigned class_গুনতি,
                             ধ্রুৱ hb_set_t &glyphset,
                             ধ্রুৱ hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  বাবে (ধ্রুৱ auto _ : hb_zip (coverage, hb_range (rows)))
    যদি (glyphset.has (_.first))
      বাবে (unsigned col = 0; col < class_গুনতি; col++)
        যদি (klass_mapping.has (col))
          indexes.push (_.second * class_গুনতি + col);
}

HB_INTERNAL অন্তঃস্থ
শূন্য collect_subset_anchor_indexes (ধ্রুৱ Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_গুনতি,
                                    glyph_data_t offsets,
                                    ধ্রুৱ hb_set_t &glyphset,
                                    ধ্রুৱ hb_map_t &glyph_map,
                                    ধ্রুৱ hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  বাবে (ধ্রুৱ auto _ : hb_zip (coverage, hb_range (rows)))
  {
    যদি (!glyphset.has (_.first)) অনুৱর্ত;
    unsigned row = _.second;
    তার্কিক non_empty = মিছা;
    বাবে (unsigned col = 0; col < class_গুনতি; col++)
      যদি (klass_mapping.has (col) && offsets[row * class_গুনতি + col])
      {
        non_empty = সঁচা;
        ভঙ্গ;
      }
    যদি (!non_empty) অনুৱর্ত;

    new_coverage.push (glyph_map.get (_.first));
    বাবে (unsigned col = 0; col < class_গুনতি; col++)
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
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (পরিমাণ (Row_গুনতি), matrixZ);

  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক cols) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!c->check_struct (এতত্)) return_trace (মিছা);
    hb_barrier ();
    যদি (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (মিছা);
    unsigned পূর্ণাঙ্ক গুনতি = rows * cols;
    যদি (!c->check_array (matrixZ.arrayZ, গুনতি)) return_trace (মিছা);

    যদি (c->lazy_some_gpos)
      return_trace (সঁচা);

    hb_barrier ();
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (!matrixZ[i].sanitize (c, এতত্)) return_trace (মিছা);
    return_trace (সঁচা);
  }

  ধ্রুৱ Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned পূর্ণাঙ্ক row, unsigned পূর্ণাঙ্ক col,
			    unsigned পূর্ণাঙ্ক cols, তার্কিক *found) ধ্রুৱ
  {
    *found = মিছা;
    যদি (unlikely (row >= rows || col >= cols)) ঘূৰাই Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    যদি (unlikely (!offset.sanitize (&c->sanitizer, এতত্))) ঘূৰাই Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    ঘূৰাই এতত্+offset;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ধ্রুৱ unsigned> indexes) ধ্রুৱ
  {
    collect_anchor_variation_indices (c, এতত্, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  তার্কিক subset (hb_subset_context_t *c,
               unsigned             অঙ্ক_rows,
               Iterator             index_iter) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);

    auto *out = c->serializer->start_embed (এতত্);

    যদি (!index_iter) return_trace (মিছা);
    যদি (unlikely (!c->serializer->extend_min (out)))  return_trace (মিছা);

    out->rows = অঙ্ক_rows;
    বাবে (ধ্রুৱ unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      যদি (!offset) return_trace (মিছা);
      offset->serialize_subset (c, matrixZ[i], এতত্);
    }

    return_trace (সঁচা);
  }

  তার্কিক offset_is_null (unsigned row, unsigned col, unsigned অঙ্ক_cols) ধ্রুৱ
  {
    যদি (unlikely (row >= rows || col >= অঙ্ক_cols)) ঘূৰাই সঁচা;
    auto &offset = matrixZ[row * অঙ্ক_cols + col];
    ঘূৰাই offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
