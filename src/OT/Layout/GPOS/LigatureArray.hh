#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

HB_INTERNAL अन्तःस्थ
शून्य collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        अचर Coverage &coverage,
                                        अचर शून्य *base,
                                        glyph_data_t offsets,
                                        अचिन्हित गन्ती,
                                        अचिन्हित class_गन्ती,
                                        अचर hb_map_t &klass_mapping)
{
  लागि (अचर स्वतः _ : hb_zip (coverage, hb_range (गन्ती)))
  {
    यदि (!c->glyph_set->has (_.first)) जारी;
    hb_barrier ();
    अचिन्हित offset = offsets[_.second];
    यदि (!offset) जारी;

    अचर शून्य *matrix = &StructAtOffset<अचर HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    अचिन्हित rows = *static_cast<अचर HBUINT16 *> (matrix);
    glyph_data_t anchors = offsets;
    anchors.data = (अचर वर्ण *) matrix + HBUINT16::static_size;

    hb_sorted_vector_t<अचिन्हित> indexes;
    collect_ligature_anchor_indexes (rows, class_गन्ती, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
ढाँचा <typename Types>
संरचना LigatureArray :
  Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  प्रकारनिर्देश AnchorMatrix<Types, HBUINT16> LigatureAttach;
  प्रकारनिर्देश typename Types::ढाँचा OffsetTo<LigatureAttach> LigatureAttachOffset;
  प्रकारनिर्देश typename Types::ढाँचा ArrayOf<LigatureAttachOffset> Super;

  अचर LigatureAttach& चालक [] (पूर्णाङ्क i_) अचर
  {
    अचिन्हित पूर्णाङ्क i = (अचिन्हित पूर्णाङ्क) i_;
    यदि (unlikely (i >= एतत्->len)) फर्काऊ Null (LigatureAttach);
    hb_barrier ();
    फर्काऊ एतत्+एतत्->arrayZ[i];
  }

  ढाँचा <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (Super::sanitize (c, एतत्, std::forward<Ts> (ds)...));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  अचर Coverage &coverage,
                                  अचिन्हित class_गन्ती,
                                  अचर hb_map_t &klass_mapping) अचर
  {
    collect_ligature_variation_indices (c, coverage, एतत्, एतत्->arrayZ,
                                        एतत्->len, class_गन्ती, klass_mapping);
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  तार्किक subset (hb_subset_context_t *c,
               Iterator             coverage,
               अचिन्हित             class_गन्ती,
               अचर hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    स्वतः *out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!c->serializer->extend_min (out)))  return_trace (असत्य);

    तार्किक ret = असत्य;
    लागि (अचर स्वतः _ : + hb_zip (coverage, *एतत्)
                        | hb_filter (glyph_map, hb_first))
    {
      अचर LigatureAttach& src = (एतत् + _.second);
      तार्किक non_empty = + hb_range (src.rows * class_गन्ती)
                       | hb_filter ([=] (अचिन्हित index) { फर्काऊ klass_mapping->has (index % class_गन्ती); })
                       | hb_map ([&] (अचर अचिन्हित index) { फर्काऊ !src.offset_is_null (index / class_गन्ती, index % class_गन्ती, class_गन्ती); })
                       | hb_any;

      यदि (!non_empty) जारी;

      स्वतः *matrix = out->serialize_append (c->serializer);
      यदि (unlikely (!matrix)) return_trace (असत्य);

      स्वतः indexes =
          + hb_range (src.rows * class_गन्ती)
          | hb_filter ([=] (अचिन्हित index) { फर्काऊ klass_mapping->has (index % class_गन्ती); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       एतत्,
				       src.rows,
				       indexes);

      hb_codepoint_t new_gid = glyph_map.get (_.first);
      new_coverage.push (new_gid);
    }
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_LIGATUREARRAY_HH */
