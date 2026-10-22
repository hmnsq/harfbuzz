#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_INTERNAL مضمن
فراغ collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_گنتی,
                                     ثابت hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  لكل (unsigned row = 0; row < rows; row++)
    لكل (unsigned col = 0; col < class_گنتی; col++)
      إذا (klass_mapping.has (col))
        indexes.push (row * class_گنتی + col);
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
    إذا (offset)
      StructAtOffset<ثابت Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL مضمن
فراغ collect_anchor_indexes (ثابت Coverage &coverage,
                             unsigned rows,
                             unsigned class_گنتی,
                             ثابت hb_set_t &glyphset,
                             ثابت hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  لكل (ثابت auto _ : hb_zip (coverage, hb_range (rows)))
    إذا (glyphset.has (_.first))
      لكل (unsigned col = 0; col < class_گنتی; col++)
        إذا (klass_mapping.has (col))
          indexes.push (_.second * class_گنتی + col);
}

HB_INTERNAL مضمن
فراغ collect_subset_anchor_indexes (ثابت Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_گنتی,
                                    glyph_data_t offsets,
                                    ثابت hb_set_t &glyphset,
                                    ثابت hb_map_t &glyph_map,
                                    ثابت hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  لكل (ثابت auto _ : hb_zip (coverage, hb_range (rows)))
  {
    إذا (!glyphset.has (_.first)) استمر;
    unsigned row = _.second;
    منطقي non_empty = خطأ;
    لكل (unsigned col = 0; col < class_گنتی; col++)
      إذا (klass_mapping.has (col) && offsets[row * class_گنتی + col])
      {
        non_empty = true;
        اكسر;
      }
    إذا (!non_empty) استمر;

    new_coverage.push (glyph_map.get (_.first));
    لكل (unsigned col = 0; col < class_گنتی; col++)
      إذا (klass_mapping.has (col))
        indexes.push (row * class_گنتی + col);
  }
}

template <typename Types, typename Row_گنتی=typename Types::HBUINT>
بنية AnchorMatrix
{
  Row_گنتی      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  عام:
  DEFINE_SIZE_ARRAY (حجم (Row_گنتی), matrixZ);

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح cols) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (خطأ);
    unsigned صحيح گنتی = rows * cols;
    إذا (!c->check_array (matrixZ.arrayZ, گنتی)) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (true);

    hb_barrier ();
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
      إذا (!matrixZ[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (true);
  }

  ثابت Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned صحيح row, unsigned صحيح col,
			    unsigned صحيح cols, منطقي *found) ثابت
  {
    *found = خطأ;
    إذا (unlikely (row >= rows || col >= cols)) أرجع Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    إذا (unlikely (!offset.sanitize (&c->sanitizer, هذا))) أرجع Null (Anchor);
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
               unsigned             عدد_rows,
               Iterator             index_iter) ثابت
  {
    TRACE_SUBSET (هذا);

    auto *out = c->serializer->start_embed (هذا);

    إذا (!index_iter) return_trace (خطأ);
    إذا (unlikely (!c->serializer->extend_min (out)))  return_trace (خطأ);

    out->rows = عدد_rows;
    لكل (ثابت unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      إذا (!offset) return_trace (خطأ);
      offset->serialize_subset (c, matrixZ[i], هذا);
    }

    return_trace (true);
  }

  منطقي offset_is_null (unsigned row, unsigned col, unsigned عدد_cols) ثابت
  {
    إذا (unlikely (row >= rows || col >= عدد_cols)) أرجع true;
    auto &offset = matrixZ[row * عدد_cols + col];
    أرجع offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
