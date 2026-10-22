#ifndef OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#define OT_LAYOUT_GPOS_ANCHORMATRIX_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

HB_INTERNAL ओळीत
शून्य collect_ligature_anchor_indexes (अचिन्हित rows,
                                     अचिन्हित class_मोजणी,
                                     स्थिरांक hb_map_t &klass_mapping,
                                     hb_sorted_vector_t<अचिन्हित> &indexes)
{
  साठी (अचिन्हित row = 0; row < rows; row++)
    साठी (अचिन्हित col = 0; col < class_मोजणी; col++)
      जर (klass_mapping.has (col))
        indexes.push (row * class_मोजणी + col);
}

HB_INTERNAL ओळीत
शून्य collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      स्थिरांक शून्य *base,
                                      glyph_data_t offsets,
                                      hb_array_t<स्थिरांक अचिन्हित> indexes)
{
  साठी (अचिन्हित i : indexes)
  {
    अचिन्हित offset = offsets[i];
    जर (offset)
      StructAtOffset<स्थिरांक Anchor> (base, offset).collect_variation_indices (c);
  }
}

HB_INTERNAL ओळीत
शून्य collect_anchor_indexes (स्थिरांक Coverage &coverage,
                             अचिन्हित rows,
                             अचिन्हित class_मोजणी,
                             स्थिरांक hb_set_t &glyphset,
                             स्थिरांक hb_map_t &klass_mapping,
                             hb_sorted_vector_t<अचिन्हित> &indexes)
{
  साठी (स्थिरांक स्वतः _ : hb_zip (coverage, hb_range (rows)))
    जर (glyphset.has (_.first))
      साठी (अचिन्हित col = 0; col < class_मोजणी; col++)
        जर (klass_mapping.has (col))
          indexes.push (_.second * class_मोजणी + col);
}

HB_INTERNAL ओळीत
शून्य collect_subset_anchor_indexes (स्थिरांक Coverage &coverage,
                                    अचिन्हित rows,
                                    अचिन्हित class_मोजणी,
                                    glyph_data_t offsets,
                                    स्थिरांक hb_set_t &glyphset,
                                    स्थिरांक hb_map_t &glyph_map,
                                    स्थिरांक hb_map_t &klass_mapping,
                                    hb_sorted_vector_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_vector_t<अचिन्हित> &indexes)
{
  साठी (स्थिरांक स्वतः _ : hb_zip (coverage, hb_range (rows)))
  {
    जर (!glyphset.has (_.first)) सुरूठेवा;
    अचिन्हित row = _.second;
    तार्किक non_empty = असत्य;
    साठी (अचिन्हित col = 0; col < class_मोजणी; col++)
      जर (klass_mapping.has (col) && offsets[row * class_मोजणी + col])
      {
        non_empty = सत्य;
        थांबा;
      }
    जर (!non_empty) सुरूठेवा;

    new_coverage.push (glyph_map.get (_.first));
    साठी (अचिन्हित col = 0; col < class_मोजणी; col++)
      जर (klass_mapping.has (col))
        indexes.push (row * class_मोजणी + col);
  }
}

ढाँचा <typename Types, typename Row_मोजणी=typename Types::HBUINT>
संरचना AnchorMatrix
{
  Row_मोजणी      rows;                   /* Number of rows */
  UnsizedArrayOf<typename Types::ढाँचा OffsetTo<Anchor, AnchorMatrix>>
                matrixZ;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (आकार (Row_मोजणी), matrixZ);

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक cols) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!c->check_struct (हे)) return_trace (असत्य);
    hb_barrier ();
    जर (unlikely (hb_unsigned_mul_overflows (rows, cols))) return_trace (असत्य);
    अचिन्हित पूर्णांक मोजणी = rows * cols;
    जर (!c->check_array (matrixZ.arrayZ, मोजणी)) return_trace (असत्य);

    जर (c->lazy_some_gpos)
      return_trace (सत्य);

    hb_barrier ();
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (!matrixZ[i].sanitize (c, हे)) return_trace (असत्य);
    return_trace (सत्य);
  }

  स्थिरांक Anchor& get_anchor (hb_ot_apply_context_t *c,
			    अचिन्हित पूर्णांक row, अचिन्हित पूर्णांक col,
			    अचिन्हित पूर्णांक cols, तार्किक *found) स्थिरांक
  {
    *found = असत्य;
    जर (unlikely (row >= rows || col >= cols)) परत Null (Anchor);
    स्वतः &offset = matrixZ[row * cols + col];
    जर (unlikely (!offset.sanitize (&c->sanitizer, हे))) परत Null (Anchor);
    hb_barrier ();
    *found = !offset.is_null ();
    परत हे+offset;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<स्थिरांक अचिन्हित> indexes) स्थिरांक
  {
    collect_anchor_variation_indices (c, हे, matrixZ.arrayZ, indexes);
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  तार्किक subset (hb_subset_context_t *c,
               अचिन्हित             अंक_rows,
               Iterator             index_iter) स्थिरांक
  {
    TRACE_SUBSET (हे);

    स्वतः *out = c->serializer->start_embed (हे);

    जर (!index_iter) return_trace (असत्य);
    जर (unlikely (!c->serializer->extend_min (out)))  return_trace (असत्य);

    out->rows = अंक_rows;
    साठी (स्थिरांक अचिन्हित i : index_iter)
    {
      स्वतः *offset = c->serializer->embed (matrixZ[i]);
      जर (!offset) return_trace (असत्य);
      offset->serialize_subset (c, matrixZ[i], हे);
    }

    return_trace (सत्य);
  }

  तार्किक offset_is_null (अचिन्हित row, अचिन्हित col, अचिन्हित अंक_cols) स्थिरांक
  {
    जर (unlikely (row >= rows || col >= अंक_cols)) परत सत्य;
    स्वतः &offset = matrixZ[row * अंक_cols + col];
    परत offset.is_null ();
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
