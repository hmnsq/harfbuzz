#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#অন্তর্ভাব "PosLookupSubTable.hh"
#অন্তর্ভাব "../../../hb-ot-layout-common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

কাঠামো PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  ধ্রুবক SubTable& get_subtable (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত Lookup::get_subtable<SubTable> (i); }

  যৌক্তিক is_reverse () ধ্রুবক
  {
    ফেরত মিথ্যা;
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

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  { ফেরত dispatch (c); }

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

  template <typename set_t>
  শূন্য collect_coverage (set_t *glyphs) ধ্রুবক
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  স্থির typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned পূর্ণসংখ্যা lookup_সূচক);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  { ফেরত Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  { ফেরত Lookup::subset<SubTable> (c); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  { ফেরত Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
