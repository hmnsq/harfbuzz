#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_LIGATURESET_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_LIGATURESET_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "Ligature.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Ligature_ਸੈੱਟ
{
  ਜਨਤਕ:
  Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_ਮਾਪ_ARRAY (2, ligature);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ligature.sanitize (c, ਇਹ));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਵਾਪਸ
    + hb_iter (ligature)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_map ([glyphs] (ਅਚਲ Ligature<Types> &_) { ਵਾਪਸ _.intersects (glyphs); })
    | hb_any
    ;
  }

  ਤਾਰਕਿਕ intersects_lig_glyph (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਵਾਪਸ
    + hb_iter (ligature)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_map ([glyphs] (ਅਚਲ Ligature<Types> &_) { 
      ਵਾਪਸ _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    + hb_iter (ligature)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([c] (ਅਚਲ Ligature<Types> &_) { _.ਘੇਰਾ (c); })
    ;
  }

  ਖਾਲੀ depend (hb_depend_context_t *c, hb_codepoint_t ਪਹਿਲਾ) ਅਚਲ
  {
    + hb_iter (ligature)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ Ligature<Types> &_) { _.depend (c, ਪਹਿਲਾ); })
    ;
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    + hb_iter (ligature)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([c] (ਅਚਲ Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_seconds (ਸੈੱਟ_t &s) ਅਚਲ
  {
    + hb_iter (ligature)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&s] (ਅਚਲ Ligature<Types> &_) { _.collect_ਦੂਜਾ (s); })
    ;
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  {
    ਵਾਪਸ
    + hb_iter (ligature)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_map ([c] (ਅਚਲ Ligature<Types> &_) { ਵਾਪਸ _.would_apply (c); })
    | hb_any
    ;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c, ਅਚਲ hb_ਸੈੱਟ_digest_t *seconds = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ligs = ligature.len;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_RULESETS_FAST_ਰਸਤਾ
    ਜੇ (HB_OPTIMIZE_ਮਾਪ_VAL || ਅੰਕ_ligs <= 1)
#ਜੇ_ਅੰਤ
    {
    slow:
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_ligs; i++)
      {
	ਅਚਲ ਸਵੈਚਾਲਿਤ &lig = ਇਹ+ligature.arrayZ[i];
	ਜੇ (lig.apply (c)) return_trace (ਸੱਚ);
      }
      return_trace (ਝੂਠ);
    }

    /* This version is optimized for speed by matching the second component
     * of the ligature here, instead of calling into the ligation code.
     *
     * This is replicated in ChainRuleSet and RuleSet. */

    ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_context;
    skippy_iter.reset (c->ਬਫਰ->idx);
    skippy_iter.ਸੈੱਟ_ਮੇਲ_func (ਮੇਲ_always, ਖਾਲੀ_ਸੂਚਕ);
    skippy_iter.ਸੈੱਟ_glyph_ਅੰਕੜੇ ((HBUINT16 *) ਖਾਲੀ_ਸੂਚਕ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_ਮੰਜ਼ਿਲ;
    hb_codepoint_t second = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;
    ਤਾਰਕਿਕ matched = skippy_iter.next (&unsafe_ਮੰਜ਼ਿਲ);
    ਜੇ (likely (matched))
    {
      second = c->ਬਫਰ->info[skippy_iter.idx].codepoint;
      unsafe_ਮੰਜ਼ਿਲ = skippy_iter.idx + 1;

      ਜੇ (skippy_iter.may_skip (c->ਬਫਰ->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ਜਾਓ slow;
      }
    }
    ਨਹੀਂਤਾਂ
      ਜਾਓ slow;

    ਜੇ (seconds && !seconds->may_have (second))
    {
      c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_ਮੰਜ਼ਿਲ);
      return_trace (ਝੂਠ);
    }
    ਤਾਰਕਿਕ unsafe_ਮੰਜ਼ਿਲ_concat = ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_ligs; i++)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &lig = ਇਹ+ligature.arrayZ[i];
      ਜੇ (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	ਜੇ (lig.apply (c))
	{
	  ਜੇ (unsafe_ਮੰਜ਼ਿਲ_concat)
	    c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_ਮੰਜ਼ਿਲ);
	  return_trace (ਸੱਚ);
	}
      }
      ਨਹੀਂਤਾਂ ਜੇ (likely (lig.component.lenP1 > 1))
        unsafe_ਮੰਜ਼ਿਲ_concat = ਸੱਚ;
    }
    ਜੇ (likely (unsafe_ਮੰਜ਼ਿਲ_concat))
      c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, unsafe_ਮੰਜ਼ਿਲ);

    return_trace (ਝੂਠ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ GlyphID>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> component_ਗਿਣਤੀ_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!ligature.serialize (c, ligatures.ਲੰਮਾਈ))) return_trace (ਝੂਠ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ligatures.ਲੰਮਾਈ; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ component_ਗਿਣਤੀ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_max ((ਪੂਰਨ) component_ਗਿਣਤੀ_list[i] - 1, 0);
      ਜੇ (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_ਗਿਣਤੀ))))
        return_trace (ਝੂਠ);
      component_list += component_ਗਿਣਤੀ;
    }
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_idx) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    + hb_iter (ligature)
    | hb_filter (subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->ligature, ਇਹ, coverage_idx))
    | hb_drain
    ;

    ਜੇ (ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->ਜੋੜੋ_virtual_link (coverage_idx);

    return_trace (ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->ligature));
  }
};

}
}
}

#ਜੇ_ਅੰਤ  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
