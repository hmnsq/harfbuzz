#که_نه_تعریف_شوی OT_LAYOUT_GSUB_LIGATURESET_HH
#تعریف_کړه OT_LAYOUT_GSUB_LIGATURESET_HH

#ضمن "Common.hh"
#ضمن "Ligature.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

کينډۍ <ډول_نوم Types>
بنية Ligature_سېټ
{
  عام:
  Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_اندازه_ARRAY (2, ligature);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ligature.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    أرجع
    + hb_iter (ligature)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_map ([glyphs] (ثابت Ligature<Types> &_) { أرجع _.intersects (glyphs); })
    | hb_any
    ;
  }

  منطقي intersects_lig_glyph (ثابت hb_سېټ_t *glyphs) ثابت
  {
    أرجع
    + hb_iter (ligature)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_map ([glyphs] (ثابت Ligature<Types> &_) { 
      أرجع _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([c] (ثابت Ligature<Types> &_) { _.تړون (c); })
    ;
  }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t لومړی) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت Ligature<Types> &_) { _.depend (c, لومړی); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([c] (ثابت Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_seconds (سېټ_t &s) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&s] (ثابت Ligature<Types> &_) { _.collect_دوهم (s); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    أرجع
    + hb_iter (ligature)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_map ([c] (ثابت Ligature<Types> &_) { أرجع _.would_apply (c); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c, ثابت hb_سېټ_digest_t *seconds = تش_اشاره) ثابت
  {
    TRACE_APPLY (هذا);

    بینښه صحيح عدد_ligs = ligature.len;

#که_نه_تعریف_شوی HB_NO_OT_RULESETS_FAST_لار
    إذا (HB_OPTIMIZE_اندازه_VAL || عدد_ligs <= 1)
#د_که_پای
    {
    slow:
      لكل (بینښه صحيح i = 0; i < عدد_ligs; i++)
      {
	ثابت خپلکاری &lig = هذا+ligature.arrayZ[i];
	إذا (lig.apply (c)) return_trace (ریښتیا);
      }
      return_trace (خطأ);
    }

    /* This version is optimized for speed by matching the second component
     * of the ligature here, instead of calling into the ligation code.
     *
     * This is replicated in ChainRuleSet and RuleSet. */

    خپلکاری &skippy_iter = c->iter_context;
    skippy_iter.reset (c->بفر->idx);
    skippy_iter.سېټ_سمون_func (سمون_always, تش_اشاره);
    skippy_iter.سېټ_glyph_معلومات ((HBUINT16 *) تش_اشاره);
    بینښه unsafe_منزل;
    hb_codepoint_t second = (بینښه) -1;
    منطقي matched = skippy_iter.next (&unsafe_منزل);
    إذا (likely (matched))
    {
      second = c->بفر->info[skippy_iter.idx].codepoint;
      unsafe_منزل = skippy_iter.idx + 1;

      إذا (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
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
      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);
      return_trace (خطأ);
    }
    منطقي unsafe_منزل_concat = خطأ;
    لكل (بینښه صحيح i = 0; i < عدد_ligs; i++)
    {
      ثابت خپلکاری &lig = هذا+ligature.arrayZ[i];
      إذا (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	إذا (lig.apply (c))
	{
	  إذا (unsafe_منزل_concat)
	    c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);
	  return_trace (ریښتیا);
	}
      }
      وإلا إذا (likely (lig.component.lenP1 > 1))
        unsafe_منزل_concat = ریښتیا;
    }
    إذا (likely (unsafe_منزل_concat))
      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);

    return_trace (خطأ);
  }

  کينډۍ <ډول_نوم GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ثابت بینښه صحيح> component_شمېر_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!ligature.serialize (c, ligatures.اوږدتیا))) return_trace (خطأ);
    لكل (بینښه صحيح i = 0; i < ligatures.اوږدتیا; i++)
    {
      بینښه صحيح component_شمېر = (بینښه) hb_max ((صحيح) component_شمېر_list[i] - 1, 0);
      إذا (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_شمېر))))
        return_trace (خطأ);
      component_list += component_شمېر;
    }
    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c, بینښه coverage_idx) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    + hb_iter (ligature)
    | hb_filter (subset_دننه_واټن_array (c, بهر->ligature, هذا, coverage_idx))
    | hb_drain
    ;

    إذا (منطقي (بهر->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->جمع_کړه_virtual_link (coverage_idx);

    return_trace (منطقي (بهر->ligature));
  }
};

}
}
}

#د_که_پای  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
