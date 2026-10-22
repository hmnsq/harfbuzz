#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#அந்தர்பாவ "PosLookupSubTable.hh"
#அந்தர்பாவ "../../../hb-ot-layout-common.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

கட்டமைப்பு PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  மாறிலி SubTable& get_subtable (unsigned முழு i) மாறிலி
  { திரும்பு Lookup::get_subtable<SubTable> (i); }

  தருக்க is_reverse () மாறிலி
  {
    திரும்பு பொய்;
  }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    return_trace (dispatch (c));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    hb_intersects_context_t c (glyphs);
    திரும்பு dispatch (&c);
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  { திரும்பு dispatch (c); }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) மாறிலி
  {
    எனில் (c->is_lookup_visited (this_index))
      திரும்பு hb_closure_lookups_context_t::default_return_மதிப்பு ();

    c->set_lookup_visited (this_index);
    எனில் (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      திரும்பு hb_closure_lookups_context_t::default_return_மதிப்பு ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    திரும்பு ret;
  }

  template <typename set_t>
  வெறுமை collect_coverage (set_t *glyphs) மாறிலி
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  நிலையான typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned முழு lookup_index);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  { திரும்பு Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  { திரும்பு Lookup::subset<SubTable> (c); }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  { திரும்பு Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
