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
  עבור (unsigned row = 0; row < rows; row++)
    עבור (unsigned col = 0; col < class_count; col++)
      אם (klass_mapping.has (col))
        indexes.push (row * class_count + col);
}

HB_INTERNAL مضمن
فراغ collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ثابت فراغ *base,
                                      glyph_data_t offsets,
                                      hb_array_t<ثابت unsigned> indexes)
{
  עבור (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    אם (offset)
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
  עבור (ثابت auto _ : hb_zip (coverage, hb_range (rows)))
    אם (glyphset.has (_.first))
      עבור (unsigned col = 0; col < class_count; col++)
        אם (klass_mapping.has (col))
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
  עבור (ثابت auto _ : hb_zip (coverage, hb_range (rows)))
  {
    אם (!glyphset.has (_.first)) استمر;
    unsigned row = _.second;
    منطقي non_empty = שקר;
    עבור (unsigned col = 0; col < class_count; col++)
      אם (klass_mapping.has (col) && offsets[row * class_count + col])
      {
        non_empty = אמת;
        اكسر;
      }
    אם (!non_empty) استمر;

    new_coverage.push (glyph_map.get (_.first));
    עבור (unsigned col = 0; col < class_count; col++)
      אם (klass_mapping.has (col))
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
    אם (!c->check_struct (هذا)) return_trace (שקר);
    hb_barrier ();
    אם (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (שקר);
    unsigned صحيح count = rows * cols;
    אם (!c->check_array (matrixZ.arrayZ, count)) return_trace (שקר);

    אם (c->lazy_some_gpos)
      return_trace (אמת);

    hb_barrier ();
    עבור (unsigned صحيح i = 0; i < count; i++)
      אם (!matrixZ[i].sanitize (c, هذا)) return_trace (שקר);
    return_trace (אמת);
  }

  ثابت Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned صحيح row, unsigned صحيح col,
			    unsigned صحيح cols, منطقي *found) ثابت
  {
    *found = שקר;
    אם (unlikely (row >= rows || col >= cols)) החזר Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    אם (unlikely (!offset.sanitize (&c->sanitizer, هذا))) החזר Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    החזר هذا+offset;
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

    אם (!index_iter) return_trace (שקר);
    אם (unlikely (!c->serializer->extend_min (out)))  return_trace (שקר);

    out->rows = num_rows;
    עבור (ثابت unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      אם (!offset) return_trace (שקר);
      offset->serialize_subset (c, matrixZ[i], هذا);
    }

    return_trace (אמת);
  }

  منطقي offset_is_null (unsigned row, unsigned col, unsigned num_cols) ثابت
  {
    אם (unlikely (row >= rows || col >= num_cols)) החזר אמת;
    auto &offset = matrixZ[row * num_cols + col];
    החזר offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
