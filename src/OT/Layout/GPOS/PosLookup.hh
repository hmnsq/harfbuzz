#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_POSLOOKUP_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_POSLOOKUP_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "PosLookupSubTable.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-layout-common.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਬਣਤਰ PosLookup : Lookup
{
  ਵਰਤੋਂ Sub_ਸਾਰਣੀ = Pos_Lookup_Sub_ਸਾਰਣੀ;

  ਅਚਲ Sub_ਸਾਰਣੀ& get_subtable (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ Lookup::get_subtable<Sub_ਸਾਰਣੀ> (i); }

  ਤਾਰਕਿਕ is_ਉਲਟ () ਅਚਲ
  {
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    return_trace (dispatch (c));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    hb_intersects_context_t c (glyphs);
    ਵਾਪਸ dispatch (&c);
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  { ਵਾਪਸ dispatch (c); }

  hb_ਘੇਰਾ_lookups_context_t::return_t ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਜੇ (c->is_lookup_ਦੇਖਿਆ (this_ਸੂਚਕਾਂਕ))
      ਵਾਪਸ hb_ਘੇਰਾ_lookups_context_t::default_return_ਮੁੱਲ ();

    c->ਸੈੱਟ_lookup_ਦੇਖਿਆ (this_ਸੂਚਕਾਂਕ);
    ਜੇ (!intersects (c->glyphs))
    {
      c->ਸੈੱਟ_lookup_inactive (this_ਸੂਚਕਾਂਕ);
      ਵਾਪਸ hb_ਘੇਰਾ_lookups_context_t::default_return_ਮੁੱਲ ();
    }

    hb_ਘੇਰਾ_lookups_context_t::return_t ret = dispatch (c);
    ਵਾਪਸ ret;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_coverage (ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    hb_collect_coverage_context_t<ਸੈੱਟ_t> c (glyphs);
    dispatch (&c);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t>
  ਸਥਿਰ ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch_recurse_func (context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ);

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  { ਵਾਪਸ Lookup::dispatch<Sub_ਸਾਰਣੀ> (c, std::forward<Ts> (ds)...); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  { ਵਾਪਸ Lookup::subset<Sub_ਸਾਰਣੀ> (c); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  { ਵਾਪਸ Lookup::sanitize<Sub_ਸਾਰਣੀ> (c); }
};

}
}
}

#ਜੇ_ਅੰਤ  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
