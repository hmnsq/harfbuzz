#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#અન્તર્ભાવ "Common.hh"
#અન્તર્ભાવ "SubstLookupSubTable.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

સંરચના SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  { પાછા Lookup::sanitize<SubTable> (c); }

  અચલ SubTable& get_subtable (unsigned પૂર્ણાંક i) અચલ
  { પાછા Lookup::get_subtable<SubTable> (i); }

  સ્થિર અંતઃરેખ તાર્કિક lookup_type_is_reverse (unsigned પૂર્ણાંક lookup_type)
  { પાછા lookup_type == SubTable::ReverseChainSingle; }

  તાર્કિક is_reverse () અચલ
  {
    unsigned પૂર્ણાંક type = get_type ();
    જો (unlikely (type == SubTable::Extension) && hb_barrier ())
      પાછા get_subtable (0).u.extension.is_reverse ();
    પાછા lookup_type_is_reverse (type);
  }

  તાર્કિક may_have_non_1to1 () અચલ
  {
    hb_have_non_1to1_context_t c;
    પાછા dispatch (&c);
  }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    return_trace (dispatch (c));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    hb_intersects_context_t c (glyphs);
    પાછા dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned પૂર્ણાંક this_index) અચલ
  {
    જો (!c->should_visit_lookup (this_index))
      પાછા hb_closure_context_t::default_return_કિંમત ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    પાછા ret;
  }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) અચલ
  {
    જો (c->is_lookup_visited (this_index))
      પાછા hb_closure_lookups_context_t::default_return_કિંમત ();

    c->set_lookup_visited (this_index);
    જો (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      પાછા hb_closure_lookups_context_t::default_return_કિંમત ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    પાછા ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    પાછા dispatch (c);
  }

  template <typename set_t>
  શૂન્ય collect_coverage (set_t *glyphs) અચલ
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c,
                    અચલ hb_ot_layout_lookup_accelerator_t *accel) અચલ
  {
    જો (unlikely (!c->len)) પાછા ખોટું;
    જો (!accel->may_have (c->glyphs[0])) પાછા ખોટું;
      પાછા dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						અચલ hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 અચલ hb_codepoint_t))>
  તાર્કિક serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (ખોટું);
    જો (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->કાઢો_pack ());
      return_trace (સાચું);
    }
    c->કાઢો_discard ();
    return_trace (ખોટું);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (ખોટું);
    જો (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->કાઢો_pack ());
      return_trace (સાચું);
    }
    c->કાઢો_discard ();
    return_trace (ખોટું);
  }

  template <typename GlyphID>
  તાર્કિક serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<અચલ unsigned પૂર્ણાંક> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (ખોટું);

    જો (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->કાઢો_pack ());
      return_trace (સાચું);
    }
    c->કાઢો_discard ();
    return_trace (ખોટું);
  }

  template <typename GlyphID>
  તાર્કિક serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<અચલ unsigned પૂર્ણાંક> ligature_per_first_glyph_ગણતરી_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<અચલ unsigned પૂર્ણાંક> component_ગણતરી_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (ખોટું);
    જો (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_ગણતરી_list,
                   ligatures_list,
                   component_ગણતરી_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->કાઢો_pack ());
      return_trace (સાચું);
    }
    c->કાઢો_discard ();
    return_trace (ખોટું);
  }

  template <typename context_t>
  સ્થિર અંતઃરેખ typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned પૂર્ણાંક lookup_index);

  સ્થિર અંતઃરેખ typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index);


  સ્થિર અંતઃરેખ hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    જો (!c->should_visit_lookup (lookup_index))
      પાછા hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    પાછા ret;
  }

  સ્થિર અંતઃરેખ hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);

  સ્થિર અંતઃરેખ hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    પાછા depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  { પાછા Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  { પાછા Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
