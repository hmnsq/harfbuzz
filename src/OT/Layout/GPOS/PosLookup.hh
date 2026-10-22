#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#অন্তর্ভাৱ "PosLookupSubTable.hh"
#অন্তর্ভাৱ "../../../hb-ot-layout-common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

সংরচনা PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  ধ্রুৱ SubTable& get_subtable (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই Lookup::get_subtable<SubTable> (i); }

  তার্কিক is_reverse () ধ্রুৱ
  {
    ঘূৰাই মিছা;
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

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  { ঘূৰাই dispatch (c); }

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

  template <typename set_t>
  শূন্য collect_coverage (set_t *glyphs) ধ্রুৱ
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  স্থির typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned পূর্ণাঙ্ক lookup_index);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  { ঘূৰাই Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  { ঘূৰাই Lookup::subset<SubTable> (c); }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  { ঘূৰাই Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
