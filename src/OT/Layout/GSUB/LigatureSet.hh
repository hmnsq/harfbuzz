#ifndef OT_LAYOUT_GSUB_LIGATURESET_HH
#define OT_LAYOUT_GSUB_LIGATURESET_HH

#અન્તર્ભાવ "Common.hh"
#અન્તર્ભાવ "Ligature.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

template <typename Types>
સંરચના LigatureSet
{
  જાહેર:
  Array16Of<typename Types::template OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_SIZE_ARRAY (2, ligature);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (ligature.sanitize (c, આ));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    પાછા
    + hb_iter (ligature)
    | hb_map (hb_add (આ))
    | hb_map ([glyphs] (અચલ Ligature<Types> &_) { પાછા _.intersects (glyphs); })
    | hb_any
    ;
  }

  તાર્કિક intersects_lig_glyph (અચલ hb_set_t *glyphs) અચલ
  {
    પાછા
    + hb_iter (ligature)
    | hb_map (hb_add (આ))
    | hb_map ([glyphs] (અચલ Ligature<Types> &_) { 
      પાછા _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (આ))
    | hb_apply ([c] (અચલ Ligature<Types> &_) { _.closure (c); })
    ;
  }

  શૂન્ય depend (hb_depend_context_t *c, hb_codepoint_t first) અચલ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (આ))
    | hb_apply ([c] (અચલ Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  template <typename set_t>
  શૂન્ય collect_seconds (set_t &s) અચલ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (આ))
    | hb_apply ([&s] (અચલ Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  {
    પાછા
    + hb_iter (ligature)
    | hb_map (hb_add (આ))
    | hb_map ([c] (અચલ Ligature<Types> &_) { પાછા _.would_apply (c); })
    | hb_any
    ;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c, અચલ hb_set_digest_t *seconds = nullptr) અચલ
  {
    TRACE_APPLY (આ);

    unsigned પૂર્ણાંક અંક_ligs = ligature.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    જો (HB_OPTIMIZE_SIZE_VAL || અંક_ligs <= 1)
#endif
    {
    slow:
      માટે (unsigned પૂર્ણાંક i = 0; i < અંક_ligs; i++)
      {
	અચલ auto &lig = આ+ligature.arrayZ[i];
	જો (lig.apply (c)) return_trace (સાચું);
      }
      return_trace (ખોટું);
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
    તાર્કિક matched = skippy_iter.next (&unsafe_to);
    જો (likely (matched))
    {
      second = c->buffer->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      જો (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        જાઓ slow;
      }
    }
    નહીંતર
      જાઓ slow;

    જો (seconds && !seconds->may_have (second))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
      return_trace (ખોટું);
    }
    તાર્કિક unsafe_to_concat = ખોટું;
    માટે (unsigned પૂર્ણાંક i = 0; i < અંક_ligs; i++)
    {
      અચલ auto &lig = આ+ligature.arrayZ[i];
      જો (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	જો (lig.apply (c))
	{
	  જો (unsafe_to_concat)
	    c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	  return_trace (સાચું);
	}
      }
      નહીંતર જો (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = સાચું;
    }
    જો (likely (unsafe_to_concat))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (ખોટું);
  }

  template <typename GlyphID>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<અચલ unsigned પૂર્ણાંક> component_ગણતરી_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    જો (unlikely (!ligature.serialize (c, ligatures.length))) return_trace (ખોટું);
    માટે (unsigned પૂર્ણાંક i = 0; i < ligatures.length; i++)
    {
      unsigned પૂર્ણાંક component_ગણતરી = (unsigned) hb_max ((પૂર્ણાંક) component_ગણતરી_list[i] - 1, 0);
      જો (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_ગણતરી))))
        return_trace (ખોટું);
      component_list += component_ગણતરી;
    }
    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c, unsigned coverage_idx) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, આ, coverage_idx))
    | hb_drain
    ;

    જો (તાર્કિક (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (તાર્કિક (out->ligature));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
