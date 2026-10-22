#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

HB_INTERNAL വരിയിൽ
ശൂന്യം collect_ligature_anchor_indexes (unsigned rows,
                                     unsigned class_എണ്ണം,
                                     സ്ഥിരം hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<unsigned> &indexes)
{
  വേണ്ടി (unsigned row = 0; row < rows; row++)
    വേണ്ടി (unsigned col = 0; col < class_എണ്ണം; col++)
      എങ്കിൽ (klass_mapping.has (col))
        indexes.push (row * class_എണ്ണം + col);
}

HB_INTERNAL വരിയിൽ
ശൂന്യം collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      സ്ഥിരം ശൂന്യം *base,
                                      glyph_data_t offsets,
                                      hb_array_t<സ്ഥിരം unsigned> indexes)
{
  വേണ്ടി (unsigned i : indexes)
  {
    unsigned offset = offsets[i];
    എങ്കിൽ (offset)
      StructAtOffset<സ്ഥിരം Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL വരിയിൽ
ശൂന്യം collect_anchor_indexes (സ്ഥിരം Coverage &coverage,
                             unsigned rows,
                             unsigned class_എണ്ണം,
                             സ്ഥിരം hb_set_t &glyphset,
                             സ്ഥിരം hb_map_t &klass_mapping,
                             hb_sorted_vector_t<unsigned> &indexes)
{
  വേണ്ടി (സ്ഥിരം auto _ : hb_zip (coverage, hb_range (rows)))
    എങ്കിൽ (glyphset.has (_.first))
      വേണ്ടി (unsigned col = 0; col < class_എണ്ണം; col++)
        എങ്കിൽ (klass_mapping.has (col))
          indexes.push (_.second * class_എണ്ണം + col);
}

HB_INTERNAL വരിയിൽ
ശൂന്യം collect_subset_anchor_indexes (സ്ഥിരം Coverage &coverage,
                                    unsigned rows,
                                    unsigned class_എണ്ണം,
                                    glyph_data_t offsets,
                                    സ്ഥിരം hb_set_t &glyphset,
                                    സ്ഥിരം hb_map_t &glyph_map,
                                    സ്ഥിരം hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<unsigned> &indexes)
{
  വേണ്ടി (സ്ഥിരം auto _ : hb_zip (coverage, hb_range (rows)))
  {
    എങ്കിൽ (!glyphset.has (_.first)) തുടരുക;
    unsigned row = _.second;
    തർക്കം non_empty = അസത്യം;
    വേണ്ടി (unsigned col = 0; col < class_എണ്ണം; col++)
      എങ്കിൽ (klass_mapping.has (col) && offsets[row * class_എണ്ണം + col])
      {
        non_empty = സത്യം;
        നിർത്തുക;
      }
    എങ്കിൽ (!non_empty) തുടരുക;

    new_coverage.push (glyph_map.get (_.first));
    വേണ്ടി (unsigned col = 0; col < class_എണ്ണം; col++)
      എങ്കിൽ (klass_mapping.has (col))
        indexes.push (row * class_എണ്ണം + col);
  }
}

template <typename Types, typename Row_എണ്ണം=typename Types::HBUINT>
ഘടന AnchorMatrix
{
  Row_എണ്ണം      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::template OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  പൊതു:
  DEFINE_SIZE_ARRAY (വലുപ്പം (Row_എണ്ണം), matrixZ);

  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ cols) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!c->check_struct (ഇത്)) return_trace (അസത്യം);
    hb_barrier ();
    എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (അസത്യം);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = rows * cols;
    എങ്കിൽ (!c->check_array (matrixZ.arrayZ, എണ്ണം)) return_trace (അസത്യം);

    എങ്കിൽ (c->lazy_some_gpos)
      return_trace (സത്യം);

    hb_barrier ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!matrixZ[i].sanitize (c, ഇത്)) return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  സ്ഥിരം Anchor& get_anchor (hb_ot_apply_context_t *c,
			    unsigned പൂർണ്ണസംഖ്യ row, unsigned പൂർണ്ണസംഖ്യ col,
			    unsigned പൂർണ്ണസംഖ്യ cols, തർക്കം *found) സ്ഥിരം
  {
    *found = അസത്യം;
    എങ്കിൽ (unlikely (row >= rows || col >= cols)) തിരികെ Null (Anchor);
    auto &offset = matrixZ[row * cols + col];
    എങ്കിൽ (unlikely (!offset.sanitize (&c->sanitizer, ഇത്))) തിരികെ Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    തിരികെ ഇത്+offset;
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<സ്ഥിരം unsigned> indexes) സ്ഥിരം
  {
    collect_anchor_variation_indices (c, ഇത്, matrixZ.arrayZ, indexes);
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  തർക്കം subset (hb_subset_context_t *c,
               unsigned             അക്കം_rows,
               Iterator             index_iter) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    auto *out = c->serializer->start_embed (ഇത്);

    എങ്കിൽ (!index_iter) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out)))  return_trace (അസത്യം);

    out->rows = അക്കം_rows;
    വേണ്ടി (സ്ഥിരം unsigned i : index_iter)
    {
      auto *offset = c->serializer->embed (matrixZ[i]);
      എങ്കിൽ (!offset) return_trace (അസത്യം);
      offset->serialize_subset (c, matrixZ[i], ഇത്);
    }

    return_trace (സത്യം);
  }

  തർക്കം offset_is_null (unsigned row, unsigned col, unsigned അക്കം_cols) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (row >= rows || col >= അക്കം_cols)) തിരികെ സത്യം;
    auto &offset = matrixZ[row * അക്കം_cols + col];
    തിരികെ offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
