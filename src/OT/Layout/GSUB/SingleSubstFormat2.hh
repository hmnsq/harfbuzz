#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ SingleSubstFormat2_4
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, substitute);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (coverage.sanitize (c, ਇਹ) && substitute.sanitize (c));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+coverage).intersects (glyphs); }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਝੂਠ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ &cov = ਇਹ+coverage;
    ਸਵੈਚਾਲਿਤ &glyph_ਸੈੱਟ = c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs ();

    // Filter by active glyphs like closure does
    ਜੇ (substitute.len > glyph_ਸੈੱਟ.get_population () * 4)
    {
      ਲਈ (ਸਵੈਚਾਲਿਤ g : glyph_ਸੈੱਟ)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ i = cov.get_coverage (g);
	ਜੇ (i == NOT_COVERED || i >= substitute.len)
	  ਜਾਰੀ;
	c->depend_ਅੰਕੜੇ->ਜੋੜੋ_gsub_lookup (g, c->lookup_ਸੂਚਕਾਂਕ, substitute.arrayZ[i]);
      }

      ਵਾਪਸ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_ਸੈੱਟ, hb_ਪਹਿਲਾ)
    | hb_apply ([&] (ਅਚਲ hb_codepoint_pair_t &_) { c->depend_ਅੰਕੜੇ->ਜੋੜੋ_gsub_lookup (_.ਪਹਿਲਾ, c->lookup_ਸੂਚਕਾਂਕ, _.second); })
    ;
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ &cov = ਇਹ+coverage;
    ਸਵੈਚਾਲਿਤ &glyph_ਸੈੱਟ = c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs ();

    ਜੇ (substitute.len > glyph_ਸੈੱਟ.get_population () * 4)
    {
      ਲਈ (ਸਵੈਚਾਲਿਤ g : glyph_ਸੈੱਟ)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ i = cov.get_coverage (g);
	ਜੇ (i == NOT_COVERED || i >= substitute.len)
	  ਜਾਰੀ;
	c->output->ਜੋੜੋ (substitute.arrayZ[i]);
      }

      ਵਾਪਸ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_ਸੈੱਟ, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_sink (c->output)
    ;
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
    + hb_zip (ਇਹ+coverage, substitute)
    | hb_map (hb_ਦੂਜਾ)
    | hb_sink (c->output)
    ;
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  { ਵਾਪਸ c->len == 1 && (ਇਹ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        ਬਿਨਾਨਿਸ਼ਾਨੀ        ਸ਼ੁਰੂ_ਵਿੱਥ,
                        ਬਿਨਾਨਿਸ਼ਾਨੀ       *alternate_ਗਿਣਤੀ  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (glyph_id);
    ਜੇ (likely (ਸੂਚਕਾਂਕ_2 == NOT_COVERED))
    {
      ਜੇ (alternate_ਗਿਣਤੀ)
        *alternate_ਗਿਣਤੀ = 0;
      ਵਾਪਸ 0;
    }

    ਜੇ (alternate_ਗਿਣਤੀ && *alternate_ਗਿਣਤੀ && alternate_glyphs)
    {
      glyph_id = substitute[ਸੂਚਕਾਂਕ_2];

      *alternate_glyphs = glyph_id;
      *alternate_ਗਿਣਤੀ = 1;
    }

    ਵਾਪਸ 1;
  }

  ਖਾਲੀ
  collect_glyph_alternates (hb_map_t  *alternate_ਗਿਣਤੀ /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ਅਚਲ
  {
    + hb_zip (ਇਹ+coverage, substitute)
    | hb_apply ([&] (ਅਚਲ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ਖਾਲੀ
		{ hb_collect_glyph_alternates_ਜੋੜੋ (p.ਪਹਿਲਾ, p.second,
						    alternate_ਗਿਣਤੀ, alternate_glyphs); })
    ;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (c->ਬਫਰ->cur().codepoint);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 >= substitute.len)) return_trace (ਝੂਠ);

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->sync_so_far ();
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->ਬਫਰ->idx);
    }

    c->replace_glyph (substitute[ਸੂਚਕਾਂਕ_2]);

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->ਬਫਰ->idx - 1u);
    }

    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator,
                                                hb_codepoint_pair_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ substitutes =
      + it
      | hb_map (hb_ਦੂਜਾ)
      ;
    ਸਵੈਚਾਲਿਤ glyphs =
      + it
      | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
      ;
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!substitute.serialize (c, substitutes))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ it =
    + hb_zip (ਇਹ+coverage, substitute)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter (glyphset, hb_ਦੂਜਾ)
    | hb_map_retains_ਛਾਂਟੀ ([&] (hb_pair_t<hb_codepoint_t, ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ਵਾਪਸ hb_pair (glyph_map[p.ਪਹਿਲਾ], glyph_map[p.second]); })
    ;

    ਤਾਰਕਿਕ ret = ਤਾਰਕਿਕ (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */
