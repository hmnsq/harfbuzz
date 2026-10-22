#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#অন্তর্ভাব "Common.hh"
#অন্তর্ভাব "SubstLookupSubTable.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

কাঠামো SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  { ফেরত Lookup::sanitize<SubTable> (c); }

  ধ্রুবক SubTable& get_subtable (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত Lookup::get_subtable<SubTable> (i); }

  স্থির অন্তঃরেখ যৌক্তিক lookup_type_is_reverse (unsigned পূর্ণসংখ্যা lookup_type)
  { ফেরত lookup_type == SubTable::ReverseChainSingle; }

  যৌক্তিক is_reverse () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা type = get_type ();
    যদি (unlikely (type == SubTable::Extension) && hb_barrier ())
      ফেরত get_subtable (0).u.extension.is_reverse ();
    ফেরত lookup_type_is_reverse (type);
  }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  {
    hb_have_non_1to1_context_t c;
    ফেরত dispatch (&c);
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    return_trace (dispatch (c));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    hb_intersects_context_t c (glyphs);
    ফেরত dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned পূর্ণসংখ্যা this_সূচক) ধ্রুবক
  {
    যদি (!c->should_visit_lookup (this_সূচক))
      ফেরত hb_closure_context_t::default_return_মান ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    ফেরত ret;
  }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_সূচক) ধ্রুবক
  {
    যদি (c->is_lookup_visited (this_সূচক))
      ফেরত hb_closure_lookups_context_t::default_return_মান ();

    c->set_lookup_visited (this_সূচক);
    যদি (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_সূচক);
      ফেরত hb_closure_lookups_context_t::default_return_মান ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    ফেরত ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    ফেরত dispatch (c);
  }

  template <typename set_t>
  শূন্য collect_coverage (set_t *glyphs) ধ্রুবক
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c,
                    ধ্রুবক hb_ot_layout_lookup_accelerator_t *accel) ধ্রুবক
  {
    যদি (unlikely (!c->len)) ফেরত মিথ্যা;
    যদি (!accel->may_have (c->glyphs[0])) ফেরত মিথ্যা;
      ফেরত dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						ধ্রুবক hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 ধ্রুবক hb_codepoint_t))>
  যৌক্তিক serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (মিথ্যা);
    যদি (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->বের_করো_pack ());
      return_trace (সত্য);
    }
    c->বের_করো_discard ();
    return_trace (মিথ্যা);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (মিথ্যা);
    যদি (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->বের_করো_pack ());
      return_trace (সত্য);
    }
    c->বের_করো_discard ();
    return_trace (মিথ্যা);
  }

  template <typename GlyphID>
  যৌক্তিক serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ধ্রুবক unsigned পূর্ণসংখ্যা> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (মিথ্যা);

    যদি (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->বের_করো_pack ());
      return_trace (সত্য);
    }
    c->বের_করো_discard ();
    return_trace (মিথ্যা);
  }

  template <typename GlyphID>
  যৌক্তিক serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<ধ্রুবক unsigned পূর্ণসংখ্যা> ligature_per_first_glyph_গুনতি_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ধ্রুবক unsigned পূর্ণসংখ্যা> component_গুনতি_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (মিথ্যা);
    যদি (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_গুনতি_list,
                   ligatures_list,
                   component_গুনতি_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->বের_করো_pack ());
      return_trace (সত্য);
    }
    c->বের_করো_discard ();
    return_trace (মিথ্যা);
  }

  template <typename context_t>
  স্থির অন্তঃরেখ typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned পূর্ণসংখ্যা lookup_সূচক);

  স্থির অন্তঃরেখ typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_সূচক, hb_set_t *covered_seq_indices, unsigned seq_সূচক, unsigned end_সূচক);


  স্থির অন্তঃরেখ hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_সূচক, hb_set_t *covered_seq_indices, unsigned seq_সূচক, unsigned end_সূচক)
  {
    যদি (!c->should_visit_lookup (lookup_সূচক))
      ফেরত hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_সূচক, covered_seq_indices, seq_সূচক, end_সূচক);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    ফেরত ret;
  }

  স্থির অন্তঃরেখ hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_সূচক, hb_bit_page_t *covered_seq_indices, unsigned seq_সূচক, unsigned end_সূচক);

  স্থির অন্তঃরেখ hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_সূচক, hb_bit_page_t *covered_seq_indices, unsigned seq_সূচক, unsigned end_সূচক)
  {
    ফেরত depend_glyphs_recurse_func (c, lookup_সূচক, covered_seq_indices, seq_সূচক, end_সূচক);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  { ফেরত Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  { ফেরত Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
