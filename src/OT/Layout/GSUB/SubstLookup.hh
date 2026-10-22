#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"
#ಅನ್ತರ್ಭಾವ "SubstLookupSubTable.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

ಸಂರಚನಾ SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ Lookup::sanitize<SubTable> (c); }

  ಧ್ರುವ SubTable& get_subtable (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ Lookup::get_subtable<SubTable> (i); }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ lookup_type_is_reverse (unsigned ಪೂರ್ಣಾಙ್ಕ lookup_type)
  { ಪ್ರತ್ಯಾಗಮ lookup_type == SubTable::ReverseChainSingle; }

  ತಾರ್ಕಿಕ is_reverse () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ type = get_type ();
    ಯದಿ (unlikely (type == SubTable::Extension) && hb_barrier ())
      ಪ್ರತ್ಯಾಗಮ get_subtable (0).u.extension.is_reverse ();
    ಪ್ರತ್ಯಾಗಮ lookup_type_is_reverse (type);
  }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  {
    hb_have_non_1to1_context_t c;
    ಪ್ರತ್ಯಾಗಮ dispatch (&c);
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    return_trace (dispatch (c));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    hb_intersects_context_t c (glyphs);
    ಪ್ರತ್ಯಾಗಮ dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ this_index) ಧ್ರುವ
  {
    ಯದಿ (!c->should_visit_lookup (this_index))
      ಪ್ರತ್ಯಾಗಮ hb_closure_context_t::default_return_ಮೌಲ್ಯ ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) ಧ್ರುವ
  {
    ಯದಿ (c->is_lookup_visited (this_index))
      ಪ್ರತ್ಯಾಗಮ hb_closure_lookups_context_t::default_return_ಮೌಲ್ಯ ();

    c->set_lookup_visited (this_index);
    ಯದಿ (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      ಪ್ರತ್ಯಾಗಮ hb_closure_lookups_context_t::default_return_ಮೌಲ್ಯ ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    ಪ್ರತ್ಯಾಗಮ dispatch (c);
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_coverage (set_t *glyphs) ಧ್ರುವ
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c,
                    ಧ್ರುವ hb_ot_layout_lookup_accelerator_t *accel) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->len)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (!accel->may_have (c->glyphs[0])) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						ಧ್ರುವ hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 ಧ್ರುವ hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->ತೆಗೆ_pack ());
      return_trace (ಸತ್ಯ);
    }
    c->ತೆಗೆ_discard ();
    return_trace (ಅಸತ್ಯ);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->ತೆಗೆ_pack ());
      return_trace (ಸತ್ಯ);
    }
    c->ತೆಗೆ_discard ();
    return_trace (ಅಸತ್ಯ);
  }

  template <typename GlyphID>
  ತಾರ್ಕಿಕ serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->ತೆಗೆ_pack ());
      return_trace (ಸತ್ಯ);
    }
    c->ತೆಗೆ_discard ();
    return_trace (ಅಸತ್ಯ);
  }

  template <typename GlyphID>
  ತಾರ್ಕಿಕ serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ> ligature_per_first_glyph_ಎಣಿಕೆ_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ> component_ಎಣಿಕೆ_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_ಎಣಿಕೆ_list,
                   ligatures_list,
                   component_ಎಣಿಕೆ_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->ತೆಗೆ_pack ());
      return_trace (ಸತ್ಯ);
    }
    c->ತೆಗೆ_discard ();
    return_trace (ಅಸತ್ಯ);
  }

  template <typename context_t>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index);

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index);


  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    ಯದಿ (!c->should_visit_lookup (lookup_index))
      ಪ್ರತ್ಯಾಗಮ hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    ಪ್ರತ್ಯಾಗಮ depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
