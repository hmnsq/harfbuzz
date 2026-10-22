#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

HB_INTERNAL अन्तःस्थ
शून्य collect_ligature_anchor_indexes (अचिन्हित rows,
                                     अचिन्हित class_गिनती,
                                     ध्रुव hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<अचिन्हित> &indexes)
{
  प्रति (अचिन्हित row = 0; row < rows; row++)
    प्रति (अचिन्हित col = 0; col < class_गिनती; col++)
      यदि (klass_mapping.has (col))
        indexes.push (row * class_गिनती + col);
}

HB_INTERNAL अन्तःस्थ
शून्य collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ध्रुव शून्य *base,
                                      glyph_data_t offsets,
                                      hb_array_t<ध्रुव अचिन्हित> indexes)
{
  प्रति (अचिन्हित i : indexes)
  {
    अचिन्हित offset = offsets[i];
    यदि (offset)
      StructAtOffset<ध्रुव Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL अन्तःस्थ
शून्य collect_anchor_indexes (ध्रुव Coverage &coverage,
                             अचिन्हित rows,
                             अचिन्हित class_गिनती,
                             ध्रुव hb_set_t &glyphset,
                             ध्रुव hb_map_t &klass_mapping,
                             hb_sorted_vector_t<अचिन्हित> &indexes)
{
  प्रति (ध्रुव स्वतः _ : hb_zip (coverage, hb_range (rows)))
    यदि (glyphset.has (_.first))
      प्रति (अचिन्हित col = 0; col < class_गिनती; col++)
        यदि (klass_mapping.has (col))
          indexes.push (_.second * class_गिनती + col);
}

HB_INTERNAL अन्तःस्थ
शून्य collect_subset_anchor_indexes (ध्रुव Coverage &coverage,
                                    अचिन्हित rows,
                                    अचिन्हित class_गिनती,
                                    glyph_data_t offsets,
                                    ध्रुव hb_set_t &glyphset,
                                    ध्रुव hb_map_t &glyph_map,
                                    ध्रुव hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<अचिन्हित> &indexes)
{
  प्रति (ध्रुव स्वतः _ : hb_zip (coverage, hb_range (rows)))
  {
    यदि (!glyphset.has (_.first)) अनुवर्त;
    अचिन्हित row = _.second;
    तार्किक non_empty = असत्य;
    प्रति (अचिन्हित col = 0; col < class_गिनती; col++)
      यदि (klass_mapping.has (col) && offsets[row * class_गिनती + col])
      {
        non_empty = सत्य;
        भङ्ग;
      }
    यदि (!non_empty) अनुवर्त;

    new_coverage.push (glyph_map.get (_.first));
    प्रति (अचिन्हित col = 0; col < class_गिनती; col++)
      यदि (klass_mapping.has (col))
        indexes.push (row * class_गिनती + col);
  }
}

ढाँचा <typename Types, typename Row_गिनती=typename Types::HBUINT>
संरचना AnchorMatrix
{
  Row_गिनती      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::ढाँचा OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (परिमाण (Row_गिनती), matrixZ);

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णाङ्क cols) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (!c->check_struct (एतत्)) return_trace (असत्य);
    hb_barrier ();
    यदि (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (असत्य);
    अचिन्हित पूर्णाङ्क गिनती = rows * cols;
    यदि (!c->check_array (matrixZ.arrayZ, गिनती)) return_trace (असत्य);

    यदि (c->lazy_some_gpos)
      return_trace (सत्य);

    hb_barrier ();
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < गिनती; i++)
      यदि (!matrixZ[i].sanitize (c, एतत्)) return_trace (असत्य);
    return_trace (सत्य);
  }

  ध्रुव Anchor& get_anchor (hb_ot_apply_context_t *c,
			    अचिन्हित पूर्णाङ्क row, अचिन्हित पूर्णाङ्क col,
			    अचिन्हित पूर्णाङ्क cols, तार्किक *found) ध्रुव
  {
    *found = असत्य;
    यदि (unlikely (row >= rows || col >= cols)) प्रत्यागम Null (Anchor);
    स्वतः &offset = matrixZ[row * cols + col];
    यदि (unlikely (!offset.sanitize (&c->sanitizer, एतत्))) प्रत्यागम Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    प्रत्यागम एतत्+offset;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ध्रुव अचिन्हित> indexes) ध्रुव
  {
    collect_anchor_variation_indices (c, एतत्, matrixZ.arrayZ, indexes);
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  तार्किक subset (hb_subset_context_t *c,
               अचिन्हित             अंक_rows,
               Iterator             index_iter) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    स्वतः *out = c->serializer->start_embed (एतत्);

    यदि (!index_iter) return_trace (असत्य);
    यदि (unlikely (!c->serializer->extend_min (out)))  return_trace (असत्य);

    out->rows = अंक_rows;
    प्रति (ध्रुव अचिन्हित i : index_iter)
    {
      स्वतः *offset = c->serializer->embed (matrixZ[i]);
      यदि (!offset) return_trace (असत्य);
      offset->serialize_subset (c, matrixZ[i], एतत्);
    }

    return_trace (सत्य);
  }

  तार्किक offset_is_null (अचिन्हित row, अचिन्हित col, अचिन्हित अंक_cols) ध्रुव
  {
    यदि (unlikely (row >= rows || col >= अंक_cols)) प्रत्यागम सत्य;
    स्वतः &offset = matrixZ[row * अंक_cols + col];
    प्रत्यागम offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
