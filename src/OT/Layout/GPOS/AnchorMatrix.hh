#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

HB_INTERNAL درونخطی
تهی collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_شمار,
                                     ثابت hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  برای (unsigned row = 0; row < rows; row++)
    برای (unsigned col = 0; col < class_شمار; col++)
      اگر (klass_mapping.has (col))
        indexes.push (row * class_شمار + col);
}

HB_INTERNAL درونخطی
تهی collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ثابت تهی *base,
                                      glyph_data_t offsets,
                                      hb_array_t<ثابت unsigned> indexes)
{
  برای (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    اگر (offset)
      StructAtOffset<ثابت Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL درونخطی
تهی collect_anchor_indexes (ثابت Coverage &coverage,
                             unsigned rows,
                             unsigned class_شمار,
                             ثابت hb_set_t &glyphset,
                             ثابت hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  برای (ثابت auto _ : hb_zip (coverage, hb_range (rows)))
    اگر (glyphset.has (_.first))
      برای (unsigned col = 0; col < class_شمار; col++)
        اگر (klass_mapping.has (col))
          indexes.push (_.second * class_شمار + col);
}

HB_INTERNAL درونخطی
تهی collect_subset_anchor_indexes (ثابت Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_شمار,
                                    glyph_data_t offsets,
                                    ثابت hb_set_t &glyphset,
                                    ثابت hb_map_t &glyph_map,
                                    ثابت hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  برای (ثابت auto _ : hb_zip (coverage, hb_range (rows)))
  {
    اگر (!glyphset.has (_.first)) ادامه;
    unsigned row = _.second;
    منطقی non_empty = نادرست;
    برای (unsigned col = 0; col < class_شمار; col++)
      اگر (klass_mapping.has (col) && offsets[row * class_شمار + col])
      {
        non_empty = درست;
        بشکن;
      }
    اگر (!non_empty) ادامه;

    new_coverage.push (glyph_map.get (_.first));
    برای (unsigned col = 0; col < class_شمار; col++)
      اگر (klass_mapping.has (col))
        indexes.push (row * class_شمار + col);
  }
}

template <typename Types, typename Row_شمار=typename Types::HBUINT>
ساختار AnchorMatrix
{
  Row_شمار      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  عمومی:
  DEFINE_SIZE_ARRAY (اندازه (Row_شمار), matrixZ);

  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح cols) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!c->check_struct (این)) return_trace (نادرست);
    hb_barrier ();
    اگر (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (نادرست);
    unsigned صحیح شمار = rows * cols;
    اگر (!c->check_array (matrixZ.arrayZ, شمار)) return_trace (نادرست);

    اگر (c->lazy_some_gpos)
      return_trace (درست);

    hb_barrier ();
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (!matrixZ[i].sanitize (c, این)) return_trace (نادرست);
    return_trace (درست);
  }

  ثابت Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned صحیح row, unsigned صحیح col,
			    unsigned صحیح cols, منطقی *found) ثابت
  {
    *found = نادرست;
    اگر (unlikely (row >= rows || col >= cols)) بازگشت Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    اگر (unlikely (!offset.sanitize (&c->sanitizer, این))) بازگشت Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    بازگشت این+offset;
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ثابت unsigned> indexes) ثابت
  {
    collect_anchor_variation_indices (c, این, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقی subset (hb_subset_context_t *c,
               unsigned             رقم_rows,
               Iterator             index_iter) ثابت
  {
    TRACE_SUBSET (این);

    auto *out = c->serializer->start_embed (این);

    اگر (!index_iter) return_trace (نادرست);
    اگر (unlikely (!c->serializer->extend_min (out)))  return_trace (نادرست);

    out->rows = رقم_rows;
    برای (ثابت unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      اگر (!offset) return_trace (نادرست);
      offset->serialize_subset (c, matrixZ[i], این);
    }

    return_trace (درست);
  }

  منطقی offset_is_null (unsigned row, unsigned col, unsigned رقم_cols) ثابت
  {
    اگر (unlikely (row >= rows || col >= رقم_cols)) بازگشت درست;
    auto &offset = matrixZ[row * رقم_cols + col];
    بازگشت offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
