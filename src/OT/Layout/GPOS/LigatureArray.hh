#ifndef OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#define OT_LAYOUT_GPOS_LIGATUREARRAY_HH

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

HB_INTERNAL درونخطی
تهی collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ثابت Coverage &coverage,
                                        ثابت تهی *base,
                                        glyph_data_t offsets,
                                        unsigned شمار,
                                        unsigned class_شمار,
                                        ثابت hb_map_t &klass_mapping)
{
  برای (ثابت auto _ : hb_zip (coverage, hb_range (شمار)))
  {
    اگر (!c->glyph_set->has (_.first)) ادامه;
    hb_barrier ();
    unsigned offset = offsets[_.second];
    اگر (!offset) ادامه;

    ثابت تهی *matrix = &StructAtOffset<ثابت HBUINT16> (base, offset);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    unsigned rows = *static_cast<ثابت HBUINT16 *> (matrix);
    glyph_data_t anchors = offsets;
    anchors.data = (ثابت نویسه *) matrix + HBUINT16::static_size;

    hb_sorted_vector_t<unsigned> indexes;
    collect_ligature_anchor_indexes (rows, class_شمار, klass_mapping, indexes);
    collect_anchor_variation_indices (c, matrix, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
template <typename Types>
ساختار LigatureArray :
  Types::template ArrayOf<typename Types::template OffsetTo<AnchorMatrix<Types, HBUINT16>>>
{
  تعریفنوع AnchorMatrix<Types, HBUINT16> LigatureAttach;
  تعریفنوع typename Types::template OffsetTo<LigatureAttach> LigatureAttachOffset;
  تعریفنوع typename Types::template ArrayOf<LigatureAttachOffset> Super;

  ثابت LigatureAttach& operator [] (صحیح i_) ثابت
  {
    unsigned صحیح i = (unsigned صحیح) i_;
    اگر (unlikely (i >= این->len)) بازگشت Null (LigatureAttach);
    hb_barrier ();
    بازگشت این+این->arrayZ[i];
  }

  template <typename ...Ts>
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (Super::sanitize (c, این, std::forward<Ts> (ds)...));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage,
                                  unsigned class_شمار,
                                  ثابت hb_map_t &klass_mapping) ثابت
  {
    collect_ligature_variation_indices (c, coverage, این, این->arrayZ,
                                        این->len, class_شمار, klass_mapping);
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  منطقی subset (hb_subset_context_t *c,
               Iterator             coverage,
               unsigned             class_شمار,
               ثابت hb_map_t      *klass_mapping,
               hb_sorted_vector_t<hb_codepoint_t> &new_coverage /* OUT */) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!c->serializer->extend_min (out)))  return_trace (نادرست);

    منطقی ret = نادرست;
    برای (ثابت auto _ : + hb_zip (coverage, *این)
                        | hb_filter (glyph_map, hb_first))
    {
      ثابت LigatureAttach& src = (این + _.second);
      منطقی non_empty = + hb_range (src.rows * class_شمار)
                       | hb_filter ([=] (unsigned index) { بازگشت klass_mapping->has (index % class_شمار); })
                       | hb_map ([&] (ثابت unsigned index) { بازگشت !src.offset_is_null (index / class_شمار, index % class_شمار, class_شمار); })
                       | hb_any;

      اگر (!non_empty) ادامه;

      auto *matrix = out->serialize_append (c->serializer);
      اگر (unlikely (!matrix)) return_trace (نادرست);

      auto indexes =
          + hb_range (src.rows * class_شمار)
          | hb_filter ([=] (unsigned index) { بازگشت klass_mapping->has (index % class_شمار); })
          ;
      ret |= matrix->serialize_subset (c,
				       _.second,
				       این,
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
