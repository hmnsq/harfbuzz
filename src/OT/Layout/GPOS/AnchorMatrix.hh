#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

HB_INTERNAL अन्तःस्थ
शून्य collect_ligature_anchor_indexes (अचिन्हित rows,
                                     अचिन्हित class_गन्ती,
                                     अचर hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<अचिन्हित> &indexes)
{
  लागि (अचिन्हित row = 0; row < rows; row++)
    लागि (अचिन्हित col = 0; col < class_गन्ती; col++)
      यदि (klass_mapping.has (col))
        indexes.push (row * class_गन्ती + col);
}

HB_INTERNAL अन्तःस्थ
शून्य collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      अचर शून्य *base,
                                      glyph_data_t offsets,
                                      hb_array_t<अचर अचिन्हित> indexes)
{
  लागि (अचिन्हित i : indexes)
  {
    अचिन्हित offset = offsets[i];
    यदि (offset)
      StructAtOffset<अचर Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL अन्तःस्थ
शून्य collect_anchor_indexes (अचर Coverage &coverage,
                             अचिन्हित rows,
                             अचिन्हित class_गन्ती,
                             अचर hb_set_t &glyphset,
                             अचर hb_map_t &klass_mapping,
                             hb_sorted_vector_t<अचिन्हित> &indexes)
{
  लागि (अचर स्वतः _ : hb_zip (coverage, hb_range (rows)))
    यदि (glyphset.has (_.first))
      लागि (अचिन्हित col = 0; col < class_गन्ती; col++)
        यदि (klass_mapping.has (col))
          indexes.push (_.second * class_गन्ती + col);
}

HB_INTERNAL अन्तःस्थ
शून्य collect_subset_anchor_indexes (अचर Coverage &coverage,
                                    अचिन्हित rows,
                                    अचिन्हित class_गन्ती,
                                    glyph_data_t offsets,
                                    अचर hb_set_t &glyphset,
                                    अचर hb_map_t &glyph_map,
                                    अचर hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<अचिन्हित> &indexes)
{
  लागि (अचर स्वतः _ : hb_zip (coverage, hb_range (rows)))
  {
    यदि (!glyphset.has (_.first)) जारी;
    अचिन्हित row = _.second;
    तार्किक non_empty = असत्य;
    लागि (अचिन्हित col = 0; col < class_गन्ती; col++)
      यदि (klass_mapping.has (col) && offsets[row * class_गन्ती + col])
      {
        non_empty = सत्य;
        रोक;
      }
    यदि (!non_empty) जारी;

    new_coverage.push (glyph_map.get (_.first));
    लागि (अचिन्हित col = 0; col < class_गन्ती; col++)
      यदि (klass_mapping.has (col))
        indexes.push (row * class_गन्ती + col);
  }
}

ढाँचा <typename Types, typename Row_गन्ती=typename Types::HBUINT>
संरचना AnchorMatrix
{
  Row_गन्ती      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::ढाँचा OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (परिमाण (Row_गन्ती), matrixZ);

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णाङ्क cols) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!c->check_struct (एतत्)) return_trace (असत्य);
    hb_barrier ();
    यदि (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (असत्य);
    अचिन्हित पूर्णाङ्क गन्ती = rows * cols;
    यदि (!c->check_array (matrixZ.arrayZ, गन्ती)) return_trace (असत्य);

    यदि (c->lazy_some_gpos)
      return_trace (सत्य);

    hb_barrier ();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (!matrixZ[i].sanitize (c, एतत्)) return_trace (असत्य);
    return_trace (सत्य);
  }

  अचर Anchor& get_anchor (hb_ot_apply_context_t *c,
			    अचिन्हित पूर्णाङ्क row, अचिन्हित पूर्णाङ्क col,
			    अचिन्हित पूर्णाङ्क cols, तार्किक *found) अचर
  {
    *found = असत्य;
    यदि (unlikely (row >= rows || col >= cols)) फर्काऊ Null (Anchor);
    स्वतः &offset = matrixZ[row * cols + col];
    यदि (unlikely (!offset.sanitize (&c->sanitizer, एतत्))) फर्काऊ Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    फर्काऊ एतत्+offset;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<अचर अचिन्हित> indexes) अचर
  {
    collect_anchor_variation_indices (c, एतत्, matrixZ.arrayZ, indexes);
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  तार्किक subset (hb_subset_context_t *c,
               अचिन्हित             अङ्क_rows,
               Iterator             index_iter) अचर
  {
    TRACE_SUBSET (एतत्);

    स्वतः *out = c->serializer->start_embed (एतत्);

    यदि (!index_iter) return_trace (असत्य);
    यदि (unlikely (!c->serializer->extend_min (out)))  return_trace (असत्य);

    out->rows = अङ्क_rows;
    लागि (अचर अचिन्हित i : index_iter)
    {
      स्वतः *offset = c->serializer->embed (matrixZ[i]);
      यदि (!offset) return_trace (असत्य);
      offset->serialize_subset (c, matrixZ[i], एतत्);
    }

    return_trace (सत्य);
  }

  तार्किक offset_is_null (अचिन्हित row, अचिन्हित col, अचिन्हित अङ्क_cols) अचर
  {
    यदि (unlikely (row >= rows || col >= अङ्क_cols)) फर्काऊ सत्य;
    स्वतः &offset = matrixZ[row * अङ्क_cols + col];
    फर्काऊ offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
