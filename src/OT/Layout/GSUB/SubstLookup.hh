#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#அந்தர்பாவ "Common.hh"
#அந்தர்பாவ "SubstLookupSubTable.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

கட்டமைப்பு SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  { திரும்பு Lookup::sanitize<SubTable> (c); }

  மாறிலி SubTable& get_subtable (unsigned முழு i) மாறிலி
  { திரும்பு Lookup::get_subtable<SubTable> (i); }

  நிலையான உள்வரி தருக்க lookup_type_is_reverse (unsigned முழு lookup_type)
  { திரும்பு lookup_type == SubTable::ReverseChainSingle; }

  தருக்க is_reverse () மாறிலி
  {
    unsigned முழு type = get_type ();
    எனில் (unlikely (type == SubTable::Extension) && hb_barrier ())
      திரும்பு get_subtable (0).u.extension.is_reverse ();
    திரும்பு lookup_type_is_reverse (type);
  }

  தருக்க may_have_non_1to1 () மாறிலி
  {
    hb_have_non_1to1_context_t c;
    திரும்பு dispatch (&c);
  }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    return_trace (dispatch (c));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    hb_intersects_context_t c (glyphs);
    திரும்பு dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned முழு this_index) மாறிலி
  {
    எனில் (!c->should_visit_lookup (this_index))
      திரும்பு hb_closure_context_t::default_return_மதிப்பு ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    திரும்பு ret;
  }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) மாறிலி
  {
    எனில் (c->is_lookup_visited (this_index))
      திரும்பு hb_closure_lookups_context_t::default_return_மதிப்பு ();

    c->set_lookup_visited (this_index);
    எனில் (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      திரும்பு hb_closure_lookups_context_t::default_return_மதிப்பு ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    திரும்பு ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    திரும்பு dispatch (c);
  }

  template <typename set_t>
  வெறுமை collect_coverage (set_t *glyphs) மாறிலி
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  தருக்க would_apply (hb_would_apply_context_t *c,
                    மாறிலி hb_ot_layout_lookup_accelerator_t *accel) மாறிலி
  {
    எனில் (unlikely (!c->len)) திரும்பு பொய்;
    எனில் (!accel->may_have (c->glyphs[0])) திரும்பு பொய்;
      திரும்பு dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						மாறிலி hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 மாறிலி hb_codepoint_t))>
  தருக்க serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (பொய்);
    எனில் (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->எடு_pack ());
      return_trace (உண்மை);
    }
    c->எடு_discard ();
    return_trace (பொய்);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  தருக்க serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (பொய்);
    எனில் (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->எடு_pack ());
      return_trace (உண்மை);
    }
    c->எடு_discard ();
    return_trace (பொய்);
  }

  template <typename GlyphID>
  தருக்க serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<மாறிலி unsigned முழு> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (பொய்);

    எனில் (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->எடு_pack ());
      return_trace (உண்மை);
    }
    c->எடு_discard ();
    return_trace (பொய்);
  }

  template <typename GlyphID>
  தருக்க serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<மாறிலி unsigned முழு> ligature_per_first_glyph_மொத்தம்_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<மாறிலி unsigned முழு> component_மொத்தம்_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (பொய்);
    எனில் (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_மொத்தம்_list,
                   ligatures_list,
                   component_மொத்தம்_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->எடு_pack ());
      return_trace (உண்மை);
    }
    c->எடு_discard ();
    return_trace (பொய்);
  }

  template <typename context_t>
  நிலையான உள்வரி typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned முழு lookup_index);

  நிலையான உள்வரி typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index);


  நிலையான உள்வரி hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    எனில் (!c->should_visit_lookup (lookup_index))
      திரும்பு hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    திரும்பு ret;
  }

  நிலையான உள்வரி hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);

  நிலையான உள்வரி hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    திரும்பு depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  { திரும்பு Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  { திரும்பு Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
