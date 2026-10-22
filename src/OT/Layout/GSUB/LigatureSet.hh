#ifndef OT_LAYOUT_GSUB_LIGATURESET_HH
#define OT_LAYOUT_GSUB_LIGATURESET_HH

#شامل "Common.hh"
#شامل "Ligature.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

template <typename Types>
ساختار LigatureSet
{
  عمومی:
  Array16Of<typename Types::template OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_SIZE_ARRAY (2, ligature);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (ligature.sanitize (c, این));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    بازگشت
    + hb_iter (ligature)
    | hb_map (hb_add (این))
    | hb_map ([glyphs] (ثابت Ligature<Types> &_) { بازگشت _.intersects (glyphs); })
    | hb_any
    ;
  }

  منطقی intersects_lig_glyph (ثابت hb_set_t *glyphs) ثابت
  {
    بازگشت
    + hb_iter (ligature)
    | hb_map (hb_add (این))
    | hb_map ([glyphs] (ثابت Ligature<Types> &_) { 
      بازگشت _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_add (این))
    | hb_apply ([c] (ثابت Ligature<Types> &_) { _.closure (c); })
    ;
  }

  تهی depend (hb_depend_context_t *c, hb_codepoint_t first) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_add (این))
    | hb_apply ([c] (ثابت Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  template <typename set_t>
  تهی collect_seconds (set_t &s) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_add (این))
    | hb_apply ([&s] (ثابت Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    بازگشت
    + hb_iter (ligature)
    | hb_map (hb_add (این))
    | hb_map ([c] (ثابت Ligature<Types> &_) { بازگشت _.would_apply (c); })
    | hb_any
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c, ثابت hb_set_digest_t *seconds = nullptr) ثابت
  {
    TRACE_APPLY (این);

    unsigned صحیح رقم_ligs = ligature.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    اگر (HB_OPTIMIZE_SIZE_VAL || رقم_ligs <= 1)
#endif
    {
    slow:
      برای (unsigned صحیح i = 0; i < رقم_ligs; i++)
      {
	ثابت auto &lig = این+ligature.arrayZ[i];
	اگر (lig.apply (c)) return_trace (درست);
      }
      return_trace (نادرست);
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
    منطقی matched = skippy_iter.next (&unsafe_to);
    اگر (likely (matched))
    {
      second = c->buffer->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      اگر (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        برو slow;
      }
    }
    وگرنه
      برو slow;

    اگر (seconds && !seconds->may_have (second))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
      return_trace (نادرست);
    }
    منطقی unsafe_to_concat = نادرست;
    برای (unsigned صحیح i = 0; i < رقم_ligs; i++)
    {
      ثابت auto &lig = این+ligature.arrayZ[i];
      اگر (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	اگر (lig.apply (c))
	{
	  اگر (unsafe_to_concat)
	    c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	  return_trace (درست);
	}
      }
      وگرنه اگر (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = درست;
    }
    اگر (likely (unsafe_to_concat))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (نادرست);
  }

  template <typename GlyphID>
  منطقی serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ثابت unsigned صحیح> component_شمار_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    اگر (unlikely (!ligature.serialize (c, ligatures.length))) return_trace (نادرست);
    برای (unsigned صحیح i = 0; i < ligatures.length; i++)
    {
      unsigned صحیح component_شمار = (unsigned) hb_max ((صحیح) component_شمار_list[i] - 1, 0);
      اگر (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_شمار))))
        return_trace (نادرست);
      component_list += component_شمار;
    }
    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c, unsigned coverage_idx) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, این, coverage_idx))
    | hb_drain
    ;

    اگر (منطقی (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (منطقی (out->ligature));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
