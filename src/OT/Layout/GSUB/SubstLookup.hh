#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#ضمن "Common.hh"
#ضمن "SubstLookupSubTable.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { החזר Lookup::sanitize<SubTable> (c); }

  ثابت SubTable& get_subtable (unsigned صحيح i) ثابت
  { החזר Lookup::get_subtable<SubTable> (i); }

  ساكن مضمن منطقي lookup_type_is_reverse (unsigned صحيح lookup_type)
  { החזר lookup_type == SubTable::ReverseChainSingle; }

  منطقي is_reverse () ثابت
  {
    unsigned صحيح type = get_type ();
    אם (unlikely (type == SubTable::Extension) && hb_barrier ())
      החזר get_subtable (0).u.extension.is_reverse ();
    החזר lookup_type_is_reverse (type);
  }

  منطقي may_have_non_1to1 () ثابت
  {
    hb_have_non_1to1_context_t c;
    החזר dispatch (&c);
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    return_trace (dispatch (c));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    hb_intersects_context_t c (glyphs);
    החזר dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned صحيح this_index) ثابت
  {
    אם (!c->should_visit_lookup (this_index))
      החזר hb_closure_context_t::default_return_value ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    החזר ret;
  }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) ثابت
  {
    אם (c->is_lookup_visited (this_index))
      החזר hb_closure_lookups_context_t::default_return_value ();

    c->set_lookup_visited (this_index);
    אם (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      החזר hb_closure_lookups_context_t::default_return_value ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    החזר ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    החזר dispatch (c);
  }

  template <typename set_t>
  فراغ collect_coverage (set_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
                    ثابت hb_ot_layout_lookup_accelerator_t *accel) ثابت
  {
    אם (unlikely (!c->len)) החזר שקר;
    אם (!accel->may_have (c->glyphs[0])) החזר שקר;
      החזר dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						ثابت hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 ثابت hb_codepoint_t))>
  منطقي serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (هذا);
    אם (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (שקר);
    אם (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->pop_pack ());
      return_trace (אמת);
    }
    c->pop_discard ();
    return_trace (שקר);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    אם (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (שקר);
    אם (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->pop_pack ());
      return_trace (אמת);
    }
    c->pop_discard ();
    return_trace (שקר);
  }

  template <typename GlyphID>
  منطقي serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ثابت unsigned صحيح> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (هذا);
    אם (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (שקר);

    אם (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->pop_pack ());
      return_trace (אמת);
    }
    c->pop_discard ();
    return_trace (שקר);
  }

  template <typename GlyphID>
  منطقي serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<ثابت unsigned صحيح> ligature_per_first_glyph_count_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ثابت unsigned صحيح> component_count_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    אם (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (שקר);
    אם (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_count_list,
                   ligatures_list,
                   component_count_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->pop_pack ());
      return_trace (אמת);
    }
    c->pop_discard ();
    return_trace (שקר);
  }

  template <typename context_t>
  ساكن مضمن typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned صحيح lookup_index);

  ساكن مضمن typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index);


  ساكن مضمن hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    אם (!c->should_visit_lookup (lookup_index))
      החזר hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    החזר ret;
  }

  ساكن مضمن hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);

  ساكن مضمن hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    החזר depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { החזר Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  منطقي subset (hb_subset_context_t *c) ثابت
  { החזר Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
