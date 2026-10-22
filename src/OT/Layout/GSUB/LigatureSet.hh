#ifndef OT_LAYOUT_GSUB_LIGATURESET_HH
#define OT_LAYOUT_GSUB_LIGATURESET_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "Ligature.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

template <typename Types>
ସଂରଚନା LigatureSet
{
  ସର୍ବସାଧାରଣ:
  Array16Of<typename Types::template OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_SIZE_ARRAY (2, ligature);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (ligature.sanitize (c, ଏହି));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + hb_iter (ligature)
    | hb_map (hb_add (ଏହି))
    | hb_map ([glyphs] (ଅଚଳ Ligature<Types> &_) { ଫେରନ୍ତୁ _.intersects (glyphs); })
    | hb_any
    ;
  }

  ତାର୍କିକ intersects_lig_glyph (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + hb_iter (ligature)
    | hb_map (hb_add (ଏହି))
    | hb_map ([glyphs] (ଅଚଳ Ligature<Types> &_) { 
      ଫେରନ୍ତୁ _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([c] (ଅଚଳ Ligature<Types> &_) { _.closure (c); })
    ;
  }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c, hb_codepoint_t first) ଅଚଳ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([c] (ଅଚଳ Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_seconds (set_t &s) ଅଚଳ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&s] (ଅଚଳ Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + hb_iter (ligature)
    | hb_map (hb_add (ଏହି))
    | hb_map ([c] (ଅଚଳ Ligature<Types> &_) { ଫେରନ୍ତୁ _.would_apply (c); })
    | hb_any
    ;
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c, ଅଚଳ hb_set_digest_t *seconds = nullptr) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_ligs = ligature.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    ଯଦି (HB_OPTIMIZE_SIZE_VAL || ଅଙ୍କ_ligs <= 1)
#endif
    {
    slow:
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_ligs; i++)
      {
	ଅଚଳ auto &lig = ଏହି+ligature.arrayZ[i];
	ଯଦି (lig.apply (c)) return_trace (ସତ୍ୟ);
      }
      return_trace (ମିଥ୍ୟା);
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
    ତାର୍କିକ matched = skippy_iter.next (&unsafe_to);
    ଯଦି (likely (matched))
    {
      second = c->buffer->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      ଯଦି (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        ଯାଅ slow;
      }
    }
    ଅନ୍ୟଥା
      ଯାଅ slow;

    ଯଦି (seconds && !seconds->may_have (second))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
      return_trace (ମିଥ୍ୟା);
    }
    ତାର୍କିକ unsafe_to_concat = ମିଥ୍ୟା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_ligs; i++)
    {
      ଅଚଳ auto &lig = ଏହି+ligature.arrayZ[i];
      ଯଦି (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	ଯଦି (lig.apply (c))
	{
	  ଯଦି (unsafe_to_concat)
	    c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	  return_trace (ସତ୍ୟ);
	}
      }
      ଅନ୍ୟଥା ଯଦି (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = ସତ୍ୟ;
    }
    ଯଦି (likely (unsafe_to_concat))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (ମିଥ୍ୟା);
  }

  template <typename GlyphID>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> component_ଗଣନା_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!ligature.serialize (c, ligatures.length))) return_trace (ମିଥ୍ୟା);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ligatures.length; i++)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା component_ଗଣନା = (unsigned) hb_max ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) component_ଗଣନା_list[i] - 1, 0);
      ଯଦି (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_ଗଣନା))))
        return_trace (ମିଥ୍ୟା);
      component_list += component_ଗଣନା;
    }
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c, unsigned coverage_idx) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, ଏହି, coverage_idx))
    | hb_drain
    ;

    ଯଦି (ତାର୍କିକ (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (ତାର୍କିକ (out->ligature));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
