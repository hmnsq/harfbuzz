#अगर_अघोषित OT_LAYOUT_GSUB_LIGATURESET_HH
#घोषणा OT_LAYOUT_GSUB_LIGATURESET_HH

#समावेश "Common.hh"
#समावेश "Ligature.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

ढाँचा <typename Types>
काष्ठा LigatureSet
{
  खुला:
  Array16Of<typename Types::ढाँचा OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_आकार_ARRAY (2, ligature);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (ligature.sanitize (c, यह));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    वापस
    + hb_iter (ligature)
    | hb_map (hb_add (यह))
    | hb_map ([glyphs] (स्थिर Ligature<Types> &_) { वापस _.intersects (glyphs); })
    | hb_any
    ;
  }

  bool intersects_lig_glyph (स्थिर hb_set_t *glyphs) स्थिर
  {
    वापस
    + hb_iter (ligature)
    | hb_map (hb_add (यह))
    | hb_map ([glyphs] (स्थिर Ligature<Types> &_) { 
      वापस _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    + hb_iter (ligature)
    | hb_map (hb_add (यह))
    | hb_apply ([c] (स्थिर Ligature<Types> &_) { _.closure (c); })
    ;
  }

  व्योम depend (hb_depend_context_t *c, hb_codepoint_t first) स्थिर
  {
    + hb_iter (ligature)
    | hb_map (hb_add (यह))
    | hb_apply ([&] (स्थिर Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    + hb_iter (ligature)
    | hb_map (hb_add (यह))
    | hb_apply ([c] (स्थिर Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ढाँचा <typename set_t>
  व्योम collect_seconds (set_t &s) स्थिर
  {
    + hb_iter (ligature)
    | hb_map (hb_add (यह))
    | hb_apply ([&s] (स्थिर Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  {
    वापस
    + hb_iter (ligature)
    | hb_map (hb_add (यह))
    | hb_map ([c] (स्थिर Ligature<Types> &_) { वापस _.would_apply (c); })
    | hb_any
    ;
  }

  bool apply (hb_ot_apply_context_t *c, स्थिर hb_set_digest_t *seconds = nullptr) स्थिर
  {
    TRACE_APPLY (यह);

    अचिन्हित पूर्णांक अंक_ligs = ligature.len;

#अगर_अघोषित HB_NO_OT_RULESETS_FAST_PATH
    अगर (HB_OPTIMIZE_आकार_VAL || अंक_ligs <= 1)
#पूर्ण_अगर
    {
    slow:
      क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_ligs; i++)
      {
	स्थिर स्वतः &lig = यह+ligature.arrayZ[i];
	अगर (lig.apply (c)) return_trace (true);
      }
      return_trace (false);
    }

    /* This version is optimized for speed by matching the second component
     * of the ligature here, instead of calling into the ligation code.
     *
     * This is replicated in ChainRuleSet and RuleSet. */

    स्वतः &skippy_iter = c->iter_context;
    skippy_iter.reset (c->बफर->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_तथ्य ((HBUINT16 *) nullptr);
    अचिन्हित unsafe_to;
    hb_codepoint_t second = (अचिन्हित) -1;
    bool matched = skippy_iter.next (&unsafe_to);
    अगर (likely (matched))
    {
      second = c->बफर->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      अगर (skippy_iter.may_skip (c->बफर->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        जाओ slow;
      }
    }
    अन्यथा
      जाओ slow;

    अगर (seconds && !seconds->may_have (second))
    {
      c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to);
      return_trace (false);
    }
    bool unsafe_to_concat = false;
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_ligs; i++)
    {
      स्थिर स्वतः &lig = यह+ligature.arrayZ[i];
      अगर (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	अगर (lig.apply (c))
	{
	  अगर (unsafe_to_concat)
	    c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to);
	  return_trace (true);
	}
      }
      अन्यथा अगर (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = true;
    }
    अगर (likely (unsafe_to_concat))
      c->बफर->unsafe_to_concat (c->बफर->idx, unsafe_to);

    return_trace (false);
  }

  ढाँचा <typename GlyphID>
  bool serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<स्थिर अचिन्हित पूर्णांक> component_गिनती_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    अगर (unlikely (!ligature.serialize (c, ligatures.लंबाई))) return_trace (false);
    क्रम (अचिन्हित पूर्णांक i = 0; i < ligatures.लंबाई; i++)
    {
      अचिन्हित पूर्णांक component_गिनती = (अचिन्हित) hb_max ((पूर्णांक) component_गिनती_list[i] - 1, 0);
      अगर (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_गिनती))))
        return_trace (false);
      component_list += component_गिनती;
    }
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c, अचिन्हित coverage_idx) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, यह, coverage_idx))
    | hb_drain
    ;

    अगर (bool (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (bool (out->ligature));
  }
};

}
}
}

#पूर्ण_अगर  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
