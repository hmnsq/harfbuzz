#ifndef OT_LAYOUT_GSUB_LIGATURESET_HH
#define OT_LAYOUT_GSUB_LIGATURESET_HH

#ضمن "Common.hh"
#ضمن "Ligature.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

template <typename Types>
بنية LigatureSet
{
  عام:
  Array16Of<typename Types::template OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_SIZE_ARRAY (2, ligature);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ligature.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    أرجع
    + hb_iter (ligature)
    | hb_map (hb_add (هذا))
    | hb_map ([glyphs] (ثابت Ligature<Types> &_) { أرجع _.intersects (glyphs); })
    | hb_any
    ;
  }

  منطقي intersects_lig_glyph (ثابت hb_set_t *glyphs) ثابت
  {
    أرجع
    + hb_iter (ligature)
    | hb_map (hb_add (هذا))
    | hb_map ([glyphs] (ثابت Ligature<Types> &_) { 
      أرجع _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_add (هذا))
    | hb_apply ([c] (ثابت Ligature<Types> &_) { _.closure (c); })
    ;
  }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t first) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_add (هذا))
    | hb_apply ([c] (ثابت Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  template <typename set_t>
  فراغ collect_seconds (set_t &s) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_add (هذا))
    | hb_apply ([&s] (ثابت Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    أرجع
    + hb_iter (ligature)
    | hb_map (hb_add (هذا))
    | hb_map ([c] (ثابت Ligature<Types> &_) { أرجع _.would_apply (c); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c, ثابت hb_set_digest_t *seconds = nullptr) ثابت
  {
    TRACE_APPLY (هذا);

    unsigned صحيح عدد_ligs = ligature.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    إذا (HB_OPTIMIZE_SIZE_VAL || عدد_ligs <= 1)
#endif
    {
    slow:
      لكل (unsigned صحيح i = 0; i < عدد_ligs; i++)
      {
	ثابت auto &lig = هذا+ligature.arrayZ[i];
	إذا (lig.apply (c)) return_trace (true);
      }
      return_trace (خطأ);
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
    منطقي matched = skippy_iter.next (&unsafe_to);
    إذا (likely (matched))
    {
      second = c->buffer->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      إذا (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }
    }
    وإلا
      اذهب slow;

    إذا (seconds && !seconds->may_have (second))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
      return_trace (خطأ);
    }
    منطقي unsafe_to_concat = خطأ;
    لكل (unsigned صحيح i = 0; i < عدد_ligs; i++)
    {
      ثابت auto &lig = هذا+ligature.arrayZ[i];
      إذا (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	إذا (lig.apply (c))
	{
	  إذا (unsafe_to_concat)
	    c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	  return_trace (true);
	}
      }
      وإلا إذا (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = true;
    }
    إذا (likely (unsafe_to_concat))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (خطأ);
  }

  template <typename GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ثابت unsigned صحيح> component_گنتی_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!ligature.serialize (c, ligatures.length))) return_trace (خطأ);
    لكل (unsigned صحيح i = 0; i < ligatures.length; i++)
    {
      unsigned صحيح component_گنتی = (unsigned) hb_max ((صحيح) component_گنتی_list[i] - 1, 0);
      إذا (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_گنتی))))
        return_trace (خطأ);
      component_list += component_گنتی;
    }
    return_trace (true);
  }

  منطقي subset (hb_subset_context_t *c, unsigned coverage_idx) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, هذا, coverage_idx))
    | hb_drain
    ;

    إذا (منطقي (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (منطقي (out->ligature));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
