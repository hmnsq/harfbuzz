#جے_بنا_تعریف OT_LAYOUT_GSUB_LIGATURESET_HH
#تعریف_کرو OT_LAYOUT_GSUB_LIGATURESET_HH

#ضمن "Common.hh"
#ضمن "Ligature.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچا <قسم_ناں Types>
بنية Ligature_سیٹ
{
  عام:
  Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_ناپ_ARRAY (2, ligature);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ligature.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    أرجع
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (هذا))
    | hb_map ([glyphs] (ثابت Ligature<Types> &_) { أرجع _.intersects (glyphs); })
    | hb_any
    ;
  }

  منطقي intersects_lig_glyph (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    أرجع
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (هذا))
    | hb_map ([glyphs] (ثابت Ligature<Types> &_) { 
      أرجع _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([c] (ثابت Ligature<Types> &_) { _.گھیرا (c); })
    ;
  }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t پہلا) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت Ligature<Types> &_) { _.depend (c, پہلا); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([c] (ثابت Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_seconds (سیٹ_t &s) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&s] (ثابت Ligature<Types> &_) { _.collect_دوجا (s); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    أرجع
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (هذا))
    | hb_map ([c] (ثابت Ligature<Types> &_) { أرجع _.would_apply (c); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c, ثابت hb_سیٹ_digest_t *seconds = خالی_سوچک) ثابت
  {
    TRACE_APPLY (هذا);

    بنانشانی صحيح عدد_ligs = ligature.len;

#جے_بنا_تعریف HB_NO_OT_RULESETS_FAST_رستہ
    إذا (HB_OPTIMIZE_ناپ_VAL || عدد_ligs <= 1)
#جے_انت
    {
    slow:
      لكل (بنانشانی صحيح i = 0; i < عدد_ligs; i++)
      {
	ثابت خودکار &lig = هذا+ligature.arrayZ[i];
	إذا (lig.apply (c)) return_trace (سچ);
      }
      return_trace (خطأ);
    }

    /* This version is optimized for speed by matching the second component
     * of the ligature here, instead of calling into the ligation code.
     *
     * This is replicated in ChainRuleSet and RuleSet. */

    خودکار &skippy_iter = c->iter_context;
    skippy_iter.reset (c->بفر->idx);
    skippy_iter.سیٹ_میل_func (میل_always, خالی_سوچک);
    skippy_iter.سیٹ_glyph_انکڑے ((HBUINT16 *) خالی_سوچک);
    بنانشانی unsafe_منزل;
    hb_codepoint_t second = (بنانشانی) -1;
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
    لكل (بنانشانی صحيح i = 0; i < عدد_ligs; i++)
    {
      ثابت خودکار &lig = هذا+ligature.arrayZ[i];
      إذا (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	إذا (lig.apply (c))
	{
	  إذا (unsafe_منزل_concat)
	    c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);
	  return_trace (سچ);
	}
      }
      وإلا إذا (likely (lig.component.lenP1 > 1))
        unsafe_منزل_concat = سچ;
    }
    إذا (likely (unsafe_منزل_concat))
      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);

    return_trace (خطأ);
  }

  سانچا <قسم_ناں GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ثابت بنانشانی صحيح> component_گنتی_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!ligature.serialize (c, ligatures.لمائی))) return_trace (خطأ);
    لكل (بنانشانی صحيح i = 0; i < ligatures.لمائی; i++)
    {
      بنانشانی صحيح component_گنتی = (بنانشانی) hb_max ((صحيح) component_گنتی_list[i] - 1, 0);
      إذا (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_گنتی))))
        return_trace (خطأ);
      component_list += component_گنتی;
    }
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c, بنانشانی coverage_idx) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    + hb_iter (ligature)
    | hb_filter (subset_وتھ_array (c, باہرلا->ligature, هذا, coverage_idx))
    | hb_drain
    ;

    إذا (منطقي (باہرلا->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->جوڑو_virtual_link (coverage_idx);

    return_trace (منطقي (باہرلا->ligature));
  }
};

}
}
}

#جے_انت  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
