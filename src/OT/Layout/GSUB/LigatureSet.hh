#ifndef OT_LAYOUT_GSUB_LIGATURESET_HH
#define OT_LAYOUT_GSUB_LIGATURESET_HH

#অন্তর্ভাব "Common.hh"
#অন্তর্ভাব "Ligature.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
কাঠামো LigatureSet
{
  সর্বজনীন:
  Array16Of<typename Types::template OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_আকার_ARRAY (2, ligature);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (ligature.sanitize (c, এই));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    ফেরত
    + hb_iter (ligature)
    | hb_map (hb_add (এই))
    | hb_map ([glyphs] (ধ্রুবক Ligature<Types> &_) { ফেরত _.intersects (glyphs); })
    | hb_any
    ;
  }

  যৌক্তিক intersects_lig_glyph (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    ফেরত
    + hb_iter (ligature)
    | hb_map (hb_add (এই))
    | hb_map ([glyphs] (ধ্রুবক Ligature<Types> &_) { 
      ফেরত _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    + hb_iter (ligature)
    | hb_map (hb_add (এই))
    | hb_apply ([c] (ধ্রুবক Ligature<Types> &_) { _.closure (c); })
    ;
  }

  শূন্য depend (hb_depend_context_t *c, hb_codepoint_t first) ধ্রুবক
  {
    + hb_iter (ligature)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    + hb_iter (ligature)
    | hb_map (hb_add (এই))
    | hb_apply ([c] (ধ্রুবক Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  template <typename set_t>
  শূন্য collect_seconds (set_t &s) ধ্রুবক
  {
    + hb_iter (ligature)
    | hb_map (hb_add (এই))
    | hb_apply ([&s] (ধ্রুবক Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  {
    ফেরত
    + hb_iter (ligature)
    | hb_map (hb_add (এই))
    | hb_map ([c] (ধ্রুবক Ligature<Types> &_) { ফেরত _.would_apply (c); })
    | hb_any
    ;
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c, ধ্রুবক hb_set_digest_t *seconds = nullptr) ধ্রুবক
  {
    TRACE_APPLY (এই);

    unsigned পূর্ণসংখ্যা অঙ্ক_ligs = ligature.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    যদি (HB_OPTIMIZE_আকার_VAL || অঙ্ক_ligs <= 1)
#endif
    {
    slow:
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_ligs; i++)
      {
	ধ্রুবক auto &lig = এই+ligature.arrayZ[i];
	যদি (lig.apply (c)) return_trace (সত্য);
      }
      return_trace (মিথ্যা);
    }

    /* This version is optimized for speed by matching the second component
     * of the ligature here, instead of calling into the ligation code.
     *
     * This is replicated in ChainRuleSet and RuleSet. */

    auto &skippy_iter = c->iter_context;
    skippy_iter.reset (c->বাফার->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_তথ্য ((HBUINT16 *) nullptr);
    unsigned unsafe_to;
    hb_codepoint_t second = (unsigned) -1;
    যৌক্তিক matched = skippy_iter.next (&unsafe_to);
    যদি (likely (matched))
    {
      second = c->বাফার->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      যদি (skippy_iter.may_skip (c->বাফার->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        যাও slow;
      }
    }
    অন্যথা
      যাও slow;

    যদি (seconds && !seconds->may_have (second))
    {
      c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to);
      return_trace (মিথ্যা);
    }
    যৌক্তিক unsafe_to_concat = মিথ্যা;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_ligs; i++)
    {
      ধ্রুবক auto &lig = এই+ligature.arrayZ[i];
      যদি (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	যদি (lig.apply (c))
	{
	  যদি (unsafe_to_concat)
	    c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to);
	  return_trace (সত্য);
	}
      }
      অন্যথা যদি (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = সত্য;
    }
    যদি (likely (unsafe_to_concat))
      c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to);

    return_trace (মিথ্যা);
  }

  template <typename GlyphID>
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ধ্রুবক unsigned পূর্ণসংখ্যা> component_গুনতি_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    যদি (unlikely (!ligature.serialize (c, ligatures.দৈর্ঘ্য))) return_trace (মিথ্যা);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ligatures.দৈর্ঘ্য; i++)
    {
      unsigned পূর্ণসংখ্যা component_গুনতি = (unsigned) hb_max ((পূর্ণসংখ্যা) component_গুনতি_list[i] - 1, 0);
      যদি (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_গুনতি))))
        return_trace (মিথ্যা);
      component_list += component_গুনতি;
    }
    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c, unsigned coverage_idx) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, এই, coverage_idx))
    | hb_drain
    ;

    যদি (যৌক্তিক (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (যৌক্তিক (out->ligature));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
