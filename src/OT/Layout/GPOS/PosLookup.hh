#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#ଅନ୍ତର୍ଭାଵ "PosLookupSubTable.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-layout-common.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

ସଂରଚନା PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  ଅଚଳ SubTable& get_subtable (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ Lookup::get_subtable<SubTable> (i); }

  ତାର୍କିକ is_reverse () ଅଚଳ
  {
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    return_trace (dispatch (c));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    hb_intersects_context_t c (glyphs);
    ଫେରନ୍ତୁ dispatch (&c);
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ dispatch (c); }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) ଅଚଳ
  {
    ଯଦି (c->is_lookup_visited (this_index))
      ଫେରନ୍ତୁ hb_closure_lookups_context_t::default_return_ମୂଲ୍ୟ ();

    c->set_lookup_visited (this_index);
    ଯଦି (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      ଫେରନ୍ତୁ hb_closure_lookups_context_t::default_return_ମୂଲ୍ୟ ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    ଫେରନ୍ତୁ ret;
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_coverage (set_t *glyphs) ଅଚଳ
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  ସ୍ଥିର typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  { ଫେରନ୍ତୁ Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ Lookup::subset<SubTable> (c); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
