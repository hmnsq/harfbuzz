#ifndef OT_LAYOUT_GSUB_LIGATURESET_HH
#define OT_LAYOUT_GSUB_LIGATURESET_HH

#അന്തര്ഭാവ "Common.hh"
#അന്തര്ഭാവ "Ligature.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

template <typename Types>
ഘടന LigatureSet
{
  പൊതു:
  Array16Of<typename Types::template OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_SIZE_ARRAY (2, ligature);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (ligature.sanitize (c, ഇത്));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    തിരികെ
    + hb_iter (ligature)
    | hb_map (hb_add (ഇത്))
    | hb_map ([glyphs] (സ്ഥിരം Ligature<Types> &_) { തിരികെ _.intersects (glyphs); })
    | hb_any
    ;
  }

  തർക്കം intersects_lig_glyph (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    തിരികെ
    + hb_iter (ligature)
    | hb_map (hb_add (ഇത്))
    | hb_map ([glyphs] (സ്ഥിരം Ligature<Types> &_) { 
      തിരികെ _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([c] (സ്ഥിരം Ligature<Types> &_) { _.closure (c); })
    ;
  }

  ശൂന്യം depend (hb_depend_context_t *c, hb_codepoint_t first) സ്ഥിരം
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([c] (സ്ഥിരം Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  template <typename set_t>
  ശൂന്യം collect_seconds (set_t &s) സ്ഥിരം
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&s] (സ്ഥിരം Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  {
    തിരികെ
    + hb_iter (ligature)
    | hb_map (hb_add (ഇത്))
    | hb_map ([c] (സ്ഥിരം Ligature<Types> &_) { തിരികെ _.would_apply (c); })
    | hb_any
    ;
  }

  തർക്കം apply (hb_ot_apply_context_t *c, സ്ഥിരം hb_set_digest_t *seconds = nullptr) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    unsigned പൂർണ്ണസംഖ്യ അക്കം_ligs = ligature.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    എങ്കിൽ (HB_OPTIMIZE_SIZE_VAL || അക്കം_ligs <= 1)
#endif
    {
    slow:
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_ligs; i++)
      {
	സ്ഥിരം auto &lig = ഇത്+ligature.arrayZ[i];
	എങ്കിൽ (lig.apply (c)) return_trace (സത്യം);
      }
      return_trace (അസത്യം);
    }

    /* This version is optimized for speed by matching the second component
     * of the ligature here, instead of calling into the ligation code.
     *
     * This is replicated in ChainRuleSet and RuleSet. */

    auto &skippy_iter = c->iter_context;
    skippy_iter.reset (c->buffer->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_data ((HBUINT16 *) nullptr);
    unsigned unsafe_to;
    hb_codepoint_t second = (unsigned) -1;
    തർക്കം matched = skippy_iter.next (&unsafe_to);
    എങ്കിൽ (likely (matched))
    {
      second = c->buffer->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      എങ്കിൽ (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        പോകുക slow;
      }
    }
    അല്ലെങ്കിൽ
      പോകുക slow;

    എങ്കിൽ (seconds && !seconds->may_have (second))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
      return_trace (അസത്യം);
    }
    തർക്കം unsafe_to_concat = അസത്യം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_ligs; i++)
    {
      സ്ഥിരം auto &lig = ഇത്+ligature.arrayZ[i];
      എങ്കിൽ (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	എങ്കിൽ (lig.apply (c))
	{
	  എങ്കിൽ (unsafe_to_concat)
	    c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	  return_trace (സത്യം);
	}
      }
      അല്ലെങ്കിൽ എങ്കിൽ (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = സത്യം;
    }
    എങ്കിൽ (likely (unsafe_to_concat))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (അസത്യം);
  }

  template <typename GlyphID>
  തർക്കം serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ> component_എണ്ണം_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!ligature.serialize (c, ligatures.length))) return_trace (അസത്യം);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ligatures.length; i++)
    {
      unsigned പൂർണ്ണസംഖ്യ component_എണ്ണം = (unsigned) hb_max ((പൂർണ്ണസംഖ്യ) component_എണ്ണം_list[i] - 1, 0);
      എങ്കിൽ (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_എണ്ണം))))
        return_trace (അസത്യം);
      component_list += component_എണ്ണം;
    }
    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c, unsigned coverage_idx) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, ഇത്, coverage_idx))
    | hb_drain
    ;

    എങ്കിൽ (തർക്കം (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (തർക്കം (out->ligature));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
