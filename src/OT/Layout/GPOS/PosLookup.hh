#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#अन्तर्भाव "PosLookupSubTable.hh"
#अन्तर्भाव "../../../hb-ot-layout-common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  ध्रुव SubTable& get_subtable (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त Lookup::get_subtable<SubTable> (i); }

  तार्किक is_reverse () ध्रुव
  {
    पटिनिवत्त मिच्छा;
  }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    return_trace (dispatch (c));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    hb_intersects_context_t c (glyphs);
    पटिनिवत्त dispatch (&c);
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  { पटिनिवत्त dispatch (c); }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) ध्रुव
  {
    सचे (c->is_lookup_visited (this_index))
      पटिनिवत्त hb_closure_lookups_context_t::default_return_मानम् ();

    c->set_lookup_visited (this_index);
    सचे (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      पटिनिवत्त hb_closure_lookups_context_t::default_return_मानम् ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    पटिनिवत्त ret;
  }

  template <typename set_t>
  शून्य collect_coverage (set_t *glyphs) ध्रुव
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  स्थिर typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned पूर्णाङ्क lookup_index);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  { पटिनिवत्त Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  { पटिनिवत्त Lookup::subset<SubTable> (c); }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  { पटिनिवत्त Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
