#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#അന്തര്ഭാവ "Common.hh"
#അന്തര്ഭാവ "SubstLookupSubTable.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

ഘടന SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  { തിരികെ Lookup::sanitize<SubTable> (c); }

  സ്ഥിരം SubTable& get_subtable (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ Lookup::get_subtable<SubTable> (i); }

  സ്ഥിതം വരിയിൽ തർക്കം lookup_type_is_reverse (unsigned പൂർണ്ണസംഖ്യ lookup_type)
  { തിരികെ lookup_type == SubTable::ReverseChainSingle; }

  തർക്കം is_reverse () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ type = get_type ();
    എങ്കിൽ (unlikely (type == SubTable::Extension) && hb_barrier ())
      തിരികെ get_subtable (0).u.extension.is_reverse ();
    തിരികെ lookup_type_is_reverse (type);
  }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  {
    hb_have_non_1to1_context_t c;
    തിരികെ dispatch (&c);
  }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    return_trace (dispatch (c));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    hb_intersects_context_t c (glyphs);
    തിരികെ dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned പൂർണ്ണസംഖ്യ this_index) സ്ഥിരം
  {
    എങ്കിൽ (!c->should_visit_lookup (this_index))
      തിരികെ hb_closure_context_t::default_return_മൂല്യം ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    തിരികെ ret;
  }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) സ്ഥിരം
  {
    എങ്കിൽ (c->is_lookup_visited (this_index))
      തിരികെ hb_closure_lookups_context_t::default_return_മൂല്യം ();

    c->set_lookup_visited (this_index);
    എങ്കിൽ (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      തിരികെ hb_closure_lookups_context_t::default_return_മൂല്യം ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    തിരികെ ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    തിരികെ dispatch (c);
  }

  template <typename set_t>
  ശൂന്യം collect_coverage (set_t *glyphs) സ്ഥിരം
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  തർക്കം would_apply (hb_would_apply_context_t *c,
                    സ്ഥിരം hb_ot_layout_lookup_accelerator_t *accel) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->len)) തിരികെ അസത്യം;
    എങ്കിൽ (!accel->may_have (c->glyphs[0])) തിരികെ അസത്യം;
      തിരികെ dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						സ്ഥിരം hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 സ്ഥിരം hb_codepoint_t))>
  തർക്കം serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (അസത്യം);
    എങ്കിൽ (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->എടുക്കുക_pack ());
      return_trace (സത്യം);
    }
    c->എടുക്കുക_discard ();
    return_trace (അസത്യം);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  തർക്കം serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (അസത്യം);
    എങ്കിൽ (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->എടുക്കുക_pack ());
      return_trace (സത്യം);
    }
    c->എടുക്കുക_discard ();
    return_trace (അസത്യം);
  }

  template <typename GlyphID>
  തർക്കം serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (അസത്യം);

    എങ്കിൽ (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->എടുക്കുക_pack ());
      return_trace (സത്യം);
    }
    c->എടുക്കുക_discard ();
    return_trace (അസത്യം);
  }

  template <typename GlyphID>
  തർക്കം serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ> ligature_per_first_glyph_എണ്ണം_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ> component_എണ്ണം_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (അസത്യം);
    എങ്കിൽ (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_എണ്ണം_list,
                   ligatures_list,
                   component_എണ്ണം_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->എടുക്കുക_pack ());
      return_trace (സത്യം);
    }
    c->എടുക്കുക_discard ();
    return_trace (അസത്യം);
  }

  template <typename context_t>
  സ്ഥിതം വരിയിൽ typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned പൂർണ്ണസംഖ്യ lookup_index);

  സ്ഥിതം വരിയിൽ typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index);


  സ്ഥിതം വരിയിൽ hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    എങ്കിൽ (!c->should_visit_lookup (lookup_index))
      തിരികെ hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    തിരികെ ret;
  }

  സ്ഥിതം വരിയിൽ hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);

  സ്ഥിതം വരിയിൽ hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    തിരികെ depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  { തിരികെ Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  { തിരികെ Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
