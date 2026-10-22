#ifndef OT_LAYOUT_GSUB_LIGATURESET_HH
#define OT_LAYOUT_GSUB_LIGATURESET_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "Ligature.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

ढाँचा <typename Types>
संरचना LigatureSet
{
  सार्वजनिक:
  Array16Of<typename Types::ढाँचा OffsetTo<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_SIZE_ARRAY (2, ligature);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (ligature.sanitize (c, एतत्));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    प्रत्यागम
    + hb_iter (ligature)
    | hb_map (hb_add (एतत्))
    | hb_map ([glyphs] (ध्रुव Ligature<Types> &_) { प्रत्यागम _.intersects (glyphs); })
    | hb_any
    ;
  }

  तार्किक intersects_lig_glyph (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    प्रत्यागम
    + hb_iter (ligature)
    | hb_map (hb_add (एतत्))
    | hb_map ([glyphs] (ध्रुव Ligature<Types> &_) { 
      प्रत्यागम _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    + hb_iter (ligature)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (ध्रुव Ligature<Types> &_) { _.closure (c); })
    ;
  }

  शून्य depend (hb_depend_context_t *c, hb_codepoint_t first) ध्रुव
  {
    + hb_iter (ligature)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव Ligature<Types> &_) { _.depend (c, first); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    + hb_iter (ligature)
    | hb_map (hb_add (एतत्))
    | hb_apply ([c] (ध्रुव Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ढाँचा <typename set_t>
  शून्य collect_seconds (set_t &s) ध्रुव
  {
    + hb_iter (ligature)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&s] (ध्रुव Ligature<Types> &_) { _.collect_second (s); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  {
    प्रत्यागम
    + hb_iter (ligature)
    | hb_map (hb_add (एतत्))
    | hb_map ([c] (ध्रुव Ligature<Types> &_) { प्रत्यागम _.would_apply (c); })
    | hb_any
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c, ध्रुव hb_set_digest_t *seconds = nullptr) ध्रुव
  {
    TRACE_APPLY (एतत्);

    अचिन्हित पूर्णाङ्क अंक_ligs = ligature.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    यदि (HB_OPTIMIZE_SIZE_VAL || अंक_ligs <= 1)
#endif
    {
    slow:
      प्रति (अचिन्हित पूर्णाङ्क i = 0; i < अंक_ligs; i++)
      {
	ध्रुव स्वतः &lig = एतत्+ligature.arrayZ[i];
	यदि (lig.apply (c)) return_trace (सत्य);
      }
      return_trace (असत्य);
    }

    /* This version is optimized for speed by matching the second component
     * of the ligature here, instead of calling into the ligation code.
     *
     * This is replicated in ChainRuleSet and RuleSet. */

    स्वतः &skippy_iter = c->iter_context;
    skippy_iter.reset (c->buffer->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_data ((HBUINT16 *) nullptr);
    अचिन्हित unsafe_to;
    hb_codepoint_t second = (अचिन्हित) -1;
    तार्किक matched = skippy_iter.next (&unsafe_to);
    यदि (likely (matched))
    {
      second = c->buffer->info[skippy_iter.idx].codepoint;
      unsafe_to = skippy_iter.idx + 1;

      यदि (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        गच्छ slow;
      }
    }
    अन्यथा
      गच्छ slow;

    यदि (seconds && !seconds->may_have (second))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
      return_trace (असत्य);
    }
    तार्किक unsafe_to_concat = असत्य;
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < अंक_ligs; i++)
    {
      ध्रुव स्वतः &lig = एतत्+ligature.arrayZ[i];
      यदि (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	यदि (lig.apply (c))
	{
	  यदि (unsafe_to_concat)
	    c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	  return_trace (सत्य);
	}
      }
      अन्यथा यदि (likely (lig.component.lenP1 > 1))
        unsafe_to_concat = सत्य;
    }
    यदि (likely (unsafe_to_concat))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (असत्य);
  }

  ढाँचा <typename GlyphID>
  तार्किक serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<ध्रुव अचिन्हित पूर्णाङ्क> component_गिनती_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    यदि (unlikely (!ligature.serialize (c, ligatures.length))) return_trace (असत्य);
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < ligatures.length; i++)
    {
      अचिन्हित पूर्णाङ्क component_गिनती = (अचिन्हित) hb_max ((पूर्णाङ्क) component_गिनती_list[i] - 1, 0);
      यदि (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_गिनती))))
        return_trace (असत्य);
      component_list += component_गिनती;
    }
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c, अचिन्हित coverage_idx) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    + hb_iter (ligature)
    | hb_filter (subset_offset_array (c, out->ligature, एतत्, coverage_idx))
    | hb_drain
    ;

    यदि (तार्किक (out->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->add_virtual_link (coverage_idx);

    return_trace (तार्किक (out->ligature));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
