#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#شامل "PosLookupSubTable.hh"
#شامل "../../../hb-ot-layout-common.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

ساختار PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  ثابت SubTable& get_subtable (unsigned صحیح i) ثابت
  { بازگشت Lookup::get_subtable<SubTable> (i); }

  منطقی is_reverse () ثابت
  {
    بازگشت نادرست;
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

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { بازگشت dispatch (c); }

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

  template <typename set_t>
  تهی collect_coverage (set_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  ایستا typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned صحیح lookup_index);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { بازگشت Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  منطقی subset (hb_subset_context_t *c) ثابت
  { بازگشت Lookup::subset<SubTable> (c); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  { بازگشت Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
