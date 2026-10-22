#ifndef OT_LAYOUT_GSUB_LIGATURESET_HH
#define OT_LAYOUT_GSUB_LIGATURESET_HH

#அந்தர்பாவ "Common.hh"
#அந்தர்பாவ "Ligature.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

template <typename Types>
கட்டமைப்பு LigatureSet
{
  பொது:
  Array16Of<typename Types::template OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_SIZE_ARRAY (2, ligature);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (ligature.sanitize (c, இது));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    திரும்பு
    + hb_iter (ligature)
    | hb_map (hb_add (இது))
    | hb_map ([glyphs] (மாறிலி Ligature<Types> &_) { திரும்பு _.intersects (glyphs); })
    | hb_any
    ;
  }

  தருக்க intersects_lig_glyph (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    திரும்பு
    + hb_iter (ligature)
    | hb_map (hb_add (இது))
    | hb_map ([glyphs] (மாறிலி Ligature<Types> &_) { 
      திரும்பு _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    + hb_iter (ligature)
    | hb_map (hb_add (இது))
    | hb_apply ([c] (மாறிலி Ligature<Types> &_) { _.closure (c); })
    ;
  }

  வெறுமை depend (hb_depend_context_t *c, hb_codepoint_t first) மாறிலி
  {
    + hb_iter (ligature)
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    + hb_iter (ligature)
    | hb_map (hb_add (இது))
    | hb_apply ([c] (மாறிலி Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  template <typename set_t>
  வெறுமை collect_seconds (set_t &s) மாறிலி
  {
    + hb_iter (ligature)
    | hb_map (hb_add (இது))
    | hb_apply ([&s] (மாறிலி Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  {
    திரும்பு
    + hb_iter (ligature)
    | hb_map (hb_add (இது))
    | hb_map ([c] (மாறிலி Ligature<Types> &_) { திரும்பு _.would_apply (c); })
    | hb_any
    ;
  }

  தருக்க apply (hb_ot_apply_context_t *c, மாறிலி hb_set_digest_t *seconds = nullptr) மாறிலி
  {
    TRACE_APPLY (இது);

    unsigned முழு இலக்கம்_ligs = ligature.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    எனில் (HB_OPTIMIZE_SIZE_VAL || இலக்கம்_ligs <= 1)
#endif
    {
    slow:
      க்கு (unsigned முழு i = 0; i < இலக்கம்_ligs; i++)
      {
	மாறிலி auto &lig = இது+ligature.arrayZ[i];
	எனில் (lig.apply (c)) return_trace (உண்மை);
      }
      return_trace (பொய்);
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
    தருக்க matched = skippy_iter.next (&unsafe_to);
    எனில் (likely (matched))
    {
      second = c->buffer->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      எனில் (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        செல் slow;
      }
    }
    இல்லையெனில்
      செல் slow;

    எனில் (seconds && !seconds->may_have (second))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
      return_trace (பொய்);
    }
    தருக்க unsafe_to_concat = பொய்;
    க்கு (unsigned முழு i = 0; i < இலக்கம்_ligs; i++)
    {
      மாறிலி auto &lig = இது+ligature.arrayZ[i];
      எனில் (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	எனில் (lig.apply (c))
	{
	  எனில் (unsafe_to_concat)
	    c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	  return_trace (உண்மை);
	}
      }
      இல்லையெனில் எனில் (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = உண்மை;
    }
    எனில் (likely (unsafe_to_concat))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (பொய்);
  }

  template <typename GlyphID>
  தருக்க serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<மாறிலி unsigned முழு> component_மொத்தம்_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    எனில் (unlikely (!ligature.serialize (c, ligatures.length))) return_trace (பொய்);
    க்கு (unsigned முழு i = 0; i < ligatures.length; i++)
    {
      unsigned முழு component_மொத்தம் = (unsigned) hb_max ((முழு) component_மொத்தம்_list[i] - 1, 0);
      எனில் (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_மொத்தம்))))
        return_trace (பொய்);
      component_list += component_மொத்தம்;
    }
    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c, unsigned coverage_idx) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, இது, coverage_idx))
    | hb_drain
    ;

    எனில் (தருக்க (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (தருக்க (out->ligature));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
