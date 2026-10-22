#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_LIGATURESET_HH
#تعریف_کرو OT_LAYOUT_GSUB_LIGATURESET_HH

#شامل "Common.hh"
#شامل "Ligature.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

سانچہ <قسم_نام Types>
ساخت Ligature_سیٹ
{
  عوامی:
  Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_سائز_ARRAY (2, ligature);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (ligature.sanitize (c, یہ));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    واپس
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (یہ))
    | hb_map ([glyphs] (ثابت Ligature<Types> &_) { واپس _.intersects (glyphs); })
    | hb_any
    ;
  }

  منطقی intersects_lig_glyph (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    واپس
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (یہ))
    | hb_map ([glyphs] (ثابت Ligature<Types> &_) { 
      واپس _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([c] (ثابت Ligature<Types> &_) { _.احاطہ (c); })
    ;
  }

  خالی depend (hb_depend_context_t *c, hb_codepoint_t پہلا) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت Ligature<Types> &_) { _.depend (c, پہلا); })
    ;
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([c] (ثابت Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_seconds (سیٹ_t &s) ثابت
  {
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&s] (ثابت Ligature<Types> &_) { _.collect_دوسرا (s); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    واپس
    + hb_iter (ligature)
    | hb_map (hb_جوڑو (یہ))
    | hb_map ([c] (ثابت Ligature<Types> &_) { واپس _.would_apply (c); })
    | hb_any
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c, ثابت hb_سیٹ_digest_t *seconds = خالی_اشاریہ) ثابت
  {
    TRACE_APPLY (یہ);

    غیرعلامتی صحیح عدد_ligs = ligature.len;

#اگر_غیر_تعریف_شدہ HB_NO_OT_RULESETS_FAST_راستہ
    اگر (HB_OPTIMIZE_سائز_VAL || عدد_ligs <= 1)
#اگر_ختم
    {
    slow:
      برائے (غیرعلامتی صحیح i = 0; i < عدد_ligs; i++)
      {
	ثابت خودکار &lig = یہ+ligature.arrayZ[i];
	اگر (lig.apply (c)) return_trace (سچ);
      }
      return_trace (جھوٹ);
    }

    /* This version is optimized for speed by matching the second component
     * of the ligature here, instead of calling into the ligation code.
     *
     * This is replicated in ChainRuleSet and RuleSet. */

    خودکار &skippy_iter = c->iter_context;
    skippy_iter.reset (c->بفر->idx);
    skippy_iter.سیٹ_مطابقت_func (مطابقت_always, خالی_اشاریہ);
    skippy_iter.سیٹ_glyph_ڈیٹا ((HBUINT16 *) خالی_اشاریہ);
    غیرعلامتی unsafe_مقصد;
    hb_codepoint_t second = (غیرعلامتی) -1;
    منطقی matched = skippy_iter.next (&unsafe_مقصد);
    اگر (likely (matched))
    {
      second = c->بفر->info[skippy_iter.idx].codepoint;
      unsafe_مقصد = skippy_iter.idx + 1;

      اگر (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        جاؤ slow;
      }
    }
    ورنہ
      جاؤ slow;

    اگر (seconds && !seconds->may_have (second))
    {
      c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_مقصد);
      return_trace (جھوٹ);
    }
    منطقی unsafe_مقصد_concat = جھوٹ;
    برائے (غیرعلامتی صحیح i = 0; i < عدد_ligs; i++)
    {
      ثابت خودکار &lig = یہ+ligature.arrayZ[i];
      اگر (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	اگر (lig.apply (c))
	{
	  اگر (unsafe_مقصد_concat)
	    c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_مقصد);
	  return_trace (سچ);
	}
      }
      ورنہ اگر (likely (lig.component.lenP1 > 1))
        unsafe_مقصد_concat = سچ;
    }
    اگر (likely (unsafe_مقصد_concat))
      c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_مقصد);

    return_trace (جھوٹ);
  }

  سانچہ <قسم_نام GlyphID>
  منطقی serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ثابت غیرعلامتی صحیح> component_گنتی_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    اگر (unlikely (!ligature.serialize (c, ligatures.لمبائی))) return_trace (جھوٹ);
    برائے (غیرعلامتی صحیح i = 0; i < ligatures.لمبائی; i++)
    {
      غیرعلامتی صحیح component_گنتی = (غیرعلامتی) hb_max ((صحیح) component_گنتی_list[i] - 1, 0);
      اگر (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_گنتی))))
        return_trace (جھوٹ);
      component_list += component_گنتی;
    }
    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c, غیرعلامتی coverage_idx) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    + hb_iter (ligature)
    | hb_filter (subset_انحراف_array (c, برآمد->ligature, یہ, coverage_idx))
    | hb_drain
    ;

    اگر (منطقی (برآمد->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->جوڑو_virtual_link (coverage_idx);

    return_trace (منطقی (برآمد->ligature));
  }
};

}
}
}

#اگر_ختم  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
