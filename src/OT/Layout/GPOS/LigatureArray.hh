#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

HB_INTERNAL ओळीत
शून्य collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        स्थिरांक Coverage &coverage,
                                        स्थिरांक शून्य *base,
                                        glyph_data_t offsets,
                                        अचिन्हित मोजणी,
                                        अचिन्हित class_मोजणी,
                                        स्थिरांक hb_map_t &klass_mapping)
{
  साठी (स्थिरांक स्वतः _ : hb_zip (coverage, hb_range (मोजणी)))
  {
    जर (!c->glyph_set->has (_.first)) सुरूठेवा;
    hb_barrier ();
    अचिन्हित offset = offsets[_.second];
    जर (!offset) सुरूठेवा;

    स्थिरांक शून्य *matrix = &StructAtOffset<स्थिरांक HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    अचिन्हित rows = *static_cast<स्थिरांक HBUINT16 *> (matrix);
    glyph_data_t anchors = offsets;
    anchors.data = (स्थिरांक अक्षर *) matrix + HBUINT16::static_size;

    hb_sorted_vector_t<अचिन्हित> indexes;
    collect_ligature_anchor_indexes (rows, class_मोजणी, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
ढाँचा <typename Types>
संरचना LigatureArray :
  Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  प्रकारव्याख्या AnchorMatrix<Types, HBUINT16> LigatureAttach;
  प्रकारव्याख्या typename Types::ढाँचा OffsetTo<LigatureAttach> LigatureAttachOffset;
  प्रकारव्याख्या typename Types::ढाँचा ArrayOf<LigatureAttachOffset> Super;

  स्थिरांक LigatureAttach& चालक [] (पूर्णांक i_) स्थिरांक
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    जर (unlikely (i >= हे->len)) परत Null (LigatureAttach);
    hb_barrier ();
    परत हे+हे->arrayZ[i];
  }

  ढाँचा <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (Super::sanitize (c, हे, std::forward<Ts> (ds)...));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिरांक Coverage &coverage,
                                  अचिन्हित class_मोजणी,
                                  स्थिरांक hb_map_t &klass_mapping) स्थिरांक
  {
    collect_ligature_variation_indices (c, coverage, हे, हे->arrayZ,
                                        हे->len, class_मोजणी, klass_mapping);
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  तार्किक subset (hb_subset_context_t *c,
               Iterator             coverage,
               अचिन्हित             class_मोजणी,
               स्थिरांक hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!c->serializer->extend_min (out)))  return_trace (असत्य);

    तार्किक ret = असत्य;
    साठी (स्थिरांक स्वतः _ : + hb_zip (coverage, *हे)
                        | hb_filter (glyph_map, hb_first))
    {
      स्थिरांक LigatureAttach& src = (हे + _.second);
      तार्किक non_empty = + hb_range (src.rows * class_मोजणी)
                       | hb_filter ([=] (अचिन्हित index) { परत klass_mapping->has (index % class_मोजणी); })
                       | hb_map ([&] (स्थिरांक अचिन्हित index) { परत !src.offset_is_null (index / class_मोजणी, index % class_मोजणी, class_मोजणी); })
                       | hb_any;

      जर (!non_empty) सुरूठेवा;

      स्वतः *matrix = out->serialize_append (c->serializer);
      जर (unlikely (!matrix)) return_trace (असत्य);

      स्वतः indexes =
          + hb_range (src.rows * class_मोजणी)
          | hb_filter ([=] (अचिन्हित index) { परत klass_mapping->has (index % class_मोजणी); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       हे,
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
