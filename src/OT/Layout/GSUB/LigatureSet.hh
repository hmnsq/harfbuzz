#ifndef OT_LAYOUT_GSUB_LIGATURESET_HH
#define OT_LAYOUT_GSUB_LIGATURESET_HH

#অন্তর্ভাৱ "Common.hh"
#অন্তর্ভাৱ "Ligature.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
সংরচনা LigatureSet
{
  সার্ৱজনিক:
  Array16Of<typename Types::template OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_SIZE_ARRAY (2, ligature);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (ligature.sanitize (c, এতত্));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    ঘূৰাই
    + hb_iter (ligature)
    | hb_map (hb_add (এতত্))
    | hb_map ([glyphs] (ধ্রুৱ Ligature<Types> &_) { ঘূৰাই _.intersects (glyphs); })
    | hb_any
    ;
  }

  তার্কিক intersects_lig_glyph (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    ঘূৰাই
    + hb_iter (ligature)
    | hb_map (hb_add (এতত্))
    | hb_map ([glyphs] (ধ্রুৱ Ligature<Types> &_) { 
      ঘূৰাই _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (এতত্))
    | hb_apply ([c] (ধ্রুৱ Ligature<Types> &_) { _.closure (c); })
    ;
  }

  শূন্য depend (hb_depend_context_t *c, hb_codepoint_t first) ধ্রুৱ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (এতত্))
    | hb_apply ([c] (ধ্রুৱ Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  template <typename set_t>
  শূন্য collect_seconds (set_t &s) ধ্রুৱ
  {
    + hb_iter (ligature)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&s] (ধ্রুৱ Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  {
    ঘূৰাই
    + hb_iter (ligature)
    | hb_map (hb_add (এতত্))
    | hb_map ([c] (ধ্রুৱ Ligature<Types> &_) { ঘূৰাই _.would_apply (c); })
    | hb_any
    ;
  }

  তার্কিক apply (hb_ot_apply_context_t *c, ধ্রুৱ hb_set_digest_t *seconds = nullptr) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);

    unsigned পূর্ণাঙ্ক অঙ্ক_ligs = ligature.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    যদি (HB_OPTIMIZE_SIZE_VAL || অঙ্ক_ligs <= 1)
#endif
    {
    slow:
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_ligs; i++)
      {
	ধ্রুৱ auto &lig = এতত্+ligature.arrayZ[i];
	যদি (lig.apply (c)) return_trace (সঁচা);
      }
      return_trace (মিছা);
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
    তার্কিক matched = skippy_iter.next (&unsafe_to);
    যদি (likely (matched))
    {
      second = c->buffer->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      যদি (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        গচ্ছ slow;
      }
    }
    অন্যথা
      গচ্ছ slow;

    যদি (seconds && !seconds->may_have (second))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
      return_trace (মিছা);
    }
    তার্কিক unsafe_to_concat = মিছা;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_ligs; i++)
    {
      ধ্রুৱ auto &lig = এতত্+ligature.arrayZ[i];
      যদি (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	যদি (lig.apply (c))
	{
	  যদি (unsafe_to_concat)
	    c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	  return_trace (সঁচা);
	}
      }
      অন্যথা যদি (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = সঁচা;
    }
    যদি (likely (unsafe_to_concat))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (মিছা);
  }

  template <typename GlyphID>
  তার্কিক serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ধ্রুৱ unsigned পূর্ণাঙ্ক> component_গুনতি_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    যদি (unlikely (!ligature.serialize (c, ligatures.length))) return_trace (মিছা);
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < ligatures.length; i++)
    {
      unsigned পূর্ণাঙ্ক component_গুনতি = (unsigned) hb_max ((পূর্ণাঙ্ক) component_গুনতি_list[i] - 1, 0);
      যদি (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_গুনতি))))
        return_trace (মিছা);
      component_list += component_গুনতি;
    }
    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c, unsigned coverage_idx) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, এতত্, coverage_idx))
    | hb_drain
    ;

    যদি (তার্কিক (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (তার্কিক (out->ligature));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
