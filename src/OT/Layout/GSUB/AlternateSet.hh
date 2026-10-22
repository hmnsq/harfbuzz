#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_ALTERNATESET_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_ALTERNATESET_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Alternate_ਸੈੱਟ
{
  ਸੁਰੱਖਿਅਤ:
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, alternates);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (alternates.sanitize (c));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ hb_any (alternates, glyphs); }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  { c->output->ਜੋੜੋ_array (alternates.arrayZ, alternates.len); }

  ਖਾਲੀ depend (hb_depend_context_t *c, hb_codepoint_t ਸਰੋਤ) ਅਚਲ
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ਅਚਲ hb_codepoint_t &target) { c->depend_ਅੰਕੜੇ->ਜੋੜੋ_gsub_lookup (ਸਰੋਤ, c->lookup_ਸੂਚਕਾਂਕ, target); })
    ;
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  { c->output->ਜੋੜੋ_array (alternates.arrayZ, alternates.len); }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = alternates.len;

    ਜੇ (unlikely (!ਗਿਣਤੀ_2)) return_trace (ਝੂਠ);

    hb_ਨਕਾਬ_t glyph_ਨਕਾਬ = c->ਬਫਰ->cur().ਨਕਾਬ;
    hb_ਨਕਾਬ_t lookup_ਨਕਾਬ = c->lookup_ਨਕਾਬ;

    /* Note: This breaks badly if two features enabled this lookup together. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਿਸਕਾਓ = hb_ctz (lookup_ਨਕਾਬ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ alt_ਸੂਚਕਾਂਕ = ((lookup_ਨਕਾਬ & glyph_ਨਕਾਬ) >> ਖਿਸਕਾਓ);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    ਜੇ (alt_ਸੂਚਕਾਂਕ == HB_OT_MAP_MAX_ਮੁੱਲ && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (0, c->ਬਫਰ->len);
      alt_ਸੂਚਕਾਂਕ = c->random_ਸੰਖਿਆ () % ਗਿਣਤੀ_2 + 1;
    }

    ਜੇ (unlikely (alt_ਸੂਚਕਾਂਕ > ਗਿਣਤੀ_2 || alt_ਸੂਚਕਾਂਕ == 0)) return_trace (ਝੂਠ);

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->sync_so_far ();
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->ਬਫਰ->idx);
    }

    c->replace_glyph (alternates[alt_ਸੂਚਕਾਂਕ - 1]);

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->ਬਫਰ->idx - 1u);
    }

    return_trace (ਸੱਚ);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ
  get_alternates (ਬਿਨਾਨਿਸ਼ਾਨੀ        ਸ਼ੁਰੂ_ਵਿੱਥ,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ       *alternate_ਗਿਣਤੀ  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ਅਚਲ
  {
    ਜੇ (alternates.len && alternate_ਗਿਣਤੀ && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, alternate_ਗਿਣਤੀ)
      | hb_sink (hb_array (alternate_glyphs, *alternate_ਗਿਣਤੀ))
      ;
    }
    ਵਾਪਸ alternates.len;
  }

  ਖਾਲੀ
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_ਗਿਣਤੀ /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ਅਚਲ
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_codepoint_t> _) { ਵਾਪਸ hb_pair (gid + (_.ਪਹਿਲਾ << 24), _.second); })
    | hb_apply ([&] (ਅਚਲ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ਖਾਲੀ
		{ hb_collect_glyph_alternates_ਜੋੜੋ (p.ਪਹਿਲਾ, p.second,
						    alternate_ਗਿਣਤੀ, alternate_glyphs); })
    ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
            hb_requires (hb_is_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (alternates.serialize (c, alts));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    return_trace (ਬਾਹਰਲਾ->serialize (c->serializer, it) &&
                  ਬਾਹਰਲਾ->alternates);
  }
};

}
}
}


#ਜੇ_ਅੰਤ /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
