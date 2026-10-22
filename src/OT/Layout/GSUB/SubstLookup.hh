#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#অন্তর্ভাৱ "Common.hh"
#অন্তর্ভাৱ "SubstLookupSubTable.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

সংরচনা SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  { ঘূৰাই Lookup::sanitize<SubTable> (c); }

  ধ্রুৱ SubTable& get_subtable (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই Lookup::get_subtable<SubTable> (i); }

  স্থির অন্তঃস্থ তার্কিক lookup_type_is_reverse (unsigned পূর্ণাঙ্ক lookup_type)
  { ঘূৰাই lookup_type == SubTable::ReverseChainSingle; }

  তার্কিক is_reverse () ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক type = get_type ();
    যদি (unlikely (type == SubTable::Extension) && hb_barrier ())
      ঘূৰাই get_subtable (0).u.extension.is_reverse ();
    ঘূৰাই lookup_type_is_reverse (type);
  }

  তার্কিক may_have_non_1to1 () ধ্রুৱ
  {
    hb_have_non_1to1_context_t c;
    ঘূৰাই dispatch (&c);
  }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    return_trace (dispatch (c));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    hb_intersects_context_t c (glyphs);
    ঘূৰাই dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned পূর্ণাঙ্ক this_index) ধ্রুৱ
  {
    যদি (!c->should_visit_lookup (this_index))
      ঘূৰাই hb_closure_context_t::default_return_মান ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    ঘূৰাই ret;
  }

  শূন্য depend (hb_depend_context_t *c) ধ্রুৱ
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) ধ্রুৱ
  {
    যদি (c->is_lookup_visited (this_index))
      ঘূৰাই hb_closure_lookups_context_t::default_return_মান ();

    c->set_lookup_visited (this_index);
    যদি (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      ঘূৰাই hb_closure_lookups_context_t::default_return_মান ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    ঘূৰাই ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    ঘূৰাই dispatch (c);
  }

  template <typename set_t>
  শূন্য collect_coverage (set_t *glyphs) ধ্রুৱ
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  তার্কিক would_apply (hb_would_apply_context_t *c,
                    ধ্রুৱ hb_ot_layout_lookup_accelerator_t *accel) ধ্রুৱ
  {
    যদি (unlikely (!c->len)) ঘূৰাই মিছা;
    যদি (!accel->may_have (c->glyphs[0])) ঘূৰাই মিছা;
      ঘূৰাই dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						ধ্রুৱ hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 ধ্রুৱ hb_codepoint_t))>
  তার্কিক serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (মিছা);
    যদি (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->বেৰ_কৰো_pack ());
      return_trace (সঁচা);
    }
    c->বেৰ_কৰো_discard ();
    return_trace (মিছা);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (মিছা);
    যদি (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->বেৰ_কৰো_pack ());
      return_trace (সঁচা);
    }
    c->বেৰ_কৰো_discard ();
    return_trace (মিছা);
  }

  template <typename GlyphID>
  তার্কিক serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ধ্রুৱ unsigned পূর্ণাঙ্ক> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (মিছা);

    যদি (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->বেৰ_কৰো_pack ());
      return_trace (সঁচা);
    }
    c->বেৰ_কৰো_discard ();
    return_trace (মিছা);
  }

  template <typename GlyphID>
  তার্কিক serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<ধ্রুৱ unsigned পূর্ণাঙ্ক> ligature_per_first_glyph_গুনতি_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ধ্রুৱ unsigned পূর্ণাঙ্ক> component_গুনতি_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (মিছা);
    যদি (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_গুনতি_list,
                   ligatures_list,
                   component_গুনতি_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->বেৰ_কৰো_pack ());
      return_trace (সঁচা);
    }
    c->বেৰ_কৰো_discard ();
    return_trace (মিছা);
  }

  template <typename context_t>
  স্থির অন্তঃস্থ typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned পূর্ণাঙ্ক lookup_index);

  স্থির অন্তঃস্থ typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index);


  স্থির অন্তঃস্থ hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    যদি (!c->should_visit_lookup (lookup_index))
      ঘূৰাই hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    ঘূৰাই ret;
  }

  স্থির অন্তঃস্থ hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);

  স্থির অন্তঃস্থ hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    ঘূৰাই depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  { ঘূৰাই Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  { ঘূৰাই Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
