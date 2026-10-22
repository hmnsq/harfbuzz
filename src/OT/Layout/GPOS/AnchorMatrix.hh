#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_ANCHORMATRIX_HH

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ collect_ligature_anchor_indexes (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀਆਂ,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ,
                                     ਅਚਲ hb_map_t &klass_mapping,
                                     hb_sorted_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &indexes)
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ = 0; ਪੰਕਤੀ < ਪੰਕਤੀਆਂ; ਪੰਕਤੀ++)
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਾਨਾ = 0; ਖਾਨਾ < class_ਗਿਣਤੀ; ਖਾਨਾ++)
      ਜੇ (klass_mapping.has (ਖਾਨਾ))
        indexes.push (ਪੰਕਤੀ * class_ਗਿਣਤੀ + ਖਾਨਾ);
}

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                                      glyph_ਅੰਕੜੇ_t offsets,
                                      hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ> indexes)
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : indexes)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = offsets[i];
    ਜੇ (ਵਿੱਥ)
      Struct_At_ਵਿੱਥ<ਅਚਲ Anchor> (ਅਧਾਰ, ਵਿੱਥ).collect_variation_indices (c);
  }
}

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ collect_anchor_indexes (ਅਚਲ Coverage &coverage,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀਆਂ,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ,
                             ਅਚਲ hb_ਸੈੱਟ_t &glyphset,
                             ਅਚਲ hb_map_t &klass_mapping,
                             hb_sorted_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &indexes)
{
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : hb_zip (coverage, hb_range (ਪੰਕਤੀਆਂ)))
    ਜੇ (glyphset.has (_.ਪਹਿਲਾ))
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਾਨਾ = 0; ਖਾਨਾ < class_ਗਿਣਤੀ; ਖਾਨਾ++)
        ਜੇ (klass_mapping.has (ਖਾਨਾ))
          indexes.push (_.second * class_ਗਿਣਤੀ + ਖਾਨਾ);
}

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ collect_subset_anchor_indexes (ਅਚਲ Coverage &coverage,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀਆਂ,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ,
                                    glyph_ਅੰਕੜੇ_t offsets,
                                    ਅਚਲ hb_ਸੈੱਟ_t &glyphset,
                                    ਅਚਲ hb_map_t &glyph_map,
                                    ਅਚਲ hb_map_t &klass_mapping,
                                    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &indexes)
{
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : hb_zip (coverage, hb_range (ਪੰਕਤੀਆਂ)))
  {
    ਜੇ (!glyphset.has (_.ਪਹਿਲਾ)) ਜਾਰੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ = _.second;
    ਤਾਰਕਿਕ non_ਖਾਲੀ_ਥਾਵਾਂ = ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਾਨਾ = 0; ਖਾਨਾ < class_ਗਿਣਤੀ; ਖਾਨਾ++)
      ਜੇ (klass_mapping.has (ਖਾਨਾ) && offsets[ਪੰਕਤੀ * class_ਗਿਣਤੀ + ਖਾਨਾ])
      {
        non_ਖਾਲੀ_ਥਾਵਾਂ = ਸੱਚ;
        ਰੋਕੋ;
      }
    ਜੇ (!non_ਖਾਲੀ_ਥਾਵਾਂ) ਜਾਰੀ;

    new_coverage.push (glyph_map.get (_.ਪਹਿਲਾ));
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਾਨਾ = 0; ਖਾਨਾ < class_ਗਿਣਤੀ; ਖਾਨਾ++)
      ਜੇ (klass_mapping.has (ਖਾਨਾ))
        indexes.push (ਪੰਕਤੀ * class_ਗਿਣਤੀ + ਖਾਨਾ);
  }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types, ਕਿਸਮ_ਨਾਂ ਪੰਕਤੀ_ਗਿਣਤੀ=ਕਿਸਮ_ਨਾਂ Types::HBUINT>
