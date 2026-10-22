#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ଅଚଳ Coverage &coverage,
                                        ଅଚଳ ଶୂନ୍ୟ *base,
                                        glyph_data_t offsets,
                                        unsigned ଗଣନା_2,
                                        unsigned class_ଗଣନା,
                                        ଅଚଳ hb_map_t &klass_mapping)
{
  ପାଇଁ (ଅଚଳ auto _ : hb_zip (coverage, hb_range (ଗଣନା_2)))
  {
    ଯଦି (!c->glyph_set->has (_.first)) ଜାରି;
    hb_barrier ();
    unsigned offset = offsets[_.second];
    ଯଦି (!offset) ଜାରି;

    ଅଚଳ ଶୂନ୍ୟ *matrix = &StructAtOffset<ଅଚଳ HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    unsigned rows = *static_cast<ଅଚଳ HBUINT16 *> (matrix);
    glyph_data_t anchors = offsets;
    anchors.data = (ଅଚଳ ଅକ୍ଷର *) matrix + HBUINT16::static_size;

    hb_sorted_vector_t<unsigned> indexes;
    collect_ligature_anchor_indexes (rows, class_ଗଣନା, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
template <typename Types>
ସଂରଚନା LigatureArray :
  Types::template ArrayOf<typename Types::template OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା AnchorMatrix<Types, HBUINT16> LigatureAttach;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::template OffsetTo<LigatureAttach> LigatureAttachOffset;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::template ArrayOf<LigatureAttachOffset> Super;

  ଅଚଳ LigatureAttach& operator [] (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i_) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i_;
    ଯଦି (unlikely (i >= ଏହି->len)) ଫେରନ୍ତୁ Null (LigatureAttach);
    hb_barrier ();
    ଫେରନ୍ତୁ ଏହି+ଏହି->arrayZ[i];
  }

  template <typename ...Ts>
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (Super::sanitize (c, ଏହି, std::forward<Ts> (ds)...));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ଅଚଳ Coverage &coverage,
                                  unsigned class_ଗଣନା,
                                  ଅଚଳ hb_map_t &klass_mapping) ଅଚଳ
  {
    collect_ligature_variation_indices (c, coverage, ଏହି, ଏହି->arrayZ,
                                        ଏହି->len, class_ଗଣନା, klass_mapping);
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ subset (hb_subset_context_t *c,
               Iterator             coverage,
               unsigned             class_ଗଣନା,
               ଅଚଳ hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out)))  return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ ret = ମିଥ୍ୟା;
    ପାଇଁ (ଅଚଳ auto _ : + hb_zip (coverage, *ଏହି)
                        | hb_filter (glyph_map, hb_first))
    {
      ଅଚଳ LigatureAttach& src = (ଏହି + _.second);
      ତାର୍କିକ non_empty = + hb_range (src.rows * class_ଗଣନା)
                       | hb_filter ([=] (unsigned index) { ଫେରନ୍ତୁ klass_mapping->has (index % class_ଗଣନା); })
                       | hb_map ([&] (ଅଚଳ unsigned index) { ଫେରନ୍ତୁ !src.offset_is_null (index / class_ଗଣନା, index % class_ଗଣନା, class_ଗଣନା); })
                       | hb_any;

      ଯଦି (!non_empty) ଜାରି;

      auto *matrix = out->serialize_append (c->serializer);
      ଯଦି (unlikely (!matrix)) return_trace (ମିଥ୍ୟା);

      auto indexes =
          + hb_range (src.rows * class_ଗଣନା)
          | hb_filter ([=] (unsigned index) { ଫେରନ୍ତୁ klass_mapping->has (index % class_ଗଣନା); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       ଏହି,
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
