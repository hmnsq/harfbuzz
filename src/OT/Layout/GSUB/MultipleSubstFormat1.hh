#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "Sequence.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ਗੁਣਜ_Subst_Format1_2
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, sequence);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (coverage.sanitize (c, ਇਹ) && sequence.sanitize (c, ਇਹ));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+coverage).intersects (glyphs); }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਸੱਚ; }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    + hb_zip (ਇਹ+coverage, sequence)
    | hb_filter (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([c] (ਅਚਲ Sequence<Types> &_) { _.ਘੇਰਾ (c); })
    ;
  }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (ਇਹ+coverage, sequence)
    | hb_filter (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), hb_ਪਹਿਲਾ)
    | hb_apply ([&] (ਅਚਲ hb_pair_t<hb_codepoint_t, ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Sequence<Types>>&> &_)
                {
                  ਅਚਲ Sequence<Types>& s = ਇਹ+_.second;
                  s.depend (c, _.ਪਹਿਲਾ);
                })
    ;
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
    + hb_zip (ਇਹ+coverage, sequence)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([c] (ਅਚਲ Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  { ਵਾਪਸ c->len == 1 && (ਇਹ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (c->ਬਫਰ->cur().codepoint);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    return_trace ((ਇਹ+sequence[ਸੂਚਕਾਂਕ_2]).apply (c));
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ sequences =
      + it
      | hb_map (hb_ਦੂਜਾ)
      ;
    ਸਵੈਚਾਲਿਤ glyphs =
      + it
      | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
      ;
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);

    ਜੇ (unlikely (!sequence.serialize (c, sequences.ਲੰਮਾਈ))) return_trace (ਝੂਠ);

    ਲਈ (ਸਵੈਚਾਲਿਤ& pair : hb_zip (sequences, sequence))
    {
      ਜੇ (unlikely (!pair.second
		    .serialize_serialize (c, pair.ਪਹਿਲਾ)))
        return_trace (ਝੂਠ);
    }

    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;
    + hb_zip (ਇਹ+coverage, sequence)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->sequence, ਇਹ), hb_ਦੂਜਾ)
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    ਬਾਹਰਲਾ->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (ਤਾਰਕਿਕ (new_coverage));
  }
};

}
}
}


#ਜੇ_ਅੰਤ /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
