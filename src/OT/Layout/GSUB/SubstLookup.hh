#अगर_अघोषित OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#घोषणा OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#समावेश "Common.hh"
#समावेश "SubstLookupSubTable.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

काष्ठा SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  { वापस Lookup::sanitize<SubTable> (c); }

  स्थिर SubTable& get_subtable (अचिन्हित पूर्णांक i) स्थिर
  { वापस Lookup::get_subtable<SubTable> (i); }

  अटल अंतरभूत bool lookup_type_is_reverse (अचिन्हित पूर्णांक lookup_type)
  { वापस lookup_type == SubTable::ReverseChainSingle; }

  bool is_reverse () स्थिर
  {
    अचिन्हित पूर्णांक type = get_type ();
    अगर (unlikely (type == SubTable::Extension) && hb_barrier ())
      वापस get_subtable (0).u.extension.is_reverse ();
    वापस lookup_type_is_reverse (type);
  }

  bool may_have_non_1to1 () स्थिर
  {
    hb_have_non_1to1_context_t c;
    वापस dispatch (&c);
  }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    return_trace (dispatch (c));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    hb_intersects_context_t c (glyphs);
    वापस dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, अचिन्हित पूर्णांक this_सूचकांक) स्थिर
  {
    अगर (!c->should_visit_lookup (this_सूचकांक))
      वापस hb_closure_context_t::default_return_मूल्य ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->सफाई ();

    वापस ret;
  }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, अचिन्हित this_सूचकांक) स्थिर
  {
    अगर (c->is_lookup_visited (this_सूचकांक))
      वापस hb_closure_lookups_context_t::default_return_मूल्य ();

    c->set_lookup_visited (this_सूचकांक);
    अगर (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_सूचकांक);
      वापस hb_closure_lookups_context_t::default_return_मूल्य ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    वापस ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    वापस dispatch (c);
  }

  ढाँचा <typename set_t>
  व्योम collect_coverage (set_t *glyphs) स्थिर
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  bool would_apply (hb_would_apply_context_t *c,
                    स्थिर hb_ot_layout_lookup_accelerator_t *accel) स्थिर
  {
    अगर (unlikely (!c->len)) वापस false;
    अगर (!accel->may_have (c->glyphs[0])) वापस false;
      वापस dispatch (c);
  }

  ढाँचा<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						स्थिर hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 स्थिर hb_codepoint_t))>
  bool serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (false);
    अगर (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->निकालो_pack ());
      return_trace (true);
    }
    c->निकालो_discard ();
    return_trace (false);
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  bool serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (false);
    अगर (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->निकालो_pack ());
      return_trace (true);
    }
    c->निकालो_discard ();
    return_trace (false);
  }

  ढाँचा <typename GlyphID>
  bool serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<स्थिर अचिन्हित पूर्णांक> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (false);

    अगर (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->निकालो_pack ());
      return_trace (true);
    }
    c->निकालो_discard ();
    return_trace (false);
  }

  ढाँचा <typename GlyphID>
  bool serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<स्थिर अचिन्हित पूर्णांक> ligature_per_first_glyph_गिनती_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<स्थिर अचिन्हित पूर्णांक> component_गिनती_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (false);
    अगर (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_गिनती_list,
                   ligatures_list,
                   component_गिनती_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->निकालो_pack ());
      return_trace (true);
    }
    c->निकालो_discard ();
    return_trace (false);
  }

  ढाँचा <typename context_t>
  अटल अंतरभूत typename context_t::return_t dispatch_recurse_func (context_t *c, अचिन्हित पूर्णांक lookup_सूचकांक);

  अटल अंतरभूत typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, अचिन्हित lookup_सूचकांक, hb_set_t *covered_seq_indices, अचिन्हित seq_सूचकांक, अचिन्हित end_सूचकांक);


  अटल अंतरभूत hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, अचिन्हित lookup_सूचकांक, hb_set_t *covered_seq_indices, अचिन्हित seq_सूचकांक, अचिन्हित end_सूचकांक)
  {
    अगर (!c->should_visit_lookup (lookup_सूचकांक))
      वापस hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_सूचकांक, covered_seq_indices, seq_सूचकांक, end_सूचकांक);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    वापस ret;
  }

  अटल अंतरभूत hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, अचिन्हित lookup_सूचकांक, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_सूचकांक, अचिन्हित end_सूचकांक);

  अटल अंतरभूत hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, अचिन्हित lookup_सूचकांक, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_सूचकांक, अचिन्हित end_सूचकांक)
  {
    वापस depend_glyphs_recurse_func (c, lookup_सूचकांक, covered_seq_indices, seq_सूचकांक, end_सूचकांक);
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  { वापस Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  bool subset (hb_subset_context_t *c) स्थिर
  { वापस Lookup::subset<SubTable> (c); }
};


}
}
}

#पूर्ण_अगर  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
