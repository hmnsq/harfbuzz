#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#شامل "Common.hh"
#شامل "SubstLookupSubTable.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

ساختار SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  { بازگشت Lookup::sanitize<SubTable> (c); }

  ثابت SubTable& get_subtable (unsigned صحیح i) ثابت
  { بازگشت Lookup::get_subtable<SubTable> (i); }

  ایستا درونخطی منطقی lookup_type_is_reverse (unsigned صحیح lookup_type)
  { بازگشت lookup_type == SubTable::ReverseChainSingle; }

  منطقی is_reverse () ثابت
  {
    unsigned صحیح type = get_type ();
    اگر (unlikely (type == SubTable::Extension) && hb_barrier ())
      بازگشت get_subtable (0).u.extension.is_reverse ();
    بازگشت lookup_type_is_reverse (type);
  }

  منطقی may_have_non_1to1 () ثابت
  {
    hb_have_non_1to1_context_t c;
    بازگشت dispatch (&c);
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    return_trace (dispatch (c));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    hb_intersects_context_t c (glyphs);
    بازگشت dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned صحیح this_index) ثابت
  {
    اگر (!c->should_visit_lookup (this_index))
      بازگشت hb_closure_context_t::default_return_مقدار ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    بازگشت ret;
  }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) ثابت
  {
    اگر (c->is_lookup_visited (this_index))
      بازگشت hb_closure_lookups_context_t::default_return_مقدار ();

    c->set_lookup_visited (this_index);
    اگر (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      بازگشت hb_closure_lookups_context_t::default_return_مقدار ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    بازگشت ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    بازگشت dispatch (c);
  }

  template <typename set_t>
  تهی collect_coverage (set_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  منطقی would_apply (hb_would_apply_context_t *c,
                    ثابت hb_ot_layout_lookup_accelerator_t *accel) ثابت
  {
    اگر (unlikely (!c->len)) بازگشت نادرست;
    اگر (!accel->may_have (c->glyphs[0])) بازگشت نادرست;
      بازگشت dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						ثابت hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 ثابت hb_codepoint_t))>
  منطقی serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (نادرست);
    اگر (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->برداشت_pack ());
      return_trace (درست);
    }
    c->برداشت_discard ();
    return_trace (نادرست);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (نادرست);
    اگر (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->برداشت_pack ());
      return_trace (درست);
    }
    c->برداشت_discard ();
    return_trace (نادرست);
  }

  template <typename GlyphID>
  منطقی serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ثابت unsigned صحیح> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (نادرست);

    اگر (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->برداشت_pack ());
      return_trace (درست);
    }
    c->برداشت_discard ();
    return_trace (نادرست);
  }

  template <typename GlyphID>
  منطقی serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<ثابت unsigned صحیح> ligature_per_first_glyph_شمار_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ثابت unsigned صحیح> component_شمار_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (نادرست);
    اگر (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_شمار_list,
                   ligatures_list,
                   component_شمار_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->برداشت_pack ());
      return_trace (درست);
    }
    c->برداشت_discard ();
    return_trace (نادرست);
  }

  template <typename context_t>
  ایستا درونخطی typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned صحیح lookup_index);

  ایستا درونخطی typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index);


  ایستا درونخطی hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    اگر (!c->should_visit_lookup (lookup_index))
      بازگشت hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    بازگشت ret;
  }

  ایستا درونخطی hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);

  ایستا درونخطی hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    بازگشت depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { بازگشت Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  منطقی subset (hb_subset_context_t *c) ثابت
  { بازگشت Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
