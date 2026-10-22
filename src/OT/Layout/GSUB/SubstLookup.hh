#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "SubstLookupSubTable.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

ସଂରଚନା SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ Lookup::sanitize<SubTable> (c); }

  ଅଚଳ SubTable& get_subtable (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ Lookup::get_subtable<SubTable> (i); }

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ lookup_type_is_reverse (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_type)
  { ଫେରନ୍ତୁ lookup_type == SubTable::ReverseChainSingle; }

  ତାର୍କିକ is_reverse () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = get_type ();
    ଯଦି (unlikely (type == SubTable::Extension) && hb_barrier ())
      ଫେରନ୍ତୁ get_subtable (0).u.extension.is_reverse ();
    ଫେରନ୍ତୁ lookup_type_is_reverse (type);
  }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  {
    hb_have_non_1to1_context_t c;
    ଫେରନ୍ତୁ dispatch (&c);
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    return_trace (dispatch (c));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    hb_intersects_context_t c (glyphs);
    ଫେରନ୍ତୁ dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_index) ଅଚଳ
  {
    ଯଦି (!c->should_visit_lookup (this_index))
      ଫେରନ୍ତୁ hb_closure_context_t::default_return_ମୂଲ୍ୟ ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    ଫେରନ୍ତୁ ret;
  }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) ଅଚଳ
  {
    ଯଦି (c->is_lookup_visited (this_index))
      ଫେରନ୍ତୁ hb_closure_lookups_context_t::default_return_ମୂଲ୍ୟ ();

    c->set_lookup_visited (this_index);
    ଯଦି (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      ଫେରନ୍ତୁ hb_closure_lookups_context_t::default_return_ମୂଲ୍ୟ ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    ଫେରନ୍ତୁ ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    ଫେରନ୍ତୁ dispatch (c);
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_coverage (set_t *glyphs) ଅଚଳ
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c,
                    ଅଚଳ hb_ot_layout_lookup_accelerator_t *accel) ଅଚଳ
  {
    ଯଦି (unlikely (!c->len)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (!accel->may_have (c->glyphs[0])) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଫେରନ୍ତୁ dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						ଅଚଳ hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 ଅଚଳ hb_codepoint_t))>
  ତାର୍କିକ serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (ମିଥ୍ୟା);
    ଯଦି (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->କାଢ଼_pack ());
      return_trace (ସତ୍ୟ);
    }
    c->କାଢ଼_discard ();
    return_trace (ମିଥ୍ୟା);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (ମିଥ୍ୟା);
    ଯଦି (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->କାଢ଼_pack ());
      return_trace (ସତ୍ୟ);
    }
    c->କାଢ଼_discard ();
    return_trace (ମିଥ୍ୟା);
  }

  template <typename GlyphID>
  ତାର୍କିକ serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (ମିଥ୍ୟା);

    ଯଦି (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->କାଢ଼_pack ());
      return_trace (ସତ୍ୟ);
    }
    c->କାଢ଼_discard ();
    return_trace (ମିଥ୍ୟା);
  }

  template <typename GlyphID>
  ତାର୍କିକ serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ligature_per_first_glyph_ଗଣନା_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> component_ଗଣନା_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (ମିଥ୍ୟା);
    ଯଦି (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_ଗଣନା_list,
                   ligatures_list,
                   component_ଗଣନା_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->କାଢ଼_pack ());
      return_trace (ସତ୍ୟ);
    }
    c->କାଢ଼_discard ();
    return_trace (ମିଥ୍ୟା);
  }

  template <typename context_t>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index);

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index);


  ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    ଯଦି (!c->should_visit_lookup (lookup_index))
      ଫେରନ୍ତୁ hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    ଫେରନ୍ତୁ ret;
  }

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    ଫେରନ୍ତୁ depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  { ଫେରନ୍ତୁ Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
