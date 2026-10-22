#ifndef OT_LAYOUT_GSUB_LIGATURESET_HH
#define OT_LAYOUT_GSUB_LIGATURESET_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"
#ಅನ್ತರ್ಭಾವ "Ligature.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

template <typename Types>
ಸಂರಚನಾ LigatureSet
{
  ಸಾರ್ವಜನಿಕ:
  Array16Of<typename Types::template OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_SIZE_ARRAY (2, ligature);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (ligature.sanitize (c, ಏತತ್));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (ligature)
    | hb_map (hb_add (ಏತತ್))
    | hb_map ([glyphs] (ಧ್ರುವ Ligature<Types> &_) { ಪ್ರತ್ಯಾಗಮ _.intersects (glyphs); })
    | hb_any
    ;
  }

  ತಾರ್ಕಿಕ intersects_lig_glyph (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (ligature)
    | hb_map (hb_add (ಏತತ್))
    | hb_map ([glyphs] (ಧ್ರುವ Ligature<Types> &_) { 
      ಪ್ರತ್ಯಾಗಮ _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([c] (ಧ್ರುವ Ligature<Types> &_) { _.closure (c); })
    ;
  }

  ಶೂನ್ಯ depend (hb_depend_context_t *c, hb_codepoint_t first) ಧ್ರುವ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([c] (ಧ್ರುವ Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_seconds (set_t &s) ಧ್ರುವ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&s] (ಧ್ರುವ Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + hb_iter (ligature)
    | hb_map (hb_add (ಏತತ್))
    | hb_map ([c] (ಧ್ರುವ Ligature<Types> &_) { ಪ್ರತ್ಯಾಗಮ _.would_apply (c); })
    | hb_any
    ;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c, ಧ್ರುವ hb_set_digest_t *seconds = nullptr) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_ligs = ligature.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    ಯದಿ (HB_OPTIMIZE_SIZE_VAL || ಅಂಕೆ_ligs <= 1)
#endif
    {
    slow:
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_ligs; i++)
      {
	ಧ್ರುವ auto &lig = ಏತತ್+ligature.arrayZ[i];
	ಯದಿ (lig.apply (c)) return_trace (ಸತ್ಯ);
      }
      return_trace (ಅಸತ್ಯ);
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
    ತಾರ್ಕಿಕ matched = skippy_iter.next (&unsafe_to);
    ಯದಿ (likely (matched))
    {
      second = c->buffer->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      ಯದಿ (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ಗಚ್ಛ slow;
      }
    }
    ಅನ್ಯಥಾ
      ಗಚ್ಛ slow;

    ಯದಿ (seconds && !seconds->may_have (second))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
      return_trace (ಅಸತ್ಯ);
    }
    ತಾರ್ಕಿಕ unsafe_to_concat = ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_ligs; i++)
    {
      ಧ್ರುವ auto &lig = ಏತತ್+ligature.arrayZ[i];
      ಯದಿ (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	ಯದಿ (lig.apply (c))
	{
	  ಯದಿ (unsafe_to_concat)
	    c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	  return_trace (ಸತ್ಯ);
	}
      }
      ಅನ್ಯಥಾ ಯದಿ (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = ಸತ್ಯ;
    }
    ಯದಿ (likely (unsafe_to_concat))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (ಅಸತ್ಯ);
  }

  template <typename GlyphID>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ> component_ಎಣಿಕೆ_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!ligature.serialize (c, ligatures.length))) return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ligatures.length; i++)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ component_ಎಣಿಕೆ = (unsigned) hb_max ((ಪೂರ್ಣಾಙ್ಕ) component_ಎಣಿಕೆ_list[i] - 1, 0);
      ಯದಿ (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_ಎಣಿಕೆ))))
        return_trace (ಅಸತ್ಯ);
      component_list += component_ಎಣಿಕೆ;
    }
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c, unsigned coverage_idx) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, ಏತತ್, coverage_idx))
    | hb_drain
    ;

    ಯದಿ (ತಾರ್ಕಿಕ (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (ತಾರ್ಕಿಕ (out->ligature));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
