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
      𐤀𐤌 (klass_mapping.has (col))
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
    𐤀𐤌 (offset)
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
    𐤀𐤌 (glyphset.has (_.first))
      لكل (unsigned col = 0; col < class_count; col++)
        𐤀𐤌 (klass_mapping.has (col))
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
    𐤀𐤌 (!glyphset.has (_.first)) استمر;
    unsigned row = _.second;
    منطقي non_empty = خطأ;
    لكل (unsigned col = 0; col < class_count; col++)
      𐤀𐤌 (klass_mapping.has (col) && offsets[row * class_count + col])
      {
        non_empty = true;
        اكسر;
      }
    𐤀𐤌 (!non_empty) استمر;

    new_coverage.push (glyph_map.get (_.first));
    لكل (unsigned col = 0; col < class_count; col++)
      𐤀𐤌 (klass_mapping.has (col))
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
    𐤀𐤌 (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();
    𐤀𐤌 (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (خطأ);
    unsigned صحيح count = rows * cols;
    𐤀𐤌 (!c->check_array (matrixZ.arrayZ, count)) return_trace (خطأ);

    𐤀𐤌 (c->lazy_some_gpos)
      return_trace (true);

    hb_barrier ();
    لكل (unsigned صحيح i = 0; i < count; i++)
      𐤀𐤌 (!matrixZ[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (true);
  }

  ثابت Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned صحيح row, unsigned صحيح col,
			    unsigned صحيح cols, منطقي *found) ثابت
  {
    *found = خطأ;
    𐤀𐤌 (unlikely (row >= rows || col >= cols)) أرجع Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    𐤀𐤌 (unlikely (!offset.sanitize (&c->sanitizer, هذا))) أرجع Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    أرجع هذا+offset;
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

    𐤀𐤌 (!index_iter) return_trace (خطأ);
    𐤀𐤌 (unlikely (!c->serializer->extend_min (out)))  return_trace (خطأ);

    out->rows = num_rows;
    لكل (ثابت unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      𐤀𐤌 (!offset) return_trace (خطأ);
      offset->serialize_subset (c, matrixZ[i], هذا);
    }

    return_trace (true);
  }

  منطقي offset_is_null (unsigned row, unsigned col, unsigned num_cols) ثابت
  {
    𐤀𐤌 (unlikely (row >= rows || col >= num_cols)) أرجع true;
    auto &offset = matrixZ[row * num_cols + col];
    أرجع offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
