#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

HB_INTERNAL உள்வரி
வெறுமை collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        மாறிலி Coverage &coverage,
                                        மாறிலி வெறுமை *base,
                                        glyph_data_t offsets,
                                        unsigned மொத்தம்,
                                        unsigned class_மொத்தம்,
                                        மாறிலி hb_map_t &klass_mapping)
{
  க்கு (மாறிலி auto _ : hb_zip (coverage, hb_range (மொத்தம்)))
  {
    எனில் (!c->glyph_set->has (_.first)) தொடர்;
    hb_barrier ();
    unsigned offset = offsets[_.second];
    எனில் (!offset) தொடர்;

    மாறிலி வெறுமை *matrix = &StructAtOffset<மாறிலி HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    unsigned rows = *static_cast<மாறிலி HBUINT16 *> (matrix);
    glyph_data_t anchors = offsets;
    anchors.data = (மாறிலி எழுத்து *) matrix + HBUINT16::static_size;

    hb_sorted_vector_t<unsigned> indexes;
    collect_ligature_anchor_indexes (rows, class_மொத்தம், klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
template <typename Types>
கட்டமைப்பு LigatureArray :
  Types::template ArrayOf<typename Types::template OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  வகைவரை AnchorMatrix<Types, HBUINT16> LigatureAttach;
  வகைவரை typename Types::template OffsetTo<LigatureAttach> LigatureAttachOffset;
  வகைவரை typename Types::template ArrayOf<LigatureAttachOffset> Super;

  மாறிலி LigatureAttach& operator [] (முழு i_) மாறிலி
  {
    unsigned முழு i = (unsigned முழு) i_;
    எனில் (unlikely (i >= இது->len)) திரும்பு Null (LigatureAttach);
    hb_barrier ();
    திரும்பு இது+இது->arrayZ[i];
  }

  template <typename ...Ts>
  தருக்க sanitize (hb_sanitize_context_t *c, Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (Super::sanitize (c, இது, std::forward<Ts> (ds)...));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  மாறிலி Coverage &coverage,
                                  unsigned class_மொத்தம்,
                                  மாறிலி hb_map_t &klass_mapping) மாறிலி
  {
    collect_ligature_variation_indices (c, coverage, இது, இது->arrayZ,
                                        இது->len, class_மொத்தம், klass_mapping);
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  தருக்க subset (hb_subset_context_t *c,
               Iterator             coverage,
               unsigned             class_மொத்தம்,
               மாறிலி hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!c->serializer->extend_min (out)))  return_trace (பொய்);

    தருக்க ret = பொய்;
    க்கு (மாறிலி auto _ : + hb_zip (coverage, *இது)
                        | hb_filter (glyph_map, hb_first))
    {
      மாறிலி LigatureAttach& src = (இது + _.second);
      தருக்க non_empty = + hb_range (src.rows * class_மொத்தம்)
                       | hb_filter ([=] (unsigned index) { திரும்பு klass_mapping->has (index % class_மொத்தம்); })
                       | hb_map ([&] (மாறிலி unsigned index) { திரும்பு !src.offset_is_null (index / class_மொத்தம், index % class_மொத்தம், class_மொத்தம்); })
                       | hb_any;

      எனில் (!non_empty) தொடர்;

      auto *matrix = out->serialize_append (c->serializer);
      எனில் (unlikely (!matrix)) return_trace (பொய்);

      auto indexes =
          + hb_range (src.rows * class_மொத்தம்)
          | hb_filter ([=] (unsigned index) { திரும்பு klass_mapping->has (index % class_மொத்தம்); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       இது,
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
