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

  स्थिरांक SubTable& get_subtable (अचिन्हित पूर्णांक i) स्थिरांक
  { परत Lookup::get_subtable<SubTable> (i); }

  तार्किक is_reverse () स्थिरांक
  {
    परत असत्य;
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

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  { परत dispatch (c); }

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

  ढाँचा <typename set_t>
  शून्य collect_coverage (set_t *glyphs) स्थिरांक
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  ढाँचा <typename context_t>
  स्थिर typename context_t::return_t dispatch_recurse_func (context_t *c, अचिन्हित पूर्णांक lookup_index);

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  { परत Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  { परत Lookup::subset<SubTable> (c); }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  { परत Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
