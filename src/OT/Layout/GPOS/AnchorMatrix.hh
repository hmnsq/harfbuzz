#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

HB_INTERNAL અંતઃરેખ
શૂન્ય collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_ગણતરી,
                                     અચલ hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  માટે (unsigned row = 0; row < rows; row++)
    માટે (unsigned col = 0; col < class_ગણતરી; col++)
      જો (klass_mapping.has (col))
        indexes.push (row * class_ગણતરી + col);
}

HB_INTERNAL અંતઃરેખ
શૂન્ય collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      અચલ શૂન્ય *base,
                                      glyph_data_t offsets,
                                      hb_array_t<અચલ unsigned> indexes)
{
  માટે (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    જો (offset)
      StructAtOffset<અચલ Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL અંતઃરેખ
શૂન્ય collect_anchor_indexes (અચલ Coverage &coverage,
                             unsigned rows,
                             unsigned class_ગણતરી,
                             અચલ hb_set_t &glyphset,
                             અચલ hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  માટે (અચલ auto _ : hb_zip (coverage, hb_range (rows)))
    જો (glyphset.has (_.first))
      માટે (unsigned col = 0; col < class_ગણતરી; col++)
        જો (klass_mapping.has (col))
          indexes.push (_.second * class_ગણતરી + col);
}

HB_INTERNAL અંતઃરેખ
શૂન્ય collect_subset_anchor_indexes (અચલ Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_ગણતરી,
                                    glyph_data_t offsets,
                                    અચલ hb_set_t &glyphset,
                                    અચલ hb_map_t &glyph_map,
                                    અચલ hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  માટે (અચલ auto _ : hb_zip (coverage, hb_range (rows)))
  {
    જો (!glyphset.has (_.first)) ચાલુ;
    unsigned row = _.second;
    તાર્કિક non_empty = ખોટું;
    માટે (unsigned col = 0; col < class_ગણતરી; col++)
      જો (klass_mapping.has (col) && offsets[row * class_ગણતરી + col])
      {
        non_empty = સાચું;
        તોડો;
      }
    જો (!non_empty) ચાલુ;

    new_coverage.push (glyph_map.get (_.first));
    માટે (unsigned col = 0; col < class_ગણતરી; col++)
      જો (klass_mapping.has (col))
        indexes.push (row * class_ગણતરી + col);
  }
}

template <typename Types, typename Row_ગણતરી=typename Types::HBUINT>
સંરચના AnchorMatrix
{
  Row_ગણતરી      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  જાહેર:
  DEFINE_SIZE_ARRAY (કદ (Row_ગણતરી), matrixZ);

  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક cols) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!c->check_struct (આ)) return_trace (ખોટું);
    hb_barrier ();
    જો (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (ખોટું);
    unsigned પૂર્ણાંક ગણતરી = rows * cols;
    જો (!c->check_array (matrixZ.arrayZ, ગણતરી)) return_trace (ખોટું);

    જો (c->lazy_some_gpos)
      return_trace (સાચું);

    hb_barrier ();
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (!matrixZ[i].sanitize (c, આ)) return_trace (ખોટું);
    return_trace (સાચું);
  }

  અચલ Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned પૂર્ણાંક row, unsigned પૂર્ણાંક col,
			    unsigned પૂર્ણાંક cols, તાર્કિક *found) અચલ
  {
    *found = ખોટું;
    જો (unlikely (row >= rows || col >= cols)) પાછા Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    જો (unlikely (!offset.sanitize (&c->sanitizer, આ))) પાછા Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    પાછા આ+offset;
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<અચલ unsigned> indexes) અચલ
  {
    collect_anchor_variation_indices (c, આ, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક subset (hb_subset_context_t *c,
               unsigned             અંક_rows,
               Iterator             index_iter) અચલ
  {
    TRACE_SUBSET (આ);

    auto *out = c->serializer->start_embed (આ);

    જો (!index_iter) return_trace (ખોટું);
    જો (unlikely (!c->serializer->extend_min (out)))  return_trace (ખોટું);

    out->rows = અંક_rows;
    માટે (અચલ unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      જો (!offset) return_trace (ખોટું);
      offset->serialize_subset (c, matrixZ[i], આ);
    }

    return_trace (સાચું);
  }

  તાર્કિક offset_is_null (unsigned row, unsigned col, unsigned અંક_cols) અચલ
  {
    જો (unlikely (row >= rows || col >= અંક_cols)) પાછા સાચું;
    auto &offset = matrixZ[row * અંક_cols + col];
    પાછા offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
