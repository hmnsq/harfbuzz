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

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  { परत Lookup::sanitize<SubTable> (c); }

  स्थिरांक SubTable& get_subtable (अचिन्हित पूर्णांक i) स्थिरांक
  { परत Lookup::get_subtable<SubTable> (i); }

  स्थिर ओळीत तार्किक lookup_type_is_reverse (अचिन्हित पूर्णांक lookup_type)
  { परत lookup_type == SubTable::ReverseChainSingle; }

  तार्किक is_reverse () स्थिरांक
  {
    अचिन्हित पूर्णांक type = get_type ();
    जर (unlikely (type == SubTable::Extension) && hb_barrier ())
      परत get_subtable (0).u.extension.is_reverse ();
    परत lookup_type_is_reverse (type);
  }

  तार्किक may_have_non_1to1 () स्थिरांक
  {
    hb_have_non_1to1_context_t c;
    परत dispatch (&c);
  }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    return_trace (dispatch (c));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    hb_intersects_context_t c (glyphs);
    परत dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, अचिन्हित पूर्णांक this_index) स्थिरांक
  {
    जर (!c->should_visit_lookup (this_index))
      परत hb_closure_context_t::default_return_मूल्य ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    परत ret;
  }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, अचिन्हित this_index) स्थिरांक
  {
    जर (c->is_lookup_visited (this_index))
      परत hb_closure_lookups_context_t::default_return_मूल्य ();

    c->set_lookup_visited (this_index);
    जर (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      परत hb_closure_lookups_context_t::default_return_मूल्य ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    परत ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    परत dispatch (c);
  }

  ढाँचा <typename set_t>
  शून्य collect_coverage (set_t *glyphs) स्थिरांक
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  तार्किक would_apply (hb_would_apply_context_t *c,
                    स्थिरांक hb_ot_layout_lookup_accelerator_t *accel) स्थिरांक
  {
    जर (unlikely (!c->len)) परत असत्य;
    जर (!accel->may_have (c->glyphs[0])) परत असत्य;
      परत dispatch (c);
  }

  ढाँचा<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						स्थिरांक hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 स्थिरांक hb_codepoint_t))>
  तार्किक serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (असत्य);
    जर (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->काढा_pack ());
      return_trace (सत्य);
    }
    c->काढा_discard ();
    return_trace (असत्य);
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (असत्य);
    जर (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->काढा_pack ());
      return_trace (सत्य);
    }
    c->काढा_discard ();
    return_trace (असत्य);
  }

  ढाँचा <typename GlyphID>
  तार्किक serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<स्थिरांक अचिन्हित पूर्णांक> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (असत्य);

    जर (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->काढा_pack ());
      return_trace (सत्य);
    }
    c->काढा_discard ();
    return_trace (असत्य);
  }

  ढाँचा <typename GlyphID>
  तार्किक serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<स्थिरांक अचिन्हित पूर्णांक> ligature_per_first_glyph_मोजणी_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<स्थिरांक अचिन्हित पूर्णांक> component_मोजणी_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (असत्य);
    जर (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_मोजणी_list,
                   ligatures_list,
                   component_मोजणी_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->काढा_pack ());
      return_trace (सत्य);
    }
    c->काढा_discard ();
    return_trace (असत्य);
  }

  ढाँचा <typename context_t>
  स्थिर ओळीत typename context_t::return_t dispatch_recurse_func (context_t *c, अचिन्हित पूर्णांक lookup_index);

  स्थिर ओळीत typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, अचिन्हित lookup_index, hb_set_t *covered_seq_indices, अचिन्हित seq_index, अचिन्हित end_index);


  स्थिर ओळीत hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, अचिन्हित lookup_index, hb_set_t *covered_seq_indices, अचिन्हित seq_index, अचिन्हित end_index)
  {
    जर (!c->should_visit_lookup (lookup_index))
      परत hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    परत ret;
  }

  स्थिर ओळीत hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, अचिन्हित lookup_index, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_index, अचिन्हित end_index);

  स्थिर ओळीत hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, अचिन्हित lookup_index, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_index, अचिन्हित end_index)
  {
    परत depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  { परत Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  { परत Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
