#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#അന്തര്ഭാവ "PosLookupSubTable.hh"
#അന്തര്ഭാവ "../../../hb-ot-layout-common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

ഘടന PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  സ്ഥിരം SubTable& get_subtable (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ Lookup::get_subtable<SubTable> (i); }

  തർക്കം is_reverse () സ്ഥിരം
  {
    തിരികെ അസത്യം;
  }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    return_trace (dispatch (c));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    hb_intersects_context_t c (glyphs);
    തിരികെ dispatch (&c);
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  { തിരികെ dispatch (c); }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) സ്ഥിരം
  {
    എങ്കിൽ (c->is_lookup_visited (this_index))
      തിരികെ hb_closure_lookups_context_t::default_return_മൂല്യം ();

    c->set_lookup_visited (this_index);
    എങ്കിൽ (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      തിരികെ hb_closure_lookups_context_t::default_return_മൂല്യം ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    തിരികെ ret;
  }

  template <typename set_t>
  ശൂന്യം collect_coverage (set_t *glyphs) സ്ഥിരം
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  സ്ഥിതം typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned പൂർണ്ണസംഖ്യ lookup_index);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  { തിരികെ Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  { തിരികെ Lookup::subset<SubTable> (c); }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  { തിരികെ Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
