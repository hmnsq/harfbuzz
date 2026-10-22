#अगर_अघोषित OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#घोषणा OT_LAYOUT_GPOS_LIGATUREARRAY_HH

namespace OT {
namespace Layout {
namespace GPOS_impl {

HB_INTERNAL अंतरभूत
व्योम collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        स्थिर Coverage &coverage,
                                        स्थिर व्योम *base,
                                        glyph_तथ्य_t offsets,
                                        अचिन्हित गिनती,
                                        अचिन्हित class_गिनती,
                                        स्थिर hb_map_t &klass_mapping)
{
  क्रम (स्थिर स्वतः _ : hb_zip (coverage, hb_range (गिनती)))
  {
    अगर (!c->glyph_set->has (_.first)) जारी;
    hb_barrier ();
    अचिन्हित offset = offsets[_.second];
    अगर (!offset) जारी;

    स्थिर व्योम *matrix = &StructAtOffset<स्थिर HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    अचिन्हित rows = *static_cast<स्थिर HBUINT16 *> (matrix);
    glyph_तथ्य_t anchors = offsets;
    anchors.तथ्य = (स्थिर अक्षर *) matrix + HBUINT16::static_आकार;

    hb_sorted_vector_t<अचिन्हित> indexes;
    collect_ligature_anchor_indexes (rows, class_गिनती, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
ढाँचा <typename Types>
काष्ठा LigatureArray :
  Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  प्रकार AnchorMatrix<Types, HBUINT16> LigatureAttach;
  प्रकार typename Types::ढाँचा OffsetTo<LigatureAttach> LigatureAttachOffset;
  प्रकार typename Types::ढाँचा ArrayOf<LigatureAttachOffset> Super;

  स्थिर LigatureAttach& चालक [] (पूर्णांक i_) स्थिर
  {
    अचिन्हित पूर्णांक i = (अचिन्हित पूर्णांक) i_;
    अगर (unlikely (i >= यह->len)) वापस Null (LigatureAttach);
    hb_barrier ();
    वापस यह+यह->arrayZ[i];
  }

  ढाँचा <typename ...Ts>
  bool sanitize (hb_sanitize_context_t *c, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (Super::sanitize (c, यह, std::forward<Ts> (ds)...));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिर Coverage &coverage,
                                  अचिन्हित class_गिनती,
                                  स्थिर hb_map_t &klass_mapping) स्थिर
  {
    collect_ligature_variation_indices (c, coverage, यह, यह->arrayZ,
                                        यह->len, class_गिनती, klass_mapping);
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  bool subset (hb_subset_context_t *c,
               Iterator             coverage,
               अचिन्हित             class_गिनती,
               स्थिर hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!c->serializer->extend_min (out)))  return_trace (false);

    bool ret = false;
    क्रम (स्थिर स्वतः _ : + hb_zip (coverage, *यह)
                        | hb_filter (glyph_map, hb_first))
    {
      स्थिर LigatureAttach& src = (यह + _.second);
      bool non_empty = + hb_range (src.rows * class_गिनती)
                       | hb_filter ([=] (अचिन्हित सूचकांक_2) { वापस klass_mapping->has (सूचकांक_2 % class_गिनती); })
                       | hb_map ([&] (स्थिर अचिन्हित सूचकांक_2) { वापस !src.offset_is_null (सूचकांक_2 / class_गिनती, सूचकांक_2 % class_गिनती, class_गिनती); })
                       | hb_any;

      अगर (!non_empty) जारी;

      स्वतः *matrix = out->serialize_append (c->serializer);
      अगर (unlikely (!matrix)) return_trace (false);

      स्वतः indexes =
          + hb_range (src.rows * class_गिनती)
          | hb_filter ([=] (अचिन्हित सूचकांक_2) { वापस klass_mapping->has (सूचकांक_2 % class_गिनती); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       यह,
				       src.rows,
				       indexes);

      hb_codepoint_t new_gid = glyph_map.लो (_.first);
      new_coverage.push (new_gid);
    }
    return_trace (ret);
  }
};


}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_LIGATUREARRAY_HH */
