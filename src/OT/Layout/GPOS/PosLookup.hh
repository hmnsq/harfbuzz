#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#ضمن "PosLookupSubTable.hh"
#ضمن "../../../hb-ot-layout-common.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  ثابت SubTable& get_subtable (unsigned صحيح i) ثابت
  { أرجع Lookup::get_subtable<SubTable> (i); }

  منطقي is_reverse () ثابت
  {
    أرجع خطأ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    return_trace (dispatch (c));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    hb_intersects_context_t c (glyphs);
    أرجع dispatch (&c);
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { أرجع dispatch (c); }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) ثابت
  {
    𐤀𐤌 (c->is_lookup_visited (this_index))
      أرجع hb_closure_lookups_context_t::default_return_value ();

    c->set_lookup_visited (this_index);
    𐤀𐤌 (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      أرجع hb_closure_lookups_context_t::default_return_value ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    أرجع ret;
  }

  template <typename set_t>
  فراغ collect_coverage (set_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  ساكن typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned صحيح lookup_index);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { أرجع Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع Lookup::subset<SubTable> (c); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
