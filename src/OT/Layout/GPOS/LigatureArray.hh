#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_LIGATUREARRAY_HH

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ਅਚਲ Coverage &coverage,
                                        ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                                        glyph_ਅੰਕੜੇ_t offsets,
                                        ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2,
                                        ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ,
                                        ਅਚਲ hb_map_t &klass_mapping)
{
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : hb_zip (coverage, hb_range (ਗਿਣਤੀ_2)))
  {
    ਜੇ (!c->glyph_ਸੈੱਟ->has (_.ਪਹਿਲਾ)) ਜਾਰੀ;
    hb_barrier ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = offsets[_.second];
    ਜੇ (!ਵਿੱਥ) ਜਾਰੀ;

    ਅਚਲ ਖਾਲੀ *ਮੈਟ੍ਰਿਕਸ = &Struct_At_ਵਿੱਥ<ਅਚਲ HBUINT16> (ਅਧਾਰ, ਵਿੱਥ);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀਆਂ = *static_cast<ਅਚਲ HBUINT16 *> (ਮੈਟ੍ਰਿਕਸ);
    glyph_ਅੰਕੜੇ_t anchors = offsets;
    anchors.ਅੰਕੜੇ = (ਅਚਲ ਅੱਖਰ *) ਮੈਟ੍ਰਿਕਸ + HBUINT16::static_ਮਾਪ;

    hb_sorted_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> indexes;
    collect_ligature_anchor_indexes (ਪੰਕਤੀਆਂ, class_ਗਿਣਤੀ, klass_mapping, indexes);
    collect_anchor_variation_indices (c, ਮੈਟ੍ਰਿਕਸ, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ LigatureArray :
  Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Anchor_ਮੈਟ੍ਰਿਕਸ<Types, HBUINT16>>>
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Anchor_ਮੈਟ੍ਰਿਕਸ<Types, HBUINT16> LigatureAttach;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<LigatureAttach> Ligature_Attach_ਵਿੱਥ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<Ligature_Attach_ਵਿੱਥ> Super;

  ਅਚਲ LigatureAttach& ਸੰਚਾਲਕ [] (ਪੂਰਨ i_) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) i_;
    ਜੇ (unlikely (i >= ਇਹ->len)) ਵਾਪਸ Null (LigatureAttach);
    hb_barrier ();
    ਵਾਪਸ ਇਹ+ਇਹ->arrayZ[i];
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (Super::sanitize (c, ਇਹ, std::forward<Ts> (ds)...));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ਅਚਲ Coverage &coverage,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ,
                                  ਅਚਲ hb_map_t &klass_mapping) ਅਚਲ
  {
    collect_ligature_variation_indices (c, coverage, ਇਹ, ਇਹ->arrayZ,
                                        ਇਹ->len, class_ਗਿਣਤੀ, klass_mapping);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               Iterator             coverage,
               ਬਿਨਾਨਿਸ਼ਾਨੀ             class_ਗਿਣਤੀ,
               ਅਚਲ hb_map_t      *klass_mapping,
               hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> &new_coverage /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ)))  return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : + hb_zip (coverage, *ਇਹ)
                        | hb_filter (glyph_map, hb_ਪਹਿਲਾ))
    {
      ਅਚਲ LigatureAttach& src = (ਇਹ + _.second);
      ਤਾਰਕਿਕ non_ਖਾਲੀ_ਥਾਵਾਂ = + hb_range (src.ਪੰਕਤੀਆਂ * class_ਗਿਣਤੀ)
                       | hb_filter ([=] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2) { ਵਾਪਸ klass_mapping->has (ਸੂਚਕਾਂਕ_2 % class_ਗਿਣਤੀ); })
                       | hb_map ([&] (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2) { ਵਾਪਸ !src.ਵਿੱਥ_is_null (ਸੂਚਕਾਂਕ_2 / class_ਗਿਣਤੀ, ਸੂਚਕਾਂਕ_2 % class_ਗਿਣਤੀ, class_ਗਿਣਤੀ); })
                       | hb_any;

      ਜੇ (!non_ਖਾਲੀ_ਥਾਵਾਂ) ਜਾਰੀ;

      ਸਵੈਚਾਲਿਤ *ਮੈਟ੍ਰਿਕਸ = ਬਾਹਰਲਾ->serialize_append (c->serializer);
      ਜੇ (unlikely (!ਮੈਟ੍ਰਿਕਸ)) return_trace (ਝੂਠ);

      ਸਵੈਚਾਲਿਤ indexes =
          + hb_range (src.ਪੰਕਤੀਆਂ * class_ਗਿਣਤੀ)
          | hb_filter ([=] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2) { ਵਾਪਸ klass_mapping->has (ਸੂਚਕਾਂਕ_2 % class_ਗਿਣਤੀ); })
          ;
      ret |= ਮੈਟ੍ਰਿਕਸ->serialize_subset (c,
				       _.second,
				       ਇਹ,
				       src.ਪੰਕਤੀਆਂ,
				       indexes);

      hb_codepoint_t new_gid = glyph_map.get (_.ਪਹਿਲਾ);
      new_coverage.push (new_gid);
    }
    return_trace (ret);
  }
};


}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_LIGATUREARRAY_HH */
