#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#ಅನ್ತರ್ಭಾವ "PosLookupSubTable.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-layout-common.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಸಂರಚನಾ PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  ಧ್ರುವ SubTable& get_subtable (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ Lookup::get_subtable<SubTable> (i); }

  ತಾರ್ಕಿಕ is_reverse () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    return_trace (dispatch (c));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    hb_intersects_context_t c (glyphs);
    ಪ್ರತ್ಯಾಗಮ dispatch (&c);
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ dispatch (c); }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) ಧ್ರುವ
  {
    ಯದಿ (c->is_lookup_visited (this_index))
      ಪ್ರತ್ಯಾಗಮ hb_closure_lookups_context_t::default_return_ಮೌಲ್ಯ ();

    c->set_lookup_visited (this_index);
    ಯದಿ (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      ಪ್ರತ್ಯಾಗಮ hb_closure_lookups_context_t::default_return_ಮೌಲ್ಯ ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_coverage (set_t *glyphs) ಧ್ರುವ
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  ಸ್ಥಿರ typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ Lookup::subset<SubTable> (c); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
