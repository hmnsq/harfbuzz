#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "SubstLookupSubTable.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

संरचना SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  { पटिनिवत्त Lookup::sanitize<SubTable> (c); }

  ध्रुव SubTable& get_subtable (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त Lookup::get_subtable<SubTable> (i); }

  स्थिर अन्तःस्थ तार्किक lookup_type_is_reverse (unsigned पूर्णाङ्क lookup_type)
  { पटिनिवत्त lookup_type == SubTable::ReverseChainSingle; }

  तार्किक is_reverse () ध्रुव
  {
    unsigned पूर्णाङ्क type = get_type ();
    सचे (unlikely (type == SubTable::Extension) && hb_barrier ())
      पटिनिवत्त get_subtable (0).u.extension.is_reverse ();
    पटिनिवत्त lookup_type_is_reverse (type);
  }

  तार्किक may_have_non_1to1 () ध्रुव
  {
    hb_have_non_1to1_context_t c;
    पटिनिवत्त dispatch (&c);
  }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    return_trace (dispatch (c));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    hb_intersects_context_t c (glyphs);
    पटिनिवत्त dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned पूर्णाङ्क this_index) ध्रुव
  {
    सचे (!c->should_visit_lookup (this_index))
      पटिनिवत्त hb_closure_context_t::default_return_मानम् ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    पटिनिवत्त ret;
  }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) ध्रुव
  {
    सचे (c->is_lookup_visited (this_index))
      पटिनिवत्त hb_closure_lookups_context_t::default_return_मानम् ();

    c->set_lookup_visited (this_index);
    सचे (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      पटिनिवत्त hb_closure_lookups_context_t::default_return_मानम् ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    पटिनिवत्त ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    पटिनिवत्त dispatch (c);
  }

  template <typename set_t>
  शून्य collect_coverage (set_t *glyphs) ध्रुव
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  तार्किक would_apply (hb_would_apply_context_t *c,
                    ध्रुव hb_ot_layout_lookup_accelerator_t *accel) ध्रुव
  {
    सचे (unlikely (!c->len)) पटिनिवत्त मिच्छा;
    सचे (!accel->may_have (c->glyphs[0])) पटिनिवत्त मिच्छा;
      पटिनिवत्त dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						ध्रुव hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 ध्रुव hb_codepoint_t))>
  तार्किक serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (मिच्छा);
    सचे (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->निष्कासय_pack ());
      return_trace (सच्च);
    }
    c->निष्कासय_discard ();
    return_trace (मिच्छा);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (मिच्छा);
    सचे (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->निष्कासय_pack ());
      return_trace (सच्च);
    }
    c->निष्कासय_discard ();
    return_trace (मिच्छा);
  }

  template <typename GlyphID>
  तार्किक serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ध्रुव unsigned पूर्णाङ्क> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (मिच्छा);

    सचे (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->निष्कासय_pack ());
      return_trace (सच्च);
    }
    c->निष्कासय_discard ();
    return_trace (मिच्छा);
  }

  template <typename GlyphID>
  तार्किक serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<ध्रुव unsigned पूर्णाङ्क> ligature_per_first_glyph_गणनम्_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ध्रुव unsigned पूर्णाङ्क> component_गणनम्_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (मिच्छा);
    सचे (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_गणनम्_list,
                   ligatures_list,
                   component_गणनम्_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->निष्कासय_pack ());
      return_trace (सच्च);
    }
    c->निष्कासय_discard ();
    return_trace (मिच्छा);
  }

  template <typename context_t>
  स्थिर अन्तःस्थ typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned पूर्णाङ्क lookup_index);

  स्थिर अन्तःस्थ typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index);


  स्थिर अन्तःस्थ hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    सचे (!c->should_visit_lookup (lookup_index))
      पटिनिवत्त hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    पटिनिवत्त ret;
  }

  स्थिर अन्तःस्थ hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);

  स्थिर अन्तःस्थ hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    पटिनिवत्त depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  { पटिनिवत्त Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  { पटिनिवत्त Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
