#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

HB_INTERNAL অন্তঃস্থ
শূন্য collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ধ্রুব Coverage &coverage,
                                        ধ্রুব শূন্য *base,
                                        glyph_data_t offsets,
                                        unsigned গুনতি,
                                        unsigned class_গুনতি,
                                        ধ্রুব hb_map_t &klass_mapping)
{
  প্রতি (ধ্রুব auto _ : hb_zip (coverage, hb_range (গুনতি)))
  {
    যদি (!c->glyph_set->has (_.first)) অনুবর্ত;
    hb_barrier ();
    unsigned offset = offsets[_.second];
    যদি (!offset) অনুবর্ত;

    ধ্রুব শূন্য *matrix = &StructAtOffset<ধ্রুব HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    unsigned rows = *static_cast<ধ্রুব HBUINT16 *> (matrix);
    glyph_data_t anchors = offsets;
    anchors.data = (ধ্রুব বর্ণ *) matrix + HBUINT16::static_size;

    hb_sorted_vector_t<unsigned> indexes;
    collect_ligature_anchor_indexes (rows, class_গুনতি, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
template <typename Types>
সংরচনা LigatureArray :
  Types::template ArrayOf<typename Types::template OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  প্রকারনির্দেশ AnchorMatrix<Types, HBUINT16> LigatureAttach;
  প্রকারনির্দেশ typename Types::template OffsetTo<LigatureAttach> LigatureAttachOffset;
  প্রকারনির্দেশ typename Types::template ArrayOf<LigatureAttachOffset> Super;

  ধ্রুব LigatureAttach& operator [] (পূর্ণাঙ্ক i_) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i = (unsigned পূর্ণাঙ্ক) i_;
    যদি (unlikely (i >= এতত্->len)) প্রত্যাগম Null (LigatureAttach);
    hb_barrier ();
    প্রত্যাগম এতত্+এতত্->arrayZ[i];
  }

  template <typename ...Ts>
  তার্কিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (Super::sanitize (c, এতত্, std::forward<Ts> (ds)...));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুব Coverage &coverage,
                                  unsigned class_গুনতি,
                                  ধ্রুব hb_map_t &klass_mapping) ধ্রুব
  {
    collect_ligature_variation_indices (c, coverage, এতত্, এতত্->arrayZ,
                                        এতত্->len, class_গুনতি, klass_mapping);
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  তার্কিক subset (hb_subset_context_t *c,
               Iterator             coverage,
               unsigned             class_গুনতি,
               ধ্রুব hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!c->serializer->extend_min (out)))  return_trace (অসত্য);

    তার্কিক ret = অসত্য;
    প্রতি (ধ্রুব auto _ : + hb_zip (coverage, *এতত্)
                        | hb_filter (glyph_map, hb_first))
    {
      ধ্রুব LigatureAttach& src = (এতত্ + _.second);
      তার্কিক non_empty = + hb_range (src.rows * class_গুনতি)
                       | hb_filter ([=] (unsigned index) { প্রত্যাগম klass_mapping->has (index % class_গুনতি); })
                       | hb_map ([&] (ধ্রুব unsigned index) { প্রত্যাগম !src.offset_is_null (index / class_গুনতি, index % class_গুনতি, class_গুনতি); })
                       | hb_any;

      যদি (!non_empty) অনুবর্ত;

      auto *matrix = out->serialize_append (c->serializer);
      যদি (unlikely (!matrix)) return_trace (অসত্য);

      auto indexes =
          + hb_range (src.rows * class_গুনতি)
          | hb_filter ([=] (unsigned index) { প্রত্যাগম klass_mapping->has (index % class_গুনতি); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       এতত্,
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
