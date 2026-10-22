#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

HB_INTERNAL অন্তঃরেখ
শূন্য collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ধ্রুবক Coverage &coverage,
                                        ধ্রুবক শূন্য *base,
                                        glyph_তথ্য_t offsets,
                                        unsigned গুনতি,
                                        unsigned class_গুনতি,
                                        ধ্রুবক hb_map_t &klass_mapping)
{
  জন্য (ধ্রুবক auto _ : hb_zip (coverage, hb_range (গুনতি)))
  {
    যদি (!c->glyph_set->has (_.first)) চালু;
    hb_barrier ();
    unsigned offset = offsets[_.second];
    যদি (!offset) চালু;

    ধ্রুবক শূন্য *matrix = &StructAtOffset<ধ্রুবক HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    unsigned rows = *static_cast<ধ্রুবক HBUINT16 *> (matrix);
    glyph_তথ্য_t anchors = offsets;
    anchors.তথ্য = (ধ্রুবক অক্ষর *) matrix + HBUINT16::static_আকার;

    hb_sorted_vector_t<unsigned> indexes;
    collect_ligature_anchor_indexes (rows, class_গুনতি, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
template <typename Types>
কাঠামো LigatureArray :
  Types::template ArrayOf<typename Types::template OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  প্রকারলেখ AnchorMatrix<Types, HBUINT16> LigatureAttach;
  প্রকারলেখ typename Types::template OffsetTo<LigatureAttach> LigatureAttachOffset;
  প্রকারলেখ typename Types::template ArrayOf<LigatureAttachOffset> Super;

  ধ্রুবক LigatureAttach& operator [] (পূর্ণসংখ্যা i_) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i = (unsigned পূর্ণসংখ্যা) i_;
    যদি (unlikely (i >= এই->len)) ফেরত Null (LigatureAttach);
    hb_barrier ();
    ফেরত এই+এই->arrayZ[i];
  }

  template <typename ...Ts>
  যৌক্তিক sanitize (hb_sanitize_context_t *c, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (Super::sanitize (c, এই, std::forward<Ts> (ds)...));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুবক Coverage &coverage,
                                  unsigned class_গুনতি,
                                  ধ্রুবক hb_map_t &klass_mapping) ধ্রুবক
  {
    collect_ligature_variation_indices (c, coverage, এই, এই->arrayZ,
                                        এই->len, class_গুনতি, klass_mapping);
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক subset (hb_subset_context_t *c,
               Iterator             coverage,
               unsigned             class_গুনতি,
               ধ্রুবক hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!c->serializer->extend_min (out)))  return_trace (মিথ্যা);

    যৌক্তিক ret = মিথ্যা;
    জন্য (ধ্রুবক auto _ : + hb_zip (coverage, *এই)
                        | hb_filter (glyph_map, hb_first))
    {
      ধ্রুবক LigatureAttach& src = (এই + _.second);
      যৌক্তিক non_empty = + hb_range (src.rows * class_গুনতি)
                       | hb_filter ([=] (unsigned সূচক_2) { ফেরত klass_mapping->has (সূচক_2 % class_গুনতি); })
                       | hb_map ([&] (ধ্রুবক unsigned সূচক_2) { ফেরত !src.offset_is_null (সূচক_2 / class_গুনতি, সূচক_2 % class_গুনতি, class_গুনতি); })
                       | hb_any;

      যদি (!non_empty) চালু;

      auto *matrix = out->serialize_append (c->serializer);
      যদি (unlikely (!matrix)) return_trace (মিথ্যা);

      auto indexes =
          + hb_range (src.rows * class_গুনতি)
          | hb_filter ([=] (unsigned সূচক_2) { ফেরত klass_mapping->has (সূচক_2 % class_গুনতি); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       এই,
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
