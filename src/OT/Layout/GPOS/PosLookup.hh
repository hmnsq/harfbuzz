#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#અન્તર્ભાવ "PosLookupSubTable.hh"
#અન્તર્ભાવ "../../../hb-ot-layout-common.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

સંરચના PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  અચલ SubTable& get_subtable (unsigned પૂર્ણાંક i) અચલ
  { પાછા Lookup::get_subtable<SubTable> (i); }

  તાર્કિક is_reverse () અચલ
  {
    પાછા ખોટું;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    return_trace (dispatch (c));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    hb_intersects_context_t c (glyphs);
    પાછા dispatch (&c);
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  { પાછા dispatch (c); }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) અચલ
  {
    જો (c->is_lookup_visited (this_index))
      પાછા hb_closure_lookups_context_t::default_return_કિંમત ();

    c->set_lookup_visited (this_index);
    જો (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      પાછા hb_closure_lookups_context_t::default_return_કિંમત ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    પાછા ret;
  }

  template <typename set_t>
  શૂન્ય collect_coverage (set_t *glyphs) અચલ
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  સ્થિર typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned પૂર્ણાંક lookup_index);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  { પાછા Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  { પાછા Lookup::subset<SubTable> (c); }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  { પાછા Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
