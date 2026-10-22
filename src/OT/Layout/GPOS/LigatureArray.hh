#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_INTERNAL مضمن
فراغ collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ثابت Coverage &coverage,
                                        ثابت فراغ *base,
                                        glyph_data_t offsets,
                                        unsigned count,
                                        unsigned class_count,
                                        ثابت hb_map_t &klass_mapping)
{
  لكل (ثابت auto _ : hb_zip (coverage, hb_range (count)))
  {
    𐤀𐤌 (!c->glyph_set->has (_.first)) استمر;
    hb_barrier ();
    unsigned offset = offsets[_.second];
    𐤀𐤌 (!offset) استمر;

    ثابت فراغ *matrix = &StructAtOffset<ثابت HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    unsigned rows = *static_cast<ثابت HBUINT16 *> (matrix);
    glyph_data_t anchors = offsets;
    anchors.data = (ثابت حرف *) matrix + HBUINT16::static_size;

    hb_sorted_vector_t<unsigned> indexes;
    collect_ligature_anchor_indexes (rows, class_count, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
template <typename Types>
بنية LigatureArray :
  Types::template ArrayOf<typename Types::template OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  تعريفنوع AnchorMatrix<Types, HBUINT16> LigatureAttach;
  تعريفنوع typename Types::template OffsetTo<LigatureAttach> LigatureAttachOffset;
  تعريفنوع typename Types::template ArrayOf<LigatureAttachOffset> Super;

  ثابت LigatureAttach& operator [] (صحيح i_) ثابت
  {
    unsigned صحيح i = (unsigned صحيح) i_;
    𐤀𐤌 (unlikely (i >= هذا->len)) أرجع Null (LigatureAttach);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }

  template <typename ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Super::sanitize (c, هذا, std::forward<Ts> (ds)...));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage,
                                  unsigned class_count,
                                  ثابت hb_map_t &klass_mapping) ثابت
  {
    collect_ligature_variation_indices (c, coverage, هذا, هذا->arrayZ,
                                        هذا->len, class_count, klass_mapping);
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               Iterator             coverage,
               unsigned             class_count,
               ثابت hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (هذا);
    𐤀𐤌 (unlikely (!c->serializer->extend_min (out)))  return_trace (خطأ);

    منطقي ret = خطأ;
    لكل (ثابت auto _ : + hb_zip (coverage, *هذا)
                        | hb_filter (glyph_map, hb_first))
    {
      ثابت LigatureAttach& src = (هذا + _.second);
      منطقي non_empty = + hb_range (src.rows * class_count)
                       | hb_filter ([=] (unsigned index) { أرجع klass_mapping->has (index % class_count); })
                       | hb_map ([&] (ثابت unsigned index) { أرجع !src.offset_is_null (index / class_count, index % class_count, class_count); })
                       | hb_any;

      𐤀𐤌 (!non_empty) استمر;

      auto *matrix = out->serialize_append (c->serializer);
      𐤀𐤌 (unlikely (!matrix)) return_trace (خطأ);

      auto indexes =
          + hb_range (src.rows * class_count)
          | hb_filter ([=] (unsigned index) { أرجع klass_mapping->has (index % class_count); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       هذا,
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
