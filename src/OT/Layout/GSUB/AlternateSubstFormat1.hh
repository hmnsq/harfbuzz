#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "AlternateSet.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ AlternateSubstFormat1_2
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Alternate_ਸੈੱਟ<Types>>>
                alternate_ਸੈੱਟ;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, alternate_ਸੈੱਟ);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (coverage.sanitize (c, ਇਹ) && alternate_ਸੈੱਟ.sanitize (c, ਇਹ));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+coverage).intersects (glyphs); }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਝੂਠ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (ਇਹ+coverage, alternate_ਸੈੱਟ)
    | hb_filter (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), hb_ਪਹਿਲਾ)
    | hb_apply ([&] (ਅਚਲ hb_pair_t<hb_codepoint_t, ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Alternate_ਸੈੱਟ<Types>>&> &_)
                {
                  ਅਚਲ Alternate_ਸੈੱਟ<Types>& as = ਇਹ+_.second;
                  as.depend (c, _.ਪਹਿਲਾ);
                })
    ;
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    + hb_zip (ਇਹ+coverage, alternate_ਸੈੱਟ)
    | hb_filter (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([c] (ਅਚਲ Alternate_ਸੈੱਟ<Types> &_) { _.ਘੇਰਾ (c); })
    ;
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
    + hb_zip (ਇਹ+coverage, alternate_ਸੈੱਟ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([c] (ਅਚਲ Alternate_ਸੈੱਟ<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  { ਵਾਪਸ c->len == 1 && (ਇਹ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ
  get_glyph_alternates (hb_codepoint_t  gid,
                        ਬਿਨਾਨਿਸ਼ਾਨੀ        ਸ਼ੁਰੂ_ਵਿੱਥ,
                        ਬਿਨਾਨਿਸ਼ਾਨੀ       *alternate_ਗਿਣਤੀ  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+alternate_ਸੈੱਟ[(ਇਹ+coverage).get_coverage (gid)])
           .get_alternates (ਸ਼ੁਰੂ_ਵਿੱਥ, alternate_ਗਿਣਤੀ, alternate_glyphs); }

  ਖਾਲੀ
  collect_glyph_alternates (hb_map_t  *alternate_ਗਿਣਤੀ /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ਅਚਲ
  {
    + hb_iter (alternate_ਸੈੱਟ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_zip (ਇਹ+coverage)
    | hb_apply ([&] (ਅਚਲ hb_pair_t<ਅਚਲ Alternate_ਸੈੱਟ<Types> &, hb_codepoint_t> _) {
		  _.ਪਹਿਲਾ.collect_alternates (_.second, alternate_ਗਿਣਤੀ, alternate_glyphs);
		})
    ;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (c->ਬਫਰ->cur().codepoint);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    return_trace ((ਇਹ+alternate_ਸੈੱਟ[ਸੂਚਕਾਂਕ_2]).apply (c));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ GlyphID>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!alternate_ਸੈੱਟ.serialize (c, glyphs.ਲੰਮਾਈ))) return_trace (ਝੂਠ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < glyphs.ਲੰਮਾਈ; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ alternate_len = alternate_len_list[i];
      ਜੇ (unlikely (!alternate_ਸੈੱਟ[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (ਝੂਠ);
      alternate_glyphs_list += alternate_len;
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
    + hb_zip (ਇਹ+coverage, alternate_ਸੈੱਟ)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->alternate_ਸੈੱਟ, ਇਹ), hb_ਦੂਜਾ)
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

#ਜੇ_ਅੰਤ  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