ਬਣਤਰ Anchor_ਮੈਟ੍ਰਿਕਸ
{
  ਪੰਕਤੀ_ਗਿਣਤੀ      ਪੰਕਤੀਆਂ;                   /* Number of rows */
  Unsized_Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Anchor, Anchor_ਮੈਟ੍ਰਿਕਸ>>
                ਮੈਟ੍ਰਿਕਸ_Z;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (ਆਕਾਰ (ਪੰਕਤੀ_ਗਿਣਤੀ), ਮੈਟ੍ਰਿਕਸ_Z);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!c->check_struct (ਇਹ)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਜੇ (unlikely (hb_unsigned_mul_overflows (ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ))) return_trace (ਝੂਠ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਪੰਕਤੀਆਂ * ਖਾਨੇ_ਗਿਣਤੀ;
    ਜੇ (!c->check_array (ਮੈਟ੍ਰਿਕਸ_Z.arrayZ, ਗਿਣਤੀ_2)) return_trace (ਝੂਠ);

    ਜੇ (c->lazy_some_gpos)
      return_trace (ਸੱਚ);

    hb_barrier ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!ਮੈਟ੍ਰਿਕਸ_Z[i].sanitize (c, ਇਹ)) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਅਚਲ Anchor& get_anchor (hb_ot_apply_context_t *c,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪੰਕਤੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਾਨਾ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਤਾਰਕਿਕ *ਮਿਲਿਆ) ਅਚਲ
  {
    *ਮਿਲਿਆ = ਝੂਠ;
    ਜੇ (unlikely (ਪੰਕਤੀ >= ਪੰਕਤੀਆਂ || ਖਾਨਾ >= ਖਾਨੇ_ਗਿਣਤੀ)) ਵਾਪਸ Null (Anchor);
    ਸਵੈਚਾਲਿਤ &ਵਿੱਥ = ਮੈਟ੍ਰਿਕਸ_Z[ਪੰਕਤੀ * ਖਾਨੇ_ਗਿਣਤੀ + ਖਾਨਾ];
    ਜੇ (unlikely (!ਵਿੱਥ.sanitize (&c->sanitizer, ਇਹ))) ਵਾਪਸ Null (Anchor);
    hb_barrier ();
    *ਮਿਲਿਆ = !ਵਿੱਥ.is_null ();
    ਵਾਪਸ ਇਹ+ਵਿੱਥ;
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ> indexes) ਅਚਲ
  {
    collect_anchor_variation_indices (c, ਇਹ, ਮੈਟ੍ਰਿਕਸ_Z.arrayZ, indexes);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਬਿਨਾਨਿਸ਼ਾਨੀ             ਅੰਕ_ਪੰਕਤੀਆਂ,
               Iterator             ਸੂਚਕਾਂਕ_iter) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);

    ਜੇ (!ਸੂਚਕਾਂਕ_iter) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ)))  return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਪੰਕਤੀਆਂ = ਅੰਕ_ਪੰਕਤੀਆਂ;
    ਲਈ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ i : ਸੂਚਕਾਂਕ_iter)
    {
      ਸਵੈਚਾਲਿਤ *ਵਿੱਥ = c->serializer->embed (ਮੈਟ੍ਰਿਕਸ_Z[i]);
      ਜੇ (!ਵਿੱਥ) return_trace (ਝੂਠ);
      ਵਿੱਥ->serialize_subset (c, ਮੈਟ੍ਰਿਕਸ_Z[i], ਇਹ);
    }

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ ਵਿੱਥ_is_null (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਖਾਨਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ) ਅਚਲ
  {
    ਜੇ (unlikely (ਪੰਕਤੀ >= ਪੰਕਤੀਆਂ || ਖਾਨਾ >= ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ)) ਵਾਪਸ ਸੱਚ;
    ਸਵੈਚਾਲਿਤ &ਵਿੱਥ = ਮੈਟ੍ਰਿਕਸ_Z[ਪੰਕਤੀ * ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ + ਖਾਨਾ];
    ਵਾਪਸ ਵਿੱਥ.is_null ();
  }
};


}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
