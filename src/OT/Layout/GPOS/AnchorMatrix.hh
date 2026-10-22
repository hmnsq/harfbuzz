#अगर_अघोषित OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#घोषणा OT_LAYOUT_GPOS_ANCHORMATRIX_HH

namespace OT {
namespace Layout {
namespace GPOS_impl {

HB_INTERNAL अंतरभूत
व्योम collect_ligature_anchor_indexes (अचिन्हित rows,
                                     अचिन्हित class_गिनती,
                                     स्थिर hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<अचिन्हित> &indexes)
{
  क्रम (अचिन्हित row = 0; row < rows; row++)
    क्रम (अचिन्हित col = 0; col < class_गिनती; col++)
      अगर (klass_mapping.has (col))
        indexes.push (row * class_गिनती + col);
}

HB_INTERNAL अंतरभूत
व्योम collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      स्थिर व्योम *base,
                                      glyph_तथ्य_t offsets,
                                      hb_array_t<स्थिर अचिन्हित> indexes)
{
  क्रम (अचिन्हित i : indexes)
  {
    अचिन्हित offset = offsets[i];
    अगर (offset)
      StructAtOffset<स्थिर Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL अंतरभूत
व्योम collect_anchor_indexes (स्थिर Coverage &coverage,
                             अचिन्हित rows,
                             अचिन्हित class_गिनती,
                             स्थिर hb_set_t &glyphset,
                             स्थिर hb_map_t &klass_mapping,
                             hb_sorted_vector_t<अचिन्हित> &indexes)
{
  क्रम (स्थिर स्वतः _ : hb_zip (coverage, hb_range (rows)))
    अगर (glyphset.has (_.first))
      क्रम (अचिन्हित col = 0; col < class_गिनती; col++)
        अगर (klass_mapping.has (col))
          indexes.push (_.second * class_गिनती + col);
}

HB_INTERNAL अंतरभूत
व्योम collect_subset_anchor_indexes (स्थिर Coverage &coverage,
                                    अचिन्हित rows,
                                    अचिन्हित class_गिनती,
                                    glyph_तथ्य_t offsets,
                                    स्थिर hb_set_t &glyphset,
                                    स्थिर hb_map_t &glyph_map,
                                    स्थिर hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<अचिन्हित> &indexes)
{
  क्रम (स्थिर स्वतः _ : hb_zip (coverage, hb_range (rows)))
  {
    अगर (!glyphset.has (_.first)) जारी;
    अचिन्हित row = _.second;
    bool non_empty = false;
    क्रम (अचिन्हित col = 0; col < class_गिनती; col++)
      अगर (klass_mapping.has (col) && offsets[row * class_गिनती + col])
      {
        non_empty = true;
        अवरोध;
      }
    अगर (!non_empty) जारी;

    new_coverage.push (glyph_map.लो (_.first));
    क्रम (अचिन्हित col = 0; col < class_गिनती; col++)
      अगर (klass_mapping.has (col))
        indexes.push (row * class_गिनती + col);
  }
}

ढाँचा <typename Types, typename Row_गिनती=typename Types::HBUINT>
काष्ठा AnchorMatrix
{
  Row_गिनती      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::ढाँचा OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  खुला:
  DEFINE_आकार_ARRAY (माप (Row_गिनती), matrixZ);

  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक cols) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!c->check_struct (यह)) return_trace (false);
    hb_barrier ();
    अगर (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (false);
    अचिन्हित पूर्णांक गिनती = rows * cols;
    अगर (!c->check_array (matrixZ.arrayZ, गिनती)) return_trace (false);

    अगर (c->lazy_some_gpos)
      return_trace (true);

    hb_barrier ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (!matrixZ[i].sanitize (c, यह)) return_trace (false);
    return_trace (true);
  }

  स्थिर Anchor& get_anchor (hb_ot_apply_context_t *c,
			    अचिन्हित पूर्णांक row, अचिन्हित पूर्णांक col,
			    अचिन्हित पूर्णांक cols, bool *found) स्थिर
  {
    *found = false;
    अगर (unlikely (row >= rows || col >= cols)) वापस Null (Anchor);
    स्वतः &offset = matrixZ[row * cols + col];
    अगर (unlikely (!offset.sanitize (&c->sanitizer, यह))) वापस Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    वापस यह+offset;
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<स्थिर अचिन्हित> indexes) स्थिर
  {
    collect_anchor_variation_indices (c, यह, matrixZ.arrayZ, indexes);
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  bool subset (hb_subset_context_t *c,
               अचिन्हित             अंक_rows,
               Iterator             सूचकांक_iter) स्थिर
  {
    TRACE_SUBSET (यह);

    स्वतः *out = c->serializer->start_embed (यह);

    अगर (!सूचकांक_iter) return_trace (false);
    अगर (unlikely (!c->serializer->extend_min (out)))  return_trace (false);

    out->rows = अंक_rows;
    क्रम (स्थिर अचिन्हित i : सूचकांक_iter)
    {
      स्वतः *offset = c->serializer->embed (matrixZ[i]);
      अगर (!offset) return_trace (false);
      offset->serialize_subset (c, matrixZ[i], यह);
    }

    return_trace (true);
  }

  bool offset_is_null (अचिन्हित row, अचिन्हित col, अचिन्हित अंक_cols) स्थिर
  {
    अगर (unlikely (row >= rows || col >= अंक_cols)) वापस true;
    स्वतः &offset = matrixZ[row * अंक_cols + col];
    वापस offset.is_null ();
  }
};


}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
